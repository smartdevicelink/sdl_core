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
#include <vector>
#include <string>
#include <map>

#include "mobile/change_registration_request.h"
#include "mobile/change_registration_response.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace change_registration {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::Mock;
using ::testing::InSequence;

namespace am = ::application_manager;

using am::commands::ChangeRegistrationRequest;
using am::commands::ChangeRegistrationResponse;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;

typedef SharedPtr<ChangeRegistrationRequest> ChangeRegistrationRequestPtr;
typedef SharedPtr<ChangeRegistrationResponse> ChangeRegistrationResponsePtr;
typedef NiceMock<
    ::test::components::application_manager_test::MockHMICapabilities>
    MockHMICapabilities;

typedef std::vector<hmi_apis::FunctionID::eType> FunctionIdsArray;

ACTION_TEMPLATE(SetArgPointer,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_1_VALUE_PARAMS(vec)) {
  *std::tr1::get<k>(args) = *vec;
}

namespace {
const uint32_t kConnectionKey = 2u;
const int32_t kMenuId = 5;
const std::string kAppName1 = "app_name_1";
const std::string kAppName2 = "app_name_2";
const std::string kVrSynonyms1 = "vr_synonyms_1";
const std::string kVrSynonyms2 = "vr_synonyms_2";
const std::string kPolicyAppId = "policy_app_id";
}  // namespace

class ChangeRegistrationRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  ChangeRegistrationRequestTest()
      : msg_(CreateMessage())
      , mock_app_(CreateMockApp())
      , name_(kAppName2)
      , applications_(application_set_, applications_lock_) {}

  ~ChangeRegistrationRequestTest() {
    // Fix DataAccessor release and WinQt crash
    Mock::VerifyAndClearExpectations(&mock_app_manager_);
  }

  void CreateDefaultMessage(MessageSharedPtr message) {
    (*message)[am::strings::msg_params][am::strings::app_name] = kAppName1;
    (*message)[am::strings::params][am::strings::connection_key] =
        kConnectionKey;
    (*message)[am::strings::msg_params][am::strings::vr_synonyms][0] =
        kVrSynonyms1;
    (*message)[am::strings::msg_params][am::strings::hmi_display_language] =
        hmi_apis::Common_Language::EN_US;
  }

 protected:
  void SetUp() OVERRIDE {
    (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    command_ = CreateCommand<ChangeRegistrationRequest>(msg_);
    application_set_.insert(mock_app_);
    ON_CALL(mock_app_manager_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(mock_app_manager_, applications())
        .WillByDefault(Return(applications_));
    ON_CALL(mock_app_manager_, hmi_capabilities())
        .WillByDefault(ReturnRef(mock_hmi_capabilities_));
    ON_CALL(mock_app_manager_, GetPolicyHandler())
        .WillByDefault(ReturnRef(policy_interface_));
  }

  void SetMockAppName() {
    ON_CALL(*mock_app_, name()).WillByDefault(ReturnRef(name_));
  }

  void FillEventArray(FunctionIdsArray& out_codes_array) {
    out_codes_array.push_back(hmi_apis::FunctionID::UI_ChangeRegistration);
    out_codes_array.push_back(hmi_apis::FunctionID::VR_ChangeRegistration);
    out_codes_array.push_back(hmi_apis::FunctionID::TTS_ChangeRegistration);
  }

  void AddHmiDisplayLang(MessageSharedPtr msg) {
    (*msg)[am::strings::msg_params][am::strings::hmi_display_language][0] =
        hmi_apis::Common_Language::EN_US;
    (*msg)[am::strings::msg_params][am::strings::hmi_display_language][1] =
        hmi_apis::Common_Language::ES_MX;
  }

  void AddHmiResponseLang(MessageSharedPtr msg) {
    (*msg)[am::strings::msg_params][am::hmi_response::languages][0] =
        hmi_apis::Common_Language::EN_US;
    (*msg)[am::strings::msg_params][am::hmi_response::languages][1] =
        hmi_apis::Common_Language::ES_MX;
  }

  void ExpectationHmiCapabilities() {
    EXPECT_CALL(mock_hmi_capabilities_, is_ui_cooperating())
        .WillOnce(Return(false));
    EXPECT_CALL(mock_hmi_capabilities_, is_vr_cooperating())
        .WillOnce(Return(false));
    EXPECT_CALL(mock_hmi_capabilities_, is_tts_cooperating())
        .WillOnce(Return(false));
    EXPECT_CALL(mock_app_manager_, GetNextHMICorrelationID()).Times(0);
  }

  void ExpectVrSynonyms(MessageSharedPtr vr_synonyms_message) {
    vr_synonyms_ = &(*vr_synonyms_message)[am::strings::msg_params]
                                          [am::strings::vr_synonyms];
    ON_CALL(*mock_app_, vr_synonyms()).WillByDefault(Return(vr_synonyms_));
  }

  ChangeRegistrationRequestPtr command_;
  MessageSharedPtr msg_;
  MockAppPtr mock_app_;
  utils::custom_string::CustomString name_;
  smart_objects::SmartObject* vr_synonyms_;

  am::ApplicationSet application_set_;
  sync_primitives::Lock applications_lock_;
  DataAccessor<am::ApplicationSet> applications_;
  MockHMICapabilities mock_hmi_capabilities_;
  NiceMock<policy_test::MockPolicyHandlerInterface> policy_interface_;
};

class ChangeRegistrationResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(ChangeRegistrationRequestTest, OnEvent_UnknownEvent_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  EXPECT_CALL(mock_app_manager_, application(_)).Times(0);
  command_->on_event(event);
}

TEST_F(ChangeRegistrationRequestTest, OnEvent_InvalidApp_UNSUCCESS) {
  FunctionIdsArray function_id_vector;
  FillEventArray(function_id_vector);

  MockAppPtr invalid_app;
  std::vector<hmi_apis::FunctionID::eType>::iterator it =
      function_id_vector.begin();

  MessageSharedPtr event_message(CreateMessage());
  (*event_message)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_REQUEST;
  (*event_message)[am::strings::msg_params] = 0;

  for (; it != function_id_vector.end(); ++it) {
    Event event(*it);
    event.set_smart_object(*event_message);
    EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
        .WillOnce(Return(invalid_app));
    command_->on_event(event);
  }
}

TEST_F(ChangeRegistrationRequestTest, OnEvent_ValidApp_SUCCESS) {
  FunctionIdsArray function_id_vector;
  FillEventArray(function_id_vector);

  std::vector<hmi_apis::FunctionID::eType>::iterator it =
      function_id_vector.begin();

  MessageSharedPtr event_message(CreateMessage());
  (*event_message)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*event_message)[am::strings::msg_params] = 0;

  for (; it != function_id_vector.end(); ++it) {
    Event event(*it);
    event.set_smart_object(*event_message);
    EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(*mock_app_, set_ui_language(_));
    command_->on_event(event);
    EXPECT_EQ((*msg_)[am::strings::params][am::strings::function_id].asInt(),
              static_cast<mobile_apis::FunctionID::eType>(
                  mobile_apis::FunctionID::eType::ChangeRegistrationID));
  }
}

TEST_F(ChangeRegistrationRequestTest, Run_InvalidApp_UNSUCCESS) {
  MockAppPtr invalid_app;
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(invalid_app));
  EXPECT_CALL(mock_hmi_capabilities_, is_ui_cooperating()).Times(0);
  command_->Run();
}

TEST_F(ChangeRegistrationRequestTest,
       Run_IsWhiteSpaceExist_InvalidAppNameAndScreenName_UNSUCCESS) {
  std::map<const std::string, const std::string> params;
  params.insert(std::pair<std::string, std::string>(
      std::string("appName"), std::string("application_name\n")));
  params.insert(std::pair<std::string, std::string>(
      std::string("ngnMediaScreenAppName"),
      std::string("ngn_media_screen_app_name\n")));

  std::map<const std::string, const std::string>::iterator it = params.begin();
  for (; it != params.end(); ++it) {
    MessageSharedPtr message(CreateMessage());
    (*message)[am::strings::msg_params][it->first] = it->second;
    ChangeRegistrationRequestPtr command(
        CreateCommand<ChangeRegistrationRequest>(message));

    MockAppPtr mock_app(CreateMockApp());
    EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(mock_app));
    EXPECT_CALL(mock_app_manager_,
                ManageMobileCommand(
                    MobileResultCodeIs(mobile_apis::Result::INVALID_DATA),
                    am::commands::Command::ORIGIN_SDL));
    command->Run();
  }
}

TEST_F(ChangeRegistrationRequestTest,
       Run_IsWhiteSpaceExist_InvalidTTS_UNSUCCESS) {
  (*msg_)[am::strings::msg_params][am::strings::tts_name][0]
         [am::strings::text] = "invalid_tts_name_with_empty_str\\n";

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::INVALID_DATA),
                          am::commands::Command::ORIGIN_SDL));
  command_->Run();
}

TEST_F(ChangeRegistrationRequestTest,
       Run_IsWhiteSpaceExist_InvalidVrSynonyms_UNSUCCESS) {
  (*msg_)[am::strings::msg_params][am::strings::vr_synonyms][0] =
      "vr_synonyms_1\n";
  (*msg_)[am::strings::msg_params][am::strings::vr_synonyms][1] =
      "vr_synonyms_2\n";

  EXPECT_CALL(mock_hmi_capabilities_, is_ui_cooperating()).Times(0);
  command_->Run();
}

TEST_F(ChangeRegistrationRequestTest,
       Run_CheckCoincidence_InvalidAppName_UNSUCCESS) {
  (*msg_)[am::strings::msg_params][am::strings::app_name] = kAppName1;

  const utils::custom_string::CustomString name(kAppName1);
  mock_app_->set_name(name);
  application_set_.insert(mock_app_);

  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(Return(applications_));
  EXPECT_CALL(*mock_app_, name()).WillOnce(ReturnRef(name));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::DUPLICATE_NAME),
                  am::commands::Command::ORIGIN_SDL));
  command_->Run();
}

TEST_F(ChangeRegistrationRequestTest,
       Run_CheckCoincidence_InvalidNameSynonyms_UNSUCCESS) {
  (*msg_)[am::strings::msg_params][am::strings::app_name] = kAppName1;

  SetMockAppName();

  MessageSharedPtr name_synonyms_message(CreateMessage());
  (*name_synonyms_message)[am::strings::msg_params][am::strings::app_name][0] =
      kAppName1;
  smart_objects::SmartObject* name_synonyms =
      &(*name_synonyms_message)[am::strings::msg_params][am::strings::app_name];
  ON_CALL(*mock_app_, vr_synonyms()).WillByDefault(Return(name_synonyms));

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::DUPLICATE_NAME),
                  am::commands::Command::ORIGIN_SDL));
  command_->Run();
}

TEST_F(ChangeRegistrationRequestTest,
       Run_CheckCoincidence_InvalidVrSynonyms_UNSUCCESS) {
  (*msg_)[am::strings::msg_params][am::strings::app_name] = kAppName1;
  (*msg_)[am::strings::msg_params][am::strings::vr_synonyms][0] = kAppName2;

  SetMockAppName();

  MessageSharedPtr vr_synonyms_message(CreateMessage());
  (*vr_synonyms_message)[am::strings::msg_params][am::strings::vr_synonyms][0] =
      kVrSynonyms1;

  ExpectVrSynonyms(vr_synonyms_message);

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::DUPLICATE_NAME),
                  am::commands::Command::ORIGIN_SDL));
  command_->Run();
}

TEST_F(ChangeRegistrationRequestTest, Run_IsLanguageSupportedByUI_UNSUCCESS) {
  CreateDefaultMessage(msg_);
  SetMockAppName();

  MessageSharedPtr vr_synonyms_message(CreateMessage());
  (*vr_synonyms_message)[am::strings::msg_params][am::strings::vr_synonyms][0] =
      kVrSynonyms2;

  ExpectVrSynonyms(vr_synonyms_message);

  smart_objects::SmartObject* ui_languages = NULL;
  EXPECT_CALL(mock_hmi_capabilities_, ui_supported_languages())
      .WillOnce(Return(ui_languages));

  ExpectationHmiCapabilities();
  EXPECT_CALL(mock_app_manager_, GetNextHMICorrelationID()).Times(0);
  command_->Run();
}

TEST_F(ChangeRegistrationRequestTest,
       Run_IsLanguageSupportedByUIEmptyUILanguages_UNSUCCESS) {
  CreateDefaultMessage(msg_);
  SetMockAppName();

  MessageSharedPtr vr_synonyms_message(CreateMessage());
  (*vr_synonyms_message)[am::strings::msg_params][am::strings::vr_synonyms][0] =
      kVrSynonyms2;

  ExpectVrSynonyms(vr_synonyms_message);

  smart_objects::SmartObjectSPtr ui_languages = CreateMessage();
  EXPECT_CALL(mock_hmi_capabilities_, ui_supported_languages())
      .WillOnce(Return(ui_languages.get()));

  ExpectationHmiCapabilities();
  EXPECT_CALL(mock_app_manager_, GetNextHMICorrelationID()).Times(0);
  command_->Run();
}

TEST_F(ChangeRegistrationRequestTest, Run_IsLanguageSupportedByUITrue_SUCCESS) {
  CreateDefaultMessage(msg_);
  SetMockAppName();

  MessageSharedPtr vr_synonyms_message(CreateMessage());
  (*vr_synonyms_message)[am::strings::msg_params][am::strings::vr_synonyms][0] =
      kVrSynonyms2;

  ExpectVrSynonyms(vr_synonyms_message);
  AddHmiDisplayLang(vr_synonyms_message);

  smart_objects::SmartObject* ui_languages =
      &(*vr_synonyms_message)[am::strings::msg_params]
                             [am::strings::hmi_display_language];
  EXPECT_CALL(mock_hmi_capabilities_, ui_supported_languages())
      .WillOnce(Return(ui_languages));

  smart_objects::SmartObject* vr_languages = NULL;
  EXPECT_CALL(mock_hmi_capabilities_, vr_supported_languages())
      .WillOnce(Return(vr_languages));

  ExpectationHmiCapabilities();
  EXPECT_CALL(mock_app_manager_, GetNextHMICorrelationID()).Times(0);
  command_->Run();
}

TEST_F(ChangeRegistrationRequestTest,
       Run_IsLanguageSupportedByVRLanguagesEmpty_UNSUCCESS) {
  CreateDefaultMessage(msg_);
  SetMockAppName();

  MessageSharedPtr vr_synonyms_message(CreateMessage());
  (*vr_synonyms_message)[am::strings::msg_params][am::strings::vr_synonyms][0] =
      kVrSynonyms2;

  ExpectVrSynonyms(vr_synonyms_message);
  AddHmiDisplayLang(vr_synonyms_message);

  smart_objects::SmartObject* ui_languages =
      &(*vr_synonyms_message)[am::strings::msg_params]
                             [am::strings::hmi_display_language];
  EXPECT_CALL(mock_hmi_capabilities_, ui_supported_languages())
      .WillOnce(Return(ui_languages));

  smart_objects::SmartObjectSPtr vr_languages = CreateMessage();
  EXPECT_CALL(mock_hmi_capabilities_, vr_supported_languages())
      .WillOnce(Return(vr_languages.get()));

  ExpectationHmiCapabilities();
  EXPECT_CALL(mock_app_manager_, GetNextHMICorrelationID()).Times(0);
  command_->Run();
}

TEST_F(ChangeRegistrationRequestTest, Run_IsLanguageSupportedByVrTrue_SUCCESS) {
  CreateDefaultMessage(msg_);
  (*msg_)[am::strings::msg_params][am::strings::language] =
      hmi_apis::Common_Language::EN_US;

  SetMockAppName();

  MessageSharedPtr vr_synonyms_message(CreateMessage());
  (*vr_synonyms_message)[am::strings::msg_params][am::strings::vr_synonyms][0] =
      kVrSynonyms2;

  ExpectVrSynonyms(vr_synonyms_message);
  AddHmiDisplayLang(vr_synonyms_message);

  smart_objects::SmartObject* ui_languages =
      &((*vr_synonyms_message)[am::strings::msg_params]
                              [am::strings::hmi_display_language]);
  EXPECT_CALL(mock_hmi_capabilities_, ui_supported_languages())
      .WillOnce(Return(ui_languages));

  AddHmiResponseLang(vr_synonyms_message);

  smart_objects::SmartObject* vr_languages =
      &(*vr_synonyms_message)[am::strings::msg_params]
                             [am::hmi_response::languages];
  EXPECT_CALL(mock_hmi_capabilities_, vr_supported_languages())
      .WillOnce(Return(vr_languages));

  smart_objects::SmartObject* tts_languages = NULL;
  EXPECT_CALL(mock_hmi_capabilities_, tts_supported_languages())
      .WillOnce(Return(tts_languages));
  ExpectationHmiCapabilities();
  command_->Run();
}

TEST_F(ChangeRegistrationRequestTest,
       Run_IsLanguageSupportedByTTSLanguagesEmpty_SUCCESS) {
  CreateDefaultMessage(msg_);
  (*msg_)[am::strings::msg_params][am::strings::language] =
      hmi_apis::Common_Language::EN_US;

  SetMockAppName();

  MessageSharedPtr vr_synonyms_message(CreateMessage());
  (*vr_synonyms_message)[am::strings::msg_params][am::strings::vr_synonyms][0] =
      kVrSynonyms2;

  ExpectVrSynonyms(vr_synonyms_message);
  AddHmiDisplayLang(vr_synonyms_message);

  smart_objects::SmartObject* ui_languages =
      &((*vr_synonyms_message)[am::strings::msg_params]
                              [am::strings::hmi_display_language]);
  EXPECT_CALL(mock_hmi_capabilities_, ui_supported_languages())
      .WillOnce(Return(ui_languages));

  AddHmiResponseLang(vr_synonyms_message);

  smart_objects::SmartObject* vr_languages =
      &(*vr_synonyms_message)[am::strings::msg_params]
                             [am::hmi_response::languages];
  EXPECT_CALL(mock_hmi_capabilities_, vr_supported_languages())
      .WillOnce(Return(vr_languages));

  smart_objects::SmartObjectSPtr tts_languages = CreateMessage();
  EXPECT_CALL(mock_hmi_capabilities_, tts_supported_languages())
      .WillOnce(Return(tts_languages.get()));
  ExpectationHmiCapabilities();
  command_->Run();
}

TEST_F(ChangeRegistrationRequestTest, Run_IsNicknameAllowedFalse_UNSUCCESS) {
  CreateDefaultMessage(msg_);
  (*msg_)[am::strings::msg_params][am::strings::language] =
      hmi_apis::Common_Language::EN_US;

  SetMockAppName();

  MessageSharedPtr vr_synonyms_message(CreateMessage());
  (*vr_synonyms_message)[am::strings::msg_params][am::strings::vr_synonyms][0] =
      kVrSynonyms2;

  ExpectVrSynonyms(vr_synonyms_message);
  AddHmiDisplayLang(vr_synonyms_message);

  smart_objects::SmartObject* ui_languages =
      &((*vr_synonyms_message)[am::strings::msg_params]
                              [am::strings::hmi_display_language]);
  EXPECT_CALL(mock_hmi_capabilities_, ui_supported_languages())
      .WillOnce(Return(ui_languages));

  AddHmiResponseLang(vr_synonyms_message);

  smart_objects::SmartObject* vr_languages =
      &(*vr_synonyms_message)[am::strings::msg_params]
                             [am::hmi_response::languages];
  EXPECT_CALL(mock_hmi_capabilities_, vr_supported_languages())
      .WillOnce(Return(vr_languages));

  EXPECT_CALL(mock_hmi_capabilities_, tts_supported_languages())
      .WillOnce(Return(vr_languages));
  ExpectationHmiCapabilities();

  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId));

  policy::StringArray app_nicknames;

  app_nicknames.push_back(kPolicyAppId);

  EXPECT_CALL(policy_interface_, GetInitialAppData(kPolicyAppId, _, _))
      .WillOnce(DoAll(SetArgPointer<1>(&app_nicknames),
                      SetArgPointer<2>(&app_nicknames),
                      Return(true)));
  utils::SharedPtr<usage_statistics::StatisticsManager> manager;
  EXPECT_CALL(policy_interface_, GetStatisticManager())
      .WillOnce(Return(manager));
  command_->Run();
}

TEST_F(ChangeRegistrationRequestTest,
       Run_IsNicknameAllowedInvalidApp_UNSUCCESS) {
  CreateDefaultMessage(msg_);
  (*msg_)[am::strings::msg_params][am::strings::language] =
      hmi_apis::Common_Language::EN_US;
  (*msg_)[am::strings::msg_params][am::strings::app_name] =
      utils::custom_string::CustomString("app_name");

  SetMockAppName();

  MockAppPtr invalid_app;
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app_))
      .WillOnce(Return(invalid_app));

  MessageSharedPtr vr_synonyms_message(CreateMessage());
  (*vr_synonyms_message)[am::strings::msg_params][am::strings::vr_synonyms][0] =
      kVrSynonyms2;

  ExpectVrSynonyms(vr_synonyms_message);

  AddHmiDisplayLang(vr_synonyms_message);

  smart_objects::SmartObject* ui_languages =
      &(*vr_synonyms_message)[am::strings::msg_params]
                             [am::strings::hmi_display_language];
  EXPECT_CALL(mock_hmi_capabilities_, ui_supported_languages())
      .WillOnce(Return(ui_languages));

  AddHmiResponseLang(vr_synonyms_message);

  smart_objects::SmartObject* vr_languages =
      &(*vr_synonyms_message)[am::strings::msg_params]
                             [am::hmi_response::languages];
  EXPECT_CALL(mock_hmi_capabilities_, vr_supported_languages())
      .WillOnce(Return(vr_languages));

  EXPECT_CALL(mock_hmi_capabilities_, tts_supported_languages())
      .WillOnce(Return(vr_languages));
  ExpectationHmiCapabilities();

  EXPECT_CALL(*mock_app_, policy_app_id()).Times(0);

  command_->Run();
  Mock::VerifyAndClearExpectations(&mock_app_);
}

TEST_F(ChangeRegistrationRequestTest,
       Run_IsNicknameAllowedInitResultFalse_UNSUCCESS) {
  CreateDefaultMessage(msg_);
  (*msg_)[am::strings::msg_params][am::strings::language] =
      hmi_apis::Common_Language::EN_US;

  SetMockAppName();

  MessageSharedPtr vr_synonyms_message(CreateMessage());
  (*vr_synonyms_message)[am::strings::msg_params][am::strings::vr_synonyms][0] =
      kVrSynonyms2;

  ExpectVrSynonyms(vr_synonyms_message);

  AddHmiDisplayLang(vr_synonyms_message);

  smart_objects::SmartObject* ui_languages =
      &(*vr_synonyms_message)[am::strings::msg_params]
                             [am::strings::hmi_display_language];
  EXPECT_CALL(mock_hmi_capabilities_, ui_supported_languages())
      .WillOnce(Return(ui_languages));

  AddHmiResponseLang(vr_synonyms_message);

  smart_objects::SmartObject* vr_languages =
      &(*vr_synonyms_message)[am::strings::msg_params]
                             [am::hmi_response::languages];
  EXPECT_CALL(mock_hmi_capabilities_, vr_supported_languages())
      .WillOnce(Return(vr_languages));

  EXPECT_CALL(mock_hmi_capabilities_, tts_supported_languages())
      .WillOnce(Return(vr_languages));
  ExpectationHmiCapabilities();

  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId));

  policy::StringArray app_nicknames;

  app_nicknames.push_back(kPolicyAppId);

  EXPECT_CALL(policy_interface_, GetInitialAppData(kPolicyAppId, _, _))
      .WillOnce(DoAll(SetArgPointer<1>(&app_nicknames),
                      SetArgPointer<2>(&app_nicknames),
                      Return(false)));
  EXPECT_CALL(policy_interface_, GetStatisticManager()).Times(0);
  command_->Run();
}

TEST_F(ChangeRegistrationRequestTest, Run_SUCCESS) {
  CreateDefaultMessage(msg_);
  (*msg_)[am::strings::msg_params][am::strings::language] =
      hmi_apis::Common_Language::EN_US;

  (*msg_)[am::strings::msg_params][am::strings::app_name] =
      utils::custom_string::CustomString("app_name");
  (*msg_)[am::strings::msg_params][am::strings::ngn_media_screen_app_name] =
      "ngn_media_screen_app_name";
  (*msg_)[am::strings::msg_params][am::strings::vr_synonyms][0] = "vr_synonyms";
  (*msg_)[am::strings::msg_params][am::strings::tts_name][0] = "tts_name";

  application_set_.insert(mock_app_);

  EXPECT_CALL(mock_app_manager_, applications())
      .WillOnce(Return(applications_));

  MessageSharedPtr vr_synonyms_message(CreateMessage());
  (*vr_synonyms_message)[am::strings::msg_params][am::strings::vr_synonyms][0] =
      kVrSynonyms2;

  AddHmiDisplayLang(vr_synonyms_message);

  smart_objects::SmartObject* ui_languages =
      &((*vr_synonyms_message)[am::strings::msg_params]
                              [am::strings::hmi_display_language]);
  EXPECT_CALL(mock_hmi_capabilities_, ui_supported_languages())
      .WillOnce(Return(ui_languages));

  AddHmiResponseLang(vr_synonyms_message);

  smart_objects::SmartObject* vr_languages =
      &(*vr_synonyms_message)[am::strings::msg_params]
                             [am::hmi_response::languages];
  EXPECT_CALL(mock_hmi_capabilities_, vr_supported_languages())
      .WillOnce(Return(vr_languages));

  EXPECT_CALL(mock_hmi_capabilities_, tts_supported_languages())
      .WillOnce(Return(vr_languages));
  ExpectationHmiCapabilities();

  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId));

  policy::StringArray app_nicknames;

  EXPECT_CALL(policy_interface_, GetInitialAppData(kPolicyAppId, _, _))
      .WillOnce(DoAll(SetArgPointer<1>(&app_nicknames),
                      SetArgPointer<2>(&app_nicknames),
                      Return(true)));
  EXPECT_CALL(policy_interface_, GetStatisticManager()).Times(0);

  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kConnectionKey));
  EXPECT_CALL(*mock_app_, set_name(_));
  EXPECT_CALL(*mock_app_, set_ngn_media_screen_name(_));
  EXPECT_CALL(*mock_app_, set_vr_synonyms(_));
  EXPECT_CALL(*mock_app_, set_tts_name(_));
  EXPECT_CALL(mock_app_manager_, GetNextHMICorrelationID())
      .WillRepeatedly(Return(0));
  command_->Run();
}

TEST_F(ChangeRegistrationResponseTest, Run_SUCCESS) {
  MessageSharedPtr message(CreateMessage());

  ChangeRegistrationResponsePtr command(
      CreateCommand<ChangeRegistrationResponse>(message));

  EXPECT_CALL(mock_app_manager_, SendMessageToMobile(message, false));
  command->Run();
}

}  // namespace change_registration
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test
