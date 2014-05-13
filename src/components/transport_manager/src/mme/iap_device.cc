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

#include "transport_manager/mme/iap_device.h"
#include "transport_manager/mme/iap_connection.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

IAPDevice::IAPDevice(const std::string& mount_point,
                     const std::string& name,
                     const DeviceUID& unique_device_id,
                     TransportAdapterController* controller) :
  MmeDevice(mount_point, name, unique_device_id), controller_(controller), last_app_id_(0) {

  LOG4CXX_TRACE(logger_, "iAP: connecting to " << mount_point);
  ipod_hdl_ = ipod_connect(mount_point.c_str(), 0);
  if (ipod_hdl_ != 0) {
    LOG4CXX_DEBUG(logger_, "iAP: connected to " << mount_point);

    receiver_thread_ = new threads::Thread("iAP event notifier",
      new IAPEventThreadDelegate(ipod_hdl_, this));
    receiver_thread_->start();
  }
  else {
    LOG4CXX_ERROR(logger_, "iAP: could not connect to " << mount_point);
  }
}

IAPDevice::~IAPDevice() {
  receiver_thread_->stop();

  LOG4CXX_TRACE(logger_, "iAP: disconecting from " << mount_point());
  if (ipod_disconnect(ipod_hdl_) != -1) {
    LOG4CXX_DEBUG(logger_, "iAP: disconnected from " << mount_point());
  }
  else {
    LOG4CXX_WARN(logger_, "iAP: could not disconnect from " << mount_point());
  }
}

ApplicationList IAPDevice::GetApplicationList() const {
  ApplicationList app_list;
  apps_lock_.Acquire();
  for (AppContainer::const_iterator i = apps_.begin(); i != apps_.end(); ++i) {
    ApplicationHandle app_id = i->second;
    app_list.push_back(app_id);
  }
  apps_lock_.Release();
  return app_list;
}

ipod_hdl_t* IAPDevice::RegisterConnection(ApplicationHandle app_id, IAPConnection* connection) {
  connections_lock_.Acquire();
  LOG4CXX_INFO(logger_, "iAP: registering new connection for application " << app_id);
  connections_.insert(std::make_pair(app_id, connection));
  connections_lock_.Release();
  return ipod_hdl_;
}

void IAPDevice::UnregisterConnection(ApplicationHandle app_id) {
  connections_lock_.Acquire();
  LOG4CXX_INFO(logger_, "iAP: unregistering connection for application " << app_id);
  connections_.erase(app_id);
  connections_lock_.Release();

  app_table_lock_.Acquire();
  for (AppTable::const_iterator i = app_table_.begin(); i != app_table_.end();) {
    if (i->second == app_id) {
      int session_id = i->first;
      LOG4CXX_DEBUG(logger_, "iAP: dropping session " << session_id << " for application " << app_id);
      app_table_.erase(session_id);
      break;
    }
    else {
      ++i;
    }
  }
  app_table_lock_.Release();

  apps_lock_.Acquire();
  for (AppContainer::const_iterator i = apps_.begin(); i != apps_.end();) {
    if (i->second == app_id) {
      uint32_t protocol_id = i->first;
      LOG4CXX_DEBUG(logger_, "iAP: dropping protocol " << protocol_id << " for application " << app_id);
      apps_.erase(protocol_id);
      break;
    }
    else {
      ++i;
    }
  }
  apps_lock_.Release();
}

void IAPDevice::OnSessionOpened(uint32_t protocol_id, int session_id) {
  ApplicationHandle app_id;
  apps_lock_.Acquire();
  AppContainer::const_iterator i = apps_.find(protocol_id);
  if (i != apps_.end()) {
    app_id = i->second;
  }
  else {
    app_id = ++last_app_id_;
    LOG4CXX_DEBUG(logger_, "iAP: adding new application " << app_id << " on protocol " << protocol_id);
    apps_.insert(std::make_pair(protocol_id, app_id));
    // Here will be notification
    // controller_->...
  }
  apps_lock_.Release();

  app_table_lock_.Acquire();
  LOG4CXX_DEBUG(logger_, "iAP: adding new session " << session_id << " for application " << app_id);
  app_table_.insert(std::make_pair(session_id, app_id));
  app_table_lock_.Release();

  connections_lock_.Acquire();
  ConnectionContainer::const_iterator j = connections_.find(app_id);
  if (j != connections_.end()) {
    IAPConnection* connection = j->second;
    connection->OnSessionOpened(session_id);
  }
  else {
    LOG4CXX_WARN(logger_, "iAP: no connection corresponding to application " << app_id);
  }
  connections_lock_.Release();
}

