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
#include <strings.h>

#include <string>
#include <list>
#include <algorithm>
#include <memory>

#include "connection_handler/connection_handler_impl.h"
#include "transport_manager/info.h"
#include "encryption/hashing.h"

#ifdef ENABLE_SECURITY
#include "security_manager/security_manager.h"
#endif  // ENABLE_SECURITY

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace connection_handler {

CREATE_LOGGERPTR_GLOBAL(logger_, "ConnectionHandler")

ConnectionHandle HandleFromConnectionUID(transport_manager::ConnectionUID uid) {
  return ConnectionHandle(uid);
}

transport_manager::ConnectionUID ConnectionUIDFromHandle(
    ConnectionHandle handle) {
  return transport_manager::ConnectionUID(handle);
}

ConnectionHandlerImpl::ConnectionHandlerImpl(
    const ConnectionHandlerSettings& settings,
    transport_manager::TransportManager& tm)
    : settings_(settings)
    , connection_handler_observer_(NULL)
    , connection_handler_observer_valid_(false)
    , transport_manager_(tm)
    , protocol_handler_(NULL)
    , connection_list_lock_()
    , connection_handler_observer_lock_()
    , connection_list_deleter_(&connection_list_)
    , start_service_context_map_lock_()
    , start_service_context_map_() {}

ConnectionHandlerImpl::~ConnectionHandlerImpl() {
  LOG4CXX_AUTO_TRACE(logger_);
}

void ConnectionHandlerImpl::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  ConnectionList::iterator itr = connection_list_.begin();
  while (itr != connection_list_.end()) {
    RemoveConnection(itr->second->connection_handle());
    itr = connection_list_.begin();
  }

  sync_primitives::AutoLock auto_lock(start_service_context_map_lock_);
  start_service_context_map_.clear();
}

void ConnectionHandlerImpl::invalidate_connection_handler_observer() {
  connection_handler_observer_valid_ = false;
}

void ConnectionHandlerImpl::set_connection_handler_observer(
    ConnectionHandlerObserver* observer) {
  LOG4CXX_DEBUG(logger_,
                "ConnectionHandlerImpl::set_connection_handler_observer() "
                    << observer);
  sync_primitives::AutoWriteLock write_lock(connection_handler_observer_lock_);
  if (!observer) {
    LOG4CXX_WARN(logger_, "Set Null pointer to observer.");
    connection_handler_observer_valid_ = false;
  }
  else {
    connection_handler_observer_valid_ = true;
  }
  connection_handler_observer_ = observer;
}

void ConnectionHandlerImpl::set_protocol_handler(
    protocol_handler::ProtocolHandler* protocol_handler) {
  LOG4CXX_DEBUG(logger_,
                "ConnectionHandlerImpl::set_protocol_handler()"
                    << protocol_handler);
  if (!protocol_handler) {
    LOG4CXX_WARN(logger_, "Set Null pointer to protocol handler.");
  }
  protocol_handler_ = protocol_handler;
}

void ConnectionHandlerImpl::OnDeviceListUpdated(
    const std::vector<transport_manager::DeviceInfo>&) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_valid_ && connection_handler_observer_) {
    connection_handler_observer_->OnDeviceListUpdated(device_list_);
  }
}

void ConnectionHandlerImpl::OnFindNewApplicationsRequest() {
  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_valid_ && connection_handler_observer_) {
    connection_handler_observer_->OnFindNewApplicationsRequest();
  }
}

void ConnectionHandlerImpl::OnDeviceFound(
    const transport_manager::DeviceInfo&) {
  LOG4CXX_AUTO_TRACE(logger_);
}

void ConnectionHandlerImpl::OnDeviceAdded(
    const transport_manager::DeviceInfo& device_info) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto handle = device_info.device_handle();

  Device device(handle,
                device_info.name(),
                device_info.mac_address(),
                device_info.connection_type());

  auto result = device_list_.insert(std::make_pair(handle, device));

  if (!result.second) {
    LOG4CXX_ERROR(logger_,
                  "Device with handle " << handle
                                        << " is known already. "
                                           "Information won't be updated.");
    return;
  }
}

void ConnectionHandlerImpl::OnDeviceRemoved(
    const transport_manager::DeviceInfo& device_info) {
  LOG4CXX_AUTO_TRACE(logger_);
  // Device has been removed. Perform all needed actions.
  // 1. Delete all the connections and sessions of this device
  // 2. Delete device from a list
  // 3. Let observer know that device has been deleted.

  std::vector<ConnectionHandle> connections_to_remove;
  {
    sync_primitives::AutoReadLock lock(connection_list_lock_);
    for (ConnectionList::iterator it = connection_list_.begin();
         it != connection_list_.end();
         ++it) {
      if (device_info.device_handle() ==
          (*it).second->connection_device_handle()) {
        connections_to_remove.push_back((*it).first);
      }
    }
  }

  std::vector<ConnectionHandle>::iterator it = connections_to_remove.begin();
  for (; it != connections_to_remove.end(); ++it) {
    RemoveConnection(*it);
  }

  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_valid_ && connection_handler_observer_) {
    connection_handler_observer_->RemoveDevice(device_info.device_handle());
    connection_handler_observer_->OnDeviceListUpdated(device_list_);
  }
  device_list_.erase(device_info.device_handle());
}

