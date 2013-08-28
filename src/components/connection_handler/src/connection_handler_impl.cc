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

#include "connection_handler/connection_handler_impl.h"
#include "transport_manager/info.h"

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace connection_handler {

log4cxx::LoggerPtr  ConnectionHandlerImpl::logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("ConnectionHandler"));

ConnectionHandlerImpl* ConnectionHandlerImpl::instance() {
  static ConnectionHandlerImpl instance;
  return &instance;
}

ConnectionHandlerImpl::ConnectionHandlerImpl()
  : connection_handler_observer_(NULL),
    transport_manager_(NULL) {
}

ConnectionHandlerImpl::~ConnectionHandlerImpl() {
}

void ConnectionHandlerImpl::set_connection_handler_observer(
  ConnectionHandlerObserver* observer) {
  LOG4CXX_INFO(logger_, "CConnectionHandler::setConnectionHandlerObserver()");
  if (!observer) {
    LOG4CXX_ERROR(logger_, "Null pointer to observer.");
    return;
  }
  connection_handler_observer_ = observer;
}

void ConnectionHandlerImpl::OnTMMessageReceived(
  const transport_manager::RawMessageSptr message) {}

void ConnectionHandlerImpl::OnTMMessageReceiveFailed(
  transport_manager::ConnectionUID connection_id,
  const transport_manager::DataReceiveError& error) {}
void ConnectionHandlerImpl::OnTMMessageSendFailed(
  const transport_manager::DataSendError& error,
  const transport_manager::RawMessageSptr message) {}
void ConnectionHandlerImpl::OnTMMessageSend() {}

void ConnectionHandlerImpl::OnDeviceListUpdated(const std::vector<transport_manager::DeviceInfo>& device_info_list) {
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
            == (*it).second.connection_device_handle()) {
          RemoveConnection((*it).first);
        }
      }
      device_list_.erase(device_for_remove_handle);
      if (connection_handler_observer_) {
        connection_handler_observer_->RemoveDevice(device_for_remove_handle);
      }
    }
  }
  for (std::vector<transport_manager::DeviceInfo>::const_iterator it_in =
         device_info_list.begin(); it_in != device_info_list.end(); ++it_in) {
    AddDeviceInDeviceListIfNotExist((*it_in));
  }
  if (connection_handler_observer_) {
    connection_handler_observer_->OnDeviceListUpdated(device_list_);
  }
}

void ConnectionHandlerImpl::OnDeviceFound(
  const transport_manager::DeviceInfo& device_info) {
  LOG4CXX_INFO(logger_, "CConnectionHandler::onDeviceListUpdated()");
  /*DeviceList::iterator it = device_list_during_search_.find(
                              device_info.device_handle());
  if (device_list_during_search_.end() != it) {
    LOG4CXX_WARN(logger_, "Device found for second time. Skipping it");
    return;
  }

  device_list_during_search_.insert(
    DeviceList::value_type(device_info.device_handle(),
                           Device(device_info.device_handle(),
                                  device_info.name(),
                                  device_info.mac_address())));

  if (connection_handler_observer_) {
    connection_handler_observer_->OnDeviceListUpdated(
      device_list_during_search_);
  }*/
}

void  ConnectionHandlerImpl::OnNoDeviceFound() {
  for (DeviceListIterator itr = device_list_.begin(); itr != device_list_.end();
       ++itr) {
    DeviceHandle device_for_remove_handle = (*itr).first;
    for (ConnectionListIterator it = connection_list_.begin();
         it != connection_list_.end(); ++it) {
      if (device_for_remove_handle
          == (*it).second.connection_device_handle()) {
        RemoveConnection((*it).first);
      }
    }
    device_list_.erase(device_for_remove_handle);
    if (connection_handler_observer_) {
      connection_handler_observer_->RemoveDevice(device_for_remove_handle);
    }
  }
  if (connection_handler_observer_) {
    connection_handler_observer_->OnDeviceListUpdated(device_list_);
  }
}

bool ConnectionHandlerImpl::DoesDeviceExistInTMList(
  const std::vector<transport_manager::DeviceInfo>& device_list,
  const connection_handler::DeviceHandle device_handle) {
  bool result = false;
  for (std::vector<transport_manager::DeviceInfo>::const_iterator it_in =
         device_list.begin();
       it_in != device_list.end(); ++it_in) {
    if (it_in->device_handle() == device_handle) {
      result = true;
      break;
    }
  }
  return result;
}

