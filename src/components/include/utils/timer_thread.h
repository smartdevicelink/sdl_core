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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_TIMER_THREAD_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_TIMER_THREAD_H_

#include <time.h>
#include <inttypes.h>
#include <cstdint>
#include <limits>
#include <string>
#include <algorithm>

#include "utils/conditional_variable.h"
#include "utils/lock.h"
#include "utils/logger.h"
#include "utils/macro.h"
#include "utils/timer_thread.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

namespace timer {
// TODO(AKutsan): Remove this logger after bugfix
CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

class TimerDelegate;

/**
 * \class TimerThread
 * \brief TimerThread class provide possibility to run timer in a separate thread.
 * The client should specify callee and const callback function.
 * Example usage:
 *
 * Create timer in mobile request
 *
 * timer::TimerThread<MobileRequest> timer(this, &MobileRequest::onTimeOut);
 * timer.start(10);
 *
 * some actions
 *
 * To stop timer call timer.stop();
 *
 */
template<class T>
class TimerThread {
 public:
  friend class TimerDelegate;
  friend class TimerLooperDelegate;

  /**
   * @brief Default constructor
   *
   * @param name - display string to identify the thread.
   * @param callee A class that use timer
   * @param f    CallBackFunction which will be called on timeout
   *  Attention! "f()" will be called not in main thread but in timer thread
   *  Never use stop() and start() methods inside f
   * @param is_looper    Define this timer as looper,
   *  if true, TimerThread will call "f()" function every time out
   *  until stop()
   */
  TimerThread(const char* name, T* callee, void (T::*f)(), bool is_looper =
                  false);

  /**
   * @brief Destructor
   */
  virtual ~TimerThread();

  /**
   * @brief Starts timer for specified timeout.
   * Previously started timeout will be set to new value.
   * On timeout TimerThread::onTimeOut interface will be called.
   * Must not be used in callback function!
   *
   * @param timeout_milliseconds Timeout in seconds to be set
   */
  virtual void start(uint32_t timeout_milliseconds);

  /**
   * @brief Starts timer for specified timeout.
   * Previously started timeout will be set to new value.
   * On timeout TimerThread::onTimeOut interface will be called.
   * Must not be used in callback function!
   *
   * @param timeout_milliseconds Timeout in seconds to be set
   *
   * @param callee A class that use timer
   *
   * @param allBackFunction which will be called on timeout
   *  Attention! "f()" will be called not in main thread but in timer thread
   *  Never use stop() and start() methods inside f
   */
  virtual void start(uint32_t timeout_milliseconds, T* callee, void (T::*f)());

  /**
   * @brief Stops timer execution
   * Must not be used in callback function!
   */
  virtual void stop();

  /**
   * @brief Tell timer status
   * @return true if timer is currently running, otherwise return false
   */
  virtual bool isRunning();

  /**
   * @brief Suspends timer execution after next loop.
   */
  virtual void suspend();

  /**
   * @brief Stop timer update timeout and start timer again
   * Note that it cancel thread of timer, If you use it from callback,
   * it probably will stop execution of callback function
   * @param timeout_milliseconds new timeout value
   *
   */
  virtual void updateTimeOut(const uint32_t timeout_milliseconds);

 protected:
  /**
   * @brief Interface called by delegator on timeout
   */
  void onTimeOut() const;

 private:
  /**
   * @brief Delegate release timer, will call callback function one time
   */
  class TimerDelegate : public threads::ThreadDelegate {
   public:
    /**
     * @brief Default constructor
     *
     * @param timer_thread The Timer_thread pointer
     */
    explicit TimerDelegate(TimerThread* timer_thread);

    /**
     * @brief Destructor
     */
    virtual ~TimerDelegate();

    /**
     * @brief Thread main function.
     */
    virtual void threadMain();

    /**
     * @brief Called by thread::thread to free all allocated resources.
     */
    virtual void exitThreadMain();

    /**
     * @brief Set new Timeout
     * @param timeout_milliseconds New timeout to be set
     */
    virtual void setTimeOut(const uint32_t timeout_milliseconds);

