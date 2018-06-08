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
#include "remote_control/remote_control_plugin.h"
#include "remote_control/rc_app_extension.h"
#include "remote_control/rc_module_constants.h"
#include "remote_control/message_helper.h"
#include "functional_module/module_observer.h"
#include "application_manager/mock_application.h"
#include "include/mock_service.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_application_manager_settings.h"

using functional_modules::PluginInfo;
using functional_modules::ProcessResult;
using functional_modules::RCFunctionID;
using application_manager::ServicePtr;

using application_manager::MockService;
using application_manager::MockMessageHelper;
using test::components::application_manager_test::MockApplication;
using test::components::application_manager_test::
    MockApplicationManagerSettings;

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
const uint32_t kDefaultTimeout = 10000;
}

namespace remote_control {

class RCModuleTest : public ::testing::Test {
 public:
  RCModuleTest()
      : mock_service_(std::make_shared<NiceMock<MockService> >())
      , mock_settings_(
            std::make_shared<NiceMock<MockApplicationManagerSettings> >())
      , mock_message_helper_(*MockMessageHelper::message_helper_mock())
      , app0_(std::make_shared<NiceMock<MockApplication> >())
      , app1_(std::make_shared<NiceMock<MockApplication> >())
      , message_(std::make_shared<Message>(MessagePriority::FromServiceType(
            protocol_handler::ServiceType::kRpc)))
      , rc_app_extention_(
            std::make_shared<RCAppExtension>(module_.GetModuleID())) {}

  void HandleMessage() {
    module_.ProcessHMIMessage(message_);
  }

 protected:
  RemoteControlPlugin module_;
  std::shared_ptr<NiceMock<MockService> > mock_service_;
  std::shared_ptr<NiceMock<MockApplicationManagerSettings> > mock_settings_;
  MockMessageHelper& mock_message_helper_;
  std::vector<ApplicationSharedPtr> apps_;
  std::shared_ptr<NiceMock<MockApplication> > app0_;
  std::shared_ptr<NiceMock<MockApplication> > app1_;
  application_manager::MessagePtr message_;
  std::shared_ptr<RCAppExtension> rc_app_extention_;

