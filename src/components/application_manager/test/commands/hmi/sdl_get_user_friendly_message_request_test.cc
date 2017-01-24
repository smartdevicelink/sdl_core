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
#include <vector>

#include "gtest/gtest.h"
#include "application_manager/commands/hmi/sdl_get_user_friendly_message_request.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_state_controller.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "smart_objects/smart_object.h"
#include "commands/command_request_test.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace sdl_get_user_friendly_message_request {

using application_manager::commands::MessageSharedPtr;
using application_manager::commands::SDLGetUserFriendlyMessageRequest;
using application_manager::MockMessageHelper;
using test::components::policy_test::MockPolicyHandlerInterface;
using test::components::application_manager_test::MockHMICapabilities;
using testing::_;
using testing::Return;
using testing::ReturnRef;

namespace {
const uint32_t kCorrelationID = 1u;
const uint32_t kAppID = 2u;
const std::string kLanguageDe = "de-de";
const std::string kLanguageEn = "en-gb";
const std::string kMessageCodes = "messageCodes";
const hmi_apis::Common_Language::eType kLanguage =
    hmi_apis::Common_Language::EN_GB;
}  // namespace

namespace strings = ::application_manager::strings;

class SDLGetUserFriendlyMessageRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SDLGetUserFriendlyMessageRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock()) {}

 protected:
  void SetUp() OVERRIDE {
    mock_app_ = CreateMockApp();
  }

  void InitCommand(const uint32_t& timeout) OVERRIDE {
    CommandRequestTest<CommandsTestMocks::kIsNice>::InitCommand(timeout);
    ON_CALL((*mock_app_), app_id()).WillByDefault(Return(kAppID));
    EXPECT_CALL(app_mngr_, application_by_hmi_app(kAppID))
        .WillOnce(Return(mock_app_));
  }
  MockAppPtr mock_app_;
  MockPolicyHandlerInterface mock_policy_handler_;
  MockMessageHelper& mock_message_helper_;
};

TEST_F(SDLGetUserFriendlyMessageRequestTest, Run_LanguageSet_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::correlation_id] = kCorrelationID;
  (*msg)[strings::msg_params][strings::app_id] = kAppID;

  (*msg)[strings::msg_params][kMessageCodes] =
      SmartObject(smart_objects::SmartType_Array);
  (*msg)[strings::msg_params][kMessageCodes][0] = SmartObject(kLanguageDe);
  (*msg)[strings::msg_params][kMessageCodes][1] = SmartObject(kLanguageEn);

  (*msg)[strings::msg_params][strings::language] = kLanguage;

  SharedPtr<SDLGetUserFriendlyMessageRequest> command(
      CreateCommand<SDLGetUserFriendlyMessageRequest>(msg));

  EXPECT_CALL(mock_message_helper_, CommonLanguageToString(kLanguage))
      .WillOnce(Return(kLanguageEn));
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler_));
  std::vector<std::string> msg_codes;
  msg_codes.push_back(kLanguageDe);
  msg_codes.push_back(kLanguageEn);
  EXPECT_CALL(mock_policy_handler_,
              OnGetUserFriendlyMessage(msg_codes, kLanguageEn, kCorrelationID));

  command->Run();
}

TEST_F(SDLGetUserFriendlyMessageRequestTest, Run_LanguageNotSet_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::correlation_id] = kCorrelationID;
  (*msg)[strings::msg_params][strings::app_id] = kAppID;

  (*msg)[strings::msg_params][kMessageCodes] =
      SmartObject(smart_objects::SmartType_Array);
  (*msg)[strings::msg_params][kMessageCodes][0] = SmartObject(kLanguageDe);
  (*msg)[strings::msg_params][kMessageCodes][1] = SmartObject(kLanguageEn);

  SharedPtr<SDLGetUserFriendlyMessageRequest> command(
      CreateCommand<SDLGetUserFriendlyMessageRequest>(msg));

  MockHMICapabilities mock_hmi_capabilities;
  EXPECT_CALL(app_mngr_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities));
  EXPECT_CALL(mock_hmi_capabilities, active_ui_language())
      .WillOnce(Return(kLanguage));
  EXPECT_CALL(mock_message_helper_, CommonLanguageToString(kLanguage))
      .WillOnce(Return(kLanguageEn));
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler_));
  std::vector<std::string> msg_codes;
  msg_codes.push_back(kLanguageDe);
  msg_codes.push_back(kLanguageEn);
  EXPECT_CALL(mock_policy_handler_,
              OnGetUserFriendlyMessage(msg_codes, kLanguageEn, kCorrelationID));

  command->Run();
}

TEST_F(SDLGetUserFriendlyMessageRequestTest, Run_NoMsgCodes_Canceled) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::correlation_id] = kCorrelationID;
  (*msg)[strings::msg_params][strings::app_id] = kAppID;

  SharedPtr<SDLGetUserFriendlyMessageRequest> command(
      CreateCommand<SDLGetUserFriendlyMessageRequest>(msg));

  EXPECT_CALL(mock_message_helper_, CommonLanguageToString(_)).Times(0);
  EXPECT_CALL(app_mngr_, GetPolicyHandler()).Times(0);
  EXPECT_CALL(mock_policy_handler_, OnGetUserFriendlyMessage(_, _, _)).Times(0);

  command->Run();
}

}  // namespace sdl_get_user_friendly_message_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
