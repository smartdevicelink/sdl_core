/*
 * Copyright (c) 2017, Ford Motor Company
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

#include <gmock/gmock.h>

#include "application_manager/commands/command.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "resumption/mock_last_state.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace application_manager_test {

using testing::_;
using ::testing::DoAll;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SaveArg;

namespace am = application_manager;

const char* kAppServiceSection = "AppServices";
const char* kDefaults = "defaults";
const std::string kServiceType = "MEDIA";
const std::string kServiceName = "service_name";
const std::string kServiceId = "service_id";
const std::string kPolicyAppId = "p_app_id";
const uint32_t kConnectionKey = 43629;

MATCHER_P(CapabilityUpdateMatcher, reason, "") {
  smart_objects::SmartObject& services_updated =
      arg[am::strings::system_capability]
         [am::strings::app_services_capabilities][am::strings::app_services];
  if (smart_objects::SmartType_Array != services_updated.getType()) {
    return false;
  }
  for (size_t i = 0; i < services_updated.length(); i++) {
    smart_objects::SmartObject& service_cap = services_updated[i];
    if (reason == service_cap[am::strings::update_reason].asInt()) {
      return true;
    }
  }
  return false;
}

class AppServiceManagerTest : public testing::Test {
 public:
  AppServiceManagerTest()
      : mock_app_ptr_(new MockApplication)
      , app_service_manager_(mock_app_manager_, mock_last_state_)
      , mock_message_helper_(
            application_manager::MockMessageHelper::message_helper_mock()) {
    Mock::VerifyAndClearExpectations(mock_message_helper_);
  }
  ~AppServiceManagerTest() {
    Mock::VerifyAndClearExpectations(mock_message_helper_);
  }

  void SetUp() OVERRIDE {
    ON_CALL(*mock_message_helper_, BroadcastCapabilityUpdate(_, _))
        .WillByDefault(Return());
    ON_CALL(mock_settings_, embedded_services())
        .WillByDefault(ReturnRef(embedded_services_));
    ON_CALL(mock_app_manager_, get_settings())
        .WillByDefault(ReturnRef(mock_settings_));
    ON_CALL(*mock_app_ptr_, policy_app_id())
        .WillByDefault(Return(kPolicyAppId));
    ON_CALL(mock_last_state_, get_dictionary())
        .WillByDefault(ReturnRef(dict_));
    auto app_ptr = std::static_pointer_cast<am::Application>(mock_app_ptr_);
    ON_CALL(mock_app_manager_, application(kConnectionKey))
        .WillByDefault(Return(app_ptr));
  }

 protected:
  smart_objects::SmartObject GenerateMediaManifest(
      bool allow_app_consumers, std::string app_service_name = kServiceName) {
    smart_objects::SmartObject manifest(smart_objects::SmartType_Map);
    manifest[am::strings::service_type] = "MEDIA";
    manifest[am::strings::service_name] = app_service_name;
    manifest[am::strings::allow_app_consumers] = allow_app_consumers;
    manifest[am::strings::media_service_manifest] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    return manifest;
  }

  smart_objects::SmartObject PublishMobileService(
      uint32_t connection_key = kConnectionKey, bool first_run = true) {
    smart_objects::SmartObject manifest = GenerateMediaManifest(true);

    Json::Value empty_json;
    EXPECT_CALL(mock_last_state_, get_dictionary())
        .WillOnce(ReturnRef(empty_json));

    EXPECT_CALL(*mock_message_helper_,
                BroadcastCapabilityUpdate(
                    CapabilityUpdateMatcher(
                        mobile_apis::ServiceUpdateReason::PUBLISHED),
                    _))
        .WillOnce(Return());
    if (first_run) {
      EXPECT_CALL(*mock_message_helper_,
                  BroadcastCapabilityUpdate(
                      CapabilityUpdateMatcher(
                          mobile_apis::ServiceUpdateReason::ACTIVATED),
                      _))
          .WillOnce(Return());
    }

    return app_service_manager_.PublishAppService(manifest, true, connection_key);
  }

  void CheckCapabilityUpdate(smart_objects::SmartObject& msg_params,
                             size_t length,
                             std::string service_id,
                             mobile_apis::ServiceUpdateReason::eType reason,
                             bool published,
                             bool active) {
    smart_objects::SmartObject& services_updated =
        msg_params[am::strings::system_capability]
                  [am::strings::app_services_capabilities]
                  [am::strings::app_services];
    ASSERT_EQ(smart_objects::SmartType_Array, services_updated.getType());
    EXPECT_EQ(length, services_updated.length());
    for (size_t i = 0; i < services_updated.length(); i++) {
      smart_objects::SmartObject& service_cap = services_updated[i];
      if (service_id == service_cap[am::strings::updated_app_service_record]
                                   [am::strings::service_id]
                                       .asString()) {
        CheckCapability(service_cap, service_id, reason, published, active);
        return;
      }
    }
    FAIL() << "Unable to find service in capability update with service_id: "
           << service_id;
  }

  void CheckCapability(smart_objects::SmartObject& app_service_capability,
                       std::string service_id,
                       mobile_apis::ServiceUpdateReason::eType reason,
                       bool published,
                       bool active) {
    EXPECT_EQ(reason,
              app_service_capability[am::strings::update_reason].asInt());
    smart_objects::SmartObject& updated_record =
        app_service_capability[am::strings::updated_app_service_record];
    EXPECT_EQ(service_id, updated_record[am::strings::service_id].asString());
    EXPECT_EQ(published, updated_record[am::strings::service_published].asBool());
    EXPECT_EQ(active, updated_record[am::strings::service_active].asBool());
  }

  Json::Value dict_;
  std::vector<std::string> embedded_services_;
  std::shared_ptr<MockApplication> mock_app_ptr_;
  MockApplicationManager mock_app_manager_;
  resumption_test::MockLastState mock_last_state_;
  MockApplicationManagerSettings mock_settings_;
  application_manager::AppServiceManager app_service_manager_;
  application_manager::MockMessageHelper* mock_message_helper_;
};

TEST_F(AppServiceManagerTest, PublishAppService_Mobile_SUCCESS) {
  smart_objects::SmartObject manifest = GenerateMediaManifest(true);

  Json::Value empty_json;
  // dict[kAppServiceSection][kDefaults][kServiceType] = kPolicyAppId;
  EXPECT_CALL(mock_last_state_, get_dictionary())
      .WillOnce(ReturnRef(empty_json));

  smart_objects::SmartObject syscap_update_published;
  EXPECT_CALL(
      *mock_message_helper_,
      BroadcastCapabilityUpdate(
          CapabilityUpdateMatcher(mobile_apis::ServiceUpdateReason::PUBLISHED),
          _))
      .WillOnce(DoAll(SaveArg<0>(&syscap_update_published), Return()));
  
  smart_objects::SmartObject syscap_update_activated;
  EXPECT_CALL(
      *mock_message_helper_,
      BroadcastCapabilityUpdate(
          CapabilityUpdateMatcher(mobile_apis::ServiceUpdateReason::ACTIVATED),
          _))
      .WillOnce(DoAll(SaveArg<0>(&syscap_update_activated), Return()));

  auto record =
      app_service_manager_.PublishAppService(manifest, true, kConnectionKey);

  // Check final record
  std::string service_id = record[am::strings::service_id].asString();
  EXPECT_EQ(manifest, record[am::strings::service_manifest]);
  EXPECT_EQ(true, record[am::strings::service_published].asBool());
  EXPECT_EQ(true, record[am::strings::service_active].asBool());

  // Check first capability update
  CheckCapabilityUpdate(syscap_update_published,
                        1,
                        service_id,
                        mobile_apis::ServiceUpdateReason::PUBLISHED,
                        true,
                        false);

  // Check second capability update
  CheckCapabilityUpdate(syscap_update_activated,
                        1,
                        service_id,
                        mobile_apis::ServiceUpdateReason::ACTIVATED,
                        true,
                        true);
}

TEST_F(AppServiceManagerTest, PublishAppService_HMI_SUCCESS) {
  smart_objects::SmartObject manifest = GenerateMediaManifest(true);

  Json::Value empty_json;
  EXPECT_CALL(mock_last_state_, get_dictionary())
      .WillOnce(ReturnRef(empty_json));

  smart_objects::SmartObject syscap_update_published;
  EXPECT_CALL(
      *mock_message_helper_,
      BroadcastCapabilityUpdate(
          CapabilityUpdateMatcher(mobile_apis::ServiceUpdateReason::PUBLISHED),
          _))
      .WillOnce(DoAll(SaveArg<0>(&syscap_update_published), Return()));

  smart_objects::SmartObject syscap_update_activated;
  EXPECT_CALL(
      *mock_message_helper_,
      BroadcastCapabilityUpdate(
          CapabilityUpdateMatcher(mobile_apis::ServiceUpdateReason::ACTIVATED),
          _))
      .WillOnce(DoAll(SaveArg<0>(&syscap_update_activated), Return()));

  auto record =
      app_service_manager_.PublishAppService(manifest, false);

  // Check final record
  std::string service_id = record[am::strings::service_id].asString();
  EXPECT_EQ(manifest, record[am::strings::service_manifest]);
  EXPECT_EQ(true, record[am::strings::service_published].asBool());
  EXPECT_EQ(true, record[am::strings::service_active].asBool());

  // Check first capability update
  CheckCapabilityUpdate(syscap_update_published,
                        1,
                        service_id,
                        mobile_apis::ServiceUpdateReason::PUBLISHED,
                        true,
                        false);

  // Check second capability update
  CheckCapabilityUpdate(syscap_update_activated,
                        1,
                        service_id,
                        mobile_apis::ServiceUpdateReason::ACTIVATED,
                        true,
                        true);
}

TEST_F(AppServiceManagerTest, ActivateAppService_AlreadyActivated) {
  auto record = PublishMobileService();

  // No capability update
  EXPECT_CALL(
      *mock_message_helper_,
      BroadcastCapabilityUpdate(
          CapabilityUpdateMatcher(mobile_apis::ServiceUpdateReason::ACTIVATED),
          _))
      .Times(0);
  std::string service_id = record[am::strings::service_id].asString();
  
  EXPECT_TRUE(app_service_manager_.ActivateAppService(service_id));
}

TEST_F(AppServiceManagerTest, ActivateAppService_TwoApps_SUCCESS) {
  // Register two services with the same service type, the first is activated
  // automatically
  auto record = PublishMobileService();
  auto record2 = PublishMobileService(kConnectionKey + 1, false);

  // No capability update
  smart_objects::SmartObject syscap_update_activated;
  EXPECT_CALL(*mock_message_helper_,
              BroadcastCapabilityUpdate(
                  CapabilityUpdateMatcher(
                      mobile_apis::ServiceUpdateReason::ACTIVATED),
                  _))
      .WillOnce(DoAll(SaveArg<0>(&syscap_update_activated), Return()));
  std::string service_id = record[am::strings::service_id].asString();
  std::string service_id2 = record2[am::strings::service_id].asString();

  // Activate the second service
  EXPECT_TRUE(app_service_manager_.ActivateAppService(service_id2));

  // Check that the first service is deactivated
  CheckCapabilityUpdate(syscap_update_activated,
                        2,
                        service_id,
                        mobile_apis::ServiceUpdateReason::DEACTIVATED,
                        true,
                        false);

  // Check that the second service is activated
  CheckCapabilityUpdate(syscap_update_activated,
                        2,
                        service_id2,
                        mobile_apis::ServiceUpdateReason::ACTIVATED,
                        true,
                        true);
}

TEST_F(AppServiceManagerTest, DeactivateAppService_SUCCESS) {
  auto record = PublishMobileService();

  smart_objects::SmartObject syscap_update_deactivated;
  EXPECT_CALL(*mock_message_helper_,
              BroadcastCapabilityUpdate(
                  CapabilityUpdateMatcher(
                      mobile_apis::ServiceUpdateReason::DEACTIVATED),
                  _))
      .WillOnce(DoAll(SaveArg<0>(&syscap_update_deactivated), Return()));
  std::string service_id = record[am::strings::service_id].asString();

  EXPECT_TRUE(app_service_manager_.DeactivateAppService(service_id));

  // Check deactivated capability updated
  CheckCapabilityUpdate(syscap_update_deactivated,
                        1,
                        service_id,
                        mobile_apis::ServiceUpdateReason::DEACTIVATED,
                        true,
                        false);
}

TEST_F(AppServiceManagerTest, ActiveServiceForType_SUCCESS) {
  auto record = PublishMobileService();

  auto service = app_service_manager_.ActiveServiceForType(kServiceType);

  ASSERT_FALSE(service == NULL);
  EXPECT_EQ(service->record, record);
}

}  // namespace application_manager_test
}  // namespace components
}  // namespace test