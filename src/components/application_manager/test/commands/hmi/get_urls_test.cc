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
#include "application_manager/message.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/command.h"
#include "application_manager/commands/hmi/get_urls.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "commands/commands_test.h"
#include "commands/command_request_test.h"
#include "hmi/request_from_hmi.h"
#include "policy/mock_policy_manager.h"
#include "application_manager/event_engine/event_dispatcher.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace get_urls {

using namespace hmi_apis;
using namespace policy;
using ::utils::SharedPtr;
using ::testing::NiceMock;
using ::testing::SetArgReferee;
using ::test::components::application_manager_test::MockApplication;
namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;
namespace am = ::application_manager;
namespace strings = ::application_manager::strings;
using am::commands::RequestFromHMI;
using am::commands::GetUrls;
using am::commands::CommandImpl;
using policy::PolicyHandler;
using policy_test::MockPolicyHandlerInterface;

typedef SharedPtr<RequestFromHMI> RequestFromHMIPtr;

namespace {
const uint32_t kInvalidAppId_ = 0u;
const uint32_t kAppIdForSending = 1u;
const uint32_t kConnectionKey = 2u;
const std::string kInitialService = "0x0";
const std::string kPolicyService = "7";
const std::string kDefaultUrl = "URL is not found";
const std::string kDefaultId = "default";
}  // namespace

class GetUrlsTest : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  policy_test::MockPolicyHandlerInterface policy_handler_;
  utils::SharedPtr<policy_manager_test::MockPolicyManager> mock_policy_manager_;
  NiceMock<event_engine_test::MockEventDispatcher> mock_event_dispatcher_;
  ApplicationSharedPtr app;

  virtual void SetUp() OVERRIDE {
    ON_CALL(mock_app_manager_, GetPolicyHandler())
        .WillByDefault(ReturnRef(policy_handler_));
    mock_policy_manager_ =
        utils::MakeShared<policy_manager_test::MockPolicyManager>();
    ASSERT_TRUE(mock_policy_manager_);
  }
};

TEST_F(GetUrlsTest, RUN_SUCCESS) {
  MessageSharedPtr command_msg(
      CreateMessage(NsSmartDeviceLink::NsSmartObjects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::number] = "123";
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][am::hmi_request::service] =
      kInitialService;

  ON_CALL(mock_app_manager_, event_dispatcher())
      .WillByDefault(ReturnRef(mock_event_dispatcher_));

  RequestFromHMIPtr command(CreateCommand<GetUrls>(command_msg));

  EXPECT_CALL(mock_app_manager_, GetPolicyHandler()).Times(2);

  EXPECT_CALL(policy_handler_, GetServiceUrls(kInitialService, _));

  EXPECT_CALL(policy_handler_, PolicyEnabled()).WillOnce(Return(true));

  command->Run();
}
TEST_F(GetUrlsTest, RUN_PolicyNotEnabled_UNSUCCESS) {
  MessageSharedPtr command_msg(
      CreateMessage(NsSmartDeviceLink::NsSmartObjects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::number] = "123";
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][am::hmi_request::service] =
      kInitialService;

  ON_CALL(mock_app_manager_, event_dispatcher())
      .WillByDefault(ReturnRef(mock_event_dispatcher_));

  RequestFromHMIPtr command(CreateCommand<GetUrls>(command_msg));

  EXPECT_CALL(mock_app_manager_, GetPolicyHandler());

  EXPECT_CALL(policy_handler_, PolicyEnabled()).WillOnce(Return(false));

  EXPECT_CALL(mock_app_manager_, ManageHMICommand(command_msg))
      .WillOnce(Return(true));

  command->Run();

  EXPECT_EQ((*command_msg)[strings::params][strings::message_type].asInt(),
            am::MessageType::kResponse);
  EXPECT_EQ((*command_msg)[strings::params][am::hmi_response::code].asInt(),
            Common_Result::DATA_NOT_AVAILABLE);
}
TEST_F(GetUrlsTest, RUN_EmptyEndpoints_UNSUCCESS) {
  MessageSharedPtr command_msg(
      CreateMessage(NsSmartDeviceLink::NsSmartObjects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::number] = "123";
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][am::hmi_request::service] =
      kInitialService;

  ON_CALL(mock_app_manager_, event_dispatcher())
      .WillByDefault(ReturnRef(mock_event_dispatcher_));

  RequestFromHMIPtr command(CreateCommand<GetUrls>(command_msg));

  EXPECT_CALL(mock_app_manager_, GetPolicyHandler()).Times(2);

  EXPECT_CALL(policy_handler_, GetServiceUrls(kInitialService, _));

  EXPECT_CALL(policy_handler_, PolicyEnabled()).WillOnce(Return(true));

  EXPECT_CALL(mock_app_manager_, ManageHMICommand(command_msg))
      .WillOnce(Return(true));

  command->Run();

  EXPECT_EQ((*command_msg)[strings::params][strings::message_type].asInt(),
            am::MessageType::kResponse);
  EXPECT_EQ((*command_msg)[strings::params][am::hmi_response::code].asInt(),
            Common_Result::DATA_NOT_AVAILABLE);
}

#ifdef EXTENDED_POLICY
TEST_F(GetUrlsTest, ProcessPolicyServiceURLs_SUCCESS) {
  MessageSharedPtr command_msg(
      CreateMessage(NsSmartDeviceLink::NsSmartObjects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::number] = "123";
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][am::hmi_request::service] =
      kPolicyService;

  ON_CALL(mock_app_manager_, event_dispatcher())
      .WillByDefault(ReturnRef(mock_event_dispatcher_));

  RequestFromHMIPtr command(CreateCommand<GetUrls>(command_msg));

  EXPECT_CALL(mock_app_manager_, GetPolicyHandler()).Times(3);
  EXPECT_CALL(policy_handler_, PolicyEnabled()).WillOnce(Return(true));

  EndpointUrls endpoints_;
  EndpointData data(kDefaultUrl);
  endpoints_.push_back(data);

  ON_CALL(policy_handler_, GetServiceUrls(kPolicyService, _))
      .WillByDefault(SetArgReferee<1>(endpoints_));
  EXPECT_CALL(policy_handler_, GetServiceUrls(kPolicyService, _));

  MockAppPtr mock_app = CreateMockApp();

  EXPECT_CALL(policy_handler_, GetAppIdForSending())
      .WillOnce(Return(kAppIdForSending));

  EXPECT_CALL(mock_app_manager_, application(kAppIdForSending))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, app_id()).WillOnce(Return(kAppIdForSending));
  EXPECT_CALL(mock_app_manager_, ManageHMICommand(command_msg))
      .WillRepeatedly(Return(true));

  command->Run();

  EXPECT_FALSE((*command_msg)[am::strings::msg_params].keyExists(
      am::hmi_request::service));

  EXPECT_EQ((*command_msg)[strings::params][strings::message_type].asInt(),
            am::MessageType::kResponse);
  EXPECT_EQ((*command_msg)[strings::params][am::hmi_response::code].asInt(),
            Common_Result::SUCCESS);

  EXPECT_EQ(kAppIdForSending,
            (*command_msg)[am::strings::msg_params][am::hmi_response::urls][0]
                          [strings::app_id].asInt());
  EXPECT_EQ(kDefaultUrl,
            (*command_msg)[am::strings::msg_params][am::hmi_response::urls][0]
                          [strings::url].asString());
}
#endif
TEST_F(GetUrlsTest, ProcessServiceURLs_SUCCESS) {
  MessageSharedPtr command_msg(
      CreateMessage(NsSmartDeviceLink::NsSmartObjects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::number] = "123";
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][am::hmi_request::service] =
      kInitialService;
  (*command_msg)[am::strings::msg_params][am::hmi_response::urls][0] =
      kDefaultUrl;
  (*command_msg)[am::strings::msg_params][am::hmi_response::urls][0]
                [am::hmi_response::policy_app_id].asString() = "1";

  ON_CALL(mock_app_manager_, event_dispatcher())
      .WillByDefault(ReturnRef(mock_event_dispatcher_));

  RequestFromHMIPtr command(CreateCommand<GetUrls>(command_msg));

  EXPECT_CALL(mock_app_manager_, GetPolicyHandler()).Times(2);
  EXPECT_CALL(policy_handler_, PolicyEnabled()).WillOnce(Return(true));

  EndpointUrls endpoints_;
  EndpointData data(kDefaultUrl);
  data.app_id = "1";
  endpoints_.push_back(data);

  ON_CALL(policy_handler_, GetServiceUrls(kInitialService, _))
      .WillByDefault(SetArgReferee<1>(endpoints_));
  EXPECT_CALL(policy_handler_, GetServiceUrls(kInitialService, _));

  command->Run();

  EXPECT_FALSE((*command_msg)[am::strings::msg_params].keyExists(
      am::hmi_request::service));
  EXPECT_EQ((*command_msg)[am::strings::msg_params][am::hmi_response::urls][0]
                          [am::strings::url].asString(),
            kDefaultUrl);

  EXPECT_EQ((*command_msg)[am::strings::msg_params][am::hmi_response::urls][0]
                          [am::hmi_response::policy_app_id].asString(),
            endpoints_[0].app_id);
}
TEST_F(GetUrlsTest, ProcessServiceURLs_PolicyDefaultId_SUCCESS) {
  MessageSharedPtr command_msg(
      CreateMessage(NsSmartDeviceLink::NsSmartObjects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::number] = "123";
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][am::hmi_request::service] =
      kInitialService;
  (*command_msg)[am::strings::msg_params][am::hmi_response::urls][0] =
      kDefaultUrl;
  (*command_msg)[am::strings::msg_params][am::hmi_response::urls][0]
                [am::hmi_response::policy_app_id].asString() = kDefaultId;

  ON_CALL(mock_app_manager_, event_dispatcher())
      .WillByDefault(ReturnRef(mock_event_dispatcher_));

  RequestFromHMIPtr command(CreateCommand<GetUrls>(command_msg));

  EXPECT_CALL(mock_app_manager_, GetPolicyHandler()).Times(2);
  EXPECT_CALL(policy_handler_, PolicyEnabled()).WillOnce(Return(true));

  EndpointUrls endpoints_;
  EndpointData data(kDefaultUrl);
  endpoints_.push_back(data);

  ON_CALL(policy_handler_, GetServiceUrls(kInitialService, _))
      .WillByDefault(SetArgReferee<1>(endpoints_));
  EXPECT_CALL(policy_handler_, GetServiceUrls(kInitialService, _));

  command->Run();

  EXPECT_FALSE((*command_msg)[am::strings::msg_params].keyExists(
      am::hmi_request::service));
  EXPECT_FALSE(
      (*command_msg)[am::strings::msg_params][am::hmi_response::urls][0]
          .keyExists(am::hmi_response::policy_app_id));
}

}  // namespace get_urls
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
