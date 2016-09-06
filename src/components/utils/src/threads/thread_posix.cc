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
#ifdef THREAD_COUNT
#include "utils/threads/thread_counter.h"
#endif  // THREAD_COUNT
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
  thread->thread_state_ = kThreadStateNone;
  thread->thread_command_ = kThreadCommandNone;
  thread->state_cond_.Broadcast();
}

void* Thread::threadFunc(void* arg) {
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

  threads::Thread* thread = static_cast<Thread*>(arg);
  DCHECK(thread);

  pthread_cleanup_push(&cleanup, thread);

  thread->state_lock_.Acquire();
  thread->thread_state_ = kThreadStateIdle;
  thread->state_cond_.Broadcast();

  // We have special variable for controlling iterations/exiting thread
  // in order to separate decision logic (continue iterations or exit?)
  // from controlling while cycle
  bool continueIterations = true;

  while (continueIterations) {
    SDL_DEBUG("Thread #" << pthread_self() << " iteration");
    while (kThreadCommandNone == thread->thread_command_) {
      thread->run_cond_.Wait(thread->state_lock_);
    }
    SDL_DEBUG("Thread #" << pthread_self() << " execute. "
                         << "thread_command_ = " << thread->thread_command_);

    switch (thread->thread_command_) {
      case kThreadCommandRun:
        thread->thread_state_ = kThreadStateRunning;
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        pthread_testcancel();
        thread->state_lock_.Release();
        thread->delegate_->threadMain();
        thread->state_lock_.Acquire();
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        thread->thread_state_ = kThreadStateIdle;
        break;

      case kThreadCommandFinalize:
        continueIterations = false;
        break;

      default:
        // check for not expected command:
        if ((kThreadCommandNone > thread->thread_command_) &&
            (kThreadCommandFinalize < thread->thread_command_)) {
          SDL_ERROR("Incorrect thread command: " << thread->thread_command_);
        }
        // else - nothing to do
        break;
    }

    thread->thread_command_ = kThreadCommandNone;  // consumed
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
    , thread_command_(kThreadCommandNone)
    , thread_state_(kThreadStateNone) {
#ifdef THREAD_COUNT
  ThreadCounter::Increment();
#endif  // THREAD_COUNT
}

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

  if (kThreadStateRunning == thread_state_) {
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

  if (kThreadStateNone == thread_state_) {
    // state_lock 1
    pthread_result = pthread_create(&handle_, &attributes, threadFunc, this);
    if (pthread_result == EOK) {
      SDL_DEBUG("Created thread: " << name_);
      SetNameForId(handle_, name_);
      // state_lock 0
      while (kThreadStateNone == thread_state_) {
        state_cond_.Wait(auto_lock);
      }
    } else {
      SDL_ERROR("Couldn't create thread "
                << name_ << ". Error code = " << pthread_result << " (\""
                << strerror(pthread_result) << "\")");
    }
  }

  thread_command_ = kThreadCommandRun;
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
  StopUnsafe();
}

void Thread::join(const JoinOptionStop force_stop) {
  SDL_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(!IsCurrentThread());
  sync_primitives::AutoLock auto_lock(state_lock_);
  JoinUnsafe(force_stop);
}

void Thread::JoinUnsafe(const JoinOptionStop force_stop) {
  SDL_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(!IsCurrentThread());
  // if thread still exist at all
  if (kThreadStateNone != thread_state_) {
    if (kForceStop == force_stop) {
      StopUnsafe();
    }
    while ((kThreadStateRunning == thread_state_) ||
           (kThreadCommandNone != thread_command_)) {
      state_cond_.Wait(state_lock_);
    }
  }
}

void Thread::StopUnsafe() {
  SDL_AUTO_TRACE();
  thread_command_ = kThreadCommandNone;  // cancel all active commands
  SDL_DEBUG("Stopping thread #" << handle_ << " \"" << name_ << " \"");

  if (delegate_ && (kThreadStateRunning == thread_state_)) {
    delegate_->exitThreadMain();
  }

  SDL_DEBUG("Stopped thread #" << handle_ << " \"" << name_ << " \"");
}

Thread::~Thread() {
  if (0 != handle_) {
    state_lock_.Acquire();
    if (kThreadStateNone != thread_state_) {
      JoinUnsafe(kForceStop);
      thread_command_ = kThreadCommandFinalize;
      run_cond_.NotifyOne();
    }
    state_lock_.Release();
    if (thread_options_.is_joinable()) {
      int join_res = pthread_join(handle_, NULL);
      if (0 != join_res) {
        SDL_ERROR("pthread_join() for thread " << handle_ << " returned "
                                               << join_res);
      }
    }
  }
#ifdef THREAD_COUNT
  ThreadCounter::Decrement();
#endif  // THREAD_COUNT
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
