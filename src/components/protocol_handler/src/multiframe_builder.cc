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

#include "protocol_handler/multiframe_builder.h"

#include <limits>

#include "utils/logger.h"
#include "utils/make_shared.h"
#include "utils/lock.h"
#include "utils/date_time.h"

namespace protocol_handler {

CREATE_LOGGERPTR_GLOBAL(logger_, "ProtocolHandler")

MultiFrameBuilder::MultiFrameBuilder()
  : consecutive_frame_wait_msecs_(0u) {
}

void MultiFrameBuilder::set_waiting_timeout(const uint32_t consecutive_frame_wait_msecs) {
  consecutive_frame_wait_msecs_ = static_cast<int64_t>(consecutive_frame_wait_msecs);
  if (consecutive_frame_wait_msecs == 0) {
    LOG4CXX_WARN(logger_, "Waiting timout disabled");
  } else {
    LOG4CXX_DEBUG(logger_, "Waiting time in msec: " << consecutive_frame_wait_msecs_);
  }
}

bool MultiFrameBuilder::AddConnection(const ConnectionID connection_id) {
  LOG4CXX_DEBUG(logger_, "Adding connection_id: " << connection_id);
  LOG4CXX_DEBUG(logger_, "Current state is: " << multiframes_map_);
  const MultiFrameMap::const_iterator it = multiframes_map_.find(connection_id);
  if (it != multiframes_map_.end()) {
    LOG4CXX_ERROR(logger_, "Exists connection_id: " << connection_id);
    return false;
  }
  multiframes_map_[connection_id] = SessionToFrameMap();
  return true;
}

bool MultiFrameBuilder::RemoveConnection(const ConnectionID connection_id) {
  LOG4CXX_DEBUG(logger_, "Removing connection_id: " << connection_id);
  LOG4CXX_DEBUG(logger_, "Current state is: " << multiframes_map_);
  const MultiFrameMap::iterator it = multiframes_map_.find(connection_id);
  if (it == multiframes_map_.end()) {
    LOG4CXX_ERROR(logger_, "Non-existent connection_id: " << connection_id);
    return false;
  }
  const SessionToFrameMap& session_to_frame_map = it->second;
  if (!session_to_frame_map.empty()) {
    // FIXME(EZamakhov): Ask ReqManager - do we need to send GenericError
    LOG4CXX_WARN(logger_, "For connection_id: " << connection_id
                 << " waiting: " << multiframes_map_);
  }
  multiframes_map_.erase(it);
  return true;
}

ProtocolFramePtrList MultiFrameBuilder::PopMultiframes() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Current state is: " << multiframes_map_);
  ProtocolFramePtrList outpute_frame_list;
  for (MultiFrameMap::iterator connection_it = multiframes_map_.begin();
       connection_it != multiframes_map_.end(); ++connection_it) {
    LOG4CXX_TRACE(logger_, "Step over connection: " << connection_it->first);
    SessionToFrameMap& session_map = connection_it->second;

    for (SessionToFrameMap::iterator session_it = session_map.begin();
         session_it != session_map.end(); ++session_it) {
      LOG4CXX_TRACE(logger_, "Step over session: "
                    << static_cast<int>(session_it->first));
      MessageIDToFrameMap& messageId_map = session_it->second;

      MessageIDToFrameMap::iterator messageId_it = messageId_map.begin();
      while (messageId_it != messageId_map.end()) {
        LOG4CXX_TRACE(logger_, "Step over messageId: " << messageId_it->first);
        ProtocolFrameData& frame_data = messageId_it->second;
        ProtocolFramePtr frame = frame_data.frame;

        if (frame &&
            frame->frame_data() == FRAME_DATA_LAST_CONSECUTIVE &&
            frame->payload_size() > 0u ) {
          LOG4CXX_DEBUG(logger_, "Ready frame: " << frame);
          outpute_frame_list.push_back(frame);
          messageId_map.erase(messageId_it++);
          continue;
        }
        if (consecutive_frame_wait_msecs_ != 0) {
          LOG4CXX_TRACE(logger_, "Expiration verification");
          const int64_t time_left =
            date_time::DateTime::calculateTimeSpan(frame_data.append_time);
          LOG4CXX_DEBUG(logger_, "mSecs left: " << time_left);
          if (time_left >= consecutive_frame_wait_msecs_) {
            LOG4CXX_WARN(logger_, "Expired frame: " << frame);
            outpute_frame_list.push_back(frame);
            messageId_map.erase(messageId_it++);
            continue;
          }
        }
        ++messageId_it;
      }  // iteration over messageId_map
    }  // iteration over session_map
  }    // iteration over multiframes_map_
  LOG4CXX_DEBUG(logger_, "Result frames count: " << outpute_frame_list.size());
  return outpute_frame_list;
}

RESULT_CODE MultiFrameBuilder::AddFrame(const ProtocolFramePtr packet) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Handling frame: " << packet);
  LOG4CXX_DEBUG(logger_, "Current state is: " << multiframes_map_);
  if (!packet) {
    LOG4CXX_ERROR(logger_, "Skip empty frame");
    return RESULT_FAIL;
  }
  switch (packet->frame_type()) {
    case FRAME_TYPE_FIRST:
      LOG4CXX_TRACE(logger_, "FRAME_TYPE_FIRST");
      return HandleFirstFrame(packet);
    case FRAME_TYPE_CONSECUTIVE:
      LOG4CXX_TRACE(logger_, "FRAME_TYPE_CONSECUTIVE");
      return HandleConsecutiveFrame(packet);
    default:
      LOG4CXX_ERROR(logger_, "Frame is not FIRST or CONSECUTIVE :"
                    << packet);
      break;
  }
  return RESULT_FAIL;
}

