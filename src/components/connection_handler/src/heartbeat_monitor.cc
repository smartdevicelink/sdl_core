/*
 * Copyright (c) 2013-2014, Ford Motor Company
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

#include "utils/logger.h"
#include "connection_handler/connection.h"

namespace connection_handler {

using namespace sync_primitives;

CREATE_LOGGERPTR_GLOBAL(logger_, "HeartBeatMonitor")

HeartBeatMonitor::HeartBeatMonitor(int32_t heartbeat_timeout_seconds,
                                   Connection *connection)
    : heartbeat_timeout_seconds_(heartbeat_timeout_seconds),
      connection_(connection),
      sessions_list_lock_(true),
      run_(true) {
}

bool HeartBeatMonitor::HasTimeoutElapsed(const TimevalStruct& expiration) const {
  TimevalStruct now = date_time::DateTime::getCurrentTime();
  return date_time::DateTime::Greater(now, expiration);
}

void HeartBeatMonitor::Process() {
  AutoLock auto_lock(sessions_list_lock_);

  SessionMap::iterator it = sessions_.begin();
  while (it != sessions_.end()) {
    SessionState &state = it->second;
    if (HasTimeoutElapsed(state.heartbeat_expiration)) {
      const uint8_t session_id = it->first;
      if (state.is_heartbeat_sent) {
        LOG4CXX_DEBUG(logger_,
          "Session with id " << static_cast<int32_t>(session_id) << " timed out, closing");
        connection_->CloseSession(session_id);
        it = sessions_.begin();
        continue;
      } else {
        LOG4CXX_DEBUG(logger_,
          "Send heart beat into session with id " << static_cast<int32_t>(session_id));
        RefreshExpiration(&state.heartbeat_expiration);
        connection_->SendHeartBeat(it->first);
        state.is_heartbeat_sent = true;
      }
    }

    ++it;
  }
}

void HeartBeatMonitor::RefreshExpiration(TimevalStruct* expiration) const {
  LOG4CXX_TRACE_ENTER(logger_);
  sync_primitives::AutoLock locker(heartbeat_timeout_seconds_lock_);
  DCHECK(expiration);
  *expiration = date_time::DateTime::getCurrentTime();
  expiration->tv_sec += heartbeat_timeout_seconds_;
  LOG4CXX_TRACE_EXIT(logger_);
}

void HeartBeatMonitor::threadMain() {
  AutoLock main_lock(main_thread_lock_);
  LOG4CXX_DEBUG(
      logger_,
      "Start heart beat monitor. Timeout is " << heartbeat_timeout_seconds_);
  while (run_) {
    usleep(kDefaultCycleTimeout);
    Process();
  }
}

void HeartBeatMonitor::AddSession(uint8_t session_id) {
  LOG4CXX_DEBUG(logger_, "Add session with id " << static_cast<int32_t>(session_id));
  AutoLock auto_lock(sessions_list_lock_);
  if (sessions_.end() != sessions_.find(session_id)) {
    LOG4CXX_WARN(
        logger_,
        "Session with id " << static_cast<int32_t>(session_id) << " already exists");
    return;
  }
  SessionState session_state;
  RefreshExpiration(&session_state.heartbeat_expiration);
  session_state.is_heartbeat_sent = false;
  sessions_[session_id] = session_state;

  LOG4CXX_INFO(
      logger_,
      "Start heartbeat for session " << static_cast<int32_t>(session_id));
}

void HeartBeatMonitor::RemoveSession(uint8_t session_id) {
  AutoLock auto_lock(sessions_list_lock_);

  if (sessions_.end() != sessions_.find(session_id)) {
    LOG4CXX_INFO(logger_,
                 "Remove session with id " << static_cast<int32_t>(session_id));
    sessions_.erase(session_id);
  }
}

void HeartBeatMonitor::KeepAlive(uint8_t session_id) {
  AutoLock auto_lock(sessions_list_lock_);

  if (sessions_.end() != sessions_.find(session_id)) {
    LOG4CXX_INFO(
        logger_,
        "Resetting heart beat timer for session with id " << static_cast<int32_t>(session_id));

    RefreshExpiration(&sessions_[session_id].heartbeat_expiration);
    sessions_[session_id].is_heartbeat_sent = false;
  }
}

bool HeartBeatMonitor::exitThreadMain() {
  LOG4CXX_TRACE_ENTER(logger_);
  run_ = false;
  AutoLock main_lock(main_thread_lock_);
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

void HeartBeatMonitor::set_heartbeat_timeout_seconds(int32_t timeout) {
  LOG4CXX_DEBUG(logger_, "Set new heart beat timeout " << timeout);
  {
    AutoLock locker(heartbeat_timeout_seconds_lock_);
    heartbeat_timeout_seconds_ = timeout;
  }

  AutoLock session_locker(sessions_list_lock_);
  for (SessionMap::iterator i = sessions_.begin(); i != sessions_.end(); ++i) {
    SessionState& session_state = i->second;
    RefreshExpiration(&session_state.heartbeat_expiration);
  }
}

}  // namespace connection_handler
