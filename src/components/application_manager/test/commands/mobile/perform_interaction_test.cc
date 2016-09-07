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

using namespace application_manager;

using ::testing::Return;
using ::testing::ReturnNull;
using ::testing::ReturnPointee;
using ::testing::_;

namespace {
const std::string kNotValidText = "wrong prompt\n";
const std::string kValidText = "correct prompt";
const uint32_t kCorrelationId = 666u;
const int32_t kSomeNumber = 1;
const int32_t kSomeAnotherNumber = 666;
}

class PerformInteractionRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  PerformInteractionRequestTest()
      : message_(utils::MakeShared<SmartObject>(::smart_objects::SmartType_Map))
      , kMsgParams_((*message_)[strings::msg_params]) {}
  void SetUp() OVERRIDE {
    command_sptr_ = CreateCommand<commands::PerformInteractionRequest>(
        CommandsTest::kDefaultTimeout_, message_);

    mock_application_sptr_ = CreateMockApp();
    ON_CALL(mock_app_manager_, application(_))
        .WillByDefault(Return(mock_application_sptr_));
    ON_CALL(*mock_application_sptr_, is_perform_interaction_active())
        .WillByDefault(Return(false));
  }

 protected:
  void ExpectMobileResult(mobile_api::Result::eType result) {
    CallRun caller(*command_sptr_);
    MessageSharedPtr result_message = CatchMobileCommandResult(caller);

    const mobile_api::Result::eType kResult =
        static_cast<mobile_api::Result::eType>(
            (*result_message)[strings::msg_params][strings::result_code]
                .asInt());
    EXPECT_EQ(result, kResult);
  }

  void TestPromptFieldValidation(const std::string& field) {
    kMsgParams_[strings::interaction_choice_set_id_list][0] = kSomeNumber;

    ::smart_objects::SmartObject choise_sets;
    choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;
    EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
        .WillOnce(Return(&choise_sets));

    kMsgParams_[strings::initial_text] = kValidText;
    kMsgParams_[field][0][strings::text] = kNotValidText;

    ExpectMobileResult(mobile_api::Result::INVALID_DATA);
  }

  void TestVRResponceResultCode(mobile_apis::Result::eType code) {
    using namespace helpers;
    event_engine::Event event(hmi_apis::FunctionID::VR_PerformInteraction);
    CallOnEvent caller(*command_sptr_, event);

    (*message_)[strings::params][hmi_response::code] = code;

    const bool is_vr_aborted_timeout =
        Compare<mobile_apis::Result::eType, EQ, ONE>(
            code, mobile_apis::Result::ABORTED, mobile_apis::Result::TIMED_OUT);

    if (is_vr_aborted_timeout) {
      (*message_)[strings::msg_params][strings::interaction_mode] =
          mobile_api::InteractionMode::VR_ONLY;
      command_sptr_->Init();
    }

    if (mobile_api::Result::UNSUPPORTED_REQUEST == code) {
      // Because in case UNSUPPORTED_REQUEST we will send WARNINGS
      code = mobile_api::Result::WARNINGS;
    }

    event.set_smart_object(*message_);

    MessageSharedPtr hmi_result_msg;
    MessageSharedPtr mobile_result_msg;

    {
      ::testing::InSequence dummy;
      EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
          .WillOnce(DoAll(SaveArg<0>(&hmi_result_msg), Return(true)));

      EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _))
          .WillOnce(DoAll(SaveArg<0>(&mobile_result_msg), Return(true)));

      if (!is_vr_aborted_timeout) {
        EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
            .WillOnce(Return(true));

        EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _))
            .WillOnce(Return(true));
      }
    }

    caller();

    const hmi_apis::FunctionID::eType kHmiResult =
        static_cast<hmi_apis::FunctionID::eType>(
            (*hmi_result_msg)[strings::params][strings::function_id].asInt());
    const mobile_api::Result::eType kMobileResult =
        static_cast<mobile_api::Result::eType>(
            (*mobile_result_msg)[strings::msg_params][strings::result_code]
                .asInt());

    EXPECT_EQ(hmi_apis::FunctionID::UI_ClosePopUp, kHmiResult);
    EXPECT_EQ(code, kMobileResult);
  }

  sync_primitives::Lock test_lock_;
  MessageSharedPtr message_;
  ::smart_objects::SmartObject& kMsgParams_;
  utils::SharedPtr<commands::PerformInteractionRequest> command_sptr_;
  MockAppPtr mock_application_sptr_;
};

