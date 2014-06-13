/**
 * Copyright (c) 2013, Ford Motor Company
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
#include "connection_handler/heartbeat_monitor.h"
#include <unistd.h>
#include "connection_handler/connection.h"
#include "utils/logger.h"

namespace connection_handler {

using namespace sync_primitives;

CREATE_LOGGERPTR_GLOBAL(logger_, "HeartBeatMonitor")

HeartBeatMonitor::HeartBeatMonitor(int32_t heartbeat_timeout_seconds,
                                   Connection* connection)
    : heartbeat_timeout_seconds_(heartbeat_timeout_seconds),
      connection_(connection),
      stop_flag_(false) {
}

HeartBeatMonitor::~HeartBeatMonitor() {
  LOG4CXX_TRACE_ENTER(logger_);
}


void HeartBeatMonitor::threadMain() {
   std::map<uint8_t, SessionState>::iterator it;

  while (!stop_flag_) {
    usleep(kdefault_cycle_timeout);

    AutoLock auto_lock(sessions_list_lock_);

    it = sessions_.begin();
    while (it != sessions_.end()) {
      SessionState& state = it->second;
      if (state.heartbeat_expiration_.tv_sec < date_time::DateTime::getCurrentTime().tv_sec) {
        if (state.is_heartbeat_sent_) {
          LOG4CXX_WARN(logger_, "Session with id " << static_cast<int32_t>(it->first) <<" timed out, closing");
          const uint8_t session_id = it->first;

          // Unlock sessions_list_lock_ to avoid deadlock during session and session heartbeat removing
          {
            AutoUnlock auto_unlock(auto_lock);
            connection_->CloseSession(session_id);
          }

          it = sessions_.begin();
          if (sessions_.empty()) {
            stop_flag_ = true;
          }
        } else {
          state.heartbeat_expiration_ =
              date_time::DateTime::getCurrentTime();
          state.heartbeat_expiration_.tv_sec +=
              heartbeat_timeout_seconds_;

          connection_->SendHeartBeat(it->first);

          state.is_heartbeat_sent_ = true;
          ++it;
        }
      } else {
        ++it;
      }
    }
  }
}

bool HeartBeatMonitor::AddSession(uint8_t session_id) {
  LOG4CXX_INFO(logger_, "Add session with id " <<
               static_cast<int32_t>(session_id));

  AutoLock auto_lock(sessions_list_lock_);

  if (sessions_.end() != sessions_.find(session_id)) {
    return false;
  }

  SessionState session_state;
  session_state.heartbeat_expiration_ = date_time::DateTime::getCurrentTime();
  session_state.heartbeat_expiration_.tv_sec +=  heartbeat_timeout_seconds_;
  session_state.is_heartbeat_sent_ = false;

  sessions_[session_id] = session_state;

  LOG4CXX_INFO(logger_, "Start heartbeat for session: " <<
               static_cast<int32_t>(session_id));

  //first session added, so we need to start monitoring thread
  if (1 == sessions_.size()) {
    return true;
  }

  return false;
}

void HeartBeatMonitor::RemoveSession(uint8_t session_id) {
  AutoLock auto_lock(sessions_list_lock_);

  if (sessions_.end() != sessions_.find(session_id)) {
    LOG4CXX_INFO(logger_, "Remove session with id" <<
                 static_cast<int32_t>(session_id));
    sessions_.erase(session_id);
  }
}

void HeartBeatMonitor::KeepAlive(uint8_t session_id) {
  AutoLock auto_lock(sessions_list_lock_);

  if (sessions_.end() != sessions_.find(session_id)) {
    LOG4CXX_INFO(logger_, "Resetting heart beat timer for session with id"
                 << static_cast<int32_t>(session_id));

    sessions_[session_id].heartbeat_expiration_ =
        date_time::DateTime::getCurrentTime();
    sessions_[session_id].heartbeat_expiration_.tv_sec +=
        heartbeat_timeout_seconds_;
    sessions_[session_id].is_heartbeat_sent_ = false;
  }
}

bool HeartBeatMonitor::exitThreadMain() {
  stop_flag_ = true;
  LOG4CXX_INFO(logger_, "exitThreadMain");
  return true;
}

} // namespace connection_handler
