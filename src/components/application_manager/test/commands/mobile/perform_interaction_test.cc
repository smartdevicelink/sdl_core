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
#include "utils/helpers.h"

#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_message_helper.h"

#include "mobile/perform_interaction_request.h"

#include "utils/data_accessor.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace perform_interaction_request {

using namespace application_manager;

using ::testing::Return;
using ::testing::ReturnNull;
using ::testing::ReturnPointee;
using ::testing::Mock;
using ::testing::_;
using am::commands::PerformInteractionRequest;

namespace {
const std::string kNotValidText = "wrong prompt\n";
const std::string kValidText = "correct prompt";
const int32_t kSomeNumber = 1;
const int32_t kSomeAnotherNumber = 123;
const uint32_t kConnectionKey = 2u;
}

class PerformInteractionRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  PerformInteractionRequestTest()
      : message_(utils::MakeShared<SmartObject>(::smart_objects::SmartType_Map))
      , msg_params_((*message_)[strings::msg_params])
      , mock_message_helper_(*MockMessageHelper::message_helper_mock())
      , mock_app_(CreateMockApp()) {}

  ~PerformInteractionRequestTest() {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    Mock::VerifyAndClearExpectations(mock_application_sptr_.get());
  }

  void SetUp() OVERRIDE {
    command_sptr_ =
        CreateCommand<commands::PerformInteractionRequest>(message_);

    mock_application_sptr_ = CreateMockApp();
    ON_CALL(app_mngr_, application(_))
        .WillByDefault(Return(mock_application_sptr_));
    ON_CALL(*mock_application_sptr_, is_perform_interaction_active())
        .WillByDefault(Return(false));
  }

 protected:
  void SetHMIInterfaceState(const am::HmiInterfaces::InterfaceState ui_state,
                            const am::HmiInterfaces::InterfaceState vr_state) {
    ON_CALL(hmi_interfaces_,
            GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
        .WillByDefault(Return(ui_state));
    ON_CALL(hmi_interfaces_,
            GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VR))
        .WillByDefault(Return(vr_state));
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
                         const hmi_apis::Common_Result::eType vr_hmi_response,
                         const char* ui_info,
                         const char* vr_info,
                         const mobile_apis::Result::eType mobile_response,
                         const char* mobile_info,
                         const am::HmiInterfaces::InterfaceState ui_state,
                         const am::HmiInterfaces::InterfaceState vr_state,
                         const bool success) {
    MessageSharedPtr msg_from_mobile =
        CreateMessage(smart_objects::SmartType_Map);
    (*msg_from_mobile)[strings::params][strings::connection_key] =
        kConnectionKey;
    (*msg_from_mobile)[strings::msg_params][strings::interaction_mode] =
        mobile_apis::InteractionMode::VR_ONLY;
    utils::SharedPtr<PerformInteractionRequest> command =
        CreateCommand<PerformInteractionRequest>(msg_from_mobile);
    SetHMIInterfaceState(ui_state, vr_state);
    command->Init();
    MessageSharedPtr response_msg_vr =
        PrepareResponseFromHMI(vr_hmi_response, vr_info);
    am::event_engine::Event event_vr(
        hmi_apis::FunctionID::VR_PerformInteraction);
    event_vr.set_smart_object(*response_msg_vr);

    MessageSharedPtr response_msg_ui =
        PrepareResponseFromHMI(ui_hmi_response, ui_info);
    am::event_engine::Event event_ui(
        hmi_apis::FunctionID::UI_PerformInteraction);
    event_ui.set_smart_object(*response_msg_ui);

    MessageSharedPtr response_to_mobile;
    EXPECT_CALL(app_mngr_,
                ManageMobileCommand(
                    _, am::commands::Command::CommandOrigin::ORIGIN_SDL))
        .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

    command->on_event(event_vr);
    command->on_event(event_ui);

    EXPECT_EQ(
        (*response_to_mobile)[am::strings::msg_params][am::strings::success]
            .asBool(),
        success);
    EXPECT_EQ(
        (*response_to_mobile)[am::strings::msg_params][am::strings::result_code]
            .asInt(),
        static_cast<int32_t>(mobile_response));
    if (mobile_info) {
      EXPECT_EQ(
          (*response_to_mobile)[am::strings::msg_params][am::strings::info]
              .asString(),
          mobile_info);
    }
  }

  void ExpectMobileResult(mobile_api::Result::eType result) {
    CallRun caller(*command_sptr_);
    MessageSharedPtr result_message = CatchMobileCommandResult(caller);

    const mobile_api::Result::eType result_value =
        static_cast<mobile_api::Result::eType>(
            (*result_message)[strings::msg_params][strings::result_code]
                .asInt());
    EXPECT_EQ(result, result_value);
  }

  void TestPromptFieldValidation(const std::string& field) {
    msg_params_[strings::interaction_choice_set_id_list][0] = kSomeNumber;

    ::smart_objects::SmartObject choise_sets;
    choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;
    EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
        .WillOnce(Return(&choise_sets));

    msg_params_[strings::initial_text] = kValidText;
    msg_params_[field][0][strings::text] = kNotValidText;

    ExpectMobileResult(mobile_api::Result::INVALID_DATA);
  }

  sync_primitives::Lock test_lock_;
  MessageSharedPtr message_;
  ::smart_objects::SmartObject& msg_params_;
  utils::SharedPtr<commands::PerformInteractionRequest> command_sptr_;
  MockAppPtr mock_application_sptr_;
  NiceMock<MockHmiInterfaces> hmi_interfaces_;
  MockMessageHelper& mock_message_helper_;
  MockAppPtr mock_app_;
};

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRResponceRPIWrongChoiset_ResponceGenericError) {
  event_engine::Event event(hmi_apis::FunctionID::VR_PerformInteraction);
  CallOnEvent caller(*command_sptr_, event);

  (*message_)[strings::msg_params][strings::choice_id] = kSomeNumber;
  (*message_)[strings::params][hmi_response::code] =
      mobile_apis::Result::SUCCESS;

  event.set_smart_object(*message_);

  PerformChoiceSetMap empty_choice_set_map;
  const DataAccessor<PerformChoiceSetMap> empty_data_accessor(
      empty_choice_set_map, test_lock_);
  EXPECT_CALL(*mock_application_sptr_, performinteraction_choice_set_map())
      .WillOnce(Return(empty_data_accessor));

  MessageSharedPtr hmi_result_msg;
  MessageSharedPtr mobile_result_msg;

  {
    ::testing::InSequence dummy;
    EXPECT_CALL(app_mngr_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&hmi_result_msg), Return(true)));

    EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _))
        .WillOnce(DoAll(SaveArg<0>(&mobile_result_msg), Return(true)));
  }

  caller();

  const hmi_apis::FunctionID::eType hmi_result =
      static_cast<hmi_apis::FunctionID::eType>(
          (*hmi_result_msg)[strings::params][strings::function_id].asInt());
  const mobile_api::Result::eType mobile_result =
      static_cast<mobile_api::Result::eType>(
          (*mobile_result_msg)[strings::msg_params][strings::result_code]
              .asInt());

  EXPECT_EQ(hmi_apis::FunctionID::UI_ClosePopUp, hmi_result);
  EXPECT_EQ(mobile_apis::Result::GENERIC_ERROR, mobile_result);
}

