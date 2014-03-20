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

#include <sys/siginfo.h>
#include <sys/netmgr.h>

#include "transport_manager/usb/qnx/usb_iap2_connection.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

namespace transport_manager {
namespace transport_adapter {

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
  LOG4CXX_TRACE(logger_, "Connecting to " << device_path_);
  iap2_hdl_ = iap2_connect(device_path_.c_str(), 0);
  if (iap2_hdl_ != 0) {
    LOG4CXX_DEBUG(logger_, "Connected to " << device_path_);
  }
  else {
    LOG4CXX_ERROR(logger_, "Could not connect to " << device_path_);
    return false;
  }

  LOG4CXX_TRACE(logger_, "Opening protocol " << protocol);
  iap2ea_hdl_ = iap2_eap_open(device_path_.c_str(), protocol, 0);
  if (iap2ea_hdl_ != 0) {
    LOG4CXX_DEBUG(logger_, "Protocol " << protocol << " opened");
  }
  else {
    LOG4CXX_ERROR(logger_, "Could not open protocol " << protocol);
    return false;
  }

  receiver_thread_ = new threads::Thread("USB iAP2 receiver",
    new ReceiverThreadDelegate(iap2ea_hdl_, this));
  receiver_thread_->start();

  controller_->ConnectDone(device_uid_, app_handle_);
  return true;
}

TransportAdapter::Error UsbIAP2Connection::SendData(RawMessageSptr message) {
  LOG4CXX_TRACE(logger_, "USB iAP2: sending data");
  if (iap2_eap_send(iap2ea_hdl_, message->data(), message->data_size()) != -1) {
    LOG4CXX_INFO(logger_, "USB iAP2: data sent successfully");
    controller_->DataSendDone(device_uid_, app_handle_, message);
    return TransportAdapter::OK;
  }
  else {
    LOG4CXX_WARN(logger_, "USB iAP2: error occured while sending data");
    controller_->DataSendFailed(device_uid_, app_handle_, message, DataSendError());
    return TransportAdapter::FAIL;
  }
}

TransportAdapter::Error UsbIAP2Connection::Disconnect() {
  TransportAdapter::Error error = TransportAdapter::OK;

  receiver_thread_->stop();

  LOG4CXX_TRACE(logger_, "Closing protocol " << protocol);
  if (0 == iap2_eap_close(iap2ea_hdl_)) {
    LOG4CXX_DEBUG(logger_, "Protocol " << protocol << " closed");
  }
  else {
    LOG4CXX_WARN(logger_, "Could not close protocol " << protocol);
    error = TransportAdapter::FAIL;
  }

  LOG4CXX_TRACE(logger_, "Disconnecting from " << device_path_);
  if (0 == iap2_disconnect(iap2_hdl_)) {
    LOG4CXX_DEBUG(logger_, "Disconnected from " << device_path_);
  }
  else {
    LOG4CXX_WARN(logger_, "Could not disconnect from " << device_path_);
    error = TransportAdapter::FAIL;
  }

  controller_->DisconnectDone(device_uid_, app_handle_);
  return error;
}

void UsbIAP2Connection::OnDataReceived(RawMessageSptr message) {
  controller_->DataReceiveDone(device_uid_, app_handle_, message);
}

void UsbIAP2Connection::OnReceiveFailed() {
  controller_->DataReceiveFailed(device_uid_, app_handle_, DataReceiveError());
}

UsbIAP2Connection::ReceiverThreadDelegate::ReceiverThreadDelegate(iap2ea_hdl_t* iap2ea_hdl,
  UsbIAP2Connection* parent) :
  parent_(parent), run_(false), iap2ea_hdl_(iap2ea_hdl) {

  LOG4CXX_TRACE(logger_, "Creating QNX channel");
  chid_ = ChannelCreate(0);
  if (chid_ != -1) {
    LOG4CXX_DEBUG(logger_, "Created QNX channel " << chid_);
  }
  else {
    LOG4CXX_ERROR(logger_, "Failed to create QNX channel");
    return;
  }

  LOG4CXX_TRACE(logger_, "Connecting to QNX channel " << chid_);
  coid_ = ConnectAttach(ND_LOCAL_NODE, 0, chid_, _NTO_SIDE_CHANNEL, 0);
  if (coid_ != -1) {
    LOG4CXX_DEBUG(logger_, "Connected to QNX channel " << chid_);
  }
  else {
    LOG4CXX_ERROR(logger_, "Failed to connect to QNX channel " << chid_);
    return;
  }

  run_ = true;
}

void UsbIAP2Connection::ReceiverThreadDelegate::threadMain() {
  while (run_) {
    struct sigevent event;
    SIGEV_PULSE_INIT(&event, coid_, SIGEV_PULSE_PRIO_INHERIT, PULSE_CODE_EAP, 0);
    LOG4CXX_TRACE(logger_, "Arming for USB iAP2 input notification");
    int arm_result = iap2_eap_event_arm(iap2ea_hdl_, &event);
    switch (arm_result) {
      case -1: // failure
        LOG4CXX_WARN(logger_, "Could not arm for USB iAP2 input notification");
        break;
      case 0: { // successfully armed
        LOG4CXX_DEBUG(logger_, "Successfully armed for USB iAP2 input notification");
        struct _pulse pulse;
        LOG4CXX_INFO(logger_, "USB iAP2: waiting for pulse on QNX channel " << chid_);
        int pulse_result = MsgReceivePulse(chid_, &pulse, sizeof(pulse), 0);
        if (0 == pulse_result) {
          LOG4CXX_INFO(logger_, "USB iAP2: received pulse on QNX channel " << chid_);
          switch (pulse.code) {
            case PULSE_CODE_EAP:
              receive();
              break;
          }
        }
        else {
          LOG4CXX_WARN(logger_, "USB iAP2: error occured while waiting for pulse on QNX channel " << chid_);
        }
        break;
      }
      case 1: // data is available
        LOG4CXX_DEBUG(logger_, "USB iAP2: data is already available");
        receive();
        break;
    }
  }
}

bool UsbIAP2Connection::ReceiverThreadDelegate::exitThreadMain() {
  run_ = false;

  LOG4CXX_TRACE(logger_, "Disconnecting from QNX channel" << chid_);
  if (ConnectDetach(coid_) != -1) {
    LOG4CXX_DEBUG(logger_, "Disconnected from QNX channel " << chid_);
  }
  else {
    LOG4CXX_WARN(logger_, "Failed to disconnect from QNX channel " << chid_);
  }

  LOG4CXX_TRACE(logger_, "Destroying QNX channel " << chid_);
  if (ChannelDestroy(chid_) != -1) { // unblocks MsgReceivePulse()
    LOG4CXX_DEBUG(logger_, "QNX channel " << chid_ << " destroyed");
  }
  else {
    LOG4CXX_WARN(logger_, "Failed to destroy QNX channel " << chid_);
  }

  return true;
}

void UsbIAP2Connection::ReceiverThreadDelegate::receive() {
// this method can be invoked from the only thread
// thus it does not need any synchronization
  LOG4CXX_TRACE(logger_, "USB iAP2: receiving data");
  int size = iap2_eap_recv(iap2ea_hdl_, buffer_, kBufferSize);
  if (size != -1) {
    LOG4CXX_INFO(logger_, "USB iAP2: received " << size << " bytes");
    RawMessageSptr message(new protocol_handler::RawMessage(0, 0, buffer_, size));
    parent_->OnDataReceived(message);
  }
  else {
    LOG4CXX_WARN(logger_, "USB iAP2: error occured while receiving data");
    parent_->OnReceiveFailed();
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager
