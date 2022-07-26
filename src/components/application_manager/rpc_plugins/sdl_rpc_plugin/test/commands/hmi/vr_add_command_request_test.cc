/*
 * Copyright (c) 2022, Ford Motor Company
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

#include "hmi/vr_add_command_request.h"

#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "application_manager/commands/command_request_test.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/request_to_hmi.h"
#include "application_manager/smart_object_keys.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace vr_add_command_request {

using ::testing::_;
using ::testing::Return;
namespace am = ::application_manager;
namespace strings = ::application_manager::strings;
using am::commands::CommandImpl;
using am::commands::RequestToHMI;
using sdl_rpc_plugin::commands::VRAddCommandRequest;

typedef std::shared_ptr<RequestToHMI> RequestToHMIPtr;

class VRAddCommandRequestTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(VRAddCommandRequestTest, RUN_SendRequest_with_vr_commands_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[strings::msg_params][strings::vr_commands] =
      SmartObject(smart_objects::SmartType_Array);

  InitEventDispatcher();
  RequestToHMIPtr command(CreateCommand<VRAddCommandRequest>(command_msg));

  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(command_msg));
  ASSERT_TRUE(command->Init());

  command->Run();

  EXPECT_EQ(CommandImpl::hmi_protocol_type_,
            (*command_msg)[strings::params][strings::protocol_type].asInt());
  EXPECT_EQ(CommandImpl::protocol_version_,
            (*command_msg)[strings::params][strings::protocol_version].asInt());
}

TEST_F(VRAddCommandRequestTest, RUN_Doesnt_SendRequest_without_vr_command) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));

  InitEventDispatcher();
  RequestToHMIPtr command(CreateCommand<VRAddCommandRequest>(command_msg));

  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(command_msg)).Times(0);
  ASSERT_TRUE(command->Init());

  command->Run();
}

}  // namespace vr_add_command_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test