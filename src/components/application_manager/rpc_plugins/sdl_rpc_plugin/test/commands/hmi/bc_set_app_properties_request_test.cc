/*
 * Copyright (c) 2020 Ford Motor Company
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
 * Neither the names of the copyright holders nor the names of its contributors
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

#include "application_manager/policies/policy_handler.h"
#include "hmi/bc_set_app_properties_request.h"

#include "application_manager/commands/command_request_test.h"
#include "application_manager/policies/policy_handler.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace bc_set_app_properties_request {

namespace am = ::application_manager;
namespace strings = am::strings;

using sdl_rpc_plugin::commands::BCSetAppPropertiesRequest;
using ::testing::_;
using ::testing::ReturnRef;

namespace {
const uint32_t kAppId = 1u;
const std::string kPolicyAppId = "00001";
const std::string kToken = "TOKEN";
const uint32_t kHmiAppId = 13u;
const uint32_t kCorrelationId = 1u;
const auto kSource = am::commands::Command::SOURCE_HMI;
}  // namespace

typedef std::shared_ptr<BCSetAppPropertiesRequest> BCSetAppPropertiesRequestPtr;

class BCSetAppPropertiesRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  BCSetAppPropertiesRequestTest() : mock_app_(CreateMockApp()) {
    ON_CALL((*mock_app_), app_id()).WillByDefault(Return(kAppId));
    ON_CALL(app_mngr_, event_dispatcher())
        .WillByDefault(ReturnRef(event_dispatcher_));
    ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app_));
    ON_CALL(app_mngr_, application_by_hmi_app(_))
        .WillByDefault(Return(mock_app_));
    ON_CALL(app_mngr_, GetRPCService())
        .WillByDefault(ReturnRef(mock_rpc_service_));
    ON_CALL(mock_rpc_service_, ManageHMICommand(_, _))
        .WillByDefault(Return(true));
  }

  MessageSharedPtr CreateMessageWithCustomProperties(
      const smart_objects::SmartObject& properties) {
    MessageSharedPtr msg = CreateMessage();
    (*msg)[strings::params][strings::correlation_id] = kCorrelationId;
    (*msg)[strings::msg_params][strings::app_id] = kAppId;
    (*msg)[strings::msg_params][strings::properties] = properties;
    return msg;
  }

  MockAppPtr mock_app_;
};

TEST_F(BCSetAppPropertiesRequestTest,
       Run_NoAppPropertyChanges_AppIsSame_FailToUpdateAppList) {
  smart_objects::SmartObject properties(smart_objects::SmartType_Map);
  properties[strings::policy_app_id] = kPolicyAppId;

  MessageSharedPtr msg = CreateMessageWithCustomProperties(properties);
  auto command = CreateCommand<BCSetAppPropertiesRequest>(msg);

  const auto change_status =
      policy::PolicyHandler::AppPropertiesState::NO_CHANGES;
  ON_CALL(mock_policy_handler_,
          GetAppPropertiesStatus(properties, kPolicyAppId))
      .WillByDefault(Return(change_status));
  ON_CALL(mock_policy_handler_, IsNewApplication(kPolicyAppId))
      .WillByDefault(Return(false));

  MessageSharedPtr notification = CreateMessage();
  ON_CALL(mock_message_helper_,
          CreateOnAppPropertiesChangeNotification(kPolicyAppId, _))
      .WillByDefault(Return(notification));

  EXPECT_CALL(mock_message_helper_,
              CreateOnAppPropertiesChangeNotification(_, _))
      .Times(0);
  EXPECT_CALL(mock_policy_handler_,
              GetAppPropertiesStatus(properties, kPolicyAppId));
  EXPECT_CALL(mock_policy_handler_, OnSetAppProperties(properties));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(
              hmi_apis::FunctionID::BasicCommunication_SetAppProperties),
          kSource));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(notification, kSource))
      .Times(0);
  EXPECT_CALL(app_mngr_, CreatePendingLocalApplication(kPolicyAppId)).Times(0);
  EXPECT_CALL(app_mngr_, SendUpdateAppList()).Times(0);

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(BCSetAppPropertiesRequestTest,
       Run_AppAuthTokenChanged_AppIsSame_NoUpdateAppList) {
  smart_objects::SmartObject properties(smart_objects::SmartType_Map);
  properties[strings::auth_token] = kToken;
  properties[strings::policy_app_id] = kPolicyAppId;

  MessageSharedPtr msg = CreateMessageWithCustomProperties(properties);
  auto command = CreateCommand<BCSetAppPropertiesRequest>(msg);

  const auto change_status =
      policy::PolicyHandler::AppPropertiesState::AUTH_TOKEN_CHANGED;
  ON_CALL(mock_policy_handler_,
          GetAppPropertiesStatus(properties, kPolicyAppId))
      .WillByDefault(Return(change_status));
  ON_CALL(mock_policy_handler_, IsNewApplication(kPolicyAppId))
      .WillByDefault(Return(false));

  MessageSharedPtr notification = CreateMessage();
  ON_CALL(mock_message_helper_,
          CreateOnAppPropertiesChangeNotification(kPolicyAppId, _))
      .WillByDefault(Return(notification));

  EXPECT_CALL(mock_message_helper_,
              CreateOnAppPropertiesChangeNotification(kPolicyAppId, _));
  EXPECT_CALL(mock_policy_handler_,
              GetAppPropertiesStatus(properties, kPolicyAppId));
  EXPECT_CALL(mock_policy_handler_, OnSetAppProperties(properties));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(
              hmi_apis::FunctionID::BasicCommunication_SetAppProperties),
          kSource));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(notification, kSource));
  EXPECT_CALL(app_mngr_, CreatePendingLocalApplication(kPolicyAppId)).Times(0);
  EXPECT_CALL(app_mngr_, SendUpdateAppList()).Times(0);

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(BCSetAppPropertiesRequestTest,
       Run_NoAppPropetiesChanged_AppIsNewAndEnabled_AppListUpdated) {
  smart_objects::SmartObject properties(smart_objects::SmartType_Map);
  properties[strings::policy_app_id] = kPolicyAppId;
  properties[strings::enabled] = true;

  MessageSharedPtr msg = CreateMessageWithCustomProperties(properties);
  auto command = CreateCommand<BCSetAppPropertiesRequest>(msg);

  const auto change_status =
      policy::PolicyHandler::AppPropertiesState::NO_CHANGES;
  ON_CALL(mock_policy_handler_,
          GetAppPropertiesStatus(properties, kPolicyAppId))
      .WillByDefault(Return(change_status));
  ON_CALL(mock_policy_handler_, IsNewApplication(kPolicyAppId))
      .WillByDefault(Return(true));

  MessageSharedPtr notification = CreateMessage();
  ON_CALL(mock_message_helper_,
          CreateOnAppPropertiesChangeNotification(kPolicyAppId, _))
      .WillByDefault(Return(notification));

  EXPECT_CALL(mock_message_helper_,
              CreateOnAppPropertiesChangeNotification(kPolicyAppId, _));
  EXPECT_CALL(mock_policy_handler_,
              GetAppPropertiesStatus(properties, kPolicyAppId));
  EXPECT_CALL(mock_policy_handler_, OnSetAppProperties(properties));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(
              hmi_apis::FunctionID::BasicCommunication_SetAppProperties),
          kSource));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(notification, kSource));
  EXPECT_CALL(app_mngr_, CreatePendingLocalApplication(kPolicyAppId));
  EXPECT_CALL(app_mngr_, SendUpdateAppList());

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(BCSetAppPropertiesRequestTest,
       Run_AppEnabledFlagChanged_AppListUpdated) {
  smart_objects::SmartObject properties(smart_objects::SmartType_Map);
  properties[strings::policy_app_id] = kPolicyAppId;
  properties[strings::enabled] = true;

  MessageSharedPtr msg = CreateMessageWithCustomProperties(properties);
  auto command = CreateCommand<BCSetAppPropertiesRequest>(msg);

  const auto change_status =
      policy::PolicyHandler::AppPropertiesState::ENABLED_FLAG_SWITCH;
  ON_CALL(mock_policy_handler_,
          GetAppPropertiesStatus(properties, kPolicyAppId))
      .WillByDefault(Return(change_status));
  ON_CALL(mock_policy_handler_, IsNewApplication(kPolicyAppId))
      .WillByDefault(Return(false));

  MessageSharedPtr notification = CreateMessage();
  ON_CALL(mock_message_helper_,
          CreateOnAppPropertiesChangeNotification(kPolicyAppId, _))
      .WillByDefault(Return(notification));

  EXPECT_CALL(mock_message_helper_,
              CreateOnAppPropertiesChangeNotification(kPolicyAppId, _));
  EXPECT_CALL(mock_policy_handler_,
              GetAppPropertiesStatus(properties, kPolicyAppId));
  EXPECT_CALL(mock_policy_handler_, OnSetAppProperties(properties));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(
              hmi_apis::FunctionID::BasicCommunication_SetAppProperties),
          kSource));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(notification, kSource));
  EXPECT_CALL(app_mngr_, CreatePendingLocalApplication(kPolicyAppId));
  EXPECT_CALL(app_mngr_, SendUpdateAppList());

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(BCSetAppPropertiesRequestTest,
       Run_AppEnabledFlagChangedToFalse_AppRemoveFromList_AppListUpdated) {
  smart_objects::SmartObject properties(smart_objects::SmartType_Map);
  properties[strings::policy_app_id] = kPolicyAppId;
  properties[strings::enabled] = false;

  MessageSharedPtr msg = CreateMessageWithCustomProperties(properties);
  auto command = CreateCommand<BCSetAppPropertiesRequest>(msg);

  const auto change_status =
      policy::PolicyHandler::AppPropertiesState::ENABLED_FLAG_SWITCH;
  ON_CALL(mock_policy_handler_,
          GetAppPropertiesStatus(properties, kPolicyAppId))
      .WillByDefault(Return(change_status));
  ON_CALL(mock_policy_handler_, IsNewApplication(kPolicyAppId))
      .WillByDefault(Return(false));

  MessageSharedPtr notification = CreateMessage();
  ON_CALL(mock_message_helper_,
          CreateOnAppPropertiesChangeNotification(kPolicyAppId, _))
      .WillByDefault(Return(notification));

  EXPECT_CALL(mock_message_helper_,
              CreateOnAppPropertiesChangeNotification(kPolicyAppId, _));
  EXPECT_CALL(mock_policy_handler_,
              GetAppPropertiesStatus(properties, kPolicyAppId));
  EXPECT_CALL(mock_policy_handler_, OnSetAppProperties(properties));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageHMICommand(
          HMIResultCodeIs(
              hmi_apis::FunctionID::BasicCommunication_SetAppProperties),
          kSource));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(notification, kSource));
  EXPECT_CALL(app_mngr_, CreatePendingLocalApplication(kPolicyAppId)).Times(0);
  EXPECT_CALL(app_mngr_, RemovePendingApplication(kPolicyAppId));
  EXPECT_CALL(app_mngr_, SendUpdateAppList());

  ASSERT_TRUE(command->Init());
  command->Run();
}

}  // namespace bc_set_app_properties_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
