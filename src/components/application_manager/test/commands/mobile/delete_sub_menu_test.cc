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

#include "application_manager/commands/mobile/delete_sub_menu_request.h"
#include "application_manager/commands/mobile/delete_sub_menu_response.h"

#include "gtest/gtest.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace delete_sub_menu_request {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::InSequence;
namespace am = ::application_manager;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
using am::MockHmiInterfaces;
using am::MockMessageHelper;

using am::commands::DeleteSubMenuRequest;
using am::commands::DeleteSubMenuResponse;

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
  DeleteSubMenuRequestTest()
      : commands_lock_(std::make_shared<sync_primitives::Lock>())
      , accessor_(commands_map_, commands_lock_)
      , message_(CreateMessage())
      , command_(CreateCommand<DeleteSubMenuRequest>(message_))
      , app_(CreateMockApp()) {}

  am::CommandsMap commands_map_;
  mutable std::shared_ptr<sync_primitives::Lock> commands_lock_;
  DataAccessor<am::CommandsMap> accessor_;

  MessageSharedPtr message_;
  DeleteSubMenuRequestPtr command_;
  MockAppPtr app_;
};

class DeleteSubMenuResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(DeleteSubMenuRequestTest, DISABLED_OnEvent_UI_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg)[am::strings::msg_params][am::strings::menu_id] = 10u;

  utils::SharedPtr<DeleteSubMenuRequest> command =
      CreateCommand<DeleteSubMenuRequest>(msg);

  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));
  ON_CALL(*mock_app, app_id()).WillByDefault(Return(kConnectionKey));
  EXPECT_CALL(*mock_app, RemoveSubMenu(_));

  MessageSharedPtr ev_msg = CreateMessage(smart_objects::SmartType_Map);
  (*ev_msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*ev_msg)[am::strings::msg_params][am::strings::info] = "info";

  Event event(hmi_apis::FunctionID::UI_DeleteSubMenu);
  event.set_smart_object(*ev_msg);

  MockHmiInterfaces hmi_interfaces;
  ON_CALL(app_mngr_, hmi_interfaces()).WillByDefault(ReturnRef(hmi_interfaces));
  EXPECT_CALL(hmi_interfaces,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillOnce(Return(am::HmiInterfaces::STATE_AVAILABLE));

  am::CommandsMap commands_map;
  smart_objects::SmartObject commands_msg(smart_objects::SmartType_Map);
  commands_map.insert(std::pair<uint32_t, SmartObject*>(1u, &commands_msg));
  DataAccessor<am::CommandsMap> accessor(
      commands_map, std::make_shared<sync_primitives::Lock>());
  EXPECT_CALL(*mock_app, commands_map())
      .WillOnce(Return(accessor))
      .WillOnce(Return(accessor));

  MessageSharedPtr ui_command_result;
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));

  command->on_event(event);

  EXPECT_EQ((*ui_command_result)[am::strings::msg_params][am::strings::success]
                .asBool(),
            true);
  EXPECT_EQ(
      (*ui_command_result)[am::strings::msg_params][am::strings::result_code]
          .asInt(),
      static_cast<int32_t>(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE));
  if ((*ui_command_result)[am::strings::msg_params].keyExists(
          am::strings::info)) {
    EXPECT_FALSE(
        (*ui_command_result)[am::strings::msg_params][am::strings::info]
            .asString()
            .empty());
  }
}

TEST_F(DeleteSubMenuRequestTest, Run_InvalidApp_UNSUCCESS) {
  MockAppPtr invalid_app;
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(invalid_app));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(CheckMessageResultCode(
                              mobile_apis::Result::APPLICATION_NOT_REGISTERED),
                          am::commands::Command::CommandOrigin::SOURCE_SDL));
  EXPECT_CALL(*app_, FindSubMenu(_)).Times(0);
  command_->Run();
}

TEST_F(DeleteSubMenuRequestTest, Run_FindSubMenuFalse_UNSUCCESS) {
  (*message_)[am::strings::msg_params][am::strings::menu_id] = kMenuId;
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  smart_objects::SmartObject* invalid_sub_menu = NULL;
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app_));
  EXPECT_CALL(*app_, FindSubMenu(kMenuId)).WillOnce(Return(invalid_sub_menu));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(rpc_service_,
              ManageMobileCommand(
                  CheckMessageResultCode(mobile_apis::Result::INVALID_ID),
                  am::commands::Command::CommandOrigin::SOURCE_SDL));
  EXPECT_CALL(*app_, app_id()).Times(0);
  command_->Run();
}

