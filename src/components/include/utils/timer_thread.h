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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_TIMER_THREAD
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_TIMER_THREAD

#include <time.h>

#include "utils/conditional_variable.h"
#include "utils/lock.h"
#include "utils/logger.h"
#include "utils/macro.h"
#include "utils/timer_thread.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

namespace timer {
// TODO(AKutsan): Remove this logger after bugfix
CREATE_LOGGERPTR_GLOBAL(logger_, "Timer")
class TimerDelegate;

/*
 * The TimerThread class provide possibility to run timer in a separate thread.
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
template <class T>
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
     *  Atantion! "f()" will be called not in main thread but in timer thread
     *  Never use stop() and start() methods inside f
     * @param is_looper    Define this timer as looer,
     *  if true, TimerThread will call "f()" function every time out
     *  until stop()
     */
    TimerThread(const char* name, T* callee , void (T::*f)(), bool is_looper = false);

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
     * @param timeout_seconds Timeout in seconds to be set
     */
    virtual void start(uint32_t timeout_seconds);

    /**
     * @brief Stops timer execution
     * Must not be used in callback function!
     */
    virtual void stop();

    /**
     * @brief Tell tmer status
     * @return true if timer is currently running, therwise return false
     */
    virtual bool isRunning();

    /*
     * @brief Stop timer update timeout and start timer again
     * Note that it cancel thread of timer, If you use it from callback,
     * it probably will stop execution of callback function
     * @param timeout_seconds new timout value
     *
     */
    virtual void updateTimeOut(const uint32_t timeout_seconds);
    threads::Thread*                                   thread_;
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
        TimerDelegate(const TimerThread* timer_thread);

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
        virtual bool exitThreadMain();

        /**
         * @brief Set new Timeout
         * @param timeout_seconds New timeout to be set
         */
        virtual void setTimeOut(const uint32_t timeout_seconds);

      protected:
        const TimerThread*                               timer_thread_;
        uint32_t                                         timeout_seconds_;
        sync_primitives::Lock                            state_lock_;
        sync_primitives::ConditionalVariable             termination_condition_;
        volatile bool                                    stop_flag_;

      private:
        DISALLOW_COPY_AND_ASSIGN(TimerDelegate);
    };


    /**
     * @brief Delegate release looper timer.
     * Will call delegate every timeot function while stop()
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
        TimerLooperDelegate(const TimerThread* timer_thread);

        /**
         * @brief Thread main function.
         */
        virtual void threadMain();
      private:
        DISALLOW_COPY_AND_ASSIGN(TimerLooperDelegate);
    };
    void (T::*callback_)();
    T*                                                callee_;
    TimerDelegate*                                     delegate_;
    //threads::Thread*                                   thread_;
    mutable bool                                       is_running_;

    DISALLOW_COPY_AND_ASSIGN(TimerThread);
};

template <class T>
TimerThread<T>::TimerThread(const char* name, T* callee, void (T::*f)(), bool is_looper)
  : thread_(NULL),
    callback_(f),
    callee_(callee),
    delegate_(NULL),
    is_running_(false) {
  if (is_looper) {
    delegate_ = new TimerLooperDelegate(this);
  } else {
    delegate_ = new TimerDelegate(this);
  }
  thread_ = new threads::Thread("TimerThread", delegate_);
}

template <class T>
TimerThread<T>::~TimerThread() {
  if (is_running_) {
    stop();
  }
  callback_ = NULL;
  callee_ = NULL;
  delete thread_;
  // delegate_ will be deleted by thread_
  thread_ = NULL;
  delegate_ = NULL;
}

template <class T>
void TimerThread<T>::start(uint32_t timeout_seconds) {
  if (is_running_) {
    stop();
  }

  delegate_->setTimeOut(timeout_seconds);
  if (delegate_ && thread_) {
    is_running_ = true;
    thread_->start();
  }
}

template <class T>
void TimerThread<T>::stop() {
  if (delegate_ && thread_) {
    thread_->stop();
    is_running_ = false;
  }
}

template <class T>
bool TimerThread<T>::isRunning() {
  return is_running_;
}

template <class T>
void TimerThread<T>::updateTimeOut(const uint32_t timeout_seconds) {
  delegate_->setTimeOut(timeout_seconds);
}

template <class T>
void TimerThread<T>::onTimeOut() const {
  if (callee_ && callback_) {
    (callee_->*callback_)();
    is_running_ = false;
  }
}

template <class T>
TimerThread<T>::TimerDelegate::TimerDelegate(const TimerThread* timer_thread)
  : timer_thread_(timer_thread),
    timeout_seconds_(0),
    stop_flag_(false) {
  DCHECK(timer_thread_);
}

template <class T>
TimerThread<T>::TimerLooperDelegate::TimerLooperDelegate(const TimerThread* timer_thread)
  : TimerDelegate(timer_thread) {
}

template <class T>
TimerThread<T>::TimerDelegate::~TimerDelegate() {
  timer_thread_ = NULL;
}

template <class T>
void TimerThread<T>::TimerDelegate::threadMain() {
  using sync_primitives::ConditionalVariable;
  sync_primitives::AutoLock auto_lock(state_lock_);
  const time_t end_time = time(NULL) + timeout_seconds_;
  int32_t wait_seconds_left = int32_t(difftime(end_time, time(NULL)));
  while (!stop_flag_) {
    // Sleep
    ConditionalVariable::WaitStatus wait_status =
        termination_condition_.WaitFor(auto_lock, wait_seconds_left * 1000);
    wait_seconds_left = int32_t(difftime(end_time, time(NULL)));
    // Quit sleeping or continue sleeping in case of spurious wake up
    if (ConditionalVariable::kTimeout == wait_status ||
        wait_seconds_left <= 0) {
      break;
    }
  }
  if (!stop_flag_) {
    timer_thread_->onTimeOut();
  }
  stop_flag_ = false;
}

template <class T>
void TimerThread<T>::TimerLooperDelegate::threadMain() {
  using sync_primitives::ConditionalVariable;
  sync_primitives::AutoLock auto_lock(TimerDelegate::state_lock_);
  while (!TimerDelegate::stop_flag_) {
    time_t cur_time = time(NULL);
    uint64_t end_time64 = static_cast<uint64_t>(cur_time) + TimerDelegate::timeout_seconds_;
    time_t end_time = INT32_MAX;
    if (end_time64 < INT32_MAX) {
      end_time = static_cast<time_t>(end_time64);
    }
    int32_t wait_seconds_left = int32_t(difftime(end_time, cur_time));
    ConditionalVariable::WaitStatus wait_status =
        TimerDelegate::termination_condition_.WaitFor(auto_lock, wait_seconds_left * 1000);
    // Quit sleeping or continue sleeping in case of spurious wake up
    if (ConditionalVariable::kTimeout == wait_status ||
        wait_seconds_left <= 0) {
      TimerDelegate::timer_thread_->onTimeOut();
    } else {
      LOG4CXX_DEBUG(logger_, "Timeout reset force:" << TimerDelegate::timeout_seconds_);
    }
  }
  TimerDelegate::stop_flag_ = false;
}


template <class T>
bool TimerThread<T>::TimerDelegate::exitThreadMain() {
  {
    sync_primitives::AutoLock auto_lock(state_lock_);
    stop_flag_ = true;
  }
  termination_condition_.NotifyOne();
  return true;
}

template <class T>
void TimerThread<T>::TimerDelegate::setTimeOut(const uint32_t timeout_seconds) {
  timeout_seconds_ = timeout_seconds;
  termination_condition_.NotifyOne();
}


}  // namespace timer

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_TIMER_THREAD
