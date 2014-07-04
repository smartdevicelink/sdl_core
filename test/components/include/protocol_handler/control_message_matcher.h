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
#ifndef TEST_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_CONTROL_MESSAGE_MATCHER_H_
#define TEST_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_CONTROL_MESSAGE_MATCHER_H_

#include <gmock/gmock.h>
#include <string>

namespace test {
namespace components {
namespace protocol_handler_test {

/*
 * Matcher for checking RawMessage with ControlMessage
 * Check error id
 */
MATCHER_P2(ControlMessage, ExpectedFrameData, ExpectedEncryption,
           std::string(ExpectedEncryption ? "Protected" : "Unprotected")
           + " control message ") {
  // Nack shall be always with flag protected off
  DCHECK(ExpectedFrameData  != 0x03 /*FRAME_DATA_START_SERVICE_NACK*/ ||
         !ExpectedEncryption);
  const ::protocol_handler::RawMessagePtr message = arg;
  const ::protocol_handler::ProtocolPacket packet(
        message->connection_key(), message->data(), message->data_size());
  if (::protocol_handler::FRAME_TYPE_CONTROL != packet.frame_type()) {
    *result_listener << "Is not control message";
    return false;
  }
  if (ExpectedFrameData != packet.frame_data()) {
    *result_listener << "Control message is not with data 0x"
                     << std::hex << static_cast<int>(ExpectedFrameData);
    return false;
  }
  if (ExpectedEncryption != packet.protection_flag()) {
    *result_listener << "Control message is " <<
                     (ExpectedEncryption ? "" : "not ") << "protected";
    return false;
  }
  return true;
}
}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test
#endif  // TEST_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_CONTROL_MESSAGE_MATCHER_H_
