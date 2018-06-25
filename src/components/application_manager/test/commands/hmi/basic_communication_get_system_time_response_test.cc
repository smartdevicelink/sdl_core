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

#include "gtest/gtest.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/hmi/basic_communication_get_system_time_response.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/smart_object_keys.h"
#include "interfaces/HMI_API.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace basic_communication_get_system_time_response {

using application_manager::commands::BasicCommunicationGetSystemTimeResponse;
using test::components::event_engine_test::MockEventDispatcher;
using testing::ReturnRef;

ACTION_P(GetEventId, event_id) {
  *event_id = arg0.id();
}

class BasicCommunicationGetSystemTimeResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(BasicCommunicationGetSystemTimeResponseTest, Run_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  MockEventDispatcher mock_event_dispatcher;
  int32_t event_id = hmi_apis::FunctionID::INVALID_ENUM;

  auto command(CreateCommand<BasicCommunicationGetSystemTimeResponse>(msg));

  EXPECT_CALL(app_mngr_, event_dispatcher())
      .WillOnce(ReturnRef(mock_event_dispatcher));
  EXPECT_CALL(mock_event_dispatcher, raise_event(_))
      .WillOnce(GetEventId(&event_id));

  command->Run();

  EXPECT_EQ(hmi_apis::FunctionID::BasicCommunication_GetSystemTime, event_id);
}

}  // namespace basic_communication_get_system_time_response
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
