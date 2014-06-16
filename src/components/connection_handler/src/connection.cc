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

#include <limits.h>

#include <algorithm>

#include "connection_handler/connection.h"
#include "connection_handler/connection_handler.h"
#include "protocol_handler/protocol_packet.h"
#include "utils/logger.h"
#include "utils/macro.h"

#ifdef ENABLE_SECURITY
#include "security_manager/ssl_context.h"
#include "security_manager/security_query.h"
#endif  // ENABLE_SECURITY

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace connection_handler {

CREATE_LOGGERPTR_GLOBAL(logger_, "ConnectionHandler")

Connection::Connection(ConnectionHandle connection_handle,
                       DeviceHandle connection_device_handle,
                       ConnectionHandler* connection_handler,
                       int32_t heartbeat_timeout)
    : connection_handler_(connection_handler),
      connection_handle_(connection_handle),
      connection_device_handle_(connection_device_handle) {
  DCHECK(connection_handler_);

  heartbeat_monitor_ = new HeartBeatMonitor(heartbeat_timeout, this);
  heart_beat_monitor_thread_ = new threads::Thread("HeartBeatMonitorThread",
                                                   heartbeat_monitor_);
}

Connection::~Connection() {
  session_map_.clear();
  heart_beat_monitor_thread_->stop();
  delete heart_beat_monitor_thread_;
}

// Finds a key not presented in std::map<unsigned char, T>
// Returns 0 if that key not found
namespace {
template <class T>
uint32_t findGap(const std::map<unsigned char, T>& map) {
  for (uint32_t i = 1; i <= UCHAR_MAX; ++i) {
    if (map.find(i) == map.end()) {
      return i;
    }
  }
  return 0;
}
}  // namespace

uint32_t Connection::AddNewSession() {
  sync_primitives::AutoLock lock(session_map_lock_);
  const uint32_t session_id = findGap(session_map_);
  if (session_id > 0) {
    session_map_[session_id].service_list.push_back(protocol_handler::kRpc);
    session_map_[session_id].service_list.push_back(protocol_handler::kBulk);
  }
  return session_id;
}

uint32_t Connection::RemoveSession(uint8_t session) {
  sync_primitives::AutoLock lock(session_map_lock_);
  SessionMapIterator it = session_map_.find(session);
  if (session_map_.end() == it) {
    LOG4CXX_WARN(logger_, "Session not found in this connection!");
    return 0;
  }
  heartbeat_monitor_->RemoveSession(session);
  session_map_.erase(session);
  return session;
}

bool Connection::AddNewService(uint8_t session,
                               protocol_handler::ServiceType service_type,
                               const bool is_protected) {
  // Ignore wrong services
  if (protocol_handler::kControl == service_type ||
     protocol_handler::kInvalidServiceType == service_type ) {
    LOG4CXX_WARN(logger_, "Wrong service " << static_cast<int>(service_type));
    return false;
  }

  sync_primitives::AutoLock lock(session_map_lock_);

  SessionMapIterator session_it = session_map_.find(session);
  if (session_it == session_map_.end()) {
    LOG4CXX_WARN(logger_, "Session not found in this connection!");
    return false;
  }

  ServiceList& service_list = session_it->second.service_list;
  ServiceListIterator service_it = find(service_list.begin(),
                                        service_list.end(), service_type);
  // if service already exists
  if (service_it != service_list.end()) {
    Service& service = *service_it;
    // For unproteced service could be start protection
    if (!service.is_protected_ && is_protected) {
      service.is_protected_ = true;
      // Rpc and bulk shall be protected as one service
      if (service.service_type == protocol_handler::kRpc) {
        ServiceListIterator service_Bulk_it = find(service_list.begin(),
                                                service_list.end(),
                                                   protocol_handler::kBulk);
        DCHECK(service_Bulk_it != service_list.end());
        service_Bulk_it->is_protected_ = true;
      } else if (service.service_type == protocol_handler::kBulk) {
        ServiceListIterator service_Rpc_it = find(service_list.begin(),
                                                  service_list.end(),
                                                  protocol_handler::kRpc);
        DCHECK(service_Rpc_it != service_list.end());
        service_Rpc_it->is_protected_ = true;
      }
    } else {
      LOG4CXX_WARN(logger_,
                   "Session " << static_cast<int>(session) <<
                   " already established service " << static_cast<int>(service_type));
      return false;
    }
  } else { // service is not exists
    service_list.push_back(Service(service_type, is_protected));
  }

  return true;
}

bool Connection::RemoveService(
    uint8_t session, protocol_handler::ServiceType service_type) {
  // Ignore wrong and required for Session services
  if (protocol_handler::kControl == service_type ||
     protocol_handler::kInvalidServiceType == service_type ||
     protocol_handler::kRpc  == service_type ||
     protocol_handler::kBulk == service_type )
    return false;
  sync_primitives::AutoLock lock(session_map_lock_);

  SessionMapIterator session_it = session_map_.find(session);
  if (session_it == session_map_.end()) {
    LOG4CXX_WARN(logger_, "Session not found in this connection!");
    return false;
  }

  ServiceList& service_list = session_it->second.service_list;
  ServiceListIterator service_it = find(service_list.begin(),
                                        service_list.end(), service_type);
  if (service_it == service_list.end()) {
    LOG4CXX_WARN(logger_, "Session " << session << " didn't established"
                  " service " << service_type);
    return false;
  }
  service_list.erase(service_it);
  return true;
}

#ifdef ENABLE_SECURITY
int Connection::SetSSLContext(uint8_t sessionId,
                              security_manager::SSLContext *context) {
  sync_primitives::AutoLock lock(session_map_lock_);
  SessionMap::iterator session_it = session_map_.find(sessionId);
  if (session_it == session_map_.end()) {
    LOG4CXX_WARN(logger_, "Session not found in this connection!");
    return security_manager::SecurityQuery::ERROR_INTERNAL;
  }
  Session& session = session_it->second;
  session.ssl_context = context;
  return security_manager::SecurityQuery::ERROR_SUCCESS;
}

security_manager::SSLContext* Connection::GetSSLContext(
    uint8_t sessionId, const protocol_handler::ServiceType &service_type) const {
  LOG4CXX_TRACE(logger_, "Connection::GetSSLContext");
  sync_primitives::AutoLock lock(session_map_lock_);
  SessionMap::const_iterator session_it = session_map_.find(sessionId);
  if (session_it == session_map_.end()) {
    LOG4CXX_WARN(logger_, "Session not found in this connection!");
    return NULL;
  }
  const Session& session = session_it->second;
  // for control services return current SSLContext value
  if (protocol_handler::kControl == service_type)
    return session.ssl_context;
  const ServiceList& service_list = session_it->second.service_list;
  ServiceList::const_iterator service_it = std::find(service_list.begin(),
                                                     service_list.end(),
                                                     service_type);
  if (service_it == service_list.end()) {
    LOG4CXX_WARN(logger_, "Service not found in this session!");
    return NULL;
  }
  const Service& service = *service_it;
  if (!service.is_protected_)
    return NULL;
  LOG4CXX_TRACE(logger_, "SSLContext is " << session.ssl_context);
  return session.ssl_context;
}
#endif // ENABLE_SECURITY

ConnectionHandle Connection::connection_handle() const {
  return connection_handle_;
}

DeviceHandle Connection::connection_device_handle() {
  return connection_device_handle_;
}

const SessionMap Connection::session_map() const {
  sync_primitives::AutoLock lock(session_map_lock_);
  return session_map_;
}

void Connection::CloseSession(uint8_t session_id) {
  sync_primitives::AutoLock lock(session_map_lock_);

  SessionMapIterator session_it = session_map_.find(session_id);
  if (session_it == session_map_.end()) {
    return;
  }

  const size_t size = session_map_.size();

  //Close connection if it is last session
  if (1 == size) {
    heartbeat_monitor_->RemoveSession(session_id);
    connection_handler_->CloseConnection(connection_handle_);
  } else {
    connection_handler_->CloseSession(connection_handle_, session_id);
  }
}

void Connection::StartHeartBeat(uint8_t session_id) {
    bool is_first_session = heartbeat_monitor_->AddSession(session_id);

    // start monitoring thread when first session with heartbeat added
    if (is_first_session) {
      heart_beat_monitor_thread_->start();
    }
}

void Connection::SendHeartBeat(uint8_t session_id) {
  connection_handler_->SendHeartBeat(connection_handle_, session_id);
}

void Connection::KeepAlive(uint8_t session_id) {
  heartbeat_monitor_->KeepAlive(session_id);
}

} // namespace connection_handler