TEST_F(PerformInteractionRequestTest, DISABLED_OnTimeout_VR_GENERIC_ERROR) {
  MessageSharedPtr response_msg_vr =
      CreateMessage(smart_objects::SmartType_Map);
  (*response_msg_vr)[strings::params][hmi_response::code] =
      static_cast<uint64_t>(hmi_apis::Common_Result::SUCCESS);
  (*response_msg_vr)[strings::msg_params][strings::info] = "info";
  MessageSharedPtr request_msg = CreateMessage(smart_objects::SmartType_Map);
  (*request_msg)[strings::msg_params][strings::interaction_mode] =
      mobile_apis::InteractionMode::BOTH;
  utils::SharedPtr<PerformInteractionRequest> command =
      CreateCommand<PerformInteractionRequest>(request_msg);
  MockAppPtr mock_app;

  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));

  Event event(hmi_apis::FunctionID::VR_PerformInteraction);
  event.set_smart_object(*response_msg_vr);

  command->Init();
  command->on_event(event);
  MessageSharedPtr response_to_mobile =
      CreateMessage(smart_objects::SmartType_Map);
  (*response_to_mobile)[strings::msg_params][strings::result_code] =
      static_cast<uint64_t>(am::mobile_api::Result::GENERIC_ERROR);
  EXPECT_CALL(
      mock_message_helper_,
      CreateNegativeResponse(_, _, _, am::mobile_api::Result::GENERIC_ERROR))
      .WillOnce(Return(response_to_mobile));

  MessageSharedPtr vr_command_result;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&vr_command_result), Return(true)));

  command->onTimeOut();

  EXPECT_EQ(
      (*vr_command_result)[strings::msg_params][strings::success].asBool(),
      false);
  EXPECT_EQ(
      (*vr_command_result)[strings::msg_params][strings::result_code].asInt(),
      static_cast<int32_t>(am::mobile_api::Result::GENERIC_ERROR));
  EXPECT_FALSE((*vr_command_result)[strings::msg_params][strings::info]
                   .asString()
                   .empty());
}

