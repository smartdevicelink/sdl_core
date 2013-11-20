#include <pthread.h>

// Platform-indepenednt (in future) lock wrapper
class Lock {
 public:
  Lock();
  ~Lock();

  void Ackquire();
  void Release();
  bool Try();

 private:
  pthread_mutex_t mutex_;

#ifndef NDEBUG
  bool lock_taken_;
#endif

 private:
  Lock(const Lock&);
  void operator=(const Lock&);
};

// This class is used to automatically acquire and release the a lock
class AutoLock {
 public:
  explicit AutoLock(Lock& lock)
   : lock_(lock) { lock_.Ackquire(); }
  ~AutoLock()    { lock_.Release();  }
 private:
  Lock& lock_;

 private:
  AutoLock(const AutoLock&);
  void operator=(const AutoLock&);
};

