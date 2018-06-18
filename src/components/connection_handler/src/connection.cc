/*
 * Copyright (c) 2018, Ford Motor Company
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
#include "utils/helpers.h"

#ifdef ENABLE_SECURITY
#include "security_manager/ssl_context.h"
#include "security_manager/security_manager.h"
#endif  // ENABLE_SECURITY

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace connection_handler {

CREATE_LOGGERPTR_GLOBAL(logger_, "ConnectionHandler")

Service* Session::FindService(
    const protocol_handler::ServiceType& service_type) {
  ServiceList::iterator service_it =
      std::find(service_list.begin(), service_list.end(), service_type);
  if (service_it != service_list.end()) {
    return &(*service_it);
  }
  return NULL;
}

const Service* Session::FindService(
    const protocol_handler::ServiceType& service_type) const {
  ServiceList::const_iterator service_it =
      std::find(service_list.begin(), service_list.end(), service_type);
  if (service_it != service_list.end()) {
    return &(*service_it);
  }
  return NULL;
}

Connection::Connection(ConnectionHandle connection_handle,
                       DeviceHandle connection_device_handle,
                       ConnectionHandler* connection_handler,
                       uint32_t heartbeat_timeout)
    : connection_handler_(connection_handler)
    , connection_handle_(connection_handle)
    , connection_device_handle_(connection_device_handle)
    , session_map_lock_(true)
    , primary_connection_handle_(0)
    , heartbeat_timeout_(heartbeat_timeout) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(connection_handler_);

  heartbeat_monitor_ = new HeartBeatMonitor(heartbeat_timeout_, this);
  heart_beat_monitor_thread_ =
      threads::CreateThread("HeartBeatMonitor", heartbeat_monitor_);
  heart_beat_monitor_thread_->start();
}

Connection::~Connection() {
  LOG4CXX_AUTO_TRACE(logger_);
  heart_beat_monitor_thread_->join();
  delete heartbeat_monitor_;
  threads::DeleteThread(heart_beat_monitor_thread_);

  // Before clearing out the session_map_, we must remove all sessions
  // associated with this Connection from the SessionConnectionMap.
  NonConstDataAccessor<SessionConnectionMap> session_connection_map_accessor =
      connection_handler_->session_connection_map();
  SessionConnectionMap& session_connection_map =
      session_connection_map_accessor.GetData();
  sync_primitives::AutoLock lock(session_map_lock_);
  SessionMap::iterator session_it = session_map_.begin();
  while (session_it != session_map_.end()) {
    LOG4CXX_INFO(
        logger_,
        "Removed Session ID "
            << static_cast<int>(session_it->first)
            << " from Session/Connection Map in Connection Destructor");
    SessionConnectionMap::iterator itr =
        session_connection_map.find(session_it->first);
    if (session_connection_map.end() != itr) {
      session_connection_map.erase(session_it->first);
    }
    session_it++;
  }

  session_map_.clear();
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

DEPRECATED uint32_t Connection::AddNewSession() {
  return AddNewSession(0);
}

uint32_t Connection::AddNewSession(
    const transport_manager::ConnectionUID connection_handle) {
  LOG4CXX_AUTO_TRACE(logger_);

  // Even though we have our own SessionMap, we use the Connection Handler's
  // SessionConnectionMap to generate a session ID. We want to make sure that
  // session IDs are globally unique, and not only unique within a Connection.
  NonConstDataAccessor<SessionConnectionMap> session_connection_map_accessor =
      connection_handler_->session_connection_map();
  SessionConnectionMap& session_connection_map =
      session_connection_map_accessor.GetData();
  const uint32_t session_id = findGap(session_connection_map);
  if (session_id > 0) {
    LOG4CXX_INFO(logger_,
                 "New session ID " << session_id << " and Connection Id "
                                   << static_cast<int>(connection_handle)
                                   << " added to Session/Connection Map");
    SessionTransports st;
    st.primary_transport = connection_handle;
    st.secondary_transport = 0;
    session_connection_map[session_id] = st;

    // NESTED LOCK: make sure to never lock the ConnectionHandler's
    // SessionConnectedMap inside of the "session_map_lock_"
    sync_primitives::AutoLock lock(session_map_lock_);
    Session& new_session = session_map_[session_id];
    new_session.protocol_version = ::protocol_handler::PROTOCOL_VERSION_2;
    new_session.service_list.push_back(
        Service(protocol_handler::kRpc, connection_handle));
    new_session.service_list.push_back(
        Service(protocol_handler::kBulk, connection_handle));
  } else {
    LOG4CXX_WARN(logger_,
                 "Session/Connection Map could not create a new session ID!!!");
  }

  return session_id;
}

uint32_t Connection::RemoveSession(uint8_t session_id) {
  NonConstDataAccessor<SessionConnectionMap> session_connection_map_accessor =
      connection_handler_->session_connection_map();
  SessionConnectionMap& session_connection_map =
      session_connection_map_accessor.GetData();
  SessionConnectionMap::iterator itr = session_connection_map.find(session_id);
  if (session_connection_map.end() == itr) {
    LOG4CXX_WARN(logger_, "Session not found in Session/Connection Map!");
    return 0;
  } else {
    LOG4CXX_INFO(logger_,
                 "Removed Session ID " << static_cast<int>(session_id)
                                       << " from Session/Connection Map");
    session_connection_map.erase(session_id);

    // Again, a NESTED lock, but it follows the rules.
    sync_primitives::AutoLock lock(session_map_lock_);
    SessionMap::iterator it = session_map_.find(session_id);
    if (session_map_.end() == it) {
      LOG4CXX_WARN(logger_, "Session not found in this connection!");
      return 0;
    }
    heartbeat_monitor_->RemoveSession(session_id);
    session_map_.erase(session_id);
  }

  return session_id;
}

DEPRECATED bool Connection::AddNewService(
    uint8_t session_id,
    protocol_handler::ServiceType service_type,
    const bool request_protection) {
  return AddNewService(session_id, service_type, request_protection, 0);
}

bool Connection::AddNewService(uint8_t session_id,
                               protocol_handler::ServiceType service_type,
                               const bool request_protection,
                               transport_manager::ConnectionUID connection_id) {
  // Ignore wrong services
  if (protocol_handler::kControl == service_type ||
      protocol_handler::kInvalidServiceType == service_type) {
    LOG4CXX_WARN(logger_, "Wrong service " << static_cast<int>(service_type));
    return false;
  }

  LOG4CXX_DEBUG(logger_,
                "Add service " << service_type << " for session "
                               << static_cast<uint32_t>(session_id)
                               << " using connection ID "
                               << static_cast<uint32_t>(connection_id));
  sync_primitives::AutoLock lock(session_map_lock_);

  SessionMap::iterator session_it = session_map_.find(session_id);
  if (session_it == session_map_.end()) {
    LOG4CXX_WARN(logger_, "Session not found in this connection!");
    return false;
  }
  Session& session = session_it->second;

  if (session.protocol_version <= protocol_handler::PROTOCOL_VERSION_2 &&
      helpers::Compare<protocol_handler::ServiceType,
                       helpers::EQ,
                       helpers::ONE>(
          service_type,
          protocol_handler::ServiceType::kAudio,
          protocol_handler::ServiceType::kMobileNav)) {
    LOG4CXX_WARN(logger_,
                 "Audio and video services are disallowed for protocol version "
                 "2 or lower");
    return false;
  }

  Service* service = session.FindService(service_type);
  // if service already exists
  if (service) {
#ifdef ENABLE_SECURITY
    if (!request_protection) {
      LOG4CXX_WARN(logger_,
                   "Session " << static_cast<int>(session_id)
                              << " already has unprotected service "
                              << static_cast<int>(service_type));
      return false;
    }
    if (service->is_protected_) {
      LOG4CXX_WARN(logger_,
                   "Session " << static_cast<int>(session_id)
                              << " already has protected service "
                              << static_cast<int>(service_type));
      return false;
    }
    // For unproteced service could be start protection
    return true;
#else
    // Service already exists
    return false;
#endif  // ENABLE_SECURITY
  }
  // id service is not exists
  session.service_list.push_back(Service(service_type, connection_id));
  return true;
}

inline bool is_incorrect_for_remove_service(
    const protocol_handler::ServiceType service_type) {
  return
      // Control type is internal part of session
      protocol_handler::kControl == service_type ||
      // RPC and bulk service is necessary part of session
      protocol_handler::kRpc == service_type ||
      protocol_handler::kBulk == service_type ||
      // Invalid service is not part of session
      protocol_handler::kInvalidServiceType == service_type;
}

bool Connection::RemoveService(uint8_t session_id,
                               protocol_handler::ServiceType service_type) {
  // Ignore wrong and required for Session services
  if (is_incorrect_for_remove_service(service_type)) {
    LOG4CXX_WARN(logger_,
                 "Could not remove service " << static_cast<int>(service_type));
    return false;
  }
  sync_primitives::AutoLock lock(session_map_lock_);

  SessionMap::iterator session_it = session_map_.find(session_id);
  if (session_map_.end() == session_it) {
    LOG4CXX_WARN(logger_, "Session not found in this connection!");
    return false;
  }

  ServiceList& service_list = session_it->second.service_list;
  ServiceList::iterator service_it =
      find(service_list.begin(), service_list.end(), service_type);
  if (service_list.end() == service_it) {
    LOG4CXX_WARN(logger_,
                 "Session " << session_id << " didn't established"
                                             " service " << service_type);
    return false;
  }
  service_list.erase(service_it);
  return true;
}

uint8_t Connection::RemoveSecondaryServices(
    transport_manager::ConnectionUID secondary_connection_handle,
    std::list<protocol_handler::ServiceType>& removed_services_list) {
  LOG4CXX_AUTO_TRACE(logger_);

  uint8_t found_session_id = 0;
  sync_primitives::AutoLock lock(session_map_lock_);

  LOG4CXX_INFO(logger_,
               "RemoveSecondaryServices looking for services on Connection ID "
                   << static_cast<int>(secondary_connection_handle));

  // Walk the SessionMap in the primary connection, and for each
  // Session, we walk its ServiceList, looking for all the services
  // that were running on the now-closed Secondary Connection.
  for (SessionMap::iterator session_it = session_map_.begin();
       session_map_.end() != session_it;
       ++session_it) {
    LOG4CXX_INFO(logger_,
                 "RemoveSecondaryServices found session ID "
                     << static_cast<int>(session_it->first));

    // Now, for each session, walk the its ServiceList, looking for services
    // that were using secondary)_connection_handle. If we find such a service,
    // set session_found and break out of the outer loop.
    ServiceList& service_list = session_it->second.service_list;
    ServiceList::iterator service_it = service_list.begin();
    for (; service_it != service_list.end();) {
      LOG4CXX_INFO(logger_,
                   "RemoveSecondaryServices found service ID "
                       << static_cast<int>(service_it->service_type));
      if (service_it->connection_id == secondary_connection_handle) {
        found_session_id = session_it->first;

        LOG4CXX_INFO(logger_,
                     "RemoveSecondaryServices removing Service "
                         << static_cast<int>(service_it->service_type)
                         << " in session "
                         << static_cast<int>(found_session_id));

        removed_services_list.push_back(service_it->service_type);
        service_it = service_list.erase(service_it);
      } else {
        service_it++;
      }
    }

    // If we found a session that had services running on the secondary
    // connection, we're done.
    if (found_session_id != 0) {
      break;
    }
  }

  return found_session_id;
}

#ifdef ENABLE_SECURITY
int Connection::SetSSLContext(uint8_t session_id,
                              security_manager::SSLContext* context) {
  sync_primitives::AutoLock lock(session_map_lock_);
  SessionMap::iterator session_it = session_map_.find(session_id);
  if (session_it == session_map_.end()) {
    LOG4CXX_WARN(logger_, "Session not found in this connection!");
    return security_manager::SecurityManager::ERROR_INTERNAL;
  }
  Session& session = session_it->second;
  session.ssl_context = context;
  return security_manager::SecurityManager::ERROR_SUCCESS;
}

security_manager::SSLContext* Connection::GetSSLContext(
    const uint8_t session_id,
    const protocol_handler::ServiceType& service_type) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(session_map_lock_);
  SessionMap::const_iterator session_it = session_map_.find(session_id);
  if (session_it == session_map_.end()) {
    LOG4CXX_WARN(logger_, "Session not found in this connection!");
    return NULL;
  }
  const Session& session = session_it->second;
  // for control services return current SSLContext value
  if (protocol_handler::kControl == service_type)
    return session.ssl_context;
  const Service* service = session.FindService(service_type);
  if (!service) {
    LOG4CXX_WARN(logger_, "Service not found in this session!");
    return NULL;
  }
  if (!service->is_protected_)
    return NULL;
  LOG4CXX_TRACE(logger_, "SSLContext is " << session.ssl_context);
  return session.ssl_context;
}

void Connection::SetProtectionFlag(
    const uint8_t session_id,
    const protocol_handler::ServiceType& service_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(session_map_lock_);
  SessionMap::iterator session_it = session_map_.find(session_id);
  if (session_it == session_map_.end()) {
    LOG4CXX_WARN(logger_, "Session not found in this connection!");
    return;
  }
  Session& session = session_it->second;
  Service* service = session.FindService(service_type);
  if (!service) {
    LOG4CXX_WARN(logger_, "Service not found in this session!");
    return;
  }
  service->is_protected_ = true;
  // Rpc and bulk shall be protected as one service
  if (service->service_type == protocol_handler::kRpc) {
    Service* service_bulk = session.FindService(protocol_handler::kBulk);
    DCHECK(service_bulk);
    service_bulk->is_protected_ = true;
  } else if (service->service_type == protocol_handler::kBulk) {
    Service* service_rpc = session.FindService(protocol_handler::kRpc);
    DCHECK(service_rpc);
    service_rpc->is_protected_ = true;
  }
}

#endif  // ENABLE_SECURITY

bool Connection::SessionServiceExists(
    const uint8_t session_id,
    const protocol_handler::ServiceType& service_type) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(session_map_lock_);

  SessionMap::const_iterator session_it = session_map_.find(session_id);
  if (session_it == session_map_.end()) {
    LOG4CXX_WARN(logger_, "Session not found in this connection!");
    return false;
  }

  const Session& session = session_it->second;
  return session.FindService(service_type);
}

ConnectionHandle Connection::connection_handle() const {
  return connection_handle_;
}

DeviceHandle Connection::connection_device_handle() const {
  return connection_device_handle_;
}

const SessionMap Connection::session_map() const {
  sync_primitives::AutoLock lock(session_map_lock_);
  return session_map_;
}

void Connection::CloseSession(uint8_t session_id) {
  size_t size;
  {
    sync_primitives::AutoLock lock(session_map_lock_);

    SessionMap::iterator session_it = session_map_.find(session_id);
    if (session_it == session_map_.end()) {
      return;
    }
    size = session_map_.size();
  }

  connection_handler_->CloseSession(
      connection_handle_, session_id, connection_handler::kCommon);

  // Close connection if it is last session
  if (1 == size) {
    connection_handler_->CloseConnection(connection_handle_);
  }
}

void Connection::UpdateProtocolVersionSession(uint8_t session_id,
                                              uint8_t protocol_version) {
  sync_primitives::AutoLock lock(session_map_lock_);
  SessionMap::iterator session_it = session_map_.find(session_id);
  if (session_map_.end() == session_it) {
    LOG4CXX_WARN(logger_, "Session not found in this connection!");
    return;
  }
  Session& session = session_it->second;
  session.protocol_version = protocol_version;
}

bool Connection::SupportHeartBeat(uint8_t session_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(session_map_lock_);
  SessionMap::iterator session_it = session_map_.find(session_id);
  if (session_map_.end() == session_it) {
    LOG4CXX_WARN(logger_, "Session not found in this connection!");
    return false;
  }
  Session& session = session_it->second;
  return (
      (session.protocol_version >= ::protocol_handler::PROTOCOL_VERSION_3) &&
      (0 != heartbeat_timeout_));
}

bool Connection::ProtocolVersion(uint8_t session_id,
                                 uint8_t& protocol_version) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(session_map_lock_);
  SessionMap::iterator session_it = session_map_.find(session_id);
  if (session_map_.end() == session_it) {
    LOG4CXX_WARN(logger_, "Session not found in this connection!");
    return false;
  }
  protocol_version = (session_it->second).protocol_version;
  return true;
}

ConnectionHandle Connection::primary_connection_handle() const {
  return primary_connection_handle_;
}

void Connection::SetPrimaryConnectionHandle(
    ConnectionHandle primary_connection_handle) {
  primary_connection_handle_ = primary_connection_handle;
}

void Connection::StartHeartBeat(uint8_t session_id) {
  heartbeat_monitor_->AddSession(session_id);
}

void Connection::SendHeartBeat(uint8_t session_id) {
  connection_handler_->SendHeartBeat(connection_handle_, session_id);
}

void Connection::KeepAlive(uint8_t session_id) {
  heartbeat_monitor_->KeepAlive(session_id);
}

void Connection::SetHeartBeatTimeout(uint32_t timeout, uint8_t session_id) {
  heartbeat_monitor_->set_heartbeat_timeout_milliseconds(timeout, session_id);
}

}  // namespace connection_handler
