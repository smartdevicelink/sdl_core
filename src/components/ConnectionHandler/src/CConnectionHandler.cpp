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

log4cplus::Logger CConnectionHandler::m_logger = log4cplus::Logger::getInstance(
    LOG4CPLUS_TEXT("ConnectionHandler"));

CConnectionHandler* CConnectionHandler::getInstance() {
  static CConnectionHandler instance;
  return &instance;
}

CConnectionHandler::CConnectionHandler()
    : mp_connection_handler_observer(NULL),
      mp_transport_manager(NULL) {
}

CConnectionHandler::~CConnectionHandler() {
}

void CConnectionHandler::setConnectionHandlerObserver(
    IConnectionHandlerObserver * observer) {
  LOG4CPLUS_INFO(m_logger, "CConnectionHandler::setConnectionHandlerObserver()");
  if (!observer) {
    LOG4CPLUS_ERROR(m_logger, "Null pointer to observer.");
    return;
  }
  mp_connection_handler_observer = observer;
}

void CConnectionHandler::onDeviceListUpdated(
    const NsSmartDeviceLink::NsTransportManager::tDeviceList & DeviceList) {
  LOG4CPLUS_INFO(m_logger, "CConnectionHandler::onDeviceListUpdated()");
  for (tDeviceListIterator itr = m_deviceList.begin(); itr != m_deviceList.end();
      ++itr) {
    if (!DoesDeviceExistInTMList(DeviceList, (*itr).first)) {
      // Device has been removed. Perform all needed actions.
      // 1. Delete all the connections and sessions of this device
      // 2. Delete device from a list
      // 3. Let observer know that device has been deleted.
      tDeviceHandle device_for_remove_handle = (*itr).first;
      for (tConnectionListIterator it = m_connection_list.begin();
          it != m_connection_list.end(); ++it) {
        if (device_for_remove_handle
            == (*it).second.getConnectionDeviceHandle()) {
          RemoveConnection((*it).first);
        }
      }
      m_deviceList.erase(device_for_remove_handle);
      if (mp_connection_handler_observer) {
        mp_connection_handler_observer->RemoveDevice(device_for_remove_handle);
      }
    }
  }
  for (NsSmartDeviceLink::NsTransportManager::tDeviceList::const_iterator it_in =
      DeviceList.begin(); it_in != DeviceList.end(); ++it_in) {
    AddDeviceInDeviceListIfNotExist((*it_in));
  }
  if (mp_connection_handler_observer) {
    mp_connection_handler_observer->onDeviceListUpdated(m_deviceList);
    mp_connection_handler_observer->UpdateDeviceList(m_deviceList);
  }
}

bool CConnectionHandler::DoesDeviceExistInTMList(
    const NsSmartDeviceLink::NsTransportManager::tDeviceList & DeviceList,
    const NsConnectionHandler::tDeviceHandle DeviceHandle) {
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
  tDeviceListIterator it = m_deviceList.find(DeviceInfo.mDeviceHandle);
  if (m_deviceList.end() == it) {
    LOG4CPLUS_INFO(m_logger, "Adding new device!");
    m_deviceList.insert(
        tDeviceList::value_type(
            DeviceInfo.mDeviceHandle,
            CDevice(DeviceInfo.mDeviceHandle, DeviceInfo.mUserFriendlyName)));
  }
}

void CConnectionHandler::onApplicationConnected(
    const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & ConnectedDevice,
    const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection) {
  LOG4CPLUS_INFO(m_logger, "CConnectionHandler::onApplicationConnected()");
  tDeviceListIterator it = m_deviceList.find(ConnectedDevice.mDeviceHandle);
  if (m_deviceList.end() == it) {
    LOG4CPLUS_ERROR(m_logger, "Unknown device!");
    return;
  }
  LOG4CPLUS_INFO(m_logger, "Add Connection:" << Connection << " to the list.");
  m_connection_list.insert(
      tConnectionList::value_type(
          Connection, CConnection(Connection, ConnectedDevice.mDeviceHandle)));
}

