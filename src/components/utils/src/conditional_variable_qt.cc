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
#include "utils/conditional_variable.h"

#include "utils/lock.h"
#include "utils/logger.h"
#include <QWaitCondition>

namespace sync_primitives {

SDL_CREATE_LOGGER("Utils")

ConditionalVariable::ConditionalVariable()
    : cond_var_()
#ifndef NDEBUG
    , waiting_threads_count_(0)
#endif
{
}

ConditionalVariable::~ConditionalVariable() {
#ifndef NDEBUG
  DCHECK(0 == waiting_threads_count_);
#endif
}

void ConditionalVariable::NotifyOne() {
  cond_var_.wakeOne();
}

void ConditionalVariable::Broadcast() {
  cond_var_.wakeAll();
}

bool ConditionalVariable::Wait(Lock& lock) {
// Disable wait recursive mutexes. Added for compatible with Qt.
// Actual Qt version (5.5) cannot support waiting on recursive mutex.
#ifndef NDEBUG
  DCHECK(!lock.is_mutex_recursive_);
  ++waiting_threads_count_;
#endif
  lock.AssertTakenAndMarkFree();
  const bool wait_status = cond_var_.wait(lock.mutex_);
  lock.AssertFreeAndMarkTaken();
#ifndef NDEBUG
  --waiting_threads_count_;
#endif
  if (!wait_status) {
    SDL_ERROR("Failed to wait for conditional variable");
    return false;
  }
  return true;
}

bool ConditionalVariable::Wait(AutoLock& auto_lock) {
  return Wait(auto_lock.GetLock());
}

ConditionalVariable::WaitStatus ConditionalVariable::WaitFor(
    AutoLock& auto_lock, uint32_t milliseconds) {
  Lock& lock = auto_lock.GetLock();
// Disable wait recursive mutexes. Added for compatible with Qt.
// Actual Qt version (5.5) cannot support waiting on recursive mutex.
#ifndef NDEBUG
  DCHECK(!lock.is_mutex_recursive_);
  ++waiting_threads_count_;
#endif
  lock.AssertTakenAndMarkFree();
  const bool timedwait_status = cond_var_.wait(lock.mutex_, milliseconds);
  lock.AssertFreeAndMarkTaken();
#ifndef NDEBUG
  --waiting_threads_count_;
#endif
  if (timedwait_status) {
    return kNoTimeout;
  }
  return kTimeout;
}

}  // namespace sync_primitives
