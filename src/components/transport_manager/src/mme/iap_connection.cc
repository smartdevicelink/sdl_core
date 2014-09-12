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
#include "transport_manager/mme/iap_device.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

IAPConnection::IAPConnection(const DeviceUID& device_uid,
  const ApplicationHandle& app_handle,
  TransportAdapterController* controller,
  IAPDevice* parent) : device_uid_(device_uid),
  app_handle_(app_handle),
  controller_(controller),
  parent_(parent) {
}

void IAPConnection::Init() {
  IAPDevice::AppRecord record = parent_->RegisterConnection(app_handle_, this);
  protocol_name_ = record.first;
  ipod_hdl_ = record.second;

  controller_->ConnectDone(device_uid_, app_handle_);
}

TransportAdapter::Error IAPConnection::SendData(RawMessagePtr message) {
  int session_id;

  { // auto_lock scope
    sync_primitives::AutoLock auto_lock(session_ids_lock_);
    if (session_ids_.empty()) {
      LOG4CXX_WARN(logger_, "iAP: no opened sessions on protocol " << protocol_name_);
      return TransportAdapter::BAD_STATE;
    }
// How is session for sending data chosen?
    session_id = *session_ids_.begin();
  } // auto_lock scope

  LOG4CXX_TRACE(logger_, "iAP: sending data on protocol " << protocol_name_);
  if (ipod_eaf_send(ipod_hdl_, session_id, message->data(), message->data_size()) != -1) {
    LOG4CXX_INFO(logger_, "iAP: data on protocol " << protocol_name_ << " sent successfully");
    controller_->DataSendDone(device_uid_, app_handle_, message);
    return TransportAdapter::OK;
  }
  else {
    LOG4CXX_WARN(logger_, "iAP: error occurred while sending data on protocol " << protocol_name_);
    controller_->DataSendFailed(device_uid_, app_handle_, message, DataSendError());
    return TransportAdapter::FAIL;
  }
}

TransportAdapter::Error IAPConnection::Disconnect() {
  parent_->UnregisterConnection(app_handle_);
  controller_->DisconnectDone(device_uid_, app_handle_);
  return TransportAdapter::OK;
}

void IAPConnection::ReceiveData(int session_id) {
  int iterations = 0;
// QNX undocumented feature:
// it's necessary to read data
// until ENODATA occurs
  while (true) {
    LOG4CXX_TRACE(logger_, "iAP: receiving data on protocol " << protocol_name_ << " (session " << session_id << ")");
    int size = ipod_eaf_recv(ipod_hdl_, session_id, buffer_, kBufferSize);
    if (size != -1) {
      LOG4CXX_DEBUG(logger_, "iAP: received " << size << " bytes on protocol " << protocol_name_);
      if (size != 0) {
        RawMessagePtr message(new protocol_handler::RawMessage(0, 0, buffer_, size));
        controller_->DataReceiveDone(device_uid_, app_handle_, message);
      }
      ++iterations;
    }
    else {
      switch (errno) {
        case ENODATA:
          LOG4CXX_DEBUG(logger_, "iAP: data on protocol " << protocol_name_ << " read completely in " << iterations << " iterations");
          break;
        default:
          LOG4CXX_WARN(logger_, "iAP: error occurred while receiving data on protocol " << protocol_name_);
          controller_->DataReceiveFailed(device_uid_, app_handle_, DataReceiveError());
          break;
      }
      break;
    }
  }
}

void IAPConnection::OnSessionOpened(int session_id) {
  session_ids_lock_.Acquire();
  session_ids_.insert(session_id);
  session_ids_lock_.Release();
// QNX undocumented feature:
// it's necessary to read data
// even if no IPOD_EAF_EVENT_SESSION_DATA event has come
  ReceiveData(session_id);
}

void IAPConnection::OnSessionClosed(int session_id) {
  sync_primitives::AutoLock auto_lock(session_ids_lock_);
  session_ids_.erase(session_id);
  if (session_ids_.empty()) {
    parent_->UnregisterConnection(app_handle_);
    controller_->ConnectionAborted(device_uid_, app_handle_, CommunicationError());
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager
