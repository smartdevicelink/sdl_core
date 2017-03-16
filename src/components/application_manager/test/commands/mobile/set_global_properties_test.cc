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

#include "application_manager/commands/mobile/set_global_properties_request.h"

#include "gtest/gtest.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/commands/commands_test.h"
#include "utils/custom_string.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace set_global_properties_request {

namespace am = application_manager;
namespace strings = am::strings;
namespace hmi_request = am::hmi_request;
namespace hmi_response = am::hmi_response;
using am::commands::SetGlobalPropertiesRequest;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using am::MockMessageHelper;
using am::MockHmiInterfaces;
using ::utils::SharedPtr;
using ::utils::custom_string::CustomString;
using ::testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {
const uint32_t kAppID = 2u;
const uint32_t kConnectionKey = 2u;
const uint32_t kVrPosition = 1u;
const std::string kVrTitle = "vr_help_title";
const std::string kVrText = "vr_help";
const std::string kHelpPrompt = "help_prompt";
const std::string kValidString = "correct_sting";
const std::string kNotValidString = "wrong\t\nstring\\n";
}  // namespace

class SetGlobalPropertiesRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SetGlobalPropertiesRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock())
      , mock_app_(CreateMockApp()) {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(hmi_interfaces_));
  }

  ~SetGlobalPropertiesRequestTest() {
    // Required for proper DataAccessor release
    Mock::VerifyAndClearExpectations(&app_mngr_);
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  MessageSharedPtr CreateMsgWithCommonParams() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;

    smart_objects::SmartObject keyboard_properties =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    keyboard_properties[strings::auto_complete_text] = kValidString;
    keyboard_properties[hmi_request::auto_complete_list][0] = kValidString;

    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params[strings::vr_help_title] = kVrTitle;
    msg_params[strings::vr_help][0][strings::text] = kVrText;
    msg_params[strings::vr_help][0][strings::position] = kVrPosition;
    msg_params[strings::help_prompt][0][strings::text] = kHelpPrompt;
    msg_params[hmi_request::keyboard_properties] = keyboard_properties;

    (*msg)[strings::msg_params] = msg_params;

    return msg;
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
    (*msg)[strings::params][hmi_response::code] = result_code;
    (*msg)[strings::msg_params] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    if (info) {
      (*msg)[strings::msg_params][strings::info] = info;
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
    MessageSharedPtr set_global_pr_msg = CreateMsgWithCommonParams();

    utils::SharedPtr<SetGlobalPropertiesRequest> command =
        CreateCommand<SetGlobalPropertiesRequest>(set_global_pr_msg);

    EXPECT_CALL(app_mngr_,
                RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
    const smart_objects::SmartObject& set_global_pr_msg_params =
        (*set_global_pr_msg)[strings::msg_params];
    const smart_objects::SmartObject* vr_help_title =
        &(set_global_pr_msg_params[strings::vr_help_title]);
    const smart_objects::SmartObject* vr_help =
        &(set_global_pr_msg_params[strings::vr_help]);
    const smart_objects::SmartObject* vr_help_prompt =
        &(set_global_pr_msg_params[strings::help_prompt]);
    ON_CALL(*mock_app_, vr_help_title()).WillByDefault(Return(vr_help_title));
    ON_CALL(*mock_app_, vr_help()).WillByDefault(Return(vr_help));
    ON_CALL(*mock_app_, help_prompt()).WillByDefault(Return(vr_help_prompt));
    ON_CALL(
        mock_message_helper_,
        VerifyImageVrHelpItems(
            (*set_global_pr_msg)[strings::msg_params][strings::vr_help], _, _))
        .WillByDefault(Return(mobile_apis::Result::SUCCESS));

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

    MessageSharedPtr result_to_mobile;
    EXPECT_CALL(app_mngr_,
                ManageMobileCommand(
                    _, am::commands::Command::CommandOrigin::ORIGIN_SDL))
        .WillOnce(DoAll(SaveArg<0>(&result_to_mobile), Return(true)));

    command->on_event(event_ui);

    EXPECT_EQ(
        success,
        (*result_to_mobile)[strings::msg_params][strings::success].asBool());
    EXPECT_EQ(
        static_cast<int32_t>(mobile_response),
        (*result_to_mobile)[strings::msg_params][strings::result_code].asInt());
    if (mobile_info) {
      EXPECT_EQ(
          mobile_info,
          (*result_to_mobile)[strings::msg_params][strings::info].asString());
    }
  }

  void TestOfNotValidParameters(const SmartObject& msg_params) {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;
    (*msg)[strings::msg_params] = msg_params;

    SharedPtr<SetGlobalPropertiesRequest> command(
        CreateCommand<SetGlobalPropertiesRequest>(msg));

    if ((*msg)[strings::msg_params].keyExists(strings::vr_help)) {
      EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _))
          .WillOnce(Return(mobile_apis::Result::SUCCESS));
    }
    if ((*msg)[strings::msg_params].keyExists(strings::menu_icon)) {
      EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _))
          .WillOnce(Return(mobile_apis::Result::SUCCESS));
    }

    EXPECT_CALL(app_mngr_,
                ManageMobileCommand(
                    MobileResultCodeIs(mobile_apis::Result::INVALID_DATA), _));

    command->Run();
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
  (*response)[strings::params][strings::connection_key] = kConnectionKey;
  (*response)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*response)[strings::msg_params][strings::info] = "test";

  am::event_engine::Event event(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event.set_smart_object(*response);

  utils::SharedPtr<SetGlobalPropertiesRequest> command =
      CreateCommand<SetGlobalPropertiesRequest>(response);

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .Times(0);
  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest,
       OnEvent_UNSUPPORTED_RESOURCE_Expect_false) {
  MessageSharedPtr response = CreateMessage(smart_objects::SmartType_Map);
  (*response)[strings::params][strings::connection_key] = kConnectionKey;
  (*response)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*response)[strings::msg_params][strings::info] = "qwe";

  am::event_engine::Event event_tts(
      hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event_tts.set_smart_object(*response);
  am::event_engine::Event event_ui(
      hmi_apis::FunctionID::UI_SetGlobalProperties);
  event_tts.set_smart_object(*response);
  utils::SharedPtr<SetGlobalPropertiesRequest> command =
      CreateCommand<SetGlobalPropertiesRequest>(response);

  ON_CALL(mock_message_helper_, HMIToMobileResult(_))
      .WillByDefault(Return(mobile_apis::Result::UNSUPPORTED_RESOURCE));

  EXPECT_CALL(hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  MessageSharedPtr response_to_mobile;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command->Run();
  command->on_event(event_ui);
  command->on_event(event_tts);

  EXPECT_FALSE(
      (*response_to_mobile)[strings::msg_params][strings::success].asBool());
  EXPECT_EQ(
      static_cast<int32_t>(mobile_apis::Result::INVALID_DATA),
      (*response_to_mobile)[strings::msg_params][strings::result_code].asInt());
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_MsgParametersContainNotValidData_INVALID_DATA) {
  using namespace strings;
  SmartObject test_so;
  test_so[0][msg_params][help_prompt][0][text] = kNotValidString;

  test_so[1][msg_params][timeout_prompt][0][text] = kNotValidString;

  test_so[2][msg_params][vr_help][0][text] = kNotValidString;

  test_so[3][msg_params][vr_help][0][text] = kValidString;
  test_so[3][msg_params][vr_help][0][image][value] = kNotValidString;

  test_so[4][msg_params][menu_icon][value] = kNotValidString;

  test_so[5][msg_params][vr_help_title] = kNotValidString;

  test_so[6][msg_params][menu_title] = kNotValidString;

  test_so[7][msg_params][keyboard_properties][limited_character_list][0] =
      kNotValidString;

  test_so[8][msg_params][keyboard_properties][auto_complete_text] =
      kNotValidString;

  test_so[9][msg_params][keyboard_properties][hmi_request::auto_complete_list]
         [0] = kNotValidString;

  for (size_t i = 0; i < test_so.length(); ++i) {
    TestOfNotValidParameters(test_so[i][msg_params]);
  }
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_NoVRHelpDataDefaultCreatedFromAppName_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::connection_key] = kConnectionKey;
  (*msg)[strings::msg_params][strings::app_id] = kAppID;
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[strings::msg_params][hmi_request::keyboard_properties] =
      keyboard_properties;

  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppID));
  const CustomString app_name("test_app");
  ON_CALL(*mock_app_, name()).WillByDefault(ReturnRef(app_name));
  am::CommandsMap commands_map;
  ON_CALL(*mock_app_, commands_map())
      .WillByDefault(Return(
          DataAccessor<application_manager::CommandsMap>(commands_map, lock_)));
  SmartObject vr_synonyms(smart_objects::SmartType_Null);
  ON_CALL(*mock_app_, vr_synonyms()).WillByDefault(Return(&vr_synonyms));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);

  SmartObject vr_help_title(smart_objects::SmartType_Null);
  SmartObject default_vr_help_title(app_name.AsMBString());
  EXPECT_CALL(*mock_app_, vr_help_title())
      .WillOnce(Return(&vr_help_title))
      .WillRepeatedly(Return(&default_vr_help_title));

  SmartObject default_vr_help(smart_objects::SmartType_Array);
  SmartObject vr_help_item(smart_objects::SmartType_Map);
  vr_help_item[strings::text] = app_name.AsMBString();
  vr_help_item[strings::position] = 1;
  default_vr_help[0] = vr_help_item;
  EXPECT_CALL(*mock_app_, vr_help()).WillRepeatedly(Return(&default_vr_help));

  EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_vr_help(_));
  EXPECT_CALL(*mock_app_, set_vr_help_title(_));

  EXPECT_CALL(hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillOnce(Return(am::HmiInterfaces::STATE_AVAILABLE));
  EXPECT_CALL(hmi_interfaces_, GetInterfaceFromFunction(_))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  MessageSharedPtr request_to_hmi;
  EXPECT_CALL(app_mngr_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&request_to_hmi), Return(true)));

  command->Run();

  EXPECT_EQ(app_name.AsMBString(),
            (*request_to_hmi)[strings::msg_params][strings::vr_help_title]
                .asString());
  EXPECT_EQ(1u,
            (*request_to_hmi)[strings::msg_params][strings::vr_help].length());
  EXPECT_EQ(
      app_name.AsMBString(),
      (*request_to_hmi)[strings::msg_params][strings::vr_help][0][strings::text]
          .asString());
  EXPECT_EQ(1,
            (*request_to_hmi)[strings::msg_params][strings::vr_help][0]
                             [strings::position].asInt());
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_NoVRHelpDataDefaultCreatedFromSynonyms_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::connection_key] = kConnectionKey;
  (*msg)[strings::msg_params][strings::app_id] = kAppID;
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[strings::msg_params][hmi_request::keyboard_properties] =
      keyboard_properties;

  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppID));
  const CustomString app_name("test_app");
  ON_CALL(*mock_app_, name()).WillByDefault(ReturnRef(app_name));
  am::CommandsMap commands_map;
  ON_CALL(*mock_app_, commands_map())
      .WillByDefault(Return(
          DataAccessor<application_manager::CommandsMap>(commands_map, lock_)));
  SmartObject vr_synonyms(smart_objects::SmartType_Array);
  vr_synonyms[0] = kHelpPrompt;
  ON_CALL(*mock_app_, vr_synonyms()).WillByDefault(Return(&vr_synonyms));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);

  SmartObject vr_help_title(smart_objects::SmartType_Null);
  SmartObject default_vr_help_title(app_name.AsMBString());
  EXPECT_CALL(*mock_app_, vr_help_title())
      .WillOnce(Return(&vr_help_title))
      .WillRepeatedly(Return(&default_vr_help_title));

  SmartObject default_vr_help(smart_objects::SmartType_Array);
  SmartObject vr_help_item(smart_objects::SmartType_Map);
  vr_help_item[strings::text] = kHelpPrompt;
  vr_help_item[strings::position] = 1;
  default_vr_help[0] = vr_help_item;
  EXPECT_CALL(*mock_app_, vr_help()).WillRepeatedly(Return(&default_vr_help));

  EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_vr_help(_));
  EXPECT_CALL(*mock_app_, set_vr_help_title(_));

  EXPECT_CALL(hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillOnce(Return(am::HmiInterfaces::STATE_AVAILABLE));
  EXPECT_CALL(hmi_interfaces_, GetInterfaceFromFunction(_))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  MessageSharedPtr request_to_hmi;
  EXPECT_CALL(app_mngr_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&request_to_hmi), Return(true)));

  command->Run();

  EXPECT_EQ(app_name.AsMBString(),
            (*request_to_hmi)[strings::msg_params][strings::vr_help_title]
                .asString());
  EXPECT_EQ(1u,
            (*request_to_hmi)[strings::msg_params][strings::vr_help].length());
  EXPECT_EQ(
      kHelpPrompt,
      (*request_to_hmi)[strings::msg_params][strings::vr_help][0][strings::text]
          .asString());
  EXPECT_EQ(1,
            (*request_to_hmi)[strings::msg_params][strings::vr_help][0]
                             [strings::position].asInt());
}

}  // namespace set_global_properties_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
