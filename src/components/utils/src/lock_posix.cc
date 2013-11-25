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
