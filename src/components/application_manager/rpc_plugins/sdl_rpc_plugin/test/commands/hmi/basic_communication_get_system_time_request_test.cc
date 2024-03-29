/*
 * Copyright (c) 2018 Ford Motor Company
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
 * Neither the names of the copyright holders nor the names of its contributors
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

#include "hmi/basic_communication_get_system_time_request.h"

#include "gtest/gtest.h"

#include "application_manager/commands/command_request_test.h"
#include "protocol_handler/mock_protocol_handler.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace basic_communication_get_system_time_request {

using ::testing::ReturnRef;
namespace am = ::application_manager;
using am::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::BasicCommunicationGetSystemTimeRequest;
using namespace ::protocol_handler;

class BasicCommunicationGetSystemTimeRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {};

TEST_F(BasicCommunicationGetSystemTimeRequestTest, OnTimeout) {
  MessageSharedPtr msg = CreateMessage();
  protocol_handler_test::MockProtocolHandler mock_protocol_handler;

  auto command = CreateCommand<BasicCommunicationGetSystemTimeRequest>(msg);

  ON_CALL(app_mngr_, protocol_handler())
      .WillByDefault(ReturnRef(mock_protocol_handler));
  EXPECT_CALL(mock_protocol_handler, NotifyOnGetSystemTimeFailed());

  command->OnTimeOut();
}

}  // namespace basic_communication_get_system_time_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
