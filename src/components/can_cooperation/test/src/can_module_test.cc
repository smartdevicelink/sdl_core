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
using ::testing::Eq;
using ::application_manager::Message;
using ::application_manager::MessageType;
using ::application_manager::ApplicationSharedPtr;
using ::protocol_handler::MessagePriority;

namespace {
const bool kDeviceHandle = 1u;
const std::string kDeviceId = "1";
const std::string kDeviceName = "1";
}

namespace can_cooperation {

class CanModuleTest : public ::testing::Test {
 public:
  CanModuleTest()
      : mock_service_(utils::MakeShared<NiceMock<MockService> >())
      , mock_message_helper_(*MockMessageHelper::message_helper_mock())
      , app0_(utils::MakeShared<NiceMock<MockApplication> >())
      , app1_(utils::MakeShared<NiceMock<MockApplication> >())
      , message_(utils::MakeShared<Message>(MessagePriority::FromServiceType(
            protocol_handler::ServiceType::kRpc)))
      , can_app_extention_(
            utils::MakeShared<CANAppExtension>(module_.GetModuleID())) {}

  void HandleMessage() {
    module_.ProcessHMIMessage(message_);
  }

 protected:
  CANModule module_;
  utils::SharedPtr<NiceMock<MockService> > mock_service_;
  MockMessageHelper& mock_message_helper_;
  std::vector<ApplicationSharedPtr> apps_;
  utils::SharedPtr<NiceMock<MockApplication> > app0_;
  utils::SharedPtr<NiceMock<MockApplication> > app1_;
  application_manager::MessagePtr message_;
  utils::SharedPtr<CANAppExtension> can_app_extention_;

  void SetUp() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    ServicePtr exp_service(mock_service_);
    module_.set_service(exp_service);
    ServicePtr out_service = module_.service();
    EXPECT_EQ(exp_service.get(), out_service.get());
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    EXPECT_CALL(*mock_service_, GetApplications(module_.GetModuleID()))
        .WillOnce(Return(apps_));
  }
};

TEST_F(CanModuleTest, Create) {
  EXPECT_EQ(153, module_.GetModuleID());
  PluginInfo plugin = module_.GetPluginInfo();
  EXPECT_EQ(plugin.name, "ReverseSDLPlugin");
  EXPECT_EQ(plugin.version, 1);
}

TEST_F(CanModuleTest, ProcessMessageWrongMessage) {
  message_->set_function_id(-1);
  EXPECT_CALL(*mock_service_, SendMessageToMobile(_)).Times(0);
  EXPECT_EQ(ProcessResult::CANNOT_PROCESS, module_.ProcessMessage(message_));
}

TEST_F(CanModuleTest, ProcessMessageEmptyapps_List) {
  message_->set_function_id(MobileFunctionID::ON_INTERIOR_VEHICLE_DATA);
  message_->set_function_name("OnInteriorVehicleData");

  std::string json =
      "{\"jsonrpc\": \"2.0\", \"method\": \"RC.OnInteriorVehicleData\",\
        \"params\": {\"moduleData\": {\"moduleType\": \"CLIMATE\",\
        \"climateControlData\": {\"fanSpeed\": 100} }}}";
  message_->set_json_message(json);

  EXPECT_CALL(*mock_service_, ValidateMessageBySchema(_))
      .WillOnce(Return(application_manager::SCHEMA_MISMATCH));
  EXPECT_CALL(*mock_service_, SendMessageToMobile(_)).Times(0);

  // EXPECT_EQ(ProcessResult::PROCESSED, module_.ProcessMessage(message_));
  EXPECT_EQ(ProcessResult::CANNOT_PROCESS, module_.ProcessHMIMessage(message_));
}