void ConnectionHandlerImpl::AddDeviceInDeviceListIfNotExist(
  const transport_manager::DeviceInfo& device_info) {
  DeviceListIterator it = device_list_.find(device_info.device_handle());
  if (device_list_.end() == it) {
    LOG4CXX_INFO(logger_, "Adding new device!");
    device_list_.insert(
      DeviceList::value_type(device_info.device_handle(),
                             Device(device_info.device_handle(),
                                    device_info.name(),
                                    device_info.mac_address())));
  }
}

void ConnectionHandlerImpl::OnScanDevicesFinished() {
  LOG4CXX_INFO(logger_, "scan devices finished successfully.");
  /*for (DeviceListIterator itr = device_list_.begin(); itr != device_list_.end();
       ++itr) {
    if (!DoesDeviceExistInTMList(device_list_during_search_, (*itr).first)) {
      // Device has been removed. Perform all needed actions.
      // 1. Delete all the connections and sessions of this device
      // 2. Delete device from a list
      // 3. Let observer know that device has been deleted.
      DeviceHandle device_for_remove_handle = (*itr).first;
      for (ConnectionListIterator it = connection_list_.begin();
           it != connection_list_.end(); ++it) {
        if (device_for_remove_handle
            == (*it).second.connection_device_handle()) {
          RemoveConnection((*it).first);
        }
      }
      device_list_.erase(device_for_remove_handle);
      if (connection_handler_observer_) {
        connection_handler_observer_->RemoveDevice(device_for_remove_handle);
      }
    }
  }
  for (DeviceList::const_iterator it_in =
         device_list_during_search_.begin();
       it_in != device_list_during_search_.end(); ++it_in) {
    AddDeviceInDeviceListIfNotExist(it_in->second);
  }
  device_list_during_search_.clear();*/
}

void ConnectionHandlerImpl::OnScanDevicesFailed(
  const transport_manager::SearchDeviceError& error) {
  LOG4CXX_ERROR(logger_, "Failed to scan devies.");
}

void ConnectionHandlerImpl::OnConnectionEstablished(
  const transport_manager::DeviceInfo& device_info,
  const transport_manager::ConnectionUID& connection_id) {
  LOG4CXX_INFO(logger_, "CConnectionHandler::onApplicationConnected()");
  DeviceListIterator it = device_list_.find(device_info.device_handle());
  if (device_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown device!");
    return;
  }
  LOG4CXX_INFO(logger_, "Add Connection:" << connection_id << " to the list.");
  connection_list_.insert(
    ConnectionList::value_type(
      connection_id,
      Connection(connection_id, device_info.device_handle())));
}

void ConnectionHandlerImpl::OnConnectionFailed(
  const transport_manager::DeviceInfo& device_info,
  const transport_manager::ConnectError& error) {
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_, "Failed connecting.");
}

void ConnectionHandlerImpl::OnConnectionClosed(
  transport_manager::ConnectionUID connection_id) {
  LOG4CXX_INFO(
    logger_,
    "Delete Connection:" << static_cast<int>(connection_id)
    << "from the list.");
  ConnectionListIterator itr = connection_list_.find(connection_id);
  if (connection_list_.end() == itr) {
    LOG4CXX_ERROR(logger_, "Connection not found!");
    return;
  } else {
    if (0 != connection_handler_observer_) {
      unsigned int first_session_id = (itr->second).GetFirstSessionID();
      if (0 < first_session_id) {
        first_session_id = KeyFromPair(connection_id, first_session_id);
        // In case both parameters of OnSessionEndedCallback are the same
        // AppMgr knows that Application with id=first_session_id
        // should be closed.
        connection_handler_observer_->OnSessionEndedCallback(first_session_id,
            first_session_id);
      }
    }
    connection_list_.erase(itr);
  }
}

void ConnectionHandlerImpl::OnConnectionClosedFailure(
  transport_manager::ConnectionUID connection_id,
  const transport_manager::DisconnectError& error) {
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_, "Connection closed failure");
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
  LOG4CXX_ERROR(logger_, "Trying to disconnect device failed.");
}

void ConnectionHandlerImpl::RemoveConnection(
  const ConnectionHandle connection_handle) {
  LOG4CXX_INFO(logger_, "CConnectionHandler::DisconnectApplication()");
  LOG4CXX_INFO(
    logger_,
    "Delete Connection:" << static_cast<int>(connection_handle)
    << "from the list.");
  ConnectionListIterator itr = connection_list_.find(connection_handle);
  if (connection_list_.end() == itr) {
    LOG4CXX_ERROR(logger_, "Connection not found!");
    return;
  } else {
    if (0 != connection_handler_observer_) {
      int first_session_id = (itr->second).GetFirstSessionID();
      if (0 < first_session_id) {
        first_session_id = KeyFromPair(connection_handle, first_session_id);
        // In case both parameters of OnSessionEndedCallback are the same
        // AppMgr knows that Application with id=first_session_id
        // should be closed.
        connection_handler_observer_->OnSessionEndedCallback(first_session_id,
            first_session_id);
      }
    }
    connection_list_.erase(itr);
  }
}

unsigned int ConnectionHandlerImpl::OnSessionStartedCallback(
  transport_manager::ConnectionUID connection_handle) {
  LOG4CXX_INFO(logger_, "CConnectionHandler::onSessionStartedCallback()");
  int newSessionID = -1;
  ConnectionListIterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
  } else {
    newSessionID = (it->second).AddNewSession();
    int firstSessionID = (it->second).GetFirstSessionID();
    if (0 > newSessionID) {
      LOG4CXX_ERROR(logger_, "Not possible to start session!");
    } else {
      LOG4CXX_INFO(logger_,
                   "New session ID:" << static_cast<int>(newSessionID));
      if (0 != connection_handler_observer_) {
        if (0 < firstSessionID) {
          firstSessionID = KeyFromPair(connection_handle, firstSessionID);
        }
        int sessionKey = KeyFromPair(connection_handle, newSessionID);
        connection_handler_observer_->OnSessionStartedCallback(
          (it->second).connection_device_handle(), sessionKey,
          firstSessionID);
      }
    }
  }
  return newSessionID;
}

unsigned int ConnectionHandlerImpl::OnSessionEndedCallback(
  unsigned int connection_handle, unsigned char sessionId,
  unsigned int hashCode) {
  LOG4CXX_INFO(logger_, "CConnectionHandler::onSessionEndedCallback()");
  int result = -1;
  ConnectionListIterator it = connection_list_.find(connection_handle);
  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
  } else {
    int firstSessionID = (it->second).GetFirstSessionID();
    result = (it->second).RemoveSession(sessionId);
    if (0 > result) {
      LOG4CXX_ERROR(logger_, "Not possible to remove session!");
    } else {
      LOG4CXX_INFO(logger_, "Session removed:" << static_cast<int>(result));
      if (0 != connection_handler_observer_) {
        if (0 < firstSessionID) {
          firstSessionID = KeyFromPair(connection_handle, firstSessionID);
        }
        int sessionKey = KeyFromPair(connection_handle, sessionId);
        connection_handler_observer_->OnSessionEndedCallback(sessionKey,
            firstSessionID);
        result = sessionKey;
      }
    }
  }
  return result;
}

unsigned int ConnectionHandlerImpl::KeyFromPair(
  transport_manager::ConnectionUID connection_handle,
  unsigned char sessionId) {
  int key = connection_handle | (sessionId << 16);
  LOG4CXX_INFO(
    logger_,
    "Key for ConnectionHandle:" << static_cast<int>(connection_handle)
    << " Session:" << static_cast<int>(sessionId)
    << " is: " << static_cast<int>(key));
  return key;
}

void ConnectionHandlerImpl::PairFromKey(unsigned int key,
                                        unsigned int* connection_handle,
                                        unsigned char* sessionId) {
  *connection_handle = key & 0xFF00FFFF;
  *sessionId = key >> 16;
  LOG4CXX_INFO(
    logger_,
    "ConnectionHandle:" << static_cast<int>(*connection_handle)
    << " Session:" << static_cast<int>(*sessionId)
    << " for key:" << static_cast<int>(key));
}

