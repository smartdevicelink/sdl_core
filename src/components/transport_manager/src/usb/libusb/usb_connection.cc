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

#include <libusb-1.0/libusb.h>

#include <sstream>

#include "transport_manager/transport_adapter/transport_adapter_impl.h"
#include "transport_manager/usb/libusb/usb_connection.h"

#include "utils/logger.h"

// Define the buffer size, because the Android accessory protocol packet support
// packet buffers up to 16Kbytes
#define TRANSPORT_USB_BUFFER_MAX_SIZE (16 * 1024)

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOG_VARIABLE("TransportManager")

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
  SDL_LOG_TRACE("enter with this" << this);
  Finalise();
  libusb_free_transfer(in_transfer_);
  delete[] in_buffer_;
  SDL_LOG_TRACE("exit");
}

// Callback for handling income and outcome data from lib_usb
void InTransferCallback(libusb_transfer* transfer) {
  static_cast<UsbConnection*>(transfer->user_data)->OnInTransfer(transfer);
}

void OutTransferCallback(libusb_transfer* transfer) {
  static_cast<UsbConnection*>(transfer->user_data)->OnOutTransfer(transfer);
}

bool UsbConnection::PostInTransfer() {
  SDL_LOG_TRACE("enter");
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
    SDL_LOG_ERROR(
        "libusb_submit_transfer failed: " << libusb_error_name(libusb_ret));
    SDL_LOG_TRACE(
        "exit with FALSE. Condition: LIBUSB_SUCCESS != libusb_submit_transfer");
    return false;
  }
  SDL_LOG_TRACE("exit with TRUE");
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
  SDL_LOG_AUTO_TRACE();
  if (transfer->status == LIBUSB_TRANSFER_CANCELLED) {
    SDL_LOG_DEBUG("Free already canceled transfer.");
    libusb_free_transfer(transfer);
    return;
  }

  SDL_LOG_TRACE("enter with Libusb_transfer*: " << transfer);
  if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
    SDL_LOG_DEBUG("USB incoming transfer, size:"
                  << transfer->actual_length << ", data:"
                  << hex_data(transfer->buffer, transfer->actual_length));
    ::protocol_handler::RawMessagePtr data(new protocol_handler::RawMessage(
        0, 0, in_buffer_, transfer->actual_length, false));
    controller_->DataReceiveDone(device_uid_, app_handle_, data);
  } else {
    SDL_LOG_ERROR("USB incoming transfer failed: "
                  << libusb_error_name(transfer->status));
    controller_->DataReceiveFailed(
        device_uid_, app_handle_, DataReceiveError());
  }
  if (disconnecting_) {
    waiting_in_transfer_cancel_ = false;
  } else {
    if (!PostInTransfer()) {
      SDL_LOG_ERROR("USB incoming transfer failed with "
                    << "LIBUSB_TRANSFER_NO_DEVICE. Abort connection.");
      AbortConnection();
    }
  }
  SDL_LOG_TRACE("exit");
}

TransportAdapter::Error UsbConnection::PopOutMessage() {
  SDL_LOG_TRACE("enter");
  bytes_sent_ = 0;
  auto error_code = TransportAdapter::OK;
  if (out_messages_.empty()) {
    current_out_message_.reset();
  } else {
    current_out_message_ = out_messages_.front();
    out_messages_.pop_front();
    error_code = PostOutTransfer();
  }
  SDL_LOG_TRACE("exit");
  return error_code;
}

TransportAdapter::Error UsbConnection::PostOutTransfer() {
  SDL_LOG_TRACE("enter");
  out_transfer_ = libusb_alloc_transfer(0);
  if (nullptr == out_transfer_) {
    SDL_LOG_ERROR("libusb_alloc_transfer failed");
    SDL_LOG_TRACE(
        "exit with TransportAdapter::BAD_STATE. Condition: nullptr "
        "== out_transfer_");
    return TransportAdapter::BAD_STATE;
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
    SDL_LOG_ERROR(
        "libusb_submit_transfer failed: " << libusb_error_name(libusb_ret));
    SDL_LOG_TRACE("exit with TransportAdapter::FAIL. Condition: "
                  << "LIBUSB_SUCCESS != libusb_fill_bulk_transfer");
    return TransportAdapter::FAIL;
  }
  SDL_LOG_TRACE("exit with TransportAdapter::OK");
  return TransportAdapter::OK;
}

