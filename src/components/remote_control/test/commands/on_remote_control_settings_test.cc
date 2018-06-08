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

#include "remote_control/commands/on_remote_control_settings_notification.h"
#include "gtest/gtest.h"
#include "mock_remote_control_plugin.h"
#include "mock_application.h"
#include "mock_resource_allocation_manager.h"
#include "remote_control/rc_app_extension.h"
#include "remote_control/remote_control_event.h"
#include "remote_control/rc_module_constants.h"
#include "remote_control/message_helper.h"
#include "remote_control/rc_command_factory.h"
#include "remote_control/event_engine/event_dispatcher.h"
#include "functional_module/function_ids.h"
#include "include/mock_service.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"

using functional_modules::RCFunctionID;
using application_manager::ServicePtr;

using application_manager::MockService;
using test::components::remote_control_test::MockApplication;

using ::testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SaveArg;
using ::application_manager::Message;
using ::application_manager::MessageType;
using ::application_manager::ApplicationSharedPtr;
using ::protocol_handler::MessagePriority;
using remote_control::RemotePluginInterface;
using remote_control::MessageHelper;
using namespace remote_control;

namespace {
const int kModuleId = 153;
const std::string kAllowedTrueAUTO_ALLOWJson =
    "{ \"jsonrpc\": \"2.0\",\"method\": \"RC.OnRemoteControlSettings\",\
         \"params\":{\"accessMode\": \"AUTO_ALLOW\", \"allowed\":true}}";
const std::string kAllowedTrueASK_DRIVERJson =
    "{ \"jsonrpc\": \"2.0\",\"method\": \"RC.OnRemoteControlSettings\",\
         \"params\":{\"accessMode\": \"ASK_DRIVER\", \"allowed\":true}}";
const std::string kAllowedTrueAUTO_DENYJson =
    "{ \"jsonrpc\": \"2.0\",\"method\": \"RC.OnRemoteControlSettings\",\
         \"params\":{\"accessMode\": \"AUTO_DENY\", \"allowed\":true}}";
const std::string kAllowedFalseAUTO_ALLOWJson =
    "{ \"jsonrpc\": \"2.0\",\"method\": \"RC.OnRemoteControlSettings\",\
         \"params\":{\"accessMode\": \"AUTO_ALLOW\", \"allowed\":false}}";
const std::string kAllowedFalseASK_DRIVERJson =
    "{ \"jsonrpc\": \"2.0\",\"method\": \"RC.OnRemoteControlSettings\",\
         \"params\":{\"accessMode\": \"ASK_DRIVER\", \"allowed\":false}}";
const std::string kAllowedFalseAUTO_DENYJson =
    "{ \"jsonrpc\": \"2.0\",\"method\": \"RC.OnRemoteControlSettings\",\
         \"params\":{\"accessMode\": \"AUTO_DENY\", \"allowed\":false}}";
const std::string kWithoutParamsJson =
    "{ \"jsonrpc\": \"2.0\",\"method\": \"RC.OnRemoteControlSettings\"}";
}

namespace test {
namespace components {
namespace remote_control_test {
namespace on_remote_control_settings_notification_test {

class OnRemoteControlSettingsNotificationTest : public ::testing::Test {
 public:
  OnRemoteControlSettingsNotificationTest()
      : mock_service_(std::make_shared<NiceMock<MockService> >())
      , mock_app_(std::make_shared<NiceMock<MockApplication> >())
      , rc_app_extention_(
            std::make_shared<remote_control::RCAppExtension>(kModuleId)) {
    ON_CALL(mock_module_, resource_allocation_manager())
        .WillByDefault(ReturnRef(mock_allocation_manager_));
    ON_CALL(*mock_service_, IsInterfaceAvailable(_))
        .WillByDefault(Return(true));
    ON_CALL(*mock_service_, IsRemoteControlApplication(_))
        .WillByDefault(Return(true));
    apps_.push_back(mock_app_);
  }

  remote_control::request_controller::MobileRequestPtr CreateCommand(
      application_manager::MessagePtr msg) {
    return remote_control::RCCommandFactory::CreateCommand(msg, mock_module_);
  }

  application_manager::MessagePtr CreateBasicMessage(
      const std::string& json_message) {
    application_manager::MessagePtr message = std::make_shared<Message>(
        MessagePriority::FromServiceType(protocol_handler::ServiceType::kRpc));
    message->set_function_id(RCFunctionID::ON_REMOTE_CONTROL_SETTINGS);
    message->set_function_name(
        MessageHelper::GetMobileAPIName(functional_modules::BUTTON_PRESS));
    message->set_json_message(json_message);
    return message;
  }

