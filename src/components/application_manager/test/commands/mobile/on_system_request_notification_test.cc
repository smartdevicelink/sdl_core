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
#include "application_manager/commands/mobile/on_system_request_notification.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/test/include/application_manager/commands/commands_test.h"
#include "application_manager/commands/command_impl.h"
#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_system_request_notification {

namespace strings = application_manager::strings;

using application_manager::commands::mobile::OnSystemRequestNotification;
using application_manager::commands::CommandImpl;
using application_manager::MockMessageHelper;
using test::components::policy_test::MockPolicyHandlerInterface;
using namespace mobile_apis;
using testing::Mock;
using testing::Return;
using testing::ReturnRef;
using testing::_;

namespace {
const uint32_t kConnectionKey = 1u;
}  // namespace

class OnSystemRequestNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  OnSystemRequestNotificationTest()
      : message_helper_(*MockMessageHelper::message_helper_mock()) {}

  void SetUp() OVERRIDE {
    Mock::VerifyAndClearExpectations(&message_helper_);
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&message_helper_);
  }
  MockMessageHelper& message_helper_;
};

TEST_F(OnSystemRequestNotificationTest, Run_ProprietaryType_SUCCESS) {
  const RequestType::eType kRequestType = RequestType::PROPRIETARY;

  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::connection_key] = kConnectionKey;
  (*msg)[strings::msg_params][strings::request_type] = kRequestType;

  SharedPtr<OnSystemRequestNotification> command =
      CreateCommand<OnSystemRequestNotification>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  MockPolicyHandlerInterface mock_policy_handler;
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillRepeatedly(ReturnRef(mock_policy_handler));
  std::string policy_app_id;
  EXPECT_CALL(*mock_app, policy_app_id()).WillOnce(Return(policy_app_id));
  EXPECT_CALL(mock_policy_handler, IsRequestTypeAllowed(_, _))
      .WillOnce(Return(true));

#ifdef EXTENDED_POLICY
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .Times(2)
      .WillRepeatedly(ReturnRef(mock_policy_handler));
  EXPECT_CALL(mock_policy_handler, TimeoutExchange()).WillOnce(Return(5u));
#endif

  EXPECT_CALL(message_helper_, PrintSmartObject(_)).WillOnce(Return(false));
  EXPECT_CALL(app_mngr_, SendMessageToMobile(msg, _));

  command->Run();

  ASSERT_EQ(FileType::JSON,
            (*msg)[strings::msg_params][strings::file_type].asInt());
  ASSERT_EQ(application_manager::MessageType::kNotification,
            (*msg)[strings::params][strings::message_type].asInt());
  ASSERT_EQ(CommandImpl::mobile_protocol_type_,
            (*msg)[strings::params][strings::protocol_type].asInt());
  ASSERT_EQ(CommandImpl::protocol_version_,
            (*msg)[strings::params][strings::protocol_version].asInt());
}

TEST_F(OnSystemRequestNotificationTest, Run_HTTPType_SUCCESS) {
  const RequestType::eType kRequestType = RequestType::HTTP;

  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::connection_key] = kConnectionKey;
  (*msg)[strings::msg_params][strings::request_type] = kRequestType;

  SharedPtr<OnSystemRequestNotification> command =
      CreateCommand<OnSystemRequestNotification>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  MockPolicyHandlerInterface mock_policy_handler;
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler));
  std::string policy_app_id;
  EXPECT_CALL(*mock_app, policy_app_id()).WillOnce(Return(policy_app_id));
  EXPECT_CALL(mock_policy_handler, IsRequestTypeAllowed(_, _))
      .WillOnce(Return(true));

  EXPECT_CALL(message_helper_, PrintSmartObject(_)).WillOnce(Return(false));
  EXPECT_CALL(app_mngr_, SendMessageToMobile(msg, _));

  command->Run();

  ASSERT_EQ(FileType::BINARY,
            (*msg)[strings::msg_params][strings::file_type].asInt());
  ASSERT_EQ(application_manager::MessageType::kNotification,
            (*msg)[strings::params][strings::message_type].asInt());
  ASSERT_EQ(CommandImpl::mobile_protocol_type_,
            (*msg)[strings::params][strings::protocol_type].asInt());
  ASSERT_EQ(CommandImpl::protocol_version_,
            (*msg)[strings::params][strings::protocol_version].asInt());
}

TEST_F(OnSystemRequestNotificationTest, Run_InvalidApp_NoNotification) {
  const RequestType::eType kRequestType = RequestType::HTTP;

  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::connection_key] = kConnectionKey;
  (*msg)[strings::msg_params][strings::request_type] = kRequestType;

  SharedPtr<OnSystemRequestNotification> command =
      CreateCommand<OnSystemRequestNotification>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(app_mngr_, GetPolicyHandler()).Times(0);
  EXPECT_CALL(*mock_app, policy_app_id()).Times(0);
  MockPolicyHandlerInterface mock_policy_handler;
  EXPECT_CALL(mock_policy_handler, IsRequestTypeAllowed(_, _)).Times(0);

  EXPECT_CALL(message_helper_, PrintSmartObject(_)).Times(0);
  EXPECT_CALL(app_mngr_, SendMessageToMobile(msg, _)).Times(0);

  command->Run();
}

TEST_F(OnSystemRequestNotificationTest, Run_RequestNotAllowed_NoNotification) {
  const RequestType::eType kRequestType = RequestType::HTTP;

  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::connection_key] = kConnectionKey;
  (*msg)[strings::msg_params][strings::request_type] = kRequestType;

  SharedPtr<OnSystemRequestNotification> command =
      CreateCommand<OnSystemRequestNotification>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  MockPolicyHandlerInterface mock_policy_handler;
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler));
  std::string policy_app_id;
  EXPECT_CALL(*mock_app, policy_app_id()).WillOnce(Return(policy_app_id));
  EXPECT_CALL(mock_policy_handler, IsRequestTypeAllowed(_, _))
      .WillOnce(Return(false));

  EXPECT_CALL(message_helper_, PrintSmartObject(_)).Times(0);
  EXPECT_CALL(app_mngr_, SendMessageToMobile(msg, _)).Times(0);
  ;

  command->Run();
}

}  // namespace on_system_request_notification
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
