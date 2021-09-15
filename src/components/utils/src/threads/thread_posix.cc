/*
 * Copyright (c) 2015, Ford Motor Company
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
#include <pthread.h>
#include <signal.h>
#include <stddef.h>
#include <algorithm>
#include <functional>

#include "utils/atomic.h"
#include "utils/logger.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

#ifndef __QNXNTO__
const int EOK = 0;
#endif

#if defined(OS_POSIX)
const size_t THREAD_NAME_SIZE = 15;
#endif

namespace threads {

SDL_CREATE_LOG_VARIABLE("Utils")

size_t Thread::kMinStackSize =
    PTHREAD_STACK_MIN; /* Ubuntu : 16384 ; QNX : 256; */

void Thread::cleanup(void* arg) {
  SDL_LOG_AUTO_TRACE();
  Thread* thread = static_cast<Thread*>(arg);
  sync_primitives::AutoLock auto_lock(thread->state_lock_);
  thread->thread_state_ = kThreadStateCompleted;
  thread->thread_command_ = kThreadCommandNone;
  thread->state_cond_.Broadcast();
}

void* Thread::threadFunc(void* arg) {
  auto thread_procedure_execution = [](Thread* thread) {
    thread->thread_state_ = kThreadStateRunning;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();
    thread->state_lock_.Release();
    thread->delegate_->threadMain();
    thread->state_lock_.Acquire();
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
  };

  threads::Thread* thread = static_cast<Thread*>(arg);
  DCHECK(thread);

  thread->state_lock_.Acquire();

  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
  pthread_cleanup_push(&cleanup, thread);

  thread->state_cond_.Broadcast();

  // We have special variable for controlling iterations/exiting thread
  // in order to separate decision logic (continue iterations or exit?)
  // from controlling while cycle
  bool continueIterations = true;

  while (continueIterations) {
    thread->thread_state_ = kThreadStateIdle;
    SDL_LOG_DEBUG("Thread #" << pthread_self() << " iteration");
    thread->state_cond_.Wait(thread->state_lock_);
    SDL_LOG_DEBUG("Thread #"
                  << pthread_self() << " execute. "
                  << "thread_command_ = " << thread->thread_command_);

    switch (thread->thread_command_) {
      case kThreadCommandRun:
        thread_procedure_execution(thread);
        break;

      case kThreadCommandFinalize:
        continueIterations = false;
        break;

      default:
        SDL_LOG_ERROR("Incorrect thread command: " << thread->thread_command_);
        break;
    }

    thread->thread_command_ = kThreadCommandNone;  // consumed
    thread->state_cond_.Broadcast();
    SDL_LOG_DEBUG("Thread #" << pthread_self() << " finished iteration");
  }

  thread->state_lock_.Release();

  const auto execute_cleanup = 1;
  // The pthread_cleanup_pop() function shall remove the routine at the top of
  // the calling thread's cancellation cleanup stack and optionally invoke it
  // (if execute is non-zero).
  pthread_cleanup_pop(execute_cleanup);

  SDL_LOG_DEBUG("Thread #" << pthread_self() << " exited successfully");
  return NULL;
}

void Thread::SetNameForId(const PlatformThreadHandle& thread_id,
                          std::string name) {
  if (name.size() > THREAD_NAME_SIZE)
    name.erase(THREAD_NAME_SIZE);
  const int rc = pthread_setname_np(thread_id, name.c_str());
  if (rc != EOK) {
    SDL_LOG_WARN("Couldn't set pthread name \"" << name << "\", error code "
                                                << rc << " (" << strerror(rc)
                                                << ")");
  }
}

Thread::Thread(const char* name, ThreadDelegate* delegate)
    : name_(name ? name : "undefined")
    , delegate_(delegate)
    , handle_(0)
    , thread_options_()
    , thread_command_(kThreadCommandNone)
    , thread_state_(kThreadStateNone) {}

bool Thread::Start() {
  return Start(thread_options_);
}

PlatformThreadHandle Thread::CurrentId() {
  return pthread_self();
}

bool Thread::IsCurrentThread() const {
  return pthread_equal(CurrentId(), ThreadHandle());
}

bool Thread::Start(const ThreadOptions& options) {
  SDL_LOG_AUTO_TRACE();

  sync_primitives::AutoLock auto_lock(state_lock_);

  if (!delegate_) {
    SDL_LOG_ERROR("Cannot start thread " << name_ << ": delegate is NULL");
    return false;
  }

  if (kThreadStateCompleted == thread_state_) {
    SDL_LOG_ERROR("Cannot start thread " << name_ << ":  thread completed");
    return false;
  }

  if (kThreadStateRunning == thread_state_) {
    SDL_LOG_TRACE("EXIT thread " << name_ << " #" << handle_
                                 << " is already running");
    return true;
  }

  if (!handle_) {
    thread_options_ = options;
    pthread_attr_t attributes = SetThreadCreationAttributes(&thread_options_);

    int pthread_result =
        pthread_create(&handle_, &attributes, threadFunc, this);
    pthread_attr_destroy(&attributes);

    if (EOK != pthread_result) {
      SDL_LOG_ERROR("Couldn't create thread "
                    << name_ << ". Error code = " << pthread_result << " (\""
                    << strerror(pthread_result) << "\")");
      handle_ = 0;
      thread_state_ = kThreadStateError;
      return false;
    }

    SDL_LOG_DEBUG("Created thread: " << name_);
    SetNameForId(handle_, name_);
    // state_lock 0
    // possible concurrencies: stop and threadFunc
    state_cond_.Wait(auto_lock);
  }

  if (kThreadCommandFinalize == thread_command_) {
    SDL_LOG_DEBUG("Thread " << name_ << " #" << handle_
                            << " waiting finalize.");
    return false;
  }

  thread_command_ = kThreadCommandRun;
  state_cond_.NotifyOne();

  SDL_LOG_DEBUG("Thread " << name_ << " #" << handle_ << " started."
                          << " pthread_result = " << EOK);
  return true;
}

void Thread::SchedYield() {
  sched_yield();
}

bool Thread::Stop(const ThreadStopOption stop_option) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN(
      (kThreadStopDelegate <= stop_option) && (kThreadForceStop >= stop_option),
      false);
  sync_primitives::AutoLock auto_lock(state_lock_);
  thread_command_ = kThreadCommandNone;  // cancel all active commands

  if (!handle_ && kThreadStateError != thread_state_) {
    SDL_LOG_WARN(
        "Thread " << name_ << ": can't stopped,thread is not run handle_: "
                  << handle_ << " thread_state_ is: " << thread_state_);
    return false;
  }

  if (kThreadStateError == thread_state_ ||
      kThreadStateCompleted == thread_state_) {
    SDL_LOG_WARN("Thread " << name_ << ": can't stopped thread_state_ is: "
                           << thread_state_);
    return false;
  }

  SDL_LOG_DEBUG("Stopping thread #" << handle_ << " \"" << name_ << "\"");

  bool result = false;
  switch (stop_option) {
    case kThreadStopDelegate:
      result = StopDelegate(auto_lock);
      break;
    case kThreadSoftStop:
      result = StopSoft(auto_lock);
      break;
    case kThreadForceStop:
      StopForce(auto_lock);
      result = true;
      break;
    default:
      SDL_LOG_ERROR("Incorrect thread stop option: " << stop_option);
      break;
  }

  SDL_LOG_DEBUG("Is thread stopped #" << handle_ << " \"" << name_
                                      << " \": " << result);
  return result;
}

void Thread::Join(const ThreadJoinOption join_option) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(!IsCurrentThread());
  DCHECK_OR_RETURN_VOID((kThreadJoinDelegate <= join_option) &&
                        (kThreadJoinThread >= join_option));
  if (!handle_ || kThreadStateError == thread_state_) {
    SDL_LOG_WARN("Thread " << name_ << ": is not joinable handle_: " << handle_
                           << " thread_state_ is: " << thread_state_);
    return;
  }

  {
    sync_primitives::AutoLock auto_lock(state_lock_);
    JoinDelegate(auto_lock);
  }

  if (kThreadJoinDelegate == join_option) {
    return;
  }

  SDL_LOG_DEBUG("Waiting for #" << handle_ << " to finished thread #"
                                << pthread_self());

  pthread_join(handle_, NULL);
}

Thread::~Thread() {
  Stop(kThreadForceStop);
  Join(kThreadJoinThread);
}

Thread* CreateThread(const char* name, ThreadDelegate* delegate) {
  Thread* thread = new Thread(name, delegate);
  delegate->set_thread(thread);
  return thread;
}

void DeleteThread(Thread* thread) {
  delete thread;
}

