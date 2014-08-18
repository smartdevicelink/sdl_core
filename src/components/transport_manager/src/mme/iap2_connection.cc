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

#include <errno.h>

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
  parent_(parent),
  unexpected_disconnect_(false) {
}

IAP2Connection::~IAP2Connection() {
  receiver_thread_->stop();
}

bool IAP2Connection::Init() {
  IAP2Device::AppRecord record;
  if (parent_->RecordByAppId(app_handle_, record)) {
    protocol_name_ = record.first;
    iap2ea_hdl_ = record.second;
    std::string thread_name = "iAP2 receiver (" + protocol_name_ + ")";
    receiver_thread_delegate_ = new ReceiverThreadDelegate(iap2ea_hdl_, this);
    receiver_thread_ = new threads::Thread(thread_name.c_str(), receiver_thread_delegate_);
    receiver_thread_->start();

    controller_->ConnectDone(device_uid_, app_handle_);
    return true;
  }
  return false;
}

void IAP2Connection::Finalize() {
  if (unexpected_disconnect_) {
    controller_->ConnectionAborted(device_uid_, app_handle_, CommunicationError());
  }
}

TransportAdapter::Error IAP2Connection::SendData(RawMessagePtr message) {
  LOG4CXX_TRACE(logger_, "iAP2: sending data on protocol " << protocol_name_);
  if (iap2_eap_send(iap2ea_hdl_, message->data(), message->data_size()) != -1) {
    LOG4CXX_INFO(logger_, "iAP2: data on protocol " << protocol_name_ << " sent successfully");
    controller_->DataSendDone(device_uid_, app_handle_, message);
    return TransportAdapter::OK;
  }
  else {
    LOG4CXX_WARN(logger_, "iAP2: error occurred while sending data on protocol " << protocol_name_);
    controller_->DataSendFailed(device_uid_, app_handle_, message, DataSendError());
    return TransportAdapter::FAIL;
  }
}

TransportAdapter::Error IAP2Connection::Disconnect() {
  receiver_thread_->stop();
  TransportAdapter::Error error = Close() ? TransportAdapter::OK : TransportAdapter::FAIL;
  controller_->DisconnectDone(device_uid_, app_handle_);
  return error;
}

void IAP2Connection::ReceiveData() {
  LOG4CXX_TRACE(logger_, "iAP2: receiving data on protocol " << protocol_name_);
  int size = iap2_eap_recv(iap2ea_hdl_, buffer_, kBufferSize);
  if (size != -1) {
    LOG4CXX_INFO(logger_, "iAP2: received " << size << " bytes on protocol " << protocol_name_);
    if (size != 0) {
      RawMessagePtr message(new protocol_handler::RawMessage(0, 0, buffer_, size));
      controller_->DataReceiveDone(device_uid_, app_handle_, message);
    }
  }
  else {
    switch (errno) {
      case ECONNRESET:
        LOG4CXX_INFO(logger_, "iAP2: protocol " << protocol_name_ << " disconnected");
        unexpected_disconnect_ = true;
// receiver_thread_->stop() cannot be invoked here
// because this method is called from receiver_thread_
// anyway delegate can be stopped directly
        receiver_thread_delegate_->exitThreadMain();
        Close();
        break;
      default:
        LOG4CXX_WARN(logger_, "iAP2: error occurred while receiving data on protocol " << protocol_name_);
        controller_->DataReceiveFailed(device_uid_, app_handle_, DataReceiveError());
        break;
    }
  }
}

bool IAP2Connection::Close() {
  bool result;

  LOG4CXX_TRACE(logger_, "iAP2: closing connection on protocol " << protocol_name_);
  if (iap2_eap_close(iap2ea_hdl_) != -1) {
    LOG4CXX_DEBUG(logger_, "iAP2: connection on protocol " << protocol_name_ << " closed");
    result = true;
  }
  else {
    LOG4CXX_WARN(logger_, "iAP2: could not close connection on protocol " << protocol_name_);
    result = false;
  }

  parent_->OnDisconnect(app_handle_);
  return result;
}

IAP2Connection::ReceiverThreadDelegate::ReceiverThreadDelegate(iap2ea_hdl_t* iap2ea_hdl,
  IAP2Connection* parent) :
  parent_(parent), iap2ea_hdl_(iap2ea_hdl) {
}

bool IAP2Connection::ReceiverThreadDelegate::ArmEvent(struct sigevent* event) {
  LOG4CXX_TRACE(logger_, "iAP2: arming for input notification");
  int arm_result = iap2_eap_event_arm(iap2ea_hdl_, event);
  switch (arm_result) {
    case -1: // failure
      LOG4CXX_WARN(logger_, "iAP2: could not arm for input notification");
      return false;
    case 0: // successfully armed
      LOG4CXX_DEBUG(logger_, "iAP2: successfully armed for input notification");
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

void IAP2Connection::ReceiverThreadDelegate::Finalize() {
  parent_->Finalize();
}

}  // namespace transport_adapter
}  // namespace transport_manager
