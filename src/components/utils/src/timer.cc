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

#include <string>

#include "utils/macro.h"
#include "utils/logger.h"
#include "utils/lock.h"
#include "utils/timer_task.h"
#include "utils/conditional_variable.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

timer::Timer::Timer(const std::string& name, TimerTask* task)
    : name_(name)
    , task_lock_()
    , task_(task)
    , delegate_(this)
    , thread_(threads::CreateThread(name_.c_str(), &delegate_))
    , single_shot_(true) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(!name_.empty());
  DCHECK(task_);
  DCHECK(thread_);
  LOG4CXX_DEBUG(logger_, "Timer " << name_ << " has been created");
}

timer::Timer::~Timer() {
  LOG4CXX_AUTO_TRACE(logger_);
  Stop();
  DCHECK(thread_);
  DeleteThread(thread_);
  sync_primitives::AutoLock auto_lock(task_lock_);
  DCHECK(task_);
  delete task_;
  LOG4CXX_DEBUG(logger_, "Timer " << name_ << " has been destroyed");
}

void timer::Timer::Start(const Milliseconds timeout, const bool single_shot) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(thread_);
  Stop();
  delegate_.set_timeout(timeout);
  single_shot_ = single_shot;
  thread_->start();
  delegate_.set_stop_flag(false);
  LOG4CXX_DEBUG(logger_, "Timer " << name_ << " has been started");
}

void timer::Timer::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(thread_);
  delegate_.set_stop_flag(true);
  if (!thread_->IsCurrentThread()) {
    thread_->join();
  }
  delegate_.set_timeout(0);
  LOG4CXX_DEBUG(logger_, "Timer " << name_ << " has been stopped");
}

bool timer::Timer::is_running() const {
  return !delegate_.stop_flag();
}

timer::Milliseconds timer::Timer::timeout() const {
  return delegate_.timeout();
}

void timer::Timer::OnTimeout() const {
  LOG4CXX_AUTO_TRACE(logger_);
  delegate_.set_stop_flag(single_shot_);
  sync_primitives::AutoLock auto_lock(task_lock_);
  DCHECK_OR_RETURN_VOID(task_);
  task_->run();
}

timer::Timer::TimerDelegate::TimerDelegate(const Timer* timer)
    : timer_(timer)
    , params_lock_()
    , timeout_(0)
    , stop_flag_(true)
    , state_lock_() {
  DCHECK(timer_);
}

void timer::Timer::TimerDelegate::set_timeout(const Milliseconds timeout) {
  sync_primitives::AutoLock auto_lock(params_lock_);
  timeout_ = timeout;
}

timer::Milliseconds timer::Timer::TimerDelegate::timeout() const {
  sync_primitives::AutoLock auto_lock(params_lock_);
  return timeout_;
}

void timer::Timer::TimerDelegate::set_stop_flag(const bool stop_flag) {
  sync_primitives::AutoLock auto_lock(params_lock_);
  stop_flag_ = stop_flag;
}

bool timer::Timer::TimerDelegate::stop_flag() const {
  sync_primitives::AutoLock auto_lock(params_lock_);
  return stop_flag_;
}

void timer::Timer::TimerDelegate::threadMain() {
  sync_primitives::AutoLock auto_lock(state_lock_);
  set_stop_flag(false);
  while (!stop_flag()) {
    const Milliseconds curr_timeout = timeout();
    LOG4CXX_DEBUG(logger_, "Milliseconds left to wait: " << curr_timeout);
    if (sync_primitives::ConditionalVariable::kTimeout ==
        termination_condition_.WaitFor(auto_lock, curr_timeout)) {
      LOG4CXX_DEBUG(logger_,
                    "Timer has finished counting. Timeout (ms): "
                    << curr_timeout);
      if (timer_) {
        timer_->OnTimeout();
      }
    } else {
      LOG4CXX_DEBUG(logger_, "Timer has been force reset");
    }
  }
  set_timeout(0);
}

void timer::Timer::TimerDelegate::exitThreadMain() {
  sync_primitives::AutoLock auto_lock(state_lock_);
  termination_condition_.NotifyOne();
}
