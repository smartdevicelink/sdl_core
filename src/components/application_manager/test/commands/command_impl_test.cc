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
#include <string>
#include <algorithm>
#include <functional>
#include <set>

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/application_manager.h"
#include "application_manager/mock_application.h"

namespace test {
namespace components {
namespace commands_test {
namespace command_impl {

using ::testing::Return;
using ::testing::AtLeast;
using ::testing::_;

using ::utils::SharedPtr;
namespace strings = ::application_manager::strings;
using ::application_manager::commands::CommandImpl;
using ::application_manager::ApplicationManager;
using ::application_manager::commands::MessageSharedPtr;
using ::application_manager::ApplicationSharedPtr;
using ::test::components::application_manager_test::MockApplication;

typedef SharedPtr<MockApplication> MockAppPtr;

namespace {
const uint32_t kDefaultMsgCount = 5u;
const uint32_t kAppId1 = 5u;
const uint32_t kAppId2 = 10u;
}  // namespace

void ExpectEqualAppId(const smart_objects::SmartObject& obj) {
  EXPECT_EQ(kAppId2, obj[strings::app_id].asUInt());
}

void ExpectEqualKeyAppId(const std::string obj, MessageSharedPtr msg) {
  EXPECT_EQ(kAppId2, (*msg)[obj][strings::app_id].asUInt());
}

class CommandImplTest : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  class UnwrappedCommandImpl : CommandImpl {
   public:
    using CommandImpl::ReplaceMobileByHMIAppId;
    using CommandImpl::ReplaceHMIByMobileAppId;

    UnwrappedCommandImpl(const MessageSharedPtr& message,
                         ApplicationManager& application_manager)
        : CommandImpl(message, application_manager) {}
  };

  // Create `SmartObject` which handle array of `SmartObjects`
  static MessageSharedPtr CreateArrayMessage(
      const size_t msg_count = kDefaultMsgCount) {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Array);
    smart_objects::SmartArray* array = msg->asArray();
    for (size_t i = 0u; i < msg_count; ++i) {
      SmartObject obj;
      obj[strings::app_id] = i;
      array->push_back(obj);
    }
    return msg;
  }
  // Create `SmartObject` which handle map of `SmartObjects`
  static MessageSharedPtr CreateMapMessage(
      const size_t msg_count = kDefaultMsgCount) {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    char key[] = {'A', '\0'};
    for (size_t i = 0u; i < msg_count; ++i, ++key[0]) {
      SmartObject obj;
      obj[strings::app_id] = i;
      (*msg)[key] = obj;
    }
    return msg;
  }
};

typedef CommandImplTest::UnwrappedCommandImpl UCommandImpl;
typedef SharedPtr<UCommandImpl> UCommandImplPtr;

TEST_F(CommandImplTest, GetMethods_SUCCESS) {
  MessageSharedPtr msg;
  SharedPtr<CommandImpl> command =
      CreateCommand<CommandImpl>(kDefaultTimeout_, msg);

  // Current implementation always return `true`
  EXPECT_TRUE(command->Init());
  EXPECT_TRUE(command->CheckPermissions());
  EXPECT_TRUE(command->CleanUp());

  // Default value of `allowed_to_terminate_` is true
  EXPECT_TRUE(command->AllowedToTerminate());
  command->SetAllowedToTerminate(false);
  EXPECT_FALSE(command->AllowedToTerminate());

  const uint32_t kCorrelationId = 3u;
  const int32_t kFunctionId = 4u;
  const uint32_t kConnectionKey = 5u;

  (*msg)[strings::params][strings::correlation_id] = kCorrelationId;
  (*msg)[strings::params][strings::function_id] = kFunctionId;
  (*msg)[strings::params][strings::connection_key] = kConnectionKey;

  EXPECT_EQ(kDefaultTimeout_, command->default_timeout());
  EXPECT_EQ(kCorrelationId, command->correlation_id());
  EXPECT_EQ(kConnectionKey, command->connection_key());
  EXPECT_EQ(kFunctionId, command->function_id());
  EXPECT_NO_THROW(command->Run());
  EXPECT_NO_THROW(command->onTimeOut());
}

TEST_F(CommandImplTest, ReplaceMobileByHMIAppId_NoAppIdInMessage_UNSUCCESS) {
  MessageSharedPtr msg;
  UCommandImplPtr command = CreateCommand<UCommandImpl>(msg);

  EXPECT_CALL(app_mngr_, application(_)).Times(0);

  command->ReplaceMobileByHMIAppId(*msg);
}

TEST_F(CommandImplTest, ReplaceMobileByHMIAppId_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::app_id] = kAppId1;

  UCommandImplPtr command = CreateCommand<UCommandImpl>(msg);