void IAPDevice::OnSessionClosed(int session_id) {
  app_table_lock_.Acquire();
  AppTable::const_iterator i = app_table_.find(session_id);
  if (i != app_table_.end()) {
    ApplicationHandle app_id = i->second;
    connections_lock_.Acquire();
    ConnectionContainer::const_iterator j = connections_.find(app_id);
    if (j != connections_.end()) {
      IAPConnection* connection = j->second;
      connection->OnSessionClosed(session_id);
    }
    else {
      LOG4CXX_WARN(logger_, "iAP: no connection corresponding to application " << app_id);
    }
    connections_lock_.Release();
    LOG4CXX_DEBUG(logger_, "iAP: dropping session " << session_id << " for application " << app_id);
    app_table_.erase(i->first);
  }
  else {
    LOG4CXX_WARN(logger_, "iAP: no application corresponding to session " << session_id);
  }
  app_table_lock_.Release();
}

void IAPDevice::OnDataReady(int session_id) {
  app_table_lock_.Acquire();
  AppTable::const_iterator i = app_table_.find(session_id);
  if (i != app_table_.end()) {
    ApplicationHandle app_id = i->second;
    connections_lock_.Acquire();
    ConnectionContainer::const_iterator j = connections_.find(app_id);
    if (j != connections_.end()) {
      IAPConnection* connection = j->second;
      connection->ReceiveData(session_id);
    }
    else {
      LOG4CXX_WARN(logger_, "iAP: no connection corresponding to application " << app_id);
    }
    connections_lock_.Release();
  }
  else {
    LOG4CXX_WARN(logger_, "iAP: no application corresponding to session " << session_id);
  }
  app_table_lock_.Release();
}

IAPDevice::IAPEventThreadDelegate::IAPEventThreadDelegate(
  ipod_hdl_t* ipod_hdl, IAPDevice* parent) :
  parent_(parent), ipod_hdl_(ipod_hdl) {

  ParseEvents(); // parse all events before subscribing to notifications
}

bool IAPDevice::IAPEventThreadDelegate::ArmEvent(struct sigevent* event) {
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

void IAPDevice::IAPEventThreadDelegate::OnPulse() {
  ParseEvents();
}

void IAPDevice::IAPEventThreadDelegate::ParseEvents() {
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
        DataReady(events_[i].eventinfo);
        break;
      case IPOD_EAF_EVENT_SESSION_OPEN:
        OpenSession(events_[i].eventinfo);
        break;
    }
  }
}

void IAPDevice::IAPEventThreadDelegate::AcceptSession(uint32_t protocol_id) {
  char protocol_name[kProtocolNameSize];
  ipod_eaf_getprotocol(ipod_hdl_, protocol_id, protocol_name, kProtocolNameSize);
  AcceptSession(protocol_id, protocol_name);
}

void IAPDevice::IAPEventThreadDelegate::AcceptSession(uint32_t protocol_id, const char* protocol_name) {
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

void IAPDevice::IAPEventThreadDelegate::CloseSession(uint32_t session_id) {
  LOG4CXX_TRACE(logger_, "iAP: closing session " << session_id);
  if (ipod_eaf_session_free(ipod_hdl_, session_id) != -1) {
    LOG4CXX_DEBUG(logger_, "iAP: session " << session_id << " closed");
  }
  else {
    LOG4CXX_WARN(logger_, "iAP: failed to close session " << session_id);
  }
  parent_->OnSessionClosed(session_id);
}

void IAPDevice::IAPEventThreadDelegate::DataReady(uint32_t session_id) {
  parent_->OnDataReady(session_id);
}

void IAPDevice::IAPEventThreadDelegate::OpenSession(uint32_t protocol_id) {
  char protocol_name[kProtocolNameSize];
  ipod_eaf_getprotocol(ipod_hdl_, protocol_id, protocol_name, kProtocolNameSize);
  OpenSession(protocol_id, protocol_name);
}

void IAPDevice::IAPEventThreadDelegate::OpenSession(uint32_t protocol_id, const char* protocol_name) {
  LOG4CXX_TRACE(logger_, "iAP: opening session on protocol " << protocol_name);
  int session_id = ipod_eaf_session_open(ipod_hdl_, protocol_id);
  if (session_id != -1) {
    LOG4CXX_DEBUG(logger_, "iAP: opened session " << session_id << " on protocol " << protocol_name);
    parent_->OnSessionOpened(protocol_id, session_id);
  }
  else {
    LOG4CXX_ERROR(logger_, "iAP: failed to open session on protocol " << protocol_name);
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager
