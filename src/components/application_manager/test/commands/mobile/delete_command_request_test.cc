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
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "commands/commands_test.h"
#include "commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/event_engine/event.h"
#include "mobile/delete_command_request.h"
#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
namespace am = ::application_manager;
using am::commands::DeleteCommandRequest;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;

typedef SharedPtr<DeleteCommandRequest> DeleteCommandPtr;

namespace {
const uint32_t kConnectionKey = 2u;
const int32_t kCommandId = 5;
}  // namespace

class DeleteCommandRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {};

TEST_F(DeleteCommandRequestTest, Run_ApplicationIsNotRegistered_UNSUCCESS) {
  DeleteCommandPtr command(CreateCommand<DeleteCommandRequest>());

  EXPECT_CALL(mock_app_manager_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));

  ASSERT_TRUE(result_msg);

  const mobile_apis::Result::eType kReceivedResult =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED, kReceivedResult);
}

TEST_F(DeleteCommandRequestTest, Run_InvalidCommandId_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::cmd_id] = kCommandId;
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  DeleteCommandPtr command(CreateCommand<DeleteCommandRequest>(command_msg));

  MockAppPtr app = CreateMockApp();
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(app));
  EXPECT_CALL(*app, FindCommand(kCommandId))
      .WillOnce(Return(static_cast<SmartObject*>(NULL)));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));

  ASSERT_TRUE(result_msg);

  const mobile_apis::Result::eType kReceivedResult =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::INVALID_ID, kReceivedResult);
}

TEST_F(DeleteCommandRequestTest, Run_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::cmd_id] = kCommandId;
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  DeleteCommandPtr command(CreateCommand<DeleteCommandRequest>(command_msg));

  MockAppPtr app = CreateMockApp();
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(app));

  MessageSharedPtr test_msg(CreateMessage(smart_objects::SmartType_Map));
  (*test_msg)[am::strings::menu_params] = 0;
  (*test_msg)[am::strings::vr_commands] = 0;

  EXPECT_CALL(*app, FindCommand(kCommandId)).WillOnce(Return(test_msg.get()));

  MessageSharedPtr menu_params_result;
  MessageSharedPtr vr_command_result;
  {
    ::testing::InSequence sequence;
    EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&menu_params_result), Return(true)));
    EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&vr_command_result), Return(true)));
  }

  command->Run();

  ASSERT_TRUE(menu_params_result);
  ASSERT_TRUE(vr_command_result);

  const hmi_apis::FunctionID::eType kMenuParamsResult =
      static_cast<hmi_apis::FunctionID::eType>(
          (*menu_params_result)[am::strings::params][am::strings::function_id]
              .asInt());
  const hmi_apis::FunctionID::eType kVrCommandResult =
      static_cast<hmi_apis::FunctionID::eType>(
          (*vr_command_result)[am::strings::params][am::strings::function_id]
              .asInt());

  EXPECT_EQ(hmi_apis::FunctionID::UI_DeleteCommand, kMenuParamsResult);
  EXPECT_EQ(hmi_apis::FunctionID::VR_DeleteCommand, kVrCommandResult);
}

TEST_F(DeleteCommandRequestTest, OnEvent_UnknownEvent_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::INVALID_ENUM);

  DeleteCommandPtr command(CreateCommand<DeleteCommandRequest>());

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);

  command->on_event(event);
}

TEST_F(DeleteCommandRequestTest, OnEvent_UiDeleteCommand_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::cmd_id] = kCommandId;
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  DeleteCommandPtr command(CreateCommand<DeleteCommandRequest>(command_msg));

  MockAppPtr app = CreateMockApp();
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillRepeatedly(Return(app));

  MessageSharedPtr test_msg(CreateMessage(smart_objects::SmartType_Map));
  (*test_msg)[am::strings::menu_params] = 0;

  EXPECT_CALL(*app, FindCommand(kCommandId))
      .WillRepeatedly(Return(test_msg.get()));

  command->Run();

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::WARNINGS;
  Event event(hmi_apis::FunctionID::UI_DeleteCommand);
  event.set_smart_object(*event_msg);

  EXPECT_CALL(*app, RemoveCommand(kCommandId));

  EXPECT_CALL(*app, UpdateHash());

  MessageSharedPtr result_msg(
      CatchMobileCommandResult(CallOnEvent(*command, event)));

  ASSERT_TRUE(result_msg);

  const mobile_apis::Result::eType kReceivedResult =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());

  EXPECT_EQ(mobile_apis::Result::WARNINGS, kReceivedResult);
}

TEST_F(DeleteCommandRequestTest, OnEvent_VrDeleteCommand_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::cmd_id] = kCommandId;
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  DeleteCommandPtr command(CreateCommand<DeleteCommandRequest>(command_msg));

  MockAppPtr app = CreateMockApp();
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillRepeatedly(Return(app));

  MessageSharedPtr test_msg(CreateMessage(smart_objects::SmartType_Map));
  (*test_msg)[am::strings::vr_commands] = 0;

  EXPECT_CALL(*app, FindCommand(kCommandId))
      .WillRepeatedly(Return(test_msg.get()));

  command->Run();

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::WARNINGS;
  Event event(hmi_apis::FunctionID::VR_DeleteCommand);
  event.set_smart_object(*event_msg);

  EXPECT_CALL(*app, RemoveCommand(kCommandId));

  EXPECT_CALL(*app, UpdateHash());

  MessageSharedPtr result_msg(
      CatchMobileCommandResult(CallOnEvent(*command, event)));

  ASSERT_TRUE(result_msg);

  const mobile_apis::Result::eType kReceivedResult =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::WARNINGS, kReceivedResult);
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