int ConnectionHandlerImpl::GetDataOnSessionKey(unsigned int key,
    unsigned int* app_id,
    std::list<int>* sessions_list,
    unsigned int* device_id) {
  DCHECK(app_id);
  DCHECK(sessions_list);
  //DCHECK(device_id);

  LOG4CXX_INFO(logger_, "CConnectionHandler::GetDataOnSessionKey()");
  int result = -1;
  transport_manager::ConnectionUID conn_handle = 0;
  unsigned char session_id = 0;
  PairFromKey(key, &conn_handle, &session_id);
  ConnectionListIterator it = connection_list_.find(conn_handle);

  if (connection_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Unknown connection!");
  } else {
    Connection connection = it->second;
    *device_id = connection.connection_device_handle();
    int first_session_id = connection.GetFirstSessionID();
    sessions_list->clear();
    if (0 == first_session_id) {
      LOG4CXX_INFO(
        logger_,
        "No sessions in connection " << static_cast<int>(conn_handle) << ".");
      *app_id = 0;
    } else {
      *app_id = KeyFromPair(conn_handle, first_session_id);
      SessionList session_list;
      connection.GetSessionList(session_list);
      LOG4CXX_INFO(
        logger_,
        "Connection " << static_cast<int>(conn_handle)
        << "has " << static_cast<int>(session_list.size())
        << "sessions.");
      for (SessionListIterator itr = session_list.begin();
           itr != session_list.end(); ++itr) {
        sessions_list->push_back(KeyFromPair(conn_handle, *itr));
      }
      result = 0;
    }
  }
  return result;
}

int ConnectionHandlerImpl::GetDataOnDeviceID(
  DeviceHandle device_handle, std::string* device_name,
  std::list<unsigned int>* applications_list) {
  DCHECK(device_name);
  DCHECK(applications_list);

  LOG4CXX_INFO(logger_, "CConnectionHandler::GetDataOnDeviceID()");
  int result = -1;
  DeviceListIterator it = device_list_.find(device_handle);
  if (device_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Device not found!");
  } else {
    *device_name = (*it).second.user_friendly_name();
    applications_list->clear();
    for (ConnectionListIterator itr = connection_list_.begin();
         itr != connection_list_.end(); ++itr) {
      if (device_handle == (*itr).second.connection_device_handle()) {
        applications_list->push_back((*itr).second.GetFirstSessionID());
      }
    }
    result = 0;
  }
  return result;
}

void ConnectionHandlerImpl::set_transport_manager(
  transport_manager::TransportManager* transport_manager) {
  LOG4CXX_INFO(logger_, "CConnectionHandler::setTransportManager()");
  if (!transport_manager) {
    LOG4CXX_ERROR(logger_, "Null pointer to TransportManager.");
    return;
  }
  transport_manager_ = transport_manager;
}
void ConnectionHandlerImpl::StartDevicesDiscovery() {
  LOG4CXX_INFO(logger_, "CConnectionHandler::startDevicesDiscovery()");
  if (NULL == transport_manager_) {
    LOG4CXX_ERROR(logger_, "Null pointer to TransportManager.");
    return;
  }
  //device_list_during_search_.clear();
  transport_manager_->SearchDevices();
}

void ConnectionHandlerImpl::ConnectToDevice(
  connection_handler::DeviceHandle device_handle) {
  connection_handler::DeviceList::const_iterator it_in;
  it_in = device_list_.find(device_handle);
  if (device_list_.end() != it_in) {
    LOG4CXX_INFO_EXT(
      logger_,
      "Connecting to device with handle " << device_handle);
    if (transport_manager_) {
      // TODO(PV): change this
      transport_manager_->connectDevice(device_handle);
    }
  } else {
    LOG4CXX_ERROR(
      logger_, "Application Manager wanted to connect to non-existing device");
  }
}

void ConnectionHandlerImpl::StartTransportManager() {
  LOG4CXX_INFO(logger_, "CConnectionHandler::startTransportManager()");
  if (NULL == transport_manager_) {
    LOG4CXX_ERROR(logger_, "Null pointer to TransportManager.");
    return;
  }

  transport_manager_->Visibility(true);
}

}/* namespace connection_handler */
