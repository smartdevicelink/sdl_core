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
#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_TEST_INCLUDE_PROTOCOL_HANDLER_MOCK_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_TEST_INCLUDE_PROTOCOL_HANDLER_MOCK_H_

#include <gmock/gmock.h>
#include "transport_manager/transport_manager.h"
#include "protocol_handler/session_observer.h"
#include "protocol_handler/protocol_handler.h"
#include "protocol_handler/protocol_handler_settings.h"

namespace test {
namespace components {
namespace protocol_handler_test {

using namespace protocol_handler;
using namespace transport_manager;

/*
 * MOCK implementation of ::protocol_handler::ProtocolObserver interface
 */

class ProtocolHandlerMock : public protocol_handler::ProtocolHandler {
 public:
  MOCK_METHOD2(SendMessageToMobileApp,
      void(const ::protocol_handler::RawMessagePtr message,
          bool final_message));
  MOCK_METHOD1(AddProtocolObserver,
      void(::protocol_handler::ProtocolObserver *observer));
  MOCK_METHOD1(RemoveProtocolObserver,
      void(::protocol_handler::ProtocolObserver *observer));
  MOCK_METHOD2(SendFramesNumber,
      void(uint32_t connection_key, int32_t number_of_frames));
  MOCK_METHOD2(SendHeartBeat, void(int32_t connection_id, uint8_t session_id));
  MOCK_METHOD2(SendEndSession, void(int32_t connection_id, uint8_t session_id));
  MOCK_METHOD3(SendEndService,
      void(int32_t connection_id, uint8_t session_id, uint8_t service_type));
  MOCK_CONST_METHOD0(get_settings,
                     const ::protocol_handler::ProtocolHandlerSettings&());
};

#ifdef ENABLE_SECURITY
/*
 * MOCK implementation of security_manager::SecurityManager
 */
class SecurityManagerMock : public security_manager::SecurityManager {
 public:
  MOCK_METHOD1(AddListener,
      void(security_manager::SecurityManagerListener *));
  MOCK_METHOD1(CreateSSLContext,
      security_manager::SSLContext*(const uint32_t &));
  MOCK_METHOD1(StartHandshake,
      void(uint32_t));
  MOCK_METHOD4(SendInternalError,
      void(const uint32_t ,
          const uint8_t&,
          const std::string&,
          const uint32_t ));

  MOCK_METHOD1(set_session_observer,
      void(::protocol_handler::SessionObserver *));
  MOCK_METHOD1(set_protocol_handler,
      void(::protocol_handler::ProtocolHandler *));
  MOCK_METHOD1(set_crypto_manager,
      void(::security_manager::CryptoManager *));
  MOCK_METHOD1(RemoveListener,
      void(::security_manager::SecurityManagerListener *));
  // protocol_handler::ProtocolObserver part
  MOCK_METHOD1(OnMessageReceived,
      void(const ::protocol_handler::RawMessagePtr));
  MOCK_METHOD1(OnMobileMessageSent,
      void(const ::protocol_handler::RawMessagePtr));
};

class SSLContextMock : public security_manager::SSLContext {
 public:
  MOCK_CONST_METHOD0(mode, int ());
  MOCK_METHOD2(StartHandshake,
      security_manager::SSLContext::HandshakeResult (
          const uint8_t** const, size_t*));
  MOCK_METHOD4(DoHandshakeStep,
      security_manager::SSLContext::HandshakeResult (
          const uint8_t* const, size_t,
          const uint8_t** const, size_t*));
  MOCK_METHOD4(Encrypt,
      bool (const uint8_t* const, size_t,
          const uint8_t** const, size_t*));
  MOCK_METHOD4(Decrypt,
      bool (const uint8_t* const, size_t,
          const uint8_t** const, size_t*));
  MOCK_CONST_METHOD1(get_max_block_size, size_t (size_t));
  MOCK_CONST_METHOD0(IsInitCompleted, bool());
  MOCK_CONST_METHOD0(IsHandshakePending, bool());
  MOCK_CONST_METHOD0(LastError,
                     std::string());
  MOCK_METHOD0(ResetConnection,
               void());
  MOCK_METHOD1(SetHandshakeContext, void (const HandshakeContext& hsh_ctx));
};
#endif  // ENABLE_SECURITY
}
  // namespace test
} // namespace components
}  // namespace protocol_handler_test
#endif  // SRC_COMPONENTS_PROTOCOL_HANDLER_TEST_INCLUDE_PROTOCOL_HANDLER_MOCK_H_

