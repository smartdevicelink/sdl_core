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

#include "remote_control/commands/button_press_request.h"
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
#include "remote_control/rc_module_constants.h"
#include "functional_module/function_ids.h"
#include "include/mock_service.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "application_manager/mock_hmi_capabilities.h"

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
using ::protocol_handler::MessagePriority;
using remote_control::RemotePluginInterface;
using remote_control::MessageHelper;

namespace {
const int kModuleId = 153;

const std::string kCorrectMobileRequest =
    "{\"moduleType\":\"CLIMATE\",\"buttonName\":\"AC\",\"buttonPressMode\":"
    "\"SHORT\"}";
const std::string kWrongMobileRequest =
    "{\"moduleType\":\"RADIO\",\"buttonName\":\"AC\",\"buttonPressMode\":"
    "\"SHORT\"}";
const std::string kValidHmiResponse =
    "{\"result\":{\"code\":0,\"method\":\"Buttons.ButtonPress\"},\"id\":31,"
    "\"jsonrpc\":\"2.0\"} ";
const std::string KReadOnlyHmiResponse =
    "{\"jsonrpc\":\"2.0\",\"error\":{\"code\":26,\"data\":{\"method\":"
    "\"Buttons.ButtonPress\"},\"message\":\"Read only parameters "
    "received\"},\"id\":31}";
const uint32_t kAppId = 11u;
}

namespace test {
namespace components {
namespace remote_control_test {
namespace button_press_request_test {

class ButtonPressRequestTest : public ::testing::Test {
 public:
  ButtonPressRequestTest()
      : rc_capabilities_(smart_objects::SmartType_Map)
      , mock_service_(utils::MakeShared<NiceMock<MockService> >())
      , mock_app_(utils::MakeShared<NiceMock<MockApplication> >())
      , rc_app_extention_(
            utils::MakeShared<remote_control::RCAppExtension>(kModuleId)) {
    ON_CALL(mock_module_, service()).WillByDefault(Return(mock_service_));
    ON_CALL(mock_module_, resource_allocation_manager())
        .WillByDefault(ReturnRef(mock_allocation_manager_));
    ON_CALL(*mock_service_, GetApplication(kAppId))
        .WillByDefault(Return(mock_app_));
    EXPECT_CALL(mock_module_, event_dispatcher())
        .WillRepeatedly(ReturnRef(event_dispatcher_));
    ServicePtr exp_service(mock_service_);
    mock_module_.set_service(exp_service);
  }

  smart_objects::SmartObject ButtonCapability(
      const mobile_apis::ButtonName::eType button_name) {
    smart_objects::SmartObject button(smart_objects::SmartType_Map);
    button["name"] = button_name;
    return button;
  }

  void SetUp() OVERRIDE {
    using namespace remote_control;
    using namespace mobile_apis;

    std::vector<ButtonName::eType> button_names;
    button_names.push_back(ButtonName::AC_MAX);
    button_names.push_back(ButtonName::AC);
    button_names.push_back(ButtonName::RECIRCULATE);
    button_names.push_back(ButtonName::FAN_UP);
    button_names.push_back(ButtonName::FAN_DOWN);
    button_names.push_back(ButtonName::TEMP_UP);
    button_names.push_back(ButtonName::TEMP_DOWN);
    button_names.push_back(ButtonName::DEFROST_MAX);
    button_names.push_back(ButtonName::DEFROST);
    button_names.push_back(ButtonName::DEFROST_REAR);
    button_names.push_back(ButtonName::UPPER_VENT);
    button_names.push_back(ButtonName::LOWER_VENT);
    button_names.push_back(ButtonName::VOLUME_UP);
    button_names.push_back(ButtonName::VOLUME_DOWN);
    button_names.push_back(ButtonName::EJECT);
    button_names.push_back(ButtonName::SOURCE);
    button_names.push_back(ButtonName::SHUFFLE);
    button_names.push_back(ButtonName::REPEAT);

    smart_objects::SmartObject button_caps(smart_objects::SmartType_Array);
    for (size_t i = 0; i < button_names.size(); i++) {
      button_caps[i] = ButtonCapability(button_names[i]);
    }
    rc_capabilities_[strings::kbuttonCapabilities] = button_caps;
    ON_CALL(*mock_service_, GetRCCapabilities())
        .WillByDefault(Return(DataAccessor<const smart_objects::SmartObject *>(&rc_capabilities_, rc_lock_)));
    ON_CALL(*mock_service_, IsInterfaceAvailable(_))
        .WillByDefault(Return(true));
    ON_CALL(*mock_service_, IsRemoteControlApplication(_))
        .WillByDefault(Return(true));
  }

  remote_control::request_controller::MobileRequestPtr CreateCommand(
      application_manager::MessagePtr msg) {
    return remote_control::RCCommandFactory::CreateCommand(msg, mock_module_);
  }

  application_manager::MessagePtr CreateBasicMessage() {
    application_manager::MessagePtr message = utils::MakeShared<Message>(
        MessagePriority::FromServiceType(protocol_handler::ServiceType::kRpc));
    message->set_function_id(RCFunctionID::BUTTON_PRESS);
    message->set_function_name(
        MessageHelper::GetMobileAPIName(functional_modules::BUTTON_PRESS));
    message->set_connection_key(kAppId);
    return message;
  }

 protected:
  smart_objects::SmartObject rc_capabilities_;
  ::sync_primitives::Lock rc_lock_;
  utils::SharedPtr<NiceMock<application_manager::MockService> > mock_service_;
  utils::SharedPtr<NiceMock<MockApplication> > mock_app_;
  utils::SharedPtr<remote_control::RCAppExtension> rc_app_extention_;
  testing::NiceMock<remote_control_test::MockRemotePluginInterface>
      mock_module_;
  testing::NiceMock<remote_control_test::MockResourceAllocationManager>
      mock_allocation_manager_;
  RemotePluginInterface::RCPluginEventDispatcher event_dispatcher_;
};

TEST_F(ButtonPressRequestTest,
       Execute_ButtonNameMatchesModuleType_ExpectCorrectMessageSentToHMI) {
  namespace json_keys = remote_control::json_keys;
  // Arrange
  application_manager::MessagePtr mobile_message = CreateBasicMessage();
  mobile_message->set_json_message(kCorrectMobileRequest);
  // Expectations
  ON_CALL(*mock_app_, app_id())
      .WillByDefault(Return(mobile_message->connection_key()));
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

  const std::string resource = "CLIMATE";

  EXPECT_CALL(mock_allocation_manager_, IsResourceFree(resource))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_allocation_manager_, AcquireResource(resource, kAppId))
      .WillOnce(Return(remote_control::AcquireResult::ALLOWED));
  EXPECT_CALL(
      mock_allocation_manager_,
      SetResourceState(resource, kAppId, remote_control::ResourceState::BUSY));

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
  const Json::Value& hmi_request_params =
      MessageHelper::StringToValue(result_msg->json_message());
  EXPECT_EQ(functional_modules::hmi_api::button_press,
            hmi_request_params[json_keys::kMethod].asString());
}

TEST_F(
    ButtonPressRequestTest,
    Execute_ButtonNameDoesNotMatchModuleType_ExpectMessageNotSentToHMI_AndFalseSentToMobile) {
  namespace json_keys = remote_control::json_keys;
  namespace result_codes = remote_control::result_codes;
  // Arrange
  application_manager::MessagePtr mobile_message = CreateBasicMessage();
  mobile_message->set_json_message(kWrongMobileRequest);
  // Expectations
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
  EXPECT_CALL(mock_allocation_manager_, IsResourceFree(_))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_allocation_manager_, AcquireResource(_, _))
      .WillOnce(Return(::remote_control::AcquireResult::ALLOWED));
  EXPECT_CALL(*mock_service_, SendMessageToHMI(_)).Times(0);
  application_manager::MessagePtr result_msg;
  EXPECT_CALL(mock_module_, SendResponseToMobile(_))
      .WillOnce(SaveArg<0>(&result_msg));
  // Act
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(mobile_message);
  command->Run();
  // Assertions
  const Json::Value& response_params =
      MessageHelper::StringToValue(result_msg->json_message());
  EXPECT_FALSE(response_params[result_codes::kSuccess].asBool());
  EXPECT_EQ(result_codes::kInvalidData,
            response_params[json_keys::kResultCode].asString());
}

TEST_F(ButtonPressRequestTest, OnEvent_ExpectSuccessfullResponseSentToMobile) {
  namespace json_keys = remote_control::json_keys;
  namespace result_codes = remote_control::result_codes;
  // Arrange
  application_manager::MessagePtr mobile_message = CreateBasicMessage();
  mobile_message->set_json_message(kCorrectMobileRequest);

  application_manager::MessagePtr hmi_message = CreateBasicMessage();
  hmi_message->set_json_message(kValidHmiResponse);
  hmi_message->set_message_type(application_manager::MessageType::kResponse);
  // Expectations
  EXPECT_CALL(*mock_service_, GetApplication(mobile_message->connection_key()))
      .WillOnce(Return(mock_app_));
  application_manager::MessagePtr result_msg;
  EXPECT_CALL(mock_module_, SendResponseToMobile(_))
      .WillOnce(SaveArg<0>(&result_msg));
  // Act
  remote_control::RCPluginEvent event(
      hmi_message, functional_modules::hmi_api::button_press);
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(mobile_message);
  command->on_event(event);
  // Assertions
  const Json::Value& response_params =
      MessageHelper::StringToValue(result_msg->json_message());
  EXPECT_TRUE(response_params[json_keys::kSuccess].asBool());
  EXPECT_EQ(result_codes::kSuccess,
            response_params[json_keys::kResultCode].asString());
}

TEST_F(ButtonPressRequestTest,
       OnEvent_ReadOnlyParamFromHMI_ExpectFalseSentToMobile) {
  namespace json_keys = remote_control::json_keys;
  namespace result_codes = remote_control::result_codes;
  // Arrange
  application_manager::MessagePtr mobile_message = CreateBasicMessage();
  mobile_message->set_json_message(kCorrectMobileRequest);

  application_manager::MessagePtr hmi_message = CreateBasicMessage();
  hmi_message->set_json_message(KReadOnlyHmiResponse);
  hmi_message->set_message_type(application_manager::MessageType::kResponse);
  // Expectations
  EXPECT_CALL(*mock_service_, GetApplication(mobile_message->connection_key()))
      .WillOnce(Return(mock_app_));
  application_manager::MessagePtr result_msg;
  EXPECT_CALL(mock_module_, SendResponseToMobile(_))
      .WillOnce(SaveArg<0>(&result_msg));
  // Act
  remote_control::RCPluginEvent event(
      hmi_message, functional_modules::hmi_api::button_press);
  remote_control::request_controller::MobileRequestPtr command =
      CreateCommand(mobile_message);
  command->on_event(event);
  // Assertions
  const Json::Value& response_params =
      MessageHelper::StringToValue(result_msg->json_message());
  EXPECT_FALSE(response_params[json_keys::kSuccess].asBool());
  EXPECT_EQ(result_codes::kGenericError,
            response_params[json_keys::kResultCode].asString());
}

}  // namespace button_press_request_test
}  // namespace remote_control_test
}  // namespace components
}  // namespace test