void UsbConnection::OnOutTransfer(libusb_transfer* transfer) {
  SDL_LOG_AUTO_TRACE();
  if (transfer->status == LIBUSB_TRANSFER_CANCELLED) {
    SDL_LOG_DEBUG("Free already canceled transfer.");
    libusb_free_transfer(transfer);
    return;
  }

  SDL_LOG_TRACE("enter with  Libusb_transfer*: " << transfer);
  auto error_code = TransportAdapter::OK;
  {
    sync_primitives::AutoLock locker(out_messages_mutex_);
    if (LIBUSB_TRANSFER_COMPLETED == transfer->status) {
      bytes_sent_ += transfer->actual_length;
      if (current_out_message_->data_size() == bytes_sent_) {
        SDL_LOG_DEBUG(
            "USB out transfer, data sent: " << current_out_message_.get());
        controller_->DataSendDone(
            device_uid_, app_handle_, current_out_message_);
        error_code = PopOutMessage();
      }
    } else {
      SDL_LOG_ERROR(
          "USB out transfer failed: " << libusb_error_name(transfer->status));
      controller_->DataSendFailed(
          device_uid_, app_handle_, current_out_message_, DataSendError());
      error_code = PopOutMessage();
    }
    if (current_out_message_.use_count() == 0) {
      libusb_free_transfer(transfer);
      out_transfer_ = nullptr;
      waiting_out_transfer_cancel_ = false;
    }
  }

  if (TransportAdapter::FAIL == error_code) {
    AbortConnection();
  }

  SDL_LOG_TRACE("exit");
}

TransportAdapter::Error UsbConnection::SendData(
    ::protocol_handler::RawMessagePtr message) {
  SDL_LOG_TRACE("enter with RawMessagePtr: " << message.get());
  if (disconnecting_) {
    SDL_LOG_TRACE("exit with TransportAdapter::BAD_STATE. Condition: "
                  << "disconnecting_");
    return TransportAdapter::BAD_STATE;
  }

  auto process_message = [this, &message]() {
    sync_primitives::AutoLock locker(out_messages_mutex_);
    if (current_out_message_.use_count() == 0) {
      current_out_message_ = message;
      return PostOutTransfer();
    }
    out_messages_.push_back(message);
    return TransportAdapter::OK;
  };

  auto error_code = process_message();

  if (TransportAdapter::OK == error_code) {
    SDL_LOG_TRACE("exit with TransportAdapter::OK.");
    return TransportAdapter::OK;
  }

  controller_->DataSendFailed(
      device_uid_, app_handle_, message, DataSendError());

  if (TransportAdapter::FAIL == error_code) {
    AbortConnection();
  }

  SDL_LOG_TRACE("exit with TransportAdapter::FAIL. PostOutTransfer сondition: "
                << error_code);
  return TransportAdapter::FAIL;
}

void UsbConnection::Finalise() {
  SDL_LOG_TRACE("enter");
  SDL_LOG_DEBUG("Finalise USB connection " << device_uid_);
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
  SDL_LOG_TRACE("exit");
}

void UsbConnection::AbortConnection() {
  SDL_LOG_TRACE("enter");
  Finalise();
  controller_->ConnectionAborted(
      device_uid_, app_handle_, CommunicationError());
  SDL_LOG_TRACE("exit");
}

TransportAdapter::Error UsbConnection::Disconnect() {
  Finalise();
  controller_->DisconnectDone(device_uid_, app_handle_);
  return TransportAdapter::OK;
}

bool UsbConnection::Init() {
  SDL_LOG_TRACE("enter");
  if (!FindEndpoints()) {
    SDL_LOG_ERROR("EndPoints was not found");
    SDL_LOG_TRACE("exit with FALSE. Condition: !FindEndpoints()");
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
    SDL_LOG_ERROR("libusb_alloc_transfer failed");
    SDL_LOG_TRACE("exit with FALSE. Condition: NULL == in_transfer_");
    return false;
  }

  controller_->ConnectDone(device_uid_, app_handle_);
  if (!PostInTransfer()) {
    SDL_LOG_ERROR("PostInTransfer failed. Call ConnectionAborted");
    controller_->ConnectionAborted(
        device_uid_, app_handle_, CommunicationError());
    SDL_LOG_TRACE("exit with FALSE. Condition: !PostInTransfer()");
    return false;
  }

  SDL_LOG_TRACE("exit with TRUE");
  return true;
}

bool UsbConnection::FindEndpoints() {
  SDL_LOG_TRACE("enter");
  struct libusb_config_descriptor* config;
  const int libusb_ret =
      libusb_get_active_config_descriptor(libusb_device_, &config);
  if (LIBUSB_SUCCESS != libusb_ret) {
    SDL_LOG_ERROR("libusb_get_active_config_descriptor failed: "
                  << libusb_error_name(libusb_ret));
    SDL_LOG_TRACE("exit with FALSE. Condition: LIBUSB_SUCCESS != libusb_ret");
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
  SDL_LOG_TRACE("exit with " << (result ? "TRUE" : "FALSE"));
  return result;
}
}  // namespace transport_adapter
}  // namespace transport_manager
