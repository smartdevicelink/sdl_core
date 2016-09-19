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
#include "vr_module/commands/activate_service.h"
#include "vr_module/interface/hmi.pb.h"
#include "vr_module/interface/mobile.pb.h"
#include "vr_module/mobile_event.h"

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

class ActivateServiceTest : public ::testing::Test {
 protected:
};

TEST_F(ActivateServiceTest, Execute) {
  MockServiceModule service;

  const int32_t kHmiId = 1;
  const int32_t kAppId = 3;
  vr_hmi_api::ServiceMessage input;
  input.set_rpc(vr_hmi_api::ACTIVATE);
  input.set_rpc_type(vr_hmi_api::REQUEST);
  input.set_correlation_id(kHmiId);
  vr_hmi_api::ActivateServiceRequest request;
  request.set_appid(kAppId);
  std::string params;
  request.SerializeToString(&params);
  input.set_params(params);
  ActivateService cmd(input, &service);

  vr_hmi_api::ServiceMessage expected;
  expected.set_rpc(vr_hmi_api::ACTIVATE);
  expected.set_rpc_type(vr_hmi_api::RESPONSE);
  expected.set_correlation_id(kHmiId);
  vr_hmi_api::ActivateServiceResponse hmi_response;
  hmi_response.set_result(vr_hmi_api::SUCCESS);
  std::string hmi_params;
  hmi_response.SerializeToString(&hmi_params);
  expected.set_params(hmi_params);
  EXPECT_CALL(service, ActivateService(kAppId)).Times(1);
  EXPECT_CALL(service, SendToHmi(ServiceMessageEq(expected))).Times(1)
      .WillOnce(Return(true));
  EXPECT_TRUE(cmd.Execute());
}

TEST_F(ActivateServiceTest, ExecuteNoParams) {
  MockServiceModule service;

  const int32_t kHmiId = 1;
  vr_hmi_api::ServiceMessage input;
  input.set_rpc(vr_hmi_api::ACTIVATE);
  input.set_rpc_type(vr_hmi_api::REQUEST);
  input.set_correlation_id(kHmiId);
  ActivateService cmd(input, &service);

  vr_hmi_api::ServiceMessage expected;
  expected.set_rpc(vr_hmi_api::ACTIVATE);
  expected.set_rpc_type(vr_hmi_api::RESPONSE);
  expected.set_correlation_id(kHmiId);
  vr_hmi_api::ActivateServiceResponse hmi_response;
  hmi_response.set_result(vr_hmi_api::INVALID_DATA);
  std::string hmi_params;
  hmi_response.SerializeToString(&hmi_params);
  expected.set_params(hmi_params);
  EXPECT_CALL(service, SendToHmi(ServiceMessageEq(expected))).Times(1)
      .WillOnce(Return(true));
  EXPECT_CALL(service, ActivateService(_)).Times(0);
  EXPECT_TRUE(cmd.Execute());
}

}  // namespace commands
}  // namespace vr_module
