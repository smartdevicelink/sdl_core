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
#include <string>
#include <set>

#include "mobile/add_command_request.h"

#include "gtest/gtest.h"

#include "utils/helpers.h"

#include "smart_objects/smart_object.h"
#include "utils/custom_string.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_help_prompt_manager.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace add_command_request {

namespace am = application_manager;
namespace am_test = application_manager_test;
using am::commands::CommandImpl;
using am::ApplicationManager;
using am::commands::MessageSharedPtr;
using am::ApplicationSharedPtr;
using ::testing::_;

using ::testing::Return;
using ::testing::InSequence;
using sdl_rpc_plugin::commands::AddCommandRequest;
using NsSmartDeviceLink::NsSmartObjects::SmartObjectSPtr;
using ::test::components::application_manager_test::MockApplication;
using am::event_engine::EventObserver;
using namespace smart_objects;

namespace custom_str = utils::custom_string;
namespace strings = ::application_manager::strings;
namespace mobile_result = mobile_apis::Result;
namespace hmi_response = ::application_manager::hmi_response;
namespace hmi_request = ::application_manager::hmi_request;
using namespace strings;

namespace {
const hmi_apis::FunctionID::eType kInvalidFunctionId =
    hmi_apis::FunctionID::INVALID_ENUM;
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
const std::string kMenuName = "LG";
const uint32_t kFirstParentId = 10u;
const uint32_t kSecondParentId = 1u;
const std::string kErroredVRCommand = "l\namer";
const std::string kFirstVrCommand = "lamer";
const std::string kSecondVrCommand = "hacker";
const uint32_t kFirstCommandId = 10u;
const uint32_t kSecondCommandId = 11u;
const int32_t kType = 34;
const int32_t kGrammarId = 12;
const int32_t kPosition = 10;
}  // namespace

class AddCommandRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  AddCommandRequestTest()
      : msg_(CreateMessage())
      , default_app_name_("test_default_app_name_")
      , lock_ptr_(std::make_shared<sync_primitives::Lock>())
      , mock_help_prompt_manager_(
            std::make_shared<am_test::MockHelpPromptManager>())
      , mock_app_(CreateMockApp()) {
    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillRepeatedly(Return(mock_app_));
    InitGetters();
    InitBasicMessage();
  }

 protected:
  void InitBasicMessage() {
    (*msg_)[params][connection_key] = kConnectionKey;
    (*msg_)[msg_params][app_id] = kAppId;
    (*msg_)[msg_params][app_name] = default_app_name_;
  }

  void InitGetters() {
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));
    ON_CALL(*mock_app_, FindCommand(kCmdId))
        .WillByDefault(Return(so_ptr_.get()));
  }

  void CreateBasicParamsUIRequest() {
    SmartObject menu_params = SmartObject(SmartType_Map);
    menu_params[position] = kPosition;
    menu_params[menu_name] = kMenuName;
    SmartObject& msg_params = (*msg_)[strings::msg_params];
    msg_params[cmd_id] = kCmdId;
    msg_params[strings::menu_params] = menu_params;
    msg_params[cmd_icon] = 1;
    msg_params[cmd_icon][value] = "10";
    msg_params[info] = "UI info";
  }

  void CreateBasicParamsVRRequest() {
    SmartObject& msg_params = (*msg_)[strings::msg_params];
    msg_params[cmd_id] = kCmdId;
    msg_params[vr_commands] = SmartObject(SmartType_Array);
    msg_params[vr_commands][0] = kFirstVrCommand;
    msg_params[type] = kPosition;
    msg_params[grammar_id] = kGrammarId;
    msg_params[info] = "VR info";
  }

  const am::CommandsMap CreateCommandsMap(SmartObject& first_command,
                                          SmartObject& second_command) {
    second_command[menu_params] = SmartObject(SmartType_Map);
    second_command[menu_params][hmi_request::parent_id] = kFirstParentId;
    second_command[menu_params][menu_name] = kMenuName;
    second_command[vr_commands] = SmartObject(SmartType_Array);
    second_command[vr_commands][0] = kSecondVrCommand;
    am::CommandsMap commands_map;
    commands_map.insert(std::make_pair(kFirstCommandId, &first_command));
    commands_map.insert(std::make_pair(kSecondCommandId, &second_command));
    return commands_map;
  }

  void CheckOnTimeOutCommandDeletion(
      const hmi_apis::FunctionID::eType incoming_cmd,
      const hmi_apis::FunctionID::eType cmd_to_delete) {
    CreateBasicParamsVRRequest();
    CreateBasicParamsUIRequest();
    SmartObject& msg_params = (*msg_)[strings::msg_params];
    msg_params[menu_params][hmi_request::parent_id] = kSecondParentId;
    SmartObject& image = msg_params[cmd_icon];
    EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
        .WillOnce(Return(mobile_apis::Result::SUCCESS));
    EXPECT_CALL(*mock_app_, FindCommand(kCmdId))
        .WillOnce(Return(so_ptr_.get()));
    SmartObject first_command = SmartObject(SmartType_Map);
    SmartObject second_command = SmartObject(SmartType_Map);
    const am::CommandsMap commands_map =
        CreateCommandsMap(first_command, second_command);
    EXPECT_CALL(*mock_app_, commands_map())
        .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
            commands_map, lock_ptr_)));
    so_ptr_ = std::make_shared<SmartObject>(SmartType_Map);
    EXPECT_CALL(*mock_app_, FindSubMenu(kSecondParentId))
        .WillOnce(Return(so_ptr_.get()));
    {
      InSequence dummy;

      EXPECT_CALL(mock_rpc_service_,
                  ManageHMICommand(
                      HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
          .WillOnce(Return(true));
      EXPECT_CALL(mock_rpc_service_,
                  ManageHMICommand(
                      HMIResultCodeIs(hmi_apis::FunctionID::VR_AddCommand)))
          .WillOnce(Return(true));
    }
    EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
    std::shared_ptr<AddCommandRequest> request_ptr =
        CreateCommand<AddCommandRequest>(msg_);
    request_ptr->Run();
    Event event(incoming_cmd);
    event.set_smart_object(*msg_);
    request_ptr->on_event(event);
    EXPECT_CALL(*mock_app_, RemoveCommand(kCmdId));

    EXPECT_CALL(mock_rpc_service_,
                ManageHMICommand(HMIResultCodeIs(cmd_to_delete)))

        .WillOnce(Return(true));
    SmartObjectSPtr response = std::make_shared<SmartObject>(SmartType_Map);
    (*response)[strings::msg_params][strings::info] = "info";
    EXPECT_CALL(
        mock_message_helper_,
        CreateNegativeResponse(_, _, _, mobile_apis::Result::GENERIC_ERROR))
        .WillOnce(Return(response));

    EXPECT_CALL(
        mock_rpc_service_,
        ManageMobileCommand(response,
                            am::commands::Command::CommandSource::SOURCE_SDL));
    std::shared_ptr<CommandRequestImpl> base_class_request =
        static_cast<std::shared_ptr<CommandRequestImpl> >(request_ptr);
    base_class_request->onTimeOut();
  }

  MessageSharedPtr msg_;
  SmartObjectSPtr so_ptr_;
  const utils::custom_string::CustomString default_app_name_;
  std::shared_ptr<sync_primitives::Lock> lock_ptr_;
  std::shared_ptr<am_test::MockHelpPromptManager> mock_help_prompt_manager_;
  MockAppPtr mock_app_;
};

TEST_F(AddCommandRequestTest, Run_AppNotExisted_EXPECT_AppNotRegistered) {
  CreateBasicParamsUIRequest();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
}

TEST_F(AddCommandRequestTest, Run_ImageVerificationFailed_EXPECT_INVALID_DATA) {
  CreateBasicParamsUIRequest();
  SmartObject& msg_params = (*msg_)[strings::msg_params];
  SmartObject& image = msg_params[cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::INVALID_DATA));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::INVALID_DATA), _));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
}

TEST_F(AddCommandRequestTest, Run_ImageVerificationFailed_EXPECT_WARNINGS) {
  CreateBasicParamsUIRequest();
  SmartObject& msg_params = (*msg_)[strings::msg_params];
  SmartObject& image = msg_params[cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::WARNINGS));

  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
      .WillOnce(Return(true));
  utils::SharedPtr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
}

TEST_F(AddCommandRequestTest, Run_MenuNameHasSyntaxError_EXPECT_INVALID_DATA) {
  CreateBasicParamsUIRequest();
  SmartObject& msg_params = (*msg_)[strings::msg_params];
  msg_params[menu_params][hmi_request::parent_id] = kFirstParentId;
  const std::string errored_menu_name = "L\nG";
  msg_params[menu_params][menu_name] = errored_menu_name;
  SmartObject& image = msg_params[cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, FindCommand(kCmdId)).WillOnce(Return(so_ptr_.get()));
  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));
  SmartObject parent = SmartObject(SmartType_Map);
  EXPECT_CALL(*mock_app_, FindSubMenu(kFirstParentId))
      .WillOnce(Return(&parent));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::INVALID_DATA), _));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
}

TEST_F(AddCommandRequestTest,
       Run_VRCommandsHaveSyntaxError_EXPECT_INVALID_DATA) {
  CreateBasicParamsVRRequest();
  SmartObject& msg_params = (*msg_)[strings::msg_params];
  msg_params[vr_commands][0] = kErroredVRCommand;
  EXPECT_CALL(*mock_app_, FindCommand(kCmdId)).WillOnce(Return(so_ptr_.get()));
  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::INVALID_DATA), _));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
}

TEST_F(AddCommandRequestTest, Run_CMDIconHasError_EXPECT_INVALID_DATA) {
  MessageSharedPtr msg = CreateMessage();
  SmartObject& msg_params = (*msg)[strings::msg_params];
  (*msg)[params][connection_key] = kConnectionKey;
  msg_params[cmd_id] = kCmdId;
  msg_params[cmd_icon] = 1;
  const std::string errored_cmd_icon_value = "1\n0";
  msg_params[cmd_icon][value] = errored_cmd_icon_value;
  msg_params[vr_commands][0] = kFirstVrCommand;
  SmartObject& image = msg_params[cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, FindCommand(kCmdId)).WillOnce(Return(so_ptr_.get()));
  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::INVALID_DATA), _));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg);
  request_ptr->Run();
}

TEST_F(AddCommandRequestTest, Run_CommandIDAlreadyExists_EXPECT_INVALID_ID) {
  CreateBasicParamsUIRequest();
  SmartObject& msg_params = (*msg_)[strings::msg_params];
  SmartObject& image = msg_params[cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  so_ptr_ = std::make_shared<SmartObject>(SmartType_Map);
  EXPECT_CALL(*mock_app_, FindCommand(kCmdId)).WillOnce(Return(so_ptr_.get()));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::INVALID_ID), _));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
}

TEST_F(AddCommandRequestTest,
       Run_CommandNameAlreadyExists_EXPECT_DUPLICATE_NAME) {
  CreateBasicParamsUIRequest();
  SmartObject& msg_params = (*msg_)[strings::msg_params];
  msg_params[menu_params][hmi_request::parent_id] = kFirstParentId;
  SmartObject& image = msg_params[cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, FindCommand(kCmdId)).WillOnce(Return(so_ptr_.get()));
  SmartObject first_command = SmartObject(SmartType_Map);
  SmartObject second_command = SmartObject(SmartType_Map);
  const am::CommandsMap commands_map =
      CreateCommandsMap(first_command, second_command);
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::DUPLICATE_NAME), _));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
}

TEST_F(AddCommandRequestTest,
       Run_CmdAndMsgParentIDsAreDifferentSubmenuNotExisted_EXPECT_INVALID_ID) {
  CreateBasicParamsUIRequest();
  SmartObject& msg_params = (*msg_)[strings::msg_params];
  msg_params[menu_params][hmi_request::parent_id] = kSecondParentId;
  SmartObject& image = msg_params[cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, FindCommand(kCmdId)).WillOnce(Return(so_ptr_.get()));
  SmartObject first_command = SmartObject(SmartType_Map);
  SmartObject second_command = SmartObject(SmartType_Map);
  const am::CommandsMap commands_map =
      CreateCommandsMap(first_command, second_command);
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));
  EXPECT_CALL(*mock_app_, FindSubMenu(kSecondParentId))
      .WillOnce(Return(so_ptr_.get()));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::INVALID_ID), _));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
}

TEST_F(AddCommandRequestTest,
       Run_CmdAndMsgVrSynonymsAreTheSame_EXPECT_DUPLICATE_NAME) {
  CreateBasicParamsVRRequest();
  SmartObject& msg_params = (*msg_)[strings::msg_params];
  msg_params[menu_params][hmi_request::parent_id] = kSecondParentId;
  msg_params[vr_commands][0] = kSecondVrCommand;
  SmartObject& image = msg_params[cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, FindCommand(kCmdId)).WillOnce(Return(so_ptr_.get()));

  SmartObject first_command = SmartObject(SmartType_Map);
  SmartObject second_command = SmartObject(SmartType_Map);
  const am::CommandsMap commands_map =
      CreateCommandsMap(first_command, second_command);
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));
  so_ptr_ = std::make_shared<SmartObject>(SmartType_Map);
  EXPECT_CALL(*mock_app_, FindSubMenu(kSecondParentId))
      .WillOnce(Return(so_ptr_.get()));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::DUPLICATE_NAME), _));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
}

TEST_F(AddCommandRequestTest, Run_MsgDataEmpty_EXPECT_INVALID_DATA) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[params][connection_key] = kConnectionKey;
  SmartObject& msg_params = (*msg)[strings::msg_params];
  msg_params[app_id] = kAppId;
  msg_params[cmd_id] = kCmdId;
  EXPECT_CALL(*mock_app_, FindCommand(kCmdId)).WillOnce(Return(so_ptr_.get()));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::INVALID_DATA), _));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg);
  request_ptr->Run();
}

TEST_F(AddCommandRequestTest,
       Run_CmdAndMsg_UI_and_Vr_AreCorrect_EXPECT_VR_AND_UI_SENT) {
  CreateBasicParamsVRRequest();
  CreateBasicParamsUIRequest();
  SmartObject& msg_params = (*msg_)[strings::msg_params];
  msg_params[menu_params][hmi_request::parent_id] = kSecondParentId;
  SmartObject& image = msg_params[cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, FindCommand(kCmdId)).WillOnce(Return(so_ptr_.get()));
  SmartObject first_command = SmartObject(SmartType_Map);
  SmartObject second_command = SmartObject(SmartType_Map);
  const am::CommandsMap commands_map =
      CreateCommandsMap(first_command, second_command);
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));
  so_ptr_ = std::make_shared<SmartObject>(SmartType_Map);
  EXPECT_CALL(*mock_app_, FindSubMenu(kSecondParentId))
      .WillOnce(Return(so_ptr_.get()));
  {
    InSequence dummy;

    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
        .WillOnce(Return(true));
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::VR_AddCommand)))
        .WillOnce(Return(true));
  }
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
}

TEST_F(AddCommandRequestTest, GetRunMethods_SUCCESS) {
  CreateBasicParamsUIRequest();
  SmartObject& image = (*msg_)[msg_params][cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, FindCommand(kCmdId)).WillOnce(Return(so_ptr_.get()));
  EXPECT_CALL(*mock_app_, AddCommand(kCmdId, (*msg_)[msg_params]));

  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
      .WillOnce(Return(true));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
}

TEST_F(AddCommandRequestTest, OnEvent_UI_SUCCESS) {
  CreateBasicParamsUIRequest();
  (*msg_)[params][hmi_response::code] = hmi_apis::Common_Result::SUCCESS;
  SmartObject& image = (*msg_)[msg_params][cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, AddCommand(kCmdId, (*msg_)[msg_params]));

  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(
          Return(DataAccessor<am::CommandsMap>(commands_map, lock_ptr_)));

  Event event(hmi_apis::FunctionID::UI_AddCommand);
  event.set_smart_object(*msg_);

  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
      .WillOnce(Return(true));

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnVrCommandAdded(kCmdId, (*msg_)[msg_params], false));

  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
  request_ptr->on_event(event);
}

TEST_F(AddCommandRequestTest, OnEvent_VR_SUCCESS) {
  CreateBasicParamsVRRequest();
  MessageSharedPtr msg = CreateMessage(SmartType_Map);
  (*msg)[params][hmi_response::code] = hmi_apis::Common_Result::SUCCESS;
  (*msg)[msg_params][cmd_id] = kCmdId;

  Event event(hmi_apis::FunctionID::VR_AddCommand);
  event.set_smart_object(*msg);
  EXPECT_CALL(*mock_app_, AddCommand(kCmdId, (*msg_)[msg_params]));

  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(
          Return(DataAccessor<am::CommandsMap>(commands_map, lock_ptr_)));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::VR_AddCommand)))
      .WillOnce(Return(true));

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnVrCommandAdded(kCmdId, (*msg_)[msg_params], false));

  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
  request_ptr->on_event(event);
}

