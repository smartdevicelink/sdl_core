/*
 * Copyright (c) 2017 Xevo Inc.
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
 * Neither the name of the Xevo Inc. nor the names of its contributors
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

#include "commands/commands_test.h"
#include "commands/command_request_test.h"

#include "mobile/send_haptic_data_request.h"
#include "mobile/send_haptic_data_response.h"

#include "interfaces/MOBILE_API.h"
#include "application_manager/application.h"
#include "application_manager/mock_application.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace send_haptic_data {

using ::testing::_;
using ::testing::Return;

namespace am = ::application_manager;

using am::commands::SendHapticDataRequest;
using am::commands::SendHapticDataResponse;
using am::commands::MessageSharedPtr;

typedef SharedPtr<SendHapticDataRequest> SendHapticDataRequestPtr;
typedef SharedPtr<SendHapticDataResponse> SendHapticDataResponsePtr;

namespace {
const uint32_t kConnectionKey = 1u;
}

class SendHapticDataRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SendHapticDataRequestTest()
      : msg_(CreateMessage(::smart_objects::SmartType_Map))
      , mock_app_(CreateMockApp()) {}

  void SetUp() OVERRIDE {
    (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
  }

  MessageSharedPtr msg_;
  MockAppPtr mock_app_;
};

class SendHapticDataResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  SendHapticDataResponseTest() : message_(CreateMessage()) {}

  void SetUp() OVERRIDE {
    command_sptr_ = CreateCommand<SendHapticDataResponse>(message_);
  }

  MessageSharedPtr message_;
  SharedPtr<SendHapticDataResponse> command_sptr_;
};

TEST_F(SendHapticDataRequestTest, Run_SUCCESS) {
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(*mock_app_, is_navi()).WillOnce(Return(true));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(rpc_service_,
              ManageHMICommand(
                  HMIResultCodeIs(hmi_apis::FunctionID::UI_SendHapticData)))
      .WillOnce(Return(true));

  SendHapticDataRequestPtr command(CreateCommand<SendHapticDataRequest>(msg_));

  command->Init();
  command->Run();
}

TEST_F(SendHapticDataRequestTest, Run_DISALLOWED) {
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(*mock_app_, is_navi()).WillOnce(Return(false));

  EXPECT_CALL(*mock_app_, mobile_projection_enabled()).WillOnce(Return(false));
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(rpc_service_, ManageMobileCommand(_, _)).WillOnce(Return(true));

  SendHapticDataRequestPtr command(CreateCommand<SendHapticDataRequest>(msg_));

  command->Init();
  command->Run();
}

TEST_F(SendHapticDataRequestTest, OnEvent_SUCCESS) {
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS),
                          am::commands::Command::SOURCE_SDL));

  (*msg_)[am::strings::msg_params] = 0;
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::SUCCESS;
  Event event(hmi_apis::FunctionID::UI_SendHapticData);
  event.set_smart_object(*msg_);
  SendHapticDataRequestPtr command(CreateCommand<SendHapticDataRequest>(msg_));

  command->Init();
  command->on_event(event);
}

TEST_F(SendHapticDataResponseTest, Run_Success) {
  ::smart_objects::SmartObject& message_ref = *message_;
  message_ref[am::strings::msg_params][am::strings::result_code] =
      mobile_apis::Result::SUCCESS;
  ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
  EXPECT_CALL(
      rpc_service_,
      SendMessageToMobile(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _));
  command_sptr_->Init();
  command_sptr_->Run();
}

}  // namespace send_haptic_data
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
