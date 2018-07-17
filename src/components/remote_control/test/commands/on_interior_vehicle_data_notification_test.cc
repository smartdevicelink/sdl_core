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

#include "remote_control/commands/on_interior_vehicle_data_notification.h"
#include "gtest/gtest.h"
#include "mock_remote_control_plugin.h"
#include "mock_application.h"
#include "remote_control/rc_app_extension.h"
#include "remote_control/remote_control_event.h"
#include "remote_control/rc_module_constants.h"
#include "remote_control/message_helper.h"
#include "remote_control/rc_command_factory.h"
#include "remote_control/event_engine/event_dispatcher.h"
#include "functional_module/function_ids.h"
#include "include/mock_service.h"
#include "utils/shared_ptr_helpers.h"
#include <memory>

using functional_modules::RCFunctionID;
using application_manager::ServicePtr;

using application_manager::MockService;
using test::components::remote_control_test::MockApplication;

using ::testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
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
const std::string kJson =
    "{ \"jsonrpc\": \"2.0\",\"method\": \"RC.OnInteriorVehicleData\",\
         \"params\":{\"moduleData\":{\
         \"moduleType\": \"CLIMATE\",\"climateControlData\": {\"fanSpeed\": 100}}}}";
const uint32_t kAppId_ = 11u;
}

namespace test {
namespace components {
namespace remote_control_test {
namespace on_interior_vehicle_data_notification_test {

class OnInteriorVehicleDataNotificationTest : public ::testing::Test {
 public:
  OnInteriorVehicleDataNotificationTest()
      : mock_service_(std::make_shared<NiceMock<MockService> >())
      , mock_app_(std::make_shared<NiceMock<MockApplication> >())
      , rc_app_extention_(
            std::make_shared<remote_control::RCAppExtension>(kModuleId)) {
    ON_CALL(mock_module_, service()).WillByDefault(Return(mock_service_));
    ServicePtr exp_service(mock_service_);
    mock_module_.set_service(exp_service);
  }

  remote_control::request_controller::MobileRequestPtr CreateCommand(
      application_manager::MessagePtr msg) {
    return remote_control::RCCommandFactory::CreateCommand(msg, mock_module_);
  }

  application_manager::MessagePtr CreateBasicMessage() {
    application_manager::MessagePtr message = std::make_shared<Message>(
        MessagePriority::FromServiceType(protocol_handler::ServiceType::kRpc));
    message->set_function_id(RCFunctionID::ON_INTERIOR_VEHICLE_DATA);
    message->set_function_name("OnInteriorVehicleData");
    message->set_json_message(kJson);
    return message;
  }

 protected:
  std::shared_ptr<NiceMock<application_manager::MockService> > mock_service_;
  std::shared_ptr<NiceMock<MockApplication> > mock_app_;
  std::shared_ptr<remote_control::RCAppExtension> rc_app_extention_;
  remote_control_test::MockRemotePluginInterface mock_module_;
  std::vector<ApplicationSharedPtr> apps_;
};

TEST_F(OnInteriorVehicleDataNotificationTest,
       Execute_SendMessageToMobile_IfAppIsSubscribed) {
  // Arrange
  application_manager::MessagePtr message = CreateBasicMessage();
  Json::Value json_value = MessageHelper::StringToValue(kJson);
  Json::Value module_type =
      json_value[json_keys::kParams][message_params::kModuleData]
                [message_params::kModuleType];
  rc_app_extention_->SubscribeToInteriorVehicleData(module_type);
  apps_.push_back(mock_app_);
  // Expectations
  EXPECT_CALL(mock_module_, service()).WillOnce(Return(mock_service_));
  EXPECT_CALL(*mock_service_, GetApplications(kModuleId))
      .WillOnce(Return(apps_));
  EXPECT_CALL(*mock_app_, QueryInterface(kModuleId))
      .WillOnce(Return(rc_app_extention_));

  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId_));
  EXPECT_CALL(*mock_app_, protocol_version())
      .WillRepeatedly(
          Return(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_3));
  EXPECT_CALL(*mock_service_, GetApplication(kAppId_))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_service_, CheckPolicyPermissions(_))
      .WillOnce(Return(mobile_apis::Result::eType::SUCCESS));
  EXPECT_CALL(*mock_service_, CheckModule(kAppId_, enums_value::kClimate))
      .WillOnce(Return(true));
  application_manager::MessagePtr result_msg;
  EXPECT_CALL(*mock_service_, SendMessageToMobile(_))
      .WillOnce(SaveArg<0>(&result_msg));
  // Act
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(message);
  command->Run();
  // Assertions
  EXPECT_EQ(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_3,
            result_msg->protocol_version());
  EXPECT_EQ(application_manager::MessageType::kNotification,
            result_msg->type());
  EXPECT_EQ(functional_modules::ON_INTERIOR_VEHICLE_DATA,
            result_msg->function_id());
  EXPECT_EQ(MessageHelper::GetMobileAPIName(
                functional_modules::ON_INTERIOR_VEHICLE_DATA),
            result_msg->function_name());
}

TEST_F(OnInteriorVehicleDataNotificationTest,
       Execute_NotSendMessageToMobile_IfAppUnsubscribed) {
  // Arrange
  application_manager::MessagePtr message = CreateBasicMessage();
  apps_.push_back(mock_app_);
  // Expectations
  EXPECT_CALL(mock_module_, service()).WillOnce(Return(mock_service_));
  EXPECT_CALL(*mock_service_, GetApplications(kModuleId))
      .WillOnce(Return(apps_));
  EXPECT_CALL(*mock_app_, QueryInterface(kModuleId))
      .WillOnce(Return(rc_app_extention_));
  EXPECT_CALL(*mock_service_, SendMessageToMobile(_)).Times(0);
  // Act
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(message);
  command->Run();
}

}  // namespace on_interior_vehicle_data_notification_test
}  // namespace remote_control_test
}  // namespace components
}  // namespace test
