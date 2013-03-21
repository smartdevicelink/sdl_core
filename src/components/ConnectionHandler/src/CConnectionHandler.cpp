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

#include "Logger.hpp"

#include "ConnectionHandler/CConnectionHandler.hpp"

/**
 * \namespace NsConnectionHandler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace NsConnectionHandler {

log4cplus::Logger CConnectionHandler::mLogger = log4cplus::Logger::getInstance(
    LOG4CPLUS_TEXT("ConnectionHandler"));

CConnectionHandler* CConnectionHandler::getInstance() {
  static CConnectionHandler instance;
  return &instance;
}

CConnectionHandler::CConnectionHandler()
    : mpConnectionHandlerObserver(NULL),
      mpTransportManager(NULL) {
}

CConnectionHandler::~CConnectionHandler() {
}

void CConnectionHandler::setConnectionHandlerObserver(
    IConnectionHandlerObserver * observer) {
  LOG4CPLUS_INFO(mLogger, "CConnectionHandler::setConnectionHandlerObserver()");
  if (!observer) {
    LOG4CPLUS_ERROR(mLogger, "Null pointer to observer.");
    return;
  }
  mpConnectionHandlerObserver = observer;
}

void CConnectionHandler::onDeviceListUpdated(
    const NsSmartDeviceLink::NsTransportManager::tDeviceList & DeviceList) {
  LOG4CPLUS_INFO(mLogger, "CConnectionHandler::onDeviceListUpdated()");
  for (tDeviceListIterator itr = mDeviceList.begin(); itr != mDeviceList.end();
      ++itr) {
    if (!DoesDeviceExistInTMList(DeviceList, (*itr).first)) {
      // Device has been removed. Perform all needed actions.
      // 1. Delete all the connections and sessions of this device
      // 2. Delete device from a list
      // 3. Let observer know that device has been deleted.
      tDeviceHandle device_for_remove_handle = (*itr).first;
      for (tConnectionListIterator it = mConnectionList.begin();
          it != mConnectionList.end(); ++it) {
        if (device_for_remove_handle == (*it).second.getConnectionDeviceHandle()) {
          RemoveConnection((*it).first);
        }
      }
      mDeviceList.erase(device_for_remove_handle);
      if (mpConnectionHandlerObserver) {
        mpConnectionHandlerObserver->RemoveDevice(device_for_remove_handle);
      }
    }
  }
  for (NsSmartDeviceLink::NsTransportManager::tDeviceList::const_iterator it_in =
      DeviceList.begin(); it_in != DeviceList.end(); ++it_in) {
    AddDeviceInDeviceListIfNotExist((*it_in));
  }
  if (mpConnectionHandlerObserver) {
    mpConnectionHandlerObserver->onDeviceListUpdated(mDeviceList);
    mpConnectionHandlerObserver->UpdateDeviceList(mDeviceList);
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
  tDeviceListIterator it = mDeviceList.find(DeviceInfo.mDeviceHandle);
  if (mDeviceList.end() == it) {
    LOG4CPLUS_ERROR(mLogger, "Adding new device!");
    mDeviceList.insert(
        tDeviceList::value_type(
            DeviceInfo.mDeviceHandle,
            CDevice(DeviceInfo.mDeviceHandle, DeviceInfo.mUserFriendlyName)));
  }
}

void CConnectionHandler::onApplicationConnected(
    const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & ConnectedDevice,
    const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection) {
  LOG4CPLUS_INFO(mLogger, "CConnectionHandler::onApplicationConnected()");
  tDeviceListIterator it = mDeviceList.find(ConnectedDevice.mDeviceHandle);
  if (mDeviceList.end() == it) {
    LOG4CPLUS_ERROR(mLogger, "Unknown device!");
    return;
  }
  LOG4CPLUS_INFO(mLogger, "Add Connection:" << Connection << " to the list.");
  mConnectionList.insert(
      tConnectionList::value_type(
          Connection, CConnection(Connection, ConnectedDevice.mDeviceHandle)));
}

void CConnectionHandler::onApplicationDisconnected(
    const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & DisconnectedDevice,
    const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection) {
  LOG4CPLUS_INFO(mLogger, "CConnectionHandler::onApplicationDisconnected()");
  tDeviceListIterator it = mDeviceList.find(DisconnectedDevice.mDeviceHandle);
  if (mDeviceList.end() == it) {
    LOG4CPLUS_ERROR(mLogger, "Unknown device!");
    return;
  }
  LOG4CPLUS_INFO(mLogger,
                 "Delete Connection:" << static_cast<int>(Connection)
                 << "from the list.");
  tConnectionListIterator itr = mConnectionList.find(Connection);
  if (mConnectionList.end() == itr) {
    LOG4CPLUS_ERROR(mLogger, "Connection not found!");
    return;
  } else {
    if (0 != mpConnectionHandlerObserver) {
      int firstSessionID = (itr->second).getFirstSessionID();
      if (0 < firstSessionID) {
        firstSessionID = keyFromPair(Connection, firstSessionID);
        // In case bot parameters of onSessionEndedCallback are the same
        // AppMgr knows that Application with id=firstSessionID
        // should be closed.
        mpConnectionHandlerObserver->onSessionEndedCallback(firstSessionID,
                                                            firstSessionID);
      }
    }
    mConnectionList.erase(itr);
  }

  if (mConnectionList.empty()) {
    mDeviceList.erase(it);
  }
}

void CConnectionHandler::RemoveConnection(
    const tConnectionHandle connection_handle) {

  LOG4CPLUS_INFO(mLogger, "CConnectionHandler::DisconnectApplication()");
  LOG4CPLUS_INFO(mLogger,
                 "Delete Connection:" << static_cast<int>(connection_handle)
                 << "from the list.");
  tConnectionListIterator itr = mConnectionList.find(connection_handle);
  if (mConnectionList.end() == itr) {
    LOG4CPLUS_ERROR(mLogger, "Connection not found!");
    return;
  } else {
    if (0 != mpConnectionHandlerObserver) {
      int firstSessionID = (itr->second).getFirstSessionID();
      if (0 < firstSessionID) {
        firstSessionID = keyFromPair(connection_handle, firstSessionID);
        // In case both parameters of onSessionEndedCallback are the same
        // AppMgr knows that Application with id=firstSessionID
        // should be closed.
        mpConnectionHandlerObserver->onSessionEndedCallback(firstSessionID,
                                                            firstSessionID);
      }
    }
    mConnectionList.erase(itr);
  }
}

int CConnectionHandler::onSessionStartedCallback(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle) {
  LOG4CPLUS_INFO(mLogger, "CConnectionHandler::onSessionStartedCallback()");
  int newSessionID = -1;
  tConnectionListIterator it = mConnectionList.find(connectionHandle);
  if (mConnectionList.end() == it) {
    LOG4CPLUS_ERROR(mLogger, "Unknown connection!");
  } else {
    newSessionID = (it->second).addNewSession();
    int firstSessionID = (it->second).getFirstSessionID();
    if (0 > newSessionID) {
      LOG4CPLUS_ERROR(mLogger, "Not possible to start session!");
    } else {
      LOG4CPLUS_INFO(mLogger, "New session ID:"
                     << static_cast<int>(newSessionID));
      if (0 != mpConnectionHandlerObserver) {
        if (0 < firstSessionID) {
          firstSessionID = keyFromPair(connectionHandle, firstSessionID);
        }
        int sessionKey = keyFromPair(connectionHandle, newSessionID);
        mpConnectionHandlerObserver->onSessionStartedCallback(
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
  LOG4CPLUS_INFO(mLogger, "CConnectionHandler::onSessionEndedCallback()");
  int result = -1;
  tConnectionListIterator it = mConnectionList.find(connectionHandle);
  if (mConnectionList.end() == it) {
    LOG4CPLUS_ERROR(mLogger, "Unknown connection!");
  } else {
    int firstSessionID = (it->second).getFirstSessionID();
    result = (it->second).removeSession(sessionId);
    if (0 > result) {
      LOG4CPLUS_ERROR(mLogger, "Not possible to remove session!");
    } else {
      LOG4CPLUS_INFO(mLogger, "Session removed:" << static_cast<int>(result));
      if (0 != mpConnectionHandlerObserver) {
        if (0 < firstSessionID) {
          firstSessionID = keyFromPair(connectionHandle, firstSessionID);
        }
        int sessionKey = keyFromPair(connectionHandle, sessionId);
        mpConnectionHandlerObserver->onSessionEndedCallback(sessionKey,
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
      mLogger,
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
      mLogger,
      "ConnectionHandle:" << static_cast<int>(connectionHandle)
      << " Session:" << static_cast<int>(sessionId) << " for key:"
      << static_cast<int>(key));
}

void CConnectionHandler::setTransportManager(
    NsSmartDeviceLink::NsTransportManager::ITransportManager * transportManager) {
  LOG4CPLUS_INFO(mLogger, "CConnectionHandler::setTransportManager()");
  if (!transportManager) {
    LOG4CPLUS_ERROR(mLogger, "Null pointer to TransportManager.");
    return;
  }
  mpTransportManager = transportManager;
}
void CConnectionHandler::startDevicesDiscovery() {
  LOG4CPLUS_INFO(mLogger, "CConnectionHandler::startDevicesDiscovery()");
  if (NULL == mpTransportManager) {
    LOG4CPLUS_ERROR(mLogger, "Null pointer to TransportManager.");
    return;
  }
  mpTransportManager->scanForNewDevices();
}

void CConnectionHandler::connectToDevice(
    NsConnectionHandler::tDeviceHandle deviceHandle) {
  NsConnectionHandler::tDeviceList::const_iterator it_in;
  it_in = mDeviceList.find(deviceHandle);
  if (mDeviceList.end() != it_in) {
    LOG4CPLUS_INFO_EXT(
        mLogger, "Connecting to device with handle "
        << static_cast<int>(deviceHandle));
    if (mpTransportManager) {
      mpTransportManager->connectDevice(deviceHandle);
    }
  } else {
    LOG4CPLUS_ERROR(
        mLogger,
        "Application Manager wanted to connect to non-existing device");
  }
}

void CConnectionHandler::startTransportManager() {
  LOG4CPLUS_INFO(mLogger, "CConnectionHandler::startTransportManager()");
  if (NULL == mpTransportManager) {
    LOG4CPLUS_ERROR(mLogger, "Null pointer to TransportManager.");
    return;
  }

  mpTransportManager->run();
}

}/* namespace NsConnectionHandler */
