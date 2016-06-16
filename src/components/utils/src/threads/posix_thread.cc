/*
 * Copyright (c) 2016, Ford Motor Company
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
#ifdef BUILD_TESTS
// Temporary fix for UnitTest until APPLINK-9987 is resolved
#include <unistd.h>
#endif

#include "utils/threads/thread.h"
#include "pthread.h"
#include "utils/atomic.h"
#include "utils/threads/thread_delegate.h"
#include "utils/logger.h"
#include <chrono>
#include <string.h>
#include <pthread.h>
#include <thread>

#ifndef __QNXNTO__
const int EOK = 0;
#endif

const size_t THREAD_NAME_SIZE = 15;

namespace threads {

SDL_CREATE_LOGGER("Utils")

void sleep(uint32_t ms) {
#if defined(SDL_CPP11)
  std::chrono::microseconds sleep_interval_mcsec(ms * 1000);
  std::this_thread::sleep_for(std::chrono::microseconds(sleep_interval_mcsec));
#else
  usleep(ms * 1000);
#endif
}

size_t Thread::kMinStackSize =
    PTHREAD_STACK_MIN; /* Ubuntu : 16384 ; QNX : 256; */

void Thread::cleanup(void* arg) {
  SDL_AUTO_TRACE();
  Thread* thread = static_cast<Thread*>(arg);
  sync_primitives::AutoLock auto_lock(thread->state_lock_);
  thread->isThreadRunning_ = false;
  thread->state_cond_.Broadcast();
}

void* Thread::threadFunc(void* arg) {
  // 0 - state_lock unlocked
  //     stopped   = 0
  //     running   = 0
  //     finalized = 0
  // 4 - state_lock unlocked
  //     stopped = 1
  //     running = 1
  //     finalized = 0
  // 5 - state_lock unlocked
  //     stopped = 1
  //     running = 1
  //     finalized = 1
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

  threads::Thread* thread = static_cast<Thread*>(arg);
  DCHECK(thread);

  pthread_cleanup_push(&cleanup, thread);

  thread->state_lock_.Acquire();
  thread->state_cond_.Broadcast();

  while (!thread->finalized_) {
    SDL_DEBUG("Thread #" << pthread_self() << " iteration");
    thread->run_cond_.Wait(thread->state_lock_);
    SDL_DEBUG("Thread #" << pthread_self() << " execute. "
                         << "stopped_ = " << thread->stopped_
                         << "; finalized_ = " << thread->finalized_);
    if (!thread->stopped_ && !thread->finalized_) {
      thread->isThreadRunning_ = true;
      pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
      pthread_testcancel();

      thread->state_lock_.Release();
      thread->delegate_->threadMain();
      thread->state_lock_.Acquire();

      pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
      thread->isThreadRunning_ = false;
    }
    thread->state_cond_.Broadcast();
    SDL_DEBUG("Thread #" << pthread_self() << " finished iteration");
  }

  thread->state_lock_.Release();
  pthread_cleanup_pop(1);

  SDL_DEBUG("Thread #" << pthread_self() << " exited successfully");
  return NULL;
}

void Thread::SetNameForId(uint64_t thread_id, std::string name) {
  if (name.size() > THREAD_NAME_SIZE)
    name.erase(THREAD_NAME_SIZE);
  const int rc =
      pthread_setname_np(static_cast<pthread_t>(thread_id), name.c_str());
  if (rc != EOK) {
    SDL_WARN("Couldn't set pthread name \"" << name << "\", error code " << rc
                                            << " (" << strerror(rc) << ")");
  }
}

Thread::Thread(const char* name, ThreadDelegate* delegate)
    : name_(name ? name : "undefined")
    , delegate_(delegate)
    , handle_(0)
    , thread_options_()
    , isThreadRunning_(false)
    , stopped_(false)
    , finalized_(false)
    , thread_created_(false) {}

bool Thread::start() {
  return start(thread_options_);
}

