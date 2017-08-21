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
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/commands/mobile/unsubscribe_vehicle_data_request.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace unsubscribe_vehicle_data_request {

namespace am = ::application_manager;
namespace mobile_result = mobile_apis::Result;

using ::testing::_;

using am::commands::UnsubscribeVehicleDataRequest;
using am::commands::MessageSharedPtr;

typedef ::utils::SharedPtr<UnsubscribeVehicleDataRequest> CommandPtr;

namespace {
const uint32_t kConnectionKey = 1u;
const std::string kMsgParamKey = "test_key";
const am::VehicleDataType kVehicleType = am::VehicleDataType::SPEED;
}  // namespace

class UnsubscribeVehicleRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  void UnsubscribeSuccessfully();
  sync_primitives::Lock app_set_lock_;
};

TEST_F(UnsubscribeVehicleRequestTest, Run_AppNotRegistered_UNSUCCESS) {
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>());
  EXPECT_CALL(app_mngr_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      app_mngr_,
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

  am::VehicleData data;
  EXPECT_CALL(*(am::MockMessageHelper::message_helper_mock()), vehicle_data())
      .WillOnce(ReturnRef(data));
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest,
       Run_UnsubscribeNotSubscribedBeforeData_IGNORED) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][kMsgParamKey] = kVehicleType;

  am::VehicleData vehicle_data;
  vehicle_data.insert(am::VehicleData::value_type(kMsgParamKey, kVehicleType));
  EXPECT_CALL(*(am::MockMessageHelper::message_helper_mock()), vehicle_data())
      .WillOnce(ReturnRef(vehicle_data));
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::IGNORED), _));

  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest,
       Run_UnsubscribeNotSubscribedBeforeData_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][kMsgParamKey] = true;

  am::VehicleData vehicle_data;
  vehicle_data.insert(am::VehicleData::value_type(kMsgParamKey, kVehicleType));
  EXPECT_CALL(*(am::MockMessageHelper::message_helper_mock()), vehicle_data())
      .WillOnce(ReturnRef(vehicle_data));
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::IGNORED), _));

  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest, Run_UnsubscribeDataDisabled_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  am::VehicleData vehicle_data;
  vehicle_data.insert(am::VehicleData::value_type(kMsgParamKey, kVehicleType));
  EXPECT_CALL(*(am::MockMessageHelper::message_helper_mock()), vehicle_data())
      .WillOnce(ReturnRef(vehicle_data));
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command->Run();
}

void UnsubscribeVehicleRequestTest::UnsubscribeSuccessfully() {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][kMsgParamKey] = true;
  am::VehicleData vehicle_data;
  vehicle_data.insert(am::VehicleData::value_type(kMsgParamKey, kVehicleType));
  EXPECT_CALL(*(am::MockMessageHelper::message_helper_mock()), vehicle_data())
      .WillOnce(ReturnRef(vehicle_data));
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  am::ApplicationSet application_set_;
  MockAppPtr mock_app(CreateMockApp());
  application_set_.insert(mock_app);
  DataAccessor<am::ApplicationSet> accessor(application_set_, app_set_lock_);
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(accessor));

  EXPECT_CALL(*mock_app, IsSubscribedToIVI(kVehicleType))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_app, UnsubscribeFromIVI(kVehicleType))
      .WillRepeatedly(Return(true));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));

  command->Run();
}

TEST_F(UnsubscribeVehicleRequestTest, Run_UnsubscribeData_SUCCESS) {
  UnsubscribeSuccessfully();
}

TEST_F(UnsubscribeVehicleRequestTest, OnEvent_DataNotSubscribed_IGNORED) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][kMsgParamKey] = true;
  CommandPtr command(CreateCommand<UnsubscribeVehicleDataRequest>(command_msg));

  am::VehicleData vehicle_data;
  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillRepeatedly(Return(mock_app));
  vehicle_data.insert(am::VehicleData::value_type(kMsgParamKey, kVehicleType));
  EXPECT_CALL(*(am::MockMessageHelper::message_helper_mock()), vehicle_data())
      .WillOnce(ReturnRef(vehicle_data));
  EXPECT_CALL(*mock_app, IsSubscribedToIVI(kVehicleType))
      .WillRepeatedly(Return(false));
  EXPECT_CALL(
      app_mngr_,
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
  EXPECT_CALL(*(am::MockMessageHelper::message_helper_mock()),
              HMIToMobileResult(hmi_result)).WillOnce(Return(mob_result));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::IGNORED), _));
  EXPECT_CALL(*mock_app, UpdateHash());

  command->on_event(test_event);
}

TEST_F(UnsubscribeVehicleRequestTest, OnEvent_DataUnsubscribed_SUCCESS) {
  UnsubscribeSuccessfully();
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][kMsgParamKey] = true;
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

  EXPECT_CALL(*(am::MockMessageHelper::message_helper_mock()),
              HMIToMobileResult(hmi_result)).WillOnce(Return(mob_result));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));

  EXPECT_CALL(*mock_app, UpdateHash());

  command->on_event(test_event);
}

}  // namespace unsubscribe_vehicle_data_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