TEST_F(PerformInteractionRequestTest, Init_CorrectTimeout) {
  const uint32_t kNewTimeOut = 1000u;
  kMsgParams_[strings::timeout] = kNewTimeOut;
  kMsgParams_[strings::interaction_mode] =
      mobile_api::InteractionMode::MANUAL_ONLY;

  EXPECT_EQ(kDefaultTimeout_, command_sptr_->default_timeout());

  command_sptr_->Init();
  EXPECT_EQ(kNewTimeOut * 2, command_sptr_->default_timeout());
}

TEST_F(PerformInteractionRequestTest,
       Run_InvalidApp_ApplicationNotRegisteredResponce) {
  utils::SharedPtr<Application> null_application_sptr;
  EXPECT_CALL(mock_app_manager_, application(_))
      .WillOnce(Return(null_application_sptr));

  ExpectMobileResult(mobile_api::Result::APPLICATION_NOT_REGISTERED);
}

TEST_F(PerformInteractionRequestTest, Run_VROnlyAndKeyboardLayout_InvalidData) {
  kMsgParams_[hmi_request::interaction_layout] =
      mobile_api::LayoutMode::KEYBOARD;
  kMsgParams_[strings::interaction_mode] = mobile_api::InteractionMode::VR_ONLY;
  command_sptr_->Init();

  ExpectMobileResult(mobile_api::Result::INVALID_DATA);
}

TEST_F(PerformInteractionRequestTest, Run_BothAndKeyboardLayout_InvalidData) {
  kMsgParams_[hmi_request::interaction_layout] =
      mobile_api::LayoutMode::KEYBOARD;
  kMsgParams_[strings::interaction_mode] = mobile_api::InteractionMode::BOTH;
  command_sptr_->Init();

  ExpectMobileResult(mobile_api::Result::INVALID_DATA);
}

TEST_F(PerformInteractionRequestTest,
       Run_SetIdsEmptyAndLayoutNotKeyboard_InvalidData) {
  kMsgParams_[hmi_request::interaction_layout] =
      mobile_api::LayoutMode::ICON_ONLY;

  ExpectMobileResult(mobile_api::Result::INVALID_DATA);
}

TEST_F(PerformInteractionRequestTest, Run_FoundInvalidChouiseSet_InvalidId) {
  kMsgParams_[strings::interaction_choice_set_id_list][0] = kSomeNumber;
  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_)).WillOnce(ReturnNull());

  ExpectMobileResult(mobile_api::Result::INVALID_ID);
}

TEST_F(PerformInteractionRequestTest, Run_TwoSameChoisesetIds_InvalidId) {
  kMsgParams_[strings::interaction_choice_set_id_list][0] = kSomeNumber;

  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;
  choise_sets[strings::choice_set][1][strings::choice_id] = kSomeNumber;

  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillOnce(Return(&choise_sets));

  ExpectMobileResult(mobile_api::Result::INVALID_ID);
}

TEST_F(PerformInteractionRequestTest, Run_InvalidImageVrHelpItems_InvalidData) {
  kMsgParams_[strings::interaction_choice_set_id_list][0] = kSomeNumber;

  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;
  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillOnce(Return(&choise_sets));

  kMsgParams_[strings::vr_help] = "vr_help";
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              VerifyImageVrHelpItems(_, _, _))
      .WillOnce(Return(mobile_api::Result::INVALID_ENUM));

  ExpectMobileResult(mobile_api::Result::INVALID_DATA);
}

TEST_F(PerformInteractionRequestTest,
       Run_WhiteSpaceExistsInInitialText_InvalidData) {
  kMsgParams_[strings::interaction_choice_set_id_list][0] = kSomeNumber;

  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;
  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillOnce(Return(&choise_sets));

  kMsgParams_[strings::initial_text] = kNotValidText;

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
  kMsgParams_[strings::interaction_choice_set_id_list][0] = kSomeNumber;

  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;
  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillOnce(Return(&choise_sets));

  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              VerifyImageVrHelpItems(_, _, _))
      .WillOnce(Return(mobile_api::Result::SUCCESS));

  kMsgParams_[strings::initial_text] = kValidText;
  kMsgParams_[strings::vr_help][0][strings::text] = kNotValidText;

  ExpectMobileResult(mobile_api::Result::INVALID_DATA);
}

TEST_F(PerformInteractionRequestTest,
       Run_WhiteSpaceExistsInVrHelpImageValue_InvalidData) {
  kMsgParams_[strings::interaction_choice_set_id_list][0] = kSomeNumber;

  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;
  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillOnce(Return(&choise_sets));

  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              VerifyImageVrHelpItems(_, _, _))
      .WillOnce(Return(mobile_api::Result::SUCCESS));

  kMsgParams_[strings::initial_text] = kValidText;
  kMsgParams_[strings::vr_help][0][strings::text] = kValidText;
  kMsgParams_[strings::vr_help][0][strings::image][strings::value] =
      kNotValidText;

  ExpectMobileResult(mobile_api::Result::INVALID_DATA);
}

TEST_F(PerformInteractionRequestTest,
       ChecVrSynonyms_InvalidChoiseSet_InvalidId) {
  kMsgParams_[strings::interaction_choice_set_id_list][0] = 1u;
  kMsgParams_[strings::interaction_choice_set_id_list][1] = 1u;
  kMsgParams_[strings::interaction_choice_set_id_list][2] = 1u;

  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillRepeatedly(ReturnNull());
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::INVALID_ID), _));

  command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::kCheckVrSynonyms);
}

TEST_F(PerformInteractionRequestTest,
       ChecVrSynonyms_TwoSameVrCommands_DuplicateName) {
  kMsgParams_[strings::interaction_choice_set_id_list][0] = kSomeNumber;
  kMsgParams_[strings::interaction_choice_set_id_list][1] = kSomeNumber;
  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::grammar_id] = kSomeNumber;
  choise_sets[strings::choice_set][0][strings::vr_commands][0] = kSomeNumber;
  choise_sets[strings::choice_set][0][strings::vr_commands][1] = kSomeNumber;

  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillRepeatedly(Return(&choise_sets));

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::DUPLICATE_NAME), _));

  const bool kResult = command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::kCheckVrSynonyms);

  EXPECT_FALSE(kResult);
}

TEST_F(PerformInteractionRequestTest,
       CheckMenuNames_InvalidListOfNames_InvalidId) {
  kMsgParams_[strings::interaction_choice_set_id_list][0] = kSomeNumber;
  kMsgParams_[strings::interaction_choice_set_id_list][1] = kSomeNumber;

  EXPECT_CALL(
      *mock_application_sptr_,
      FindChoiceSet(
          kMsgParams_[strings::interaction_choice_set_id_list][0].asInt()))
      .Times(3)
      .WillRepeatedly(ReturnNull());
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::INVALID_ID), _));

  const bool kResult = command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::kCheckMenuNames);

  EXPECT_FALSE(kResult);
}

TEST_F(PerformInteractionRequestTest,
       CheckMenuNames_TwoSameNames_DuplicateName) {
  kMsgParams_[strings::interaction_choice_set_id_list][0] = kSomeNumber;
  kMsgParams_[strings::interaction_choice_set_id_list][1] = kSomeNumber;

  ::smart_objects::SmartObject choise_set_1;
  ::smart_objects::SmartObject choise_set_2;
  choise_set_1[strings::choice_set][0][strings::menu_name] = "MenuName";
  choise_set_2[strings::choice_set][0][strings::menu_name] = "MenuName";

  EXPECT_CALL(
      *mock_application_sptr_,
      FindChoiceSet(
          kMsgParams_[strings::interaction_choice_set_id_list][0].asInt()))
      .Times(3)
      .WillOnce(Return(&choise_set_1))
      // Second call will be scipped in method cycles
      .WillOnce(ReturnNull())
      // Third call value will be compared to value from first call
      .WillOnce(Return(&choise_set_2));

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::DUPLICATE_NAME), _));

  const bool kResult = command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::kCheckMenuNames);

  EXPECT_FALSE(kResult);
}

TEST_F(PerformInteractionRequestTest, CheckMenuNames_AllRight_Success) {
  kMsgParams_[strings::interaction_choice_set_id_list][0] = kSomeNumber;
  kMsgParams_[strings::interaction_choice_set_id_list][1] = kSomeNumber;

  ::smart_objects::SmartObject choise_set_1;
  ::smart_objects::SmartObject choise_set_2;
  choise_set_1[strings::choice_set][0][strings::menu_name] = "MenuName";
  choise_set_2[strings::choice_set][0][strings::menu_name] = "AnotherMenuName";

  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .Times(6)
      .WillOnce(Return(&choise_set_1))
      // Second call will be scipped in method cycles
      .WillOnce(ReturnNull())
      // Third call value will be compared to value from first call
      .WillOnce(Return(&choise_set_2))
      // First cycle second iteration
      .WillOnce(Return(&choise_set_2))
      // Second cycle first iteration
      .WillOnce(Return(&choise_set_1))
      // Second cycle second iteration will be skipped
      .WillOnce(ReturnNull());

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);

  const bool kResult = command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::kCheckMenuNames);

  EXPECT_TRUE(kResult);
}

TEST_F(PerformInteractionRequestTest, CheckVrHelpItem_VrHelpNotExists_Success) {
  const bool kResult = command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::kCheckVrHelpItem);

  EXPECT_TRUE(kResult);
}

TEST_F(PerformInteractionRequestTest,
       CheckVrHelpItem_IncorrectPosition_Rejected) {
  kMsgParams_[strings::vr_help][0][strings::position] = kSomeNumber;
  kMsgParams_[strings::vr_help][1][strings::position] = kSomeAnotherNumber;

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::REJECTED), _));

  const bool kResult = command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::kCheckVrHelpItem);

  EXPECT_FALSE(kResult);
}

TEST_F(PerformInteractionRequestTest,
       CheckVrHelpItem_CorrectPositions_Success) {
  kMsgParams_[strings::vr_help][0][strings::position] = kSomeNumber;
  kMsgParams_[strings::vr_help][1][strings::position] = kSomeNumber + 1;

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);

  const bool kResult = command_sptr_->CallCheckMethod(
      commands::PerformInteractionRequest::CheckMethod::kCheckVrHelpItem);

  EXPECT_TRUE(kResult);
}

TEST_F(PerformInteractionRequestTest,
       Run_InvalidInteractionMode_StopProcessing) {
  kMsgParams_[strings::interaction_choice_set_id_list][0] = kSomeNumber;
  kMsgParams_[strings::interaction_mode] =
      mobile_api::InteractionMode::INVALID_ENUM;
  kMsgParams_[strings::initial_text] = kValidText;

  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;

  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillOnce(Return(&choise_sets));

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(mock_app_manager_, ManageHMICommand(_)).Times(0);

  CallRun caller(*command_sptr_);
  caller();
}

TEST_F(PerformInteractionRequestTest, Run_AllRight_SendVRAndUIRequests) {
  kMsgParams_[strings::interaction_choice_set_id_list][0] = 1u;
  kMsgParams_[strings::interaction_mode] = mobile_api::InteractionMode::VR_ONLY;
  kMsgParams_[strings::initial_text] = kValidText;
  kMsgParams_[strings::help_prompt][0][strings::text] = "Prompt";

  ::smart_objects::SmartObject choise_sets;
  choise_sets[strings::grammar_id] = kSomeNumber;
  choise_sets[strings::choice_set][0][strings::choice_id] = kSomeNumber;
  choise_sets[strings::choice_set][0][strings::vr_commands][0] = kSomeNumber;

  EXPECT_CALL(*mock_application_sptr_, FindChoiceSet(_))
      .WillRepeatedly(Return(&choise_sets));

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);

  command_sptr_->Init();
  CallRun caller(*command_sptr_);

  MessageSharedPtr first_msg;
  MessageSharedPtr second_msg;
  {
    ::testing::InSequence dummy;
    EXPECT_CALL(this->mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&first_msg), Return(true)));
    EXPECT_CALL(this->mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&second_msg), Return(true)));
  }
  caller();

  const hmi_apis::FunctionID::eType kFirstResult =
      static_cast<hmi_apis::FunctionID::eType>(
          (*first_msg)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(hmi_apis::FunctionID::VR_PerformInteraction, kFirstResult);

  const hmi_apis::FunctionID::eType kSecondResult =
      static_cast<hmi_apis::FunctionID::eType>(
          (*second_msg)[strings::params][strings::function_id].asInt());
  EXPECT_EQ(hmi_apis::FunctionID::UI_PerformInteraction, kSecondResult);
}

