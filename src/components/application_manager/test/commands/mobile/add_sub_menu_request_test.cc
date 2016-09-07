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
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "mobile/add_sub_menu_request.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/policies/policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
namespace am = ::application_manager;
using am::commands::AddSubMenuRequest;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;

typedef SharedPtr<AddSubMenuRequest> AddSubMenuPtr;

namespace {
const uint32_t kConnectionKey = 2u;
const int32_t kMenuId = 5;
}  // namespace

class AddSubMenuRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {};

TEST_F(AddSubMenuRequestTest, OnEvent_UnknownEvent_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>());

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);

  command->on_event(event);
}

TEST_F(AddSubMenuRequestTest, OnEvent_SUCCESS) {
  Event event(hmi_apis::FunctionID::UI_AddSubMenu);
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      mobile_apis::Result::SUCCESS;
  (*event_msg)[am::strings::msg_params] = 0;

  event.set_smart_object(*event_msg);

  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg));

  MockAppPtr app(CreateMockApp());
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(app));

  MessageSharedPtr result_msg(
      CatchMobileCommandResult(CallOnEvent(*command, event)));
  const mobile_apis::Result::eType kReceivedResult =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::SUCCESS, kReceivedResult);
}

TEST_F(AddSubMenuRequestTest, Run_ApplicationIsNotRegistered_UNSUCCESS) {
  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>());

  EXPECT_CALL(mock_app_manager_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType kReceivedResult =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED, kReceivedResult);
}

TEST_F(AddSubMenuRequestTest, Run_InvalidSubMenuId_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::menu_id] = kMenuId;
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg));

  MockAppPtr app(CreateMockApp());
  MessageSharedPtr dummy_sub_menu(CreateMessage(smart_objects::SmartType_Null));
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(app));
  EXPECT_CALL(*app, FindSubMenu(kMenuId))
      .WillOnce(Return(dummy_sub_menu.get()));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType kReceivedResult =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::INVALID_ID, kReceivedResult);
}

TEST_F(AddSubMenuRequestTest, Run_DuplicatedSubMenuName_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::menu_id] = kMenuId;
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg));

  MockAppPtr app(CreateMockApp());

  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(app));
  EXPECT_CALL(*app, FindSubMenu(kMenuId))
      .WillOnce(Return(static_cast<SmartObject*>(NULL)));
  EXPECT_CALL(*app, IsSubMenuNameAlreadyExist(_)).WillOnce(Return(true));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType kReceivedResult =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::DUPLICATE_NAME, kReceivedResult);
}

TEST_F(AddSubMenuRequestTest, Run_NotValidSubMenuName_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::menu_id] = kMenuId;
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  // Not valid sub-menu name.
  (*command_msg)[am::strings::msg_params][am::strings::menu_name] = "\t\n";

  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg));

  MockAppPtr app(CreateMockApp());
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(app));
  EXPECT_CALL(*app, FindSubMenu(kMenuId))
      .WillOnce(Return(static_cast<SmartObject*>(NULL)));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType kReceivedResult =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA, kReceivedResult);
}

TEST_F(AddSubMenuRequestTest, Run_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::menu_id] = kMenuId;
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][am::strings::menu_name] =
      "valid_sub_menu_name";
  (*command_msg)[am::strings::msg_params][am::strings::position] =
      "test_position";

  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg));

  MockAppPtr app(CreateMockApp());
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(app));
  EXPECT_CALL(*app, FindSubMenu(kMenuId))
      .WillOnce(Return(static_cast<SmartObject*>(NULL)));

  MessageSharedPtr result_msg(CatchHMICommandResult(CallRun(*command)));
  const hmi_apis::FunctionID::eType kReceivedResult =
      static_cast<hmi_apis::FunctionID::eType>(
          (*result_msg)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(hmi_apis::FunctionID::UI_AddSubMenu, kReceivedResult);
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
