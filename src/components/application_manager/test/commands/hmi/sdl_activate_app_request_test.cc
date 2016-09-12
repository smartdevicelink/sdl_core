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

#include "gtest/gtest.h"
#include "utils/lock.h"
#include "application_manager/commands/hmi/sdl_activate_app_request.h"
#include "application_manager/mock_application.h"
#include "application_manager/application_manager.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "commands/command_request_test.h"
#include "application_manager/mock_message_helper.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {

namespace am = ::application_manager;
namespace strings = am::strings;
namespace hmi_response = am::hmi_response;
using am::commands::MessageSharedPtr;
using am::commands::SDLActivateAppRequest;
using am::ApplicationSet;
using testing::Return;
using testing::ReturnRef;
using am::MockMessageHelper;
using policy_test::MockPolicyHandlerInterface;

namespace {
const uint32_t kCorrelationID = 1u;
const uint32_t kAppID = 2u;
const uint32_t kAppIDFirst = 1u;
const connection_handler::DeviceHandle kHandle = 2u;
}  // namespace

class SDLActivateAppRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  SDLActivateAppRequestTest()
      : message_helper_mock_(am::MockMessageHelper::message_helper_mock()) {}

  void InitCommand(const uint32_t& timeout) OVERRIDE {
    MockAppPtr mock_app = CreateMockApp();
    CommandRequestTest<CommandsTestMocks::kIsNice>::InitCommand(timeout);
    ON_CALL((*mock_app), app_id()).WillByDefault(Return(kAppID));
    EXPECT_CALL(mock_app_manager_, application_by_hmi_app(kAppID))
        .WillOnce(Return(mock_app));
  }

  ApplicationSet app_list_;
  ::sync_primitives::Lock lock_;
  am::MockMessageHelper* message_helper_mock_;
  policy_test::MockPolicyHandlerInterface policy_handler_;
};

TEST_F(SDLActivateAppRequestTest, FindAppToRegister_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::params][strings::correlation_id] = kCorrelationID;
  (*msg)[am::strings::msg_params][strings::app_id] = kAppID;

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(mock_app_manager_, application(kAppID))
      .WillOnce(Return(ApplicationSharedPtr()));
  EXPECT_CALL(mock_app_manager_, FindAppToRegister(kAppID))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));

  MockAppPtr mock_app_first(CreateMockApp());
  ON_CALL(*mock_app_first, device()).WillByDefault(Return(kHandle));
  ON_CALL(*mock_app_first, is_foreground()).WillByDefault(Return(false));

  app_list_.insert(mock_app_first);
  DataAccessor<ApplicationSet> accessor(app_list_, lock_);
  EXPECT_CALL(mock_app_manager_, applications()).WillOnce(Return(accessor));

  EXPECT_CALL(*mock_app_first, device()).WillOnce(Return(kHandle));
  EXPECT_CALL(*mock_app_first, is_foreground()).WillOnce(Return(false));

  EXPECT_CALL(*message_helper_mock_, SendLaunchApp(_, _, _, _));

  command->Run();
}

TEST_F(SDLActivateAppRequestTest, AppIdNotFound_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::params][strings::correlation_id] = kCorrelationID;
  (*msg)[am::strings::msg_params][strings::app_id] = kAppID;

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  EXPECT_CALL(mock_app_manager_, application(kAppID))
      .WillOnce(Return(ApplicationSharedPtr()));
  EXPECT_CALL(mock_app_manager_, FindAppToRegister(kAppID))
      .WillOnce(Return(ApplicationSharedPtr()));

  command->Run();
}

TEST_F(SDLActivateAppRequestTest, DevicesAppsEmpty_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::params][strings::correlation_id] = kCorrelationID;
  (*msg)[am::strings::msg_params][strings::app_id] = kAppID;

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(mock_app_manager_, application(kAppID))
      .WillByDefault(Return(mock_app));

  DataAccessor<ApplicationSet> accessor(app_list_, lock_);
  EXPECT_CALL(mock_app_manager_, applications()).WillOnce(Return(accessor));
  app_list_ = accessor.GetData();

  command->Run();
}

