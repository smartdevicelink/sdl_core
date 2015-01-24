/*
 Copyright (c) 2014, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "utils/threads/async_runner.h"

#include <string>

#include "utils/logger.h"

namespace threads {

CREATE_LOGGERPTR_GLOBAL(logger_, "AsyncRunner");

AsyncRunner::AsyncRunner(const std::string &thread_name)
  : executor_(new AsyncRunnerDelegate) {
  LOG4CXX_AUTO_TRACE(logger_);
  thread_ = threads::CreateThread(thread_name.c_str(),
                                  executor_);
  thread_->start();
}

void AsyncRunner::AsyncRun(ThreadDelegate* delegate) {
  LOG4CXX_AUTO_TRACE(logger_);
  executor_->runDelegate(delegate);
}

void AsyncRunner::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  thread_->join();
}

AsyncRunner::~AsyncRunner() {
  LOG4CXX_AUTO_TRACE(logger_);
  thread_->join();
  delete executor_;
  threads::DeleteThread(thread_);
}

AsyncRunner::AsyncRunnerDelegate::AsyncRunnerDelegate()
  : stop_flag_(false) {
}

void AsyncRunner::AsyncRunnerDelegate::processDelegate() {
  if (!delegates_queue_.empty()) {
    delegates_queue_lock_.Acquire();
    ThreadDelegate* run = delegates_queue_.front();
    delegates_queue_.pop();
    delegates_queue_lock_.Release();

    if (NULL != run) {
      run->threadMain();
      delete run;
    }
  }
}

void AsyncRunner::AsyncRunnerDelegate::waitForDelegate() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(delegates_queue_lock_);
  if (!stop_flag_ && delegates_queue_.empty()) {
    delegate_notifier_.Wait(lock);
  }
}

void AsyncRunner::AsyncRunnerDelegate::threadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
  while (!stop_flag_) {
    processDelegate();
    waitForDelegate();
  }
}

void AsyncRunner::AsyncRunnerDelegate::exitThreadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(delegates_queue_lock_);
  stop_flag_ = true;
  delegate_notifier_.NotifyOne();
}

void AsyncRunner::AsyncRunnerDelegate::runDelegate(ThreadDelegate* delegate) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(delegates_queue_lock_);
  delegates_queue_.push(delegate);
  delegate_notifier_.NotifyOne();
}

} // namespace policy.
