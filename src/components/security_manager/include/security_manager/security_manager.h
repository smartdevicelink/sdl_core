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

#include "security_manager/crypto_manager.h"
#include "security_manager/security_manager.h"
#include "security_manager/security_query.h"

namespace security_manager {

/*
 * These dummy classes are here to locally impose strong typing on different
 * kinds of messages
 * Currently there is no type difference between incoming and outgoing messages
 * And due to ApplicationManagerImpl works as message router it has to distinguish
 * messages passed from it's different connection points
 * TODO(ik): replace these with globally defined message types
 * when we have them.
 */
struct SecurityMessage: public SecuityQueryPtr {
  explicit SecurityMessage(const SecuityQueryPtr& message)
    : SecuityQueryPtr(message) {}
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const { return 0; }
};

typedef utils::PrioritizedQueue<SecurityMessage> SecurityMessageQueue;

typedef threads::MessageLoopThread<SecurityMessageQueue> SecurityMessageLoop;

//TODO(EZ): add brief
class SecurityManager :
    public protocol_handler::ProtocolObserver,
    public SecurityMessageLoop::Handler {
public:
  /**
   * \brief Constructor
   */
  SecurityManager();

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
   * \brief Handle SecurityMessage from mobile for processing
   * threads::MessageLoopThread<*>::Handler implementations
   * CALLED ON secure_service_messages_ thread
   */
  void Handle(const SecurityMessage& message) OVERRIDE;

 private:
  bool ParseProtectServiceRequest(const SecurityMessage &requestMessage);
  bool ParseHandshakeData(const SecurityMessage &inMessage);

  void SendProtectServiceResponse(
      const SecurityMessage &message,
      const SecuityQuery::ProtectServiceResult result);

  //Create new array for handling header + data
  void SendInternalError(const int32_t connectionKey,
                         const uint32_t seq_umber,
                         const std::string& error);

  //Create new array for handling header + data
  void SendData(const int32_t connectionKey,
                const SecuityQuery::QueryHeader& header,
                const uint8_t * const data,
                const size_t data_size);

  //post income array as park of RawMessage
  void SendBinaryData(const int32_t connectionKey,
                          uint8_t *data,
                          size_t data_size);

  // Thread that pumps handshake data
  SecurityMessageLoop security_messages_;

  security_manager::CryptoManager* crypto_manager_;

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

  DISALLOW_COPY_AND_ASSIGN(SecurityManager);
  static log4cxx::LoggerPtr logger_;
};
} //security_manager
#endif // SECURE_MANAGER_H
