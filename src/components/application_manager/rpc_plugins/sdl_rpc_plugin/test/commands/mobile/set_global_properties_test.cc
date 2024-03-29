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

#include <stdint.h>
#include <memory>
#include <set>
#include <string>

#include "mobile/set_global_properties_request.h"

#include "application_manager/commands/command_request_test.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/message_helper.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_help_prompt_manager.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_message_helper.h"
#include "gtest/gtest.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace set_global_properties_request {

namespace am = application_manager;
using am::CommandsMap;
using am::MockMessageHelper;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::SetGlobalPropertiesRequest;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using utils::custom_string::CustomString;

namespace {
const int32_t kCommandId = 1;
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 1u;
const std::string kText = "one";
const uint32_t kPosition = 1u;

const std::vector<hmi_apis::Common_Result::eType> success_result_codes{
    hmi_apis::Common_Result::SUCCESS,
    hmi_apis::Common_Result::WARNINGS,
    hmi_apis::Common_Result::WRONG_LANGUAGE,
    hmi_apis::Common_Result::RETRY,
    hmi_apis::Common_Result::SAVED};
}  // namespace

class SetGlobalPropertiesRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SetGlobalPropertiesRequestTest()
      : lock_ptr_(std::make_shared<sync_primitives::Lock>())
      , mock_app_(CreateMockApp()) {
    mock_help_prompt_manager_ =
        std::shared_ptr<application_manager_test::MockHelpPromptManager>(
            new application_manager_test::MockHelpPromptManager());
  }

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
    msg_params[am::strings::vr_help_title] = "vr_help_title";
    msg_params[am::strings::vr_help][0][am::strings::text] = "vr_help";
    msg_params[am::strings::vr_help][0][am::strings::position] = 1u;
    msg_params[am::strings::help_prompt][0][am::strings::text] = "help_promt";
    (*msg)[am::strings::msg_params] = msg_params;

    return msg;
  }

  MessageSharedPtr CreateMsgParams() {
    MessageSharedPtr msg = CreateMessage();
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    (*msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
    return msg;
  }

  void VRArraySetupHelper(MessageSharedPtr msg,
                          smart_objects::SmartObjectSPtr vr_help_title,
                          smart_objects::SmartObjectSPtr vr_help_array) {
    (*msg)[am::strings::msg_params][am::strings::vr_help_title] =
        *vr_help_title;
    (*vr_help_array)[0] = SmartObject(smart_objects::SmartType_Map);
    (*vr_help_array)[0][am::strings::text] = kText;
    (*vr_help_array)[0][am::strings::position] = kPosition;
    (*msg)[am::strings::msg_params][am::strings::vr_help] = *vr_help_array;
    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
  }

  void OnEventUISetupHelper(
      MessageSharedPtr msg,
      std::shared_ptr<SetGlobalPropertiesRequest> command) {
    smart_objects::SmartObjectSPtr vr_help_title =
        std::make_shared<SmartObject>("yes");
    smart_objects::SmartObjectSPtr vr_help_array =
        std::make_shared<SmartObject>(smart_objects::SmartType_Array);
    VRArraySetupHelper(msg, vr_help_title, vr_help_array);
    EXPECT_CALL(mock_message_helper_,
                VerifyImageVrHelpItems(*vr_help_array, _, _))
        .WillOnce((Return(mobile_apis::Result::SUCCESS)));
    EXPECT_CALL(app_mngr_,
                RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
    EXPECT_CALL(*mock_app_, set_vr_help_title(*vr_help_title));
    EXPECT_CALL(*mock_app_, set_vr_help(*vr_help_array));
    EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(vr_help_title));
    EXPECT_CALL(*mock_app_, vr_help()).WillOnce(Return(vr_help_array));
    EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);
    EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
    EXPECT_CALL(*mock_app_, set_keyboard_props(_)).Times(0);
    EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

    EXPECT_CALL(*mock_app_, help_prompt_manager())
        .WillRepeatedly(ReturnRef(*mock_help_prompt_manager_.get()));
    EXPECT_CALL(*mock_help_prompt_manager_,
                OnSetGlobalPropertiesReceived(_, false));

    command->Run();
  }

  void OnEventTTSSetupHelper(
      MessageSharedPtr msg,
      std::shared_ptr<SetGlobalPropertiesRequest> command) {
    smart_objects::SmartObjectSPtr help_prompt =
        std::make_shared<SmartObject>(smart_objects::SmartType_Array);
    (*help_prompt)[0][am::strings::text] = "Help_Prompt_One";
    (*msg)[am::strings::msg_params][am::strings::help_prompt] = *help_prompt;
    smart_objects::SmartObjectSPtr timeout_prompt =
        std::make_shared<SmartObject>(smart_objects::SmartType_Array);
    (*timeout_prompt)[0][am::strings::text] = "Timeout_Prompt_One";
    (*msg)[am::strings::msg_params][am::strings::timeout_prompt] =
        *timeout_prompt;

    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
    EXPECT_CALL(mock_message_helper_, VerifyTtsFiles(*help_prompt, _, _))
        .WillOnce(Return(mobile_apis::Result::SUCCESS));
    EXPECT_CALL(mock_message_helper_, VerifyTtsFiles(*timeout_prompt, _, _))
        .WillOnce(Return(mobile_apis::Result::SUCCESS));
    EXPECT_CALL(app_mngr_,
                RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
    EXPECT_CALL(*mock_app_, set_help_prompt(*help_prompt));
    EXPECT_CALL(*mock_app_, help_prompt()).WillOnce(Return(help_prompt));
    EXPECT_CALL(*mock_app_, set_timeout_prompt(*timeout_prompt));
    EXPECT_CALL(*mock_app_, timeout_prompt()).WillOnce(Return(timeout_prompt));
    EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

    EXPECT_CALL(*mock_app_, help_prompt_manager())
        .WillRepeatedly(ReturnRef(*mock_help_prompt_manager_.get()));
    EXPECT_CALL(*mock_help_prompt_manager_,
                OnSetGlobalPropertiesReceived(_, false));

    command->Run();
  }

  void EmptyExpectationsSetupHelper() {
    EXPECT_CALL(*mock_app_, set_vr_help_title(_)).Times(0);
    EXPECT_CALL(*mock_app_, set_vr_help(_)).Times(0);
    EXPECT_CALL(*mock_app_, vr_help_title()).Times(0);
    EXPECT_CALL(*mock_app_, vr_help()).Times(0);
    EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);
    EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
    EXPECT_CALL(*mock_app_, set_keyboard_props(_)).Times(0);
    EXPECT_CALL(*mock_app_, app_id()).Times(0);
  }

  void ExpectInvalidData() {
    EXPECT_CALL(mock_rpc_service_,
                ManageMobileCommand(
                    MobileResultCodeIs(mobile_apis::Result::INVALID_DATA),
                    am::commands::Command::SOURCE_SDL));
  }

  void ExpectVerifyImageVrHelpSuccess(SmartObject& smart_obj) {
    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(smart_obj, _, _))
        .WillOnce(Return(mobile_apis::Result::SUCCESS));
    EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(_)).Times(0);
  }

  void ExpectVerifyImageVrHelpUnsuccess() {
    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
    EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(_)).Times(0);
  }

  void ExpectVerifyImageSuccess(SmartObject& smart_obj) {
    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(mock_message_helper_, VerifyImage(smart_obj, _, _))
        .WillOnce(Return(mobile_apis::Result::SUCCESS));
    EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(_)).Times(0);
  }

  void SetUp() OVERRIDE {
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
    ON_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
    ON_CALL(mock_app_manager_, hmi_interfaces())
        .WillByDefault(ReturnRef(mock_hmi_interfaces_));
    ON_CALL(mock_message_helper_,
            MobileToHMIResult(mobile_apis::Result::UNSUPPORTED_RESOURCE))
        .WillByDefault(Return(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE));
    ON_CALL(mock_message_helper_,
            MobileToHMIResult(mobile_apis::Result::WARNINGS))
        .WillByDefault(Return(hmi_apis::Common_Result::WARNINGS));
  }

  void ResultCommandExpectations(MessageSharedPtr msg,
                                 const std::string& info) {
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::success].asBool(),
              true);
    EXPECT_EQ(
        static_cast<int32_t>(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE),
        (*msg)[am::strings::msg_params][am::strings::result_code].asInt());
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::info].asString(),
              info);
  }

  void ExpectationsHmiInterface_Run() {
    EXPECT_CALL(
        mock_hmi_interfaces_,
        GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
        .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
    EXPECT_CALL(
        mock_hmi_interfaces_,
        GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
        .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
    ON_CALL(mock_hmi_interfaces_,
            GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
    ON_CALL(mock_hmi_interfaces_,
            GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  }

  // Checks total result code for each properties
  void PrepareResultCodeForResponse_CheckTotalCode(
      const hmi_apis::Common_Result::eType& ui_result,
      const hmi_apis::Common_Result::eType& tts_result,
      const hmi_apis::Common_Result::eType& rc_result,
      const mobile_apis::Result::eType expected_total_result_code) {
    using namespace application_manager;
    using namespace hmi_apis;

    commands::ResponseInfo ui_properties_info(
        ui_result, HmiInterfaces::HMI_INTERFACE_UI, mock_app_manager_);
    commands::ResponseInfo tts_properties_info(
        tts_result, HmiInterfaces::HMI_INTERFACE_TTS, mock_app_manager_);
    commands::ResponseInfo rc_properties_info(
        rc_result, HmiInterfaces::HMI_INTERFACE_RC, mock_app_manager_);

    MessageSharedPtr msg = CreateMsgParams();
    std::shared_ptr<SetGlobalPropertiesRequest> command(
        CreateCommand<SetGlobalPropertiesRequest>(msg));

    auto result = command->PrepareResultCodeForResponse(
        ui_properties_info, tts_properties_info, rc_properties_info);

    EXPECT_EQ(expected_total_result_code, result);
  }

  // Sets in rotation for each result UNSUPPROTED result code and checks total
  // result code
  void PrepareResultCodeForResponse_CheckAllResultsForSpecifiedTotalResult(
      mobile_apis::Result::eType expected_total_result_code) {
    using namespace application_manager;
    using namespace hmi_apis;

    Common_Result::eType ui_result =
        MessageHelper::MobileToHMIResult(expected_total_result_code);
    Common_Result::eType tts_result = Common_Result::SUCCESS;
    Common_Result::eType rc_result = Common_Result::SUCCESS;

    PrepareResultCodeForResponse_CheckTotalCode(
        ui_result, tts_result, rc_result, expected_total_result_code);

    ui_result = Common_Result::SUCCESS;
    tts_result = MessageHelper::MobileToHMIResult(expected_total_result_code);

    PrepareResultCodeForResponse_CheckTotalCode(
        ui_result, tts_result, rc_result, expected_total_result_code);

    tts_result = Common_Result::SUCCESS;
    rc_result = MessageHelper::MobileToHMIResult(expected_total_result_code);

    PrepareResultCodeForResponse_CheckTotalCode(
        ui_result, tts_result, rc_result, expected_total_result_code);
  }

  void PrepareResultForMobileResponse_CheckTotalReault(
      const hmi_apis::Common_Result::eType& ui_result,
      const hmi_apis::Common_Result::eType& tts_result,
      const hmi_apis::Common_Result::eType& rc_result,
      const bool expected_total_result) {
    using namespace application_manager;
    using namespace hmi_apis;

    commands::ResponseInfo ui_properties_info(
        ui_result, HmiInterfaces::HMI_INTERFACE_UI, mock_app_manager_);
    commands::ResponseInfo tts_properties_info(
        tts_result, HmiInterfaces::HMI_INTERFACE_TTS, mock_app_manager_);
    commands::ResponseInfo rc_properties_info(
        rc_result, HmiInterfaces::HMI_INTERFACE_RC, mock_app_manager_);

    MessageSharedPtr msg = CreateMsgParams();
    std::shared_ptr<SetGlobalPropertiesRequest> command(
        CreateCommand<SetGlobalPropertiesRequest>(msg));

    bool result = command->PrepareResultForMobileResponse(
        ui_properties_info, tts_properties_info, rc_properties_info);

    EXPECT_EQ(expected_total_result, result);
  }

  void PrepareResultForMobileResponse_CheckResultsForAllCases() {
    using namespace application_manager;
    using namespace hmi_apis;

    Common_Result::eType ui_result = Common_Result::SUCCESS;
    Common_Result::eType tts_result = Common_Result::SUCCESS;
    Common_Result::eType rc_result = Common_Result::SUCCESS;

    PrepareResultForMobileResponse_CheckTotalReault(
        ui_result, tts_result, rc_result, true);

    // Result code isn't success
    EXPECT_FALSE(helpers::in_range(success_result_codes,
                                   Common_Result::UNSUPPORTED_REQUEST));

    ui_result = Common_Result::UNSUPPORTED_REQUEST;
    tts_result = Common_Result::SUCCESS;
    rc_result = Common_Result::SUCCESS;

    PrepareResultForMobileResponse_CheckTotalReault(
        ui_result, tts_result, rc_result, false);

    ui_result = Common_Result::SUCCESS;
    tts_result = Common_Result::UNSUPPORTED_REQUEST;
    rc_result = Common_Result::SUCCESS;

    PrepareResultForMobileResponse_CheckTotalReault(
        ui_result, tts_result, rc_result, false);

    ui_result = Common_Result::SUCCESS;
    tts_result = Common_Result::SUCCESS;
    rc_result = Common_Result::UNSUPPORTED_REQUEST;

    PrepareResultForMobileResponse_CheckTotalReault(
        ui_result, tts_result, rc_result, false);
  }

  void AddCustomizableKeys(MessageSharedPtr msg) {
    SmartObject customizable_keys(smart_objects::SmartType_Array);
    customizable_keys[0] = "%";
    customizable_keys[1] = "@";
    customizable_keys[2] = "&";
    (*msg)[am::strings::msg_params][am::strings::keyboard_properties]
          [am::hmi_request::custom_keys] = customizable_keys;
  }

  std::shared_ptr<SmartObject> GetCapabilitiesForConfigurableKeyboard(
      hmi_apis::Common_KeyboardLayout::eType layout, int num_allowed_keys) {
    auto display_capabilities =
        std::make_shared<SmartObject>(smart_objects::SmartType_Map);
    auto& supported_keyboards =
        (*display_capabilities)[0][am::strings::window_capabilities][0]
                               [am::hmi_response::keyboard_capabilities]
                               [am::hmi_response::supported_keyboards];
    supported_keyboards[0][am::hmi_request::keyboard_layout] = layout;
    supported_keyboards[0][am::hmi_response::num_configurable_keys] =
        num_allowed_keys;
    return display_capabilities;
  }

  std::shared_ptr<sync_primitives::Lock> lock_ptr_;
  MockAppPtr mock_app_;
  std::shared_ptr<application_manager_test::MockHelpPromptManager>
      mock_help_prompt_manager_;
  NiceMock<MockApplicationManager> mock_app_manager_;
};

TEST_F(SetGlobalPropertiesRequestTest,
       OnEvent_UIHmiSendSuccess_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg_vr = CreateFullParamsUISO();
  (*msg_vr)[am::strings::msg_params][am::strings::vr_commands][0] =
      "vr_command";

  std::shared_ptr<SetGlobalPropertiesRequest> command =
      CreateCommand<SetGlobalPropertiesRequest>(msg_vr);

  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  const smart_objects::SmartObjectSPtr vr_help_title =
      std::make_shared<SmartObject>(
          (*msg_vr)[am::strings::msg_params][am::strings::vr_help_title]);
  const smart_objects::SmartObjectSPtr vr_help = std::make_shared<SmartObject>(
      (*msg_vr)[am::strings::msg_params][am::strings::vr_help]);
  const smart_objects::SmartObjectSPtr vr_help_prompt =
      std::make_shared<SmartObject>(
          (*msg_vr)[am::strings::msg_params][am::strings::help_prompt]);
  ON_CALL(*mock_app_, vr_help_title()).WillByDefault(Return(vr_help_title));
  ON_CALL(*mock_app_, vr_help()).WillByDefault(Return(vr_help));
  ON_CALL(*mock_app_, help_prompt()).WillByDefault(Return(vr_help_prompt));

  ExpectationsHmiInterface_Run();

  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg)[am::strings::msg_params][am::strings::cmd_id] = kCommandId;
  (*msg)[am::strings::msg_params][am::strings::info] =
      "UI is not supported by system";

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  ON_CALL(mock_message_helper_,
          VerifyImageVrHelpItems(
              (*msg_vr)[am::strings::msg_params][am::strings::vr_help], _, _))
      .WillByDefault(Return(mobile_apis::Result::SUCCESS));

  ON_CALL(mock_message_helper_, VerifyImage(_, _, _))
      .WillByDefault(Return(mobile_apis::Result::SUCCESS));

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillRepeatedly(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, false))
      .Times(2);

  EXPECT_CALL(
      mock_message_helper_,
      VerifyTtsFiles(
          (*msg_vr)[am::strings::msg_params][am::strings::help_prompt], _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::UI_SetGlobalProperties), _))
      .WillOnce(Return(true));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::TTS_SetGlobalProperties), _))
      .WillOnce(Return(true));

  (*msg_vr)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  Event event_vr(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event_vr.set_smart_object(*msg_vr);

  command->Run();

  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, true));
  command->on_event(event_vr);

  MessageSharedPtr ui_command_result;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, true));

  command->on_event(event);

  ResultCommandExpectations(ui_command_result, "UI is not supported by system");
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_SUCCESS_Expect_MessageNotSend) {
  MessageSharedPtr response = CreateMessage(smart_objects::SmartType_Map);
  (*response)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*response)[am::strings::msg_params][am::strings::info] = "test";

  am::event_engine::Event event(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event.set_smart_object(*response);

  std::shared_ptr<SetGlobalPropertiesRequest> command =
      CreateCommand<SetGlobalPropertiesRequest>(response);

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));

  EXPECT_CALL(*mock_app, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, true));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .Times(0);
  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest,
       OnEvent_UNSUPPORTED_RESOURCE_Expect_false) {
  MessageSharedPtr response = CreateMessage(smart_objects::SmartType_Map);
  (*response)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*response)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*response)[am::strings::msg_params][am::strings::info] = "qwe";

  am::event_engine::Event event_tts(
      hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event_tts.set_smart_object(*response);
  am::event_engine::Event event_ui(
      hmi_apis::FunctionID::UI_SetGlobalProperties);
  event_tts.set_smart_object(*response);
  std::shared_ptr<SetGlobalPropertiesRequest> command =
      CreateCommand<SetGlobalPropertiesRequest>(response);

  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillRepeatedly(ReturnRef(*mock_help_prompt_manager_.get()));

  MessageSharedPtr response_to_mobile;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command->Run();

  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, true))
      .Times(2);
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

