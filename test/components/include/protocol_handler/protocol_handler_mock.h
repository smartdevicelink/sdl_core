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
#ifndef TEST_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_MOCK_H_
#define TEST_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_MOCK_H_

#include <gmock/gmock.h>
#include <string>
#include "protocol_handler/protocol_handler.h"

namespace test {
namespace components {
namespace protocol_handler_test {
/*
 * MOCK implementation of ::protocol_handler::ProtocolObserver interface
 */
class ProtocolHandlerMock: public ::protocol_handler::ProtocolHandler {
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
  MOCK_METHOD2(SendHeartBeat,
               void(int32_t connection_id, uint8_t session_id));
  MOCK_METHOD2(SendEndSession,
               void(int32_t connection_id, uint8_t session_id));
};
}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test
#endif  // TEST_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_MOCK_H_
