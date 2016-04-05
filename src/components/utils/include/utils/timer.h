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

#include <string>
#include <stdint.h>

#include "utils/macro.h"
#include "utils/lock.h"
#include "utils/timer_task.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

namespace timer {

typedef uint32_t Milliseconds;

/**
 * @brief Timer calls custom callback function after
 * specified timeout has been elapsed
 */
class Timer {
 public:
  /**
   * @brief Constructor
   * Does not starts timer
   * @param name Timer name for identity
   * @param task Task for tracking
   */
  Timer(const std::string& name, TimerTask* task);

  /**
   * @brief Destructor
   * Stops timer if it's running
   */
  ~Timer();
  
  /**
   * @brief Starts timer with specified timeout
   * @param timeout Timer timeout
   * @param single_shot Shows needs to restart timer after timeout
   */
  void Start(const Milliseconds timeout, const bool single_shot);

  /**
   * @brief Stops timer if it's running
   */
  void Stop();

  /**
   * @brief Gets current timer status
   * @return True in case of timer is running, false otherwise
   */
  bool is_running() const;

  /**
   * @brief Gets current timer timeout
   * @return Current timeout in milliseconds.
   * Null if timer has not been started
   */
  Milliseconds timeout() const;

 private:
  /**
   * @brief Delegate for timer thread
   */
  class TimerDelegate : public threads::ThreadDelegate {
   public:
    /**
     * @brief Constructor
     * @param timer Timer instance pointer for callback calling
     */
    explicit TimerDelegate(const Timer* timer);

    /**
     * @brief Sets timer timeout
     * Thread-safe method
     * @param timeout Timeout in milliseconds to be set
     */
    void set_timeout(const Milliseconds timeout);

    /**
     * @brief Gets timer timeout
     * Thread-safe method
     * @return Timer timeout
     */
    Milliseconds timeout() const;

    /**
     * @brief Sets timer delegate stop flag
     * Thread-safe method
     * @param stop_flag Bool flag to be set
     */
    void set_stop_flag(const bool stop_flag);

    /**
     * @brief Gets timer delegate stop flag
     * Thread-safe method
     * @return Delegate stop flag
     */
    bool stop_flag() const;

    void threadMain() OVERRIDE;
    void exitThreadMain() OVERRIDE;

   private:
    const Timer* timer_;

    /*
     * Params lock used to protect timeout_ and stop_flag_ variables
     */
    mutable sync_primitives::Lock params_lock_;
    Milliseconds timeout_;
    bool stop_flag_;

    /*
     * State lock used to protect condition variable
     */
    sync_primitives::Lock state_lock_;
    sync_primitives::ConditionalVariable termination_condition_;

    DISALLOW_COPY_AND_ASSIGN(TimerDelegate);
  };

  /**
   * @brief Callback called on timeout
   */
  void OnTimeout() const;

  const std::string name_;

  /*
   * Task lock used to protect task from deleting during execution
   */
  mutable sync_primitives::Lock task_lock_;
  TimerTask* task_;

  mutable TimerDelegate delegate_;
  threads::Thread* thread_;

  /*
   * We should not protect this variable with any
   * synchronization primitives in current implementation
   * because we use it only in two places, that cannot
   * be invoked simultaneously
   */
  bool single_shot_;

  DISALLOW_COPY_AND_ASSIGN(Timer);
};

}  // namespace timer

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_TIMER_H_
