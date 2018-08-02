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

#include <stdint.h>
#include <set>

#include "gtest/gtest.h"
#include "mobile/on_way_point_change_notification.h"

#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_impl.h"
#include "utils/helpers.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_way_point_change_notification {

namespace am = ::application_manager;

using ::testing::_;
using ::testing::Return;

using am::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::OnWayPointChangeNotification;

typedef std::shared_ptr<OnWayPointChangeNotification> NotificationPtr;

namespace {
const uint32_t kApp1Id = 1u;
const uint32_t kApp2Id = 2u;
}  // namespace

class OnWayPointChangeNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  OnWayPointChangeNotificationTest() {}

  void SetUp() OVERRIDE {
    command_ = CreateCommand();
  }

  std::shared_ptr<OnWayPointChangeNotification> CreateCommand() {
    InitCommand(CommandsTest<CommandsTestMocks::kIsNice>::kDefaultTimeout_);
    message_ = CreateMessage();
    return std::make_shared<OnWayPointChangeNotification>(
        message_,
        app_mngr_,
        mock_rpc_service_,
        mock_hmi_capabilities_,
        mock_policy_handler_);
  }

  MessageSharedPtr CreateMessage(
      const smart_objects::SmartType type = smart_objects::SmartType_Null) {
    return std::make_shared<SmartObject>(type);
  }

  NotificationPtr command_;
  MessageSharedPtr message_;
};

MATCHER_P(CheckMessageData, appID, "") {
  const bool kIsMobileProtocolTypeCorrect =
      (*arg)[am::strings::params][am::strings::protocol_type].asInt() ==
      am::commands::CommandImpl::mobile_protocol_type_;

  const bool kIsProtocolVersionCorrect =
      (*arg)[am::strings::params][am::strings::protocol_version].asInt() ==
      am::commands::CommandImpl::protocol_version_;

  const bool kIsNotificationCorrect =
      (*arg)[am::strings::params][am::strings::message_type].asInt() ==
      am::MessageType::kNotification;

  const bool kIsConnectionKeyCorrect =
      (*arg)[am::strings::params][am::strings::connection_key].asUInt() ==
      appID;

  using namespace helpers;
  return Compare<bool, EQ, ALL>(true,
                                kIsMobileProtocolTypeCorrect,
                                kIsProtocolVersionCorrect,
                                kIsNotificationCorrect,
                                kIsConnectionKeyCorrect);
}

TEST_F(OnWayPointChangeNotificationTest,
       Run_NotEmptyListOfAppsSubscribedForWayPoints_SUCCESS) {
  std::set<uint32_t> apps_subscribed_for_way_points;
  apps_subscribed_for_way_points.insert(kApp1Id);

  EXPECT_CALL(app_mngr_, GetAppsSubscribedForWayPoints())
      .WillOnce(Return(apps_subscribed_for_way_points));
  EXPECT_CALL(mock_rpc_service_,
              SendMessageToMobile(CheckMessageData(kApp1Id), _));

  command_->Run();
}

TEST_F(OnWayPointChangeNotificationTest,
       Run_StoreWayPointsDuringNotification_SUCCESS) {
  std::set<uint32_t> apps_subscribed_for_way_points;
  apps_subscribed_for_way_points.insert(kApp1Id);

  EXPECT_CALL(app_mngr_, GetAppsSubscribedForWayPoints())
      .WillOnce(Return(apps_subscribed_for_way_points));
  EXPECT_CALL(mock_rpc_service_,
              SendMessageToMobile(CheckMessageData(kApp1Id), _));
  EXPECT_CALL(app_mngr_, SaveWayPointsMessage(message_));

  command_->Run();
}

TEST_F(OnWayPointChangeNotificationTest,
       Run_BroadcastingWayPointsToAllApps_SUCCESS) {
  std::set<uint32_t> apps_subscribed_for_way_points;
  apps_subscribed_for_way_points.insert(kApp1Id);
  apps_subscribed_for_way_points.insert(kApp2Id);

  EXPECT_CALL(app_mngr_, GetAppsSubscribedForWayPoints())
      .WillOnce(Return(apps_subscribed_for_way_points));
  EXPECT_CALL(mock_rpc_service_,
              SendMessageToMobile(CheckMessageData(kApp1Id), _));
  EXPECT_CALL(mock_rpc_service_,
              SendMessageToMobile(CheckMessageData(kApp2Id), _));

  command_->Run();
}

}  // namespace on_way_point_change_notification
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
