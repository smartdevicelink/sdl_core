/**
 * \file ISessionObserver.h
 * \brief ISessionObserver class header file.
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

#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_SESSION_OBSERVER_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_SESSION_OBSERVER_H_

#include <list>
#include <string>
#include "transport_manager/transport_manager.h"
#include "connection_handler/connection_handler.h"
#include "security_manager/ssl_context.h"

/**
 *\namespace NsProtocolHandler
 *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
 */
namespace protocol_handler {
/**
 * \class SessionObserver
 * \brief Interface for making a bridge between ProtocolHandler and
 * ConnectionHandler components.
 */
//TODO:(EZamakhov) Rename SessionObserver to SessionManager
//TODO:(EZamakhov) Unificate interface (references)
//TODO:(EZamakhov) cahnge brief info (callback, spaces)
//TODO:(EZamakhov) rename KeyFromPair and PairFromKey
//TODO:(EZamakhov) Add const modifiers to const methods
class SessionObserver {
public:
    /**
     * \brief Callback function used by ProtocolHandler
     * when Mobile Application initiates start of new session.
     * \param connection_handle Connection identifier within which session
     * has to be started.
     * \param sessionId Identifier of the session to be ended
     * \param service_type Type of service
     * \param is_protected would be service protected
     * \return int32_t Id (number) of new session if successful otherwise -1.
     */
    virtual int32_t OnSessionStartedCallback(
      const transport_manager::ConnectionUID& connection_handle,
      const uint8_t& sessionId,
      const ServiceType& service_type,
      const bool is_protected) = 0;

    /**
     * \brief Callback function used by ProtocolHandler
     * when Mobile Application initiates session ending.
     * \param connection_handle Connection identifier within which session exists
     * \param sessionId Identifier of the session to be ended
     * \param hashCode Hash used only in second version of SmartDeviceLink protocol.
     * If not equal to hash assigned to session on start then operation fails.
     * \param service_type Type of service
     * \return uint32_t 0 if operation fails session key otherwise
     */
    virtual uint32_t OnSessionEndedCallback(
      const transport_manager::ConnectionUID& connection_handle,
      const uint8_t& sessionId,
      const uint32_t& hashCode,
      const ServiceType& service_type) = 0;

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
      uint8_t sessionId) = 0;

    /**
     * \brief Returns connection identifier and session number from given
     * session key
     * \param key Unique key used by other components as session identifier
     * \param connection_handle Returned: Connection identifier within which
     * session exists
     * \param sessionId Returned: Number of session
     */
    virtual void PairFromKey(
      uint32_t key,
      transport_manager::ConnectionUID* connection_handle,
      uint8_t* sessionId) = 0;

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
                                    uint32_t* device_id) = 0;

    /**
     * \brief information about given Connection Key.
     * \param key Unique key used by other components as session identifier
     * \param app_id Returned: ApplicationID
     * \param sessions_list Returned: List of session keys
     * \param device_id Returned: DeviceID
     * \return int32_t -1 in case of error or 0 in case of success
     */
    virtual int32_t GetDataOnDeviceID(
      uint32_t device_handle,
      std::string* device_name,
      std::list<uint32_t>* applications_list,
      std::string* mac_address) = 0;

    /**
     * \brief Sets crypto context of service
     * \param key Unique key used by other components as session identifier
     * \return \c SecurityQuery::ProtectSessionResult value
     */
    virtual int SetSSLContext(
      const uint32_t& key,
      security_manager::SSLContext* context) = 0;

    /**
     * \brief Gets crypto context of service
     * \param key Unique key used by other components as session identifier
     * \return \ref SSLContext
     */
    virtual security_manager::SSLContext* GetSSLContext(
        const uint32_t& key) = 0;
  protected:
    /**
     * \brief Destructor
     */
    virtual ~SessionObserver() {
    }
};
}  // namespace protocol_handler

#endif  // SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_SESSION_OBSERVER_H_
