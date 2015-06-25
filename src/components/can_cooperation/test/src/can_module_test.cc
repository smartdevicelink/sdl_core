/*
 * Copyright (c) 2015, Ford Motor Company
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
#include "can_cooperation/can_module.h"
#include "can_cooperation/can_app_extension.h"
#include "can_cooperation/can_module_constants.h"
#include "functional_module/module_observer.h"
#include "mock_application.h"
#include "mock_service.h"
#include "mock_can_connection.h"
#include "mock_application_some_impl.h"
#include "mock_service_some_impl.h"

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

using functional_modules::PluginInfo;
using functional_modules::ProcessResult;
using functional_modules::MobileFunctionID;
using application_manager::ServicePtr;

using application_manager::MockService;
using application_manager::MockApplication;
using application_manager::MockApplicationSomeImpl;
using application_manager::MockServiceSomeImpl;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::Return;

namespace can_cooperation {

class CanModuleTest : public ::testing::Test {
public:
  void static HandleMessage(application_manager::MessagePtr msg) {
    module->HandleMessage(msg);
  }
 protected:
  static CANModule* module;
  static MockService* mock_service;
  static MockCANConnection* mock_conn;

  static void SetUpTestCase() {
    module = CANModule::instance();
    mock_conn = new MockCANConnection();
    ::can_cooperation::CANConnectionSPtr conn(mock_conn);
    module->can_connection_ = conn;

    mock_service = new MockService();
    ServicePtr exp_service(mock_service);
    module->set_service(exp_service);
    ServicePtr out_service = module->service();
    EXPECT_EQ(exp_service.get(), out_service.get());
  }

  static void TearDownTestCase() {
    CANModule::destroy();
  }
};

CANModule* CanModuleTest::module = 0;
MockService* CanModuleTest::mock_service = 0;
MockCANConnection* CanModuleTest::mock_conn = 0;

TEST_F(CanModuleTest, Create) {
  EXPECT_EQ(153, module->GetModuleID());
  PluginInfo plugin = module->GetPluginInfo();
  EXPECT_EQ(plugin.name, "ReverseSDLPlugin");
  EXPECT_EQ(plugin.version, 1);
}

TEST_F(CanModuleTest, ProcessMessageWrongMessage) {
  application_manager::MessagePtr message = new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  message->set_function_id(-1);
  EXPECT_CALL(*mock_service, SendMessageToMobile(_)).Times(0);
  EXPECT_EQ(ProcessResult::CANNOT_PROCESS, module->ProcessMessage(message));
}

TEST_F(CanModuleTest, ProcessMessageEmptyAppsList) {
  application_manager::MessagePtr message = new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  message->set_function_id(MobileFunctionID::ON_INTERIOR_VEHICLE_DATA);
  message->set_function_name("OnInteriorVehicleData");

  std::string json = "{\"jsonrpc\": \"2.0\", \"method\": \"RC.OnInteriorVehicleData\",\
                        \"params\": {\"moduleData\": {\"moduleType\": \"CLIMATE\",\
                        \"moduleZone\":  {\"col\": 0,\"row\": 0,\"level\": 0,\"colspan\": 2,\
                        \"rowspan\": 2, \"levelspan\": 1} }}}";
  message->set_json_message(json);

  std::vector<application_manager::ApplicationSharedPtr> apps;
  EXPECT_CALL(*mock_service, GetApplications(module->GetModuleID())
    ).Times(1).WillOnce(Return(apps));
  EXPECT_CALL(*mock_service, SendMessageToMobile(_)).Times(0);
  EXPECT_EQ(ProcessResult::PROCESSED, module->ProcessMessage(message));
}

TEST_F(CanModuleTest, ProcessMessagePass) {
  application_manager::MessagePtr message = new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  message->set_function_id(MobileFunctionID::ON_INTERIOR_VEHICLE_DATA);

  message->set_function_name("OnInteriorVehicleData");

  std::string json = "{\"jsonrpc\": \"2.0\", \"method\": \"RC.OnInteriorVehicleData\",\
                        \"params\": {\"moduleData\": {\"moduleType\": \"CLIMATE\",\
                        \"moduleZone\":  {\"col\": 0,\"row\": 0,\"level\": 0,\"colspan\": 2,\
                        \"rowspan\": 2, \"levelspan\": 1} }}}";
  message->set_json_message(json);

  std::vector<application_manager::ApplicationSharedPtr> apps;
  MockApplication* app = new MockApplication();
  application_manager::ApplicationSharedPtr app_ptr(app);
  apps.push_back(app_ptr);
  application_manager::AppExtensionUID uid = module->GetModuleID();
  CANAppExtension* can_ext = new CANAppExtension(uid);
  can_ext->GiveControl(true);
  application_manager::AppExtensionPtr ext(can_ext);

  EXPECT_CALL(*app, app_id()).Times(2).WillRepeatedly(Return(1));
  EXPECT_CALL(*mock_service, GetApplications(module->GetModuleID())
    ).Times(1).WillOnce(Return(apps));
  EXPECT_CALL(*mock_service, GetApplication(_)).Times(1)
      .WillOnce(Return(app_ptr));
  EXPECT_CALL(*mock_service, CheckPolicyPermissions(_)).Times(1)
      .WillOnce(Return(mobile_apis::Result::eType::SUCCESS));
  EXPECT_CALL(*mock_service, CheckAccess(1, _, _, "OnInteriorVehicleData", _)).Times(1)
      .WillOnce(Return(application_manager::kAllowed));
  EXPECT_CALL(*mock_service, SendMessageToMobile(_)).Times(1);

  EXPECT_EQ(ProcessResult::PROCESSED, module->ProcessMessage(message));
}

TEST_F(CanModuleTest, RemoveAppExtensionPassWay) {
  MockApplication* app = new MockApplication();
  application_manager::ApplicationSharedPtr valid_app(app);
  const application_manager::AppExtensionUID kUid = module->GetModuleID();
  CANAppExtension* ext = new CANAppExtension(kUid);
  application_manager::AppExtensionPtr valid_ext(ext);
  const application_manager::ApplicationId kAppId = 1;

  EXPECT_CALL(*mock_service, GetApplication(kAppId)).Times(1).WillOnce(
    Return(valid_app));
  EXPECT_CALL(*app, RemoveExtension(kUid)).Times(1);

  module->RemoveAppExtension(kAppId);
}

TEST_F(CanModuleTest, RemoveAppExtensionIfAppNoExist) {
  application_manager::ApplicationSharedPtr invalid_app;
  const application_manager::ApplicationId kAppId = 1;

  EXPECT_CALL(*mock_service, GetApplication(kAppId)).Times(1).WillOnce(
    Return(invalid_app));

  module->RemoveAppExtension(kAppId);
}

TEST_F(CanModuleTest, RemoveAppExtensionIfExtNoExist) {
  MockApplication* app = new MockApplication();
  application_manager::ApplicationSharedPtr valid_app(app);
  const application_manager::AppExtensionUID kUid = module->GetModuleID();
  application_manager::AppExtensionPtr invalid_ext;
  const application_manager::ApplicationId kAppId = 1;

  EXPECT_CALL(*mock_service, GetApplication(kAppId)).Times(1).WillOnce(
    Return(valid_app));
  EXPECT_CALL(*app, RemoveExtension(kUid)).Times(1);

  module->RemoveAppExtension(kAppId);
}

TEST_F(CanModuleTest, SendResponseToMobile) {
  application_manager::MessagePtr message = new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));

  EXPECT_CALL(*mock_service, SendMessageToMobile(message)).Times(1);

  module->SendResponseToMobile(message);
}

TEST_F(CanModuleTest, IsAppForPluginSuccess) {
  MockApplicationSomeImpl* app = new MockApplicationSomeImpl();
  application_manager::ApplicationSharedPtr app_ptr(app);
  smart_objects::SmartObject obj(smart_objects::SmartType::SmartType_Array);
  obj[0] = 10;  //  REMOTE_CONTROL
  app_ptr->set_app_types(obj);
  ASSERT_TRUE(module->IsAppForPlugin(app_ptr));
}

TEST_F(CanModuleTest, IsAppForPluginFail) {
  MockApplicationSomeImpl* app = new MockApplicationSomeImpl();
  application_manager::ApplicationSharedPtr app_ptr(app);
  smart_objects::SmartObject obj(smart_objects::SmartType::SmartType_Array);
  obj[0] = 9;
  obj[1] = 8;
  app_ptr->set_app_types(obj);
  ASSERT_FALSE(module->IsAppForPlugin(app_ptr));
}

TEST_F(CanModuleTest, ChangeDriverDevice) {
  Json::Value value(Json::ValueType::objectValue);
  value[json_keys::kMethod] = functional_modules::hmi_api::on_set_drivers_device;
  value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice][json_keys::kId] = 1;
  Json::FastWriter writer;
  std::string json_str = writer.write(value);

  application_manager::MessagePtr message = new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  message->set_function_name(functional_modules::hmi_api::on_set_drivers_device);
  message->set_message_type(application_manager::MessageType::kNotification);
  message->set_json_message(json_str);

  NiceMock<MockApplicationSomeImpl>* app = new NiceMock<MockApplicationSomeImpl>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  smart_objects::SmartObject obj(smart_objects::SmartType::SmartType_Array);
  obj[0] = 10;
  app_ptr->set_app_types(obj);
  app_ptr->set_device(1);
  CANAppExtension* ext = new CANAppExtension(module->GetModuleID());
  app_ptr->AddExtension(ext);

  NiceMock<MockServiceSomeImpl>* mock_service2 = new NiceMock<MockServiceSomeImpl>();
  mock_service2->set_app(app_ptr);
  ServicePtr mock_service2_ptr(mock_service2);
  module->set_service(mock_service2_ptr);

  EXPECT_CALL(*mock_service2, SetPrimaryDevice(1)).Times(1);
  CanModuleTest::HandleMessage(message);
  ASSERT_TRUE(ext->is_on_driver_device());
  app_ptr->RemoveExtension(module->GetModuleID());
}

TEST_F(CanModuleTest, ChangeDriverDeviceOnOther) {
  Json::Value value(Json::ValueType::objectValue);
  value[json_keys::kMethod] = functional_modules::hmi_api::on_set_drivers_device;
  value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice][json_keys::kId] = 1;
  Json::FastWriter writer;
  std::string json_str = writer.write(value);

  application_manager::MessagePtr message = new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  message->set_function_name(functional_modules::hmi_api::on_set_drivers_device);
  message->set_message_type(application_manager::MessageType::kNotification);
  message->set_json_message(json_str);

  NiceMock<MockApplicationSomeImpl>* app = new NiceMock<MockApplicationSomeImpl>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  smart_objects::SmartObject obj(smart_objects::SmartType::SmartType_Array);
  obj[0] = 10;
  app_ptr->set_app_types(obj);
  app_ptr->set_device(2);
  CANAppExtension* ext = new CANAppExtension(module->GetModuleID());
  app_ptr->AddExtension(ext);

  NiceMock<MockServiceSomeImpl>* mock_service2 = new NiceMock<MockServiceSomeImpl>();
  mock_service2->set_app(app_ptr);
  ServicePtr mock_service2_ptr(mock_service2);
  module->set_service(mock_service2_ptr);

  EXPECT_CALL(*mock_service2, SetPrimaryDevice(1)).Times(1);
  CanModuleTest::HandleMessage(message);
  ASSERT_FALSE(ext->is_on_driver_device());
  app_ptr->RemoveExtension(module->GetModuleID());
}

TEST_F(CanModuleTest, DeactivateApp) {
  std::string reasons[] = {"AUDIO", "PHONECALL", "NAVIGATIONMAP", "PHONEMENU", "SYNCSETTINGS", "GENERAL"};

  application_manager::MessagePtr message = new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  message->set_function_name(functional_modules::hmi_api::on_app_deactivated);
  message->set_message_type(application_manager::MessageType::kNotification);

  NiceMock<MockApplicationSomeImpl>* app = new NiceMock<MockApplicationSomeImpl>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  smart_objects::SmartObject obj(smart_objects::SmartType::SmartType_Array);
  obj[0] = 10;
  app_ptr->set_app_types(obj);
  app_ptr->set_hmi_application_id(11);

  CANAppExtension* ext = new CANAppExtension(module->GetModuleID());
  app_ptr->AddExtension(ext);
  ext->set_is_on_driver_device(true);

  NiceMock<MockServiceSomeImpl>* mock_service2 = new NiceMock<MockServiceSomeImpl>();
  mock_service2->set_app(app_ptr);
  ServicePtr mock_service2_ptr(mock_service2);
  module->set_service(mock_service2_ptr);

  for (size_t i = 0; i < 6; ++ i) {
    Json::Value value(Json::ValueType::objectValue);
    value[json_keys::kMethod] = functional_modules::hmi_api::on_app_deactivated;
    value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
    value[json_keys::kParams][message_params::kHMIAppID] = 11;
    value[json_keys::kParams]["reason"] = reasons[i];
    Json::FastWriter writer;
    std::string json_str = writer.write(value);
    message->set_json_message(json_str);

    app_ptr->set_hmi_level(mobile_apis::HMILevel::eType::HMI_FULL);

    EXPECT_CALL(*mock_service2, ChangeNotifyHMILevel(_, mobile_apis::HMILevel::eType::HMI_LIMITED)).Times(1);
    CanModuleTest::HandleMessage(message);
  }
  app_ptr->RemoveExtension(module->GetModuleID());
}

/*TEST_F(CanModuleTest, DISABLED_HandleMessage) {
  std::string kMessage = "Message to handle";
  const ConnectionState kRet = ConnectionState::INVALID;

  EXPECT_CALL(*mock_conn, Flash()).Times(1).WillOnce(Return(kRet));

  module->Handle(kMessage);
}*/

}  // namespace can_cooperation
