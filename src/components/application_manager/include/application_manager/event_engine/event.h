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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_EVENT_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_EVENT_H_

#include <interfaces/HMI_API.h>

#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"

namespace application_manager {
namespace event_engine {

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

class Event {
 public:

  // Typedef for possible Event ID's from mobile_apis functionID enum
  typedef hmi_apis::FunctionID::eType EventID;

  /*
   * @brief Constructor with parameters
   *
   * @param id Event ID. Please see mobile_apis::FunctionID for possible ID's
   */
  explicit Event(const EventID& id);

  /*
   * @brief Destructor
   */
  virtual ~Event();

  /*
   * @brief Sends synchronously event to all subscribers.
   *
   */
  void raise();

  /*
   * @brief Provides event ID
   */
  inline const EventID& id() const;

  /*
   * @brief Sets event smart object
   *
   * @param so The smart_object received in HMI response
   */
  void set_smart_object(const smart_objects::SmartObject& so);

  /*
   * @brief Retrieves event smart object
   *
   * @return The smart_object received in HMI response
   */
  inline const smart_objects::SmartObject& smart_object() const;

  /*
   * @brief Retrieves smart object request ID
   */
  inline int32_t smart_object_function_id() const;

  /*
   * @brief Retrieves smart object correlation ID
   */
  inline int32_t smart_object_correlation_id() const;

  /*
   * @brief Retrieves smart_object response type
   */
  inline int32_t smart_object_type() const;

 protected:

 private:

  EventID                        id_;
  smart_objects::SmartObject     response_so_;

  /*
   * @brief Default constructor
   *
   * Unimplemented to avoid misusing
   */
  Event();

  DISALLOW_COPY_AND_ASSIGN(Event);
};

const Event::EventID& Event::id() const {
  return id_;
}

const smart_objects::SmartObject& Event::smart_object() const {
  return response_so_;
}

int32_t Event::smart_object_function_id() const {
  return response_so_.getElement(
      strings::params).getElement(strings::function_id).asInt();
}

int32_t Event::smart_object_correlation_id() const {
  return response_so_.getElement(
      strings::params).getElement(strings::correlation_id).asInt();
}

int32_t Event::smart_object_type() const {
  return response_so_.getElement(
        strings::params).getElement(strings::message_type).asInt();
}

}  // namespace event_engine
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_EVENT_H_