    /**
     * @brief Quits threadMain function after next loop.
     */
    virtual void shouldBeStoped();

    /**
     * @brief Restarts non-loop timer after current iteration.
     */
    virtual void shouldBeRestarted();

   protected:
    TimerThread* timer_thread_;
    uint32_t timeout_milliseconds_;
    sync_primitives::Lock state_lock_;
    sync_primitives::ConditionalVariable termination_condition_;
    volatile bool stop_flag_;
    sync_primitives::Lock restart_flag_lock_;
    volatile bool restart_flag_;

    /**
     * @brief Gets timeout with overflow check
     * @return timeout
     */
    inline int32_t get_timeout() const {
      return std::min(
            static_cast<uint32_t>(std::numeric_limits<int32_t>::max()),
            timeout_milliseconds_);
    }

   private:
    DISALLOW_COPY_AND_ASSIGN(TimerDelegate);
  };

  /**
   * @brief Delegate release looper timer.
   * Will call delegate every timeout function while stop()
   * won't be called
   */
  class TimerLooperDelegate : public TimerDelegate {
   public:
    /**
     * @brief Default constructor
     *
     * @param timer_thread The Timer_thread pointer
     * @param timeout      Timeout to be set
     */
    explicit TimerLooperDelegate(TimerThread* timer_thread);

    /**
     * @brief Thread main function.
     */
    virtual void threadMain();

   private:
    DISALLOW_COPY_AND_ASSIGN(TimerLooperDelegate);
  };
  threads::Thread* thread_;
  void (T::*callback_)();
  T* callee_;
  TimerDelegate* delegate_;
  std::string name_;
  volatile bool is_looper_;

  DISALLOW_COPY_AND_ASSIGN(TimerThread);
};

template<class T>
TimerThread<T>::TimerThread(const char* name, T* callee, void (T::*f)(),
                            bool is_looper)
    : thread_(NULL),
      callback_(f),
      callee_(callee),
      delegate_(NULL),
      name_(name),
      is_looper_(is_looper) {
  delegate_ =
      is_looper_ ? new TimerLooperDelegate(this) : new TimerDelegate(this);

  thread_ = threads::CreateThread(name_.c_str(), delegate_);
}

template<class T>
TimerThread<T>::~TimerThread() {
  LOG4CXX_DEBUG(logger_, "TimerThread is to be destroyed " << name_);
  thread_->join();
  delete delegate_;
  threads::DeleteThread(thread_);
  callback_ = NULL;
  callee_ = NULL;
}

template<class T>
void TimerThread<T>::start(uint32_t timeout_milliseconds) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (isRunning()) {
    LOG4CXX_INFO(logger_, "Restart timer in thread " << name_);
    delegate_->shouldBeRestarted();
    updateTimeOut(timeout_milliseconds);
  } else {
    updateTimeOut(timeout_milliseconds);
    thread_->start();
  }
}

template<class T>
void TimerThread<T>::start(uint32_t timeout_milliseconds, T* callee,
                           void (T::*f)()) {
  callee_ = callee;
  callback_ = f;
  start(timeout_milliseconds);
}

template<class T>
void TimerThread<T>::stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(thread_);
  LOG4CXX_DEBUG(logger_, "Stopping timer  " << name_);
  thread_->join();
}

template<class T>
bool TimerThread<T>::isRunning() {
  DCHECK(thread_);
  return thread_->is_running();
}

template<class T>
void TimerThread<T>::suspend() {
  LOG4CXX_DEBUG(logger_, "Suspend timer " << name_ << " after next loop");
  delegate_->shouldBeStoped();
}

template<class T>
void TimerThread<T>::updateTimeOut(const uint32_t timeout_milliseconds) {
  delegate_->setTimeOut(timeout_milliseconds);
}

template<class T> void TimerThread<T>::onTimeOut() const {
  if (callee_ && callback_) {
    (callee_->*callback_)();
  }
}

