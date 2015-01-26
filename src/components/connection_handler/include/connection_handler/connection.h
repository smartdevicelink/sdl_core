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

#ifndef SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_CONNECTION_H_
#define SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_CONNECTION_H_

#include <map>
#include <vector>

#include "utils/lock.h"
#include "connection_handler/device.h"
#include "connection_handler/heartbeat_monitor.h"
#include "protocol/service_type.h"

#ifdef ENABLE_SECURITY
namespace security_manager {
class SSLContext;
}  // namespace security_manager
#endif  // ENABLE_SECURITY

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink connection_handler namespace.
 */
namespace connection_handler {

class ConnectionHandler;

/**
 * \brief Type for ConnectionHandle
 */
typedef int32_t ConnectionHandle;

/**
 * \brief Type for Connections map
 * Key is ConnectionHandle which is unique
 */
typedef std::map<int32_t, Connection*> ConnectionList;

/**
 * \brief ServiceType
 */
struct Service {
  protocol_handler::ServiceType service_type;
  bool is_protected_;
  Service()
    : service_type(protocol_handler::kInvalidServiceType),
      is_protected_(false) {
  }
  explicit Service(protocol_handler::ServiceType service_type)
    : service_type(service_type),
      is_protected_(false) {
  }
  bool operator==(const protocol_handler::ServiceType service_type) const {
    return this->service_type == service_type;
  }
};

/**
 * \brief Type for Session Services
 */
typedef std::vector<Service> ServiceList;

struct Session {
  ServiceList service_list;
  uint8_t protocol_version;
#ifdef ENABLE_SECURITY
  security_manager::SSLContext *ssl_context;
#endif  // ENABLE_SECURITY
  Session()
    : service_list()
#ifdef ENABLE_SECURITY
    , ssl_context(NULL)
#endif  // ENABLE_SECURITY
  {}
  explicit Session(const ServiceList &services, uint8_t protocol_version)
    : service_list(services),
      protocol_version(protocol_version)
#ifdef ENABLE_SECURITY
      , ssl_context(NULL)
#endif  // ENABLE_SECURITY
  {}
  Service *FindService(const protocol_handler::ServiceType &service_type);
  const Service *FindService(const protocol_handler::ServiceType &service_type) const;
};

/**
 * \brief Type for Session map
 */
typedef std::map<uint8_t, Session> SessionMap;

/**
 * \class Connection
 * \brief Stores connection information
 */
class Connection {
 public:
  /**
   * \brief Class constructor
   */
  Connection(ConnectionHandle connection_handle,
             DeviceHandle connection_device_handle,
             ConnectionHandler *connection_handler,
             int32_t heartbeat_timeout);

  /**
   * \brief Destructor
   */
  ~Connection();

  /**
   * \brief Returns device handle
   * \return DeviceHandle
   */
  ConnectionHandle connection_handle() const;

  /**
   * \brief Returns connection device handle
   * \return ConnectionDeviceHandle
   */
  DeviceHandle connection_device_handle();

  /**
   * \brief Adds session to connection
   * \return new session id or 0 in case of issues
   */
  uint32_t AddNewSession();

  /**
   * \brief Removes session from connection
   * \param session session ID
   * \return session_id or 0 in case of issues
   */
  uint32_t RemoveSession(uint8_t session_id);

  /**
   * \brief Adds uprotected service to session or
   * check protection to service has been started before
   * \param session_id session ID
   * \param service_type Type of service
   * \param is_protected protection state
   * \return TRUE on success, otherwise FALSE
   */
  bool AddNewService(uint8_t session_id,
                     protocol_handler::ServiceType service_type,
                     const bool is_protected);
  /**
   * \brief Removes service from session
   * \param session_id session ID
   * \param service_type Type of service
   * \return TRUE on success, otherwise FALSE
   */
  bool RemoveService(uint8_t session_id,
                     protocol_handler::ServiceType service_type);
#ifdef ENABLE_SECURITY
  /**
   * \brief Sets crypto context of service
   * \param session_id Identifier of the session
   * \param context SSL for connection
   * \return \c true in case of service is protected or \c false otherwise
   */
  int SetSSLContext(uint8_t session_id,
                    security_manager::SSLContext *context);
  /**
   * \brief Gets crypto context of session, use service_type to get NULL
   * SSLContext for not protected services or ControlService (0x0)
   * to get current SSLContext of connection
   * \param session_id Identifier of the session
   * \param service_type Type of service
   * \return \ref SSLContext of connection
   */
  security_manager::SSLContext *GetSSLContext(
    const uint8_t session_id,
    const protocol_handler::ServiceType &service_type) const;
  /**
   * \brief Set protection flag to service in session by key
   * to get current SSLContext of connection
   * \param session_id Identifier of the session
   * \param service_type Type of service
   */
  void SetProtectionFlag(
    const uint8_t session_id,
    const protocol_handler::ServiceType &service_type);
#endif  // ENABLE_SECURITY
  /**
   * \brief Returns map of sessions which have been opened in
   *  current connection.
   */
  const SessionMap session_map() const;

  /**
   * \brief Close session
   * \param  session_id session id
   */
  void CloseSession(uint8_t session_id);

  /**
   * \brief Prevent session from being closed by heartbeat timeout
   * \param  session_id session id
   */
  void KeepAlive(uint8_t session_id);

  /**
   * \brief Start heartbeat for specified session
   * \param  session_id session id
   */
  void StartHeartBeat(uint8_t session_id);

  /**
   * \brief Send heartbeat to  mobile app
   * \param  session_id session id
   */
  void SendHeartBeat(uint8_t session_id);

  /**
   * Sets heart beat timeout
   * @param timeout in seconds
   */
  void SetHeartBeatTimeout(int32_t timeout);

  /**
   * \brief changes protocol version in session
   * \param  session_id session id
   * \param  protocol_version protocol version registered application
   */
  void UpdateProtocolVersionSession(uint8_t session_id, uint8_t protocol_version);

  /**
   * \brief checks if session supports heartbeat
   * \param  session_id session id
   * \return TRUE on success, otherwise FALSE
   */
  bool SupportHeartBeat(uint8_t session_id);


 private:
  /**
   * \brief Current connection handler.
   */
  ConnectionHandler *connection_handler_;

  /**
   * \brief Current connection handle.
   */
  ConnectionHandle connection_handle_;

  /**
   * \brief DeviceHandle of this connection.
   */
  DeviceHandle connection_device_handle_;

  /**
   * \brief session/services map
   */
  SessionMap  session_map_;

  mutable sync_primitives::Lock session_map_lock_;

  /**
   * \brief monitor that closes connection if there is no traffic over it
   */
  HeartBeatMonitor *heartbeat_monitor_;
  threads::Thread *heart_beat_monitor_thread_;

  DISALLOW_COPY_AND_ASSIGN(Connection);
};

}  // namespace connection_handler
#endif  // SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_CONNECTION_H_