TEST_F(DeleteSubMenuRequestTest, Run_SendHMIRequest_SUCCESS) {
  (*message_)[am::strings::msg_params][am::strings::menu_id] = kMenuId;
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  smart_objects::SmartObject* sub_menu =
      &((*message_)[am::strings::msg_params][am::strings::menu_id]);
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app_));
  EXPECT_CALL(*app_, FindSubMenu(kMenuId)).WillOnce(Return(sub_menu));

  EXPECT_CALL(*app_, app_id()).WillOnce(Return(kConnectionKey));
  EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_DeleteSubMenu)))
      .WillOnce(Return(true));
  command_->Run();
}

TEST_F(DeleteSubMenuRequestTest, OnEvent_UnknownEventId_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  EXPECT_CALL(app_mngr_, application(_)).Times(0);
  command_->on_event(event);
}

TEST_F(DeleteSubMenuRequestTest, OnEvent_InvalidApp_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::UI_DeleteSubMenu);
  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::SUCCESS;
  event.set_smart_object(*message_);
  MockAppPtr invalid_app;
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(invalid_app));
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
  const hmi_apis::Common_Result::eType result_code =
      hmi_apis::Common_Result::SUCCESS;
  (*message_)[am::strings::params][am::hmi_response::code] = result_code;
  event.set_smart_object(*message_);

  commands_map_.insert(
      std::make_pair(0, &((*message_)[am::strings::msg_params])));

  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(app_));

  InSequence seq;
  EXPECT_CALL(*app_, commands_map()).WillOnce(Return(accessor_));
  EXPECT_CALL(*app_, app_id()).WillOnce(Return(kConnectionKey));
  EXPECT_CALL(*app_, get_grammar_id()).WillOnce(Return(kGrammarId));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::VR_DeleteCommand)))
      .WillOnce(Return(true));

  EXPECT_CALL(*app_, commands_map()).WillOnce(Return(accessor_));
  EXPECT_CALL(*app_, app_id()).WillOnce(Return(kConnectionKey));
  EXPECT_CALL(*app_, RemoveCommand(_)).WillOnce(DeleteCommand(&commands_map_));
  EXPECT_CALL(
      rpc_service_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_DeleteCommand)))
      .WillOnce(Return(true));

  EXPECT_CALL(*app_, RemoveSubMenu(_));
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS),
                          am::commands::Command::SOURCE_SDL));
  EXPECT_CALL(*app_, UpdateHash());
  DeleteSubMenuRequestPtr command =
      CreateCommand<DeleteSubMenuRequest>(message_);

  command->Init();
  command->on_event(event);
}

TEST_F(DeleteSubMenuResponseTest, Run_SUCCESS) {
  MessageSharedPtr message(CreateMessage());
  (*message)[am::strings::msg_params][am::strings::connection_key] =
      kConnectionKey;
  DeleteSubMenuResponsePtr command(
      CreateCommand<DeleteSubMenuResponse>(message));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      SendMessageToMobile(CheckMessageConnectionKey(kConnectionKey), _));
  command->Run();
}

TEST_F(DeleteSubMenuRequestTest,
       DeleteSubmenu_CommandhaventVrCommadsAndMenuParams_DontSendHMIRequest) {
  Event event(hmi_apis::FunctionID::UI_DeleteSubMenu);
  (*message_)[am::strings::msg_params][am::strings::menu_id] = kMenuId;
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*message_)[am::strings::params][am::hmi_response::code] =
      am::mobile_api::Result::SUCCESS;
  event.set_smart_object(*message_);

  commands_map_.insert(
      std::make_pair(0, &((*message_)[am::strings::msg_params])));

  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(app_));
  EXPECT_CALL(rpc_service_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*app_, commands_map()).Times(2).WillRepeatedly(Return(accessor_));
  EXPECT_CALL(*app_, RemoveCommand(_)).Times(0);
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS),
                          am::commands::Command::SOURCE_SDL));
  command_->on_event(event);
}

TEST_F(DeleteSubMenuRequestTest,
       DeleteSubmenu_NotAChildOfMenupartam_DontSendHMIRequest) {
  Event event(hmi_apis::FunctionID::UI_DeleteSubMenu);
  (*message_)[am::strings::msg_params][am::strings::menu_id] = kMenuId;
  (*message_)[am::strings::msg_params][am::strings::menu_params]
             [am::hmi_request::parent_id] = kMenuId + 1;
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*message_)[am::strings::params][am::hmi_response::code] =
      am::mobile_api::Result::SUCCESS;
  event.set_smart_object(*message_);

  commands_map_.insert(
      std::make_pair(0, &((*message_)[am::strings::msg_params])));

  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(app_));
  EXPECT_CALL(rpc_service_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*app_, commands_map()).Times(2).WillRepeatedly(Return(accessor_));
  EXPECT_CALL(*app_, RemoveCommand(_)).Times(0);
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS),
                          am::commands::Command::SOURCE_SDL));
  command_->on_event(event);
}

}  // namespace delete_sub_menu_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
