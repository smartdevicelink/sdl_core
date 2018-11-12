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
#include <string>

#include "gtest/gtest.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "utils/lock.h"
#include "utils/data_accessor.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "mobile/on_seek_media_clock_timer_notification.h"
#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_seek_media_clock_timer_notification {

using ::testing::_;
using ::testing::Return;
using ::testing::Eq;
namespace am = ::application_manager;
using am::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::mobile::OnSeekMediaClockTimerNotification;

using namespace am::commands;

const uint32_t kConnectionKey = 2u;
const uint32_t kCorrelationId = 2u;
const uint32_t kAppID = 2u;
const uint32_t kHours = 2u;
const uint32_t kMinutes = 26u;
const uint32_t kSeconds = 1u;

typedef std::shared_ptr<OnSeekMediaClockTimerNotification> NotificationPtr;

class OnSeekMediaClockTimerNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  OnSeekMediaClockTimerNotificationTest() {
    CreateBasicMessage();
    mock_app = CreateMockApp();
  }

 public:
  MessageSharedPtr commands_msg;
  MockAppPtr mock_app;

 private:
  void CreateBasicMessage() {
    commands_msg = CreateMessage(smart_objects::SmartType_Map);
    (*commands_msg)[am::strings::msg_params][am::strings::seekTime]
                   [am::strings::hours] = kHours;
    (*commands_msg)[am::strings::msg_params][am::strings::seekTime]
                   [am::strings::minutes] = kMinutes;
    (*commands_msg)[am::strings::msg_params][am::strings::seekTime]
                   [am::strings::seconds] = kSeconds;
    (*commands_msg)[am::strings::msg_params][am::strings::app_id] = kAppID;
  }
};

TEST_F(OnSeekMediaClockTimerNotificationTest,
       Run_SendNotificationToMobile_SUCCESS) {
  NotificationPtr command(
      CreateCommand<OnSeekMediaClockTimerNotification>(commands_msg));

  ON_CALL(app_mngr_, application(kAppID)).WillByDefault(Return(mock_app));
  ON_CALL(*mock_app, enable_seek()).WillByDefault(Return(true));

  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(commands_msg, _));

  command->Run();
}

}  // on_seek_media_clock_timer_notification
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
