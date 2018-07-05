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
#include "rc_rpc_plugin/mock/mock_interior_data_cache.h"
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
using application_manager::ApplicationSet;
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
const int32_t kConnectionKey = 5u;
const uint32_t kAppId = 0u;
const int kModuleId = 153u;
}

namespace rc_rpc_plugin_test {

using namespace rc_rpc_plugin;
class GetInteriorVehicleDataRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  GetInteriorVehicleDataRequestTest()
      : mock_app_(utils::MakeShared<NiceMock<MockApplication> >())
      , mock_app2_(utils::MakeShared<NiceMock<MockApplication> >())
      , rc_app_extention_(utils::MakeShared<RCAppExtension>(kModuleId))
      , rc_app_extention2_(utils::MakeShared<RCAppExtension>(kModuleId))
      , apps_da_(apps_, apps_lock_) {
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
    ON_CALL(mock_policy_handler_,
            CheckHMIType(
                _, mobile_apis::AppHMIType::eType::REMOTE_CONTROL, nullptr))
        .WillByDefault(Return(true));
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
                                        mock_allocation_manager_,
                                        mock_interior_data_cache_);
  }

 protected:
  smart_objects::SmartObject rc_capabilities_;
  utils::SharedPtr<MockApplication> mock_app_;
  utils::SharedPtr<MockApplication> mock_app2_;
  utils::SharedPtr<RCAppExtension> rc_app_extention_;
  utils::SharedPtr<RCAppExtension> rc_app_extention2_;
  testing::NiceMock<rc_rpc_plugin_test::MockResourceAllocationManager>
      mock_allocation_manager_;
  testing::NiceMock<rc_rpc_plugin_test::MockInteriorDataCache>
      mock_interior_data_cache_;
  application_manager::ApplicationSet apps_;
  const sync_primitives::Lock apps_lock_;
  DataAccessor<application_manager::ApplicationSet> apps_da_;
};

TEST_F(GetInteriorVehicleDataRequestTest,
       Execute_ExpectCorrectMessageSentToHMI_NoSubscriptionNoCache) {
  // Arrange
  auto module_type = mobile_apis::ModuleType::RADIO;
  MessageSharedPtr mobile_message = CreateBasicMessage();
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleType] = module_type;
  // Expectations
  ON_CALL(mock_policy_handler_, CheckModule(_, _)).WillByDefault(Return(true));
  EXPECT_CALL(mock_hmi_capabilities_, rc_capability())
      .WillOnce(Return(nullptr));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::RC_GetInteriorVehicleData)))
      .WillOnce(Return(true));
  // Assertions
  application_manager::SharedPtr<
      rc_rpc_plugin::commands::GetInteriorVehicleDataRequest> command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);
  command->Run();
}

TEST_F(
    GetInteriorVehicleDataRequestTest,
    Execute_ExpectMessageNotSentToHMI_AndSuccessSentToMobile_AppSubscribed_DataFromCache) {
  // Arrange
  auto module_type = mobile_apis::ModuleType::RADIO;
  MessageSharedPtr mobile_message = CreateBasicMessage();
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleType] = module_type;
  rc_app_extention_->SubscribeToInteriorVehicleData(enums_value::kRadio);
  smart_objects::SmartObject radio_data;
  radio_data[message_params::kBand] = enums_value::kAM;
  // Expectations
  ON_CALL(mock_policy_handler_, CheckModule(_, enums_value::kRadio))
      .WillByDefault(Return(true));
  EXPECT_CALL(mock_hmi_capabilities_, rc_capability())
      .WillOnce(Return(nullptr));
  ON_CALL(*mock_app_, QueryInterface(_))
      .WillByDefault(Return(rc_app_extention_));
  EXPECT_CALL(mock_interior_data_cache_, Contains(enums_value::kRadio))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_interior_data_cache_, Retrieve(enums_value::kRadio))
      .WillOnce(Return(radio_data));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _))
      .WillOnce(Return(true));
  // Assertions
  application_manager::SharedPtr<
      rc_rpc_plugin::commands::GetInteriorVehicleDataRequest> command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);

  command->Run();
}

