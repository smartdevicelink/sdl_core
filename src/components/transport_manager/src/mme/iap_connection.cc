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

#include "transport_manager/mme/iap_connection.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

IAPConnection::IAPConnection(const DeviceUID& device_uid,
  const ApplicationHandle& app_handle,
  TransportAdapterController* controller,
  const std::string& device_path) : device_uid_(device_uid),
  app_handle_(app_handle),
  controller_(controller),
  device_path_(device_path) {
}

bool IAPConnection::Init() {
  LOG4CXX_TRACE(logger_, "iAP: connecting to " << device_path_);
  ipod_hdl_ = ipod_connect(device_path_.c_str(), 0);
  if (ipod_hdl_ != 0) {
    LOG4CXX_DEBUG(logger_, "iAP: connected to " << device_path_);
  }
  else {
    LOG4CXX_ERROR(logger_, "iAP: could not connect to " << device_path_);
    return false;
  }

  receiver_thread_ = new threads::Thread("iAP receiver",
    new ReceiverThreadDelegate(ipod_hdl_, this));
  receiver_thread_->start();

  return true;
}

TransportAdapter::Error IAPConnection::SendData(RawMessageSptr message) {
  if (session_ids_.empty()) {
    LOG4CXX_WARN(logger_, "iAP: no opened sessions");
    return TransportAdapter::BAD_STATE;
  }
// How is session for sending data chosen?
  int session_id = *session_ids_.begin();
  LOG4CXX_TRACE(logger_, "iAP: sending data");
  if (ipod_eaf_send(ipod_hdl_, session_id, message->data(), message->data_size()) != -1) {
    LOG4CXX_INFO(logger_, "iAP: data sent successfully");
    controller_->DataSendDone(device_uid_, app_handle_, message);
    return TransportAdapter::OK;
  }
  else {
    LOG4CXX_WARN(logger_, "iAP: error occured while sending data");
    controller_->DataSendFailed(device_uid_, app_handle_, message, DataSendError());
    return TransportAdapter::FAIL;
  }
}

TransportAdapter::Error IAPConnection::Disconnect() {
  TransportAdapter::Error error;

  receiver_thread_->stop();

  LOG4CXX_TRACE(logger_, "iAP: disconecting from " << device_path_);
  if (ipod_disconnect(ipod_hdl_) != -1) {
    LOG4CXX_DEBUG(logger_, "iAP: disconnected from " << device_path_);
    error = TransportAdapter::OK;
  }
  else {
    LOG4CXX_WARN(logger_, "iAP: could not disconnect from " << device_path_);
    error = TransportAdapter::FAIL;
  }

  controller_->DisconnectDone(device_uid_, app_handle_);
  return error;
}

void IAPConnection::OnDataReceived(RawMessageSptr message) {
  controller_->DataReceiveDone(device_uid_, app_handle_, message);
}

void IAPConnection::OnReceiveFailed() {
  controller_->DataReceiveFailed(device_uid_, app_handle_, DataReceiveError());
}

void IAPConnection::OnSessionOpened(int session_id) {
  bool firstSession = session_ids_.empty();
  session_ids_.insert(session_id);
  if (firstSession) {
    controller_->ConnectDone(device_uid_, app_handle_);
  }
}

void IAPConnection::OnSessionClosed(int session_id) {
  session_ids_.erase(session_id);
}

IAPConnection::ReceiverThreadDelegate::ReceiverThreadDelegate(
  ipod_hdl_t* ipod_hdl, IAPConnection* parent) :
  parent_(parent), ipod_hdl_(ipod_hdl) {

  ParseEvents(); // parse all events before subscribing to notifications
}

bool IAPConnection::ReceiverThreadDelegate::ArmEvent(struct sigevent* event) {
  LOG4CXX_TRACE(logger_, "Arming for iAP event notification");
  if (ipod_notify(ipod_hdl_,
    _NOTIFY_ACTION_POLLARM, _NOTIFY_COND_OBAND | _NOTIFY_COND_INPUT, event) != -1) {

    LOG4CXX_DEBUG(logger_, "Successfully armed for iAP event notification");
    return true;
  }
  else {
    LOG4CXX_WARN(logger_, "Could not arm for iAP event notification");
    return false;
  }
}

