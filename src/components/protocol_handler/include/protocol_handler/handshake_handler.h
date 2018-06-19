/*
 * Copyright (c) 2017, Ford Motor Company
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

#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_START_SESSION_HANDLER_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_START_SESSION_HANDLER_H_

#include <memory>

#include "protocol_handler/protocol_handler_impl.h"
#include "protocol_handler/protocol_packet.h"
#include "protocol_handler/session_observer.h"
#include "security_manager/security_manager_listener.h"

namespace protocol_handler {

class ProtocolHandlerImpl;

/**
 * @brief HandshakeHandler class for handling SSL handshake and sending
 * Ack/Nack on its success or fail
 */
class HandshakeHandler : public security_manager::SecurityManagerListener {
 public:
  DEPRECATED HandshakeHandler(ProtocolHandlerImpl& protocol_handler,
                              SessionObserver& session_observer,
                              uint32_t connection_key,
                              ConnectionID connection_id,
                              uint8_t session_id,
                              uint8_t protocol_version,
                              uint32_t hash_id,
                              ServiceType service_type,
                              const std::vector<int>& force_protected_service,
                              const bool is_new_service,
                              ProtocolPacket::ProtocolVersion& full_version,
                              std::shared_ptr<BsonObject> payload);

  HandshakeHandler(ProtocolHandlerImpl& protocol_handler,
                   SessionObserver& session_observer,
                   ProtocolPacket::ProtocolVersion& full_version,
                   const SessionContext& context,
                   const uint8_t protocol_version,
                   std::shared_ptr<BsonObject> payload);

  ~HandshakeHandler();

#ifdef ENABLE_SECURITY
  /**
   * @brief Get certificate data from policy
   * @param reference to string where to save certificate data
   * @return true if listener saved some data to string otherwise false
   */
  bool GetPolicyCertificateData(std::string& data) const OVERRIDE;

  /**
   * @brief Notification about protection result
   * @param connection_key Unique key of session which triggrs handshake
   * @param result result of connection protection
   * @return true on success notification handling or false otherwise
   */
  bool OnHandshakeDone(
      uint32_t connection_key,
      security_manager::SSLContext::HandshakeResult result) OVERRIDE;

  /**
   * @brief Notification about handshake failure
   * @return true on success notification handling or false otherwise
   */
  bool OnHandshakeFailed() OVERRIDE;

  /**
   * @brief Notification that certificate update is required.
   */
  void OnCertificateUpdateRequired() OVERRIDE;
#endif  // ENABLE_SECURITY

  /**
   * @brief Get connection key of this handler
   * @return connection key
   */
  uint32_t connection_key() const;

 private:
  /**
   * @brief Performs related actions if handshake was successfully finished
   * @param connection_key Unique key of session which triggrs handshake
   * @param params set of params used in bson part of message
   */
  void ProcessSuccessfulHandshake(const uint32_t connection_key,
                                  BsonObject& params);

  /**
   * @brief Performs related actions if handshake was failed
   * @param params set of params used in bson part of message
   */
  void ProcessFailedHandshake(BsonObject& params);

  ProtocolHandlerImpl& protocol_handler_;
  SessionObserver& session_observer_;
  SessionContext context_;
  ProtocolPacket::ProtocolVersion full_version_;
  const uint8_t protocol_version_;
  std::shared_ptr<BsonObject> payload_;
};

}  // namespace protocol_handler

#endif  // SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_START_SESSION_HANDLER_H_
