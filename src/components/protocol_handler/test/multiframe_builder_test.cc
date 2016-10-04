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
#include <map>
#include <algorithm>
#include <utility>
#include <limits>
#include "utils/make_shared.h"
#include "protocol_handler/multiframe_builder.h"

namespace test {
namespace components {
namespace protocol_handler_test {

using namespace protocol_handler;

typedef std::vector<ConnectionID> ConnectionList;
typedef std::vector<uint8_t> UCharDataVector;

struct MutiframeData {
  UCharDataVector binary_data;
  ProtocolFramePtrList multiframes;
};

/**
 *\brief Map of MutiframeData by MessageID key
 */
typedef std::map<MessageID, MutiframeData> MessageIDToMutiframeDataTestMap;
/**
 *\brief Map of MessageIDToMutiframeDataMap by SessionID key
 */
typedef std::map<SessionID, MessageIDToMutiframeDataTestMap>
    SessionToMutiframeDataTestMap;
/**
 *\brief Map of SessionToMutiframeDataMap by ConnectionID key
 */
typedef std::map<ConnectionID, SessionToMutiframeDataTestMap> MultiFrameTestMap;

template <typename IntegerType>
std::vector<IntegerType> GetTestVector() {
  // Prepare array with a few minimals, middle and a few maximum values
  const IntegerType array[] = {std::numeric_limits<IntegerType>::min(),
                               std::numeric_limits<IntegerType>::min() + 1,
                               std::numeric_limits<IntegerType>::max() / 2,
                               std::numeric_limits<IntegerType>::max() - 1,
                               std::numeric_limits<IntegerType>::max()};
  return std::vector<IntegerType>(array,
                                  array + sizeof(array) / sizeof(array[0]));
}

template <typename IntegerType>
struct Incrementor {
  IntegerType value;
  explicit Incrementor(const IntegerType value = 0u) : value(value) {}
  IntegerType operator()() {
    return ++value;
  }
};

class MultiFrameBuilderTest : public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    const std::vector<ConnectionID> connections = GetTestVector<ConnectionID>();
    const std::vector<SessionID> sessions = GetTestVector<SessionID>();
    const std::vector<MessageID> messages = GetTestVector<MessageID>();

    MutiframeData some_data;

    const uint8_t protocol_version = PROTOCOL_VERSION_2;
    const uint8_t service_type = SERVICE_TYPE_RPC;

    // We need 255+ messages for rolling over max uint8_t value
    int multi_frames_count = std::numeric_limits<uint8_t>::max() * 2;

    // Prepare C connections with S sessions with M messages data
    for (size_t c = 0; c < connections.size(); ++c) {
      const ConnectionID connection_id = connections[c];

      SessionToMutiframeDataTestMap sessions_map;
      for (size_t s = 0; s < sessions.size(); ++s) {
        const SessionID session_id = sessions[s];

        MessageIDToMutiframeDataTestMap messages_map;
        for (size_t m = 0; m < messages.size(); ++m) {
          const MessageID message_id = messages[m];

          UCharDataVector& data_vector = some_data.binary_data;
          // Sahll not be 1 consecutive frame
          ASSERT_GT(multi_frames_count, 1);
          data_vector.resize(++multi_frames_count * mtu_);

          std::generate(
              data_vector.begin(), data_vector.end(), Incrementor<uint8_t>(0u));

          PrepareMultiFrames(connection_id,
                             protocol_version,
                             service_type,
                             session_id,
                             message_id,
                             mtu_,
                             data_vector,
                             some_data.multiframes);
          messages_map.insert(std::make_pair(message_id, some_data));
        }
        sessions_map.insert(std::make_pair(session_id, messages_map));
      }
      test_data_map_.insert(std::make_pair(connection_id, sessions_map));
    }
  }

  void VerifyConsecutiveAdd(const MutiframeData& multiframe_data) {
    const ProtocolFramePtrList& multiframes = multiframe_data.multiframes;
    const UCharDataVector& binary_data = multiframe_data.binary_data;
    ASSERT_FALSE(multiframes.empty());

    // Frame of multiframe loop
    ProtocolFramePtrList::const_iterator it = multiframes.begin();
    // Skip last final frame
    const ProtocolFramePtrList::const_iterator it_last = --(multiframes.end());
    while (it != it_last) {
      const ProtocolFramePtr frame = *it;
      ASSERT_TRUE(frame);
      EXPECT_EQ(RESULT_OK, multiframe_builder_.AddFrame(frame))
          << "Non final CONSECUTIVE frame: " << frame;
      EXPECT_EQ(ProtocolFramePtrList(), multiframe_builder_.PopMultiframes())
          << "Non final CONSECUTIVE frame: " << frame;
      ++it;
      // Skip last final frame
    }

    const ProtocolFramePtr final_frame = multiframes.back();

    EXPECT_EQ(RESULT_OK, multiframe_builder_.AddFrame(final_frame))
        << "Final CONSECUTIVE frame: " << final_frame;

    const ProtocolFramePtrList& multiframe_list =
        multiframe_builder_.PopMultiframes();
    ASSERT_EQ(1u, multiframe_list.size());

    const ProtocolFramePtr result_multiframe = multiframe_list.front();
    EXPECT_EQ(binary_data,
              UCharDataVector(result_multiframe->data(),
                              result_multiframe->data() +
                                  result_multiframe->payload_size()));
  }

  // Support method for first and consecutive frame disassembling
  void PrepareMultiFrames(const ConnectionID connection_id,
                          const uint8_t protocol_version,
                          const uint8_t service_type,
                          const uint8_t session_id,
                          const uint32_t message_id,
                          const size_t max_payload_size,
                          const UCharDataVector& data,
                          ProtocolFramePtrList& out_frames) {
    ASSERT_LT(FIRST_FRAME_DATA_SIZE, max_payload_size);

    // TODO(EZamakhov): move to the separate class
    const size_t data_size = data.size();
    // remainder of last frame
    const size_t lastframe_remainder = data_size % max_payload_size;
    // size of last frame (full fill or not)
    const size_t lastframe_size =
        lastframe_remainder > 0 ? lastframe_remainder : max_payload_size;

    const size_t frames_count = data_size / max_payload_size +
                                // add last frame if not empty
                                (lastframe_remainder > 0 ? 1 : 0);

    uint8_t out_data[FIRST_FRAME_DATA_SIZE];
    out_data[0] = data_size >> 24;
    out_data[1] = data_size >> 16;
    out_data[2] = data_size >> 8;
    out_data[3] = data_size;

    out_data[4] = frames_count >> 24;
    out_data[5] = frames_count >> 16;
    out_data[6] = frames_count >> 8;
    out_data[7] = frames_count;

    ProtocolFramePtr first_frame(new ProtocolPacket(connection_id,
                                                    protocol_version,
                                                    PROTECTION_OFF,
                                                    FRAME_TYPE_FIRST,
                                                    service_type,
                                                    FRAME_DATA_FIRST,
                                                    session_id,
                                                    FIRST_FRAME_DATA_SIZE,
                                                    message_id,
                                                    out_data));
    // Note: PHIMpl already prepare First frames the total_data_bytes on
    // desirialization
    first_frame->set_total_data_bytes(data_size);

    out_frames.clear();
    out_frames.push_back(first_frame);

    for (size_t i = 0; i < frames_count; ++i) {
      const bool is_last_frame = ((frames_count - 1) == i);
      const size_t frame_size =
          is_last_frame ? lastframe_size : max_payload_size;
      const uint8_t data_type = is_last_frame
                                    ? FRAME_DATA_LAST_CONSECUTIVE
                                    : (i % FRAME_DATA_MAX_CONSECUTIVE + 1);

      const ProtocolFramePtr consecutive_frame(
          new ProtocolPacket(connection_id,
                             protocol_version,
                             PROTECTION_OFF,
                             FRAME_TYPE_CONSECUTIVE,
                             service_type,
                             data_type,
                             session_id,
                             frame_size,
                             message_id,
                             &data[max_payload_size * i]));
      out_frames.push_back(consecutive_frame);
    }
  }

