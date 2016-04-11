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

#include "gtest/gtest.h"
#include <vector>

#include "utils/macro.h"
#include "protocol_handler/protocol_packet.h"
#include "protocol/common.h"

namespace test {
namespace components {
namespace protocol_handler_test {

using protocol_handler::RawMessagePtr;
using protocol_handler::ProtocolPacket;
using protocol_handler::ConnectionID;
using protocol_handler::FRAME_TYPE_MAX_VALUE;
using protocol_handler::FRAME_DATA_FIRST;
using protocol_handler::FRAME_TYPE_FIRST;
using protocol_handler::PROTOCOL_VERSION_1;
using protocol_handler::PROTOCOL_VERSION_3;
using protocol_handler::PROTECTION_OFF;
using protocol_handler::RESULT_CODE;
using protocol_handler::RESULT_OK;
using protocol_handler::RESULT_FAIL;
using protocol_handler::FRAME_TYPE_CONTROL;
using protocol_handler::kControl;
using protocol_handler::kRpc;
using protocol_handler::kAudio;
using protocol_handler::kMobileNav;
using protocol_handler::kBulk;
using protocol_handler::kInvalidServiceType;
using protocol_handler::FRAME_DATA_HEART_BEAT;
using protocol_handler::FRAME_DATA_START_SERVICE_ACK;
using protocol_handler::FRAME_DATA_LAST_CONSECUTIVE;
using protocol_handler::PROTOCOL_HEADER_V1_SIZE;
using protocol_handler::PROTOCOL_HEADER_V2_SIZE;
using protocol_handler::PROTOCOL_VERSION_MAX;

class ProtocolPacketTest : public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    some_message_id_ = 0xABCDEF0;
    some_session_id_ = 0xFEDCBA0;
    some_connection_id_ = 10;
  }

  RawMessagePtr GetRawMessage(uint8_t version,
                              uint8_t frame_type,
                              uint8_t service_type) {
    ProtocolPacket prot_packet(some_connection_id_,
                               version,
                               PROTECTION_OFF,
                               frame_type,
                               service_type,
                               FRAME_DATA_HEART_BEAT,
                               some_session_id_,
                               0u,
                               some_message_id_);
    EXPECT_EQ(frame_type, prot_packet.frame_type());
    return prot_packet.serializePacket();
  }