TEST_F(PerformInteractionRequestTest, OnEvent_OnResetTimeout_UpdateTimeout) {
  event_engine::Event event(hmi_apis::FunctionID::UI_OnResetTimeout);
  CallOnEvent caller(*command_sptr_, event);

  EXPECT_CALL(mock_app_manager_, updateRequestTimeout(_, _, _));

  caller();
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_PerformInteractionResponce_Unsuccess) {
  event_engine::Event event(hmi_apis::FunctionID::UI_PerformInteraction);
  CallOnEvent caller(*command_sptr_, event);

  ApplicationSharedPtr null_application;
  EXPECT_CALL(mock_app_manager_, application(_))
      .WillOnce(Return(null_application));
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);

  caller();
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_PerformInteractionResponceUnsupportedResource_CorrectCode) {
  event_engine::Event event(hmi_apis::FunctionID::UI_PerformInteraction);
  CallOnEvent caller(*command_sptr_, event);
  const std::string kExpectedInfo =
      "Unsupported phoneme type was sent in an item";
  const std::string kExpectedData = "some data";

  (*message_)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*message_)[strings::params][strings::data]["some_field"] = kExpectedData;

  event.set_smart_object(*message_);

  EXPECT_CALL(*mock_application_sptr_, DeletePerformInteractionChoiceSet(_))
      .Times(2);

  MessageSharedPtr result_msg = CatchMobileCommandResult(caller);

  const mobile_api::Result::eType kResult =
      static_cast<mobile_api::Result::eType>(
          (*result_msg)[strings::msg_params][strings::result_code].asInt());
  const std::string& kResultInfo =
      (*result_msg)[strings::msg_params][strings::info].asString();

  EXPECT_EQ(mobile_api::Result::UNSUPPORTED_RESOURCE, kResult);
  EXPECT_EQ(kExpectedInfo, kResultInfo);
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_PerformInteractionResponceInvalidChoiseId_GenericError) {
  event_engine::Event event(hmi_apis::FunctionID::UI_PerformInteraction);
  CallOnEvent caller(*command_sptr_, event);
  const std::string kExpectedInfo = "Wrong choiceID was received from HMI";

  ::smart_objects::SmartObject message;
  message[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  message[strings::msg_params][strings::choice_id] = kSomeNumber;

  event.set_smart_object(message);

  PerformChoiceSetMap empty_choice_set_map;
  const DataAccessor<PerformChoiceSetMap> empty_data_accessor(
      empty_choice_set_map, test_lock_);
  EXPECT_CALL(*mock_application_sptr_, performinteraction_choice_set_map())
      .WillOnce(Return(empty_data_accessor));

  EXPECT_CALL(*mock_application_sptr_, DeletePerformInteractionChoiceSet(_))
      .Times(2);

  MessageSharedPtr result_msg = CatchMobileCommandResult(caller);

  const mobile_api::Result::eType kResult =
      static_cast<mobile_api::Result::eType>(
          (*result_msg)[strings::msg_params][strings::result_code].asInt());
  const std::string& kResultInfo =
      (*result_msg)[strings::msg_params][strings::info].asString();

  EXPECT_EQ(mobile_api::Result::GENERIC_ERROR, kResult);
  EXPECT_EQ(kExpectedInfo, kResultInfo);
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_PerformInteractionResponceCorrectChoiseSet_TriggerSourceTsMenu) {
  event_engine::Event event(hmi_apis::FunctionID::UI_PerformInteraction);
  CallOnEvent caller(*command_sptr_, event);
  const std::string kExpectedInfo = "";

  (*message_)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*message_)[strings::params][strings::correlation_id] = kCorrelationId;
  (*message_)[strings::msg_params][strings::choice_id] = kCorrelationId;

  event.set_smart_object(*message_);

  ::smart_objects::SmartObject choise_set;
  choise_set[strings::choice_set][0][strings::choice_id] = kCorrelationId;

  PerformChoice perform_choise_map;
  perform_choise_map[kCorrelationId] = &choise_set;

  PerformChoiceSetMap choice_set_map;
  choice_set_map[kCorrelationId] = perform_choise_map;

  const DataAccessor<PerformChoiceSetMap> data_accessor(choice_set_map,
                                                        test_lock_);
  EXPECT_CALL(*mock_application_sptr_, performinteraction_choice_set_map())
      .WillOnce(Return(data_accessor));

  EXPECT_CALL(*mock_application_sptr_, DeletePerformInteractionChoiceSet(_))
      .Times(2);

  MessageSharedPtr result_msg = CatchMobileCommandResult(caller);

  const mobile_api::Result::eType kResult =
      static_cast<mobile_api::Result::eType>(
          (*result_msg)[strings::msg_params][strings::result_code].asInt());
  const std::string& kResultInfo =
      (*result_msg)[strings::msg_params][strings::info].asString();

  EXPECT_EQ(mobile_api::Result::SUCCESS, kResult);
  EXPECT_EQ(kExpectedInfo, kResultInfo);
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_PerformInteractionResponceEmptyTextEntry_TriggerSourceKeyboard) {
  event_engine::Event event(hmi_apis::FunctionID::UI_PerformInteraction);
  CallOnEvent caller(*command_sptr_, event);
  const std::string kExpectedInfo = "";

  (*message_)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*message_)[strings::params][strings::correlation_id] = kCorrelationId;
  (*message_)[strings::msg_params][strings::manual_text_entry] = "";

  event.set_smart_object(*message_);

  EXPECT_CALL(*mock_application_sptr_, DeletePerformInteractionChoiceSet(_))
      .Times(2);

  MessageSharedPtr result_msg = CatchMobileCommandResult(caller);

  const mobile_api::Result::eType kResult =
      static_cast<mobile_api::Result::eType>(
          (*result_msg)[strings::msg_params][strings::result_code].asInt());
  const std::string& kResultInfo =
      (*result_msg)[strings::msg_params][strings::info].asString();
  const bool kTextEntryExists =
      (*result_msg)[strings::msg_params].keyExists(strings::manual_text_entry);

  EXPECT_FALSE(kTextEntryExists);
  EXPECT_EQ(mobile_api::Result::SUCCESS, kResult);
  EXPECT_EQ(kExpectedInfo, kResultInfo);
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRResponceIncorrectApp_NoResponces) {
  event_engine::Event event(hmi_apis::FunctionID::VR_PerformInteraction);
  CallOnEvent caller(*command_sptr_, event);

  ApplicationSharedPtr null_application;
  EXPECT_CALL(mock_app_manager_, application(_))
      .WillRepeatedly(Return(null_application));
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(mock_app_manager_, ManageHMICommand(_)).Times(0);

  caller();
}

