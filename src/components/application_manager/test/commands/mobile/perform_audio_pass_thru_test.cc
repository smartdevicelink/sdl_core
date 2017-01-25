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

#include "application_manager/commands/mobile/perform_audio_pass_thru_request.h"

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
namespace perform_audio_pass_thru_request {

namespace am = application_manager;
using am::commands::PerformAudioPassThruRequest;
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
const int32_t kCommandId = 1;
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
const std::string kEmptyInfo = "";
const std::string kIconName = "icon.png";
const std::string kTypeStatic = "STATIC";
const std::string kTypeDynamic = "DYNAMIC";
const std::string kIconNameInvalid = "icon\t.png";
}  // namespace

class PerformAudioPassThruRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  PerformAudioPassThruRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock())
      , mock_app_(CreateMockApp()) {}

  MessageSharedPtr CreateMobileMessageSO() {
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

  MessageSharedPtr CreateHMIMessageSO(const int32_t code, const uint32_t cmd) {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::hmi_response::code] = code;
    (*msg)[am::strings::msg_params][am::strings::cmd_id] = cmd;

    return msg;
  }

  bool CreateImageFile(const std::string& name) {
    const std::string& full_file_path = file_system::CurrentWorkingDirectory();
    return file_system::CreateFile(full_file_path + "/" + name);
  }

  void SetUp() OVERRIDE {
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(hmi_interfaces_));
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void ResultCommandExpectations(MessageSharedPtr msg,
                                 const std::string& info,
                                 const int32_t code,
                                 const bool success) {
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::success].asBool(),
              success);
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::result_code].asInt(),
              (code));
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::info].asString(),
              info);
  }

  void SetupIconParameter(MessageSharedPtr msg,
                          const std::string& type,
                          const std::string& value) {
    smart_objects::SmartObject icon =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    icon[am::strings::type] = type;
    icon[am::strings::value] = value;
    (*msg)[am::strings::msg_params][am::strings::audio_pass_thru_icon] = icon;
  }

  void DefineUIsAvailable() {
    ON_CALL(hmi_interfaces_,
            GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  }

  void DefineTTSsAvailable() {
    ON_CALL(hmi_interfaces_,
            GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  }

  void DefineHMIAvailable() {
    DefineUIsAvailable();
    DefineTTSsAvailable();
  }

  void DefineHMILevelUIAvailable() {
    ON_CALL(*mock_app_, hmi_level())
        .WillByDefault(Return(mobile_apis::HMILevel::HMI_FULL));
    ON_CALL(hmi_interfaces_, GetInterfaceFromFunction(_))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
    DefineUIsAvailable();
  }

  sync_primitives::Lock lock_;
  NiceMock<MockHmiInterfaces> hmi_interfaces_;
  MockMessageHelper& mock_message_helper_;
  MockAppPtr mock_app_;
};

TEST_F(PerformAudioPassThruRequestTest, OnTimeout_GENERIC_ERROR) {
  MessageSharedPtr msg_mobile_response =
      CreateMessage(smart_objects::SmartType_Map);
  (*msg_mobile_response)[am::strings::msg_params][am::strings::result_code] =
      am::mobile_api::Result::GENERIC_ERROR;
  (*msg_mobile_response)[am::strings::msg_params][am::strings::success] = false;

  utils::SharedPtr<PerformAudioPassThruRequest> command =
      CreateCommand<PerformAudioPassThruRequest>();

  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(true));
  EXPECT_CALL(app_mngr_, StopAudioPassThru(_));
  EXPECT_CALL(
      mock_message_helper_,
      CreateNegativeResponse(_, _, _, am::mobile_api::Result::GENERIC_ERROR))
      .WillOnce(Return(msg_mobile_response));
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&msg_mobile_response), Return(true)));

  command->onTimeOut();

  ResultCommandExpectations(msg_mobile_response,
                            kEmptyInfo,
                            am::mobile_api::Result::GENERIC_ERROR,
                            false);
}

TEST_F(PerformAudioPassThruRequestTest,
       OnEvent_UIHmiSendUnsupportedResource_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg_mobile = CreateMobileMessageSO();
  utils::SharedPtr<PerformAudioPassThruRequest> command =
      CreateCommand<PerformAudioPassThruRequest>(msg_mobile);

  MessageSharedPtr msg_tts =
      CreateHMIMessageSO(hmi_apis::Common_Result::SUCCESS, kCmdId);
  am::event_engine::Event event_tts(hmi_apis::FunctionID::TTS_Speak);
  event_tts.set_smart_object(*msg_tts);

  DefineHMIAvailable();
  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::SUCCESS))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  command->on_event(event_tts);

  MessageSharedPtr msg_ui = CreateHMIMessageSO(
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE, kCommandId);
  (*msg_ui)[am::strings::msg_params][am::strings::info] =
      "UI is not supported by system";
  Event event_ui(hmi_apis::FunctionID::UI_PerformAudioPassThru);
  event_ui.set_smart_object(*msg_ui);

  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(false));

  MessageSharedPtr msg_mobile_response;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&msg_mobile_response), Return(true)));

  command->on_event(event_ui);

  ResultCommandExpectations(msg_mobile_response,
                            "UI is not supported by system",
                            am::mobile_api::Result::UNSUPPORTED_RESOURCE,
                            true);
}

TEST_F(PerformAudioPassThruRequestTest,
       OnEvent_UIHmiSendTruncatedData_TRUNCATED_DATA) {
  MessageSharedPtr msg_mobile = CreateMobileMessageSO();
  utils::SharedPtr<PerformAudioPassThruRequest> command =
      CreateCommand<PerformAudioPassThruRequest>(msg_mobile);

  MessageSharedPtr msg_tts =
      CreateHMIMessageSO(hmi_apis::Common_Result::SUCCESS, kCmdId);
  am::event_engine::Event event_tts(hmi_apis::FunctionID::TTS_Speak);
  event_tts.set_smart_object(*msg_tts);

  DefineHMIAvailable();
  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::SUCCESS))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  command->on_event(event_tts);

  MessageSharedPtr msg_ui =
      CreateHMIMessageSO(hmi_apis::Common_Result::TRUNCATED_DATA, kCommandId);
  Event event_ui(hmi_apis::FunctionID::UI_PerformAudioPassThru);
  event_ui.set_smart_object(*msg_ui);

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::TRUNCATED_DATA))
      .WillOnce(Return(mobile_apis::Result::TRUNCATED_DATA));
  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(false));

  MessageSharedPtr msg_mobile_response;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&msg_mobile_response), Return(true)));

  command->on_event(event_ui);

  ResultCommandExpectations(msg_mobile_response,
                            kEmptyInfo,
                            am::mobile_api::Result::TRUNCATED_DATA,
                            false);
}

TEST_F(PerformAudioPassThruRequestTest,
       OnEvent_TTSHmiSendTruncatedData_TRUNCATED_DATA) {
  MessageSharedPtr msg_mobile = CreateMobileMessageSO();
  utils::SharedPtr<PerformAudioPassThruRequest> command =
      CreateCommand<PerformAudioPassThruRequest>(msg_mobile);

  MessageSharedPtr msg_tts =
      CreateHMIMessageSO(hmi_apis::Common_Result::TRUNCATED_DATA, kCmdId);
  am::event_engine::Event event_tts(hmi_apis::FunctionID::TTS_Speak);
  event_tts.set_smart_object(*msg_tts);

  DefineHMIAvailable();
  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::TRUNCATED_DATA))
      .WillOnce(Return(mobile_apis::Result::TRUNCATED_DATA));

  command->on_event(event_tts);

  MessageSharedPtr msg_ui =
      CreateHMIMessageSO(hmi_apis::Common_Result::SUCCESS, kCommandId);
  Event event_ui(hmi_apis::FunctionID::UI_PerformAudioPassThru);
  event_ui.set_smart_object(*msg_ui);

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::WARNINGS))
      .WillOnce(Return(mobile_apis::Result::WARNINGS));
  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(false));

  MessageSharedPtr msg_mobile_response;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&msg_mobile_response), Return(true)));

  command->on_event(event_ui);

  ResultCommandExpectations(
      msg_mobile_response, kEmptyInfo, am::mobile_api::Result::WARNINGS, true);
}

