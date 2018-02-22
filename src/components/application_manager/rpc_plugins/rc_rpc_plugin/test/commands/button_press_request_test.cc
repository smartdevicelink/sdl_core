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

#include "gtest/gtest.h"
#include "rc_rpc_plugin/commands/mobile/button_press_request.h"
#include "rc_rpc_plugin/rc_command_factory.h"
#include "rc_rpc_plugin/rc_app_extension.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/mock/mock_resource_allocation_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"

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
using ::application_manager::Message;
using ::application_manager::MessageType;
using application_manager::commands::MessageSharedPtr;
using ::protocol_handler::MessagePriority;

namespace {
const int kModuleId = 153u;
const uint32_t kConnectionKey = 1u;
const uint32_t kAppId = 0u;
}

namespace rc_rpc_plugin_test {

using namespace rc_rpc_plugin;

class ButtonPressRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  ButtonPressRequestTest()
      : rc_capabilities_(smart_objects::SmartType_Map)
      , mock_app_(utils::MakeShared<NiceMock<MockApplication> >())
      , rc_app_extention_(
            utils::MakeShared<rc_rpc_plugin::RCAppExtension>(kModuleId)) {}

  smart_objects::SmartObject ButtonCapability(
      const mobile_apis::ButtonName::eType button_name) {
    smart_objects::SmartObject button(smart_objects::SmartType_Map);
    button["name"] = button_name;
    return button;
  }

  void SetUp() OVERRIDE {
    using namespace mobile_apis;

    std::vector<ButtonName::eType> button_names = {ButtonName::AC_MAX,
                                                   ButtonName::AC,
                                                   ButtonName::RECIRCULATE,
                                                   ButtonName::FAN_UP,
                                                   ButtonName::FAN_DOWN,
                                                   ButtonName::TEMP_UP,
                                                   ButtonName::TEMP_DOWN,
                                                   ButtonName::DEFROST_MAX,
                                                   ButtonName::DEFROST,
                                                   ButtonName::DEFROST_REAR,
                                                   ButtonName::UPPER_VENT,
                                                   ButtonName::LOWER_VENT,
                                                   ButtonName::VOLUME_UP,
                                                   ButtonName::VOLUME_DOWN,
                                                   ButtonName::EJECT,
                                                   ButtonName::SOURCE,
                                                   ButtonName::SHUFFLE,
                                                   ButtonName::REPEAT};

    smart_objects::SmartObject button_caps(smart_objects::SmartType_Array);
    for (size_t i = 0; i < button_names.size(); i++) {
      button_caps[i] = ButtonCapability(button_names[i]);
    }
    rc_capabilities_[strings::kbuttonCapabilities] = button_caps;
    ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app_));
    ON_CALL(mock_allocation_manager_, GetApplicationExtention(_))
        .WillByDefault(Return(rc_app_extention_));
    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));
    ON_CALL(app_mngr_, hmi_capabilities())
        .WillByDefault(ReturnRef(mock_hmi_capabilities_));
    ON_CALL(mock_hmi_capabilities_, rc_capability())
        .WillByDefault(Return(&rc_capabilities_));
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
  application_manager::SharedPtr<Command> CreateRCCommand(
      MessageSharedPtr& msg) {
    InitCommand(kDefaultTimeout_);
    return ::utils::MakeShared<Command>(msg ? msg : msg = CreateMessage(),
                                        app_mngr_,
                                        mock_rpc_service_,
                                        mock_hmi_capabilities_,
                                        mock_policy_handler_,
                                        mock_allocation_manager_);
  }

 protected:
  smart_objects::SmartObject rc_capabilities_;
  utils::SharedPtr<MockApplication> mock_app_;
  utils::SharedPtr<rc_rpc_plugin::RCAppExtension> rc_app_extention_;
  test::components::policy_test::MockPolicyHandlerInterface
      mock_policy_handler_;
  testing::NiceMock<rc_rpc_plugin_test::MockResourceAllocationManager>
      mock_allocation_manager_;
};

