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

#include "mobile/create_interaction_choice_set_request.h"
#include "mobile/create_interaction_choice_set_response.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "utils/helpers.h"
#include "utils/make_shared.h"
#include "smart_objects/smart_object.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/hmi_interfaces.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "utils/lock.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace create_interaction_choice_set_request {

namespace am = application_manager;
namespace strings = am::strings;
namespace hmi_response = am::hmi_response;

using ::testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::AtLeast;
using ::utils::SharedPtr;
using am::commands::MessageSharedPtr;
using am::MockMessageHelper;
using am::commands::CreateInteractionChoiceSetRequest;
using am::commands::CreateInteractionChoiceSetResponse;

typedef SharedPtr<CreateInteractionChoiceSetRequest>
    CreateInteractionChoiceSetRequestPtr;
typedef SharedPtr<CreateInteractionChoiceSetResponse>
    CreateInteractionChoiceSetResponsePtr;

namespace {
const uint32_t kCommandId = 1u;
const uint32_t kConnectionKey = 2u;
const uint32_t kCorrelationId = 10u;
const uint32_t kGrammarId = 10u;
const uint32_t kChoiceSetId = 1u;
const uint32_t kChoiceId0 = 2u;
const uint32_t kChoiceId1 = 3u;
const std::string kImage = "image";
const std::string kSecondImage = "second_image";
const std::string kVrCommand0 = "vr_command_0";
const std::string kVrCommand1 = "vr_command_1";
const std::string kMenuName0 = "menu_name_0";
const std::string kMenuName1 = "menu_name_1";
const am::HmiInterfaces::InterfaceID kHmiInterface =
    am::HmiInterfaces::HMI_INTERFACE_SDL;
}  // namespace

class CreateInteractionChoiceSetRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  CreateInteractionChoiceSetRequestTest()
      : mock_message_helper_(am::MockMessageHelper::message_helper_mock())
      , message_(CreateMessage())
      , command_(CreateCommand<CreateInteractionChoiceSetRequest>(message_))
      , mock_app_(CreateMockApp()) {
    (*message_)[strings::params][strings::connection_key] = kConnectionKey;
    ON_CALL(*mock_app_, choice_set_map())
        .WillByDefault(Return(
            DataAccessor<am::ChoiceSetMap>(dummy_choice_set_map_, lock_)));
    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(mock_hmi_interfaces_));
    ON_CALL(mock_hmi_interfaces_, GetInterfaceFromFunction(_))
        .WillByDefault(Return(kHmiInterface));
    ON_CALL(mock_hmi_interfaces_, GetInterfaceState(kHmiInterface))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  ~CreateInteractionChoiceSetRequestTest() {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void FillMessageFieldsItem1(MessageSharedPtr message) {
    SmartObject& choice_set =
        (*message)[am::strings::msg_params][am::strings::choice_set];
    choice_set[0][am::strings::menu_name] = kMenuName0;
    choice_set[0][am::strings::image][am::strings::value] = kImage;
    choice_set[0][am::strings::choice_id] = kChoiceId0;
    choice_set[0][am::strings::vr_commands][0] = kVrCommand0;
    choice_set[0][am::strings::secondary_image][am::strings::value] =
        kSecondImage;
  }
  void FillMessageFieldsItem2(MessageSharedPtr message) {
    SmartObject& choice_set =
        (*message)[am::strings::msg_params][am::strings::choice_set];

    choice_set[1][am::strings::choice_id] = kChoiceId1;
    choice_set[1][am::strings::menu_name] = kMenuName1;
    choice_set[1][am::strings::vr_commands][0] = kVrCommand1;
    (*message)[am::strings::msg_params]
              [am::strings::interaction_choice_set_id] = kChoiceSetId;
  }

  MessageSharedPtr CreateFullParamsVRSO() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;
    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params[strings::cmd_id] = kCommandId;
    msg_params[strings::vr_commands] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    msg_params[strings::vr_commands][0] = "lamer";
    msg_params[strings::type] = 34;
    msg_params[strings::grammar_id] = 12;
    msg_params[strings::app_id] = kConnectionKey;
    (*msg)[strings::msg_params] = msg_params;

    return msg;
  }

  am::MockMessageHelper* mock_message_helper_;
  MessageSharedPtr message_;
  CreateInteractionChoiceSetRequestPtr command_;
  MockAppPtr mock_app_;
  am::MockHmiInterfaces mock_hmi_interfaces_;
  am::ChoiceSetMap dummy_choice_set_map_;
  sync_primitives::Lock lock_;
};

