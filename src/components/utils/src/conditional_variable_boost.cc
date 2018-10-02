/*
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
#include "utils/conditional_variable.h"

#include <errno.h>
#include <time.h>

#include "utils/lock.h"
#include "utils/logger.h"
#include <boost/exception/diagnostic_information.hpp>

namespace {
const long kNanosecondsPerSecond = 1000000000;
const long kMillisecondsPerSecond = 1000;
const long kNanosecondsPerMillisecond = 1000000;
}  // namespace

namespace sync_primitives {

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

ConditionalVariable::ConditionalVariable() {}

ConditionalVariable::~ConditionalVariable() {}

void ConditionalVariable::NotifyOne() {
  cond_var_.notify_one();
}

void ConditionalVariable::Broadcast() {
  cond_var_.notify_all();
}

bool ConditionalVariable::Wait(BaseLock& lock) {
  // NOTE this grossness is due to boost mutex and recursive mutex not sharing a
  // superclass
  try {
    lock.AssertTakenAndMarkFree();
    // What kind of lock are we ?
    if (Lock* test_lock = dynamic_cast<Lock*>(&lock)) {
      // Regular lock
      cond_var_.wait<boost::mutex>(test_lock->mutex_);
    } else if (RecursiveLock* test_rec_lock =
                   dynamic_cast<RecursiveLock*>(&lock)) {
      // Recursive lock
      cond_var_.wait<boost::recursive_mutex>(test_rec_lock->mutex_);
    } else {
      // unknown, give up the lock
      LOG4CXX_ERROR(logger_, "Unknown lock type!");
      NOTREACHED();
    }
    lock.AssertFreeAndMarkTaken();
  } catch (const boost::exception& error) {
    std::string error_string = boost::diagnostic_information(error);
    LOG4CXX_FATAL(logger_, error_string);
    NOTREACHED();
  }
  return true;
}

bool ConditionalVariable::Wait(AutoLock& auto_lock) {
  BaseLock& lock = auto_lock.GetLock();
  return Wait(lock);
}

ConditionalVariable::WaitStatus ConditionalVariable::WaitFor(
    AutoLock& auto_lock, uint32_t milliseconds) {
  BaseLock& lock = auto_lock.GetLock();

  WaitStatus wait_status = kNoTimeout;
  try {
    lock.AssertTakenAndMarkFree();
    bool timeout = true;

    // What kind of lock are we ?
    if (Lock* test_lock = dynamic_cast<Lock*>(&lock)) {
      // Regular lock
      // cond_var_.wait<boost::mutex>(test_lock->mutex_);
      timeout = cond_var_.timed_wait<boost::mutex>(
          test_lock->mutex_, boost::posix_time::milliseconds(milliseconds));
    } else if (RecursiveLock* test_rec_lock =
                   dynamic_cast<RecursiveLock*>(&lock)) {
      // Recursive lock
      // cond_var_.wait<boost::recursive_mutex>(test_rec_lock->mutex_);
      timeout = cond_var_.timed_wait<boost::recursive_mutex>(
          test_rec_lock->mutex_, boost::posix_time::milliseconds(milliseconds));
    } else {
      // this is an unknown lock, we have an issue
      LOG4CXX_ERROR(logger_, "Unknown lock type!");
      NOTREACHED();
    }

    if (!timeout) {
      wait_status = kTimeout;
    }
    lock.AssertFreeAndMarkTaken();
  } catch (const boost::exception& error) {
    std::string error_string = boost::diagnostic_information(error);
    LOG4CXX_FATAL(logger_, error_string);
    NOTREACHED();
  }

  return wait_status;
}

}  // namespace sync_primitives
