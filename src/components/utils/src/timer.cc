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
    , task_(task)
    , state_lock_()
    , delegate_(new TimerDelegate(this, state_lock_))
    , thread_(threads::CreateThread(name_.c_str(), delegate_.get()))
    , single_shot_(true) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(!name_.empty());
  DCHECK(task_);
  DCHECK(thread_);
  LOG4CXX_DEBUG(logger_, "Timer " << name_ << " has been created");
}

timer::Timer::~Timer() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(state_lock_);
  StopThread();
  StopDelegate();
  single_shot_ = true;

  delegate_.release();
  DeleteThread(thread_);
  DCHECK(task_);
  delete task_;
  LOG4CXX_DEBUG(logger_, "Timer " << name_ << " has been destroyed");
}

void timer::Timer::Start(const Milliseconds timeout, const bool single_shot) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(state_lock_);
  StopThread();
  single_shot_ = single_shot;
  StartDelegate(timeout);
  StartThread();
  LOG4CXX_DEBUG(logger_, "Timer " << name_ << " has been started");
}

void timer::Timer::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(state_lock_);
  StopThread();
  StopDelegate();
  single_shot_ = true;
  LOG4CXX_DEBUG(logger_, "Timer " << name_ << " has been stopped");
}

bool timer::Timer::is_running() const {
  sync_primitives::AutoLock auto_lock(state_lock_);
  return !delegate_->stop_flag();
}

timer::Milliseconds timer::Timer::timeout() const {
  sync_primitives::AutoLock auto_lock(state_lock_);
  return delegate_->timeout();
}

void timer::Timer::StartDelegate(const Milliseconds timeout) const {
  delegate_->set_stop_flag(false);
  delegate_->set_timeout(timeout);
}

void timer::Timer::StopDelegate() const {
  delegate_->set_stop_flag(true);
  delegate_->set_timeout(0);
}

void timer::Timer::StartThread() {
  if (delegate_->finalized_flag()) {
    return;
  }

  DCHECK_OR_RETURN_VOID(thread_);
  if (!thread_->IsCurrentThread()) {
    thread_->start();
  }
}

void timer::Timer::StopThread() {
  if (delegate_->finalized_flag()) {
    return;
  }

  DCHECK_OR_RETURN_VOID(thread_);
  if (!thread_->IsCurrentThread()) {
    delegate_->set_finalized_flag(true);
    {
      sync_primitives::AutoUnlock auto_unlock(state_lock_);
      thread_->join();
    }
    delegate_->set_finalized_flag(false);
  }
}

void timer::Timer::OnTimeout() const {
  {
    sync_primitives::AutoLock auto_lock(state_lock_);
    if (single_shot_) {
      StopDelegate();
    }
  }

  DCHECK_OR_RETURN_VOID(task_);
  task_->run();
}

timer::Timer::TimerDelegate::TimerDelegate(
    const Timer* timer, sync_primitives::Lock& state_lock_ref)
    : timer_(timer)
    , timeout_(0)
    , stop_flag_(true)
    , finalized_flag_(false)
    , state_lock_ref_(state_lock_ref)
    , state_condition_() {
  DCHECK(timer_);
}

void timer::Timer::TimerDelegate::set_timeout(const Milliseconds timeout) {
  timeout_ = timeout;
}

timer::Milliseconds timer::Timer::TimerDelegate::timeout() const {
  return timeout_;
}

void timer::Timer::TimerDelegate::set_stop_flag(const bool stop_flag) {
  stop_flag_ = stop_flag;
}

bool timer::Timer::TimerDelegate::stop_flag() const {
  return stop_flag_;
}

void timer::Timer::TimerDelegate::set_finalized_flag(
    const bool finalized_flag) {
  finalized_flag_ = finalized_flag;
}

bool timer::Timer::TimerDelegate::finalized_flag() const {
  return finalized_flag_;
}

void timer::Timer::TimerDelegate::threadMain() {
  sync_primitives::AutoLock auto_lock(state_lock_ref_);
  while (!stop_flag_ && !finalized_flag_) {
    LOG4CXX_DEBUG(logger_, "Milliseconds left to wait: " << timeout_);
    if (sync_primitives::ConditionalVariable::kTimeout ==
        state_condition_.WaitFor(auto_lock, timeout_)) {
      LOG4CXX_DEBUG(logger_,
                    "Timer has finished counting. Timeout (ms): " << timeout_);
      if (timer_) {
        sync_primitives::AutoUnlock auto_unlock(auto_lock);
        timer_->OnTimeout();
      }
    } else {
      LOG4CXX_DEBUG(logger_, "Timer has been force reset");
    }
  }
}

void timer::Timer::TimerDelegate::exitThreadMain() {
  sync_primitives::AutoLock auto_lock(state_lock_ref_);
  state_condition_.NotifyOne();
}