TEST_F(CreateInteractionChoiceSetRequestTest, OnTimeout_GENERIC_ERROR) {
  MessageSharedPtr msg_vr = CreateMessage(smart_objects::SmartType_Map);
  (*msg_vr)[strings::params][strings::connection_key] = kConnectionKey;
  (*msg_vr)[strings::msg_params][strings::result_code] =
      am::mobile_api::Result::GENERIC_ERROR;
  (*msg_vr)[strings::msg_params][strings::success] = false;

  utils::SharedPtr<CreateInteractionChoiceSetRequest> req_vr =
      CreateCommand<CreateInteractionChoiceSetRequest>(msg_vr);

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
  ON_CALL(*mock_app_, get_grammar_id()).WillByDefault(Return(kGrammarId));
  ON_CALL(*mock_app_, RemoveCommand(_)).WillByDefault(Return());

  MessageSharedPtr vr_command_result;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&vr_command_result), Return(true)));

  req_vr->onTimeOut();
  EXPECT_EQ(
      (*vr_command_result)[strings::msg_params][strings::success].asBool(),
      false);
  EXPECT_EQ(
      (*vr_command_result)[strings::msg_params][strings::result_code].asInt(),
      static_cast<int32_t>(am::mobile_api::Result::GENERIC_ERROR));
}

TEST_F(CreateInteractionChoiceSetRequestTest, OnEvent_VR_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg_vr = CreateFullParamsVRSO();
  (*msg_vr)[strings::msg_params][strings::choice_set][0][strings::choice_id] =
      10;
  (*msg_vr)[strings::msg_params][strings::choice_set][0][strings::menu_name] =
      "menu_name";
  (*msg_vr)[strings::msg_params][strings::interaction_choice_set_id] = 11;
  utils::SharedPtr<CreateInteractionChoiceSetRequest> req_vr =
      CreateCommand<CreateInteractionChoiceSetRequest>(msg_vr);

  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app_));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
  smart_objects::SmartObject* null_obj = NULL;
  ON_CALL(*mock_app_, FindChoiceSet(_)).WillByDefault(Return(null_obj));

  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg)[strings::msg_params][strings::info] = "VR is not supported by system";
  (*msg)[strings::msg_params][strings::cmd_id] = kCommandId;

  am::event_engine::Event event(hmi_apis::FunctionID::VR_AddCommand);
  event.set_smart_object(*msg);

  smart_objects::SmartObject* ptr = NULL;
  ON_CALL(*mock_app_, FindCommand(kCommandId)).WillByDefault(Return(ptr));
  EXPECT_EQ(NULL, ptr);

  MockMessageHelper* mock_message_helper =
      MockMessageHelper::message_helper_mock();
  ON_CALL(*mock_message_helper, HMIToMobileResult(_))
      .WillByDefault(Return(mobile_apis::Result::SUCCESS));

  req_vr->Run();

  MessageSharedPtr vr_command_result;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&vr_command_result), Return(true)));

  req_vr->on_event(event);

  EXPECT_EQ(
      (*vr_command_result)[strings::msg_params][strings::success].asBool(),
      false);
  EXPECT_EQ(
      (*vr_command_result)[strings::msg_params][strings::result_code].asInt(),
      static_cast<int32_t>(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE));
  if ((*vr_command_result)[strings::msg_params].keyExists(strings::info)) {
    EXPECT_EQ(
        (*vr_command_result)[strings::msg_params][strings::info].asString(),
        (*msg)[strings::msg_params][strings::info].asString());
  }
}

class CreateInteractionChoiceSetResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(CreateInteractionChoiceSetRequestTest, Run_InvalidApp_UNSUCCESS) {
  MockAppPtr invalid_app;
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(invalid_app));
  EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);

  command_->Run();
}

TEST_F(CreateInteractionChoiceSetRequestTest, Run_VerifyImageFail_UNSUCCESS) {
  SmartObject& choice_set =
      (*message_)[am::strings::msg_params][am::strings::choice_set];
  choice_set[0][am::strings::image] = kImage;
  choice_set[0][am::strings::secondary_image] = kSecondImage;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::INVALID_DATA));
  EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);

  command_->Run();
}

TEST_F(CreateInteractionChoiceSetRequestTest, Run_FindChoiceSetFail_UNSUCCESS) {
  SmartObject& choice_set =
      (*message_)[am::strings::msg_params][am::strings::choice_set];

  choice_set[0][am::strings::image] = kImage;
  choice_set[0][am::strings::secondary_image] = kSecondImage;
  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* invalid_choice_set_id =
      &((*message_)[am::strings::msg_params]
                   [am::strings::interaction_choice_set_id]);
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(invalid_choice_set_id));
  EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
  command_->Run();
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       Run_CheckChoiceSet_InvalidChoiceId_UNSUCCESS) {
  SmartObject& choice_set =
      (*message_)[am::strings::msg_params][am::strings::choice_set];

  choice_set[0][am::strings::menu_name] = kMenuName0;
  choice_set[0][am::strings::image][am::strings::value] = kImage;
  choice_set[0][am::strings::choice_id] = kChoiceId0;
  choice_set[0][am::strings::secondary_image][am::strings::value] =
      kSecondImage;

  FillMessageFieldsItem2(message_);
  choice_set[1][am::strings::vr_commands][0] = kVrCommand0;
  choice_set[1][am::strings::vr_commands][1] = " kVrCommand1\t";

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(*mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
  command_->Run();
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       Run_IsWhiteSpaceVRCommandsExist_InvalidMenuName_UNSUCCESS) {
  SmartObject& choice_set =
      (*message_)[am::strings::msg_params][am::strings::choice_set];

  choice_set[0][am::strings::menu_name] = "menu_name\t";
  choice_set[0][am::strings::secondary_text] = "secondary_text\t";
  choice_set[0][am::strings::tertiary_text] = "tertiary_text\t";
  choice_set[0][am::strings::image][am::strings::value] = "image\t";
  choice_set[0][am::strings::choice_id] = kChoiceId0;
  choice_set[0][am::strings::secondary_image][am::strings::value] =
      "second_image\t";
  choice_set[0][am::strings::vr_commands][0] = "vr_commands_1\t";

  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillRepeatedly(Return(choice_set_id));
  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(mock_app_));

  EXPECT_CALL(*mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  if (choice_set[0].keyExists(am::strings::menu_name)) {
    CreateInteractionChoiceSetRequestPtr command(
        CreateCommand<CreateInteractionChoiceSetRequest>(message_));

    EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
    command->Run();
  }
  if (choice_set[0].keyExists(am::strings::secondary_text)) {
    choice_set[0][am::strings::menu_name] = kMenuName0;
    CreateInteractionChoiceSetRequestPtr command(
        CreateCommand<CreateInteractionChoiceSetRequest>(message_));

    EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
    command->Run();
  }
  if (choice_set[0].keyExists(am::strings::tertiary_text)) {
    choice_set[0][am::strings::secondary_text] = "secondary_text";
    CreateInteractionChoiceSetRequestPtr command(
        CreateCommand<CreateInteractionChoiceSetRequest>(message_));

    EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
    command->Run();
  }
  if (choice_set[0].keyExists(am::strings::vr_commands)) {
    choice_set[0][am::strings::tertiary_text] = "tertiary_text";
    CreateInteractionChoiceSetRequestPtr command(
        CreateCommand<CreateInteractionChoiceSetRequest>(message_));

    EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
    command->Run();
  }
  if (choice_set[0].keyExists(am::strings::image)) {
    choice_set[0][am::strings::vr_commands][0] = "vr_commands";
    CreateInteractionChoiceSetRequestPtr command(
        CreateCommand<CreateInteractionChoiceSetRequest>(message_));

    EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
    command->Run();
  }
  if (choice_set[0].keyExists(am::strings::secondary_image)) {
    choice_set[0][am::strings::image][am::strings::value] = kImage;
    CreateInteractionChoiceSetRequestPtr command(
        CreateCommand<CreateInteractionChoiceSetRequest>(message_));

    EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
    command->Run();
  }
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       Run_ValidAmountVrCommands_SUCCESS) {
  FillMessageFieldsItem1(message_);
  FillMessageFieldsItem2(message_);
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(*mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID()).WillOnce(Return(kGrammarId));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));
  EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
      .Times(AtLeast(2))
      .WillOnce(Return(kConnectionKey))
      .WillOnce(Return(kConnectionKey));
  command_->Run();
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       Run_EmptyAmountVrCommands_SUCCESS) {
  SmartObject& choice_set =
      (*message_)[am::strings::msg_params][am::strings::choice_set];

  choice_set[0][am::strings::menu_name] = kMenuName0;
  choice_set[0][am::strings::image][am::strings::value] = kImage;
  choice_set[0][am::strings::choice_id] = kChoiceId0;
  choice_set[0][am::strings::secondary_image][am::strings::value] =
      kSecondImage;

  choice_set[1][am::strings::choice_id] = kChoiceId1;
  choice_set[1][am::strings::menu_name] = kMenuName1;
  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(*mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));
  command_->Run();
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       OnEvent_InvalidEventId_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::INVALID_ENUM);

  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _)).Times(0);
  command_->on_event(event);
}

