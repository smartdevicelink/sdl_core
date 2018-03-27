/*
 * Copyright (c) 2013-2014, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <pthread.h>
#include <unistd.h>
#include <iomanip>

#include <libusb/libusb.h>

#include <sstream>

#include "transport_manager/usb/libusb/usb_connection.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

#include "utils/logger.h"

// Define the buffer size, because the Android accessory protocol packet support
// packet buffers up to 16Kbytes
#define TRANSPORT_USB_BUFFER_MAX_SIZE (16 * 1024)

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

UsbConnection::UsbConnection(const DeviceUID& device_uid,
                             const ApplicationHandle& app_handle,
                             TransportAdapterController* controller,
                             const UsbHandlerSptr usb_handler,
                             PlatformUsbDevice* device)
    : device_uid_(device_uid)
    , app_handle_(app_handle)
    , controller_(controller)
    , usb_handler_(usb_handler)
    , libusb_device_(device->GetLibusbDevice())
    , device_handle_(device->GetLibusbHandle())
    , in_endpoint_(0)
    , in_endpoint_max_packet_size_(0)
    , out_endpoint_(0)
    , out_endpoint_max_packet_size_(0)
    , in_buffer_(NULL)
    , in_buffer_size_(0)
    , in_transfer_(NULL)
    , out_transfer_(0)
    , out_messages_()
    , current_out_message_()
    , bytes_sent_(0)
    , disconnecting_(false)
    , waiting_in_transfer_cancel_(false)
    , waiting_out_transfer_cancel_(false) {}

UsbConnection::~UsbConnection() {
  LOG4CXX_TRACE(logger_, "enter with this" << this);
  Finalise();
  libusb_free_transfer(in_transfer_);
  delete[] in_buffer_;
  LOG4CXX_TRACE(logger_, "exit");
}

// Callback for handling income and outcome data from lib_usb
void InTransferCallback(libusb_transfer* transfer) {
  static_cast<UsbConnection*>(transfer->user_data)->OnInTransfer(transfer);
}

void OutTransferCallback(libusb_transfer* transfer) {
  static_cast<UsbConnection*>(transfer->user_data)->OnOutTransfer(transfer);
}

bool UsbConnection::PostInTransfer() {
  LOG4CXX_TRACE(logger_, "enter");
  libusb_fill_bulk_transfer(in_transfer_,
                            device_handle_,
                            in_endpoint_,
                            in_buffer_,
                            in_buffer_size_,
                            InTransferCallback,
                            this,
                            0);
  const int libusb_ret = libusb_submit_transfer(in_transfer_);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(
        logger_,
        "libusb_submit_transfer failed: " << libusb_error_name(libusb_ret));
    LOG4CXX_TRACE(
        logger_,
        "exit with FALSE. Condition: LIBUSB_SUCCESS != libusb_submit_transfer");
    return false;
  }
  LOG4CXX_TRACE(logger_, "exit with TRUE");
  return true;
}

std::string hex_data(const unsigned char* const buffer,
                     const int actual_length) {
  std::ostringstream hexdata;
  for (int i = 0; i < actual_length; ++i) {
    hexdata << " " << std::hex << std::setw(2) << std::setfill('0')
            << int(buffer[i]);
  }
  return hexdata.str();
}

void UsbConnection::OnInTransfer(libusb_transfer* transfer) {
  LOG4CXX_TRACE(logger_, "enter with Libusb_transfer*: " << transfer);
  if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
    LOG4CXX_DEBUG(logger_,
                  "USB incoming transfer, size:"
                      << transfer->actual_length << ", data:"
                      << hex_data(transfer->buffer, transfer->actual_length));
    ::protocol_handler::RawMessagePtr data(new protocol_handler::RawMessage(
        0, 0, in_buffer_, transfer->actual_length));
    controller_->DataReceiveDone(device_uid_, app_handle_, data);
  } else {
    LOG4CXX_ERROR(logger_,
                  "USB incoming transfer failed: "
                      << libusb_error_name(transfer->status));
    controller_->DataReceiveFailed(
        device_uid_, app_handle_, DataReceiveError());
  }
  if (disconnecting_) {
    waiting_in_transfer_cancel_ = false;
  } else {
    if (!PostInTransfer()) {
      LOG4CXX_ERROR(logger_,
                    "USB incoming transfer failed with "
                        << "LIBUSB_TRANSFER_NO_DEVICE. Abort connection.");
      AbortConnection();
    }
  }
  LOG4CXX_TRACE(logger_, "exit");
}

void UsbConnection::PopOutMessage() {
  LOG4CXX_TRACE(logger_, "enter");
  bytes_sent_ = 0;
  if (out_messages_.empty()) {
    current_out_message_.reset();
  } else {
    current_out_message_ = out_messages_.front();
    out_messages_.pop_front();
    PostOutTransfer();
  }
  LOG4CXX_TRACE(logger_, "exit");
}

bool UsbConnection::PostOutTransfer() {
  LOG4CXX_TRACE(logger_, "enter");
  out_transfer_ = libusb_alloc_transfer(0);
  if (0 == out_transfer_) {
    LOG4CXX_ERROR(logger_, "libusb_alloc_transfer failed");
    LOG4CXX_TRACE(logger_, "exit with FALSE. Condition: 0 == out_transfer_");
    return false;
  }
  libusb_fill_bulk_transfer(out_transfer_,
                            device_handle_,
                            out_endpoint_,
                            current_out_message_->data() + bytes_sent_,
                            current_out_message_->data_size() - bytes_sent_,
                            OutTransferCallback,
                            this,
                            0);
  const int libusb_ret = libusb_submit_transfer(out_transfer_);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(
        logger_,
        "libusb_submit_transfer failed: " << libusb_error_name(libusb_ret)
                                          << ". Abort connection.");
    AbortConnection();
    LOG4CXX_TRACE(logger_,
                  "exit with FALSE. Condition: "
                      << "LIBUSB_SUCCESS != libusb_fill_bulk_transfer");
    return false;
  }
  LOG4CXX_TRACE(logger_, "exit with TRUE");
  return true;
}

void UsbConnection::OnOutTransfer(libusb_transfer* transfer) {
  LOG4CXX_TRACE(logger_, "enter with  Libusb_transfer*: " << transfer);
  sync_primitives::AutoLock locker(out_messages_mutex_);
  if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
    bytes_sent_ += transfer->actual_length;
    if (bytes_sent_ == current_out_message_->data_size()) {
      LOG4CXX_DEBUG(
          logger_,
          "USB out transfer, data sent: " << current_out_message_.get());
      controller_->DataSendDone(device_uid_, app_handle_, current_out_message_);
      PopOutMessage();
    }
  } else {
    LOG4CXX_ERROR(
        logger_,
        "USB out transfer failed: " << libusb_error_name(transfer->status));
    controller_->DataSendFailed(
        device_uid_, app_handle_, current_out_message_, DataSendError());
    PopOutMessage();
  }
  if (!current_out_message_.valid()) {
    libusb_free_transfer(transfer);
    out_transfer_ = NULL;
    waiting_out_transfer_cancel_ = false;
  }
  LOG4CXX_TRACE(logger_, "exit");
}

TransportAdapter::Error UsbConnection::SendData(
    ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_TRACE(logger_, "enter with RawMessagePtr: " << message.get());
  if (disconnecting_) {
    LOG4CXX_TRACE(logger_,
                  "exit with TransportAdapter::BAD_STATE. Condition: "
                      << "disconnecting_");
    return TransportAdapter::BAD_STATE;
  }
  sync_primitives::AutoLock locker(out_messages_mutex_);
  if (current_out_message_.valid()) {
    out_messages_.push_back(message);
  } else {
    current_out_message_ = message;
    if (!PostOutTransfer()) {
      controller_->DataSendFailed(
          device_uid_, app_handle_, message, DataSendError());
      LOG4CXX_TRACE(
          logger_,
          "exit with TransportAdapter::FAIL. Condition: !PostOutTransfer()");
      return TransportAdapter::FAIL;
    }
  }
  LOG4CXX_TRACE(logger_, "exit with TransportAdapter::OK.");
  return TransportAdapter::OK;
}

void UsbConnection::Finalise() {
  LOG4CXX_TRACE(logger_, "enter");
  LOG4CXX_DEBUG(logger_, "Finalise USB connection " << device_uid_);
  {
    sync_primitives::AutoLock locker(out_messages_mutex_);
    disconnecting_ = true;
    if (out_transfer_) {
      waiting_out_transfer_cancel_ = true;
      if (LIBUSB_SUCCESS != libusb_cancel_transfer(out_transfer_)) {
        waiting_out_transfer_cancel_ = false;
      }
    }
    if (in_transfer_) {
      waiting_in_transfer_cancel_ = true;
      if (LIBUSB_SUCCESS != libusb_cancel_transfer(in_transfer_)) {
        waiting_in_transfer_cancel_ = false;
      }
    }
    for (std::list<protocol_handler::RawMessagePtr>::iterator it =
             out_messages_.begin();
         it != out_messages_.end();
         it = out_messages_.erase(it)) {
      controller_->DataSendFailed(
          device_uid_, app_handle_, *it, DataSendError());
    }
  }
  while (waiting_in_transfer_cancel_ || waiting_out_transfer_cancel_) {
    pthread_yield();
  }
  LOG4CXX_TRACE(logger_, "exit");
}

void UsbConnection::AbortConnection() {
  LOG4CXX_TRACE(logger_, "enter");
  controller_->ConnectionAborted(
      device_uid_, app_handle_, CommunicationError());
  Disconnect();
  LOG4CXX_TRACE(logger_, "exit");
}

TransportAdapter::Error UsbConnection::Disconnect() {
  Finalise();
  controller_->DisconnectDone(device_uid_, app_handle_);
  return TransportAdapter::OK;
}

bool UsbConnection::Init() {
  LOG4CXX_TRACE(logger_, "enter");
  if (!FindEndpoints()) {
    LOG4CXX_ERROR(logger_, "EndPoints was not found");
    LOG4CXX_TRACE(logger_, "exit with FALSE. Condition: !FindEndpoints()");
    return false;
  }

  if (in_endpoint_max_packet_size_ < TRANSPORT_USB_BUFFER_MAX_SIZE) {
    in_buffer_size_ = TRANSPORT_USB_BUFFER_MAX_SIZE;
  } else {
    in_buffer_size_ = in_endpoint_max_packet_size_;
  }

  in_buffer_ = new unsigned char[in_buffer_size_];
  in_transfer_ = libusb_alloc_transfer(0);
  if (NULL == in_transfer_) {
    LOG4CXX_ERROR(logger_, "libusb_alloc_transfer failed");
    LOG4CXX_TRACE(logger_, "exit with FALSE. Condition: NULL == in_transfer_");
    return false;
  }

  controller_->ConnectDone(device_uid_, app_handle_);
  if (!PostInTransfer()) {
    LOG4CXX_ERROR(logger_, "PostInTransfer failed. Call ConnectionAborted");
    controller_->ConnectionAborted(
        device_uid_, app_handle_, CommunicationError());
    LOG4CXX_TRACE(logger_, "exit with FALSE. Condition: !PostInTransfer()");
    return false;
  }

  LOG4CXX_TRACE(logger_, "exit with TRUE");
  return true;
}

bool UsbConnection::FindEndpoints() {
  LOG4CXX_TRACE(logger_, "enter");
  struct libusb_config_descriptor* config;
  const int libusb_ret =
      libusb_get_active_config_descriptor(libusb_device_, &config);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(logger_,
                  "libusb_get_active_config_descriptor failed: "
                      << libusb_error_name(libusb_ret));
    LOG4CXX_TRACE(logger_,
                  "exit with FALSE. Condition: LIBUSB_SUCCESS != libusb_ret");
    return false;
  }

  bool find_in_endpoint = true;
  bool find_out_endpoint = true;

  for (int i = 0; i < config->bNumInterfaces; ++i) {
    const libusb_interface& interface = config->interface[i];
    for (int i = 0; i < interface.num_altsetting; ++i) {
      const libusb_interface_descriptor& iface_desc = interface.altsetting[i];
      for (int i = 0; i < iface_desc.bNumEndpoints; ++i) {
        const libusb_endpoint_descriptor& endpoint_desc =
            iface_desc.endpoint[i];

        const uint8_t endpoint_dir =
            endpoint_desc.bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK;
        if (find_in_endpoint && endpoint_dir == LIBUSB_ENDPOINT_IN) {
          in_endpoint_ = endpoint_desc.bEndpointAddress;
          in_endpoint_max_packet_size_ = endpoint_desc.wMaxPacketSize;
          find_in_endpoint = false;
        } else if (find_out_endpoint && endpoint_dir == LIBUSB_ENDPOINT_OUT) {
          out_endpoint_ = endpoint_desc.bEndpointAddress;
          out_endpoint_max_packet_size_ = endpoint_desc.wMaxPacketSize;
          find_out_endpoint = false;
        }
      }
    }
  }
  libusb_free_config_descriptor(config);

  const bool result = !(find_in_endpoint || find_out_endpoint);
  LOG4CXX_TRACE(logger_, "exit with " << (result ? "TRUE" : "FALSE"));
  return result;
}
}  // namespace transport_adapter
}  // namespace transport_manager
