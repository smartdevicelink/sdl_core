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

#include "security_manager/crypto_manager.h"
#include "security_manager/security_manager_listener.h"

namespace security_manager {
/**
 * \brief SecurityManager interface implements protocol_handler::ProtocolObserver
 * and provide interface for handling Security queries from mobile side
 */
class SecurityManager
  : public protocol_handler::ProtocolObserver {
 public:
  /**
   * \brief InternalErrors is 1 byte identifier of internal error
   * Handle as binary data in Ford Protocol
   */
  enum InternalErrors {
    ERROR_SUCCESS                    = 0x00,
    ERROR_INVALID_QUERY_SIZE         = 0x01,  // wrong size of query data
    ERROR_INVALID_QUERY_ID           = 0x02,  // unknown query id
    ERROR_NOT_SUPPORTED              = 0x03,  // SDL does not support encryption
    ERROR_SERVICE_ALREADY_PROTECTED  = 0x04,
    ERROR_SERVICE_NOT_PROTECTED      = 0x05,  // got handshake or encrypted data
    // for not protected service
    ERROR_DECRYPTION_FAILED          = 0x06,
    ERROR_ENCRYPTION_FAILED          = 0x07,
    ERROR_SSL_INVALID_DATA           = 0x08,
    ERROR_INTERNAL                   = 0xFF,
    ERROR_UNKWOWN_INTERNAL_ERROR     = 0xFE  // error valeu for testing
  };
  /**
   * \brief Sets pointer for Connection Handler layer for managing sessions
   * \param session_observer pointer to object of the class implementing
   */
  virtual void set_session_observer(
    protocol_handler::SessionObserver *observer) = 0;
  /**
   * \brief Sets pointer for Protocol Handler layer for sending
   * \param protocol_handler pointer to object of the class implementing
   */
  virtual void set_protocol_handler(
    protocol_handler::ProtocolHandler *protocol_handler_) = 0;
  /**
   * \brief Sets pointer for CryptoManager for handling SSLContext
   * \param crypto_manager pointer to object of the class implementing
   */
  virtual void set_crypto_manager(CryptoManager *crypto_manager) = 0;

  /**
   * \brief Sends InternalError with text message to mobile application
   * \param connection_key Unique key used by other components as session identifier
   * \param error_id  unique error identifier
   * \param erorr_text SSL impelmentation error text
   * \param seq_number received from Mobile Application
   */
  virtual void SendInternalError(const uint32_t connection_key,
                                 const uint8_t &error_id,
                                 const std::string &erorr_text,
                                 const uint32_t seq_number) = 0;
  /**
   * \brief Sends InternalError override methode for sending without seq_number
   * \param connection_key Unique key used by other components as session identifier
   * \param error_id  unique error identifier
   * \param erorr_text SSL impelmentation error text
   */
  void SendInternalError(const uint32_t connection_key,
                         const uint8_t &error_id,
                         const std::string &erorr_text) {
    SendInternalError(connection_key, error_id, erorr_text, 0);
  }

  /**
   * \brief Create new SSLContext for connection or return exists
   * Do not notify listeners, send security error on occure
   * \param connection_key Unique key used by other components as session identifier
   * @return new \c  SSLContext or \c NULL on any error
   */
  virtual SSLContext *CreateSSLContext(const uint32_t &connection_key) = 0;

  /**
   * \brief Start handshake as SSL client
   */
  virtual void StartHandshake(uint32_t connection_key) = 0;

  /**
   * \brief Add/Remove for SecurityManagerListener
   */
  virtual void AddListener(SecurityManagerListener *const listener) = 0;
  virtual void RemoveListener(SecurityManagerListener *const listener) = 0;
};
}  // namespace security_manager
#endif  // SRC_COMPONENTS_INCLUDE_SECURITY_MANAGER_SECURITY_MANAGER_H_
