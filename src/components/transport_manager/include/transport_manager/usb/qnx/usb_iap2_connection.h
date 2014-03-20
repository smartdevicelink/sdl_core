/*
 * Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_QNX_USB_IAP2_CONNECTION_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_QNX_USB_IAP2_CONNECTION_H_

#include <iap2/iap2.h>

#include "utils/threads/thread.h"

#include "transport_manager/transport_adapter/connection.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "pulse_thread_delegate.h"

namespace transport_manager {
namespace transport_adapter {

class UsbIAP2Connection : public Connection {
 public:
  UsbIAP2Connection(const DeviceUID& device_uid,
    const ApplicationHandle& app_handle,
    TransportAdapterController* controller,
    const char* device_path);

  bool Init();

 protected:
  virtual TransportAdapter::Error SendData(RawMessageSptr message);
  virtual TransportAdapter::Error Disconnect();

 private:
  void OnDataReceived(RawMessageSptr message);
  void OnReceiveFailed();

  DeviceUID device_uid_;
  ApplicationHandle app_handle_;
  TransportAdapterController* controller_;
  std::string device_path_;

  iap2_hdl_t* iap2_hdl_;
  iap2ea_hdl_t* iap2ea_hdl_;

  utils::SharedPtr<threads::Thread> receiver_thread_;

  static const char* protocol;

  class ReceiverThreadDelegate : public PulseThreadDelegate {
   public:
    ReceiverThreadDelegate(iap2ea_hdl_t* iap2ea_hdl, UsbIAP2Connection* parent);
    virtual bool ArmEvent(struct sigevent* event);
    virtual void OnPulse();

   private:
    static const size_t kBufferSize = 1024;

    void receive();

    UsbIAP2Connection* parent_;
    iap2ea_hdl_t* iap2ea_hdl_;
    uint8_t buffer_[kBufferSize];
  };
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  //  SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_USB_QNX_USB_IAP2_CONNECTION_H_
