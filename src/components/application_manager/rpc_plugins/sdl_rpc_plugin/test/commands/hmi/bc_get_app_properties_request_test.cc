/*
 * Copyright (c) 2020, Ford Motor Company
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

#include "application_manager/commands/command_impl.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"
#include "hmi/bc_get_app_properties_request.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace bc_get_app_properties_request_test {

namespace am = ::application_manager;
namespace strings = ::application_manager::strings;

using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::BCGetAppPropertiesRequest;

using testing::DoAll;
using testing::Return;
using testing::ReturnRef;
using testing::SaveArg;

namespace {
const std::string kPolicyAppId = "00001";
const uint32_t kCorrelationId = 1u;
const auto kSource = am::commands::Command::SOURCE_HMI;

const std::string kAuthToken = "auth_token";
const std::string kCertificate = "certificate";
const std::string kTransportType = "WS";
const std::string kHybridAppPreference = "local_app";
const bool kEnabled = true;

const std::string kNickname1 = "nickname1";
const std::string kNickname2 = "nickname2";
}  // namespace

bool CompareAppProperties(const smart_objects::SmartObject& app_propersties_so,
                          const policy::AppProperties& app_properties) {
  return (app_propersties_so[strings::auth_token].asString() ==
          app_properties.auth_token) &&
         (app_propersties_so[strings::transport_type].asString() ==
          app_properties.transport_type) &&
         (app_propersties_so[strings::hybrid_app_preference].asString() ==
          app_properties.hybrid_app_preference);
}

bool CompareNicknames(const smart_objects::SmartObject& nicknames_so,
                      const policy::StringArray& nicknames) {
  if (nicknames_so.length() != nicknames.size()) {
    return false;
  }

  for (size_t i = 0; i < nicknames_so.length(); ++i) {
    if (nicknames_so[i].asString() != nicknames[i]) {
      return false;
    }
  }
  return true;
}

ACTION_P(SetTestAppProperties, app_properties) {
  arg1 = app_properties;
}

ACTION_P(SetTestNickNames, nicknames) {
  *arg1 = nicknames;
}

class BCGetAppPropertiesRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  BCGetAppPropertiesRequestTest() : mock_app_(CreateMockApp()) {
    ON_CALL(app_mngr_, GetRPCService())
        .WillByDefault(ReturnRef(mock_rpc_service_));
  }

  MockAppPtr mock_app_;
};

TEST_F(BCGetAppPropertiesRequestTest, Run_PolicyAppId_Exist_SUCCESS) {
  auto msg = CreateMessage();

  (*msg)[strings::params][strings::correlation_id] = kCorrelationId;
  (*msg)[strings::msg_params][strings::policy_app_id] = kPolicyAppId;
  auto command = CreateCommand<BCGetAppPropertiesRequest>(msg);

  // Fills app properties with default test values
  EXPECT_CALL(mock_policy_handler_, GetAppProperties(kPolicyAppId, _))
      .WillOnce(DoAll(
          SetTestAppProperties(policy::AppProperties("",
                                                     kCertificate,
                                                     kEnabled,
                                                     kAuthToken,
                                                     kTransportType,
                                                     kHybridAppPreference)),
          Return(true)));

  // Fills app nicknames with default test values
  EXPECT_CALL(mock_policy_handler_, GetInitialAppData(kPolicyAppId, _, _))
      .WillOnce(
          DoAll(SetTestNickNames(policy::StringArray{kNickname1, kNickname2}),
                Return(true)));

  auto message_to_hmi = CreateMessage();

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  HMIMessageParametersAre(
                      kCorrelationId,
                      hmi_apis::FunctionID::BasicCommunication_GetAppProperties,
                      hmi_apis::Common_Result::SUCCESS),
                  kSource))
      .WillOnce(DoAll(SaveArg<0>(&message_to_hmi), Return(true)));

  ASSERT_TRUE(command->Init());
  command->Run();

  const smart_objects::SmartObject& sent_app_properties =
      (*message_to_hmi)[strings::msg_params][strings::properties][0];

  EXPECT_FALSE(sent_app_properties.empty());

  const std::string auth_token =
      sent_app_properties[strings::auth_token].asString();
  const std::string transport_type =
      sent_app_properties[strings::transport_type].asString();
  const std::string hybrid_app_preference =
      sent_app_properties[strings::hybrid_app_preference].asString();
  // ENDPOINT should be omitted for local application properties.
  EXPECT_FALSE(sent_app_properties.keyExists(strings::endpoint));

  EXPECT_EQ(kAuthToken, auth_token);
  EXPECT_EQ(kTransportType, transport_type);
  EXPECT_EQ(kHybridAppPreference, hybrid_app_preference);

  EXPECT_TRUE(sent_app_properties.keyExists(strings::nicknames));

  const auto nick_names = sent_app_properties[strings::nicknames];
  EXPECT_EQ(kNickname1, nick_names[0].asString());
  EXPECT_EQ(kNickname2, nick_names[1].asString());
}

TEST_F(BCGetAppPropertiesRequestTest,
       PolicyAppIdExistsInMessage_GetAppPropertiesFailed_UNSUCCESS) {
  auto msg = CreateMessage();

  (*msg)[strings::params][strings::correlation_id] = kCorrelationId;
  (*msg)[strings::msg_params][strings::policy_app_id] = kPolicyAppId;
  auto command = CreateCommand<BCGetAppPropertiesRequest>(msg);

  EXPECT_CALL(mock_policy_handler_, GetAppProperties(kPolicyAppId, _))
      .WillOnce(Return(false));

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  HMIMessageParametersAre(
                      kCorrelationId,
                      hmi_apis::FunctionID::BasicCommunication_GetAppProperties,
                      hmi_apis::Common_Result::DATA_NOT_AVAILABLE),
                  application_manager::commands::Command::SOURCE_SDL_TO_HMI))
      .WillOnce(Return(true));

  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(
    BCGetAppPropertiesRequestTest,
    Run_PolicyAppIdIsAbsentInMessage_AppParametersAreAbsent_NickNamesAreAbsent_UNSUCCESS) {
  auto msg = CreateMessage();

  (*msg)[strings::params][strings::correlation_id] = kCorrelationId;
  auto command = CreateCommand<BCGetAppPropertiesRequest>(msg);

  EXPECT_CALL(mock_policy_handler_, GetApplicationPolicyIDs())
      .WillOnce(Return(policy::StringArray()));

  EXPECT_CALL(mock_policy_handler_, GetAppProperties(_, _)).Times(0);
  EXPECT_CALL(mock_policy_handler_, GetInitialAppData(_, _, _)).Times(0);

  auto message_to_hmi = CreateMessage();

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  HMIMessageParametersAre(
                      kCorrelationId,
                      hmi_apis::FunctionID::BasicCommunication_GetAppProperties,
                      hmi_apis::Common_Result::DATA_NOT_AVAILABLE),
                  Command::SOURCE_SDL_TO_HMI))
      .WillOnce(DoAll(SaveArg<0>(&message_to_hmi), Return(true)));

  ASSERT_TRUE(command->Init());
  command->Run();

  EXPECT_FALSE(
      (*message_to_hmi)[strings::msg_params].keyExists(strings::properties));
}

TEST_F(BCGetAppPropertiesRequestTest,
       Run_PolicyAppIdIsAbsentInMessage_ReturnsAllAppProperties_SUCCESS) {
  auto msg = CreateMessage();

  (*msg)[strings::params][strings::correlation_id] = kCorrelationId;
  auto command = CreateCommand<BCGetAppPropertiesRequest>(msg);

  const std::string test_policy_app_id1 = "test_policy_app_id1";
  const policy::AppProperties test_app1_properties("",
                                                   "cert1",
                                                   true,
                                                   "auth_token1",
                                                   "transport_type1",
                                                   "hybrid_app_preference1");
  const policy::StringArray test_nicknames1{"nickname1", "nickname2"};

  const std::string test_policy_app_id2 = "test_policy_app_id2";
  const policy::AppProperties test_app2_properties("",
                                                   "cert2",
                                                   false,
                                                   "auth_token2",
                                                   "transport_type2",
                                                   "hybrid_app_preference2");
  const policy::StringArray test_nicknames2{"nickname3", "nickname4"};

  EXPECT_CALL(mock_policy_handler_, GetApplicationPolicyIDs())
      .WillOnce(Return(
          policy::StringArray{test_policy_app_id1, test_policy_app_id2}));

  EXPECT_CALL(mock_policy_handler_, GetAppProperties(test_policy_app_id1, _))
      .WillOnce(
          DoAll(SetTestAppProperties(test_app1_properties), Return(true)));
  EXPECT_CALL(mock_policy_handler_,
              GetInitialAppData(test_policy_app_id1, _, _))
      .WillOnce(DoAll(SetTestNickNames(test_nicknames1), Return(true)));

  EXPECT_CALL(mock_policy_handler_, GetAppProperties(test_policy_app_id2, _))
      .WillOnce(
          DoAll(SetTestAppProperties(test_app2_properties), Return(true)));
  EXPECT_CALL(mock_policy_handler_,
              GetInitialAppData(test_policy_app_id2, _, _))
      .WillOnce(DoAll(SetTestNickNames(test_nicknames2), Return(true)));

  auto message_to_hmi = CreateMessage();

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  HMIMessageParametersAre(
                      kCorrelationId,
                      hmi_apis::FunctionID::BasicCommunication_GetAppProperties,
                      hmi_apis::Common_Result::SUCCESS),
                  kSource))
      .WillOnce(DoAll(SaveArg<0>(&message_to_hmi), Return(true)));

  ASSERT_TRUE(command->Init());
  command->Run();

  const auto& sent_app_properties =
      (*message_to_hmi)[strings::msg_params][strings::properties];

  EXPECT_EQ(2u, sent_app_properties.length());

  // Compare test_app1 app_properties with sent app properties
  const auto& sent_test_app1_properties = sent_app_properties[0];
  EXPECT_FALSE(sent_test_app1_properties.keyExists(strings::endpoint));
  EXPECT_TRUE(
      CompareAppProperties(sent_test_app1_properties, test_app1_properties));

  const auto& sent_test_app1_nicknames =
      sent_app_properties[0][strings::nicknames];
  EXPECT_TRUE(CompareNicknames(sent_test_app1_nicknames, test_nicknames1));

  // Compare test_app2 app_properties with sent app properties
  const auto& sent_test_app2_properties = sent_app_properties[1];
  EXPECT_FALSE(sent_test_app2_properties.keyExists(strings::endpoint));
  EXPECT_TRUE(
      CompareAppProperties(sent_test_app2_properties, test_app2_properties));

  const auto& sent_test_app2_nicknames =
      sent_app_properties[1][strings::nicknames];
  EXPECT_TRUE(CompareNicknames(sent_test_app2_nicknames, test_nicknames2));
}

TEST_F(BCGetAppPropertiesRequestTest,
       Run_PolicyAppIdExist_NickNamesAreAbsent_SendEmptyArray_SUCCESS) {
  auto msg = CreateMessage();

  (*msg)[strings::params][strings::correlation_id] = kCorrelationId;
  (*msg)[strings::msg_params][strings::policy_app_id] = kPolicyAppId;
  auto command = CreateCommand<BCGetAppPropertiesRequest>(msg);

  // Fills app properties with default test values
  EXPECT_CALL(mock_policy_handler_, GetAppProperties(kPolicyAppId, _))
      .WillOnce(DoAll(
          SetTestAppProperties(policy::AppProperties("",
                                                     kCertificate,
                                                     kEnabled,
                                                     kAuthToken,
                                                     kTransportType,
                                                     kHybridAppPreference)),
          Return(true)));

  // In the case when nicknames array is empty, SDL should forward the empty
  // array to HMI in the app properties
  EXPECT_CALL(mock_policy_handler_, GetInitialAppData(kPolicyAppId, _, _))
      .WillOnce(DoAll(SetTestNickNames(policy::StringArray{}), Return(true)));

  auto message_to_hmi = CreateMessage();

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(
                  HMIMessageParametersAre(
                      kCorrelationId,
                      hmi_apis::FunctionID::BasicCommunication_GetAppProperties,
                      hmi_apis::Common_Result::SUCCESS),
                  kSource))
      .WillOnce(DoAll(SaveArg<0>(&message_to_hmi), Return(true)));

  ASSERT_TRUE(command->Init());
  command->Run();

  const auto& sent_app_properties =
      (*message_to_hmi)[strings::msg_params][strings::properties];

  EXPECT_TRUE(sent_app_properties[0].keyExists(strings::nicknames));
  EXPECT_TRUE(sent_app_properties[0][strings::nicknames].empty());
}

}  // namespace bc_get_app_properties_request_test
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
