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

#include "application_manager/application.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/response_from_hmi.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/smart_object_keys.h"
#include "gtest/gtest.h"
#include "hmi/tts_get_supported_languages_response.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace tts_get_supported_languages_response {

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
namespace am = ::application_manager;
namespace strings = ::application_manager::strings;
namespace hmi_response = am::hmi_response;
namespace hmi_interface = ::application_manager::hmi_interface;
using am::commands::CommandImpl;
using application_manager::commands::ResponseFromHMI;
using sdl_rpc_plugin::commands::TTSGetSupportedLanguagesResponse;

typedef std::shared_ptr<ResponseFromHMI> ResponseFromHMIPtr;
typedef NiceMock<
    ::test::components::application_manager_test::MockHMICapabilities>
    MockHMICapabilities;

namespace {
const uint32_t kConnectionKey = 2u;
}  // namespace

class TTSGetSupportedLanguageResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  SmartObject capabilities_;
};

TEST_F(TTSGetSupportedLanguageResponseTest, RUN_SUCCESS) {
  smart_objects::SmartObject supported_languages("EN_US");

  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[strings::msg_params][strings::number] = "123";
  (*command_msg)[strings::params][strings::connection_key] = kConnectionKey;
  (*command_msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*command_msg)[strings::msg_params][hmi_response::capabilities] =
      (capabilities_);
  (*command_msg)[strings::msg_params][hmi_response::languages] =
      supported_languages;

  ResponseFromHMIPtr command(
      CreateCommand<TTSGetSupportedLanguagesResponse>(command_msg));

  EXPECT_CALL(mock_hmi_capabilities_,
              set_tts_supported_languages((
                  *command_msg)[strings::msg_params][hmi_response::languages]));
  EXPECT_CALL(mock_hmi_capabilities_,
              SaveCachedCapabilitiesToFile(hmi_interface::tts, _, _));
  ASSERT_TRUE(command->Init());

  command->Run();
}
TEST_F(TTSGetSupportedLanguageResponseTest, RUN_UNSUCCESS) {
  smart_objects::SmartObject supported_languages("EN_US");

  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[strings::msg_params][strings::number] = "123";
  (*command_msg)[strings::params][strings::connection_key] = kConnectionKey;
  (*command_msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::WRONG_LANGUAGE;
  (*command_msg)[strings::msg_params][hmi_response::capabilities] =
      (capabilities_);

  ResponseFromHMIPtr command(
      CreateCommand<TTSGetSupportedLanguagesResponse>(command_msg));

  EXPECT_CALL(mock_hmi_capabilities_,
              set_tts_supported_languages(supported_languages))
      .Times(0);
  ASSERT_TRUE(command->Init());

  command->Run();

  EXPECT_FALSE((*command_msg)[am::strings::msg_params].keyExists(
      am::hmi_response::languages));
}

TEST_F(TTSGetSupportedLanguageResponseTest,
       onTimeOut_Run_ResponseForInterface_ReceivedError) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::ABORTED;

  ResponseFromHMIPtr command(
      CreateCommand<TTSGetSupportedLanguagesResponse>(command_msg));

  EXPECT_CALL(mock_hmi_capabilities_,
              UpdateRequestsRequiredForCapabilities(
                  hmi_apis::FunctionID::TTS_GetSupportedLanguages));
  ASSERT_TRUE(command->Init());

  command->Run();
}

}  // namespace tts_get_supported_languages_response
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