TEST_F(CreateInteractionChoiceSetRequestTest, OnEvent_ValidVrNoError_SUCCESS) {
  Event event(hmi_apis::FunctionID::VR_AddCommand);

  (*message_)[am::strings::params][am::strings::correlation_id] =
      kCorrelationId;
  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::WARNINGS;

  FillMessageFieldsItem1(message_);
  FillMessageFieldsItem2(message_);

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(*mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID()).WillOnce(Return(kGrammarId));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  command_->Run();

  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _));
  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _)).Times(0);
  event.set_smart_object(*message_);

  command_->on_event(event);
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       OnEvent_InValidVrNoError_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::VR_AddCommand);

  (*message_)[am::strings::params][am::strings::correlation_id] =
      kCorrelationId;
  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::INVALID_DATA;

  FillMessageFieldsItem1(message_);
  FillMessageFieldsItem2(message_);
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(*mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID()).WillOnce(Return(kGrammarId));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  command_->Run();
  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _));
  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _)).Times(0);
  event.set_smart_object(*message_);

  command_->on_event(event);
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       OnEvent_ValidVrNoErrorAndExpectedChoiceLessThanReceiveChoice_SUCCESS) {
  Event event(hmi_apis::FunctionID::VR_AddCommand);

  (*message_)[am::strings::params][am::strings::correlation_id] =
      kCorrelationId;
  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::WARNINGS;

  FillMessageFieldsItem1(message_);

  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app_));

  EXPECT_CALL(*mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID()).WillOnce(Return(kGrammarId));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  command_->Run();

  FillMessageFieldsItem2(message_);

  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _));
  event.set_smart_object(*message_);
  command_->on_event(event);
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       OnTimeOut_InvalidErrorFromHMI_UNSUCCESS) {
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::GENERIC_ERROR),
                  am::commands::Command::ORIGIN_SDL));

  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _));
  command_->onTimeOut();
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       OnTimeOut_ValidErrorFromHMI_SUCCESS) {
  (*message_)[am::strings::params][am::strings::correlation_id] =
      kCorrelationId;
  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::INVALID_ENUM;

  FillMessageFieldsItem1(message_);
  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;

  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app_));

  EXPECT_CALL(*mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID()).WillOnce(Return(kGrammarId));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  command_->Run();

  FillMessageFieldsItem2(message_);
  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _));
  Event event(hmi_apis::FunctionID::VR_AddCommand);
  event.set_smart_object(*message_);
  command_->on_event(event);

  EXPECT_CALL(*mock_app_, RemoveChoiceSet(kChoiceSetId));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _));
  command_->onTimeOut();
}

