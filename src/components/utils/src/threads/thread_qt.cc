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

#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "utils/logger.h"

#include <QtConcurrent/QtConcurrent>

namespace threads {

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

void sleep(uint32_t ms) {
  QThread::msleep(ms);
}

size_t Thread::kMinStackSize = 0;

void Thread::cleanup(void* arg) {
  LOGGER_AUTO_TRACE(logger_);
  Thread* thread = static_cast<Thread*>(arg);
  thread->isThreadRunning_ = false;
  thread->state_cond_.Broadcast();
}

void* Thread::threadFunc(void* arg) {
  LOGGER_DEBUG(logger_,
               "Thread #" << QThread::currentThreadId()
                          << " started successfully");

  threads::Thread* thread = static_cast<Thread*>(arg);
  DCHECK(thread);

  thread->state_lock_.Acquire();
  thread->state_cond_.Broadcast();

  while (!thread->finalized_) {
    LOGGER_DEBUG(logger_,
                 "Thread #" << QThread::currentThreadId() << " iteration");
    thread->run_cond_.Wait(thread->state_lock_);
    LOGGER_DEBUG(logger_,
                 "Thread #" << QThread::currentThreadId() << " execute. "
                            << "stopped_ = " << thread->stopped_
                            << "; finalized_ = " << thread->finalized_);
    if (!thread->stopped_ && !thread->finalized_) {
      thread->isThreadRunning_ = true;
      thread->state_lock_.Release();
      thread->delegate_->threadMain();
      thread->state_lock_.Acquire();
      thread->isThreadRunning_ = false;
    }
    thread->state_cond_.Broadcast();
    LOGGER_DEBUG(logger_,
                 "Thread #" << QThread::currentThreadId()
                            << " finished iteration");
  }

  thread->state_lock_.Release();
  LOGGER_DEBUG(logger_,
               "Thread #" << QThread::currentThreadId()
                          << " exited successfully");
  return NULL;
}

void Thread::SetNameForId(const PlatformThreadHandle& thread_id,
                          std::string name) {}

Thread::Thread(const char* name, ThreadDelegate* delegate, QObject* parent)
    : QObject(parent)
    , name_(name ? name : "undefined")
    , delegate_(delegate)
    , handle_(0)
    , thread_options_()
    , isThreadRunning_(false)
    , stopped_(false)
    , finalized_(false)
    , thread_created_(false) {
  qRegisterMetaType<QThread*>("QThread*");
}

bool Thread::start() {
  start(thread_options_);
  return true;
}

PlatformThreadHandle Thread::CurrentId() {
  return QThread::currentThread();
}

bool Thread::IsCurrentThread() const {
  return CurrentId() == thread_handle();
}

void Thread::ThreadCancelledExit() {
  future_.cancel();
}

bool Thread::start(const ThreadOptions& options) {
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(state_lock_);
  // 1 - state_lock locked
  //     stopped = 0
  //     running = 0

  if (!delegate_) {
    LOGGER_ERROR(logger_,
                 "Cannot start thread " << name_ << ": delegate is NULL");
    // 0 - state_lock unlocked
  }

  if (isThreadRunning_) {
    LOGGER_TRACE(logger_,
                 "EXIT thread " << name_ << " #" << handle_
                                << " is already running");
  }

  thread_options_ = options;

  // state_lock 1
  if (!thread_created_) {
    future_ = QtConcurrent::run(threadFunc, this);
    handle_ = QThread::currentThread();
    if (NULL != handle_) {
      LOGGER_DEBUG(logger_, "Created thread: " << name_);
      // state_lock 0
      // possible concurrencies: stop and threadFunc
      state_cond_.Wait(auto_lock);
      thread_created_ = true;
    } else {
      LOGGER_ERROR(logger_, "Couldn't create thread " << name_);
    }
  }
  stopped_ = false;
  run_cond_.NotifyOne();
  LOGGER_DEBUG(logger_, "Thread " << name_ << " #" << handle_ << " started");
  return NULL != handle_;
}

void Thread::yield() {
  QThread::yieldCurrentThread();
}

void Thread::stop() {
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(state_lock_);

  stopped_ = true;

  LOGGER_DEBUG(logger_,
               "Stopping thread #" << handle_ << " \"" << name_ << " \"");
  if (future_.isCanceled()) {
    cleanup(static_cast<void*>(this));
  }

  if (delegate_ && isThreadRunning_) {
    delegate_->exitThreadMain();
  }

  LOGGER_DEBUG(logger_,
               "Stopped thread #" << handle_ << " \"" << name_ << " \"");
}

void Thread::join() {
  LOGGER_AUTO_TRACE(logger_);
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
  if (thread_options_.is_joinable()) {
    future_.waitForFinished();
  }
}

Thread* CreateThread(const char* name, ThreadDelegate* delegate) {
  Thread* thread = new Thread(name, delegate);
  delegate->set_thread(thread);
  return thread;
}

void DeleteThread(Thread* thread) {
  delete thread;
}

#include "moc_thread.cpp"

}  // namespace threads
