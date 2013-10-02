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
#include "utils/macro.h"
#include "utils/timer_thread.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "utils/synchronisation_primitives.h"

namespace timer {

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
  /*
   * @brief Default constructor
   *
   * @param callee A class that use timer
   * @param arg    CallBackFunction which will be called on timeout
   */
  TimerThread(const T* callee , void (T::*f)() const);

  /*
   * @brief Destructor
   */
  virtual ~TimerThread();

  /*
   * @brief Starts timer for specified timeout.
   * Previously started timeout will be set to new value.
   * On timeout TimerThread::onTimeOut interface will be called.
   *
   * @param timeout Timeout in seconds to be set
   */
  virtual void start(unsigned int timeout);

  /*
   * @brief Stops timer execution
   */
  virtual void stop();

 protected:

  /*
   * @brief Interface called by delegator on timeout
   */
  void onTimeOut() const;

 private:

  class TimerDelegate : public threads::ThreadDelegate {
   public:

    /*
     * @brief Default constructor
     *
     * @param timer_thread The Timer_thread pointer
     * @param timeout      Timeout to be set
     */
    TimerDelegate(const TimerThread* timer_thread);

    /*
     * @brief Destructor
     */
    virtual ~TimerDelegate();

    /*
     * @brief Thread main function.
     */
    virtual void threadMain();

    /*
     * @brief Called by thread::thread to free all allocated resources.
     */
    virtual void exitThreadMain();

    /*
     * @brief Restart timer
     *
     * @param timeout New timeout to be set
     */
    virtual void setTimeOut(unsigned int timeout);

   protected:

   private:
    const TimerThread*                               timer_thread_;
    unsigned int                                     timeout_;
    sync_primitives::SynchronisationPrimitives       sync_primitive_;
    volatile bool                                    stop_flag_;

    DISALLOW_COPY_AND_ASSIGN(TimerDelegate);
  };

  void (T::*callback_)() const;
  const T*                                           callee_;
  TimerDelegate*                                     delegate_;
  threads::Thread*                                   thread_;
  mutable bool                                       is_running_;

  DISALLOW_COPY_AND_ASSIGN(TimerThread);
};

template <class T>
TimerThread<T>::TimerThread(const T* callee, void (T::*f)() const)
: callback_(f),
  callee_(callee),
  delegate_(NULL),
  thread_(NULL),
  is_running_(false) {

  delegate_ = new TimerDelegate(this);
  if (delegate_) {
    thread_ = new threads::Thread("TimerThread", delegate_);
  }
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
void TimerThread<T>::start(unsigned int timeout) {
  if (is_running_) {
    stop();
  }

  delegate_->setTimeOut(timeout);
  if (delegate_ && thread_) {
    is_running_ = true;
    thread_->startWithOptions(
        threads::ThreadOptions(threads::Thread::kMinStackSize));
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
void TimerThread<T>::onTimeOut() const {
  if (callee_ && callback_) {
    (callee_->*callback_)();
    is_running_ = false;
  }
}

template <class T>
TimerThread<T>::TimerDelegate::TimerDelegate(const TimerThread* timer_thread)
: timer_thread_(timer_thread),
  timeout_(0),
  stop_flag_(false) {

  sync_primitive_.init();
}

template <class T>
TimerThread<T>::TimerDelegate::~TimerDelegate() {
  timer_thread_ = NULL;
}

template <class T>
void TimerThread<T>::TimerDelegate::threadMain() {
  sync_primitive_.lock();
  if (!stop_flag_) {
    timespec time_spec;
    clock_gettime(CLOCK_REALTIME, &time_spec);
    time_spec.tv_sec += timeout_;
    pthread_cond_timedwait(&sync_primitive_.conditional_var(),
                           &sync_primitive_.mutex(),
                           &time_spec);

    if (false == stop_flag_ && timer_thread_) {
      timer_thread_->onTimeOut();
    }
  }
  stop_flag_ = false;
  sync_primitive_.unlock();
}

template <class T>
void TimerThread<T>::TimerDelegate::exitThreadMain() {
  stop_flag_ = true;
  sync_primitive_.signal();
}

template <class T>
void TimerThread<T>::TimerDelegate::setTimeOut(unsigned int timeout) {
  timeout_ = timeout;
}

}  // namespace timer

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_TIMER_THREAD
