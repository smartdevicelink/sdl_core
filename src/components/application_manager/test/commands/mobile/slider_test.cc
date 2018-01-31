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
#include <set>

#include "application_manager/commands/mobile/slider_request.h"

#include "gtest/gtest.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace slider_request {

namespace am = application_manager;
using am::commands::SliderRequest;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using am::MockMessageHelper;
using policy_test::MockPolicyHandlerInterface;
using ::utils::SharedPtr;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

typedef SharedPtr<SliderRequest> CommandPtr;

namespace {
const int32_t kCommandId = 1;
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
const uint32_t kDefaultTimeout = 1000u;
const uint32_t kCorrelationId = 2u;
const uint32_t kFunctionId = 3u;
const uint32_t kNumTicks = 2u;
const uint32_t kPositionGreaterTicks = 3u;
const uint32_t kPositionLessTicks = 1u;
}  // namespace

class SliderRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SliderRequestTest()
      : mock_app_(CreateMockApp())
      , msg_(CreateMessage(smart_objects::SmartType_Map)) {}

  MessageSharedPtr CreateFullParamsUISO() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    smart_objects::SmartObject menu_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    menu_params[am::strings::position] = 10;
    menu_params[am::strings::menu_name] = "LG";

    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params[am::strings::cmd_id] = kCmdId;
    msg_params[am::strings::menu_params] = menu_params;
    msg_params[am::strings::app_id] = kAppId;
    msg_params[am::strings::cmd_icon] = 1;
    msg_params[am::strings::cmd_icon][am::strings::value] = "10";
    (*msg)[am::strings::msg_params] = msg_params;

    return msg;
  }

  void PreConditions() {
    (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    (*msg_)[am::strings::msg_params][am::strings::num_ticks] = kNumTicks;
    (*msg_)[am::strings::msg_params][am::strings::position] =
        kPositionLessTicks;
    (*msg_)[am::strings::msg_params][am::strings::slider_footer][0] =
        "slider_footer1";
    (*msg_)[am::strings::msg_params][am::strings::slider_footer][1] =
        "slider_footer2";
    (*msg_)[am::strings::msg_params][am::strings::slider_header] =
        "slider_header";

    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
  }

  void ExpectManageMobileCommandWithResultCode(
      const mobile_apis::Result::eType code) {
    ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
    EXPECT_CALL(
        rpc_service_,
        ManageMobileCommand(MobileResultCodeIs(code),
                            am::commands::Command::CommandOrigin::ORIGIN_SDL));
  }

  sync_primitives::Lock lock_;

  MockAppPtr mock_app_;
  MessageSharedPtr msg_;
  MockPolicyHandlerInterface mock_policy_handler_;
};

TEST_F(SliderRequestTest, OnEvent_UI_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg_ui = CreateFullParamsUISO();
  (*msg_ui)[am::strings::params][am::strings::connection_key] = kConnectionKey;

  utils::SharedPtr<SliderRequest> command =
      CreateCommand<SliderRequest>(msg_ui);

  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));

  ON_CALL(*mock_app, app_id()).WillByDefault(Return(kConnectionKey));

  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg)[am::strings::msg_params][am::strings::info] = "info";

  Event event(hmi_apis::FunctionID::UI_Slider);
  event.set_smart_object(*msg);

  MessageSharedPtr ui_command_result;
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));

  command->on_event(event);

  EXPECT_EQ((*ui_command_result)[am::strings::msg_params][am::strings::success]
                .asBool(),
            true);
  EXPECT_EQ(
      (*ui_command_result)[am::strings::msg_params][am::strings::result_code]
          .asInt(),
      static_cast<int32_t>(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE));
  if ((*ui_command_result)[am::strings::msg_params].keyExists(
          am::strings::info)) {
    EXPECT_FALSE(
        (*ui_command_result)[am::strings::msg_params][am::strings::info]
            .asString()
            .empty());
  }
}

class CallOnTimeOut {
 public:
  CallOnTimeOut(CommandRequestImpl& command) : command_(command) {}

  void operator()() {
    command_.onTimeOut();
  }

  CommandRequestImpl& command_;
};

TEST_F(SliderRequestTest, Init_SUCCESS) {
  PreConditions();
  (*msg_)[am::strings::msg_params][am::strings::timeout] = kDefaultTimeout;

  CommandPtr command(CreateCommand<SliderRequest>(msg_));
  EXPECT_TRUE(command->Init());
}

TEST_F(SliderRequestTest, Run_ApplicationIsNotRegistered_UNSUCCESS) {
  PreConditions();
  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(ApplicationSharedPtr()));
  ExpectManageMobileCommandWithResultCode(
      mobile_apis::Result::APPLICATION_NOT_REGISTERED);

  CommandPtr command(CreateCommand<SliderRequest>(msg_));
  command->Run();
}

TEST_F(SliderRequestTest, Run_PositionGreaterTicks_UNSUCCESS) {
  PreConditions();
  (*msg_)[am::strings::msg_params][am::strings::position] =
      kPositionGreaterTicks;

  CommandPtr command(CreateCommand<SliderRequest>(msg_));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);
  command->Run();
}

TEST_F(SliderRequestTest, Run_SliderFooterNotEqToNumticks_UNSUCCESS) {
  PreConditions();
  (*msg_)[am::strings::msg_params][am::strings::slider_footer][2] =
      "slider_footer3";
  CommandPtr command(CreateCommand<SliderRequest>(msg_));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);
  command->Run();
}

TEST_F(SliderRequestTest, Run_InvalidSliderHeader_UNSUCCESS) {
  PreConditions();
  (*msg_)[am::strings::msg_params][am::strings::slider_header] =
      "invalid_test_with_empty_str\\n";

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);

  CommandPtr command(CreateCommand<SliderRequest>(msg_));
  command->Run();
}

TEST_F(SliderRequestTest, Run_InvalidSliderFooter_UNSUCCESS) {
  PreConditions();
  (*msg_)[am::strings::msg_params][am::strings::slider_footer][0] =
      "invalid_test_with_empty_str\\n";

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);

  CommandPtr command(CreateCommand<SliderRequest>(msg_));
  command->Run();
}

TEST_F(SliderRequestTest, Run_SUCCESS) {
  PreConditions();
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_Slider)));

  CommandPtr command(CreateCommand<SliderRequest>(msg_));
  command->Run();
}

TEST_F(SliderRequestTest, OnEvent_UI_OnResetTimeout_UNSUCCESS) {
  PreConditions();
  (*msg_)[am::strings::msg_params][am::strings::timeout] = kDefaultTimeout;
  (*msg_)[am::strings::params][am::strings::correlation_id] = kCorrelationId;

  CommandPtr command(CreateCommand<SliderRequest>(msg_));
  EXPECT_TRUE(command->Init());

  EXPECT_CALL(
      app_mngr_,
      updateRequestTimeout(kConnectionKey, kCorrelationId, kDefaultTimeout));

  Event event(hmi_apis::FunctionID::UI_OnResetTimeout);
  event.set_smart_object(*msg_);
  command->on_event(event);
}

TEST_F(SliderRequestTest, OnEvent_UI_UnknownEventId_UNSUCCESS) {
  PreConditions();
  EXPECT_CALL(app_mngr_, GetRPCService()).Times(0);
  EXPECT_CALL(rpc_service_, ManageMobileCommand(_, _)).Times(0);

  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  event.set_smart_object(*msg_);

  CommandPtr command(CreateCommand<SliderRequest>(msg_));
  command->on_event(event);
}

TEST_F(SliderRequestTest, OnEvent_UISliderPositionExists_SUCCESS) {
  PreConditions();
  (*msg_)[am::strings::msg_params][am::strings::timeout] = kDefaultTimeout;
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::TIMED_OUT;
  (*msg_)[am::strings::params][am::strings::data]
         [am::strings::slider_position] = "position";

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::TIMED_OUT);

  Event event(hmi_apis::FunctionID::UI_Slider);
  event.set_smart_object(*msg_);

  CommandPtr command(CreateCommand<SliderRequest>(msg_));
  command->on_event(event);
}

TEST_F(SliderRequestTest, OnEvent_UISliderAborted_SUCCESS) {
  PreConditions();
  (*msg_)[am::strings::msg_params][am::strings::timeout] = kDefaultTimeout;
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::ABORTED;

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::ABORTED);

  Event event(hmi_apis::FunctionID::UI_Slider);
  event.set_smart_object(*msg_);

  CommandPtr command(CreateCommand<SliderRequest>(msg_));
  command->on_event(event);
}

}  // namespace slider_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests
