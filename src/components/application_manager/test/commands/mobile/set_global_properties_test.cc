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

#include "application_manager/commands/mobile/set_global_properties_request.h"

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
namespace set_global_properties_request {

namespace am = application_manager;
using am::commands::SetGlobalPropertiesRequest;
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

class SetGlobalPropertiesRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SetGlobalPropertiesRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock())
      , mock_app_(CreateMockApp()) {}

  MessageSharedPtr CreateFullParamsSO() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params[am::strings::vr_help_title] = "vr_help_title";
    msg_params[am::strings::vr_help][0][am::strings::text] = "vr_help";
    msg_params[am::strings::vr_help][0][am::strings::position] = 1u;
    msg_params[am::strings::help_prompt][0][am::strings::text] = "help_promt";
    (*msg)[am::strings::msg_params] = msg_params;
    return msg;
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
      const hmi_apis::Common_Result::eType result_code, const char* info) {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::hmi_response::code] = result_code;
    (*msg)[am::strings::msg_params] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    if (info) {
      (*msg)[am::strings::msg_params][am::strings::info] = info;
    }
    return msg;
  }

  void CheckExpectations(const hmi_apis::Common_Result::eType ui_hmi_response,
                         const hmi_apis::Common_Result::eType tts_hmi_response,
                         const char* ui_info,
                         const char* tts_info,
                         const mobile_apis::Result::eType mobile_response,
                         const char* mobile_info,
                         const am::HmiInterfaces::InterfaceState ui_state,
                         const am::HmiInterfaces::InterfaceState tts_state,
                         const bool success) {
    MessageSharedPtr set_global_pr_msg = CreateFullParamsSO();

    utils::SharedPtr<SetGlobalPropertiesRequest> command =
        CreateCommand<SetGlobalPropertiesRequest>(set_global_pr_msg);

    EXPECT_CALL(app_mngr_,
                RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
    const smart_objects::SmartObject* vr_help_title =
        &((*set_global_pr_msg)[am::strings::msg_params]
                              [am::strings::vr_help_title]);
    const smart_objects::SmartObject* vr_help =
        &((*set_global_pr_msg)[am::strings::msg_params][am::strings::vr_help]);
    const smart_objects::SmartObject* vr_help_prompt = &((
        *set_global_pr_msg)[am::strings::msg_params][am::strings::help_prompt]);
    ON_CALL(*mock_app_, vr_help_title()).WillByDefault(Return(vr_help_title));
    ON_CALL(*mock_app_, vr_help()).WillByDefault(Return(vr_help));
    ON_CALL(*mock_app_, help_prompt()).WillByDefault(Return(vr_help_prompt));
    ON_CALL(
        mock_message_helper_,
        VerifyImageVrHelpItems(
            (*set_global_pr_msg)[am::strings::msg_params][am::strings::vr_help],
            _,
            _)).WillByDefault(Return(mobile_apis::Result::SUCCESS));

    ON_CALL(mock_message_helper_, VerifyImage(_, _, _))
        .WillByDefault(Return(mobile_apis::Result::SUCCESS));
    EXPECT_CALL(
        hmi_interfaces_,
        GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
        .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
    EXPECT_CALL(
        hmi_interfaces_,
        GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
        .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
    SetHMIInterfaceState(ui_state, tts_state);
    EXPECT_CALL(*mock_app_, UpdateHash());

    MessageSharedPtr msg_ui = PrepareResponseFromHMI(ui_hmi_response, ui_info);

    Event event_ui(hmi_apis::FunctionID::UI_SetGlobalProperties);
    event_ui.set_smart_object(*msg_ui);

    MessageSharedPtr msg_tts =
        PrepareResponseFromHMI(tts_hmi_response, tts_info);

    Event event_tts(hmi_apis::FunctionID::TTS_SetGlobalProperties);
    event_tts.set_smart_object(*msg_tts);

    command->Run();
    command->on_event(event_tts);
    SetHMIInterfaceState(ui_state, tts_state);

    MessageSharedPtr result_to_mobile;
    EXPECT_CALL(app_mngr_,
                ManageMobileCommand(
                    _, am::commands::Command::CommandOrigin::ORIGIN_SDL))
        .WillOnce(DoAll(SaveArg<0>(&result_to_mobile), Return(true)));

    command->on_event(event_ui);

    EXPECT_EQ((*result_to_mobile)[am::strings::msg_params][am::strings::success]
                  .asBool(),
              success);
    EXPECT_EQ(
        (*result_to_mobile)[am::strings::msg_params][am::strings::result_code]
            .asInt(),
        static_cast<int32_t>(mobile_response));
    if (mobile_info) {
      EXPECT_EQ((*result_to_mobile)[am::strings::msg_params][am::strings::info]
                    .asString(),
                mobile_info);
    }
  }

  sync_primitives::Lock lock_;
  NiceMock<MockHmiInterfaces> hmi_interfaces_;
  MockMessageHelper& mock_message_helper_;
  MockAppPtr mock_app_;
};

TEST_F(SetGlobalPropertiesRequestTest,
       OnEvent_UIHmiSendSuccess_UNSUPPORTED_RESOURCE) {
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  const hmi_apis::Common_Result::eType tts_hmi_response =
      hmi_apis::Common_Result::SUCCESS;
  const char* ui_info = "UI is not supported by system";
  const char* tts_info = NULL;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::UNSUPPORTED_RESOURCE;
  const char* mobile_info = "UI is not supported by system";
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_NOT_AVAILABLE;
  const am::HmiInterfaces::InterfaceState tts_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const bool success = true;
  CheckExpectations(ui_hmi_response,
                    tts_hmi_response,
                    ui_info,
                    tts_info,
                    mobile_response,
                    mobile_info,
                    ui_state,
                    tts_state,
                    success);
}

TEST_F(
    SetGlobalPropertiesRequestTest,
    BothInterfaceIsAvailable_TTSResultUnsupported_UIResultSUCCESS_MobileResultWarning) {
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::SUCCESS;
  const hmi_apis::Common_Result::eType tts_hmi_response =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  const char* ui_info = NULL;
  const char* tts_info = NULL;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::WARNINGS;
  const char* mobile_info = NULL;
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const am::HmiInterfaces::InterfaceState tts_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const bool success = true;
  CheckExpectations(ui_hmi_response,
                    tts_hmi_response,
                    ui_info,
                    tts_info,
                    mobile_response,
                    mobile_info,
                    ui_state,
                    tts_state,
                    success);
}

TEST_F(
    SetGlobalPropertiesRequestTest,
    BothInterfaceIsAvailable_TTSResultSUCCESS_UIResultWARNINGS_MobileResultWarning) {
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::WARNINGS;
  const hmi_apis::Common_Result::eType tts_hmi_response =
      hmi_apis::Common_Result::SUCCESS;
  const char* ui_info = NULL;
  const char* tts_info = NULL;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::WARNINGS;
  const char* mobile_info = NULL;
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const am::HmiInterfaces::InterfaceState tts_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const bool success = true;
  CheckExpectations(ui_hmi_response,
                    tts_hmi_response,
                    ui_info,
                    tts_info,
                    mobile_response,
                    mobile_info,
                    ui_state,
                    tts_state,
                    success);
}

TEST_F(
    SetGlobalPropertiesRequestTest,
    TTSInterfaceNotRespond_TTSResultUnsupported_UIResultWARNINGS_MobileResultUnsupported) {
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::WARNINGS;
  const hmi_apis::Common_Result::eType tts_hmi_response =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  const char* ui_info = NULL;
  const char* tts_info = NULL;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::UNSUPPORTED_RESOURCE;
  const char* mobile_info = NULL;
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const am::HmiInterfaces::InterfaceState tts_state =
      am::HmiInterfaces::STATE_NOT_RESPONSE;
  const bool success = true;
  CheckExpectations(ui_hmi_response,
                    tts_hmi_response,
                    ui_info,
                    tts_info,
                    mobile_response,
                    mobile_info,
                    ui_state,
                    tts_state,
                    success);
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_SUCCESS_Expect_MessageNotSend) {
  MessageSharedPtr response = CreateMessage(smart_objects::SmartType_Map);
  (*response)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*response)[am::strings::msg_params][am::strings::info] = "test";

  am::event_engine::Event event(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event.set_smart_object(*response);

  utils::SharedPtr<SetGlobalPropertiesRequest> command =
      CreateCommand<SetGlobalPropertiesRequest>(response);

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .Times(0);
  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest,
       OnEvent_UNSUPPORTED_RESOURCE_Expect_false) {
  MessageSharedPtr response = CreateMessage(smart_objects::SmartType_Map);
  (*response)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*response)[am::strings::msg_params][am::strings::info] = "qwe";

  am::event_engine::Event event_tts(
      hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event_tts.set_smart_object(*response);
  am::event_engine::Event event_ui(
      hmi_apis::FunctionID::UI_SetGlobalProperties);
  event_tts.set_smart_object(*response);
  utils::SharedPtr<SetGlobalPropertiesRequest> command =
      CreateCommand<SetGlobalPropertiesRequest>(response);

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));

  ON_CALL(mock_message_helper_, HMIToMobileResult(_))
      .WillByDefault(Return(mobile_apis::Result::UNSUPPORTED_RESOURCE));

  MockHmiInterfaces hmi_interfaces;
  EXPECT_CALL(app_mngr_, hmi_interfaces())
      .WillRepeatedly(ReturnRef(hmi_interfaces));
  EXPECT_CALL(hmi_interfaces, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  MessageSharedPtr response_to_mobile;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command->Run();
  command->on_event(event_ui);
  command->on_event(event_tts);

  EXPECT_EQ((*response_to_mobile)[am::strings::msg_params][am::strings::success]
                .asBool(),
            false);
  EXPECT_EQ(
      (*response_to_mobile)[am::strings::msg_params][am::strings::result_code]
          .asInt(),
      static_cast<int32_t>(mobile_apis::Result::INVALID_DATA));
}

}  // namespace set_global_properties_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests
