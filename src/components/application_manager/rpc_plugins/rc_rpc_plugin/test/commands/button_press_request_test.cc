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

#include "rc_rpc_plugin/commands/mobile/button_press_request.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "gtest/gtest.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "rc_rpc_plugin/mock/mock_interior_data_cache.h"
#include "rc_rpc_plugin/mock/mock_interior_data_manager.h"
#include "rc_rpc_plugin/mock/mock_rc_capabilities_manager.h"
#include "rc_rpc_plugin/mock/mock_rc_consent_manager.h"
#include "rc_rpc_plugin/mock/mock_resource_allocation_manager.h"
#include "rc_rpc_plugin/rc_app_extension.h"
#include "rc_rpc_plugin/rc_command_factory.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"

using ::application_manager::Message;
using ::application_manager::MessageType;
using application_manager::commands::MessageSharedPtr;
using ::protocol_handler::MessagePriority;
using test::components::application_manager_test::MockApplication;
using test::components::commands_test::CommandRequestTest;
using test::components::commands_test::CommandsTestMocks;
using test::components::commands_test::HMIResultCodeIs;
using test::components::commands_test::MobileResultCodeIs;
using ::testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SaveArg;

namespace {
const int kModuleId = 153u;
const uint32_t kConnectionKey = 1u;
const uint32_t kAppId = 0u;
const std::string kPolicyAppId = "Test";
}  // namespace

namespace rc_rpc_plugin_test {

using namespace rc_rpc_plugin;

class ButtonPressRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  ButtonPressRequestTest()
      : rc_capabilities_(std::make_shared<smart_objects::SmartObject>(
            smart_objects::SmartType_Map))
      , mock_app_(std::make_shared<NiceMock<MockApplication> >())
      , rc_app_extention_(
            std::make_shared<rc_rpc_plugin::RCAppExtension>(kModuleId)) {}

  void SetUp() OVERRIDE {
    smart_objects::SmartObject control_caps((smart_objects::SmartType_Array));
    (*rc_capabilities_)[strings::kradioControlCapabilities] = control_caps;
    ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, QueryInterface(RCRPCPlugin::kRCPluginID))
        .WillByDefault(Return(rc_app_extention_));
    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));
    ON_CALL(app_mngr_, hmi_capabilities())
        .WillByDefault(ReturnRef(mock_hmi_capabilities_));
    ON_CALL(mock_hmi_capabilities_, rc_capability())
        .WillByDefault(Return(rc_capabilities_));
    ON_CALL(*mock_app_, policy_app_id()).WillByDefault(Return(kPolicyAppId));
    ON_CALL(mock_policy_handler_,
            CheckHMIType(kPolicyAppId,
                         mobile_apis::AppHMIType::eType::REMOTE_CONTROL,
                         nullptr))
        .WillByDefault(Return(true));
    ON_CALL(mock_allocation_manager_, is_rc_enabled())
        .WillByDefault(Return(true));
    ON_CALL(mock_rc_capabilities_manager_, CheckButtonName(_, _))
        .WillByDefault(Return(true));
    ON_CALL(mock_rc_capabilities_manager_, CheckIfModuleExistsInCapabilities(_))
        .WillByDefault(Return(true));
    ON_CALL(mock_rc_capabilities_manager_, CheckIfButtonExistInRCCaps(_))
        .WillByDefault(Return(true));
  }

  MessageSharedPtr CreateBasicMessage() {
    MessageSharedPtr message = CreateMessage();
    (*message)[application_manager::strings::params]
              [application_manager::strings::function_id] =
                  mobile_apis::FunctionID::ButtonPressID;
    (*message)[application_manager::strings::params]
              [application_manager::strings::connection_key] = kConnectionKey;
    (*message)[application_manager::strings::params]
              [application_manager::strings::connection_key] = kAppId;
    return message;
  }

  template <class Command>
  std::shared_ptr<Command> CreateRCCommand(MessageSharedPtr& msg) {
    InitCommand(kDefaultTimeout_);
    RCCommandParams params{app_mngr_,
                           mock_rpc_service_,
                           mock_hmi_capabilities_,
                           mock_policy_handler_,
                           mock_allocation_manager_,
                           mock_interior_data_cache_,
                           mock_interior_data_manager_,
                           mock_rc_capabilities_manager_,
                           mock_rc_consent_manger_};
    return std::make_shared<Command>(msg ? msg : msg = CreateMessage(), params);
  }

 protected:
  smart_objects::SmartObjectSPtr rc_capabilities_;
  std::shared_ptr<MockApplication> mock_app_;
  std::shared_ptr<rc_rpc_plugin::RCAppExtension> rc_app_extention_;
  test::components::policy_test::MockPolicyHandlerInterface
      mock_policy_handler_;
  testing::NiceMock<rc_rpc_plugin_test::MockResourceAllocationManager>
      mock_allocation_manager_;
  testing::NiceMock<rc_rpc_plugin_test::MockInteriorDataCache>
      mock_interior_data_cache_;
  testing::NiceMock<rc_rpc_plugin_test::MockInteriorDataManager>
      mock_interior_data_manager_;
  testing::NiceMock<rc_rpc_plugin_test::MockRCCapabilitiesManager>
      mock_rc_capabilities_manager_;
  testing::NiceMock<MockRCConsentManager> mock_rc_consent_manger_;
};

