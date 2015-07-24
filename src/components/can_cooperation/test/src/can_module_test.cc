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
#include "can_cooperation/message_helper.h"
#include "functional_module/module_observer.h"
#include "mock_application.h"
#include "mock_service.h"
#include "mock_can_connection.h"

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

using functional_modules::PluginInfo;
using functional_modules::ProcessResult;
using functional_modules::MobileFunctionID;
using application_manager::ServicePtr;

using application_manager::MockService;
using application_manager::MockApplication;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::Return;
using ::testing::ReturnRef;

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
    std::vector<application_manager::ApplicationSharedPtr> apps;
    EXPECT_CALL(*mock_service, GetApplications(module->GetModuleID())).
        WillRepeatedly(Return(apps));
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
                        \"rowspan\": 2, \"levelspan\": 1}, \"climateControlData\": {\"fanSpeed\": 100} }}}";
  message->set_json_message(json);

  std::vector<application_manager::ApplicationSharedPtr> apps;

  EXPECT_CALL(*mock_service, GetApplications(module->GetModuleID())
    ).WillOnce(Return(apps));
  EXPECT_CALL(*mock_service, SendMessageToMobile(_)).Times(0);
  EXPECT_EQ(ProcessResult::PROCESSED, module->ProcessMessage(message));
}

TEST_F(CanModuleTest, ProcessMessagePass) {
  application_manager::MessagePtr message = new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  message->set_function_id(MobileFunctionID::ON_INTERIOR_VEHICLE_DATA);

  std::string json = "{\"jsonrpc\": \"2.0\", \"method\": \"RC.OnInteriorVehicleData\",\
                        \"params\": {\"moduleData\": {\"moduleType\": \"CLIMATE\",\
                        \"moduleZone\":  {\"col\": 0,\"row\": 0,\"level\": 0,\"colspan\": 2,\
                        \"rowspan\": 2, \"levelspan\": 1}, \"climateControlData\": {\"fanSpeed\": 100} }}}";

  message->set_json_message(json);
  Json::Value json_value = MessageHelper::StringToValue(json);
  Json::Value moduleDescription;

  moduleDescription[message_params::kModuleType] =
      json_value[json_keys::kParams][message_params::kModuleData]
                                     [message_params::kModuleType];

  moduleDescription[message_params::kModuleZone] =
      json_value[json_keys::kParams][message_params::kModuleData]
                                     [message_params::kModuleZone];

  std::vector<application_manager::ApplicationSharedPtr> apps;
  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  apps.push_back(app_ptr);
  CANAppExtension* ext = new CANAppExtension(module->GetModuleID());
  ext->SubscribeToInteriorVehicleData(moduleDescription);
  EXPECT_CALL(*app, QueryInterface(module->GetModuleID())).
      WillRepeatedly(Return(ext));
  EXPECT_CALL(*app, app_id()).WillRepeatedly(Return(1));
  EXPECT_CALL(*mock_service, GetApplications(module->GetModuleID())
    ).WillRepeatedly(Return(apps));
  EXPECT_CALL(*mock_service, GetApplication(1)).Times(1)
      .WillOnce(Return(app_ptr));
  EXPECT_CALL(*mock_service, CheckPolicyPermissions(_)).Times(1)
      .WillOnce(Return(mobile_apis::Result::eType::SUCCESS));
  EXPECT_CALL(*mock_service, CheckModule(1, "CLIMATE")).WillOnce(Return(true));
  EXPECT_CALL(*mock_service, SendMessageToMobile(_)).Times(1);

  EXPECT_EQ(ProcessResult::PROCESSED, module->ProcessMessage(message));
}

TEST_F(CanModuleTest, RemoveAppExtensionPassWay) {
  MockApplication* app = new MockApplication();
  application_manager::ApplicationSharedPtr valid_app(app);

  EXPECT_CALL(*app, app_id()).WillOnce(Return(1));
  EXPECT_CALL(*mock_service, ResetAccess(1));
  EXPECT_CALL(*mock_service, GetApplication(1)).Times(1).WillOnce(
    Return(valid_app));
  EXPECT_CALL(*app, RemoveExtension(module->GetModuleID())).Times(1);

  module->RemoveAppExtension(1);
}

