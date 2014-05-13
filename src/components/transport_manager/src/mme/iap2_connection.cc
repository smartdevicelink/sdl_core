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

#include "utils/logger.h"

#include "transport_manager/mme/iap2_connection.h"
#include "transport_manager/mme/iap2_device.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

IAP2Connection::IAP2Connection(const DeviceUID& device_uid,
  const ApplicationHandle& app_handle,
  TransportAdapterController* controller,
  IAP2Device* parent) : device_uid_(device_uid),
  app_handle_(app_handle),
  controller_(controller),
  parent_(parent) {
}

bool IAP2Connection::Init() {
  iap2ea_hdl_ = parent_->HandlerByAppId(app_handle_);
  if (iap2ea_hdl_ != 0) {
    receiver_thread_ = new threads::Thread("iAP2 receiver",
      new ReceiverThreadDelegate(iap2ea_hdl_, this));
    receiver_thread_->start();

    controller_->ConnectDone(device_uid_, app_handle_);
    return true;
  }
  else {
    return false;
  }
}

TransportAdapter::Error IAP2Connection::SendData(RawMessageSptr message) {
  LOG4CXX_TRACE(logger_, "iAP2: sending data");
  if (iap2_eap_send(iap2ea_hdl_, message->data(), message->data_size()) != -1) {
    LOG4CXX_INFO(logger_, "iAP2: data sent successfully");
    controller_->DataSendDone(device_uid_, app_handle_, message);
    return TransportAdapter::OK;
  }
  else {
    LOG4CXX_WARN(logger_, "iAP2: error occured while sending data");
    controller_->DataSendFailed(device_uid_, app_handle_, message, DataSendError());
    return TransportAdapter::FAIL;
  }
}

TransportAdapter::Error IAP2Connection::Disconnect() {
  TransportAdapter::Error error = TransportAdapter::OK;

  receiver_thread_->stop();

  LOG4CXX_TRACE(logger_, "iAP2: closing connection");
  if (iap2_eap_close(iap2ea_hdl_) != -1) {
    LOG4CXX_DEBUG(logger_, "iAP2: connection closed");
  }
  else {
    LOG4CXX_WARN(logger_, "iAP2: could not close connection");
    error = TransportAdapter::FAIL;
  }
  parent_->OnDisconnect(app_handle_);
  controller_->DisconnectDone(device_uid_, app_handle_);
  return error;
}

void IAP2Connection::ReceiveData() {
  LOG4CXX_TRACE(logger_, "iAP2: receiving data");
  int size = iap2_eap_recv(iap2ea_hdl_, buffer_, kBufferSize);
  if (size != -1) {
    LOG4CXX_INFO(logger_, "iAP2: received " << size << " bytes");
    RawMessageSptr message(new protocol_handler::RawMessage(0, 0, buffer_, size));
    controller_->DataReceiveDone(device_uid_, app_handle_, message);
  }
  else {
    LOG4CXX_WARN(logger_, "iAP2: error occured while receiving data");
    controller_->DataReceiveFailed(device_uid_, app_handle_, DataReceiveError());
  }
}

IAP2Connection::ReceiverThreadDelegate::ReceiverThreadDelegate(iap2ea_hdl_t* iap2ea_hdl,
  IAP2Connection* parent) :
  parent_(parent), iap2ea_hdl_(iap2ea_hdl) {
}

bool IAP2Connection::ReceiverThreadDelegate::ArmEvent(struct sigevent* event) {
  LOG4CXX_TRACE(logger_, "Arming for iAP2 input notification");
  int arm_result = iap2_eap_event_arm(iap2ea_hdl_, event);
  switch (arm_result) {
    case -1: // failure
      LOG4CXX_WARN(logger_, "Could not arm for iAP2 input notification");
      return false;
    case 0: // successfully armed
      LOG4CXX_DEBUG(logger_, "Successfully armed for iAP2 input notification");
      return true;
    case 1: // data is available
      LOG4CXX_DEBUG(logger_, "iAP2: data is already available");
      parent_->ReceiveData();
      return false; // don't need to wait for Pulse in this case
  }
  return false;
}

void IAP2Connection::ReceiverThreadDelegate::OnPulse() {
  parent_->ReceiveData();
}

}  // namespace transport_adapter
}  // namespace transport_manager
