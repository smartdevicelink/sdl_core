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
#include <algorithm>

#include "gtest/gtest.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/commands/command_request_impl.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "utils/lock.h"
#include "utils/shared_ptr.h"
#include "utils/data_accessor.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace commands_test {

namespace strings = ::application_manager::strings;
namespace hmi_response = ::application_manager::hmi_response;

using ::testing::_;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::DoAll;

using ::utils::SharedPtr;
using ::application_manager::commands::MessageSharedPtr;
using ::application_manager::CommandParametersPermissions;
using ::application_manager::event_engine::EventObserver;
using ::application_manager::commands::CommandImpl;
using ::application_manager::commands::CommandRequestImpl;
using ::application_manager::ApplicationManager;
using ::application_manager::ApplicationSet;
using ::application_manager::RPCParams;

typedef ::application_manager::commands::CommandRequestImpl::RequestState
    RequestState;

class CommandRequestImplTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  class UnwrappedCommandRequestImpl : public CommandRequestImpl {
   public:
    using CommandRequestImpl::CheckAllowedParameters;
    using CommandRequestImpl::RemoveDisallowedParameters;
    using CommandRequestImpl::AddDisallowedParameters;
    using CommandRequestImpl::HasDisallowedParams;

    UnwrappedCommandRequestImpl(const MessageSharedPtr& message,
                                ApplicationManager& application_manager)
        : CommandRequestImpl(message, application_manager) {}

    const RequestState current_state() const {
      return current_state_;
    }
    void set_current_state(const RequestState state) {
      current_state_ = state;
    }

    CommandParametersPermissions& parameters_permissions() {
      return parameters_permissions_;
    }

    CommandParametersPermissions& removed_parameters_permissions() {
      return removed_parameters_permissions_;
    }
  };

  const uint32_t kConnectionKey_ = 5u;
  const uint32_t kCorrelationId_ = 3u;
  const hmi_apis::FunctionID::eType kInvalidFunctionId_ =
      hmi_apis::FunctionID::INVALID_ENUM;
  const std::string kPolicyAppId_ = "Test";
  const mobile_apis::Result::eType kMobResultSuccess_ =
      mobile_apis::Result::SUCCESS;

  sync_primitives::Lock app_set_lock_;
};

typedef CommandRequestImplTest::UnwrappedCommandRequestImpl UCommandRequestImpl;
typedef SharedPtr<UCommandRequestImpl> CommandPtr;

TEST_F(CommandRequestImplTest, OnTimeOut_StateCompleted_UNSUCCESS) {
  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  // Should be called twice:
  // First -- on `onTimeOut` method call
  // Second -- on destruction;
  EXPECT_CALL(ev_disp_, remove_observer(_)).Times(2);
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  // If `command` already done, then state should change to `kCompleted`.
  command->set_current_state(RequestState::kCompleted);

  command->onTimeOut();

  EXPECT_EQ(RequestState::kCompleted, command->current_state());
}

TEST_F(CommandRequestImplTest, OnTimeOut_StateAwaitingHMIResponse_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::correlation_id] = kCorrelationId_;
  (*msg)[strings::params][strings::function_id] = kInvalidFunctionId_;
  (*msg)[strings::params][strings::connection_key] = kConnectionKey_;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  // Should be called twice:
  // First -- on `onTimeOut` method call
  // Second -- on destruction;
  EXPECT_CALL(ev_disp_, remove_observer(_)).Times(2);
  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(_, Command::CommandOrigin::ORIGIN_SDL));

  command->onTimeOut();

  // If `command` not done till now, then state should change to `kTimedOut`
  // and sent it to application manager to deal with it.
  EXPECT_EQ(RequestState::kTimedOut, command->current_state());
}

TEST_F(CommandRequestImplTest, CheckSyntax_SUCCESS) {
  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  // Checking message syntax.
  const std::string str1("\t\n");
  EXPECT_FALSE(command->CheckSyntax(str1, false));
  const std::string str2("\\n");
  EXPECT_FALSE(command->CheckSyntax(str2, false));
  const std::string str3("\\t");
  EXPECT_FALSE(command->CheckSyntax(str3, false));
  const std::string str4(" ");
  EXPECT_FALSE(command->CheckSyntax(str4, false));
  EXPECT_TRUE(command->CheckSyntax(str4, true));
}

TEST_F(CommandRequestImplTest, GetMobileResultCode_SUCCESS) {
  union ResultU {
    int32_t value_;
    hmi_apis::Common_Result::eType hmi_;
    mobile_apis::Result::eType mobile_;
  };

  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  // Run thru all possible accordance
  // of HMI and Mobile result codes.
  ResultU result_it;
  for (result_it.hmi_ = hmi_apis::Common_Result::SUCCESS;
       result_it.value_ < hmi_apis::Common_Result::TRUNCATED_DATA;
       ++result_it.value_) {
    if (result_it.hmi_ != hmi_apis::Common_Result::NO_DEVICES_CONNECTED &&
        result_it.hmi_ != hmi_apis::Common_Result::NO_APPS_REGISTERED) {
      EXPECT_EQ(result_it.mobile_,
                command->GetMobileResultCode(result_it.hmi_));
    }
  }
  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED,
            command->GetMobileResultCode(
                hmi_apis::Common_Result::NO_DEVICES_CONNECTED));
  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED,
            command->GetMobileResultCode(
                hmi_apis::Common_Result::NO_APPS_REGISTERED));
  EXPECT_EQ(
      mobile_apis::Result::GENERIC_ERROR,
      command->GetMobileResultCode(hmi_apis::Common_Result::TRUNCATED_DATA));
}

