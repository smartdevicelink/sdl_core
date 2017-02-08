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

#include "application_manager/commands/mobile/alert_request.h"

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
namespace alert_request {

namespace am = application_manager;
using am::commands::AlertRequest;
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
}  // namespace

class AlertRequestTest : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  AlertRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock())
      , mock_app_(CreateMockApp()) {}

 protected:
  MessageSharedPtr CreateFullParamsSO() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;

    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params[am::strings::alert_text1] = "Test alertText1";
    msg_params[am::strings::alert_text2] = "Test alertText2";
    msg_params[am::strings::alert_text3] = "Test alertText3";
    msg_params[am::strings::tts_chunks] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    msg_params[am::strings::tts_chunks][0] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params[am::strings::tts_chunks][0][am::strings::text] = "dummy";
    (*msg)[am::strings::msg_params] = msg_params;
    return msg;
  }

  void ResultCommandExpectations(MessageSharedPtr msg,
                                 const std::string& info) {
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::success].asBool(),
              true);
    EXPECT_EQ(
        (*msg)[am::strings::msg_params][am::strings::result_code].asInt(),
        static_cast<int32_t>(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE));
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::info].asString(),
              info);
  }

  void SetHMIInterfaceState(const am::HmiInterfaces::InterfaceState ui_state,
                            const am::HmiInterfaces::InterfaceState tts_state) {
    ON_CALL(hmi_interfaces_,
            GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
        .WillByDefault(Return(ui_state));
    ON_CALL(hmi_interfaces_,
            GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
        .WillByDefault(Return(tts_state));
  }

  MessageSharedPtr PrepareResponseFromHMI(
      const hmi_apis::Common_Result::eType result_code) {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::hmi_response::code] = result_code;
    (*msg)[am::strings::msg_params] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    return msg;
  }

  void CheckExpectations(const hmi_apis::Common_Result::eType tts_hmi_response,
                         const hmi_apis::Common_Result::eType ui_hmi_response,
                         const mobile_apis::Result::eType mobile_response,
                         const am::HmiInterfaces::InterfaceState tts_state,
                         const am::HmiInterfaces::InterfaceState ui_state,
                         const bool success) {
    MessageSharedPtr msg = CreateFullParamsSO();
    utils::SharedPtr<AlertRequest> command = CreateCommand<AlertRequest>(msg);

    ON_CALL(*mock_app_, hmi_level())
        .WillByDefault(Return(mobile_apis::HMILevel::HMI_FULL));
    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(policy_interface_));
    ON_CALL(mock_message_helper_, ProcessSoftButtons(_, _, _, _))
        .WillByDefault(Return(mobile_apis::Result::SUCCESS));
    ON_CALL(hmi_interfaces_, GetInterfaceFromFunction(_))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
    SetHMIInterfaceState(ui_state, tts_state);

    MessageSharedPtr msg_ui = PrepareResponseFromHMI(ui_hmi_response);
    Event event_ui(hmi_apis::FunctionID::UI_Alert);
    event_ui.set_smart_object(*msg_ui);

    MessageSharedPtr msg_tts = PrepareResponseFromHMI(tts_hmi_response);
    Event event_tts(hmi_apis::FunctionID::TTS_Speak);
    event_tts.set_smart_object(*msg_tts);

    command->Run();

    SetHMIInterfaceState(ui_state, tts_state);

    command->on_event(event_tts);
    MessageSharedPtr response_to_mobile;
    EXPECT_CALL(app_mngr_,
                ManageMobileCommand(
                    _, am::commands::Command::CommandOrigin::ORIGIN_SDL))
        .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));
    command->on_event(event_ui);
    EXPECT_EQ(
        (*response_to_mobile)[am::strings::msg_params][am::strings::success]
            .asBool(),
        success);
    EXPECT_EQ(
        (*response_to_mobile)[am::strings::msg_params][am::strings::result_code]
            .asInt(),
        static_cast<int32_t>(mobile_response));
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
  sync_primitives::Lock lock_;

  NiceMock<policy_test::MockPolicyHandlerInterface> policy_interface_;
  NiceMock<MockHmiInterfaces> hmi_interfaces_;
  MockMessageHelper& mock_message_helper_;
  MockAppPtr mock_app_;
};

TEST_F(AlertRequestTest, OnTimeout_GENERIC_ERROR) {
  MessageSharedPtr command_msg = CreateMessage(smart_objects::SmartType_Map);
  (*command_msg)[am::strings::msg_params][am::strings::result_code] =
      am::mobile_api::Result::GENERIC_ERROR;
  (*command_msg)[am::strings::msg_params][am::strings::success] = false;
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  utils::SharedPtr<AlertRequest> command = CreateCommand<AlertRequest>();

  EXPECT_CALL(
      mock_message_helper_,
      CreateNegativeResponse(_, _, _, am::mobile_api::Result::GENERIC_ERROR))
      .WillOnce(Return(command_msg));

  MessageSharedPtr mobile_command_result;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&mobile_command_result), Return(true)));

  command->onTimeOut();
  EXPECT_EQ(
      (*mobile_command_result)[am::strings::msg_params][am::strings::success]
          .asBool(),
      false);
  EXPECT_EQ((*mobile_command_result)[am::strings::msg_params]
                                    [am::strings::result_code].asInt(),
            static_cast<int32_t>(am::mobile_api::Result::GENERIC_ERROR));
}

TEST_F(AlertRequestTest, OnEvent_UI_HmiSendSuccess_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr command_msg = CreateFullParamsSO();

  utils::SharedPtr<AlertRequest> command =
      CreateCommand<AlertRequest>(command_msg);

  SetHMIInterfaceState(am::HmiInterfaces::InterfaceState::STATE_NOT_AVAILABLE,
                       am::HmiInterfaces::InterfaceState::STATE_NOT_AVAILABLE);

  MessageSharedPtr msg_ui = CreateMessage(smart_objects::SmartType_Map);
  (*msg_ui)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg_ui)[am::strings::msg_params][am::strings::info] =
      "UI is not supported by system";

  ON_CALL(mock_message_helper_, HMIToMobileResult(_))
      .WillByDefault(Return(mobile_apis::Result::SUCCESS));

  MessageSharedPtr msg_tts = CreateMessage();
  (*msg_tts)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  Event event_tts(hmi_apis::FunctionID::TTS_Speak);
  event_tts.set_smart_object(*msg_tts);

  command->on_event(event_tts);

  Event event_ui(hmi_apis::FunctionID::UI_Alert);
  event_ui.set_smart_object(*msg_ui);

  MessageSharedPtr mobile_command_result;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&mobile_command_result), Return(true)));

  command->on_event(event_ui);

  ResultCommandExpectations(mobile_command_result,
                            "UI is not supported by system");
}

TEST_F(
    AlertRequestTest,
    BothInterfaceIsAvailable_TTSResultUnsupported_UIResultSUCCESS_MobileResultWarning) {
  const hmi_apis::Common_Result::eType tts_hmi_response =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::SUCCESS;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::WARNINGS;
  const am::HmiInterfaces::InterfaceState tts_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const bool success = true;
  CheckExpectations(tts_hmi_response,
                    ui_hmi_response,
                    mobile_response,
                    tts_state,
                    ui_state,
                    success);
}

TEST_F(
    AlertRequestTest,
    BothInterfaceIsAvailable_TTSResultSUCCESS_UIResultWARNINGS_MobileResultWarning) {
  const hmi_apis::Common_Result::eType tts_hmi_response =
      hmi_apis::Common_Result::SUCCESS;
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::WARNINGS;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::WARNINGS;
  const am::HmiInterfaces::InterfaceState tts_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const bool success = true;
  CheckExpectations(tts_hmi_response,
                    ui_hmi_response,
                    mobile_response,
                    tts_state,
                    ui_state,
                    success);
}

TEST_F(
    AlertRequestTest,
    TTSInterfaceNotRespond_TTSResultUnsupported_UIResultWARNINGS_MobileResultUnsupported) {
  const hmi_apis::Common_Result::eType tts_hmi_response =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::WARNINGS;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::UNSUPPORTED_RESOURCE;
  const am::HmiInterfaces::InterfaceState tts_state =
      am::HmiInterfaces::STATE_NOT_RESPONSE;
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const bool success = true;
  CheckExpectations(tts_hmi_response,
                    ui_hmi_response,
                    mobile_response,
                    tts_state,
                    ui_state,
                    success);
}

}  // namespace alert_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests
