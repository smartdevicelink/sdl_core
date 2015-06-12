/*
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

#include <errno.h>
#include <sched.h>
#include <cstring>

#include "transport_manager/usb/qnx/usb_connection.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

UsbConnection::UsbConnection(const DeviceUID& device_uid,
                             const ApplicationHandle& app_handle,
                             TransportAdapterController* controller,
                             const UsbHandlerSptr libusb_handler,
                             PlatformUsbDevice* device)
    : device_uid_(device_uid),
      app_handle_(app_handle),
      controller_(controller),
      libusb_handler_(libusb_handler),
      usbd_device_(device->GetUsbdDevice()),
      in_pipe_(NULL),
      out_pipe_(NULL),
      in_buffer_(NULL),
      out_buffer_(NULL),
      in_urb_(NULL),
      out_urb_(NULL),
      out_messages_(),
      current_out_message_(),
      out_messages_mutex_(),
      bytes_sent_(0),
      disconnecting_(false),
      pending_in_transfer_(false),
      pending_out_transfer_(false) {
}

UsbConnection::~UsbConnection() {
  Finalise();
  if (in_urb_) usbd_free_urb(in_urb_);
  if (out_urb_) usbd_free_urb(out_urb_);

  if (in_pipe_) {
    const int close_pipe_rc = usbd_close_pipe(in_pipe_);
    if (EOK != close_pipe_rc) {
      LOG4CXX_ERROR(logger_, "Failed to close pipe: " << close_pipe_rc);
    }
  }
  if (out_pipe_) {
    const int close_pipe_rc = usbd_close_pipe(out_pipe_);
    if (EOK != close_pipe_rc) {
      LOG4CXX_ERROR(logger_, "Failed to close pipe: " << close_pipe_rc);
    }
  }
}

void InTransferCallback(usbd_urb* urb, usbd_pipe*, void* data) {
  static_cast<UsbConnection*>(data)->OnInTransfer(urb);
}

void OutTransferCallback(usbd_urb* urb, usbd_pipe*, void* data) {
  static_cast<UsbConnection*>(data)->OnOutTransfer(urb);
}

bool UsbConnection::PostInTransfer() {
  usbd_setup_bulk(in_urb_, URB_DIR_IN, in_buffer_, kInBufferSize);
  pending_in_transfer_ = true;
  const int io_rc =
      usbd_io(in_urb_, in_pipe_, InTransferCallback, this, USBD_TIME_INFINITY);
  if (EOK != io_rc) {
    pending_in_transfer_ = false;
    LOG4CXX_ERROR(logger_, "Failed to post in transfer: " << io_rc);
    return false;
  }
  return true;
}

void UsbConnection::OnInTransfer(usbd_urb* urb) {
  uint32_t status = 0;
  uint32_t len = 0;
  bool error = false;
  const int urb_status_rc = usbd_urb_status(urb, &status, &len);
  if (EOK != urb_status_rc && EIO != urb_status_rc) {  // EIO is OK
    LOG4CXX_ERROR(logger_, "Get in urb status failed: " << urb_status_rc);
    error = true;
  }
  LOG4CXX_INFO(logger_, "USB in transfer, status " << std::hex << status
                                                   << ", length " << std::dec
                                                   << len);

  if (!error) {
    switch (status) {
      case USBD_STATUS_CMP:
      case USBD_STATUS_CMP_ERR | USBD_STATUS_DATA_UNDERRUN:
        error = false;
        break;
      default:
        error = true;
        break;
    }
  }

  if (error) {
    LOG4CXX_ERROR(logger_, "USB in transfer failed");
    controller_->DataReceiveFailed(device_uid_, app_handle_,
                                   DataReceiveError());
  } else {
    ::protocol_handler::RawMessagePtr msg(new protocol_handler::RawMessage(0, 0, in_buffer_, len));
    controller_->DataReceiveDone(device_uid_, app_handle_, msg);
  }

  pending_in_transfer_ = false;
  if (!disconnecting_) {
    if (!PostInTransfer()) {
      controller_->ConnectionAborted(device_uid_, app_handle_,
                                     CommunicationError());
      Disconnect();
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
  }
}

bool UsbConnection::PostOutTransfer() {
  const int len = current_out_message_->data_size() - bytes_sent_;
  out_buffer_ = usbd_alloc(len);
  memmove(out_buffer_, current_out_message_->data() + bytes_sent_, len);
  usbd_setup_bulk(out_urb_, URB_DIR_OUT, out_buffer_, len);
  LOG4CXX_INFO(logger_, "out transfer :" << len);
  pending_out_transfer_ = true;
  const int io_rc = usbd_io(out_urb_, out_pipe_, OutTransferCallback, this,
                            USBD_TIME_INFINITY);
  if (EOK != io_rc) {
    pending_out_transfer_ = false;
    usbd_free(out_buffer_);
    LOG4CXX_ERROR(logger_, "Failed to post out transfer: " << io_rc);
    return false;
  }
  return true;
}

void UsbConnection::OnOutTransfer(usbd_urb* urb) {
  usbd_free(out_buffer_);
  uint32_t status = 0;
  uint32_t len = 0;
  bool error = false;
  const int urb_status_rc = usbd_urb_status(urb, &status, &len);
  if (EOK != urb_status_rc && EIO != urb_status_rc) {  // EIO is OK
    LOG4CXX_ERROR(logger_, "Get out urb status failed: " << urb_status_rc);
    error = true;
  }
  LOG4CXX_INFO(logger_, "USB out transfer, status " << std::hex << status
                                                    << ", length " << std::dec
                                                    << len);

  if (!error) {
    switch (status) {
      case USBD_STATUS_CMP:
      case USBD_STATUS_CMP_ERR | USBD_STATUS_DATA_UNDERRUN:
        error = false;
        break;
      default:
        error = true;
        break;
    }
  }

  sync_primitives::AutoLock locker(out_messages_mutex_);

  if (error) {
    LOG4CXX_ERROR(logger_, "USB out transfer failed");
    controller_->DataSendFailed(device_uid_, app_handle_, current_out_message_,
                                DataSendError());
    PopOutMessage();
  } else {
    bytes_sent_ += len;
    if (bytes_sent_ == current_out_message_->data_size()) {
      LOG4CXX_INFO(logger_, "USB out transfer, data sent: "
                                << current_out_message_.get());
      controller_->DataSendDone(device_uid_, app_handle_, current_out_message_);
      PopOutMessage();
    }
  }

  if ((!disconnecting_) && current_out_message_.valid()) {
    PostOutTransfer();
  } else {
    pending_out_transfer_ = false;
  }
}

TransportAdapter::Error UsbConnection::SendData(::protocol_handler::RawMessagePtr message) {
  if (disconnecting_) {
    return TransportAdapter::BAD_STATE;
  }
  sync_primitives::AutoLock locker(out_messages_mutex_);
  if (current_out_message_.valid()) {
    out_messages_.push_back(message);
  } else {
    current_out_message_ = message;
    if (!PostOutTransfer()) {
      controller_->DataSendFailed(device_uid_, app_handle_, message,
                                  DataSendError());
    }
  }
  return TransportAdapter::OK;
}

void UsbConnection::Finalise() {
  LOG4CXX_INFO(logger_, "Finalising");
  sync_primitives::AutoLock locker(out_messages_mutex_);
  disconnecting_ = true;
  usbd_abort_pipe(in_pipe_);
  usbd_abort_pipe(out_pipe_);
  for (std::list<protocol_handler::RawMessagePtr>::iterator it = out_messages_.begin();
       it != out_messages_.end(); it = out_messages_.erase(it)) {
    controller_->DataSendFailed(device_uid_, app_handle_, *it, DataSendError());
  }
  while (pending_in_transfer_ || pending_out_transfer_) sched_yield();
}

TransportAdapter::Error UsbConnection::Disconnect() {
  LOG4CXX_INFO(logger_, "Disconnecting");
  Finalise();
  controller_->DisconnectDone(device_uid_, app_handle_);
  return TransportAdapter::OK;
}

bool UsbConnection::Init() {
  if (!OpenEndpoints()) return false;

  in_urb_ = usbd_alloc_urb(NULL);
  out_urb_ = usbd_alloc_urb(NULL);
  if (NULL == in_urb_ || NULL == out_urb_) {
    LOG4CXX_ERROR(logger_, "usbd_alloc_urb failed");
    return false;
  }

  in_buffer_ = static_cast<unsigned char*>(usbd_alloc(kInBufferSize));
  if (NULL == in_buffer_) {
    LOG4CXX_ERROR(logger_, "usbd_alloc failed");
    return false;
  }

  controller_->ConnectDone(device_uid_, app_handle_);

  if (!PostInTransfer()) {
    controller_->ConnectionAborted(device_uid_, app_handle_,
                                   CommunicationError());
    return true;
  }

  return true;
}

bool UsbConnection::OpenEndpoints() {
  usbd_descriptors_t* in_endpoint_desc = NULL;
  usbd_descriptors_t* out_endpoint_desc = NULL;

  usbd_desc_node* device_desc_node = NULL;
  usbd_device_descriptor_t* device_desc =
      usbd_device_descriptor(usbd_device_, &device_desc_node);
  if (0 == device_desc) {
    LOG4CXX_ERROR(logger_, "Device descriptor not found");
    return false;
  }
  usbd_desc_node* cfg_desc_node = NULL;
  usbd_descriptors_t* config_desc = NULL;
  usbd_descriptors_t* iface_desc = NULL;
  int cfg = 0;
  bool found = false;
  while (!found) {
    config_desc =
        usbd_parse_descriptors(usbd_device_, device_desc_node,
                               USB_DESC_CONFIGURATION, cfg++, &cfg_desc_node);
    if (config_desc == NULL) {
      break;
    }
    LOG4CXX_INFO(logger_, "USB configuration "
                              << static_cast<int>(config_desc->configuration
                                                      .bConfigurationValue));
    int iface = 0;
    usbd_desc_node* iface_desc_node;
    while (!found) {
      iface_desc =
          usbd_parse_descriptors(usbd_device_, cfg_desc_node,
                                 USB_DESC_INTERFACE, iface++, &iface_desc_node);
      if (iface_desc == NULL) {
        break;
      }
#if ENABLE_LOG
      const uint8_t interface_number = iface_desc->interface.bInterfaceNumber;
#endif
      const uint8_t interface_subclass =
          iface_desc->interface.bInterfaceSubClass;
      LOG4CXX_INFO(logger_, "USB interface number "
                                << static_cast<int>(interface_number)
                                << ", subclass " << std::hex
                                << static_cast<int>(interface_subclass)
                                << std::dec);
      if (interface_subclass != 0xff) {
        continue;
      }
      int endpoint = 0;
      in_endpoint_desc = NULL;
      out_endpoint_desc = NULL;
      while (true) {
        usbd_descriptors_t* endpoint_desc = usbd_parse_descriptors(
            usbd_device_, iface_desc_node, USB_DESC_ENDPOINT, endpoint++, NULL);
        if (NULL == endpoint_desc) break;
        const uint8_t attributes = endpoint_desc->endpoint.bmAttributes;
        if ((attributes & 0x03) == USB_ATTRIB_BULK) {
          const uint8_t endpoint_address =
              endpoint_desc->endpoint.bEndpointAddress;
          LOG4CXX_INFO(logger_, "Endpoint with address "
                                    << std::hex
                                    << static_cast<int>(endpoint_address)
                                    << std::dec << " found");
          if (endpoint_address & USB_ENDPOINT_IN) {
            if (NULL == in_endpoint_desc) {
              in_endpoint_desc = endpoint_desc;
            }
          } else {
            if (NULL == out_endpoint_desc) {
              out_endpoint_desc = endpoint_desc;
            }
          }
        }
      }
      if (in_endpoint_desc != NULL && out_endpoint_desc != NULL) {
        found = true;
      }
    }
  }

  if (!found) return false;

  int open_pipe_rc = usbd_open_pipe(usbd_device_, in_endpoint_desc, &in_pipe_);
  if (EOK != open_pipe_rc) {
    LOG4CXX_ERROR(logger_, "Cannot open input pipe, error " << open_pipe_rc);
    return false;
  }

  open_pipe_rc = usbd_open_pipe(usbd_device_, out_endpoint_desc, &out_pipe_);
  if (EOK != open_pipe_rc) {
    LOG4CXX_ERROR(logger_, "Cannot open output pipe, error " << open_pipe_rc);
    return false;
  }

  return true;
}

}  // namespace transport_adapter
}  // namespace transport_manager
