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

#include "application_manager/application.h"
#include "application_manager/mock_application.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/rpc_service_impl.h"
#include "application_manager/mock_request_controller_settings.h"
#include "application_manager/include/application_manager/command_holder_impl.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/request_controller.h"
#include "application_manager/mock_rpc_plugin_manager.h"
#include "application_manager/mock_rpc_plugin.h"
#include "application_manager/mock_command_factory.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/message.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/mock/mock_resource_allocation_manager.h"
#include "rc_rpc_plugin/mock/mock_interior_data_cache.h"
#include "rc_rpc_plugin/commands/mobile/button_press_request.h"
#include "rc_rpc_plugin/commands/hmi/rc_get_interior_vehicle_data_consent_response.h"
#include "rc_rpc_plugin/commands/hmi/rc_get_interior_vehicle_data_consent_request.h"
#include "hmi_message_handler/mock_hmi_message_handler.h"
#include "interfaces/MOBILE_API.h"
#include "include/test/protocol_handler/mock_protocol_handler.h"
#include "test/application_manager/mock_application_manager_settings.h"
#include "utils/make_shared.h"

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;
using ::testing::SaveArg;
using ::testing::Mock;
using ::testing::NiceMock;
using ::utils::SharedPtr;
namespace am = ::application_manager;
using am::ApplicationSet;
using am::commands::MessageSharedPtr;
using am::commands::CommandImpl;
using am::plugin_manager::RPCPlugin;
using ::smart_objects::SmartObject;
using rc_rpc_plugin::commands::ButtonPressRequest;
using rc_rpc_plugin::commands::RCGetInteriorVehicleDataConsentResponse;
using rc_rpc_plugin::commands::RCGetInteriorVehicleDataConsentRequest;
using rc_rpc_plugin::RCRPCPlugin;
using test::components::commands_test::CommandsTestMocks;
using test::components::commands_test::CommandRequestTest;
using am::plugin_manager::MockRPCPluginManager;
using am::plugin_manager::MockRPCPlugin;
using test::components::application_manager_test::MockApplication;
using test::components::application_manager_test::MockRequestControlerSettings;
using test::components::protocol_handler_test::MockProtocolHandler;
using test::components::hmi_message_handler_test::MockHMIMessageHandler;
using test::components::application_manager_test::MockHMICapabilities;
using test::components::application_manager_test::MockCommandFactory;
using test::components::policy_test::MockPolicyHandlerInterface;

