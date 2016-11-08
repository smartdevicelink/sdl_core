/*
 * Copyright (c) 2015, Ford Motor Company
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

#include "application_manager/policies/policy_event_observer.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

#include "gmock/gmock.h"
#include "policy/policy_types.h"
#include "smart_objects/smart_object.h"
#include "application_manager/mock_event_dispatcher.h"

namespace test {
namespace components {
namespace policy_test {

using application_manager::event_engine::Event;
using namespace policy;

using testing::_;
using ::testing::Return;

class PolicyEventObserverTest : public ::testing::Test {
 public:
  PolicyEventObserverTest()
      : policy_event_observer_(NULL), event_(NULL), field_name("odometer") {}

 protected:
  MockPolicyHandlerInterface policy_handler_mock_;
  event_engine_test::MockEventDispatcher mock_event_dispatcher_;
  PolicyEventObserver* policy_event_observer_;
  Event* event_;
  smart_objects::SmartObject smart_object_;
  const std::string field_name;
  const int field_value = 100;

  virtual void SetUp() OVERRIDE {
    policy_event_observer_ =
        new PolicyEventObserver(&policy_handler_mock_, mock_event_dispatcher_);
  }

  void TearDown() OVERRIDE {
    delete policy_event_observer_;
    DeleteEvent();
  }

  void CreateEvent(const Event::EventID& event_id) {
    event_ = new Event(event_id);
  }

  void CookSmartObject(const hmi_apis::Common_Result::eType& result,
                       const std::string& msg_params_field,
                       int msg_params_field_value) {
    smart_object_["params"]["code"] = result;
    smart_object_["msg_params"][msg_params_field] = msg_params_field_value;
  }

  void CheckResultsOnEvent(uint32_t pt_updated_calls_number,
                           uint32_t on_system_ready_calls_number) {
    event_->set_smart_object(smart_object_);
    EXPECT_CALL(policy_handler_mock_,
                PTUpdatedAt(Counters::KILOMETERS, field_value))
        .Times(pt_updated_calls_number);
    EXPECT_CALL(policy_handler_mock_, OnSystemReady())
        .Times(on_system_ready_calls_number);
    policy_event_observer_->on_event(*event_);
  }

  void DeleteEvent() {
    delete event_;
  }
};

TEST_F(PolicyEventObserverTest, OnEvent_EventInvalid_ExpectNoProcessingEvent) {
  // Arrange
  CreateEvent(Event::EventID::INVALID_ENUM);
  CookSmartObject(hmi_apis::Common_Result::SUCCESS, field_name, field_value);
  CheckResultsOnEvent(0u, 0u);
}

TEST_F(PolicyEventObserverTest,
       OnEvent_EventInvalidCommonResult_ExpectNoProcessingEvent) {
  // Arrange
  CreateEvent(Event::EventID::VehicleInfo_GetVehicleData);
  CookSmartObject(
      hmi_apis::Common_Result::INVALID_DATA, field_name, field_value);
  // Check
  CheckResultsOnEvent(0u, 0u);
}

TEST_F(PolicyEventObserverTest,
       OnEvent_EventGetVehicleData_ExpectProcessOdometerEvent) {
  // Arrange
  CreateEvent(Event::EventID::VehicleInfo_GetVehicleData);
  CookSmartObject(hmi_apis::Common_Result::SUCCESS, field_name, field_value);
  // Check
  CheckResultsOnEvent(1u, 0u);
}

TEST_F(PolicyEventObserverTest,
       OnEvent_EventBasicCommunication_OnReady_ExpectOnSystemReady) {
  // Arrange
  CreateEvent(Event::EventID::BasicCommunication_OnReady);
  CookSmartObject(hmi_apis::Common_Result::SUCCESS, field_name, field_value);
  // Check
  CheckResultsOnEvent(0u, 1u);
}

}  // namespace policy_event_observer
}  // namespace components
}  // namespace test
