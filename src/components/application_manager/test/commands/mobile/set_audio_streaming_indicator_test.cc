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
#include <set>

#include "application_manager/commands/mobile/set_audio_streaming_indicator_request.h"

#include "gtest/gtest.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace set_audio_streaming_indicator_request {

using am::commands::SetAudioStreamingIndicatorRequest;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using am::MockMessageHelper;
using am::MockHmiInterfaces;
using ::utils::SharedPtr;
using ::testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {
const uint32_t kConnectionKey = 2u;
const bool kIsNavi = true;
const bool kIsNotNavi = false;
const bool kIsMedia = true;
const bool kIsNotMedia = false;
const bool kIsVoiceCommunication = true;
const bool kIsNotVoiceCommunication = false;
const bool kIsSuccess = true;
const bool kIsNotSuccess = false;
}  // namespace

class SetAudioStreamingIndicatorRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SetAudioStreamingIndicatorRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock()) {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  ~SetAudioStreamingIndicatorRequestTest() {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  MessageSharedPtr CreateFullParamsSO() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    msg_params[am::strings::audio_streaming_indicator] =
        mobile_apis::AudioStreamingIndicator::PLAY;
    (*msg)[am::strings::msg_params] = msg_params;
    return msg;
  }

  void SetUp() OVERRIDE {
    mock_app_ = CreateMockApp();
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(hmi_interfaces_));
  }

  void DefineInterfaceAvailable(
      const am::HmiInterfaces::InterfaceID interface) {
    ON_CALL(hmi_interfaces_, GetInterfaceState(interface))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  }

  void DefineInterfaceNotAvailable(
      const am::HmiInterfaces::InterfaceID interface) {
    ON_CALL(hmi_interfaces_, GetInterfaceState(interface))
        .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  }

  void DefineHMILevelUIAvailable() {
    ON_CALL(*mock_app_, hmi_level())
        .WillByDefault(Return(mobile_apis::HMILevel::HMI_FULL));
    ON_CALL(hmi_interfaces_, GetInterfaceFromFunction(_))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
    DefineInterfaceAvailable(am::HmiInterfaces::HMI_INTERFACE_UI);
  }

  void VerifyCommandResults(MessageSharedPtr msg,
                            const mobile_apis::Result::eType mobile_code,
                            const bool is_success) {
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::success].asBool(),
              is_success);
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::result_code].asInt(),
              mobile_code);
  }

  void AppSetup(const bool is_navi,
                const bool is_voice_communication,
                const bool is_media) {
    ON_CALL(*mock_app_, is_navi()).WillByDefault(Return(is_navi));
    ON_CALL(*mock_app_, is_media_application()).WillByDefault(Return(is_media));
    ON_CALL(*mock_app_, is_voice_communication_supported())
        .WillByDefault(Return(is_voice_communication));
  }

  void MediaAppSetup() {
    AppSetup(kIsNotNavi, kIsNotVoiceCommunication, kIsMedia);
  }

  void VoiceCommunicationAppSetup() {
    AppSetup(kIsNotNavi, kIsVoiceCommunication, kIsNotMedia);
  }

  void NaviAppSetup() {
    AppSetup(kIsNavi, kIsNotVoiceCommunication, kIsNotMedia);
  }

  void IncorrectAppSetup() {
    AppSetup(kIsNotNavi, kIsNotVoiceCommunication, kIsNotMedia);
  }

  mobile_apis::AudioStreamingIndicator::eType GetIndicator(
      MessageSharedPtr msg) {
    return (static_cast<mobile_apis::AudioStreamingIndicator::eType>(
        (*msg)[am::strings::msg_params][am::strings::audio_streaming_indicator]
            .asInt()));
  }

  void TestCommandOnEventCall(
      const mobile_apis::Result::eType mobile_code,
      const bool is_success,
      MessageSharedPtr msg_hmi_response,
      utils::SharedPtr<SetAudioStreamingIndicatorRequest> cmd) {
    const hmi_apis::Common_Result::eType hmi_code =
        static_cast<hmi_apis::Common_Result::eType>(
            (*msg_hmi_response)[am::strings::params][am::hmi_response::code]
                .asInt());

    Event event_ui(hmi_apis::FunctionID::UI_SetAudioStreamingIndicator);
    event_ui.set_smart_object(*msg_hmi_response);

    EXPECT_CALL(mock_message_helper_, HMIToMobileResult(hmi_code))
        .WillOnce(Return(mobile_code));

    MessageSharedPtr msg_mobile_response =
        CatchMobileCommandResult(CallOnEvent(*cmd, event_ui));
    VerifyCommandResults(msg_mobile_response, mobile_code, is_success);
  }

  NiceMock<MockHmiInterfaces> hmi_interfaces_;
  MockMessageHelper& mock_message_helper_;
  MockAppPtr mock_app_;
};

TEST_F(SetAudioStreamingIndicatorRequestTest,
       Run_NotRegisteredApplication_APPLICATION_NOT_REGISTERED) {
  MessageSharedPtr msg_mobile = CreateFullParamsSO();
  utils::SharedPtr<SetAudioStreamingIndicatorRequest> command =
      CreateCommand<SetAudioStreamingIndicatorRequest>(msg_mobile);

  const mobile_apis::Result::eType mobile_response_code =
      mobile_apis::Result::APPLICATION_NOT_REGISTERED;
  const bool is_success = kIsNotSuccess;

  ApplicationSharedPtr mock_app_empty;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_empty));

  MessageSharedPtr msg_mobile_response =
      CatchMobileCommandResult(CallRun(*command));
  VerifyCommandResults(msg_mobile_response, mobile_response_code, is_success);
}

TEST_F(SetAudioStreamingIndicatorRequestTest, Run_NaviApplication_REJECTED) {
  MessageSharedPtr msg_mobile = CreateFullParamsSO();
  utils::SharedPtr<SetAudioStreamingIndicatorRequest> command =
      CreateCommand<SetAudioStreamingIndicatorRequest>(msg_mobile);

  const mobile_apis::Result::eType mobile_response_code =
      mobile_apis::Result::REJECTED;
  const bool is_success = kIsNotSuccess;

  NaviAppSetup();

  MessageSharedPtr msg_mobile_response =
      CatchMobileCommandResult(CallRun(*command));
  VerifyCommandResults(msg_mobile_response, mobile_response_code, is_success);
}

TEST_F(SetAudioStreamingIndicatorRequestTest,
       Run_NotMediaApplication_REJECTED) {
  MessageSharedPtr msg_mobile = CreateFullParamsSO();
  utils::SharedPtr<SetAudioStreamingIndicatorRequest> command =
      CreateCommand<SetAudioStreamingIndicatorRequest>(msg_mobile);

  const mobile_apis::Result::eType mobile_response_code =
      mobile_apis::Result::REJECTED;
  const bool is_success = kIsNotSuccess;

  IncorrectAppSetup();

  MessageSharedPtr msg_mobile_response =
      CatchMobileCommandResult(CallRun(*command));
  VerifyCommandResults(msg_mobile_response, mobile_response_code, is_success);
}

TEST_F(SetAudioStreamingIndicatorRequestTest,
       Run_VoiceCommunicationApplication_REJECTED) {
  MessageSharedPtr msg_mobile = CreateFullParamsSO();
  utils::SharedPtr<SetAudioStreamingIndicatorRequest> command =
      CreateCommand<SetAudioStreamingIndicatorRequest>(msg_mobile);

  const mobile_apis::Result::eType mobile_response_code =
      mobile_apis::Result::REJECTED;
  const bool is_success = kIsNotSuccess;

  VoiceCommunicationAppSetup();

  MessageSharedPtr msg_mobile_response =
      CatchMobileCommandResult(CallRun(*command));
  VerifyCommandResults(msg_mobile_response, mobile_response_code, is_success);
}

TEST_F(SetAudioStreamingIndicatorRequestTest,
       Run_AddIndicatorWaitingForResponseFailed_IGNORED) {
  MessageSharedPtr msg_mobile = CreateFullParamsSO();
  utils::SharedPtr<SetAudioStreamingIndicatorRequest> command =
      CreateCommand<SetAudioStreamingIndicatorRequest>(msg_mobile);
  mobile_apis::AudioStreamingIndicator::eType indicator =
      GetIndicator(msg_mobile);

  const mobile_apis::Result::eType mobile_response_code =
      mobile_apis::Result::IGNORED;
  const bool is_success = kIsNotSuccess;

  MediaAppSetup();

  EXPECT_CALL(*mock_app_, AddIndicatorWaitForResponse(indicator))
      .WillOnce(Return(false));

  MessageSharedPtr msg_mobile_response =
      CatchMobileCommandResult(CallRun(*command));
  VerifyCommandResults(msg_mobile_response, mobile_response_code, is_success);
}

TEST_F(SetAudioStreamingIndicatorRequestTest,
       Run_SetAudioStreamingIndicatorRequestOK_SendRequestToHmi) {
  MessageSharedPtr msg_mobile_response = CreateFullParamsSO();
  MessageSharedPtr msg_mobile = CreateFullParamsSO();
  utils::SharedPtr<SetAudioStreamingIndicatorRequest> command =
      CreateCommand<SetAudioStreamingIndicatorRequest>(msg_mobile);
  mobile_apis::AudioStreamingIndicator::eType indicator =
      GetIndicator(msg_mobile);

  MediaAppSetup();
  EXPECT_CALL(*mock_app_, AddIndicatorWaitForResponse(indicator))
      .WillOnce(Return(true));

  DefineHMILevelUIAvailable();
  EXPECT_CALL(app_mngr_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::UI_SetAudioStreamingIndicator)));

  command->Run();
}

TEST_F(SetAudioStreamingIndicatorRequestTest, OnEvent_IncorrectFunctionId) {
  MessageSharedPtr msg_mobile = CreateFullParamsSO();
  utils::SharedPtr<SetAudioStreamingIndicatorRequest> command =
      CreateCommand<SetAudioStreamingIndicatorRequest>(msg_mobile);

  MessageSharedPtr msg_hmi_response = CreateFullParamsSO();

  Event event_ui(hmi_apis::FunctionID::UI_PerformAudioPassThru);
  event_ui.set_smart_object(*msg_hmi_response);

  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(_)).Times(0);
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .Times(0);

  command->on_event(event_ui);
}

TEST_F(SetAudioStreamingIndicatorRequestTest,
       OnEvent_HMIResponseSUCCESS_SUCCESS) {
  MessageSharedPtr msg_mobile = CreateFullParamsSO();
  utils::SharedPtr<SetAudioStreamingIndicatorRequest> command =
      CreateCommand<SetAudioStreamingIndicatorRequest>(msg_mobile);
  mobile_apis::AudioStreamingIndicator::eType indicator =
      GetIndicator(msg_mobile);

  MessageSharedPtr msg_hmi_response = CreateFullParamsSO();
  (*msg_hmi_response)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;

  const mobile_apis::Result::eType mobile_response_code =
      mobile_apis::Result::SUCCESS;
  const bool is_success = kIsSuccess;

  MediaAppSetup();
  DefineHMILevelUIAvailable();
  EXPECT_CALL(*mock_app_, AddIndicatorWaitForResponse(indicator))
      .WillOnce(Return(true));
  EXPECT_CALL(app_mngr_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::UI_SetAudioStreamingIndicator)));

  command->Run();

  EXPECT_CALL(*mock_app_, set_audio_streaming_indicator(indicator));
  EXPECT_CALL(*mock_app_, RemoveIndicatorWaitForResponse(indicator));
  TestCommandOnEventCall(
      mobile_response_code, is_success, msg_hmi_response, command);
}

TEST_F(SetAudioStreamingIndicatorRequestTest,
       OnEvent_HMIResponseUNSUPPORTED_RESOURCE_SUCCESS) {
  MessageSharedPtr msg_mobile = CreateFullParamsSO();
  utils::SharedPtr<SetAudioStreamingIndicatorRequest> command =
      CreateCommand<SetAudioStreamingIndicatorRequest>(msg_mobile);
  mobile_apis::AudioStreamingIndicator::eType indicator =
      GetIndicator(msg_mobile);

  MessageSharedPtr msg_hmi_response = CreateFullParamsSO();
  (*msg_hmi_response)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;

  const mobile_apis::Result::eType mobile_response_code =
      mobile_apis::Result::UNSUPPORTED_RESOURCE;
  const bool is_success = kIsSuccess;

  MediaAppSetup();
  DefineHMILevelUIAvailable();
  EXPECT_CALL(*mock_app_, AddIndicatorWaitForResponse(indicator))
      .WillOnce(Return(true));
  EXPECT_CALL(app_mngr_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::UI_SetAudioStreamingIndicator)));

  command->Run();

  EXPECT_CALL(*mock_app_, set_audio_streaming_indicator(indicator));
  EXPECT_CALL(*mock_app_, RemoveIndicatorWaitForResponse(indicator));
  TestCommandOnEventCall(
      mobile_response_code, is_success, msg_hmi_response, command);
}

TEST_F(SetAudioStreamingIndicatorRequestTest,
       OnEvent_HMIResponseUNSUPPORTED_RESOURCE_NOT_SUCCESS) {
  MessageSharedPtr msg_mobile = CreateFullParamsSO();
  utils::SharedPtr<SetAudioStreamingIndicatorRequest> command =
      CreateCommand<SetAudioStreamingIndicatorRequest>(msg_mobile);
  mobile_apis::AudioStreamingIndicator::eType indicator =
      GetIndicator(msg_mobile);

  MessageSharedPtr msg_hmi_response = CreateFullParamsSO();
  (*msg_hmi_response)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;

  const mobile_apis::Result::eType mobile_response_code =
      mobile_apis::Result::UNSUPPORTED_RESOURCE;
  const bool is_success = kIsNotSuccess;

  MediaAppSetup();
  DefineHMILevelUIAvailable();
  EXPECT_CALL(*mock_app_, AddIndicatorWaitForResponse(indicator))
      .WillOnce(Return(true));
  EXPECT_CALL(app_mngr_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::UI_SetAudioStreamingIndicator)))
      .WillOnce(Return(true));

  command->Run();

  DefineInterfaceNotAvailable(am::HmiInterfaces::HMI_INTERFACE_UI);
  EXPECT_CALL(*mock_app_, RemoveIndicatorWaitForResponse(indicator));
  TestCommandOnEventCall(
      mobile_response_code, is_success, msg_hmi_response, command);
}

TEST_F(SetAudioStreamingIndicatorRequestTest,
       OnEvent_HMIResponseUSER_DISALLOWED_USER_DISALLOWED) {
  MessageSharedPtr msg_mobile = CreateFullParamsSO();
  utils::SharedPtr<SetAudioStreamingIndicatorRequest> command =
      CreateCommand<SetAudioStreamingIndicatorRequest>(msg_mobile);
  mobile_apis::AudioStreamingIndicator::eType indicator =
      GetIndicator(msg_mobile);

  MessageSharedPtr msg_hmi_response = CreateFullParamsSO();
  (*msg_hmi_response)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::USER_DISALLOWED;

  const mobile_apis::Result::eType mobile_response_code =
      mobile_apis::Result::USER_DISALLOWED;
  const bool is_success = kIsNotSuccess;

  MediaAppSetup();
  DefineHMILevelUIAvailable();
  EXPECT_CALL(*mock_app_, AddIndicatorWaitForResponse(indicator))
      .WillOnce(Return(true));
  EXPECT_CALL(app_mngr_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::UI_SetAudioStreamingIndicator)));

  command->Run();

  EXPECT_CALL(*mock_app_, RemoveIndicatorWaitForResponse(indicator));
  TestCommandOnEventCall(
      mobile_response_code, is_success, msg_hmi_response, command);
}

TEST_F(SetAudioStreamingIndicatorRequestTest,
       onTimeOutRegisteredApp_GENERIC_ERROR) {
  MessageSharedPtr msg_mobile = CreateFullParamsSO();
  utils::SharedPtr<SetAudioStreamingIndicatorRequest> command =
      CreateCommand<SetAudioStreamingIndicatorRequest>(msg_mobile);

  MessageSharedPtr msg_mobile_response = CreateFullParamsSO();
  (*msg_mobile_response)[am::strings::msg_params][am::strings::result_code] =
      mobile_apis::Result::GENERIC_ERROR;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::GENERIC_ERROR;
  const bool is_success = kIsNotSuccess;

  EXPECT_CALL(*mock_app_, RemoveIndicatorWaitForResponse(_));
  EXPECT_CALL(
      mock_message_helper_,
      CreateNegativeResponse(_, _, _, am::mobile_api::Result::GENERIC_ERROR))
      .WillOnce(Return(msg_mobile_response));
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&msg_mobile_response), Return(true)));

  command->onTimeOut();

  VerifyCommandResults(msg_mobile_response, mobile_response, is_success);
}

TEST_F(SetAudioStreamingIndicatorRequestTest,
       onTimeOutNotRegisteredApp_GENERIC_ERROR) {
  MessageSharedPtr msg_mobile = CreateFullParamsSO();
  utils::SharedPtr<SetAudioStreamingIndicatorRequest> command =
      CreateCommand<SetAudioStreamingIndicatorRequest>(msg_mobile);

  MessageSharedPtr msg_mobile_response = CreateFullParamsSO();
  (*msg_mobile_response)[am::strings::msg_params][am::strings::result_code] =
      mobile_apis::Result::GENERIC_ERROR;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::GENERIC_ERROR;
  const bool is_success = kIsNotSuccess;

  ApplicationSharedPtr mock_app_empty;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_empty));

  EXPECT_CALL(*mock_app_, RemoveIndicatorWaitForResponse(_)).Times(0);
  EXPECT_CALL(
      mock_message_helper_,
      CreateNegativeResponse(_, _, _, am::mobile_api::Result::GENERIC_ERROR))
      .WillOnce(Return(msg_mobile_response));
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&msg_mobile_response), Return(true)));

  command->onTimeOut();

  VerifyCommandResults(msg_mobile_response, mobile_response, is_success);
}
}  // namespace set_audio_streaming_indicator_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests
