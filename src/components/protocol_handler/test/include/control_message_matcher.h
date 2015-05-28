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
#include <vector>
#include "protocol/raw_message.h"
#include "protocol_handler/protocol_packet.h"

namespace test {
namespace components {
namespace protocol_handler_test {
/*
 * Matcher for checking RawMessage with ControlMessage
 * Check error id
 */

MATCHER_P2(ControlMessage, ExpectedFrameData, ExpectedEncryption,
           (std::string(ExpectedEncryption ? "Protected" : "Unprotected")
           + " control message ")) {
  // Nack shall be always with flag protected off
  DCHECK(ExpectedFrameData  != 0x03 /*FRAME_DATA_START_SERVICE_NACK*/ ||
         !ExpectedEncryption);
  const ::protocol_handler::RawMessagePtr message = arg;
  ::protocol_handler::ProtocolPacket packet(message->connection_key());
  const protocol_handler::RESULT_CODE result =
      packet.deserializePacket(message->data(), message->data_size());
  if (result != protocol_handler::RESULT_OK) {
    *result_listener <<  "Error while message deserialization.";
    return false;
  }
  if (::protocol_handler::FRAME_TYPE_CONTROL != packet.frame_type()) {
    *result_listener << "Is not control message";
    return false;
  }
  if (ExpectedFrameData != packet.frame_data()) {
    *result_listener << "Control message with data 0x"
                     << std::hex << static_cast<int>(packet.frame_data())
                     << ", not 0x"
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



MATCHER_P4(ControlMessage, ExpectedFrameData, ExpectedEncryption,
           ConnectionKey, VectorMatcher,
           (std::string(ExpectedEncryption ? "Protected" : "Unprotected")
           + " control message ")) {
  // Nack shall be always with flag protected off
  DCHECK(ExpectedFrameData  != 0x03 /*FRAME_DATA_START_SERVICE_NACK*/ ||
         !ExpectedEncryption);

  const ::protocol_handler::RawMessagePtr message = arg;
  ::protocol_handler::ProtocolPacket packet(message->connection_key());
  const protocol_handler::RESULT_CODE result =
      packet.deserializePacket(message->data(), message->data_size());
  if (result != protocol_handler::RESULT_OK) {
    *result_listener <<  "Error while message deserialization.";
    return false;
  }

  if (::protocol_handler::FRAME_TYPE_CONTROL != packet.frame_type()) {
    *result_listener << "Is not control message";
    return false;
  }
  if (ExpectedFrameData != packet.frame_data()) {
    *result_listener << "Control message with data 0x"
                     << std::hex << static_cast<int>(packet.frame_data())
                     << ", not 0x"
                     << std::hex << static_cast<int>(ExpectedFrameData);
    return false;
  }
  if (ExpectedEncryption != packet.protection_flag()) {
    *result_listener << "Control message is " <<
                     (ExpectedEncryption ? "" : "not ") << "protected";
    return false;
  }
  if (ConnectionKey != message->connection_key()) {
    *result_listener << "Message for connection_id " << message->connection_key() <<
                        ", expected for connection_id " << ConnectionKey;
    return false;
  }
  std::vector<uint8_t> data_vector;
  if (packet.data() && packet.data_size()) {
    data_vector.assign(packet.data(), packet.data() + packet.data_size());
  }
  ::testing::Matcher<std::vector<uint8_t> > m = VectorMatcher;
  if (!m.Matches(data_vector)) {
    *result_listener << "Message with " << data_vector.size()
                     << " byte data : 0x";
    for (size_t i = 0u; i < data_vector.size(); ++i) {
      *result_listener  << std::hex << static_cast<int>(data_vector[i]);
    }
    return false;
  }
  return true;
}




}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test
#endif  // TEST_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_CONTROL_MESSAGE_MATCHER_H_
