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

#ifndef SRC_COMPONENTS_INCLUDE_SECURITY_MANAGER_SECURITY_MANAGER_H_
#define SRC_COMPONENTS_INCLUDE_SECURITY_MANAGER_SECURITY_MANAGER_H_

#include <list>
#include <string>

#include "protocol_handler/protocol_handler.h"
#include "protocol_handler/protocol_observer.h"
#include "protocol_handler/session_observer.h"

#include "application_manager/policies/policy_handler_observer.h"
#include "security_manager/security_manager_listener.h"

namespace security_manager {

class CryptoManager;
/**
 * \brief SecurityManager interface implements
 * protocol_handler::ProtocolObserver
 * and provide interface for handling Security queries from mobile side
 */
class SecurityManager : public protocol_handler::ProtocolObserver,
                        public policy::PolicyHandlerObserver {
 public:
  /**
   * \brief InternalErrors is 1 byte identifier of internal error
   * Handle as binary data in Ford Protocol
   */
  enum InternalErrors {
    ERROR_SUCCESS = 0x00,
    ERROR_INVALID_QUERY_SIZE = 0x01,  // wrong size of query data
    ERROR_INVALID_QUERY_ID = 0x02,    // unknown query id
    ERROR_NOT_SUPPORTED = 0x03,       // SDL does not support encryption
    ERROR_SERVICE_ALREADY_PROTECTED = 0x04,
    ERROR_SERVICE_NOT_PROTECTED = 0x05,  // got handshake or encrypted data
    // for not protected service
    ERROR_DECRYPTION_FAILED = 0x06,
    ERROR_ENCRYPTION_FAILED = 0x07,
    ERROR_SSL_INVALID_DATA = 0x08,
    ERROR_HANDSHAKE_FAILED = 0x09,  // Handshake failed
    ERROR_INVALID_CERT = 0x0A,      // Handshake failed because cert is invalid
    ERROR_EXPIRED_CERT = 0x0B,      // Handshake failed because cert is expired
    ERROR_INTERNAL = 0xFF,
    ERROR_UNKNOWN_INTERNAL_ERROR = 0xFE  // error value for testing
  };

  //  SSL context creation strategy
  enum ContextCreationStrategy { kUseExisting = 0, kForceRecreation };

  /**
   * \brief Sets pointer for Connection Handler layer for managing sessions
   * \param session_observer pointer to object of the class implementing
   */
  virtual void set_session_observer(
      protocol_handler::SessionObserver* observer) = 0;
  /**
   * \brief Sets pointer for Protocol Handler layer for sending
   * \param protocol_handler pointer to object of the class implementing
   */
  virtual void set_protocol_handler(
      protocol_handler::ProtocolHandler* protocol_handler_) = 0;
  /**
   * \brief Sets pointer for CryptoManager for handling SSLContext
   * \param crypto_manager pointer to object of the class implementing
   */
  virtual void set_crypto_manager(CryptoManager* crypto_manager) = 0;

  /**
   * \brief Sends InternalError with text message to mobile application
   * \param connection_key Unique key used by other components as session
   * identifier
   * \param error_id  unique error identifier
   * \param error_text SSL impelmentation error text
   * \param seq_number received from Mobile Application
   */
  virtual void SendInternalError(const uint32_t connection_key,
                                 const uint8_t& error_id,
                                 const std::string& error_text,
                                 const uint32_t seq_number) = 0;
  /**
   * \brief Sends InternalError override methode for sending without seq_number
   * \param connection_key Unique key used by other components as session
   * identifier
   * \param error_id  unique error identifier
   * \param error_text SSL impelmentation error text
   */
  void SendInternalError(const uint32_t connection_key,
                         const uint8_t& error_id,
                         const std::string& error_text) {
    SendInternalError(connection_key, error_id, error_text, 0);
  }

  /**
   * @brief Create new SSLContext for connection or return exists
   * Do not notify listeners, send security error on occure
   * @param connection_key Unique key used by other components as session
   * identifier
   * @param cc_strategy - SSL context creation strategy
   * @return new \c  SSLContext or \c NULL on any error
   */
  virtual SSLContext* CreateSSLContext(const uint32_t& connection_key,
                                       ContextCreationStrategy cc_strategy) = 0;

  /**
   * \brief Start handshake as SSL client
   */
  virtual void StartHandshake(uint32_t connection_key) = 0;

  /**
   * @brief PostponeHandshake allows to postpone handshake. It notifies
   * cryptomanager that certificate should be updated and adds specified
   * connection key to the list of the certificate awaiting connections.
   * @param connection_key the identifier for connection to postpone handshake.
   */
  virtual void PostponeHandshake(const uint32_t connection_key) = 0;

  /**
   * @brief Check whether certificate should be updated
   * @param connection_key the connection identifier to check certificate for.
   * @return true if certificate should be updated otherwise false
   */
  virtual bool IsCertificateUpdateRequired(const uint32_t connection_key) = 0;

  /**
   * @brief Checks whether system time ready notification
   * was received from hmi
   * @return true if received otherwise false
   */
  virtual bool IsSystemTimeProviderReady() const = 0;

  /**
   * @brief Notify all listeners that certificate update required
   * Allows to notify that certificate should be updated
   */
  virtual void NotifyOnCertificateUpdateRequired() = 0;

  /**
   * @brief Notify all listeners that handshake was failed
   */
  virtual void NotifyListenersOnGetSystemTimeFailed() = 0;

  virtual void ProcessFailedPTU() = 0;

#if defined(EXTERNAL_PROPRIETARY_MODE) && defined(ENABLE_SECURITY)
  /**
   * @brief ProcessFailedCertDecrypt is called to notify listeners that
   * certificate decryption failed in the external flow
   */
  virtual void ProcessFailedCertDecrypt() = 0;
#endif

  /**
   * @brief Check if policy certificate data is empty
   * @return true if policy certificate data is empty otherwise false
   */
  virtual bool IsPolicyCertificateDataEmpty() = 0;

  /**
   * @brief ResetPendingSystemTimeRequests resets waiting for system time
   * requests flag
   */
  virtual void ResetPendingSystemTimeRequests() = 0;

  /**
   * \brief Add/Remove for SecurityManagerListener
   */
  virtual void AddListener(SecurityManagerListener* const listener) = 0;
  virtual void RemoveListener(SecurityManagerListener* const listener) = 0;

  /**
   * @brief OnCertificateUpdated allows to obtain notification when certificate
   * has been updated with policy table update. Pass this certificate to crypto
   * manager for further processing. Also process postopnes handshake for the
   * certain connection key.
   *
   * @param data the certificates content.
   *
   * @return always true.
   */
  virtual bool OnCertificateUpdated(const std::string& data) = 0;
};
}  // namespace security_manager
#endif  // SRC_COMPONENTS_INCLUDE_SECURITY_MANAGER_SECURITY_MANAGER_H_
