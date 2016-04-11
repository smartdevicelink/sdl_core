/*
 * Copyright (c) 2015, Ford Motor Company
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

#include "application_manager/mobile_message_handler.h"

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

#include "gmock/gmock.h"
#include "application_manager/message.h"
#include "protocol/raw_message.h"
#include "utils/make_shared.h"

using protocol_handler::RawMessage;
using protocol_handler::RawMessagePtr;
using protocol_handler::PROTOCOL_HEADER_V1_SIZE;

namespace application_manager {
namespace test {

const int32_t connection_key_p1 = 1;
const int32_t protocol_version_1 = 1;

const std::string data_json("\"{\"menu\": {\"id\": \"file\",\"va"
    "lue\": \"File\",\"popup\": {\"menuitem\": [ {\"val"
    "ue\": \"New\", \"onclick\": \"CreateNe"
    "wDoc()\"}, {\"value\": \"Open\", \"oncli"
    "ck\": \"OpenDoc()\"},{\"value\": \"Cl"
    "ose\", \"onclick\": \"CloseDoc()\"}]}}}\"");

const unsigned char binary[PROTOCOL_HEADER_V1_SIZE] = {
    0x20, 0x00, 0x00, 0xf7,
    0x00, 0x00, 0x00, 0x10 };

const unsigned char* data_v1 =
    reinterpret_cast<const unsigned char*> (data_json.c_str());

TEST(MobileMessageHandlerTestV1Test,
     HandleIncomingMessageProtocolV1_SendJSONData_ExpectEqual) {
  RawMessagePtr message =
      utils::MakeShared<RawMessage>
          (connection_key_p1, protocol_version_1, data_v1, data_json.length());

  application_manager::Message* ptr =
      MobileMessageHandler::HandleIncomingMessageProtocol(message);

  ASSERT_TRUE(ptr);

  EXPECT_EQ(connection_key_p1, ptr->connection_key());
  EXPECT_EQ(protocol_version_1, ptr->protocol_version());
  EXPECT_EQ(data_json, ptr->json_message());
}

TEST(MobileMessageHandlerTestV1Test,
     HandleIncomingMessageProtocolV1_SendBinaryJSONData_ExpectEqual) {

  std::string full_data(reinterpret_cast<const char*>(binary));
  full_data += data_json;
  const unsigned char* data_v1 =
      reinterpret_cast<const unsigned char*> (full_data.c_str());

  RawMessagePtr message =
      utils::MakeShared<RawMessage>
          (connection_key_p1, protocol_version_1, data_v1, full_data.length());

  application_manager::Message* ptr =
      MobileMessageHandler::HandleIncomingMessageProtocol(message);

  ASSERT_TRUE(ptr);

  EXPECT_EQ(connection_key_p1, ptr->connection_key());
  EXPECT_EQ(protocol_version_1, ptr->protocol_version());
  EXPECT_EQ(full_data, ptr->json_message());
}

TEST(MobileMessageHandlerTestV1Test,
     HandleOutgoingMessageProtocol_SendMessage_ExpectEqual) {

  uint32_t connection_key = 1;

  MobileMessage message =
      utils::MakeShared<Message>
          (protocol_handler::MessagePriority::kDefault);

  message->set_protocol_version(application_manager::ProtocolVersion::kV1);
  message->set_json_message(data_json);
  message->set_connection_key(connection_key_p1);

  RawMessage* ptr =
      MobileMessageHandler::HandleOutgoingMessageProtocol(message);

  ASSERT_TRUE(ptr);

  EXPECT_EQ(connection_key, ptr->connection_key());
  EXPECT_EQ(static_cast<uint32_t>(application_manager::ProtocolVersion::kV1),
      ptr->protocol_version());
}

}  // namespace test
}  // namespace application_manager