TEST_F(ButtonPressRequestTest,
       Execute_ButtonNameMatchesModuleType_ExpectCorrectMessageSentToHMI) {
  // Arrange
  const std::string resource = "CLIMATE";
  const std::string resource_id = "id1";
  MessageSharedPtr mobile_message = CreateBasicMessage();
  ns_smart_device_link::ns_smart_objects::SmartObject& msg_params =
      (*mobile_message)[application_manager::strings::msg_params];
  msg_params[message_params::kModuleType] = mobile_apis::ModuleType::CLIMATE;
  msg_params[message_params::kModuleId] = resource_id;
  msg_params[message_params::kButtonName] = mobile_apis::ButtonName::AC;
  msg_params[message_params::kButtonPressMode] =
      mobile_apis::ButtonPressMode::SHORT;

  // Expectations
  ON_CALL(mock_policy_handler_, CheckModule(_, _)).WillByDefault(Return(true));

  EXPECT_CALL(mock_allocation_manager_, IsResourceFree(resource, resource_id))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_allocation_manager_, AcquireResource(resource, _, _))
      .WillOnce(Return(rc_rpc_plugin::AcquireResult::ALLOWED));
  EXPECT_CALL(
      mock_allocation_manager_,
      SetResourceState(
          resource, resource_id, kAppId, rc_rpc_plugin::ResourceState::BUSY));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::Buttons_ButtonPress), _))
      .WillOnce(Return(true));

  // Act
  std::shared_ptr<rc_rpc_plugin::commands::ButtonPressRequest> command =
      CreateRCCommand<rc_rpc_plugin::commands::ButtonPressRequest>(
          mobile_message);
  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(
    ButtonPressRequestTest,
    Execute_ButtonNameDoesNotMatchModuleType_ExpectMessageNotSentToHMI_AndFalseSentToMobile) {
  // Arrange
  const std::string resource = "RADIO";
  const std::string resource_id = "id1";
  MessageSharedPtr mobile_message = CreateBasicMessage();
  ns_smart_device_link::ns_smart_objects::SmartObject& msg_params =
      (*mobile_message)[application_manager::strings::msg_params];
  msg_params[message_params::kModuleType] = mobile_apis::ModuleType::RADIO;
  msg_params[message_params::kModuleId] = resource_id;
  msg_params[message_params::kButtonName] = mobile_apis::ButtonName::AC;
  msg_params[message_params::kButtonPressMode] =
      mobile_apis::ButtonPressMode::SHORT;
  // Expectations

  EXPECT_CALL(mock_rc_capabilities_manager_, CheckButtonName(_, _))
      .WillOnce(Return(false));
  ON_CALL(mock_policy_handler_, CheckModule(_, _)).WillByDefault(Return(true));
  EXPECT_CALL(mock_allocation_manager_, IsResourceFree(resource, resource_id))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_allocation_manager_, AcquireResource(resource, _, _))
      .WillOnce(Return(rc_rpc_plugin::AcquireResult::ALLOWED));
  EXPECT_CALL(mock_allocation_manager_,
              SetResourceState(resource, resource_id, kAppId, _))
      .Times(2);

  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::Buttons_ButtonPress), _))
      .Times(0);
  MessageSharedPtr command_result;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::INVALID_DATA), _))
      .WillOnce(DoAll(SaveArg<0>(&command_result), Return(true)));

  // Act
  std::shared_ptr<rc_rpc_plugin::commands::ButtonPressRequest> command =
      CreateRCCommand<rc_rpc_plugin::commands::ButtonPressRequest>(
          mobile_message);
  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(ButtonPressRequestTest, OnEvent_ExpectSuccessfullResponseSentToMobile) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  ns_smart_device_link::ns_smart_objects::SmartObject& msg_params =
      (*mobile_message)[application_manager::strings::msg_params];
  msg_params[message_params::kModuleType] = mobile_apis::ModuleType::CLIMATE;
  msg_params[message_params::kButtonName] = mobile_apis::ButtonName::AC;
  msg_params[message_params::kButtonPressMode] =
      mobile_apis::ButtonPressMode::SHORT;

  MessageSharedPtr hmi_message = CreateBasicMessage();
  ns_smart_device_link::ns_smart_objects::SmartObject& hmi_msg_params =
      (*hmi_message)[application_manager::strings::msg_params];
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::SUCCESS;
  hmi_msg_params[application_manager::hmi_response::code] = response_code;
  hmi_msg_params[application_manager::strings::connection_key] = kConnectionKey;

  // Expectations
  EXPECT_CALL(
      mock_allocation_manager_,
      SetResourceState(_, _, kAppId, rc_rpc_plugin::ResourceState::FREE))
      .Times(1);

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _))
      .WillOnce(Return(true));

  // Act
  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::Buttons_ButtonPress);
  event.set_smart_object(*hmi_message);
  std::shared_ptr<rc_rpc_plugin::commands::ButtonPressRequest> command =
      CreateRCCommand<rc_rpc_plugin::commands::ButtonPressRequest>(
          mobile_message);
  command->on_event(event);
}

