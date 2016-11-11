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
#include "application_manager/commands/mobile/on_hash_change_notification.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/test/include/application_manager/commands/commands_test.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_hash_change_notification {

namespace strings = application_manager::strings;

using application_manager::commands::mobile::OnHashChangeNotification;
using application_manager::MockMessageHelper;
using application_manager::commands::CommandImpl;
using testing::Mock;
using testing::Return;
using testing::ReturnRef;
using testing::_;

class OnHashChangeNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  OnHashChangeNotificationTest()
      : message_helper_(*MockMessageHelper::message_helper_mock()) {}
  void SetUp() OVERRIDE {
    Mock::VerifyAndClearExpectations(&message_helper_);
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&message_helper_);
  }
  MockMessageHelper& message_helper_;
};

TEST_F(OnHashChangeNotificationTest, Run_ValidApp_SUCCESS) {
  const uint32_t kConnectionKey = 1u;
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::connection_key] = kConnectionKey;

  SharedPtr<OnHashChangeNotification> command =
      CreateCommand<OnHashChangeNotification>(msg);

  std::string return_string = "1234";
  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, curHash()).WillOnce(ReturnRef(return_string));
  EXPECT_CALL(message_helper_, PrintSmartObject(_)).WillOnce(Return(false));
  EXPECT_CALL(app_mngr_, SendMessageToMobile(msg, _));

  command->Run();

  ASSERT_EQ(application_manager::MessageType::kNotification,
            (*msg)[strings::params][strings::message_type].asInt());
  ASSERT_EQ(CommandImpl::mobile_protocol_type_,
            (*msg)[strings::params][strings::protocol_type].asInt());
  ASSERT_EQ(CommandImpl::protocol_version_,
            (*msg)[strings::params][strings::protocol_version].asInt());
  ASSERT_EQ(return_string,
            (*msg)[strings::msg_params][strings::hash_id].asString());
}

TEST_F(OnHashChangeNotificationTest, Run_InvalidApp_NoNotification) {
  const uint32_t kConnectionKey = 1u;
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::connection_key] = kConnectionKey;

  SharedPtr<OnHashChangeNotification> command =
      CreateCommand<OnHashChangeNotification>(msg);

  std::string return_string;
  MockAppPtr mock_app = CreateMockApp();

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(*mock_app, curHash()).Times(0);
  EXPECT_CALL(message_helper_, PrintSmartObject(_)).Times(0);
  EXPECT_CALL(app_mngr_, SendMessageToMobile(msg, _)).Times(0);

  command->Run();

  ASSERT_EQ(application_manager::MessageType::kNotification,
            (*msg)[strings::params][strings::message_type].asInt());
}

}  // namespace on_hash_change_notification
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
