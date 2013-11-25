#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOCK_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOCK_H_

#if defined(OS_POSIX)
#include <pthread.h>
#else
#error Please implement lock for your OS
#endif

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
  ~Lock();

  // Ackquire the lock. Must be called only once on a thread.
  // Please consider using AutoLock to capture it.
  void Ackquire();
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
  // Basic debugging aid, a flag that signals wether this lock is currently taken
  // Allows detection of abandoned and recursively captured mutexes
  bool lock_taken_;
  void AssertFreeAndMarkTaken();
  void AssertTakenAndMarkFree();
#else
  void AssertFreeAndMarkTaken() {}
  void AssertTakenAndMarkFree() {}
#endif

 private:
  friend class ConditionalVariable;
  DISALLOW_COPY_AND_ASSIGN(Lock);
};

// This class is used to automatically acquire and release the a lock
class AutoLock {
 public:
  explicit AutoLock(Lock& lock)
    : lock_(lock) { lock_.Ackquire(); }
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
  ~AutoUnlock()             { lock_.Ackquire();  }
 private:
  Lock& lock_;

 private:
  DISALLOW_COPY_AND_ASSIGN(AutoUnlock);
};

} // sync_primitives

#endif // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_LOCK_H_