void CConnectionHandler::onApplicationDisconnected(
    const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & DisconnectedDevice,
    const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection) {
  LOG4CPLUS_INFO(m_logger, "CConnectionHandler::onApplicationDisconnected()");
  tDeviceListIterator it = m_deviceList.find(DisconnectedDevice.mDeviceHandle);
  if (m_deviceList.end() == it) {
    LOG4CPLUS_ERROR(m_logger, "Unknown device!");
    return;
  }
  LOG4CPLUS_INFO(
      m_logger,
      "Delete Connection:" << static_cast<int>(Connection) << "from the list.");
  tConnectionListIterator itr = m_connection_list.find(Connection);
  if (m_connection_list.end() == itr) {
    LOG4CPLUS_ERROR(m_logger, "Connection not found!");
    return;
  } else {
    if (0 != mp_connection_handler_observer) {
      int firstSessionID = (itr->second).getFirstSessionID();
      if (0 < firstSessionID) {
        firstSessionID = keyFromPair(Connection, firstSessionID);
        // In case bot parameters of onSessionEndedCallback are the same
        // AppMgr knows that Application with id=firstSessionID
        // should be closed.
        mp_connection_handler_observer->onSessionEndedCallback(firstSessionID,
                                                             firstSessionID);
      }
    }
    m_connection_list.erase(itr);
  }

  if (m_connection_list.empty()) {
    m_deviceList.erase(it);
  }
}

void CConnectionHandler::RemoveConnection(
    const tConnectionHandle connection_handle) {

  LOG4CPLUS_INFO(m_logger, "CConnectionHandler::DisconnectApplication()");
  LOG4CPLUS_INFO(
      m_logger,
      "Delete Connection:" << static_cast<int>(connection_handle)
      << "from the list.");
  tConnectionListIterator itr = m_connection_list.find(connection_handle);
  if (m_connection_list.end() == itr) {
    LOG4CPLUS_ERROR(m_logger, "Connection not found!");
    return;
  } else {
    if (0 != mp_connection_handler_observer) {
      int firstSessionID = (itr->second).getFirstSessionID();
      if (0 < firstSessionID) {
        firstSessionID = keyFromPair(connection_handle, firstSessionID);
        // In case both parameters of onSessionEndedCallback are the same
        // AppMgr knows that Application with id=firstSessionID
        // should be closed.
        mp_connection_handler_observer->onSessionEndedCallback(firstSessionID,
                                                             firstSessionID);
      }
    }
    m_connection_list.erase(itr);
  }
}

int CConnectionHandler::onSessionStartedCallback(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle) {
  LOG4CPLUS_INFO(m_logger, "CConnectionHandler::onSessionStartedCallback()");
  int newSessionID = -1;
  tConnectionListIterator it = m_connection_list.find(connectionHandle);
  if (m_connection_list.end() == it) {
    LOG4CPLUS_ERROR(m_logger, "Unknown connection!");
  } else {
    newSessionID = (it->second).addNewSession();
    int firstSessionID = (it->second).getFirstSessionID();
    if (0 > newSessionID) {
      LOG4CPLUS_ERROR(m_logger, "Not possible to start session!");
    } else {
      LOG4CPLUS_INFO(m_logger,
                     "New session ID:" << static_cast<int>(newSessionID));
      if (0 != mp_connection_handler_observer) {
        if (0 < firstSessionID) {
          firstSessionID = keyFromPair(connectionHandle, firstSessionID);
        }
        int sessionKey = keyFromPair(connectionHandle, newSessionID);
        mp_connection_handler_observer->onSessionStartedCallback(
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
  LOG4CPLUS_INFO(m_logger, "CConnectionHandler::onSessionEndedCallback()");
  int result = -1;
  tConnectionListIterator it = m_connection_list.find(connectionHandle);
  if (m_connection_list.end() == it) {
    LOG4CPLUS_ERROR(m_logger, "Unknown connection!");
  } else {
    int firstSessionID = (it->second).getFirstSessionID();
    result = (it->second).removeSession(sessionId);
    if (0 > result) {
      LOG4CPLUS_ERROR(m_logger, "Not possible to remove session!");
    } else {
      LOG4CPLUS_INFO(m_logger, "Session removed:" << static_cast<int>(result));
      if (0 != mp_connection_handler_observer) {
        if (0 < firstSessionID) {
          firstSessionID = keyFromPair(connectionHandle, firstSessionID);
        }
        int sessionKey = keyFromPair(connectionHandle, sessionId);
        mp_connection_handler_observer->onSessionEndedCallback(sessionKey,
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
      m_logger,
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
      m_logger,
      "ConnectionHandle:" << static_cast<int>(connectionHandle) << " Session:"
      << static_cast<int>(sessionId) << " for key:" << static_cast<int>(key));
}

int CConnectionHandler::GetDataOnSessionKey(int key, int & app_id,
                                            std::list<int> & sessions_list,
                                            int & device_id) {
  LOG4CPLUS_INFO(m_logger, "CConnectionHandler::GetDataOnSessionKey()");
  int result = -1;
  NsSmartDeviceLink::NsTransportManager::tConnectionHandle conn_handle = 0;
  unsigned char session_id = 0;
  pairFromKey(key, conn_handle, session_id);
  tConnectionListIterator it = m_connection_list.find(conn_handle);
  if (m_connection_list.end() == it) {
    LOG4CPLUS_ERROR(m_logger, "Unknown connection!");
  } else {
    CConnection connection = it->second;
    device_id = connection.getConnectionDeviceHandle();
    int first_session_id = connection.getFirstSessionID();
    sessions_list.clear();
    if (0 == first_session_id) {
      LOG4CPLUS_INFO(
          m_logger,
          "No sessions in connection " << static_cast<int>(conn_handle) << ".");
      app_id = 0;
    } else {
      app_id = keyFromPair(conn_handle, first_session_id);
      tSessionList session_list;
      connection.GetSessionList(session_list);
      LOG4CPLUS_INFO(
          m_logger,
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
  LOG4CPLUS_INFO(m_logger, "CConnectionHandler::GetDataOnDeviceID()");
  int result = -1;
  tDeviceListIterator it = m_deviceList.find(device_id);
  if (m_deviceList.end() == it) {
    LOG4CPLUS_ERROR(m_logger, "Device not found!");
  } else {
    device_name = (*it).second.getUserFriendlyName();
    applications_list.clear();
    for (tConnectionListIterator itr = m_connection_list.begin();
        itr != m_connection_list.end(); ++itr) {
      if (device_id == (*itr).second.getConnectionDeviceHandle()) {
        applications_list.push_back((*itr).second.getFirstSessionID());
      }
    }
    result = 0;
  }
  return result;
}

void CConnectionHandler::setTransportManager(
    NsSmartDeviceLink::NsTransportManager::ITransportManager * transportManager) {
  LOG4CPLUS_INFO(m_logger, "CConnectionHandler::setTransportManager()");
  if (!transportManager) {
    LOG4CPLUS_ERROR(m_logger, "Null pointer to TransportManager.");
    return;
  }
  mp_transport_manager = transportManager;
}
void CConnectionHandler::startDevicesDiscovery() {
  LOG4CPLUS_INFO(m_logger, "CConnectionHandler::startDevicesDiscovery()");
  if (NULL == mp_transport_manager) {
    LOG4CPLUS_ERROR(m_logger, "Null pointer to TransportManager.");
    return;
  }
  mp_transport_manager->scanForNewDevices();
}

void CConnectionHandler::connectToDevice(
    NsConnectionHandler::tDeviceHandle deviceHandle) {
  NsConnectionHandler::tDeviceList::const_iterator it_in;
  it_in = m_deviceList.find(deviceHandle);
  if (m_deviceList.end() != it_in) {
    LOG4CPLUS_INFO_EXT(
        m_logger,
        "Connecting to device with handle " << static_cast<int>(deviceHandle));
    if (mp_transport_manager) {
      mp_transport_manager->connectDevice(deviceHandle);
    }
  } else {
    LOG4CPLUS_ERROR(
        m_logger,
        "Application Manager wanted to connect to non-existing device");
  }
}

void CConnectionHandler::startTransportManager() {
  LOG4CPLUS_INFO(m_logger, "CConnectionHandler::startTransportManager()");
  if (NULL == mp_transport_manager) {
    LOG4CPLUS_ERROR(m_logger, "Null pointer to TransportManager.");
    return;
  }

  mp_transport_manager->run();
}

}/* namespace NsConnectionHandler */
