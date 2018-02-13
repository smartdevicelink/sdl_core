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
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/commands/request_from_hmi.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace simple_requests_from_hmi_test {

using ::testing::_;
using ::testing::Types;
using ::testing::NotNull;
using ::testing::NiceMock;

using ::utils::SharedPtr;
namespace commands = ::application_manager::commands;
using commands::MessageSharedPtr;
using ::application_manager::event_engine::EventObserver;
using ::test::components::event_engine_test::MockEventDispatcher;

class RequestFromHMITest : public CommandsTest<CommandsTestMocks::kIsNice> {
 protected:
  void SetUp() OVERRIDE {
    EXPECT_CALL(app_mngr_, event_dispatcher())
        .WillOnce(ReturnRef(mock_event_dispatcher_));
  }
  NiceMock<event_engine_test::MockEventDispatcher> mock_event_dispatcher_;
};

TEST_F(RequestFromHMITest, BasicMethodsOverloads_SUCCESS) {
  SharedPtr<commands::RequestFromHMI> command(
      CreateCommand<commands::RequestFromHMI>());
  application_manager::event_engine::Event event(
      hmi_apis::FunctionID::BasicCommunication_ActivateApp);
  // Current implementation always return `true`
  EXPECT_TRUE(command->Init());
  EXPECT_TRUE(command->CleanUp());
  EXPECT_NO_THROW(command->Run());
  EXPECT_NO_THROW(command->on_event(event));
}

TEST_F(RequestFromHMITest, SendResponse_SUCCESS) {
  SharedPtr<commands::RequestFromHMI> command(
      CreateCommand<commands::RequestFromHMI>());

  const bool success = false;
  const uint32_t correlation_id = 1u;
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(rpc_service_, ManageHMICommand(NotNull()));

  command->SendResponse(success,
                        correlation_id,
                        hmi_apis::FunctionID::BasicCommunication_ActivateApp,
                        hmi_apis::Common_Result::SUCCESS);
}

}  // namespace simple_requests_to_hmi_test
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
