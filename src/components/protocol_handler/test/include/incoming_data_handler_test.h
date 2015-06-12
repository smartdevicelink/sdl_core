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
#ifndef TEST_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_INCOMING_DATA_HANDLER_TEST_H_
#define TEST_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_INCOMING_DATA_HANDLER_TEST_H_
#include <gtest/gtest.h>
#include <vector>
#include <list>

#include "utils/macro.h"
#include "protocol_handler/incoming_data_handler.h"

namespace test {
namespace components {
namespace protocol_handler_test {
using namespace protocol_handler;

class IncomingDataHandlerTest : public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    data_handler.set_validator(&header_validator);
    uid1 = 0x1234560;
    data_handler.AddConnection(uid1);
    uid2 = 0x1234561;
    data_handler.AddConnection(uid2);
    uid_unknown = 0xFEFEFE;
    EXPECT_NE(uid1, uid_unknown);
    EXPECT_NE(uid2, uid_unknown);
    some_data_size = 4;
    some_data2_size = 512;
    some_data = new uint8_t[some_data_size];
    some_data2 = new uint8_t[some_data2_size];
    protov1_message_id = 0x0;
    some_message_id = 0xABCDEF0;
    some_session_id = 0xFEDCBA0;
    payload_bigger_mtu.resize(MAXIMUM_FRAME_DATA_SIZE + 1);
  }
  void TearDown() OVERRIDE {
    delete[] some_data;
    delete[] some_data2;
  }
  void ProcessData(transport_manager::ConnectionUID uid, const uint8_t *const data,
                   const uint32_t data_size ) {
    actual_frames = data_handler.ProcessData(RawMessage(uid, 0, data, data_size),
                                             &result_code);
  }

  void AppendPacketToTMData(const ProtocolPacket& packet) {
    const RawMessagePtr msg = packet.serializePacket();
    EXPECT_TRUE(msg.valid());
    EXPECT_GT(msg->data_size(), 0u);
    tm_data.insert(tm_data.end(), msg->data(), msg->data() + msg->data_size());
  }
  void ProcessPacket(const ProtocolPacket& packet) {
    AppendPacketToTMData(packet);
    ProcessData(uid1, &tm_data[0], tm_data.size());
    tm_data.clear();
  }

  protocol_handler::ProtocolPacket::ProtocolHeaderValidator header_validator;
  protocol_handler::IncomingDataHandler data_handler;
  transport_manager::ConnectionUID uid1, uid2, uid_unknown;
  typedef std::list<ProtocolFramePtr> FrameList;
  FrameList actual_frames;
  RESULT_CODE result_code;
  uint8_t* some_data, *some_data2;
  size_t some_data_size, some_data2_size;
  uint32_t protov1_message_id;
  uint32_t some_message_id;
  uint32_t some_session_id;
  std::vector<uint8_t> tm_data;
  std::vector<uint8_t> payload_bigger_mtu;
};

TEST_F(IncomingDataHandlerTest, NullData) {
  ProcessData(uid1, NULL, 0);
  EXPECT_EQ(RESULT_FAIL, result_code);
  EXPECT_TRUE(actual_frames.empty());

  ProcessData(uid2, NULL, 1);
  EXPECT_EQ(RESULT_FAIL, result_code);
  EXPECT_TRUE(actual_frames.empty());

  uint8_t invalide_data[] = {0, 1, 2, 3, 4};
  ProcessData(uid_unknown, invalide_data, 0);
  EXPECT_EQ(RESULT_FAIL, result_code);
  EXPECT_TRUE(actual_frames.empty());
}

TEST_F(IncomingDataHandlerTest, DataForUnknownConnection) {
  actual_frames = data_handler.ProcessData(RawMessage(uid_unknown, 0, NULL, 0),
                                    &result_code);
  EXPECT_EQ(RESULT_FAIL, result_code);
  EXPECT_TRUE(actual_frames.empty());

  AppendPacketToTMData(ProtocolPacket());
  actual_frames = data_handler.ProcessData(RawMessage(uid_unknown, 0, tm_data.data(), tm_data.size()),
                                    &result_code);
  EXPECT_EQ(RESULT_FAIL, result_code);
  EXPECT_TRUE(actual_frames.empty());
}

TEST_F(IncomingDataHandlerTest, Heartbeat_per_byte) {
  const ProtocolPacket hb_packet(uid1, PROTOCOL_VERSION_1, PROTECTION_OFF, FRAME_TYPE_CONTROL,
                                 kControl, FRAME_DATA_HEART_BEAT, some_session_id, 0u,
                                 protov1_message_id, NULL);
  const size_t hb_count = 100;
  for (size_t i = 0; i < hb_count; ++i) {
    AppendPacketToTMData(hb_packet);
    // Send per 1 byte (except last byte)
    for (size_t i = 0; i < tm_data.size() - 1; ++i) {
      ProcessData(uid1, &tm_data[i] , 1);
      EXPECT_EQ(RESULT_OK, result_code);
      EXPECT_TRUE(actual_frames.empty());
    }
    ProcessData(uid1, &*(tm_data.end()-1), 1);
    EXPECT_EQ(RESULT_OK, result_code);
    EXPECT_EQ(1u, actual_frames.size());
    EXPECT_EQ(hb_packet, **actual_frames.begin());
    tm_data.clear();
  }
}

TEST_F(IncomingDataHandlerTest, Heartbeat_pack) {
  const ProtocolPacket hb_packet(uid1, PROTOCOL_VERSION_2, PROTECTION_OFF, FRAME_TYPE_CONTROL,
                                 kControl, FRAME_DATA_HEART_BEAT, some_session_id, 0u,
                                 some_message_id, NULL);
  const size_t hb_count = 100;
  for (size_t i = 0u; i < hb_count; ++i) {
    AppendPacketToTMData(hb_packet);
  }
  ProcessData(uid1, &tm_data[0], tm_data.size());
  EXPECT_EQ(RESULT_OK, result_code);
  EXPECT_EQ(hb_count, actual_frames.size());
  for (FrameList::iterator it = actual_frames.begin(); it != actual_frames.end(); ++it) {
    EXPECT_EQ(hb_packet, **it);
  }
}

TEST_F(IncomingDataHandlerTest, MixedPayloadData_TwoConnections) {
  FrameList mobile_packets;
  // single packet RPC
  mobile_packets.push_back(
        new ProtocolPacket(
          uid1, PROTOCOL_VERSION_1, PROTECTION_OFF, FRAME_TYPE_SINGLE,
          kRpc, FRAME_DATA_SINGLE, some_session_id, some_data_size,
          protov1_message_id, some_data));
  // consecutive packet Audio
  mobile_packets.push_back(
        new ProtocolPacket(
          uid1, PROTOCOL_VERSION_2, PROTECTION_ON, FRAME_TYPE_CONSECUTIVE,
          kAudio, FRAME_DATA_LAST_CONSECUTIVE, ++some_session_id, some_data2_size,
          some_message_id, some_data2));
  // single packet Nav
  mobile_packets.push_back(
        new ProtocolPacket(
          uid1, PROTOCOL_VERSION_3, PROTECTION_ON, FRAME_TYPE_SINGLE,
          kMobileNav, FRAME_DATA_SINGLE, ++some_session_id, some_data_size,
          ++some_message_id, some_data));
  // consecutive packet Bulk
  mobile_packets.push_back(
        new ProtocolPacket(
          uid1, PROTOCOL_VERSION_3, PROTECTION_ON, FRAME_TYPE_CONSECUTIVE,
          kBulk, FRAME_DATA_LAST_CONSECUTIVE, ++some_session_id, some_data2_size,
          ++some_message_id, some_data2));
  for (FrameList::iterator it = mobile_packets.begin(); it != mobile_packets.end(); ++it) {
    AppendPacketToTMData(**it);
  }
  ProcessData(uid1, &tm_data[0], tm_data.size());
  EXPECT_EQ(RESULT_OK, result_code);
  EXPECT_EQ(actual_frames.size(), mobile_packets.size());
  FrameList::const_iterator it2 = mobile_packets.begin();
  for (FrameList::const_iterator it = actual_frames.begin(); it != actual_frames.end();
       ++it, ++it2) {
    // TODO(EZamakhov): investigate valgrind warning (unitialized value)
    EXPECT_EQ(**it, **it2);
  }
}

// TODO(EZamakhov): add validator abstraction and replace next test with check only return frames

// Protocol version shall be from 1 to 3
TEST_F(IncomingDataHandlerTest, MalformedPacket_Version) {
  FrameList malformed_packets;
  std::vector<uint8_t> malformed_versions;
  malformed_versions.push_back(0);
  for (uint8_t version = PROTOCOL_VERSION_3 + 1; version <= PROTOCOL_VERSION_MAX; ++version) {
    malformed_versions.push_back(version);
  }
  for (size_t i = 0; i < malformed_versions.size(); ++i) {
    malformed_packets.push_back(
          new ProtocolPacket(
            uid1, malformed_versions[i], PROTECTION_OFF, FRAME_TYPE_CONTROL, kControl,
            FRAME_DATA_HEART_BEAT, some_session_id, 0u, some_message_id, NULL));
  }
  for (FrameList::iterator it = malformed_packets.begin(); it != malformed_packets.end(); ++it) {
    ProcessPacket(**it);
    EXPECT_EQ(RESULT_FAIL, result_code)
        << "Malformed vesion " << static_cast<int>((*it)->protocol_version());
    // All malformed messages shall be ignored
    EXPECT_EQ(0u, actual_frames.size());
  }
}

// ServiceType shall be equal 0x0 (Control), 0x07 (RPC), 0x0A (PCM), 0x0B (Video), 0x0F (Bulk)
TEST_F(IncomingDataHandlerTest, MalformedPacket_ServiceType) {
  FrameList malformed_packets;
  std::vector<uint8_t> malformed_serv_types;
  for (uint8_t service_type = kControl + 1; service_type < kRpc; ++service_type) {
    malformed_serv_types.push_back(service_type);
  }
  malformed_serv_types.push_back(0x08);
  malformed_serv_types.push_back(0x09);
  malformed_serv_types.push_back(0x0C);
  malformed_serv_types.push_back(0x0D);
  malformed_serv_types.push_back(0x0E);
  for (size_t i = 0; i < malformed_serv_types.size(); ++i) {
    malformed_packets.push_back(
          new ProtocolPacket(
            uid1, PROTOCOL_VERSION_3, PROTECTION_OFF, FRAME_TYPE_CONTROL,
            malformed_serv_types[i], FRAME_DATA_HEART_BEAT, some_session_id, 0u,
            some_message_id, NULL));
  }
  for (FrameList::iterator it = malformed_packets.begin(); it != malformed_packets.end(); ++it) {
    ProcessPacket(**it);
    EXPECT_EQ(RESULT_FAIL, result_code)
        << "Malformed service type " << static_cast<int>((*it)->service_type());
    // All malformed messages shall be ignored
    EXPECT_EQ(0u, actual_frames.size());
  }
}

// Frame type shall be 0x00 (Control), 0x01 (Single), 0x02 (First), 0x03 (Consecutive)
TEST_F(IncomingDataHandlerTest, MalformedPacket_FrameType) {
  FrameList malformed_packets;
  std::vector<uint8_t> malformed_frame_types;
  for (uint8_t frame_type = FRAME_TYPE_CONSECUTIVE + 1;
       frame_type <= FRAME_TYPE_MAX_VALUE; ++frame_type) {
    malformed_frame_types.push_back(frame_type);
  }
  for (size_t i = 0; i < malformed_frame_types.size(); ++i) {
    malformed_packets.push_back(
          new ProtocolPacket(
            uid1, PROTOCOL_VERSION_3, PROTECTION_OFF, malformed_frame_types[i],
            kControl, FRAME_DATA_HEART_BEAT, some_session_id, 0u, some_message_id, NULL));
  }
  for (FrameList::iterator it = malformed_packets.begin(); it != malformed_packets.end(); ++it) {
    ProcessPacket(**it);
    EXPECT_EQ(RESULT_FAIL, result_code)
        << "Malformed frame type " << static_cast<int>((*it)->service_type());
    // All malformed messages shall be ignored
    EXPECT_EQ(0u, actual_frames.size());
  }
}

// For Control frames Frame info value shall be from 0x00 to 0x06 or 0xFE(Data Ack), 0xFF(HB Ack)
TEST_F(IncomingDataHandlerTest, MalformedPacket_ControlFrame) {
  FrameList malformed_packets;
  std::vector<uint8_t> malformed_frame_data;
  for (uint8_t frame_type = FRAME_DATA_END_SERVICE_NACK + 1;
       frame_type < FRAME_DATA_SERVICE_DATA_ACK; ++frame_type) {
    malformed_frame_data.push_back(frame_type);
  }
  for (size_t i = 0; i < malformed_frame_data.size(); ++i) {
    malformed_packets.push_back(
          new ProtocolPacket(
            uid1, PROTOCOL_VERSION_3, PROTECTION_OFF, FRAME_TYPE_CONTROL, kControl,
            malformed_frame_data[i], some_session_id, 0u, some_message_id, NULL));
  }
  for (FrameList::iterator it = malformed_packets.begin(); it != malformed_packets.end(); ++it) {
    ProcessPacket(**it);
    EXPECT_EQ(RESULT_FAIL, result_code)
        << "Malformed Control frame with data " << static_cast<int>((*it)->frame_data());
    // All malformed messages shall be ignored
    EXPECT_EQ(0u, actual_frames.size());
  }
}
// For Single and First frames Frame info value shall be equal 0x00
TEST_F(IncomingDataHandlerTest, MalformedPacket_SingleFrame) {
  FrameList malformed_packets;
  std::vector<uint8_t> malformed_frame_data;
  for (uint8_t frame_type = FRAME_DATA_SINGLE + 1;
       frame_type < FRAME_DATA_MAX_VALUE; ++frame_type) {
    malformed_frame_data.push_back(frame_type);
  }
  malformed_frame_data.push_back(FRAME_DATA_MAX_VALUE);
  for (size_t i = 0; i < malformed_frame_data.size(); ++i) {
    malformed_packets.push_back(
          new ProtocolPacket(
            uid1, PROTOCOL_VERSION_3, PROTECTION_OFF, FRAME_TYPE_SINGLE, kControl,
            malformed_frame_data[i], some_session_id, 0u, some_message_id, NULL));
  }
  for (FrameList::iterator it = malformed_packets.begin(); it != malformed_packets.end(); ++it) {
    ProcessPacket(**it);
    EXPECT_EQ(RESULT_FAIL, result_code)
        << "Malformed Single frame with data " << static_cast<int>((*it)->frame_data());
    // All malformed messages shall be ignored
    EXPECT_EQ(0u, actual_frames.size());
  }
}

// For Single and First frames Frame info value shall be equal 0x00
TEST_F(IncomingDataHandlerTest, MalformedPacket_FirstFrame) {
  FrameList malformed_packets;
  std::vector<uint8_t> malformed_frame_data;
  for (uint8_t frame_type = FRAME_DATA_FIRST + 1;
       frame_type < FRAME_DATA_MAX_VALUE; ++frame_type) {
    malformed_frame_data.push_back(frame_type);
  }
  malformed_frame_data.push_back(FRAME_DATA_MAX_VALUE);
  for (size_t i = 0; i < malformed_frame_data.size(); ++i) {
    malformed_packets.push_back(
          new ProtocolPacket(
            uid1, PROTOCOL_VERSION_3, PROTECTION_OFF, FRAME_TYPE_SINGLE, kControl,
            malformed_frame_data[i], some_session_id, 0u, some_message_id, NULL));
  }
  for (FrameList::iterator it = malformed_packets.begin(); it != malformed_packets.end(); ++it) {
    ProcessPacket(**it);
    EXPECT_EQ(RESULT_FAIL, result_code)
        << "Malformed First frame with data " << static_cast<int>((*it)->frame_data());
    // All malformed messages shall be ignored
    EXPECT_EQ(0u, actual_frames.size());
  }
}

// TODO(EZamakhov): add correctness on handling 2+ connection data

}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test
#endif  // TEST_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_INCOMING_DATA_HANDLER_TEST_H_
