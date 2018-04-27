/*
 * Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_CONNECTION_HANDLER_IMPL_H_
#define SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_CONNECTION_HANDLER_IMPL_H_

#include <map>
#include <list>
#include <string>
#include <vector>

#include "transport_manager/transport_manager_listener_empty.h"
#include "protocol_handler/session_observer.h"
#include "protocol_handler/protocol_handler.h"
#include "connection_handler/connection_handler_observer.h"
#include "connection_handler/device.h"
#include "connection_handler/connection.h"
#include "connection_handler/devices_discovery_starter.h"
#include "connection_handler/connection_handler.h"

#include "utils/logger.h"
#include "utils/macro.h"
#include "utils/message_queue.h"
#include "utils/lock.h"
#include "utils/stl_utils.h"
#include "utils/rwlock.h"

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink connection_handler namespace.
 */
namespace connection_handler {

/**
 * \class ConnectionHandlerImpl
 * \brief SmartDeviceLink connection_handler main class
 * Observes TransportManager and ProtocolHandler,
 * stores information regarding connections
 * and sessions and provides it to AppManager.
 */
class ConnectionHandlerImpl
    : public ConnectionHandler,
      public transport_manager::TransportManagerListenerEmpty,
      public protocol_handler::SessionObserver,
      public DevicesDiscoveryStarter {
 public:
  /**
   * @brief ConnectionHandlerImpl
   */
  ConnectionHandlerImpl(const ConnectionHandlerSettings& settings,
                        transport_manager::TransportManager& tm);
  /**
   * \brief Destructor
   */
  ~ConnectionHandlerImpl();

  void Stop();
  /**
   * \brief Sets observer pointer for connection_handler.
   * \param observer Pointer to observer object.
   */
  void set_connection_handler_observer(
      ConnectionHandlerObserver* observer) OVERRIDE;

  /**
   * \brief Sets pointer to ProtocolHandler.
   * \param protocol_handler Pointer to ProtocolHandler object.
   **/
  void set_protocol_handler(
      protocol_handler::ProtocolHandler* protocol_handler);

  /**
   * \brief Connects to all services of device
   * \param deviceHandle Handle of device to connect to
   */
  void ConnectToDevice(connection_handler::DeviceHandle device_handle) OVERRIDE;

  /**
   * @brief RunAppOnDevice allows to run specific application on the certain
   *device.
   *
   * @param device_handle device identifier to run application on.
   *
   * @param app_id application id also known as bundle id on some devices to
   *run.
   */
  void RunAppOnDevice(const std::string& device_mac,
                      const std::string& bundle_id) const OVERRIDE;

  void ConnectToAllDevices() OVERRIDE;

  void StartTransportManager() OVERRIDE;

  void OnDeviceListUpdated(
      const std::vector<transport_manager::DeviceInfo>&) OVERRIDE;

  void OnFindNewApplicationsRequest() OVERRIDE;

  /**
   * \brief Available devices list updated.
   *
   * Called when device scanning initiated with scanForNewDevices
   * is completed or devices connected via background procedures.
   *
   * \param DeviceList New list of available devices.
   **/
  void OnDeviceFound(const transport_manager::DeviceInfo& device_info) OVERRIDE;
  void OnDeviceAdded(const transport_manager::DeviceInfo& device_info) OVERRIDE;
  void OnDeviceRemoved(
      const transport_manager::DeviceInfo& device_info) OVERRIDE;

  /**
   * @brief OnDeviceSwitchingStart notifies listeners on device transport
   * switching start
   * @param device_uid_from the id of the device which has to switch its
   * transport
   * @param device_uid_to the id of the device on new transport
   */
  void OnDeviceSwitchingStart(const std::string& device_uid_from,
                              const std::string& device_uid_to) FINAL;

  /**
   * @brief OnDeviceSwitchingFinish notifies listeners on device transport
   * switching completion
   * @param device_uid the id for the device which is fails to reconnect.
   */
  void OnDeviceSwitchingFinish(
      const transport_manager::DeviceUID& device_uid) FINAL;

  void OnScanDevicesFinished() OVERRIDE;
  void OnScanDevicesFailed(
      const transport_manager::SearchDeviceError& error) OVERRIDE;

  /**
   * \brief Notifies about established connection.
   *
   * \param connection_id ID of new connection.
   **/
  void OnConnectionEstablished(
      const transport_manager::DeviceInfo& device_info,
      const transport_manager::ConnectionUID connection_id) OVERRIDE;
  void OnConnectionFailed(
      const transport_manager::DeviceInfo& device_info,
      const transport_manager::ConnectError& error) OVERRIDE;
  void OnConnectionClosed(
      transport_manager::ConnectionUID connection_id) OVERRIDE;
  void OnConnectionClosedFailure(
      transport_manager::ConnectionUID connection_id,
      const transport_manager::DisconnectError& error) OVERRIDE;
  void OnUnexpectedDisconnect(
      transport_manager::ConnectionUID connection_id,
      const transport_manager::CommunicationError& error) OVERRIDE;
  void OnDeviceConnectionLost(
      const connection_handler::DeviceHandle& device,
      const transport_manager::DisconnectDeviceError& error) OVERRIDE;
  /**
   * \brief Informs about failure during DisconnectDevice procedure of TM
   * \param device Information about disconnected device
   * \param error Information about possible reason of loosing connection
   */
  void OnDisconnectFailed(
      const connection_handler::DeviceHandle& device,
      const transport_manager::DisconnectDeviceError& error) OVERRIDE;

  /**
   * \brief Callback function used by ProtocolHandler
   * when Mobile Application initiates start of new session.
   * \param connection_handle Connection identifier within which session has to
   * be started.
   * \param session_id Identifier of the session to be started
   * \param service_type Type of service
   * \param is_protected would be service protected
   * \param hash_id pointer for session hash identifier
   * \return uint32_t Id (number) of new session if successful, otherwise 0.
   * \deprecated
   */
  virtual uint32_t OnSessionStartedCallback(
      const transport_manager::ConnectionUID connection_handle,
      const uint8_t session_id,
      const protocol_handler::ServiceType& service_type,
      const bool is_protected,
      uint32_t* hash_id);

  /**
   * \brief Callback function used by ProtocolHandler
   * when Mobile Application initiates start of new session.
   * Result must be notified through NotifySessionStartedContext().
   * \param connection_handle Connection identifier within which session
   * has to be started.
   * \param sessionId Identifier of the session to be start
   * \param service_type Type of service
   * \param protocol_version Version of protocol
   * \param is_protected would be service protected
   * \param params configuration parameters specified by mobile
   */
  virtual void OnSessionStartedCallback(
      const transport_manager::ConnectionUID connection_handle,
      const uint8_t session_id,
      const protocol_handler::ServiceType& service_type,
      const bool is_protected,
      const BsonObject* params);
  /**
   * \brief Callback function used by ProtocolHandler
   * when Mobile Application initiates session ending.
   * \param connection_handle Connection identifier within which session exists
   * \param sessionId Identifier of the session to be ended
   * \param hashCode Hash used only in second version of SmartDeviceLink
   * protocol.
   * If not equal to hash assigned to session on start then operation fails.
   * \return uint32_t 0 if operation fails, session key otherwise
   * \deprecated
   */
  uint32_t OnSessionEndedCallback(
      const transport_manager::ConnectionUID connection_handle,
      const uint8_t session_id,
      const uint32_t& hashCode,
      const protocol_handler::ServiceType& service_type) OVERRIDE;
  /**
   * \brief Callback function used by ProtocolHandler
   * when Mobile Application initiates session ending.
   * \param connection_handle Connection identifier within which session exists
   * \param sessionId Identifier of the session to be ended
   * \param hashCode Hash used only in second version of SmartDeviceLink
   * protocol. (Set to HASH_ID_WRONG if the hash is incorrect)
   * If not equal to hash assigned to session on start then operation fails.
   * \return uint32_t 0 if operation fails, session key otherwise
   */
  uint32_t OnSessionEndedCallback(
      const transport_manager::ConnectionUID connection_handle,
      const uint8_t session_id,
      uint32_t* hashCode,
      const protocol_handler::ServiceType& service_type) OVERRIDE;

  /**
   * \brief Callback function used by ProtocolHandler
   * when Mobile Application start message flood
   * \param connection_key  used by other components as application identifier
   */
  void OnApplicationFloodCallBack(const uint32_t& connection_key) OVERRIDE;

  /**
   * \brief Callback function used by ProtocolHandler
   * when Mobile Application sends malformed message
   * \param connection_key  used by other components as application identifier
   */
  void OnMalformedMessageCallback(const uint32_t& connection_key) OVERRIDE;

  /**
   * @brief Converts connection handle to transport type string used in
   * smartDeviceLink.ini file, e.g. "TCP_WIFI"
   * @param connection_handle A connection identifier
   * @return string representation of the transport of the device
   */
  const std::string TransportTypeProfileStringFromConnHandle(
    transport_manager::ConnectionUID connection_handle) const;

  /**
   * @brief Converts device handle to transport type string used in
   * smartDeviceLink.ini file, e.g. "TCP_WIFI"
   * @param device_handle A device handle
   * @return string representation of the transport of the device
   */
  const std::string TransportTypeProfileStringFromDeviceHandle(
    DeviceHandle device_handle) const;

  /**
   * \brief Creates unique identifier of session (can be used as hash)
   * from given connection identifier
   * within which session exists and session number.
   * \param  connection_handle Connection identifier within which session exists
   * \param sessionId Identifier of the session
   * \return int32_t Unique key for session
   */
  uint32_t KeyFromPair(transport_manager::ConnectionUID connection_handle,
                       uint8_t session_id) const OVERRIDE;

  /**
   * \brief Returns connection identifier and session number from given session
   * key
   * \param key Unique key used by other components as session identifier
   * \param connection_handle Returned: Connection identifier within which
   * session exists
   * \param sessionId Returned: Number of session
   */
  void PairFromKey(uint32_t key,
                   transport_manager::ConnectionUID* connection_handle,
                   uint8_t* session_id) const OVERRIDE;

  /**
   * \brief information about device
   * \param device_handle
   * \param device_name Returned: name of device
   * \param applications_list Returned: applications on device
   * \param mac_address Returned: List of session keys
   * \param connection_type Returned: type of connection (USB, BT, etc.)
   * \return int32_t -1 in case of error or 0 in case of success
   */
  int32_t GetDataOnDeviceID(connection_handler::DeviceHandle device_handle,
                            std::string* device_name = NULL,
                            std::list<uint32_t>* applications_list = NULL,
                            std::string* mac_address = NULL,
                            std::string* connection_type = NULL) const OVERRIDE;

  /**
   * @brief GetConnectedDevicesMAC allows to obtain MAC adresses for all
   * currently connected devices.
   *
   * @param device_macs collection of MAC adresses for connected devices.
   */
  void GetConnectedDevicesMAC(
      std::vector<std::string>& device_macs) const OVERRIDE;

#ifdef ENABLE_SECURITY
  /**
   * \brief Sets crypto context of connection
   * \param key Unique key used by other components as session identifier
   * \param context SSLContext to be set
   * \return \c SecurityQuery::ProtectSessionResult value
   */
  int SetSSLContext(const uint32_t& key,
                    security_manager::SSLContext* context) OVERRIDE;

  /**
   * \brief Gets crypto context of connection, use service_type to get NULL
   * SSLContext for not protected services or ControlService (0x0)
   * to get current SSLContext of connection
   * \param key Unique key used by other components as session identifier
   * \param service_type Type of service
   * \return \ref SSLContext of connection
   */
  security_manager::SSLContext* GetSSLContext(
      const uint32_t& key,
      const protocol_handler::ServiceType& service_type) OVERRIDE;
  /**
   * \brief Set protection flag to service in session by key
   * \param key Unique key used by other components as session identifier
   * \param service_type Type of service
   */
  void SetProtectionFlag(
      const uint32_t& key,
      const protocol_handler::ServiceType& service_type) OVERRIDE;

  security_manager::SSLContext::HandshakeContext GetHandshakeContext(
      uint32_t key) const OVERRIDE;
#endif  // ENABLE_SECURITY

  /**
   * @brief Check if session contains service with specified service type
   * @param connection_key unique id of session to check
   * @param service_type type of service to check
   * @return true if session contains service with specified service type
   */
  bool SessionServiceExists(
      const uint32_t connection_key,
      const protocol_handler::ServiceType& service_type) const OVERRIDE;

  /**
   * \brief Get device handle by mac address
   * \param mac_address uniq address
   * \param device_handle
   * \return true on sucess otherwise false.
   */
  bool GetDeviceID(const std::string& mac_address,
                   DeviceHandle* device_handle) OVERRIDE;

  /**
   * \brief Method which should start devices discovering
   */
  void StartDevicesDiscovery() OVERRIDE;

  /**
   * @brief Close the connection revoked by Policy
   * @param connection_key pair of connection and session id
   */
  void CloseRevokedConnection(uint32_t connection_key) OVERRIDE;

  /**
   * @brief Close the connection pointed by handle
   * @param connection_handle Connection unique id
   */
  void CloseConnection(ConnectionHandle connection_handle) OVERRIDE;

  /**
   * \brief Close session associated with the key
   * \param key Unique key used by other components as session identifier
   */
  void CloseSession(uint32_t key, CloseSessionReason close_reason) OVERRIDE;

  /**
   * \brief Function used by HearbeatMonitior to close session on HB timeout
   * \param connection_handle Connection handler within which session exists
   * \param session_id Identifier of the session to be ended
   */
  void CloseSession(ConnectionHandle connection_handle,
                    uint8_t session_id,
                    CloseSessionReason close_reason) OVERRIDE;

  /**
   * \brief Function used by OnApplicationFloodCallback and
   * OnMalformedMessageCallback to close all connection sessions before
   * connection closing
   * \param connection_handle Connection identifier within which session exists
   * \param close_reason The reason of connection closing
   */
  virtual void CloseConnectionSessions(ConnectionHandle connection_handle,
                                       CloseSessionReason close_reason);

  /**
   * \brief Return count of session for specified connection
   * \param connection_key pair of connection handle and session id
   */
  uint32_t GetConnectionSessionsCount(uint32_t connection_key) OVERRIDE;

  /**
   * \brief Send heartbeat message to mobile app
   */
  void SendHeartBeat(ConnectionHandle connection_handle,
                     uint8_t session_id) OVERRIDE;

  /**
   * @brief SendEndService allows to end up specific service.
   *
   * @param key application identifier whose service should be closed.
   *
   * @param service_type the service that should be closed.
   */
  void SendEndService(uint32_t key, uint8_t service_type) OVERRIDE;

  /**
   * \brief Start heartbeat for specified session
   *
   * \param connection_key pair of connection and session id
   */
  void StartSessionHeartBeat(uint32_t connection_key) OVERRIDE;

  /**
   * Sets heart beat timeout for specified session
   * @param connection_key pair of connection and session id
   * @param timeout in milliseconds
   */
  void SetHeartBeatTimeout(uint32_t connection_key, uint32_t timeout) OVERRIDE;

  /**
   * \brief Keep connection associated with the key from being closed by
   * heartbeat monitor
   */
  void KeepConnectionAlive(uint32_t connection_key, uint8_t session_id);

  /**
   * \brief binds protocol version with session
   *
   * \param connection_key pair of connection and session id
   * \param protocol_version contains protocol version of
   * \registered application.
   */
  void BindProtocolVersionWithSession(uint32_t connection_key,
                                      uint8_t protocol_version) OVERRIDE;

  /**
   * \brief returns TRUE if session supports sending HEART BEAT ACK to mobile
   * side
   * \param  connection_handle Connection identifier whithin which session
   * exists
   * \param sessionId Identifier of the session
   * \return TRUE if session has protocol version which supports heartbeat
   * otherwise returns FALSE
   */
  bool IsHeartBeatSupported(transport_manager::ConnectionUID connection_handle,
                            uint8_t session_id) const OVERRIDE;

  /**
   * @brief returns protocol version which application supports
   * @param connection_id id of connection
   * @param session_id id of session
   * @param method writes value protocol version to protocol_version
   * @return TRUE if session and connection exist otherwise returns FALSE
   */
  bool ProtocolVersionUsed(uint32_t connection_id,
                           uint8_t session_id,
                           uint8_t& protocol_version) const OVERRIDE;

  /**
   * \brief information about given Connection Key.
   * \param key Unique key used by other components as session identifier
   * \param app_id Returned: ApplicationID
   * \param sessions_list Returned: List of session keys
   * \param device_id Returned: DeviceID
   * \return int32_t -1 in case of error or 0 in case of success
   */
  int32_t GetDataOnSessionKey(
      uint32_t key,
      uint32_t* app_id,
      std::list<int32_t>* sessions_list,
      connection_handler::DeviceHandle* device_id) const OVERRIDE;

#if __SIZEOF_SIZE_T__ != 4
  /**
   * DEPRECATED
   * \brief information about given Connection Key.
   * \param key Unique key used by other components as session identifier
   * \param app_id Returned: ApplicationID
   * \param sessions_list Returned: List of session keys
   * \param device_id Returned: DeviceID
   * \return int32_t -1 in case of error or 0 in case of success
   */
  int32_t GetDataOnSessionKey(uint32_t key,
                              uint32_t* app_id,
                              std::list<int32_t>* sessions_list,
                              uint32_t* device_id) const OVERRIDE;
#endif

  const ConnectionHandlerSettings& get_settings() const OVERRIDE;

  const protocol_handler::SessionObserver& get_session_observer();
  DevicesDiscoveryStarter& get_device_discovery_starter();

  NonConstDataAccessor<SessionConnectionMap> session_connection_map() OVERRIDE;

  /**
   * \brief Associate a secondary transport ID with a session
   * \param session_id the session ID
   * \param connection_id the new secondary connection ID to associate with the
   * session
   * \return the SessionTransports (newly) associated with the session
   **/
  SessionTransports SetSecondaryTransportID(
      uint8_t session_id,
      transport_manager::ConnectionUID secondary_transport_id) OVERRIDE;

  /**
   * \brief Retrieve the session transports associated with a session
   * \param session_id the session ID
   * \return the SessionTransports associated with the session
   **/
  const SessionTransports GetSessionTransports(
      uint8_t session_id) const OVERRIDE;

  /**
   * \brief Invoked when observer's OnServiceStartedCallback is completed
   * \param session_key the key of started session passed to
   * OnServiceStartedCallback().
   * \param result true if observer accepts starting service, false otherwise
   * \param rejected_params list of rejected parameters' name. Only valid when
   * result is false. Note that even if result is false, this may be empty.
   *
   * \note This is invoked only once but can be invoked by multiple threads.
   * Also it can be invoked before OnServiceStartedCallback() returns.
   **/
  virtual void NotifyServiceStartedResult(
      uint32_t session_key,
      bool result,
      std::vector<std::string>& rejected_params);

  /**
   * \brief Called when secondary transport with given session ID is established
   * \param primary_connection_handle Set to identifier of primary connection
   * \param secondary_connection_handle Identifier of secondary connection
   * \param sessionid session ID taken from Register Secondary Transport frame
   **/
  bool OnSecondaryTransportStarted(
      transport_manager::ConnectionUID& primary_connection_handle,
      const transport_manager::ConnectionUID secondary_connection_handle,
      const uint8_t session_id) OVERRIDE;

  /**
   * \brief Called when secondary transport shuts down
   * \param primary_connection_handle Identifier of primary connection
   * \param secondary_connection_handle Identifier of secondary connection
   * transport
   **/
  void OnSecondaryTransportEnded(
      const transport_manager::ConnectionUID primary_connection_handle,
      const transport_manager::ConnectionUID secondary_connection_handle)
      OVERRIDE;

 private:
  /**
   * \brief Disconnect application.
   *
   * \param device_handle DeviceHandle of disconnected device.
   * \param connection_handle Connection handle.
   **/
  void RemoveConnection(const ConnectionHandle connection_handle);

  void OnConnectionEnded(const transport_manager::ConnectionUID connection_id);

  const uint8_t GetSessionIdFromSecondaryTransport(
      transport_manager::ConnectionUID secondary_transport_id) const;

  const ConnectionHandlerSettings& settings_;
  /**
   * \brief Pointer to observer
   */
  ConnectionHandlerObserver* connection_handler_observer_;

  /**
   * \brief Pointer to TransportManager
   */
  transport_manager::TransportManager& transport_manager_;

  protocol_handler::ProtocolHandler* protocol_handler_;

  /**
   * \brief List of devices
   */
  DeviceMap device_list_;

  /**
   * @brief session/connection map
   */
  SessionConnectionMap session_connection_map_;
  mutable sync_primitives::Lock session_connection_map_lock_;

  /**
   * \brief List of connections
   */
  ConnectionList connection_list_;

  /**
   *  \brief Lock for applications list
   */
  mutable sync_primitives::RWLock connection_list_lock_;
  mutable sync_primitives::RWLock connection_handler_observer_lock_;

  /**
   * \brief Cleans connection list on destruction
   */
  utils::StlMapDeleter<ConnectionList> connection_list_deleter_;

  sync_primitives::Lock start_service_context_map_lock_;
  std::map<uint32_t, protocol_handler::SessionContext>
      start_service_context_map_;

  /**
   * @brief connection object as it's being closed
   */
  Connection* ending_connection_;

#ifdef BUILD_TESTS
  // Methods for test usage
 public:
  const DeviceMap& getDeviceList();
  ConnectionList& getConnectionList();
  void addDeviceConnection(
      const transport_manager::DeviceInfo& device_info,
      const transport_manager::ConnectionUID connection_id);
#endif
 private:
  DISALLOW_COPY_AND_ASSIGN(ConnectionHandlerImpl);
};
}  // namespace connection_handler
#endif  // SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_CONNECTION_HANDLER_IMPL_H_
