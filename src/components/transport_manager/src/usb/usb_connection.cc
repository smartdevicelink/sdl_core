/**
 * \file usb_connection.cc
 * \brief UsbConnection class source file.
 *
 * Copyright (c) 2013, Ford Motor Company
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

#include <libusb/libusb.h>

#include "transport_manager/usb/usb_connection.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

namespace transport_manager {
namespace transport_adapter {

UsbConnection::UsbConnection(const DeviceUID& device_uid,
                             const ApplicationHandle& app_handle,
                             TransportAdapterController* controller,
                             libusb_device* device)
    : device_uid_(device_uid),
      app_handle_(app_handle),
      controller_(controller),
      libusb_device_(device),
      device_handle_(0),
      in_endpoint_(0),
      in_endpoint_max_packet_size_(0),
      out_endpoint_(0),
      out_endpoint_max_packet_size_(0),
      in_buffer_(0),
      in_transfer_(0),
      out_transfer_(0),
      out_messages_(),
      current_out_message_(),
      out_messages_mutex_(),
      bytes_sent_(0),
      disconnecting_(false),
      waiting_in_transfer_cancel_(false),
      waiting_out_transfer_cancel_(false) {
  pthread_mutex_init(&out_messages_mutex_, 0);
}

UsbConnection::~UsbConnection() {
  if (device_handle_) {
    libusb_release_interface(device_handle_, 0);
    libusb_close(device_handle_);
  }
  if (in_buffer_) {
    delete[] in_buffer_;
  }
  if (in_transfer_) {
    libusb_free_transfer(in_transfer_);
  }
  pthread_mutex_destroy(&out_messages_mutex_);
}

void InTransferCallback(libusb_transfer *transfer) {
  static_cast<UsbConnection*>(transfer->user_data)->OnInTransfer(transfer);
}

void OutTransferCallback(libusb_transfer *transfer) {
  static_cast<UsbConnection*>(transfer->user_data)->OnOutTransfer(transfer);
}

bool UsbConnection::PostInTransfer() {
  libusb_fill_bulk_transfer(in_transfer_, device_handle_, in_endpoint_,
                            in_buffer_, in_endpoint_max_packet_size_,
                            InTransferCallback, this, 0);
  const int libusb_ret = libusb_submit_transfer(in_transfer_);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(
        logger_,
        "libusb_submit_transfer failed: " << libusb_error_name(libusb_ret));
    return false;
  }
  return true;
}

void UsbConnection::OnInTransfer(libusb_transfer *transfer) {
  if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
    RawMessageSptr data(
        new protocol_handler::RawMessage(0, 0, in_buffer_,
                                         transfer->actual_length));
    controller_->DataReceiveDone(device_uid_, app_handle_, data);
  } else {
    LOG4CXX_ERROR(logger_, "USB transfer failed: " << transfer->status);
    controller_->DataReceiveFailed(device_uid_, app_handle_,
                                   DataReceiveError());
  }
  if (disconnecting_) {
    if (waiting_in_transfer_cancel_) {
      waiting_in_transfer_cancel_ = false;
      CheckAllTransfersComplete();
    }
  } else {
    if (!PostInTransfer()) {
      controller_->ConnectionAborted(device_uid_, app_handle_,
                                     CommunicationError());
    }
  }
}

void UsbConnection::PopOutMessage() {
  bytes_sent_ = 0;
  if (out_messages_.empty()) {
    current_out_message_.reset();
  } else {
    current_out_message_ = out_messages_.front();
    out_messages_.pop_front();
    PostOutTransfer();
  }
}

bool UsbConnection::PostOutTransfer() {
  out_transfer_ = libusb_alloc_transfer(0);
  if (0 == out_transfer_) {
    LOG4CXX_ERROR(logger_, "libusb_alloc_transfer failed");
    return false;
  }
  libusb_fill_bulk_transfer(out_transfer_, device_handle_, out_endpoint_,
                            current_out_message_->data() + bytes_sent_,
                            current_out_message_->data_size() - bytes_sent_,
                            OutTransferCallback, this, 0);
  const int libusb_ret = libusb_submit_transfer(out_transfer_);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(
        logger_,
        "libusb_submit_transfer failed: " << libusb_error_name(libusb_ret));
    controller_->ConnectionAborted(device_uid_, app_handle_,
                                   CommunicationError());
    return false;
  }
  return true;
}

void UsbConnection::OnOutTransfer(libusb_transfer *transfer) {
  pthread_mutex_lock(&out_messages_mutex_);
  if (transfer->status == LIBUSB_TRANSFER_COMPLETED) {
    bytes_sent_ += transfer->actual_length;
    if (bytes_sent_ == current_out_message_->data_size()) {
      LOG4CXX_INFO(logger_, "USB out transfer, data sent: " << current_out_message_.get());
      controller_->DataSendDone(device_uid_, app_handle_, current_out_message_);
      PopOutMessage();
    }
  } else {
    LOG4CXX_ERROR(logger_, "USB transfer failed: " << transfer->status);
    controller_->DataSendFailed(device_uid_, app_handle_, current_out_message_,
                                DataSendError());
    PopOutMessage();
  }
  if (waiting_out_transfer_cancel_) {
    waiting_out_transfer_cancel_ = false;
    CheckAllTransfersComplete();
  }
  libusb_free_transfer(transfer);
  out_transfer_ = 0;
  pthread_mutex_unlock(&out_messages_mutex_);
}

TransportAdapter::Error UsbConnection::SendData(RawMessageSptr message) {
  if (disconnecting_) {
    return TransportAdapter::BAD_STATE;
  }
  pthread_mutex_lock(&out_messages_mutex_);
  if (current_out_message_.valid()) {
    out_messages_.push_back(message);
  } else {
    current_out_message_ = message;
    if (!PostOutTransfer()) {
      controller_->DataSendFailed(device_uid_, app_handle_, message,
                                  DataSendError());
    }
  }
  pthread_mutex_unlock(&out_messages_mutex_);
  return TransportAdapter::OK;
}

TransportAdapter::Error UsbConnection::Disconnect() {
  pthread_mutex_lock(&out_messages_mutex_);
  disconnecting_ = true;
  if (out_transfer_) {
    if (LIBUSB_SUCCESS == libusb_cancel_transfer(out_transfer_)) {
      waiting_out_transfer_cancel_ = true;
    }
  }
  if (in_transfer_) {
    if (LIBUSB_SUCCESS == libusb_cancel_transfer(in_transfer_)) {
      waiting_in_transfer_cancel_ = true;
    }
  }
  for (std::list<RawMessageSptr>::iterator it = out_messages_.begin();
      it != out_messages_.end(); it = out_messages_.erase(it)) {
    controller_->DataSendFailed(device_uid_, app_handle_, *it, DataSendError());
  }
  CheckAllTransfersComplete();
  pthread_mutex_unlock(&out_messages_mutex_);
  return TransportAdapter::OK;
}

bool UsbConnection::Init() {
  if (!FindEndpoints()) {
    return false;
  }

  int libusb_ret;
  libusb_ret = libusb_open(libusb_device_, &device_handle_);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(logger_,
                  "libusb_open failed: " << libusb_error_name(libusb_ret));
    return false;
  }

  libusb_ret = libusb_claim_interface(device_handle_, 0);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(
        logger_,
        "libusb_claim_interface failed: " << libusb_error_name(libusb_ret));
    return false;
  }

  in_buffer_ = new unsigned char[in_endpoint_max_packet_size_];
  if (0 == in_buffer_) {
    LOG4CXX_ERROR(
        logger_,
        "in buffer allocation failed (size " << in_endpoint_max_packet_size_ << ")");
    return false;
  }

  in_transfer_ = libusb_alloc_transfer(0);
  if (0 == in_transfer_) {
    LOG4CXX_ERROR(logger_, "libusb_alloc_transfer failed");
    return false;
  }

  if (!PostInTransfer()) {
    return false;
  }

  return true;
}

bool UsbConnection::FindEndpoints() {
  struct libusb_config_descriptor *config;
  const int libusb_ret = libusb_get_active_config_descriptor(libusb_device_,
                                                             &config);
  if (LIBUSB_SUCCESS != libusb_ret) {
    LOG4CXX_ERROR(
        logger_,
        "libusb_get_active_config_descriptor failed: " << libusb_error_name(libusb_ret));
    return false;
  }

  bool find_in_endpoint = true;
  bool find_out_endpoint = true;

  for (int i = 0; i < config->bNumInterfaces; ++i) {
    const libusb_interface& interface = config->interface[i];
    for (int i = 0; i < interface.num_altsetting; ++i) {
      const libusb_interface_descriptor& iface_desc = interface.altsetting[i];
      for (int i = 0; i < iface_desc.bNumEndpoints; ++i) {
        const libusb_endpoint_descriptor& endpoint_desc = iface_desc.endpoint[i];

        const uint8_t endpoint_dir = endpoint_desc.bEndpointAddress
            & LIBUSB_ENDPOINT_DIR_MASK;
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

  return !(find_in_endpoint || find_out_endpoint);
}

void UsbConnection::CheckAllTransfersComplete() {
  if (!(waiting_in_transfer_cancel_ || waiting_out_transfer_cancel_)) {
    controller_->DisconnectDone(device_uid_, app_handle_);
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager
