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

#if defined(OS_POSIX)
#include <pthread.h>
#else
#error Please implement lock for your OS
#endif
#include <stdint.h>
#include "utils/macro.h"

namespace sync_primitives {

namespace impl {
#if defined(OS_POSIX)
typedef pthread_mutex_t PlatformMutex;
#endif
} // namespace impl

/* Platform-indepenednt NON-RECURSIVE lock (mutex) wrapper
   Please use AutoLock to ackquire and (automatically) release it
   It eases balancing of multple lock taking/releasing and makes it
   Impossible to forget to release the lock:
   ...
   ConcurentlyAccessedData data_;
   sync_primitives::Lock data_lock_;
   ...
   {
     sync_primitives::AutoLock auto_lock(data_lock_);
     data_.ReadOrWriteData();
   } // lock is automatically released here
*/
class Lock {
 public:
  Lock();
  Lock(bool is_mutex_recursive);
  ~Lock();

  // Ackquire the lock. Must be called only once on a thread.
  // Please consider using AutoLock to capture it.
  void Acquire();
  // Release the lock. Must be called only once on a thread after lock.
  // was acquired. Please consider using AutoLock to automatically release
  // the lock
  void Release();
  // Try if lock can be captured and lock it if it was possible.
  // If it captured, lock must be manually released calling to Release
  // when protected resource access was finished.
  // @returns wether lock was captured.
  bool Try();

 private:
  impl::PlatformMutex mutex_;

#ifndef NDEBUG
  /**
  * @brief Basic debugging aid, a flag that signals wether this lock is currently taken
  * Allows detection of abandoned and recursively captured mutexes
  */
  uint32_t lock_taken_;

  /**
  * @brief Describe if mutex is recurcive or not
  */
  bool is_mutex_recursive_;

  void AssertFreeAndMarkTaken();
  void AssertTakenAndMarkFree();
#else
  void AssertFreeAndMarkTaken() {}
  void AssertTakenAndMarkFree() {}
#endif


  friend class ConditionalVariable;
  DISALLOW_COPY_AND_ASSIGN(Lock);
};

// This class is used to automatically acquire and release the a lock
class AutoLock {
 public:
  explicit AutoLock(Lock& lock)
    : lock_(lock) { lock_.Acquire(); }
  ~AutoLock()     { lock_.Release();  }
 private:
  Lock& GetLock(){ return lock_;     }
  Lock& lock_;

 private:
  friend class AutoUnlock;
  friend class ConditionalVariable;
  DISALLOW_COPY_AND_ASSIGN(AutoLock);
};

// This class is used to temporarly unlock autolocked lock
class AutoUnlock {
 public:
  explicit AutoUnlock(AutoLock& lock)
    : lock_(lock.GetLock()) { lock_.Release(); }
  ~AutoUnlock()             { lock_.Acquire();  }
 private:
  Lock& lock_;

 private:
  DISALLOW_COPY_AND_ASSIGN(AutoUnlock);
};
}  // namespace sync_primitives
#endif  // SRC_COMPONENTS_INCLUDE_UTILS_LOCK_H_