TEST_F(SetGlobalPropertiesRequestTest, Run_VRNoMenuAndKeyboard_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));
  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));

  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  OnEventUISetupHelper(msg, command);
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRWithMenuAndKeyboard_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  smart_objects::SmartObjectSPtr vr_help_title =
      std::make_shared<SmartObject>("yes");
  smart_objects::SmartObjectSPtr vr_help_array =
      std::make_shared<SmartObject>(smart_objects::SmartType_Array);
  VRArraySetupHelper(msg, vr_help_title, vr_help_array);
  (*msg)[am::strings::msg_params][am::strings::vr_help] = *vr_help_array;
  SmartObject menu_title("Menu_Title");
  (*msg)[am::strings::msg_params][am::hmi_request::menu_title] = menu_title;
  SmartObject menu_icon(smart_objects::SmartType_Map);
  menu_icon[am::strings::value] = "1";
  (*msg)[am::strings::msg_params][am::hmi_request::menu_icon] = menu_icon;
  smart_objects::SmartObjectSPtr keyboard_properties =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      *keyboard_properties;

  EXPECT_CALL(mock_message_helper_, VerifyImage(menu_icon, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(mock_message_helper_,
              VerifyImageVrHelpItems(*vr_help_array, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EXPECT_CALL(*mock_app_, set_vr_help_title(*vr_help_title));
  EXPECT_CALL(*mock_app_, set_vr_help(*vr_help_array));
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(vr_help_title));
  EXPECT_CALL(*mock_app_, vr_help()).WillOnce(Return(vr_help_array));
  EXPECT_CALL(*mock_app_, set_menu_title(menu_title));
  EXPECT_CALL(*mock_app_, set_menu_icon(menu_icon));
  EXPECT_CALL(*mock_app_, set_keyboard_props(*keyboard_properties));
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillOnce(Return(keyboard_properties));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));

  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, false));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_VRWithMenuAndKeyboard_Menu_WARNINGS) {
  MessageSharedPtr msg = CreateMsgParams();
  smart_objects::SmartObjectSPtr vr_help_title =
      std::make_shared<SmartObject>("yes");
  smart_objects::SmartObjectSPtr vr_help_array =
      std::make_shared<SmartObject>(smart_objects::SmartType_Array);
  VRArraySetupHelper(msg, vr_help_title, vr_help_array);
  (*msg)[am::strings::msg_params][am::strings::vr_help] = *vr_help_array;
  SmartObject menu_title("Menu_Title");
  (*msg)[am::strings::msg_params][am::hmi_request::menu_title] = menu_title;
  SmartObject menu_icon(smart_objects::SmartType_Map);
  menu_icon[am::strings::value] = "1";
  (*msg)[am::strings::msg_params][am::hmi_request::menu_icon] = menu_icon;
  smart_objects::SmartObjectSPtr keyboard_properties =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      *keyboard_properties;

  EXPECT_CALL(mock_message_helper_, VerifyImage(menu_icon, _, _))
      .WillOnce((Return(mobile_apis::Result::WARNINGS)));
  EXPECT_CALL(mock_message_helper_,
              VerifyImageVrHelpItems(*vr_help_array, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EXPECT_CALL(*mock_app_, set_vr_help_title(*vr_help_title));
  EXPECT_CALL(*mock_app_, set_vr_help(*vr_help_array));
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(vr_help_title));
  EXPECT_CALL(*mock_app_, vr_help()).WillOnce(Return(vr_help_array));
  EXPECT_CALL(*mock_app_, set_menu_title(menu_title));
  EXPECT_CALL(*mock_app_, set_menu_icon(menu_icon));
  EXPECT_CALL(*mock_app_, set_keyboard_props(*keyboard_properties));
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillOnce(Return(keyboard_properties));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, false));

  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));

  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRWithMenuAndKeyboard_VR_WARNINGS) {
  MessageSharedPtr msg = CreateMsgParams();
  smart_objects::SmartObjectSPtr vr_help_title =
      std::make_shared<SmartObject>("yes");
  smart_objects::SmartObjectSPtr vr_help_array =
      std::make_shared<SmartObject>(smart_objects::SmartType_Array);
  VRArraySetupHelper(msg, vr_help_title, vr_help_array);
  (*msg)[am::strings::msg_params][am::strings::vr_help] = *vr_help_array;
  SmartObject menu_title("Menu_Title");
  (*msg)[am::strings::msg_params][am::hmi_request::menu_title] = menu_title;
  SmartObject menu_icon(smart_objects::SmartType_Map);
  menu_icon[am::strings::value] = "1";
  (*msg)[am::strings::msg_params][am::hmi_request::menu_icon] = menu_icon;
  smart_objects::SmartObjectSPtr keyboard_properties =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      *keyboard_properties;

  EXPECT_CALL(mock_message_helper_, VerifyImage(menu_icon, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(mock_message_helper_,
              VerifyImageVrHelpItems(*vr_help_array, _, _))
      .WillOnce((Return(mobile_apis::Result::WARNINGS)));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EXPECT_CALL(*mock_app_, set_vr_help_title(*vr_help_title));
  EXPECT_CALL(*mock_app_, set_vr_help(*vr_help_array));
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(vr_help_title));
  EXPECT_CALL(*mock_app_, vr_help()).WillOnce(Return(vr_help_array));
  EXPECT_CALL(*mock_app_, set_menu_title(menu_title));
  EXPECT_CALL(*mock_app_, set_menu_icon(menu_icon));
  EXPECT_CALL(*mock_app_, set_keyboard_props(*keyboard_properties));
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillOnce(Return(keyboard_properties));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, false));

  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));

  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRBrokenMenuIcon_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help_title("yes");
  (*msg)[am::strings::msg_params][am::strings::vr_help_title] = vr_help_title;
  SmartObject menu_icon(smart_objects::SmartType_Map);
  menu_icon[am::strings::value] = " 1";
  (*msg)[am::strings::msg_params][am::hmi_request::menu_icon] = menu_icon;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(menu_icon, _, _))
      .WillOnce((Return(mobile_apis::Result::ABORTED)));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EmptyExpectationsSetupHelper();
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRBrokenVRHelp_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  smart_objects::SmartObjectSPtr vr_help_title =
      std::make_shared<SmartObject>("yes");
  SmartObject menu_icon(smart_objects::SmartType_Map);
  menu_icon[am::strings::value] = "1";
  (*msg)[am::strings::msg_params][am::hmi_request::menu_icon] = menu_icon;
  smart_objects::SmartObjectSPtr vr_help_array =
      std::make_shared<SmartObject>(smart_objects::SmartType_Array);
  VRArraySetupHelper(msg, vr_help_title, vr_help_array);
  SmartObject menu_title("Menu_Title");
  (*msg)[am::strings::msg_params][am::hmi_request::menu_title] = menu_title;

  EXPECT_CALL(mock_message_helper_, VerifyImage(menu_icon, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(mock_message_helper_,
              VerifyImageVrHelpItems(*vr_help_array, _, _))
      .WillOnce((Return(mobile_apis::Result::ABORTED)));
  EmptyExpectationsSetupHelper();

  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(vr_help_title));
  EXPECT_CALL(*mock_app_, vr_help()).WillOnce(Return(vr_help_array));
  EXPECT_CALL(*mock_app_, set_vr_help_title(*vr_help_title));
  EXPECT_CALL(*mock_app_, set_vr_help(*vr_help_array));
  EXPECT_CALL(*mock_app_, set_menu_title(menu_title));
  EXPECT_CALL(*mock_app_, set_menu_icon(menu_icon));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, false));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRIncorrectSyntax_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
  smart_objects::SmartObjectSPtr vr_help_title =
      std::make_shared<SmartObject>("wrong syntax string\\n");
  SmartObject menu_icon(smart_objects::SmartType_Map);
  menu_icon[am::strings::value] = "1";
  (*msg)[am::strings::msg_params][am::hmi_request::menu_icon] = menu_icon;
  smart_objects::SmartObjectSPtr vr_help_array =
      std::make_shared<SmartObject>(smart_objects::SmartType_Array);
  VRArraySetupHelper(msg, vr_help_title, vr_help_array);

  EXPECT_CALL(mock_message_helper_, VerifyImage(menu_icon, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(mock_message_helper_,
              VerifyImageVrHelpItems(*vr_help_array, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(_)).Times(0);
  EmptyExpectationsSetupHelper();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRMissingTitle_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help_array(smart_objects::SmartType_Array);
  vr_help_array[0] = SmartObject(smart_objects::SmartType_Map);
  vr_help_array[0][am::strings::text] = kText;
  vr_help_array[0][am::strings::position] = kPosition;
  (*msg)[am::strings::msg_params][am::strings::vr_help] = vr_help_array;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(vr_help_array, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EmptyExpectationsSetupHelper();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRMissingArray_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help_title("yes");
  (*msg)[am::strings::msg_params][am::strings::vr_help_title] = vr_help_title;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EmptyExpectationsSetupHelper();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRWrongOrder_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  smart_objects::SmartObjectSPtr vr_help_title =
      std::make_shared<SmartObject>("yes");
  smart_objects::SmartObjectSPtr vr_help_array =
      std::make_shared<SmartObject>(smart_objects::SmartType_Array);
  VRArraySetupHelper(msg, vr_help_title, vr_help_array);
  (*vr_help_array)[1] = SmartObject(smart_objects::SmartType_Map);
  (*vr_help_array)[1][am::strings::text] = "two";
  (*vr_help_array)[1][am::strings::position] = 3u;
  (*msg)[am::strings::msg_params][am::strings::vr_help] = *vr_help_array;

  EXPECT_CALL(mock_message_helper_,
              VerifyImageVrHelpItems(*vr_help_array, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EmptyExpectationsSetupHelper();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_NoVR_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  smart_objects::SmartObjectSPtr keyboard_properties =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      *keyboard_properties;
  SmartObject menu_title("Menu_Title");
  (*msg)[am::strings::msg_params][am::hmi_request::menu_title] = menu_title;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EXPECT_CALL(*mock_app_, set_menu_title(menu_title));
  EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_keyboard_props(*keyboard_properties));
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillOnce(Return(keyboard_properties));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRCouldNotGenerate_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  smart_objects::SmartObjectSPtr keyboard_properties =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      *keyboard_properties;
  SmartObject menu_title("Menu_Title");
  (*msg)[am::strings::msg_params][am::hmi_request::menu_title] = menu_title;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EXPECT_CALL(*mock_app_, set_keyboard_props(*keyboard_properties));
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillOnce(Return(std::shared_ptr<SmartObject>()));
  EXPECT_CALL(*mock_app_, set_menu_title(menu_title));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_NoVRNoDataNoDefault_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      keyboard_properties;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_keyboard_props(_));
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillOnce(Return(std::shared_ptr<SmartObject>()));
  EXPECT_CALL(*mock_app_, app_id());

  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_NoVRNoDataDefaultCreated_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      keyboard_properties;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_keyboard_props(keyboard_properties));
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillOnce(Return(std::shared_ptr<SmartObject>()));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_NoVRNoDataFromSynonyms_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      keyboard_properties;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));

  SmartObject vr_help_array(smart_objects::SmartType_Array);
  vr_help_array[0] = SmartObject(smart_objects::SmartType_Map);
  vr_help_array[0][am::strings::text] = kText;
  vr_help_array[0][am::strings::position] = kPosition;
  SmartObject vr_synonyms(smart_objects::SmartType_Array);
  vr_synonyms[0] = vr_help_array;
  EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_keyboard_props(keyboard_properties));
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillOnce(Return(std::shared_ptr<SmartObject>()));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_TTSHelpAndTimeout_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  smart_objects::SmartObjectSPtr help_prompt =
      std::make_shared<SmartObject>(smart_objects::SmartType_Array);
  (*help_prompt)[0][am::strings::text] = "Help_Prompt_One";
  (*msg)[am::strings::msg_params][am::strings::help_prompt] = *help_prompt;
  smart_objects::SmartObjectSPtr timeout_prompt =
      std::make_shared<SmartObject>(smart_objects::SmartType_Array);
  (*timeout_prompt)[0][am::strings::text] = "Timeout_Prompt_One";
  (*msg)[am::strings::msg_params][am::strings::timeout_prompt] =
      *timeout_prompt;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(mock_message_helper_, VerifyTtsFiles(*help_prompt, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(mock_message_helper_, VerifyTtsFiles(*timeout_prompt, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EXPECT_CALL(*mock_app_, set_help_prompt(*help_prompt));
  EXPECT_CALL(*mock_app_, help_prompt()).WillOnce(Return(help_prompt));
  EXPECT_CALL(*mock_app_, set_timeout_prompt(*timeout_prompt));
  EXPECT_CALL(*mock_app_, timeout_prompt()).WillOnce(Return(timeout_prompt));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, false));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_TTSOnlyHelp_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  smart_objects::SmartObjectSPtr help_prompt =
      std::make_shared<SmartObject>(smart_objects::SmartType_Array);
  (*help_prompt)[0][am::strings::text] = "Help_Prompt_One";
  (*msg)[am::strings::msg_params][am::strings::help_prompt] = *help_prompt;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(mock_message_helper_, VerifyTtsFiles(*help_prompt, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EXPECT_CALL(*mock_app_, set_help_prompt(*help_prompt));
  EXPECT_CALL(*mock_app_, help_prompt()).WillOnce(Return(help_prompt));
  EXPECT_CALL(*mock_app_, set_timeout_prompt(_)).Times(0);
  EXPECT_CALL(*mock_app_, timeout_prompt()).Times(0);
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, false));
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_TTSOnlyTimeout_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  smart_objects::SmartObjectSPtr timeout_prompt =
      std::make_shared<SmartObject>(smart_objects::SmartType_Array);
  (*timeout_prompt)[0][am::strings::text] = "Timeout_Prompt_One";
  (*msg)[am::strings::msg_params][am::strings::timeout_prompt] =
      *timeout_prompt;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(mock_message_helper_, VerifyTtsFiles(*timeout_prompt, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EXPECT_CALL(*mock_app_, set_help_prompt(_)).Times(0);
  EXPECT_CALL(*mock_app_, help_prompt()).Times(0);
  EXPECT_CALL(*mock_app_, set_timeout_prompt(*timeout_prompt));
  EXPECT_CALL(*mock_app_, timeout_prompt()).WillOnce(Return(timeout_prompt));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, false));
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_TTSIncorrectSyntax_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject timeout_prompt(smart_objects::SmartType_Array);
  timeout_prompt[0][am::strings::text] = "Timeout_Prompt_One\\n";
  (*msg)[am::strings::msg_params][am::strings::timeout_prompt] = timeout_prompt;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EmptyExpectationsSetupHelper();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_InvalidHelpPromptText_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject help_prompt(smart_objects::SmartType_Array);
  help_prompt[0][am::strings::text] =
      "invalid help prompt text with empty line in the end\\n";
  (*msg)[am::strings::msg_params][am::strings::help_prompt] = help_prompt;

  ExpectVerifyImageVrHelpUnsuccess();
  EmptyExpectationsSetupHelper();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_InvalidVrHelpText_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help(smart_objects::SmartType_Array);
  vr_help[0][am::strings::text] =
      "invalid vr_help text with empty line in the end\\n";
  (*msg)[am::strings::msg_params][am::strings::vr_help] = vr_help;

  ExpectVerifyImageVrHelpSuccess(vr_help);
  EmptyExpectationsSetupHelper();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_InvalidImageValue_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help(smart_objects::SmartType_Array);
  vr_help[0][am::strings::text] = "vr_help";
  vr_help[0][am::strings::image][am::strings::value] =
      "invalid value text with empty line in the end\\n";
  (*msg)[am::strings::msg_params][am::strings::vr_help] = vr_help;

  ExpectVerifyImageVrHelpSuccess(vr_help);
  EmptyExpectationsSetupHelper();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_InvalidMenuIcon_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject menu_icon(smart_objects::SmartType_Array);
  menu_icon[am::strings::value] =
      "invalid menu icon text with empty line in the end\\n";
  (*msg)[am::strings::msg_params][am::strings::menu_icon] = menu_icon;

  ExpectVerifyImageSuccess(menu_icon);
  EmptyExpectationsSetupHelper();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_InvalidMenuTitle_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject menu_title(smart_objects::SmartType_Array);
  menu_title = "invalid menu title text with empty line in the end\\n";
  (*msg)[am::strings::msg_params][am::strings::menu_title] = menu_title;

  ExpectVerifyImageVrHelpUnsuccess();
  EmptyExpectationsSetupHelper();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_InvalidLimitedCharacterList_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject limited_character_list(smart_objects::SmartType_Array);
  limited_character_list[0] =
      "invalid limited character list text with empty line in the end\\n";
  (*msg)[am::strings::msg_params][am::strings::keyboard_properties]
        [am::strings::limited_character_list] = limited_character_list;

  ExpectVerifyImageVrHelpUnsuccess();
  EmptyExpectationsSetupHelper();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_InvalidAutoCompleteText_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject auto_complete_text(smart_objects::SmartType_Array);
  auto_complete_text =
      "invalid auto completetext with empty line in the end\\n";
  (*msg)[am::strings::msg_params][am::strings::keyboard_properties]
        [am::strings::auto_complete_text] = auto_complete_text;

  ExpectVerifyImageVrHelpUnsuccess();
  EmptyExpectationsSetupHelper();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_InvalidCustomizableKeys_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject customizable_keys(smart_objects::SmartType_Array);
  customizable_keys[0] = "%";
  customizable_keys[1] = "\\n";
  customizable_keys[2] = " ";
  (*msg)[am::strings::msg_params][am::strings::keyboard_properties]
        [am::hmi_request::custom_keys] = customizable_keys;

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(
    SetGlobalPropertiesRequestTest,
    Run_DisallowedNumberOfCustomizableKeysAndLayoutFromRequest_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  AddCustomizableKeys(msg);
  (*msg)[am::strings::msg_params][am::strings::keyboard_properties]
        [am::hmi_request::keyboard_layout] =
            hmi_apis::Common_KeyboardLayout::QWERTZ;

  const int num_allowed_keys = 0;
  auto display_capabilities = GetCapabilitiesForConfigurableKeyboard(
      hmi_apis::Common_KeyboardLayout::QWERTZ, num_allowed_keys);

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  ON_CALL(*mock_app_, display_capabilities())
      .WillByDefault(Return(display_capabilities));
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_DisallowedNumberOfCustomizableKeysAndSavedLayout_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  AddCustomizableKeys(msg);

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  auto saved_keyboard_props =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);
  (*saved_keyboard_props)[am::hmi_request::keyboard_layout] =
      hmi_apis::Common_KeyboardLayout::QWERTZ;
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillOnce(Return(saved_keyboard_props));

  const int num_allowed_keys = 0;
  auto display_capabilities = GetCapabilitiesForConfigurableKeyboard(
      hmi_apis::Common_KeyboardLayout::QWERTZ, num_allowed_keys);

  ON_CALL(*mock_app_, display_capabilities())
      .WillByDefault(Return(display_capabilities));
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_DisallowedNumberOfCustomizableKeysAndDefaultLayout_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  AddCustomizableKeys(msg);

  const int num_allowed_keys = 0;
  auto display_capabilities = GetCapabilitiesForConfigurableKeyboard(
      hmi_apis::Common_KeyboardLayout::QWERTY, num_allowed_keys);

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  ON_CALL(*mock_app_, display_capabilities())
      .WillByDefault(Return(display_capabilities));
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillOnce(Return(std::shared_ptr<SmartObject>()));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_AllowedNumberOfCustomizableKeysAndNotSupportedLayout_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  AddCustomizableKeys(msg);
  (*msg)[am::strings::msg_params][am::strings::keyboard_properties]
        [am::hmi_request::keyboard_layout] =
            hmi_apis::Common_KeyboardLayout::QWERTZ;

  const int num_allowed_keys = 3;
  auto display_capabilities = GetCapabilitiesForConfigurableKeyboard(
      hmi_apis::Common_KeyboardLayout::QWERTY, num_allowed_keys);

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  ON_CALL(*mock_app_, display_capabilities())
      .WillByDefault(Return(display_capabilities));
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_AllowedNumberOfCustomizableKeys_LayoutFromRequestCached) {
  MessageSharedPtr msg = CreateMsgParams();
  AddCustomizableKeys(msg);
  auto& keyboard_properties =
      (*msg)[am::strings::msg_params][am::strings::keyboard_properties];
  keyboard_properties[am::hmi_request::keyboard_layout] =
      hmi_apis::Common_KeyboardLayout::QWERTY;

  const int num_allowed_keys = 3;
  auto display_capabilities = GetCapabilitiesForConfigurableKeyboard(
      hmi_apis::Common_KeyboardLayout::QWERTY, num_allowed_keys);

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  ON_CALL(*mock_app_, display_capabilities())
      .WillByDefault(Return(display_capabilities));
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  EXPECT_CALL(*mock_app_, set_keyboard_props(keyboard_properties));
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillOnce(Return(std::shared_ptr<SmartObject>()));

  command->Run();
}

TEST_F(
    SetGlobalPropertiesRequestTest,
    Run_AllowedNumberOfCustomizableKeysAndSavedLayout_SavedLayoutCachedAgain) {
  MessageSharedPtr msg = CreateMsgParams();
  AddCustomizableKeys(msg);

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  auto saved_keyboard_props =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);
  (*saved_keyboard_props)[am::hmi_request::keyboard_layout] =
      hmi_apis::Common_KeyboardLayout::QWERTZ;
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillRepeatedly(Return(saved_keyboard_props));

  const int num_allowed_keys = 3;
  auto display_capabilities = GetCapabilitiesForConfigurableKeyboard(
      hmi_apis::Common_KeyboardLayout::QWERTZ, num_allowed_keys);

  ON_CALL(*mock_app_, display_capabilities())
      .WillByDefault(Return(display_capabilities));
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  auto requested_keyboard_properties =
      (*msg)[am::strings::msg_params][am::strings::keyboard_properties];
  auto cached_keyboard_props(requested_keyboard_properties);
  cached_keyboard_props[am::hmi_request::keyboard_layout] =
      hmi_apis::Common_KeyboardLayout::QWERTZ;
  EXPECT_CALL(*mock_app_, set_keyboard_props(cached_keyboard_props));

  command->Run();
}

TEST_F(
    SetGlobalPropertiesRequestTest,
    Run_AllowedNumberOfCustomizableKeysAndDefaultLayout_KeyboardPropsCachedAsIs) {
  MessageSharedPtr msg = CreateMsgParams();
  AddCustomizableKeys(msg);

  const int num_allowed_keys = 3;
  auto display_capabilities = GetCapabilitiesForConfigurableKeyboard(
      hmi_apis::Common_KeyboardLayout::QWERTY, num_allowed_keys);

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  ON_CALL(*mock_app_, display_capabilities())
      .WillByDefault(Return(display_capabilities));
  ON_CALL(*mock_app_, keyboard_props())
      .WillByDefault(Return(std::shared_ptr<SmartObject>()));
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  auto requested_keyboard_properties =
      (*msg)[am::strings::msg_params][am::strings::keyboard_properties];
  EXPECT_CALL(*mock_app_, set_keyboard_props(requested_keyboard_properties));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_RequestContainsLanguageParam_KeyboardPropsCachedAsIs) {
  MessageSharedPtr msg = CreateMsgParams();
  auto& keyboard_properties =
      (*msg)[am::strings::msg_params][am::strings::keyboard_properties];
  keyboard_properties[am::hmi_response::language] =
      hmi_apis::Common_Language::EN_GB;

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  EXPECT_CALL(*mock_app_, set_keyboard_props(keyboard_properties));
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillOnce(Return(std::shared_ptr<SmartObject>()));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_NoLanguageInRequestButPresentInSaved_SavedLanguageCachedAgain) {
  MessageSharedPtr msg = CreateMsgParams();

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  auto saved_keyboard_props =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);
  (*saved_keyboard_props)[am::hmi_response::language] =
      hmi_apis::Common_Language::EN_GB;
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillRepeatedly(Return(saved_keyboard_props));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  auto cached_keyboard_props =
      (*msg)[am::strings::msg_params][am::strings::keyboard_properties];
  cached_keyboard_props[am::hmi_response::language] =
      hmi_apis::Common_Language::EN_GB;
  EXPECT_CALL(*mock_app_, set_keyboard_props(cached_keyboard_props));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_RequestContainsMaskInputCharactersParam_KeyboardPropsCachedAsIs) {
  MessageSharedPtr msg = CreateMsgParams();
  auto& keyboard_properties =
      (*msg)[am::strings::msg_params][am::strings::keyboard_properties];
  keyboard_properties[am::hmi_request::mask_input_characters] =
      hmi_apis::Common_KeyboardInputMask::ENABLE_INPUT_KEY_MASK;

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  EXPECT_CALL(*mock_app_, set_keyboard_props(keyboard_properties));
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillOnce(Return(std::shared_ptr<SmartObject>()));

  command->Run();
}

TEST_F(
    SetGlobalPropertiesRequestTest,
    Run_NoMaskInputCharactersInRequestButPresentInSaved_SavedParamCachedAgain) {
  MessageSharedPtr msg = CreateMsgParams();

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  auto saved_keyboard_props =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);
  (*saved_keyboard_props)[am::hmi_request::mask_input_characters] =
      hmi_apis::Common_KeyboardInputMask::USER_CHOICE_INPUT_KEY_MASK;
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillRepeatedly(Return(saved_keyboard_props));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  auto cached_keyboard_props =
      (*msg)[am::strings::msg_params][am::strings::keyboard_properties];
  cached_keyboard_props[am::hmi_request::mask_input_characters] =
      hmi_apis::Common_KeyboardInputMask::USER_CHOICE_INPUT_KEY_MASK;
  EXPECT_CALL(*mock_app_, set_keyboard_props(cached_keyboard_props));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_NoAutocompleteListInRequestButPresentInSaved_SavedArrayCachedAgain) {
  MessageSharedPtr msg = CreateMsgParams();

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  auto saved_keyboard_props =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);

  SmartObject autocomplete_list(smart_objects::SmartType_Array);
  autocomplete_list[0] = "first";
  autocomplete_list[1] = "second";
  (*saved_keyboard_props)[am::hmi_request::auto_complete_list] =
      autocomplete_list;
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillRepeatedly(Return(saved_keyboard_props));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  auto cached_keyboard_props =
      (*msg)[am::strings::msg_params][am::strings::keyboard_properties];
  cached_keyboard_props[am::hmi_request::auto_complete_list] =
      autocomplete_list;
  EXPECT_CALL(*mock_app_, set_keyboard_props(cached_keyboard_props));

  command->Run();
}

