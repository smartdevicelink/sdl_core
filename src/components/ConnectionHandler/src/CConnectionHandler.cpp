/**
 * \file CConnectionHandler.cpp
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

#include "Logger.hpp"

#include "ConnectionHandler/CConnectionHandler.hpp"

/**
 * \namespace NsConnectionHandler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace NsConnectionHandler {

log4cplus::Logger CConnectionHandler::logger_ = log4cplus::Logger::getInstance(
    LOG4CPLUS_TEXT("ConnectionHandler"));

CConnectionHandler* CConnectionHandler::getInstance() {
  static CConnectionHandler instance;
  return &instance;
}

CConnectionHandler::CConnectionHandler()
    : connection_handler_observer_(NULL),
      transport_manager_(NULL) {
}

CConnectionHandler::~CConnectionHandler() {
}

void CConnectionHandler::set_connection_handler_observer(
    IConnectionHandlerObserver * observer) {
  LOG4CPLUS_INFO(logger_, "CConnectionHandler::setConnectionHandlerObserver()");
  if (!observer) {
    LOG4CPLUS_ERROR(logger_, "Null pointer to observer.");
    return;
  }
  connection_handler_observer_ = observer;
}

void CConnectionHandler::onDeviceListUpdated(
    const NsSmartDeviceLink::NsTransportManager::tDeviceList & DeviceList) {
  LOG4CPLUS_INFO(logger_, "CConnectionHandler::onDeviceListUpdated()");
  for (DeviceListIterator itr = device_list_.begin(); itr != device_list_.end();
      ++itr) {
    if (!DoesDeviceExistInTMList(DeviceList, (*itr).first)) {
      // Device has been removed. Perform all needed actions.
      // 1. Delete all the connections and sessions of this device
      // 2. Delete device from a list
      // 3. Let observer know that device has been deleted.
      DeviceHandle device_for_remove_handle = (*itr).first;
      for (tConnectionListIterator it = connection_list_.begin();
          it != connection_list_.end(); ++it) {
        if (device_for_remove_handle
            == (*it).second.getConnectionDeviceHandle()) {
          RemoveConnection((*it).first);
        }
      }
      device_list_.erase(device_for_remove_handle);
      if (connection_handler_observer_) {
        connection_handler_observer_->RemoveDevice(device_for_remove_handle);
      }
    }
  }
  for (NsSmartDeviceLink::NsTransportManager::tDeviceList::const_iterator it_in =
      DeviceList.begin(); it_in != DeviceList.end(); ++it_in) {
    AddDeviceInDeviceListIfNotExist((*it_in));
  }
  if (connection_handler_observer_) {
    connection_handler_observer_->OnDeviceListUpdated(device_list_);
    connection_handler_observer_->UpdateDeviceList(device_list_);
  }
}

bool CConnectionHandler::DoesDeviceExistInTMList(
    const NsSmartDeviceLink::NsTransportManager::tDeviceList & DeviceList,
    const NsConnectionHandler::DeviceHandle DeviceHandle) {
  bool result = false;
  for (NsSmartDeviceLink::NsTransportManager::tDeviceList::const_iterator it_in =
      DeviceList.begin(); it_in != DeviceList.end(); ++it_in) {
    if ((*it_in).mDeviceHandle == DeviceHandle) {
      result = true;
      break;
    }
  }
  return result;
}

void CConnectionHandler::AddDeviceInDeviceListIfNotExist(
    const NsSmartDeviceLink::NsTransportManager::SDeviceInfo DeviceInfo) {
  DeviceListIterator it = device_list_.find(DeviceInfo.mDeviceHandle);
  if (device_list_.end() == it) {
    LOG4CPLUS_INFO(logger_, "Adding new device!");
    device_list_.insert(
        DeviceList::value_type(
            DeviceInfo.mDeviceHandle,
            CDevice(DeviceInfo.mDeviceHandle, DeviceInfo.mUserFriendlyName)));
  }
}

void CConnectionHandler::onApplicationConnected(
    const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & ConnectedDevice,
    const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection) {
  LOG4CPLUS_INFO(logger_, "CConnectionHandler::onApplicationConnected()");
  DeviceListIterator it = device_list_.find(ConnectedDevice.mDeviceHandle);
  if (device_list_.end() == it) {
    LOG4CPLUS_ERROR(logger_, "Unknown device!");
    return;
  }
  LOG4CPLUS_INFO(logger_, "Add Connection:" << Connection << " to the list.");
  connection_list_.insert(
      tConnectionList::value_type(
          Connection, CConnection(Connection, ConnectedDevice.mDeviceHandle)));
}

void CConnectionHandler::onApplicationDisconnected(
    const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & DisconnectedDevice,
    const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection) {
  LOG4CPLUS_INFO(logger_, "CConnectionHandler::onApplicationDisconnected()");
  DeviceListIterator it = device_list_.find(DisconnectedDevice.mDeviceHandle);
  if (device_list_.end() == it) {
    LOG4CPLUS_ERROR(logger_, "Unknown device!");
    return;
  }
  LOG4CPLUS_INFO(
      logger_,
      "Delete Connection:" << static_cast<int>(Connection) << "from the list.");
  tConnectionListIterator itr = connection_list_.find(Connection);
  if (connection_list_.end() == itr) {
    LOG4CPLUS_ERROR(logger_, "Connection not found!");
    return;
  } else {
    if (0 != connection_handler_observer_) {
      int firstSessionID = (itr->second).getFirstSessionID();
      if (0 < firstSessionID) {
        firstSessionID = keyFromPair(Connection, firstSessionID);
        // In case bot parameters of OnSessionEndedCallback are the same
        // AppMgr knows that Application with id=firstSessionID
        // should be closed.
        connection_handler_observer_->OnSessionEndedCallback(firstSessionID,
                                                             firstSessionID);
      }
    }
    connection_list_.erase(itr);
  }

  if (connection_list_.empty()) {
    device_list_.erase(it);
  }
}

void CConnectionHandler::RemoveConnection(
    const tConnectionHandle connection_handle) {

  LOG4CPLUS_INFO(logger_, "CConnectionHandler::DisconnectApplication()");
  LOG4CPLUS_INFO(
      logger_,
      "Delete Connection:" << static_cast<int>(connection_handle)
      << "from the list.");
  tConnectionListIterator itr = connection_list_.find(connection_handle);
  if (connection_list_.end() == itr) {
    LOG4CPLUS_ERROR(logger_, "Connection not found!");
    return;
  } else {
    if (0 != connection_handler_observer_) {
      int firstSessionID = (itr->second).getFirstSessionID();
      if (0 < firstSessionID) {
        firstSessionID = keyFromPair(connection_handle, firstSessionID);
        // In case both parameters of OnSessionEndedCallback are the same
        // AppMgr knows that Application with id=firstSessionID
        // should be closed.
        connection_handler_observer_->OnSessionEndedCallback(firstSessionID,
                                                             firstSessionID);
      }
    }
    connection_list_.erase(itr);
  }
}

int CConnectionHandler::onSessionStartedCallback(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle) {
  LOG4CPLUS_INFO(logger_, "CConnectionHandler::onSessionStartedCallback()");
  int newSessionID = -1;
  tConnectionListIterator it = connection_list_.find(connectionHandle);
  if (connection_list_.end() == it) {
    LOG4CPLUS_ERROR(logger_, "Unknown connection!");
  } else {
    newSessionID = (it->second).addNewSession();
    int firstSessionID = (it->second).getFirstSessionID();
    if (0 > newSessionID) {
      LOG4CPLUS_ERROR(logger_, "Not possible to start session!");
    } else {
      LOG4CPLUS_INFO(logger_,
                     "New session ID:" << static_cast<int>(newSessionID));
      if (0 != connection_handler_observer_) {
        if (0 < firstSessionID) {
          firstSessionID = keyFromPair(connectionHandle, firstSessionID);
        }
        int sessionKey = keyFromPair(connectionHandle, newSessionID);
        connection_handler_observer_->OnSessionStartedCallback(
            (it->second).getConnectionDeviceHandle(), sessionKey,
            firstSessionID);
      }
    }
  }
  return newSessionID;
}

int CConnectionHandler::onSessionEndedCallback(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
    unsigned char sessionId, unsigned int hashCode) {
  LOG4CPLUS_INFO(logger_, "CConnectionHandler::onSessionEndedCallback()");
  int result = -1;
  tConnectionListIterator it = connection_list_.find(connectionHandle);
  if (connection_list_.end() == it) {
    LOG4CPLUS_ERROR(logger_, "Unknown connection!");
  } else {
    int firstSessionID = (it->second).getFirstSessionID();
    result = (it->second).removeSession(sessionId);
    if (0 > result) {
      LOG4CPLUS_ERROR(logger_, "Not possible to remove session!");
    } else {
      LOG4CPLUS_INFO(logger_, "Session removed:" << static_cast<int>(result));
      if (0 != connection_handler_observer_) {
        if (0 < firstSessionID) {
          firstSessionID = keyFromPair(connectionHandle, firstSessionID);
        }
        int sessionKey = keyFromPair(connectionHandle, sessionId);
        connection_handler_observer_->OnSessionEndedCallback(sessionKey,
                                                             firstSessionID);
        result = sessionKey;
      }
    }
  }
  return result;
}

int CConnectionHandler::keyFromPair(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
    unsigned char sessionId) {
  int key = connectionHandle | (sessionId << 16);
  LOG4CPLUS_INFO(
      logger_,
      "Key for ConnectionHandle:" << static_cast<int>(connectionHandle)
      << " Session:" << static_cast<int>(sessionId) << " is: "
      << static_cast<int>(key));
  return key;
}

void CConnectionHandler::pairFromKey(
    int key,
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle & connectionHandle,
    unsigned char & sessionId) {
  connectionHandle = key & 0xFF00FFFF;
  sessionId = key >> 16;
  LOG4CPLUS_INFO(
      logger_,
      "ConnectionHandle:" << static_cast<int>(connectionHandle) << " Session:"
      << static_cast<int>(sessionId) << " for key:" << static_cast<int>(key));
}

int CConnectionHandler::GetDataOnSessionKey(int key, int & app_id,
                                            std::list<int> & sessions_list,
                                            int & device_id) {
  LOG4CPLUS_INFO(logger_, "CConnectionHandler::GetDataOnSessionKey()");
  int result = -1;
  NsSmartDeviceLink::NsTransportManager::tConnectionHandle conn_handle = 0;
  unsigned char session_id = 0;
  pairFromKey(key, conn_handle, session_id);
  tConnectionListIterator it = connection_list_.find(conn_handle);
  if (connection_list_.end() == it) {
    LOG4CPLUS_ERROR(logger_, "Unknown connection!");
  } else {
    CConnection connection = it->second;
    device_id = connection.getConnectionDeviceHandle();
    int first_session_id = connection.getFirstSessionID();
    sessions_list.clear();
    if (0 == first_session_id) {
      LOG4CPLUS_INFO(
          logger_,
          "No sessions in connection " << static_cast<int>(conn_handle) << ".");
      app_id = 0;
    } else {
      app_id = keyFromPair(conn_handle, first_session_id);
      tSessionList session_list;
      connection.GetSessionList(session_list);
      LOG4CPLUS_INFO(
          logger_,
          "Connection " << static_cast<int>(conn_handle) << "has "
          << static_cast<int>(session_list.size()) << "sessions.");
      for (tSessionListIterator itr = session_list.begin();
          itr != session_list.end(); ++itr) {
        sessions_list.push_back(keyFromPair(conn_handle, *itr));
      }
      result = 0;
    }
  }
  return result;
}

int CConnectionHandler::GetDataOnDeviceID(int device_id,
                                          std::string & device_name,
                                          std::list<int> & applications_list) {
  LOG4CPLUS_INFO(logger_, "CConnectionHandler::GetDataOnDeviceID()");
  int result = -1;
  DeviceListIterator it = device_list_.find(device_id);
  if (device_list_.end() == it) {
    LOG4CPLUS_ERROR(logger_, "Device not found!");
  } else {
    device_name = (*it).second.user_friendly_name();
    applications_list.clear();
    for (tConnectionListIterator itr = connection_list_.begin();
        itr != connection_list_.end(); ++itr) {
      if (device_id == (*itr).second.getConnectionDeviceHandle()) {
        applications_list.push_back((*itr).second.getFirstSessionID());
      }
    }
    result = 0;
  }
  return result;
}

void CConnectionHandler::set_transport_manager(
    NsSmartDeviceLink::NsTransportManager::ITransportManager * transportManager) {
  LOG4CPLUS_INFO(logger_, "CConnectionHandler::setTransportManager()");
  if (!transportManager) {
    LOG4CPLUS_ERROR(logger_, "Null pointer to TransportManager.");
    return;
  }
  transport_manager_ = transportManager;
}
void CConnectionHandler::StartDevicesDiscovery() {
  LOG4CPLUS_INFO(logger_, "CConnectionHandler::startDevicesDiscovery()");
  if (NULL == transport_manager_) {
    LOG4CPLUS_ERROR(logger_, "Null pointer to TransportManager.");
    return;
  }
  transport_manager_->scanForNewDevices();
}

void CConnectionHandler::ConnectToDevice(
    NsConnectionHandler::DeviceHandle deviceHandle) {
  NsConnectionHandler::DeviceList::const_iterator it_in;
  it_in = device_list_.find(deviceHandle);
  if (device_list_.end() != it_in) {
    LOG4CPLUS_INFO_EXT(
        logger_,
        "Connecting to device with handle " << static_cast<int>(deviceHandle));
    if (transport_manager_) {
      transport_manager_->connectDevice(deviceHandle);
    }
  } else {
    LOG4CPLUS_ERROR(
        logger_,
        "Application Manager wanted to connect to non-existing device");
  }
}

void CConnectionHandler::StartTransportManager() {
  LOG4CPLUS_INFO(logger_, "CConnectionHandler::startTransportManager()");
  if (NULL == transport_manager_) {
    LOG4CPLUS_ERROR(logger_, "Null pointer to TransportManager.");
    return;
  }

  transport_manager_->run();
}

}/* namespace NsConnectionHandler */
