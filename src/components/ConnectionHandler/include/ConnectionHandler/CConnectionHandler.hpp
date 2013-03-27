/**
 * \file CConnectionHandler.hpp
 * \brief Connection handler class.
 * Observes TransportManager and ProtocolHandler, stores information regarding connections
 * and sessions and provides it to AppManager.
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

#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <map>
#include <list>

#include "Logger.hpp"
#include "TransportManager/ITransportManagerDeviceListener.hpp"
#include "TransportManager/SDeviceInfo.hpp"
#include "ProtocolHandler/ISessionObserver.h"
#include "ConnectionHandler/IConnectionHandlerObserver.hpp"
#include "ConnectionHandler/CDevice.hpp"
#include "ConnectionHandler/CConnection.hpp"
#include "ConnectionHandler/IDevicesDiscoveryStarter.hpp"
#include "ConnectionHandler/IConnectionHandler.hpp"
/**
 * \namespace NsConnectionHandler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace NsConnectionHandler {
/**
 * \class CConnectionHandler
 * \brief SmartDeviceLink ConnectionHandler main class
 */
class CConnectionHandler :
    public NsSmartDeviceLink::NsTransportManager::ITransportManagerDeviceListener,
    public NsProtocolHandler::ISessionObserver, public IDevicesDiscoveryStarter,
    public IConnectionHandler {
 public:
  /**
   * \brief Singletone instantiator.
   * \return pointer to CConnectionHandler instance.
   */
  static CConnectionHandler* getInstance();

  /**
   * \brief Destructor
   */
  ~CConnectionHandler();

  /**
   * \brief Sets observer pointer for ConnectionHandler.
   * \param observer Pointer to observer object.
   **/
  virtual void set_connection_handler_observer(IConnectionHandlerObserver * observer);

  /**
   * \brief Available devices list updated.
   *
   * Called when device scanning initiated with scanForNewDevices
   * is completed.
   *
   * \param DeviceList New list of available devices.
   **/
  virtual void onDeviceListUpdated(
      const NsSmartDeviceLink::NsTransportManager::tDeviceList & DeviceList);

  /**
   * \brief Application connected.
   *
   * \param ConnectedDevice DeviceInfo of connected device.
   * \param Connection Connection handle.
   **/
  virtual void onApplicationConnected(
      const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & ConnectedDevice,
      const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection);

  /**
   * \brief Application disconnected.
   *
   * \param DisconnectedDevice DeviceInfo of disconnected device.
   * \param Connection Connection handle.
   **/
  virtual void onApplicationDisconnected(
      const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & DisconnectedDevice,
      const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection);

  /**
   * \brief Callback function used by ProtocolHandler
   * when Mobile Application initiates start of new session.
   * \param connectionHandle Connection identifier whithin which session has to be started.
   * \return int Id (number) of new session if successful otherwise -1.
   */
  virtual int onSessionStartedCallback(
      NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle);

  /**
   * \brief Callback function used by ProtocolHandler
   * when Mobile Application initiates session ending.
   * \param connectionHandle Connection identifier whithin which session exists
   * \param sessionId Identifier of the session to be ended
   * \param hashCode Hash used only in second version of SmartDeviceLink protocol.
   * If not equal to hash assigned to session on start then operation fails.
   * \return int -1 if operation fails session key otherwise
   */
  virtual int onSessionEndedCallback(
      NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
      unsigned char sessionId, unsigned int hashCode);

  /**
   * \brief Creates unique identifier of session (can be used as hash)
   * from given connection identifier
   * whithin which session exists and session number.
   * \param  connectionHandle Connection identifier whithin which session exists
   * \param sessionId Identifier of the session
   * \return int Unique key for session
   */
  virtual int keyFromPair(
      NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
      unsigned char sessionId);

  /**
   * \brief Returns connection identifier and session number from given session key
   * \param key Unique key used by other components as session identifier
   * \param connectionHandle Returned: Connection identifier whithin which session exists
   * \param sessionId Returned: Number of session
   */
  virtual void pairFromKey(
      int key,
      NsSmartDeviceLink::NsTransportManager::tConnectionHandle & connectionHandle,
      unsigned char & sessionId);

  /**
   * \brief information about given Connection Key.
   * \param key Unique key used by other components as session identifier
   * \param app_id Returned: ApplicationID
   * \param sessions_list Returned: List of session keys
   * \param device_id Returned: DeviceID
   * \return int -1 in case of error or 0 in case of success
   */
  virtual int GetDataOnSessionKey(int key, int & app_id,
                                     std::list<int> & sessions_list,
                                     int & device_id);

  /**
   * \brief information about given Connection Key.
   * \param key Unique key used by other components as session identifier
   * \param app_id Returned: ApplicationID
   * \param sessions_list Returned: List of session keys
   * \param device_id Returned: DeviceID
   * \return int -1 in case of error or 0 in case of success
   */
  virtual int GetDataOnDeviceID(int device_id, std::string & device_name,
                                     std::list<int> & applications_list);

  /**
   * \brief Sets pointer to TransportManager.
   * \param transportManager Pointer to TransportManager object.
   **/
 virtual void set_transport_manager(
      NsSmartDeviceLink::NsTransportManager::ITransportManager * transportManager);

  /**
   * \brief Method which should start devices discoveryng
   */
  virtual void startDevicesDiscovery();

  /**
   * \brief Connects to all services of device
   * \param deviceHandle Handle of device to connect to
   */
  virtual void connectToDevice(NsConnectionHandler::tDeviceHandle deviceHandle);

  virtual void StartTransportManager();

 private:
  /**
   * \brief Default class constructor
   */
  CConnectionHandler();

  /**
   * \brief Copy constructor
   */
  CConnectionHandler(const CConnectionHandler&);

  /**
   * \brief Checks does device exist in list from TransportManager
   * \param DeviceHandle Handle of device for checking.
   * \param DeviceHandle Handle of device for checking.
  * \return True if device exists.
   */
  bool DoesDeviceExistInTMList(
      const NsSmartDeviceLink::NsTransportManager::tDeviceList & DeviceList,
      const NsConnectionHandler::tDeviceHandle DeviceHandle);

  /**
   * \brief Checks does device exist in list and adds if not
   * \param DeviceHandle Handle of device for checking.
   */
  void AddDeviceInDeviceListIfNotExist(
      const NsSmartDeviceLink::NsTransportManager::SDeviceInfo DeviceInfo);

  /**
   * \brief Disconnect application.
   *
   * \param device_handle DeviceHandle of disconnected device.
   * \param connection_handle Connection handle.
   **/
  void RemoveConnection(
      const tConnectionHandle connection_handle);

  /**
   * \brief Pointer to observer
   */
  IConnectionHandlerObserver* connection_handler_observer_;

  /**
   * \brief Pointer to TransportManager
   */
  NsSmartDeviceLink::NsTransportManager::ITransportManager * transport_manager_;

  /**
   * \brief List of devices
   */
  tDeviceList device_list_;

  /**
   * \brief List of connections
   */
  tConnectionList connection_list_;

  /**
   *\brief For logging.
   */
  static log4cplus::Logger logger_;
};
}/* namespace NsConnectionHandler */

#endif /* CONNECTIONHANDLER_H */
