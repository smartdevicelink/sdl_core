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

#ifndef SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_TIMER_MODULE_TIMER_H_
#define SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_TIMER_MODULE_TIMER_H_

#include <list>
#include <deque>
#include <algorithm>
#include <time.h>
#include "utils/lock.h"

namespace functional_modules {

typedef unsigned int TimeUnit;  // seconds

class Trackable {
 public:
  Trackable() : start_time_(0) {}
  virtual ~Trackable() {}
  virtual TimeUnit custom_interval() const {
    return 0;
  }
  virtual TimeUnit start_time() const {
    return start_time_;
  }
  virtual void set_start_time(TimeUnit start_time) {
    start_time_ = start_time;
  }

 private:
  TimeUnit start_time_;
};

template <class Trackable>
class TimerObserver {
 public:
  virtual ~TimerObserver() {}
  virtual void OnTimeoutTriggered(const Trackable& expired) = 0;
};

template <class Trackable>
class ModuleTimer {
 public:
  ModuleTimer();
  ~ModuleTimer();
  void set_period(TimeUnit period) {
    period_ = period;
  }
  TimeUnit period() const {
    return period_;
  }
  void AddObserver(TimerObserver<Trackable>* observer);
  void RemoveObserver(TimerObserver<Trackable>* observer);

  void CheckTimeout();
  /**
   * @brief Gets time in seconds when the nearest request timeout will be
   * triggered
   * @return time in seconds when the nearest request timeout will be triggered
   */
  TimeUnit GetSecondsToNearestTimeout();

  /*
   * @brief Adds object to be tracked by timer.
   If same object is already added replaces it with new one
   correspondingly updating start time of tracking.
   */
  void AddTrackable(const Trackable& object);
  void RemoveTrackable(const Trackable& object);

 protected:
  void Notify(const Trackable& object);
  void OnTimeout(const Trackable& object);
  TimeUnit CurrentTime() const;
  typename std::list<Trackable> trackables_;
  volatile TimeUnit period_;

 private:
  std::deque<TimerObserver<Trackable>*> observers_;
  mutable sync_primitives::Lock trackables_lock_;
  friend class ModuleTimerTest;
};

template <class Trackable>
ModuleTimer<Trackable>::ModuleTimer()
    : period_(10) {}

template <class Trackable>
ModuleTimer<Trackable>::~ModuleTimer() {
  observers_.clear();
  sync_primitives::AutoLock auto_lock(trackables_lock_);
  trackables_.clear();
}

template <class Trackable>
void ModuleTimer<Trackable>::AddObserver(TimerObserver<Trackable>* observer) {
  DCHECK(observer);
  if (!observer) {
    return;
  }
  observers_.push_back(observer);
}

template <class Trackable>
void ModuleTimer<Trackable>::RemoveObserver(
    TimerObserver<Trackable>* observer) {
  DCHECK(observer);
  if (!observer) {
    return;
  }
  for (typename std::deque<TimerObserver<Trackable>*>::iterator it =
           observers_.begin();
       observers_.end() != it;
       ++it) {
    if (*it == observer) {
      observers_.erase(it);
      return;
    }
  }
}

template <class Trackable>
void ModuleTimer<Trackable>::CheckTimeout() {
  sync_primitives::AutoLock trackables_lock(trackables_lock_);
  for (typename std::list<Trackable>::iterator it = trackables_.begin();
       trackables_.end() != it;
       ++it) {
    TimeUnit period = it->custom_interval();
    if (!period) {
      period = period_;
    }
    if (CurrentTime() - it->start_time() >= period) {
      OnTimeout(*it);
      it = trackables_.erase(it);
    }
  }
}

template <class Trackable>
TimeUnit ModuleTimer<Trackable>::GetSecondsToNearestTimeout() {
  sync_primitives::AutoLock trackables_lock(trackables_lock_);
  TimeUnit result = period_;
  for (typename std::list<Trackable>::iterator it = trackables_.begin();
       trackables_.end() != it;
       ++it) {
    TimeUnit period = it->custom_interval();
    if (!period) {
      period = period_;
    }
    const TimeUnit current_secs_to_timeout =
        period - (CurrentTime() - it->start_time());
    if (result > current_secs_to_timeout) {
      result = current_secs_to_timeout;
    }
  }
  return result;
}

template <class Trackable>
void ModuleTimer<Trackable>::AddTrackable(const Trackable& object) {
  sync_primitives::AutoLock trackables_lock(trackables_lock_);
  trackables_.remove(object);
  trackables_.push_back(object);
  trackables_.back().set_start_time(CurrentTime());
}

template <class Trackable>
void ModuleTimer<Trackable>::RemoveTrackable(const Trackable& object) {
  sync_primitives::AutoLock trackables_lock(trackables_lock_);
  trackables_.remove(object);
}

template <class Trackable>
void ModuleTimer<Trackable>::Notify(const Trackable& object) {
  for (typename std::deque<TimerObserver<Trackable>*>::const_iterator it =
           observers_.begin();
       observers_.end() != it;
       ++it) {
    (*it)->OnTimeoutTriggered(object);
  }
}

template <class Trackable>
void ModuleTimer<Trackable>::OnTimeout(const Trackable& object) {
  Notify(object);
}

template <class Trackable>
TimeUnit ModuleTimer<Trackable>::CurrentTime() const {
  // TODO(PV): move outside to platform-dependant parts
  struct timespec current_time;
  if (0 == clock_gettime(CLOCK_MONOTONIC, &current_time)) {
    return current_time.tv_sec;
  } else {
    return 0;
  }
}

}  //  namespace functional_modules

#endif  //  SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_TIMER_MODULE_TIMER_H_
