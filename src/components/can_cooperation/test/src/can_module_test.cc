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
#include "application_manager/mock_application.h"
#include "mock_service.h"
#include "mock_can_connection.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "application_manager/mock_message_helper.h"

using functional_modules::PluginInfo;
using functional_modules::ProcessResult;
using functional_modules::MobileFunctionID;
using application_manager::ServicePtr;

using application_manager::MockService;
using application_manager::MockMessageHelper;
using test::components::application_manager_test::MockApplication;

using ::testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::Return;
using ::testing::ReturnRef;

namespace can_cooperation {

class CanModuleTest : public ::testing::Test {
 public:
  CanModuleTest()
      : module_(utils::MakeShared<CANModule>())
      , mock_service_(utils::MakeShared<MockService>())
      , mock_conn_(utils::MakeShared<MockCANConnection>())
      , mock_message_helper_(*MockMessageHelper::message_helper_mock()) {}
  void HandleMessage(application_manager::MessagePtr msg) {
    module_->ProcessHMIMessage(msg);
  }

 protected:
  utils::SharedPtr<CANModule> module_;
  utils::SharedPtr<MockService> mock_service_;
  utils::SharedPtr<MockCANConnection> mock_conn_;
  MockMessageHelper& mock_message_helper_;
  std::vector<application_manager::ApplicationSharedPtr> apps_;

  void SetUp() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    ::can_cooperation::CANConnectionSPtr conn(mock_conn_);
    module_->set_can_connection(conn);

    ServicePtr exp_service(mock_service_);
    module_->set_service(exp_service);
    ServicePtr out_service = module_->service();
    EXPECT_EQ(exp_service.get(), out_service.get());
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    EXPECT_CALL(*mock_service_, GetApplications(module_->GetModuleID()))
        .WillOnce(Return(apps_));
  }
};

TEST_F(CanModuleTest, Create) {
  EXPECT_EQ(153, module_->GetModuleID());
  PluginInfo plugin = module_->GetPluginInfo();
  EXPECT_EQ(plugin.name, "ReverseSDLPlugin");
  EXPECT_EQ(plugin.version, 1);
}

TEST_F(CanModuleTest, ProcessMessageWrongMessage) {
  application_manager::MessagePtr message = new application_manager::Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  message->set_function_id(-1);
  EXPECT_CALL(*mock_service_, SendMessageToMobile(_)).Times(0);
  EXPECT_EQ(ProcessResult::CANNOT_PROCESS, module_->ProcessMessage(message));
}

TEST_F(CanModuleTest, ProcessMessageEmptyAppsList) {
  application_manager::MessagePtr message = new application_manager::Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  message->set_function_id(MobileFunctionID::ON_INTERIOR_VEHICLE_DATA);
  message->set_function_name("OnInteriorVehicleData");

  std::string json =
      "{\"jsonrpc\": \"2.0\", \"method\": \"RC.OnInteriorVehicleData\",\
        \"params\": {\"moduleData\": {\"moduleType\": \"CLIMATE\",\
        \"moduleZone\":  {\"col\": 0,\"row\": 0,\"level\": 0,\"colspan\": 2,\
        \"rowspan\": 2, \"levelspan\": 1}, \"climateControlData\": {\
        \"fanSpeed\": 100} }}}";
  message->set_json_message(json);

  EXPECT_CALL(*mock_service_, GetApplications(module_->GetModuleID()))
      .WillOnce(Return(apps_));
  EXPECT_CALL(*mock_service_, SendMessageToMobile(_)).Times(0);
  EXPECT_EQ(ProcessResult::PROCESSED, module_->ProcessMessage(message));
}

