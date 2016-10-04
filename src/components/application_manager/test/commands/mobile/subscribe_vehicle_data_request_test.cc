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

#include <string>
#include "gtest/gtest.h"
#include "mobile/subscribe_vehicle_data_request.h"
#include "utils/shared_ptr.h"
#include "commands/commands_test.h"
#include "commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/smart_object_keys.h"
#include "mock_message_helper.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace subscribe_vehicle_data_request {

using application_manager::commands::SubscribeVehicleDataRequest;
using ::testing::DefaultValue;
using ::testing::Mock;
using application_manager::MockMessageHelper;

namespace mobile_result = mobile_apis::Result;
namespace am = application_manager;

typedef SharedPtr<SubscribeVehicleDataRequest> CommandPtr;

namespace {
const uint32_t kAppId = 10u;
const uint32_t kConnectionKey = 5u;
const std::string kKeyName = "key_name";
}

class SubscribeVehicleDataRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SubscribeVehicleDataRequestTest() : app_(CreateMockApp()) {
    msg_ = CreateMessage(smart_objects::SmartType_Map);
    data_type = am::VehicleDataType::SPEED;
    vehicle_data.insert(
        std::pair<std::string, am::VehicleDataType>(kKeyName, data_type));
    EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
                vehicle_data()).WillRepeatedly(ReturnRef(vehicle_data));
  }

  ~SubscribeVehicleDataRequestTest() {
    // Fix DataAccessor release and WinQt crash
    Mock::VerifyAndClearExpectations(&mock_app_manager_);
  }

 protected:
  void AlreadySubscribedByThisApp();
  MockAppPtr app_;
  MessageSharedPtr msg_;

  am::VehicleData vehicle_data;
  am::VehicleDataType data_type;
  sync_primitives::Lock app_set_lock_;
};

MATCHER_P6(CheckResponseWithKeyParams,
           result_code,
           result_info,
           result_success,
           key_name,
           key_result_type,
           key_result_code,
           "") {
  mobile_apis::Result::eType code = static_cast<mobile_apis::Result::eType>(
      (*arg)[am::strings::msg_params][am::strings::result_code].asInt());
  std::string info =
      (*arg)[am::strings::msg_params][am::strings::info].asString();
  bool success = (*arg)[am::strings::msg_params][am::strings::success].asBool();

  am::VehicleDataType key_type = static_cast<am::VehicleDataType>(
      (*arg)[am::strings::msg_params][key_name][am::strings::data_type]
          .asInt());
  mobile_apis::VehicleDataResultCode::eType key_code =
      static_cast<mobile_apis::VehicleDataResultCode::eType>(
          (*arg)[am::strings::msg_params][key_name][am::strings::result_code]
              .asInt());

  return result_code == code && result_info == info &&
         result_success == success && key_result_type == key_type &&
         key_result_code == key_code;
}

class TestSubscribeVehicleDataRequest : public SubscribeVehicleDataRequest {
 public:
  TestSubscribeVehicleDataRequest(const MessageSharedPtr& message,
                                  am::ApplicationManager& application_manager)
      : SubscribeVehicleDataRequest(message, application_manager) {}

  am::CommandParametersPermissions& removed_parameters_permissions() {
    return removed_parameters_permissions_;
  }
};

TEST_F(SubscribeVehicleDataRequestTest, Run_ApplicationIsNotRegistered) {
  CommandPtr command(CreateCommand<SubscribeVehicleDataRequest>(msg_));

  ON_CALL(mock_app_manager_, application(_))
      .WillByDefault(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command->Run();
}

TEST_F(SubscribeVehicleDataRequestTest, Run_NoDataInRequest_InvalidData) {
  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  CommandPtr command(CreateCommand<SubscribeVehicleDataRequest>(msg_));
  const std::string info = "No data in the request";
  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));
  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(
          MobileResponseIs(mobile_result::INVALID_DATA, info, false), _));

  command->Run();
}

