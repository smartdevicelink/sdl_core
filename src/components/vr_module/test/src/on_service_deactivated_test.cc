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
#include "gmock/gmock.h"
#include "mock_service_module.h"
#include "vr_module/commands/on_service_deactivated.h"
#include "vr_module/interface/hmi.pb.h"
#include "vr_module/interface/mobile.pb.h"

using ::testing::Return;

namespace vr_module {
namespace commands {

MATCHER_P(ServiceMessageEq, expected, "") {
  return arg.rpc() == expected.rpc()
      && arg.rpc_type() == expected.rpc_type()
      && arg.correlation_id() == expected.correlation_id()
      && arg.params() == expected.params();
}

class OnServiceDeactivatedTest : public ::testing::Test {
 protected:
};

TEST_F(OnServiceDeactivatedTest, Execute) {
  MockServiceModule service;

  const int32_t kMobileId = 1;
  OnServiceDeactivated cmd(&service);
  vr_mobile_api::ServiceMessage expected;
  expected.set_rpc(vr_mobile_api::ON_DEACTIVATED);
  expected.set_rpc_type(vr_mobile_api::NOTIFICATION);
  expected.set_correlation_id(kMobileId);
  EXPECT_CALL(service, SendToMobile(ServiceMessageEq(expected))).Times(1)
      .WillOnce(Return(true));
  EXPECT_CALL(service, GetNextCorrelationID()).Times(1).WillOnce(
      Return(kMobileId));
  EXPECT_CALL(service, DeactivateService()).Times(1);
  EXPECT_TRUE(cmd.Execute());
}

}  // namespace commands
}  // namespace vr_module
