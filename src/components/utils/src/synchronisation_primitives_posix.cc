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

#include "utils/macro.h"
#include "utils/synchronisation_primitives.h"

namespace sync_primitives {

SynchronisationPrimitives::SynchronisationPrimitives()
    : mutex_(PTHREAD_MUTEX_INITIALIZER),
      cond_variable_(PTHREAD_COND_INITIALIZER) {
}

SynchronisationPrimitives::~SynchronisationPrimitives() {
  pthread_cond_destroy(&cond_variable_);
  pthread_mutex_destroy(&mutex_);
}

void SynchronisationPrimitives::init() {
  pthread_mutex_init(&mutex_, NULL);

  pthread_condattr_t condattr;
  pthread_condattr_init(&condattr);
  pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
  pthread_cond_init(&cond_variable_, &condattr);
}

void SynchronisationPrimitives::lock() {
  pthread_mutex_lock(&mutex_);
}

void SynchronisationPrimitives::unlock() {
  pthread_mutex_unlock(&mutex_);
}

void SynchronisationPrimitives::wait() {
  pthread_cond_wait(&cond_variable_, &mutex_);
}

SynchronisationPrimitives::WaitStatus SynchronisationPrimitives::timedwait(
    time_t sec, uint32_t nsec) {
  static const uint32_t NANOSECONDS_IN_SECOND = 1000000000;
  timespec tm;
  clock_gettime(CLOCK_MONOTONIC, &tm);
  tm.tv_sec += sec;
  tm.tv_nsec += nsec;
  if (tm.tv_nsec >= NANOSECONDS_IN_SECOND) {
    tm.tv_nsec -= NANOSECONDS_IN_SECOND;
    tm.tv_sec += 1;
  }
  const int rc = pthread_cond_timedwait(&cond_variable_, &mutex_, &tm);
  switch (rc) {
    case 0:
      return SIGNALED;
    case ETIMEDOUT:
      return TIMED_OUT;
    default:
      return FAILED;
  }
}

void SynchronisationPrimitives::signal() {
  pthread_cond_signal(&cond_variable_);
}

PlatformMutex& SynchronisationPrimitives::mutex() {
  return mutex_;
}

PlatformConditionalVar& SynchronisationPrimitives::conditional_var() {
  return cond_variable_;
}

}  // namespace sync_primitives