TEST_F(ButtonPressRequestTest,
       Execute_ButtonNameMatchesModuleType_ExpectCorrectMessageSentToHMI) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  NsSmartDeviceLink::NsSmartObjects::SmartObject& msg_params =
      (*mobile_message)[application_manager::strings::msg_params];
  msg_params[message_params::kModuleType] = "CLIMATE";
  msg_params[message_params::kButtonName] = "AC";
  msg_params[message_params::kButtonPressMode] = "SHORT";

  // Expectations
  const std::string resource = "CLIMATE";
  ON_CALL(mock_policy_handler_, CheckModule(_, _)).WillByDefault(Return(true));
  EXPECT_CALL(mock_allocation_manager_, IsResourceFree(resource))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_allocation_manager_, AcquireResource(resource, _))
      .WillOnce(Return(rc_rpc_plugin::AcquireResult::ALLOWED));
  EXPECT_CALL(
      mock_allocation_manager_,
      SetResourceState(resource, kAppId, rc_rpc_plugin::ResourceState::BUSY));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  HMIResultCodeIs(hmi_apis::FunctionID::Buttons_ButtonPress)))
      .WillOnce(Return(true));

  // Act
  application_manager::SharedPtr<rc_rpc_plugin::commands::ButtonPressRequest>
      command = CreateRCCommand<rc_rpc_plugin::commands::ButtonPressRequest>(
          mobile_message);
  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(
    ButtonPressRequestTest,
    Execute_ButtonNameDoesNotMatchModuleType_ExpectMessageNotSentToHMI_AndFalseSentToMobile) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  NsSmartDeviceLink::NsSmartObjects::SmartObject& msg_params =
      (*mobile_message)[application_manager::strings::msg_params];
  msg_params[message_params::kModuleType] = "RADIO";
  msg_params[message_params::kButtonName] = "AC";
  msg_params[message_params::kButtonPressMode] = "SHORT";
  // Expectations

  const std::string resource = "RADIO";
  ON_CALL(mock_policy_handler_, CheckModule(_, _)).WillByDefault(Return(true));
  EXPECT_CALL(mock_allocation_manager_, IsResourceFree(resource))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_allocation_manager_, AcquireResource(resource, _))
      .WillOnce(Return(rc_rpc_plugin::AcquireResult::ALLOWED));
  EXPECT_CALL(
      mock_allocation_manager_,
      SetResourceState(resource, kAppId, rc_rpc_plugin::ResourceState::BUSY));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::Buttons_ButtonPress))).Times(0);
  MessageSharedPtr command_result;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::INVALID_DATA), _))
      .WillOnce(DoAll(SaveArg<0>(&command_result), Return(true)));

  // Act
  application_manager::SharedPtr<rc_rpc_plugin::commands::ButtonPressRequest>
      command = CreateRCCommand<rc_rpc_plugin::commands::ButtonPressRequest>(
          mobile_message);
  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(ButtonPressRequestTest, OnEvent_ExpectSuccessfullResponseSentToMobile) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  NsSmartDeviceLink::NsSmartObjects::SmartObject& msg_params =
      (*mobile_message)[application_manager::strings::msg_params];
  msg_params[message_params::kModuleType] = "CLIMATE";
  msg_params[message_params::kButtonName] = "AC";
  msg_params[message_params::kButtonPressMode] = "SHORT";

  MessageSharedPtr hmi_message = CreateBasicMessage();
  NsSmartDeviceLink::NsSmartObjects::SmartObject& hmi_msg_params =
      (*hmi_message)[application_manager::strings::msg_params];
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::SUCCESS;
  hmi_msg_params[application_manager::hmi_response::code] = response_code;
  hmi_msg_params[application_manager::strings::connection_key] = kConnectionKey;

  // Expectations
  EXPECT_CALL(mock_allocation_manager_,
              SetResourceState(_, kAppId, rc_rpc_plugin::ResourceState::FREE));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _))
      .WillOnce(Return(true));

  // Act
  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::Buttons_ButtonPress);
  event.set_smart_object(*hmi_message);
  application_manager::SharedPtr<rc_rpc_plugin::commands::ButtonPressRequest>
      command = CreateRCCommand<rc_rpc_plugin::commands::ButtonPressRequest>(
          mobile_message);
  command->on_event(event);
}

TEST_F(ButtonPressRequestTest,
       OnEvent_ReadOnlyParamFromHMI_ExpectFalseSentToMobile) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  NsSmartDeviceLink::NsSmartObjects::SmartObject& msg_params =
      (*mobile_message)[application_manager::strings::msg_params];
  msg_params[message_params::kModuleType] = "CLIMATE";
  msg_params[message_params::kButtonName] = "AC";
  msg_params[message_params::kButtonPressMode] = "SHORT";

  MessageSharedPtr hmi_message = CreateBasicMessage();
  NsSmartDeviceLink::NsSmartObjects::SmartObject& hmi_msg_params =
      (*hmi_message)[application_manager::strings::params];
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::READ_ONLY;
  hmi_msg_params[application_manager::hmi_response::code] = response_code;
  hmi_msg_params[application_manager::strings::connection_key] = kConnectionKey;

  // Expectations
  EXPECT_CALL(mock_allocation_manager_,
              SetResourceState(_, kAppId, rc_rpc_plugin::ResourceState::FREE));

  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::GENERIC_ERROR), _))
      .WillOnce(Return(false));

  // Act
  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::Buttons_ButtonPress);
  event.set_smart_object(*hmi_message);
  application_manager::SharedPtr<rc_rpc_plugin::commands::ButtonPressRequest>
      command = CreateRCCommand<rc_rpc_plugin::commands::ButtonPressRequest>(
          mobile_message);
  command->on_event(event);
}

}  // namespace rc_rpc_plugin_test
