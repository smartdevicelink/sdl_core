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
#if defined(OS_WINDOWS)

#include "utils/winhdr.h"
#include "utils/conditional_variable.h"
#include "utils/lock.h"
#include "utils/logger.h"

namespace sync_primitives {

SDL_CREATE_LOGGER("Utils")

ConditionalVariable::ConditionalVariable() {
  InitializeConditionVariable(&cond_var_);
}

ConditionalVariable::~ConditionalVariable() {}

void ConditionalVariable::NotifyOne() {
  WakeConditionVariable(&cond_var_);
}

void ConditionalVariable::Broadcast() {
  WakeAllConditionVariable(&cond_var_);
}

bool ConditionalVariable::Wait(Lock& lock) {
// Disable wait recursive mutexes. Added for compatible with Qt.
// Actual Qt version (5.5) cannot support waiting on recursive mutex.
#ifndef NDEBUG
  DCHECK(!lock.is_mutex_recursive_);
#endif
  lock.AssertTakenAndMarkFree();
  const BOOL wait_status =
      SleepConditionVariableCS(&cond_var_, &lock.mutex_, INFINITE);
  lock.AssertFreeAndMarkTaken();
  if (wait_status == 0) {
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
#endif
  lock.AssertTakenAndMarkFree();
  const BOOL wait_status =
      SleepConditionVariableCS(&cond_var_, &lock.mutex_, milliseconds);
  lock.AssertFreeAndMarkTaken();
  if (wait_status == 0) {
    DWORD error_code = GetLastError();
    if (ERROR_TIMEOUT == error_code) {
      return kTimeout;
    }
  }
  return kNoTimeout;
}

}  // namespace sync_primitives

#endif  // OS_WINDOWS
