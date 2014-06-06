/* Copyright (c) 2013, Ford Motor Company
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
#ifndef SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_HEARTBEAT_MONITOR_H_
#define SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_HEARTBEAT_MONITOR_H_

#include <stdint.h>
#include <map>

#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "utils/date_time.h"
#include "utils/macro.h"
#include "utils/lock.h"

namespace connection_handler {

class Connection;

/*
 * Starts hearbeat timer for session and when it elapses closes it
 */
class HeartBeatMonitor: public threads::ThreadDelegate {
 public:
  HeartBeatMonitor(int32_t heartbeat_timeout_seconds,
                   Connection* connection);
  ~HeartBeatMonitor();

  /**
   * Thread procedure.
   */
  virtual void threadMain();

  /**
    * \brief add new session
    *
    * \return true if first session with heartbeat added
    */
  bool AddSession(uint8_t session_id);
  void RemoveSession(uint8_t session_id);

  /*
   * Resets timer preventing session from being killed
   */
  void KeepAlive(uint8_t session_id);

  /*
   * Thread exit procedure.
   */
  virtual bool exitThreadMain();

 private:
  // \brief Heartbeat timeout, should be read from profile
  const int32_t heartbeat_timeout_seconds_;
  // \brief Connection that must be closed when timeout elapsed
  Connection* connection_;

  static const int32_t kdefault_cycle_timeout = 1000000;

  struct SessionState {
    TimevalStruct heartbeat_expiration_;
    bool is_heartbeat_sent_;
  };

  // \brief monitored sessions collection
  std::map<uint8_t, SessionState> sessions_;

  sync_primitives::Lock sessions_list_lock_;

  volatile bool stop_flag_;

  DISALLOW_COPY_AND_ASSIGN(HeartBeatMonitor);
};

} // namespace connection_handler

#endif  // SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_HEARTBEAT_MONITOR_H_
