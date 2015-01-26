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

#ifndef SRC_COMPONENTS_SECURITY_MANAGER_INCLUDE_SECURITY_MANAGER_SECURITY_MANAGER_IMPL_H_
#define SRC_COMPONENTS_SECURITY_MANAGER_INCLUDE_SECURITY_MANAGER_SECURITY_MANAGER_IMPL_H_

#include <list>
#include <string>

#include "utils/macro.h"
#include "utils/message_queue.h"
#include "utils/threads/message_loop_thread.h"

#include "security_manager/security_manager.h"
#include "security_manager/security_query.h"
#include "protocol_handler/protocol_handler.h"
#include "protocol/common.h"

namespace security_manager {
/**
 * \brief SecurityMessage is wrapper for SecurityQuery with priority
 * \brief SecurityMessageQueue and SecurityMessageLoop are support typedefs
 * for thread working
 */
struct SecurityMessage: public SecurityQueryPtr {
  explicit SecurityMessage(const SecurityQueryPtr &message)
    : SecurityQueryPtr(message) {}
  // PrioritizedQueue requires this method to decide which priority to assign
  size_t PriorityOrder() const {
    return 0;
  }
};
typedef utils::PrioritizedQueue<SecurityMessage> SecurityMessageQueue;
typedef threads::MessageLoopThread<SecurityMessageQueue> SecurityMessageLoop;

/**
 * \brief SecurityManagerImpl class implements SecurityManager inteface
 */
class SecurityManagerImpl
  : public SecurityManager,
    public SecurityMessageLoop::Handler {
 public:
  /**
   * \brief Constructor
   */
  SecurityManagerImpl();
  /**
   * \brief Add received from Mobile Application message
   * Overriden ProtocolObserver::OnMessageReceived method
   * \param message Message with supporting params received
   */
  void OnMessageReceived(const ::protocol_handler::RawMessagePtr message) OVERRIDE;
  /**
   * \brief Post message to Mobile Application
   * Empty *overriden ProtocolObserver::OnMessageReceived method
   * \param message Message with supporting params received
   */
  void OnMobileMessageSent(const ::protocol_handler::RawMessagePtr) OVERRIDE;

  /**
   * \brief Sets pointer for Connection Handler layer for managing sessions
   * \param session_observer pointer to object of the class implementing
   */
  void set_session_observer(
    protocol_handler::SessionObserver *observer) OVERRIDE;
  /**
   * \brief Sets pointer for Protocol Handler layer for sending
   * \param protocol_handler pointer to object of the class implementing
   */
  void set_protocol_handler(
    protocol_handler::ProtocolHandler *protocol_handler_) OVERRIDE;
  /**
   * \brief Sets pointer for CryptoManager for handling SSLContext
   * \param crypto_manager pointer to object of the class implementing
   */
  void set_crypto_manager(CryptoManager *crypto_manager) OVERRIDE;
  /**
   * \brief Sends InternallError with text message to mobile application
   * \param connection_key Unique key used by other components as session identifier
   * \param error_id  unique error identifier
   * \param erorr_text SSL impelmentation error text
   * \param seq_number received from Mobile Application
   */
  void SendInternalError(const uint32_t connection_key,
                         const uint8_t &error_id,
                         const std::string &erorr_text,
                         const uint32_t seq_number) OVERRIDE;

  using SecurityManager::SendInternalError;

  /**
   * \brief Handle SecurityMessage from mobile for processing
   * threads::MessageLoopThread<*>::Handler implementations
   * CALLED in SecurityMessageLoop thread
   */
  void Handle(const SecurityMessage message) OVERRIDE;

  /**
   * \brief Create new SSLContext for connection or return exists
   * Do not notify listeners, send security error on occure
   * \param connection_key Unique key used by other components as session identifier
   * @return new \c  SSLContext or \c NULL on any error
   */
  SSLContext *CreateSSLContext(const uint32_t &connection_key) OVERRIDE;

  /**
   * \brief Start handshake as SSL client
   */
  void StartHandshake(uint32_t connection_key) OVERRIDE;

  /**
   * \brief Add/Remove for SecurityManagerListener
   */
  void AddListener(SecurityManagerListener *const listener) OVERRIDE;
  void RemoveListener(SecurityManagerListener *const listener) OVERRIDE;
  /**
   * \brief Notifiers for listeners
   * \param connection_key Unique key used by other components as session identifier
   * \param success result of connection protection
   */
  void NotifyListenersOnHandshakeDone(const uint32_t &connection_key,
                                      const bool success);
  /**
   * @brief SecurityConfigSection
   * @return Session name in config file
   */
  static const char *ConfigSection();
 private:
  /**
   * \brief Sends Handshake binary data to mobile application
   * \param connection_key Unique key used by other components as session identifier
   * \param data pointer to binary data array
   * \param data_size size of binary data array
   * \param seq_number received from Mobile Application
   */
  void SendHandshakeBinData(const uint32_t connection_key,
                            const uint8_t *const data,
                            const size_t data_size,
                            const uint32_t seq_number = 0);
  /**
   * \brief Parse SecurityMessage as HandshakeData request
   * \param inMessage SecurityMessage with binary data of handshake
   */
  bool ProccessHandshakeData(const SecurityMessage &inMessage);
  /**
   * \brief Parse InternalError from mobile side
   * \param inMessage SecurityMessage with binary data of handshake
   */
  bool ProccessInternalError(const SecurityMessage &inMessage);

  /**
   * \brief Sends security query
   * Create new array as concatenation of header and binary data
   * \param query SecurityQuery for sending via Control service
   * \param connection_key Unique key used by other components as session identifier
   */
  void SendQuery(const SecurityQuery &query, const uint32_t connection_key);

  // Thread that pumps handshake data
  SecurityMessageLoop security_messages_;

  /**
   *\brief Pointer on instance of class implementing SessionObserver
   */
  protocol_handler::SessionObserver *session_observer_;
  /**
   *\brief Pointer on instance of class implementing CryptoManager
   */
  security_manager::CryptoManager *crypto_manager_;
  /**
   *\brief Pointer on instance of class implementing ProtocolHandler
   */
  protocol_handler::ProtocolHandler *protocol_handler_;
  /**
   *\brief List of listeners for notify handshake done result
   */
  std::list<SecurityManagerListener *> listeners_;
  DISALLOW_COPY_AND_ASSIGN(SecurityManagerImpl);
};
}  // namespace security_manager
#endif  // SRC_COMPONENTS_SECURITY_MANAGER_INCLUDE_SECURITY_MANAGER_SECURITY_MANAGER_IMPL_H_
