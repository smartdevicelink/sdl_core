/*
 * Copyright (c) 2018, Ford Motor Company
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
#include "application_manager/reset_timeout_handler_impl.h"
#include "sdl_rpc_plugin/commands/hmi/on_reset_timeout_notification.h"
#include "sdl_rpc_plugin/commands/mobile/subscribe_way_points_request.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_message_helper.h"

namespace test {
namespace components {
namespace reset_timeout_handler_test {
using ::testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;
using application_manager::event_engine::Event;
using namespace application_manager::commands;
using namespace application_manager::strings;
using namespace application_manager::request_controller;
using sdl_rpc_plugin::commands::hmi::OnResetTimeoutNotification;
using sdl_rpc_plugin::commands::SubscribeWayPointsRequest;

typedef std::shared_ptr<OnResetTimeoutNotification> NotificationPtr;

const uint32_t kDefaultTimeout = 10000u;
const uint32_t kTimeout = 13000u;
const uint32_t kRequestId = 22;
const std::string kMethodName = "Navigation.SubscribeWayPoints";

class ResetTimeoutHandlerTest : public commands_test::CommandRequestTest<
                                    commands_test::CommandsTestMocks::kIsNice> {
 public:
  ResetTimeoutHandlerTest()
      : mock_message_helper_(
            application_manager::MockMessageHelper::message_helper_mock()) {}

 protected:
  std::shared_ptr<ResetTimeoutHandlerImpl> reset_timeout_handler_;
  virtual void SetUp() OVERRIDE {
    ON_CALL(app_mngr_, event_dispatcher())
        .WillByDefault(ReturnRef(event_dispatcher_));
    reset_timeout_handler_ =
        std::make_shared<ResetTimeoutHandlerImpl>(app_mngr_);
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  application_manager::MockMessageHelper* mock_message_helper_;
};

TEST_F(ResetTimeoutHandlerTest, on_event_OnResetTimeout_success) {
  std::shared_ptr<SubscribeWayPointsRequest> command =
      CreateCommand<SubscribeWayPointsRequest>();
  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));
  ON_CALL(app_mngr_, IsAppSubscribedForWayPoints(_))
      .WillByDefault(Return(false));
  ON_CALL(app_mngr_, IsAnyAppSubscribedForWayPoints())
      .WillByDefault(Return(false));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kRequestId));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).WillOnce(Return(true));

  Event event(hmi_apis::FunctionID::BasicCommunication_OnResetTimeout);
  smart_objects::SmartObject notification_msg;
  notification_msg[msg_params][reset_period] = kTimeout;
  notification_msg[msg_params][request_id] = kRequestId;
  notification_msg[msg_params][method_name] = kMethodName;
  event.set_smart_object(notification_msg);

  ON_CALL(app_mngr_, get_settings())
      .WillByDefault(ReturnRef(app_mngr_settings_));
  ON_CALL(app_mngr_settings_, default_timeout())
      .WillByDefault(ReturnRef(kDefaultTimeout));

  EXPECT_CALL(*mock_message_helper_, HMIFunctionIDFromString(kMethodName))
      .WillOnce(Return(hmi_apis::FunctionID::Navigation_SubscribeWayPoints));
  EXPECT_CALL(app_mngr_, GetResetTimeoutHandler())
      .WillOnce(ReturnRef(*reset_timeout_handler_));
  EXPECT_CALL(app_mngr_,
              IsUpdateRequestTimeoutRequired(
                  mock_app->app_id(), command->correlation_id(), kTimeout))
      .WillOnce(Return(true));
  EXPECT_CALL(app_mngr_,
              updateRequestTimeout(
                  mock_app->app_id(), command->correlation_id(), kTimeout));

  ASSERT_TRUE(command->Init());
  command->Run();
  reset_timeout_handler_->on_event(event);
}

TEST_F(ResetTimeoutHandlerTest, on_event_OnResetTimeout_missed_reset_period) {
  std::shared_ptr<SubscribeWayPointsRequest> command =
      CreateCommand<SubscribeWayPointsRequest>();
  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));
  ON_CALL(app_mngr_, IsAppSubscribedForWayPoints(_))
      .WillByDefault(Return(false));
  ON_CALL(app_mngr_, IsAnyAppSubscribedForWayPoints())
      .WillByDefault(Return(false));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kRequestId));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).WillOnce(Return(true));

  Event event(hmi_apis::FunctionID::BasicCommunication_OnResetTimeout);
  smart_objects::SmartObject notification_msg;
  notification_msg[msg_params][request_id] = kRequestId;
  notification_msg[msg_params][method_name] = kMethodName;
  event.set_smart_object(notification_msg);

  ON_CALL(app_mngr_, get_settings())
      .WillByDefault(ReturnRef(app_mngr_settings_));
  ON_CALL(app_mngr_settings_, default_timeout())
      .WillByDefault(ReturnRef(kDefaultTimeout));

  EXPECT_CALL(*mock_message_helper_, HMIFunctionIDFromString(kMethodName))
      .WillOnce(Return(hmi_apis::FunctionID::Navigation_SubscribeWayPoints));
  EXPECT_CALL(app_mngr_, GetResetTimeoutHandler())
      .WillOnce(ReturnRef(*reset_timeout_handler_));
  EXPECT_CALL(
      app_mngr_,
      IsUpdateRequestTimeoutRequired(
          mock_app->app_id(), command->correlation_id(), kDefaultTimeout))
      .WillOnce(Return(true));
  EXPECT_CALL(
      app_mngr_,
      updateRequestTimeout(
          mock_app->app_id(), command->correlation_id(), kDefaultTimeout));

  ASSERT_TRUE(command->Init());
  command->Run();
  reset_timeout_handler_->on_event(event);
}

TEST_F(ResetTimeoutHandlerTest, on_event_OnResetTimeout_invalid_request_id) {
  std::shared_ptr<SubscribeWayPointsRequest> command =
      CreateCommand<SubscribeWayPointsRequest>();
  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));
  ON_CALL(app_mngr_, IsAppSubscribedForWayPoints(_))
      .WillByDefault(Return(false));
  ON_CALL(app_mngr_, IsAnyAppSubscribedForWayPoints())
      .WillByDefault(Return(false));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kRequestId));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).WillOnce(Return(true));

  Event event(hmi_apis::FunctionID::BasicCommunication_OnResetTimeout);
  smart_objects::SmartObject notification_msg;
  notification_msg[msg_params][reset_period] = kTimeout;
  notification_msg[msg_params][method_name] = kMethodName;
  notification_msg[msg_params][request_id] = 0u;
  event.set_smart_object(notification_msg);

  ON_CALL(app_mngr_, get_settings())
      .WillByDefault(ReturnRef(app_mngr_settings_));
  ON_CALL(app_mngr_settings_, default_timeout())
      .WillByDefault(ReturnRef(kDefaultTimeout));

  EXPECT_CALL(*mock_message_helper_, HMIFunctionIDFromString(kMethodName))
      .WillOnce(Return(hmi_apis::FunctionID::Navigation_SubscribeWayPoints));
  EXPECT_CALL(app_mngr_, GetResetTimeoutHandler())
      .WillOnce(ReturnRef(*reset_timeout_handler_));
  EXPECT_CALL(app_mngr_, IsUpdateRequestTimeoutRequired(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _)).Times(0);

  ASSERT_TRUE(command->Init());
  command->Run();
  reset_timeout_handler_->on_event(event);
}

TEST_F(ResetTimeoutHandlerTest, on_event_OnResetTimeout_invalid_method_name) {
  std::shared_ptr<SubscribeWayPointsRequest> command =
      CreateCommand<SubscribeWayPointsRequest>();
  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));
  ON_CALL(app_mngr_, IsAppSubscribedForWayPoints(_))
      .WillByDefault(Return(false));
  ON_CALL(app_mngr_, IsAnyAppSubscribedForWayPoints())
      .WillByDefault(Return(false));
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kRequestId));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).WillOnce(Return(true));

  Event event(hmi_apis::FunctionID::BasicCommunication_OnResetTimeout);
  smart_objects::SmartObject notification_msg;
  notification_msg[msg_params][reset_period] = kTimeout;
  notification_msg[msg_params][method_name] = " ";
  notification_msg[msg_params][request_id] = 0u;
  event.set_smart_object(notification_msg);

  ON_CALL(app_mngr_, get_settings())
      .WillByDefault(ReturnRef(app_mngr_settings_));
  ON_CALL(app_mngr_settings_, default_timeout())
      .WillByDefault(ReturnRef(kDefaultTimeout));

  EXPECT_CALL(*mock_message_helper_, HMIFunctionIDFromString(" "))
      .WillOnce(Return(hmi_apis::FunctionID::INVALID_ENUM));
  EXPECT_CALL(app_mngr_, GetResetTimeoutHandler())
      .WillOnce(ReturnRef(*reset_timeout_handler_));
  EXPECT_CALL(app_mngr_, IsUpdateRequestTimeoutRequired(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _)).Times(0);

  ASSERT_TRUE(command->Init());
  command->Run();
  reset_timeout_handler_->on_event(event);
}
}
}
}
