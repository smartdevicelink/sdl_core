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

#include "mobile/delete_command_request.h"

#include "gtest/gtest.h"

#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_help_prompt_manager.h"
#include "application_manager/event_engine/event.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace delete_command_request {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
namespace am = ::application_manager;
namespace am_test = application_manager_test;
using sdl_rpc_plugin::commands::DeleteCommandRequest;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
using am::MockMessageHelper;
using am::MockHmiInterfaces;

typedef std::shared_ptr<DeleteCommandRequest> DeleteCommandPtr;

namespace {
const int32_t kCommandId = 1;
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
}  // namespace

class DeleteCommandRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  DeleteCommandRequestTest()
      : mock_help_prompt_manager_(
            std::make_shared<am_test::MockHelpPromptManager>())
      , mock_app_(CreateMockApp()) {}
  MessageSharedPtr CreateFullParamsUISO() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    smart_objects::SmartObject menu_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    menu_params[am::strings::position] = 10;
    menu_params[am::strings::menu_name] = "LG";

    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params[am::strings::cmd_id] = kCmdId;
    msg_params[am::strings::menu_params] = menu_params;
    msg_params[am::strings::app_id] = kAppId;
    msg_params[am::strings::cmd_icon] = 1;
    msg_params[am::strings::cmd_icon][am::strings::value] = "10";
    (*msg)[am::strings::msg_params] = msg_params;

    return msg;
  }

  MessageSharedPtr CreateFullParamsVRSO() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params[am::strings::cmd_id] = kCmdId;
    msg_params[am::strings::vr_commands] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    msg_params[am::strings::vr_commands][0] = "lamer";
    msg_params[am::strings::type] = 34;
    msg_params[am::strings::grammar_id] = 12;
    msg_params[am::strings::app_id] = kAppId;
    (*msg)[am::strings::msg_params] = msg_params;

    return msg;
  }

  void ResultCommandExpectations(MessageSharedPtr msg,
                                 const std::string& info) {
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::success].asBool(),
              true);
    EXPECT_EQ(
        (*msg)[am::strings::msg_params][am::strings::result_code].asInt(),
        static_cast<int32_t>(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE));
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::info].asString(),
              info);
  }

  void SetUp() OVERRIDE {
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(hmi_interfaces_));
  }

  NiceMock<MockHmiInterfaces> hmi_interfaces_;
  std::shared_ptr<am_test::MockHelpPromptManager> mock_help_prompt_manager_;
  MockAppPtr mock_app_;
};

TEST_F(DeleteCommandRequestTest,
       OnEvent_VrHmiSendUnsupportedResource_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr command_msg = CreateFullParamsVRSO();
  (*command_msg)[am::strings::msg_params][am::strings::cmd_id] = kCommandId;
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  DeleteCommandPtr command(CreateCommand<DeleteCommandRequest>(command_msg));

  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app_));

  MessageSharedPtr test_msg(CreateMessage(smart_objects::SmartType_Map));
  (*test_msg)[am::strings::vr_commands] = 0;
  (*test_msg)[am::strings::menu_params] = 0;

  ON_CALL(hmi_interfaces_, GetInterfaceFromFunction(_))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_VR));
  ON_CALL(hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  ON_CALL(hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VR))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  ON_CALL(*mock_app_, FindCommand(kCommandId))
      .WillByDefault(Return(test_msg.get()));
  ON_CALL(*mock_app_, get_grammar_id()).WillByDefault(Return(kConnectionKey));
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  Event event_ui(hmi_apis::FunctionID::UI_DeleteCommand);
  event_ui.set_smart_object(*msg);
  command->Init();
  command->Run();
  command->on_event(event_ui);

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*event_msg)[am::strings::msg_params][am::strings::info] =
      "VR is not supported by system";
  Event event_vr(hmi_apis::FunctionID::VR_DeleteCommand);
  event_vr.set_smart_object(*event_msg);

  EXPECT_CALL(*mock_app_, RemoveCommand(kCommandId));

  EXPECT_CALL(*mock_app_, UpdateHash());

  MessageSharedPtr vr_command_result;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&vr_command_result), Return(true)));

  EXPECT_CALL(*mock_app_, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnVrCommandDeleted(kCommandId, false));

  command->on_event(event_vr);

  ResultCommandExpectations(vr_command_result, "VR is not supported by system");
}

TEST_F(DeleteCommandRequestTest,
       OnEvent_UIHmiSendUnsupportedResource_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr command_msg = CreateFullParamsUISO();
  (*command_msg)[am::strings::msg_params][am::strings::cmd_id] = kCommandId;
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  DeleteCommandPtr command(CreateCommand<DeleteCommandRequest>(command_msg));

  MockAppPtr app = CreateMockApp();
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));

  MessageSharedPtr test_msg(CreateMessage(smart_objects::SmartType_Map));
  (*test_msg)[am::strings::vr_commands] = 0;
  (*test_msg)[am::strings::menu_params] = 0;

  ON_CALL(hmi_interfaces_, GetInterfaceFromFunction(_))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  ON_CALL(hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_VR))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  ON_CALL(*app, FindCommand(kCommandId)).WillByDefault(Return(test_msg.get()));
  ON_CALL(*app, get_grammar_id()).WillByDefault(Return(kConnectionKey));
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  Event event_vr(hmi_apis::FunctionID::VR_DeleteCommand);
  event_vr.set_smart_object(*msg);

  command->Init();
  command->Run();
  command->on_event(event_vr);

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*event_msg)[am::strings::msg_params][am::strings::info] =
      "UI is not supported by system";
  Event event_ui(hmi_apis::FunctionID::UI_DeleteCommand);
  event_ui.set_smart_object(*event_msg);

  EXPECT_CALL(*app, RemoveCommand(kCommandId));

  EXPECT_CALL(*app, UpdateHash());

  EXPECT_CALL(*app, help_prompt_manager())
      .WillOnce(ReturnRef(*mock_help_prompt_manager_));
  EXPECT_CALL(*mock_help_prompt_manager_,
              OnVrCommandDeleted(kCommandId, false));

  MessageSharedPtr result_msg(
      CatchMobileCommandResult(CallOnEvent(*command, event_ui)));

  ASSERT_TRUE((bool)result_msg);

  ResultCommandExpectations(result_msg, "UI is not supported by system");
}

}  // namespace delete_command_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
