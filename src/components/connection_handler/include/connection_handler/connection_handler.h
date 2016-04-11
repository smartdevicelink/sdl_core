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

#ifndef SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_CONNECTION_HANDLER_H_
#define SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_CONNECTION_HANDLER_H_

#include "connection_handler/connection_handler_settings.h"
#include "transport_manager/transport_manager_listener.h"
#include "protocol_handler/session_observer.h"
#include "connection_handler/device.h"
#include "connection_handler/connection.h"
#include "connection_handler/devices_discovery_starter.h"

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink connection_handler namespace.
 */
namespace connection_handler {

enum CloseSessionReason { kCommon = 0, kFlood, kMalformed, kUnauthorizedApp };

class ConnectionHandlerObserver;

/**
 * \class ConnectionHandler
 * \brief SmartDeviceLink ConnectionHandler interface class
 */
class ConnectionHandler {
 public:
  /**
   * \brief Sets observer pointer for ConnectionHandler.
   * \param observer Pointer to observer object.
   **/
  virtual void set_connection_handler_observer(
      ConnectionHandlerObserver* observer) = 0;

  virtual void StartTransportManager() = 0;

  virtual void ConnectToDevice(
      connection_handler::DeviceHandle device_handle) = 0;

  virtual void ConnectToAllDevices() = 0;

  /**
   * @brief  Close the connection revoked by Policy
   * @param connection_key pair of connection and session id
   */
  virtual void CloseRevokedConnection(uint32_t connection_key) = 0;

  /**
   * \brief Close all associated sessions and close the connection pointed by
   * handle
   */
  virtual void CloseConnection(ConnectionHandle connection_handle) = 0;

  /**
   * \brief Return count of session for specified connection
   * \param connection_key pair of connection handle and session id
   */
  virtual uint32_t GetConnectionSessionsCount(uint32_t connection_key) = 0;

  /**
   * Gets device id by mac address
   * @param mac_address
   * @return true if successfully
   */
  virtual bool GetDeviceID(const std::string& mac_address,
                           DeviceHandle* device_handle) = 0;

  /**
   * Close session associated with the key
   */
  virtual void CloseSession(uint32_t key, CloseSessionReason close_reason) = 0;

  /**
   * Close session
   */
  virtual void CloseSession(ConnectionHandle connection_handle,
                            uint8_t session_id,
                            CloseSessionReason close_reason) = 0;

  /**
 * @brief SendEndService allows to end up specific service.
 *
 * @param key application identifier whose service should be closed.
 *
 * @param service_type the service that should be closed.
 */
  virtual void SendEndService(uint32_t key, uint8_t service_type) = 0;

  /**
   * \brief Start heartbeat for specified session
   *
   * \param connection_key pair of connection and session id
   */
  virtual void StartSessionHeartBeat(uint32_t connection_key) = 0;

  /**
   * \brief Send heartbeat to mobile app
   */
  virtual void SendHeartBeat(ConnectionHandle connection_handle,
                             uint8_t session_id) = 0;

  /**
   * Sets heart beat timeout for specified session
   * @param connection_key pair of connection and session id
   * @param timeout in milliseconds
   */
  virtual void SetHeartBeatTimeout(uint32_t connection_key,
                                   uint32_t timeout) = 0;

  /**
  * \brief Keep connection associated with the key from being closed by
  * heartbeat monitor
  */
  virtual void KeepConnectionAlive(uint32_t connection_key,
                                   uint8_t session_id) = 0;

  /**
   * \brief binds protocol version with session
   *
   * \param connection_key pair of connection and session id
   * \param protocol_version contains protocol version of
   * \registered application.
   */
  virtual void BindProtocolVersionWithSession(uint32_t connection_key,
                                              uint8_t protocol_version) = 0;

  /**
   * \brief information about given Connection Key.
   * \param key Unique key used by other components as session identifier
   * \param app_id Returned: ApplicationID
   * \param sessions_list Returned: List of session keys
   * \param device_id Returned: DeviceID
   * \return int32_t -1 in case of error or 0 in case of success
   */
  virtual int32_t GetDataOnSessionKey(uint32_t key,
                                      uint32_t* app_id,
                                      std::list<int32_t>* sessions_list,
                                      uint32_t* device_id) const = 0;
    /**
     * @brief GetConnectedDevicesMAC allows to obtain MAC adresses for all
     * currently connected devices.
     *
     * @param device_macs collection of MAC adresses for connected devices.
     */
    virtual void GetConnectedDevicesMAC(std::vector<std::string> &device_macs) const = 0;

  /**
   * \brief Connection handler settings getter
   * \return pointer to connection handler settings class
   */
  virtual const ConnectionHandlerSettings& get_settings() const = 0;

  virtual const protocol_handler::SessionObserver& get_session_observer() = 0;

  virtual DevicesDiscoveryStarter& get_device_discovery_starter() = 0;

 protected:
  /**
   * \brief Destructor
   */
  virtual ~ConnectionHandler() {}
};
}  // namespace connection_handler

#endif  // SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_CONNECTION_HANDLER_H_
