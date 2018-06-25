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

#include "utils/logger.h"
#include "utils/rwlock.h"

namespace sync_primitives {

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

RWLock::RWLock() {
  // start not write_locked
  write_locked_ = false;
}

RWLock::~RWLock() {}

bool RWLock::AcquireForReading() {
  try {
    rwmutex_.lock_shared();
  } catch (boost::lock_error) {
    // couldn't lock, return false
    LOG4CXX_ERROR(logger_, "Failed to acquire rwlock for reading");
    return false;
  }
  return true;
}

bool RWLock::TryAcquireForReading() {
  try {
    bool result = rwmutex_.try_lock_shared();
    if (result == false) {
      LOG4CXX_ERROR(logger_, "Failed to acquire rwlock for reading");
    }
    return result;
  } catch (boost::lock_error) {
    // couldn't lock, return false
    LOG4CXX_ERROR(logger_, "Failed to acquire rwlock for reading");
    return false;
  }
}

bool RWLock::AcquireForWriting() {
  // get exclusive lock
  try {
    rwmutex_.lock();
  } catch (boost::lock_error) {
    // couldn't lock, return false
    LOG4CXX_ERROR(logger_, "Failed to acquire rwlock for writing");
    return false;
  }
  // we now have an exclusive lock
  write_locked_ = true; 
  return true;
}

bool RWLock::TryAcquireForWriting() {
  try {
    bool result = rwmutex_.try_lock();
    if (result == false) {
      LOG4CXX_ERROR(logger_, "Failed to acquire rwlock for writing");
    } else {
      write_locked_ = true;
    }
    return result;
  } catch (boost::lock_error) {
    // couldn't lock, return false
    LOG4CXX_ERROR(logger_, "Failed to acquire rwlock for writing");
    return false;
  }
}

bool RWLock::Release() {
  if (write_locked_) {
    // If we have exclusive access, release it
    write_locked_ = false;
    rwmutex_.unlock();
    // lock is now free
  } else {
    // Otherwise release our shared access
    rwmutex_.unlock_shared();
  }
  return true;
}

}  // namespace sync_primitives
