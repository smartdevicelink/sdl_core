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

#ifndef SRC_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_SESSION_OBSERVER_H_
#define SRC_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_SESSION_OBSERVER_H_

#include <list>
#include <string>
#include "transport_manager/transport_manager.h"
#include "utils/macro.h"
#ifdef ENABLE_SECURITY
#include "security_manager/ssl_context.h"
#endif  // ENABLE_SECURITY

struct BsonObject;

/**
 *\namespace protocol_handler
 *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
 */
namespace protocol_handler {
/**
 * \brief HASH_ID constants.
 * Hint: Shall not intersect with connection identifiers for correct handling
 * these constants (Session identifier shall be greater than zero)
 */
enum { HASH_ID_NOT_SUPPORTED = 0, HASH_ID_WRONG = 0xFFFF0000 };

/**
 * @brief Struct with data containing attributes of starting session
 **/
struct SessionContext {
  transport_manager::ConnectionUID connection_id_;
  uint8_t initial_session_id_;
  uint8_t new_session_id_;
  protocol_handler::ServiceType service_type_;
  uint32_t hash_id_;
  bool is_protected_;
  bool is_new_service_;
  bool is_ptu_required_;

  /**
   * @brief Constructor
   */
  SessionContext()
      : connection_id_(0)
      , initial_session_id_(0)
      , new_session_id_(0)
      , service_type_(protocol_handler::kInvalidServiceType)
      , hash_id_(0)
      , is_protected_(false)
      , is_new_service_(false)
      , is_ptu_required_(false) {}

  /**
   * @brief Constructor
   * @param connection_id_ Connection identifier within which session is
   * started.
   * @param session_id Session ID specified to OnSessionStartedCallback()
   * @param new_session_id Session ID generated
   * @param service_type Type of service
   * @param hash_id Hash ID generated from connection_handle and
   * new_session_id
   * @param is_protected Whether service will be protected
   * @param is_new_service Whether service was already established
   **/
  SessionContext(transport_manager::ConnectionUID connection_id,
                 uint8_t session_id,
                 uint8_t new_session_id,
                 protocol_handler::ServiceType service_type,
                 uint32_t hash_id,
                 const bool is_protected)
      : connection_id_(connection_id)
      , initial_session_id_(session_id)
      , new_session_id_(new_session_id)
      , service_type_(service_type)
      , hash_id_(hash_id)
      , is_protected_(is_protected)
      , is_new_service_(false)
      , is_ptu_required_(false) {}
};

/**
 * \class SessionObserver
 * \brief Interface for making a bridge between ProtocolHandler and
 * ConnectionHandler components.
 */
// TODO(EZamakhov): Reconsider rename callbacks and SessionObserver to
// SessionManager
// TODO(EZamakhov): Reconsider rename KeyFromPair and PairFromKey
class SessionObserver {
 public:
  /**
   * \brief Callback function used by ProtocolHandler
   * when Mobile Application initiates start of new session.
   * \param connection_handle Connection identifier within which session
   * has to be started.
   * \param sessionId Identifier of the session to be start
   * \param service_type Type of service
   * \param protocol_version Version of protocol
   * \param is_protected would be service protected
   * \param hash_id pointer for session hash identifier, uint32_t* hash_id
   * \return uint32_t Id (number) of new session if successful, otherwise 0.
   * \deprecated
   */
  DEPRECATED virtual uint32_t OnSessionStartedCallback(
      const transport_manager::ConnectionUID connection_handle,
      const uint8_t sessionId,
      const protocol_handler::ServiceType& service_type,
      const bool is_protected,
      uint32_t* hash_id) = 0;

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
      const uint8_t sessionId,
      const protocol_handler::ServiceType& service_type,
      const bool is_protected,
      const BsonObject* params) = 0;

  /**
   * \brief Callback function used by ProtocolHandler
   * when Mobile Application initiates session ending.
   * \param connection_handle Connection identifier within which session exists
   * \param sessionId Identifier of the session to be ended
   * \param hashCode Hash used only in second version of SmartDeviceLink
   * protocol.
   * If not equal to hash assigned to session on start then operation fails.
   * \param service_type Type of service
   * \return uint32_t 0 if operation fails, session key otherwise
   * \deprecated
   */
  DEPRECATED virtual uint32_t OnSessionEndedCallback(
      const transport_manager::ConnectionUID connection_handle,
      const uint8_t sessionId,
      const uint32_t& hashCode,
      const protocol_handler::ServiceType& service_type) = 0;

  /**
   * \brief Callback function used by ProtocolHandler
   * when Mobile Application initiates session ending.
   * \param connection_handle Connection identifier within which session exists
   * \param sessionId Identifier of the session to be ended
   * \param hashCode Hash used only in second version of SmartDeviceLink
   * protocol. (Set to HASH_ID_WRONG if the hash is incorrect)
   * If not equal to hash assigned to session on start then operation fails.
   * \param service_type Type of service
   * \return uint32_t 0 if operation fails, session key otherwise
   */
  virtual uint32_t OnSessionEndedCallback(
      const transport_manager::ConnectionUID connection_handle,
      const uint8_t sessionId,
      uint32_t* hashCode,
      const protocol_handler::ServiceType& service_type) = 0;

  /**
   * \brief Callback function used by ProtocolHandler
   * when Mobile Application start message flood
   * \param connection_key used by other components as application identifier
   */
  virtual void OnApplicationFloodCallBack(const uint32_t& connection_key) = 0;