void ConnectionHandlerImpl::OnDeviceSwitchingFinish(
    const transport_manager::DeviceUID& device_uid) {
  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_valid_ && connection_handler_observer_) {
    connection_handler_observer_->OnDeviceSwitchingFinish(
        encryption::MakeHash(device_uid));
  }
}

namespace {
struct DeviceFinder {
  explicit DeviceFinder(const std::string& device_uid)
      : device_uid_(device_uid) {}
  bool operator()(const DeviceMap::value_type& device) {
    return device_uid_ == device.second.mac_address();
  }

 private:
  const std::string& device_uid_;
};
}  // namespace

void ConnectionHandlerImpl::OnDeviceSwitchingStart(
    const std::string& device_uid_from, const std::string& device_uid_to) {
  auto device_from =
      std::find_if(device_list_.begin(),
                   device_list_.end(),
                   DeviceFinder(encryption::MakeHash(device_uid_from)));

  auto device_to =
      std::find_if(device_list_.begin(),
                   device_list_.end(),
                   DeviceFinder(encryption::MakeHash(device_uid_to)));

  DCHECK_OR_RETURN_VOID(device_list_.end() != device_from);
  DCHECK_OR_RETURN_VOID(device_list_.end() != device_to);
  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_valid_ && connection_handler_observer_) {
    connection_handler_observer_->OnDeviceSwitchingStart(device_from->second,
                                                         device_to->second);
  }
}

void ConnectionHandlerImpl::OnScanDevicesFinished() {
  LOG4CXX_AUTO_TRACE(logger_);
}

void ConnectionHandlerImpl::OnScanDevicesFailed(
    const transport_manager::SearchDeviceError& error) {
  LOG4CXX_WARN(logger_, "Scan devices failed. " << error.text());
}

void ConnectionHandlerImpl::OnConnectionEstablished(
    const transport_manager::DeviceInfo& device_info,
    const transport_manager::ConnectionUID connection_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  DeviceMap::iterator it = device_list_.find(device_info.device_handle());
  if (device_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown device!");
    return;
  }
  LOG4CXX_DEBUG(logger_,
                "Add Connection #" << connection_id << " to the list.");
  sync_primitives::AutoWriteLock lock(connection_list_lock_);
  connection_list_.insert(ConnectionList::value_type(
      connection_id,
      new Connection(connection_id,
                     device_info.device_handle(),
                     this,
                     get_settings().heart_beat_timeout())));
}

void ConnectionHandlerImpl::OnConnectionFailed(
    const transport_manager::DeviceInfo& device_info,
    const transport_manager::ConnectError& error) {
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_, "Failed connecting.");
}

void ConnectionHandlerImpl::OnConnectionClosed(
    transport_manager::ConnectionUID connection_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  OnConnectionEnded(connection_id);
}

void ConnectionHandlerImpl::OnConnectionClosedFailure(
    transport_manager::ConnectionUID connection_id,
    const transport_manager::DisconnectError& error) {
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_, "ConnectionHandlerImpl::OnConnectionClosedFailure");
}

void ConnectionHandlerImpl::OnUnexpectedDisconnect(
    transport_manager::ConnectionUID connection_id,
    const transport_manager::CommunicationError& error) {
  LOG4CXX_AUTO_TRACE(logger_);

  OnConnectionEnded(connection_id);
}

void ConnectionHandlerImpl::OnDeviceConnectionLost(
    const transport_manager::DeviceHandle& device,
    const transport_manager::DisconnectDeviceError& error) {
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_, "Lost connection with device " << device);
}

void ConnectionHandlerImpl::OnDisconnectFailed(
    const transport_manager::DeviceHandle& device,
    const transport_manager::DisconnectDeviceError& error) {
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_, "Trying to Disconnect device failed.");
}

void ConnectionHandlerImpl::RemoveConnection(
    const ConnectionHandle connection_handle) {
  LOG4CXX_AUTO_TRACE(logger_);

  OnConnectionEnded(connection_handle);
}

#ifdef ENABLE_SECURITY
bool AllowProtection(const ConnectionHandlerSettings& settings,
                     const protocol_handler::ServiceType& service_type,
                     const bool is_protected) {
  const std::vector<int>& force_unprotected_list =
      is_protected ? settings.force_unprotected_service()
                   : settings.force_protected_service();

  if (std::find(force_unprotected_list.begin(),
                force_unprotected_list.end(),
                service_type) != force_unprotected_list.end()) {
    LOG4CXX_ERROR(logger_,
                  "Service " << static_cast<int>(service_type)
                             << " shall be protected");
    return false;
  }
  LOG4CXX_DEBUG(logger_,
                "Service " << static_cast<int>(service_type) << " allowed");
  return true;
}
#endif  // ENABLE_SECURITY

