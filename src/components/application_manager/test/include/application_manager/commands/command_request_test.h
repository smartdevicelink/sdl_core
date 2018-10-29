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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_COMMAND_RESPONSES_TEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_COMMAND_RESPONSES_TEST_H_

#include <stdint.h>

#include "gtest/gtest.h"

#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/test/include/application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_impl.h"
#include "application_manager/test/include/application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_reset_timeout_handler.h"

#include "application_manager/event_engine/event.h"

namespace test {
namespace components {
namespace commands_test {

using ::testing::_;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::DoAll;
using ::testing::NiceMock;
using ::test::components::event_engine_test::MockEventDispatcher;
namespace am = ::application_manager;
using am::commands::Command;
using am::commands::CommandRequestImpl;
using am::event_engine::Event;
using test::components::application_manager_test::MockResetTimeoutHandler;

class CallRun {
 public:
  CallRun(CommandRequestImpl& command) : command_(command) {}

  void operator()() {
    command_.Run();
  }

  CommandRequestImpl& command_;
};

class CallOnEvent {
 public:
  CallOnEvent(CommandRequestImpl& command, Event& event)
      : command_(command), event_(event) {}

  void operator()() {
    command_.on_event(event_);
  }

  CommandRequestImpl& command_;
  Event& event_;
};

template <const CommandsTestMocks kIsNice = CommandsTestMocks::kNotNice>
class CommandRequestTest : public CommandsTest<kIsNice> {
 public:
  typedef typename TypeIf<kIsNice,
                          NiceMock<MockEventDispatcher>,
                          MockEventDispatcher>::Result MockEventDisp;

  template <class CallableT>
  MessageSharedPtr CatchMobileCommandResult(CallableT delegate,
                                            bool call_return = true) {
    MessageSharedPtr result_msg;
    EXPECT_CALL(this->mock_rpc_service_, ManageMobileCommand(_, _))
        .WillOnce(DoAll(SaveArg<0>(&result_msg), Return(call_return)));
    delegate();
    return result_msg;
  }

  template <class CallableT>
  MessageSharedPtr CatchHMICommandResult(CallableT delegate,
                                         bool call_return = true) {
    MessageSharedPtr result_msg;
    EXPECT_CALL(this->mock_rpc_service_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&result_msg), Return(call_return)));
    delegate();
    return result_msg;
  }

  MockEventDisp event_dispatcher_;
  MockResetTimeoutHandler mock_reset_timeout_handler_;

 protected:
  CommandRequestTest() : CommandsTest<kIsNice>() {}

  virtual void InitCommand(const uint32_t& default_timeout) OVERRIDE {
    CommandsTest<kIsNice>::InitCommand(default_timeout);
    ON_CALL(CommandsTest<kIsNice>::app_mngr_, event_dispatcher())
        .WillByDefault(ReturnRef(event_dispatcher_));
    ON_CALL(CommandsTest<kIsNice>::app_mngr_, GetResetTimeoutHandler())
        .WillByDefault(ReturnRef(mock_reset_timeout_handler_));
  }
};

}  // namespace commands_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_COMMAND_RESPONSES_TEST_H_
