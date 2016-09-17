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
#include "vr_module/commands/on_register_service.h"
#include "vr_module/interface/hmi.pb.h"
#include "vr_module/interface/mobile.pb.h"

using ::testing::_;
using ::testing::Return;

namespace vr_module {
namespace commands {

MATCHER_P(ServiceMessageEq, expected, "") {
  return arg.rpc() == expected.rpc()
      && arg.rpc_type() == expected.rpc_type()
      && arg.correlation_id() == expected.correlation_id()
      && arg.params() == expected.params();
}

class OnRegisterServiceTest : public ::testing::Test {
 protected:
};

TEST_F(OnRegisterServiceTest, Execute) {
  MockServiceModule service;

  const int32_t kId = 1;
  EXPECT_CALL(service, GetNextCorrelationID()).Times(1).WillOnce(Return(kId));
  vr_hmi_api::ServiceMessage input;
  input.set_rpc(vr_hmi_api::ON_REGISTER);
  vr_hmi_api::OnRegisterServiceNotification notification;
  notification.set_appid(3);
  std::string params;
  notification.SerializeToString(&params);
  input.set_params(params);
  OnRegisterService cmd(input, &service);

  vr_hmi_api::ServiceMessage expected;
  expected.set_rpc(vr_hmi_api::ON_REGISTER);
  expected.set_rpc_type(vr_hmi_api::NOTIFICATION);
  expected.set_correlation_id(kId);
  vr_hmi_api::OnRegisterServiceNotification hmi_notification;
  hmi_notification.set_default_(true);
  hmi_notification.set_appid(3);
  std::string hmi_params;
  hmi_notification.SerializeToString(&hmi_params);
  expected.set_params(hmi_params);
  EXPECT_CALL(service, IsDefaultService(3)).Times(1).WillOnce(Return(true));
  EXPECT_CALL(service, SendToHmi(ServiceMessageEq(expected))).Times(1).WillOnce(
      Return(true));
  EXPECT_TRUE(cmd.Execute());
}

TEST_F(OnRegisterServiceTest, ExecuteNoDefault) {
  MockServiceModule service;

  const int32_t kId = 1;
  EXPECT_CALL(service, GetNextCorrelationID()).Times(1).WillOnce(Return(kId));
  vr_hmi_api::ServiceMessage input;
  input.set_rpc(vr_hmi_api::ON_REGISTER);
  vr_hmi_api::OnRegisterServiceNotification notification;
  notification.set_appid(3);
  std::string params;
  notification.SerializeToString(&params);
  input.set_params(params);
  OnRegisterService cmd(input, &service);

  vr_hmi_api::ServiceMessage expected;
  expected.set_rpc(vr_hmi_api::ON_REGISTER);
  expected.set_rpc_type(vr_hmi_api::NOTIFICATION);
  expected.set_correlation_id(kId);
  vr_hmi_api::OnRegisterServiceNotification hmi_notification;
  hmi_notification.set_default_(false);
  hmi_notification.set_appid(3);
  std::string hmi_params;
  hmi_notification.SerializeToString(&hmi_params);
  expected.set_params(hmi_params);
  EXPECT_CALL(service, IsDefaultService(3)).Times(1).WillOnce(Return(false));
  EXPECT_CALL(service, SendToHmi(ServiceMessageEq(expected))).Times(1).WillOnce(
      Return(true));
  EXPECT_TRUE(cmd.Execute());
}

TEST_F(OnRegisterServiceTest, ExecuteNoParams) {
  MockServiceModule service;

  const int32_t kId = 1;
  EXPECT_CALL(service, GetNextCorrelationID()).Times(1).WillOnce(Return(kId));
  vr_hmi_api::ServiceMessage input;
  input.set_rpc(vr_hmi_api::ON_REGISTER);
  OnRegisterService cmd(input, &service);

  EXPECT_CALL(service, IsDefaultService(_)).Times(0);
  EXPECT_CALL(service, SendToHmi(_)).Times(0);
  EXPECT_FALSE(cmd.Execute());
}

}  // namespace commands
}  // namespace vr_module