TEST_F(CanModuleTest, ProcessMessagePass) {
  message_->set_function_id(MobileFunctionID::ON_INTERIOR_VEHICLE_DATA);

  std::string json =
      "{ \"jsonrpc\": \"2.0\",\"method\": \"RC.OnInteriorVehicleData\",\
         \"params\":{\"moduleData\":{\
         \"moduleType\": \"CLIMATE\",\"climateControlData\": {\"fanSpeed\": 100}}}}";

  message_->set_json_message(json);

  application_manager::BinaryData buf;
  application_manager::BinaryData* data = &buf;
  data->push_back(1);

  message_->set_binary_data(data);

  Json::Value json_value = MessageHelper::StringToValue(json);
  Json::Value moduleDescription;

  moduleDescription[message_params::kModuleType] =
      json_value[json_keys::kParams][message_params::kModuleData]
                [message_params::kModuleType];

  apps_.push_back(app0_);
  can_app_extention_->SubscribeToInteriorVehicleData(moduleDescription);
  EXPECT_CALL(*mock_service_, ValidateMessageBySchema(_))
      .WillOnce(Return(application_manager::SUCCESS));
  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
      .WillOnce(Return(can_app_extention_));
  EXPECT_CALL(*app0_, app_id()).WillRepeatedly(Return(1));
  EXPECT_CALL(*mock_service_, GetApplications(module_.GetModuleID()))
      .WillOnce(Return(apps_));
  EXPECT_CALL(*mock_service_, GetApplication(1)).WillOnce(Return(app0_));
  EXPECT_CALL(*mock_service_, CheckPolicyPermissions(_))
      .WillOnce(Return(mobile_apis::Result::eType::SUCCESS));
  EXPECT_CALL(*mock_service_, CheckModule(1, "CLIMATE")).WillOnce(Return(true));
  EXPECT_CALL(*mock_service_, SendMessageToMobile(_));

  EXPECT_EQ(ProcessResult::PROCESSED, module_.ProcessHMIMessage(message_));
}

TEST_F(CanModuleTest, RemoveAppExtensionPassWay) {
  EXPECT_CALL(*mock_service_, GetApplication(1)).WillOnce(Return(app0_));
  EXPECT_CALL(*app0_, RemoveExtension(module_.GetModuleID()));

  module_.RemoveAppExtension(1);
}

TEST_F(CanModuleTest, RemoveAppExtensionIfAppNoExist) {
  ApplicationSharedPtr invalid_app;

  EXPECT_CALL(*mock_service_, GetApplication(_)).WillOnce(Return(invalid_app));

  module_.RemoveAppExtension(1);
}

TEST_F(CanModuleTest, SendResponseToMobile) {
  EXPECT_CALL(*mock_service_, SendMessageToMobile(message_));

  module_.SendResponseToMobile(message_);
}

TEST_F(CanModuleTest, IsAppForPluginSuccess) {
  application_manager::AppExtensionPtr invalid_ext;
  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
      .WillOnce(Return(invalid_ext))
      .WillRepeatedly(Return(can_app_extention_));
  EXPECT_CALL(*app0_, AddExtension(_)).WillOnce(Return(true));
  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_FULL;
  EXPECT_CALL(*app0_, hmi_level()).WillRepeatedly(Return(hmi));
  ON_CALL(*app0_, device()).WillByDefault(Return(1));
  EXPECT_CALL(*mock_service_, NotifyHMIAboutHMILevel(Eq(app0_), _));
  EXPECT_CALL(*mock_service_, PrimaryDevice());
  EXPECT_CALL(*mock_service_, IsRemoteControlApplication(Eq(app0_)))
      .WillOnce(Return(true));
  ASSERT_TRUE(module_.IsAppForPlugin(app0_));
}

TEST_F(CanModuleTest, IsAppForPluginNotNew) {
  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
      .WillOnce(Return(can_app_extention_));
  EXPECT_CALL(*mock_service_, NotifyHMIAboutHMILevel(Eq(app0_), _)).Times(0);
  ASSERT_TRUE(module_.IsAppForPlugin(app0_));
}

TEST_F(CanModuleTest, IsAppForPluginFail) {
  application_manager::AppExtensionPtr invalid_ext;
  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
      .WillOnce(Return(invalid_ext));
  EXPECT_CALL(*mock_service_, IsRemoteControlApplication(Eq(app0_)))
      .Times(1)
      .WillOnce(Return(false));
  ASSERT_FALSE(module_.IsAppForPlugin(app0_));
}

TEST_F(CanModuleTest, OnAppHMILevelChanged) {
  const application_manager::custom_str::CustomString name("name");
  ON_CALL(*app0_, name()).WillByDefault(ReturnRef(name));
  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_NONE;
  EXPECT_CALL(*app0_, hmi_level()).WillRepeatedly(Return(hmi));
  EXPECT_CALL(*mock_service_,
              NotifyHMIAboutHMILevel(Eq(app0_),
                                     mobile_apis::HMILevel::eType::HMI_NONE));
  module_.OnAppHMILevelChanged(app0_, mobile_apis::HMILevel::eType::HMI_FULL);
}

TEST_F(CanModuleTest, SetDriverDeviceOnRegister) {
  application_manager::AppExtensionPtr invalid_ext;
  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
      .Times(2)
      .WillOnce(Return(invalid_ext))
      .WillRepeatedly(Return(can_app_extention_));
  EXPECT_CALL(*app0_, AddExtension(_)).WillOnce(Return(true));
  ON_CALL(*app0_, device()).WillByDefault(Return(12));
  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_NONE;
  EXPECT_CALL(*app0_, hmi_level()).Times(1).WillRepeatedly(Return(hmi));
  EXPECT_CALL(*mock_service_, NotifyHMIAboutHMILevel(Eq(app0_), _)).Times(1);
  EXPECT_CALL(*mock_service_, PrimaryDevice()).Times(1).WillOnce(Return(12));
  EXPECT_CALL(*mock_service_, IsRemoteControlApplication(Eq(app0_)))
      .Times(1)
      .WillOnce(Return(true));

  ASSERT_TRUE(module_.IsAppForPlugin(app0_));
  ASSERT_TRUE(can_app_extention_->is_on_driver_device());
}

TEST_F(CanModuleTest, SetDriverDeviceOnRegisterFail) {
  application_manager::AppExtensionPtr invalid_ext;
  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
      .Times(2)
      .WillOnce(Return(invalid_ext))
      .WillRepeatedly(Return(can_app_extention_));
  EXPECT_CALL(*app0_, AddExtension(_)).WillOnce(Return(true));
  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_FULL;
  EXPECT_CALL(*app0_, hmi_level()).WillRepeatedly(Return(hmi));
  ON_CALL(*app0_, device()).WillByDefault(Return(12));
  EXPECT_CALL(*mock_service_, NotifyHMIAboutHMILevel(Eq(app0_), _)).Times(1);
  EXPECT_CALL(*mock_service_, PrimaryDevice()).Times(1).WillOnce(Return(3));
  EXPECT_CALL(*mock_service_, IsRemoteControlApplication(Eq(app0_)))
      .Times(1)
      .WillOnce(Return(true));

  ASSERT_TRUE(module_.IsAppForPlugin(app0_));
  ASSERT_FALSE(can_app_extention_->is_on_driver_device());
}

// TODO(ILytvynenko): Uncomment after CANModule::HandleMessage implementtion
// TEST_F(CanModuleTest, ChangeDriverDevice) {
//  Json::Value value(Json::ValueType::objectValue);
//  value[json_keys::kMethod] =
//      functional_modules::hmi_api::on_device_rank_changed;
//  value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
//  value[json_keys::kParams][message_params::kDevice] =
//      Json::Value(Json::ValueType::objectValue);
//  value[json_keys::kParams][message_params::kDevice][json_keys::kId] =
//      kDeviceId;
//  value[json_keys::kParams][message_params::kDevice][message_params::kName] =
//      kDeviceName;
//  value[json_keys::kParams][message_params::kRank] = "DRIVER";
//  Json::FastWriter writer;
//  std::string json_str = writer.write(value);

//  message_->set_function_name(
//      functional_modules::hmi_api::on_device_rank_changed);
//  message_->set_json_message(json_str);

//  apps_.push_back(app0_);

//  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_FULL;
//  EXPECT_CALL(*app0_, hmi_level()).WillRepeatedly(Return(hmi));
//  EXPECT_CALL(*app0_, device()).WillRepeatedly(Return(1));
//  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
//      .WillRepeatedly(Return(can_app_extention_));
//  EXPECT_CALL(*mock_service_, GetApplications(module_.GetModuleID()))
//      .WillOnce(Return(apps_));
//  EXPECT_CALL(*mock_service_, GetDeviceHandlerById(kDeviceId))
//      .WillRepeatedly(Return(kDeviceHandle));
//  EXPECT_CALL(*mock_service_, SetPrimaryDevice(kDeviceHandle)).Times(1);
//  EXPECT_CALL(*mock_service_, ResetPrimaryDevice()).Times(0);

//  CanModuleTest::HandleMessage();
//  ASSERT_TRUE(can_app_extention_->is_on_driver_device());
//}

// TEST_F(CanModuleTest, ChangeDriverDeviceOnOther) {
//  Json::Value value(Json::ValueType::objectValue);
//  value[json_keys::kMethod] =
//      functional_modules::hmi_api::on_device_rank_changed;
//  value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
//  value[json_keys::kParams][message_params::kDevice] =
//      Json::Value(Json::ValueType::objectValue);
//  value[json_keys::kParams][message_params::kDevice][json_keys::kId] =
//      kDeviceId;
//  value[json_keys::kParams][message_params::kDevice][message_params::kName] =
//      kDeviceName;
//  value[json_keys::kParams][message_params::kRank] = "DRIVER";
//  Json::FastWriter writer;
//  std::string json_str = writer.write(value);

//  message_->set_function_name(
//      functional_modules::hmi_api::on_device_rank_changed);
//  message_->set_message_type(MessageType::kNotification);
//  message_->set_json_message(json_str);

//  apps_.push_back(app0_);
//  apps_.push_back(app1_);

//  can_app_extention_->set_is_on_driver_device(true);
//  EXPECT_TRUE(can_app_extention_->is_on_driver_device());
//  utils::SharedPtr<CANAppExtension> ext =
//      utils::MakeShared<CANAppExtension>(module_.GetModuleID());
//  EXPECT_FALSE(ext->is_on_driver_device());

//  EXPECT_CALL(*app0_, device()).WillOnce(Return(1));
//  EXPECT_CALL(*app1_, device()).WillOnce(Return(2));
//  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
//      .WillOnce(Return(can_app_extention_));
//  EXPECT_CALL(*mock_service_, GetApplications(module_.GetModuleID()))
//      .WillRepeatedly(Return(apps_));
//  EXPECT_CALL(*mock_service_, GetDeviceHandlerById(kDeviceId))
//      .WillRepeatedly(Return(kDeviceHandle));
//  EXPECT_CALL(*mock_service_, SetPrimaryDevice(kDeviceHandle)).Times(1);
//  EXPECT_CALL(*mock_service_, ResetPrimaryDevice()).Times(0);

//  CanModuleTest::HandleMessage();
//  ASSERT_TRUE(can_app_extention_->is_on_driver_device());
//  ASSERT_FALSE(ext->is_on_driver_device());
//}

// TEST_F(CanModuleTest, ChangeDriverDeviceToPassenger) {
//  Json::Value value(Json::ValueType::objectValue);
//  value[json_keys::kMethod] =
//      functional_modules::hmi_api::on_device_rank_changed;
//  value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
//  value[json_keys::kParams][message_params::kDevice] =
//      Json::Value(Json::ValueType::objectValue);
//  value[json_keys::kParams][message_params::kDevice][json_keys::kId] =
//      kDeviceId;
//  value[json_keys::kParams][message_params::kDevice][message_params::kName] =
//      kDeviceName;
//  value[json_keys::kParams][message_params::kRank] = "PASSENGER";
//  Json::FastWriter writer;
//  std::string json_str = writer.write(value);

//  message_->set_function_name(
//      functional_modules::hmi_api::on_device_rank_changed);
//  message_->set_message_type(MessageType::kNotification);
//  message_->set_json_message(json_str);

//  apps_.push_back(app0_);
//  can_app_extention_->set_is_on_driver_device(true);
//  EXPECT_TRUE(can_app_extention_->is_on_driver_device());

//  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_FULL;
//  EXPECT_CALL(*app0_, hmi_level()).WillRepeatedly(Return(hmi));
//  EXPECT_CALL(*app0_, device()).WillOnce(Return(1));
//  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
//      .WillRepeatedly(Return(can_app_extention_));
//  EXPECT_CALL(*mock_service_, GetApplications(module_.GetModuleID()))
//      .WillRepeatedly(Return(apps_));
//  EXPECT_CALL(*mock_service_, GetDeviceHandlerById(kDeviceId))
//      .WillRepeatedly(Return(kDeviceHandle));
//  EXPECT_CALL(*mock_service_, PrimaryDevice()).Times(1).WillOnce(Return(1));
//  EXPECT_CALL(*mock_service_, ResetPrimaryDevice()).Times(1);

//  CanModuleTest::HandleMessage();
//  ASSERT_FALSE(can_app_extention_->is_on_driver_device());
//}

// TEST_F(CanModuleTest, ChangePassengerDeviceToPassenger) {
//  Json::Value value(Json::ValueType::objectValue);
//  value[json_keys::kMethod] =
//      functional_modules::hmi_api::on_device_rank_changed;
//  value[json_keys::kParams] = Json::Value(Json::ValueType::objectValue);
//  value[json_keys::kParams][message_params::kDevice] =
//      Json::Value(Json::ValueType::objectValue);
//  value[json_keys::kParams][message_params::kDevice][json_keys::kId] =
//      kDeviceId;
//  value[json_keys::kParams][message_params::kDevice][message_params::kName] =
//      kDeviceName;
//  value[json_keys::kParams][message_params::kRank] = "PASSENGER";
//  Json::FastWriter writer;
//  std::string json_str = writer.write(value);

//  message_->set_function_name(
//      functional_modules::hmi_api::on_device_rank_changed);
//  message_->set_message_type(MessageType::kNotification);
//  message_->set_json_message(json_str);

//  apps_.push_back(app0_);
//  can_app_extention_->set_is_on_driver_device(false);
//  EXPECT_FALSE(can_app_extention_->is_on_driver_device());

//  EXPECT_CALL(*app0_, device()).WillOnce(Return(1));
//  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
//      .WillRepeatedly(Return(can_app_extention_));
//  EXPECT_CALL(*mock_service_, GetApplications(module_.GetModuleID()))
//      .WillRepeatedly(Return(apps_));
//  EXPECT_CALL(*mock_service_, PrimaryDevice()).Times(1).WillOnce(Return(2));
//  EXPECT_CALL(*mock_service_, ResetPrimaryDevice()).Times(0);

//  CanModuleTest::HandleMessage();
//  ASSERT_FALSE(can_app_extention_->is_on_driver_device());
//}

TEST_F(CanModuleTest, CanAppChangeHMILevelPrimary) {
  apps_.push_back(app0_);

  can_app_extention_->set_is_on_driver_device(true);

  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
      .WillRepeatedly(Return(can_app_extention_));

  ASSERT_TRUE(module_.CanAppChangeHMILevel(
      app0_, mobile_apis::HMILevel::eType::HMI_FULL));
  ASSERT_TRUE(module_.CanAppChangeHMILevel(
      app0_, mobile_apis::HMILevel::eType::HMI_LIMITED));
  ASSERT_TRUE(module_.CanAppChangeHMILevel(
      app0_, mobile_apis::HMILevel::eType::HMI_BACKGROUND));
  ASSERT_TRUE(module_.CanAppChangeHMILevel(
      app0_, mobile_apis::HMILevel::eType::HMI_NONE));
}

TEST_F(CanModuleTest, CanAppChangeHMILevelPassenger) {
  apps_.push_back(app0_);

  can_app_extention_->set_is_on_driver_device(false);

  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
      .WillRepeatedly(Return(can_app_extention_));

  ASSERT_FALSE(module_.CanAppChangeHMILevel(
      app0_, mobile_apis::HMILevel::eType::HMI_FULL));
  ASSERT_FALSE(module_.CanAppChangeHMILevel(
      app0_, mobile_apis::HMILevel::eType::HMI_LIMITED));
  ASSERT_TRUE(module_.CanAppChangeHMILevel(
      app0_, mobile_apis::HMILevel::eType::HMI_BACKGROUND));
  ASSERT_TRUE(module_.CanAppChangeHMILevel(
      app0_, mobile_apis::HMILevel::eType::HMI_NONE));
}
}  // namespace can_cooperation