TEST_F(AddCommandRequestTest, OnTimeOut_EXPECT_VR_DeleteCommand) {
  CheckOnTimeOutCommandDeletion(hmi_apis::FunctionID::VR_AddCommand,
                                hmi_apis::FunctionID::VR_DeleteCommand);
}

TEST_F(AddCommandRequestTest, OnTimeOut_EXPECT_UI_DeleteCommand) {
  CheckOnTimeOutCommandDeletion(hmi_apis::FunctionID::UI_AddCommand,
                                hmi_apis::FunctionID::UI_DeleteCommand);
}

TEST_F(AddCommandRequestTest, OnEvent_BothSend_SUCCESS) {
  MessageSharedPtr command_msg = CreateMessage(SmartType_Map);
  (*command_msg)[params][connection_key] = kConnectionKey;
  MessageSharedPtr event_msg = CreateMessage(SmartType_Map);
  (*event_msg)[params][hmi_response::code] = hmi_apis::Common_Result::SUCCESS;
  (*event_msg)[msg_params][cmd_id] = kCmdId;

  Event event_ui(hmi_apis::FunctionID::UI_AddCommand);
  event_ui.set_smart_object(*event_msg);

  Event event_vr(hmi_apis::FunctionID::VR_AddCommand);
  event_vr.set_smart_object(*event_msg);

  EXPECT_CALL(*mock_app_, RemoveCommand(kCmdId)).Times(0);

  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(command_msg);
  request_ptr->Run();
  request_ptr->on_event(event_ui);
  request_ptr->on_event(event_vr);
}

TEST_F(AddCommandRequestTest, OnEvent_UnknownEvent_UNSUCCESS) {
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  request_ptr->on_event(event);
}

TEST_F(AddCommandRequestTest, OnEvent_AppNotExisted_UNSUCCESS) {
  CreateBasicParamsUIRequest();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(ApplicationSharedPtr()));
  Event event(hmi_apis::FunctionID::UI_AddCommand);

  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->on_event(event);
}

TEST_F(AddCommandRequestTest,
       OnEvent_HmiResponseCodeIsRejected_ExpectUICommandRemoved) {
  CreateBasicParamsUIRequest();
  SmartObject& params = (*msg_)[strings::params];
  params[hmi_response::code] = hmi_apis::Common_Result::REJECTED;
  SmartObject& image = (*msg_)[msg_params][cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
      .WillOnce(Return(true));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
  EXPECT_CALL(*mock_app_, RemoveCommand(kCmdId));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::REJECTED), _));
  Event event(hmi_apis::FunctionID::UI_AddCommand);
  event.set_smart_object(*msg_);
  request_ptr->on_event(event);
}

TEST_F(AddCommandRequestTest,
       OnEvent_HmiResponseCodeIsWarnings_ExpectCommandUpdated) {
  CreateBasicParamsVRRequest();
  CreateBasicParamsUIRequest();
  SmartObject& params = (*msg_)[strings::params];
  params[hmi_response::code] = hmi_apis::Common_Result::WARNINGS;
  SmartObject& image = (*msg_)[msg_params][cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));
  {
    InSequence dummy;

    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
        .WillOnce(Return(true));
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::VR_AddCommand)))
        .WillOnce(Return(true));
  }
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::WARNINGS), _));

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_));
  EXPECT_CALL(*mock_help_prompt_manager_, OnVrCommandAdded(kCmdId, _, false));

  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
  Event event_ui(hmi_apis::FunctionID::UI_AddCommand);
  event_ui.set_smart_object(*msg_);
  Event event_vr(hmi_apis::FunctionID::VR_AddCommand);
  event_vr.set_smart_object(*msg_);
  request_ptr->on_event(event_ui);
  request_ptr->on_event(event_vr);
}

