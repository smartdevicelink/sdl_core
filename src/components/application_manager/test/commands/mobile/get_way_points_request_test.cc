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
#include "utils/shared_ptr.h"
#include "commands/commands_test.h"
#include "commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "mobile/get_way_points_request.h"
#include "application_manager/smart_object_keys.h"
#include "mock_message_helper.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_message_helper.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace get_way_points_request {

using namespace mobile_apis::Result;
using ::testing::Return;
using ::testing::Mock;
using ::testing::_;
using application_manager::commands::GetWayPointsRequest;
using application_manager::MockMessageHelper;
using application_manager::MockHmiInterfaces;

typedef SharedPtr<GetWayPointsRequest> CommandPtr;
typedef mobile_apis::Result::eType MobileResult;

namespace {
const uint32_t kCorrelationId = 2u;
const uint32_t kAppId = 3u;
const uint32_t kConnectionKey = kAppId;
const std::string kMethodName = "Navigation.GetWayPoints";
}

class GetWayPointsRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  GetWayPointsRequestTest()
      : message_helper_mock_(*am::MockMessageHelper::message_helper_mock())
      , mock_app_(CreateMockApp()) {
    Mock::VerifyAndClearExpectations(&message_helper_mock_);
  }
  ~GetWayPointsRequestTest() {
    Mock::VerifyAndClearExpectations(&message_helper_mock_);
  }

  void SetUp() OVERRIDE {
    message_ = utils::MakeShared<SmartObject>(::smart_objects::SmartType_Map);
    (*message_)[am::strings::msg_params] =
        ::smart_objects::SmartObject(::smart_objects::SmartType_Map);

    command_sptr_ =
        CreateCommand<application_manager::commands::GetWayPointsRequest>(
            message_);
    ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app_));
  }
  MockMessageHelper& message_helper_mock_;
  MockAppPtr mock_app_;
  MessageSharedPtr message_;
  utils::SharedPtr<application_manager::commands::GetWayPointsRequest>
      command_sptr_;
};

class GetWayPointsRequestOnEventTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  GetWayPointsRequestOnEventTest()
      : message_helper_mock_(*am::MockMessageHelper::message_helper_mock())
      , app_(CreateMockApp()) {
    Mock::VerifyAndClearExpectations(&message_helper_mock_);
  }
  ~GetWayPointsRequestOnEventTest() {
    Mock::VerifyAndClearExpectations(&message_helper_mock_);
  }

  void CheckOnEventResponse(const std::string& wayPointsParam,
                            const MobileResult ResultCode,
                            const bool success) {
    Event event(Event::EventID::Navigation_GetWayPoints);
    CommandPtr command(CreateCommand<GetWayPointsRequest>());
    MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
    (*event_msg)[am::strings::params][am::hmi_response::code] = ResultCode;
    if ("0" == wayPointsParam) {
      (*event_msg)[am::strings::msg_params] = 0;
    } else {
      (*event_msg)[am::strings::msg_params][am::strings::way_points][0]["123"] =
          wayPointsParam;
    }

    event.set_smart_object(*event_msg);

    MessageSharedPtr result_msg(
        CatchMobileCommandResult(CallOnEvent(*command, event)));
    EXPECT_EQ(
        ResultCode,
        static_cast<mobile_apis::Result::eType>(
            (*result_msg)[am::strings::msg_params][am::strings::result_code]
                .asInt()));
    EXPECT_EQ(
        success,
        (*result_msg)[am::strings::msg_params][am::strings::success].asBool());
  }

 protected:
  MockMessageHelper& message_helper_mock_;
  MockAppPtr app_;
  MockHmiInterfaces hmi_interfaces_;
};

TEST_F(GetWayPointsRequestTest,
       Run_InvalidApp_ApplicationNotRegisteredResponce) {
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  utils::SharedPtr<am::Application> null_application_sptr;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(null_application_sptr));

  CallRun caller(*command_sptr_);

  MessageSharedPtr result_message = CatchMobileCommandResult(caller);

  const mobile_apis::Result::eType result =
      static_cast<mobile_apis::Result::eType>(
          (*result_message)[am::strings::msg_params][am::strings::result_code]
              .asInt());

  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED, result);
}

TEST_F(GetWayPointsRequestTest, Run_ApplicationRegistered_Success) {
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  MockAppPtr application_sptr = CreateMockApp();

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(application_sptr));
  EXPECT_CALL(*application_sptr, app_id()).WillOnce(Return(1));

  EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId));

  CallRun caller(*command_sptr_);

  MessageSharedPtr result_message = CatchHMICommandResult(caller);

  const hmi_apis::FunctionID::eType result_function_id =
      static_cast<hmi_apis::FunctionID::eType>(
          (*result_message)[am::strings::params][am::strings::function_id]
              .asInt());

  EXPECT_EQ(hmi_apis::FunctionID::Navigation_GetWayPoints, result_function_id);
  EXPECT_EQ(kCorrelationId,
            (*result_message)[am::strings::params][am::strings::correlation_id]
                .asUInt());
}

TEST_F(GetWayPointsRequestTest,
       OnEvent_NavigationGetWayPointsEvent_SendResponce) {
  am::event_engine::Event event(hmi_apis::FunctionID::Navigation_GetWayPoints);

  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;

  event.set_smart_object(*message_);
  EXPECT_CALL(message_helper_mock_, HMIToMobileResult(_))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  CallOnEvent caller(*command_sptr_, event);

  MessageSharedPtr result_message = CatchMobileCommandResult(caller);

  const mobile_apis::Result::eType result =
      static_cast<mobile_apis::Result::eType>(
          (*result_message)[am::strings::msg_params][am::strings::result_code]
              .asInt());

  EXPECT_EQ(mobile_apis::Result::SUCCESS, result);
}

TEST_F(GetWayPointsRequestTest, OnEvent_DefaultCase) {
  am::event_engine::Event event(hmi_apis::FunctionID::INVALID_ENUM);

  event.set_smart_object(*message_);

  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);

  CallOnEvent caller(*command_sptr_, event);
  caller();
}

TEST_F(GetWayPointsRequestOnEventTest, OnEvent_WrongEventId_UNSUCCESS) {
  Event event(Event::EventID::INVALID_ENUM);
  CommandPtr command(CreateCommand<GetWayPointsRequest>());

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);
  command->on_event(event);
}

TEST_F(GetWayPointsRequestOnEventTest, OnEvent_Expect_SUCCESS_Case1) {
  EXPECT_CALL(message_helper_mock_, HMIToMobileResult(_))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  CheckOnEventResponse("0", SUCCESS, true);
}

TEST_F(GetWayPointsRequestOnEventTest, OnEvent_Expect_SUCCESS_Case2) {
  EXPECT_CALL(message_helper_mock_, HMIToMobileResult(_))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  CheckOnEventResponse("", SUCCESS, true);
}

TEST_F(GetWayPointsRequestOnEventTest, OnEvent_Expect_SUCCESS_Case3) {
  EXPECT_CALL(message_helper_mock_, HMIToMobileResult(_))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  CheckOnEventResponse("test", SUCCESS, true);
}

TEST_F(GetWayPointsRequestOnEventTest, OnEvent_Expect_GENERIC_ERROR_Case1) {
  EXPECT_CALL(app_mngr_, hmi_interfaces()).WillOnce(ReturnRef(hmi_interfaces_));
  EXPECT_CALL(hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_Navigation))
      .WillOnce(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(message_helper_mock_, HMIToMobileResult(_))
      .WillOnce(Return(mobile_apis::Result::GENERIC_ERROR));
  CheckOnEventResponse("    ", GENERIC_ERROR, false);
}

TEST_F(GetWayPointsRequestOnEventTest, OnEvent_Expect_GENERIC_ERROR_Case2) {
  EXPECT_CALL(app_mngr_, hmi_interfaces()).WillOnce(ReturnRef(hmi_interfaces_));
  EXPECT_CALL(hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_Navigation))
      .WillOnce(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(message_helper_mock_, HMIToMobileResult(_))
      .WillOnce(Return(mobile_apis::Result::GENERIC_ERROR));
  CheckOnEventResponse("test\t", GENERIC_ERROR, false);
}

TEST_F(GetWayPointsRequestOnEventTest, OnEvent_Expect_GENERIC_ERROR_Case3) {
  EXPECT_CALL(app_mngr_, hmi_interfaces()).WillOnce(ReturnRef(hmi_interfaces_));
  EXPECT_CALL(hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_Navigation))
      .WillOnce(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(message_helper_mock_, HMIToMobileResult(_))
      .WillOnce(Return(mobile_apis::Result::GENERIC_ERROR));
  CheckOnEventResponse("test\n", GENERIC_ERROR, false);
}

TEST_F(GetWayPointsRequestOnEventTest, OnEvent_Expect_GENERIC_ERROR_Case4) {
  EXPECT_CALL(app_mngr_, hmi_interfaces()).WillOnce(ReturnRef(hmi_interfaces_));
  EXPECT_CALL(hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_Navigation))
      .WillOnce(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(message_helper_mock_, HMIToMobileResult(_))
      .WillOnce(Return(mobile_apis::Result::GENERIC_ERROR));
  CheckOnEventResponse("test\t\n", GENERIC_ERROR, false);
}

}  // namespace get_way_points_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
