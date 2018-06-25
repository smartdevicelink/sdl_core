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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_RWLOCK_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_RWLOCK_H_

#include <boost/thread/shared_mutex.hpp>
#include "utils/macro.h"
namespace sync_primitives {

/**
 * RW locks wrapper
 * Read-write locks permit concurrent reads and exclusive writes to a protected
 * shared resource. The read-write lock is a single entity that can be locked in
 * read or write mode. To modify a resource, a thread must first acquire the
 * exclusive write lock. An exclusive write lock is not permitted until all read
 * locks have been released.
 *
 * Current implementation is a minimal wrapper of Boost shared_lock
 * https://www.boost.org/doc/libs/1_67_0/doc/html/thread/synchronization.html#thread.synchronization.mutex_types.shared_mutex
 */

class RWLock {
 public:
  RWLock();
  ~RWLock();

  /**
   * @brief Acquire read-write lock for reading.
   * The calling thread acquires the read lock if a writer does not
   * hold the lock and there are no writers blocked on the lock.
   * It is unspecified whether the calling thread acquires the lock
   * when a writer does not hold the lock and there are writers waiting for the
   * lock.
   * If a writer holds the lock, the calling thread will not acquire the read
   * lock.
   * If the read lock is not acquired, the calling thread blocks
   * (that is, it does not return from the AcquireForReading()) until it can
   * acquire the lock.
   * Results are undefined if the calling thread holds a write lock on rwlock at
   * the time the call is made.
   * A thread can hold multiple concurrent read locks on rwlock
   * (that is, successfully call AcquireForReading() n times)
   * If so, the thread must perform matching unlocks (that is, it must call
   * Release() n times).
   * @returns true if lock was acquired and false if was not
   */
  bool AcquireForReading();

  /**
   * @brief try to Acquire read-write lock for reading.
   * @returns true if lock was acquired and false if was not
   */
  bool TryAcquireForReading();

  /**
   * @brief Acquire read-write lock for writing.
   * Applies a write lock to the read-write lock.
   * The calling thread acquires the write lock if no other thread (reader or
   * writer) holds the read-write lock rwlock. Otherwise, the thread blocks
   * (that is, does not return from the AcquireForWriting() call)
   * until it can acquire the lock. Results are undefined if the calling thread
   * holds the read-write lock (whether a read or write lock) at the time the
   * call is made. The thread must perform matching unlock (that is, it must
   * call Release()).
   * @returns true if lock was acquired and false if was not
   */
  bool AcquireForWriting();

  /**
   * @brief Try to Acquire read-write lock for writing.
   * @returns true if lock was acquired and false if was not
   */
  bool TryAcquireForWriting();

  /**
   * @brief Release read-write lock.
   * Releases a lock held on the read-write lock object.
   * Results are undefined if the read-write lock rwlock
   * is not held by the calling thread.
   * @returns true if lock was released and false if was not
   */
  bool Release();

 private:
  DISALLOW_COPY_AND_ASSIGN(RWLock);
  boost::shared_mutex rwmutex_;
  bool write_locked_;
};

/**
 * @brief Makes auto lock read-write locks for reading
 * Please use AutoReadLock to acquire for reading and (automatically) release it
 */
class AutoReadLock {
 public:
  explicit AutoReadLock(RWLock& rwlock) : rwlock_(rwlock) {
    rwlock_.AcquireForReading();
  }
  ~AutoReadLock() {
    rwlock_.Release();
  }

 private:
  RWLock& rwlock_;
  DISALLOW_COPY_AND_ASSIGN(AutoReadLock);
};

/**
 * @brief Makes auto lock read-write locks for writing
 * Please use AutoWriteLock to acquire for writing and (automatically) release
 * it
 */
class AutoWriteLock {
 public:
  explicit AutoWriteLock(RWLock& rwlock) : rwlock_(rwlock) {
    rwlock_.AcquireForWriting();
  }
  ~AutoWriteLock() {
    rwlock_.Release();
  }

 private:
  RWLock& rwlock_;
  DISALLOW_COPY_AND_ASSIGN(AutoWriteLock);
};

}  // namespace sync_primitives

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_RWLOCK_H_
