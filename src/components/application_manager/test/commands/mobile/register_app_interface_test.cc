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

#include "utils/shared_ptr.h"

#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "connection_handler/mock_connection_handler.h"

#include "mobile/register_app_interface_response.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {

using namespace application_manager;

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::_;

class RegisterAppInterfaceResponceTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  void SetUp() OVERRIDE {
    message_ = utils::MakeShared<SmartObject>(::smart_objects::SmartType_Map);
    (*message_)[strings::msg_params] =
        ::smart_objects::SmartObject(::smart_objects::SmartType_Map);
    command_sptr_ = CreateCommand<
        application_manager::commands::RegisterAppInterfaceResponse>(
        CommandsTest::kDefaultTimeout_, message_);
  }

  MessageSharedPtr message_;
  utils::SharedPtr<commands::RegisterAppInterfaceResponse> command_sptr_;
  policy_test::MockPolicyHandlerInterface mock_policy_handler_;
};

TEST_F(RegisterAppInterfaceResponceTest,
       Run_ApplicationRegisteredAlready_Unsuccess) {
  (*message_)[strings::msg_params][strings::success] = false;
  (*message_)[strings::msg_params][strings::result_code] =
      mobile_apis::Result::APPLICATION_REGISTERED_ALREADY;

  EXPECT_CALL(mock_app_manager_,
              SendMessageToMobile(
                  MobileResultCodeIs(
                      mobile_apis::Result::APPLICATION_REGISTERED_ALREADY),
                  false)).Times(1);

  EXPECT_CALL(mock_app_manager_, application(_)).Times(0);
  command_sptr_->Run();
}

TEST_F(RegisterAppInterfaceResponceTest, Run_WrongConnectionKey_Unsuccess) {
  EXPECT_CALL(mock_app_manager_, SendMessageToMobile(_, _)).Times(1);

  ApplicationSharedPtr null_application;
  EXPECT_CALL(mock_app_manager_, application(_))
      .WillOnce(Return(null_application));

  EXPECT_CALL(mock_app_manager_, GetPolicyHandler()).Times(0);
  command_sptr_->Run();
}

TEST_F(RegisterAppInterfaceResponceTest,
       Run_PolicyEnabled_SetHeartbeatTimeout) {
  const std::string kPolicyAppId = "id";
  EXPECT_CALL(mock_app_manager_, SendMessageToMobile(_, _)).Times(1);

  utils::SharedPtr<NiceMock<MockApplication> > mock_application =
      utils::MakeShared<NiceMock<MockApplication> >();
  EXPECT_CALL(mock_app_manager_, application(_))
      .WillOnce(Return(mock_application));
  ON_CALL(*mock_application, policy_app_id())
      .WillByDefault(Return(kPolicyAppId));

  ON_CALL(mock_app_manager_, GetPolicyHandler())
      .WillByDefault(ReturnRef(mock_policy_handler_));
  EXPECT_CALL(mock_policy_handler_, PolicyEnabled()).WillOnce(Return(true));
  EXPECT_CALL(mock_policy_handler_, HeartBeatTimeout(_))
      .WillOnce(Return(CommandsTest::kDefaultTimeout_));

  connection_handler_test::MockConnectionHandler mock_connection_handler;
  EXPECT_CALL(mock_app_manager_, connection_handler())
      .WillOnce(ReturnRef(mock_connection_handler));
  EXPECT_CALL(mock_connection_handler, SetHeartBeatTimeout(_, _)).Times(1);

  EXPECT_CALL(mock_app_manager_, OnApplicationRegistered(_));
  EXPECT_CALL(mock_policy_handler_, OnAppRegisteredOnMobile(kPolicyAppId));

  command_sptr_->Run();
}

TEST_F(RegisterAppInterfaceResponceTest,
       Run_PolicyDisabled_DontSetHeartbeatTimeout) {
  const std::string kPolicyAppId = "id";
  EXPECT_CALL(mock_app_manager_, SendMessageToMobile(_, _)).Times(1);

  utils::SharedPtr<NiceMock<MockApplication> > mock_application =
      utils::MakeShared<NiceMock<MockApplication> >();
  EXPECT_CALL(mock_app_manager_, application(_))
      .WillOnce(Return(mock_application));
  ON_CALL(*mock_application, policy_app_id())
      .WillByDefault(Return(kPolicyAppId));

  ON_CALL(mock_app_manager_, GetPolicyHandler())
      .WillByDefault(ReturnRef(mock_policy_handler_));
  EXPECT_CALL(mock_policy_handler_, PolicyEnabled()).WillOnce(Return(false));
  EXPECT_CALL(mock_policy_handler_, HeartBeatTimeout(_)).Times(0);
  EXPECT_CALL(mock_app_manager_, connection_handler()).Times(0);

  EXPECT_CALL(mock_app_manager_, OnApplicationRegistered(_));
  EXPECT_CALL(mock_policy_handler_, OnAppRegisteredOnMobile(kPolicyAppId));

  command_sptr_->Run();
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