TEST_F(
    AddCommandRequestTest,
    OnEvent_UI_HmiResponseCodeIsGenericError_VR_HmiResponseCodeIsUnsupportedResourse_ExpectCommandRemoved) {
  CreateBasicParamsVRRequest();
  CreateBasicParamsUIRequest();
  SmartObject& params = (*msg_)[strings::params];
  params[hmi_response::code] = hmi_apis::Common_Result::GENERIC_ERROR;
  SmartObject& image = (*msg_)[msg_params][cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));
  {
    InSequence dummy;
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
        .WillOnce(Return(true));
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::VR_AddCommand)))
        .WillOnce(Return(true));
  }

  EXPECT_CALL(*mock_app_, RemoveCommand(kCmdId));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();

  Event event_ui(hmi_apis::FunctionID::UI_AddCommand);
  event_ui.set_smart_object(*msg_);
  request_ptr->on_event(event_ui);

  Event event_vr(hmi_apis::FunctionID::VR_AddCommand);
  MessageSharedPtr msg_vr = CreateMessage(SmartType_Map);
  (*msg_vr)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg_vr)[msg_params][cmd_id] = kCmdId;
  event_vr.set_smart_object(*msg_vr);
  request_ptr->on_event(event_vr);
}

TEST_F(
    AddCommandRequestTest,
    OnEvent_VR_HmiResponseCodeIsGenericError_UI_HmiResponseCodeIsUnsupportedResourse_ExpectCommandRemoved) {
  CreateBasicParamsVRRequest();
  CreateBasicParamsUIRequest();
  SmartObject& params = (*msg_)[strings::params];
  params[hmi_response::code] = hmi_apis::Common_Result::GENERIC_ERROR;
  SmartObject& image = (*msg_)[msg_params][cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));
  {
    InSequence dummy;

    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
        .WillOnce(Return(true));
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::VR_AddCommand)))
        .WillOnce(Return(true));
  }

  EXPECT_CALL(*mock_app_, RemoveCommand(kCmdId));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();

  Event event_vr(hmi_apis::FunctionID::VR_AddCommand);
  event_vr.set_smart_object(*msg_);
  request_ptr->on_event(event_vr);

  Event event_ui(hmi_apis::FunctionID::UI_AddCommand);
  MessageSharedPtr msg_ui = CreateMessage(SmartType_Map);
  (*msg_ui)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg_ui)[msg_params][cmd_id] = kCmdId;
  event_ui.set_smart_object(*msg_ui);
  request_ptr->on_event(event_ui);
}

TEST_F(
    AddCommandRequestTest,
    OnEvent_UI_VR_HmiResponseCodeIsUnsupportedResourse_UI_NotAvailableInterfaceState_ExpectCommandRemoved) {
  CreateBasicParamsVRRequest();
  CreateBasicParamsUIRequest();
  SmartObject& params = (*msg_)[strings::params];
  params[hmi_response::code] = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  SmartObject& image = (*msg_)[msg_params][cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));
  {
    InSequence dummy;

    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
        .WillOnce(Return(true));
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::VR_AddCommand)))
        .WillOnce(Return(true));
  }
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();

  EXPECT_CALL(mock_hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillRepeatedly(
          Return(am::HmiInterfaces::InterfaceState::STATE_NOT_AVAILABLE));
  EXPECT_CALL(mock_hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VR))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_AVAILABLE));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_apis::Result::UNSUPPORTED_RESOURCE), _));
  EXPECT_CALL(*mock_app_, RemoveCommand(kCmdId));
  Event event_ui(hmi_apis::FunctionID::UI_AddCommand);
  event_ui.set_smart_object(*msg_);
  Event event_vr(hmi_apis::FunctionID::VR_AddCommand);
  event_vr.set_smart_object(*msg_);
  request_ptr->on_event(event_ui);
  request_ptr->on_event(event_vr);
}

TEST_F(
    AddCommandRequestTest,
    OnEvent_UI_VR_HmiResponseCodeIsUnsupportedResourse_VR_NotAvailableInterfaceState_ExpectCommandRemoved) {
  CreateBasicParamsVRRequest();
  CreateBasicParamsUIRequest();
  SmartObject& params = (*msg_)[strings::params];
  params[hmi_response::code] = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  SmartObject& image = (*msg_)[msg_params][cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));
  {
    InSequence dummy;
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
        .WillOnce(Return(true));
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::VR_AddCommand)))
        .WillOnce(Return(true));
  }
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();

  EXPECT_CALL(mock_hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_AVAILABLE));
  EXPECT_CALL(mock_hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VR))
      .WillRepeatedly(
          Return(am::HmiInterfaces::InterfaceState::STATE_NOT_AVAILABLE));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_apis::Result::UNSUPPORTED_RESOURCE), _));
  EXPECT_CALL(*mock_app_, RemoveCommand(kCmdId));
  Event event_ui(hmi_apis::FunctionID::UI_AddCommand);
  event_ui.set_smart_object(*msg_);
  Event event_vr(hmi_apis::FunctionID::VR_AddCommand);
  event_vr.set_smart_object(*msg_);
  request_ptr->on_event(event_ui);
  request_ptr->on_event(event_vr);
}