uint64_t Thread::CurrentId() {
  return static_cast<uint64_t>(pthread_self());
}

bool Thread::IsCurrentThread() const {
  return pthread_equal(static_cast<pthread_t>(CurrentId()), thread_handle());
}

bool Thread::start(const ThreadOptions& options) {
  SDL_AUTO_TRACE();

  sync_primitives::AutoLock auto_lock(state_lock_);
  // 1 - state_lock locked
  //     stopped = 0
  //     running = 0

  if (!delegate_) {
    SDL_ERROR("Cannot start thread " << name_ << ": delegate is NULL");
    // 0 - state_lock unlocked
    return false;
  }

  if (isThreadRunning_) {
    SDL_TRACE("EXIT thread " << name_ << " #" << handle_
                             << " is already running");
    return true;
  }

  thread_options_ = options;

  pthread_attr_t attributes;
  int pthread_result = pthread_attr_init(&attributes);
  if (pthread_result != EOK) {
    SDL_WARN("Couldn't init pthread attributes. Error code = "
             << pthread_result << " (\"" << strerror(pthread_result) << "\")");
  }

  if (!thread_options_.is_joinable()) {
    pthread_result =
        pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
    if (pthread_result != EOK) {
      SDL_WARN("Couldn't set detach state attribute. Error code = "
               << pthread_result << " (\"" << strerror(pthread_result)
               << "\")");
      thread_options_.is_joinable(false);
    }
  }

  const size_t stack_size = thread_options_.stack_size();
  if (stack_size >= Thread::kMinStackSize) {
    pthread_result = pthread_attr_setstacksize(&attributes, stack_size);
    if (pthread_result != EOK) {
      SDL_WARN("Couldn't set stacksize = "
               << stack_size << ". Error code = " << pthread_result << " (\""
               << strerror(pthread_result) << "\")");
    }
  } else {
    ThreadOptions thread_options_temp(Thread::kMinStackSize,
                                      thread_options_.is_joinable());
    thread_options_ = thread_options_temp;
  }

  if (!thread_created_) {
    // state_lock 1
    pthread_result = pthread_create(&handle_, &attributes, threadFunc, this);
    if (pthread_result == EOK) {
      SDL_DEBUG("Created thread: " << name_);
      SetNameForId(handle_, name_);
      // state_lock 0
      // possible concurrencies: stop and threadFunc
      state_cond_.Wait(auto_lock);
      thread_created_ = true;
    } else {
      SDL_ERROR("Couldn't create thread "
                << name_ << ". Error code = " << pthread_result << " (\""
                << strerror(pthread_result) << "\")");
    }
  }
  stopped_ = false;
  run_cond_.NotifyOne();
  SDL_DEBUG("Thread " << name_ << " #" << handle_
                      << " started. pthread_result = " << pthread_result);
  pthread_attr_destroy(&attributes);
  return pthread_result == EOK;
}

void Thread::yield() {
  sched_yield();
}

void Thread::stop() {
  SDL_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(state_lock_);

  stopped_ = true;

  SDL_DEBUG("Stopping thread #" << handle_ << " \"" << name_ << " \"");

  if (delegate_ && isThreadRunning_) {
    delegate_->exitThreadMain();
  }

  SDL_DEBUG("Stopped thread #" << handle_ << " \"" << name_ << " \"");
}

void Thread::join() {
  SDL_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(!IsCurrentThread());

  stop();

  sync_primitives::AutoLock auto_lock(state_lock_);
  run_cond_.NotifyOne();
  if (isThreadRunning_) {
    state_cond_.Wait(auto_lock);
  }
}

Thread::~Thread() {
  finalized_ = true;
  stopped_ = true;
  join();
  pthread_join(handle_, NULL);
}

Thread* CreateThread(const char* name, ThreadDelegate* delegate) {
  Thread* thread = new Thread(name, delegate);
  delegate->set_thread(thread);
  return thread;
}

void DeleteThread(Thread* thread) {
  delete thread;
}

}  // namespace threads
