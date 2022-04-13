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
#include <set>
#include <string>

#include "mobile/set_display_layout_request.h"

#include "application_manager/commands/command_request_test.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_message_helper.h"
#include "gtest/gtest.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace set_display_layout_request {

namespace am = application_manager;
namespace mobile_result = mobile_apis::Result;
using am::MockMessageHelper;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::SetDisplayLayoutRequest;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

typedef std::shared_ptr<SetDisplayLayoutRequest> CommandPtr;

namespace {
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
const uint32_t kCorrelationKey = 2u;
const std::string kLayout = "media";
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

  void CreateResponseCapabilities() {
    using namespace smart_objects;
    // Display capabilities
    SmartObjectSPtr display_capabilities =
        std::make_shared<SmartObject>(SmartType_Map);
    SmartObject text_fields(SmartType_Array);
    text_fields[0] = SmartObject(SmartType_Map);
    text_fields[0][am::hmi_response::button_name] = am::strings::main_field_1;
    text_fields[0][am::strings::character_set] = "UTF-8";
    text_fields[0]["rows"] = 1;
    text_fields[0]["width"] = 500;
    (*display_capabilities)[am::hmi_response::display_type] = "SDL_GENERIC";
    (*display_capabilities)[am::hmi_response::text_fields] = text_fields;
    (*display_capabilities)[am::hmi_response::media_clock_formats] =
        SmartObject(SmartType_Array);
    (*display_capabilities)[am::hmi_response::graphic_supported] = true;

    // Button capabilities
    SmartObjectSPtr button_capabilities =
        std::make_shared<SmartObject>(SmartType_Array);
    SmartObject button_capability(SmartType_Map);
    button_capability[strings::button_name] = "OK";
    button_capability["shortPressAvailable"] = true;
    button_capability["longPressAvailable"] = true;
    button_capability["upDownAvailable"] = true;
    (*button_capabilities)[0] = button_capability;

    // Soft Button capabilities
    SmartObjectSPtr soft_button_capabilities =
        std::make_shared<SmartObject>(SmartType_Array);
    SmartObject soft_button_capability(SmartType_Map);
    soft_button_capability[am::hmi_response::image_supported] = true;
    soft_button_capability["longPressAvailable"] = true;
    soft_button_capability["shortPressAvailable"] = true;
    soft_button_capability["upDownAvailable"] = true;
    soft_button_capability["imageSupported"] = false;
    (*soft_button_capabilities) = soft_button_capability;

    // Preset Bank capabilities
    SmartObjectSPtr preset_bank_capabilities =
        std::make_shared<SmartObject>(SmartType_Map);
    (*preset_bank_capabilities)[am::hmi_response::on_screen_presets_available] =
        false;

    EXPECT_CALL(mock_hmi_capabilities_, display_capabilities())
        .WillOnce(Return(display_capabilities));
    EXPECT_CALL(mock_hmi_capabilities_, button_capabilities())
        .WillOnce(Return(button_capabilities));
    EXPECT_CALL(mock_hmi_capabilities_, soft_button_capabilities())
        .WillOnce(Return(soft_button_capabilities));
    EXPECT_CALL(mock_hmi_capabilities_, preset_bank_capabilities())
        .WillOnce(Return(preset_bank_capabilities));
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

  std::shared_ptr<SetDisplayLayoutRequest> command =
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

  Event event(hmi_apis::FunctionID::UI_Show);
  event.set_smart_object(*msg);

  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  MessageSharedPtr ui_command_result;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));

  command->Init();
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
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED),
                  am::commands::Command::CommandSource::SOURCE_SDL));

  command->Init();
  command->Run();
}

TEST_F(SetDisplayLayoutRequestTest, Run_SUCCESS) {
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  CommandPtr command(CreateCommand<SetDisplayLayoutRequest>(msg));
  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, display_layout()).WillOnce(Return(kLayout));
  EXPECT_CALL(*mock_app, app_id()).WillOnce(Return(kAppId));

  EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationKey));
  EXPECT_CALL(mock_hmi_interfaces_,
              GetInterfaceFromFunction(hmi_apis::FunctionID::UI_Show))
      .WillRepeatedly(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  EXPECT_CALL(mock_hmi_interfaces_,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_AVAILABLE));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(CheckMshCorrId(kCorrelationKey), _))
      .WillOnce(Return(true));

  command->Init();
  command->Run();
}

TEST_F(SetDisplayLayoutRequestTest, OnEvent_InvalidEventId_UNSUCCESS) {
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  CommandPtr command(CreateCommand<SetDisplayLayoutRequest>(msg));

  am::event_engine::Event event(hmi_apis::FunctionID::INVALID_ENUM);

  event.set_smart_object(*msg);
  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  command->Init();
  command->on_event(event);
}

TEST_F(SetDisplayLayoutRequestTest, OnEvent_AppVersion_v6_WARNING) {
  am::event_engine::Event event(hmi_apis::FunctionID::UI_Show);
  MessageSharedPtr msg = CreateMessage();

  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[am::strings::msg_params][am::hmi_response::display_capabilities] = 0;
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  event.set_smart_object(*msg);

  CreateResponseCapabilities();
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::WARNINGS),
                          am::commands::Command::CommandSource::SOURCE_SDL));

  CommandPtr command(CreateCommand<SetDisplayLayoutRequest>(msg));
  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  ::application_manager::Version app_version;
  app_version.max_supported_api_version =
      ::application_manager::APIVersion::kAPIV6;
  EXPECT_CALL(*mock_app, version()).WillOnce(ReturnRef(app_version));

  command->Init();
  command->on_event(event);
}

TEST_F(SetDisplayLayoutRequestTest, OnEvent_AppVersion_v5_SUCCESS) {
  am::event_engine::Event event(hmi_apis::FunctionID::UI_Show);
  MessageSharedPtr msg = CreateMessage();

  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[am::strings::msg_params][am::hmi_response::display_capabilities] = 0;
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  event.set_smart_object(*msg);

  CreateResponseCapabilities();
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS),
                          am::commands::Command::CommandSource::SOURCE_SDL));

  CommandPtr command(CreateCommand<SetDisplayLayoutRequest>(msg));
  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  ::application_manager::Version app_version;
  app_version.max_supported_api_version =
      ::application_manager::APIVersion::kAPIV5;
  EXPECT_CALL(*mock_app, version()).WillOnce(ReturnRef(app_version));

  command->Init();
  command->on_event(event);
}

}  // namespace set_display_layout_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
