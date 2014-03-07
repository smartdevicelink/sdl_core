/**
 * \file ConnectionHandlerImpl.cpp
 * \brief Connection handler class.
 * \Observes TransportManager and ProtocolHandler, stores information regarding connections
 * \and sessions and provides it to AppManager.
 *
 * Copyright (c) 2013, Ford Motor Company
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
#include <string>
#include <list>
#include <algorithm>

#include "connection_handler/connection_handler_impl.h"
#include "transport_manager/info.h"
#include "config_profile/profile.h"
#include "security_manager/security_query.h"

namespace {
int32_t HeartBeatTimeout() {
  return profile::Profile::instance()->heart_beat_timeout();
}
}

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace connection_handler {

ConnectionHandle HandleFromConnectionUID(transport_manager::ConnectionUID uid) {
  return ConnectionHandle(uid);
}

transport_manager::ConnectionUID ConnectionUIDFromHandle(ConnectionHandle handle) {
  return transport_manager::ConnectionUID(handle);
}

log4cxx::LoggerPtr ConnectionHandlerImpl::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("ConnectionHandler"));

ConnectionHandlerImpl::ConnectionHandlerImpl()
  : connection_handler_observer_(NULL),
    transport_manager_(NULL),
    connection_list_deleter_(&connection_list_){
}

ConnectionHandlerImpl::~ConnectionHandlerImpl() {
  LOG4CXX_INFO(logger_, "Destructing ConnectionHandlerImpl.");
}

void ConnectionHandlerImpl::set_connection_handler_observer(
  ConnectionHandlerObserver* observer) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::set_connection_handler_observer()");
  if (!observer) {
    LOG4CXX_ERROR(logger_, "Null pointer to observer.");
    return;
  }
  connection_handler_observer_ = observer;
}

void ConnectionHandlerImpl::OnDeviceListUpdated(
  const std::vector<transport_manager::DeviceInfo>& device_info_list) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::OnDeviceListUpdated()");

  bool list_actually_changed = false;
  for (DeviceListIterator itr = device_list_.begin(); itr != device_list_.end();
       ++itr) {
    if (!DoesDeviceExistInTMList(device_info_list, (*itr).first)) {
      // Device has been removed. Perform all needed actions.
      // 1. Delete all the connections and sessions of this device
      // 2. Delete device from a list
      // 3. Let observer know that device has been deleted.
      DeviceHandle device_for_remove_handle = (*itr).first;
      for (ConnectionListIterator it = connection_list_.begin();
           it != connection_list_.end(); ++it) {
        if (device_for_remove_handle
            == (*it).second->connection_device_handle()) {
          RemoveConnection((*it).first);
        }
      }
      device_list_.erase(device_for_remove_handle);
      list_actually_changed = true;
      if (connection_handler_observer_) {
        connection_handler_observer_->RemoveDevice(device_for_remove_handle);
      }
    }
  }
  for (std::vector<transport_manager::DeviceInfo>::const_iterator it_in =
         device_info_list.begin(); it_in != device_info_list.end(); ++it_in) {
    list_actually_changed |= AddDeviceInDeviceListIfNotExist((*it_in));
  }
  if (connection_handler_observer_ && list_actually_changed) {
    connection_handler_observer_->OnDeviceListUpdated(device_list_);
  }
}

void ConnectionHandlerImpl::OnDeviceFound(
  const transport_manager::DeviceInfo& device_info) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::OnDeviceFound()");
}

void ConnectionHandlerImpl::OnDeviceAdded(
  const transport_manager::DeviceInfo& device_info) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::OnDeviceAdded()");
}

void ConnectionHandlerImpl::OnDeviceRemoved(
  const transport_manager::DeviceInfo& device_info) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::OnDeviceRemoved()");
}

bool ConnectionHandlerImpl::DoesDeviceExistInTMList(
  const std::vector<transport_manager::DeviceInfo>& device_list,
  const connection_handler::DeviceHandle device_handle) {
  bool result = false;
  for (std::vector<transport_manager::DeviceInfo>::const_iterator it_in =
         device_list.begin();
       it_in != device_list.end();
       ++it_in) {
    if (it_in->device_handle() == device_handle) {
      result = true;
      break;
    }
  }
  return result;
}

bool ConnectionHandlerImpl::AddDeviceInDeviceListIfNotExist(
  const transport_manager::DeviceInfo& device_info) {
  DeviceListIterator it = device_list_.find(device_info.device_handle());
  if (device_list_.end() == it) {
    LOG4CXX_INFO(logger_, "Adding new device!");
    device_list_.insert(
      DeviceList::value_type(
        device_info.device_handle(),
        Device(device_info.device_handle(), device_info.name(),
               device_info.mac_address())));
    return true;
  }
  return false;
}

void ConnectionHandlerImpl::OnScanDevicesFinished() {
  LOG4CXX_INFO(logger_, "Scan devices finished successfully.");
}

void ConnectionHandlerImpl::OnScanDevicesFailed(
  const transport_manager::SearchDeviceError& error) {
  LOG4CXX_ERROR(logger_, "Failed to Scan devices.");
}

void ConnectionHandlerImpl::OnConnectionEstablished(
  const transport_manager::DeviceInfo& device_info,
  const transport_manager::ConnectionUID& connection_id) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::OnConnectionEstablished()");

  DeviceListIterator it = device_list_.find(device_info.device_handle());
  if (device_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown device!");
    return;
  }
  LOG4CXX_INFO(logger_, "Add Connection:" << connection_id << " to the list.");
  sync_primitives::AutoLock lock(connection_list_lock_);
  connection_list_.insert(
      ConnectionList::value_type(
          connection_id,
          new Connection(connection_id, device_info.device_handle(),
                         this, HeartBeatTimeout())));
}

void ConnectionHandlerImpl::OnConnectionFailed(
  const transport_manager::DeviceInfo& device_info,
  const transport_manager::ConnectError& error) {
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
  const transport_manager::DisconnectError& error) {
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_, "ConnectionHandlerImpl::OnConnectionClosedFailure");
}

void ConnectionHandlerImpl::OnUnexpectedDisconnect(
    transport_manager::ConnectionUID connection_id,
    const transport_manager::CommunicationError& error) {
  LOG4CXX_ERROR(logger_, "ConnectionHandlerImpl::OnUnexpectedDisconnect");
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
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::RemoveConnection()");

  OnConnectionEnded(connection_handle);
}

 int32_t ConnectionHandlerImpl::OnSessionStartedCallback(
  const transport_manager::ConnectionUID& connection_handle,
  const uint8_t& sessionId,
  const protocol_handler::ServiceType& service_type) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::OnSessionStartedCallback()");

  int32_t new_session_id = -1;

  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionListIterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
    return -1;
  }

  if ((0 == sessionId) && (protocol_handler::kRpc == service_type)) {
    new_session_id = (it->second)->AddNewSession();
    if (0 > new_session_id) {
      LOG4CXX_ERROR(logger_, "Not possible to start session!");
      return -1;
    }
  } else if ((0 != sessionId) && (protocol_handler::kRpc != service_type)) {
    if (!(it->second)->AddNewService(sessionId, service_type)) {
      LOG4CXX_ERROR(logger_, "Not possible to establish service!");
      return -1;
    }
    new_session_id = sessionId;
    if(protocol_handler::kSecure != service_type)
      {
      }
  } else {
    LOG4CXX_ERROR(logger_, "Not possible to establish service!");
    return -1;
  }

  if (connection_handler_observer_) {
    int32_t session_key = KeyFromPair(connection_handle, new_session_id);

    bool success = connection_handler_observer_->OnServiceStartedCallback(
        (it->second)->connection_device_handle(), session_key, service_type);

    if (!success && (protocol_handler::kRpc == service_type)) {
      (it->second)->RemoveSession(new_session_id);
      new_session_id = -1;
    } else if (!success) {
      (it->second)->RemoveService(sessionId, service_type);
      new_session_id = -1;
    }
  }

  return new_session_id;
}

uint32_t ConnectionHandlerImpl::OnSessionEndedCallback(
  const uint32_t& connection_handle, const uint8_t& sessionId,
  const uint32_t& hashCode,
  const protocol_handler::ServiceType& service_type) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::OnSessionEndedCallback()");

  int32_t result = -1;
  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionListIterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
    return result;
  }

  if (protocol_handler::kRpc == service_type) {
    LOG4CXX_INFO(logger_, "Session to be removed");

    result = (it->second)->RemoveSession(sessionId);
    if (0 > result) {
      LOG4CXX_ERROR(logger_, "Not possible to remove session!");
      return result;
    }
  } else {
    if (!(it->second)->RemoveService(sessionId, service_type)) {
      LOG4CXX_ERROR(logger_, "Not possible to remove service!");
      return result;
    }
    result = sessionId;
  }

  if (0 != connection_handler_observer_) {
    int32_t sessionKey = KeyFromPair(connection_handle, sessionId);
    connection_handler_observer_->OnServiceEndedCallback(sessionKey,
                                                         service_type);
    result = sessionKey;
  }

  return result;
}

uint32_t ConnectionHandlerImpl::KeyFromPair(
  transport_manager::ConnectionUID connection_handle,
  uint8_t sessionId) {
  int32_t key = connection_handle | (sessionId << 16);
  LOG4CXX_INFO(
    logger_,
    "Key for ConnectionHandle:" << static_cast<int32_t>(connection_handle)
    << " Session:" << static_cast<int32_t>(sessionId) << " is: "
    << static_cast<int32_t>(key));
  return key;
}

void ConnectionHandlerImpl::PairFromKey(uint32_t key,
                                        uint32_t* connection_handle,
                                        uint8_t* sessionId) {
  *connection_handle = key & 0xFF00FFFF;
  *sessionId = key >> 16;
  LOG4CXX_INFO(
    logger_,
    "ConnectionHandle:" << static_cast<int32_t>(*connection_handle) << " Session:"
    << static_cast<int32_t>(*sessionId) << " for key:"
    << static_cast<int32_t>(key));
}

int32_t ConnectionHandlerImpl::GetDataOnSessionKey(uint32_t key,
    uint32_t* app_id,
    std::list<int32_t>* sessions_list,
    uint32_t* device_id) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::GetDataOnSessionKey");

  int32_t result = -1;
  transport_manager::ConnectionUID conn_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &conn_handle, &session_id);
  ConnectionListIterator it = connection_list_.find(conn_handle);

  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
    return result;
  }

  Connection& connection = *it->second;
  if (device_id) {
    *device_id = connection.connection_device_handle();
  }

  if (sessions_list) {
    sessions_list->clear();
  }

  if (0 == session_id) {
    LOG4CXX_INFO(logger_,
        "No sessions in connection " << static_cast<int32_t>(conn_handle));
    if (app_id) {
      *app_id = 0;
    }
  } else {
    if (app_id) {
      *app_id = KeyFromPair(conn_handle, session_id);
    }

    LOG4CXX_INFO(
      logger_,
      "Connection " << static_cast<int32_t>(conn_handle) << " has "
      << connection.session_map().size() << " sessions.");

    if (sessions_list) {
      const SessionMap& session_map = connection.session_map();
      for (SessionMapConstIterator session_it = session_map.begin(),
           end = session_map.end(); session_it != end; ++session_it) {
        sessions_list->push_back(KeyFromPair(conn_handle, it->first));
      }
    }

    result = 0;
  }

  return result;
}

int32_t ConnectionHandlerImpl::GetDataOnDeviceID(
  DeviceHandle device_handle, std::string* device_name,
  std::list<uint32_t>* applications_list,
  std::string* mac_address) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::GetDataOnDeviceID");

  int32_t result = -1;
  DeviceListIterator it = device_list_.find(device_handle);
  if (device_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Device not found!");
    return result;
  }

  if (device_name) {
    *device_name = (*it).second.user_friendly_name();
  }

  if (applications_list) {
    applications_list->clear();
    for (ConnectionListIterator itr = connection_list_.begin();
         itr != connection_list_.end(); ++itr) {
      if (device_handle == (*itr).second->connection_device_handle()) {
        const SessionMap& session_map = (itr->second)->session_map();
        for (SessionMapConstIterator session_it = session_map.begin(),
             end = session_map.end(); session_it != end; ++session_it) {
          applications_list->push_back(it->first);
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

int ConnectionHandlerImpl::SetSSLContext(
    const uint32_t &key, protocol_handler::ServiceType service_type,
    security_manager::SSLContext *context) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::SetSSLContext");
  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionListIterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
      LOG4CXX_ERROR(logger_, "Unknown connection!");
      //WARNING(EZ): return INTERNAL_ERROR or SERVICE_NOT_FOUND ?
      return security_manager::SecurityQuery::INTERNAL_ERROR;
    }
  Connection& connection = *it->second;
  return connection.SetSSLContext(session_id, service_type, context);
}

security_manager::SSLContext *ConnectionHandlerImpl::GetSSLContext(
      const uint32_t &key, protocol_handler::ServiceType service_type) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::GetSSLContext");
  transport_manager::ConnectionUID connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionListIterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
      LOG4CXX_ERROR(logger_, "Unknown connection!");
      return NULL;
    }
  Connection& connection = *it->second;
  return connection.GetSSLContext(session_id, service_type);
}

void ConnectionHandlerImpl::set_transport_manager(
  transport_manager::TransportManager* transport_mngr) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::set_transport_manager()");
  if (!transport_mngr) {
    LOG4CXX_ERROR(logger_, "Null pointer to TransportManager.");
    return;
  }
  transport_manager_ = transport_mngr;
}

void ConnectionHandlerImpl::StartDevicesDiscovery() {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::StartDevicesDiscovery()");

  if (NULL == transport_manager_) {
    LOG4CXX_ERROR(logger_, "Null pointer to TransportManager.");
    return;
  }
  //transport_manager_->SearchDevices();
  if (connection_handler_observer_) {
    connection_handler_observer_->OnDeviceListUpdated(device_list_);
  }
}

void ConnectionHandlerImpl::ConnectToDevice(
  connection_handler::DeviceHandle device_handle) {
  connection_handler::DeviceList::const_iterator it_in;
  it_in = device_list_.find(device_handle);
  if (device_list_.end() != it_in) {
    LOG4CXX_INFO_EXT(logger_,
                     "Connecting to device with handle " << device_handle);
    if (transport_manager_) {
      if (transport_manager::E_SUCCESS !=
          transport_manager_->ConnectDevice(device_handle)) {
        LOG4CXX_WARN(logger_, "Can't connect to device");
      }
    } else {
      LOG4CXX_ERROR(logger_, "Null pointer to TransportManager.");
    }
  } else {
    LOG4CXX_ERROR(
      logger_,
      "Application Manager wanted to connect to non-existing device");
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

void ConnectionHandlerImpl::CloseConnection(uint32_t key) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::CloseConnection by HB");

  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(key, &connection_handle, &session_id);

  CloseConnection(connection_handle);
}

void ConnectionHandlerImpl::CloseConnection(ConnectionHandle connection_handle) {
  LOG4CXX_INFO(logger_, "ConnectionHandlerImpl::CloseConnection");
  if (!transport_manager_) {
    LOG4CXX_ERROR(logger_, "Null pointer to TransportManager.");
    return;
  }
  transport_manager::ConnectionUID connection_uid =
      ConnectionUIDFromHandle(connection_handle);
  transport_manager_->Disconnect(connection_uid);
}

void ConnectionHandlerImpl::KeepConnectionAlive(uint32_t connection_key) {
  uint32_t connection_handle = 0;
  uint8_t session_id = 0;
  PairFromKey(connection_key, &connection_handle, &session_id);

  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionListIterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() != it) {
    it->second->KeepAlive();
    }
}

void ConnectionHandlerImpl::OnConnectionEnded(
    const transport_manager::ConnectionUID& connection_id) {
  LOG4CXX_INFO(logger_, "Delete Connection: " <<
               static_cast<int32_t>(connection_id) << " from the list.");

  sync_primitives::AutoLock lock(connection_list_lock_);
  ConnectionListIterator itr = connection_list_.find(connection_id);
  if (connection_list_.end() == itr) {
    LOG4CXX_ERROR(logger_, "Connection not found!");
    return;
  }

  if (0 != connection_handler_observer_) {
    const Connection* connection = itr->second;
    const SessionMap session_map = connection->session_map();
    for (SessionMapConstIterator session_it = session_map.begin(),
         end = session_map.end(); session_it != end; ++session_it) {

      uint32_t session_key = KeyFromPair(connection_id, session_it->first);
      for (ServiceListConstIterator service_it = session_it->second.begin(),
          end = session_it->second.end(); service_it != end; ++service_it) {

      connection_handler_observer_->OnServiceEndedCallback(session_key, service_it->service_type);
      }
    }
  }

  delete itr->second;
  connection_list_.erase(itr);
}

#ifdef BUILD_TESTS
ConnectionList &ConnectionHandlerImpl::getConnectionList(){
  return connection_list_;
}

bool ConnectionHandlerImpl::addDeviceConnection(const transport_manager::DeviceInfo &device_info, const transport_manager::ConnectionUID &connection_id) {
  //Add Device
  const bool result =
      AddDeviceInDeviceListIfNotExist(device_info);
  if(result)
    //Add connection
    OnConnectionEstablished(device_info, connection_id);
  return result;
}
#endif

}/* namespace connection_handler */
