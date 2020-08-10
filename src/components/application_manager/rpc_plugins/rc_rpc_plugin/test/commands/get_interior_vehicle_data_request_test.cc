/*
 * Copyright (c) 2019, Ford Motor Company
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
#include "application_manager/commands/command_request_test.h"
#include "application_manager/event_engine/event_dispatcher.h"
#include "application_manager/message_helper.h"
#include "application_manager/mock_application.h"
#include "gtest/gtest.h"
#include "rc_rpc_plugin/mock/mock_interior_data_cache.h"
#include "rc_rpc_plugin/mock/mock_interior_data_manager.h"
#include "rc_rpc_plugin/mock/mock_rc_capabilities_manager.h"
#include "rc_rpc_plugin/mock/mock_rc_consent_manager.h"
#include "rc_rpc_plugin/mock/mock_resource_allocation_manager.h"
#include "rc_rpc_plugin/rc_app_extension.h"
#include "rc_rpc_plugin/rc_command_factory.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"

#include <stdint.h>
#include <chrono>
#include <thread>

using application_manager::ApplicationSet;
using ::application_manager::ApplicationSharedPtr;
using ::application_manager::Message;
using ::application_manager::MessageType;
using application_manager::commands::MessageSharedPtr;
using ::protocol_handler::MessagePriority;
using test::components::application_manager_test::MockApplication;
using test::components::application_manager_test::MockApplicationManager;
using test::components::commands_test::CommandRequestTest;
using test::components::commands_test::CommandsTestMocks;
using test::components::commands_test::HMIResultCodeIs;
using test::components::commands_test::MobileResultCodeIs;
using test::components::commands_test::MockApplicationManager;
using ::testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SaveArg;
using ::testing::StrictMock;

namespace {
const int32_t kConnectionKey = 5u;
const uint32_t kAppId = 0u;
const uint32_t kAppId2 = 1u;
const int kModuleId = 153u;
const auto module_eType = mobile_apis::ModuleType::RADIO;
const auto module_type = "RADIO";
const auto module_id = "eb7739ea-b263-4fe1-af9c-9311d1acac2d";
const int32_t time_frame_of_allowed_requests = 1;
const uint32_t max_request_in_time_frame = 5u;
}  // namespace

namespace rc_rpc_plugin_test {

using namespace rc_rpc_plugin;
class GetInteriorVehicleDataRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  GetInteriorVehicleDataRequestTest()
      : mock_app_(std::make_shared<NiceMock<MockApplication> >())
      , mock_app2_(std::make_shared<NiceMock<MockApplication> >())
      , rc_app_extention_(std::make_shared<RCAppExtension>(kModuleId))
      , rc_app_extention2_(std::make_shared<RCAppExtension>(kModuleId))
      , apps_lock_(std::make_shared<sync_primitives::Lock>())
      , apps_da_(apps_, apps_lock_)
      , rc_capabilities_(std::make_shared<smart_objects::SmartObject>(
            smart_objects::SmartType::SmartType_Array)) {
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));
    ON_CALL(*mock_app2_, app_id()).WillByDefault(Return(kAppId2));
    ON_CALL(*mock_app_, is_remote_control_supported())
        .WillByDefault(Return(true));
    ON_CALL(*mock_app2_, is_remote_control_supported())
        .WillByDefault(Return(true));
    ON_CALL(*mock_app_, QueryInterface(_))
        .WillByDefault(Return(rc_app_extention_));
    ON_CALL(*mock_app2_, QueryInterface(_))
        .WillByDefault(Return(rc_app_extention2_));
  }

  /**
   * @brief CreateBasicMessage creates message for
   * GetInteriorVehicleData request for app1
   * @return message shared ptr
   */
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

  void SetUp() OVERRIDE {
    std::pair<uint32_t, int32_t> frequency;
    frequency.first = max_request_in_time_frame;
    frequency.second = time_frame_of_allowed_requests;
    smart_objects::SmartObject control_caps((smart_objects::SmartType_Array));
    (*rc_capabilities_)[strings::kradioControlCapabilities] = control_caps;
    ON_CALL(app_mngr_, get_settings())
        .WillByDefault(ReturnRef(app_mngr_settings_));
    ON_CALL(app_mngr_settings_, get_interior_vehicle_data_frequency())
        .WillByDefault(ReturnRef(frequency));

    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(mock_hmi_interfaces_));
    ON_CALL(
        mock_hmi_interfaces_,
        GetInterfaceState(application_manager::HmiInterfaces::HMI_INTERFACE_RC))
        .WillByDefault(Return(application_manager::HmiInterfaces::
                                  InterfaceState::STATE_AVAILABLE));
    ON_CALL(app_mngr_, application(kAppId)).WillByDefault(Return(mock_app_));
    ON_CALL(app_mngr_, application(kAppId2)).WillByDefault(Return(mock_app2_));
    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));
    ON_CALL(app_mngr_, hmi_capabilities())
        .WillByDefault(ReturnRef(mock_hmi_capabilities_));
    ON_CALL(mock_hmi_capabilities_, rc_capability())
        .WillByDefault(Return(rc_capabilities_));
    ON_CALL(mock_policy_handler_,
            CheckHMIType(
                _, mobile_apis::AppHMIType::eType::REMOTE_CONTROL, nullptr))
        .WillByDefault(Return(true));
    ON_CALL(mock_policy_handler_, CheckModule(_, _))
        .WillByDefault(Return(true));
    ON_CALL(mock_allocation_manager_, is_rc_enabled())
        .WillByDefault(Return(true));
    ON_CALL(mock_rc_capabilities_manager_, CheckIfModuleExistsInCapabilities(_))
        .WillByDefault(Return(true));
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
  std::shared_ptr<MockApplication> mock_app_;
  std::shared_ptr<MockApplication> mock_app2_;
  std::shared_ptr<RCAppExtension> rc_app_extention_;
  std::shared_ptr<RCAppExtension> rc_app_extention2_;
  testing::NiceMock<rc_rpc_plugin_test::MockResourceAllocationManager>
      mock_allocation_manager_;
  testing::NiceMock<rc_rpc_plugin_test::MockInteriorDataCache>
      mock_interior_data_cache_;
  testing::NiceMock<rc_rpc_plugin_test::MockInteriorDataManager>
      mock_interior_data_manager_;
  application_manager::ApplicationSet apps_;
  const std::shared_ptr<sync_primitives::Lock> apps_lock_;
  DataAccessor<application_manager::ApplicationSet> apps_da_;
  testing::NiceMock<rc_rpc_plugin_test::MockRCCapabilitiesManager>
      mock_rc_capabilities_manager_;
  smart_objects::SmartObjectSPtr rc_capabilities_;
  testing::NiceMock<MockRCConsentManager> mock_rc_consent_manger_;
};

TEST_F(GetInteriorVehicleDataRequestTest,
       Execute_ExpectCorrectMessageSentToHMI_NoSubscriptionNoCache) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleType] = module_eType;
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleType] = module_id;
  const ModuleUid module(module_type, module_id);
  ON_CALL(mock_interior_data_cache_, Contains(module))
      .WillByDefault(Return(false));
  ON_CALL(mock_interior_data_manager_, CheckRequestsToHMIFrequency(_))
      .WillByDefault(Return(true));
  auto command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);

  // Expectations
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::RC_GetInteriorVehicleData), _))
      .WillOnce(Return(true));
  // Act

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(GetInteriorVehicleDataRequestTest,
       Execute_ExpectCorrectMessageSentToHMI_SupscribeAppNoCache) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleType] = module_eType;
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleId] = module_id;
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kSubscribe] = true;
  const ModuleUid module(module_type, module_id);
  ON_CALL(mock_interior_data_cache_, Contains(module))
      .WillByDefault(Return(false));
  ON_CALL(mock_interior_data_manager_, CheckRequestsToHMIFrequency(_))
      .WillByDefault(Return(true));
  auto command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);

  // Expectations
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::RC_GetInteriorVehicleData), _))
      .WillOnce(Return(true));
  // Act
  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(
    GetInteriorVehicleDataRequestTest,
    Execute_ExpectMessageNotSentToHMI_SuccessSentToMobile_AppSubscribed_DataFromCache) {
  // Arrange
  const ModuleUid module(module_type, module_id);
  rc_app_extention_->SubscribeToInteriorVehicleData(module);
  MessageSharedPtr mobile_message = CreateBasicMessage();
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleType] = module_eType;
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleId] = module_id;
  smart_objects::SmartObject radio_data;
  smart_objects::SmartObject sis_data;
  smart_objects::SmartObject gps_data;

  gps_data[application_manager::strings::longitude_degrees] = 1.0;
  gps_data[application_manager::strings::latitude_degrees] = 1.0;

  sis_data[application_manager::strings::station_short_name] =
      "dummy_short_name";
  sis_data[application_manager::strings::station_location] = gps_data;

  radio_data[message_params::kBand] = enums_value::kAM;
  radio_data[message_params::kSisData] = sis_data;

  auto command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);

  // Expectations
  EXPECT_CALL(mock_interior_data_cache_, Contains(module))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_interior_data_cache_, Retrieve(module))
      .WillOnce(Return(radio_data));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(0);
  MessageSharedPtr command_result;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _))
      .WillOnce(DoAll(SaveArg<0>(&command_result), Return(true)));

  // Act
  ASSERT_TRUE(command->Init());
  command->Run();

  // Assert
  EXPECT_EQ((*command_result)[application_manager::strings::msg_params]
                             [message_params::kModuleData]
                             [message_params::kRadioControlData],
            radio_data);
}

