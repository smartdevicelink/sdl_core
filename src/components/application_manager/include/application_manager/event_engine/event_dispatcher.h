/*
 Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_EVENT_DISPATCHER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_EVENT_DISPATCHER_H_

#include <list>
#include "application_manager/event_engine/event.h"

namespace application_manager {
namespace event_engine {

class EventObserver;

class EventDispatcher {
 public:

  /*
   * @brief Delivers the event to all subscribers
   *
   * @param event Received event
   */
  virtual void raise_event(const Event& event) = 0;

  /*
   * @brief Subscribe the observer to event
   *
   * @param event_id    The event ID to subscribe for
   * @param hmi_correlation_id  The event HMI correlation ID
   * @param observer    The observer to subscribe for event
   */
  virtual void add_observer(const Event::EventID& event_id,
                    int32_t hmi_correlation_id,
                    EventObserver* const observer) = 0;

  /*
   * @brief Unsubscribes the observer from specific event
   *
   * @param event_id    The event ID to unsubscribe from
   * @param observer    The observer to be unsubscribed
   */
  virtual void remove_observer(const Event::EventID& event_id,
                       EventObserver* const observer) = 0;

  /*
   * @brief Unsubscribes the observer from all events
   *
   * @param observer  The observer to be unsubscribed
   */
  virtual void remove_observer(EventObserver* const observer) = 0;

  /*
   * @brief Destructor
   */
  virtual ~EventDispatcher() {
  };
};

}  // namespace event_engine
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_EVENT_DISPATCHER_H_
