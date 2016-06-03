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
#include <pthread.h>

#include "utils/rwlock.h"
#include "utils/logger.h"
#include "utils/pimpl_impl.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

namespace sync_primitives {

class RWLock::Impl {
 public:
  Impl();
  ~Impl();

  void AcquireForReading();
  bool TryAcquireForReading();
  void AcquireForWriting();
  bool TryAcquireForWriting();
  void ReleaseForReading();
  void ReleaseForWriting();

 private:
  pthread_rwlock_t rwlock_;
};

}  // namespace sync_primitives

sync_primitives::RWLock::RWLock() {}

sync_primitives::RWLock::~RWLock() {}

void sync_primitives::RWLock::AcquireForReading() {
  impl_->AcquireForReading();
}

bool sync_primitives::RWLock::TryAcquireForReading() {
  return impl_->TryAcquireForReading();
}

void sync_primitives::RWLock::AcquireForWriting() {
  impl_->AcquireForWriting();
}

bool sync_primitives::RWLock::TryAcquireForWriting() {
  return impl_->TryAcquireForWriting();
}

void sync_primitives::RWLock::ReleaseForReading() {
  impl_->ReleaseForReading();
}

void sync_primitives::RWLock::ReleaseForWriting() {
  impl_->ReleaseForWriting();
}

sync_primitives::RWLock::Impl::Impl() {
  if (pthread_rwlock_init(&rwlock_, 0) != 0) {
    LOGGER_ERROR(logger_, "Failed to initialize rwlock");
  }
}

sync_primitives::RWLock::Impl::~Impl() {
  if (pthread_rwlock_destroy(&rwlock_) != 0) {
    LOGGER_ERROR(logger_, "Failed to destroy rwlock");
  }
}

void sync_primitives::RWLock::Impl::AcquireForReading() {
  if (pthread_rwlock_rdlock(&rwlock_) != 0) {
    LOGGER_ERROR(logger_, "Failed to acquire rwlock for reading");
  }
}

bool sync_primitives::RWLock::Impl::TryAcquireForReading() {
  if (pthread_rwlock_tryrdlock(&rwlock_) != 0) {
    LOGGER_DEBUG(logger_, "Cannot acquire rwlock for reading");
    return false;
  }
  return true;
}

void sync_primitives::RWLock::Impl::AcquireForWriting() {
  if (pthread_rwlock_wrlock(&rwlock_) != 0) {
    LOGGER_ERROR(logger_, "Failed to acquire rwlock for writing");
  }
}

bool sync_primitives::RWLock::Impl::TryAcquireForWriting() {
  if (pthread_rwlock_trywrlock(&rwlock_) != 0) {
    LOGGER_DEBUG(logger_, "Cannot acquire rwlock for writing");
    return false;
  }
  return true;
}

void sync_primitives::RWLock::Impl::ReleaseForReading() {
  if (pthread_rwlock_unlock(&rwlock_) != 0) {
    LOGGER_ERROR(logger_, "Failed to release rwlock for reading");
  }
}

void sync_primitives::RWLock::Impl::ReleaseForWriting() {
  if (pthread_rwlock_unlock(&rwlock_) != 0) {
    LOGGER_ERROR(logger_, "Failed to release rwlock for writing");
  }
}