TEST_F(SubscribeVehicleDataRequestTest, Run_DataAlreadySubscribed_IGNORED) {
  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg_)[am::strings::msg_params][kKeyName] = true;
  CommandPtr command(CreateCommand<SubscribeVehicleDataRequest>(msg_));

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));
  // Data is subscribed
  EXPECT_CALL(*app_, IsSubscribedToIVI(data_type)).WillOnce(Return(true));

  const std::string info = "Already subscribed on some provided VehicleData.";
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::IGNORED, info, true), _));

  command->Run();
}

TEST_F(SubscribeVehicleDataRequestTest,
       Run_DataSubscribedByAnotherApp_Success) {
  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg_)[am::strings::msg_params][kKeyName] = true;
  CommandPtr command(CreateCommand<SubscribeVehicleDataRequest>(msg_));

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));
  // Data is not subscribed
  EXPECT_CALL(*app_, IsSubscribedToIVI(data_type)).WillOnce(Return(false));

  MockAppPtr app2_(CreateMockApp());
  am::ApplicationSet app_set;
  app_set.insert(app2_);

  DataAccessor<am::ApplicationSet> accessor(app_set, app_set_lock_);

  EXPECT_CALL(mock_app_manager_, applications()).WillOnce(Return(accessor));
  EXPECT_CALL(*app2_, IsSubscribedToIVI(data_type)).WillOnce(Return(true));

  EXPECT_CALL(*app_, SubscribeToIVI(static_cast<uint32_t>(data_type)))
      .WillOnce(Return(true));
  const std::string info = "";
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::SUCCESS, info, true), _));
  command->Run();
}

TEST_F(SubscribeVehicleDataRequestTest,
       Run_DataSubscribedByAnotherAppKeyCannotBeSubscribed_IGNORED) {
  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg_)[am::strings::msg_params][kKeyName] = true;
  CommandPtr command(CreateCommand<SubscribeVehicleDataRequest>(msg_));

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));
  // Data is not subscribed
  EXPECT_CALL(*app_, IsSubscribedToIVI(data_type)).WillOnce(Return(false));

  MockAppPtr app2_(CreateMockApp());
  am::ApplicationSet app_set;
  app_set.insert(app2_);

  DataAccessor<am::ApplicationSet> accessor(app_set, app_set_lock_);

  EXPECT_CALL(mock_app_manager_, applications()).WillOnce(Return(accessor));

  EXPECT_CALL(*app2_, IsSubscribedToIVI(data_type)).WillOnce(Return(true));
  // Key could not be subscribed
  EXPECT_CALL(*app_, SubscribeToIVI(static_cast<uint32_t>(data_type)))
      .WillOnce(Return(false));
  const std::string info = "Already subscribed on provided VehicleData.";
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::IGNORED, info, false), _));
  command->Run();
}

TEST_F(SubscribeVehicleDataRequestTest,
       Run_DataNotSubscribedByAnotherApp_Success) {
  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg_)[am::strings::msg_params][kKeyName] = true;
  CommandPtr command(CreateCommand<SubscribeVehicleDataRequest>(msg_));

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));
  // Data is not subscribed
  EXPECT_CALL(*app_, IsSubscribedToIVI(data_type)).WillOnce(Return(false));

  MockAppPtr app2_(CreateMockApp());
  am::ApplicationSet app_set;
  app_set.insert(app2_);

  DataAccessor<am::ApplicationSet> accessor(app_set, app_set_lock_);

  EXPECT_CALL(mock_app_manager_, applications()).WillOnce(Return(accessor));
  EXPECT_CALL(*app2_, IsSubscribedToIVI(data_type)).WillOnce(Return(false));

  EXPECT_CALL(*app_, SubscribeToIVI(static_cast<uint32_t>(data_type)))
      .WillOnce(Return(true));
  EXPECT_CALL(
      event_dispatcher_,
      add_observer(
          hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData, _, _));
  EXPECT_CALL(mock_app_manager_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData)))
      .WillOnce(Return(true));
  command->Run();
}

