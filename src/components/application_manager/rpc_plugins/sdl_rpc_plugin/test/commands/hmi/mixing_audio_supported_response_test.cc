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
#include "application_manager/commands/command.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/commands/response_from_hmi.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "sdl_rpc_plugin/commands/hmi/mixing_audio_supported_response.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace mixing_audio_supported_response {

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;
using ::utils::SharedPtr;
namespace am = ::application_manager;
namespace strings = ::application_manager::strings;
using sdl_rpc_plugin::commands::MixingAudioSupportedResponse;
using application_manager::commands::ResponseFromHMI;
using am::commands::CommandImpl;
using am::HMICapabilities;
namespace hmi_response = ::application_manager::hmi_response;

typedef SharedPtr<ResponseFromHMI> ResponseFromHMIPtr;
typedef NiceMock<
    ::test::components::application_manager_test::MockHMICapabilities>
    MockHMICapabilities;

namespace {
const uint32_t kConnectionKey = 2u;
}  // namespace

class MixingAudioSupportedResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(MixingAudioSupportedResponseTest, RUN_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::number] = "123";
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  ResponseFromHMIPtr command(
      CreateCommand<MixingAudioSupportedResponse>(command_msg));

  const bool hmiResponse =
      (*command_msg)[strings::msg_params][hmi_response::attenuated_supported]
          .asBool();

  EXPECT_CALL(mock_hmi_capabilities_, set_attenuated_supported(hmiResponse));

  command->Run();
}

}  // namespace mixing_audio_supported_response
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