namespace rc_rpc_plugin_test {
using namespace rc_rpc_plugin;

namespace {
const uint32_t kConnectionKey = 2u;
const uint32_t kAppId = 5u;
const std::string kResource = "CLIMATE";
const uint32_t kPluginID = RCRPCPlugin::kRCPluginID;
}  // namespace

class RCGetInteriorVehicleDataConsentTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  RCGetInteriorVehicleDataConsentTest()
      : mock_app_(utils::MakeShared<NiceMock<MockApplication> >())
      , command_holder(app_mngr_)
      , request_controller(mock_request_controler)
      , rpc_service(app_mngr_,
                    request_controller,
                    &mock_protocol_handler,
                    &mock_hmi_handler,
                    command_holder)
      , rc_app_extention_(utils::MakeShared<RCAppExtension>(kPluginID))
      , mock_rpc_plugin_manager(
            utils::MakeShared<NiceMock<MockRPCPluginManager> >())
      , rpc_plugin(mock_rpc_plugin)
      , optional_mock_rpc_plugin(mock_rpc_plugin) {
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));
    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(mock_hmi_interfaces_));
    ON_CALL(
        mock_hmi_interfaces_,
        GetInterfaceState(application_manager::HmiInterfaces::HMI_INTERFACE_RC))
        .WillByDefault(Return(application_manager::HmiInterfaces::
                                  InterfaceState::STATE_AVAILABLE));
    ON_CALL(app_mngr_, application(kAppId)).WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, QueryInterface(RCRPCPlugin::kRCPluginID))
        .WillByDefault(Return(rc_app_extention_));
    testing::NiceMock<rc_rpc_plugin_test::MockInteriorDataCache>
        mock_interior_data_cache_;
    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));
    ON_CALL(app_mngr_, hmi_capabilities())
        .WillByDefault(ReturnRef(mock_hmi_capabilities_));
    ON_CALL(mock_hmi_capabilities_, rc_capability())
        .WillByDefault(Return(&rc_capabilities_));
    ON_CALL(mock_policy_handler_,
            CheckHMIType(
                _, mobile_apis::AppHMIType::eType::REMOTE_CONTROL, nullptr))
        .WillByDefault(Return(true));
    ON_CALL(mock_policy_handler_, CheckModule(_, _))
        .WillByDefault(Return(true));
    ON_CALL(app_mngr_, GetPluginManager())
        .WillByDefault(ReturnRef(*mock_rpc_plugin_manager));
    ON_CALL(*mock_rpc_plugin_manager, FindPluginToProcess(_, _))
        .WillByDefault(Return(rpc_plugin));
    ON_CALL(mock_allocation_manager_, IsResourceFree(kResource))
        .WillByDefault(Return(true));
  }

  template <class Command>
  application_manager::SharedPtr<Command> CreateRCCommand(
      MessageSharedPtr& msg) {
    InitCommand(kDefaultTimeout_);
    return ::utils::MakeShared<Command>(msg ? msg : msg = CreateMessage(),
                                        app_mngr_,
                                        rpc_service,
                                        mock_hmi_capabilities_,
                                        mock_policy_handler_,
                                        mock_allocation_manager_,
                                        mock_interior_data_cache_);
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

 protected:
  utils::SharedPtr<MockApplication> mock_app_;
  MockRequestControlerSettings mock_request_controler;
  MockProtocolHandler mock_protocol_handler;
  MockHMIMessageHandler mock_hmi_handler;
  am::CommandHolderImpl command_holder;
  testing::NiceMock<rc_rpc_plugin_test::MockResourceAllocationManager>
      mock_allocation_manager_;
  testing::NiceMock<rc_rpc_plugin_test::MockInteriorDataCache>
      mock_interior_data_cache_;
  smart_objects::SmartObject rc_capabilities_;
  MockRPCPlugin mock_rpc_plugin;
  MockCommandFactory mock_command_factory;
  am::request_controller::RequestController request_controller;
  am::rpc_service::RPCServiceImpl rpc_service;
  utils::SharedPtr<RCAppExtension> rc_app_extention_;
  utils::SharedPtr<am::plugin_manager::MockRPCPluginManager>
      mock_rpc_plugin_manager;
  utils::Optional<RPCPlugin> rpc_plugin;
  utils::Optional<MockRPCPlugin> optional_mock_rpc_plugin;
};

TEST_F(RCGetInteriorVehicleDataConsentTest,
       Run_MobileSendButtonPressMessage_HMISendASKDRIVERModeToMobile) {
  // Arrange
  auto mobile_message = CreateBasicMessage();

  // Expectations
  EXPECT_CALL(mock_allocation_manager_, AcquireResource(_, _))
      .WillOnce(Return(rc_rpc_plugin::AcquireResult::ASK_DRIVER));
  EXPECT_CALL(*optional_mock_rpc_plugin, GetCommandFactory())
      .WillOnce(ReturnRef(mock_command_factory));
  auto rc_consent_request =
      CreateRCCommand<commands::RCGetInteriorVehicleDataConsentRequest>(
          mobile_message);
  EXPECT_CALL(mock_command_factory, CreateCommand(_, _))
      .WillOnce(Return(rc_consent_request));
  EXPECT_CALL(mock_hmi_interfaces_,
              GetInterfaceFromFunction(
                  hmi_apis::FunctionID::RC_GetInteriorVehicleDataConsent))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_RC));

  // Act
  auto command = CreateRCCommand<commands::ButtonPressRequest>(mobile_message);
  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(RCGetInteriorVehicleDataConsentTest,
       Run_MobileSendButtonPressMessage_HMISendINUSEModeToMobile) {
  // Arrange
  auto mobile_message = CreateBasicMessage();

  // Expectations
  EXPECT_CALL(mock_allocation_manager_, AcquireResource(_, _))
      .WillOnce(Return(rc_rpc_plugin::AcquireResult::IN_USE));

  EXPECT_CALL(*optional_mock_rpc_plugin, GetCommandFactory())
      .WillOnce(ReturnRef(mock_command_factory));
  auto rc_consent_response =
      CreateRCCommand<commands::RCGetInteriorVehicleDataConsentResponse>(
          mobile_message);
  EXPECT_CALL(mock_command_factory, CreateCommand(_, _))
      .WillOnce(Return(rc_consent_response));
  auto command = CreateRCCommand<commands::ButtonPressRequest>(mobile_message);

  // Act
  ASSERT_TRUE(command->Init());
  command->Run();
}

}  // namespace rc_rpc_plugin_test
