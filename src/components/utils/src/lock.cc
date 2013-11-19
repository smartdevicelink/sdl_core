#include "utils/lock.h"
#include "utils/logger.h"


namespace {
log4cxx::LoggerPtr g_logger =
    log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Utils"));
}

Lock::Lock()
  : lock_taken_(false) {
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
    LOG4CXX_ERROR(g_logger, "Failed acquire mutex");
  }
#ifndef NDEBUG
  lock_taken_ = true;
#endif
}

void Lock::Release() {
#ifndef NDEBUG
  lock_taken_ = false;
#endif
  int status = pthread_mutex_unlock(&mutex_);
}

bool Lock::Try() {
  bool ackquired = false;
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
      LOG4CXX_ERROR(g_logger, "Failed to try lock mutex");
    }
  }
  return ackquired;
}