TEST_F(
    GetInteriorVehicleDataRequestTest,
    Execute_ExpectCorrectMessageSentToHMI_LastAppSubscribedUnsubscribe_ClearCache) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleType] = module_eType;
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kSubscribe] = false;
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleId] = module_id;
  const ModuleUid module(module_type, module_id);

  MessageSharedPtr hmi_response = CreateBasicMessage();
  ns_smart_device_link::ns_smart_objects::SmartObject& hmi_msg_params =
      (*hmi_response)[application_manager::strings::msg_params];
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::SUCCESS;
  hmi_msg_params[application_manager::hmi_response::code] = response_code;
  hmi_msg_params[application_manager::strings::connection_key] = kConnectionKey;
  hmi_msg_params[message_params::kModuleData][message_params::kModuleId] =
      module_id;

  apps_.insert(mock_app_);
  rc_app_extention_->SubscribeToInteriorVehicleData(module);
  ON_CALL(app_mngr_, applications()).WillByDefault(Return(apps_da_));
  ON_CALL(mock_interior_data_manager_, CheckRequestsToHMIFrequency(_))
      .WillByDefault(Return(true));

  // Expectations
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::RC_GetInteriorVehicleData), _))
      .WillOnce(Return(true));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _))
      .WillOnce(Return(true));

  EXPECT_CALL(mock_interior_data_cache_, Remove(module));

  // Act
  auto command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);
  ASSERT_TRUE(command->Init());
  command->Run();
  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::RC_GetInteriorVehicleData);
  event.set_smart_object(*hmi_response);
  command->on_event(event);
}

TEST_F(GetInteriorVehicleDataRequestTest,
       Execute_ExpectMessageNotSentToHMI_TwoApps_OneUnsubscribed) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleType] = module_eType;
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kSubscribe] = false;
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleId] = module_id;
  const ModuleUid module(module_type, module_id);

  apps_.insert(mock_app_);
  apps_.insert(mock_app2_);
  rc_app_extention_->SubscribeToInteriorVehicleData(module);
  rc_app_extention2_->SubscribeToInteriorVehicleData(module);

  smart_objects::SmartObject radio_data;
  radio_data[message_params::kBand] = enums_value::kAM;
  ON_CALL(app_mngr_, applications()).WillByDefault(Return(apps_da_));

  auto command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);

  // Expectations
  EXPECT_CALL(mock_interior_data_cache_, Contains(module))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_interior_data_cache_, Retrieve(module))
      .WillOnce(Return(radio_data));

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(0);
  MessageSharedPtr command_result;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _))
      .WillOnce(DoAll(SaveArg<0>(&command_result), Return(true)));

  // Act
  ASSERT_TRUE(command->Init());
  command->Run();

  // Assert
  EXPECT_FALSE(rc_app_extention_->IsSubscribedToInteriorVehicleDataOfType(
      enums_value::kRadio));
  EXPECT_EQ((*command_result)[application_manager::strings::msg_params]
                             [message_params::kModuleData]
                             [message_params::kRadioControlData],
            radio_data);
}

TEST_F(
    GetInteriorVehicleDataRequestTest,
    Execute_CapabilitiesValidationFailed_ExpectMessageNotSentToHMI_AndFalseSentToMobile) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  ns_smart_device_link::ns_smart_objects::SmartObject& msg_params =
      (*mobile_message)[application_manager::strings::msg_params];
  msg_params[message_params::kModuleType] = module_eType;
  auto command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);
  ON_CALL(mock_hmi_capabilities_, rc_capability())
      .WillByDefault(Return(nullptr));

  // Expectations
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(0);
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_apis::Result::UNSUPPORTED_RESOURCE), _))
      .WillOnce((Return(true)));
  // Act
  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(
    GetInteriorVehicleDataRequestTest,
    Execute_PolicyValidationFailed_ExpectMessageNotSentToHMI_AndFalseSentToMobile) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  ns_smart_device_link::ns_smart_objects::SmartObject& msg_params =
      (*mobile_message)[application_manager::strings::msg_params];
  msg_params[message_params::kModuleType] = module_eType;
  auto command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);
  ON_CALL(mock_policy_handler_, CheckModule(_, _)).WillByDefault(Return(false));

  // Expectations
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(0);
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::DISALLOWED), _))
      .WillOnce((Return(true)));

  // Act
  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(GetInteriorVehicleDataRequestTest,
       OnEvent_ValidHmiResponse_ExpectSuccessfulResponseSentToMobile_NoCache) {
  using rc_rpc_plugin::commands::GetInteriorVehicleDataRequest;
  namespace hmi_response = application_manager::hmi_response;
  namespace strings = application_manager::strings;

  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  auto& msg_params = (*mobile_message)[strings::msg_params];
  msg_params[message_params::kModuleType] = module_eType;

  MessageSharedPtr hmi_response_message = CreateBasicMessage();
  auto& hmi_response_params = (*hmi_response_message)[strings::msg_params];
  hmi_response_params[hmi_response::code] = hmi_apis::Common_Result::SUCCESS;
  hmi_response_params[strings::connection_key] = kConnectionKey;
  hmi_response_params[message_params::kModuleData][message_params::kModuleId] =
      module_id;

  ON_CALL(mock_interior_data_cache_, Contains(_)).WillByDefault(Return(false));
  ON_CALL(mock_interior_data_manager_, CheckRequestsToHMIFrequency(_))
      .WillByDefault(Return(true));

  // Expectations
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _))
      .WillOnce(Return(true));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::RC_GetInteriorVehicleData), _))
      .WillOnce(Return(true));

  // Act
  auto command = CreateRCCommand<GetInteriorVehicleDataRequest>(mobile_message);
  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::RC_GetInteriorVehicleData);
  event.set_smart_object(*hmi_response_message);
  ASSERT_TRUE(command->Init());
  command->Run();
  command->on_event(event);
}

TEST_F(GetInteriorVehicleDataRequestTest,
       OnEvent_InvalidHmiResponse_ExpectGenericErrorResponseSentToMobile) {
  using rc_rpc_plugin::commands::GetInteriorVehicleDataRequest;
  namespace hmi_response = application_manager::hmi_response;
  namespace strings = application_manager::strings;

  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  auto& msg_params =
      (*mobile_message)[application_manager::strings::msg_params];
  msg_params[message_params::kModuleType] = module_eType;

  MessageSharedPtr hmi_message = CreateBasicMessage();
  auto& hmi_msg_params = (*hmi_message)[strings::params];
  hmi_msg_params[hmi_response::code] = hmi_apis::Common_Result::READ_ONLY;
  hmi_msg_params[strings::connection_key] = kConnectionKey;
  ON_CALL(mock_interior_data_cache_, Contains(_)).WillByDefault(Return(false));
  ON_CALL(mock_interior_data_manager_, CheckRequestsToHMIFrequency(_))
      .WillByDefault(Return(true));

  // Expectations
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::RC_GetInteriorVehicleData), _))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::GENERIC_ERROR), _))
      .WillOnce(Return(true));

  // Act
  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::RC_GetInteriorVehicleData);
  event.set_smart_object(*hmi_message);
  auto command = CreateRCCommand<GetInteriorVehicleDataRequest>(mobile_message);
  ASSERT_TRUE(command->Init());
  command->Run();
  command->on_event(event);
}

TEST_F(GetInteriorVehicleDataRequestTest,
       OnEvent_InvalidHmiResponse_DontUnsubscribeLastApp_NoClearCache) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleType] = module_eType;
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleId] = module_id;
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kSubscribe] = false;
  const ModuleUid module(module_type, module_id);
  MessageSharedPtr hmi_response = CreateBasicMessage();
  ns_smart_device_link::ns_smart_objects::SmartObject& hmi_msg_params =
      (*hmi_response)[application_manager::strings::params];
  hmi_msg_params[application_manager::hmi_response::code] =
      hmi_apis::Common_Result::READ_ONLY;
  hmi_msg_params[application_manager::strings::connection_key] = kConnectionKey;

  apps_.insert(mock_app_);
  rc_app_extention_->SubscribeToInteriorVehicleData(module);
  ON_CALL(app_mngr_, applications()).WillByDefault(Return(apps_da_));
  ON_CALL(mock_interior_data_manager_, CheckRequestsToHMIFrequency(_))
      .WillByDefault(Return(true));

  // Expectations
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(hmi_apis::FunctionID::RC_GetInteriorVehicleData), _))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::GENERIC_ERROR), _))
      .WillOnce(Return(true));

  EXPECT_CALL(mock_interior_data_cache_, Clear()).Times(0);

  // Act
  auto command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);
  ASSERT_TRUE(command->Init());
  command->Run();
  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::RC_GetInteriorVehicleData);
  event.set_smart_object(*hmi_response);
  command->on_event(event);

  // Assert
  EXPECT_TRUE(rc_app_extention_->IsSubscribedToInteriorVehicleData(module));
}

TEST_F(GetInteriorVehicleDataRequestTest,
       Execute_ExpectRejectDuToRequestLimitation_NoCahce) {
  // Arrange
  rc_app_extention_->UnsubscribeFromInteriorVehicleDataOfType(
      enums_value::kRadio);
  MessageSharedPtr mobile_message = CreateBasicMessage();
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleType] = module_eType;
  (*mobile_message)[application_manager::strings::msg_params]
                   [message_params::kModuleId] = module_id;
  smart_objects::SmartObject radio_data;
  radio_data[message_params::kBand] = enums_value::kAM;
  auto command =
      CreateRCCommand<rc_rpc_plugin::commands::GetInteriorVehicleDataRequest>(
          mobile_message);
  const ModuleUid module(module_type, module_id);
  size_t i = 0;
  for (; i <= max_request_in_time_frame; ++i) {
    // Expectations
    EXPECT_CALL(mock_interior_data_manager_,
                CheckRequestsToHMIFrequency(module))
        .WillOnce(Return(true));
    EXPECT_CALL(mock_interior_data_manager_, StoreRequestToHMITime(module));
    EXPECT_CALL(mock_interior_data_cache_, Contains(module))
        .WillRepeatedly(Return(false));
    EXPECT_CALL(
        mock_rpc_service_,
        ManageHMICommand(
            HMIResultCodeIs(hmi_apis::FunctionID::RC_GetInteriorVehicleData),
            _))
        .WillRepeatedly(Return(true));
    // Act
    ASSERT_TRUE(command->Init());
    command->Run();
  }

  // Expectations
  EXPECT_CALL(mock_interior_data_manager_, CheckRequestsToHMIFrequency(module))
      .WillOnce(Return(false));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::REJECTED), _))
      .WillOnce(Return(false));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).Times(0);

  // Act
  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(GetInteriorVehicleDataRequestTest,
       OnEvent_ValidHmiResponse_AvailableHDChanelsIsArrayWithHDChanels) {
  using rc_rpc_plugin::commands::GetInteriorVehicleDataRequest;
  namespace hmi_response = application_manager::hmi_response;
  namespace strings = application_manager::strings;

  const uint32_t chanel1_index = 1u;
  const uint32_t chanel2_index = 2u;
  const uint32_t chanel3_index = 3u;

  const uint32_t expected_array_length = 3u;

  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();

  MessageSharedPtr hmi_response_message = CreateBasicMessage();
  auto& hmi_response_params = (*hmi_response_message)[strings::msg_params];
  hmi_response_params[hmi_response::code] = hmi_apis::Common_Result::SUCCESS;
  hmi_response_params[strings::connection_key] = kAppId;

  auto& msg_params = (*hmi_response_message)[strings::msg_params];
  msg_params[message_params::kModuleType] = module_type;

  auto available_hd_chanels =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  available_hd_chanels[0] = chanel1_index;
  available_hd_chanels[1] = chanel2_index;
  available_hd_chanels[2] = chanel3_index;

  msg_params[message_params::kModuleData][message_params::kRadioControlData]
            [message_params::kAvailableHdChannels] = available_hd_chanels;

  ON_CALL(mock_interior_data_cache_, Contains(_)).WillByDefault(Return(false));
  ON_CALL(mock_interior_data_manager_, CheckRequestsToHMIFrequency(_))
      .WillByDefault(Return(true));

  MessageSharedPtr message_to_mob = CreateBasicMessage();

  // Expectations
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).WillOnce(Return(true));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&message_to_mob), Return(true)));

  // Act
  auto command = CreateRCCommand<GetInteriorVehicleDataRequest>(mobile_message);
  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::RC_GetInteriorVehicleData);

  ASSERT_TRUE(command->Init());
  command->Run();

  event.set_smart_object(*hmi_response_message);
  command->on_event(event);

  auto& hd_chanels =
      (*message_to_mob)[strings::msg_params][message_params::kModuleData]
                       [message_params::kRadioControlData]
                       [message_params::kAvailableHdChannels];
  const size_t array_length = hd_chanels.length();

  EXPECT_EQ(expected_array_length, array_length);

  EXPECT_EQ(chanel1_index, hd_chanels[0].asUInt());
  EXPECT_EQ(chanel2_index, hd_chanels[1].asUInt());
  EXPECT_EQ(chanel3_index, hd_chanels[2].asUInt());
}

TEST_F(GetInteriorVehicleDataRequestTest,
       OnEvent_ValidHmiResponse_ClimateEnableAvailable) {
  using rc_rpc_plugin::commands::GetInteriorVehicleDataRequest;
  namespace hmi_response = application_manager::hmi_response;
  namespace strings = application_manager::strings;

  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();

  MessageSharedPtr hmi_response_message = CreateBasicMessage();
  auto& hmi_response_params = (*hmi_response_message)[strings::msg_params];
  hmi_response_params[hmi_response::code] = hmi_apis::Common_Result::SUCCESS;
  hmi_response_params[strings::connection_key] = kAppId;

  auto& msg_params = (*hmi_response_message)[strings::msg_params];

  auto climate_control_data =
      smart_objects::SmartObject(smart_objects::SmartType_Boolean);
  climate_control_data = true;

  msg_params[message_params::kModuleData][message_params::kClimateControlData]
            [message_params::kClimateEnableAvailable] = climate_control_data;

  ON_CALL(mock_interior_data_cache_, Contains(_)).WillByDefault(Return(false));
  ON_CALL(mock_interior_data_manager_, CheckRequestsToHMIFrequency(_))
      .WillByDefault(Return(true));

  auto message_to_mob = CreateBasicMessage();

  // Expectations
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).WillOnce(Return(true));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&message_to_mob), Return(true)));

  // Act
  auto command = CreateRCCommand<GetInteriorVehicleDataRequest>(mobile_message);
  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::RC_GetInteriorVehicleData);

  ASSERT_TRUE(command->Init());
  command->Run();

  event.set_smart_object(*hmi_response_message);
  command->on_event(event);

  const bool climate_enable_available =
      (*message_to_mob)[strings::msg_params][message_params::kModuleData]
                       [message_params::kClimateControlData]
                       [message_params::kClimateEnableAvailable]
                           .asBool();

  EXPECT_TRUE(climate_enable_available);
}

}  // namespace rc_rpc_plugin_test
