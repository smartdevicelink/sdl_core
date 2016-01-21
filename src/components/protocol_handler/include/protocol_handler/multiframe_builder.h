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

#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_MULTIFRAME_BUILDER_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_MULTIFRAME_BUILDER_H_

#include <map>
#include <ostream>      // std::basic_ostream
#include <iterator>     // std::ostream_iterator
#include <algorithm>    // std::copy

#include "utils/date_time.h"
#include "protocol_handler/protocol_packet.h"

/**
 *\namespace protocol_handlerHandler
 *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
 */
namespace protocol_handler {
/**
 * \brief Session identifier - contains connection identifier and
 * session_id from protocol  (can be used as hash)
 */
// TODO(EZamakhov): move SessionID to protocol_handler/protocol_packet.h
typedef uint8_t SessionID;
/**
 * \brief Message identifier - unique to the session messages
 */
// TODO(EZamakhov): move MessageID to protocol_handler/session_observer.h
typedef uint32_t MessageID;

struct ProtocolFrameData {
  ProtocolFramePtr frame;
  TimevalStruct append_time;
};
/**
 *\brief Map of frames with last frame data for messages received in multiple frames.
 */
typedef std::map<MessageID, ProtocolFrameData> MessageIDToFrameMap;
/**
 *\brief Map of frames with last frame data for messages received in multiple frames.
 */
typedef std::map<SessionID, MessageIDToFrameMap> SessionToFrameMap;
/**
 *\brief Map of frames with last frame data for messages received in multiple frames.
 */
typedef std::map<ConnectionID, SessionToFrameMap> MultiFrameMap;

/**
 * \class MultiFrameBuilder
 * \brief Class for assembling consecutive frames according to
 * messageID to complete multiframes.
 */
class MultiFrameBuilder {
 public:
  /**
  * @brief Constructor
  */
  MultiFrameBuilder();

  /**
  *\brief Set timeout of waiting CONSECUTIVE frames
  */
  void set_waiting_timeout(const uint32_t consecutive_frame_wait_msecs);

  /**
  * @brief Add connection for pending data
  * @return true on success
  */
  bool AddConnection(const ConnectionID connection_id);

  /**
  * @brief Clear all data related to connection_id
  * @return true on success
  */
  bool RemoveConnection(const ConnectionID connection_id);

  /**
  *\brief Pop assembled and expired frames
  */
  ProtocolFramePtrList PopMultiframes();

  /**
  *\brief Handle Single or Consecutive frame
  * @return RESULT_OK on success, or RESULT_FAIL in case of any error
  */
  RESULT_CODE AddFrame(const ProtocolFramePtr packet);

 private:
  RESULT_CODE HandleFirstFrame(const ProtocolFramePtr packet);
  RESULT_CODE HandleConsecutiveFrame(const ProtocolFramePtr packet);

  //  Map of frames with last frame data for messages received in multiple frames.
  MultiFrameMap multiframes_map_;
  int64_t consecutive_frame_wait_msecs_;
};

template<typename _CharT>
std::basic_ostream<_CharT>& operator<<(std::basic_ostream<_CharT>& stream,
                                       const protocol_handler::MultiFrameMap& map) {
  if (map.empty()) {
    stream << "{empty}";
    return stream;
  }
  for (MultiFrameMap::const_iterator connection_it = map.begin();
       connection_it != map.end(); ++connection_it) {
    const SessionToFrameMap& session_map = connection_it->second;

    for (SessionToFrameMap::const_iterator session_it = session_map.begin();
         session_it != session_map.end(); ++session_it) {
      const MessageIDToFrameMap& messageId_map = session_it->second;

      for (MessageIDToFrameMap::const_iterator messageId_it = messageId_map.begin();
           messageId_it != messageId_map.end(); ++messageId_it) {
        const ProtocolFrameData& frame_data = messageId_it->second;

        stream << "ConnectionID: " << connection_it->first
               << ", SessionID: "  << static_cast<uint32_t>(session_it->first)
               << ", MessageID: "  << static_cast<uint32_t>(messageId_it->first)
               << " msec, frame: " << frame_data.frame << std::endl;
      }
    }
  }
  return stream;
}

}  // namespace protocol_handler
#endif  // SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_MULTIFRAME_BUILDER_H_