TEST_F(SubscribeVehicleDataRequestTest,
       Run_DataNotSubscribedByAnotherAppRequestNotSending_OutOfMemory) {
  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg_)[am::strings::msg_params][kKeyName] = true;
  CommandPtr command(CreateCommand<SubscribeVehicleDataRequest>(msg_));

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));
  // Data is not subscribed
  EXPECT_CALL(*app_, IsSubscribedToIVI(data_type)).WillOnce(Return(false));

  MockAppPtr app2_(CreateMockApp());
  am::ApplicationSet app_set;
  app_set.insert(app2_);

  DataAccessor<am::ApplicationSet> accessor(app_set, app_set_lock_);

  EXPECT_CALL(mock_app_manager_, applications()).WillOnce(Return(accessor));
  EXPECT_CALL(*app2_, IsSubscribedToIVI(data_type)).WillOnce(Return(false));

  EXPECT_CALL(*app_, SubscribeToIVI(static_cast<uint32_t>(data_type)))
      .WillOnce(Return(true));
  const std::string info = "";

  EXPECT_CALL(
      event_dispatcher_,
      add_observer(
          hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData, _, _));
  // Manage unsuccessful
  EXPECT_CALL(mock_app_manager_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData)))
      .WillOnce(Return(false));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(
          MobileResponseIs(mobile_result::OUT_OF_MEMORY, info, false), _));
  command->Run();
}

TEST_F(SubscribeVehicleDataRequestTest,
       OnEvent_NotSubscribedBefore_SuccessResult) {
  Event event(Event::EventID::VehicleInfo_SubscribeVehicleData);
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  const hmi_apis::Common_Result::eType hmi_code =
      hmi_apis::Common_Result::SUCCESS;
  const mobile_apis::Result::eType mobile_code = mobile_apis::Result::SUCCESS;
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_code;
  (*event_msg)[am::strings::msg_params][kKeyName][am::strings::result_code] =
      hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS;
  event.set_smart_object(*event_msg);

  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  CommandPtr command(CreateCommand<SubscribeVehicleDataRequest>(msg_));

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));

  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              HMIToMobileResult(hmi_code)).WillOnce(Return(mobile_code));

  const std::string info = "";
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::SUCCESS, info, true), _));
  EXPECT_CALL(*app_, UpdateHash());
  command->on_event(event);
}

TEST_F(SubscribeVehicleDataRequestTest,
       OnEvent_SubscribeUnsuccsess_UnsubscribeFromIvi_SuccessResult) {
  Event event(Event::EventID::VehicleInfo_SubscribeVehicleData);
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  const hmi_apis::Common_Result::eType hmi_code =
      hmi_apis::Common_Result::SUCCESS;
  const mobile_apis::Result::eType mobile_code = mobile_apis::Result::SUCCESS;
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_code;
  (*event_msg)[am::strings::msg_params][kKeyName][am::strings::result_code] =
      hmi_apis::Common_VehicleDataResultCode::VDRC_IGNORED;
  event.set_smart_object(*event_msg);

  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  CommandPtr command(CreateCommand<SubscribeVehicleDataRequest>(msg_));

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));
  // Expect unsibscribe from unsuccessful subscription
  EXPECT_CALL(*app_, UnsubscribeFromIVI(data_type));
  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              HMIToMobileResult(hmi_code)).WillOnce(Return(mobile_code));

  const std::string info = "";
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::SUCCESS, info, true), _));
  EXPECT_CALL(*app_, UpdateHash());
  command->on_event(event);
}