 protected:
  std::shared_ptr<NiceMock<application_manager::MockService> > mock_service_;
  std::shared_ptr<NiceMock<MockApplication> > mock_app_;
  std::shared_ptr<remote_control::RCAppExtension> rc_app_extention_;
  remote_control_test::MockRemotePluginInterface mock_module_;
  std::vector<ApplicationSharedPtr> apps_;
  testing::NiceMock<remote_control_test::MockResourceAllocationManager>
      mock_allocation_manager_;
};

TEST_F(OnRemoteControlSettingsNotificationTest,
       Execute_SetAccessModeAUTO_ALLOW_IfAllowedTrue) {
  // Arrange
  const Json::Value value =
      MessageHelper::StringToValue(kAllowedTrueAUTO_ALLOWJson);
  const std::string access_mode =
      value[json_keys::kParams][message_params::kAccessMode].asString();
  const hmi_apis::Common_RCAccessMode::eType access_mode_ =
      MessageHelper::AccessModeFromString(access_mode);
  // Expectations
  EXPECT_CALL(mock_module_, service()).WillOnce(Return(mock_service_));
  EXPECT_CALL(mock_allocation_manager_, SetAccessMode(access_mode_));

  application_manager::MessagePtr message =
      CreateBasicMessage(kAllowedTrueAUTO_ALLOWJson);
  // Act
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(message);
  command->Run();
}

TEST_F(OnRemoteControlSettingsNotificationTest,
       Execute_SetAccessModeAUTO_DENY_IfAllowedTrue) {
  // Arrange
  const Json::Value value =
      MessageHelper::StringToValue(kAllowedTrueAUTO_DENYJson);
  const std::string access_mode =
      value[json_keys::kParams][message_params::kAccessMode].asString();
  const hmi_apis::Common_RCAccessMode::eType access_mode_ =
      MessageHelper::AccessModeFromString(access_mode);
  // Expectations
  EXPECT_CALL(mock_module_, service()).WillOnce(Return(mock_service_));
  EXPECT_CALL(mock_allocation_manager_, SetAccessMode(access_mode_));

  application_manager::MessagePtr message =
      CreateBasicMessage(kAllowedTrueAUTO_DENYJson);
  // Act
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(message);
  command->Run();
}

TEST_F(OnRemoteControlSettingsNotificationTest,
       Execute_SetAccessModeASK_DRIVER_IfAllowedTrue) {
  // Arrange
  const Json::Value value =
      MessageHelper::StringToValue(kAllowedTrueASK_DRIVERJson);
  const std::string access_mode =
      value[json_keys::kParams][message_params::kAccessMode].asString();
  const hmi_apis::Common_RCAccessMode::eType access_mode_ =
      MessageHelper::AccessModeFromString(access_mode);
  // Expectations
  EXPECT_CALL(mock_module_, service()).WillOnce(Return(mock_service_));
  EXPECT_CALL(mock_allocation_manager_, SetAccessMode(access_mode_));
  application_manager::MessagePtr message =
      CreateBasicMessage(kAllowedTrueASK_DRIVERJson);
  // Act
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(message);
  command->Run();
}

TEST_F(OnRemoteControlSettingsNotificationTest,
       Execute_DisallowRCFunctionality_IfAllowedFalseAccessModeAUTO_ALLOW) {
  // Arrange
  const Json::Value module_type(enums_value::kClimate);
  rc_app_extention_->SubscribeToInteriorVehicleData(module_type);
  const Json::Value value =
      MessageHelper::StringToValue(kAllowedFalseAUTO_ALLOWJson);
  const std::string access_mode =
      value[json_keys::kParams][message_params::kAccessMode].asString();
  const hmi_apis::Common_RCAccessMode::eType access_mode_ =
      MessageHelper::AccessModeFromString(access_mode);
  // Expectations
  EXPECT_CALL(mock_module_, service()).WillOnce(Return(mock_service_));
  EXPECT_CALL(mock_allocation_manager_, SetAccessMode(access_mode_)).Times(0);
  EXPECT_CALL(*mock_service_, GetApplications(kModuleId))
      .WillOnce(Return(apps_));
  EXPECT_CALL(
      *mock_service_,
      ChangeNotifyHMILevel(apps_[0], mobile_apis::HMILevel::eType::HMI_NONE));
  EXPECT_CALL(*mock_app_, QueryInterface(kModuleId))
      .WillOnce(Return(rc_app_extention_));
  EXPECT_EQ(rc_app_extention_->IsSubscibedToInteriorVehicleData(module_type),
            true);
  application_manager::MessagePtr message =
      CreateBasicMessage(kAllowedFalseAUTO_ALLOWJson);
  // Act
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(message);
  command->Run();
  // Assertions
  EXPECT_EQ(rc_app_extention_->IsSubscibedToInteriorVehicleData(module_type),
            false);
}

TEST_F(OnRemoteControlSettingsNotificationTest,
       Execute_DisallowRCFunctionality_IfAllowedFalseAccessModeASK_DRIVER) {
  // Arrange
  const Json::Value module_type(enums_value::kRadio);
  rc_app_extention_->SubscribeToInteriorVehicleData(module_type);
  const Json::Value value =
      MessageHelper::StringToValue(kAllowedFalseASK_DRIVERJson);
  const std::string access_mode =
      value[json_keys::kParams][message_params::kAccessMode].asString();
  const hmi_apis::Common_RCAccessMode::eType access_mode_ =
      MessageHelper::AccessModeFromString(access_mode);
  // Expectations
  EXPECT_CALL(mock_module_, service()).WillOnce(Return(mock_service_));
  EXPECT_CALL(mock_allocation_manager_, SetAccessMode(access_mode_)).Times(0);
  EXPECT_CALL(*mock_service_, GetApplications(kModuleId))
      .WillOnce(Return(apps_));
  EXPECT_CALL(
      *mock_service_,
      ChangeNotifyHMILevel(apps_[0], mobile_apis::HMILevel::eType::HMI_NONE));
  EXPECT_CALL(*mock_app_, QueryInterface(kModuleId))
      .WillOnce(Return(rc_app_extention_));
  EXPECT_EQ(rc_app_extention_->IsSubscibedToInteriorVehicleData(module_type),
            true);
  application_manager::MessagePtr message =
      CreateBasicMessage(kAllowedFalseASK_DRIVERJson);
  // Act
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(message);
  command->Run();
  // Assertions
  EXPECT_EQ(rc_app_extention_->IsSubscibedToInteriorVehicleData(module_type),
            false);
}

TEST_F(OnRemoteControlSettingsNotificationTest,
       Execute_DisallowRCFunctionality_IfAllowedFalseAccessModeAUTO_DENY) {
  // Arrange
  const Json::Value module_type(enums_value::kClimate);
  rc_app_extention_->SubscribeToInteriorVehicleData(module_type);
  const Json::Value value =
      MessageHelper::StringToValue(kAllowedFalseAUTO_DENYJson);
  const std::string access_mode =
      value[json_keys::kParams][message_params::kAccessMode].asString();
  const hmi_apis::Common_RCAccessMode::eType access_mode_ =
      MessageHelper::AccessModeFromString(access_mode);
  // Expectations
  EXPECT_CALL(mock_module_, service()).WillOnce(Return(mock_service_));
  EXPECT_CALL(mock_allocation_manager_, SetAccessMode(access_mode_)).Times(0);
  EXPECT_CALL(*mock_service_, GetApplications(kModuleId))
      .WillOnce(Return(apps_));
  EXPECT_CALL(
      *mock_service_,
      ChangeNotifyHMILevel(apps_[0], mobile_apis::HMILevel::eType::HMI_NONE));
  EXPECT_CALL(*mock_app_, QueryInterface(kModuleId))
      .WillOnce(Return(rc_app_extention_));
  EXPECT_EQ(rc_app_extention_->IsSubscibedToInteriorVehicleData(module_type),
            true);
  application_manager::MessagePtr message =
      CreateBasicMessage(kAllowedFalseAUTO_DENYJson);
  // Act
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(message);
  command->Run();
  // Assertions
  EXPECT_EQ(rc_app_extention_->IsSubscibedToInteriorVehicleData(module_type),
            false);
}

TEST_F(OnRemoteControlSettingsNotificationTest,
       Execute_NothingHappens_IfParamsOmitted) {
  // Expectations
  EXPECT_CALL(mock_module_, service()).WillOnce(Return(mock_service_));
  EXPECT_CALL(mock_allocation_manager_, SetAccessMode(_)).Times(0);
  EXPECT_CALL(*mock_service_, GetApplications(kModuleId)).Times(0);
  EXPECT_CALL(*mock_app_, QueryInterface(kModuleId)).Times(0);
  EXPECT_CALL(*mock_service_,
              ChangeNotifyHMILevel(
                  apps_[0], mobile_apis::HMILevel::eType::HMI_NONE)).Times(0);

  application_manager::MessagePtr message =
      CreateBasicMessage(kWithoutParamsJson);
  // Act
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(message);
  command->Run();
}

}  // namespace on_remote_control_settings_notification_test
}  // namespace remote_control_test
}  // namespace components
}  // namespace test