TEST_F(CanModuleTest, ProcessMessagePass) {
  application_manager::MessagePtr message = new application_manager::Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  message->set_function_id(MobileFunctionID::ON_INTERIOR_VEHICLE_DATA);

  std::string json =
      "{ \"jsonrpc\": \"2.0\",\"method\": \"RC.OnInteriorVehicleData\",\
         \"params\":{\"moduleData\":{\
         \"moduleType\": \"CLIMATE\",\"moduleZone\":{\"col\": 0,\"row\": 0,\
         \"level\": 0,\"colspan\": 2,\"rowspan\": 2,\
         \"levelspan\": 1},\"climateControlData\": {\"fanSpeed\": 100}}}}";

  message->set_json_message(json);

  application_manager::BinaryData buf;
  application_manager::BinaryData* data = &buf;
  data->push_back(1);

  message->set_binary_data(data);

  Json::Value json_value = MessageHelper::StringToValue(json);
  Json::Value moduleDescription;

  moduleDescription[message_params::kModuleType] =
      json_value[json_keys::kParams][message_params::kModuleData]
                [message_params::kModuleType];

  moduleDescription[message_params::kModuleZone] =
      json_value[json_keys::kParams][message_params::kModuleData]
                [message_params::kModuleZone];

  utils::SharedPtr<MockApplication> app = utils::MakeShared<MockApplication>();

  apps_.push_back(app);

  utils::SharedPtr<CANAppExtension> ext =
      utils::MakeShared<CANAppExtension>(module_->GetModuleID());
  ext->SubscribeToInteriorVehicleData(moduleDescription);

  EXPECT_CALL(*app, QueryInterface(module_->GetModuleID()))
      .WillOnce(Return(ext));
  EXPECT_CALL(*app, app_id()).WillRepeatedly(Return(1));
  EXPECT_CALL(*mock_service_, GetApplications(module_->GetModuleID()))
      .WillOnce(Return(apps_));
  EXPECT_CALL(*mock_service_, GetApplication(1)).WillOnce(Return(app));
  EXPECT_CALL(*mock_service_, CheckPolicyPermissions(_))
      .WillOnce(Return(mobile_apis::Result::eType::SUCCESS));
  EXPECT_CALL(*mock_service_, CheckModule(1, "CLIMATE")).WillOnce(Return(true));
  EXPECT_CALL(*mock_service_, SendMessageToMobile(_));

  EXPECT_EQ(ProcessResult::PROCESSED, module_->ProcessMessage(message));
}

TEST_F(CanModuleTest, RemoveAppExtensionPassWay) {
  MockApplication* app = new MockApplication();
  application_manager::ApplicationSharedPtr valid_app(app);

  EXPECT_CALL(*app, app_id()).WillOnce(Return(1));
  EXPECT_CALL(*mock_service_, ResetAccess(1));
  EXPECT_CALL(*mock_service_, GetApplication(1)).WillOnce(Return(valid_app));
  EXPECT_CALL(*app, RemoveExtension(module_->GetModuleID()));

  module_->RemoveAppExtension(1);
}

TEST_F(CanModuleTest, RemoveAppExtensionIfAppNoExist) {
  application_manager::ApplicationSharedPtr invalid_app;

  EXPECT_CALL(*mock_service_, ResetAccess(_)).Times(0);
  EXPECT_CALL(*mock_service_, GetApplication(_)).WillOnce(Return(invalid_app));

  module_->RemoveAppExtension(1);
}

TEST_F(CanModuleTest, SendResponseToMobile) {
  application_manager::MessagePtr message = new application_manager::Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));

  EXPECT_CALL(*mock_service_, SendMessageToMobile(message));

  module_->SendResponseToMobile(message);
}