  /**
   * \brief Callback function used by ProtocolHandler
   * when Mobile Application sends malformed message
   * \param connection_key used by other components as application identifier
   */
  virtual void OnMalformedMessageCallback(const uint32_t& connection_key) = 0;

  /**
   * \brief Creates unique identifier of session (can be used as hash)
   * from given connection identifier
   * within which session exists and session number.
   * \param connection_handle Connection identifier within which session exists
   * \param sessionId Identifier of the session
   * \return uint32_t Unique key for session
   */
  virtual uint32_t KeyFromPair(
      transport_manager::ConnectionUID connection_handle,
      uint8_t session_id) const = 0;

  /**
   * \brief Returns connection identifier and session number from given
   * session key
   * \param key Unique key used by other components as session identifier
   * \param connection_handle Connection identifier within which session exists
   * \param sessionId Returned: Number of session
   */
  virtual void PairFromKey(uint32_t key,
                           transport_manager::ConnectionUID* connection_handle,
                           uint8_t* sessionId) const = 0;

  /**
   * \deprecated
   * \brief information about given Connection Key.
   * \param key Unique key used by other components as session identifier
   * \param app_id Returned: ApplicationID
   * \param sessions_list Returned: List of session keys
   * \param device_id Returned: DeviceID
   * \return int32_t -1 in case of error or 0 in case of success
   */
  DEPRECATED virtual int32_t GetDataOnSessionKey(
      uint32_t key,
      uint32_t* app_id,
      std::list<int32_t>* sessions_list,
      uint32_t* device_id) const = 0;

  /**
   * \brief information about given Connection Key.
   * \param key Unique key used by other components as session identifier
   * \param app_id Returned: ApplicationID
   * \param sessions_list Returned: List of session keys
   * \param device_id Returned: DeviceID
   * \return int32_t -1 in case of error or 0 in case of success
   */
  virtual int32_t GetDataOnSessionKey(
      uint32_t key,
      uint32_t* app_id,
      std::list<int32_t>* sessions_list,
      transport_manager::DeviceHandle* device_id) const = 0;

  /**
   * \brief information about device
   * \param device_handle
   * \param device_name Returned: name of device
   * \param applications_list Returned: applications on device
   * \param mac_address Returned: List of session keys
   * \param connection_type Returned: type of connection (USB, BT, etc.)
   * \return int32_t -1 in case of error or 0 in case of success
   */
  virtual int32_t GetDataOnDeviceID(
      transport_manager::DeviceHandle device_handle,
      std::string* device_name = NULL,
      std::list<uint32_t>* applications_list = NULL,
      std::string* mac_address = NULL,
      std::string* connection_type = NULL) const = 0;

  /**
   * \brief returns TRUE if session supports sending HEARTBEAT ACK to mobile
   * side
   * \param connection_handle Connection identifier whithin which session exists
   * \param session_id Identifier of the session
   * \return TRUE if session has protocol version which supports heartbeat
   * otherwise returns FALSE
   */
  virtual bool IsHeartBeatSupported(
      transport_manager::ConnectionUID connection_handle,
      uint8_t session_id) const = 0;

  /**
   * @brief returns protocol version which application supports
   * @param connection_id id of connection
   * @param session_id id of session
   * @param method writes value protocol version to protocol_version
   * @return TRUE if session and connection exist otherwise returns FALSE
   */
  virtual bool ProtocolVersionUsed(uint32_t connection_id,
                                   uint8_t session_id,
                                   uint8_t& protocol_version) const = 0;

#ifdef ENABLE_SECURITY
  /**
   * \brief Sets crypto context of connection
   * \param key Unique key used by other components as session identifier
   * \param context SSLContext to be set
   * \return \c SecurityQuery::ProtectSessionResult value
   */
  virtual int SetSSLContext(const uint32_t& key,
                            security_manager::SSLContext* context) = 0;
  /**
   * \brief Gets crypto context of connection, use service_type to get NULL
   * SSLContext for not protected services or ControlService (0x0)
   * to get current SSLContext of connection
   * \param key Unique key used by other components as session identifier
   * \param service_type Type of service
   * \return \ref SSLContext of connection
   */
  virtual security_manager::SSLContext* GetSSLContext(
      const uint32_t& key,
      const protocol_handler::ServiceType& service_type) = 0;
  /**
   * \brief Set protection flag to service in session by key
   * to get current SSLContext of connection
   * \param key Unique key used by other components as session identifier
   * \param service_type Type of service
   */
  virtual void SetProtectionFlag(
      const uint32_t& key,
      const protocol_handler::ServiceType& service_type) = 0;

  /**
   * @brief Check if session contains service with specified service type
   * @param connection_key unique id of session to check
   * @param service_type type of service to check
   * @return true if session contains service with specified service type
   */
  virtual bool SessionServiceExists(
      const uint32_t connection_key,
      const protocol_handler::ServiceType& service_type) const = 0;

  virtual security_manager::SSLContext::HandshakeContext GetHandshakeContext(
      uint32_t key) const = 0;
#endif  // ENABLE_SECURITY

 protected:
  /**
   * \brief Destructor
   */
  virtual ~SessionObserver() {}
};
}  // namespace protocol_handler
#endif  // SRC_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_SESSION_OBSERVER_H_
