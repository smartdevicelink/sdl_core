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
#include <utility>

#include "utils/logger.h"
#include "connection_handler/connection.h"

namespace connection_handler {

using namespace sync_primitives;

CREATE_LOGGERPTR_GLOBAL(logger_, "HeartBeatMonitor")

HeartBeatMonitor::HeartBeatMonitor(uint32_t heartbeat_timeout_mseconds,
                                   Connection *connection)
    : default_heartbeat_timeout_(heartbeat_timeout_mseconds),
      connection_(connection),
      sessions_list_lock_(true),
      run_(true) {
}

void HeartBeatMonitor::Process() {
  sessions_list_lock_.Acquire();
  SessionMap::iterator it = sessions_.begin();
  while (it != sessions_.end()) {
    SessionState &state = it->second;
    if (state.HasTimeoutElapsed()) {
      const uint8_t session_id = it->first;
      if (state.IsReadyToClose()) {
        LOG4CXX_WARN(logger_, "Will close session");
        sessions_list_lock_.Release();
        RemoveSession(session_id);
        connection_->CloseSession(session_id);
        sessions_list_lock_.Acquire();
        it = sessions_.begin();
        continue;
      } else {
        LOG4CXX_DEBUG(logger_,
          "Send heart beat into session with id " << static_cast<int32_t>(session_id));
        state.PrepareToClose();
        connection_->SendHeartBeat(it->first);
      }
    }
    ++it;
  }
  sessions_list_lock_.Release();
}

void HeartBeatMonitor::threadMain() {
  AutoLock main_lock(main_thread_lock_);
  LOG4CXX_DEBUG(
      logger_,
      "Start heart beat monitor. Timeout is " << default_heartbeat_timeout_);
  while (run_) {
    heartbeat_monitor_.WaitFor(main_lock, kDefaultCycleTimeout);
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
  sessions_.insert(std::make_pair(session_id, SessionState(default_heartbeat_timeout_)));
  LOG4CXX_INFO(logger_, "Start heartbeat for session " << session_id);
}

void HeartBeatMonitor::RemoveSession(uint8_t session_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  AutoLock auto_lock(sessions_list_lock_);

  LOG4CXX_DEBUG(logger_,
               "Remove session with id " << static_cast<int>(session_id));

  if (sessions_.erase(session_id) == 0) {
    LOG4CXX_WARN(logger_,
                 "Remove session with id " << static_cast<int>(session_id) <<
                 " was unsuccessful");
  }
}

void HeartBeatMonitor::KeepAlive(uint8_t session_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  AutoLock auto_lock(sessions_list_lock_);

  if (sessions_.end() != sessions_.find(session_id)) {
    LOG4CXX_INFO( logger_, "Resetting heart beat timer for session with id " <<
                  static_cast<int32_t>(session_id));

    sessions_[session_id].KeepAlive();
  }
}

void HeartBeatMonitor::exitThreadMain() {
  // FIXME (dchmerev@luxoft.com): thread requested to stop should stop as soon as possible,
  // not running one more iteration before actual stop
  LOG4CXX_AUTO_TRACE(logger_);
  AutoLock main_lock(main_thread_lock_);
  run_ = false;
  heartbeat_monitor_.NotifyOne();
}

void HeartBeatMonitor::set_heartbeat_timeout_milliseconds(uint32_t timeout,
                                                     uint8_t session_id) {
  LOG4CXX_DEBUG(logger_, "Set new heart beat timeout " << timeout <<
                "For session: " << session_id);

  AutoLock session_locker(sessions_list_lock_);
  if (sessions_.end() != sessions_.find(session_id)) {
    sessions_[session_id].UpdateTimeout(timeout);
  }
}

HeartBeatMonitor::SessionState::SessionState(uint32_t heartbeat_timeout_mseconds)
  : heartbeat_timeout_mseconds_(heartbeat_timeout_mseconds),
    is_heartbeat_sent_(false) {
  LOG4CXX_AUTO_TRACE(logger_);
  RefreshExpiration();
}

void HeartBeatMonitor::SessionState::RefreshExpiration() {
  LOG4CXX_DEBUG(logger_, "Refresh expiration: " << heartbeat_timeout_mseconds_);
  using namespace date_time;
  TimevalStruct time = DateTime::getCurrentTime();
  DateTime::AddMilliseconds(time, heartbeat_timeout_mseconds_);
  heartbeat_expiration_ = time;

}

void HeartBeatMonitor::SessionState::UpdateTimeout(
    uint32_t heartbeat_timeout_mseconds) {
  LOG4CXX_DEBUG(logger_, "Update timout with value " <<
                heartbeat_timeout_mseconds_);
  heartbeat_timeout_mseconds_ = heartbeat_timeout_mseconds;
  RefreshExpiration();
}

void HeartBeatMonitor::SessionState::PrepareToClose() {
  is_heartbeat_sent_ = true;
  LOG4CXX_DEBUG(logger_, "Prepare to close");
  RefreshExpiration();
}

bool HeartBeatMonitor::SessionState::IsReadyToClose() const {
  return is_heartbeat_sent_;
}

void HeartBeatMonitor::SessionState::KeepAlive() {
  LOG4CXX_AUTO_TRACE(logger_);
  is_heartbeat_sent_ = false;
  RefreshExpiration();
}

bool HeartBeatMonitor::SessionState::HasTimeoutElapsed() {
  TimevalStruct now = date_time::DateTime::getCurrentTime();
  return date_time::DateTime::Greater(now, heartbeat_expiration_);
}

}  // namespace connection_handler