  const uint8_t zero_test_data_element_ = 0x0u;
  uint32_t some_message_id_;
  uint32_t some_session_id_;
  ConnectionID some_connection_id_;
};

TEST_F(ProtocolPacketTest, SerializePacketWithDiffVersions) {
  uint8_t version = PROTOCOL_VERSION_1;
  for (; version <= PROTOCOL_VERSION_MAX; ++version) {
    RawMessagePtr res = GetRawMessage(version, FRAME_TYPE_CONTROL, kControl);
    EXPECT_EQ(version, res->protocol_version());
    EXPECT_EQ(kControl, res->service_type());
    EXPECT_EQ(some_connection_id_, res->connection_key());
    if (res->protocol_version() == PROTOCOL_VERSION_1) {
      EXPECT_EQ(PROTOCOL_HEADER_V1_SIZE, res->data_size());
    } else {
      EXPECT_EQ(PROTOCOL_HEADER_V2_SIZE, res->data_size());
    }
  }
}

// ServiceType should be equal 0x0 (Control), 0x07 (RPC), 0x0A (PCM), 0x0B
// (Video), 0x0F (Bulk)
TEST_F(ProtocolPacketTest, SerializePacketWithDiffServiceType) {
  std::vector<uint8_t> serv_types;
  serv_types.push_back(kControl);
  serv_types.push_back(kRpc);
  serv_types.push_back(kAudio);
  serv_types.push_back(kMobileNav);
  serv_types.push_back(kBulk);

  for (size_t i = 0; i < serv_types.size(); ++i) {
    RawMessagePtr res =
        GetRawMessage(PROTOCOL_VERSION_3, FRAME_TYPE_CONTROL, serv_types[i]);
    EXPECT_EQ(PROTOCOL_VERSION_3, res->protocol_version());
    EXPECT_EQ(serv_types[i], res->service_type());
    EXPECT_EQ(PROTOCOL_HEADER_V2_SIZE, res->data_size());
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

  for (size_t i = 0; i < serv_types.size(); ++i) {
    RawMessagePtr res =
        GetRawMessage(PROTOCOL_VERSION_3, FRAME_TYPE_CONTROL, serv_types[i]);
    EXPECT_EQ(PROTOCOL_VERSION_3, res->protocol_version());
    EXPECT_EQ(kInvalidServiceType, res->service_type());
  }
}

TEST_F(ProtocolPacketTest, SetPacketWithDiffFrameType) {
  uint8_t frame_type;
  for (frame_type = FRAME_TYPE_CONTROL + 1;
       frame_type <= FRAME_TYPE_MAX_VALUE;
       ++frame_type) {
    RawMessagePtr res = GetRawMessage(PROTOCOL_VERSION_3, frame_type, kControl);
    EXPECT_EQ(PROTOCOL_VERSION_3, res->protocol_version());
    EXPECT_EQ(kControl, res->service_type());
  }
}

TEST_F(ProtocolPacketTest, AppendDataToEmptyPacket) {
  // Set version, serviceType, frameData, sessionId
  const uint8_t session_id = 1u;
  uint8_t some_data[] = {
      zero_test_data_element_, kRpc, FRAME_DATA_START_SERVICE_ACK, session_id};
  ProtocolPacket protocol_packet;
  RESULT_CODE res = protocol_packet.appendData(some_data, sizeof(some_data));
  EXPECT_EQ(RESULT_FAIL, res);
}

TEST_F(ProtocolPacketTest, SetTotalDataBytes) {
  const uint8_t new_data_size = 10u;
  ProtocolPacket protocol_packet;
  protocol_packet.set_total_data_bytes(new_data_size);
  EXPECT_EQ(new_data_size, protocol_packet.total_data_bytes());
}

TEST_F(ProtocolPacketTest, AppendDataToPacketWithNonZeroSize) {
  // Set version, serviceType, frameData, sessionId
  const uint8_t session_id = 1u;
  uint8_t some_data[] = {
      zero_test_data_element_, kRpc, FRAME_DATA_LAST_CONSECUTIVE, session_id};
  ProtocolPacket protocol_packet;
  protocol_packet.set_total_data_bytes(sizeof(some_data) + 1);
  RESULT_CODE res = protocol_packet.appendData(some_data, sizeof(some_data));
  EXPECT_EQ(RESULT_OK, res);
  EXPECT_EQ(zero_test_data_element_, protocol_packet.data()[0]);
  EXPECT_EQ(kRpc, protocol_packet.data()[1]);
  EXPECT_EQ(FRAME_DATA_LAST_CONSECUTIVE, protocol_packet.data()[2]);
  EXPECT_EQ(session_id, protocol_packet.data()[3]);
}

TEST_F(ProtocolPacketTest, SetData) {
  const uint8_t session_id = 1u;
  uint8_t some_data[] = {
      zero_test_data_element_, kRpc, FRAME_DATA_HEART_BEAT, session_id};
  ProtocolPacket protocol_packet;
  protocol_packet.set_data(some_data, sizeof(some_data));
  EXPECT_EQ(zero_test_data_element_, protocol_packet.data()[0]);
  EXPECT_EQ(kRpc, protocol_packet.data()[1]);
  EXPECT_EQ(FRAME_DATA_HEART_BEAT, protocol_packet.data()[2]);
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
  const uint8_t session_id = 1u;
  const uint8_t version_frame_type = 0x21;
  uint8_t some_message[] = {
      version_frame_type, kRpc, FRAME_DATA_START_SERVICE_ACK, session_id};
  ProtocolPacket protocol_packet;
  RESULT_CODE res =
      protocol_packet.deserializePacket(some_message, PROTOCOL_HEADER_V2_SIZE);
  EXPECT_EQ(RESULT_OK, res);
}

TEST_F(ProtocolPacketTest, DeserializePacket_FrameTypeFirst_ResultOK) {
  // Arrange
  const uint8_t session_id = 1u;
  const uint8_t data_size = 1u;
  const uint8_t version_frame_type = 0x22;
  // Set protol version - 2 and frame type - first
  uint8_t message[] = {version_frame_type,
                       kRpc,
                       FRAME_DATA_FIRST,
                       session_id,
                       zero_test_data_element_,
                       zero_test_data_element_,
                       zero_test_data_element_,
                       data_size,
                       zero_test_data_element_,
                       zero_test_data_element_,
                       zero_test_data_element_,
                       zero_test_data_element_,
                       zero_test_data_element_};
  ProtocolPacket protocol_packet;
  // Act
  RESULT_CODE res =
      protocol_packet.deserializePacket(message, PROTOCOL_HEADER_V2_SIZE);
  uint8_t frame_type = protocol_packet.frame_type();
  // Assert
  EXPECT_EQ(FRAME_TYPE_FIRST, frame_type);
  EXPECT_EQ(RESULT_OK, res);
}

}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test
