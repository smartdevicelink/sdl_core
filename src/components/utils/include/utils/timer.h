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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_TIMER_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_TIMER_H_

#include <stdint.h>

#ifdef __QNX__
#include <sys/siginfo.h>
#else
#include <signal.h>
typedef sigval_t sigval;
#endif

#include <time.h>
#include <string>

#include "utils/lock.h"
#include "utils/timer_task.h"

namespace timer {
typedef uint32_t Milliseconds;
/**
 * @brief The Timer is class for calling any method after out of internal time.
 * Time setups in ::Start(uint,bool) method and starts time out steps.
 * User can call Start, Stop, IsRunning, GetTimeout.
 */
class Timer {
 public:
  /**
   * @brief constructor
   * @param name for indentify of current timer
   * @param task_for_tracking is SPtr to trackable task
   */
  Timer(const std::string& name, const TimerTask* task_for_tracking);

  /**
   * @brief destructor - if timer running : call stop in the body
   */
  ~Timer();
  /**
   * @brief starts timer with new timeout
   * @param timeout - time to call method from trackable class
   * @param repeatable - should timer repeat after calling callback, or no
   */
  void Start(const Milliseconds timeout, const bool repeatable);
  /**
   * @brief stops timer without calling callback
   */
  void Stop();
  /**
   * @brief method which know about Timer state: is running or not
   * @return true when timer runned, false when timer stand
   */
  bool IsRunning() const;
  /**
   * @brief GetTimeout
   * @return returns timeout
   */
  Milliseconds GetTimeout() const;

 private:
  const std::string name_;
  const TimerTask* task_;
  bool repeatable_;
  uint32_t timeout_ms_;
  bool is_running_;
  timer_t timer_;
  mutable sync_primitives::Lock lock_;
  sync_primitives::Lock task_lock_;

  /**
   * @brief method called from friend handler_wrapper and call run() from task.
   */
  void OnTimeout();

  /**
   * @brief method for setting correct timeout.
   * @param timeout - if it`s value = 0, timeout will be setted to 1
   * There would be no way to stop thread if timeout in lopper will be 0
   * and if we puts to timer_create zero timeout then we get sys error(22)
   */
  void SetTimeoutUnsafe(const Milliseconds timeout);

  /**
   * @brief startUnsafe, stopUnsafe - methods used for correct synchronization
   *  and must be used only with sync_primitive (auto_lock e.g.)
   * @return true if start/stop successfull, false if unsuccessfull
   */
  void StartUnsafe();
  bool StopUnsafe();

  /**
   * @brief alone function which sends to posix_timer as callee
   * @param signal_value - structure with parameters of posix_timer callee
   */
  friend void HandlePosixTimer(sigval signal_value);

  DISALLOW_COPY_AND_ASSIGN(Timer);
};
}  // namespace timer

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_TIMER_H_
