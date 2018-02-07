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

#include "application_manager/commands/mobile/set_display_layout_request.h"

#include "gtest/gtest.h"
#include "application_manager/commands/command_request_test.h"
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
namespace set_display_layout_request {

namespace am = application_manager;
namespace mobile_result = mobile_apis::Result;
using am::commands::SetDisplayLayoutRequest;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using am::MockMessageHelper;
using ::utils::SharedPtr;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

typedef ::utils::SharedPtr<SetDisplayLayoutRequest> CommandPtr;

namespace {
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
const uint32_t kCorrelationKey = 2u;
}  // namespace

MATCHER_P(CheckMshCorrId, corr_id, "") {
  return (*arg)[am::strings::params][am::strings::correlation_id].asUInt() ==
         corr_id;
}

class SetDisplayLayoutRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SetDisplayLayoutRequestTest() : mock_app_(CreateMockApp()) {
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
  }

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
  typedef TypeIf<kMocksAreNice,
                 NiceMock<application_manager_test::MockHMICapabilities>,
                 application_manager_test::MockHMICapabilities>::Result
      MockHMICapabilities;

  void ResultCommandExpectations(MessageSharedPtr msg,
                                 const std::string& info) {
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::success].asBool(),
              false);
    EXPECT_EQ(
        (*msg)[am::strings::msg_params][am::strings::result_code].asInt(),
        static_cast<int32_t>(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE));
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::info].asString(),
              info);
  }

  sync_primitives::Lock lock_;
  MockAppPtr mock_app_;
};

typedef SetDisplayLayoutRequestTest::MockHMICapabilities MockHMICapabilities;

TEST_F(SetDisplayLayoutRequestTest,
       OnEvent_UIHmiUnsupportedResource_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg_ui = CreateFullParamsUISO();
  (*msg_ui)[am::strings::params][am::strings::connection_key] = kConnectionKey;

  utils::SharedPtr<SetDisplayLayoutRequest> command =
      CreateCommand<SetDisplayLayoutRequest>(msg_ui);

  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));

  ON_CALL(*mock_app, app_id()).WillByDefault(Return(kConnectionKey));

  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg)[am::strings::msg_params][am::strings::app_id] = kConnectionKey;
  (*msg)[am::strings::msg_params][am::strings::info] =
      "UI is not supported by system";

  Event event(hmi_apis::FunctionID::UI_SetDisplayLayout);
  event.set_smart_object(*msg);

  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  MessageSharedPtr ui_command_result;
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));

  command->on_event(event);

  ResultCommandExpectations(ui_command_result, "UI is not supported by system");
}

TEST_F(SetDisplayLayoutRequestTest, Run_InvalidApp_UNSUCCESS) {
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  CommandPtr command(CreateCommand<SetDisplayLayoutRequest>(msg));

  MockAppPtr invalid_mock_app;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(invalid_mock_app));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED),
                  am::commands::Command::CommandSource::SOURCE_SDL));

  command->Run();
}

TEST_F(SetDisplayLayoutRequestTest, Run_SUCCESS) {
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  CommandPtr command(CreateCommand<SetDisplayLayoutRequest>(msg));
  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, app_id()).WillOnce(Return(kAppId));

  EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationKey));
  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetDisplayLayout))
      .WillRepeatedly(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  EXPECT_CALL(mock_hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_AVAILABLE));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(rpc_service_, ManageHMICommand(CheckMshCorrId(kCorrelationKey)))
      .WillOnce(Return(true));

  command->Run();
}

TEST_F(SetDisplayLayoutRequestTest, OnEvent_InvalidEventId_UNSUCCESS) {
  CommandPtr command(CreateCommand<SetDisplayLayoutRequest>());
  am::event_engine::Event event(hmi_apis::FunctionID::INVALID_ENUM);
  SmartObject msg(smart_objects::SmartType_Map);

  event.set_smart_object(msg);

  EXPECT_CALL(app_mngr_, hmi_capabilities()).Times(0);
  command->on_event(event);
}

TEST_F(SetDisplayLayoutRequestTest, OnEvent_SUCCESS) {
  am::event_engine::Event event(hmi_apis::FunctionID::UI_SetDisplayLayout);
  MessageSharedPtr msg = CreateMessage();

  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[am::strings::msg_params][am::hmi_response::display_capabilities] = 0;
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  event.set_smart_object(*msg);

  MockHMICapabilities hmi_capabilities;
  MessageSharedPtr dispaly_capabilities_msg = CreateMessage();
  (*dispaly_capabilities_msg)[am::hmi_response::templates_available] =
      "templates_available";

  EXPECT_CALL(app_mngr_, hmi_capabilities())
      .WillOnce(ReturnRef(hmi_capabilities));

  EXPECT_CALL(hmi_capabilities, display_capabilities())
      .WillOnce(Return(dispaly_capabilities_msg.get()));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS),
                          am::commands::Command::CommandSource::SOURCE_SDL));

  CommandPtr command(CreateCommand<SetDisplayLayoutRequest>(msg));
  command->on_event(event);
}

}  // namespace set_display_layout_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests
