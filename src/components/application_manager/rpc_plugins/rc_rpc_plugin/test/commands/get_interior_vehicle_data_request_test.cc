/*
 * Copyright (c) 2017, Ford Motor Company
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

#include "rc_rpc_plugin/commands/mobile/get_interior_vehicle_data_request.h"
#include "gtest/gtest.h"
#include "application_manager/mock_application.h"
#include "rc_rpc_plugin/rc_app_extension.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "application_manager/message_helper.h"
#include "rc_rpc_plugin/rc_command_factory.h"
#include "application_manager/event_engine/event_dispatcher.h"
#include "application_manager/commands/command_request_test.h"
#include "rc_rpc_plugin/mock/mock_resource_allocation_manager.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"

using ::testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SaveArg;
using ::application_manager::Message;
using ::application_manager::MessageType;
using application_manager::commands::MessageSharedPtr;
using ::application_manager::ApplicationSharedPtr;
using ::protocol_handler::MessagePriority;
using test::components::application_manager_test::MockApplication;
using test::components::application_manager_test::MockApplicationManager;
using test::components::commands_test::MockApplicationManager;
using test::components::commands_test::CommandRequestTest;
using test::components::commands_test::CommandsTestMocks;
using test::components::commands_test::HMIResultCodeIs;
using test::components::commands_test::MobileResultCodeIs;

namespace {
const int32_t kConnectionKey = 5;
const uint32_t kAppId = 0;
const int kModuleId = 153;
const std::string kInvalidMobileRequest = "{{\"moduleTip\":\"LUXOFT\"}}";
const std::string kValidMobileRequest =
    "{\"subscribe\":true,\"moduleDescription\":{\"moduleType\":\"CLIMATE\"}}";
const std::string kValidHmiResponse =
    "{\"jsonrpc\":\"2.0\",\"id\":51,\"result\":{\"code\":0,\"method\":\"RC."
    "GetInteriorVehicleData\",\"moduleData\":{\"moduleType\":\"CLIMATE\","
    "\"climateControlData\":{\"fanSpeed\":0,\"currentTemperature\":{\"unit\":"
    "\"CELSIUS\",\"value\":20},\"desiredTemperature\":{\"unit\":\"CELSIUS\","
    "\"value\":25},\"acEnable\":true,\"circulateAirEnable\":true,"
    "\"autoModeEnable\":true,\"defrostZone\":\"ALL\",\"dualModeEnable\":true,"
    "\"acMaxEnable\":true,\"ventilationMode\":\"BOTH\"}},\"isSubscribed\":"
    "true}}";
const std::string kInvalidHmiResponse =
    "{\"jsonrpc\":\"2.0\",\"id\":51,\"result\":{\"code\":21,\"method\":\"RC."
    "GetInteriorVehicleData\",\"moduleData\":{\"moduleType\":\"CLIMATE\","
    "\"ControlData\":{\"Speed\":0,\"outsideTemperature\":{\"unit\":"
    "\"CELSIUS\",\"value\":\"high\"},\"desiredTemperature\":{\"unit\":"
    "\"CELSIUS\","
    "\"value\":25},\"acEnable\":true,\"circulateAirEnable\":true,"
    "\"autoModeEnable\":true,\"defrostZone\":\"ALL\",\"dualModeEnable\":true,"
    "\"acMaxEnable\":true,\"ventilationMode\":\"BOTH\"}},\"isSubscribed\":"
    "false}}";
}

namespace rc_rpc_plugin_test {

using namespace rc_rpc_plugin;
class GetInteriorVehicleDataRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  GetInteriorVehicleDataRequestTest()
      : mock_app_(utils::MakeShared<NiceMock<MockApplication> >())
      , rc_app_extention_(utils::MakeShared<RCAppExtension>(kModuleId)) {
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));
    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(mock_hmi_interfaces_));

    ON_CALL(
        mock_hmi_interfaces_,
        GetInterfaceState(application_manager::HmiInterfaces::HMI_INTERFACE_RC))
        .WillByDefault(Return(application_manager::HmiInterfaces::
                                  InterfaceState::STATE_AVAILABLE));
    ON_CALL(app_mngr_, application(kAppId)).WillByDefault(Return(mock_app_));
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
                  mobile_apis::FunctionID::GetInteriorVehicleDataID;
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
  utils::SharedPtr<RCAppExtension> rc_app_extention_;
  testing::NiceMock<rc_rpc_plugin_test::MockResourceAllocationManager>
      mock_allocation_manager_;
};
TEST_F(GetInteriorVehicleDataRequestTest,
       Execute_MessageValidationOk_ExpectCorrectMessageSentToHMI) {
  // Arrange
  ON_CALL(mock_policy_handler_, CheckModule(_, _)).WillByDefault(Return(true));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::RC_GetInteriorVehicleData)))
      .WillOnce(Return(true));
  MessageSharedPtr mobile_message = CreateBasicMessage();
  application_manager::SharedPtr<
      rc_rpc_plugin::commands::GetInteriorVehicleDataRequest> command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);
  command->Run();
}

TEST_F(
    GetInteriorVehicleDataRequestTest,
    Execute_MessageValidationFailed_ExpectMessageNotSentToHMI_AndFalseSentToMobile) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  NsSmartDeviceLink::NsSmartObjects::SmartObject& msg_params =
      (*mobile_message)[application_manager::strings::msg_params];
  msg_params[message_params::kModuleType] = "RADIO";
  ON_CALL(mock_policy_handler_, CheckModule(_, _)).WillByDefault(Return(true));
  MessageSharedPtr command_result;
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::RC_GetInteriorVehicleData))).Times(0);
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_apis::Result::UNSUPPORTED_RESOURCE), _))
      .WillOnce(DoAll(SaveArg<0>(&command_result), Return(true)));
  application_manager::SharedPtr<
      rc_rpc_plugin::commands::GetInteriorVehicleDataRequest> command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);
  command->Run();
}

TEST_F(GetInteriorVehicleDataRequestTest,
       OnEvent_ValidHmiResponse_ExpectSuccessfullResponseSentToMobile) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  NsSmartDeviceLink::NsSmartObjects::SmartObject& msg_params =
      (*mobile_message)[application_manager::strings::msg_params];
  msg_params[message_params::kModuleType] = "CLIMATE";

  MessageSharedPtr hmi_message = CreateBasicMessage();
  NsSmartDeviceLink::NsSmartObjects::SmartObject& hmi_msg_params =
      (*hmi_message)[application_manager::strings::msg_params];
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::SUCCESS;
  hmi_msg_params[application_manager::hmi_response::code] = response_code;
  hmi_msg_params[application_manager::strings::connection_key] = kConnectionKey;

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _))
      .WillOnce(Return(true));

  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::RC_GetInteriorVehicleData);
  event.set_smart_object(*hmi_message);
  application_manager::SharedPtr<
      rc_rpc_plugin::commands::GetInteriorVehicleDataRequest> command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);
  command->on_event(event);
}

TEST_F(GetInteriorVehicleDataRequestTest,
       OnEvent_InvalidHmiResponse_ExpectGenericErrorResponseSentToMobile) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  NsSmartDeviceLink::NsSmartObjects::SmartObject& msg_params =
      (*mobile_message)[application_manager::strings::msg_params];
  msg_params[message_params::kModuleType] = "CLIMATE";

  MessageSharedPtr hmi_message = CreateBasicMessage();
  NsSmartDeviceLink::NsSmartObjects::SmartObject& hmi_msg_params =
      (*hmi_message)[application_manager::strings::params];
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::READ_ONLY;
  hmi_msg_params[application_manager::hmi_response::code] = response_code;
  hmi_msg_params[application_manager::strings::connection_key] = kConnectionKey;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::GENERIC_ERROR), _))
      .WillOnce(Return(false));

  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::RC_GetInteriorVehicleData);
  event.set_smart_object(*hmi_message);
  application_manager::SharedPtr<
      rc_rpc_plugin::commands::GetInteriorVehicleDataRequest> command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);
  command->on_event(event);
}
}  // namespace rc_rpc_plugin_test