uint32_t ConnectionHandlerImpl::OnSessionStartedCallback(
    const transport_manager::ConnectionUID connection_handle,
    const uint8_t session_id,
    const protocol_handler::ServiceType& service_type,
    const bool is_protected,
    uint32_t* hash_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (hash_id) {
    *hash_id = protocol_handler::HASH_ID_WRONG;
  }

#ifdef ENABLE_SECURITY
  if (!AllowProtection(get_settings(), service_type, is_protected)) {
    return 0;
  }
#endif  // ENABLE_SECURITY
  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
    return 0;
  }
  uint32_t new_session_id = 0;

  Connection* connection = it->second;
  if ((0 == session_id) && (protocol_handler::kRpc == service_type)) {
    new_session_id = connection->AddNewSession();
    if (0 == new_session_id) {
      LOG4CXX_ERROR(logger_, "Couldn't start new session!");
      return 0;
    }
    if (hash_id) {
      *hash_id = KeyFromPair(connection_handle, new_session_id);
    }
  } else {  // Could be create new service or protected exists one
    if (!connection->AddNewService(session_id, service_type, is_protected)) {
      LOG4CXX_ERROR(logger_,
                    "Couldn't establish "
#ifdef ENABLE_SECURITY
                        << (is_protected ? "protected" : "non-protected")
#endif  // ENABLE_SECURITY
                        << " service " << static_cast<int>(service_type)
                        << " for session " << static_cast<int>(session_id));
      return 0;
    }
    new_session_id = session_id;
    if (hash_id) {
      *hash_id = protocol_handler::HASH_ID_NOT_SUPPORTED;
    }
  }
  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_valid_ && connection_handler_observer_) {
    const uint32_t session_key = KeyFromPair(connection_handle, new_session_id);
    const bool success = connection_handler_observer_->OnServiceStartedCallback(
        connection->connection_device_handle(), session_key, service_type);
    if (!success) {
      LOG4CXX_WARN(logger_,
                   "Service starting forbidden by connection_handler_observer");
      if (protocol_handler::kRpc == service_type) {
        connection->RemoveSession(new_session_id);
      } else {
        connection->RemoveService(session_id, service_type);
      }
      return 0;
    }
  }
  return new_session_id;
}

void ConnectionHandlerImpl::OnSessionStartedCallback(
    const transport_manager::ConnectionUID connection_handle,
    const uint8_t session_id,
    const protocol_handler::ServiceType& service_type,
    const bool is_protected,
    const BsonObject* params) {
  LOG4CXX_AUTO_TRACE(logger_);

  std::vector<std::string> rejected_params;
  protocol_handler::SessionContext context(connection_handle,
                                           session_id,
                                           0,
                                           service_type,
                                           protocol_handler::HASH_ID_WRONG,
                                           is_protected);

#ifdef ENABLE_SECURITY
  if (!AllowProtection(get_settings(), service_type, is_protected)) {
    protocol_handler_->NotifySessionStarted(context, rejected_params);
    return;
  }
#endif  // ENABLE_SECURITY
  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
    protocol_handler_->NotifySessionStarted(context, rejected_params);
    return;
  }

  Connection* connection = it->second;
  context.is_new_service_ =
      !connection->SessionServiceExists(session_id, service_type);

  if ((0 == session_id) && (protocol_handler::kRpc == service_type)) {
    context.new_session_id_ = connection->AddNewSession();
    if (0 == context.new_session_id_) {
      LOG4CXX_ERROR(logger_, "Couldn't start new session!");
      protocol_handler_->NotifySessionStarted(context, rejected_params);
      return;
    }
    context.hash_id_ = KeyFromPair(connection_handle, context.new_session_id_);
  } else {  // Could be create new service or protected exists one
    if (!connection->AddNewService(session_id, service_type, is_protected)) {
      LOG4CXX_ERROR(logger_,
                    "Couldn't establish "
#ifdef ENABLE_SECURITY
                        << (is_protected ? "protected" : "non-protected")
#endif  // ENABLE_SECURITY
                        << " service " << static_cast<int>(service_type)
                        << " for session " << static_cast<int>(session_id));
      protocol_handler_->NotifySessionStarted(context, rejected_params);
      return;
    }
    context.new_session_id_ = session_id;
    context.hash_id_ = protocol_handler::HASH_ID_NOT_SUPPORTED;
  }
  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_valid_ && connection_handler_observer_) {
    const uint32_t session_key =
        KeyFromPair(connection_handle, context.new_session_id_);

    uint32_t app_id = 0;
    GetDataOnSessionKey(
        session_key, &app_id, NULL, static_cast<DeviceHandle*>(NULL));
    if (app_id > 0) {
      context.is_ptu_required_ =
          !connection_handler_observer_->CheckAppIsNavi(app_id);
    }

    {
      sync_primitives::AutoLock auto_lock(start_service_context_map_lock_);
      start_service_context_map_[session_key] = context;
    }

    connection_handler_observer_->OnServiceStartedCallback(
        connection->connection_device_handle(),
        session_key,
        service_type,
        params);
  } else {
    if (protocol_handler_) {
      protocol_handler_->NotifySessionStarted(context, rejected_params);
    }
  }
}

void ConnectionHandlerImpl::NotifyServiceStartedResult(
    uint32_t session_key,
    bool result,
    std::vector<std::string>& rejected_params) {
  LOG4CXX_AUTO_TRACE(logger_);

  protocol_handler::SessionContext context;
  {
    sync_primitives::AutoLock auto_lock(start_service_context_map_lock_);
    auto it = start_service_context_map_.find(session_key);
    if (it == start_service_context_map_.end()) {
      LOG4CXX_ERROR(logger_, "context for start service not found!");
      return;
    }
    context = it->second;
    start_service_context_map_.erase(it);
  }

  Connection* connection = NULL;
  {
    sync_primitives::AutoReadLock lock(connection_list_lock_);
    ConnectionList::iterator it = connection_list_.find(context.connection_id_);
    if (connection_list_.end() == it) {
      LOG4CXX_ERROR(logger_, "connection not found");
      return;
    }
    connection = it->second;
  }

  if (!result) {
    LOG4CXX_WARN(logger_,
                 "Service starting forbidden by connection_handler_observer");
    if (protocol_handler::kRpc == context.service_type_) {
      connection->RemoveSession(context.new_session_id_);
    } else {
      connection->RemoveService(context.initial_session_id_,
                                context.service_type_);
    }
    context.new_session_id_ = 0;
  }

  if (protocol_handler_ != NULL) {
    protocol_handler_->NotifySessionStarted(context, rejected_params);
  }
}

void ConnectionHandlerImpl::OnApplicationFloodCallBack(
    const uint32_t& connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);

  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  LOG4CXX_INFO(logger_, "Disconnect flooding application");
  if (session_id != 0) {
    CloseSession(connection_handle, session_id, kFlood);
  } else {
    CloseConnectionSessions(connection_handle, kCommon);
    CloseConnection(connection_handle);
  }
}

void ConnectionHandlerImpl::OnMalformedMessageCallback(
    const uint32_t& connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);

  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  LOG4CXX_INFO(logger_, "Disconnect malformed messaging application");
  CloseConnectionSessions(connection_handle, kMalformed);
  CloseConnection(connection_handle);
}

uint32_t ConnectionHandlerImpl::OnSessionEndedCallback(
    const transport_manager::ConnectionUID connection_handle,
    const uint8_t session_id,
    const uint32_t& hashCode,
    const protocol_handler::ServiceType& service_type) {
  uint32_t hashValue = hashCode;
  return OnSessionEndedCallback(
      connection_handle, session_id, &hashValue, service_type);
}

uint32_t ConnectionHandlerImpl::OnSessionEndedCallback(
    const transport_manager::ConnectionUID connection_handle,
    const uint8_t session_id,
    uint32_t* hashCode,
    const protocol_handler::ServiceType& service_type) {
  LOG4CXX_AUTO_TRACE(logger_);

  connection_list_lock_.AcquireForReading();
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_WARN(logger_, "Unknown connection!");
    connection_list_lock_.Release();
    return 0;
  }
  std::pair<int32_t, Connection*> connection_item = *it;
  connection_list_lock_.Release();

  Connection* connection = connection_item.second;
  const uint32_t session_key = KeyFromPair(connection_handle, session_id);

  if (protocol_handler::kRpc == service_type) {
    LOG4CXX_INFO(logger_,
                 "Session " << static_cast<uint32_t>(session_id)
                            << " to be removed");
    // old version of protocol doesn't support hash
    if (protocol_handler::HASH_ID_NOT_SUPPORTED != *hashCode) {
      if (protocol_handler::HASH_ID_WRONG == *hashCode ||
          session_key != *hashCode) {
        LOG4CXX_WARN(logger_,
                     "Wrong hash_id for session "
                         << static_cast<uint32_t>(session_id));
        *hashCode = protocol_handler::HASH_ID_WRONG;
        return 0;
      }
    }
    if (!connection->RemoveSession(session_id)) {
      LOG4CXX_WARN(logger_,
                   "Couldn't remove session "
                       << static_cast<uint32_t>(session_id));
      return 0;
    }
  } else {
    LOG4CXX_INFO(logger_,
                 "Service " << static_cast<uint32_t>(service_type)
                            << " to be removed");
    if (!connection->RemoveService(session_id, service_type)) {
      LOG4CXX_WARN(logger_,
                   "Couldn't remove service "
                       << static_cast<uint32_t>(service_type));
      return 0;
    }
  }

  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_valid_ && connection_handler_observer_) {
    connection_handler_observer_->OnServiceEndedCallback(
        session_key, service_type, CloseSessionReason::kCommon);
  }
  return session_key;
}

