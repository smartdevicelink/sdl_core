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

#include "LoggerHelper.hpp"

#include "utils/threads/thread.h"
#include "utils/macro.h"

namespace {
static void* threadFunc(void* closure) {
  threads::ThreadDelegate* delegate =
      static_cast<threads::ThreadDelegate*>(closure);
  delegate->threadMain();
  return NULL;
}
}

namespace threads {
size_t Thread::kMinStackSize = PTHREAD_STACK_MIN;
log4cplus::Logger Thread::logger_ = log4cplus::Logger::getInstance(
    LOG4CPLUS_TEXT("threads::Thread"));

Thread::Thread(const char* name, ThreadDelegate* delegate)
    : name_("undefined"),
      delegate_(delegate),
      thread_handle_(0),
      thread_id_(0),
      thread_options_() {
  if (name) {
    name_ = name;
  }
}

Thread::~Thread() {
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
  pthread_attr_init(&attributes);

  if (!thread_options_.is_joinable()) {
    pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
  }

  // 0 - default value
  if (thread_options_.stack_size() > 0
      && thread_options_.stack_size() >= Thread::kMinStackSize)
    pthread_attr_setstacksize(&attributes, thread_options_.stack_size());

  success = !pthread_create(&thread_handle_, &attributes, threadFunc,
                            delegate_);

  pthread_attr_destroy(&attributes);
  return success;
}

void Thread::stop() {
  if (!is_running()) {
    return;
  }

  pthread_cancel(thread_handle_);

  // Wait for the thread to exit.  It should already have terminated but make
  // sure this assumption is valid.
  join();
}

void Thread::join() {
  pthread_join(thread_handle_, NULL);
}
}  // namespace threads
