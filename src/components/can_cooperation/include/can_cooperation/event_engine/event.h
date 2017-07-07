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

#ifndef SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_EVENT_ENGINE_CAN_COOPERATION_EVENT_H_
#define SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_EVENT_ENGINE_CAN_COOPERATION_EVENT_H_

#include <string>
#include "can_cooperation/event_engine/event_dispatcher.h"
#include "utils/shared_ptr.h"
#include "application_manager/message.h"

namespace can_event_engine {

template <typename EventMessage, typename EventID>
class EventDispatcher;

template <typename EventMessage, typename EventID>
class Event {
 public:
  /*
   * @brief Constructor with parameters
   *
   * @param id Event ID.
   * @param message Message received in event
   */
  Event(EventMessage& message, const EventID& id);

  /*
   * @brief Destructor
   */
  virtual ~Event() {}

  /*
   * @brief Provides event ID
   */
  inline const EventID& id() const;

  /*
   * @brief Sets event message
   *
   * @param message The message received in event
   */
  void set_event_message(EventMessage& message);

  /*
   * @brief Retrieves event message
   *
   * @return The message received in event
   */
  inline const EventMessage& event_message() const;

  /*
   * @brief Retrieves event message request ID
   */
  virtual int32_t event_message_function_id() const = 0;

  /*
   * @brief Retrieves event message correlation ID
   */
  virtual int32_t event_message_correlation_id() const = 0;

  /*
   * @brief Retrieves event message response type
   */
  virtual int32_t event_message_type() const = 0;

  void raise(EventDispatcher<EventMessage, EventID>& event_dispatcher);

 protected:
  EventMessage event_message_;

 private:
  EventID id_;
};

template <typename EventMessage, typename EventID>
const EventID& Event<EventMessage, EventID>::id() const {
  return id_;
}

template <typename EventMessage, typename EventID>
const EventMessage& Event<EventMessage, EventID>::event_message() const {
  return event_message_;
}

template <typename EventMessage, typename EventID>
Event<EventMessage, EventID>::Event(EventMessage& message, const EventID& id)
    : event_message_(message), id_(id) {}

template <typename EventMessage, typename EventID>
void Event<EventMessage, EventID>::raise(
    EventDispatcher<EventMessage, EventID>& event_dispatcher) {
  event_dispatcher.raise_event(*this);
}

}  // namespace event_engine

#endif  // SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_EVENT_ENGINE_CAN_COOPERATION_EVENT_H_
