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
#include <memory>
#include <set>
#include <string>

#include "utils/macro.h"
#include "utils/message_queue.h"
#include "utils/threads/message_loop_thread.h"

#include "protocol/common.h"
#include "protocol_handler/protocol_handler.h"
#include "security_manager/security_manager.h"
#include "security_manager/security_query.h"
#include "utils/system_time_handler.h"

namespace security_manager {
/**
 * \brief SecurityMessage is wrapper for SecurityQuery with priority
 * \brief SecurityMessageQueue and SecurityMessageLoop are support typedefs
 * for thread working
 */
struct SecurityMessage : public SecurityQueryPtr {
  SecurityMessage() {}
  explicit SecurityMessage(const SecurityQueryPtr& message)
      : SecurityQueryPtr(message) {}
  // PrioritizedQueue requires this method to decide which priority to assign
  size_t PriorityOrder() const {
    return 0;
  }
};
typedef utils::PrioritizedQueue<SecurityMessage> SecurityMessageQueue;
typedef threads::MessageLoopThread<SecurityMessageQueue> SecurityMessageLoop;

/**
 * \brief SecurityManagerImpl class implements SecurityManager interface
 */
class SecurityManagerImpl : public SecurityManager,
                            public SecurityMessageLoop::Handler,
                            public utils::SystemTimeListener {
 public:
  /**
   * \brief Constructor
   * \param system_time_handler allows to work with system time.
   */
  explicit SecurityManagerImpl(
      std::unique_ptr<utils::SystemTimeHandler>&& system_time_handler);

  /**
   * \brief Destructor
   */
  ~SecurityManagerImpl();

  /**
   * \brief Add received from Mobile Application message
   * Overriden ProtocolObserver::OnMessageReceived method
   * \param message Message with supporting params received
   */
  void OnMessageReceived(
      const ::protocol_handler::RawMessagePtr message) OVERRIDE;
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
      protocol_handler::SessionObserver* observer) OVERRIDE;
  /**
   * \brief Sets pointer for Protocol Handler layer for sending
   * \param protocol_handler pointer to object of the class implementing
   */
  void set_protocol_handler(
      protocol_handler::ProtocolHandler* protocol_handler_) OVERRIDE;
  /**
   * \brief Sets pointer for CryptoManager for handling SSLContext
   * \param crypto_manager pointer to object of the class implementing
   */
  void set_crypto_manager(CryptoManager* crypto_manager) OVERRIDE;
  /**
   * \brief Sends InternallError with text message to mobile application
   * \param connection_key Unique key used by other components as session
   * identifier
   * \param error_id  unique error identifier
   * \param erorr_text SSL impelmentation error text
   * \param seq_number received from Mobile Application
   */
  void SendInternalError(const uint32_t connection_key,
                         const uint8_t& error_id,
                         const std::string& erorr_text,
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
   * \param connection_key Unique key used by other components as session
   * identifier
   * @param cc_strategy - SSL context creation strategy
   * @return new \c  SSLContext or \c NULL on any error
   */
  SSLContext* CreateSSLContext(const uint32_t& connection_key,
                               ContextCreationStrategy cc_strategy) OVERRIDE;

  /**
   * \brief Start handshake as SSL client
   */
  void StartHandshake(uint32_t connection_key) OVERRIDE;

  /**
   * @brief PostponeHandshake allows to postpone handshake. It notifies
   * cryptomanager that certificate should be updated and adds specified
   * connection key to the list of the certificate awaiting connections.
   * @param connection_key the identifier for connection to postpone handshake.
   */
  void PostponeHandshake(const uint32_t connection_key) OVERRIDE;

  /**
   * @brief Checks whether certificate should be updated
   * @param connection_key the connection identifier to check certificate for.
   * @return true if certificate should be updated otherwise false
   */
  bool IsCertificateUpdateRequired(const uint32_t connection_key) OVERRIDE;

  /**
   * @brief Checks whether system time ready notification
   * was received from hmi
   * @return true if received otherwise false
   */
  bool IsSystemTimeProviderReady() const OVERRIDE;

  /**
   * \brief Add/Remove for SecurityManagerListener
   */
  void AddListener(SecurityManagerListener* const listener) OVERRIDE;
  void RemoveListener(SecurityManagerListener* const listener) OVERRIDE;

  /**
   * \brief Notifiers for listeners
   * \param connection_key Unique key used by other components as session
   * identifier
   * \param success result of connection protection
   */
  void NotifyListenersOnHandshakeDone(const uint32_t& connection_key,
                                      SSLContext::HandshakeResult error);

  /**
   * @brief Notifiers for listeners.
   * Allows to notify that certificate should be updated
   */
  void NotifyOnCertificateUpdateRequired() OVERRIDE;

  /**
   * @brief Notify all listeners that handshake was failed
   */
  void NotifyListenersOnGetSystemTimeFailed() OVERRIDE;

  /**
   * @brief Check is policy certificate data is empty
   * @return true if policy certificate data is not empty otherwise false
   */
  bool IsPolicyCertificateDataEmpty() OVERRIDE;

  /**
   * @brief SecurityConfigSection
   * @return Session name in config file
   */
  static const char* ConfigSection();

  void ProcessFailedPTU() OVERRIDE;

#if defined(EXTERNAL_PROPRIETARY_MODE) && defined(ENABLE_SECURITY)
  /**
   * @brief ProcessFailedCertDecrypt is called to notify listeners that
   * certificate decryption failed in the external flow
   */
  void ProcessFailedCertDecrypt() OVERRIDE;
#endif

 private:
  /**
   * \brief Sends Handshake binary data to mobile application
   * \param connection_key Unique key used by other components as session
   * identifier
   * \param data pointer to binary data array
   * \param data_size size of binary data array
   * \param seq_number received from Mobile Application
   */
  void SendHandshakeBinData(const uint32_t connection_key,
                            const uint8_t* const data,
                            const size_t data_size,
                            const uint32_t seq_number = 0);
  /**
   * \brief Parse SecurityMessage as HandshakeData request
   * \param inMessage SecurityMessage with binary data of handshake
   */
  bool ProcessHandshakeData(const SecurityMessage& inMessage);
  /**
   * \brief Parse InternalError from mobile side
   * \param inMessage SecurityMessage with binary data of handshake
   */
  bool ProcessInternalError(const SecurityMessage& inMessage);

  /**
   * \brief Sends security query
   * Create new array as concatenation of header and binary data
   * \param query SecurityQuery for sending via Control service
   * \param connection_key Unique key used by other components as session
   * identifier
   */
  void SendQuery(const SecurityQuery& query, const uint32_t connection_key);

  /**
   * @brief OnCertificateUpdated allows to obtain notification when certificate
   * has been updated with policy table update. Pass this certificate to crypto
   * manager for further processing. Also process postopnes handshake for the
   * certain connection key.
   *
   * @param data the certificates content.
   * @return always true.
   */
  bool OnCertificateUpdated(const std::string& data) OVERRIDE;

  /**
   * @brief ResumeHandshake allows to resume handshake after certificate has
   * been updated.
   * @param connection_key the connection identifier to start handshake.
   */
  void ResumeHandshake(uint32_t connection_key);

  /**
   * @brief ProceedHandshake starts the handshake process.
   * @param ssl_context ssl context for the handshake. COntains certificate,
   * keys, etc.
   * @param connection_key the connection identifier to process handshake.
   */
  void ProceedHandshake(SSLContext* ssl_context, uint32_t connection_key);

  /**
   * @brief OnSystemTimeArrived method which notifies
   * crypto manager with updated time in order to check certificate validity
   * @param utc_time the current system time.
   */
  void OnSystemTimeArrived(const time_t utc_time) OVERRIDE;

  /**
   * @brief OnSystemTimeFailed Notify about system request failure
   */
  void OnSystemTimeFailed() OVERRIDE;

  /**
   * @brief ResetPendingSystemTimeRequests resets waiting for system time
   * requests flag
   */
  void ResetPendingSystemTimeRequests();

  // Thread that pumps handshake data
  SecurityMessageLoop security_messages_;

  /**
   *\brief Pointer on instance of class implementing SessionObserver
   */
  protocol_handler::SessionObserver* session_observer_;
  /**
   *\brief Pointer on instance of class implementing CryptoManager
   */
  security_manager::CryptoManager* crypto_manager_;
  /**
   *\brief Pointer on instance of class implementing ProtocolHandler
   */
  protocol_handler::ProtocolHandler* protocol_handler_;
  /**
   *\brief List of listeners for notify handshake done result
   */

  std::list<SecurityManagerListener*> listeners_;

  std::unique_ptr<utils::SystemTimeHandler> system_time_handler_;
  sync_primitives::Lock connections_lock_;
  std::set<uint32_t> awaiting_certificate_connections_;
  std::set<uint32_t> awaiting_time_connections_;

  mutable sync_primitives::Lock waiters_lock_;
  volatile bool waiting_for_certificate_;
  volatile bool waiting_for_time_;
  DISALLOW_COPY_AND_ASSIGN(SecurityManagerImpl);
};
}  // namespace security_manager
#endif  // SRC_COMPONENTS_SECURITY_MANAGER_INCLUDE_SECURITY_MANAGER_SECURITY_MANAGER_IMPL_H_
