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

#include "application_manager/commands/mobile/create_interaction_choice_set_request.h"
#include "application_manager/commands/mobile/create_interaction_choice_set_response.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "utils/helpers.h"
#include "utils/make_shared.h"
#include "smart_objects/smart_object.h"
#include "utils/custom_string.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_hmi_capabilities.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace create_interaction_choice_set_request {

namespace am = application_manager;
using am::commands::CommandImpl;
using am::ApplicationManager;
using am::commands::MessageSharedPtr;
using am::ApplicationSharedPtr;
using am::MockMessageHelper;
using ::testing::_;
using ::utils::SharedPtr;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::AtLeast;
using am::commands::CreateInteractionChoiceSetRequest;
using am::commands::CreateInteractionChoiceSetResponse;
using ::test::components::application_manager_test::MockApplication;

namespace custom_str = utils::custom_string;
namespace strings = ::application_manager::strings;
namespace hmi_response = ::application_manager::hmi_response;

typedef SharedPtr<CreateInteractionChoiceSetRequest>
    CreateInteractionChoiceSetRequestPtr;
typedef SharedPtr<CreateInteractionChoiceSetResponse>
    CreateInteractionChoiceSetResponsePtr;

typedef NiceMock<
    ::test::components::application_manager_test::MockHMICapabilities>
    MockHMICapabilities;

namespace {
const hmi_apis::FunctionID::eType kInvalidFunctionId =
    hmi_apis::FunctionID::INVALID_ENUM;
const int32_t kCommandId = 1;
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
const uint32_t kCorrelationId = 10u;
const uint32_t kGrammarId = 10u;
const int32_t kMenuId = 5;
const uint32_t kChoiceSetId = 1u;
const uint32_t kChoiceId1 = 2u;
const uint32_t kChoiceId2 = 3u;
const std::string kImage = "image";
const std::string kSecondImage = "second_image";
const std::string kVrCommands1 = "vr_commands_1";
const std::string kVrCommands2 = "vr_commands_2";
const std::string kMenuName = "menu_name";

}  // namespace

class CreateInteractionChoiceSetRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  CreateInteractionChoiceSetRequestTest()
      : message_(CreateMessage())
      , command_(CreateCommand<CreateInteractionChoiceSetRequest>(message_))
      , mock_app_(CreateMockApp())
      , lock_(std::make_shared<sync_primitives::Lock>()) {}

  MessageSharedPtr CreateFullParamsVRSO() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;
    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params[strings::cmd_id] = kCmdId;
    msg_params[strings::vr_commands] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    msg_params[strings::vr_commands][0] = "lamer";
    msg_params[strings::type] = 34;
    msg_params[strings::grammar_id] = 12;
    msg_params[strings::app_id] = kAppId;
    (*msg)[strings::msg_params] = msg_params;

    return msg;
  }

  void FillMessageFieldsItem1(MessageSharedPtr message) {
    (*message)[am::strings::msg_params][am::strings::choice_set][0]
              [am::strings::menu_name] = kMenuName;
    (*message)[am::strings::msg_params][am::strings::choice_set][0]
              [am::strings::image][am::strings::value] = kImage;
    (*message)[am::strings::msg_params][am::strings::choice_set][0]
              [am::strings::choice_id] = kChoiceId1;
    (*message)[am::strings::msg_params][am::strings::choice_set][0]
              [am::strings::vr_commands][0] = kVrCommands1;
    (*message)[am::strings::msg_params][am::strings::choice_set][0]
              [am::strings::secondary_image][am::strings::value] = kSecondImage;
  }
  void FillMessageFieldsItem2(MessageSharedPtr message) {
    (*message)[am::strings::msg_params][am::strings::choice_set][1]
              [am::strings::choice_id] = kChoiceId2;
    (*message)[am::strings::msg_params][am::strings::choice_set][1]
              [am::strings::menu_name] = kMenuName;
    (*message)[am::strings::msg_params][am::strings::choice_set][1]
              [am::strings::vr_commands][0] = kVrCommands2;
    (*message)[am::strings::msg_params]
              [am::strings::interaction_choice_set_id] = kChoiceSetId;
  }

  MessageSharedPtr message_;
  CreateInteractionChoiceSetRequestPtr command_;
  MockAppPtr mock_app_;
  std::shared_ptr<sync_primitives::Lock> lock_;
};

class CreateInteractionChoiceSetResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(CreateInteractionChoiceSetRequestTest, OnTimeout_GENERIC_ERROR) {
  MessageSharedPtr msg_vr = CreateMessage(smart_objects::SmartType_Map);
  (*msg_vr)[strings::msg_params][strings::result_code] =
      am::mobile_api::Result::GENERIC_ERROR;
  (*msg_vr)[strings::msg_params][strings::success] = false;

  utils::SharedPtr<CreateInteractionChoiceSetRequest> req_vr =
      CreateCommand<CreateInteractionChoiceSetRequest>();

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app));
  ON_CALL(*mock_app, app_id()).WillByDefault(Return(kConnectionKey));
  ON_CALL(*mock_app, get_grammar_id()).WillByDefault(Return(kConnectionKey));
  ON_CALL(*mock_app, RemoveCommand(_)).WillByDefault(Return());

  MessageSharedPtr vr_command_result;
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
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
  ON_CALL(*mock_app_, FindCommand(kCmdId)).WillByDefault(Return(ptr));
  EXPECT_EQ(NULL, ptr);

  am::CommandsMap commands_map;
  ON_CALL(*mock_app_, commands_map())
      .WillByDefault(
          Return(DataAccessor<am::CommandsMap>(commands_map, lock_)));

  ON_CALL(mock_hmi_interfaces_, GetInterfaceFromFunction(_))
      .WillByDefault(
          Return(am::HmiInterfaces::HMI_INTERFACE_BasicCommunication));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(rpc_service_, ManageHMICommand(_)).WillOnce(Return(true));

  req_vr->Run();

  MessageSharedPtr vr_command_result;
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
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

TEST_F(CreateInteractionChoiceSetRequestTest, Run_InvalidApp_UNSUCCESS) {
  MockAppPtr invalid_app;
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(invalid_app));
  EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));

  command_->Run();
}

TEST_F(CreateInteractionChoiceSetRequestTest, Run_VerifyImageFail_UNSUCCESS) {
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::image] = kImage;
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::secondary_image] = kSecondImage;

  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::INVALID_DATA));
  EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));

  command_->Run();
}

TEST_F(CreateInteractionChoiceSetRequestTest, Run_FindChoiceSetFail_UNSUCCESS) {
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::image] = kImage;
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::secondary_image] = kSecondImage;
  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;

  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* invalid_choice_set_id =
      &((*message_)[am::strings::msg_params]
                   [am::strings::interaction_choice_set_id]);
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(invalid_choice_set_id));
  EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  command_->Run();
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       Run_CheckChoiceSet_InvalidChoiceId_UNSUCCESS) {
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::menu_name] = kMenuName;
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::image][am::strings::value] = kImage;
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::choice_id] = kChoiceId1;
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::secondary_image][am::strings::value] = kSecondImage;
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::vr_commands][0] = kVrCommands1;

  FillMessageFieldsItem2(message_);
  (*message_)[am::strings::msg_params][am::strings::choice_set][1]
             [am::strings::vr_commands][0] = kVrCommands1;
  (*message_)[am::strings::msg_params][am::strings::choice_set][1]
             [am::strings::vr_commands][1] = " kVrCommands2\t";
  (*message_)[am::strings::msg_params][am::strings::choice_set][1]
             [am::strings::vr_commands][0] = kVrCommands1;

  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app_));

  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));

  EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
  command_->Run();
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       Run_IsWhiteSpaceVRCommandsExist_InvalidMenuName_UNSUCCESS) {
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::menu_name] = "menu_name\t";
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::secondary_text] = "secondary_text\t";
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::tertiary_text] = "tertiary_text\t";
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::image][am::strings::value] = "image\t";
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::choice_id] = kChoiceId1;
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::secondary_image][am::strings::value] =
                 "second_image\t";
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::vr_commands][0] = "vr_commands_1\t";

  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillRepeatedly(Return(choice_set_id));
  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(mock_app_));

  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));

  if ((*message_)[am::strings::msg_params][am::strings::choice_set][0]
          .keyExists(am::strings::menu_name)) {
    CreateInteractionChoiceSetRequestPtr command(
        CreateCommand<CreateInteractionChoiceSetRequest>(message_));

    EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
    command->Run();
  }
  if ((*message_)[am::strings::msg_params][am::strings::choice_set][0]
          .keyExists(am::strings::secondary_text)) {
    (*message_)[am::strings::msg_params][am::strings::choice_set][0]
               [am::strings::menu_name] = kMenuName;
    CreateInteractionChoiceSetRequestPtr command(
        CreateCommand<CreateInteractionChoiceSetRequest>(message_));

    EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
    command->Run();
  }
  if ((*message_)[am::strings::msg_params][am::strings::choice_set][0]
          .keyExists(am::strings::tertiary_text)) {
    (*message_)[am::strings::msg_params][am::strings::choice_set][0]
               [am::strings::secondary_text] = "secondary_text";
    CreateInteractionChoiceSetRequestPtr command(
        CreateCommand<CreateInteractionChoiceSetRequest>(message_));

    EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
    command->Run();
  }
  if ((*message_)[am::strings::msg_params][am::strings::choice_set][0]
          .keyExists(am::strings::vr_commands)) {
    (*message_)[am::strings::msg_params][am::strings::choice_set][0]
               [am::strings::tertiary_text] = "tertiary_text";
    CreateInteractionChoiceSetRequestPtr command(
        CreateCommand<CreateInteractionChoiceSetRequest>(message_));

    EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
    command->Run();
  }
  if ((*message_)[am::strings::msg_params][am::strings::choice_set][0]
          .keyExists(am::strings::image)) {
    (*message_)[am::strings::msg_params][am::strings::choice_set][0]
               [am::strings::vr_commands][0] = "vr_commands";
    CreateInteractionChoiceSetRequestPtr command(
        CreateCommand<CreateInteractionChoiceSetRequest>(message_));

    EXPECT_CALL(app_mngr_, GenerateGrammarID()).Times(0);
    command->Run();
  }
  if ((*message_)[am::strings::msg_params][am::strings::choice_set][0]
          .keyExists(am::strings::secondary_image)) {
    (*message_)[am::strings::msg_params][am::strings::choice_set][0]
               [am::strings::image][am::strings::value] = kImage;
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
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app_));

  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _))
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
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  command_->Run();
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       Run_EmptyAmountVrCommands_SUCCESS) {
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::menu_name] = kMenuName;
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::image][am::strings::value] = kImage;
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::choice_id] = kChoiceId1;
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::secondary_image][am::strings::value] = kSecondImage;
  (*message_)[am::strings::msg_params][am::strings::choice_set][0]
             [am::strings::vr_commands][0] = kVrCommands1;
  (*message_)[am::strings::msg_params][am::strings::choice_set][1]
             [am::strings::choice_id] = kChoiceId2;
  (*message_)[am::strings::msg_params][am::strings::choice_set][1]
             [am::strings::menu_name] = kMenuName;
  (*message_)[am::strings::msg_params][am::strings::interaction_choice_set_id] =
      kChoiceSetId;
  (*message_)[am::strings::msg_params][am::strings::choice_set][1]
             [am::strings::vr_commands][0] = kVrCommands2;

  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app_));

  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  command_->Run();
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       OnEvent_InvalidEventId_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::INVALID_ENUM);

  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _)).Times(0);
  command_->on_event(event);
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       OnEvent_InvalidVrCommand_UNSUCCESS) {
  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::WARNINGS;
  (*message_)[am::strings::params][am::strings::correlation_id] =
      kCorrelationId;

  Event event(hmi_apis::FunctionID::VR_AddCommand);
  event.set_smart_object(*message_);

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

  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app_));

  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID()).WillOnce(Return(kGrammarId));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
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
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app_));

  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID()).WillOnce(Return(kGrammarId));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
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

  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID()).WillOnce(Return(kGrammarId));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  EXPECT_CALL(app_mngr_, GetRPCService())
      .Times(3)
      .WillRepeatedly(ReturnRef(rpc_service_));
  command_->Run();

  FillMessageFieldsItem2(message_);

  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _));
  event.set_smart_object(*message_);
  command_->on_event(event);
}

TEST_F(CreateInteractionChoiceSetRequestTest,
       OnTimeOut_InvalidErrorFromHMI_UNSUCCESS) {
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app_));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::GENERIC_ERROR),
                  am::commands::Command::SOURCE_SDL));

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

  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID()).WillOnce(Return(kGrammarId));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  EXPECT_CALL(app_mngr_, GetRPCService())
      .Times(3)
      .WillRepeatedly(ReturnRef(rpc_service_));
  command_->Run();

  FillMessageFieldsItem2(message_);
  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _));
  Event event(hmi_apis::FunctionID::VR_AddCommand);
  event.set_smart_object(*message_);
  command_->on_event(event);

  EXPECT_CALL(*mock_app_, RemoveChoiceSet(kChoiceSetId));
  EXPECT_CALL(app_mngr_, GetRPCService()).Times(0);
  EXPECT_CALL(rpc_service_, ManageMobileCommand(_, _)).Times(0);
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

  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID()).WillOnce(Return(kGrammarId));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(app_mngr_, GetRPCService())
      .Times(3)
      .WillRepeatedly(ReturnRef(rpc_service_));
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

  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObject* choice_set_id = NULL;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(app_mngr_, GenerateGrammarID()).WillOnce(Return(kGrammarId));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));

  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillOnce(Return(choice_set_id));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _));

  EXPECT_CALL(app_mngr_, GetRPCService())
      .Times(6)
      .WillRepeatedly(ReturnRef(rpc_service_));
  command_->Run();

  FillMessageFieldsItem2(message_);

  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _));

  Event event(hmi_apis::FunctionID::VR_AddCommand);
  event.set_smart_object(*message_);

  command_->on_event(event);

  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _));
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
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(rpc_service_, SendMessageToMobile(message, false));
  command->Run();
}

TEST_F(CreateInteractionChoiceSetResponseTest, Run_SuccessTrue_SUCCESS) {
  MessageSharedPtr message(CreateMessage());
  (*message)[am::strings::msg_params][am::strings::success] = true;
  (*message)[am::strings::msg_params][am::strings::result_code] =
      mobile_apis::Result::SUCCESS;
  CreateInteractionChoiceSetResponsePtr command(
      CreateCommand<CreateInteractionChoiceSetResponse>(message));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(rpc_service_, SendMessageToMobile(message, false));
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

  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _))
      .WillRepeatedly(Return(mobile_apis::Result::GENERIC_ERROR));

  smart_objects::SmartObject* choice_set_id = NULL;
  EXPECT_CALL(*mock_app_, FindChoiceSet(kChoiceSetId))
      .WillRepeatedly(Return(choice_set_id));

  EXPECT_CALL(app_mngr_, GenerateGrammarID())
      .WillRepeatedly(Return(kGrammarId));
  EXPECT_CALL(*mock_app_, AddChoiceSet(kChoiceSetId, _)).Times(2);
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  EXPECT_CALL(app_mngr_, GetRPCService())
      .Times(3)
      .WillRepeatedly(ReturnRef(rpc_service_));
  command_->Run();

  FillMessageFieldsItem2(message_);
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::GENERIC_ERROR),
                  am::commands::Command::SOURCE_SDL));
  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, TerminateRequest(_, _, _));
  event.set_smart_object(*message_);
  command_->on_event(event);
  command_->Run();
}

}  // namespace create_interaction_choice_set_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests
