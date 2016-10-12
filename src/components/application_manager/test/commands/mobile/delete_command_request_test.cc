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

#include "application_manager/commands/mobile/delete_command_request.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/test/include/application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/event_engine/event.h"

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
using am::MockMessageHelper;
using am::MockHmiInterfaces;

typedef SharedPtr<DeleteCommandRequest> DeleteCommandPtr;

namespace {
const int32_t kCommandId = 1;
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
}  // namespace

class DeleteCommandRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
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
};

TEST_F(DeleteCommandRequestTest, OnEvent_VrDeleteCommand_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr command_msg = CreateFullParamsVRSO();
  (*command_msg)[am::strings::msg_params][am::strings::cmd_id] = kCommandId;
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  DeleteCommandPtr command(CreateCommand<DeleteCommandRequest>(command_msg));

  MockAppPtr app = CreateMockApp();
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(app));

  MessageSharedPtr test_msg(CreateMessage(smart_objects::SmartType_Map));
  (*test_msg)[am::strings::vr_commands] = 0;
  (*test_msg)[am::strings::menu_params] = 0;

  MockHmiInterfaces hmi_interfaces;
  ON_CALL(app_mngr_, hmi_interfaces()).WillByDefault(ReturnRef(hmi_interfaces));
  ON_CALL(hmi_interfaces, GetInterfaceFromFunction(_))
      .WillByDefault(
          Return(am::HmiInterfaces::HMI_INTERFACE_BasicCommunication));
  ON_CALL(hmi_interfaces, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  ON_CALL(*app, FindCommand(kCommandId)).WillByDefault(Return(test_msg.get()));
  ON_CALL(*app, app_id()).WillByDefault(Return(kConnectionKey));
  ON_CALL(*app, get_grammar_id()).WillByDefault(Return(kConnectionKey));

  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[am::strings::params][am::strings::info] = "info";
  Event event_ui(hmi_apis::FunctionID::UI_DeleteCommand);
  event_ui.set_smart_object(*msg);

  command->Run();
  command->on_event(event_ui);

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*event_msg)[am::strings::params][am::strings::info] = "info";
  Event event_vr(hmi_apis::FunctionID::VR_DeleteCommand);
  event_vr.set_smart_object(*event_msg);

  MessageSharedPtr result_msg(
      CatchMobileCommandResult(CallOnEvent(*command, event_vr)));

  ASSERT_TRUE(result_msg);

  EXPECT_EQ(
      (*result_msg)[am::strings::msg_params][am::strings::success].asBool(),
      false);
  EXPECT_EQ(
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE,
      (*result_msg)[am::strings::msg_params][am::strings::result_code].asInt());

  if ((*result_msg)[am::strings::msg_params].keyExists(am::strings::info)) {
    EXPECT_EQ(
        (*event_msg)[am::strings::params][am::strings::info].asString(),
        (*result_msg)[am::strings::msg_params][am::strings::info].asString());
  }
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