TEST_F(CanModuleTest, RemoveAppExtensionIfAppNoExist) {
  application_manager::ApplicationSharedPtr invalid_app;

  EXPECT_CALL(*mock_service, ResetAccess(_)).Times(0);
  EXPECT_CALL(*mock_service, GetApplication(_)).Times(1).WillOnce(
    Return(invalid_app));

  module->RemoveAppExtension(1);
}

TEST_F(CanModuleTest, SendResponseToMobile) {
  application_manager::MessagePtr message = new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));

  EXPECT_CALL(*mock_service, SendMessageToMobile(message)).Times(1);

  module->SendResponseToMobile(message);
}

TEST_F(CanModuleTest, IsAppForPluginSuccess) {
  MockApplication* app = new MockApplication();
  application_manager::ApplicationSharedPtr app_ptr(app);
  smart_objects::SmartObject obj(smart_objects::SmartType::SmartType_Array);
  obj[0] = 10;  //  REMOTE_CONTROL
  EXPECT_CALL(*app, app_types()).Times(2).WillRepeatedly(Return(&obj));
  application_manager::AppExtensionPtr ext;
  CANAppExtension* valid_ext = new CANAppExtension(module->GetModuleID());
  EXPECT_CALL(*app, QueryInterface(module->GetModuleID())).Times(2).
      WillOnce(Return(ext)).WillOnce(Return(valid_ext));
  EXPECT_CALL(*app, AddExtension(_)).WillOnce(Return(true));
  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_FULL;
  EXPECT_CALL(*app, hmi_level()).WillOnce(ReturnRef(hmi));
  EXPECT_CALL(*app, device()).WillOnce(Return(1));
  EXPECT_CALL(*mock_service, NotifyHMIAboutHMILevel(app_ptr, _)).Times(1);
  EXPECT_CALL(*mock_service, PrimaryDevice()).Times(1);
  ASSERT_TRUE(module->IsAppForPlugin(app_ptr));
}

TEST_F(CanModuleTest, IsAppForPluginNotNew) {
  MockApplication* app = new MockApplication();
  application_manager::ApplicationSharedPtr app_ptr(app);
  CANAppExtension* ext = new CANAppExtension(module->GetModuleID());
  EXPECT_CALL(*app, QueryInterface(module->GetModuleID())).
      WillOnce(Return(ext));
  EXPECT_CALL(*mock_service, NotifyHMIAboutHMILevel(app_ptr, _)).Times(0);
  ASSERT_TRUE(module->IsAppForPlugin(app_ptr));
}

TEST_F(CanModuleTest, IsAppForPluginFail) {
  MockApplication* app = new MockApplication();
  application_manager::ApplicationSharedPtr app_ptr(app);
  smart_objects::SmartObject obj(smart_objects::SmartType::SmartType_Array);
  obj[0] = 9;
  obj[1] = 8;
  application_manager::AppExtensionPtr ext;
  EXPECT_CALL(*app, QueryInterface(module->GetModuleID())).
      WillOnce(Return(ext));
  EXPECT_CALL(*app, app_types()).Times(2).WillRepeatedly(Return(&obj));
  ASSERT_FALSE(module->IsAppForPlugin(app_ptr));
}

TEST_F(CanModuleTest, OnAppHMILevelChanged) {
  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  std::string name("name");
  ON_CALL(*app, name()).WillByDefault(ReturnRef(name));
  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_NONE;
  EXPECT_CALL(*app, hmi_level()).Times(2).WillRepeatedly(ReturnRef(hmi));
  EXPECT_CALL(*mock_service, NotifyHMIAboutHMILevel(
      app_ptr, mobile_apis::HMILevel::eType::HMI_NONE));
  module->OnAppHMILevelChanged(app_ptr, mobile_apis::HMILevel::eType::HMI_FULL);
}

TEST_F(CanModuleTest, SetDriverDeviceOnRegister) {
  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);

  smart_objects::SmartObject obj(smart_objects::SmartType::SmartType_Array);
  obj[0] = 10;
  EXPECT_CALL(*app, app_types()).Times(2).WillRepeatedly(Return(&obj));
  application_manager::AppExtensionPtr invalid_ext;
  CANAppExtension* ext = new CANAppExtension(module->GetModuleID());
  EXPECT_CALL(*app, QueryInterface(module->GetModuleID())).Times(2).
      WillOnce(Return(invalid_ext)).WillOnce(Return(ext));
  EXPECT_CALL(*app, AddExtension(_)).WillOnce(Return(true));
  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_FULL;
  EXPECT_CALL(*app, hmi_level()).WillOnce(ReturnRef(hmi));
  EXPECT_CALL(*app, device()).WillOnce(Return(12));
  EXPECT_CALL(*mock_service, NotifyHMIAboutHMILevel(app_ptr, _)).Times(1);
  EXPECT_CALL(*mock_service, PrimaryDevice()).Times(1).WillOnce(Return(12));

  ASSERT_TRUE(module->IsAppForPlugin(app_ptr));
  ASSERT_TRUE(ext->is_on_driver_device());
}

TEST_F(CanModuleTest, SetDriverDeviceOnRegisterFail) {
  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);

  smart_objects::SmartObject obj(smart_objects::SmartType::SmartType_Array);
  obj[0] = 10;
  EXPECT_CALL(*app, app_types()).Times(2).WillRepeatedly(Return(&obj));
  application_manager::AppExtensionPtr invalid_ext;
  CANAppExtension* ext = new CANAppExtension(module->GetModuleID());
  EXPECT_CALL(*app, QueryInterface(module->GetModuleID())).Times(2).
      WillOnce(Return(invalid_ext)).WillOnce(Return(ext));
  EXPECT_CALL(*app, AddExtension(_)).WillOnce(Return(true));
  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_FULL;
  EXPECT_CALL(*app, hmi_level()).WillOnce(ReturnRef(hmi));
  EXPECT_CALL(*app, device()).WillOnce(Return(12));
  EXPECT_CALL(*mock_service, NotifyHMIAboutHMILevel(app_ptr, _)).Times(1);
  EXPECT_CALL(*mock_service, PrimaryDevice()).Times(1).WillOnce(Return(3));

  ASSERT_TRUE(module->IsAppForPlugin(app_ptr));
  ASSERT_FALSE(ext->is_on_driver_device());
}

TEST_F(CanModuleTest, ChangeDriverDevice) {
  Json::Value value(Json::ValueType::objectValue);
  value[json_keys::kMethod] = functional_modules::hmi_api::on_device_rank_changed;
  value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice][json_keys::kId] = 1;
  value[json_keys::kParams][message_params::kDevice][message_params::kName] = "1";
  value[json_keys::kParams][message_params::kRank] = "DRIVER";
  Json::FastWriter writer;
  std::string json_str = writer.write(value);

  application_manager::MessagePtr message = new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  message->set_function_name(functional_modules::hmi_api::on_device_rank_changed);
  message->set_message_type(application_manager::MessageType::kNotification);
  message->set_json_message(json_str);

  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
    application_manager::ApplicationSharedPtr app_ptr(app);
  std::vector<application_manager::ApplicationSharedPtr> apps;
  apps.push_back(app_ptr);
  CANAppExtension* ext = new CANAppExtension(module->GetModuleID());

  EXPECT_CALL(*app, device()).WillOnce(Return(1));
  EXPECT_CALL(*app, QueryInterface(module->GetModuleID())).
      WillOnce(Return(ext));
  EXPECT_CALL(*mock_service, GetApplications(module->GetModuleID())).
      WillRepeatedly(Return(apps));
  EXPECT_CALL(*mock_service, SetPrimaryDevice(1)).Times(1);
  EXPECT_CALL(*mock_service, ResetPrimaryDevice()).Times(0);

  CanModuleTest::HandleMessage(message);
  ASSERT_TRUE(ext->is_on_driver_device());
}

TEST_F(CanModuleTest, ChangeDriverDeviceOnOther) {
  Json::Value value(Json::ValueType::objectValue);
  value[json_keys::kMethod] = functional_modules::hmi_api::on_device_rank_changed;
  value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice][json_keys::kId] = 1;
  value[json_keys::kParams][message_params::kDevice][message_params::kName] = "1";
  value[json_keys::kParams][message_params::kRank] = "DRIVER";
  Json::FastWriter writer;
  std::string json_str = writer.write(value);

  application_manager::MessagePtr message = new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  message->set_function_name(functional_modules::hmi_api::on_device_rank_changed);
  message->set_message_type(application_manager::MessageType::kNotification);
  message->set_json_message(json_str);

  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  std::vector<application_manager::ApplicationSharedPtr> apps;
  apps.push_back(app_ptr);
  CANAppExtension* ext = new CANAppExtension(module->GetModuleID());
  ext->set_is_on_driver_device(true);
  EXPECT_TRUE(ext->is_on_driver_device());

  EXPECT_CALL(*app, device()).WillOnce(Return(2));
  EXPECT_CALL(*app, QueryInterface(module->GetModuleID())).
      WillOnce(Return(ext));
  EXPECT_CALL(*mock_service, GetApplications(module->GetModuleID())).
      WillRepeatedly(Return(apps));
  EXPECT_CALL(*mock_service, SetPrimaryDevice(1)).Times(1);
  EXPECT_CALL(*mock_service, ResetPrimaryDevice()).Times(0);

  CanModuleTest::HandleMessage(message);
  ASSERT_FALSE(ext->is_on_driver_device());
}

TEST_F(CanModuleTest, ChangeDriverDeviceToPassenger) {
  Json::Value value(Json::ValueType::objectValue);
  value[json_keys::kMethod] = functional_modules::hmi_api::on_device_rank_changed;
  value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice][json_keys::kId] = 1;
  value[json_keys::kParams][message_params::kDevice][message_params::kName] = "1";
  value[json_keys::kParams][message_params::kRank] = "PASSENGER";
  Json::FastWriter writer;
  std::string json_str = writer.write(value);

  application_manager::MessagePtr message = new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  message->set_function_name(functional_modules::hmi_api::on_device_rank_changed);
  message->set_message_type(application_manager::MessageType::kNotification);
  message->set_json_message(json_str);

  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  std::vector<application_manager::ApplicationSharedPtr> apps;
  apps.push_back(app_ptr);
  CANAppExtension* ext = new CANAppExtension(module->GetModuleID());
  ext->set_is_on_driver_device(true);
  EXPECT_TRUE(ext->is_on_driver_device());

  EXPECT_CALL(*app, device()).WillOnce(Return(1));
  EXPECT_CALL(*app, QueryInterface(module->GetModuleID())).
      WillOnce(Return(ext));
  EXPECT_CALL(*mock_service, GetApplications(module->GetModuleID())).
      WillRepeatedly(Return(apps));
  EXPECT_CALL(*mock_service, PrimaryDevice()).Times(1).WillOnce(Return(1));
  EXPECT_CALL(*mock_service, ResetPrimaryDevice()).Times(1);

  CanModuleTest::HandleMessage(message);
  ASSERT_FALSE(ext->is_on_driver_device());
}

TEST_F(CanModuleTest, ChangePassengerDeviceToPassenger) {
  Json::Value value(Json::ValueType::objectValue);
  value[json_keys::kMethod] = functional_modules::hmi_api::on_device_rank_changed;
  value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice][json_keys::kId] = 1;
  value[json_keys::kParams][message_params::kDevice][message_params::kName] = "1";
  value[json_keys::kParams][message_params::kRank] = "PASSENGER";
  Json::FastWriter writer;
  std::string json_str = writer.write(value);

  application_manager::MessagePtr message = new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  message->set_function_name(functional_modules::hmi_api::on_device_rank_changed);
  message->set_message_type(application_manager::MessageType::kNotification);
  message->set_json_message(json_str);

  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  std::vector<application_manager::ApplicationSharedPtr> apps;
  apps.push_back(app_ptr);
  CANAppExtension* ext = new CANAppExtension(module->GetModuleID());
  ext->set_is_on_driver_device(false);
  EXPECT_FALSE(ext->is_on_driver_device());

  EXPECT_CALL(*app, device()).Times(0);
  EXPECT_CALL(*app, QueryInterface(module->GetModuleID())).Times(0);
  EXPECT_CALL(*mock_service, GetApplications(module->GetModuleID())).
      WillRepeatedly(Return(apps));
  EXPECT_CALL(*mock_service, PrimaryDevice()).Times(1).WillOnce(Return(2));
  EXPECT_CALL(*mock_service, ResetPrimaryDevice()).Times(0);

  CanModuleTest::HandleMessage(message);
  ASSERT_FALSE(ext->is_on_driver_device());
}

TEST_F(CanModuleTest, DeactivateApp) {
  std::string reasons[] = {"AUDIO", "PHONECALL", "NAVIGATIONMAP", "PHONEMENU", "SYNCSETTINGS", "GENERAL"};

  application_manager::MessagePtr message = new application_manager::Message(
    protocol_handler::MessagePriority::FromServiceType(
      protocol_handler::ServiceType::kRpc));
  message->set_function_name(functional_modules::hmi_api::on_app_deactivated);
  message->set_message_type(application_manager::MessageType::kNotification);

  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  std::vector<application_manager::ApplicationSharedPtr> apps;
  apps.push_back(app_ptr);

  smart_objects::SmartObject obj(smart_objects::SmartType::SmartType_Array);
  obj[0] = 10;

  CANAppExtension* ext = new CANAppExtension(module->GetModuleID());
  ext->set_is_on_driver_device(true);

  EXPECT_CALL(*app, hmi_app_id()).WillRepeatedly(Return(11));
  EXPECT_CALL(*app, IsFullscreen()).WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_service, GetApplications(module->GetModuleID())).
      WillRepeatedly(Return(apps));
  EXPECT_CALL(*mock_service,
              ChangeNotifyHMILevel(_,
                                   mobile_apis::HMILevel::eType::HMI_LIMITED)).
                                       Times(6);

  for (size_t i = 0; i < 6; ++ i) {
    Json::Value value(Json::ValueType::objectValue);
    value[json_keys::kMethod] = functional_modules::hmi_api::on_app_deactivated;
    value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
    value[json_keys::kParams][message_params::kHMIAppID] = 11;
    value[json_keys::kParams]["reason"] = reasons[i];
    Json::FastWriter writer;
    std::string json_str = writer.write(value);
    message->set_json_message(json_str);

    CanModuleTest::HandleMessage(message);
  }
}

TEST_F(CanModuleTest, CanAppChangeHMILevelPrimary) {
  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  std::vector<application_manager::ApplicationSharedPtr> apps;
  apps.push_back(app_ptr);

  CANAppExtension* ext = new CANAppExtension(module->GetModuleID());
  ext->set_is_on_driver_device(true);

  EXPECT_CALL(*app, QueryInterface(module->GetModuleID())).
      WillRepeatedly(Return(ext));

  ASSERT_TRUE(module->CanAppChangeHMILevel(app_ptr, mobile_apis::HMILevel::eType::HMI_FULL));
  ASSERT_TRUE(module->CanAppChangeHMILevel(app_ptr, mobile_apis::HMILevel::eType::HMI_LIMITED));
  ASSERT_TRUE(module->CanAppChangeHMILevel(app_ptr, mobile_apis::HMILevel::eType::HMI_BACKGROUND));
  ASSERT_TRUE(module->CanAppChangeHMILevel(app_ptr, mobile_apis::HMILevel::eType::HMI_NONE));
}

TEST_F(CanModuleTest, CanAppChangeHMILevelPassenger) {
  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  std::vector<application_manager::ApplicationSharedPtr> apps;
  apps.push_back(app_ptr);

  CANAppExtension* ext = new CANAppExtension(module->GetModuleID());
  ext->set_is_on_driver_device(false);

  EXPECT_CALL(*app, QueryInterface(module->GetModuleID())).
      WillRepeatedly(Return(ext));

  ASSERT_FALSE(module->CanAppChangeHMILevel(app_ptr, mobile_apis::HMILevel::eType::HMI_FULL));
  ASSERT_TRUE(module->CanAppChangeHMILevel(app_ptr, mobile_apis::HMILevel::eType::HMI_LIMITED));
  ASSERT_TRUE(module->CanAppChangeHMILevel(app_ptr, mobile_apis::HMILevel::eType::HMI_BACKGROUND));
  ASSERT_TRUE(module->CanAppChangeHMILevel(app_ptr, mobile_apis::HMILevel::eType::HMI_NONE));
}

}  // namespace can_cooperation
