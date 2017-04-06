/*
 * Copyright (c) 2017, Ford Motor Company
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
#include "application_manager/commands/mobile/show_constant_tbt_request.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "utils/shared_ptr.h"
#include "application_manager/commands/command_request_test.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace show_constant_tbt_request {

using application_manager::commands::MessageSharedPtr;
using application_manager::commands::ShowConstantTBTRequest;
using application_manager::MockMessageHelper;
using test::components::policy_test::MockPolicyHandlerInterface;
using test::components::application_manager_test::MockHMICapabilities;
using utils::SharedPtr;
using testing::_;
using testing::Return;
using testing::ReturnRef;

namespace strings = application_manager::strings;
namespace hmi_request = application_manager::hmi_request;
namespace hmi_response = application_manager::hmi_response;
namespace mobile_result = mobile_apis::Result;

namespace {
const uint32_t kConnectionKey = 1u;
const uint32_t kAppID = 2u;
const std::string kWrongSyntax = "Wrong Syntax\\n";
}  // namespace

class ShowConstantTBTRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  ShowConstantTBTRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock()) {
    mock_app_ = CreateMockApp();
  }

  MessageSharedPtr CreateMsgParams() {
    MessageSharedPtr msg = CreateMessage();
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;
    return msg;
  }

  void GeneralExpectationsSetupHelper(SmartObject& msg_params) {
    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(app_mngr_, GetPolicyHandler())
        .WillOnce(ReturnRef(mock_policy_handler_));
    EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(msg_params, _, _, _))
        .WillOnce(Return(mobile_apis::Result::SUCCESS));
    EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));
  }

  void MsgParamsSetupHelper(SmartObject& msg_params,
                            const char* field,
                            hmi_apis::Common_TextFieldName::eType text_field) {
    msg_params[strings::app_id] = kAppID;
    msg_params[hmi_request::navi_texts] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    msg_params.erase(field);
    msg_params[hmi_request::navi_texts][0][hmi_request::field_name] =
        static_cast<int32_t>(text_field);
    msg_params[hmi_request::navi_texts][0][hmi_request::field_text] =
        navi_text_;

    EXPECT_CALL(*mock_app_, set_tbt_show_command(msg_params));
  }

  MockAppPtr mock_app_;
  MockMessageHelper& mock_message_helper_;
  MockPolicyHandlerInterface mock_policy_handler_;
  MockHMICapabilities mock_hmi_capabilities_;
  std::string navi_text_;
};

TEST_F(ShowConstantTBTRequestTest, Run_TurnIcon_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject turn_icon(smart_objects::SmartType_Map);
  turn_icon[strings::value] = "1";
  msg_params[strings::turn_icon] = turn_icon;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  GeneralExpectationsSetupHelper(msg_params);
  EXPECT_CALL(mock_message_helper_, VerifyImage(turn_icon, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  msg_params[strings::app_id] = kAppID;
  msg_params[hmi_request::navi_texts] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  EXPECT_CALL(*mock_app_, set_tbt_show_command(msg_params));

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_TurnIcon_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject turn_icon(smart_objects::SmartType_Map);
  turn_icon[strings::value] = "1";
  msg_params[strings::turn_icon] = turn_icon;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler_));
  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(msg_params, _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(mock_message_helper_, VerifyImage(turn_icon, _, _))
      .WillOnce(Return(mobile_apis::Result::REJECTED));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);
  EXPECT_CALL(*mock_app_, set_tbt_show_command(_)).Times(0);
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_NextTurnIcon_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject next_turn_icon(smart_objects::SmartType_Map);
  next_turn_icon[strings::value] = "1";
  msg_params[strings::next_turn_icon] = next_turn_icon;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  GeneralExpectationsSetupHelper(msg_params);
  EXPECT_CALL(mock_message_helper_, VerifyImage(next_turn_icon, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  msg_params[strings::app_id] = kAppID;
  msg_params[hmi_request::navi_texts] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  EXPECT_CALL(*mock_app_, set_tbt_show_command(msg_params));

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_NextTurnIcon_UNSUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject next_turn_icon(smart_objects::SmartType_Map);
  next_turn_icon[strings::value] = "invalid_turn_icon\t\n";
  msg_params[strings::next_turn_icon] = next_turn_icon;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_TurnIcon_UNSUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject turn_icon(smart_objects::SmartType_Map);
  turn_icon[strings::value] = "invalid_turn_icon\t\n";
  msg_params[strings::turn_icon] = turn_icon;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(msg_params, _, _, _))
      .Times(0);

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_NextTurnIcon_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject next_turn_icon(smart_objects::SmartType_Map);
  next_turn_icon[strings::value] = "1";
  msg_params[strings::next_turn_icon] = next_turn_icon;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler_));
  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(msg_params, _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(mock_message_helper_, VerifyImage(next_turn_icon, _, _))
      .WillOnce(Return(mobile_apis::Result::REJECTED));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);
  EXPECT_CALL(*mock_app_, set_tbt_show_command(_)).Times(0);
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_NavigationText1_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  navi_text_ = "Navigation_Text_1";
  msg_params[strings::navigation_text_1] = navi_text_;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  GeneralExpectationsSetupHelper(msg_params);

  MsgParamsSetupHelper(msg_params,
                       strings::navigation_text_1,
                       hmi_apis::Common_TextFieldName::navigationText1);

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_NavigationText2_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  navi_text_ = "Navigation_Text_2";
  msg_params[strings::navigation_text_2] = navi_text_;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  GeneralExpectationsSetupHelper(msg_params);

  MsgParamsSetupHelper(msg_params,
                       strings::navigation_text_2,
                       hmi_apis::Common_TextFieldName::navigationText2);

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest,
       Run_NavigationText2ContainsWhiteSpace_UNSUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  navi_text_ = "invalid_Navigation_Text_2\t\t\n";
  msg_params[strings::navigation_text_2] = navi_text_;
  (*msg)[strings::msg_params] = msg_params;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(msg_params, _, _, _))
      .Times(0);

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_ETA_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  navi_text_ = "ETA";
  msg_params[strings::eta] = navi_text_;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  GeneralExpectationsSetupHelper(msg_params);

  MsgParamsSetupHelper(
      msg_params, strings::eta, hmi_apis::Common_TextFieldName::ETA);

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_ETAContainsWhiteSpace_UNSUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  navi_text_ = "ETA\t\nn";
  msg_params[strings::eta] = navi_text_;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(msg_params, _, _, _))
      .Times(0);

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_TotalDistance_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  navi_text_ = "Total_Distance";
  msg_params[strings::total_distance] = navi_text_;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  GeneralExpectationsSetupHelper(msg_params);

  MsgParamsSetupHelper(msg_params,
                       strings::total_distance,
                       hmi_apis::Common_TextFieldName::totalDistance);

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest,
       Run_TotalDistanceContainsWhiteSpace_UNSUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  (*msg)[strings::msg_params][strings::total_distance] = kWrongSyntax;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_TimeToDistanation_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  navi_text_ = "Time_to_Destination";
  msg_params[strings::time_to_destination] = navi_text_;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  GeneralExpectationsSetupHelper(msg_params);

  MsgParamsSetupHelper(msg_params,
                       strings::time_to_destination,
                       hmi_apis::Common_TextFieldName::timeToDestination);

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest,
       Run_TimeToDestinationContainsWhiteSpace_UNSUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params[strings::time_to_destination] = "\\n";
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(msg_params, _, _, _))
      .Times(0);
  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_SoftButtons_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  (*msg)[strings::params][strings::function_id] = kAppID;
  SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params[strings::soft_buttons] = SmartObject(smart_objects::SmartType_Map);
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  GeneralExpectationsSetupHelper(msg_params);

  msg_params[strings::app_id] = kAppID;
  msg_params[hmi_request::navi_texts] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  EXPECT_CALL(mock_message_helper_,
              SubscribeApplicationToSoftButton(msg_params, _, kAppID));
  EXPECT_CALL(*mock_app_, set_tbt_show_command(msg_params));

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_InvalidApp_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));
  EXPECT_CALL(app_mngr_, GetPolicyHandler()).Times(0);
  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(_, _, _, _)).Times(0);
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(*mock_app_, set_tbt_show_command(_)).Times(0);

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_EmptyMsgParams_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));
  EXPECT_CALL(app_mngr_, GetPolicyHandler()).Times(0);
  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(_, _, _, _)).Times(0);
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(*mock_app_, set_tbt_show_command(_)).Times(0);

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_WrongSyntax_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  (*msg)[strings::msg_params][strings::navigation_text_1] = "Wrong Syntax\\n";

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));
  EXPECT_CALL(app_mngr_, GetPolicyHandler()).Times(0);
  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(_, _, _, _)).Times(0);
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(*mock_app_, set_tbt_show_command(_)).Times(0);

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_UnsuccessfulProcessing_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  (*msg)[strings::msg_params][strings::navigation_text_1] = "Navigation_Text_1";

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler_));
  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(_, _, _, _))
      .WillOnce(Return(mobile_apis::Result::ABORTED));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(*mock_app_, set_tbt_show_command(_)).Times(0);

  command->Run();
}

TEST_F(ShowConstantTBTRequestTest, OnEvent_WrongFunctionID_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();

  SharedPtr<ShowConstantTBTRequest> command(
      CreateCommand<ShowConstantTBTRequest>(msg));

  EXPECT_CALL(app_mngr_, hmi_capabilities()).Times(0);
  EXPECT_CALL(mock_hmi_capabilities_, is_ui_cooperating()).Times(0);
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  command->on_event(event);
}

}  // namespace show_constant_tbt_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
