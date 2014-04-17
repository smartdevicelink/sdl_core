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

#include <limits.h>
#include <stddef.h>

#include "utils/threads/thread.h"
#include "utils/threads/thread_manager.h"

using namespace std;
using namespace threads::impl;

namespace {
static void* threadFunc(void* closure) {
  threads::ThreadDelegate* delegate =
    static_cast<threads::ThreadDelegate*>(closure);
  delegate->threadMain();
  return NULL;
}
}

namespace threads {
size_t Thread::kMinStackSize = PTHREAD_STACK_MIN; /* Ubuntu : 16384 ; QNX : 256; */

GETLOGGER(Thread::logger_, "threads::Thread")

bool Thread::Id::operator==(const Thread::Id other) const {
  return pthread_equal(id_, other.id_) != 0;
}

// static
Thread::Id Thread::CurrentId() {
  return Id(pthread_self());
}

//static
std::string Thread::NameFromId(Id thread_id) {
  return ThreadManager::instance()->GetName(thread_id.id_);
}

//static
void Thread::SetNameForId(Id thread_id, const std::string& name) {
  ThreadManager::instance()->RegisterName(thread_id.id_, name);
}


Thread::Thread(const char* name, ThreadDelegate* delegate)
  : name_("undefined"),
    delegate_(delegate),
    thread_handle_(0),
    thread_options_(),
    isThreadRunning_(false) {
  if (name) {
    name_ = name;
  }
}

Thread::~Thread() {
  ThreadManager::instance()->Unregister(thread_handle_);
  if (delegate_) {
    delete delegate_;
  }
}

bool Thread::start() {
  return startWithOptions(thread_options_);
}

bool Thread::startWithOptions(const ThreadOptions& options) {
  if (!delegate_) {
    NOTREACHED();
    return false;
  }

  thread_options_ = options;

  bool success = false;
  pthread_attr_t attributes;
  int pthread_result = pthread_attr_init(&attributes);
  if (pthread_result != 0) {
    LOG4CXX_INFO(logger_,"Couldn't init pthread attributes."
                 " Error code = " << pthread_result);
  }
  if (!thread_options_.is_joinable()) {
    pthread_result = pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
    if (pthread_result != 0) {
      LOG4CXX_INFO(logger_,"Couldn't set detach state attribute. Error code = " << pthread_result);
    }
  }

  // 0 - default value
  if (thread_options_.stack_size() > 0
      && thread_options_.stack_size() >= Thread::kMinStackSize) {
    pthread_result = pthread_attr_setstacksize(&attributes, thread_options_.stack_size());
    if (pthread_result != 0) {
      LOG4CXX_INFO(logger_,"Couldn't set stacksize = " << thread_options_.stack_size() <<
                   "Error code = " << pthread_result);
    }
  }
  success = !pthread_create(&thread_handle_, &attributes, threadFunc,
                            delegate_);
  if (success) {
    pthread_result = pthread_setname_np(thread_handle_, name_.c_str());
#   ifdef __QNXNTO__
    if (pthread_result != EOK) {
      LOG4CXX_INFO(logger_,"Couldn't set pthread name"
                   " Error code = " << pthread_result);
    }
#   endif
    LOG4CXX_INFO(logger_,"Created thread: " << name_);
    ThreadManager::instance()->RegisterName(thread_handle_, name_);
  }

  isThreadRunning_ = success;

  pthread_result = pthread_attr_destroy(&attributes);
  if (pthread_result != 0) {
    LOG4CXX_INFO(logger_,"Couldn't destroy pthread attributes."
                 " Error code = " << pthread_result);
  }
  return success;
}

void Thread::stop() {
  if (!is_running()) {
    return;
  }

  if (NULL != delegate_) {
    if (!delegate_->exitThreadMain()) {
      if (thread_handle_ != pthread_self()) {
        pthread_cancel(thread_handle_);
      }
    }
  }

  // Wait for the thread to exit.  It should already have terminated but make
  // sure this assumption is valid.

  join();
}

void Thread::join() {
  int pthread_result = pthread_join(thread_handle_, NULL);
  if (pthread_result != 0) {
    LOG4CXX_INFO(logger_,"Couldn't join thread "
                 " Error code = " << pthread_result);
  }
  isThreadRunning_ = false;
}

std::ostream& operator<<(std::ostream& os, Thread::Id thread_id) {
  return os<<Thread::NameFromId(thread_id);
}

}  // namespace threads