TEST_F(
    AddCommandRequestTest,
    OnEvent_UI_HmiResponseCodeIsUnsupportedResource_NotAvailableInterfaceState_ExpectCommandRemoved) {
  CreateBasicParamsUIRequest();
  SmartObject& params = (*msg_)[strings::params];
  params[hmi_response::code] = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  SmartObject& image = (*msg_)[msg_params][cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
      .WillOnce(Return(true));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
  EXPECT_CALL(mock_hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillRepeatedly(
          Return(am::HmiInterfaces::InterfaceState::STATE_NOT_AVAILABLE));
  EXPECT_CALL(mock_hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VR))
      .WillRepeatedly(
          Return(am::HmiInterfaces::InterfaceState::STATE_NOT_AVAILABLE));
  EXPECT_CALL(*mock_app_, RemoveCommand(kCmdId));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_apis::Result::UNSUPPORTED_RESOURCE), _));
  Event event(hmi_apis::FunctionID::UI_AddCommand);
  event.set_smart_object(*msg_);
  request_ptr->on_event(event);
}

TEST_F(
    AddCommandRequestTest,
    OnEvent_VR_HmiResponseCodeIsUnsupportedResource_NotAvailableInterfaceState_ExpectCommandRemoved) {
  CreateBasicParamsVRRequest();
  SmartObject& params = (*msg_)[strings::params];
  params[hmi_response::code] = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::VR_AddCommand)))
      .WillOnce(Return(true));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
  EXPECT_CALL(mock_hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillRepeatedly(
          Return(am::HmiInterfaces::InterfaceState::STATE_NOT_AVAILABLE));
  EXPECT_CALL(mock_hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VR))
      .WillRepeatedly(
          Return(am::HmiInterfaces::InterfaceState::STATE_NOT_AVAILABLE));
  EXPECT_CALL(*mock_app_, RemoveCommand(kCmdId));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_apis::Result::UNSUPPORTED_RESOURCE), _));
  Event event(hmi_apis::FunctionID::VR_AddCommand);
  event.set_smart_object(*msg_);
  request_ptr->on_event(event);
}

TEST_F(AddCommandRequestTest,
       OnEvent_UI_EventWithNotSuccesResponseCode_ExpectVRCommandDelete) {
  CreateBasicParamsVRRequest();
  CreateBasicParamsUIRequest();
  SmartObject& params = (*msg_)[strings::params];
  params[hmi_response::code] = hmi_apis::Common_Result::SUCCESS;
  SmartObject& image = (*msg_)[msg_params][cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));
  {
    InSequence dummy;
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
        .WillOnce(Return(true));
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::VR_AddCommand)))
        .WillOnce(Return(true));
  }
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::GENERIC_ERROR), _));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();

  MessageSharedPtr msg_ui = CreateMessage(SmartType_Map);
  (*msg_ui)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::ABORTED;
  (*msg_ui)[msg_params][cmd_id] = kCmdId;
  Event event_ui(hmi_apis::FunctionID::UI_AddCommand);
  event_ui.set_smart_object(*msg_ui);
  Event event_vr(hmi_apis::FunctionID::VR_AddCommand);
  event_vr.set_smart_object(*msg_);
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::VR_DeleteCommand)))
      .WillOnce(Return(true));
  EXPECT_CALL(*mock_app_, RemoveCommand(kCmdId)).Times(2);
  request_ptr->on_event(event_ui);
  request_ptr->on_event(event_vr);
}