uint32_t ConnectionHandlerImpl::KeyFromPair(
    transport_manager::ConnectionUID connection_handle,
    uint8_t session_id) const {
  const uint32_t key = connection_handle | (session_id << 16);
  LOG4CXX_DEBUG(logger_,
                "Key for ConnectionHandle:"
                    << static_cast<uint32_t>(connection_handle)
                    << " Session:" << static_cast<uint32_t>(session_id)
                    << " is: " << static_cast<uint32_t>(key));
  if (protocol_handler::HASH_ID_WRONG == key) {
    LOG4CXX_ERROR(logger_,
                  "Connection key is WRONG_HASH_ID "
                  "(session id shall be greater 0)");
  }
  return key;
}

void ConnectionHandlerImpl::PairFromKey(
    uint32_t key,
    transport_manager::ConnectionUID* connection_handle,
    uint8_t* session_id) const {
  *connection_handle = key & 0xFF00FFFF;
  *session_id = key >> 16;
  LOG4CXX_DEBUG(
      logger_,
      "ConnectionHandle:" << static_cast<int32_t>(*connection_handle)
                          << " Session:" << static_cast<int32_t>(*session_id)
                          << " for key:" << static_cast<int32_t>(key));
}

int32_t ConnectionHandlerImpl::GetDataOnSessionKey(
    uint32_t key,
    uint32_t* app_id,
    std::list<int32_t>* sessions_list,
    connection_handler::DeviceHandle* device_id) const {
  LOG4CXX_AUTO_TRACE(logger_);

  const int32_t error_result = -1;
  transport_manager::ConnectionUID conn_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &conn_handle, &session_id);

  ConnectionList::const_iterator it = connection_list_.find(conn_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Connection not found for key: " << key);
    return error_result;
  }

  const Connection& connection = *it->second;
  const SessionMap session_map = connection.session_map();
  if (0 == session_id || session_map.end() == session_map.find(session_id)) {
    LOG4CXX_ERROR(logger_,
                  "Session not found in connection: "
                      << static_cast<int32_t>(conn_handle));
    return error_result;
  }

  if (device_id) {
    *device_id = connection.connection_device_handle();
  }
  if (app_id) {
    *app_id = KeyFromPair(conn_handle, session_id);
  }
  if (sessions_list) {
    sessions_list->clear();

    SessionMap::const_iterator session_it = session_map.begin();
    for (; session_map.end() != session_it; ++session_it) {
      sessions_list->push_back(KeyFromPair(conn_handle, it->first));
    }
  }

  LOG4CXX_INFO(logger_,
               "Connection " << static_cast<int32_t>(conn_handle) << " has "
                             << session_map.size() << " sessions.");
  return 0;
}

int32_t ConnectionHandlerImpl::GetDataOnSessionKey(
    uint32_t key,
    uint32_t* app_id,
    std::list<int32_t>* sessions_list,
    uint32_t* device_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  DeviceHandle handle;
  int32_t result = GetDataOnSessionKey(key, app_id, sessions_list, &handle);
  *device_id = static_cast<uint32_t>(handle);
  return result;
}

const ConnectionHandlerSettings& ConnectionHandlerImpl::get_settings() const {
  return settings_;
}

const protocol_handler::SessionObserver&
ConnectionHandlerImpl::get_session_observer() {
  return *this;
}

DevicesDiscoveryStarter& ConnectionHandlerImpl::get_device_discovery_starter() {
  return *this;
}

struct CompareMAC {
  explicit CompareMAC(const std::string& mac) : mac_(mac) {}
  bool operator()(const DeviceMap::value_type& device) {
    return strcasecmp(device.second.mac_address().c_str(), mac_.c_str()) == 0;
  }

 private:
  std::string mac_;
};

bool ConnectionHandlerImpl::GetDeviceID(const std::string& mac_address,
                                        DeviceHandle* device_handle) {
  DCHECK_OR_RETURN(device_handle, false);
  DeviceMap::const_iterator it = std::find_if(
      device_list_.begin(), device_list_.end(), CompareMAC(mac_address));
  if (it != device_list_.end()) {
    *device_handle = it->first;
    return true;
  }
  return false;
}

int32_t ConnectionHandlerImpl::GetDataOnDeviceID(
    DeviceHandle device_handle,
    std::string* device_name,
    std::list<uint32_t>* applications_list,
    std::string* mac_address,
    std::string* connection_type) const {
  LOG4CXX_AUTO_TRACE(logger_);

  int32_t result = -1;
  DeviceMap::const_iterator it = device_list_.find(device_handle);
  if (device_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Device not found for handle " << device_handle);
    return result;
  }

  if (device_name) {
    *device_name = it->second.user_friendly_name();
  }
  if (connection_type) {
    *connection_type = it->second.connection_type();
  }
  if (applications_list) {
    applications_list->clear();
    sync_primitives::AutoReadLock connection_list_lock(connection_list_lock_);
    for (ConnectionList::const_iterator itr = connection_list_.begin();
         itr != connection_list_.end();
         ++itr) {
      if (device_handle == (*itr).second->connection_device_handle()) {
        const SessionMap& session_map = (itr->second)->session_map();
        for (SessionMap::const_iterator session_it = session_map.begin();
             session_map.end() != session_it;
             ++session_it) {
          const transport_manager::ConnectionUID connection_handle = itr->first;
          const uint32_t session_id = session_it->first;
          const uint32_t application_id =
              KeyFromPair(connection_handle, session_id);
          applications_list->push_back(application_id);
        }
      }
    }
  }

  if (mac_address) {
    *mac_address = it->second.mac_address();
  }

  result = 0;

  return result;
}

void ConnectionHandlerImpl::GetConnectedDevicesMAC(
    std::vector<std::string>& device_macs) const {
  DeviceMap::const_iterator first = device_list_.begin();
  DeviceMap::const_iterator last = device_list_.end();

  while (first != last) {
    device_macs.push_back((*first).second.mac_address());
    ++first;
  }
}

#ifdef ENABLE_SECURITY
int ConnectionHandlerImpl::SetSSLContext(
    const uint32_t& key, security_manager::SSLContext* context) {
  LOG4CXX_AUTO_TRACE(logger_);
  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
    return security_manager::SecurityManager::ERROR_INTERNAL;
  }
  Connection& connection = *it->second;
  return connection.SetSSLContext(session_id, context);
}

security_manager::SSLContext* ConnectionHandlerImpl::GetSSLContext(
    const uint32_t& key, const protocol_handler::ServiceType& service_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
    return NULL;
  }
  Connection& connection = *it->second;
  return connection.GetSSLContext(session_id, service_type);
}

void ConnectionHandlerImpl::SetProtectionFlag(
    const uint32_t& key, const protocol_handler::ServiceType& service_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
    return;
  }
  Connection& connection = *it->second;
  connection.SetProtectionFlag(session_id, service_type);
}

security_manager::SSLContext::HandshakeContext
ConnectionHandlerImpl::GetHandshakeContext(uint32_t key) const {
  return connection_handler_observer_->GetHandshakeContext(key);
}

#endif  // ENABLE_SECURITY

bool ConnectionHandlerImpl::SessionServiceExists(
    const uint32_t connection_key,
    const protocol_handler::ServiceType& service_type) const {
  LOG4CXX_AUTO_TRACE(logger_);
  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::const_iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
    return false;
  }
  const Connection& connection = *it->second;
  return connection.SessionServiceExists(session_id, service_type);
}

void ConnectionHandlerImpl::StartDevicesDiscovery() {
  LOG4CXX_AUTO_TRACE(logger_);

  transport_manager_.SearchDevices();
  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_valid_ && connection_handler_observer_) {
    connection_handler_observer_->OnDeviceListUpdated(device_list_);
  }
}

void ConnectionHandlerImpl::ConnectToDevice(
    connection_handler::DeviceHandle device_handle) {
  connection_handler::DeviceMap::const_iterator it_in;
  it_in = device_list_.find(device_handle);
  if (device_list_.end() != it_in) {
    LOG4CXX_INFO(logger_, "Connecting to device with handle " << device_handle);
    if (transport_manager::E_SUCCESS !=
        transport_manager_.ConnectDevice(device_handle)) {
      LOG4CXX_WARN(logger_, "Can't connect to device");
    } else {
      LOG4CXX_ERROR(
          logger_,
          "Application Manager wanted to connect to non-existing device");
    }
  }
}

void ConnectionHandlerImpl::RunAppOnDevice(const std::string& device_mac,
                                           const std::string& bundle_id) const {
  for (DeviceMap::const_iterator i = device_list_.begin();
       i != device_list_.end();
       ++i) {
    const connection_handler::Device& device = i->second;
    if (device.mac_address() == device_mac) {
      transport_manager_.RunAppOnDevice(device.device_handle(), bundle_id);
      return;
    }
  }
  LOG4CXX_WARN(logger_, "No apps found on device " << device_mac);
}

void ConnectionHandlerImpl::ConnectToAllDevices() {
  for (DeviceMap::iterator i = device_list_.begin(); i != device_list_.end();
       ++i) {
    connection_handler::DeviceHandle device_handle = i->first;
    ConnectToDevice(device_handle);
  }
}

void ConnectionHandlerImpl::StartTransportManager() {
  LOG4CXX_AUTO_TRACE(logger_);
  transport_manager_.Visibility(true);
}

void ConnectionHandlerImpl::CloseRevokedConnection(uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  CloseConnection(connection_handle);
}

void ConnectionHandlerImpl::CloseConnection(
    ConnectionHandle connection_handle) {
  LOG4CXX_AUTO_TRACE(logger_);
  transport_manager::ConnectionUID connection_uid =
      ConnectionUIDFromHandle(connection_handle);
  transport_manager_.DisconnectForce(connection_uid);

  sync_primitives::AutoWriteLock connection_list_lock(connection_list_lock_);

  ConnectionList::iterator connection_list_itr =
      connection_list_.find(connection_uid);
  if (connection_list_.end() != connection_list_itr) {
    connection_list_.erase(connection_list_itr);
  }
}

uint32_t ConnectionHandlerImpl::GetConnectionSessionsCount(
    uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::iterator itr = connection_list_.find(connection_handle);

  if (connection_list_.end() != itr) {
    return itr->second->session_map().size();
  }

  return 0;
}

void ConnectionHandlerImpl::CloseSession(uint32_t key,
                                         CloseSessionReason close_reason) {
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  CloseSession(connection_handle, session_id, close_reason);
}

void ConnectionHandlerImpl::CloseSession(ConnectionHandle connection_handle,
                                         uint8_t session_id,
                                         CloseSessionReason close_reason) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "Closing session with id: " << static_cast<int>(session_id));

  // In case of malformed message the connection should be broke up without
  // any other notification to mobile.
  if (close_reason != kMalformed && protocol_handler_) {
    protocol_handler_->SendEndSession(connection_handle, session_id);
  }

  transport_manager::ConnectionUID connection_id =
      ConnectionUIDFromHandle(connection_handle);

  SessionMap session_map;
  {
    sync_primitives::AutoReadLock connection_list_lock(connection_list_lock_);

    ConnectionList::iterator connection_list_itr =
        connection_list_.find(connection_id);
    if (connection_list_.end() != connection_list_itr) {
      session_map = connection_list_itr->second->session_map();
      connection_list_itr->second->RemoveSession(session_id);
    } else {
      LOG4CXX_ERROR(logger_,
                    "Connection with id: " << connection_id << " not found");
      return;
    }
  }

  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (!connection_handler_observer_valid_ || !connection_handler_observer_) {
    LOG4CXX_ERROR(logger_, "Connection handler observer not found");
    return;
  }

  SessionMap::const_iterator session_map_itr = session_map.find(session_id);
  if (session_map_itr != session_map.end()) {
    const uint32_t session_key = KeyFromPair(connection_id, session_id);
    const Session& session = session_map_itr->second;
    const ServiceList& service_list = session.service_list;

    ServiceList::const_reverse_iterator service_list_itr =
        service_list.rbegin();
    for (; service_list_itr != service_list.rend(); ++service_list_itr) {
      const protocol_handler::ServiceType service_type =
          service_list_itr->service_type;
      connection_handler_observer_->OnServiceEndedCallback(
          session_key, service_type, close_reason);
    }
  } else {
    LOG4CXX_ERROR(logger_, "Session with id: " << session_id << " not found");
    return;
  }

  LOG4CXX_DEBUG(logger_,
                "Session with id: " << session_id
                                    << " has been closed successfully");
}

void ConnectionHandlerImpl::CloseConnectionSessions(
    ConnectionHandle connection_handle, CloseSessionReason close_reason) {
  LOG4CXX_AUTO_TRACE(logger_);

  transport_manager::ConnectionUID connection_id =
      ConnectionUIDFromHandle(connection_handle);

  LOG4CXX_DEBUG(
      logger_,
      "Closing all sessions for connection with id: " << connection_id);

  typedef std::vector<uint8_t> SessionIdVector;
  SessionIdVector session_id_vector;
  {
    sync_primitives::AutoReadLock connection_list_lock(connection_list_lock_);

    ConnectionList::iterator connection_list_itr =
        connection_list_.find(connection_id);
    if (connection_list_.end() != connection_list_itr) {
      const SessionMap session_map = connection_list_itr->second->session_map();

      SessionMap::const_iterator session_map_itr = session_map.begin();
      for (; session_map_itr != session_map.end(); ++session_map_itr) {
        session_id_vector.push_back(session_map_itr->first);
      }
    } else {
      LOG4CXX_ERROR(logger_,
                    "Connection with id: " << connection_id << " not found");
      return;
    }
  }
  SessionIdVector::const_iterator session_id_itr = session_id_vector.begin();
  for (; session_id_itr != session_id_vector.end(); ++session_id_itr) {
    CloseSession(connection_handle, *session_id_itr, close_reason);
  }
  session_id_vector.clear();

  LOG4CXX_DEBUG(logger_,
                "All sessions for connection with id: "
                    << connection_id << " have been closed successfully");
}

void ConnectionHandlerImpl::SendEndService(uint32_t key, uint8_t service_type) {
  if (protocol_handler_) {
    uint32_t connection_handle = 0;
    uint8_t session_id = 0;
    PairFromKey(key, &connection_handle, &session_id);
    protocol_handler_->SendEndService(
        connection_handle, session_id, service_type);
  }
}

void ConnectionHandlerImpl::StartSessionHeartBeat(uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() != it) {
    it->second->StartHeartBeat(session_id);
  }
}

void ConnectionHandlerImpl::SetHeartBeatTimeout(uint32_t connection_key,
                                                uint32_t timeout) {
  LOG4CXX_AUTO_TRACE(logger_);
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);
  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() != it) {
    it->second->SetHeartBeatTimeout(timeout, session_id);
  }
}

void ConnectionHandlerImpl::SendHeartBeat(ConnectionHandle connection_handle,
                                          uint8_t session_id) {
  transport_manager::ConnectionUID connection_uid =
      ConnectionUIDFromHandle(connection_handle);
  if (protocol_handler_) {
    protocol_handler_->SendHeartBeat(connection_uid, session_id);
  }
}

void ConnectionHandlerImpl::KeepConnectionAlive(uint32_t connection_key,
                                                uint8_t session_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_key);
  if (connection_list_.end() != it) {
    it->second->KeepAlive(session_id);
  }
}

void ConnectionHandlerImpl::OnConnectionEnded(
    const transport_manager::ConnectionUID connection_id) {
  LOG4CXX_INFO(logger_,
               "Delete Connection: " << static_cast<int32_t>(connection_id)
                                     << " from the list.");
  connection_list_lock_.AcquireForWriting();
  ConnectionList::iterator itr = connection_list_.find(connection_id);
  if (connection_list_.end() == itr) {
    LOG4CXX_ERROR(logger_, "Connection not found!");
    connection_list_lock_.Release();
    return;
  }
  std::auto_ptr<Connection> connection(itr->second);
  connection_list_.erase(itr);
  connection_list_lock_.Release();

  sync_primitives::AutoReadLock read_lock(connection_handler_observer_lock_);
  if (connection_handler_observer_valid_ && connection_handler_observer_ && connection.get() != NULL) {
    const SessionMap session_map = connection->session_map();

    for (SessionMap::const_iterator session_it = session_map.begin();
         session_map.end() != session_it;
         ++session_it) {
      const uint32_t session_key =
          KeyFromPair(connection_id, session_it->first);
      const ServiceList& service_list = session_it->second.service_list;
      for (ServiceList::const_iterator service_it = service_list.begin(),
                                       end = service_list.end();
           service_it != end;
           ++service_it) {
        connection_handler_observer_->OnServiceEndedCallback(
            session_key, service_it->service_type, CloseSessionReason::kCommon);
      }
    }
  }
}

void ConnectionHandlerImpl::BindProtocolVersionWithSession(
    uint32_t connection_key, uint8_t protocol_version) {
  LOG4CXX_AUTO_TRACE(logger_);
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() != it) {
    it->second->UpdateProtocolVersionSession(session_id, protocol_version);
  }
}

bool ConnectionHandlerImpl::IsHeartBeatSupported(
    transport_manager::ConnectionUID connection_handle,
    uint8_t session_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoReadLock lock(connection_list_lock_);
  uint32_t connection = static_cast<uint32_t>(connection_handle);
  ConnectionList::const_iterator it = connection_list_.find(connection);
  if (connection_list_.end() == it) {
    LOG4CXX_WARN(logger_, "Connection not found !");
    return false;
  }
  return it->second->SupportHeartBeat(session_id);
}

bool ConnectionHandlerImpl::ProtocolVersionUsed(
    uint32_t connection_id,
    uint8_t session_id,
    uint8_t& protocol_version) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoReadLock lock(connection_list_lock_);
  ConnectionList::const_iterator it = connection_list_.find(connection_id);
  if (connection_list_.end() != it) {
    return it->second->ProtocolVersion(session_id, protocol_version);
  }
  LOG4CXX_WARN(logger_, "Connection not found !");
  return false;
}

#ifdef BUILD_TESTS
ConnectionList& ConnectionHandlerImpl::getConnectionList() {
  return connection_list_;
}

const DeviceMap& ConnectionHandlerImpl::getDeviceList() {
  return device_list_;
}

void ConnectionHandlerImpl::addDeviceConnection(
    const transport_manager::DeviceInfo& device_info,
    const transport_manager::ConnectionUID connection_id) {
  // Add Device
  OnDeviceAdded(device_info);
  // Add connection
  OnConnectionEstablished(device_info, connection_id);
}
#endif  // BUILD_TESTS
}  // namespace connection_handler
