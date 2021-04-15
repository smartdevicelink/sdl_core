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

#include "utils/date_time.h"
#include "utils/lock.h"

namespace protocol_handler {

SDL_CREATE_LOG_VARIABLE("ProtocolHandler")

MultiFrameBuilder::MultiFrameBuilder() : consecutive_frame_wait_msecs_(0u) {}

void MultiFrameBuilder::set_waiting_timeout(
    const uint32_t consecutive_frame_wait_msecs) {
  consecutive_frame_wait_msecs_ =
      static_cast<int64_t>(consecutive_frame_wait_msecs);
  if (consecutive_frame_wait_msecs == 0) {
    SDL_LOG_WARN("Waiting timout disabled");
  } else {
    SDL_LOG_DEBUG("Waiting time in msec: " << consecutive_frame_wait_msecs_);
  }
}

bool MultiFrameBuilder::AddConnection(const ConnectionID connection_id) {
  SDL_LOG_DEBUG("Adding connection_id: " << connection_id);
  sync_primitives::AutoLock lock(multiframes_map_lock_);
  SDL_LOG_DEBUG("Current state is: " << multiframes_map_);
  const MultiFrameMap::const_iterator it = multiframes_map_.find(connection_id);
  if (it != multiframes_map_.end()) {
    SDL_LOG_ERROR("Exists connection_id: " << connection_id);
    return false;
  }
  multiframes_map_[connection_id] = SessionToFrameMap();
  return true;
}

bool MultiFrameBuilder::RemoveConnection(const ConnectionID connection_id) {
  SDL_LOG_DEBUG("Removing connection_id: " << connection_id);
  sync_primitives::AutoLock lock(multiframes_map_lock_);
  SDL_LOG_DEBUG("Current state is: " << multiframes_map_);
  const MultiFrameMap::iterator it = multiframes_map_.find(connection_id);
  if (it == multiframes_map_.end()) {
    SDL_LOG_ERROR("Non-existent connection_id: " << connection_id);
    return false;
  }
  const SessionToFrameMap& session_to_frame_map = it->second;
  if (!session_to_frame_map.empty()) {
    // FIXME(EZamakhov): Ask ReqManager - do we need to send GenericError
    SDL_LOG_WARN("For connection_id: " << connection_id
                                       << " waiting: " << multiframes_map_);
  }
  multiframes_map_.erase(it);
  return true;
}

ProtocolFramePtrList MultiFrameBuilder::PopMultiframes() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(multiframes_map_lock_);
  SDL_LOG_TRACE("Current state is: " << multiframes_map_);
  SDL_LOG_TRACE("Current multiframe map size is: " << multiframes_map_.size());
  ProtocolFramePtrList outpute_frame_list;
  for (MultiFrameMap::iterator connection_it = multiframes_map_.begin();
       connection_it != multiframes_map_.end();
       ++connection_it) {
    SDL_LOG_TRACE("Step over connection: " << connection_it->first);
    SessionToFrameMap& session_map = connection_it->second;

    for (SessionToFrameMap::iterator session_it = session_map.begin();
         session_it != session_map.end();
         ++session_it) {
      SDL_LOG_TRACE(
          "Step over session: " << static_cast<int>(session_it->first));
      MessageIDToFrameMap& messageId_map = session_it->second;

      MessageIDToFrameMap::iterator messageId_it = messageId_map.begin();
      while (messageId_it != messageId_map.end()) {
        SDL_LOG_TRACE("Step over messageId: " << messageId_it->first);
        ProtocolFrameData& frame_data = messageId_it->second;
        ProtocolFramePtr frame = frame_data.frame;

        if (frame && frame->frame_data() == FRAME_DATA_LAST_CONSECUTIVE &&
            frame->payload_size() > 0u) {
          SDL_LOG_TRACE("Ready frame: " << frame);
          outpute_frame_list.push_back(frame);
          messageId_map.erase(messageId_it++);
          continue;
        }
        if (consecutive_frame_wait_msecs_ != 0) {
          SDL_LOG_TRACE("Expiration verification");
          const int64_t time_left =
              date_time::calculateTimeSpan(frame_data.append_time);
          SDL_LOG_DEBUG("mSecs left: " << time_left);
          if (time_left >= consecutive_frame_wait_msecs_) {
            SDL_LOG_WARN("Expired frame: " << frame);
            outpute_frame_list.push_back(frame);
            messageId_map.erase(messageId_it++);
            continue;
          }
        }
        ++messageId_it;
      }  // iteration over messageId_map
    }    // iteration over session_map
  }      // iteration over multiframes_map_
  SDL_LOG_TRACE("Result frames count: " << outpute_frame_list.size());
  return outpute_frame_list;
}

RESULT_CODE MultiFrameBuilder::AddFrame(const ProtocolFramePtr packet) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("Handling frame: " << packet);
  if (!packet) {
    SDL_LOG_ERROR("Skip empty frame");
    return RESULT_FAIL;
  }
  switch (packet->frame_type()) {
    case FRAME_TYPE_FIRST:
      SDL_LOG_TRACE("FRAME_TYPE_FIRST");
      return HandleFirstFrame(packet);
    case FRAME_TYPE_CONSECUTIVE:
      SDL_LOG_TRACE("FRAME_TYPE_CONSECUTIVE");
      return HandleConsecutiveFrame(packet);
    default:
      SDL_LOG_ERROR("Frame is not FIRST or CONSECUTIVE :" << packet);
      break;
  }
  return RESULT_FAIL;
}

RESULT_CODE MultiFrameBuilder::HandleFirstFrame(const ProtocolFramePtr packet) {
  DCHECK_OR_RETURN(packet->frame_type() == FRAME_TYPE_FIRST, RESULT_FAIL);
  sync_primitives::AutoLock lock(multiframes_map_lock_);
  SDL_LOG_TRACE("Waiting : " << multiframes_map_);
  SDL_LOG_TRACE("Handling FIRST frame: " << packet);
  if (packet->payload_size() != 0u) {
    SDL_LOG_ERROR("First frame shall have no data:" << packet);
    return RESULT_FAIL;
  }

  const ConnectionID connection_id = packet->connection_id();
  MultiFrameMap::iterator connection_it = multiframes_map_.find(connection_id);
  if (connection_it == multiframes_map_.end()) {
    SDL_LOG_ERROR("Unknown connection_id: " << connection_id);
    return RESULT_FAIL;
  }
  SessionToFrameMap& session_map = connection_it->second;

  const SessionID session_id = packet->session_id();
  // No need to verify session existance
  MessageIDToFrameMap& messageId_map = session_map[session_id];

  const MessageID message_id = packet->message_id();
  MessageIDToFrameMap::iterator messageId_it = messageId_map.find(message_id);
  if (messageId_it != messageId_map.end()) {
    SDL_LOG_ERROR("Already waiting message for connection_id: "
                  << connection_id
                  << ", session_id: " << static_cast<int>(session_id)
                  << ", message_id: " << message_id);
    return RESULT_FAIL;
  }

  SDL_LOG_TRACE("Start waiting frames for connection_id: "
                << connection_id
                << ", session_id: " << static_cast<int>(session_id)
                << ", message_id: " << message_id);
  messageId_map[message_id] = {packet, date_time::getCurrentTime()};
  return RESULT_OK;
}

RESULT_CODE MultiFrameBuilder::HandleConsecutiveFrame(
    const ProtocolFramePtr packet) {
  DCHECK_OR_RETURN(packet->frame_type() == FRAME_TYPE_CONSECUTIVE, RESULT_FAIL);
  sync_primitives::AutoLock lock(multiframes_map_lock_);
  SDL_LOG_DEBUG("Handling CONSECUTIVE frame: " << packet);

  const ConnectionID connection_id = packet->connection_id();
  MultiFrameMap::iterator connection_it = multiframes_map_.find(connection_id);
  if (connection_it == multiframes_map_.end()) {
    SDL_LOG_ERROR("Unknown connection_id: " << connection_id);
    return RESULT_FAIL;
  }
  SessionToFrameMap& session_map = connection_it->second;

  const SessionID session_id = packet->session_id();
  // No need to verify session existance
  MessageIDToFrameMap& messageId_map = session_map[session_id];

  const MessageID message_id = packet->message_id();
  MessageIDToFrameMap::iterator messageId_it = messageId_map.find(message_id);
  if (messageId_it == messageId_map.end()) {
    SDL_LOG_ERROR("No waiting message for connection_id: "
                  << connection_id
                  << ", session_id: " << static_cast<int>(session_id)
                  << ", message_id: " << message_id);
    return RESULT_FAIL;
  }

  ProtocolFrameData& frame_data = messageId_it->second;
  ProtocolFramePtr assembling_frame = frame_data.frame;
  DCHECK_OR_RETURN(packet->message_id() == assembling_frame->message_id(),
                   RESULT_FAIL);

  const uint8_t new_frame_data = packet->frame_data();
  const bool is_last_consecutive =
      (new_frame_data == FRAME_DATA_LAST_CONSECUTIVE);

  if (is_last_consecutive) {
    // TODO(EZamakhov): implement count of frames and result size verification
    SDL_LOG_TRACE("Last CONSECUTIVE frame");
  } else {
    uint8_t previous_frame_data = assembling_frame->frame_data();
    if (previous_frame_data == std::numeric_limits<uint8_t>::max()) {
      previous_frame_data = 0u;
    }
    // The next frame data is bigger at 1
    if (new_frame_data != (previous_frame_data + 1)) {
      SDL_LOG_ERROR("Unexpected CONSECUTIVE frame for connection_id: "
                    << connection_id
                    << ", session_id: " << static_cast<int>(session_id)
                    << ", message_id: " << message_id << ", frame: " << packet);
      return RESULT_FAIL;
    }
  }

  const bool packet_protection_flag = packet->protection_flag();
  if (!assembling_frame->protection_flag() && packet_protection_flag) {
    assembling_frame->set_protection_flag(packet_protection_flag);
  }

  assembling_frame->set_frame_data(new_frame_data);

  SDL_LOG_DEBUG("Appending " << packet->data_size() << " bytes "
                             << "; frame_data "
                             << static_cast<int>(new_frame_data)
                             << "; for connection_id: " << connection_id
                             << ", session_id: " << static_cast<int>(session_id)
                             << ", message_id: " << message_id);

  if (assembling_frame->appendData(packet->data(), packet->data_size()) !=
      RESULT_OK) {
    SDL_LOG_ERROR("Failed to append frame for multiframe message.");
    return RESULT_FAIL;
  }
  SDL_LOG_TRACE("Assembled frame with payload size: "
                << assembling_frame->payload_size());
  frame_data.append_time = date_time::getCurrentTime();
  return RESULT_OK;
}

}  // namespace protocol_handler
