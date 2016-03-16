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

namespace timer {

// Function HandlePosixTimer is not in anonymous namespace
// because we need to set this func as friend to Timer
// and for setting friend function must be located in same namespace with class
void HandlePosixTimer(sigval signal_value) {
  LOG4CXX_AUTO_TRACE(logger_);

  DCHECK_OR_RETURN_VOID(signal_value.sival_ptr)

  timer::Timer* timer = static_cast<timer::Timer*>(signal_value.sival_ptr);
  timer->OnTimeout();
}
}  // namespace timer

namespace {
const int kErrorCode = -1;

itimerspec MillisecondsToItimerspec(const timer::Milliseconds miliseconds) {
  struct itimerspec result;

  result.it_value.tv_sec = miliseconds / DateTime::MILLISECONDS_IN_SECOND;
  result.it_value.tv_nsec = (miliseconds % DateTime::MILLISECONDS_IN_SECOND) *
                            DateTime::NANOSECONDS_IN_MILLISECOND;
  result.it_interval.tv_sec = 0;
  result.it_interval.tv_nsec = 0;

  return result;
}

timer_t StartPosixTimer(timer::Timer& trackable,
                        const timer::Milliseconds timeout) {
  LOG4CXX_AUTO_TRACE(logger_);
  timer_t internal_timer = NULL;

  sigevent signal_event;
  signal_event.sigev_notify = SIGEV_THREAD;
  signal_event.sigev_notify_attributes = NULL;
  signal_event.sigev_value.sival_ptr = static_cast<void*>(&trackable);
  signal_event.sigev_notify_function = timer::HandlePosixTimer;

  if (timer_create(CLOCK_REALTIME, &signal_event, &internal_timer) ==
      kErrorCode) {
    int error_code = errno;
    LOG4CXX_FATAL(logger_,
                  "Can`t create posix_timer. Error("
                      << error_code << "): " << strerror(error_code));
    return NULL;
  }
  const itimerspec itimer = MillisecondsToItimerspec(timeout);

  if (timer_settime(internal_timer, 0, &itimer, NULL) == kErrorCode) {
    int error_code = errno;
    UNUSED(error_code);
    LOG4CXX_FATAL(logger_,
                  "Can`t set timeout to posix_timer. Error("
                      << error_code << "): " << strerror(error_code));
    return NULL;
  }
  return internal_timer;
}

bool StopPosixTimer(timer_t timer) {
  LOG4CXX_AUTO_TRACE(logger_);
  const int resultCode = timer_delete(timer);
  if (kErrorCode == resultCode) {
    int error_code = errno;
    LOG4CXX_ERROR(logger_,
                  "Can`t delete posix_timer. Error("
                      << error_code << "): " << strerror(error_code));
    return false;
  }
  return true;
}
}  // namespace

timer::Timer::Timer(const std::string& name, const TimerTask* task_for_tracking)
    : name_(name)
    , task_(task_for_tracking)
    , repeatable_(false)
    , timeout_ms_(0u)
    , is_running_(false)
    , timer_(NULL) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(!name_.empty());
  DCHECK(task_);
}

timer::Timer::~Timer() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Timer is to be destroyed " << name_);
  Stop();
  sync_primitives::AutoLock auto_lock(task_lock_);
  DCHECK(task_);
  delete task_;
}

void timer::Timer::Start(const Milliseconds timeout, const bool repeatable) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(lock_);
  SetTimeoutUnsafe(timeout);
  repeatable_ = repeatable;
  if (is_running_) {
    const bool stop_result = StopUnsafe();
    DCHECK_OR_RETURN_VOID(stop_result);
  }
  StartUnsafe();
}

void timer::Timer::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(lock_);
  repeatable_ = false;
  if (is_running_) {
    const bool stop_result = StopUnsafe();
    DCHECK(stop_result);
  }
}

bool timer::Timer::IsRunning() const {
  sync_primitives::AutoLock auto_lock(lock_);
  return is_running_;
}

void timer::Timer::OnTimeout() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "Timer has finished counting. Timeout(ms): "
                    << static_cast<uint32_t>(timeout_ms_));
  {
    // Task locked by own lock because from this task in callback we can
    // call Stop of this timer and get DeadLock
    sync_primitives::AutoLock auto_lock(task_lock_);
    DCHECK(task_);
    task_->run();
  }
  sync_primitives::AutoLock auto_lock(lock_);
  if (is_running_) {
    const bool stop_result = StopUnsafe();
    DCHECK_OR_RETURN_VOID(stop_result);
  }
  if (repeatable_) {
    StartUnsafe();
  }
}

void timer::Timer::SetTimeoutUnsafe(const timer::Milliseconds timeout) {
  timeout_ms_ = (0u != timeout) ? timeout : 1u;
}

void timer::Timer::StartUnsafe() {
  LOG4CXX_DEBUG(logger_, "Creating posix_timer in " << name_);
  // Create new posix timer
  timer_ = StartPosixTimer(*this, timeout_ms_);
  DCHECK_OR_RETURN_VOID(timer_);
  is_running_ = true;
}

bool timer::Timer::StopUnsafe() {
  LOG4CXX_DEBUG(logger_, "Stopping timer  " << name_);
  //  Destroing of posix timer
  if (StopPosixTimer(timer_)) {
    is_running_ = false;
    return true;
  }
  return false;
}

timer::Milliseconds timer::Timer::GetTimeout() const {
  sync_primitives::AutoLock auto_lock(lock_);
  return timeout_ms_;
}
