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

#include "hmi/vr_get_language_response.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/smart_object_keys.h"
#include "gtest/gtest.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace vr_get_language_response {

using sdl_rpc_plugin::commands::VRGetLanguageResponse;
using test::components::event_engine_test::MockEventDispatcher;
using testing::_;
using ::testing::NiceMock;
using testing::ReturnRef;

namespace strings = application_manager::strings;
namespace hmi_response = application_manager::hmi_response;
namespace hmi_interface = application_manager::hmi_interface;
using namespace hmi_apis;

typedef NiceMock<
    ::test::components::application_manager_test::MockHMICapabilities>
    MockHMICapabilities;

namespace {
const hmi_apis::Common_Language::eType kLanguage = Common_Language::EN_GB;
const hmi_apis::Common_Result::eType kSuccess =
    hmi_apis::Common_Result::SUCCESS;
}  // namespace

class VRGetLanguageResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(VRGetLanguageResponseTest, Run_LanguageSet_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::msg_params][hmi_response::language] = kLanguage;
  (*msg)[strings::params][hmi_response::code] = kSuccess;

  std::shared_ptr<VRGetLanguageResponse> command(
      CreateCommand<VRGetLanguageResponse>(msg));

  EXPECT_CALL(mock_hmi_capabilities_, set_active_vr_language(kLanguage));

  EXPECT_CALL(mock_hmi_capabilities_,
              SaveCachedCapabilitiesToFile(hmi_interface::vr, _, _));

  MockEventDispatcher mock_event_dispatcher;
  EXPECT_CALL(app_mngr_, event_dispatcher())
      .WillOnce(ReturnRef(mock_event_dispatcher));
  EXPECT_CALL(mock_event_dispatcher, raise_event(_));
  ASSERT_TRUE(command->Init());

  command->Run();
}

TEST_F(VRGetLanguageResponseTest, Run_LanguageNotSet_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][hmi_response::code] = kSuccess;

  std::shared_ptr<VRGetLanguageResponse> command(
      CreateCommand<VRGetLanguageResponse>(msg));

  EXPECT_CALL(mock_hmi_capabilities_,
              set_active_vr_language(Common_Language::INVALID_ENUM));

  EXPECT_CALL(mock_hmi_capabilities_,
              SaveCachedCapabilitiesToFile(hmi_interface::vr, _, _));

  MockEventDispatcher mock_event_dispatcher;
  EXPECT_CALL(app_mngr_, event_dispatcher())
      .WillOnce(ReturnRef(mock_event_dispatcher));
  EXPECT_CALL(mock_event_dispatcher, raise_event(_));
  ASSERT_TRUE(command->Init());

  command->Run();
}

TEST_F(VRGetLanguageResponseTest,
       onTimeOut_Run_ResponseForInterface_ReceivedError) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::ABORTED;

  std::shared_ptr<VRGetLanguageResponse> command(
      CreateCommand<VRGetLanguageResponse>(msg));

  EXPECT_CALL(mock_hmi_capabilities_,
              OnCapabilityInitialized(hmi_apis::FunctionID::VR_GetLanguage));
  ASSERT_TRUE(command->Init());

  command->Run();
}

}  // namespace vr_get_language_response
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
