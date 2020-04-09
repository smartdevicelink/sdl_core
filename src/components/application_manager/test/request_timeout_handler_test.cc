/*
 * Copyright (c) 2020, Ford Motor Company
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

#include "application_manager/request_timeout_handler_impl.h"

#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_request_controller.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "sdl_rpc_plugin/commands/hmi/on_reset_timeout_notification.h"
#include "sdl_rpc_plugin/commands/mobile/subscribe_way_points_request.h"
#include "sdl_rpc_plugin/commands/mobile/unsubscribe_way_points_request.h"

namespace test {
namespace components {
namespace request_timeout_handler_test {

using application_manager::event_engine::Event;
using ::testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;
using namespace application_manager::commands;
using namespace application_manager::strings;
using namespace application_manager::request_controller;
using application_manager_test::MockRequestController;
using sdl_rpc_plugin::commands::SubscribeWayPointsRequest;
using sdl_rpc_plugin::commands::UnsubscribeWayPointsRequest;

namespace {
const uint32_t kDefaultTimeout = 10000u;
const uint32_t kTimeout = 13000u;
const uint32_t kRequestId = 22;
const std::string kMethodNameSubscribeWayPoints =
    "Navigation.SubscribeWayPoints";
const std::string kMethodNameUnsubscribeWayPoints =
    "Navigation.UnsubscribeWayPoints";
const uint32_t kConnectionKey = 3u;
}  // namespace

class RequestTimeoutHandlerTest
    : public commands_test::CommandRequestTest<
          commands_test::CommandsTestMocks::kIsNice> {
 public:
  RequestTimeoutHandlerTest()
      : mock_message_helper_(
            application_manager::MockMessageHelper::message_helper_mock()) {}

 protected:
  void SetUp() OVERRIDE {
    ON_CALL(app_mngr_, event_dispatcher())
        .WillByDefault(ReturnRef(event_dispatcher_));
    ON_CALL(app_mngr_, get_request_controller())
        .WillByDefault(ReturnRef(mock_request_controller_));
    request_timeout_handler_ =
        std::make_shared<RequestTimeoutHandlerImpl>(app_mngr_);
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  template <typename Command>
  std::unique_ptr<Command> CreateCommand() {
    ON_CALL(app_mngr_, get_settings())
        .WillByDefault(ReturnRef(app_mngr_settings_));
    ON_CALL(app_mngr_settings_, default_timeout())
        .WillByDefault(ReturnRef(kDefaultTimeout));

    std::shared_ptr<smart_objects::SmartObject> msg =
        std::make_shared<smart_objects::SmartObject>(
            smart_objects::SmartType_Null);

    std::unique_ptr<Command> command(new Command(msg,
                                                 app_mngr_,
                                                 mock_rpc_service_,
                                                 mock_hmi_capabilities_,
                                                 mock_policy_handler_));
    return command;
  }

  application_manager::MockMessageHelper* mock_message_helper_;
  MockRequestController mock_request_controller_;
  std::shared_ptr<RequestTimeoutHandlerImpl> request_timeout_handler_;
  testing::NiceMock<application_manager_test::MockHMICapabilities>
      mock_hmi_capabilities_;
  policy_test::MockPolicyHandlerInterface mock_policy_handler_;
};

TEST_F(RequestTimeoutHandlerTest, OnEvent_OnResetTimeout_SUCCESS) {
  // RequestTimeoutHandler works with all the commands.
  // But just for testing, only SubscribeWayPointsRequest was chosen in current
  // test.
  std::unique_ptr<SubscribeWayPointsRequest> command =
      CreateCommand<SubscribeWayPointsRequest>();
  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));
  ON_CALL(app_mngr_, IsAppSubscribedForWayPoints(_))
      .WillByDefault(Return(false));
  ON_CALL(app_mngr_, IsAnyAppSubscribedForWayPoints())
      .WillByDefault(Return(false));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kRequestId));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).WillOnce(Return(true));

  Event event(hmi_apis::FunctionID::BasicCommunication_OnResetTimeout);
  smart_objects::SmartObject notification_msg;
  notification_msg[msg_params][timeout_period_for_reset] = kTimeout;
  notification_msg[msg_params][request_id] = kRequestId;
  notification_msg[msg_params][method_name] = kMethodNameSubscribeWayPoints;
  event.set_smart_object(notification_msg);

  ON_CALL(app_mngr_, get_settings())
      .WillByDefault(ReturnRef(app_mngr_settings_));
  ON_CALL(app_mngr_settings_, default_timeout())
      .WillByDefault(ReturnRef(kDefaultTimeout));

  EXPECT_CALL(*mock_message_helper_,
              HMIFunctionIDFromString(kMethodNameSubscribeWayPoints))
      .WillOnce(Return(hmi_apis::FunctionID::Navigation_SubscribeWayPoints));
  EXPECT_CALL(app_mngr_, get_request_timeout_handler())
      .WillOnce(ReturnRef(*request_timeout_handler_));

  EXPECT_CALL(mock_request_controller_,
              IsRequestTimeoutUpdateRequired(
                  mock_app->app_id(), command->correlation_id(), kTimeout))
      .WillOnce(Return(true));
  EXPECT_CALL(app_mngr_,
              UpdateRequestTimeout(
                  mock_app->app_id(), command->correlation_id(), kTimeout));

  ASSERT_TRUE(command->Init());
  command->Run();
  request_timeout_handler_->on_event(event);
}

TEST_F(RequestTimeoutHandlerTest, OnEvent_OnResetTimeout_MissedResetPeriod) {
  // RequestTimeoutHandler works with all the commands.
  // But just for testing, only SubscribeWayPointsRequest was chosen in current
  // test.
  std::unique_ptr<SubscribeWayPointsRequest> command =
      CreateCommand<SubscribeWayPointsRequest>();
  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));
  ON_CALL(app_mngr_, IsAppSubscribedForWayPoints(_))
      .WillByDefault(Return(false));
  ON_CALL(app_mngr_, IsAnyAppSubscribedForWayPoints())
      .WillByDefault(Return(false));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kRequestId));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).WillOnce(Return(true));

  Event event(hmi_apis::FunctionID::BasicCommunication_OnResetTimeout);
  smart_objects::SmartObject notification_msg;
  notification_msg[msg_params][request_id] = kRequestId;
  notification_msg[msg_params][method_name] = kMethodNameSubscribeWayPoints;
  event.set_smart_object(notification_msg);

  ON_CALL(app_mngr_, get_settings())
      .WillByDefault(ReturnRef(app_mngr_settings_));
  ON_CALL(app_mngr_settings_, default_timeout())
      .WillByDefault(ReturnRef(kDefaultTimeout));

  EXPECT_CALL(*mock_message_helper_,
              HMIFunctionIDFromString(kMethodNameSubscribeWayPoints))
      .WillOnce(Return(hmi_apis::FunctionID::Navigation_SubscribeWayPoints));
  EXPECT_CALL(app_mngr_, get_request_timeout_handler())
      .WillOnce(ReturnRef(*request_timeout_handler_));
  EXPECT_CALL(
      mock_request_controller_,
      IsRequestTimeoutUpdateRequired(
          mock_app->app_id(), command->correlation_id(), kDefaultTimeout))
      .WillOnce(Return(true));
  EXPECT_CALL(
      app_mngr_,
      UpdateRequestTimeout(
          mock_app->app_id(), command->correlation_id(), kDefaultTimeout));

  ASSERT_TRUE(command->Init());
  command->Run();
  request_timeout_handler_->on_event(event);
}

TEST_F(RequestTimeoutHandlerTest, OnEvent_OnResetTimeout_InvalidRequestId) {
  // RequestTimeoutHandler works with all the commands.
  // But just for testing, only UnsubscribeWayPointsRequest was chosen in
  // current test.
  std::unique_ptr<UnsubscribeWayPointsRequest> command =
      CreateCommand<UnsubscribeWayPointsRequest>();
  MockAppPtr mock_app = CreateMockApp();

  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app));
  EXPECT_CALL(app_mngr_,
              IsAppSubscribedForWayPoints(
                  ::testing::Matcher<application_manager::ApplicationSharedPtr>(
                      mock_app)))
      .WillOnce(Return(true));

  const std::set<uint32_t> subscribed_apps{kConnectionKey};

  EXPECT_CALL(app_mngr_, GetAppsSubscribedForWayPoints())
      .WillOnce(Return(subscribed_apps));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).WillOnce(Return(true));

  Event event(hmi_apis::FunctionID::BasicCommunication_OnResetTimeout);
  smart_objects::SmartObject notification_msg;
  notification_msg[msg_params][timeout_period_for_reset] = kTimeout;
  notification_msg[msg_params][method_name] = kMethodNameUnsubscribeWayPoints;
  notification_msg[msg_params][request_id] = kRequestId;
  event.set_smart_object(notification_msg);

  ON_CALL(app_mngr_, get_settings())
      .WillByDefault(ReturnRef(app_mngr_settings_));
  ON_CALL(app_mngr_settings_, default_timeout())
      .WillByDefault(ReturnRef(kDefaultTimeout));

  EXPECT_CALL(*mock_message_helper_,
              HMIFunctionIDFromString(kMethodNameUnsubscribeWayPoints))
      .WillOnce(Return(hmi_apis::FunctionID::Navigation_UnsubscribeWayPoints));
  EXPECT_CALL(app_mngr_, get_request_timeout_handler())
      .WillOnce(ReturnRef(*request_timeout_handler_));
  EXPECT_CALL(mock_request_controller_, IsRequestTimeoutUpdateRequired(_, _, _))
      .Times(0);
  EXPECT_CALL(app_mngr_, UpdateRequestTimeout(_, _, _)).Times(0);

  ASSERT_TRUE(command->Init());
  command->Run();
  request_timeout_handler_->on_event(event);
}

TEST_F(RequestTimeoutHandlerTest, OnEvent_OnResetTimeout_InvalidMethodName) {
  // RequestTimeoutHandler works with all the commands.
  // But just for testing, only UnsubscribeWayPointsRequest was chosen in
  // current test.
  std::unique_ptr<UnsubscribeWayPointsRequest> command =
      CreateCommand<UnsubscribeWayPointsRequest>();
  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app));
  EXPECT_CALL(app_mngr_,
              IsAppSubscribedForWayPoints(
                  ::testing::Matcher<application_manager::ApplicationSharedPtr>(
                      mock_app)))
      .WillOnce(Return(true));

  const std::set<uint32_t> subscribed_apps{kConnectionKey};

  EXPECT_CALL(app_mngr_, GetAppsSubscribedForWayPoints())
      .WillOnce(Return(subscribed_apps));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _)).WillOnce(Return(true));

  Event event(hmi_apis::FunctionID::BasicCommunication_OnResetTimeout);
  smart_objects::SmartObject notification_msg;
  notification_msg[msg_params][timeout_period_for_reset] = kTimeout;
  notification_msg[msg_params][method_name] = " ";
  notification_msg[msg_params][request_id] = 0u;
  event.set_smart_object(notification_msg);

  ON_CALL(app_mngr_, get_settings())
      .WillByDefault(ReturnRef(app_mngr_settings_));
  ON_CALL(app_mngr_settings_, default_timeout())
      .WillByDefault(ReturnRef(kDefaultTimeout));

  EXPECT_CALL(*mock_message_helper_, HMIFunctionIDFromString(" "))
      .WillOnce(Return(hmi_apis::FunctionID::INVALID_ENUM));
  EXPECT_CALL(app_mngr_, get_request_timeout_handler())
      .WillOnce(ReturnRef(*request_timeout_handler_));
  EXPECT_CALL(mock_request_controller_, IsRequestTimeoutUpdateRequired(_, _, _))
      .Times(0);
  EXPECT_CALL(app_mngr_, UpdateRequestTimeout(_, _, _)).Times(0);

  ASSERT_TRUE(command->Init());
  command->Run();
  request_timeout_handler_->on_event(event);
}
}  // namespace request_timeout_handler_test
}  // namespace components
}  // namespace test
