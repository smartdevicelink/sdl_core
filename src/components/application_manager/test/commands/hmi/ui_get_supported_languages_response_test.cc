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

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/application.h"
#include "commands/commands_test.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/commands/hmi/ui_get_supported_languages_response.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace ui_get_supported_languages_response {

using ::testing::Return;
using ::utils::SharedPtr;
using ::testing::NiceMock;
namespace am = ::application_manager;
namespace strings = ::application_manager::strings;
namespace hmi_response = am::hmi_response;
using am::commands::UIGetSupportedLanguagesResponse;

typedef SharedPtr<UIGetSupportedLanguagesResponse>
    UIGetSupportedLanguagesResponsePtr;
typedef NiceMock<
    ::test::components::application_manager_test::MockHMICapabilities>
    MockHMICapabilities;

namespace {
const uint32_t kConnectionKey = 2u;
const std::string kStringNum = "123";
const std::string kLanguage = "EN_US";
const smart_objects::SmartObject supported_languages(kLanguage);
}  // namespace

class UIGetSupportedLanguagesResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  MockHMICapabilities mock_hmi_capabilities_;
  SmartObject capabilities_;
};

TEST_F(UIGetSupportedLanguagesResponseTest, RUN_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[strings::msg_params][strings::number] = kStringNum;
  (*command_msg)[strings::params][strings::connection_key] = kConnectionKey;
  (*command_msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*command_msg)[strings::msg_params][hmi_response::capabilities] =
      (capabilities_);
  (*command_msg)[strings::msg_params][hmi_response::languages] =
      supported_languages;

  UIGetSupportedLanguagesResponsePtr command(
      CreateCommand<UIGetSupportedLanguagesResponse>(command_msg));

  EXPECT_CALL(app_mngr_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities_));

  EXPECT_CALL(mock_hmi_capabilities_,
              set_ui_supported_languages((supported_languages)));

  command->Run();
}
TEST_F(UIGetSupportedLanguagesResponseTest, RUN_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[strings::msg_params][strings::number] = kStringNum;
  (*command_msg)[strings::params][strings::connection_key] = kConnectionKey;
  (*command_msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::WRONG_LANGUAGE;
  (*command_msg)[strings::msg_params][hmi_response::capabilities] =
      (capabilities_);

  UIGetSupportedLanguagesResponsePtr command(
      CreateCommand<UIGetSupportedLanguagesResponse>(command_msg));

  EXPECT_CALL(mock_hmi_capabilities_,
              set_ui_supported_languages(supported_languages)).Times(0);

  command->Run();

  EXPECT_FALSE((*command_msg)[am::strings::msg_params].keyExists(
      am::hmi_response::languages));
}

}  // namespace ui_get_supported_languages_response
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
