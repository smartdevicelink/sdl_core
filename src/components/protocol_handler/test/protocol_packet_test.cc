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

#include <gtest/gtest.h>
#include <vector>
#include <list>

#include "utils/macro.h"
#include "protocol_handler/protocol_packet.h"

namespace test {
namespace components {
namespace protocol_handler_test {
using namespace ::protocol_handler;

class ProtocolPacketTest : public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    some_message_id = 0xABCDEF0;
    some_session_id = 0xFEDCBA0;
    some_connection_id = 10;
  }
  uint32_t some_message_id;
  uint32_t some_session_id;
  ConnectionID some_connection_id;
};

TEST_F(ProtocolPacketTest, SerializePacketWithDiffVersions) {
  RawMessagePtr res;
  uint8_t version = PROTOCOL_VERSION_1;
  for (; version <= PROTOCOL_VERSION_MAX; ++version) {
    ProtocolPacket prot_packet(
        some_connection_id, version, PROTECTION_OFF, FRAME_TYPE_CONTROL,
        kControl, FRAME_DATA_HEART_BEAT, some_session_id, 0u, some_message_id);
    res = prot_packet.serializePacket();
    EXPECT_EQ(res->protocol_version(), version);
    EXPECT_EQ(res->service_type(), kControl);
    EXPECT_EQ(res->connection_key(), some_connection_id);
    if (res->protocol_version() == PROTOCOL_VERSION_1) {
      EXPECT_EQ(res->data_size(), 8u);
    } else {
      EXPECT_EQ(res->data_size(), 12u);
    }
  }
}

// ServiceType should be equal 0x0 (Control), 0x07 (RPC), 0x0A (PCM), 0x0B
// (Video), 0x0F (Bulk)
TEST_F(ProtocolPacketTest, SerializePacketWithDiffServiceType) {
  std::vector<uint8_t> serv_types;
  serv_types.push_back(0x0);
  serv_types.push_back(0x07);
  serv_types.push_back(0x0A);
  serv_types.push_back(0x0B);
  serv_types.push_back(0x0F);

  RawMessagePtr res;
  for (size_t i = 0; i < serv_types.size(); ++i) {
    ProtocolPacket prot_packet(some_connection_id, PROTOCOL_VERSION_3,
                               PROTECTION_OFF, FRAME_TYPE_CONTROL,
                               serv_types[i], FRAME_DATA_HEART_BEAT,
                               some_session_id, 0u, some_message_id);
    res = prot_packet.serializePacket();
    EXPECT_EQ(PROTOCOL_VERSION_3, res->protocol_version());
    EXPECT_EQ(serv_types[i], res->service_type());
    EXPECT_EQ(12u, res->data_size());
  }
}

TEST_F(ProtocolPacketTest, SerializePacketWithWrongServiceType) {
  std::vector<uint8_t> serv_types;
  for (uint8_t service_type = kControl + 1; service_type < kRpc;
       ++service_type) {
    serv_types.push_back(service_type);
  }
  serv_types.push_back(0x08);
  serv_types.push_back(0x09);
  serv_types.push_back(0x0C);
  serv_types.push_back(0x0D);
  serv_types.push_back(0x0E);

  RawMessagePtr res;
  for (size_t i = 0; i < serv_types.size(); ++i) {
    ProtocolPacket prot_packet(some_connection_id, PROTOCOL_VERSION_3,
                               PROTECTION_OFF, FRAME_TYPE_CONTROL,
                               serv_types[i], FRAME_DATA_HEART_BEAT,
                               some_session_id, 0u, some_message_id);
    res = prot_packet.serializePacket();
    EXPECT_EQ(PROTOCOL_VERSION_3, res->protocol_version());
    EXPECT_EQ(kInvalidServiceType, res->service_type());
  }
}

TEST_F(ProtocolPacketTest, SetPacketWithDiffFrameType) {
  RawMessagePtr res;
  uint8_t frame_type;
  for (frame_type = FRAME_TYPE_CONTROL + 1; frame_type <= FRAME_TYPE_MAX_VALUE;
       ++frame_type) {
    ProtocolPacket prot_packet(
        some_connection_id, PROTOCOL_VERSION_3, PROTECTION_OFF, frame_type,
        kControl, FRAME_DATA_HEART_BEAT, some_session_id, 0u, some_message_id);
    res = prot_packet.serializePacket();
    EXPECT_EQ(PROTOCOL_VERSION_3, res->protocol_version());
    EXPECT_EQ(kControl, res->service_type());
    EXPECT_EQ(frame_type, prot_packet.frame_type());
  }
}

TEST_F(ProtocolPacketTest, AppendDataToEmptyPacket) {
  // Set version, serviceType, frameData, sessionId
  uint8_t session_id = 1u;
  uint8_t some_data[] = {0x0, 0x07, 0x02, session_id};
  ProtocolPacket protocol_packet;
  RESULT_CODE res = protocol_packet.appendData(some_data, sizeof(some_data));
  EXPECT_EQ(RESULT_FAIL, res);
}

TEST_F(ProtocolPacketTest, SetTotalDataBytes) {
  uint8_t new_data_size = 10u;
  ProtocolPacket protocol_packet;
  protocol_packet.set_total_data_bytes(new_data_size);

  EXPECT_EQ(new_data_size, protocol_packet.total_data_bytes());
}

TEST_F(ProtocolPacketTest, AppendDataToPacketWithNonZeroSize) {
  // Set version, serviceType, frameData, sessionId
  uint8_t session_id = 1u;
  uint8_t some_data[] = {0x0, 0x07, FRAME_TYPE_CONTROL, session_id};
  ProtocolPacket protocol_packet;
  protocol_packet.set_total_data_bytes(sizeof(some_data) + 1);
  RESULT_CODE res = protocol_packet.appendData(some_data, sizeof(some_data));
  EXPECT_EQ(RESULT_OK, res);

  EXPECT_EQ(0x0, protocol_packet.data()[0]);
  EXPECT_EQ(0x07, protocol_packet.data()[1]);
  EXPECT_EQ(FRAME_TYPE_CONTROL, protocol_packet.data()[2]);
  EXPECT_EQ(session_id, protocol_packet.data()[3]);
}

TEST_F(ProtocolPacketTest, SetData) {
  uint8_t session_id = 1u;
  uint8_t some_data[] = {0x0, 0x07, FRAME_TYPE_CONTROL, session_id};
  ProtocolPacket protocol_packet;
  protocol_packet.set_data(some_data, sizeof(some_data));

  EXPECT_EQ(0x0, protocol_packet.data()[0]);
  EXPECT_EQ(0x07, protocol_packet.data()[1]);
  EXPECT_EQ(FRAME_TYPE_CONTROL, protocol_packet.data()[2]);
  EXPECT_EQ(session_id, protocol_packet.data()[3]);
}

TEST_F(ProtocolPacketTest, DeserializeZeroPacket) {
  uint8_t message[] = {};
  ProtocolPacket protocol_packet;
  RESULT_CODE res = protocol_packet.deserializePacket(message, 0);
  EXPECT_EQ(RESULT_OK, res);
}

TEST_F(ProtocolPacketTest, DeserializeNonZeroPacket) {
  // Set header, serviceType, frameData, sessionId
  uint8_t session_id = 1u;
  uint8_t some_message[] = {0x21, 0x07, 0x02, session_id};
  ProtocolPacket protocol_packet;
  RESULT_CODE res =
      protocol_packet.deserializePacket(some_message, PROTOCOL_HEADER_V2_SIZE);
  EXPECT_EQ(RESULT_OK, res);
}

}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test
