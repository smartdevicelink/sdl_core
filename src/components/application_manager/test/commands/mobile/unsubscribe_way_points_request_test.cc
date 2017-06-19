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

#include "gtest/gtest.h"
#include "mobile/unsubscribe_way_points_request.h"
#include "interfaces/MOBILE_API.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "commands/commands_test.h"
#include "commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/event_engine/event.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace unsubscribe_way_points_request {

namespace am = ::application_manager;
namespace mobile_result = mobile_apis::Result;

using ::testing::_;
using ::testing::Return;

using am::commands::UnSubscribeWayPointsRequest;
using am::commands::MessageSharedPtr;

namespace {
const uint32_t kConnectionKey = 3u;
const uint32_t kAppId = 5u;
}  // namespace

class UnSubscribeWayPointsRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  UnSubscribeWayPointsRequestTest()
      : command_msg_(CreateMessage(smart_objects::SmartType_Map))
      , command_(CreateCommand<UnSubscribeWayPointsRequest>(command_msg_))
      , mock_message_helper_(*am::MockMessageHelper::message_helper_mock()) {
    (*command_msg_)[am::strings::params][am::strings::connection_key] =
        kConnectionKey;
    testing::Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  ~UnSubscribeWayPointsRequestTest() {
    testing::Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  MessageSharedPtr command_msg_;
  ::utils::SharedPtr<UnSubscribeWayPointsRequest> command_;
  am::MockMessageHelper& mock_message_helper_;
};

TEST_F(UnSubscribeWayPointsRequestTest,
       Run_ApplicationIsNotRegistered_UNSUCCESS) {
  EXPECT_CALL(app_mngr_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command_->Run();
}

TEST_F(UnSubscribeWayPointsRequestTest,
       Run_AppIsNotSubscribedForWayPoints_UNSUCCESS) {
  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(*mock_app, app_id()).WillOnce(Return(kAppId));

  EXPECT_CALL(app_mngr_, IsAppSubscribedForWayPoints(kAppId))
      .WillOnce(Return(false));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::IGNORED), _));

  command_->Run();
}

TEST_F(UnSubscribeWayPointsRequestTest, Run_AppSubscribedForWayPoints_SUCCESS) {
  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(*mock_app, app_id()).WillOnce(Return(kAppId));

  EXPECT_CALL(app_mngr_, IsAppSubscribedForWayPoints(kAppId))
      .WillOnce(Return(true));

  EXPECT_CALL(app_mngr_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::Navigation_UnsubscribeWayPoints)));

  command_->Run();
}

TEST_F(UnSubscribeWayPointsRequestTest, OnEvent_UnknownEvent_UNSUCCESS) {
  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  Event event(hmi_apis::FunctionID::INVALID_ENUM);

  command_->on_event(event);
}

TEST_F(UnSubscribeWayPointsRequestTest,
       OnEvent_ReceivedNavigationUnSubscribeWayPointsEvent_SUCCESS) {
  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::msg_params] = 0;
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      mobile_result::SUCCESS;
  Event event(hmi_apis::FunctionID::Navigation_UnsubscribeWayPoints);
  event.set_smart_object(*event_msg);

  EXPECT_CALL(*mock_app, app_id()).WillOnce(Return(kAppId));

  EXPECT_CALL(app_mngr_, UnsubscribeAppFromWayPoints(kAppId));

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::SUCCESS))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));

  command_->on_event(event);
}

}  // namespace unsubscribe_way_points_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