TEST_F(CanModuleTest, IsAppForPluginSuccess) {
  MockApplication* app = new MockApplication();
  application_manager::ApplicationSharedPtr app_ptr(app);
  application_manager::AppExtensionPtr ext;
  CANAppExtension* valid_ext = new CANAppExtension(module_->GetModuleID());
  EXPECT_CALL(*app, QueryInterface(module_->GetModuleID()))
      .WillOnce(Return(ext))
      .WillRepeatedly(Return(valid_ext));
  EXPECT_CALL(*app, AddExtension(_)).WillOnce(Return(true));
  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_FULL;
  EXPECT_CALL(*app, hmi_level()).WillRepeatedly(Return(hmi));
  EXPECT_CALL(*app, device()).WillOnce(Return(1));
  EXPECT_CALL(*mock_service_, NotifyHMIAboutHMILevel(app_ptr, _));
  EXPECT_CALL(*mock_service_, PrimaryDevice());
  EXPECT_CALL(*mock_service_, IsRemoteControlApplication(app_ptr))
      .WillOnce(Return(true));
  ASSERT_TRUE(module_->IsAppForPlugin(app_ptr));
}

TEST_F(CanModuleTest, IsAppForPluginNotNew) {
  MockApplication* app = new MockApplication();
  application_manager::ApplicationSharedPtr app_ptr(app);
  CANAppExtension* ext = new CANAppExtension(module_->GetModuleID());
  EXPECT_CALL(*app, QueryInterface(module_->GetModuleID()))
      .WillOnce(Return(ext));
  EXPECT_CALL(*mock_service_, NotifyHMIAboutHMILevel(app_ptr, _)).Times(0);
  ASSERT_TRUE(module_->IsAppForPlugin(app_ptr));
}

TEST_F(CanModuleTest, IsAppForPluginFail) {
  MockApplication* app = new MockApplication();
  application_manager::ApplicationSharedPtr app_ptr(app);
  application_manager::AppExtensionPtr ext;
  EXPECT_CALL(*app, QueryInterface(module_->GetModuleID()))
      .WillOnce(Return(ext));
  EXPECT_CALL(*mock_service_, IsRemoteControlApplication(app_ptr))
      .Times(1)
      .WillOnce(Return(false));
  ASSERT_FALSE(module_->IsAppForPlugin(app_ptr));
}

TEST_F(CanModuleTest, OnAppHMILevelChanged) {
  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  const application_manager::custom_str::CustomString name("name");
  ON_CALL(*app, name()).WillByDefault(ReturnRef(name));
  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_NONE;
  EXPECT_CALL(*app, hmi_level()).WillRepeatedly(Return(hmi));
  EXPECT_CALL(
      *mock_service_,
      NotifyHMIAboutHMILevel(app_ptr, mobile_apis::HMILevel::eType::HMI_NONE));
  module_->OnAppHMILevelChanged(app_ptr,
                                mobile_apis::HMILevel::eType::HMI_FULL);
}

TEST_F(CanModuleTest, SetDriverDeviceOnRegister) {
  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  application_manager::AppExtensionPtr invalid_ext;
  CANAppExtension* ext = new CANAppExtension(module_->GetModuleID());
  EXPECT_CALL(*app, QueryInterface(module_->GetModuleID()))
      .Times(2)
      .WillOnce(Return(invalid_ext))
      .WillRepeatedly(Return(ext));
  EXPECT_CALL(*app, AddExtension(_)).WillOnce(Return(true));
  EXPECT_CALL(*app, device()).WillOnce(Return(12));
  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_NONE;
  EXPECT_CALL(*app, hmi_level()).Times(1).WillRepeatedly(Return(hmi));
  EXPECT_CALL(*mock_service_, NotifyHMIAboutHMILevel(app_ptr, _)).Times(1);
  EXPECT_CALL(*mock_service_, PrimaryDevice()).Times(1).WillOnce(Return(12));
  EXPECT_CALL(*mock_service_, IsRemoteControlApplication(app_ptr))
      .Times(1)
      .WillOnce(Return(true));

  ASSERT_TRUE(module_->IsAppForPlugin(app_ptr));
  ASSERT_TRUE(ext->is_on_driver_device());
}

TEST_F(CanModuleTest, SetDriverDeviceOnRegisterFail) {
  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  application_manager::AppExtensionPtr invalid_ext;
  CANAppExtension* ext = new CANAppExtension(module_->GetModuleID());
  EXPECT_CALL(*app, QueryInterface(module_->GetModuleID()))
      .Times(2)
      .WillOnce(Return(invalid_ext))
      .WillRepeatedly(Return(ext));
  EXPECT_CALL(*app, AddExtension(_)).WillOnce(Return(true));
  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_FULL;
  EXPECT_CALL(*app, hmi_level()).WillRepeatedly(Return(hmi));
  EXPECT_CALL(*app, device()).WillOnce(Return(12));
  EXPECT_CALL(*mock_service_, NotifyHMIAboutHMILevel(app_ptr, _)).Times(1);
  EXPECT_CALL(*mock_service_, PrimaryDevice()).Times(1).WillOnce(Return(3));
  EXPECT_CALL(*mock_service_, IsRemoteControlApplication(app_ptr))
      .Times(1)
      .WillOnce(Return(true));

  ASSERT_TRUE(module_->IsAppForPlugin(app_ptr));
  ASSERT_FALSE(ext->is_on_driver_device());
}

TEST_F(CanModuleTest, ChangeDriverDevice) {
  Json::Value value(Json::ValueType::objectValue);
  value[json_keys::kMethod] =
      functional_modules::hmi_api::on_device_rank_changed;
  value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice] =
      Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice][json_keys::kId] = 1;
  value[json_keys::kParams][message_params::kDevice][message_params::kName] =
      "1";
  value[json_keys::kParams][message_params::kRank] = "DRIVER";
  Json::FastWriter writer;
  std::string json_str = writer.write(value);

  application_manager::MessagePtr message = new application_manager::Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  message->set_function_name(
      functional_modules::hmi_api::on_device_rank_changed);
  message->set_json_message(json_str);

  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  std::vector<application_manager::ApplicationSharedPtr> apps;
  apps.push_back(app_ptr);
  CANAppExtension* ext = new CANAppExtension(module_->GetModuleID());

  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_FULL;
  EXPECT_CALL(*app, hmi_level()).WillRepeatedly(Return(hmi));
  EXPECT_CALL(*app, device()).WillRepeatedly(Return(1));
  EXPECT_CALL(*app, QueryInterface(module_->GetModuleID()))
      .WillRepeatedly(Return(ext));
  EXPECT_CALL(*mock_service_, GetApplications(module_->GetModuleID()))
      .WillOnce(Return(apps));
  EXPECT_CALL(*mock_service_, SetPrimaryDevice(1)).Times(1);
  EXPECT_CALL(*mock_service_, ResetPrimaryDevice()).Times(0);

  CanModuleTest::HandleMessage(message);
  ASSERT_TRUE(ext->is_on_driver_device());
}

TEST_F(CanModuleTest, ChangeDriverDeviceOnOther) {
  Json::Value value(Json::ValueType::objectValue);
  value[json_keys::kMethod] =
      functional_modules::hmi_api::on_device_rank_changed;
  value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice] =
      Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice][json_keys::kId] = 1;
  value[json_keys::kParams][message_params::kDevice][message_params::kName] =
      "1";
  value[json_keys::kParams][message_params::kRank] = "DRIVER";
  Json::FastWriter writer;
  std::string json_str = writer.write(value);

  application_manager::MessagePtr message = new application_manager::Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  message->set_function_name(
      functional_modules::hmi_api::on_device_rank_changed);
  message->set_message_type(application_manager::MessageType::kNotification);
  message->set_json_message(json_str);

  std::vector<application_manager::ApplicationSharedPtr> apps;
  NiceMock<MockApplication>* app_1 = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr_1(app_1);
  apps.push_back(app_ptr_1);
  NiceMock<MockApplication>* app_2 = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr_2(app_2);
  apps.push_back(app_ptr_2);

  CANAppExtension* ext_1 = new CANAppExtension(module_->GetModuleID());
  ext_1->set_is_on_driver_device(true);
  EXPECT_TRUE(ext_1->is_on_driver_device());
  CANAppExtension* ext_2 = new CANAppExtension(module_->GetModuleID());
  EXPECT_FALSE(ext_2->is_on_driver_device());

  EXPECT_CALL(*app_1, device()).WillOnce(Return(1));
  EXPECT_CALL(*app_2, device()).WillOnce(Return(2));
  EXPECT_CALL(*app_1, QueryInterface(module_->GetModuleID()))
      .WillOnce(Return(ext_1));
  EXPECT_CALL(*mock_service_, GetApplications(module_->GetModuleID()))
      .WillRepeatedly(Return(apps));
  EXPECT_CALL(*mock_service_, SetPrimaryDevice(1)).Times(1);
  EXPECT_CALL(*mock_service_, ResetPrimaryDevice()).Times(0);

  CanModuleTest::HandleMessage(message);
  ASSERT_TRUE(ext_1->is_on_driver_device());
  ASSERT_FALSE(ext_2->is_on_driver_device());
}

TEST_F(CanModuleTest, ChangeDriverDeviceToPassenger) {
  Json::Value value(Json::ValueType::objectValue);
  value[json_keys::kMethod] =
      functional_modules::hmi_api::on_device_rank_changed;
  value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice] =
      Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice][json_keys::kId] = 1;
  value[json_keys::kParams][message_params::kDevice][message_params::kName] =
      "1";
  value[json_keys::kParams][message_params::kRank] = "PASSENGER";
  Json::FastWriter writer;
  std::string json_str = writer.write(value);

  application_manager::MessagePtr message = new application_manager::Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  message->set_function_name(
      functional_modules::hmi_api::on_device_rank_changed);
  message->set_message_type(application_manager::MessageType::kNotification);
  message->set_json_message(json_str);

  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  std::vector<application_manager::ApplicationSharedPtr> apps;
  apps.push_back(app_ptr);
  CANAppExtension* ext = new CANAppExtension(module_->GetModuleID());
  ext->set_is_on_driver_device(true);
  EXPECT_TRUE(ext->is_on_driver_device());

  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_FULL;
  EXPECT_CALL(*app, hmi_level()).WillRepeatedly(Return(hmi));
  EXPECT_CALL(*app, device()).WillOnce(Return(1));
  EXPECT_CALL(*app, QueryInterface(module_->GetModuleID()))
      .WillRepeatedly(Return(ext));
  EXPECT_CALL(*mock_service_, GetApplications(module_->GetModuleID()))
      .WillRepeatedly(Return(apps));
  EXPECT_CALL(*mock_service_, PrimaryDevice()).Times(1).WillOnce(Return(1));
  EXPECT_CALL(*mock_service_, ResetPrimaryDevice()).Times(1);

  CanModuleTest::HandleMessage(message);
  ASSERT_FALSE(ext->is_on_driver_device());
}

TEST_F(CanModuleTest, ChangePassengerDeviceToPassenger) {
  Json::Value value(Json::ValueType::objectValue);
  value[json_keys::kMethod] =
      functional_modules::hmi_api::on_device_rank_changed;
  value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice] =
      Json::Value(Json::ValueType::objectValue);
  value[json_keys::kParams][message_params::kDevice][json_keys::kId] = 1;
  value[json_keys::kParams][message_params::kDevice][message_params::kName] =
      "1";
  value[json_keys::kParams][message_params::kRank] = "PASSENGER";
  Json::FastWriter writer;
  std::string json_str = writer.write(value);

  application_manager::MessagePtr message = new application_manager::Message(
      protocol_handler::MessagePriority::FromServiceType(
          protocol_handler::ServiceType::kRpc));
  message->set_function_name(
      functional_modules::hmi_api::on_device_rank_changed);
  message->set_message_type(application_manager::MessageType::kNotification);
  message->set_json_message(json_str);

  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  std::vector<application_manager::ApplicationSharedPtr> apps;
  apps.push_back(app_ptr);
  CANAppExtension* ext = new CANAppExtension(module_->GetModuleID());
  ext->set_is_on_driver_device(false);
  EXPECT_FALSE(ext->is_on_driver_device());

  EXPECT_CALL(*app, device()).WillOnce(Return(1));
  EXPECT_CALL(*app, QueryInterface(module_->GetModuleID()))
      .WillRepeatedly(Return(ext));
  EXPECT_CALL(*mock_service_, GetApplications(module_->GetModuleID()))
      .WillRepeatedly(Return(apps));
  EXPECT_CALL(*mock_service_, PrimaryDevice()).Times(1).WillOnce(Return(2));
  EXPECT_CALL(*mock_service_, ResetPrimaryDevice()).Times(0);

  CanModuleTest::HandleMessage(message);
  ASSERT_FALSE(ext->is_on_driver_device());
}

TEST_F(CanModuleTest, DeactivateApp) {
  std::string reasons[] = {"AUDIO",
                           "PHONECALL",
                           "NAVIGATIONMAP",
                           "PHONEMENU",
                           "SYNCSETTINGS",
                           "GENERAL"};

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

  CANAppExtension* ext = new CANAppExtension(module_->GetModuleID());
  ext->set_is_on_driver_device(true);

  EXPECT_CALL(*app, hmi_app_id()).WillRepeatedly(Return(11));
  EXPECT_CALL(*app, IsFullscreen()).WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_service_, GetApplications(module_->GetModuleID()))
      .WillRepeatedly(Return(apps));
  EXPECT_CALL(*mock_service_,
              ChangeNotifyHMILevel(
                  _, mobile_apis::HMILevel::eType::HMI_LIMITED)).Times(6);

  for (size_t i = 0; i < 6; ++i) {
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

  CANAppExtension* ext = new CANAppExtension(module_->GetModuleID());
  ext->set_is_on_driver_device(true);

  EXPECT_CALL(*app, QueryInterface(module_->GetModuleID()))
      .WillRepeatedly(Return(ext));

  ASSERT_TRUE(module_->CanAppChangeHMILevel(
      app_ptr, mobile_apis::HMILevel::eType::HMI_FULL));
  ASSERT_TRUE(module_->CanAppChangeHMILevel(
      app_ptr, mobile_apis::HMILevel::eType::HMI_LIMITED));
  ASSERT_TRUE(module_->CanAppChangeHMILevel(
      app_ptr, mobile_apis::HMILevel::eType::HMI_BACKGROUND));
  ASSERT_TRUE(module_->CanAppChangeHMILevel(
      app_ptr, mobile_apis::HMILevel::eType::HMI_NONE));
}

TEST_F(CanModuleTest, CanAppChangeHMILevelPassenger) {
  NiceMock<MockApplication>* app = new NiceMock<MockApplication>();
  application_manager::ApplicationSharedPtr app_ptr(app);
  std::vector<application_manager::ApplicationSharedPtr> apps;
  apps.push_back(app_ptr);

  CANAppExtension* ext = new CANAppExtension(module_->GetModuleID());
  ext->set_is_on_driver_device(false);

  EXPECT_CALL(*app, QueryInterface(module_->GetModuleID()))
      .WillRepeatedly(Return(ext));

  ASSERT_FALSE(module_->CanAppChangeHMILevel(
      app_ptr, mobile_apis::HMILevel::eType::HMI_FULL));
  ASSERT_FALSE(module_->CanAppChangeHMILevel(
      app_ptr, mobile_apis::HMILevel::eType::HMI_LIMITED));
  ASSERT_TRUE(module_->CanAppChangeHMILevel(
      app_ptr, mobile_apis::HMILevel::eType::HMI_BACKGROUND));
  ASSERT_TRUE(module_->CanAppChangeHMILevel(
      app_ptr, mobile_apis::HMILevel::eType::HMI_NONE));
}
}  // namespace can_cooperation