TEST_F(CreateInteractionChoiceSetRequestTest, OnTimeOut_InvalidApp_UNSUCCESS) {
  (*message_)[am::strings::params][am::strings::correlation_id] =
      kCorrelationId;
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::INVALID_ENUM;

  FillMessageFieldsItem1(message_);
  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;

  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(mock_app_));

  EXPECT_CALL(*mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID()).WillOnce(Return(kGrammarId));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  command_->Run();

  FillMessageFieldsItem2(message_);
  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _)).Times(2);
  Event event(hmi_apis::FunctionID::VR_AddCommand);
  event.set_smart_object(*message_);
  command_->on_event(event);

  MockAppPtr invalid_app;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(invalid_app));
  EXPECT_CALL(*mock_app_, RemoveChoiceSet(_)).Times(0);
  command_->onTimeOut();
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       OnTimeOut_SuccessfulResponseReceived_UNSUCCESS) {
  (*message_)[am::strings::params][am::strings::correlation_id] =
      kCorrelationId;
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::SUCCESS;

  FillMessageFieldsItem1(message_);
  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(*mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID()).WillOnce(Return(kGrammarId));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  command_->Run();

  FillMessageFieldsItem2(message_);
  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _));
  Event event(hmi_apis::FunctionID::VR_AddCommand);
  event.set_smart_object(*message_);

  MockAppPtr invalid_app;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(invalid_app));
  command_->on_event(event);

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, RemoveChoiceSet(_));
  command_->onTimeOut();
}

TEST_F(CreateInteractionChoiceSetResponseTest, Run_SuccessFalse_UNSUCCESS) {
  MessageSharedPtr message(CreateMessage());
  (*message)[am::strings::msg_params][am::strings::success] = false;
  (*message)[am::strings::msg_params][am::strings::result_code] =
      mobile_apis::Result::INVALID_ENUM;
  CreateInteractionChoiceSetResponsePtr command(
      CreateCommand<CreateInteractionChoiceSetResponse>(message));

  EXPECT_CALL(app_mngr_, SendMessageToMobile(message, false));
  command->Run();
}

TEST_F(CreateInteractionChoiceSetResponseTest, Run_SuccessTrue_SUCCESS) {
  MessageSharedPtr message(CreateMessage());
  (*message)[am::strings::msg_params][am::strings::success] = true;
  (*message)[am::strings::msg_params][am::strings::result_code] =
      mobile_apis::Result::SUCCESS;
  CreateInteractionChoiceSetResponsePtr command(
      CreateCommand<CreateInteractionChoiceSetResponse>(message));

  EXPECT_CALL(app_mngr_, SendMessageToMobile(message, false));
  command->Run();
}

TEST_F(CreateInteractionChoiceSetRequestTest, Run_ErrorFromHmiFalse_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::VR_AddCommand);

  (*message_)[am::strings::params][am::strings::correlation_id] =
      kCorrelationId;
  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::GENERIC_ERROR;

  FillMessageFieldsItem1(message_);

  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app_));

  EXPECT_CALL(*mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::GENERIC_ERROR));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillRepeatedly(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID())
      .WillRepeatedly(Return(kGrammarId));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _)).Times(2);
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  command_->Run();

  FillMessageFieldsItem2(message_);

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::GENERIC_ERROR),
                  am::commands::Command::ORIGIN_SDL));
  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _));
  event.set_smart_object(*message_);
  command_->on_event(event);
  command_->Run();
}

}  // namespace create_interaction_choice_set
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
