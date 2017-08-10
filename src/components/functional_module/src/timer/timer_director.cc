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
#include <typeinfo>
#include "functional_module/timer/timer_director.h"
#include "remote_control/rc_module_timer.h"
#include "utils/logger.h"

namespace functional_modules {

template <class T>
TimerThreadDelegate<T>::TimerThreadDelegate(ModuleTimer<T>& timer)
    : timer_(timer), keep_running_(false) {}

template <class T>
void TimerThreadDelegate<T>::threadMain() {
  if (keep_running_) {
    this->exitThreadMain();
  }
  {
    sync_primitives::AutoLock run_lock(keep_running_lock_);
    keep_running_ = true;
  }
  sync_primitives::AutoLock run_lock(keep_running_lock_);
  while (keep_running_) {
    const TimeUnit msecs_to_wait = timer_.GetSecondsToNearestTimeout() * 1000;
    sync_primitives::ConditionalVariable::WaitStatus wait_status =
        keep_running_cond_.WaitFor(run_lock, msecs_to_wait);
    if (sync_primitives::ConditionalVariable::kTimeout == wait_status &&
        keep_running_) {
      timer_.CheckTimeout();
    }
  }
}

template <class T>
void TimerThreadDelegate<T>::exitThreadMain() {
  if (keep_running_) {
    sync_primitives::AutoLock run_lock(keep_running_lock_);
    keep_running_ = false;
    keep_running_cond_.NotifyOne();
  }
}

template <class T>
void TimerThreadDelegate<T>::ResetTimer() {
  if (keep_running_) {
    sync_primitives::AutoLock run_lock(keep_running_lock_);
    keep_running_cond_.NotifyOne();
  }
}

TimerDirector::TimerDirector() {}

TimerDirector::~TimerDirector() {
  UnregisterAllTimers();
}

template <class T>
void TimerDirector::RegisterTimer(ModuleTimer<T>& timer) {
  std::string type_name = typeid(timer).name();
  std::map<std::string, threads::Thread*>::iterator it =
      timer_threads_.find(type_name);
  if (timer_threads_.end() != it) {
    //  Attempt to register timer of already existing type fail.
    return;
  }
  TimerThreadDelegate<T>* delegate = new TimerThreadDelegate<T>(timer);
  threads::Thread* thread = threads::CreateThread(type_name.c_str(), delegate);

  const size_t kStackSize = 16384;
  if (thread->start(threads::ThreadOptions(kStackSize))) {
    timer_threads_.insert(std::make_pair(type_name, thread));
  } else {
    //  Failed to start timer thread for
  }
}

template void TimerDirector::RegisterTimer<remote_control::TrackableMessage>(
    ModuleTimer<remote_control::TrackableMessage>& timer);

template <class T>
void TimerDirector::UnregisterTimer(const ModuleTimer<T>& timer) {
  std::string type_name = typeid(timer).name();
  std::map<std::string, threads::Thread*>::iterator it =
      timer_threads_.find(type_name);
  if (timer_threads_.end() == it) {
    ///  Failed to unregister timer that was not registered
    return;
  }
  threads::ThreadDelegate* delegate = it->second->delegate();
  DeleteThread(it->second);
  delete delegate;
  timer_threads_.erase(it);
}

template void TimerDirector::UnregisterTimer<remote_control::TrackableMessage>(
    const ModuleTimer<remote_control::TrackableMessage>& timer);

template <class T>
void TimerDirector::ResetTimer(ModuleTimer<T>& timer) {
  const std::string type_name = typeid(timer).name();
  std::map<std::string, threads::Thread*>::iterator it =
      timer_threads_.find(type_name);
  if (timer_threads_.end() == it) {
    return;
  }
  TimerThreadDelegate<T>* delegate =
      static_cast<TimerThreadDelegate<T>*>(it->second->delegate());
  delegate->ResetTimer();
}

template void TimerDirector::ResetTimer<remote_control::TrackableMessage>(
    ModuleTimer<remote_control::TrackableMessage>& timer);

void TimerDirector::UnregisterAllTimers() {
  for (std::map<std::string, threads::Thread*>::iterator it =
           timer_threads_.begin();
       timer_threads_.end() != it;
       ++it) {
    threads::ThreadDelegate* delegate = it->second->delegate();
    DeleteThread(it->second);
    delete delegate;
  }
  timer_threads_.clear();
}

}  //  namespace functional_modules
