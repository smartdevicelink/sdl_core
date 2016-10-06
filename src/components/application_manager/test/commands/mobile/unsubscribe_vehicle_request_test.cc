/*
 Copyright (c) 2016, Ford Motor Company
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
#include "utils/shared_ptr.h"
#include "commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "mobile/unsubscribe_vehicle_data_request.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace unsubscribe_vehicle_data_request {

namespace am = ::application_manager;
namespace mobile_result = mobile_apis::Result;

using ::testing::Mock;
using ::testing::_;
using ::testing::Mock;

using am::commands::UnsubscribeVehicleDataRequest;
using am::commands::MessageSharedPtr;
using am::MockMessageHelper;
typedef ::utils::SharedPtr<UnsubscribeVehicleDataRequest> CommandPtr;

namespace {
const uint32_t kConnectionKey = 1u;
const uint32_t kCorrelationKey = 2u;
const std::string kMsgParamKey = "test_key";
const am::VehicleDataType kVehicleType = am::VehicleDataType::SPEED;
}  // namespace

MATCHER_P(CheckMsgCorrelationKey, correlation_key, "") {
  return correlation_key ==
         static_cast<uint32_t>(
             (*arg)[am::strings::params][am::strings::correlation_id].asUInt());
}

class UnsubscribeVehicleRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  ~UnsubscribeVehicleRequestTest() {
    // Fix DataAccessor release and WinQt crash
    Mock::VerifyAndClearExpectations(&mock_app_manager_);
  }

 protected:
  UnsubscribeVehicleRequestTest()
      : message_helper_mock_(am::MockMessageHelper::message_helper_mock()) {}

  void UnsubscribeSuccessfully();
  void SetMsgParamsAndVehicleData(MessageSharedPtr msg) {
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    (*msg)[am::strings::msg_params][kMsgParamKey] = kVehicleType;

    vehicle_data_.insert(
        am::VehicleData::value_type(kMsgParamKey, kVehicleType));
  }
  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&message_helper_mock_);
  }

  sync_primitives::Lock app_set_lock_;
  am::VehicleData vehicle_data_;
  am::MockMessageHelper* message_helper_mock_;
};

TEST_F(UnsubscribeVehicleRequestTest, Run_AppNotRegistered_UNSUCCESS) {
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>());
  EXPECT_CALL(mock_app_manager_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      mock_app_manager_,
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
      kVehicleType;
  am::VehicleData empty_vehicle_data;
  EXPECT_CALL(*message_helper_mock_, vehicle_data())
      .WillOnce(ReturnRef(empty_vehicle_data));
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest,
       Run_UnsubscribeNotSubscribedBeforeData_IGNORED) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));

  SetMsgParamsAndVehicleData(command_msg);
  EXPECT_CALL(*message_helper_mock_, vehicle_data())
      .WillOnce(ReturnRef(vehicle_data_));
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::IGNORED), _));

  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest,
       Run_UnsubscribeNotSubscribedBeforeData_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));

  SetMsgParamsAndVehicleData(command_msg);

  EXPECT_CALL(*message_helper_mock_, vehicle_data())
      .WillOnce(ReturnRef(vehicle_data_));
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::IGNORED), _));

  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest, Run_UnsubscribeDataDisabled_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  am::VehicleData vehicle_data;
  vehicle_data.insert(am::VehicleData::value_type(kMsgParamKey, kVehicleType));
  EXPECT_CALL(*message_helper_mock_, vehicle_data())
      .WillOnce(ReturnRef(vehicle_data));
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command->Run();
}

void UnsubscribeVehicleRequestTest::UnsubscribeSuccessfully() {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));

  SetMsgParamsAndVehicleData(command_msg);
  EXPECT_CALL(*message_helper_mock_, vehicle_data())
      .WillOnce(ReturnRef(vehicle_data_));
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  am::ApplicationSet application_set_;
  MockAppPtr mock_app(CreateMockApp());
  application_set_.insert(mock_app);
  DataAccessor<am::ApplicationSet> accessor(application_set_, app_set_lock_);
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(mock_app_manager_, applications()).WillOnce(Return(accessor));

  EXPECT_CALL(*mock_app, IsSubscribedToIVI(kVehicleType))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_app, UnsubscribeFromIVI(kVehicleType))
      .WillRepeatedly(Return(true));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));

  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest, Run_UnsubscribedKeyType_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));

  SetMsgParamsAndVehicleData(command_msg);
  EXPECT_CALL(*message_helper_mock_, vehicle_data())
      .WillOnce(ReturnRef(vehicle_data_));
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(*mock_app, IsSubscribedToIVI(kVehicleType))
      .WillOnce(Return(true));
  EXPECT_CALL(*mock_app, UnsubscribeFromIVI(kVehicleType))
      .WillOnce(Return(false));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::IGNORED), _));

  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest, Run_SubscribedKeyType_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));

  SetMsgParamsAndVehicleData(command_msg);
  EXPECT_CALL(*message_helper_mock_, vehicle_data())
      .WillOnce(ReturnRef(vehicle_data_));
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  am::ApplicationSet application_set_;
  MockAppPtr mock_app(CreateMockApp());
  application_set_.insert(mock_app);
  DataAccessor<am::ApplicationSet> accessor(application_set_, app_set_lock_);
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(mock_app_manager_, applications()).WillOnce(Return(accessor));

  EXPECT_CALL(*mock_app, IsSubscribedToIVI(kVehicleType))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_CALL(*mock_app, UnsubscribeFromIVI(kVehicleType))
      .WillOnce(Return(true));

  EXPECT_CALL(mock_app_manager_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationKey));

  EXPECT_CALL(mock_app_manager_,
              ManageHMICommand(CheckMsgCorrelationKey(kCorrelationKey)))
      .WillOnce(Return(false));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::OUT_OF_MEMORY), _));

  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest, Run_UnsubscribeData_SUCCESS) {
  UnsubscribeSuccessfully();
}

TEST_F(UnsubscribeVehicleRequestTest, OnEvent_UnknownEvent_UNSUCCESS) {
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>());
  am::event_engine::Event test_event(hmi_apis::FunctionID::INVALID_ENUM);
  SmartObject message(smart_objects::SmartType_Map);
  const hmi_apis::Common_Result::eType hmi_result =
      hmi_apis::Common_Result::SUCCESS;
  message[am::strings::params][am::hmi_response::code] = hmi_result;
  message[am::strings::msg_params][kMsgParamKey] = true;
  test_event.set_smart_object(message);
  EXPECT_CALL(*message_helper_mock_, HMIToMobileResult(hmi_result)).Times(0);

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);

  command->on_event(test_event);
}

TEST_F(UnsubscribeVehicleRequestTest,
       OnEvent_AddAlreadyUnsubscribedVI_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));

  SetMsgParamsAndVehicleData(command_msg);
  EXPECT_CALL(*message_helper_mock_, vehicle_data())
      .WillOnce(ReturnRef(vehicle_data_));
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  am::ApplicationSet application_set_;
  MockAppPtr mock_app(CreateMockApp());
  application_set_.insert(mock_app);
  DataAccessor<am::ApplicationSet> accessor(application_set_, app_set_lock_);
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(mock_app_manager_, applications()).WillOnce(Return(accessor));

  EXPECT_CALL(*mock_app, IsSubscribedToIVI(kVehicleType))
      .WillOnce(Return(true))
      .WillOnce(Return(true));
  EXPECT_CALL(*mock_app, UnsubscribeFromIVI(kVehicleType))
      .WillOnce(Return(true));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));

  command->Run();

  am::event_engine::Event test_event(
      hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData);
  SmartObject message(smart_objects::SmartType_Map);
  const hmi_apis::Common_Result::eType hmi_result =
      hmi_apis::Common_Result::SUCCESS;
  const mobile_apis::Result::eType mob_result = mobile_apis::Result::SUCCESS;
  message[am::strings::params][am::hmi_response::code] = hmi_result;
  message[am::strings::msg_params][kMsgParamKey] = true;
  test_event.set_smart_object(message);

  EXPECT_CALL(*message_helper_mock_, HMIToMobileResult(hmi_result))
      .WillOnce(Return(mob_result));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));
  MockAppPtr invalid_app;
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(invalid_app));
  EXPECT_CALL(*mock_app, UpdateHash()).Times(0);

  command->on_event(test_event);
}

TEST_F(UnsubscribeVehicleRequestTest, OnEvent_DataNotSubscribed_IGNORED) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));

  SetMsgParamsAndVehicleData(command_msg);

  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillRepeatedly(Return(mock_app));
  EXPECT_CALL(*message_helper_mock_, vehicle_data())
      .WillOnce(ReturnRef(vehicle_data_));
  EXPECT_CALL(*mock_app, IsSubscribedToIVI(kVehicleType))
      .WillRepeatedly(Return(false));
  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::IGNORED), _));
  command->Run();

  am::event_engine::Event test_event(
      hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData);
  SmartObject message(smart_objects::SmartType_Map);
  const hmi_apis::Common_Result::eType hmi_result =
      hmi_apis::Common_Result::SUCCESS;
  const mobile_apis::Result::eType mob_result = mobile_apis::Result::SUCCESS;
  message[am::strings::params][am::hmi_response::code] = hmi_result;
  message[am::strings::msg_params][kMsgParamKey] = true;
  test_event.set_smart_object(message);
  EXPECT_CALL(*message_helper_mock_, HMIToMobileResult(hmi_result))
      .WillOnce(Return(mob_result));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::IGNORED), _));
  EXPECT_CALL(*mock_app, UpdateHash());

  command->on_event(test_event);
}

TEST_F(UnsubscribeVehicleRequestTest, OnEvent_DataUnsubscribed_SUCCESS) {
  UnsubscribeSuccessfully();
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));

  SetMsgParamsAndVehicleData(command_msg);
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));
  MockAppPtr mock_app(CreateMockApp());

  am::event_engine::Event test_event(
      hmi_apis::FunctionID::VehicleInfo_UnsubscribeVehicleData);
  SmartObject message(smart_objects::SmartType_Map);
  const hmi_apis::Common_Result::eType hmi_result =
      hmi_apis::Common_Result::SUCCESS;
  const mobile_apis::Result::eType mob_result = mobile_apis::Result::SUCCESS;
  message[am::strings::params][am::hmi_response::code] = hmi_result;
  message[am::strings::msg_params][kMsgParamKey] = true;
  test_event.set_smart_object(message);

  EXPECT_CALL(*message_helper_mock_, HMIToMobileResult(hmi_result))
      .WillOnce(Return(mob_result));

  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));

  EXPECT_CALL(*mock_app, UpdateHash());

  command->on_event(test_event);
}

}  // namespace unsubscribe_vehicle_data_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
