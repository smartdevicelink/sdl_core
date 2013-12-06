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

#include "utils/lock.h"

#include "utils/logger.h"

namespace {
log4cxx::LoggerPtr g_logger =
    log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Utils"));
}

namespace sync_primitives {

Lock::Lock()
#ifndef NDEBUG
      : lock_taken_(false)
#endif // NDEBUG
{
  int status = pthread_mutex_init(&mutex_, NULL);
  if (status != 0) {
    LOG4CXX_ERROR(g_logger, "Failed to initialize mutex");
  }
}

Lock::~Lock() {
#ifndef NDEBUG
  if (lock_taken_) {
    LOG4CXX_ERROR(g_logger, "Destroying non-released mutex");
  }
#endif
  int status = pthread_mutex_destroy(&mutex_);
  if (status != 0) {
    LOG4CXX_ERROR(g_logger, "Failed to destroy mutex");
  }
}

void Lock::Ackquire() {
  int status = pthread_mutex_lock(&mutex_);
  if (status != 0) {
    LOG4CXX_ERROR(g_logger, "Failed to acquire mutex");
  }
  AssertFreeAndMarkTaken();
}

void Lock::Release() {
  AssertTakenAndMarkFree();
  int status = pthread_mutex_unlock(&mutex_);
  if (status != 0) {
    LOG4CXX_ERROR(g_logger, "Failed to unlock mutex");
  }
}

bool Lock::Try() {
  bool ackquired = false;
#ifndef NDEBUG
  if (lock_taken_) {
    LOG4CXX_ERROR(g_logger, "Trying to lock already taken mutex");
  }
#endif
  switch(pthread_mutex_trylock(&mutex_)) {
    case 0: {
      ackquired = true;
#ifndef NDEBUG
      lock_taken_ = true;
#endif
    } break;
    case EBUSY: {
      ackquired = false;
    } break;
    default: {
      ackquired = false;
      LOG4CXX_ERROR(g_logger, "Failed to try lock the mutex");
    }
  }
  return ackquired;
}

#ifndef NDEBUG
void Lock::AssertFreeAndMarkTaken() {
  if (lock_taken_) {
    LOG4CXX_ERROR(g_logger, "Locking already taken mutex");
  }
  lock_taken_ = true;
}
void Lock::AssertTakenAndMarkFree() {
  if (!lock_taken_) {
    LOG4CXX_ERROR(g_logger, "Unlocking a mutex that is not taken");
  }
  lock_taken_ = false;
}
#endif


} // namespace sync_primitives