pthread_attr_t Thread::SetThreadCreationAttributes(
    ThreadOptions* thread_options) {
  pthread_attr_t attributes;
  int pthread_result = pthread_attr_init(&attributes);
  if (pthread_result != EOK) {
    SDL_LOG_WARN("Couldn't init pthread attributes. Error code = "
                 << pthread_result << " (\"" << strerror(pthread_result)
                 << "\")");
  }

  if (!thread_options) {
    return attributes;
  }

  if (!thread_options->is_joinable()) {
    SDL_LOG_WARN(
        "Set state detach attribute, undefined behavior possible with "
        "this attribute");
    pthread_result =
        pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
    if (pthread_result != EOK) {
      SDL_LOG_WARN("Couldn't set detach state attribute. Error code = "
                   << pthread_result << " (\"" << strerror(pthread_result)
                   << "\")");
      thread_options->is_joinable(false);
    }
  }

  const size_t stack_size = thread_options->stack_size();
  if (stack_size >= Thread::kMinStackSize) {
    pthread_result = pthread_attr_setstacksize(&attributes, stack_size);
    if (pthread_result != EOK) {
      SDL_LOG_WARN("Couldn't set stacksize = "
                   << stack_size << ". Error code = " << pthread_result
                   << " (\"" << strerror(pthread_result) << "\")");
    }
  } else {
    ThreadOptions thread_options_temp(Thread::kMinStackSize,
                                      thread_options->is_joinable());
    *thread_options = thread_options_temp;
  }

  return attributes;
}

bool Thread::StopDelegate(sync_primitives::AutoLock& auto_lock) {
  SDL_LOG_AUTO_TRACE();

  if (kThreadStateRunning != thread_state_) {
    SDL_LOG_WARN("Thread " << name_ << ": task can't stopped thread_state_ is: "
                           << thread_state_);
    return false;
  }

  if (!delegate_) {
    SDL_LOG_WARN("Thread " << name_ << ": task can't stopped delegate is NULL");
    return false;
  }

  delegate_->exitThreadMain();

  JoinDelegate(auto_lock);

  return true;
}

bool Thread::StopSoft(sync_primitives::AutoLock& auto_lock) {
  SDL_LOG_AUTO_TRACE();

  if (kThreadStateRunning == thread_state_) {
    bool result = StopDelegate(auto_lock);
    if (!result) {
      return false;
    }
  }

  if (kThreadStateIdle != thread_state_) {
    SDL_LOG_WARN("Thread " << name_ << ": can't stopped thread_state_ is: "
                           << thread_state_);
    return false;
  }

  thread_command_ = kThreadCommandFinalize;
  state_cond_.NotifyOne();

  if (!pthread_equal(pthread_self(), handle_)) {
    SDL_LOG_DEBUG("Waiting for #" << handle_ << " last iteration in thread #"
                                  << pthread_self());
    state_cond_.Wait(auto_lock);
  }

  return true;
}

void Thread::StopForce(sync_primitives::AutoLock& auto_lock) {
  SDL_LOG_AUTO_TRACE();

  if (kThreadStateRunning == thread_state_ ||
      kThreadStateIdle == thread_state_) {
    bool result = StopSoft(auto_lock);
    if (result) {
      return;
    }
  }
  // Notify not to thread but to actor
  // that may starting this thread in race condition.
  thread_state_ = kThreadStateCompleted;
  state_cond_.NotifyOne();

  SDL_LOG_WARN("The thread was not soft stopped, the start of a forced stop");

  if (!pthread_equal(pthread_self(), handle_)) {
    SDL_LOG_DEBUG("Thread #" << handle_ << " cancel");
    pthread_cancel(handle_);
  } else {
    SDL_LOG_DEBUG("Thread #" << handle_ << " exit");
    pthread_exit(NULL);
    NOTREACHED();
  }
}

void Thread::JoinDelegate(sync_primitives::AutoLock& auto_lock) {
  SDL_LOG_AUTO_TRACE();
  if (kThreadStateRunning != thread_state_) {
    SDL_LOG_WARN("Thread " << name_
                           << ": delegate is not joinable thread_state_ is: "
                           << thread_state_);
    return;
  }

  if (!pthread_equal(pthread_self(), handle_)) {
    SDL_LOG_DEBUG("Waiting for #" << handle_
                                  << " finished iteration in thread #"
                                  << pthread_self());
    state_cond_.Wait(auto_lock);
  }
}

}  // namespace threads
