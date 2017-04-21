/*
 * Copyright (c) 2016, Ford Motor Company
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

#include "gtest/gtest.h"

#include "application_manager/event_engine/event_observer.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/event_engine/event_dispatcher_impl.h"
#include "application_manager/event_observer_mock.h"
#include "application_manager/mock_event_dispatcher.h"
#include "smart_objects/smart_object.h"
#include "utils/make_shared.h"
#include "interfaces/HMI_API.h"

namespace test {
namespace components {
namespace event_engine_test {

using application_manager::event_engine::EventDispatcherImpl;
using application_manager::event_engine::Event;
using application_manager::event_engine::EventObserver;
using testing::_;

class EventEngineTest : public testing::Test {
 public:
  EventEngineTest()
      : event_id(Event::EventID::BasicCommunication_ActivateApp)
      , event_id2(Event::EventID::BasicCommunication_OnAppActivated)
      , event_id3(Event::EventID::VR_IsReady)
      , event_observer_mock_(mock_event_dispatcher_) {}

 protected:
  EventDispatcherImpl* event_dispatcher_instance_;
  Event* event_;
  const application_manager::event_engine::Event::EventID event_id;
  const application_manager::event_engine::Event::EventID event_id2;
  const application_manager::event_engine::Event::EventID event_id3;
  MockEventDispatcher mock_event_dispatcher_;
  MockEventObserver event_observer_mock_;
  const int32_t correlation_id = 1121;
  smart_objects::SmartObject smart_object_with_type_notification;
  smart_objects::SmartObject smart_object_with_type_response;
  smart_objects::SmartObject smart_object_with_type_error_response;
  smart_objects::SmartObject smart_object_with_type_request;
  smart_objects::SmartObject smart_object_with_invalid_type;

  virtual void SetUp() OVERRIDE {
    event_dispatcher_instance_ = new EventDispatcherImpl();
    event_ = new Event(hmi_apis::FunctionID::eType::VR_IsReady);
    smart_object_with_type_notification["params"]["message_type"] =
        hmi_apis::messageType::notification;
    smart_object_with_type_notification["params"]["correlation_id"] =
        correlation_id;
    smart_object_with_type_notification["params"]["function_id"] =
        hmi_apis::FunctionID::eType::VR_IsReady;

    smart_object_with_type_response["params"]["message_type"] =
        hmi_apis::messageType::response;
    smart_object_with_type_response["params"]["correlation_id"] =
        correlation_id;
    smart_object_with_type_response["params"]["function_id"] =
        hmi_apis::FunctionID::eType::VR_IsReady;

    smart_object_with_type_error_response["params"]["message_type"] =
        hmi_apis::messageType::error_response;
    smart_object_with_type_error_response["params"]["correlation_id"] =
        correlation_id;
    smart_object_with_type_error_response["params"]["function_id"] =
        hmi_apis::FunctionID::eType::VR_IsReady;

    smart_object_with_type_request["params"]["message_type"] =
        hmi_apis::messageType::request;
    smart_object_with_type_request["params"]["correlation_id"] = correlation_id;
    smart_object_with_type_request["params"]["function_id"] =
        hmi_apis::FunctionID::eType::VR_IsReady;

    smart_object_with_invalid_type["params"]["message_type"] =
        hmi_apis::messageType::INVALID_ENUM;
    smart_object_with_invalid_type["params"]["correlation_id"] = correlation_id;
    smart_object_with_invalid_type["params"]["function_id"] =
        hmi_apis::FunctionID::eType::VR_IsReady;
  }

  void TearDown() OVERRIDE {
    delete event_dispatcher_instance_;
    delete event_;
  }

  void CheckRaiseEvent(const Event::EventID& event_id,
                       const uint32_t calls_number,
                       const smart_objects::SmartObject& so) {
    // Arrange
    event_dispatcher_instance_->add_observer(
        event_id, correlation_id, event_observer_mock_);
    event_->set_smart_object(so);
    EXPECT_CALL(event_observer_mock_, on_event(_)).Times(calls_number);
    event_dispatcher_instance_->raise_event(*event_);
  }
};

TEST_F(EventEngineTest, EventObserverTest_ExpectObserversEmpty) {
  // Arrange
  EventObserver* event_observer_ptr =
      static_cast<EventObserver*>(&event_observer_mock_);
  // Check
  EXPECT_EQ(reinterpret_cast<unsigned long>(event_observer_ptr),
            event_observer_mock_.id());
}

TEST_F(EventEngineTest,
       EventDispatcherImpl_RaiseEvent_EventSOTypeResponse_ExpectEventRaised) {
  CheckRaiseEvent(event_id3, 1u, smart_object_with_type_response);
}

TEST_F(
    EventEngineTest,
    EventDispatcherImpl_RaiseEvent_EventSOTypeErrorResponse_ExpectEventRaised) {
  CheckRaiseEvent(event_id3, 1u, smart_object_with_type_error_response);
}

TEST_F(EventEngineTest,
       EventDispatcherImpl_RaiseEvent_EventSOTypeInvalid_ExpectEventNotRaised) {
  CheckRaiseEvent(event_id3, 0u, smart_object_with_invalid_type);
}

TEST_F(EventEngineTest,
       EventDispatcherImpl_RaiseEvent_EventSOTypeRequest_ExpectEventNotRaised) {
  CheckRaiseEvent(event_id3, 0u, smart_object_with_type_request);
}

TEST_F(
    EventEngineTest,
    EventDispatcherImpl_RaiseEvent_EventSOTypeNotification_ExpectEventNotRaised) {
  CheckRaiseEvent(event_id3, 0u, smart_object_with_type_notification);
}

TEST_F(EventEngineTest, Event_set_smart_object_ExpectObjectSet) {
  // Act
  event_->set_smart_object(smart_object_with_type_notification);
  const int32_t obj_type =
      static_cast<int32_t>(hmi_apis::messageType::notification);
  const int32_t function_id =
      static_cast<int32_t>(hmi_apis::FunctionID::eType::VR_IsReady);
  // Checks
  EXPECT_EQ(obj_type, event_->smart_object_type());
  EXPECT_EQ(function_id, event_->smart_object_function_id());
  EXPECT_EQ(correlation_id, event_->smart_object_correlation_id());
  EXPECT_EQ(smart_object_with_type_notification, event_->smart_object());
}

}  // namespace event_engine
}  // namespace components
}  // namespace test