  void AddConnections() {
    for (MultiFrameTestMap::iterator connection_it = test_data_map_.begin();
         connection_it != test_data_map_.end();
         ++connection_it) {
      const ConnectionID connection_id = connection_it->first;
      ASSERT_TRUE(multiframe_builder_.AddConnection(connection_id));
    }
  }

  MultiFrameBuilder multiframe_builder_;
  MultiFrameTestMap test_data_map_;
  static size_t mtu_;
};

size_t MultiFrameBuilderTest::mtu_ = 10;

TEST_F(MultiFrameBuilderTest, Pop_Frames_From_Empty_builder) {
  EXPECT_EQ(ProtocolFramePtrList(), multiframe_builder_.PopMultiframes());
}

TEST_F(MultiFrameBuilderTest, Pop_Frames_with_existing_connections) {
  AddConnections();
  EXPECT_EQ(ProtocolFramePtrList(), multiframe_builder_.PopMultiframes());
}

TEST_F(MultiFrameBuilderTest, Add_EmptyFrame) {
  EXPECT_EQ(RESULT_FAIL, multiframe_builder_.AddFrame(ProtocolFramePtr()));
  EXPECT_EQ(ProtocolFramePtrList(), multiframe_builder_.PopMultiframes());
}

TEST_F(MultiFrameBuilderTest, Add_NonSingleOrConsecutive_Frames) {
  UCharDataVector types;
  types.reserve(std::numeric_limits<uint8_t>::max());
  for (uint8_t type = std::numeric_limits<uint8_t>::min();
       type < std::numeric_limits<uint8_t>::max();
       ++type) {
    if (type != FRAME_TYPE_FIRST && type != FRAME_TYPE_CONSECUTIVE) {
      types.push_back(type);
    }
  }

  for (UCharDataVector::iterator it = types.begin(); it != types.end(); ++it) {
    const uint8_t frame_type = *it;
    const ProtocolFramePtr unexpected_frame(
        new ProtocolPacket(0u,
                           PROTOCOL_VERSION_3,
                           PROTECTION_OFF,
                           frame_type,
                           SERVICE_TYPE_RPC,
                           FRAME_DATA_FIRST,
                           0u,
                           0u,
                           0u));
    EXPECT_EQ(RESULT_FAIL, multiframe_builder_.AddFrame(unexpected_frame))
        << "Unexpected frame: " << unexpected_frame;

    EXPECT_EQ(ProtocolFramePtrList(), multiframe_builder_.PopMultiframes())
        << "Unexpected frame: " << unexpected_frame;
  }
}

TEST_F(MultiFrameBuilderTest, Add_FirstFrames_NoConnections) {
  for (MultiFrameTestMap::iterator connection_it = test_data_map_.begin();
       connection_it != test_data_map_.end();
       ++connection_it) {
    SessionToMutiframeDataTestMap& session_map = connection_it->second;
    const ConnectionID connection_id = connection_it->first;

    for (SessionToMutiframeDataTestMap::iterator session_it =
             session_map.begin();
         session_it != session_map.end();
         ++session_it) {
      MessageIDToMutiframeDataTestMap& messageId_map = session_it->second;

      for (MessageIDToMutiframeDataTestMap::iterator messageId_it =
               messageId_map.begin();
           messageId_it != messageId_map.end();
           ++messageId_it) {
        const MutiframeData& multiframe_data = messageId_it->second;

        const ProtocolFramePtrList& multiframes = multiframe_data.multiframes;
        ASSERT_FALSE(multiframes.empty());
        const ProtocolFramePtr first_frame = multiframes.front();
        ASSERT_TRUE(first_frame);
        EXPECT_EQ(RESULT_FAIL, multiframe_builder_.AddFrame(first_frame))
            << "Non-existed connection " << connection_id
            << "- to be skipped first frame: " << first_frame;

        EXPECT_EQ(ProtocolFramePtrList(), multiframe_builder_.PopMultiframes())
            << "First frame: " << first_frame;
      }
    }
  }
}

TEST_F(MultiFrameBuilderTest, Add_FirstFrames_only) {
  AddConnections();
  for (MultiFrameTestMap::iterator connection_it = test_data_map_.begin();
       connection_it != test_data_map_.end();
       ++connection_it) {
    SessionToMutiframeDataTestMap& session_map = connection_it->second;

    for (SessionToMutiframeDataTestMap::iterator session_it =
             session_map.begin();
         session_it != session_map.end();
         ++session_it) {
      MessageIDToMutiframeDataTestMap& messageId_map = session_it->second;

      for (MessageIDToMutiframeDataTestMap::iterator messageId_it =
               messageId_map.begin();
           messageId_it != messageId_map.end();
           ++messageId_it) {
        const MutiframeData& multiframe_data = messageId_it->second;

        const ProtocolFramePtrList& multiframes = multiframe_data.multiframes;
        ASSERT_FALSE(multiframes.empty());
        const ProtocolFramePtr first_frame = multiframes.front();
        ASSERT_TRUE(first_frame);
        EXPECT_EQ(RESULT_OK, multiframe_builder_.AddFrame(first_frame))
            << "First frame: " << first_frame;

        EXPECT_EQ(ProtocolFramePtrList(), multiframe_builder_.PopMultiframes())
            << "First frame: " << first_frame;
      }
    }
  }
}

TEST_F(MultiFrameBuilderTest, Add_ConsecutiveFrame) {
  ASSERT_FALSE(test_data_map_.empty());
  const ConnectionID& connection_id = test_data_map_.begin()->first;
  SessionToMutiframeDataTestMap& session_map = test_data_map_.begin()->second;

  ASSERT_TRUE(multiframe_builder_.AddConnection(connection_id));
  ASSERT_FALSE(session_map.empty());
  MessageIDToMutiframeDataTestMap& messageId_map = session_map.begin()->second;

  ASSERT_FALSE(messageId_map.empty());
  const MutiframeData& multiframe_data = messageId_map.begin()->second;

  VerifyConsecutiveAdd(multiframe_data);
}

TEST_F(MultiFrameBuilderTest, Add_ConsecutiveFrames_OneByOne) {
  AddConnections();
  for (MultiFrameTestMap::iterator connection_it = test_data_map_.begin();
       connection_it != test_data_map_.end();
       ++connection_it) {
    SessionToMutiframeDataTestMap& session_map = connection_it->second;

    for (SessionToMutiframeDataTestMap::iterator session_it =
             session_map.begin();
         session_it != session_map.end();
         ++session_it) {
      MessageIDToMutiframeDataTestMap& messageId_map = session_it->second;

      for (MessageIDToMutiframeDataTestMap::iterator messageId_it =
               messageId_map.begin();
           messageId_it != messageId_map.end();
           ++messageId_it) {
        const MutiframeData& multiframe_data = messageId_it->second;

        VerifyConsecutiveAdd(multiframe_data);
      }
    }
  }
}

TEST_F(MultiFrameBuilderTest, Add_ConsecutiveFrames_per1) {
  AddConnections();
  ASSERT_FALSE(test_data_map_.empty());
  // After processing each frame we remove it from messageId_it
  // After processing all session data - it removes from session_map
  // and so on
  // TODO(Ezamakhov): optimize speed of test by skipping erasing data
  while (!test_data_map_.empty()) {
    MultiFrameTestMap::iterator connection_it = test_data_map_.begin();
    while (connection_it != test_data_map_.end()) {
      SessionToMutiframeDataTestMap& session_map = connection_it->second;

      SessionToMutiframeDataTestMap::iterator session_it = session_map.begin();
      while (session_it != session_map.end()) {
        MessageIDToMutiframeDataTestMap& messageId_map = session_it->second;

        MessageIDToMutiframeDataTestMap::iterator messageId_it =
            messageId_map.begin();
        while (messageId_it != messageId_map.end()) {
          MutiframeData& multiframe_data = messageId_it->second;
          ProtocolFramePtrList& multiframes = multiframe_data.multiframes;
          ASSERT_FALSE(multiframes.empty());

          const ProtocolFramePtr frame = multiframes.front();
          ASSERT_TRUE(frame);

          EXPECT_EQ(RESULT_OK, multiframe_builder_.AddFrame(frame))
              << "Frame: " << frame;

          multiframes.pop_front();

          // If all frames are assembled
          if (multiframes.empty()) {
            const ProtocolFramePtrList& multiframe_list =
                multiframe_builder_.PopMultiframes();
            ASSERT_EQ(1u, multiframe_list.size());

            const ProtocolFramePtr result_multiframe = multiframe_list.front();
            const UCharDataVector& binary_data = multiframe_data.binary_data;
            EXPECT_EQ(binary_data,
                      UCharDataVector(result_multiframe->data(),
                                      result_multiframe->data() +
                                          result_multiframe->payload_size()));
            messageId_map.erase(messageId_it++);
          } else {
            // Multiframe is not completed
            EXPECT_EQ(ProtocolFramePtrList(),
                      multiframe_builder_.PopMultiframes())
                << "Frame: " << frame;
            ++messageId_it;
          }
        }
        if (messageId_map.empty()) {
          session_map.erase(session_it++);
        } else {
          ++session_it;
        }
      }
      if (session_map.empty()) {
        test_data_map_.erase(connection_it++);
      } else {
        ++connection_it;
      }
    }
  }
}

TEST_F(MultiFrameBuilderTest, FrameExpired_OneMSec) {
  multiframe_builder_.set_waiting_timeout(1);

  ASSERT_FALSE(test_data_map_.empty());
  const ConnectionID& connection_id = test_data_map_.begin()->first;
  SessionToMutiframeDataTestMap& session_map = test_data_map_.begin()->second;

  ASSERT_TRUE(multiframe_builder_.AddConnection(connection_id));

  ASSERT_FALSE(session_map.empty());
  MessageIDToMutiframeDataTestMap& messageId_map = session_map.begin()->second;

  ASSERT_FALSE(messageId_map.empty());
  const MutiframeData& multiframe_data = messageId_map.begin()->second;

  const ProtocolFramePtrList& multiframes = multiframe_data.multiframes;
  ASSERT_FALSE(multiframes.empty());
  const ProtocolFramePtr first_frame = multiframes.front();
  ASSERT_TRUE(first_frame);
  EXPECT_EQ(RESULT_OK, multiframe_builder_.AddFrame(first_frame))
      << "First frame: " << first_frame;

  // Wait frame expire
  usleep(1000);
  const ProtocolFramePtrList& list = multiframe_builder_.PopMultiframes();
  ASSERT_FALSE(list.empty());
  EXPECT_EQ(first_frame, list.front());
}

TEST_F(MultiFrameBuilderTest, RemoveConnection_NoConnection_ResultFail) {
  // Arrange
  const ConnectionID& connection_id = test_data_map_.begin()->first;
  // Act
  const bool connection_result =
      multiframe_builder_.RemoveConnection(connection_id);
  // Assert
  ASSERT_FALSE(connection_result);
}

TEST_F(MultiFrameBuilderTest, RemoveConnection_Successful) {
  // Arrange
  const ConnectionID& connection_id = test_data_map_.begin()->first;
  // Variable test_data_map initially contains IDs of not existed in
  // MultiframeBuilder connections
  ASSERT_TRUE(multiframe_builder_.AddConnection(connection_id));
  // Act
  const bool connection_result =
      multiframe_builder_.RemoveConnection(connection_id);
  // Assert
  ASSERT_TRUE(connection_result);
}

}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test