  MockAppPtr app = CreateMockApp();

  EXPECT_CALL(app_mngr_, application(kAppId1)).WillOnce(Return(app));
  ON_CALL(*app, hmi_app_id()).WillByDefault(Return(kAppId2));

  command->ReplaceMobileByHMIAppId(*msg);

  EXPECT_EQ(kAppId2, (*msg)[strings::app_id].asUInt());
}

TEST_F(CommandImplTest, ReplaceMobileByHMIAppId_Array_SUCCESS) {
  MessageSharedPtr msg = CreateArrayMessage(kDefaultMsgCount);
  UCommandImplPtr command = CreateCommand<UCommandImpl>(msg);

  MockAppPtr app = CreateMockApp();

  EXPECT_CALL(app_mngr_, application(_))
      .Times(kDefaultMsgCount)
      .WillRepeatedly(Return(app));
  ON_CALL(*app, hmi_app_id()).WillByDefault(Return(kAppId2));

  command->ReplaceMobileByHMIAppId(*msg);

  EXPECT_TRUE(msg->asArray());
  std::for_each(
      msg->asArray()->begin(), msg->asArray()->end(), ExpectEqualAppId);
}

TEST_F(CommandImplTest, ReplaceMobileByHMIAppId_Map_SUCCESS) {
  MessageSharedPtr msg = CreateMapMessage(kDefaultMsgCount);
  UCommandImplPtr command = CreateCommand<UCommandImpl>(msg);

  MockAppPtr app = CreateMockApp();

  EXPECT_CALL(app_mngr_, application(_))
      .Times(kDefaultMsgCount)
      .WillRepeatedly(Return(app));
  ON_CALL(*app, hmi_app_id()).WillByDefault(Return(kAppId2));

  command->ReplaceMobileByHMIAppId(*msg);

  std::set<std::string> keys(msg->enumerate());
  std::for_each(keys.begin(),
                keys.end(),
                std::bind2nd(std::ptr_fun(&ExpectEqualKeyAppId), msg));
}

TEST_F(CommandImplTest, ReplaceHMIByMobileAppId_NoHMIAppIdInMessage_UNSUCCESS) {
  MessageSharedPtr msg;
  UCommandImplPtr command = CreateCommand<UCommandImpl>(msg);

  EXPECT_CALL(app_mngr_, application_by_hmi_app(_)).Times(0);

  command->ReplaceHMIByMobileAppId(*msg);
}

TEST_F(CommandImplTest, ReplaceHMIByMobileAppId_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::app_id] = kAppId1;

  UCommandImplPtr command = CreateCommand<UCommandImpl>(msg);

  MockAppPtr app = CreateMockApp();

  EXPECT_CALL(app_mngr_, application_by_hmi_app(kAppId1)).WillOnce(Return(app));
  ON_CALL(*app, app_id()).WillByDefault(Return(kAppId2));

  command->ReplaceHMIByMobileAppId(*msg);

  EXPECT_EQ(kAppId2, (*msg)[strings::app_id].asUInt());
}

TEST_F(CommandImplTest, ReplaceHMIByMobileAppId_Array_SUCCESS) {
  MessageSharedPtr msg = CreateArrayMessage(kDefaultMsgCount);

  UCommandImplPtr command = CreateCommand<UCommandImpl>(msg);
  MockAppPtr app = CreateMockApp();

  EXPECT_CALL(app_mngr_, application_by_hmi_app(_))
      .Times(kDefaultMsgCount)
      .WillRepeatedly(Return(app));
  ON_CALL(*app, app_id()).WillByDefault(Return(kAppId2));

  command->ReplaceHMIByMobileAppId(*msg);

  EXPECT_TRUE(msg->asArray());
  std::for_each(
      msg->asArray()->begin(), msg->asArray()->end(), ExpectEqualAppId);
}

TEST_F(CommandImplTest, ReplaceHMIByMobileAppId_Map_SUCCESS) {
  MessageSharedPtr msg = CreateMapMessage(kDefaultMsgCount);

  UCommandImplPtr command = CreateCommand<UCommandImpl>(msg);
  MockAppPtr app = CreateMockApp();

  EXPECT_CALL(app_mngr_, application_by_hmi_app(_))
      .Times(kDefaultMsgCount)
      .WillRepeatedly(Return(app));
  ON_CALL(*app, app_id()).WillByDefault(Return(kAppId2));

  command->ReplaceHMIByMobileAppId(*msg);

  std::set<std::string> keys = msg->enumerate();
  std::for_each(keys.begin(),
                keys.end(),
                std::bind2nd(std::ptr_fun(&ExpectEqualKeyAppId), msg));
}

}  // namespace command_impl
}  // namespace commands_test
}  // namespace components
}  // namespace test
