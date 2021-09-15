﻿/*
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

#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <cstring>
#include "utils/lock.h"
#include "utils/logger.h"

namespace sync_primitives {

SDL_CREATE_LOG_VARIABLE("Utils")

Lock::Lock() : lock_taken_(0) {}

Lock::~Lock() {
  if (lock_taken_ > 0) {
    SDL_LOG_FATAL("Destroying non-released regular mutex " << &mutex_);
  }
}

void Lock::Acquire() {
  mutex_.lock();
  AssertFreeAndMarkTaken();
}

void Lock::Release() {
  AssertTakenAndMarkFree();
  mutex_.unlock();
}

bool Lock::Try() {
  bool status = mutex_.try_lock();
  if (status) {
    AssertFreeAndMarkTaken();
  }
  return status;
}

void Lock::AssertFreeAndMarkTaken() {
  if (lock_taken_ != 0) {
    SDL_LOG_FATAL("Locking already taken not recursive mutex");
    NOTREACHED();
  }
  ++lock_taken_;
}

void Lock::AssertTakenAndMarkFree() {
  if (lock_taken_ == 0) {
    SDL_LOG_FATAL("Unlocking a mutex that is not taken");
    NOTREACHED();
  }
  --lock_taken_;
}

// Recursive lock looks the same on the surface, some code duplication is
// necessary since they don't have a shared parent superclass
RecursiveLock::RecursiveLock() : lock_taken_(0) {}

RecursiveLock::~RecursiveLock() {
  if (lock_taken_ > 0) {
    SDL_LOG_FATAL("Destroying non-released recursive mutex " << &mutex_);
  }
}

void RecursiveLock::Acquire() {
  mutex_.lock();
  AssertFreeAndMarkTaken();
}

void RecursiveLock::Release() {
  AssertTakenAndMarkFree();
  mutex_.unlock();
}

bool RecursiveLock::Try() {
  bool status = mutex_.try_lock();
  if (status) {
    AssertFreeAndMarkTaken();
  }
  return status;
}

void RecursiveLock::AssertFreeAndMarkTaken() {
  ++lock_taken_;
}

void RecursiveLock::AssertTakenAndMarkFree() {
  if (lock_taken_ == 0) {
    SDL_LOG_FATAL("Unlocking a recursive mutex that is not taken");
    NOTREACHED();
  }
  --lock_taken_;
}

}  // namespace sync_primitives