RESULT_CODE MultiFrameBuilder::HandleFirstFrame(const ProtocolFramePtr packet) {
  DCHECK_OR_RETURN(packet->frame_type() == FRAME_TYPE_FIRST,
                   RESULT_FAIL);
  LOG4CXX_DEBUG(logger_, "Waiting : " << multiframes_map_);
  LOG4CXX_DEBUG(logger_, "Handling FIRST frame: " << packet);
  if (packet->payload_size() != 0u) {
    LOG4CXX_ERROR(logger_,
                  "First frame shall have no data:" << packet);
    return RESULT_FAIL;
  }

  const ConnectionID connection_id = packet->connection_id();
  MultiFrameMap::iterator connection_it = multiframes_map_.find(connection_id);
  if (connection_it == multiframes_map_.end()) {
    LOG4CXX_ERROR(logger_, "Unknown connection_id: " << connection_id);
    return RESULT_FAIL;
  }
  SessionToFrameMap& session_map = connection_it->second;

  const SessionID session_id = packet->session_id();
  // No need to verify session existance
  MessageIDToFrameMap& messageId_map = session_map[session_id];

  const MessageID message_id = packet->message_id();
  MessageIDToFrameMap::iterator messageId_it = messageId_map.find(message_id);
  if (messageId_it != messageId_map.end()) {
    LOG4CXX_ERROR(logger_, "Already waiting message for connection_id: " << connection_id
                  << ", session_id: " << static_cast<int>(session_id)
                  << ", message_id: " << message_id);
    return RESULT_FAIL;
  }

  LOG4CXX_DEBUG(logger_, "Start waiting frames for connection_id: " << connection_id
                << ", session_id: " << static_cast<int>(session_id)
                << ", message_id: " << message_id);
  messageId_map[message_id] = {packet, date_time::DateTime::getCurrentTime()};
  return RESULT_OK;
}

RESULT_CODE MultiFrameBuilder::HandleConsecutiveFrame(const ProtocolFramePtr packet) {
  DCHECK_OR_RETURN(packet->frame_type() == FRAME_TYPE_CONSECUTIVE,
                   RESULT_FAIL);
  LOG4CXX_DEBUG(logger_, "Handling CONSECUTIVE frame: " << packet);


  const ConnectionID connection_id = packet->connection_id();
  MultiFrameMap::iterator connection_it = multiframes_map_.find(connection_id);
  if (connection_it == multiframes_map_.end()) {
    LOG4CXX_ERROR(logger_, "Unknown connection_id: " << connection_id);
    return RESULT_FAIL;
  }
  SessionToFrameMap& session_map = connection_it->second;

  const SessionID session_id = packet->session_id();
  // No need to verify session existance
  MessageIDToFrameMap& messageId_map = session_map[session_id];

  const MessageID message_id = packet->message_id();
  MessageIDToFrameMap::iterator messageId_it = messageId_map.find(message_id);
  if (messageId_it == messageId_map.end()) {
    LOG4CXX_ERROR(logger_, "No waiting message for connection_id: " << connection_id
                  << ", session_id: " << static_cast<int>(session_id)
                  << ", message_id: " << message_id);
    return RESULT_FAIL;
  }

  ProtocolFrameData& frame_data = messageId_it->second;
  ProtocolFramePtr assembling_frame = frame_data.frame;
  DCHECK_OR_RETURN(packet->message_id() == assembling_frame->message_id(),
                   RESULT_FAIL);

  const uint8_t new_frame_data = packet->frame_data();
  const bool is_last_consecutive = (new_frame_data ==
                                    FRAME_DATA_LAST_CONSECUTIVE);

  if (is_last_consecutive) {
    // TODO(EZamakhov): implement count of frames and result size verification
    LOG4CXX_DEBUG(logger_, "Last CONSECUTIVE frame");
  } else {
    uint8_t previous_frame_data = assembling_frame->frame_data();
    if (previous_frame_data == std::numeric_limits<uint8_t>::max()) {
      previous_frame_data = 0u;
    }
    // The next frame data is bigger at 1
    if (new_frame_data != (previous_frame_data + 1)) {
      LOG4CXX_ERROR(logger_,
                    "Unexpected CONSECUTIVE frame for connection_id: " << connection_id
                    << ", session_id: " << static_cast<int>(session_id)
                    << ", message_id: " << message_id
                    << ", frame: " << packet);
      return RESULT_FAIL;
    }
  }

  assembling_frame->set_frame_data(new_frame_data);

  LOG4CXX_DEBUG(logger_,
                "Appending " << packet->data_size() << " bytes "
                << "; frame_data " << static_cast<int>(new_frame_data)
                << "; for connection_id: " << connection_id
                << ", session_id: " << static_cast<int>(session_id)
                << ", message_id: " << message_id);

  if (assembling_frame->appendData(packet->data(),
                                   packet->data_size()) != RESULT_OK) {
    LOG4CXX_ERROR(logger_, "Failed to append frame for multiframe message.");
    return RESULT_FAIL;
  }
  LOG4CXX_INFO(logger_, "Assembled frame with payload size: " << assembling_frame->payload_size());
  frame_data.append_time = date_time::DateTime::getCurrentTime();
  return RESULT_OK;
}

}  // namespace protocol_handler
