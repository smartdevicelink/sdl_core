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
#include <vector>

#include "gtest/gtest.h"
#include "application_manager/commands/mobile/on_tbt_client_state_notification.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_impl.h"
#include "utils/helpers.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_tbt_client_state_notification {

namespace am = ::application_manager;

using ::testing::_;
using ::testing::Return;

using am::commands::MessageSharedPtr;
using am::commands::OnTBTClientStateNotification;

typedef ::utils::SharedPtr<OnTBTClientStateNotification> NotificationPtr;

namespace {
const uint32_t kAppId = 1u;
}  // namespace

class OnTBTClientStateNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  OnTBTClientStateNotificationTest()
      : command_(CreateCommand<OnTBTClientStateNotification>()) {}

  NotificationPtr command_;
};

TEST_F(OnTBTClientStateNotificationTest, Run_HmiLevelNone_UNSUCCESS) {
  MockAppPtr mock_app(CreateMockApp());
  std::vector<ApplicationSharedPtr> applications_with_navi;
  applications_with_navi.push_back(mock_app);

  EXPECT_CALL(app_mngr_, applications_with_navi())
      .WillOnce(Return(applications_with_navi));

  EXPECT_CALL(*mock_app, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));

  EXPECT_CALL(*mock_app, app_id()).Times(0);

  EXPECT_CALL(app_mngr_, SendMessageToMobile(_, _)).Times(0);

  command_->Run();
}

MATCHER(CheckMessageData, "") {
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
      kAppId;

  using namespace helpers;
  return Compare<bool, EQ, ALL>(true,
                                kIsMobileProtocolTypeCorrect,
                                kIsProtocolVersionCorrect,
                                kIsNotificationCorrect,
                                kIsConnectionKeyCorrect);
}

TEST_F(OnTBTClientStateNotificationTest,
       Run_NotEmptyListOfAppsWithNavi_SUCCESS) {
  MockAppPtr mock_app(CreateMockApp());
  std::vector<ApplicationSharedPtr> applications_with_navi;
  applications_with_navi.push_back(mock_app);

  EXPECT_CALL(app_mngr_, applications_with_navi())
      .WillOnce(Return(applications_with_navi));

  EXPECT_CALL(*mock_app, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_FULL));

  EXPECT_CALL(*mock_app, app_id()).WillOnce(Return(kAppId));

  EXPECT_CALL(app_mngr_, SendMessageToMobile(CheckMessageData(), _));

  command_->Run();
}

}  // namespace on_tbt_client_state_notification
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