TEST_F(
    PerformInteractionRequestTest,
    OnEvent_VRUnsupportedResource_UISuccess_MobileResultUNSUPPORTED_RESOURCE) {
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::SUCCESS;
  const hmi_apis::Common_Result::eType vr_hmi_response =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  const char* ui_info = NULL;
  const char* vr_info = "VR is not supported by system";
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::UNSUPPORTED_RESOURCE;
  const char* mobile_info = "VR is not supported by system";
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const am::HmiInterfaces::InterfaceState vr_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const bool success = true;
  CheckExpectations(ui_hmi_response,
                    vr_hmi_response,
                    ui_info,
                    vr_info,
                    mobile_response,
                    mobile_info,
                    ui_state,
                    vr_state,
                    success);
}

TEST_F(
    PerformInteractionRequestTest,
    OnEvent_UIUnsupportedResource_VRSuccess_MobileResultUNSUPPORTED_RESOURCE) {
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  const hmi_apis::Common_Result::eType vr_hmi_response =
      hmi_apis::Common_Result::SUCCESS;
  const char* ui_info = "UI is not supported by system";
  const char* vr_info = NULL;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::UNSUPPORTED_RESOURCE;
  const char* mobile_info = "UI is not supported by system";
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const am::HmiInterfaces::InterfaceState vr_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const bool success = true;
  CheckExpectations(ui_hmi_response,
                    vr_hmi_response,
                    ui_info,
                    vr_info,
                    mobile_response,
                    mobile_info,
                    ui_state,
                    vr_state,
                    success);
}

TEST_F(
    PerformInteractionRequestTest,
    OnEvent_BothInterfaceIsAvailable_VRResultSUCCESS_UIResultWARNINGS_MobileResultWarning) {
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::WARNINGS;
  const hmi_apis::Common_Result::eType vr_hmi_response =
      hmi_apis::Common_Result::SUCCESS;
  const char* ui_info = NULL;
  const char* vr_info = NULL;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::WARNINGS;
  const char* mobile_info = NULL;
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const am::HmiInterfaces::InterfaceState vr_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const bool success = true;
  CheckExpectations(ui_hmi_response,
                    vr_hmi_response,
                    ui_info,
                    vr_info,
                    mobile_response,
                    mobile_info,
                    ui_state,
                    vr_state,
                    success);
}

TEST_F(
    PerformInteractionRequestTest,
    OnEvent_VRInterfaceNotRespond_VRResultUnsupported_UIResultWARNINGS_MobileResultUnsupported) {
  const hmi_apis::Common_Result::eType ui_hmi_response =
      hmi_apis::Common_Result::WARNINGS;
  const hmi_apis::Common_Result::eType vr_hmi_response =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  const char* ui_info = NULL;
  const char* vr_info = NULL;
  const mobile_apis::Result::eType mobile_response =
      mobile_apis::Result::UNSUPPORTED_RESOURCE;
  const char* mobile_info = NULL;
  const am::HmiInterfaces::InterfaceState ui_state =
      am::HmiInterfaces::STATE_AVAILABLE;
  const am::HmiInterfaces::InterfaceState vr_state =
      am::HmiInterfaces::STATE_NOT_RESPONSE;
  const bool success = true;
  CheckExpectations(ui_hmi_response,
                    vr_hmi_response,
                    ui_info,
                    vr_info,
                    mobile_response,
                    mobile_info,
                    ui_state,
                    vr_state,
                    success);
}

TEST_F(PerformInteractionRequestTest, Init_CorrectTimeout) {
  const uint32_t new_timeout = 1000u;
  msg_params_[strings::timeout] = new_timeout;
  msg_params_[strings::interaction_mode] =
      mobile_api::InteractionMode::MANUAL_ONLY;

  EXPECT_EQ(kDefaultTimeout_, command_sptr_->default_timeout());

  command_sptr_->Init();
  EXPECT_EQ(new_timeout * 2, command_sptr_->default_timeout());
}

TEST_F(PerformInteractionRequestTest,
       Run_InvalidApp_ApplicationNotRegisteredResponce) {
  utils::SharedPtr<Application> null_application_sptr;
  EXPECT_CALL(app_mngr_, application(_))
      .WillOnce(Return(null_application_sptr));

  ExpectMobileResult(mobile_api::Result::APPLICATION_NOT_REGISTERED);
}

TEST_F(PerformInteractionRequestTest, Run_VROnlyAndKeyboardLayout_InvalidData) {
  msg_params_[hmi_request::interaction_layout] =
      mobile_api::LayoutMode::KEYBOARD;
  msg_params_[strings::interaction_mode] = mobile_api::InteractionMode::VR_ONLY;
  command_sptr_->Init();

  ExpectMobileResult(mobile_api::Result::INVALID_DATA);
}

TEST_F(PerformInteractionRequestTest, Run_BothAndKeyboardLayout_InvalidData) {
  msg_params_[hmi_request::interaction_layout] =
      mobile_api::LayoutMode::KEYBOARD;
  msg_params_[strings::interaction_mode] = mobile_api::InteractionMode::BOTH;
  command_sptr_->Init();

  ExpectMobileResult(mobile_api::Result::INVALID_DATA);
}

TEST_F(PerformInteractionRequestTest,
       Run_SetIdsEmptyAndLayoutNotKeyboard_InvalidData) {
  msg_params_[hmi_request::interaction_layout] =
      mobile_api::LayoutMode::ICON_ONLY;

  ExpectMobileResult(mobile_api::Result::INVALID_DATA);
}

TEST_F(PerformInteractionRequestTest, Run_FoundInvalidChouiseSet_InvalidId) {
  msg_params_[strings::interaction_choice_set_id_list][0] = kSomeNumber;
  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_)).WillOnce(ReturnNull());

  ExpectMobileResult(mobile_api::Result::INVALID_ID);
}

TEST_F(PerformInteractionRequestTest, Run_TwoSameChoisesetIds_InvalidId) {
  msg_params_[strings::interaction_choice_set_id_list][0] = kSomeNumber;

  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;
  choise_sets[strings::choice_set][1][strings::choice_id] = kSomeNumber;

  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillOnce(Return(&choise_sets));

  ExpectMobileResult(mobile_api::Result::INVALID_ID);
}

