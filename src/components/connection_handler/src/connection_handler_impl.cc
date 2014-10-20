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

#include "connection_handler/connection_handler_impl.h"
#include "transport_manager/info.h"
#include "config_profile/profile.h"

#ifdef ENABLE_SECURITY
#include "security_manager/security_manager.h"
#endif  // ENABLE_SECURITY

namespace {
int32_t HeartBeatTimeout() {
  return profile::Profile::instance()->heart_beat_timeout();
}
}  // namespace

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

ConnectionHandlerImpl::ConnectionHandlerImpl()
  : connection_handler_observer_(NULL),
    transport_manager_(NULL),
    protocol_handler_(NULL),
    connection_list_deleter_(&connection_list_) {
}

ConnectionHandlerImpl::~ConnectionHandlerImpl() {
  LOG4CXX_TRACE(logger_, "Destructing ConnectionHandlerImpl.");
}

void ConnectionHandlerImpl::Stop() {
  LOG4CXX_TRACE_ENTER(logger_);
  ConnectionList::iterator itr = connection_list_.begin();
  while (itr != connection_list_.end()) {
    RemoveConnection(itr->second->connection_handle());
    itr = connection_list_.begin();
  }
  LOG4CXX_TRACE_EXIT(logger_);
}

void ConnectionHandlerImpl::set_connection_handler_observer(
    ConnectionHandlerObserver *observer) {
  LOG4CXX_DEBUG(logger_, "ConnectionHandlerImpl::set_connection_handler_observer() "
                << observer);
  if (!observer) {
    LOG4CXX_WARN(logger_, "Set Null pointer to observer.");
  }
  connection_handler_observer_ = observer;
}

void ConnectionHandlerImpl::set_transport_manager(
    transport_manager::TransportManager *transport_mngr) {
  LOG4CXX_DEBUG(logger_, "ConnectionHandlerImpl::set_transport_manager() "
                << transport_mngr);
  if (!transport_mngr) {
    LOG4CXX_ERROR(logger_, "Null pointer to TransportManager.");
    return;
  }
  transport_manager_ = transport_mngr;
}

void ConnectionHandlerImpl::set_protocol_handler(
    protocol_handler::ProtocolHandler *protocol_handler) {
  LOG4CXX_DEBUG(logger_, "ConnectionHandlerImpl::set_protocol_handler()"
                << protocol_handler);
  if (!protocol_handler) {
    LOG4CXX_WARN(logger_, "Set Null pointer to protocol handler.");
  }
  protocol_handler_ = protocol_handler;
}

void ConnectionHandlerImpl::OnDeviceListUpdated(
    const std::vector<transport_manager::DeviceInfo>&) {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::OnDeviceListUpdated()");
  if (connection_handler_observer_) {
    connection_handler_observer_->OnDeviceListUpdated(device_list_);
  }
}

void ConnectionHandlerImpl::OnFindNewApplicationsRequest() {
  if (connection_handler_observer_) {
    connection_handler_observer_->OnFindNewApplicationsRequest();
  }
}

void ConnectionHandlerImpl::OnDeviceFound(
    const transport_manager::DeviceInfo&) {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::OnDeviceFound()");
}

void ConnectionHandlerImpl::OnDeviceAdded(
    const transport_manager::DeviceInfo &device_info) {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::OnDeviceAdded()");
  device_list_.insert(
        DeviceMap::value_type(
          device_info.device_handle(),
          Device(device_info.device_handle(), device_info.name(),
                 device_info.mac_address(), device_info.connection_type())));
  if (connection_handler_observer_) {
    connection_handler_observer_->OnDeviceListUpdated(device_list_);
  }
}

void ConnectionHandlerImpl::OnDeviceRemoved(
    const transport_manager::DeviceInfo &device_info) {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::OnDeviceRemoved()");
  // Device has been removed. Perform all needed actions.
  // 1. Delete all the connections and sessions of this device
  // 2. Delete device from a list
  // 3. Let observer know that device has been deleted.

  std::vector<ConnectionHandle> connections_to_remove;
  {
    sync_primitives::AutoLock lock(connection_list_lock_);
    for (ConnectionList::iterator it = connection_list_.begin();
         it != connection_list_.end(); ++it) {
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

  device_list_.erase(device_info.device_handle());
  if (connection_handler_observer_) {
    connection_handler_observer_->RemoveDevice(device_info.device_handle());
    connection_handler_observer_->OnDeviceListUpdated(device_list_);
  }
}

void ConnectionHandlerImpl::OnScanDevicesFinished() {
  LOG4CXX_TRACE(logger_, "Scan devices finished successfully.");
}

void ConnectionHandlerImpl::OnScanDevicesFailed(
    const transport_manager::SearchDeviceError &error) {
  LOG4CXX_WARN(logger_, "Scan devices failed. " << error.text());
}

void ConnectionHandlerImpl::OnConnectionEstablished(
    const transport_manager::DeviceInfo &device_info,
    const transport_manager::ConnectionUID &connection_id) {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::OnConnectionEstablished()");

  DeviceMap::iterator it = device_list_.find(device_info.device_handle());
  if (device_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown device!");
    return;
  }
  LOG4CXX_INFO(logger_, "Add Connection:" << connection_id << " to the list.");
  sync_primitives::AutoLock lock(connection_list_lock_);
  connection_list_.insert(
      ConnectionList::value_type(
          connection_id,
          new Connection(connection_id, device_info.device_handle(), this,
                         HeartBeatTimeout())));
}

void ConnectionHandlerImpl::OnConnectionFailed(
    const transport_manager::DeviceInfo &device_info,
    const transport_manager::ConnectError &error) {
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_, "Failed connecting.");
}

void ConnectionHandlerImpl::OnConnectionClosed(
    transport_manager::ConnectionUID connection_id) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::OnConnectionClosed");

  OnConnectionEnded(connection_id);
}

void ConnectionHandlerImpl::OnConnectionClosedFailure(
    transport_manager::ConnectionUID connection_id,
    const transport_manager::DisconnectError &error) {
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_, "ConnectionHandlerImpl::OnConnectionClosedFailure");
}

void ConnectionHandlerImpl::OnUnexpectedDisconnect(
    transport_manager::ConnectionUID connection_id,
    const transport_manager::CommunicationError &error) {
  LOG4CXX_ERROR(logger_, "ConnectionHandlerImpl::OnUnexpectedDisconnect");

  OnConnectionEnded(connection_id);
}

void ConnectionHandlerImpl::OnDeviceConnectionLost(
    const transport_manager::DeviceHandle &device,
    const transport_manager::DisconnectDeviceError &error) {
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_, "Lost connection with device " << device);
}

void ConnectionHandlerImpl::OnDisconnectFailed(
    const transport_manager::DeviceHandle &device,
    const transport_manager::DisconnectDeviceError &error) {
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_, "Trying to Disconnect device failed.");
}

void ConnectionHandlerImpl::RemoveConnection(
    const ConnectionHandle connection_handle) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::RemoveConnection()");

  OnConnectionEnded(connection_handle);
}

#ifdef ENABLE_SECURITY
namespace {
bool AllowProtection(const protocol_handler::ServiceType &service_type,
                          const bool is_protected) {
  if (is_protected) {
    // Check deliver-specific services (which shall not be protected)
    const std::list<int> force_unprotected_list =
        profile::Profile::instance()->ReadIntContainer(
          "Security Manager", "ForceUnprotectedService", NULL);
    if (std::find(force_unprotected_list.begin(), force_unprotected_list.end(), service_type) !=
        force_unprotected_list.end()) {
      LOG4CXX_ERROR(logger_, "Service " << static_cast<int>(service_type)
                    << " is forbidden to be protected");
      return false;
    }
  } else {
    // Check deliver-specific services (which shall be protected)
    const std::list<int> force_protected_list =
        profile::Profile::instance()->ReadIntContainer(
          "Security Manager", "ForceProtectedService", NULL);
    if (std::find(force_protected_list.begin(), force_protected_list.end(), service_type) !=
        force_protected_list.end()) {
      LOG4CXX_ERROR(logger_, "Service " << static_cast<int>(service_type)
                    << " shall be protected");
      return false;
    }
  }
  return true;
}
}  // namespace
#endif  // ENABLE_SECURITY

uint32_t ConnectionHandlerImpl::OnSessionStartedCallback(
    const transport_manager::ConnectionUID &connection_handle,
    const uint8_t session_id, const protocol_handler::ServiceType &service_type,
    const bool is_protected, uint32_t* hash_id) {
  LOG4CXX_TRACE_ENTER(logger_);

  if (hash_id) {
    *hash_id = protocol_handler::HASH_ID_WRONG;
  }
#ifdef ENABLE_SECURITY
  if (!AllowProtection(service_type, is_protected)) {
    return 0;
  }
#endif  // ENABLE_SECURITY

  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
    LOG4CXX_TRACE_EXIT(logger_);
    return 0;
  }
  uint32_t new_session_id = 0;

  Connection *connection = it->second;
  if ((0 == session_id) && (protocol_handler::kRpc == service_type)) {
    new_session_id = connection->AddNewSession();
    if (0 == new_session_id) {
      LOG4CXX_ERROR(logger_, "Couldn't start new session!");
      LOG4CXX_TRACE_EXIT(logger_);
      return 0;
    }
    if (hash_id) {
      *hash_id = KeyFromPair(connection_handle, new_session_id);
    }
  } else {  // Could be create new service or protected exists one
    if (!connection->AddNewService(session_id, service_type, is_protected)) {
      LOG4CXX_ERROR(logger_, "Couldn't establish "
#ifdef ENABLE_SECURITY
                    << (is_protected ? "protected" : "non-protected")
#endif  // ENABLE_SECURITY
                    << " service " << static_cast<int>(service_type)
                    << " for session " << static_cast<int>(session_id));
      LOG4CXX_TRACE_EXIT(logger_);
      return 0;
    }
    new_session_id = session_id;
    if (hash_id) {
      *hash_id = protocol_handler::HASH_ID_NOT_SUPPORTED;
    }
  }

  if (connection_handler_observer_) {
    const uint32_t session_key = KeyFromPair(connection_handle, new_session_id);
    const bool success = connection_handler_observer_->OnServiceStartedCallback(
        connection->connection_device_handle(), session_key, service_type);
    if (!success) {
      if (protocol_handler::kRpc == service_type) {
        connection->RemoveSession(new_session_id);
      } else {
        connection->RemoveService(session_id, service_type);
      }
      LOG4CXX_TRACE_EXIT(logger_);
      return 0;
    }
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return new_session_id;
}

uint32_t ConnectionHandlerImpl::OnSessionEndedCallback(
    const uint32_t &connection_handle, const uint8_t session_id,
    const uint32_t &hashCode,
    const protocol_handler::ServiceType &service_type) {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::OnSessionEndedCallback()");

  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_WARN(logger_, "Unknown connection!");
    return 0;
  }
  Connection *connection = it->second;
  const uint32_t session_key = KeyFromPair(connection_handle, session_id);

  if (protocol_handler::kRpc == service_type) {
    LOG4CXX_INFO(logger_, "Session "  << static_cast<uint32_t>(session_id)
                 << " to be removed");
    // old version of protocol doesn't support hash
    if (protocol_handler::HASH_ID_NOT_SUPPORTED != hashCode) {
      if (protocol_handler::HASH_ID_WRONG == hashCode ||
          session_key != hashCode) {
        LOG4CXX_WARN(logger_, "Wrong hash_id for session "
                     << static_cast<uint32_t>(session_id));
        return 0;
      }
    }
    if (!connection->RemoveSession(session_id)) {
      LOG4CXX_WARN(logger_, "Couldn't remove session "
                   << static_cast<uint32_t>(session_id));
      return 0;
    }
  } else {
    LOG4CXX_INFO(logger_, "Service "  << static_cast<uint32_t>(service_type)
                 << " to be removed");
    if (!connection->RemoveService(session_id, service_type)) {
      LOG4CXX_WARN(logger_, "Couldn't remove service "
                   << static_cast<uint32_t>(service_type));
      return 0;
    }
  }

  if (connection_handler_observer_) {
    connection_handler_observer_->OnServiceEndedCallback(session_key,
                                                         service_type);
  }
  return session_key;
}

