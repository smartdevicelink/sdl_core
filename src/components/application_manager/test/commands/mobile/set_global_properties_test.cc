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

#include "application_manager/commands/mobile/set_global_properties_request.h"

#include "gtest/gtest.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"

namespace test {
namespace components {
namespace commands_test {

namespace am = application_manager;
using am::commands::SetGlobalPropertiesRequest;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using am::MockMessageHelper;
using am::MockHmiInterfaces;
using ::utils::SharedPtr;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {
const int32_t kCommandId = 1;
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
}  // namespace

class SetGlobalPropertiesRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  sync_primitives::Lock lock_;

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
    msg_params[am::strings::vr_help_title] = "vr_help_title";
    msg_params[am::strings::vr_help][0][am::strings::text] = "vr_help";
    msg_params[am::strings::vr_help][0][am::strings::position] = 1u;
    msg_params[am::strings::help_prompt][0][am::strings::text] = "help_promt";
    (*msg)[am::strings::msg_params] = msg_params;

    return msg;
  }
};

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_UI_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg_vr = CreateFullParamsUISO();
  (*msg_vr)[am::strings::msg_params][am::strings::vr_commands][0] =
      "vr_command";

  utils::SharedPtr<SetGlobalPropertiesRequest> req_vr =
      CreateCommand<SetGlobalPropertiesRequest>(msg_vr);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(_));
  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));
  ON_CALL(*mock_app, app_id()).WillByDefault(Return(kConnectionKey));
  const smart_objects::SmartObject* vr_help_title =
      &((*msg_vr)[am::strings::msg_params][am::strings::vr_help_title]);
  const smart_objects::SmartObject* vr_help =
      &((*msg_vr)[am::strings::msg_params][am::strings::vr_help]);
  const smart_objects::SmartObject* vr_help_prompt =
      &((*msg_vr)[am::strings::msg_params][am::strings::help_prompt]);
  ON_CALL(*mock_app, vr_help_title()).WillByDefault(Return(vr_help_title));
  ON_CALL(*mock_app, vr_help()).WillByDefault(Return(vr_help));
  ON_CALL(*mock_app, help_prompt()).WillByDefault(Return(vr_help_prompt));

  MockHmiInterfaces hmi_interfaces;
  ON_CALL(app_mngr_, hmi_interfaces()).WillByDefault(ReturnRef(hmi_interfaces));
  ON_CALL(hmi_interfaces, GetInterfaceFromFunction(_))
      .WillByDefault(
          Return(am::HmiInterfaces::HMI_INTERFACE_BasicCommunication));
  ON_CALL(hmi_interfaces, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));

  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg)[am::strings::msg_params][am::strings::cmd_id] = kCommandId;

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  MockMessageHelper* mock_message_helper =
      MockMessageHelper::message_helper_mock();
  ON_CALL(*mock_message_helper, VerifyImageVrHelpItems(_, _, _))
      .WillByDefault(Return(mobile_apis::Result::SUCCESS));
  ON_CALL(*mock_message_helper, HMIToMobileResult(_))
      .WillByDefault(Return(mobile_apis::Result::UNSUPPORTED_RESOURCE));

  ON_CALL(*mock_message_helper, VerifyImage(_, _, _))
      .WillByDefault(Return(mobile_apis::Result::SUCCESS));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(Return(true))
      .WillOnce(Return(true));

  EXPECT_CALL(*mock_app, UpdateHash());

  MessageSharedPtr msg_ui(CreateMessage(smart_objects::SmartType_Map));
  (*msg_ui)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;

  Event event_vr(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event_vr.set_smart_object(*msg_vr);

  req_vr->Run();
  req_vr->on_event(event_vr);

  MessageSharedPtr ui_command_result;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));

  req_vr->on_event(event);

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

}  // namespace commands_test
}  // namespace components
}  // namespace tests
