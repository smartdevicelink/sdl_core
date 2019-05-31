/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <string>

#include "gtest/gtest.h"

#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "mobile/unsubscribe_vehicle_data_request.h"
#include "resumption/mock_last_state.h"
#include "vehicle_info_plugin/commands/vi_command_request_test.h"
#include "vehicle_info_plugin/vehicle_info_app_extension.h"
#include "vehicle_info_plugin/vehicle_info_plugin.h"

#include "application_manager/mock_rpc_handler.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace unsubscribe_vehicle_data_request {

namespace am = ::application_manager;
namespace mobile_result = mobile_apis::Result;

using ::testing::_;

using am::commands::MessageSharedPtr;
using vehicle_info_plugin::commands::UnsubscribeVehicleDataRequest;

typedef std::shared_ptr<UnsubscribeVehicleDataRequest> CommandPtr;

namespace {
const uint32_t kConnectionKey = 1u;
const std::string kMsgParamKey = "test_key";
const mobile_apis::VehicleDataType::eType kVehicleType =
    mobile_apis::VehicleDataType::VEHICLEDATA_SPEED;
const std::string kVehicleTypeStr = am::strings::speed;
}  // namespace

class UnsubscribeVehicleRequestTest
    : public VICommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  UnsubscribeVehicleRequestTest()
      : mock_app_(CreateMockApp())
      , vi_app_extension_ptr_(
            std::make_shared<vehicle_info_plugin::VehicleInfoAppExtension>(
                vi_plugin_, *mock_app_))
      , app_set_lock_ptr_(std::make_shared<sync_primitives::Lock>()) {}

 protected:
  void UnsubscribeSuccessfully();
  void SetUp() OVERRIDE {
    ON_CALL(app_mngr_, GetRPCHandler())
        .WillByDefault(ReturnRef(mock_rpc_handler_));

    vi_plugin_.Init(app_mngr_,
                    mock_rpc_service_,
                    mock_hmi_capabilities_,
                    mock_policy_handler_,
                    mock_last_state_);
    ON_CALL(*mock_app_, AddExtension(vi_app_extension_ptr_));
    vi_plugin_.OnApplicationEvent(application_manager::plugin_manager::
                                      ApplicationEvent::kApplicationRegistered,
                                  mock_app_);
    ON_CALL(*mock_app_,
            QueryInterface(vehicle_info_plugin::VehicleInfoAppExtension::
                               VehicleInfoAppExtensionUID))
        .WillByDefault(Return(vi_app_extension_ptr_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
  }

  MockAppPtr mock_app_;
  application_manager::AppExtensionPtr vi_app_extension_ptr_;
  std::shared_ptr<sync_primitives::Lock> app_set_lock_ptr_;
  vehicle_info_plugin::VehicleInfoPlugin vi_plugin_;
  application_manager_test::MockRPCHandler mock_rpc_handler_;
  resumption_test::MockLastState mock_last_state_;
};

TEST_F(UnsubscribeVehicleRequestTest, Run_AppNotRegistered_UNSUCCESS) {
  CommandPtr command(CreateCommandVI<UnsubscribeVehicleDataRequest>());
  EXPECT_CALL(app_mngr_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest,
       Run_DataToUnsubscribeIsNotExisted_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][am::strings::button_name] =
      kVehicleTypeStr;

  am::VehicleData data;
  EXPECT_CALL(mock_message_helper_, vehicle_data())
      .WillRepeatedly(ReturnRef(data));

  smart_objects::SmartObject empty_hmi_custom_params;
  ON_CALL(mock_custom_vehicle_data_manager_, CreateHMIMessageParams(_))
      .WillByDefault(Return(empty_hmi_custom_params));

  CommandPtr command(
      CreateCommandVI<UnsubscribeVehicleDataRequest>(command_msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest,
       Run_UnsubscribeFromSubscribedBeforeData_RequestSuccessfullySentToHmi) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][kVehicleTypeStr] = true;

  am::VehicleData vehicle_data;
  vehicle_data.insert(am::VehicleData::value_type(kMsgParamKey, kVehicleType));
  ON_CALL(mock_message_helper_, vehicle_data())
      .WillByDefault(ReturnRef(vehicle_data));

  smart_objects::SmartObject hmi_custom_params(smart_objects::SmartType_Map);
  hmi_custom_params[kVehicleTypeStr] = true;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  HMIResultCodeIs(
                      hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData),
                  _))
      .WillOnce(Return(true));
  vehicle_info_plugin::VehicleInfoAppExtension* vi_app_extension =
      dynamic_cast<vehicle_info_plugin::VehicleInfoAppExtension*>(
          vi_app_extension_ptr_.get());
  ASSERT_TRUE(vi_app_extension->subscribeToVehicleInfo(kVehicleTypeStr));
  application_manager::ApplicationSet app_set = {mock_app_};
  DataAccessor<application_manager::ApplicationSet> accessor(app_set,
                                                             app_set_lock_ptr_);
  // Expectations
  EXPECT_CALL(app_mngr_, applications()).WillRepeatedly(Return(accessor));
  CommandPtr command(
      CreateCommandVI<UnsubscribeVehicleDataRequest>(command_msg));
  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest,
       Run_EmptyVehicleData_INVALID_DATA_SentToMobile) {
  am::VehicleData vehicle_data;
  EXPECT_CALL(mock_message_helper_, vehicle_data())
      .WillRepeatedly(ReturnRef(vehicle_data));

  smart_objects::SmartObject empty_hmi_custom_params;
  ON_CALL(mock_custom_vehicle_data_manager_, CreateHMIMessageParams(_))
      .WillByDefault(Return(empty_hmi_custom_params));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][kMsgParamKey] = false;
  CommandPtr command(
      CreateCommandVI<UnsubscribeVehicleDataRequest>(command_msg));
  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest,
       Run_UnsubscribeFromNotSubscribedBeforeData_UNSUCCESS) {
  smart_objects::SmartObject empty_hmi_custom_params;
  ON_CALL(mock_custom_vehicle_data_manager_, CreateHMIMessageParams(_))
      .WillByDefault(Return(empty_hmi_custom_params));

  am::VehicleData vehicle_data;
  vehicle_data.insert(am::VehicleData::value_type(kMsgParamKey, kVehicleType));
  EXPECT_CALL(mock_message_helper_, vehicle_data())
      .WillRepeatedly(ReturnRef(vehicle_data));
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::IGNORED), _));

  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][kMsgParamKey] = true;
  CommandPtr command(
      CreateCommandVI<UnsubscribeVehicleDataRequest>(command_msg));
  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest, Run_UnsubscribeDataDisabled_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  smart_objects::SmartObject empty_hmi_custom_params;
  ON_CALL(mock_custom_vehicle_data_manager_, CreateHMIMessageParams(_))
      .WillByDefault(Return(empty_hmi_custom_params));

  CommandPtr command(
      CreateCommandVI<UnsubscribeVehicleDataRequest>(command_msg));

  am::VehicleData vehicle_data;
  vehicle_data.insert(am::VehicleData::value_type(kMsgParamKey, kVehicleType));
  EXPECT_CALL(mock_message_helper_, vehicle_data())
      .WillRepeatedly(ReturnRef(vehicle_data));
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest, OnEvent_DataNotSubscribed_IGNORED) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][kMsgParamKey] = true;
  CommandPtr command(
      CreateCommandVI<UnsubscribeVehicleDataRequest>(command_msg));

  am::VehicleData vehicle_data;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillRepeatedly(Return(mock_app_));
  vehicle_data.insert(am::VehicleData::value_type(kMsgParamKey, kVehicleType));
  EXPECT_CALL(mock_message_helper_, vehicle_data())
      .WillRepeatedly(ReturnRef(vehicle_data));

  smart_objects::SmartObject empty_hmi_custom_params;
  ON_CALL(mock_custom_vehicle_data_manager_, CreateHMIMessageParams(_))
      .WillByDefault(Return(empty_hmi_custom_params));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::IGNORED), _));
  command->Init();
  command->Run();

  am::event_engine::Event test_event(
      hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData);
  SmartObject message(smart_objects::SmartType_Map);
  const hmi_apis::Common_Result::eType hmi_result =
      hmi_apis::Common_Result::SUCCESS;
  message[am::strings::params][am::hmi_response::code] = hmi_result;
  message[am::strings::msg_params][kMsgParamKey] = true;
  test_event.set_smart_object(message);

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::IGNORED), _));
  EXPECT_CALL(*mock_app_, UpdateHash());

  command->on_event(test_event);
}

TEST_F(UnsubscribeVehicleRequestTest, OnEvent_DataUnsubscribed_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][kMsgParamKey] = true;
  CommandPtr command(
      CreateCommandVI<UnsubscribeVehicleDataRequest>(command_msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillRepeatedly(Return(mock_app_));
  am::event_engine::Event test_event(
      hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData);
  SmartObject message(smart_objects::SmartType_Map);
  const hmi_apis::Common_Result::eType hmi_result =
      hmi_apis::Common_Result::SUCCESS;
  message[am::strings::params][am::hmi_response::code] = hmi_result;
  message[am::strings::msg_params][kMsgParamKey] = true;
  test_event.set_smart_object(message);

  am::VehicleData vehicle_data;
  vehicle_data.insert(am::VehicleData::value_type(kMsgParamKey, kVehicleType));
  EXPECT_CALL(mock_message_helper_, vehicle_data())
      .WillRepeatedly(ReturnRef(vehicle_data));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));

  command->on_event(test_event);
}

}  // namespace unsubscribe_vehicle_data_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