TEST_F(SDLActivateAppRequestTest, FirstAppActive_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::params][strings::correlation_id] = kCorrelationID;
  (*msg)[am::strings::msg_params][strings::app_id] = kAppID;

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(mock_app_manager_, application(kAppID))
      .WillByDefault(Return(mock_app));

  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));

  DataAccessor<ApplicationSet> accessor(app_list_, lock_);
  EXPECT_CALL(mock_app_manager_, applications()).WillOnce(Return(accessor));

  app_list_ = accessor.GetData();

  MockAppPtr mock_app_first(CreateMockApp());
  ON_CALL(mock_app_manager_, application(kAppIDFirst))
      .WillByDefault(Return(mock_app_first));

  app_list_.insert(mock_app_first);

  EXPECT_CALL(*mock_app_first, device()).WillOnce(Return(kHandle));
  EXPECT_CALL(*mock_app_first, is_foreground()).WillRepeatedly(Return(true));

  EXPECT_CALL(*message_helper_mock_, SendLaunchApp(_, _, _, _));

  command->Run();
}

TEST_F(SDLActivateAppRequestTest, FirstAppNotActive_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::params][strings::correlation_id] = kCorrelationID;
  (*msg)[am::strings::msg_params][strings::app_id] = kAppID;

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(mock_app_manager_, application(kAppID))
      .WillByDefault(Return(mock_app));

  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));

  MockAppPtr mock_app_first(CreateMockApp());
  ON_CALL(*mock_app_first, device()).WillByDefault(Return(kHandle));
  ON_CALL(*mock_app_first, is_foreground()).WillByDefault(Return(false));

  app_list_.insert(mock_app_first);
  DataAccessor<ApplicationSet> accessor(app_list_, lock_);
  EXPECT_CALL(mock_app_manager_, applications()).WillOnce(Return(accessor));

  EXPECT_CALL(*mock_app, IsRegistered()).WillOnce(Return(true));
  EXPECT_CALL(*mock_app_first, device()).WillOnce(Return(kHandle));
  EXPECT_CALL(*mock_app_first, is_foreground()).WillOnce(Return(false));

  EXPECT_CALL(mock_app_manager_, GetPolicyHandler())
      .WillOnce(ReturnRef(policy_handler_));
  EXPECT_CALL(policy_handler_, OnActivateApp(kAppID, kCorrelationID));

  command->Run();
}

TEST_F(SDLActivateAppRequestTest, FirstAppNotRegisteredAndEmpty_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::params][strings::correlation_id] = kCorrelationID;
  (*msg)[am::strings::msg_params][strings::app_id] = kAppID;

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(mock_app_manager_, application(kAppID))
      .WillByDefault(Return(mock_app));
  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));

  MockAppPtr mock_app_first(CreateMockApp());
  ON_CALL(*mock_app_first, device()).WillByDefault(Return(kHandle));
  ON_CALL(*mock_app_first, is_foreground()).WillByDefault(Return(false));

  app_list_.insert(mock_app_first);
  DataAccessor<ApplicationSet> accessor(app_list_, lock_);
  EXPECT_CALL(mock_app_manager_, applications()).WillOnce(Return(accessor));

  EXPECT_CALL(*mock_app_first, device()).WillOnce(Return(kHandle));
  EXPECT_CALL(*mock_app_first, is_foreground()).WillOnce(Return(false));

  EXPECT_CALL(*message_helper_mock_, SendLaunchApp(_, _, _, _));

  command->Run();
}

TEST_F(SDLActivateAppRequestTest, FirstAppNotRegistered_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::params][strings::correlation_id] = kCorrelationID;
  (*msg)[am::strings::msg_params][strings::app_id] = kAppID;

  SharedPtr<SDLActivateAppRequest> command(
      CreateCommand<SDLActivateAppRequest>(msg));

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(mock_app_manager_, application(kAppID))
      .WillByDefault(Return(mock_app));
  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));

  DataAccessor<ApplicationSet> accessor(app_list_, lock_);
  EXPECT_CALL(mock_app_manager_, applications()).WillOnce(Return(accessor));

  app_list_ = accessor.GetData();

  MockAppPtr mock_app_first(CreateMockApp());
  ON_CALL(*mock_app_first, device()).WillByDefault(Return(kHandle));
  ON_CALL(*mock_app_first, is_foreground()).WillByDefault(Return(false));

  app_list_.insert(mock_app_first);

  EXPECT_CALL(*mock_app_first, device()).WillOnce(Return(kHandle));
  EXPECT_CALL(*mock_app_first, is_foreground()).WillRepeatedly(Return(true));

  EXPECT_CALL(*message_helper_mock_, SendLaunchApp(_, _, _, _));

  command->Run();
}

}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
