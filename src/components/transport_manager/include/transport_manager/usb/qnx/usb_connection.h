/*
 * \file usb_connection.h
 * \brief UsbConnection class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_QNX_USB_CONNECTION_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_QNX_USB_CONNECTION_H_

#include "utils/lock.h"

#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/transport_adapter/connection.h"
#include "transport_manager/usb/common.h"

namespace transport_manager {
namespace transport_adapter {

class UsbConnection : public Connection {
  static const uint32_t kInBufferSize = 2048;
 public:
  UsbConnection(const DeviceUID& device_uid,
                const ApplicationHandle& app_handle,
                TransportAdapterController* controller,
                const UsbHandlerSptr libusb_handler,
                PlatformUsbDevice* device);

  bool Init();

  virtual ~UsbConnection();
 protected:
  virtual TransportAdapter::Error SendData(::protocol_handler::RawMessagePtr message);
  virtual TransportAdapter::Error Disconnect();
 private:
  friend void InTransferCallback(usbd_urb* urb, usbd_pipe*, void*);
  friend void OutTransferCallback(usbd_urb* urb, usbd_pipe*, void*);
  bool OpenEndpoints();
  void PopOutMessage();
  bool PostInTransfer();
  bool PostOutTransfer();
  void OnInTransfer(usbd_urb* urb);
  void OnOutTransfer(usbd_urb* urb);
  void Finalise();

  const DeviceUID device_uid_;
  const ApplicationHandle app_handle_;
  TransportAdapterController* controller_;
  UsbHandlerSptr libusb_handler_;
  usbd_device* usbd_device_;

  usbd_pipe* in_pipe_;
  usbd_pipe* out_pipe_;

  unsigned char* in_buffer_;
  void* out_buffer_;

  usbd_urb* in_urb_;
  usbd_urb* out_urb_;

  std::list<protocol_handler::RawMessagePtr> out_messages_;
  ::protocol_handler::RawMessagePtr current_out_message_;
  sync_primitives::Lock out_messages_mutex_;
  size_t bytes_sent_;
  bool disconnecting_;
  bool pending_in_transfer_;
  bool pending_out_transfer_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_QNX_USB_CONNECTION_H_
