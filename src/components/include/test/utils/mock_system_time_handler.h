/*
 * Copyright (c) 2018, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_TEST_SECURITY_MANAGER_MOCK_SYSTEM_TIME_HANDLER_H
#define SRC_COMPONENTS_INCLUDE_TEST_SECURITY_MANAGER_MOCK_SYSTEM_TIME_HANDLER_H

#include "gmock/gmock.h"
#include "utils/system_time_handler.h"

namespace test {
namespace components {
namespace security_manager_test {

class MockSystemTimeHandler : public ::utils::SystemTimeHandler {
 public:
  MockSystemTimeHandler() {}
  MOCK_METHOD0(QuerySystemTime, void());
  MOCK_METHOD1(SubscribeOnSystemTime,
               void(utils::SystemTimeListener* listener));
  MOCK_METHOD1(UnSubscribeFromSystemTime,
               void(utils::SystemTimeListener* listener));
  MOCK_METHOD0(GetUTCTime, time_t());
  MOCK_CONST_METHOD0(system_time_can_be_received, bool());
  ~MockSystemTimeHandler() {}

 private:
  void DoSubscribe(utils::SystemTimeListener*) {}
  void DoSystemTimeQuery() {}
  void DoUnsubscribe(utils::SystemTimeListener* listener) {}
  bool utc_time_can_be_received() const {
    return true;
  }
  time_t FetchSystemTime() {
    return 0;
  }
};
}  // namespace security_manager_test
}  // namespace components
}  // namespace test
#endif  // SRC_COMPONENTS_INCLUDE_TEST_SECURITY_MANAGER_MOCK_SYSTEM_TIME_HANDLER_H
