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

#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include <sys/netmgr.h>

#include "transport_manager/usb/qnx/usb_iap2_connection.h"

namespace transport_manager {
namespace transport_adapter {

void OnDataReceived(UsbIAP2Connection* connection, RawMessageSptr message) {
  connection->OnDataReceived(message);
}

namespace {

class ReceiverThreadDelegate : public threads::ThreadDelegate {
 public:
  ReceiverThreadDelegate(iap2ea_hdl_t* iap2ea_hdl, UsbIAP2Connection* parent);
  virtual void threadMain();
  virtual bool exitThreadMain();

 private:
  enum {PULSE_CODE_EAP = _PULSE_CODE_MINAVAIL + 1};

  void receive();

  UsbIAP2Connection* parent_;
  bool run_;
  int chid_;
  int coid_;
  iap2ea_hdl_t* iap2ea_hdl_;
};

ReceiverThreadDelegate::ReceiverThreadDelegate(iap2ea_hdl_t* iap2ea_hdl,
  UsbIAP2Connection* parent) :
  parent_(parent), run_(true), iap2ea_hdl_(iap2ea_hdl) {

  chid_ = ChannelCreate(0);
  coid_ = ConnectAttach(ND_LOCAL_NODE, 0, chid_, _NTO_SIDE_CHANNEL, 0);
}

void ReceiverThreadDelegate::threadMain() {
  while (run_) {
    struct sigevent event;
    SIGEV_PULSE_INIT(&event, coid_, SIGEV_PULSE_PRIO_INHERIT, PULSE_CODE_EAP, 0);
    int arm_result = iap2_eap_event_arm(iap2ea_hdl_, &event);
    switch (arm_result) {
      case -1: // failure
        break;
      case 0: { // successfully armed
        struct _pulse pulse;
        int pulse_result = MsgReceivePulse(chid_, &pulse, sizeof(pulse), 0);
        if (0 == pulse_result) {
          switch (pulse.code) {
            case PULSE_CODE_EAP:
              receive();
              break;
          }
        }
        break;
      }
      case 1: // data is available
        receive();
        break;
    }
  }
}

bool ReceiverThreadDelegate::exitThreadMain() {
  run_ = false;
  ConnectDetach(coid_);
  ChannelDestroy(chid_); // unblocks MsgReceivePulse()
  return true;
}

void ReceiverThreadDelegate::receive() {
  const size_t buflen = 1024;
  uint8_t buf[buflen];
  int size = iap2_eap_recv(iap2ea_hdl_, buf, buflen);
  RawMessageSptr message(new protocol_handler::RawMessage(0, 0, buf, size));
  OnDataReceived(parent_, message); // cannot directly invoke parent's private method
}

}  // anonimous namespace

const char* UsbIAP2Connection::protocol = "com.qnx.eatest";  // TODO(nvaganov@luxoft.com) choose protocol name

UsbIAP2Connection::UsbIAP2Connection(const DeviceUID& device_uid,
  const ApplicationHandle& app_handle,
  TransportAdapterController* controller,
  const char* device_path) : device_uid_(device_uid),
  app_handle_(app_handle),
  controller_(controller),
  device_path_(device_path) {
}

bool UsbIAP2Connection::Init() {
  iap2_hdl_ = iap2_connect(device_path_.c_str(), 0);
  iap2ea_hdl_ = iap2_eap_open(device_path_.c_str(), protocol, 0);

  receiver_thread_ = new threads::Thread("USB iAP2 receiver",
    new ReceiverThreadDelegate(iap2ea_hdl_, this));
  receiver_thread_->start();

  return true;
}

TransportAdapter::Error UsbIAP2Connection::SendData(RawMessageSptr message) {
  iap2_eap_send(iap2ea_hdl_, message->data(), message->data_size());
  return TransportAdapter::OK;
}

TransportAdapter::Error UsbIAP2Connection::Disconnect() {
  receiver_thread_->stop();
  iap2_eap_close(iap2ea_hdl_);
  iap2_disconnect(iap2_hdl_);
  return TransportAdapter::OK;
}

void UsbIAP2Connection::OnDataReceived(RawMessageSptr message) {
  controller_->DataReceiveDone(device_uid_, app_handle_, message);
}

}  // namespace transport_adapter
}  // namespace transport_manager
