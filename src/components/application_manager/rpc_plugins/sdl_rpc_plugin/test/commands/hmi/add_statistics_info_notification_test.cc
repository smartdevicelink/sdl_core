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
#include "hmi/add_statistics_info_notification.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace add_statistics_info_notification {

namespace am = ::application_manager;
namespace strings = ::application_manager::strings;
namespace hmi_notification = ::application_manager::hmi_notification;
using am::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::AddStatisticsInfoNotification;
using am::commands::CommandImpl;
using policy::PolicyHandler;
using policy_test::MockPolicyHandlerInterface;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

typedef ::utils::SharedPtr<AddStatisticsInfoNotification> NotificationPtr;

namespace {
const uint32_t kStatisticType = 1u;
}  // namespace

class AddStatisticsInfoNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 protected:
  MockPolicyHandlerInterface policy_handler_;
};

TEST_F(AddStatisticsInfoNotificationTest, Run_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::msg_params][am::hmi_notification::statistic_type] =
      kStatisticType;
  NotificationPtr command(CreateCommand<AddStatisticsInfoNotification>(msg));

  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(policy_handler_));
  EXPECT_CALL(policy_handler_, AddStatisticsInfo(kStatisticType));

  command->Run();
}

}  // namespace add_statistics_info_notification
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
