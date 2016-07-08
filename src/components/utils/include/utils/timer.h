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
#include <memory>

#include "utils/macro.h"
#include "utils/lock.h"
#include "utils/timer_task.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

namespace timer {

typedef uint32_t Milliseconds;

/**
 * @brief Timer calls custom callback function after
 * specified timeout has been elapsed.
 * Thread-safe class
 */
class Timer {
 public:
  /**
   * @brief Constructor
   * Does not start timer
   * @param name Timer name for identification
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
   * @param single_shot Single shot flag for timer
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
    TimerDelegate(const Timer* timer, sync_primitives::Lock& state_lock_ref);

    /**
     * @brief Sets timer timeout
     * @param timeout Timeout in milliseconds to be set
     */
    void set_timeout(const Milliseconds timeout);

    /**
     * @brief Gets timer timeout
     * @return Timer timeout
     */
    Milliseconds timeout() const;

    /**
     * @brief Sets timer delegate stop flag
     * @param stop_flag Bool flag to be set
     */
    void set_stop_flag(const bool stop_flag);

    /**
     * @brief Gets timer delegate stop flag
     * @return Delegate stop flag
     */
    bool stop_flag() const;

    /**
     * @brief Sets timer delegate finalized flag
     * @param finalized_flag Bool flag to be set
     */
    void set_finalized_flag(const bool finalized_flag);

    /**
     * @brief Gets timer delegate finalized flag
     * @return Delegate finalized flag
     */
    bool finalized_flag() const;

    void threadMain() OVERRIDE;
    void exitThreadMain() OVERRIDE;

   private:
    const Timer* timer_;
    Milliseconds timeout_;

    /**
     * @brief Stop flag shows if timer should be stopped
     * after next iteration
     */
    bool stop_flag_;

    /**
     * @brief Finalized flag shows if timer is finalized
     * and cannot be restarted until actual thread stopping
     */
    bool finalized_flag_;

    sync_primitives::Lock& state_lock_ref_;
    sync_primitives::ConditionalVariable state_condition_;

    DISALLOW_COPY_AND_ASSIGN(TimerDelegate);
  };

  /**
   * @brief Sets up timer delegate to start state.
   * Not thread-safe
   * @param timeout Timer timeout
   */
  void StartDelegate(const Milliseconds timeout) const;

  /**
   * @brief Sets up timer delegate to stop state.
   * Not thread-safe
   */
  void StopDelegate() const;

  /**
   * @brief Starts timer thread.
   * Not thread-safe
   */
  void StartThread();

  /**
   * @brief Stops timer thread.
   * Not thread-safe
   */
  void StopThread();

  /**
   * @brief Callback called on timeout.
   * Not thread-safe
   */
  void OnTimeout() const;

  const std::string name_;
  TimerTask* task_;

  mutable sync_primitives::Lock state_lock_;

  mutable std::auto_ptr<TimerDelegate> delegate_;
  threads::Thread* thread_;

  /**
   * @brief Single shot flag shows if timer should be fired once
   */
  bool single_shot_;

  DISALLOW_COPY_AND_ASSIGN(Timer);
};

}  // namespace timer

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_TIMER_H_