TEST_F(
    GetInteriorVehicleDataRequestTest,
    Execute_ExpectCorrectMessageSentToHMI_AppSubscribedUnsubscibe_ClearCache) {
  // Arrange
  auto module_type = mobile_apis::ModuleType::RADIO;
  MessageSharedPtr mobile_message = CreateBasicMessage();
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleType] = module_type;
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kSubscribe] = false;
  // Expectations
  ON_CALL(mock_policy_handler_, CheckModule(_, enums_value::kRadio))
      .WillByDefault(Return(true));
  EXPECT_CALL(mock_hmi_capabilities_, rc_capability())
      .WillOnce(Return(nullptr));
  ON_CALL(*mock_app_, QueryInterface(_))
      .WillByDefault(Return(rc_app_extention_));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::RC_GetInteriorVehicleData)))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  // Assertions
  application_manager::SharedPtr<
      rc_rpc_plugin::commands::GetInteriorVehicleDataRequest> command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);
  command->Run();
}

TEST_F(GetInteriorVehicleDataRequestTest,
       Execute_ExpectCorrectMessageSentToHMI_TwoApps_OneUnsubscribed) {
  // Arrange
  auto module_type = mobile_apis::ModuleType::RADIO;
  MessageSharedPtr mobile_message = CreateBasicMessage();
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleType] = module_type;
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kSubscribe] = false;
  apps_.insert(mock_app_);
  apps_.insert(mock_app2_);
  rc_app_extention_->SubscribeToInteriorVehicleData(enums_value::kRadio);
  rc_app_extention2_->SubscribeToInteriorVehicleData(enums_value::kRadio);
  smart_objects::SmartObject radio_data;
  radio_data[message_params::kBand] = enums_value::kAM;

  // Expectations
  ON_CALL(mock_policy_handler_, CheckModule(_, enums_value::kRadio))
      .WillByDefault(Return(true));
  EXPECT_CALL(mock_hmi_capabilities_, rc_capability())
      .WillOnce(Return(nullptr));
  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(apps_da_));
  ON_CALL(*mock_app_, QueryInterface(_))
      .WillByDefault(Return(rc_app_extention_));
  ON_CALL(*mock_app2_, QueryInterface(_))
      .WillByDefault(Return(rc_app_extention2_));
  EXPECT_CALL(mock_interior_data_cache_, Contains(enums_value::kRadio))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_interior_data_cache_, Retrieve(enums_value::kRadio))
      .WillOnce(Return(radio_data));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _))
      .WillOnce(Return(true));
  // Assertions
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
  msg_params[message_params::kModuleType] = mobile_apis::ModuleType::RADIO;
  // Expectations
  ON_CALL(mock_policy_handler_, CheckModule(_, _)).WillByDefault(Return(true));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::RC_GetInteriorVehicleData))).Times(0);
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::UNSUPPORTED_RESOURCE),
                  _)).WillOnce((Return(true)));
  // Assertions
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
  msg_params[message_params::kModuleType] = mobile_apis::ModuleType::CLIMATE;

  MessageSharedPtr hmi_message = CreateBasicMessage();
  NsSmartDeviceLink::NsSmartObjects::SmartObject& hmi_msg_params =
      (*hmi_message)[application_manager::strings::msg_params];
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::SUCCESS;
  hmi_msg_params[application_manager::hmi_response::code] = response_code;
  hmi_msg_params[application_manager::strings::connection_key] = kConnectionKey;
  // Expectations
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _))
      .WillOnce(Return(true));
  // Assertions
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
  msg_params[message_params::kModuleType] = mobile_apis::ModuleType::CLIMATE;

  MessageSharedPtr hmi_message = CreateBasicMessage();
  NsSmartDeviceLink::NsSmartObjects::SmartObject& hmi_msg_params =
      (*hmi_message)[application_manager::strings::params];
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::READ_ONLY;
  hmi_msg_params[application_manager::hmi_response::code] = response_code;
  hmi_msg_params[application_manager::strings::connection_key] = kConnectionKey;
  // Expectations
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::GENERIC_ERROR), _))
      .WillOnce(Return(false));
  // Assertions
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