TEST_F(AddCommandRequestTest,
       OnEvent_UI_VR_Events_VRErrorPresent_ExpectRemoveCommand) {
  CreateBasicParamsVRRequest();
  CreateBasicParamsUIRequest();
  SmartObject& params = (*msg_)[strings::params];
  params[hmi_response::code] = hmi_apis::Common_Result::SUCCESS;
  SmartObject& image = (*msg_)[msg_params][cmd_icon];
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  am::CommandsMap commands_map;
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));
  {
    InSequence dummy;
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
        .WillOnce(Return(true));
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::VR_AddCommand)))
        .WillOnce(Return(true));
  }
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::GENERIC_ERROR), _));
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();

  Event event_ui(hmi_apis::FunctionID::UI_AddCommand);
  event_ui.set_smart_object(*msg_);
  request_ptr->on_event(event_ui);
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_DeleteCommand)))
      .WillOnce(Return(true));
  EXPECT_CALL(*mock_app_, RemoveCommand(kCmdId)).Times(2);
  Event event_vr(hmi_apis::FunctionID::VR_AddCommand);
  MessageSharedPtr msg_vr = CreateMessage(SmartType_Map);
  (*msg_vr)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::ABORTED;
  (*msg_vr)[msg_params][cmd_id] = kCmdId;
  event_vr.set_smart_object(*msg_vr);
  request_ptr->on_event(event_vr);
}

TEST_F(AddCommandRequestTest,
       OnTimeOut_AppNotExisted_NoAppRemoveCommandCalled) {
  CreateBasicParamsUIRequest();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(ApplicationSharedPtr()));
  EXPECT_CALL(*mock_app_, RemoveCommand(kCmdId)).Times(0);
  SmartObjectSPtr response = std::make_shared<SmartObject>(SmartType_Map);
  (*response)[strings::msg_params][strings::info] = "info";
  EXPECT_CALL(
      mock_message_helper_,
      CreateNegativeResponse(_, _, _, mobile_apis::Result::GENERIC_ERROR))
      .WillOnce(Return(response));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  response, am::commands::Command::CommandSource::SOURCE_SDL));
  std::shared_ptr<CommandRequestImpl> base_class_request =
      static_cast<std::shared_ptr<CommandRequestImpl> >(
          CreateCommand<AddCommandRequest>(msg_));
  base_class_request->onTimeOut();
}

TEST_F(AddCommandRequestTest, OnTimeOut_AppRemoveCommandCalled) {
  CreateBasicParamsVRRequest();
  CreateBasicParamsUIRequest();
  SmartObject& msg_params = (*msg_)[strings::msg_params];
  SmartObject& image = msg_params[cmd_icon];
  msg_params[menu_params][hmi_request::parent_id] = kSecondParentId;
  EXPECT_CALL(mock_message_helper_, VerifyImage(image, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, FindCommand(kCmdId)).WillOnce(Return(so_ptr_.get()));
  SmartObject first_command = SmartObject(SmartType_Map);
  SmartObject second_command = SmartObject(SmartType_Map);
  const am::CommandsMap commands_map =
      CreateCommandsMap(first_command, second_command);
  EXPECT_CALL(*mock_app_, commands_map())
      .WillRepeatedly(Return(DataAccessor<application_manager::CommandsMap>(
          commands_map, lock_ptr_)));
  so_ptr_ = std::make_shared<SmartObject>(SmartType_Map);
  EXPECT_CALL(*mock_app_, FindSubMenu(kSecondParentId))
      .WillOnce(Return(so_ptr_.get()));
  {
    InSequence dummy;
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_AddCommand)))
        .WillOnce(Return(true));
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::VR_AddCommand)))
        .WillOnce(Return(true));
  }
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  std::shared_ptr<AddCommandRequest> request_ptr =
      CreateCommand<AddCommandRequest>(msg_);
  request_ptr->Run();
  EXPECT_CALL(*mock_app_, RemoveCommand(kCmdId));
  SmartObjectSPtr response = std::make_shared<SmartObject>(SmartType_Map);
  (*response)[strings::msg_params][strings::info] = "info";
  EXPECT_CALL(
      mock_message_helper_,
      CreateNegativeResponse(_, _, _, mobile_apis::Result::GENERIC_ERROR))
      .WillOnce(Return(response));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  response, am::commands::Command::CommandSource::SOURCE_SDL));
  std::shared_ptr<CommandRequestImpl> base_class_request =
      static_cast<std::shared_ptr<CommandRequestImpl> >(request_ptr);
  base_class_request->onTimeOut();
}

}  // namespace add_command_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
