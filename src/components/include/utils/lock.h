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
#ifndef SRC_COMPONENTS_INCLUDE_UTILS_LOCK_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_LOCK_H_

#include <stdint.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <iostream>
#include "utils/atomic.h"
#include "utils/macro.h"
#include "utils/memory_barrier.h"

using boost::system::error_code;

namespace sync_primitives {

class SpinMutex {
 public:
  SpinMutex() : state_(0) {}
  void Lock() {
    // Comment below add exception for lint error
    // Reason: FlexeLint doesn't know about compiler's built-in instructions
    /*lint -e1055*/
    if (atomic_post_set(&state_) == 0) {
      return;
    }
    for (;;) {
      sched_yield();
      /*lint -e1055*/
      if (state_ == 0 && atomic_post_set(&state_) == 0) {
        return;
      }
    }
  }
  void Unlock() {
    state_ = 0;
  }
  ~SpinMutex() {}

 private:
  volatile unsigned int state_;
};

/* Abstract base class that allows AutoLock to handle both recursive and
 * non-recursive locks
 */
class BaseLock {
 public:
  BaseLock() {}
  virtual ~BaseLock() {}
  // Ackquire the lock. Must be called only once on a thread.
  // Please consider using AutoLock to capture it.
  virtual void Acquire() = 0;
  // Release the lock. Must be called only once on a thread after lock.
  // was acquired. Please consider using AutoLock to automatically release
  // the lock
  virtual void Release() = 0;
  // Try if lock can be captured and lock it if it was possible.
  // If it captured, lock must be manually released calling to Release
  // when protected resource access was finished.
  // @returns wether lock was captured.
  virtual bool Try() = 0;

 protected:
  // Ensures safety in locking
  virtual void AssertTakenAndMarkFree() = 0;
  virtual void AssertFreeAndMarkTaken() = 0;

  friend class ConditionalVariable;
};

/*
 * Platform-indepenednt NON-RECURSIVE lock (mutex) wrapper
 */
class Lock : public BaseLock {
 public:
  Lock();
  ~Lock();

  virtual void Acquire();

  virtual void Release();

  virtual bool Try();

 private:
  /**
   * @brief Basic debugging aid, a flag that signals wether this lock is
   * currently taken
   * Allows detection of abandoned and recursively captured mutexes
   */
  uint32_t lock_taken_;
  virtual void AssertTakenAndMarkFree();
  virtual void AssertFreeAndMarkTaken();
  boost::mutex mutex_;
  DISALLOW_COPY_AND_ASSIGN(Lock);
  friend class ConditionalVariable;
};

/*
 * Platform-indepenednt RECURSIVE lock (mutex) wrapper
 */
class RecursiveLock : public BaseLock {
 public:
  RecursiveLock();
  ~RecursiveLock();

  virtual void Acquire();

  virtual void Release();

  virtual bool Try();

 private:
  /**
   * @brief Basic debugging aid, a flag that signals wether this lock is
   * currently taken
   * Allows detection of abandoned and recursively captured mutexes
   */
  uint32_t lock_taken_;
  virtual void AssertTakenAndMarkFree();
  virtual void AssertFreeAndMarkTaken();
  boost::recursive_mutex mutex_;
  DISALLOW_COPY_AND_ASSIGN(RecursiveLock);
  friend class ConditionalVariable;
};

// This class is used to automatically acquire and release the a lock
class AutoLock {
 public:
  explicit AutoLock(BaseLock& lock) : lock_(lock) {
    // std::cerr << "lock is at " << &lock << std::endl;
    lock_.Acquire();
  }
  ~AutoLock() {
    lock_.Release();
  }

 private:
  BaseLock& GetLock() {
    return lock_;
  }
  BaseLock& lock_;

 private:
  friend class AutoUnlock;
  friend class ConditionalVariable;
  DISALLOW_COPY_AND_ASSIGN(AutoLock);
};
/*   Please use AutoLock to ackquire and (automatically) release it
 * It eases balancing of multple lock taking/releasing and makes it
 * Impossible to forget to release the lock:
 *   ...
 *   ConcurentlyAccessedData data_;
 *   sync_primitives::Lock data_lock_;
 *   ...
 * {
 *   sync_primitives::AutoLock auto_lock(data_lock_);
 *   data_.ReadOrWriteData();
 * } // lock is automatically released here
 */
// This class is used to temporarly unlock autolocked lock
class AutoUnlock {
 public:
  explicit AutoUnlock(BaseLock& lock) : lock_(lock) {
    lock_.Release();
  }
  explicit AutoUnlock(AutoLock& lock) : lock_(lock.GetLock()) {
    lock_.Release();
  }
  ~AutoUnlock() {
    lock_.Acquire();
  }

 private:
  BaseLock& lock_;

 private:
  DISALLOW_COPY_AND_ASSIGN(AutoUnlock);
};
}  // namespace sync_primitives
#endif  // SRC_COMPONENTS_INCLUDE_UTILS_LOCK_H_