void IAPConnection::ReceiverThreadDelegate::OnPulse() {
  ParseEvents();
}

void IAPConnection::ReceiverThreadDelegate::ParseEvents() {
  ssize_t nevents = ipod_eaf_getevents(ipod_hdl_, events_, kEventsBufferSize);
  for (ssize_t i = 0; i < nevents; ++i) {
    switch (events_[i].eventtype) {
      case IPOD_EAF_EVENT_SESSION_REQ:
        AcceptSession(events_[i].eventinfo);
        break;
      case IPOD_EAF_EVENT_SESSION_CLOSE:
        CloseSession(events_[i].eventinfo);
        break;
      case IPOD_EAF_EVENT_SESSION_DATA:
        ReceiveData(events_[i].eventinfo);
        break;
      case IPOD_EAF_EVENT_SESSION_OPEN:
        OpenSession(events_[i].eventinfo);
        break;
    }
  }
}

void IAPConnection::ReceiverThreadDelegate::AcceptSession(uint32_t protocol_id) {
  char protocol_name[kProtocolNameSize];
  ipod_eaf_getprotocol(ipod_hdl_, protocol_id, protocol_name, kProtocolNameSize);
  AcceptSession(protocol_id, protocol_name);
}

void IAPConnection::ReceiverThreadDelegate::AcceptSession(uint32_t protocol_id, const char* protocol_name) {
  LOG4CXX_INFO(logger_, "iAP: session request on protocol " << protocol_name);
  LOG4CXX_TRACE(logger_, "iAP: accepting session on protocol " << protocol_name);
  if (ipod_eaf_session_accept(ipod_hdl_, protocol_id, 0) != -1) {
    LOG4CXX_DEBUG(logger_, "iAP: session on protocol " << protocol_name << " accepted");
// For some reason event IPOD_EAF_EVENT_SESSION_OPEN is never reported
// and we have to open session right after having accepted it
    OpenSession(protocol_id, protocol_name);
  }
  else {
    LOG4CXX_ERROR(logger_, "iAP: failed to accept session on protocol " << protocol_name);
  }
}

void IAPConnection::ReceiverThreadDelegate::CloseSession(uint32_t session_id) {
  LOG4CXX_TRACE(logger_, "iAP: closing session " << session_id);
  if (ipod_eaf_session_free(ipod_hdl_, session_id) != -1) {
    LOG4CXX_DEBUG(logger_, "iAP: session " << session_id << " closed");
  }
  else {
    LOG4CXX_WARN(logger_, "iAP: failed to close session " << session_id);
  }
  parent_->OnSessionClosed(session_id);
}

void IAPConnection::ReceiverThreadDelegate::ReceiveData(uint32_t session_id) {
  LOG4CXX_TRACE(logger_, "iAP: receiving data on session " << session_id);
  int size = ipod_eaf_recv(ipod_hdl_, session_id, buffer_, kBufferSize);
  if (size != -1) {
    LOG4CXX_INFO(logger_, "iAP: received " << size << " bytes");
    RawMessageSptr message(new protocol_handler::RawMessage(0, 0, buffer_, size));
    parent_->OnDataReceived(message);
  }
  else {
    LOG4CXX_WARN(logger_, "iAP: error occured while receiving data");
    parent_->OnReceiveFailed();
  }
}

void IAPConnection::ReceiverThreadDelegate::OpenSession(uint32_t protocol_id) {
  char protocol_name[kProtocolNameSize];
  ipod_eaf_getprotocol(ipod_hdl_, protocol_id, protocol_name, kProtocolNameSize);
  OpenSession(protocol_id, protocol_name);
}

void IAPConnection::ReceiverThreadDelegate::OpenSession(uint32_t protocol_id, const char* protocol_name) {
  LOG4CXX_TRACE(logger_, "iAP: opening session on protocol " << protocol_name);
  int session_id = ipod_eaf_session_open(ipod_hdl_, protocol_id);
  if (session_id != -1) {
    LOG4CXX_DEBUG(logger_, "iAP: opened session " << session_id << " on protocol " << protocol_name);
    parent_->OnSessionOpened(session_id);
  }
  else {
    LOG4CXX_ERROR(logger_, "iAP: failed to open session on protocol " << protocol_name);
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager
