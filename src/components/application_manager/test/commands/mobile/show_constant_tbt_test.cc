/*
 * Copyright (c) 2018, Ford Motor Company
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

#include <string>

#include "application_manager/commands/mobile/show_constant_tbt_request.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/message_helper.h"
#include "gtest/gtest.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace show_constant_tbt_request {

namespace am = application_manager;
using am::commands::ShowConstantTBTRequest;
using ::utils::SharedPtr;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

using policy_test::MockPolicyHandlerInterface;

namespace {
const uint32_t kAppId = 1u;
const uint32_t kConnectionKey = 2u;
const std::string kNavigationText1 = "Some text1";
const std::string kNavigationText2 = "Some text2";
const std::string kEta = "Some time";
const std::string kTotalDistance = "Some distance";
const std::string kTimeToDestination = "Some time";
const std::string kTutnIconValue = "0x1A3F";
const std::string kNextTutnIconValue = "0x1A40";
}  // namespace

class ShowConstantTBTRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  MessageSharedPtr CreateMessageWithParams() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;

    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    msg_params[am::strings::turn_icon][am::strings::value] = kTutnIconValue;
    msg_params[am::strings::next_turn_icon][am::strings::value] =
        kNextTutnIconValue;
    msg_params[am::strings::navigation_text_1] = kNavigationText1;
    msg_params[am::strings::navigation_text_2] = kNavigationText2;
    msg_params[am::strings::eta] = kEta;
    msg_params[am::strings::total_distance] = kTotalDistance;
    msg_params[am::strings::time_to_destination] = kTimeToDestination;

    smart_objects::SmartObject soft_button =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    soft_button[am::strings::type] = am::mobile_api::SoftButtonType::SBT_TEXT;
    soft_button[am::strings::text] = "some text";
    soft_button[am::strings::soft_button_id] = 15u;

    smart_objects::SmartObject soft_buttons =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    soft_buttons[0] = soft_button;

    msg_params[am::strings::soft_buttons] = soft_buttons;
    (*msg)[am::strings::msg_params] = msg_params;

    return msg;
  }

  MessageSharedPtr CreateExpectedMessage() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);

    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    int32_t index = 0;

    msg_params[am::hmi_request::navi_texts][index]
              [am::hmi_request::field_name] =
                  hmi_apis::Common_TextFieldName::navigationText1;
    msg_params[am::hmi_request::navi_texts][index++]
              [am::hmi_request::field_text] = kNavigationText1;

    msg_params[am::hmi_request::navi_texts][index]
              [am::hmi_request::field_name] = static_cast<int32_t>(
                  hmi_apis::Common_TextFieldName::navigationText2);
    msg_params[am::hmi_request::navi_texts][index++]
              [am::hmi_request::field_text] = kNavigationText2;

    msg_params[am::hmi_request::navi_texts][index]
              [am::hmi_request::field_name] =
                  static_cast<int32_t>(hmi_apis::Common_TextFieldName::ETA);
    msg_params[am::hmi_request::navi_texts][index++]
              [am::hmi_request::field_text] = kEta;

    msg_params
        [am::hmi_request::navi_texts][index][am::hmi_request::field_name] =
            static_cast<int32_t>(hmi_apis::Common_TextFieldName::totalDistance);
    msg_params[am::hmi_request::navi_texts][index++]
              [am::hmi_request::field_text] = kTotalDistance;

    msg_params[am::hmi_request::navi_texts][index]
              [am::hmi_request::field_name] = static_cast<int32_t>(
                  hmi_apis::Common_TextFieldName::timeToDestination);
    msg_params[am::hmi_request::navi_texts][index++]
              [am::hmi_request::field_text] = kTimeToDestination;

    (*msg)[am::strings::msg_params] = msg_params;

    return msg;
  }

  void ExpectedManageMobileCommandResultCode(mobile_apis::Result::eType code) {
    EXPECT_CALL(app_mngr_, ManageMobileCommand(MobileResultCodeIs(code), _))
        .WillOnce(Return(true));
  }

 private:
  void SetUp() OVERRIDE {
    message_ = CreateMessageWithParams();
    expected_message_ = CreateExpectedMessage();
    mock_app_ = CreateMockApp();
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_interface_));
  }

 protected:
  MessageSharedPtr message_;
  SharedPtr<ShowConstantTBTRequest> request_;
  MessageSharedPtr expected_message_;
  MockAppPtr mock_app_;
  MockPolicyHandlerInterface mock_policy_handler_interface_;
};

TEST_F(ShowConstantTBTRequestTest, Run_AllCorrectData_SendHMIRequest) {
  am::ApplicationConstSharedPtr app_const_ptr(mock_app_);
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons(
                  (*message_)[am::strings::msg_params], app_const_ptr, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  EXPECT_CALL(mock_message_helper_, VerifyImage(_, app_const_ptr, _))
      .Times(2)
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

  am::ApplicationSharedPtr app_ptr(mock_app_);
  EXPECT_CALL(mock_message_helper_,
              SubscribeApplicationToSoftButton(_, app_ptr, _));

  smart_objects::SmartObject msg_params;
  EXPECT_CALL(*mock_app_, set_tbt_show_command(_))
      .WillOnce(SaveArg<0>(&msg_params));

  MessageSharedPtr result;
  EXPECT_CALL(app_mngr_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  ASSERT_TRUE(request_->Init());
  request_->Run();

  EXPECT_TRUE((*expected_message_)[am::strings::msg_params]
                                  [am::hmi_request::navi_texts] ==
              (*result)[am::strings::msg_params][am::hmi_request::navi_texts]);
}

TEST_F(ShowConstantTBTRequestTest,
       Run_NonExistentApp_ApplicationNotRegistered) {
  MockAppPtr invalid_mock_app;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(invalid_mock_app));

  ExpectedManageMobileCommandResultCode(
      mobile_apis::Result::APPLICATION_NOT_REGISTERED);

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  ASSERT_TRUE(request_->Init());
  request_->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_EmptyParams_InvalidData) {
  message_ = CreateMessage(smart_objects::SmartType_Map);
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  ExpectedManageMobileCommandResultCode(mobile_apis::Result::INVALID_DATA);

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  ASSERT_TRUE(request_->Init());
  request_->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_ProcessSoftButtonsInvalid_InvalidData) {
  am::ApplicationConstSharedPtr app_const_ptr(mock_app_);
  mobile_apis::Result::eType expected_result_code =
      mobile_apis::Result::INVALID_DATA;
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons(
                  (*message_)[am::strings::msg_params], app_const_ptr, _, _))
      .WillOnce(Return(expected_result_code));

  ExpectedManageMobileCommandResultCode(expected_result_code);

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  ASSERT_TRUE(request_->Init());
  request_->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_TurnIconInvalid_InvalidData) {
  am::ApplicationConstSharedPtr app_const_ptr(mock_app_);
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons(
                  (*message_)[am::strings::msg_params], app_const_ptr, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  mobile_apis::Result::eType expected_result_code =
      mobile_apis::Result::INVALID_DATA;
  EXPECT_CALL(mock_message_helper_, VerifyImage(_, app_const_ptr, _))
      .WillOnce(Return(expected_result_code));

  ExpectedManageMobileCommandResultCode(expected_result_code);

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  ASSERT_TRUE(request_->Init());
  request_->Run();
}

TEST_F(ShowConstantTBTRequestTest, Run_NextTurnIconInvalid_InvalidData) {
  am::ApplicationConstSharedPtr app_const_ptr(mock_app_);
  mobile_apis::Result::eType success = mobile_apis::Result::SUCCESS;
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons(
                  (*message_)[am::strings::msg_params], app_const_ptr, _, _))
      .WillOnce(Return(success));

  mobile_apis::Result::eType expected_result_code =
      mobile_apis::Result::INVALID_DATA;
  EXPECT_CALL(mock_message_helper_, VerifyImage(_, app_const_ptr, _))
      .WillOnce(Return(success))
      .WillOnce(Return(expected_result_code));

  ExpectedManageMobileCommandResultCode(expected_result_code);

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  ASSERT_TRUE(request_->Init());
  request_->Run();
}

TEST_F(ShowConstantTBTRequestTest,
       IsWhiteSpaceExist_TurnIconInvalidSyntax_InvalidData) {
  (*message_)[am::strings::msg_params][am::strings::turn_icon]
             [am::strings::value] = kTutnIconValue + "\n";

  ExpectedManageMobileCommandResultCode(mobile_apis::Result::INVALID_DATA);

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  ASSERT_TRUE(request_->Init());
  request_->Run();
}

TEST_F(ShowConstantTBTRequestTest,
       IsWhiteSpaceExist_NextTurnIconInvalidSyntax_InvalidData) {
  (*message_)[am::strings::msg_params][am::strings::next_turn_icon]
             [am::strings::value] = kNextTutnIconValue + "\n";

  ExpectedManageMobileCommandResultCode(mobile_apis::Result::INVALID_DATA);

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  ASSERT_TRUE(request_->Init());
  request_->Run();
}

TEST_F(ShowConstantTBTRequestTest,
       IsWhiteSpaceExist_NavigationText1InvalidSyntax_InvalidData) {
  (*message_)[am::strings::msg_params][am::strings::navigation_text_1] =
      kNavigationText1 + "\n";

  ExpectedManageMobileCommandResultCode(mobile_apis::Result::INVALID_DATA);

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  ASSERT_TRUE(request_->Init());
  request_->Run();
}

TEST_F(ShowConstantTBTRequestTest,
       IsWhiteSpaceExist_NavigationText2InvalidSyntax_InvalidData) {
  (*message_)[am::strings::msg_params][am::strings::navigation_text_2] =
      kNavigationText2 + "\n";

  ExpectedManageMobileCommandResultCode(mobile_apis::Result::INVALID_DATA);

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  ASSERT_TRUE(request_->Init());
  request_->Run();
}

TEST_F(ShowConstantTBTRequestTest,
       IsWhiteSpaceExist_EtaInvalidSyntax_InvalidData) {
  (*message_)[am::strings::msg_params][am::strings::eta] = kEta + "\n";

  ExpectedManageMobileCommandResultCode(mobile_apis::Result::INVALID_DATA);

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  ASSERT_TRUE(request_->Init());
  request_->Run();
}

TEST_F(ShowConstantTBTRequestTest,
       IsWhiteSpaceExist_TotalDistanceInvalidSyntax_InvalidData) {
  (*message_)[am::strings::msg_params][am::strings::total_distance] =
      kTotalDistance + "\n";

  ExpectedManageMobileCommandResultCode(mobile_apis::Result::INVALID_DATA);

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  ASSERT_TRUE(request_->Init());
  request_->Run();
}

TEST_F(ShowConstantTBTRequestTest,
       IsWhiteSpaceExist_TimeToDestinationInvalidSyntax_InvalidData) {
  (*message_)[am::strings::msg_params][am::strings::total_distance] =
      kTotalDistance + "\n";

  ExpectedManageMobileCommandResultCode(mobile_apis::Result::INVALID_DATA);

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  ASSERT_TRUE(request_->Init());
  request_->Run();
}

TEST_F(ShowConstantTBTRequestTest,
       OnEvent_NavigationShowConstantTBT_SendMobileResponse) {
  MessageSharedPtr message = CreateMessage(smart_objects::SmartType_Map);
  (*message)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*message)[am::strings::msg_params][am::strings::info] = "info1";

  Event event(hmi_apis::FunctionID::Navigation_ShowConstantTBT);
  event.set_smart_object(*message);

  ExpectedManageMobileCommandResultCode(mobile_apis::Result::SUCCESS);

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  request_->on_event(event);
}

TEST_F(ShowConstantTBTRequestTest, OnEvent_InvalidEnum_ReceivedUnknownEvent) {
  MessageSharedPtr message = CreateMessage(smart_objects::SmartType_Map);

  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  event.set_smart_object(*message);

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  request_ = CreateCommand<ShowConstantTBTRequest>(message_);
  request_->on_event(event);
}

}  // namespace show_constant_tbt_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
