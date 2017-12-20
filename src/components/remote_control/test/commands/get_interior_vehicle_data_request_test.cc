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

#include "remote_control/commands/get_interior_vehicle_data_request.h"
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
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"

using functional_modules::RCFunctionID;
using application_manager::ServicePtr;

using application_manager::MockService;
using test::components::remote_control_test::MockApplication;

using ::testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::StrictMock;
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
const int32_t kConnectionKey = 5;
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

namespace test {
namespace components {
namespace remote_control_test {
namespace get_interior_vehicle_data_request_test {

class GetInteriorVehicleDataRequestTest : public ::testing::Test {
 public:
  typedef utils::SharedPtr<remote_control::commands::BaseCommandRequest>
      GIVDRequestPtr;
  typedef rc_event_engine::Event<application_manager::MessagePtr, std::string>
      GIVD_HMI_Response;
  GetInteriorVehicleDataRequestTest()
      : mock_service_(utils::MakeShared<NiceMock<MockService> >())
      , mock_app_(utils::MakeShared<NiceMock<MockApplication> >())
      , rc_app_extention_(
            utils::MakeShared<remote_control::RCAppExtension>(kModuleId)) {
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(app_id_));
    ON_CALL(mock_module_, service()).WillByDefault(Return(mock_service_));
    ON_CALL(*mock_service_, GetApplication(app_id_))
        .WillByDefault(Return(mock_app_));
    ON_CALL(*mock_service_, IsInterfaceAvailable(_))
        .WillByDefault(Return(true));
    ON_CALL(*mock_service_, IsRemoteControlApplication(_))
        .WillByDefault(Return(true));
    EXPECT_CALL(mock_module_, event_dispatcher())
        .WillRepeatedly(ReturnRef(event_dispatcher_));
    ServicePtr exp_service(mock_service_);
    mock_module_.set_service(exp_service);
  }

  remote_control::request_controller::MobileRequestPtr CreateCommand(
      application_manager::MessagePtr msg) {
    return remote_control::RCCommandFactory::CreateCommand(msg, mock_module_);
  }

  application_manager::MessagePtr CreateBasicMessage() {
    application_manager::MessagePtr message = utils::MakeShared<Message>(
        MessagePriority::FromServiceType(protocol_handler::ServiceType::kRpc));
    message->set_connection_key(kConnectionKey);
    message->set_function_id(RCFunctionID::GET_INTERIOR_VEHICLE_DATA);
    message->set_function_name("GetInteriorVehicleData");
    return message;
  }