TEST_F(PerformAudioPassThruRequestTest,
       OnEvent_UISendInvalidIdTTSSendTruncatedData_INVALID_ID) {
  MessageSharedPtr msg_mobile = CreateMobileMessageSO();
  utils::SharedPtr<PerformAudioPassThruRequest> command =
      CreateCommand<PerformAudioPassThruRequest>(msg_mobile);

  MessageSharedPtr msg_tts =
      CreateHMIMessageSO(hmi_apis::Common_Result::TRUNCATED_DATA, kCmdId);
  am::event_engine::Event event_tts(hmi_apis::FunctionID::TTS_Speak);
  event_tts.set_smart_object(*msg_tts);

  DefineHMIAvailable();
  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::TRUNCATED_DATA))
      .WillOnce(Return(mobile_apis::Result::TRUNCATED_DATA));

  command->on_event(event_tts);

  MessageSharedPtr msg_ui =
      CreateHMIMessageSO(hmi_apis::Common_Result::INVALID_ID, kCommandId);
  Event event_ui(hmi_apis::FunctionID::UI_PerformAudioPassThru);
  event_ui.set_smart_object(*msg_ui);

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::INVALID_ID))
      .WillOnce(Return(mobile_apis::Result::INVALID_ID));
  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(false));

  MessageSharedPtr msg_mobile_response;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&msg_mobile_response), Return(true)));

  command->on_event(event_ui);

  ResultCommandExpectations(msg_mobile_response,
                            kEmptyInfo,
                            am::mobile_api::Result::INVALID_ID,
                            false);
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_MobileSendAudioPassThruIconMissing_WARNINGS) {
  MessageSharedPtr msg_mobile = CreateMobileMessageSO();
  utils::SharedPtr<PerformAudioPassThruRequest> command =
      CreateCommand<PerformAudioPassThruRequest>(msg_mobile);

  DefineHMILevelUIAvailable();
  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _)).Times(0);

  command->Run();

  MessageSharedPtr msg_tts =
      CreateHMIMessageSO(hmi_apis::Common_Result::SUCCESS, kCmdId);
  am::event_engine::Event event_tts(hmi_apis::FunctionID::TTS_Speak);
  event_tts.set_smart_object(*msg_tts);

  DefineHMIAvailable();

  command->on_event(event_tts);

  MessageSharedPtr msg_ui =
      CreateHMIMessageSO(hmi_apis::Common_Result::WARNINGS, kCommandId);
  Event event_ui(hmi_apis::FunctionID::UI_PerformAudioPassThru);
  event_ui.set_smart_object(*msg_ui);

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::WARNINGS))
      .WillOnce(Return(mobile_apis::Result::WARNINGS));
  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(false));

  MessageSharedPtr msg_mobile_response;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&msg_mobile_response), Return(true)));

  command->on_event(event_ui);

  ResultCommandExpectations(
      msg_mobile_response, kEmptyInfo, am::mobile_api::Result::WARNINGS, true);
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_MobileSendAudioPassThruIconStatic_SUCCESS) {
  MessageSharedPtr msg_mobile = CreateMobileMessageSO();
  SetupIconParameter(msg_mobile, kTypeStatic, kIconName);
  utils::SharedPtr<PerformAudioPassThruRequest> command =
      CreateCommand<PerformAudioPassThruRequest>(msg_mobile);

  DefineHMILevelUIAvailable();
  ON_CALL(app_mngr_, ManageHMICommand(_)).WillByDefault(Return(true));
  EXPECT_CALL(mock_message_helper_,
              VerifyImage((*msg_mobile)[am::strings::msg_params]
                                       [am::strings::audio_pass_thru_icon],
                          _,
                          _)).WillOnce(Return(mobile_apis::Result::SUCCESS));

  command->Run();

  MessageSharedPtr msg_tts =
      CreateHMIMessageSO(hmi_apis::Common_Result::SUCCESS, kCmdId);
  am::event_engine::Event event_tts(hmi_apis::FunctionID::TTS_Speak);
  event_tts.set_smart_object(*msg_tts);

  DefineHMIAvailable();

  command->on_event(event_tts);

  MessageSharedPtr msg_ui =
      CreateHMIMessageSO(hmi_apis::Common_Result::SUCCESS, kCommandId);
  Event event_ui(hmi_apis::FunctionID::UI_PerformAudioPassThru);
  event_ui.set_smart_object(*msg_ui);

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::SUCCESS))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(false));

  MessageSharedPtr msg_mobile_response;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&msg_mobile_response), Return(true)));

  command->on_event(event_ui);

  ResultCommandExpectations(
      msg_mobile_response, kEmptyInfo, am::mobile_api::Result::SUCCESS, true);
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_MobileSendAudioPassThruIconDynamic_WARNINGS) {
  MessageSharedPtr msg_mobile = CreateMobileMessageSO();
  SetupIconParameter(msg_mobile, kTypeDynamic, kIconName);
  utils::SharedPtr<PerformAudioPassThruRequest> command =
      CreateCommand<PerformAudioPassThruRequest>(msg_mobile);

  DefineHMILevelUIAvailable();
  EXPECT_CALL(mock_message_helper_,
              VerifyImage((*msg_mobile)[am::strings::msg_params]
                                       [am::strings::audio_pass_thru_icon],
                          _,
                          _))
      .WillOnce(Return(mobile_apis::Result::INVALID_DATA));

  command->Run();

  MessageSharedPtr msg_tts =
      CreateHMIMessageSO(hmi_apis::Common_Result::SUCCESS, kCmdId);
  am::event_engine::Event event_tts(hmi_apis::FunctionID::TTS_Speak);
  event_tts.set_smart_object(*msg_tts);

  DefineHMIAvailable();

  command->on_event(event_tts);

  MessageSharedPtr msg_ui =
      CreateHMIMessageSO(hmi_apis::Common_Result::WARNINGS, kCommandId);
  Event event_ui(hmi_apis::FunctionID::UI_PerformAudioPassThru);
  event_ui.set_smart_object(*msg_ui);

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::WARNINGS))
      .WillOnce(Return(mobile_apis::Result::WARNINGS));
  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(false));

  MessageSharedPtr msg_mobile_response;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&msg_mobile_response), Return(true)));

  command->on_event(event_ui);

  ResultCommandExpectations(msg_mobile_response,
                            "Reference image(s) not found",
                            am::mobile_api::Result::WARNINGS,
                            true);
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_MobileSendAudioPassThruIconDynamic_SUCCESS) {
  MessageSharedPtr msg_mobile = CreateMobileMessageSO();
  ASSERT_TRUE(CreateImageFile(kIconName));
  SetupIconParameter(msg_mobile, kTypeDynamic, kIconName);
  utils::SharedPtr<PerformAudioPassThruRequest> command =
      CreateCommand<PerformAudioPassThruRequest>(msg_mobile);

  DefineHMILevelUIAvailable();
  EXPECT_CALL(mock_message_helper_,
              VerifyImage((*msg_mobile)[am::strings::msg_params]
                                       [am::strings::audio_pass_thru_icon],
                          _,
                          _)).WillOnce(Return(mobile_apis::Result::SUCCESS));

  command->Run();

  MessageSharedPtr msg_tts =
      CreateHMIMessageSO(hmi_apis::Common_Result::SUCCESS, kCmdId);
  am::event_engine::Event event_tts(hmi_apis::FunctionID::TTS_Speak);
  event_tts.set_smart_object(*msg_tts);

  DefineHMIAvailable();

  command->on_event(event_tts);

  MessageSharedPtr msg_ui =
      CreateHMIMessageSO(hmi_apis::Common_Result::SUCCESS, kCommandId);
  Event event_ui(hmi_apis::FunctionID::UI_PerformAudioPassThru);
  event_ui.set_smart_object(*msg_ui);

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::SUCCESS))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(false));

  MessageSharedPtr msg_mobile_response;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&msg_mobile_response), Return(true)));

  command->on_event(event_ui);

  ResultCommandExpectations(
      msg_mobile_response, kEmptyInfo, am::mobile_api::Result::SUCCESS, true);
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_MobileSendAudioPassThruIconInvalid_INVALID_DATA) {
  MessageSharedPtr msg_mobile = CreateMobileMessageSO();
  SetupIconParameter(msg_mobile, kTypeStatic, kIconNameInvalid);
  utils::SharedPtr<PerformAudioPassThruRequest> command =
      CreateCommand<PerformAudioPassThruRequest>(msg_mobile);

  DefineHMILevelUIAvailable();
  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _)).Times(0);

  MessageSharedPtr msg_mobile_response;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&msg_mobile_response), Return(true)));
  command->Run();

  ResultCommandExpectations(msg_mobile_response,
                            kEmptyInfo,
                            am::mobile_api::Result::INVALID_DATA,
                            false);
}

}  // namespace perform_audio_pass_thru_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests
