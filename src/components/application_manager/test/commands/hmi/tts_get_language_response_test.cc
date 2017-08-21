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
#include "application_manager/commands/hmi/tts_get_language_response.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_event_dispatcher.h"
#include "commands/commands_test.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace tts_get_language_response {

using utils::SharedPtr;
using application_manager::commands::TTSGetLanguageResponse;
using test::components::application_manager_test::MockHMICapabilities;
using test::components::event_engine_test::MockEventDispatcher;
using testing::_;
using testing::ReturnRef;

namespace strings = application_manager::strings;
namespace hmi_response = application_manager::hmi_response;
using namespace hmi_apis;

namespace {
const Common_Language::eType kLanguage = Common_Language::EN_GB;
}  // namespace

class TTSGetLanguageResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(TTSGetLanguageResponseTest, Run_LanguageSet_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::msg_params][hmi_response::language] = kLanguage;

  SharedPtr<TTSGetLanguageResponse> command(
      CreateCommand<TTSGetLanguageResponse>(msg));

  MockHMICapabilities mock_hmi_capabilities;
  EXPECT_CALL(app_mngr_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities));
  EXPECT_CALL(mock_hmi_capabilities, set_active_tts_language(kLanguage));

  MockEventDispatcher mock_event_dispatcher;
  EXPECT_CALL(app_mngr_, event_dispatcher())
      .WillOnce(ReturnRef(mock_event_dispatcher));
  EXPECT_CALL(mock_event_dispatcher, raise_event(_));

  command->Run();
}

TEST_F(TTSGetLanguageResponseTest, Run_LanguageNotSet_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();

  SharedPtr<TTSGetLanguageResponse> command(
      CreateCommand<TTSGetLanguageResponse>(msg));

  MockHMICapabilities mock_hmi_capabilities;
  EXPECT_CALL(app_mngr_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities));
  EXPECT_CALL(mock_hmi_capabilities,
              set_active_tts_language(Common_Language::INVALID_ENUM));

  MockEventDispatcher mock_event_dispatcher;
  EXPECT_CALL(app_mngr_, event_dispatcher())
      .WillOnce(ReturnRef(mock_event_dispatcher));
  EXPECT_CALL(mock_event_dispatcher, raise_event(_));

  command->Run();
}

}  // namespace tts_get_language_response
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
