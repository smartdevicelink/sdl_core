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

#include <stdint.h>
#include <string>

#include "gtest/gtest.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"

#include "mobile/get_way_points_request.h"

#include "utils/shared_ptr.h"
#include "utils/make_shared.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {

using namespace application_manager;

using ::testing::Return;
using ::testing::_;

namespace {
const uint32_t kCorrelationId = 2u;
const uint32_t kAppId = 3u;
const uint32_t kConnectionKey = kAppId;
const std::string kMethodName = "Navigation.GetWayPoints";
}

class GetWayPointsRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  void SetUp() OVERRIDE {
    message_ = utils::MakeShared<SmartObject>(::smart_objects::SmartType_Map);
    (*message_)[strings::msg_params] =
        ::smart_objects::SmartObject(::smart_objects::SmartType_Map);

    command_sptr_ =
        CreateCommand<application_manager::commands::GetWayPointsRequest>(
            message_);
    mock_app_ = CreateMockApp();
    ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(mock_app_));
  }

  MockAppPtr mock_app_;
  MessageSharedPtr message_;
  utils::SharedPtr<commands::GetWayPointsRequest> command_sptr_;
};

TEST_F(GetWayPointsRequestTest,
       Run_InvalidApp_ApplicationNotRegisteredResponce) {
  (*message_)[strings::params][strings::connection_key] = kConnectionKey;

  utils::SharedPtr<Application> null_application_sptr;
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(null_application_sptr));

  CallRun caller(*command_sptr_);

  MessageSharedPtr result_message = CatchMobileCommandResult(caller);

  const mobile_api::Result::eType result =
      static_cast<mobile_api::Result::eType>(
          (*result_message)[strings::msg_params][strings::result_code].asInt());

  EXPECT_EQ(mobile_api::Result::APPLICATION_NOT_REGISTERED, result);
}

TEST_F(GetWayPointsRequestTest, Run_ApplicationRegistered_Success) {
  (*message_)[strings::params][strings::connection_key] = kConnectionKey;

  MockAppPtr application_sptr = CreateMockApp();

  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(application_sptr));
  EXPECT_CALL(*application_sptr, app_id()).WillOnce(Return(1));

  EXPECT_CALL(mock_app_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId));

  CallRun caller(*command_sptr_);

  MessageSharedPtr result_message = CatchHMICommandResult(caller);

  const hmi_apis::FunctionID::eType result_function_id =
      static_cast<hmi_apis::FunctionID::eType>(
          (*result_message)[strings::params][strings::function_id].asInt());

  EXPECT_EQ(hmi_apis::FunctionID::Navigation_GetWayPoints, result_function_id);
  EXPECT_EQ(
      kCorrelationId,
      (*result_message)[strings::params][strings::correlation_id].asUInt());
}

TEST_F(GetWayPointsRequestTest,
       OnEvent_NavigationGetWayPointsEvent_SendResponce) {
  event_engine::Event event(hmi_apis::FunctionID::Navigation_GetWayPoints);

  (*message_)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;

  event.set_smart_object(*message_);

  CallOnEvent caller(*command_sptr_, event);

  MessageSharedPtr result_message = CatchMobileCommandResult(caller);

  const mobile_api::Result::eType result =
      static_cast<mobile_api::Result::eType>(
          (*result_message)[strings::msg_params][strings::result_code].asInt());

  EXPECT_EQ(mobile_api::Result::SUCCESS, result);
}

TEST_F(GetWayPointsRequestTest, OnEvent_DefaultCase) {
  event_engine::Event event(hmi_apis::FunctionID::INVALID_ENUM);

  event.set_smart_object(*message_);

  EXPECT_CALL(mock_app_manager_, updateRequestTimeout(_, _, _)).Times(0);
  EXPECT_CALL(mock_app_manager_, ManageHMICommand(_)).Times(0);

  CallOnEvent caller(*command_sptr_, event);
  caller();
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
