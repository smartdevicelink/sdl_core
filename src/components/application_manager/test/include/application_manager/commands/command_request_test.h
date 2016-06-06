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
#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_event_dispatcher.h"

namespace test {
namespace components {
namespace commands_test {

using ::testing::ReturnRef;
using ::testing::NiceMock;
using ::test::components::event_engine_test::MockEventDispatcher;
using ::application_manager::commands::Command;

template <const CommandsTestMocks kIsNice = CommandsTestMocks::kNotNice>
class CommandRequestTest : public CommandsTest<kIsNice> {
 public:
  typedef typename TypeIf<kIsNice,
                          NiceMock<MockEventDispatcher>,
                          MockEventDispatcher>::Result MockEventDisp;

  MockEventDisp ev_disp_;

 protected:
  CommandRequestTest() : CommandsTest<kIsNice>() {}

  virtual void InitCommand(const uint32_t& default_timeout) OVERRIDE {
    CommandsTest<kIsNice>::InitCommand(default_timeout);
    EXPECT_CALL(CommandsTest<kIsNice>::app_mngr_, event_dispatcher())
        .WillOnce(ReturnRef(ev_disp_));
  }
};

}  // namespace commands_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_COMMAND_RESPONSES_TEST_H_
