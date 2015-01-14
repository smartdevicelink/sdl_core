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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_EVENT_OBSERVER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_EVENT_OBSERVER_H_

#include <string>
#include "application_manager/event_engine/event.h"
#include "application_manager/event_engine/event_dispatcher.h"

namespace application_manager {
namespace event_engine {

class EventObserver
{
 public:

  friend class EventDispatcher;

  // Typedef for possible Observer ID's from mobile_apis functionID enum
  typedef unsigned long ObserverID;

  /*
   * @brief Constructor
   *
   */
  EventObserver();

  /*
   * @brief Destructor
   */
  virtual ~EventObserver();

  /**
   * @brief Retrieves observer unique id
   *
   * @return Unique Observer id
   */
  inline const ObserverID& id() const;

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  virtual void on_event(const Event& event) = 0;

 protected:

  /*
   * @brief Subscribe to an event
   *
   * @param event_id            The event ID to subscribe for
   * @param hmi_correlation_id  The event HMI correlation ID.
   * If param is omitted, it means subscription for HMI notification
   */
  void subscribe_on_event(
      const Event::EventID& event_id, int32_t hmi_correlation_id = 0);

  /*
   * @brief Unsubscribes the observer from specific event
   *
   * @param event_id    The event ID to unsubscribe from
   */
  void unsubscribe_from_event(const Event::EventID& event_id);

  /*
   * @brief Unsubscribes the observer from all events
   *
   */
  void unsubscribe_from_all_events();

 private:

  ObserverID id_;

  DISALLOW_COPY_AND_ASSIGN(EventObserver);
};

const EventObserver::ObserverID& EventObserver::id() const {
  return id_;
}

}
}

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_EVENT_OBSERVER_H_
