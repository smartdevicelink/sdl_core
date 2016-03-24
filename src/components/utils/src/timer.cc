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

#include "utils/timer.h"

#include <ctime>
#include <cerrno>
#include <cstring>

#include "utils/timer_task.h"
#include "utils/date_time.h"
#include "utils/macro.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

using date_time::DateTime;

timer::Timer::Timer(const std::string& name, const TimerTask* task_for_tracking)
    : name_(name)
    , task_(task_for_tracking)
    , timeout_ms_(0u)
    , is_running_(false)
    , thread_(NULL)
    , delegate_(NULL) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(!name_.empty());
  DCHECK(task_);
}

timer::Timer::~Timer() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Timer is to be destroyed " << name_);
  sync_primitives::AutoLock auto_lock(task_lock_);
  DCHECK(task_);
  delete task_;
  if (thread_) {
    thread_->join();
    DCHECK(delegate_);
    delete delegate_;
    threads::DeleteThread(thread_);
  }
}

void timer::Timer::Start(const Milliseconds timeout, const bool repeatable) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(lock_);

  // There would be no way to stop thread if timeout in lopper will be 0
  timeout_ms_ = (0u != timeout) ? timeout : 1u;

  // It's new timer and we have to create it
  if (!delegate_ && !thread_) {
    delegate_ = new TimerDelegate(this);
    thread_ = threads::CreateThread(name_.c_str(), delegate_);
    DCHECK(delegate_);
    DCHECK(thread_);

    if (repeatable) {
      delegate_->MakeRepetable();
    }
  }
  // Else timer has been already starting

  if (thread_->is_running()) {
    LOG4CXX_INFO(logger_, "Restart timer in thread " << name_);
    delegate_->ShouldBeRestarted();
    UpdateTimeOut(timeout_ms_);
  } else {
    UpdateTimeOut(timeout_ms_);
    thread_->start();
  }

  delegate_->WaitUntilStart();
}

void timer::Timer::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(lock_);
  DCHECK(thread_);
  LOG4CXX_DEBUG(logger_, "Stopping timer  " << name_);
  if (pthread_equal(pthread_self(), thread_->thread_handle())) {
    // Thread can't stop itself , so it will suspend
    Suspend();
  } else {
    thread_->join();
  }
}

bool timer::Timer::IsRunning() const {
  sync_primitives::AutoLock auto_lock(lock_);
  if (!thread_)
    return false;
  DCHECK(delegate_);
  return (thread_->is_running() && !delegate_->IsGoingStop());
}

void timer::Timer::Suspend() {
  LOG4CXX_DEBUG(logger_, "Suspend timer " << name_ << " after next loop");
  delegate_->ShouldBeStoped();
}

void timer::Timer::UpdateTimeOut(const uint32_t timeout_milliseconds) {
  delegate_->SetTimeOut(timeout_milliseconds);
}

void timer::Timer::OnTimeout() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "Timer has finished counting. Timeout(ms): "
                    << static_cast<uint32_t>(timeout_ms_));

  // Task locked by own lock because from this task in callback
  // we can call Stop of this timer and get DeadLock
  sync_primitives::AutoLock auto_lock(task_lock_);
  DCHECK(task_);
  task_->run();
}

timer::Milliseconds timer::Timer::GetTimeout() const {
  sync_primitives::AutoLock auto_lock(lock_);
  return timeout_ms_;
}

timer::Timer::TimerDelegate::TimerDelegate(Timer* timer)
    : timer_(timer)
    , timeout_milliseconds_(0)
    , state_lock_(true)
    , stop_flag_(false)
    , restart_flag_(false)
    , is_started_flag_(false)
    , is_repeatable_(false) {
  DCHECK(timer_);
}

timer::Timer::TimerDelegate::~TimerDelegate() {
  timer_ = NULL;
}

void timer::Timer::TimerDelegate::threadMain() {
  using sync_primitives::ConditionalVariable;
  sync_primitives::AutoLock auto_lock(state_lock_);
  stop_flag_ = false;
  // Notify that we've already started
  TimerDelegate::is_started_flag_ = true;
  TimerDelegate::starting_condition_.NotifyOne();

  while (!stop_flag_) {
    // Sleep
    int32_t wait_milliseconds_left = TimerDelegate::Get_timeout();
    LOG4CXX_DEBUG(logger_,
                  "Milliseconds left to wait: " << wait_milliseconds_left);
    ConditionalVariable::WaitStatus wait_status =
        termination_condition_.WaitFor(auto_lock, wait_milliseconds_left);
    // Quit sleeping or continue sleeping in case of spurious wake up
    if (ConditionalVariable::kTimeout == wait_status ||
        wait_milliseconds_left <= 0) {
      LOG4CXX_DEBUG(logger_,
                    "Timer has finished counting. Timeout(ms): "
                        << wait_milliseconds_left);
      timer_->OnTimeout();
    } else {
      LOG4CXX_DEBUG(
          logger_,
          "Timeout reset force (ms): " << TimerDelegate::timeout_milliseconds_);
    }
    if (!is_repeatable_) {
      if (!restart_flag_) {
        return;
      }
      restart_flag_ = false;
    }
  }
}

void timer::Timer::TimerDelegate::exitThreadMain() {
  ShouldBeStoped();
  termination_condition_.NotifyOne();
}

void timer::Timer::TimerDelegate::SetTimeOut(
    const uint32_t timeout_milliseconds) {
  timeout_milliseconds_ = timeout_milliseconds;
  termination_condition_.NotifyOne();
}

void timer::Timer::TimerDelegate::ShouldBeStoped() {
    stop_flag_ = true;
    restart_flag_ = false;
}

void timer::Timer::TimerDelegate::ShouldBeRestarted() {
  restart_flag_ = true;
}

bool timer::Timer::TimerDelegate::IsGoingStop() {
  return stop_flag_;
}

void timer::Timer::TimerDelegate::WaitUntilStart() {
  sync_primitives::AutoLock auto_lock(TimerDelegate::state_lock_);

  while (!is_started_flag_) {
    starting_condition_.Wait(auto_lock);
  }
}