template<class T>
TimerThread<T>::TimerDelegate::TimerDelegate(TimerThread* timer_thread)
    : timer_thread_(timer_thread),
      timeout_milliseconds_(0),
      state_lock_(true),
      stop_flag_(false),
      restart_flag_(false) {
  DCHECK(timer_thread_);
}

template<class T>
TimerThread<T>::TimerLooperDelegate::TimerLooperDelegate(
    TimerThread* timer_thread)
    : TimerDelegate(timer_thread) {
}

template<class T>
TimerThread<T>::TimerDelegate::~TimerDelegate() {
  timer_thread_ = NULL;
}

template<class T>
void TimerThread<T>::TimerDelegate::threadMain() {
  using sync_primitives::ConditionalVariable;
  sync_primitives::AutoLock auto_lock(state_lock_);
  stop_flag_ = false;
  while (!stop_flag_) {
    // Sleep
    int32_t wait_milliseconds_left = TimerDelegate::get_timeout();
    LOG4CXX_DEBUG(logger_, "Milliseconds left to wait: "
                  << wait_milliseconds_left);
    ConditionalVariable::WaitStatus wait_status =
        termination_condition_.WaitFor(auto_lock, wait_milliseconds_left);
    // Quit sleeping or continue sleeping in case of spurious wake up
    if (ConditionalVariable::kTimeout == wait_status
        || wait_milliseconds_left <= 0) {
      LOG4CXX_TRACE(logger_,
                    "Timer timeout (ms): " << wait_milliseconds_left);
      timer_thread_->onTimeOut();
    } else {
      LOG4CXX_DEBUG(logger_, "Timeout reset force (ms): "
                    << TimerDelegate::timeout_milliseconds_);
    }
    {
      sync_primitives::AutoLock auto_lock(restart_flag_lock_);
      if (!restart_flag_) {
        return;
      }
      restart_flag_ = false;
    }
  }
}

template<class T>
void TimerThread<T>::TimerLooperDelegate::threadMain() {
  using sync_primitives::ConditionalVariable;
  sync_primitives::AutoLock auto_lock(TimerDelegate::state_lock_);
  TimerDelegate::stop_flag_ = false;
  while (!TimerDelegate::stop_flag_) {
    int32_t wait_milliseconds_left = TimerDelegate::get_timeout();
    LOG4CXX_DEBUG(logger_, "Milliseconds left to wait: "
                  << wait_milliseconds_left);
    ConditionalVariable::WaitStatus wait_status =
        TimerDelegate::termination_condition_.WaitFor(auto_lock,
                                                      wait_milliseconds_left);
    // Quit sleeping or continue sleeping in case of spurious wake up
    if (ConditionalVariable::kTimeout == wait_status
        || wait_milliseconds_left <= 0) {
      LOG4CXX_TRACE(logger_,
                    "Timer timeout (ms): " << wait_milliseconds_left);
      TimerDelegate::timer_thread_->onTimeOut();
    } else {
      LOG4CXX_DEBUG(logger_, "Timeout reset force (ms): "
                    << TimerDelegate::timeout_milliseconds_);
    }
  }
}

template<class T>
void TimerThread<T>::TimerDelegate::exitThreadMain() {
  shouldBeStoped();
  termination_condition_.NotifyOne();
}

template<class T>
void TimerThread<T>::TimerDelegate::setTimeOut(
    const uint32_t timeout_milliseconds) {
  timeout_milliseconds_ = timeout_milliseconds;
  termination_condition_.NotifyOne();
}

template<class T>
void TimerThread<T>::TimerDelegate::shouldBeStoped() {
  {
    sync_primitives::AutoLock auto_lock(state_lock_);
    stop_flag_ = true;
  }
  {
    sync_primitives::AutoLock auto_lock(restart_flag_lock_);
    restart_flag_ = false;
  }
}

template<class T>
void TimerThread<T>::TimerDelegate::shouldBeRestarted() {
  sync_primitives::AutoLock auto_lock(restart_flag_lock_);
  restart_flag_ = true;
}

}  // namespace timer

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_TIMER_THREAD_H_
