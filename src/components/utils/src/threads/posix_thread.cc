/*
 * Copyright (c) 2014, Ford Motor Company
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

#include <errno.h>

#include <limits.h>
#include <stddef.h>
#include <signal.h>

#include "utils/atomic.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_manager.h"
#include "utils/logger.h"
#include "pthread.h"


#ifndef __QNXNTO__
  const int EOK = 0;
#endif

#if defined(OS_POSIX)
  const size_t THREAD_NAME_SIZE = 15;
#endif

namespace threads {

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

namespace {

static void* threadFunc(void* arg) {
  LOG4CXX_INFO(logger_, "Thread #" << pthread_self() << " started successfully");
  threads::Thread* thread = static_cast<threads::Thread*>(arg);
  threads::ThreadDelegate* delegate = thread->delegate();
  delegate->threadMain();
  thread->set_running(false);
  MessageQueue<ThreadManager::ThreadDesc>& threads = ::threads::ThreadManager::instance()->threads_to_terminate;
  if (!threads.IsShuttingDown()) {
    LOG4CXX_INFO(logger_, "Pushing thread #" << pthread_self() << " to join queue");
    ThreadManager::ThreadDesc desc = { pthread_self(), delegate };
    threads.push(desc);
  }
  LOG4CXX_INFO(logger_, "Thread #" << pthread_self() << " exited successfully");
  return NULL;
}

}

size_t Thread::kMinStackSize = PTHREAD_STACK_MIN; /* Ubuntu : 16384 ; QNX : 256; */

/*
void ThreadBase::set_name(const std::string name) {
  std::string trimname = name.erase(15);
  if(pthread_setname_np(thread_handle_, trimname.c_str()) != EOK) {
    LOG4CXX_WARN(logger_, "Couldn't set pthread name \""
                       << trimname
                       << "\", error code "
                       << pthread_result
                       << " ("
                       << strerror(pthread_result)
                       << ")");
  }
}
*/

void Thread::SetNameForId(const Id& thread_id, const std::string& name) {
  std::string nm = name;
  std::string& trimname = nm.size() > 15 ? nm.erase(15) : nm;
  const int rc = pthread_setname_np(thread_id.id_, trimname.c_str());
  if(rc == EOK) {
    LOG4CXX_WARN(logger_, "Couldn't set pthread name \""
                       << trimname
                       << "\", error code "
                       << rc
                       << " ("
                       << strerror(rc)
                       << ")");
  }
}

Thread::Thread(const char* name, ThreadDelegate* delegate)
  : name_(name ? name : "undefined"),
    delegate_(delegate),
    thread_handle_(0),
    thread_options_(),
    isThreadRunning_(0) {
}

ThreadDelegate* Thread::delegate() const {
  return delegate_;
}

bool Thread::start() {
  return startWithOptions(thread_options_);
}

Thread::Id Thread::CurrentId() {
  return Id(pthread_self());
}

bool Thread::startWithOptions(const ThreadOptions& options) {
  LOG4CXX_TRACE_ENTER(logger_);
  if (!delegate_) {
    NOTREACHED();
    LOG4CXX_ERROR(logger_, "NULL delegate");
    LOG4CXX_TRACE_EXIT(logger_);
    return false;
  }

  thread_options_ = options;

  pthread_attr_t attributes;
  int pthread_result = pthread_attr_init(&attributes);
  if (pthread_result != EOK) {
    LOG4CXX_WARN(logger_,"Couldn't init pthread attributes. Error code = "
                 << pthread_result << "(\"" << strerror(pthread_result) << "\")");
  }

  if (!thread_options_.is_joinable()) {
    pthread_result = pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
    if (pthread_result != EOK) {
      LOG4CXX_WARN(logger_,"Couldn't set detach state attribute.. Error code = "
                   << pthread_result << "(\"" << strerror(pthread_result) << "\")");
      thread_options_.is_joinable(false);
    }
  }

  const size_t stack_size = thread_options_.stack_size();
  if (stack_size >= Thread::kMinStackSize) {
    pthread_result = pthread_attr_setstacksize(&attributes, stack_size);
    if (pthread_result != EOK) {
      LOG4CXX_WARN(logger_,"Couldn't set stacksize = " << stack_size <<
                   ". Error code = " << pthread_result << "(\""
                   << strerror(pthread_result) << "\")");
    }
  }

  pthread_result = pthread_create(&thread_handle_, &attributes, threadFunc, this);
  isThreadRunning_ = (pthread_result == EOK);
  if (!isThreadRunning_) {
    LOG4CXX_WARN(logger_, "Couldn't create thread. Error code = "
                 << pthread_result << "(\"" << strerror(pthread_result) << "\")");
  } else {
    LOG4CXX_INFO(logger_,"Created thread: " << name_);
    SetNameForId(Id(thread_handle_), name_);
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return isThreadRunning_;
}

void Thread::stop() {
  LOG4CXX_TRACE_ENTER(logger_);

  if (!atomic_post_clr(&isThreadRunning_))
  {
    return;
  }

  if (delegate_ && !delegate_->exitThreadMain()) {
    if (thread_handle_ != pthread_self()) {
      LOG4CXX_WARN(logger_, "Cancelling thread #" << thread_handle_);
      const int pthread_result = pthread_cancel(thread_handle_);
      if (pthread_result != EOK) {
        LOG4CXX_WARN(logger_,
                     "Couldn't cancel thread (#" << thread_handle_ << " \"" << name_ <<
                     "\") from thread #" << pthread_self() << ". Error code = "
                     << pthread_result << " (\"" << strerror(pthread_result) << "\")");
      }
    } else {
      LOG4CXX_ERROR(logger_,
                    "Couldn't cancel the same thread (#" << thread_handle_
                    << "\"" << name_ << "\")");
    }
  }

  LOG4CXX_TRACE_EXIT(logger_);
}

bool Thread::Id::operator==(const Thread::Id& other) const {
  return pthread_equal(id_, other.id_) != 0;
}

std::ostream& operator<<(std::ostream& os, const Thread::Id& thread_id) {
  char name[32];
  if(pthread_getname_np(thread_id.Handle(), name, 32) == 0) {
    os << name;
  }
  return os;
}

Thread* CreateThread(const char* name, ThreadDelegate* delegate) {
  return new Thread(name, delegate);
}

void DeleteThread(Thread* thread) {
  delete thread;
}


}  // namespace threads
