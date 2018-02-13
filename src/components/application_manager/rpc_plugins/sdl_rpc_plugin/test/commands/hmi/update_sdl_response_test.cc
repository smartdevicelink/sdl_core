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
#include "application_manager/commands/commands_test.h"
#include "application_manager/application.h"
#include "hmi/update_sdl_response.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace update_sdl_response {

using ::utils::SharedPtr;
namespace am = ::application_manager;
namespace strings = ::application_manager::strings;
using sdl_rpc_plugin::commands::UpdateSDLResponse;
using am::commands::CommandImpl;

typedef SharedPtr<UpdateSDLResponse> UpdateSDLResponsePtr;

namespace {
const uint32_t kConnectionKey = 2u;
const std::string kStrNumber = "123";
}  // namespace

class UpdateSDLResponseTest : public CommandsTest<CommandsTestMocks::kIsNice> {
};

TEST_F(UpdateSDLResponseTest, RUN_SendRequest_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[strings::msg_params][strings::number] = kStrNumber;
  (*command_msg)[strings::params][strings::connection_key] = kConnectionKey;

  UpdateSDLResponsePtr command(CreateCommand<UpdateSDLResponse>(command_msg));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(rpc_service_, SendMessageToHMI(command_msg));

  command->Run();

  EXPECT_EQ((*command_msg)[strings::params][strings::protocol_type].asInt(),
            CommandImpl::hmi_protocol_type_);
  EXPECT_EQ((*command_msg)[strings::params][strings::protocol_version].asInt(),
            CommandImpl::protocol_version_);
}

}  // namespace update_sdl_response
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
