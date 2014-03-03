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

#ifndef SECURE_MANAGER_H
#define SECURE_MANAGER_H

#include <log4cxx/log4cxx.h>

#include "protocol_handler/protocol_observer.h"
#include "protocol_handler/raw_message.h"
#include "protocol_handler/session_observer.h"

#include "utils/message_queue.h"
#include "utils/message_queue.h"
#include "utils/threads/message_loop_thread.h"

#include "secure_service_manager/crypto_manager.h"
#include "secure_service_manager/secure_service_manager.h"
#include "secure_service_manager/secure_service_query.h"

namespace secure_service_manager {

/*
 * These dummy classes are here to locally impose strong typing on different
 * kinds of messages
 * Currently there is no type difference between incoming and outgoing messages
 * And due to ApplicationManagerImpl works as message router it has to distinguish
 * messages passed from it's different connection points
 * TODO(ik): replace these with globally defined message types
 * when we have them.
 */
struct SecureServiceMessage: public SecureServiceQueryPtr {
  SecureServiceMessage(const SecureServiceQueryPtr& message)
    : SecureServiceQueryPtr(message) {}
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const { return 0; }
};

typedef utils::PrioritizedQueue<SecureServiceMessage> SecureServiceMessageQueue;

typedef threads::MessageLoopThread<SecureServiceMessageQueue> SecureServiceMessageLoop;

// TODO: (EZAmakhov) add brief
class SecureServiceManager :
    public protocol_handler::ProtocolObserver,
    public SecureServiceMessageLoop::Handler {
public:

  /**
   * \brief Constructor
   */
  SecureServiceManager();

  // TODO: (EZamakhov) StartSecureService ????

  /**
   * \brief Add message from mobile to
   * when new message is received from Mobile Application.
   * Overriden ProtocolObserver::OnMessageReceived method
   * \param message Message with supporting params received
   */
  void OnMessageReceived(const protocol_handler::RawMessagePtr& message) OVERRIDE;
  void OnMobileMessageSent(const protocol_handler::RawMessagePtr& message) OVERRIDE;

  /**
   * \brief Sets pointer for Connection Handler layer for managing sessions
   * \param observer Pointer to object of the class implementing
   */
  void set_session_observer(protocol_handler::SessionObserver* observer);

  /**
   * \brief Sets pointer for Protocol Handler layer for sending
   * \param Secure Service Requests and Handshake data.
   */
  void set_protocol_handler(protocol_handler::ProtocolHandler* protocol_handler_);

  /**
   * \brief Convert RawMessagePtr to SecureMessagePtr
   * with deep copy data.
   * \param message ProtocoloHandler raw message
   */
  static SecureServiceQueryPtr SecureMessageFromRawMessage(
      const protocol_handler::RawMessagePtr& message);

  // threads::MessageLoopThread<*>::Handler implementations
  // CALLED ON message_for_encryption thread!
  void Handle(const SecureServiceMessage& message) OVERRIDE;
 private:

  bool ProtectServiceRequest(const SecureServiceMessage &message);
  bool ProtectServiceResponse(const SecureServiceMessage &message);
  bool SendHandshakeData(const SecureServiceMessage &message);

  void PostProtectServiceResponse(
      const SecureServiceMessage &requestMessage,
      const SecureServiceQuery::ProtectServiceResult response);

  // Thread that pumps handshake data
  SecureServiceMessageLoop secure_service_messages_;

  secure_service_manager::CryptoManager* crypto_manager_;

  /**
   *\brief Pointer on instance of class implementing SessionObserver
   *\brief (Connection Handler)
   */
  protocol_handler::SessionObserver* session_observer_;

  /**
   *\brief Pointer on instance of class implementing ProtocolHandler
   *\brief (Protocol Handler)
   */
  protocol_handler::ProtocolHandler* protocol_handler_;

  DISALLOW_COPY_AND_ASSIGN(SecureServiceManager);
  static log4cxx::LoggerPtr logger_;
};
} //secure_service_manager
#endif // SECURE_MANAGER_H
