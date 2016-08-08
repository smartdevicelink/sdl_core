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
#include "mobile/subscribe_way_points_request.h"
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

using application_manager::commands::SubscribeWayPointsRequest;
using ::testing::DefaultValue;
using application_manager::MockMessageHelper;

namespace mobile_result = mobile_apis::Result;
namespace am = application_manager;

typedef SharedPtr<SubscribeWayPointsRequest> CommandPtr;

namespace {
const uint32_t kAppId = 10u;
const uint32_t kKey = 5u;
}

class SubscribeWayPointsRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  SubscribeWayPointsRequestTest() : app_(CreateMockApp()), info_("") {
    msg_ = CreateMessage(smart_objects::SmartType_Map);
    (*msg_)[am::strings::params][am::strings::connection_key] = kKey;
  }

  void SetUp() OVERRIDE {
    ON_CALL(*app_, app_id()).WillByDefault(Return(kAppId));
  }

  MockAppPtr app_;
  MessageSharedPtr msg_;
  std::string info_;
};

TEST_F(SubscribeWayPointsRequestTest, Run_ApplicationIsNotRegistered) {
  CommandPtr command(CreateCommand<SubscribeWayPointsRequest>(msg_));

  ON_CALL(app_mngr_, application(_))
      .WillByDefault(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command->Run();
}

TEST_F(SubscribeWayPointsRequestTest, Run_WayPointsSubscribedBefore_Ignored) {
  CommandPtr command(CreateCommand<SubscribeWayPointsRequest>(msg_));
  ON_CALL(app_mngr_, application(kKey)).WillByDefault(Return(app_));

  EXPECT_CALL(app_mngr_, IsAppSubscribedForWayPoints(kAppId))
      .WillOnce(Return(true));
  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::IGNORED, info_, false), _));

  command->Run();
}

TEST_F(SubscribeWayPointsRequestTest, Run_SomeAppSubscribedBefore_Success) {
  CommandPtr command(CreateCommand<SubscribeWayPointsRequest>(msg_));
  ON_CALL(app_mngr_, application(kKey)).WillByDefault(Return(app_));

  EXPECT_CALL(app_mngr_, IsAppSubscribedForWayPoints(kAppId))
      .WillOnce(Return(false));
  EXPECT_CALL(app_mngr_, IsAnyAppSubscribedForWayPoints())
      .WillOnce(Return(true));
  EXPECT_CALL(app_mngr_, SubscribeAppForWayPoints(kAppId));
  EXPECT_CALL(*app_, UpdateHash());

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::SUCCESS, info_, true), _));

  command->Run();
}

TEST_F(SubscribeWayPointsRequestTest,
       Run_AnyAppSubscribedBefore_SuccessSubscribe) {
  CommandPtr command(CreateCommand<SubscribeWayPointsRequest>(msg_));
  ON_CALL(app_mngr_, application(kKey)).WillByDefault(Return(app_));

  EXPECT_CALL(app_mngr_, IsAppSubscribedForWayPoints(kAppId))
      .WillOnce(Return(false));
  EXPECT_CALL(app_mngr_, IsAnyAppSubscribedForWayPoints())
      .WillOnce(Return(false));
  // App is not subscribed now
  EXPECT_CALL(app_mngr_, SubscribeAppForWayPoints(kAppId)).Times(0);

  // Subscribe on Navigation_SubscribeWayPoints
  EXPECT_CALL(
      event_dispatcher_,
      add_observer(hmi_apis::FunctionID::Navigation_SubscribeWayPoints, _, _));

  EXPECT_CALL(app_mngr_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::Navigation_SubscribeWayPoints)))
      .WillOnce(Return(true));

  // Response to mobile is not sending
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  command->Run();
}

TEST_F(SubscribeWayPointsRequestTest, OnEvent_SuccessResult_SubscribeApp) {
  Event event(Event::EventID::Navigation_SubscribeWayPoints);

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));

  const hmi_apis::Common_Result::eType hmi_code =
      hmi_apis::Common_Result::SUCCESS;
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_code;
  smart_objects::SmartObject& unused = (*event_msg)[am::strings::msg_params];
  // msg_params is unused in this case
  UNUSED(unused)
  event.set_smart_object(*event_msg);

  CommandPtr command(CreateCommand<SubscribeWayPointsRequest>(msg_));
  ON_CALL(app_mngr_, application(kKey)).WillByDefault(Return(app_));

  EXPECT_CALL(app_mngr_, SubscribeAppForWayPoints(kAppId));

  EXPECT_CALL(*app_, UpdateHash());

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::SUCCESS, info_, true), _));
  command->on_event(event);
}

TEST_F(SubscribeWayPointsRequestTest, OnEvent_UnsuccessResult_NotSubscribeApp) {
  Event event(Event::EventID::Navigation_SubscribeWayPoints);

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));

  const hmi_apis::Common_Result::eType hmi_code =
      hmi_apis::Common_Result::GENERIC_ERROR;
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_code;
  smart_objects::SmartObject& unused = (*event_msg)[am::strings::msg_params];
  // msg_params is unused in this case
  UNUSED(unused)
  event.set_smart_object(*event_msg);

  CommandPtr command(CreateCommand<SubscribeWayPointsRequest>(msg_));
  ON_CALL(app_mngr_, application(kKey)).WillByDefault(Return(app_));

  EXPECT_CALL(app_mngr_, SubscribeAppForWayPoints(kAppId)).Times(0);

  EXPECT_CALL(*app_, UpdateHash()).Times(0);

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResponseIs(mobile_result::GENERIC_ERROR, info_, false), _));
  command->on_event(event);
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
