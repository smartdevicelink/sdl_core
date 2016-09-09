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

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"

#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"

#include "command_impl.h"
#include "commands/commands_test.h"
#include "mobile/on_button_event_notification.h"

#include "interfaces/MOBILE_API.h"
#include "application_manager/application.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {

namespace am = ::application_manager;
namespace commands = am::commands;
namespace mobile_result = mobile_apis::Result;

using ::testing::_;
using ::testing::Return;
using namespace mobile_apis;
using am::commands::MessageSharedPtr;
using am::commands::mobile::OnButtonEventNotification;

typedef ::utils::SharedPtr<OnButtonEventNotification> NotificationPtr;

namespace {
const int kAppId1 = 10;
const int kAppId2 = 8;
const bool kFullScreen = true;
const bool kAppIdValid = true;
const bool kSendMessageExpected = true;
}

class OnButtonEventNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 protected:
  OnButtonEventNotificationTest()
      : command_(CreateCommand<OnButtonEventNotification>()) {}

  void CheckExpectations(const bool is_app_id_valid,
                         const HMILevel::eType app_hmi_level,
                         const size_t app_id1,
                         const size_t app_id2,
                         const bool is_full_screen = false,
                         const bool is_send_message_expected = false) {
    MockAppPtr mock_app(CreateMockApp());
    MockAppPtr subscribed(CreateMockApp());
    std::vector<ApplicationSharedPtr> subscribedApps;
    subscribedApps.push_back(subscribed);

    if (is_app_id_valid) {
      EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(mock_app));
    } else {
      EXPECT_CALL(mock_app_manager_, application(_))
          .WillOnce(Return(ApplicationSharedPtr()));
    }
    EXPECT_CALL(mock_app_manager_, applications_by_button(_))
        .WillOnce(Return(subscribedApps));
    EXPECT_CALL(*subscribed, hmi_level()).WillRepeatedly(Return(app_hmi_level));
    EXPECT_CALL(*subscribed, app_id()).WillRepeatedly(Return(app_id1));
    EXPECT_CALL(*mock_app, app_id()).WillRepeatedly(Return(app_id2));
    if (is_full_screen) {
      EXPECT_CALL(*subscribed, IsFullscreen()).WillOnce(Return(true));
    }
    if (is_send_message_expected) {
      EXPECT_CALL(mock_app_manager_, SendMessageToMobile(_, _));
    }

    command_->Run();
  }

  NotificationPtr command_;
};

TEST_F(OnButtonEventNotificationTest,
       DISABLED_Run_SendOnButtonEvent_in_FULL_Case1_SUCCESS) {
  CheckExpectations(!kAppIdValid,
                    HMILevel::HMI_FULL,
                    kAppId1,
                    kAppId2,
                    kFullScreen,
                    kSendMessageExpected);
}

TEST_F(OnButtonEventNotificationTest,
       DISABLED_Run_SendOnButtonEvent_in_FULL_Case2_SUCCESS) {
  CheckExpectations(kAppIdValid,
                    HMILevel::HMI_FULL,
                    kAppId1,
                    kAppId1,
                    !kFullScreen,
                    kSendMessageExpected);
}

TEST_F(OnButtonEventNotificationTest,
       DISABLED_Run_SendOnButtonEvent_in_FULL_UNSUCCESS) {
  CheckExpectations(kAppIdValid, HMILevel::HMI_FULL, kAppId1, kAppId2);
}

TEST_F(OnButtonEventNotificationTest,
       DISABLED_Run_SendOnButtonEvent_in_LIMITED_SUCCESS) {
  CheckExpectations(kAppIdValid,
                    HMILevel::HMI_LIMITED,
                    kAppId1,
                    kAppId1,
                    !kFullScreen,
                    kSendMessageExpected);
}

TEST_F(OnButtonEventNotificationTest,
       DISABLED_Run_SendOnButtonEvent_in_NONE_UNSUCCESS) {
  CheckExpectations(kAppIdValid, HMILevel::HMI_NONE, kAppId1, kAppId1);
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
