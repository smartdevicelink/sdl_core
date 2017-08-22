/*
 Copyright (c) 2017, Ford Motor Company
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

#ifndef SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_EVENT_ENGINE_EVENT_DISPATCHER_H_
#define SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_EVENT_ENGINE_EVENT_DISPATCHER_H_

#include <list>
#include <map>

#include "utils/lock.h"

#include "remote_control/event_engine/event.h"
#include "remote_control/event_engine/event_observer.h"

#include "interfaces/HMI_API.h"

namespace rc_event_engine {

template <typename EventMessage, typename EventID>
class Event;

template <typename EventMessage, typename EventID>
class EventObserver;

template <typename EventMessage, typename EventID>
class EventDispatcher {
 public:
  /**
   * @brief Default constructor
   */
  EventDispatcher();

  /**
   * @brief Destructor
   */
  virtual ~EventDispatcher();

  /*
   * @brief Delivers the event to all subscribers
   *
   * @param event Received event
   */
  void raise_event(const Event<EventMessage, EventID>& event);

  /*
   * @brief Subscribe the observer to event
   *
   * @param event_id    The event ID to subscribe for
   * @param hmi_correlation_id  The event HMI correlation ID
   * @param observer    The observer to subscribe for event
   */
  void add_observer(const EventID& event_id,
                    int32_t hmi_correlation_id,
                    EventObserver<EventMessage, EventID>* const observer);

  /*
   * @brief Unsubscribes the observer from specific event
   *
   * @param event_id    The event ID to unsubscribe from
   * @param observer    The observer to be unsubscribed
   */
  void remove_observer(
      const EventID& event_id,
      const EventObserver<EventMessage, EventID>* const observer);

  /*
   * @brief Unsubscribes the observer from all events
   *
   * @param observer  The observer to be unsubscribed
   */
  void remove_observer(
      const EventObserver<EventMessage, EventID>* const observer);

 protected:
 private:
  DISALLOW_COPY_AND_ASSIGN(EventDispatcher);

  // Data types section
  typedef std::list<EventObserver<EventMessage, EventID>*> ObserverList;
  typedef std::map<int32_t, ObserverList> ObserversMap;
  typedef std::map<EventID, ObserversMap> EventObserverMap;

  // Members section
  sync_primitives::Lock state_lock_;
  EventObserverMap observers_;
};

template <typename EventMessage, typename EventID>
EventDispatcher<EventMessage, EventID>::EventDispatcher()
    : observers_() {}

template <typename EventMessage, typename EventID>
EventDispatcher<EventMessage, EventID>::~EventDispatcher() {}

template <typename EventMessage, typename EventID>
void EventDispatcher<EventMessage, EventID>::raise_event(
    const Event<EventMessage, EventID>& event) {
  // create local list
  ObserverList list;
  {
    sync_primitives::AutoLock auto_lock(state_lock_);
    // check if event is notification
    if (hmi_apis::messageType::notification == event.event_message_type()) {
      // ObserversMap iterator
      typename ObserversMap::iterator it = observers_[event.id()].begin();
      for (; observers_[event.id()].end() != it; ++it) {
        list = it->second;
      }
    }

    if ((hmi_apis::messageType::response == event.event_message_type()) ||
        (hmi_apis::messageType::error_response == event.event_message_type())) {
      list = observers_[event.id()][event.event_message_correlation_id()];
    }
  }

  // Call observers
  typename ObserverList::iterator observers = list.begin();
  for (; list.end() != observers; ++observers) {
    (*observers)->on_event(event);
  }
}

template <typename EventMessage, typename EventID>
void EventDispatcher<EventMessage, EventID>::add_observer(
    const EventID& event_id,
    int32_t hmi_correlation_id,
    EventObserver<EventMessage, EventID>* const observer) {
  sync_primitives::AutoLock auto_lock(state_lock_);
  observers_[event_id][hmi_correlation_id].push_back(observer);
}

template <typename EventMessage, typename EventID>
void EventDispatcher<EventMessage, EventID>::remove_observer(
    const EventID& event_id,
    const EventObserver<EventMessage, EventID>* const observer) {
  sync_primitives::AutoLock auto_lock(state_lock_);
  typename ObserversMap::iterator it = observers_[event_id].begin();
  for (; observers_[event_id].end() != it; ++it) {
    // ObserverList iterator
    typename ObserverList::iterator observer_it = it->second.begin();
    while (it->second.end() != observer_it) {
      if (observer->id() == (*observer_it)->id()) {
        observer_it = it->second.erase(observer_it);
      } else {
        ++observer_it;
      }
    }
  }
}

template <typename EventMessage, typename EventID>
void EventDispatcher<EventMessage, EventID>::remove_observer(
    const EventObserver<EventMessage, EventID>* const observer) {
  sync_primitives::AutoLock auto_lock(state_lock_);
  typename EventObserverMap::iterator event_map = observers_.begin();
  for (; observers_.end() != event_map; ++event_map) {
    typename ObserversMap::iterator it = event_map->second.begin();
    for (; event_map->second.end() != it; ++it) {
      // ObserverList iterator
      typename ObserverList::iterator observer_it = it->second.begin();
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

#endif  // SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_EVENT_ENGINE_EVENT_DISPATCHER_H_
