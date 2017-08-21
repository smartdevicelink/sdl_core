/*
 * Copyright (c) 2016, Ford Motor Company
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
#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_TEST_INCLUDE_PROTOCOL_HANDLER_CONTROL_MESSAGE_MATCHER_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_TEST_INCLUDE_PROTOCOL_HANDLER_CONTROL_MESSAGE_MATCHER_H_

#include "gmock/gmock.h"
#include <ios>
#include <string>
#include <vector>
#include "protocol/raw_message.h"
#include "protocol_handler/protocol_packet.h"

namespace test {
namespace components {
namespace protocol_handler_test {

using protocol_handler::ProtocolPacket;
using protocol_handler::RawMessagePtr;
using protocol_handler::RESULT_CODE;
using protocol_handler::FRAME_TYPE_CONTROL;
using protocol_handler::FRAME_DATA_START_SERVICE_NACK;

bool CheckRegularMatches(const ProtocolPacket& packet,
                         RESULT_CODE result,
                         testing::MatchResultListener& result_listener,
                         uint8_t ExpectedFrameType,
                         uint8_t ExpectedFrameData,
                         uint8_t ExpectedEncryption) {
  if (result != protocol_handler::RESULT_OK) {
    result_listener << "Error while message deserialization.";
    return false;
  }
  if (ExpectedFrameType != packet.frame_type()) {
    result_listener << "Message with frame type 0x" << std::hex
                    << static_cast<int>(packet.frame_type()) << ", not 0x"
                    << std::hex << static_cast<int>(ExpectedFrameType);
    return false;
  }
  if (ExpectedFrameData != packet.frame_data()) {
    result_listener << "Message with data 0x" << std::hex
                    << static_cast<int>(packet.frame_data()) << ", not 0x"
                    << std::hex << static_cast<int>(ExpectedFrameData);
    return false;
  }
  if (ExpectedEncryption != packet.protection_flag()) {
    result_listener << "Message is " << (ExpectedEncryption ? "" : "not ")
                    << "protected";
    return false;
  }
  return true;
}

/*
 * Matcher for checking RawMessage with ControlMessage
 * Check error id
 */

MATCHER_P2(ControlMessage,
           ExpectedFrameData,
           ExpectedEncryption,
           (std::string(ExpectedEncryption ? "Protected" : "Unprotected") +
            " control message ")) {
  // Nack shall be always with flag protected off
  if (ExpectedFrameData == FRAME_DATA_START_SERVICE_NACK &&
      ExpectedEncryption) {
    *result_listener << "NACK message with PROTECTION_ON flag";
    return false;
  }
  const RawMessagePtr message = arg;
  ProtocolPacket packet(message->connection_key());
  const RESULT_CODE result =
      packet.deserializePacket(message->data(), message->data_size());

  if (!CheckRegularMatches(packet,
                           result,
                           *result_listener,
                           FRAME_TYPE_CONTROL,
                           ExpectedFrameData,
                           ExpectedEncryption)) {
    return false;
  }
  return true;
}

MATCHER_P4(ControlMessage,
           ExpectedFrameData,
           ExpectedEncryption,
           ConnectionKey,
           VectorMatcher,
           (std::string(ExpectedEncryption ? "Protected" : "Unprotected") +
            " control message ")) {
  // Nack shall be always with flag protected off
  if (ExpectedFrameData == FRAME_DATA_START_SERVICE_NACK &&
      ExpectedEncryption) {
    *result_listener << "NACK message with PROTECTION_ON flag";
    return false;
  }
  const RawMessagePtr message = arg;
  ProtocolPacket packet(message->connection_key());
  const RESULT_CODE result =
      packet.deserializePacket(message->data(), message->data_size());

  if (!CheckRegularMatches(packet,
                           result,
                           *result_listener,
                           FRAME_TYPE_CONTROL,
                           ExpectedFrameData,
                           ExpectedEncryption)) {
    return false;
  }

  if (ConnectionKey != message->connection_key()) {
    *result_listener << "Message for connection_id "
                     << message->connection_key()
                     << ", expected for connection_id " << ConnectionKey;
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
      *result_listener << std::hex << static_cast<int>(data_vector[i]);
    }
    return false;
  }
  return true;
}

/*
 * Matcher for checking RawMessage with any ExpectedMessage
 */

MATCHER_P4(ExpectedMessage,
           ExpectedFrameType,
           ExpectedFrameData,
           ExpectedEncryption,
           ExpectedServiceType,
           (std::string(ExpectedEncryption ? "Protected" : "Unprotected") +
            " message ")) {
  // Nack shall be always with flag protected off
  if (ExpectedFrameType == FRAME_TYPE_CONTROL &&
      ExpectedFrameData == FRAME_DATA_START_SERVICE_NACK &&
      ExpectedEncryption) {
    *result_listener << "NACK message with PROTECTION_ON flag";
    return false;
  }
  const RawMessagePtr message = arg;
  ProtocolPacket packet(message->connection_key());
  const RESULT_CODE result =
      packet.deserializePacket(message->data(), message->data_size());

  if (!CheckRegularMatches(packet,
                           result,
                           *result_listener,
                           ExpectedFrameType,
                           ExpectedFrameData,
                           ExpectedEncryption)) {
    return false;
  }
  if (ExpectedServiceType != packet.service_type()) {
    *result_listener << "Service type is 0x" << std::hex
                     << static_cast<int>(packet.service_type()) << ", not 0x"
                     << std::hex << static_cast<int>(ExpectedServiceType);
    return false;
  }
  return true;
}

}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_PROTOCOL_HANDLER_TEST_INCLUDE_PROTOCOL_HANDLER_CONTROL_MESSAGE_MATCHER_H_
