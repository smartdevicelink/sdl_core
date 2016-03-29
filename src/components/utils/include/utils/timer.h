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
#include <stdint.h>
#include <string>
#include <limits>
#include <memory>

#include "utils/lock.h"
#include "utils/timer_task.h"
#include "utils/atomic_object.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

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
  Timer(const std::string& name, TimerTask* task_for_tracking);

  /**
   * @brief destructor - if timer running : call stop in the body
   */
  virtual ~Timer();
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
   * @brief Stop timer update timeout and start timer again
   * Note that it cancel thread of timer, If you use it from callback,
   * it probably will stop execution of callback function
   * @param timeout_milliseconds new timeout value
   *
   */
  void UpdateTimeOut(const Milliseconds timeout_milliseconds);

  /**
   * @brief GetTimeout
   * @return returns timeout
   */
  Milliseconds GetTimeout() const;

 private:
  /**
   * @brief Delegate release timer, will call callback function one time
   * or call delegate every timeout function while stop()
   * won't be called. It's depend on flag.
   */
  class TimerDelegate : public threads::ThreadDelegate {
   public:
    /**
     * @brief Default constructor
     *
     * @param timer_thread The Timer_thread pointer
     */
    explicit TimerDelegate(Timer* timer);

    /**
     * @brief Destructor
     */
    virtual ~TimerDelegate();

    /**
     * @brief Thread main function.
     */
    void threadMain() OVERRIDE;

    /**
     * @brief Called by thread::thread to free all allocated resources.
     */
    void exitThreadMain() OVERRIDE;

    /**
     * @brief Set new Timeout
     * @param timeout_milliseconds New timeout to be set
     */
    virtual void SetTimeOut(const uint32_t timeout_milliseconds);

    /**
      * @brief Wait until timer will start
      */
    virtual void WaitUntilStart();

    /**
     * @brief Quits threadMain function after next loop.
     */
    virtual void ShouldBeStopped();

    /**
     * @brief Restarts non-loop timer after current iteration.
     */
    virtual void ShouldBeRestarted();

    /**
     * @brief Return flag IsGoingStop
     */
    bool IsGoingToStop() const;

    /**
     * @brief Gets timeout with overflow check
     * @return timeout
     */
    inline int32_t get_timeout() const {
      return std::min(
          static_cast<uint32_t>(std::numeric_limits<int32_t>::max()),
          timeout_milliseconds_);
    }

    /**
      * @brief Make class delegate repeatable
      */
    inline void make_repeatable() {
      is_repeatable_ = true;
    }

   protected:
    Timer* timer_;
    uint32_t timeout_milliseconds_;
    sync_primitives::Lock state_lock_;
    sync_primitives::ConditionalVariable termination_condition_;
    sync_primitives::ConditionalVariable starting_condition_;
    sync_primitives::atomic_bool stop_flag_;
    sync_primitives::atomic_bool restart_flag_;
    sync_primitives::atomic_bool is_started_flag_;
    sync_primitives::atomic_bool is_repeatable_;

   private:
    DISALLOW_COPY_AND_ASSIGN(TimerDelegate);
  };

  const std::string name_;
  std::auto_ptr<TimerTask> task_;
  uint32_t timeout_ms_;
  TimerDelegate delegate_;
  threads::Thread* thread_;

  mutable sync_primitives::Lock lock_;
  mutable sync_primitives::Lock task_lock_;

  /**
   * @brief method called from friend handler_wrapper and call run() from task.
   */
  void OnTimeout() const;

  /**
   * @brief Suspends timer execution after next loop.
   */
  virtual void Suspend();

  DISALLOW_COPY_AND_ASSIGN(Timer);
};
}  // namespace timer

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_TIMER_H_
