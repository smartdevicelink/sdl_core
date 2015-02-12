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

#ifndef SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_MODULE_TIMER_H_
#define SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_MODULE_TIMER_H_

#include <set>
#include <deque>
#include <time.h>

namespace functional_modules {

typedef unsigned int TimeUnit;  //millisecs

class Trackable {
 public:
  virtual ~Trackable() {}
  virtual TimeUnit custorm_interval() const {
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

template<class Trackable> class TimerObserver {
 public:
  virtual ~TimerObserver() {}
  virtual void OnTimeoutTriggered(const Trackable& expired) = 0;
};

template<class Trackable> class ModuleTimer {
 public:
  ModuleTimer();
  ~ModuleTimer();
  inline void set_period(TimeUnit period) {
    period_ = period;
  }
  void AddObserver(TimerObserver<Trackable>* observer);
  void RemoveObserver(TimerObserver<Trackable>* observer);

  void Start();
  void Stop() {
    keep_running_ = false;
  }

  void AddTrackable(Trackable object);
  void RemoveTrackable(const Trackable& object);

 protected:
  void Notify(const Trackable& object);
  void OnTimeout(const Trackable& object);
  TimeUnit CurrentTime() const;
  typename std::set<Trackable> trackables_;
  TimeUnit period_;
 private:
  std::deque<TimerObserver<Trackable>*> observers_;
  volatile bool keep_running_;

  friend class ModuleTimerTest;
};

typedef std::set<Trackable> TrackablesCollection;

template<class Trackable>
ModuleTimer<Trackable>::ModuleTimer()
  : period_(10)
  , keep_running_(false) {

}

template<class Trackable>
ModuleTimer<Trackable>::~ModuleTimer() {
  observers_.clear();
  trackables_.clear();
}

template<class Trackable>
void ModuleTimer<Trackable>::AddObserver(TimerObserver<Trackable>* observer) {
  DCHECK(observer);
  if (!observer) {
    return;
  }
  observers_.push_back(observer);
}

template<class Trackable>
void ModuleTimer<Trackable>::RemoveObserver(TimerObserver<Trackable>* observer) {
  DCHECK(observer);
  if (!observer) {
    return;
  }
  for (typename std::deque<TimerObserver<Trackable>*>::iterator it = observers_.begin();
       observers_.end() != it;
       ++it) {
    if (*it == observer) {
      observers_.erase(it);
      return;
    }
  }
}

template<class Trackable>
void ModuleTimer<Trackable>::Start() {
  if (keep_running_) {
    this->Stop();
  }
  keep_running_ = true;
  while (keep_running_) {
    for (TrackablesCollection::iterator it = trackables_.begin();
         trackables_.end() != it; ++it) {
      TimeUnit period = it->custorm_interval();
      if (!period) {
        period = period_;
      }
      if (it->start_time() - CurrentTime() >= period) {
        OnTimeout(*it);
      }
    }
  }
}

template<class Trackable>
void ModuleTimer<Trackable>::AddTrackable(Trackable object) {
  object.set_start_time(CurrentTime());
  trackables_.insert(object);
}

template<class Trackable>
void ModuleTimer<Trackable>::RemoveTrackable(const Trackable& object) {
  trackables_.erase(object);
}

template<class Trackable>
void ModuleTimer<Trackable>::Notify(const Trackable& object) {
  for (typename std::deque<TimerObserver<Trackable>*>::const_iterator it = observers_.begin();
       observers_.end() != it; ++it) {
    (*it)->OnTimeoutTriggered(object);
  }
}

template<class Trackable>
void ModuleTimer<Trackable>::OnTimeout(const Trackable& object) {
  Notify(object);
}

template<class Trackable>
TimeUnit ModuleTimer<Trackable>::CurrentTime() const {
  return 1000 * ((float)clock()) / CLOCKS_PER_SEC;
}

}  //  namespace functional_modules

#endif  //  SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_MODULE_TIMER_H_
