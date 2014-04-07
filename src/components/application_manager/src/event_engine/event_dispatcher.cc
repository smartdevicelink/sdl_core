/*
 Copyright (c) 2013, Ford Motor Company
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

#include "interfaces/HMI_API.h"
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/event_engine/event_dispatcher.h"

namespace application_manager {
namespace event_engine {
using namespace sync_primitives;

EventDispatcher::EventDispatcher()
    : observers_() {
}

EventDispatcher::~EventDispatcher() {
}

void EventDispatcher::raise_event(const Event& event) {
  // create local list
  ObserverList list;
  {
    AutoLock auto_lock(state_lock_);
    // check if event is notification
    if (hmi_apis::messageType::notification == event.smart_object_type()) {

      //ObserversMap iterator
      ObserversMap::iterator it = observers_[event.id()].begin();
      for (; observers_[event.id()].end() != it; ++it) {
        list = it->second;
      }
    }

    if (hmi_apis::messageType::response == event.smart_object_type()
        || hmi_apis::messageType::error_response == event.smart_object_type()) {
      list = observers_[event.id()][event.smart_object_correlation_id()];
    }
  }

  // Call observers
  ObserverList::iterator observers =  list.begin();
  for (; list.end() != observers; ++observers) {
      (*observers)->on_event(event);
  }
}

void EventDispatcher::add_observer(const Event::EventID& event_id,
                                   int32_t hmi_correlation_id,
                                   EventObserver* const observer) {
  AutoLock auto_lock(state_lock_);
  observers_[event_id][hmi_correlation_id].push_back(observer);
}

void EventDispatcher::remove_observer(const Event::EventID& event_id,
                                      EventObserver* const observer) {
  AutoLock auto_lock(state_lock_);
  ObserversMap::iterator it =  observers_[event_id].begin();
  for (; observers_[event_id].end() != it; ++it) {

    //ObserverList iterator
    ObserverList::iterator observer_it =  it->second.begin();
    while (it->second.end() != observer_it) {
      if (observer->id() == (*observer_it)->id()) {
        observer_it = it->second.erase(observer_it);
      } else {
        ++observer_it;
      }
    }
  }
}

void EventDispatcher::remove_observer(EventObserver* const observer) {
  AutoLock auto_lock(state_lock_);
  EventObserverMap::iterator event_map = observers_.begin();
  for (; observers_.end() != event_map; ++event_map) {

    ObserversMap::iterator it =  event_map->second.begin();
    for (; event_map->second.end() != it; ++it) {

      //ObserverList iterator
      ObserverList::iterator observer_it =  it->second.begin();
      while (it->second.end() != observer_it) {
        if (observer->id() == (*observer_it)->id()) {
          observer_it = it->second.erase(observer_it);
        } else {
          ++observer_it;
        }
      }
    }
  }
}

}
}
