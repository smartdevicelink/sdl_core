/*
 * Copyright (c) 2017, Ford Motor Company
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
  SubscribeVehicleDataRequestTest()
      : app_(CreateMockApp())
      , mock_message_helper_(*am::MockMessageHelper::message_helper_mock())

  {
    msg_ = CreateMessage(smart_objects::SmartType_Map);
    data_type = am::VehicleDataType::SPEED;
    vehicle_data.insert(
        std::pair<std::string, am::VehicleDataType>(kKeyName, data_type));
    EXPECT_CALL(*application_manager::MockMessageHelper::message_helper_mock(),
                vehicle_data()).WillRepeatedly(ReturnRef(vehicle_data));
  }

  ~SubscribeVehicleDataRequestTest() {
    // Fix DataAccessor release and WinQt crash
    Mock::VerifyAndClearExpectations(&app_mngr_);
  }

 protected:
  MockAppPtr app_;
  MessageSharedPtr msg_;
  ::utils::SharedPtr<SubscribeVehicleDataRequest> command_;
  am::MockMessageHelper& mock_message_helper_;

  am::VehicleData vehicle_data;
  am::VehicleDataType data_type;
  sync_primitives::Lock app_set_lock_;
};

class TestSubscribeVehicleDataRequest : public SubscribeVehicleDataRequest {
 public:
  TestSubscribeVehicleDataRequest(const MessageSharedPtr& message,
                                  am::ApplicationManager& application_manager)
      : SubscribeVehicleDataRequest(message, application_manager) {}

  am::CommandParametersPermissions& removed_parameters_permissions() {
    return removed_parameters_permissions_;
  }
};

TEST_F(SubscribeVehicleDataRequestTest, Run_DataAlreadySubscribed_IGNORED) {
  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg_)[am::strings::msg_params][kKeyName] = true;
  CommandPtr command(CreateCommand<SubscribeVehicleDataRequest>(msg_));

  ON_CALL(app_mngr_, application(kConnectionKey)).WillByDefault(Return(app_));
  // Data is subscribed
  EXPECT_CALL(*app_, IsSubscribedToIVI(data_type)).WillOnce(Return(true));

  const std::string info = "Already subscribed on some provided VehicleData.";
  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::IGNORED, info, true), _));

  command->Run();
}

TEST_F(SubscribeVehicleDataRequestTest,
       OnEvent_HMIRespondSuccess_DataSubscribed) {
  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));

  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg_)[am::strings::msg_params][kKeyName] = true;
  CommandPtr command(CreateCommand<SubscribeVehicleDataRequest>(msg_));

  // Data previously is not subscribed
  EXPECT_CALL(*mock_app, IsSubscribedToIVI(_)).WillRepeatedly(Return(false));

  am::ApplicationSet app_set;
  app_set.insert(mock_app);

  DataAccessor<am::ApplicationSet> accessor(app_set, app_set_lock_);
  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(accessor));

  // Command was sent to HMI
  EXPECT_CALL(app_mngr_, ManageHMICommand(_));
  command->Run();
  // HMI respond success
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::msg_params] = 0;
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      mobile_result::SUCCESS;
  Event event(hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(*event_msg);

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::SUCCESS))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));
  // Application will subscribe data
  EXPECT_CALL(*mock_app, SubscribeToIVI(static_cast<uint32_t>(data_type)));

  command->on_event(event);
}

TEST_F(SubscribeVehicleDataRequestTest,
       OnEvent_HMIRespondFail_DataNotSubscribed) {
  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));

  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg_)[am::strings::msg_params][kKeyName] = true;
  CommandPtr command(CreateCommand<SubscribeVehicleDataRequest>(msg_));
  // Data previously is not subscribed
  EXPECT_CALL(*mock_app, IsSubscribedToIVI(_)).WillRepeatedly(Return(false));

  am::ApplicationSet app_set;
  app_set.insert(mock_app);

  DataAccessor<am::ApplicationSet> accessor(app_set, app_set_lock_);

  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(accessor));

  // Command was sent to HMI
  EXPECT_CALL(app_mngr_, ManageHMICommand(_));
  command->Run();

  // HMI respond with fail
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::msg_params] = 0;
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      mobile_result::INVALID_DATA;
  Event event(hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData);
  event.set_smart_object(*event_msg);

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::INVALID_DATA))
      .WillOnce(Return(mobile_apis::Result::INVALID_DATA));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));
  // Application will not subscribe data
  EXPECT_CALL(*mock_app, SubscribeToIVI(_)).Times(0);

  command->on_event(event);
}

}  // namespace subscribe_vehicle_data_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