TEST_F(SubscribeVehicleDataRequestTest, OnEvent_AppAlreadySubscribed_Ignored) {
  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg_)[am::strings::msg_params][kKeyName] = true;
  CommandPtr command(CreateCommand<SubscribeVehicleDataRequest>(msg_));

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));
  // Data is subscribed
  EXPECT_CALL(*app_, IsSubscribedToIVI(data_type)).WillOnce(Return(true));

  const std::string info = "Already subscribed on some provided VehicleData.";
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::IGNORED, info, true), _));

  command->Run();

  Event event(Event::EventID::VehicleInfo_SubscribeVehicleData);
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  const hmi_apis::Common_Result::eType hmi_code =
      hmi_apis::Common_Result::SUCCESS;
  const mobile_apis::Result::eType mobile_code = mobile_apis::Result::SUCCESS;
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_code;
  (*event_msg)[am::strings::msg_params][kKeyName][am::strings::result_code] =
      hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS;
  event.set_smart_object(*event_msg);

  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              HMIToMobileResult(hmi_code)).WillOnce(Return(mobile_code));
  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(
          CheckResponseWithKeyParams(
              mobile_result::IGNORED,
              info,
              true,
              kKeyName,
              data_type,
              mobile_apis::VehicleDataResultCode::VDRC_DATA_ALREADY_SUBSCRIBED),
          _));
  EXPECT_CALL(*app_, UpdateHash());
  command->on_event(event);
}

TEST_F(SubscribeVehicleDataRequestTest,
       OnEvent_OtherAppIsSubscribedToIVI_SUCCESS) {
  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg_)[am::strings::msg_params][kKeyName] = true;
  CommandPtr command(CreateCommand<SubscribeVehicleDataRequest>(msg_));

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));
  // Data is subscribed
  EXPECT_CALL(*app_, IsSubscribedToIVI(data_type)).WillOnce(Return(false));

  MockAppPtr other_app(CreateMockApp());
  am::ApplicationSet app_set;
  app_set.insert(other_app);
  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(
          Return(DataAccessor<am::ApplicationSet>(app_set, app_set_lock_)));
  EXPECT_CALL(*other_app, IsSubscribedToIVI(data_type)).WillOnce(Return(true));

  EXPECT_CALL(*app_, SubscribeToIVI(data_type)).WillOnce(Return(true));
  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));

  command->Run();

  Event event(Event::EventID::VehicleInfo_SubscribeVehicleData);
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  const hmi_apis::Common_Result::eType hmi_result_code =
      hmi_apis::Common_Result::SUCCESS;
  const mobile_apis::Result::eType mobile_result_code =
      mobile_apis::Result::SUCCESS;
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_result_code;
  (*event_msg)[am::strings::msg_params][kKeyName][am::strings::result_code] =
      hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS;
  event.set_smart_object(*event_msg);

  EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
              HMIToMobileResult(hmi_result_code))
      .WillOnce(Return(mobile_result_code));
  const std::string info = "";
  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(CheckResponseWithKeyParams(
                              mobile_result_code,
                              info,
                              true,
                              kKeyName,
                              data_type,
                              mobile_apis::VehicleDataResultCode::VDRC_SUCCESS),
                          _));
  EXPECT_CALL(*app_, UpdateHash());
  command->on_event(event);
}

TEST_F(SubscribeVehicleDataRequestTest, Run_HasDisallowedParams_DISALLOWED) {
  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg_)[am::strings::msg_params][kKeyName] = false;
  SharedPtr<TestSubscribeVehicleDataRequest> command(
      CreateCommand<TestSubscribeVehicleDataRequest>(msg_));

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));

  am::CommandParametersPermissions& removed_parameters_permissions =
      command->removed_parameters_permissions();

  // If command have 0 items to subscribe
  // and disallowed parameters, it will send `DISALLOWED`
  removed_parameters_permissions.disallowed_params.push_back(
      "disallowed_param");
  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::DISALLOWED), _));

  command->Run();
}

}  // namespace subscribe_vehicle_data_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