TEST_F(ButtonPressRequestTest,
       OnEvent_ReadOnlyParamFromHMI_ExpectFalseSentToMobile) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  ns_smart_device_link::ns_smart_objects::SmartObject& msg_params =
      (*mobile_message)[application_manager::strings::msg_params];
  msg_params[message_params::kModuleType] = mobile_apis::ModuleType::CLIMATE;
  msg_params[message_params::kButtonName] = mobile_apis::ButtonName::AC;
  msg_params[message_params::kButtonPressMode] =
      mobile_apis::ButtonPressMode::SHORT;

  MessageSharedPtr hmi_message = CreateBasicMessage();
  ns_smart_device_link::ns_smart_objects::SmartObject& hmi_msg_params =
      (*hmi_message)[application_manager::strings::params];
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::READ_ONLY;
  hmi_msg_params[application_manager::hmi_response::code] = response_code;
  hmi_msg_params[application_manager::strings::connection_key] = kConnectionKey;

  // Expectations
  EXPECT_CALL(
      mock_allocation_manager_,
      SetResourceState(_, _, kAppId, rc_rpc_plugin::ResourceState::FREE))
      .Times(1);

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::GENERIC_ERROR), _))
      .WillOnce(Return(false));

  // Act
  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::Buttons_ButtonPress);
  event.set_smart_object(*hmi_message);
  std::shared_ptr<rc_rpc_plugin::commands::ButtonPressRequest> command =
      CreateRCCommand<rc_rpc_plugin::commands::ButtonPressRequest>(
          mobile_message);
  command->on_event(event);
}

}  // namespace rc_rpc_plugin_test
