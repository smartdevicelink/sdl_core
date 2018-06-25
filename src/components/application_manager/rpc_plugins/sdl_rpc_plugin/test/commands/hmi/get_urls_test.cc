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

#include <stdint.h>
#include <string>

#include "gtest/gtest.h"
#include "application_manager/message.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/command.h"
#include "hmi/get_urls.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/commands/request_from_hmi.h"
#include "policy/mock_policy_manager.h"
#include "application_manager/event_engine/event_dispatcher.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace get_urls {

using namespace hmi_apis;
using namespace policy;
using ::testing::NiceMock;
using ::testing::_;
using ::testing::SetArgReferee;
using ::test::components::application_manager_test::MockApplication;
namespace am = ::application_manager;
namespace strings = ::application_manager::strings;
using am::commands::RequestFromHMI;
using sdl_rpc_plugin::commands::GetUrls;
using am::commands::CommandImpl;
using policy::PolicyHandler;
using policy_test::MockPolicyHandlerInterface;

typedef std::shared_ptr<RequestFromHMI> RequestFromHMIPtr;

namespace {
const uint32_t kInvalidAppId_ = 0u;
const uint32_t kAppIdForSending = 1u;
const uint32_t kConnectionKey = 2u;
const uint32_t kServiceType = 0u;
const std::string kInitialService = "0x0";
const std::string kPolicyService = "7";
const std::string kDefaultUrl = "URL is not found";
const std::string kDefaultId = "default";
const std::string kPolicyAppId = "policy_app_id";
}  // namespace

class GetUrlsTest : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  MessageSharedPtr command_msg_;
  RequestFromHMIPtr request_command_;

  GetUrlsTest() {
    command_msg_ =
        CreateMessage(ns_smart_device_link::ns_smart_objects::SmartType_Map);
    (*command_msg_)[am::strings::params][am::strings::connection_key] =
        kConnectionKey;
    (*command_msg_)[am::strings::msg_params][am::hmi_request::service] =
        kInitialService;

    request_command_ = CreateCommand<GetUrls>(command_msg_);
  }
};

TEST_F(GetUrlsTest, RUN_SUCCESS) {
  EXPECT_CALL(mock_policy_handler_, PolicyEnabled()).WillOnce(Return(true));
  //  EXPECT_CALL(mock_policy_handler_, GetUpdateUrls(_, _));

  request_command_->Run();
}

TEST_F(GetUrlsTest, RUN_PolicyNotEnabled_UNSUCCESS) {
  EXPECT_CALL(mock_policy_handler_, PolicyEnabled()).WillOnce(Return(false));

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(command_msg_))
      .WillOnce(Return(true));

  request_command_->Run();

  EXPECT_EQ(am::MessageType::kResponse,
            (*command_msg_)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(Common_Result::DATA_NOT_AVAILABLE,
            (*command_msg_)[strings::params][am::hmi_response::code].asInt());
}

TEST_F(GetUrlsTest, RUN_EmptyEndpoints_UNSUCCESS) {
  EndpointUrls endpoints_;
  EXPECT_CALL(mock_policy_handler_, GetUpdateUrls(kServiceType, _))
      .WillOnce(SetArgReferee<1>(endpoints_));
  EXPECT_CALL(mock_policy_handler_, PolicyEnabled()).WillOnce(Return(true));

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(command_msg_))
      .WillOnce(Return(true));

  request_command_->Run();

  EXPECT_EQ(am::MessageType::kResponse,
            (*command_msg_)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(Common_Result::DATA_NOT_AVAILABLE,
            (*command_msg_)[strings::params][am::hmi_response::code].asInt());
}

#ifdef EXTENDED_POLICY
TEST_F(GetUrlsTest, ProcessPolicyServiceURLs_SUCCESS) {
  (*command_msg_)[am::strings::msg_params][am::hmi_request::service] =
      kPolicyService;

  EXPECT_CALL(mock_policy_handler_, PolicyEnabled()).WillOnce(Return(true));

  EndpointUrls endpoints_;
  EndpointData data(kDefaultUrl);
  endpoints_.push_back(data);

  EXPECT_CALL(mock_policy_handler_, GetUpdateUrls(kPolicyService, _))
      .WillOnce(SetArgReferee<1>(endpoints_));

  MockAppPtr mock_app = CreateMockApp();

  EXPECT_CALL(mock_policy_handler_, GetAppIdForSending())
      .WillOnce(Return(kAppIdForSending));

  EXPECT_CALL(app_mngr_, application(kAppIdForSending))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, app_id()).WillOnce(Return(kAppIdForSending));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(command_msg_))
      .WillOnce(Return(true));

  request_command_->Run();

  EXPECT_FALSE((*command_msg_)[am::strings::msg_params].keyExists(
      am::hmi_request::service));

  EXPECT_EQ(am::MessageType::kResponse,
            (*command_msg_)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(Common_Result::SUCCESS,
            (*command_msg_)[strings::params][am::hmi_response::code].asInt());

  EXPECT_EQ(kAppIdForSending,
            (*command_msg_)[am::strings::msg_params][am::hmi_response::urls][0]
                           [strings::app_id].asInt());
  EXPECT_EQ(kDefaultUrl,
            (*command_msg_)[am::strings::msg_params][am::hmi_response::urls][0]
                           [strings::url].asString());
}

TEST_F(GetUrlsTest, ProcessPolicyServiceURLs_IncorrectIdForSending_UNSUCCESS) {
  (*command_msg_)[am::strings::msg_params][am::hmi_request::service] =
      kPolicyService;

  EXPECT_CALL(mock_policy_handler_, PolicyEnabled()).WillOnce(Return(true));

  EndpointUrls endpoints_;
  EndpointData data(kDefaultUrl);
  endpoints_.push_back(data);

  EXPECT_CALL(mock_policy_handler_, GetUpdateUrls(kPolicyService, _))
      .WillOnce(SetArgReferee<1>(endpoints_));

  EXPECT_CALL(mock_policy_handler_, GetAppIdForSending())
      .WillOnce(Return(kInvalidAppId_));

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(command_msg_))
      .WillOnce(Return(true));

  EXPECT_CALL(app_mngr_, application(kInvalidAppId_)).Times(0);

  request_command_->Run();
}

TEST_F(GetUrlsTest, ProcessPolicyServiceURLs_ApplicationIsNotValid_UNSUCCESS) {
  (*command_msg_)[am::strings::msg_params][am::hmi_request::service] =
      kPolicyService;

  EXPECT_CALL(mock_policy_handler_, PolicyEnabled()).WillOnce(Return(true));

  EndpointUrls endpoints_;
  EndpointData data(kDefaultUrl);
  endpoints_.push_back(data);

  EXPECT_CALL(mock_policy_handler_, GetUpdateUrls(kPolicyService, _))
      .WillOnce(SetArgReferee<1>(endpoints_));

  MockAppPtr invalid_mock_app;

  EXPECT_CALL(mock_policy_handler_, GetAppIdForSending())
      .WillOnce(Return(kAppIdForSending));

  EXPECT_CALL(app_mngr_, application(kAppIdForSending))
      .WillOnce(Return(invalid_mock_app));

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(command_msg_))
      .WillOnce(Return(true));

  request_command_->Run();

  EXPECT_EQ(am::MessageType::kResponse,
            (*command_msg_)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(Common_Result::DATA_NOT_AVAILABLE,
            (*command_msg_)[strings::params][am::hmi_response::code].asInt());
}

TEST_F(GetUrlsTest, ProcessPolicyServiceURLs_FoundURLForApplication_SUCCESS) {
  (*command_msg_)[am::strings::msg_params][am::hmi_request::service] =
      kPolicyService;

  EXPECT_CALL(mock_policy_handler_, PolicyEnabled()).WillOnce(Return(true));

  EndpointUrls endpoints_;
  EndpointData data(kDefaultUrl);
  data.app_policy_id = kPolicyAppId;
  endpoints_.push_back(data);

  EXPECT_CALL(mock_policy_handler_, GetUpdateUrls(kPolicyService, _))
      .WillOnce(SetArgReferee<1>(endpoints_));

  MockAppPtr mock_app = CreateMockApp();

  EXPECT_CALL(mock_policy_handler_, GetAppIdForSending())
      .WillOnce(Return(kAppIdForSending));

  EXPECT_CALL(app_mngr_, application(kAppIdForSending))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(*mock_app, policy_app_id()).WillOnce(Return(kPolicyAppId));

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(command_msg_))
      .WillOnce(Return(true));

  request_command_->Run();

  EXPECT_FALSE((*command_msg_)[am::strings::msg_params].keyExists(
      am::hmi_request::service));

  EXPECT_EQ(am::MessageType::kResponse,
            (*command_msg_)[strings::params][strings::message_type].asInt());
  EXPECT_EQ(Common_Result::SUCCESS,
            (*command_msg_)[strings::params][am::hmi_response::code].asInt());
}
#endif

TEST_F(GetUrlsTest, DISABLED_ProcessServiceURLs_SUCCESS) {
  (*command_msg_)[am::strings::msg_params][am::hmi_response::urls][0] =
      kDefaultUrl;
  (*command_msg_)[am::strings::msg_params][am::hmi_response::urls][0]
                 [am::hmi_response::policy_app_id] = "1";

  EXPECT_CALL(mock_policy_handler_, PolicyEnabled()).WillOnce(Return(true));

  EndpointUrls endpoints_;
  EndpointData data(kDefaultUrl);
  data.app_policy_id = "1";
  endpoints_.push_back(data);
  EXPECT_CALL(mock_policy_handler_, GetUpdateUrls(kServiceType, _))
      .WillOnce(SetArgReferee<1>(endpoints_));

  request_command_->Run();

  EXPECT_FALSE((*command_msg_)[am::strings::msg_params].keyExists(
      am::hmi_request::service));
  EXPECT_EQ(kDefaultUrl,
            (*command_msg_)[am::strings::msg_params][am::hmi_response::urls][0]
                           [am::strings::url].asString());
  EXPECT_EQ(endpoints_[0].app_policy_id,
            (*command_msg_)[am::strings::msg_params][am::hmi_response::urls][0]
                           [am::hmi_response::policy_app_id].asString());
}

TEST_F(GetUrlsTest, ProcessServiceURLs_PolicyDefaultId_SUCCESS) {
  (*command_msg_)[am::strings::msg_params][am::hmi_response::urls][0] =
      kDefaultUrl;
  (*command_msg_)[am::strings::msg_params][am::hmi_response::urls][0]
                 [am::hmi_response::policy_app_id] = kDefaultId;

  EXPECT_CALL(mock_policy_handler_, PolicyEnabled()).WillOnce(Return(true));
  EndpointUrls endpoints_;
  EndpointData data(kDefaultUrl);
  endpoints_.push_back(data);
  EXPECT_CALL(mock_policy_handler_, GetUpdateUrls(kServiceType, _))
      .WillOnce(SetArgReferee<1>(endpoints_));
  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application_by_policy_id(_))
      .WillOnce(Return(mock_app));
  request_command_->Run();

  EXPECT_FALSE((*command_msg_)[am::strings::msg_params].keyExists(
      am::hmi_request::service));
  EXPECT_TRUE(
      (*command_msg_)[am::strings::msg_params][am::hmi_response::urls][0]
          .keyExists(am::hmi_response::policy_app_id));
}

}  // namespace get_urls
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
