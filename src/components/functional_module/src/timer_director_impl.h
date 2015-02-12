/*
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

#include "./timer_director.h"
#include <typeinfo>
#include "utils/logger.h"

namespace functional_modules {

CREATE_LOGGERPTR_GLOBAL(logger_, "TimerDirector");

template<class Trackable>
TimerThreadDelegate<Trackable>::TimerThreadDelegate(const ModuleTimer<Trackable>& timer)
  : timer_(timer)
  , keep_running_(false) {
}

template<class Trackable>
void TimerThreadDelegate<Trackable>::threadMain() {
  if (keep_running_) {
    this->exitThreadMain();
  }
  {
    sync_primitives::AutoLock run_lock(keep_running_lock_);
    keep_running_ = true;
  }
  sync_primitives::AutoLock run_lock(keep_running_lock_);
  while (keep_running_) {
    sync_primitives::ConditionalVariable::WaitStatus wait_status =
      keep_running_cond_.WaitFor(run_lock,
                                 timer_.period() * 1000);
    if (sync_primitives::ConditionalVariable::kTimeout ==
        wait_status && keep_running_) {
      timer_.CheckTimeout();
    }
  }
}

template<class Trackable>
bool TimerThreadDelegate<Trackable>::exitThreadMain() {
  {
    sync_primitives::AutoLock run_lock(keep_running_lock_);
    keep_running_ = false;
  }
  keep_running_cond_.NotifyOne();
  return false;
}

TimerDirector::TimerDirector() {
}

TimerDirector::~TimerDirector() {
  UnregisterAllTimers();
}

template<class Trackable>
void TimerDirector::RegisterTimer(const ModuleTimer<Trackable>& timer) {
  std::string type_name = typeid(timer).name();
  std::map<std::string, threads::Thread*>::iterator it =
    timer_threads_.find(type_name);
  if (timer_threads_.end() != it) {
    LOG4CXX_WARN(logger_, "Attempt to register timer of already existing type fail.");
    return;
  }
  TimerThreadDelegate<Trackable>* delegate = new TimerThreadDelegate<Trackable>(
    timer);
  threads::Thread* thread = new threads::Thread(type_name, delegate);

  const size_t kStackSize = 16384;
  if (thread->startWithOptions(threads::ThreadOptions(kStackSize))) {
    timer_threads_.insert(std::make_pair(type_name, thread));
    thread_delegates_.insert(
      std::make_pair<std::string, TimerThreadDelegate<Trackable>*>(
        type_name, delegate));
  } else {
    LOG4CXX_ERROR(logger_, "Failed to start timer thread for " << type_name);
  }
}
typedef typename std::map<std::string,
        TimerThreadDelegate<Trackable>*>::iterator TimerThreadDelegateIterator;

template<class Trackable>
void TimerDirector::UnregisterTimer(const ModuleTimer<Trackable>& timer) {
  std::string type_name = typeid(timer).name();
  std::map<std::string, threads::Thread*>::iterator it =
    timer_threads_.find(type_name);
  if (timer_threads_.end() == it) {
    LOG4CXX_ERROR(logger_, "Failed to unregister timer that was \
        not registered " << type_name);
    return;
  }
  it->second->stop();
  delete it->second;
  TimerThreadDelegateIterator deleage_it = thread_delegates_.find(type_name);
  if (thread_delegates_.end() != deleage_it) {
    delete deleage_it->second;
    thread_delegates_.erase(deleage_it);
  }
  timer_threads_.erase(it);
}

void TimerDirector::UnregisterAllTimers() {
  for (std::map<std::string, threads::Thread*>::iterator it =
         timer_threads_.begin(); timer_threads_.end() != it; ++it) {
    it->second->stop();
    delete it->second;
  }
  for (TimerThreadDelegateIterator it = thread_delegates_.begin();
       thread_delegates_.end() != it; ++it) {
    delete it->second;
  }
  timer_threads_.clear();
  thread_delegates_.clear();
}

}  //  namespace functional_modules