TEST_F(
    SetGlobalPropertiesRequestTest,
    Run_NewAutocompleteListInRequestAndAlsoPresentInSaved_TransferAndSaveNewArray) {
  MessageSharedPtr msg = CreateMsgParams();

  auto& keyboard_properties =
      (*msg)[am::strings::msg_params][am::strings::keyboard_properties];
  SmartObject new_list(smart_objects::SmartType_Array);
  new_list[0] = "first_new_value";
  new_list[1] = "second_new_value";
  keyboard_properties[am::hmi_request::auto_complete_list] = new_list;

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  SmartObject old_list(smart_objects::SmartType_Array);
  old_list[0] = "old_value";
  old_list[1] = "another_old_value";
  auto saved_keyboard_props =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);
  (*saved_keyboard_props)[am::hmi_request::auto_complete_list] = old_list;
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillRepeatedly(Return(saved_keyboard_props));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  EXPECT_CALL(*mock_app_, set_keyboard_props(keyboard_properties));

  command->Run();
}

TEST_F(
    SetGlobalPropertiesRequestTest,
    Run_EmptyAutocompleteListInRequestAndAlsoPresentInSaved_TransferButNotSaveEmptyArray) {
  MessageSharedPtr msg = CreateMsgParams();

  auto& keyboard_properties =
      (*msg)[am::strings::msg_params][am::strings::keyboard_properties];
  SmartObject new_list(smart_objects::SmartType_Array);
  keyboard_properties[am::hmi_request::auto_complete_list] = new_list;

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  SmartObject old_list(smart_objects::SmartType_Array);
  old_list[0] = "old_value";
  old_list[1] = "another_old_value";
  auto saved_keyboard_props =
      std::make_shared<SmartObject>(smart_objects::SmartType_Map);
  (*saved_keyboard_props)[am::hmi_request::auto_complete_list] = old_list;
  EXPECT_CALL(*mock_app_, keyboard_props())
      .WillRepeatedly(Return(saved_keyboard_props));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  auto properties_without_empty_list(keyboard_properties);
  properties_without_empty_list.erase(am::hmi_request::auto_complete_list);
  EXPECT_CALL(*mock_app_, set_keyboard_props(properties_without_empty_list));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_NoData_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  ExpectVerifyImageVrHelpUnsuccess();
  EmptyExpectationsSetupHelper();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_InvalidApp_Canceled) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;

  ExpectVerifyImageVrHelpUnsuccess();

  EmptyExpectationsSetupHelper();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_PendingRequest_UNSUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[am::strings::params][am::hmi_response::code] = response_code;

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, true));

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_UIAndSuccessResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[am::strings::params][am::hmi_response::code] = response_code;

  ON_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));
  OnEventUISetupHelper(msg, command);

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, true));

  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, am::commands::Command::SOURCE_SDL))
      .WillOnce(Return(true));

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_UIAndWarningResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::WARNINGS;
  (*msg)[am::strings::params][am::hmi_response::code] = response_code;

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));
  ON_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  OnEventUISetupHelper(msg, command);

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, true));
  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, am::commands::Command::SOURCE_SDL))
      .WillOnce(Return(true));

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_InvalidApp_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::WARNINGS;
  (*msg)[am::strings::params][am::hmi_response::code] = response_code;
  ON_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  OnEventUISetupHelper(msg, command);
  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillRepeatedly(Return(MockAppPtr()));

  EXPECT_CALL(*mock_app_, UpdateHash()).Times(0);

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_InvalidEventID_Canceled) {
  MessageSharedPtr msg = CreateMessage();

  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, UpdateHash()).Times(0);

  Event event(hmi_apis::FunctionID::TTS_Stopped);
  event.set_smart_object(*msg);

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest,
       OnEvent_TTSAndSuccessResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[am::strings::params][am::hmi_response::code] = response_code;
  ON_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  OnEventTTSSetupHelper(msg, command);
  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, am::commands::Command::SOURCE_SDL))
      .WillOnce(Return(true));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, true));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  Event event(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event.set_smart_object(*msg);

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest,
       OnEvent_TTSAndWarningsResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  const hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::WARNINGS;
  (*msg)[am::strings::params][am::hmi_response::code] = response_code;
  ON_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  OnEventTTSSetupHelper(msg, command);

  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_.get()));

  MessageSharedPtr ui_command_result;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnSetGlobalPropertiesReceived(_, true));

  Event event(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event.set_smart_object(*msg);

  command->on_event(event);

  EXPECT_EQ((*ui_command_result)[am::strings::msg_params][am::strings::success]
                .asBool(),
            true);
  EXPECT_EQ(
      (*ui_command_result)[am::strings::msg_params][am::strings::result_code]
          .asInt(),
      static_cast<int32_t>(hmi_apis::Common_Result::WARNINGS));
}

TEST_F(SetGlobalPropertiesRequestTest,
       PrepareResultCodeForResponse_AllResultsAreSUCCESS_TotalResultSUCCESS) {
  using namespace application_manager;
  using namespace hmi_apis;
  const auto expected_total_result = mobile_apis::Result::eType::SUCCESS;

  Common_Result::eType ui_result = Common_Result::SUCCESS;
  Common_Result::eType tts_result = Common_Result::SUCCESS;
  Common_Result::eType rc_result = Common_Result::SUCCESS;

  app_mngr::commands::ResponseInfo ui_properties_info(
      ui_result, HmiInterfaces::HMI_INTERFACE_UI, mock_app_manager_);

  app_mngr::commands::ResponseInfo tts_properties_info(
      tts_result, HmiInterfaces::HMI_INTERFACE_TTS, mock_app_manager_);

  app_mngr::commands::ResponseInfo rc_properties_info(
      rc_result, HmiInterfaces::HMI_INTERFACE_RC, mock_app_manager_);

  MessageSharedPtr msg = CreateMsgParams();
  std::shared_ptr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  auto result = command->PrepareResultCodeForResponse(
      ui_properties_info, tts_properties_info, rc_properties_info);

  EXPECT_EQ(expected_total_result, result);
}

TEST_F(SetGlobalPropertiesRequestTest,
       PrepareResultCodeForResponse_UNSUPPORTED) {
  PrepareResultCodeForResponse_CheckAllResultsForSpecifiedTotalResult(
      mobile_apis::Result::UNSUPPORTED_RESOURCE);
}

TEST_F(SetGlobalPropertiesRequestTest, PrepareResultCodeForResponse_WARNINGS) {
  PrepareResultCodeForResponse_CheckAllResultsForSpecifiedTotalResult(
      mobile_apis::Result::WARNINGS);
}

TEST_F(SetGlobalPropertiesRequestTest,
       PrepareResultForMobileResponse_AllCases) {
  PrepareResultForMobileResponse_CheckResultsForAllCases();
}

}  // namespace set_global_properties_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