  void SetUp() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    ON_CALL(*mock_service_, GetSettings())
        .WillByDefault(ReturnRef(*mock_settings_));
    ON_CALL(*mock_settings_, default_timeout())
        .WillByDefault(ReturnRef(kDefaultTimeout));
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

TEST_F(RCModuleTest, Create) {
  EXPECT_EQ(153, module_.GetModuleID());
  PluginInfo plugin = module_.GetPluginInfo();
  EXPECT_EQ(plugin.name, "RemoteControlPlugin");
  EXPECT_EQ(plugin.version, 1);
}

TEST_F(RCModuleTest, ProcessMessageWrongMessage) {
  message_->set_function_id(-1);
  EXPECT_CALL(*mock_service_, SendMessageToMobile(_)).Times(0);
  EXPECT_EQ(ProcessResult::CANNOT_PROCESS, module_.ProcessMessage(message_));
}

TEST_F(RCModuleTest, ProcessMessageEmptyapps_List) {
  message_->set_function_id(RCFunctionID::ON_INTERIOR_VEHICLE_DATA);
  message_->set_function_name("OnInteriorVehicleData");

  std::string json =
      "{\"jsonrpc\": \"2.0\", \"method\": \"RC.OnInteriorVehicleData\",\
        \"params\": {\"moduleData\": {\"moduleType\": \"CLIMATE\",\
        \"climateControlData\": {\"fanSpeed\": 100} }}}";
  message_->set_json_message(json);

  EXPECT_CALL(*mock_service_, ValidateMessageBySchema(_))
      .WillOnce(Return(
          application_manager::MessageValidationResult::SCHEMA_MISMATCH));
  EXPECT_CALL(*mock_service_, SendMessageToMobile(_)).Times(0);

  // EXPECT_EQ(ProcessResult::PROCESSED, module_.ProcessMessage(message_));
  EXPECT_EQ(ProcessResult::CANNOT_PROCESS, module_.ProcessHMIMessage(message_));
}

TEST_F(RCModuleTest, ProcessMessagePass) {
  message_->set_function_id(RCFunctionID::ON_INTERIOR_VEHICLE_DATA);

  std::string json =
      "{ \"jsonrpc\": \"2.0\",\"method\": \"RC.OnInteriorVehicleData\",\
         \"params\":{\"moduleData\":{\
         \"moduleType\": \"CLIMATE\",\"climateControlData\": {\"fanSpeed\": 100}}}}";

  message_->set_json_message(json);

  application_manager::BinaryData buf;
  application_manager::BinaryData* data = &buf;
  data->push_back(1);

  message_->set_binary_data(
      static_cast<const application_manager::BinaryData*>(data));

  Json::Value json_value = MessageHelper::StringToValue(json);
  Json::Value module_type =
      json_value[json_keys::kParams][message_params::kModuleData]
                [message_params::kModuleType];
  apps_.push_back(app0_);
  rc_app_extention_->SubscribeToInteriorVehicleData(module_type);
  EXPECT_CALL(*mock_service_, ValidateMessageBySchema(_))
      .WillOnce(Return(application_manager::MessageValidationResult::SUCCESS));
  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
      .WillOnce(Return(rc_app_extention_));
  EXPECT_CALL(*app0_, app_id()).WillRepeatedly(Return(1));
  EXPECT_CALL(*app0_, protocol_version())
      .WillRepeatedly(
          Return(protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_4));
  EXPECT_CALL(*mock_service_, GetApplications(module_.GetModuleID()))
      .WillOnce(Return(apps_));
  EXPECT_CALL(*mock_service_, GetApplication(1)).WillOnce(Return(app0_));
  EXPECT_CALL(*mock_service_, CheckPolicyPermissions(_))
      .WillOnce(Return(mobile_apis::Result::eType::SUCCESS));
  EXPECT_CALL(*mock_service_, CheckModule(1, "CLIMATE")).WillOnce(Return(true));
  EXPECT_CALL(*mock_service_, SendMessageToMobile(_));

  EXPECT_EQ(ProcessResult::PROCESSED, module_.ProcessHMIMessage(message_));
}

TEST_F(RCModuleTest, RemoveAppExtensionPassWay) {
  EXPECT_CALL(*mock_service_, GetApplication(1)).WillOnce(Return(app0_));
  EXPECT_CALL(*app0_, RemoveExtension(module_.GetModuleID()));

  module_.RemoveAppExtension(1);
}

TEST_F(RCModuleTest, RemoveAppExtensionIfAppNoExist) {
  ApplicationSharedPtr invalid_app;

  EXPECT_CALL(*mock_service_, GetApplication(_)).WillOnce(Return(invalid_app));

  module_.RemoveAppExtension(1);
}

TEST_F(RCModuleTest, SendResponseToMobile) {
  EXPECT_CALL(*mock_service_, SendMessageToMobile(message_));

  module_.SendResponseToMobile(message_);
}

TEST_F(RCModuleTest, IsAppForPluginSuccess) {
  application_manager::AppExtensionPtr invalid_ext;
  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
      .WillOnce(Return(invalid_ext))
      .WillRepeatedly(Return(rc_app_extention_));
  EXPECT_CALL(*app0_, AddExtension(_)).WillOnce(Return(true));
  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_FULL;
  EXPECT_CALL(*app0_, hmi_level()).WillRepeatedly(Return(hmi));
  ON_CALL(*app0_, device()).WillByDefault(Return(1));
  EXPECT_CALL(*mock_service_, IsRemoteControlApplication(Eq(app0_)))
      .WillOnce(Return(true));
  ASSERT_TRUE(module_.IsAppForPlugin(app0_));
}

TEST_F(RCModuleTest, IsAppForPluginNotNew) {
  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
      .WillOnce(Return(rc_app_extention_));
  EXPECT_CALL(*mock_service_, NotifyHMIAboutHMILevel(Eq(app0_), _)).Times(0);
  ASSERT_TRUE(module_.IsAppForPlugin(app0_));
}

TEST_F(RCModuleTest, IsAppForPluginFail) {
  application_manager::AppExtensionPtr invalid_ext;
  EXPECT_CALL(*app0_, QueryInterface(module_.GetModuleID()))
      .WillOnce(Return(invalid_ext));
  EXPECT_CALL(*mock_service_, IsRemoteControlApplication(Eq(app0_)))
      .Times(1)
      .WillOnce(Return(false));
  ASSERT_FALSE(module_.IsAppForPlugin(app0_));
}

TEST_F(RCModuleTest, OnAppHMILevelChanged) {
  const application_manager::custom_str::CustomString name("name");
  ON_CALL(*app0_, name()).WillByDefault(ReturnRef(name));
  mobile_apis::HMILevel::eType hmi = mobile_apis::HMILevel::eType::HMI_NONE;
  EXPECT_CALL(*app0_, hmi_level()).WillRepeatedly(Return(hmi));
  EXPECT_CALL(*mock_service_,
              NotifyHMIAboutHMILevel(Eq(app0_),
                                     mobile_apis::HMILevel::eType::HMI_NONE));
  module_.OnAppHMILevelChanged(app0_, mobile_apis::HMILevel::eType::HMI_FULL);
}

}  // namespace remote_control