TEST_F(CommandRequestImplTest, BasicMethodsOverloads_SUCCESS) {
  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  // Current implementation always return `true`
  EXPECT_TRUE(command->Init());
  EXPECT_TRUE(command->CleanUp());
  EXPECT_NO_THROW(command->Run());
  application_manager::event_engine::Event event(kInvalidFunctionId_);
  EXPECT_NO_THROW(command->on_event(event));
}

TEST_F(CommandRequestImplTest, CreateHMINotification_SUCCESS) {
  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  const std::string kTestParamsKey = "test_msg_params";

  MessageSharedPtr msg_params = CreateMessage();
  (*msg_params)[kTestParamsKey] = 0;

  MessageSharedPtr result;

  EXPECT_CALL(app_mngr_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  command->CreateHMINotification(kInvalidFunctionId_, *msg_params);

  // Check if message been formed and sent to application manager.
  EXPECT_TRUE((*result).keyExists(strings::msg_params));
  EXPECT_TRUE((*result)[strings::msg_params].keyExists(kTestParamsKey));
}

TEST_F(CommandRequestImplTest, SendHMIRequest_NoUseEvent_SUCCESS) {
  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId_));
  // Return `true` prevents call of `SendResponse` method;
  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).WillOnce(Return(true));

  EXPECT_EQ(kCorrelationId_,
            command->SendHMIRequest(kInvalidFunctionId_, NULL, false));
}

TEST_F(CommandRequestImplTest, SendHMIRequest_UseEvent_SUCCESS) {
  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  EXPECT_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillOnce(Return(kCorrelationId_));
  // Return `true` prevents call of `SendResponse` method;
  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).WillOnce(Return(true));

  EXPECT_CALL(ev_disp_, add_observer(_, _, _));

  EXPECT_EQ(kCorrelationId_,
            command->SendHMIRequest(kInvalidFunctionId_, NULL, true));
}

TEST_F(CommandRequestImplTest, RemoveDisallowedParameters_SUCCESS) {
  const std::string kDisParam1 = "disallowed_param1";
  const std::string kDisParam2 = "disallowed_param2";
  const std::string kAlParam = "allowed_param";
  const std::string kUnParam = "undefined_params";
  const std::string kMisParam =
      ::application_manager::MessageHelper::vehicle_data().begin()->first;

  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::msg_params][kDisParam1] = 0u;
  (*msg)[strings::msg_params][kDisParam2] = 0u;
  (*msg)[strings::msg_params][kAlParam] = 0u;
  (*msg)[strings::msg_params][kUnParam] = 0u;
  (*msg)[strings::msg_params][kMisParam] = 0u;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  CommandParametersPermissions& permission = command->parameters_permissions();
  permission.disallowed_params.push_back(kDisParam1);
  permission.disallowed_params.push_back(kDisParam2);
  permission.allowed_params.push_back(kAlParam);
  permission.undefined_params.push_back(kUnParam);

  command->RemoveDisallowedParameters();

  EXPECT_FALSE((*msg)[strings::msg_params].keyExists(kDisParam1));
  EXPECT_FALSE((*msg)[strings::msg_params].keyExists(kDisParam2));
  EXPECT_FALSE((*msg)[strings::msg_params].keyExists(kUnParam));
  EXPECT_FALSE((*msg)[strings::msg_params].keyExists(kMisParam));
  EXPECT_TRUE((*msg)[strings::msg_params].keyExists(kAlParam));
  EXPECT_TRUE(command->HasDisallowedParams());
}

TEST_F(CommandRequestImplTest,
       CheckAllowedParameters_RegisterAppInterface_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::RegisterAppInterfaceID;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  EXPECT_CALL(app_mngr_, applications()).Times(0);
  EXPECT_TRUE(command->CheckPermissions());
}

TEST_F(CommandRequestImplTest,
       CheckAllowedParameters_NoAppWithSameConnectionKey_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::connection_key] = kConnectionKey_;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  MockAppPtr app = CreateMockApp();
  ApplicationSet app_set;
  app_set.insert(app);

  DataAccessor<ApplicationSet> accessor(app_set, app_set_lock_);

  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(accessor));
  EXPECT_CALL(*app, app_id()).WillOnce(Return(6u));
  EXPECT_TRUE(command->CheckPermissions());
}

