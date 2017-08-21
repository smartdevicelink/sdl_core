/*
 * Copyright (c) 2017, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_REQUEST_H_

#include "gmock/gmock.h"
#include "application_manager/commands/command.h"

namespace test {
namespace components {
namespace application_manager_test {

using ::testing::Return;

class MockRequest : public application_manager::commands::Command {
 public:
  MockRequest(uint32_t connection_key, uint32_t correlation_id) {
    ON_CALL(*this, correlation_id()).WillByDefault(Return(correlation_id));
    ON_CALL(*this, connection_key()).WillByDefault(Return(connection_key));
  }

  MOCK_METHOD0(CheckPermissions, bool());
  MOCK_METHOD0(Init, bool());
  MOCK_METHOD0(Run, void());
  MOCK_METHOD0(CleanUp, bool());
  MOCK_CONST_METHOD0(default_timeout, uint32_t());
  MOCK_CONST_METHOD0(function_id, int32_t());
  MOCK_METHOD0(onTimeOut, void());
  MOCK_METHOD0(AllowedToTerminate, bool());
  MOCK_METHOD1(SetAllowedToTerminate, void(bool is_allowed));

  MOCK_CONST_METHOD0(connection_key, uint32_t());
  MOCK_CONST_METHOD0(correlation_id, uint32_t());
};

}  // namespace application_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_REQUEST_H_
