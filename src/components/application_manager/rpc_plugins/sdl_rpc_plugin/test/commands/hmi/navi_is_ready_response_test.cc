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
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_event_dispatcher.h"
#include "hmi/navi_is_ready_response.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace navi_is_ready_responce {

namespace am = ::application_manager;
namespace commands = sdl_rpc_plugin::commands;

using ::testing::ReturnRef;
using ::utils::SharedPtr;
using application_manager::commands::ResponseFromHMI;
using test::components::event_engine_test::MockEventDispatcher;

typedef SharedPtr<ResponseFromHMI> ResponseFromHMIPtr;

class NaviIsReadyResponseTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {};

TEST_F(NaviIsReadyResponseTest, NaviIsReadyResponse_Run_SUCCESS) {
  ResponseFromHMIPtr command(CreateCommand<commands::NaviIsReadyResponse>());
  MockEventDispatcher mock_event_dispatcher;
  EXPECT_CALL(app_mngr_, event_dispatcher())
      .WillOnce(ReturnRef(mock_event_dispatcher));
  EXPECT_CALL(mock_event_dispatcher, raise_event(_));
  command->Run();
}

}  // namespace navi_is_ready_responce
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
