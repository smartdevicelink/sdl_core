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

#include "mobile/delete_sub_menu_request.h"
#include "mobile/delete_sub_menu_response.h"

#include "application_manager/commands/command_request_test.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_help_prompt_manager.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_message_helper.h"
#include "gtest/gtest.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace delete_sub_menu_request {

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
namespace am = ::application_manager;
namespace am_test = application_manager_test;
using am::MockHmiInterfaces;
using am::MockMessageHelper;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;

using sdl_rpc_plugin::commands::DeleteSubMenuRequest;
using sdl_rpc_plugin::commands::DeleteSubMenuResponse;

typedef std::shared_ptr<DeleteSubMenuRequest> DeleteSubMenuRequestPtr;
typedef std::shared_ptr<DeleteSubMenuResponse> DeleteSubMenuResponsePtr;
typedef std::shared_ptr<Event> EventPtr;

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
const uint32_t kMenuIdChild = 101u;
const uint32_t kGrammarId = 101u;
const int32_t kCmdId = 102u;
}  // namespace

class DeleteSubMenuRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  DeleteSubMenuRequestTest()
      : commands_lock_(std::make_shared<sync_primitives::Lock>())
      , sub_menu_lock_(std::make_shared<sync_primitives::RecursiveLock>())
      , accessor_(commands_map_, commands_lock_)
      , sub_menu_accessor_(sub_menu_map_, sub_menu_lock_)
      , message_(CreateMessage())
      , command_(CreateCommand<DeleteSubMenuRequest>(message_))
      , mock_help_prompt_manager_(
            std::make_shared<am_test::MockHelpPromptManager>())
      , app_(CreateMockApp()) {}

  /**
   * @brief Generates submenu smart object with specified params
   * @param menu_id id of the menu
   * @param parent_id id of parent menu
   * @return generated smart object
   */
  smart_objects::SmartObjectSPtr GenerateSubMenuMap(const uint32_t menu_id,
                                                    const uint32_t parent_id) {
    smart_objects::SmartObjectSPtr smartObjectPtr =
        std::make_shared<smart_objects::SmartObject>();

    smart_objects::SmartObject& object = *smartObjectPtr;

    object[am::strings::position] = 1;
    object[am::strings::menu_name] = "SubMenu " + std::to_string(menu_id);
    object[am::strings::menu_id] = menu_id;
    object[am::strings::parent_id] = parent_id;

    return smartObjectPtr;
  }

  /**
   * @brief Generates UI & VR commands smart object with specified params
   * @param cmd_id id of the command
   * @param parent_id id of parent command
   * @return generated smart object
   */
  smart_objects::SmartObjectSPtr GenerateUiVrCommandMapChild(
      const uint32_t cmd_id, const uint32_t parent_id) {
    smart_objects::SmartObjectSPtr smartObjectPtr =
        std::make_shared<smart_objects::SmartObject>();

    smart_objects::SmartObject& object = *smartObjectPtr;

    object[am::strings::cmd_id] = cmd_id;
    object[am::strings::grammar_id] = kGrammarId;
    object[am::strings::vr_commands] = "VR commands";
    object[am::strings::type] = hmi_apis::Common_VRCommandType::Command;
    object[am::strings::menu_params][am::hmi_request::parent_id] = parent_id;

    return smartObjectPtr;
  }

  /**
   * @brief Sets up expectations for a successful execution of submenu request
   * @param next_function_id function id of the next request
   */
  void SetUpSubMenuRequestExpectations(
      const hmi_apis::FunctionID::eType next_function_id) {
    (*message_)[am::strings::msg_params][am::strings::menu_id] = kMenuId;
    (*message_)[am::strings::params][am::strings::connection_key] =
        kConnectionKey;
    (*message_)[am::strings::params][am::strings::function_id] =
        next_function_id;

    smart_objects::SmartObject sub_menu =
        (*message_)[am::strings::msg_params][am::strings::menu_id];
    EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app_));
    EXPECT_CALL(*app_, FindSubMenu(kMenuId)).WillOnce(Return(sub_menu));

    ON_CALL(*app_, sub_menu_map()).WillByDefault(Return(sub_menu_accessor_));
    ON_CALL(*app_, commands_map()).WillByDefault(Return(accessor_));

    EXPECT_CALL(*app_, app_id()).WillRepeatedly(Return(kConnectionKey));
    EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
        .WillOnce(Return(kCorrelationId));

    EXPECT_CALL(mock_rpc_service_,
                ManageHMICommand(HMIResultCodeIs(next_function_id), _))
        .WillOnce(Return(true));
  }

  /**
   * @brief Prepares submenu response event with the specified params
   * @param correlation_id correlation id
   * @param menu_id id of the menu
   * @return pointer to prepared event
   */
  EventPtr PrepareSubMenuResponseEvent(const uint32_t correlation_id,
                                       const uint32_t menu_id) {
    EventPtr menu_event =
        std::make_shared<Event>(hmi_apis::FunctionID::UI_DeleteSubMenu);
    (*message_)[am::strings::params][am::hmi_response::code] =
        hmi_apis::Common_Result::SUCCESS;
    (*message_)[am::strings::msg_params][am::strings::app_id] = kConnectionKey;
    (*message_)[am::strings::params][am::strings::correlation_id] =
        correlation_id;
    (*message_)[am::strings::msg_params][am::strings::menu_id] = menu_id;
    menu_event->set_smart_object(*message_);

    return menu_event;
  }

  /**
   * @brief Sets up expectations for a successful execution of submenu
   * subsequent response
   * @param event pointer to event representing the response message
   * @param next_function_id function id of the next request
   */
  void SetUpSubMenuSubsequentResponseExpectations(
      EventPtr event, hmi_apis::FunctionID::eType next_function_id) {
    const auto& event_so_ref = event->smart_object();
    const auto correlation_id = event->smart_object_correlation_id();
    const auto menu_id =
        event_so_ref[am::strings::msg_params][am::strings::menu_id].asUInt();

    EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app_));
    EXPECT_CALL(*app_, RemoveSubMenu(menu_id));

    EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
        .WillOnce(Return(correlation_id + 1));
    EXPECT_CALL(mock_rpc_service_,
                ManageHMICommand(HMIResultCodeIs(next_function_id), _))
        .WillOnce(Return(true));
  }

  /**
   * @brief Prepares VR command response event with the specified params
   * @param correlation_id correlation id
   * @return pointer to prepared event
   */
  EventPtr PrepareVrCommandResponseEvent(const uint32_t correlation_id) {
    EventPtr vr_event =
        std::make_shared<Event>(hmi_apis::FunctionID::VR_DeleteCommand);
    (*message_)[am::strings::msg_params][am::strings::app_id] = kConnectionKey;
    (*message_)[am::strings::params][am::hmi_response::code] =
        hmi_apis::Common_Result::SUCCESS;
    (*message_)[am::strings::params][am::strings::correlation_id] =
        correlation_id;
    vr_event->set_smart_object(*message_);

    return vr_event;
  }

  /**
   * @brief Sets up expectations for a successful execution of VR command
   * subsequent response
   * @param event pointer to event representing the response message
   * @param next_function_id function id of the next request
   */
  void SetUpVrCommandSubsequentResponseExpectations(
      EventPtr event, hmi_apis::FunctionID::eType next_function_id) {
    const auto correlation_id = event->smart_object_correlation_id();

    EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app_));
    EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
        .WillOnce(Return(correlation_id + 1));
    EXPECT_CALL(mock_rpc_service_,
                ManageHMICommand(HMIResultCodeIs(next_function_id), _))
        .WillOnce(Return(true));
  }

  /**
   * @brief Prepares UI command response event with the specified params
   * @param correlation_id correlation id
   * @param cmd_id id of the command
   * @return pointer to prepared event
   */
  EventPtr PrepareUiCommandResponseEvent(const uint32_t correlation_id,
                                         const uint32_t cmd_id) {
    EventPtr ui_event =
        std::make_shared<Event>(hmi_apis::FunctionID::UI_DeleteCommand);
    (*message_)[am::strings::params][am::strings::correlation_id] =
        correlation_id;
    (*message_)[am::strings::params][am::hmi_response::code] =
        hmi_apis::Common_Result::SUCCESS;
    (*message_)[am::strings::msg_params][am::strings::app_id] = kConnectionKey;
    (*message_)[am::strings::msg_params][am::strings::cmd_id] = cmd_id;
    ui_event->set_smart_object(*message_);

    return ui_event;
  }

  /**
   * @brief Sets up expectations for a successful execution of UI command
   * subsequent response
   * @param event pointer to event representing the response message
   * @param next_function_id function id of the next request
   */
  void SetUpUiCommandSubsequentResponseExpectations(
      EventPtr event, hmi_apis::FunctionID::eType next_function_id) {
    const auto correlation_id = event->smart_object_correlation_id();
    const auto& event_so_ref = event->smart_object();
    const auto cmd_id =
        event_so_ref[am::strings::msg_params][am::strings::cmd_id].asUInt();

    EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app_));
    EXPECT_CALL(*app_, RemoveCommand(cmd_id))
        .WillOnce(DeleteCommand(&commands_map_));
    EXPECT_CALL(*app_, help_prompt_manager())
        .WillOnce(ReturnRef(*mock_help_prompt_manager_));
    EXPECT_CALL(*mock_help_prompt_manager_, OnVrCommandDeleted(cmd_id, false));

    EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
        .WillOnce(Return(correlation_id + 1));
    EXPECT_CALL(mock_rpc_service_,
                ManageHMICommand(HMIResultCodeIs(next_function_id), _))
        .WillOnce(Return(true));
  }

  /**
   * @brief Sets up expectations for a successful execution of submenu final
   * response
   * @param event pointer to event representing the response message
   */
  void SetUpSubMenuFinalResponseExpectations(EventPtr event) {
    const auto& event_so_ref = event->smart_object();
    const auto menu_id =
        event_so_ref[am::strings::msg_params][am::strings::menu_id].asUInt();

    EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app_));
    EXPECT_CALL(*app_, RemoveSubMenu(menu_id));
    EXPECT_CALL(
        mock_rpc_service_,
        ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS),
                            am::commands::Command::SOURCE_SDL));
  }

  am::CommandsMap commands_map_;
  am::SubMenuMap sub_menu_map_;
  mutable std::shared_ptr<sync_primitives::Lock> commands_lock_;
  mutable std::shared_ptr<sync_primitives::RecursiveLock> sub_menu_lock_;
  DataAccessor<am::CommandsMap> accessor_;
  DataAccessor<am::SubMenuMap> sub_menu_accessor_;

  MessageSharedPtr message_;
  DeleteSubMenuRequestPtr command_;
  std::shared_ptr<am_test::MockHelpPromptManager> mock_help_prompt_manager_;
  MockAppPtr app_;
};

class DeleteSubMenuResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(DeleteSubMenuRequestTest, DISABLED_OnEvent_UI_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg)[am::strings::msg_params][am::strings::menu_id] = 10u;

  std::shared_ptr<DeleteSubMenuRequest> command =
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

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
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

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(CheckMessageResultCode(
                              mobile_apis::Result::APPLICATION_NOT_REGISTERED),
                          am::commands::Command::CommandSource::SOURCE_SDL));
  EXPECT_CALL(*app_, FindSubMenu(_)).Times(0);
  command_->Run();
}

TEST_F(DeleteSubMenuRequestTest, Run_FindSubMenuFalse_UNSUCCESS) {
  (*message_)[am::strings::msg_params][am::strings::menu_id] = kMenuId;
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  smart_objects::SmartObject invalid_sub_menu(smart_objects::SmartType_Null);
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app_));
  EXPECT_CALL(*app_, FindSubMenu(kMenuId)).WillOnce(Return(invalid_sub_menu));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  CheckMessageResultCode(mobile_apis::Result::INVALID_ID),
                  am::commands::Command::CommandSource::SOURCE_SDL));
  EXPECT_CALL(*app_, app_id()).Times(0);
  command_->Run();
}

TEST_F(DeleteSubMenuRequestTest, OnEvent_UnknownEventId_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, am::commands::Command::SOURCE_SDL))
      .Times(0);
  command_->on_event(event);
}

TEST_F(DeleteSubMenuRequestTest, OnEvent_InvalidApp_UNSUCCESS) {
  auto sub_menu_ptr = GenerateSubMenuMap(kMenuId, 0);
  sub_menu_map_.insert(std::pair<uint32_t, smart_objects::SmartObject*>(
      kMenuId, sub_menu_ptr.get()));

  SetUpSubMenuRequestExpectations(hmi_apis::FunctionID::UI_DeleteSubMenu);
  command_->Run();

  MockAppPtr invalid_app;
  EventPtr event = PrepareSubMenuResponseEvent(kCorrelationId, kMenuId);

  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(invalid_app));
  EXPECT_CALL(*app_, RemoveSubMenu(_)).Times(0);
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, am::commands::Command::SOURCE_SDL))
      .Times(0);

  command_->on_event(*event);
}

TEST_F(DeleteSubMenuRequestTest, OnEvent_DeleteSubmenuOnly_SUCCESS) {
  auto sub_menu_ptr = GenerateSubMenuMap(kMenuId, 0);
  sub_menu_map_.insert(std::pair<uint32_t, smart_objects::SmartObject*>(
      kMenuId, sub_menu_ptr.get()));

  SetUpSubMenuRequestExpectations(hmi_apis::FunctionID::UI_DeleteSubMenu);
  command_->Run();

  EventPtr event = PrepareSubMenuResponseEvent(kCorrelationId, kMenuId);
  SetUpSubMenuFinalResponseExpectations(event);
  command_->on_event(*event);
}

TEST_F(DeleteSubMenuRequestTest, OnEvent_DeleteSubmenuWithChildMenu_SUCCESS) {
  auto sub_menu_ptr = GenerateSubMenuMap(kMenuId, 0);
  sub_menu_map_.insert(std::pair<uint32_t, smart_objects::SmartObject*>(
      kMenuId, sub_menu_ptr.get()));

  auto sub_menu_child_ptr = GenerateSubMenuMap(kMenuIdChild, kMenuId);
  sub_menu_map_.insert(std::pair<uint32_t, smart_objects::SmartObject*>(
      kMenuIdChild, sub_menu_child_ptr.get()));

  SetUpSubMenuRequestExpectations(hmi_apis::FunctionID::UI_DeleteSubMenu);
  command_->Run();

  EventPtr event = PrepareSubMenuResponseEvent(kCorrelationId, kMenuIdChild);
  SetUpSubMenuSubsequentResponseExpectations(
      event, hmi_apis::FunctionID::UI_DeleteSubMenu);
  command_->on_event(*event);

  EventPtr event_final =
      PrepareSubMenuResponseEvent(kCorrelationId + 1, kMenuId);
  SetUpSubMenuFinalResponseExpectations(event_final);
  command_->on_event(*event_final);
}

TEST_F(DeleteSubMenuRequestTest, OnEvent_DeleteSubmenuWithVrAndUi_SUCCESS) {
  auto sub_menu_ptr = GenerateSubMenuMap(kMenuId, 0);
  sub_menu_map_.insert(std::pair<uint32_t, smart_objects::SmartObject*>(
      kMenuId, sub_menu_ptr.get()));

  auto command_ptr = GenerateUiVrCommandMapChild(kCmdId, kMenuId);
  commands_map_.insert(std::pair<uint32_t, smart_objects::SmartObject*>(
      kCmdId, command_ptr.get()));

  SetUpSubMenuRequestExpectations(hmi_apis::FunctionID::VR_DeleteCommand);
  command_->Run();

  EventPtr event_vr = PrepareVrCommandResponseEvent(kCorrelationId);
  SetUpVrCommandSubsequentResponseExpectations(
      event_vr, hmi_apis::FunctionID::UI_DeleteCommand);
  command_->on_event(*event_vr);

  EventPtr event_ui = PrepareUiCommandResponseEvent(kCorrelationId + 1, kCmdId);
  SetUpUiCommandSubsequentResponseExpectations(
      event_ui, hmi_apis::FunctionID::UI_DeleteSubMenu);
  command_->on_event(*event_ui);

  EventPtr event_final =
      PrepareSubMenuResponseEvent(kCorrelationId + 2, kMenuId);
  SetUpSubMenuFinalResponseExpectations(event_final);
  command_->on_event(*event_final);
}

TEST_F(DeleteSubMenuRequestTest,
       OnEvent_DeleteSubmenuWithChildVrAndUi_SUCCESS) {
  auto sub_menu_ptr = GenerateSubMenuMap(kMenuId, 0);
  sub_menu_map_.insert(std::pair<uint32_t, smart_objects::SmartObject*>(
      kMenuId, sub_menu_ptr.get()));

  auto sub_menu_child_ptr = GenerateSubMenuMap(kMenuIdChild, kMenuId);
  sub_menu_map_.insert(std::pair<uint32_t, smart_objects::SmartObject*>(
      kMenuIdChild, sub_menu_child_ptr.get()));

  auto command_ptr = GenerateUiVrCommandMapChild(kCmdId, kMenuIdChild);
  commands_map_.insert(std::pair<uint32_t, smart_objects::SmartObject*>(
      kCmdId, command_ptr.get()));

  SetUpSubMenuRequestExpectations(hmi_apis::FunctionID::VR_DeleteCommand);
  command_->Run();

  EventPtr event_vr = PrepareVrCommandResponseEvent(kCorrelationId);
  SetUpVrCommandSubsequentResponseExpectations(
      event_vr, hmi_apis::FunctionID::UI_DeleteCommand);
  command_->on_event(*event_vr);

  EventPtr event_ui = PrepareUiCommandResponseEvent(kCorrelationId + 1, kCmdId);
  SetUpUiCommandSubsequentResponseExpectations(
      event_ui, hmi_apis::FunctionID::UI_DeleteSubMenu);
  command_->on_event(*event_ui);

  EventPtr event_submenu =
      PrepareSubMenuResponseEvent(kCorrelationId + 2, kMenuIdChild);
  SetUpSubMenuSubsequentResponseExpectations(
      event_submenu, hmi_apis::FunctionID::UI_DeleteSubMenu);
  command_->on_event(*event_submenu);

  EventPtr event_final =
      PrepareSubMenuResponseEvent(kCorrelationId + 3, kMenuId);
  SetUpSubMenuFinalResponseExpectations(event_final);
  command_->on_event(*event_final);
}

TEST_F(DeleteSubMenuRequestTest,
       OnEvent_DeleteSubmenuVrAndUiWithChild_SUCCESS) {
  auto sub_menu_ptr = GenerateSubMenuMap(kMenuId, 0);
  sub_menu_map_.insert(std::pair<uint32_t, smart_objects::SmartObject*>(
      kMenuId, sub_menu_ptr.get()));

  auto command_ptr = GenerateUiVrCommandMapChild(kCmdId, kMenuId);
  commands_map_.insert(std::pair<uint32_t, smart_objects::SmartObject*>(
      kCmdId, command_ptr.get()));

  auto sub_menu_child_ptr = GenerateSubMenuMap(kMenuIdChild, kMenuId);
  sub_menu_map_.insert(std::pair<uint32_t, smart_objects::SmartObject*>(
      kMenuIdChild, sub_menu_child_ptr.get()));

  SetUpSubMenuRequestExpectations(hmi_apis::FunctionID::UI_DeleteSubMenu);
  command_->Run();

  EventPtr event_submenu =
      PrepareSubMenuResponseEvent(kCorrelationId, kMenuIdChild);
  SetUpSubMenuSubsequentResponseExpectations(
      event_submenu, hmi_apis::FunctionID::VR_DeleteCommand);
  command_->on_event(*event_submenu);

  EventPtr event_vr = PrepareVrCommandResponseEvent(kCorrelationId + 1);
  SetUpVrCommandSubsequentResponseExpectations(
      event_vr, hmi_apis::FunctionID::UI_DeleteCommand);
  command_->on_event(*event_vr);

  EventPtr event_ui = PrepareUiCommandResponseEvent(kCorrelationId + 2, kCmdId);
  SetUpUiCommandSubsequentResponseExpectations(
      event_ui, hmi_apis::FunctionID::UI_DeleteSubMenu);
  command_->on_event(*event_ui);

  EventPtr event_final =
      PrepareSubMenuResponseEvent(kCorrelationId + 3, kMenuId);
  SetUpSubMenuFinalResponseExpectations(event_final);
  command_->on_event(*event_final);
}

TEST_F(DeleteSubMenuResponseTest, Run_SUCCESS) {
  MessageSharedPtr message(CreateMessage());
  (*message)[am::strings::msg_params][am::strings::connection_key] =
      kConnectionKey;
  DeleteSubMenuResponsePtr command(
      CreateCommand<DeleteSubMenuResponse>(message));

  EXPECT_CALL(
      mock_rpc_service_,
      SendMessageToMobile(CheckMessageConnectionKey(kConnectionKey), _));
  command->Run();
}

}  // namespace delete_sub_menu_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