uint32_t ConnectionHandlerImpl::KeyFromPair(
    transport_manager::ConnectionUID connection_handle, uint8_t session_id) {
  const uint32_t key = connection_handle | (session_id << 16);
  LOG4CXX_TRACE(logger_, "Key for ConnectionHandle:"
                << static_cast<uint32_t>(connection_handle)
                << " Session:" << static_cast<uint32_t>(session_id)
                << " is: " << static_cast<uint32_t>(key));
  if (protocol_handler::HASH_ID_WRONG == key) {
    LOG4CXX_ERROR(logger_, "Connection key is WRONG_HASH_ID "
                  "(session id shall be greater 0)");
  }
  return key;
}

void ConnectionHandlerImpl::PairFromKey(uint32_t key,
                                        uint32_t *connection_handle,
                                        uint8_t *session_id) {
  *connection_handle = key & 0xFF00FFFF;
  *session_id = key >> 16;
  LOG4CXX_TRACE(logger_, "ConnectionHandle:"
                << static_cast<int32_t>(*connection_handle)
                << " Session:" << static_cast<int32_t>(*session_id)
                << " for key:" << static_cast<int32_t>(key));
}

int32_t ConnectionHandlerImpl::GetDataOnSessionKey(
    uint32_t key, uint32_t *app_id, std::list<int32_t> *sessions_list,
    uint32_t *device_id) {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::GetDataOnSessionKey");

  int32_t result = -1;
  transport_manager::ConnectionUID conn_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &conn_handle, &session_id);
  ConnectionList::iterator it = connection_list_.find(conn_handle);

  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
    return result;
  }

  Connection &connection = *it->second;
  if (device_id) {
    *device_id = connection.connection_device_handle();
  }

  if (sessions_list) {
    sessions_list->clear();
  }

  if (0 == session_id) {
    LOG4CXX_WARN(
        logger_,
        "No sessions in connection " << static_cast<int32_t>(conn_handle));
    if (app_id) {
      *app_id = 0;
    }
  } else {
    if (app_id) {
      *app_id = KeyFromPair(conn_handle, session_id);
    }

    LOG4CXX_INFO(logger_, "Connection "
                 << static_cast<int32_t>(conn_handle)
                 << " has " << connection.session_map().size()
                 << " sessions.");

    if (sessions_list) {
      const SessionMap session_map = connection.session_map();
      for (SessionMap::const_iterator session_it = session_map.begin();
           session_map.end() != session_it; ++session_it) {
        sessions_list->push_back(KeyFromPair(conn_handle, it->first));
      }
    }

    result = 0;
  }

  return result;
}

struct CompareMAC {
  explicit CompareMAC(const std::string &mac) : mac_(mac) {}
  bool operator() (const DeviceMap::value_type &device) {
    return strcasecmp(device.second.mac_address().c_str(), mac_.c_str()) == 0;
  }
 private:
  std::string mac_;
};

bool ConnectionHandlerImpl::GetDeviceID(const std::string &mac_address,
                                        DeviceHandle *device_handle) {
  DeviceMap::const_iterator it = std::find_if(device_list_.begin(),
                                               device_list_.end(),
                                               CompareMAC(mac_address));
  if (it != device_list_.end()) {
    if (device_handle) {
      *device_handle = it->first;
    }
    return true;
  }
  return false;
}

int32_t ConnectionHandlerImpl::GetDataOnDeviceID(
    DeviceHandle device_handle,
    std::string *device_name,
    std::list<uint32_t> *applications_list, std::string *mac_address,
    std::string* connection_type ) {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::GetDataOnDeviceID");

  int32_t result = -1;
  DeviceMap::iterator it = device_list_.find(device_handle);
  if (device_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Device not found!");
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
    sync_primitives::AutoLock connection_list_lock(connection_list_lock_);
    for (ConnectionList::iterator itr = connection_list_.begin();
        itr != connection_list_.end(); ++itr) {
      if (device_handle == (*itr).second->connection_device_handle()) {
        const SessionMap &session_map = (itr->second)->session_map();
        for (SessionMap::const_iterator session_it = session_map.begin();
             session_map.end() != session_it; ++session_it) {
          const transport_manager::ConnectionUID &connection_handle = itr->first;
          const uint32_t session_id = session_it->first;
          const uint32_t application_id = KeyFromPair(connection_handle, session_id);
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
#ifdef ENABLE_SECURITY
int ConnectionHandlerImpl::SetSSLContext(
    const uint32_t &key, security_manager::SSLContext *context) {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::SetSSLContext");
  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
    return security_manager::SecurityManager::ERROR_INTERNAL;
  }
  Connection &connection = *it->second;
  return connection.SetSSLContext(session_id, context);
}

security_manager::SSLContext *ConnectionHandlerImpl::GetSSLContext(
    const uint32_t &key, const protocol_handler::ServiceType &service_type) {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::GetSSLContext");
  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
    return NULL;
  }
  Connection &connection = *it->second;
  return connection.GetSSLContext(session_id, service_type);
}

void ConnectionHandlerImpl::SetProtectionFlag(
    const uint32_t &key, const protocol_handler::ServiceType &service_type) {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::SetProtectionFlag");
  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
    return;
  }
  Connection &connection = *it->second;
  connection.SetProtectionFlag(session_id, service_type);
}
#endif  // ENABLE_SECURITY

void ConnectionHandlerImpl::StartDevicesDiscovery() {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::StartDevicesDiscovery()");

  if (NULL == transport_manager_) {
    LOG4CXX_ERROR(logger_, "Null pointer to TransportManager.");
    return;
  }
  transport_manager_->SearchDevices();
  if (connection_handler_observer_) {
    connection_handler_observer_->OnDeviceListUpdated(device_list_);
  }
}

void ConnectionHandlerImpl::ConnectToDevice(
    connection_handler::DeviceHandle device_handle) {
  connection_handler::DeviceMap::const_iterator it_in;
  it_in = device_list_.find(device_handle);
  if (device_list_.end() != it_in) {
    LOG4CXX_INFO_EXT(logger_,
                     "Connecting to device with handle " << device_handle);
    if (transport_manager_) {
      if (transport_manager::E_SUCCESS
          != transport_manager_->ConnectDevice(device_handle)) {
        LOG4CXX_WARN(logger_, "Can't connect to device");
      }
    } else {
      LOG4CXX_ERROR(logger_, "Null pointer to TransportManager.");
    }
  } else {
    LOG4CXX_ERROR(
        logger_, "Application Manager wanted to connect to non-existing device");
  }
}

void ConnectionHandlerImpl::ConnectToAllDevices() {
  for (DeviceMap::iterator i = device_list_.begin(); i != device_list_.end(); ++i) {
    connection_handler::DeviceHandle device_handle = i->first;
    ConnectToDevice(device_handle);
  }
}

void ConnectionHandlerImpl::StartTransportManager() {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::StartTransportManager()");
  if (NULL == transport_manager_) {
    LOG4CXX_ERROR(logger_, "Null pointer to TransportManager.");
    return;
  }

  transport_manager_->Visibility(true);
}

void ConnectionHandlerImpl::CloseRevokedConnection(uint32_t connection_key) {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::CloseRevokedConnection");

  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  CloseConnection(connection_handle);
}

void ConnectionHandlerImpl::CloseConnection(
    ConnectionHandle connection_handle) {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::CloseConnection");
  if (!transport_manager_) {
    LOG4CXX_ERROR(logger_, "Null pointer to TransportManager.");
    return;
  }
  transport_manager::ConnectionUID connection_uid =
      ConnectionUIDFromHandle(connection_handle);
  transport_manager_->DisconnectForce(connection_uid);
}

uint32_t ConnectionHandlerImpl::GetConnectionSessionsCount(
    uint32_t connection_key) {
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionList::iterator itr = connection_list_.find(connection_handle);

  if (connection_list_.end() != itr) {
    return itr->second->session_map().size();
  }

  return 0;
}

void ConnectionHandlerImpl::CloseSession(uint32_t key) {
  LOG4CXX_TRACE(logger_, "ConnectionHandlerImpl::CloseSession");

  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  CloseSession(connection_handle, session_id);
}

void ConnectionHandlerImpl::CloseSession(ConnectionHandle connection_handle,
                                         uint8_t session_id) {
  if (protocol_handler_) {
    protocol_handler_->SendEndSession(connection_handle, session_id);
  }

  transport_manager::ConnectionUID connection_id =
        ConnectionUIDFromHandle(connection_handle);

  sync_primitives::AutoLock connection_list_lock(connection_list_lock_);
  ConnectionList::iterator itr = connection_list_.find(connection_id);

  if (connection_list_.end() != itr) {
    if (connection_handler_observer_) {
      SessionMap session_map = itr->second->session_map();
      SessionMap::iterator session_it = session_map.find(session_id);
      if (session_it != session_map.end()) {
        const Session &session = session_it->second;
        const ServiceList &service_list = session.service_list;
        ServiceList::const_iterator it = service_list.begin();
        for (;it != service_list.end(); ++it) {
          const uint32_t session_key = KeyFromPair(connection_id, session_id);
          const protocol_handler::ServiceType service_type = it->service_type;
          connection_handler_observer_->OnServiceEndedCallback(session_key,
                                                               service_type);
        }
      }
    }
    itr->second->RemoveSession(session_id);
  }
}

void ConnectionHandlerImpl::StartSessionHeartBeat(uint32_t connection_key) {
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() != it) {
    it->second->StartHeartBeat(session_id);
  }
}

void ConnectionHandlerImpl::SetHeartBeatTimeout(uint32_t connection_key,
                                                int32_t timeout) {
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);
  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() != it) {
    it->second->SetHeartBeatTimeout(timeout);
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
  sync_primitives::AutoLock lock(connection_list_lock_);

  ConnectionList::iterator it = connection_list_.find(connection_key);
  if (connection_list_.end() != it) {
    it->second->KeepAlive(session_id);
  }
}

void ConnectionHandlerImpl::OnConnectionEnded(
    const transport_manager::ConnectionUID &connection_id) {
  LOG4CXX_INFO(logger_, "Delete Connection: " << static_cast<int32_t>(connection_id)
               << " from the list.");

  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionList::iterator itr = connection_list_.find(connection_id);
  if (connection_list_.end() == itr) {
    LOG4CXX_ERROR(logger_, "Connection not found!");
    return;
  }

  if (connection_handler_observer_) {
    const Connection *connection = itr->second;
    const SessionMap session_map = connection->session_map();

    for (SessionMap::const_iterator session_it = session_map.begin();
         session_map.end() != session_it; ++session_it) {
      const uint32_t session_key = KeyFromPair(connection_id, session_it->first);
      const ServiceList &service_list = session_it->second.service_list;
      for (ServiceList::const_iterator service_it = service_list.begin(), end =
           service_list.end(); service_it != end; ++service_it) {
        connection_handler_observer_->OnServiceEndedCallback(
              session_key, service_it->service_type);
      }
    }
  }
  delete itr->second;
  connection_list_.erase(itr);
}

void ConnectionHandlerImpl::BindProtocolVersionWithSession(
    uint32_t connection_key, uint8_t protocol_version) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::BindProtocolVersionWithSession()");
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionList::iterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() != it) {
    it->second->UpdateProtocolVersionSession(session_id, protocol_version);
  }
}

bool ConnectionHandlerImpl::IsHeartBeatSupported(
    transport_manager::ConnectionUID connection_handle,uint8_t session_id) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::IsHeartBeatSupported()");
  sync_primitives::AutoLock lock(connection_list_lock_);
  uint32_t connection = static_cast<uint32_t>(connection_handle);
  ConnectionList::iterator it = connection_list_.find(connection);
  if (connection_list_.end() == it) {
    LOG4CXX_WARN(logger_, "Connection not found !");
    return false;
  }
  return it->second->SupportHeartBeat(session_id);
}

#ifdef BUILD_TESTS
ConnectionList &ConnectionHandlerImpl::getConnectionList() {
  return connection_list_;
}

void ConnectionHandlerImpl::addDeviceConnection(
    const transport_manager::DeviceInfo &device_info,
    const transport_manager::ConnectionUID &connection_id) {
  // Add Device
  OnDeviceAdded(device_info);
  // Add connection
  OnConnectionEstablished(device_info, connection_id);
}
#endif  // BUILD_TESTS
}  // namespace connection_handler