TEST_F(
    PerformInteractionRequestTest,
    OnEvent_VRResponcVRPIcodeGenericError_TerminatePIAndGenericErrorResponce) {
  TestVRResponceResultCode(mobile_apis::Result::GENERIC_ERROR);
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRResponcVRPIcodeAbortedVrOnly_TerminatePIAndAbortedResponce) {
  TestVRResponceResultCode(mobile_apis::Result::ABORTED);
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRResponcVRPIcodeTimeout_TerminatePIAndTimeoutResponce) {
  TestVRResponceResultCode(mobile_apis::Result::TIMED_OUT);
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRResponcVRPIcodeTimeOut_ResetTimeout) {
  event_engine::Event event(hmi_apis::FunctionID::VR_PerformInteraction);
  CallOnEvent caller(*command_sptr_, event);

  (*message_)[strings::params][hmi_response::code] =
      mobile_apis::Result::TIMED_OUT;

  event.set_smart_object(*message_);

  EXPECT_CALL(mock_app_manager_, updateRequestTimeout(_, _, _));
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);

  caller();
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRResponcVRPIcodeRejected_TerminatePIAndRejectedResponce) {
  TestVRResponceResultCode(mobile_apis::Result::REJECTED);
}

TEST_F(
    PerformInteractionRequestTest,
    OnEvent_VRResponcVRPIcodeUnsupportedRequest_TerminatePIAndWarningsResponce) {
  TestVRResponceResultCode(mobile_apis::Result::UNSUPPORTED_REQUEST);
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRResponcVRPIcodeWarnings_TerminatePIAndWarningsResponce) {
  TestVRResponceResultCode(mobile_apis::Result::WARNINGS);
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRResponcVRPIcodeSuccessManualOnly_Return) {
  event_engine::Event event(hmi_apis::FunctionID::VR_PerformInteraction);
  CallOnEvent caller(*command_sptr_, event);

  (*message_)[strings::params][hmi_response::code] =
      mobile_apis::Result::SUCCESS;

  event.set_smart_object(*message_);

  (*message_)[strings::msg_params][strings::interaction_mode] =
      mobile_api::InteractionMode::MANUAL_ONLY;
  command_sptr_->Init();

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);

  caller();
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRResponcVRPIWrongChoiset_ResponceGenericError) {
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
    EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&hmi_result_msg), Return(true)));

    EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _))
        .WillOnce(DoAll(SaveArg<0>(&mobile_result_msg), Return(true)));
  }

  caller();

  const hmi_apis::FunctionID::eType kHmiResult =
      static_cast<hmi_apis::FunctionID::eType>(
          (*hmi_result_msg)[strings::params][strings::function_id].asInt());
  const mobile_api::Result::eType kMobileResult =
      static_cast<mobile_api::Result::eType>(
          (*mobile_result_msg)[strings::msg_params][strings::result_code]
              .asInt());

  EXPECT_EQ(hmi_apis::FunctionID::UI_ClosePopUp, kHmiResult);
  EXPECT_EQ(mobile_apis::Result::GENERIC_ERROR, kMobileResult);
}

TEST_F(PerformInteractionRequestTest,
       OnEvent_VRResponcVRPICorrectChoiSet_ResponceSuccess) {
  event_engine::Event event(hmi_apis::FunctionID::VR_PerformInteraction);
  CallOnEvent caller(*command_sptr_, event);

  (*message_)[strings::msg_params][strings::result_code] =
      mobile_api::Result::SUCCESS;

  (*message_)[strings::params][strings::correlation_id] = kCorrelationId;
  (*message_)[strings::msg_params][strings::choice_id] = kSomeNumber;

  event.set_smart_object(*message_);

  ::smart_objects::SmartObject choise_set;
  choise_set[strings::choice_set][0][strings::choice_id] = kSomeNumber;

  PerformChoice perform_choise_map;
  perform_choise_map[kCorrelationId] = &choise_set;

  PerformChoiceSetMap choice_set_map;
  choice_set_map[kCorrelationId] = perform_choise_map;

  const DataAccessor<PerformChoiceSetMap> data_accessor(choice_set_map,
                                                        test_lock_);
  EXPECT_CALL(*mock_application_sptr_, performinteraction_choice_set_map())
      .WillOnce(Return(data_accessor));

  MessageSharedPtr hmi_result_msg;
  MessageSharedPtr mobile_result_msg;
  {
    ::testing::InSequence dummy;
    EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&hmi_result_msg), Return(true)));

    EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _))
        .WillOnce(DoAll(SaveArg<0>(&mobile_result_msg), Return(true)));
  }

  caller();

  const hmi_apis::FunctionID::eType kHmiResult =
      static_cast<hmi_apis::FunctionID::eType>(
          (*hmi_result_msg)[strings::params][strings::function_id].asInt());
  const mobile_api::Result::eType kMobileResult =
      static_cast<mobile_api::Result::eType>(
          (*mobile_result_msg)[strings::msg_params][strings::result_code]
              .asInt());

  const int32_t kResultChoiseId =
      (*mobile_result_msg)[strings::msg_params][strings::choice_id].asInt();
  const mobile_api::TriggerSource::eType ts_result =
      static_cast<mobile_api::TriggerSource::eType>(
          (*mobile_result_msg)[strings::msg_params][strings::trigger_source]
              .asInt());

  EXPECT_EQ(hmi_apis::FunctionID::UI_ClosePopUp, kHmiResult);

  EXPECT_EQ(mobile_api::TriggerSource::TS_VR, ts_result);
  EXPECT_EQ(kSomeNumber, kResultChoiseId);
  EXPECT_EQ(mobile_apis::Result::SUCCESS, kMobileResult);
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
