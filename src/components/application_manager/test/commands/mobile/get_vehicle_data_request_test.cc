/*
 * Copyright (c) 2016, Ford Motor Company
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

#include <stdint.h>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/test/include/application_manager/commands/commands_test.h"
#include "application_manager/test/include/application_manager/commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/message_helper.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/commands/mobile/get_vehicle_data_request.h"
#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace get_vehicle_data_request {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
namespace am = ::application_manager;
using am::commands::MessageSharedPtr;
using am::commands::GetVehicleDataRequest;
using am::event_engine::Event;
namespace mobile_result = mobile_apis::Result;

typedef SharedPtr<GetVehicleDataRequest> GetVehicleDataRequestPtr;

namespace {
const uint32_t kConnectionKey = 2u;
}  // namespace

class GetVehicleDataRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  GetVehicleDataRequestTest()
      : mock_message_helper_(*am::MockMessageHelper::message_helper_mock()) {
    testing::Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  ~GetVehicleDataRequestTest() {
    testing::Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  am::MockMessageHelper& mock_message_helper_;
};

class UnwrappedGetVehicleDataRequest : public GetVehicleDataRequest {
 public:
  UnwrappedGetVehicleDataRequest(const MessageSharedPtr& message,
                                 am::ApplicationManager& application_manager)
      : GetVehicleDataRequest(message, application_manager) {}

  std::vector<std::string>& get_disallowed_params() {
    return removed_parameters_permissions_.disallowed_params;
  }

  using GetVehicleDataRequest::on_event;
};

#ifdef HMI_DBUS_API
// HMI_DBUS_API currently not supported
#else

TEST_F(GetVehicleDataRequestTest, Run_ApplicationIsNotRegistered_UNSUCCESS) {
  GetVehicleDataRequestPtr command(CreateCommand<GetVehicleDataRequest>());

  EXPECT_CALL(app_mngr_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command->Run();
}

TEST_F(GetVehicleDataRequestTest, Run_TooHighFrequency_UNSUCCESS) {
  const mobile_apis::FunctionID::eType kFunctionId =
      mobile_apis::FunctionID::GetVehicleDataID;
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::function_id] = kFunctionId;
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  GetVehicleDataRequestPtr command(
      CreateCommand<GetVehicleDataRequest>(command_msg));

  MockAppPtr app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));

  EXPECT_CALL(
      *app,
      AreCommandLimitsExceeded(kFunctionId, am::TLimitSource::CONFIG_FILE))
      .WillOnce(Return(true));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::REJECTED), _));

  command->Run();
}

TEST_F(GetVehicleDataRequestTest, Run_EmptyMsgParams_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  GetVehicleDataRequestPtr command(
      CreateCommand<GetVehicleDataRequest>(command_msg));

  const am::VehicleData kEmptyVehicleData;
  EXPECT_CALL(mock_message_helper_, vehicle_data())
      .WillOnce(ReturnRef(kEmptyVehicleData));

  MockAppPtr app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command->Run();
}

TEST_F(GetVehicleDataRequestTest,
       Run_EmptyMsgParamsAndHasDisallowedParams_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  SharedPtr<UnwrappedGetVehicleDataRequest> command(
      CreateCommand<UnwrappedGetVehicleDataRequest>(command_msg));

  const am::VehicleData kEmptyVehicleData;
  EXPECT_CALL(mock_message_helper_, vehicle_data())
      .WillRepeatedly(ReturnRef(kEmptyVehicleData));

  std::vector<std::string>& disallowed_params =
      command->get_disallowed_params();
  disallowed_params.push_back("test_param");

  MockAppPtr app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::DISALLOWED), _));

  command->Run();
}

TEST_F(GetVehicleDataRequestTest, Run_SUCCESS) {
  const std::string kMsgParamKey("test_key");

  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][kMsgParamKey] = true;

  GetVehicleDataRequestPtr command(
      CreateCommand<GetVehicleDataRequest>(command_msg));

  am::VehicleData vehicle_data;
  vehicle_data.insert(
      am::VehicleData::value_type(kMsgParamKey, am::VehicleDataType::SPEED));
  EXPECT_CALL(mock_message_helper_, vehicle_data())
      .WillOnce(ReturnRef(vehicle_data));

  MockAppPtr app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));

  EXPECT_CALL(app_mngr_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::VehicleInfo_GetVehicleData)));

  command->Run();
}

TEST_F(GetVehicleDataRequestTest, OnEvent_UnknownEvent_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  SharedPtr<UnwrappedGetVehicleDataRequest> command(
      CreateCommand<UnwrappedGetVehicleDataRequest>(command_msg));

  Event event(hmi_apis::FunctionID::INVALID_ENUM);

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  command->on_event(event);
}

TEST_F(GetVehicleDataRequestTest, OnEvent_DataNotAvailable_SUCCESS) {
  const hmi_apis::Common_Result::eType hmi_response_code =
      hmi_apis::Common_Result::DATA_NOT_AVAILABLE;
  const mobile_result::eType mobile_response_code =
      mobile_result::VEHICLE_DATA_NOT_AVAILABLE;

  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  SharedPtr<UnwrappedGetVehicleDataRequest> command(
      CreateCommand<UnwrappedGetVehicleDataRequest>(command_msg));

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_response_code;
  (*event_msg)[am::strings::params][am::strings::error_msg] = "test_error";
  (*event_msg)[am::strings::msg_params][am::hmi_response::method] = 0;

  Event event(hmi_apis::FunctionID::VehicleInfo_GetVehicleData);
  event.set_smart_object(*event_msg);

  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(hmi_response_code))
      .WillOnce(Return(mobile_response_code));

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(MobileResultCodeIs(mobile_response_code), _));

  command->on_event(event);
}

#endif  // HMI_DBUS_API

}  // namespace get_vehicle_data_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