TEST_F(CommandRequestImplTest, CheckAllowedParameters_NoMsgParamsMap_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::connection_key] = kConnectionKey_;
  (*msg)[strings::msg_params] = 0u;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  MockAppPtr app = CreateMockApp();
  ApplicationSet app_set;
  app_set.insert(app);

  DataAccessor<ApplicationSet> accessor(app_set, app_set_lock_);

  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(accessor));
  EXPECT_CALL(*app, app_id()).WillOnce(Return(kConnectionKey_));
  EXPECT_CALL(*app, policy_app_id()).WillOnce(Return(kPolicyAppId_));
  EXPECT_CALL(*app, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));

  EXPECT_CALL(app_mngr_, CheckPolicyPermissions(_, _, _, _, _))
      .WillOnce(Return(kMobResultSuccess_));

  EXPECT_TRUE(command->CheckPermissions());
}

TEST_F(CommandRequestImplTest,
       CheckAllowedParameters_WrongPolicyPermissions_UNSUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::connection_key] = kConnectionKey_;
  (*msg)[strings::msg_params] = 0u;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  MockAppPtr app = CreateMockApp();
  ApplicationSet app_set;
  app_set.insert(app);

  DataAccessor<ApplicationSet> accessor(app_set, app_set_lock_);

  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(accessor));
  EXPECT_CALL(*app, app_id()).Times(2).WillRepeatedly(Return(kConnectionKey_));
  EXPECT_CALL(*app, policy_app_id()).WillOnce(Return(kPolicyAppId_));
  EXPECT_CALL(*app, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));

  EXPECT_CALL(app_mngr_, CheckPolicyPermissions(_, _, _, _, _))
      .WillOnce(Return(mobile_apis::Result::INVALID_ENUM));

  EXPECT_CALL(app_mngr_, SendMessageToMobile(_, _));
  EXPECT_FALSE(command->CheckPermissions());
}

ACTION_P(GetArg3, output) {
  *output = arg3;
}

TEST_F(CommandRequestImplTest, CheckAllowedParameters_MsgParamsMap_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::connection_key] = kConnectionKey_;
  (*msg)[strings::msg_params][kPolicyAppId_] = true;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  MockAppPtr app = CreateMockApp();
  ApplicationSet app_set;
  app_set.insert(app);

  DataAccessor<ApplicationSet> accessor(app_set, app_set_lock_);

  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(accessor));
  EXPECT_CALL(*app, app_id()).WillOnce(Return(kConnectionKey_));
  EXPECT_CALL(*app, policy_app_id()).WillOnce(Return(kPolicyAppId_));
  EXPECT_CALL(*app, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));

  RPCParams params;
  EXPECT_CALL(app_mngr_, CheckPolicyPermissions(_, _, _, _, _))
      .WillOnce(DoAll(GetArg3(&params), Return(kMobResultSuccess_)));

  EXPECT_TRUE(command->CheckPermissions());
  EXPECT_TRUE(params.end() !=
              std::find(params.begin(), params.end(), kPolicyAppId_));
}

TEST_F(CommandRequestImplTest, AddDisallowedParameters_SUCCESS) {
  const std::string kDisParam =
      ::application_manager::MessageHelper::vehicle_data().begin()->first;

  MessageSharedPtr msg;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  command->removed_parameters_permissions().disallowed_params.push_back(
      kDisParam);

  command->AddDisallowedParameters(*msg);

  EXPECT_TRUE((*msg)[strings::msg_params].keyExists(kDisParam));
}

TEST_F(CommandRequestImplTest, SendResponse_TimedOut_UNSUCCESS) {
  CommandPtr command = CreateCommand<UCommandRequestImpl>();

  command->set_current_state(RequestState::kTimedOut);

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  // Args do not affect on anything in this case;
  command->SendResponse(true, kMobResultSuccess_, NULL, NULL);

  EXPECT_EQ(RequestState::kTimedOut, command->current_state());
}

TEST_F(CommandRequestImplTest, SendResponse_SUCCESS) {
  MessageSharedPtr msg;
  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  EXPECT_TRUE(smart_objects::SmartType_Null == (*msg).getType());

  MessageSharedPtr result;
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  // Args do not affect on anything in this case;
  command->SendResponse(true, kMobResultSuccess_, NULL, NULL);

  EXPECT_EQ(RequestState::kCompleted, command->current_state());

  EXPECT_TRUE(smart_objects::SmartType_Map == (*msg).getType());
}

TEST_F(CommandRequestImplTest,
       SendResponse_AddDisallowedParametersToInfo_SUCCESS) {
  const std::string kDisParam = "disallowed_param";
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::SubscribeVehicleDataID;

  CommandPtr command = CreateCommand<UCommandRequestImpl>(msg);

  command->removed_parameters_permissions().disallowed_params.push_back(
      kDisParam);

  MessageSharedPtr result;
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  command->SendResponse(true, kMobResultSuccess_, NULL, NULL);

  EXPECT_EQ(RequestState::kCompleted, command->current_state());

  EXPECT_TRUE((*result)[strings::msg_params].keyExists(strings::info));
  EXPECT_NE("", (*result)[strings::msg_params][strings::info].asString());
}

}  // namespace commands_test
}  // namespace components
}  // namespace test
