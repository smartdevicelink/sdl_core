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

#include "application_manager/event_engine/event.h"
#include "application_manager/event_engine/event_dispatcher_impl.h"
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/message.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_event_observer.h"
#include "smart_objects/smart_object.h"

#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace event_engine_test {

using application_manager::event_engine::Event;
using application_manager::event_engine::EventDispatcherImpl;
using application_manager::event_engine::EventObserver;
using application_manager::event_engine::MobileEvent;
using testing::_;
using ::testing::An;
using ::testing::Matcher;

class MobileEventEngineTest : public testing::Test {
 public:
  MobileEventEngineTest()
      : event_id(MobileEvent::MobileEventID::GetAppServiceDataID)
      , event_observer_mock_(mock_event_dispatcher_) {}

 protected:
  EventDispatcherImpl* event_dispatcher_instance_;
  MobileEvent* event_;
  const MobileEvent::MobileEventID event_id;
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
    event_ =
        new MobileEvent(mobile_apis::FunctionID::eType::GetAppServiceDataID);
    smart_object_with_type_notification["params"]["message_type"] =
        application_manager::MessageType::kNotification;
    smart_object_with_type_notification["params"]["correlation_id"] =
        correlation_id;
    smart_object_with_type_notification["params"]["function_id"] =
        mobile_apis::FunctionID::eType::GetAppServiceDataID;

    smart_object_with_type_response["params"]["message_type"] =
        application_manager::MessageType::kResponse;
    smart_object_with_type_response["params"]["correlation_id"] =
        correlation_id;
    smart_object_with_type_response["params"]["function_id"] =
        mobile_apis::FunctionID::eType::GetAppServiceDataID;

    smart_object_with_type_request["params"]["message_type"] =
        application_manager::MessageType::kRequest;
    smart_object_with_type_request["params"]["correlation_id"] = correlation_id;
    smart_object_with_type_request["params"]["function_id"] =
        mobile_apis::FunctionID::eType::GetAppServiceDataID;

    smart_object_with_invalid_type["params"]["message_type"] =
        application_manager::MessageType::kUnknownType;
    smart_object_with_invalid_type["params"]["correlation_id"] = correlation_id;
    smart_object_with_invalid_type["params"]["function_id"] =
        mobile_apis::FunctionID::eType::GetAppServiceDataID;
  }

  void TearDown() OVERRIDE {
    delete event_dispatcher_instance_;
    delete event_;
  }

  void CheckRaiseMobileEvent(const MobileEvent::MobileEventID& event_id,
                             const uint32_t calls_number,
                             const smart_objects::SmartObject& so) {
    // Arrange
    event_dispatcher_instance_->add_mobile_observer(
        event_id, correlation_id, event_observer_mock_);
    event_->set_smart_object(so);
    EXPECT_CALL(event_observer_mock_, on_event(An<const MobileEvent&>()))
        .Times(calls_number);
    event_dispatcher_instance_->raise_mobile_event(*event_);
  }
};

TEST_F(MobileEventEngineTest, EventObserverTest_ExpectObserversEmpty) {
  // Arrange
  EventObserver* event_observer_ptr =
      static_cast<EventObserver*>(&event_observer_mock_);
  // Check
  EXPECT_EQ(reinterpret_cast<unsigned long>(event_observer_ptr),
            event_observer_mock_.id());
}

TEST_F(MobileEventEngineTest,
       EventDispatcherImpl_RaiseEvent_EventSOTypeResponse_ExpectEventRaised) {
  CheckRaiseMobileEvent(event_id, 1u, smart_object_with_type_response);
}

TEST_F(MobileEventEngineTest,
       EventDispatcherImpl_RaiseEvent_EventSOTypeInvalid_ExpectEventNotRaised) {
  CheckRaiseMobileEvent(event_id, 0u, smart_object_with_invalid_type);
}

TEST_F(MobileEventEngineTest,
       EventDispatcherImpl_RaiseEvent_EventSOTypeRequest_ExpectEventNotRaised) {
  CheckRaiseMobileEvent(event_id, 0u, smart_object_with_type_request);
}

TEST_F(
    MobileEventEngineTest,
    EventDispatcherImpl_RaiseEvent_EventSOTypeNotification_ExpectEventNotRaised) {
  CheckRaiseMobileEvent(event_id, 0u, smart_object_with_type_notification);
}

TEST_F(MobileEventEngineTest, Event_set_smart_object_ExpectObjectSet) {
  // Act
  event_->set_smart_object(smart_object_with_type_response);
  const int32_t obj_type =
      static_cast<int32_t>(application_manager::MessageType::kResponse);
  const int32_t function_id =
      static_cast<int32_t>(mobile_apis::FunctionID::eType::GetAppServiceDataID);
  // Checks
  EXPECT_EQ(obj_type, event_->smart_object_type());
  EXPECT_EQ(function_id, event_->smart_object_function_id());
  EXPECT_EQ(correlation_id, event_->smart_object_correlation_id());
  EXPECT_EQ(smart_object_with_type_response, event_->smart_object());
}

}  // namespace event_engine_test
}  // namespace components
}  // namespace test