 protected:
  utils::SharedPtr<NiceMock<application_manager::MockService> > mock_service_;
  utils::SharedPtr<NiceMock<MockApplication> > mock_app_;
  utils::SharedPtr<remote_control::RCAppExtension> rc_app_extention_;
  remote_control_test::MockRemotePluginInterface mock_module_;
  RemotePluginInterface::RCPluginEventDispatcher event_dispatcher_;
  const uint32_t app_id_ = 11u;
};

TEST_F(GetInteriorVehicleDataRequestTest,
       Execute_MessageValidationOk_ExpectCorrectMessageSentToHMI) {
  // Arrange
  application_manager::MessagePtr mobile_message = CreateBasicMessage();
  mobile_message->set_json_message(kValidMobileRequest);
  // Expectations
  EXPECT_CALL(mock_module_, service()).Times(2).WillOnce(Return(mock_service_));
  EXPECT_CALL(*mock_service_, GetApplication(mobile_message->connection_key()))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_service_, ValidateMessageBySchema(*mobile_message))
      .WillOnce(Return(application_manager::MessageValidationResult::SUCCESS));
  EXPECT_CALL(*mock_service_, CheckPolicyPermissions(mobile_message))
      .WillOnce(Return(mobile_apis::Result::eType::SUCCESS));
  application_manager::AppExtensionPtr invalid_ext;
  EXPECT_CALL(*mock_app_, QueryInterface(kModuleId))
      .WillOnce(Return(invalid_ext))
      .WillRepeatedly(Return(rc_app_extention_));
  application_manager::AppExtensionPtr app_extension;
  EXPECT_CALL(*mock_app_, AddExtension(_))
      .WillOnce(DoAll(SaveArg<0>(&app_extension), Return(true)));
  EXPECT_CALL(*mock_service_, CheckModule(_, _)).WillOnce(Return(true));
  EXPECT_CALL(*mock_service_, GetNextCorrelationID()).WillOnce(Return(1));
  ::sync_primitives::Lock rc_lock;
  EXPECT_CALL(*mock_service_, GetRCCapabilities())
      .WillOnce(Return(DataAccessor<unsigned long>((unsigned long)nullptr, rc_lock)));
  application_manager::MessagePtr result_msg;
  EXPECT_CALL(*mock_service_, SendMessageToHMI(_))
      .WillOnce(SaveArg<0>(&result_msg));
  // Act
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(mobile_message);
  command->Run();
  // Assertions
  EXPECT_EQ(kModuleId, app_extension->uid());
  EXPECT_EQ(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_HMI,
            result_msg->protocol_version());
  EXPECT_EQ(1, result_msg->correlation_id());
  EXPECT_EQ(application_manager::MessageType::kRequest, result_msg->type());
  const Json::Value hmi_request_params =
      MessageHelper::StringToValue(result_msg->json_message());
  EXPECT_EQ(functional_modules::hmi_api::get_interior_vehicle_data,
            hmi_request_params[json_keys::kMethod].asString());
}

TEST_F(
    GetInteriorVehicleDataRequestTest,
    Execute_MessageValidationFailed_ExpectMessageNotSentToHMI_AndFalseSentToMobile) {
  // Arrange
  application_manager::MessagePtr mobile_message = CreateBasicMessage();
  mobile_message->set_json_message(kInvalidMobileRequest);
  // Expectations
  EXPECT_CALL(mock_module_, service()).WillOnce(Return(mock_service_));
  EXPECT_CALL(*mock_service_, GetApplication(mobile_message->connection_key()))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_service_, ValidateMessageBySchema(*mobile_message))
      .WillOnce(
          Return(application_manager::MessageValidationResult::INVALID_JSON));
  EXPECT_CALL(*mock_service_, SendMessageToHMI(_)).Times(0);
  application_manager::MessagePtr result_msg;
  EXPECT_CALL(mock_module_, SendResponseToMobile(_))
      .WillOnce(SaveArg<0>(&result_msg));
  // Act
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(mobile_message);
  command->Run();
  // Assertions
  const Json::Value response_params =
      MessageHelper::StringToValue(result_msg->json_message());
  EXPECT_FALSE(response_params[result_codes::kSuccess].asBool());
  EXPECT_EQ(result_codes::kInvalidData,
            response_params[json_keys::kResultCode].asString());
}

TEST_F(GetInteriorVehicleDataRequestTest,
       OnEvent_ValidHmiResponse_ExpectSuccessfullResponseSentToMobile) {
  // Arrange
  application_manager::MessagePtr mobile_message = CreateBasicMessage();
  mobile_message->set_json_message(kValidMobileRequest);

  application_manager::MessagePtr hmi_message = CreateBasicMessage();
  hmi_message->set_json_message(kValidHmiResponse);
  hmi_message->set_message_type(application_manager::MessageType::kResponse);
  // Expectations
  EXPECT_CALL(mock_module_, service()).WillOnce(Return(mock_service_));
  EXPECT_CALL(*mock_service_, GetApplication(mobile_message->connection_key()))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_service_, ValidateMessageBySchema(*hmi_message))
      .WillOnce(Return(application_manager::MessageValidationResult::SUCCESS));
  EXPECT_CALL(*mock_app_, QueryInterface(kModuleId))
      .WillOnce(Return(rc_app_extention_));
  application_manager::MessagePtr result_msg;
  EXPECT_CALL(mock_module_, SendResponseToMobile(_))
      .WillOnce(SaveArg<0>(&result_msg));
  // Act
  remote_control::RCPluginEvent event(
      hmi_message, functional_modules::hmi_api::get_interior_vehicle_data);
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(mobile_message);
  command->on_event(event);
  // Assertions
  const Json::Value response_params =
      MessageHelper::StringToValue(result_msg->json_message());
  EXPECT_TRUE(response_params[json_keys::kSuccess].asBool());
  EXPECT_EQ(result_codes::kSuccess,
            response_params[json_keys::kResultCode].asString());
}

TEST_F(GetInteriorVehicleDataRequestTest,
       OnEvent_InvalidHmiResponse_ExpectGenericErrorResponseSentToMobile) {
  // Arrange
  application_manager::MessagePtr mobile_message = CreateBasicMessage();
  mobile_message->set_json_message(kValidMobileRequest);
  application_manager::MessagePtr hmi_message = CreateBasicMessage();
  hmi_message->set_json_message(kInvalidHmiResponse);
  hmi_message->set_message_type(application_manager::MessageType::kResponse);
  // Expectations
  EXPECT_CALL(mock_module_, service()).WillOnce(Return(mock_service_));
  EXPECT_CALL(*mock_service_, GetApplication(mobile_message->connection_key()))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_service_, ValidateMessageBySchema(*hmi_message))
      .WillOnce(
          Return(application_manager::MessageValidationResult::INVALID_JSON));
  application_manager::MessagePtr result_msg;
  EXPECT_CALL(mock_module_, SendResponseToMobile(_))
      .WillOnce(SaveArg<0>(&result_msg));
  // Act
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(mobile_message);
  remote_control::RCPluginEvent event(
      hmi_message, functional_modules::hmi_api::get_interior_vehicle_data);
  command->on_event(event);
  const Json::Value response_params =
      MessageHelper::StringToValue(result_msg->json_message());
  EXPECT_FALSE(response_params[json_keys::kSuccess].asBool());
  EXPECT_EQ(result_codes::kGenericError,
            response_params[json_keys::kResultCode].asString());
  const Json::Value hmi_response =
      MessageHelper::StringToValue(hmi_message->json_message());
  EXPECT_EQ(hmi_response[json_keys::kInfo].asString(),
            response_params[json_keys::kInfo].asString());
}

}  // namespace get_interior_vehicle_data_request_test
}  // namespace remote_control_test
}  // namespace components
}  // namespace test