TEST_F(PerformInteractionRequestTest, Run_InvalidImageVrHelpItems_InvalidData) {
  msg_params_[strings::interaction_choice_set_id_list][0] = kSomeNumber;

  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;
  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillOnce(Return(&choise_sets));

  msg_params_[strings::vr_help] = "vr_help";
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              VerifyImageVrHelpItems(_, _, _))
      .WillOnce(Return(mobile_api::Result::INVALID_ENUM));

  ExpectMobileResult(mobile_api::Result::INVALID_DATA);
}

TEST_F(PerformInteractionRequestTest,
       Run_WhiteSpaceExistsInInitialText_InvalidData) {
  msg_params_[strings::interaction_choice_set_id_list][0] = kSomeNumber;

  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;
  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillOnce(Return(&choise_sets));

  msg_params_[strings::initial_text] = kNotValidText;

  ExpectMobileResult(mobile_api::Result::INVALID_DATA);
}

TEST_F(PerformInteractionRequestTest,
       Run_WhiteSpaceExistsInInitialPrompt_InvalidData) {
  TestPromptFieldValidation(strings::initial_prompt);
}

TEST_F(PerformInteractionRequestTest,
       Run_WhiteSpaceExistsInHelpPrompt_InvalidData) {
  TestPromptFieldValidation(strings::help_prompt);
}

TEST_F(PerformInteractionRequestTest,
       Run_WhiteSpaceExistsInTimeoutPrompt_InvalidData) {
  TestPromptFieldValidation(strings::timeout_prompt);
}

TEST_F(PerformInteractionRequestTest,
       Run_WhiteSpaceExistsInVrHelpText_InvalidData) {
  msg_params_[strings::interaction_choice_set_id_list][0] = kSomeNumber;

  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;
  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillOnce(Return(&choise_sets));

  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              VerifyImageVrHelpItems(_, _, _))
      .WillOnce(Return(mobile_api::Result::SUCCESS));

  msg_params_[strings::initial_text] = kValidText;
  msg_params_[strings::vr_help][0][strings::text] = kNotValidText;

  ExpectMobileResult(mobile_api::Result::INVALID_DATA);
}

TEST_F(PerformInteractionRequestTest,
       Run_WhiteSpaceExistsInVrHelpImageValue_InvalidData) {
  msg_params_[strings::interaction_choice_set_id_list][0] = kSomeNumber;

  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;
  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillOnce(Return(&choise_sets));

  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              VerifyImageVrHelpItems(_, _, _))
      .WillOnce(Return(mobile_api::Result::SUCCESS));

  msg_params_[strings::initial_text] = kValidText;
  msg_params_[strings::vr_help][0][strings::text] = kValidText;
  msg_params_[strings::vr_help][0][strings::image][strings::value] =
      kNotValidText;

  ExpectMobileResult(mobile_api::Result::INVALID_DATA);
}

TEST_F(PerformInteractionRequestTest,
       ChecVrSynonyms_InvalidChoiseSet_InvalidId) {
  msg_params_[strings::interaction_choice_set_id_list][0] = 1u;
  msg_params_[strings::interaction_choice_set_id_list][1] = 1u;
  msg_params_[strings::interaction_choice_set_id_list][2] = 1u;

  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillRepeatedly(ReturnNull());
  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::INVALID_ID), _));

  command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::CHECK_VR_SYNONYMS);
}

TEST_F(PerformInteractionRequestTest,
       ChecVrSynonyms_TwoSameVrCommands_DuplicateName) {
  msg_params_[strings::interaction_choice_set_id_list][0] = kSomeNumber;
  msg_params_[strings::interaction_choice_set_id_list][1] = kSomeNumber;
  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::grammar_id] = kSomeNumber;
  choise_sets[strings::choice_set][0][strings::vr_commands][0] = kSomeNumber;
  choise_sets[strings::choice_set][0][strings::vr_commands][1] = kSomeNumber;

  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillRepeatedly(Return(&choise_sets));

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::DUPLICATE_NAME), _));

  const bool result = command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::CHECK_VR_SYNONYMS);

  EXPECT_FALSE(result);
}

TEST_F(PerformInteractionRequestTest,
       CheckMenuNames_InvalidListOfNames_InvalidId) {
  msg_params_[strings::interaction_choice_set_id_list][0] = kSomeNumber;
  msg_params_[strings::interaction_choice_set_id_list][1] = kSomeNumber;

  EXPECT_CALL(
      *mock_application_sptr_,
      FindChoiceSet(
          msg_params_[strings::interaction_choice_set_id_list][0].asInt()))
      .Times(3)
      .WillRepeatedly(ReturnNull());
  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::INVALID_ID), _));

  const bool result = command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::CHECK_MENU_NAMES);

  EXPECT_FALSE(result);
}

TEST_F(PerformInteractionRequestTest,
       CheckMenuNames_TwoSameNames_DuplicateName) {
  msg_params_[strings::interaction_choice_set_id_list][0] = kSomeNumber;
  msg_params_[strings::interaction_choice_set_id_list][1] = kSomeNumber;

  ::smart_objects::SmartObject choise_set_1;
  ::smart_objects::SmartObject choise_set_2;
  choise_set_1[strings::choice_set][0][strings::menu_name] = "MenuName";
  choise_set_2[strings::choice_set][0][strings::menu_name] = "MenuName";

  EXPECT_CALL(
      *mock_application_sptr_,
      FindChoiceSet(
          msg_params_[strings::interaction_choice_set_id_list][0].asInt()))
      .Times(3)
      .WillOnce(Return(&choise_set_1))
      // Second call will be skipped in method cycles
      .WillOnce(ReturnNull())
      // Third call value will be compared to value from first call
      .WillOnce(Return(&choise_set_2));

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::DUPLICATE_NAME), _));

  const bool result = command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::CHECK_MENU_NAMES);

  EXPECT_FALSE(result);
}

TEST_F(PerformInteractionRequestTest, CheckMenuNames_AllRight_Success) {
  msg_params_[strings::interaction_choice_set_id_list][0] = kSomeNumber;
  msg_params_[strings::interaction_choice_set_id_list][1] = kSomeNumber;

  ::smart_objects::SmartObject choise_set_1;
  ::smart_objects::SmartObject choise_set_2;
  choise_set_1[strings::choice_set][0][strings::menu_name] = "MenuName";
  choise_set_2[strings::choice_set][0][strings::menu_name] = "AnotherMenuName";

  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .Times(6)
      .WillOnce(Return(&choise_set_1))
      // Second call will be skipped in method cycles
      .WillOnce(ReturnNull())
      // Third call value will be compared to value from first call
      .WillOnce(Return(&choise_set_2))
      // First cycle second iteration
      .WillOnce(Return(&choise_set_2))
      // Second cycle first iteration
      .WillOnce(Return(&choise_set_1))
      // Second cycle second iteration will be skipped
      .WillOnce(ReturnNull());

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  const bool result = command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::CHECK_MENU_NAMES);

  EXPECT_TRUE(result);
}

TEST_F(PerformInteractionRequestTest, CheckVrHelpItem_VrHelpNotExists_Success) {
  const bool result = command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::CHECK_VR_HELP_ITEM);

  EXPECT_TRUE(result);
}

TEST_F(PerformInteractionRequestTest,
       CheckVrHelpItem_IncorrectPosition_Rejected) {
  msg_params_[strings::vr_help][0][strings::position] = kSomeNumber;
  msg_params_[strings::vr_help][1][strings::position] = kSomeAnotherNumber;

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::REJECTED), _));

  const bool result = command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::CHECK_VR_HELP_ITEM);

  EXPECT_FALSE(result);
}

TEST_F(PerformInteractionRequestTest,
       CheckVrHelpItem_CorrectPositions_Success) {
  msg_params_[strings::vr_help][0][strings::position] = kSomeNumber;
  msg_params_[strings::vr_help][1][strings::position] = kSomeNumber + 1;

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  const bool result = command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::CHECK_VR_HELP_ITEM);

  EXPECT_TRUE(result);
}

TEST_F(PerformInteractionRequestTest,
       Run_InvalidInteractionMode_StopProcessing) {
  msg_params_[strings::interaction_choice_set_id_list][0] = kSomeNumber;
  msg_params_[strings::interaction_mode] =
      mobile_api::InteractionMode::INVALID_ENUM;
  msg_params_[strings::initial_text] = kValidText;

  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;

  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillOnce(Return(&choise_sets));

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);

  CallRun caller(*command_sptr_);
  caller();
}

TEST_F(PerformInteractionRequestTest,
       Run_AllRight_SendVRAndUIRequests_Success) {
  msg_params_[strings::interaction_choice_set_id_list][0] = 1u;
  msg_params_[strings::interaction_mode] = mobile_api::InteractionMode::VR_ONLY;
  msg_params_[strings::initial_text] = kValidText;
  msg_params_[strings::help_prompt][0][strings::text] = "Prompt";

  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::grammar_id] = kSomeNumber;
  choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;
  choise_sets[strings::choice_set][0][strings::vr_commands][0] = kSomeNumber;

  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillRepeatedly(Return(&choise_sets));

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  command_sptr_->Init();
  CallRun caller(*command_sptr_);

  MessageSharedPtr first_msg;
  MessageSharedPtr second_msg;
  {
    ::testing::InSequence dummy;
    EXPECT_CALL(this->app_mngr_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&first_msg), Return(true)));
    EXPECT_CALL(this->app_mngr_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&second_msg), Return(true)));
  }
  caller();

  const hmi_apis::FunctionID::eType first_result =
      static_cast<hmi_apis::FunctionID::eType>(
          (*first_msg)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(hmi_apis::FunctionID::VR_PerformInteraction, first_result);

  const hmi_apis::FunctionID::eType second_result =
      static_cast<hmi_apis::FunctionID::eType>(
          (*second_msg)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(hmi_apis::FunctionID::UI_PerformInteraction, second_result);
}

TEST_F(PerformInteractionRequestTest, OnEvent_OnResetTimeout_UpdateTimeout) {
  event_engine::Event event(hmi_apis::FunctionID::UI_OnResetTimeout);
  CallOnEvent caller(*command_sptr_, event);

  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _));

  caller();
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_PerformInteractionResponce_Unsuccess) {
  event_engine::Event event(hmi_apis::FunctionID::UI_PerformInteraction);
  CallOnEvent caller(*command_sptr_, event);

  ApplicationSharedPtr null_application;
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(null_application));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  caller();
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRResponceIncorrectApp_NoResponces) {
  event_engine::Event event(hmi_apis::FunctionID::VR_PerformInteraction);
  CallOnEvent caller(*command_sptr_, event);

  ApplicationSharedPtr null_application;
  EXPECT_CALL(app_mngr_, application(_))
      .WillRepeatedly(Return(null_application));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);

  caller();
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRResponceRPIcodeTimeOut_ResetTimeout) {
  event_engine::Event event(hmi_apis::FunctionID::VR_PerformInteraction);
  CallOnEvent caller(*command_sptr_, event);

  (*message_)[strings::params][hmi_response::code] =
      mobile_apis::Result::TIMED_OUT;

  event.set_smart_object(*message_);

  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  caller();
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRResponceRPIcodeSuccessManualOnly_Return) {
  event_engine::Event event(hmi_apis::FunctionID::VR_PerformInteraction);
  CallOnEvent caller(*command_sptr_, event);

  (*message_)[strings::params][hmi_response::code] =
      mobile_apis::Result::SUCCESS;

  event.set_smart_object(*message_);

  (*message_)[strings::msg_params][strings::interaction_mode] =
      mobile_api::InteractionMode::MANUAL_ONLY;
  command_sptr_->Init();

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  caller();
}
}  // namespace perform_interaction_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
