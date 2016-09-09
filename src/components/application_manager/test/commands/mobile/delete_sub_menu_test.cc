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
#include <map>

#include "mobile/delete_sub_menu_request.h"
#include "mobile/delete_sub_menu_response.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/event_engine/event.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {

using ::testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::InSequence;

namespace am = ::application_manager;

using am::commands::DeleteSubMenuRequest;
using am::commands::DeleteSubMenuResponse;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;

typedef SharedPtr<DeleteSubMenuRequest> DeleteSubMenuRequestPtr;
typedef SharedPtr<DeleteSubMenuResponse> DeleteSubMenuResponsePtr;

MATCHER_P(CheckMessageResultCode, result_code, "") {
  return (*arg)[am::strings::msg_params][am::strings::result_code].asInt() ==
         result_code;
}

MATCHER_P(CheckMessageConnectionKey, connection_key, "") {
  return (*arg)[am::strings::msg_params][am::strings::connection_key].asInt() ==
         connection_key;
}

ACTION_P(DeleteCommand, commands_map) {
  am::CommandsMap::iterator it = (*commands_map).begin();
  if ((*commands_map).end() != it) {
    (*commands_map).erase(it);
  }
}

namespace {
const uint32_t kConnectionKey = 2u;
const uint32_t kCorrelationId = 10u;
const uint32_t kMenuId = 100u;
const uint32_t kGrammarId = 101u;
const int32_t kCmdId = 102;
}  // namespace

class DeleteSubMenuRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  DeleteSubMenuRequestTest() : accessor_(commands_map_, commands_lock_) {}

  am::CommandsMap commands_map_;
  mutable sync_primitives::Lock commands_lock_;
  DataAccessor<am::CommandsMap> accessor_;

 protected:
  void SetUp() OVERRIDE {
    message_ = CreateMessage();
    command_ = CreateCommand<DeleteSubMenuRequest>(message_);
    app_ = CreateMockApp();
  }
  DeleteSubMenuRequestPtr command_;
  MessageSharedPtr message_;
  MockAppPtr app_;
};

class DeleteSubMenuResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(DeleteSubMenuRequestTest, Run_InvalidApp_UNSUCCESS) {
  MockAppPtr invalid_app;
  EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(invalid_app));
  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(CheckMessageResultCode(
                              mobile_apis::Result::APPLICATION_NOT_REGISTERED),
                          am::commands::Command::CommandOrigin::ORIGIN_SDL));
  EXPECT_CALL(*app_, FindSubMenu(_)).Times(0);
  command_->Run();
}

TEST_F(DeleteSubMenuRequestTest, Run_FindSubMenuFalse_UNSUCCESS) {
  (*message_)[am::strings::msg_params][am::strings::menu_id] = kMenuId;
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  smart_objects::SmartObject* invalid_sub_menu = NULL;
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(app_));
  EXPECT_CALL(*app_, FindSubMenu(kMenuId)).WillOnce(Return(invalid_sub_menu));

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  CheckMessageResultCode(mobile_apis::Result::INVALID_ID),
                  am::commands::Command::CommandOrigin::ORIGIN_SDL));
  EXPECT_CALL(*app_, app_id()).Times(0);
  command_->Run();
}

TEST_F(DeleteSubMenuRequestTest, Run_SendHMIRequest_SUCCESS) {
  (*message_)[am::strings::msg_params][am::strings::menu_id] = kMenuId;
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  smart_objects::SmartObject* sub_menu =
      &((*message_)[am::strings::msg_params][am::strings::menu_id]);
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(app_));
  EXPECT_CALL(*app_, FindSubMenu(kMenuId)).WillOnce(Return(sub_menu));

  EXPECT_CALL(*app_, app_id()).WillOnce(Return(kConnectionKey));
  EXPECT_CALL(mock_app_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId));

  command_->Run();
}

TEST_F(DeleteSubMenuRequestTest, OnEvent_UnknownEventId_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  EXPECT_CALL(mock_app_manager_, application(_)).Times(0);
  command_->on_event(event);
}

TEST_F(DeleteSubMenuRequestTest, OnEvent_InvalidApp_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::UI_DeleteSubMenu);
  (*message_)[am::strings::params][am::hmi_response::code] =
      am::mobile_api::Result::SUCCESS;
  event.set_smart_object(*message_);

  MockAppPtr invalid_app;
  EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(invalid_app));
  EXPECT_CALL(*app_, RemoveSubMenu(_)).Times(0);
  command_->on_event(event);
}

TEST_F(DeleteSubMenuRequestTest, OnEvent_DeleteSubmenu_SUCCESS) {
  Event event(hmi_apis::FunctionID::UI_DeleteSubMenu);
  (*message_)[am::strings::msg_params][am::strings::menu_id] = kMenuId;
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*message_)[am::strings::msg_params][am::strings::vr_commands] =
      "vr_commands";
  (*message_)[am::strings::msg_params][am::strings::cmd_id] = kCmdId;
  (*message_)[am::strings::msg_params][am::strings::menu_params]
             [am::hmi_request::parent_id] = kMenuId;
  (*message_)[am::strings::params][am::hmi_response::code] =
      am::mobile_api::Result::SUCCESS;
  event.set_smart_object(*message_);

  commands_map_.insert(
      std::make_pair(0, &((*message_)[am::strings::msg_params])));

  InSequence seq;
  EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(app_));
  EXPECT_CALL(*app_, commands_map()).WillOnce(Return(accessor_));
  EXPECT_CALL(*app_, app_id()).WillOnce(Return(kConnectionKey));
  EXPECT_CALL(*app_, get_grammar_id()).WillOnce(Return(kGrammarId));

  EXPECT_CALL(*app_, commands_map()).WillOnce(Return(accessor_));
  EXPECT_CALL(*app_, app_id()).WillOnce(Return(kConnectionKey));
  EXPECT_CALL(*app_, RemoveCommand(_)).WillOnce(DeleteCommand(&commands_map_));

  EXPECT_CALL(*app_, RemoveSubMenu(_));
  EXPECT_CALL(*app_, UpdateHash());
  command_->on_event(event);
  EXPECT_TRUE(Mock::VerifyAndClearExpectations(app_.get()));
}

TEST_F(DeleteSubMenuResponseTest, Run_SUCCESS) {
  MessageSharedPtr message(CreateMessage());
  (*message)[am::strings::msg_params][am::strings::connection_key] =
      kConnectionKey;
  DeleteSubMenuResponsePtr command(
      CreateCommand<DeleteSubMenuResponse>(message));
  EXPECT_CALL(
      mock_app_manager_,
      SendMessageToMobile(CheckMessageConnectionKey(kConnectionKey), _));
  command->Run();
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
