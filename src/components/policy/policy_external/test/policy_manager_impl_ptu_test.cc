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

#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>

#include "gtest/gtest.h"

#include "policy/policy_manager_impl_test_base.h"
#include "policy/policy_table/types.h"

#include "utils/date_time.h"
#include "utils/gen_hash.h"
#include "json/reader.h"

namespace test {
namespace components {
namespace policy_test {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

TEST_F(PolicyManagerImplTest,
       DISABLED_TiggerPTUForNaviAppInCaseNoCertificateExistsInPolicyTable) {
  EXPECT_CALL(*cache_manager_, GetDeviceConsent(_))
      .WillOnce(Return(kDeviceAllowed));
  const uint32_t type = 0;
  const uint32_t id = 1;
  const EntityStatus status = kStatusOn;
  ExternalConsentStatusItem item(type, id, status);

  ExternalConsentStatus external_consent_status;
  external_consent_status.insert(item);

  GroupsByExternalConsentStatus group;
  group[item].push_back(std::make_pair<std::string, bool>("group_name", true));

  EXPECT_CALL(*cache_manager_, GetExternalConsentStatus())
      .WillOnce(Return(external_consent_status));

  EXPECT_CALL(*cache_manager_,
              GetGroupsWithSameEntities(external_consent_status))
      .WillOnce(Return(group));

  EXPECT_CALL(*cache_manager_, ResetCalculatedPermissions());

  EXPECT_CALL(*cache_manager_, GetPermissionsForApp(_, _, _))
      .WillOnce(Return(true))
      .WillOnce(Return(true));
  EXPECT_CALL(*cache_manager_, GetFunctionalGroupNames(_))
      .WillOnce(Return(true))
      .WillOnce(Return(true));

  EXPECT_CALL(*cache_manager_, SetUserPermissionsForApp(_, _))
      .WillOnce(Return(false));

  EXPECT_CALL(*cache_manager_, SetExternalConsentForApp(_));

  EXPECT_CALL(*cache_manager_, IsPredataPolicy(_)).WillOnce(Return(false));
  EXPECT_CALL(*cache_manager_, IsApplicationRepresented(_))
      .WillOnce(Return(true));
  EXPECT_EQ(policy_manager_->GetPolicyTableStatus(), "UP_TO_DATE");
  policy_manager_->AddApplication(kDefaultId,
                                  HmiTypes(policy_table::DEFAULT));
  EXPECT_EQ(policy_manager_->GetPolicyTableStatus(), "UP_TO_DATE");
}

TEST_F(PolicyManagerImplTest2, GetNotificationsNumberAfterPTUpdate) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);

  Json::Value table = createPTforLoad();
  policy_manager_->ForcePTExchange();
  policy_manager_->OnUpdateStarted();
  policy_table::Table update(&table);
  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);
  // Act
  std::string json = table.toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());
  EXPECT_CALL(listener_, OnUpdateStatusChanged(_));
  EXPECT_TRUE(policy_manager_->LoadPT(kFilePtUpdateJson, msg));
  EXPECT_FALSE(policy_manager_->GetCache()->IsPTPreloaded());

  std::string priority = "emergency";
  uint32_t notif_number = policy_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(1u, notif_number);

  priority = "navigation";
  notif_number = policy_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(2u, notif_number);

  priority = "emergency";
  notif_number = policy_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(1u, notif_number);

  priority = "VOICECOMM";
  notif_number = policy_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(3u, notif_number);

  priority = "normal";
  notif_number = policy_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(5u, notif_number);

  priority = "none";
  notif_number = policy_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(6u, notif_number);
}

TEST_F(PolicyManagerImplTest2, IsAppRevoked_SetRevokedAppID_ExpectAppRevoked) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);

  policy_manager_->AddApplication(app_id_1_,
                                  HmiTypes(policy_table::DEFAULT));

  std::ifstream ifile(kValidSdlPtUpdateJson);
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open()) {
    if (reader.parse(ifile, root, true)) {
      root["policy_table"]["app_policies"][app_id_1_] = Json::nullValue;
      json = root.toStyledString();
    }
  }
  ifile.close();

  ::policy::BinaryMessage msg(json.begin(), json.end());
  ASSERT_TRUE(policy_manager_->LoadPT(kFilePtUpdateJson, msg));
  EXPECT_FALSE(policy_manager_->GetCache()->IsPTPreloaded());
  CheckRpcPermissions(
      app_id_1_, "UnregisterAppInterface", ::policy::kRpcDisallowed);
  EXPECT_TRUE(policy_manager_->IsApplicationRevoked(app_id_1_));
}

// Related to manual test APPLINK-18792
TEST_F(PolicyManagerImplTest2, AppRevokedOne_AppRegistered) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  EmulatePTAppRevoked(kPtu2Json);

  EXPECT_FALSE(policy_manager_->GetCache()->IsPTPreloaded());
  ASSERT_TRUE(
      (policy_manager_->GetCache())->AddDevice(device_id_2_, "Bluetooth"));
  policy_manager_->AddApplication(application_id_,
                                  HmiTypes(policy_table::DEFAULT));
  // Registration is allowed
  CheckRpcPermissions("RegisterAppInterface", ::policy::kRpcAllowed);
}

// Related to manual test APPLINK-18794
TEST_F(PolicyManagerImplTest2, AppRevokedOne_AppRegistered_HMIDefault) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  EmulatePTAppRevoked(kPtu2Json);

  EXPECT_FALSE(policy_manager_->GetCache()->IsPTPreloaded());
  policy_manager_->AddApplication(application_id_,
                                  HmiTypes(policy_table::DEFAULT));

  std::string default_hmi;
  // Default HMI level is NONE
  EXPECT_TRUE(policy_manager_->GetDefaultHmi(application_id_, &default_hmi));
  EXPECT_EQ("NONE", default_hmi);
}

TEST_F(PolicyManagerImplTest2,
       CheckPermissions_SetRevokedAppID_ExpectRPCDisallowed) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  policy::CacheManagerInterfaceSPtr cache = policy_manager_->GetCache();
  cache->AddDevice(device_id_1_, "Bluetooth");
  cache->SetDeviceData(device_id_1_,
                       "hardware IPX",
                       "v.8.0.1",
                       "Android",
                       "4.4.2",
                       "Life",
                       2,
                       "Bluetooth");
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_1_))
      .WillRepeatedly(Return(device_id_1_));
  policy_manager_->SetUserConsentForDevice(device_id_1_, true);
  // Add app from consented device. App will be assigned with default policies
  policy_manager_->AddApplication(app_id_1_,
                                  HmiTypes(policy_table::DEFAULT));
  // Check before action
  policy_table::RpcParameters rpc_parameters;
  rpc_parameters.hmi_levels.push_back(policy_table::HL_FULL);

  policy_table::Rpc rpc;
  rpc["Alert"] = rpc_parameters;

  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;

  policy_manager_->CheckPermissions(
      app_id_1_, kHmiLevelFull, "Alert", input_params, output);

  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  ASSERT_TRUE(output.list_of_allowed_params.empty());
  // Act
  std::ifstream ifile(kValidSdlPtUpdateJson);
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  EXPECT_TRUE(ifile.is_open());
  EXPECT_TRUE(reader.parse(ifile, root, true));
  root["policy_table"]["app_policies"][app_id_1_] = Json::nullValue;
  json = root.toStyledString();
  ifile.close();

  ::policy::BinaryMessage msg(json.begin(), json.end());
  ASSERT_TRUE(policy_manager_->LoadPT(kFilePtUpdateJson, msg));
  EXPECT_FALSE(cache->IsPTPreloaded());

  policy_manager_->CheckPermissions(
      app_id_1_, kHmiLevelFull, "Alert", input_params, output);
  // Check RPC is disallowed
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  ASSERT_TRUE(output.list_of_allowed_params.empty());
}

TEST_F(PolicyManagerImplTest2,
       CheckPermissions_SetAppIDwithPolicies_ExpectRPCAllowed) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  policy_manager_->AddDevice(device_id_1_, "Bluetooth");
  policy::CacheManagerInterfaceSPtr cache = policy_manager_->GetCache();

  ASSERT_TRUE(cache->SetDeviceData(device_id_1_,
                                   "hardware IPX",
                                   "v.8.0.1",
                                   "Android",
                                   "4.4.2",
                                   "Life",
                                   2,
                                   "Bluetooth"));
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(application_id_))
      .WillRepeatedly(Return(device_id_1_));
  policy_manager_->SetUserConsentForDevice(device_id_1_, true);
  // Add app from consented device. App will be assigned with default policies
  policy_manager_->AddApplication(application_id_,
                                  HmiTypes(policy_table::MEDIA));
  // Emulate PTU with new policies for app added above
  std::ifstream ifile(kValidSdlPtUpdateJson);
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() && reader.parse(ifile, root, true)) {
    // Add AppID with policies
    root["policy_table"]["app_policies"][application_id_] =
        Json::Value(Json::objectValue);
    root["policy_table"]["app_policies"][application_id_]["memory_kb"] =
        Json::Value(50);
    root["policy_table"]["app_policies"][application_id_]
        ["heart_beat_timeout_ms"] = Json::Value(100);
    root["policy_table"]["app_policies"][application_id_]["AppHMIType"] =
        Json::Value(Json::arrayValue);
    root["policy_table"]["app_policies"][application_id_]["AppHMIType"][0] =
        Json::Value("MEDIA");
    root["policy_table"]["app_policies"][application_id_]["groups"] =
        Json::Value(Json::arrayValue);
    root["policy_table"]["app_policies"][application_id_]["groups"][0] =
        Json::Value("Base-4");
    root["policy_table"]["app_policies"][application_id_]["priority"] =
        Json::Value("EMERGENCY");
    root["policy_table"]["app_policies"][application_id_]["default_hmi"] =
        Json::Value("FULL");
    root["policy_table"]["app_policies"][application_id_]["keep_context"] =
        Json::Value(true);
    root["policy_table"]["app_policies"][application_id_]["steal_focus"] =
        Json::Value(true);
    root["policy_table"]["app_policies"][application_id_]["certificate"] =
        Json::Value("sign");
    json = root.toStyledString();
  }
  ifile.close();

  ::policy::BinaryMessage msg(json.begin(), json.end());
  // Load Json to cache
  EXPECT_TRUE(policy_manager_->LoadPT(kFilePtUpdateJson, msg));
  EXPECT_FALSE(cache->IsPTPreloaded());

  policy_table::RpcParameters rpc_parameters;
  rpc_parameters.hmi_levels.push_back(policy_table::HL_FULL);

  policy_table::Rpc rpc;
  rpc["Alert"] = rpc_parameters;
  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;

  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(application_id_))
      .WillOnce(Return(device_id_1_));
  cache->AddDevice(device_id_1_, "Bluetooth");
  cache->SetDeviceData(device_id_1_,
                       "hardware IPX",
                       "v.8.0.1",
                       "Android",
                       "4.4.2",
                       "Life",
                       2,
                       "Bluetooth");
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelFull, "Alert", input_params, output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  // Check list of parameters empty
  ASSERT_TRUE(output.list_of_allowed_params.empty());
}

TEST_F(PolicyManagerImplTest2,
       CheckPermissions_NoParametersInPT_CheckRpcsInDifferentLevels) {
  // Arrange
  AddSetDeviceData();
  LoadPTUFromJsonFile("json/sdl_update_pt_send_location.json");

  // Will be called each time permissions are checked
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(application_id_))
      .Times(4)
      .WillRepeatedly(Return(device_id_1_));

  // Check RPC in each level
  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;
  // Rpc in FULL level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelFull, "SendLocation", input_params, output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  ASSERT_TRUE(output.list_of_allowed_params.empty());

  // Reset output
  ResetOutputList(output);

  // Rpc in LIMITED level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelLimited, "SendLocation", input_params, output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  ASSERT_TRUE(output.list_of_allowed_params.empty());

  ResetOutputList(output);

  // Rpc in BACKGROUND level
  policy_manager_->CheckPermissions(application_id_,
                                    kHmiLevelBackground,
                                    "SendLocation",
                                    input_params,
                                    output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  ASSERT_TRUE(output.list_of_allowed_params.empty());

  ResetOutputList(output);

  // Rpc in NONE level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelNone, "SendLocation", input_params, output);
  // Check RPC is disallowed
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  // Check list of parameters is empty
  ASSERT_TRUE(output.list_of_allowed_params.empty());
}

TEST_F(
    PolicyManagerImplTest2,
    CheckPermissions_ParamsNotAllowedInPT_AddAppWithAllParams_CheckRpcsInDiffLvls) {
  // Arrange
  AddSetDeviceData();
  // File have empty parameters, so they are forbidden
  LoadPTUFromJsonFile("json/sdl_update_pt_send_location_no_params.json");

  // Will be called each time permissions are checked
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(application_id_))
      .Times(4)
      .WillRepeatedly(Return(device_id_1_));

  // Check RPC in each level
  ::policy::RPCParams input_params;
  InsertRpcParametersInList(input_params);

  ::policy::CheckPermissionResult output;
  // Rpc in FULL level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelFull, "SendLocation", input_params, output);

  // Group which has RPC does not require user consent, so its auto-allowed for
  // user. Since RPC 'parameters' section is present, but empty, that means
  // 'every parameter is disallowed' in case some parameter(s) will be passed
  // it will be considered as disallowed by policy (assumption, will be
  // clarified).
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);

  // Check list of allowed parameters is empty
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_EQ(10u, output.list_of_undefined_params.size());
  ResetOutputList(output);

  // Rpc in LIMITED level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelLimited, "SendLocation", input_params, output);

  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);

  // Check list of allowed parameters is empty
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_EQ(10u, output.list_of_undefined_params.size());

  ResetOutputList(output);

  // Rpc in BACKGROUND level
  policy_manager_->CheckPermissions(application_id_,
                                    kHmiLevelBackground,
                                    "SendLocation",
                                    input_params,
                                    output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);

  // Check list of allowed parameters is empty
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_EQ(10u, output.list_of_undefined_params.size());

  // Reset output
  ResetOutputList(output);

  // Rpc in NONE level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelNone, "SendLocation", input_params, output);

  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);

  // Check lists of parameters are  empty
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_TRUE(output.list_of_disallowed_params.empty());
  EXPECT_TRUE(output.list_of_undefined_params.empty());
}

TEST_F(PolicyManagerImplTest2,
       CheckPermissions_SomeParamsAllowedInPT_CheckRpcsInDiffLvls) {
  // Arrange
  AddSetDeviceData();

  // File have some parameters, so only "longitudeDegrees", "latitudeDegrees",
  //"locationDescription",  "phoneNumber" are allowed
  LoadPTUFromJsonFile("json/sdl_update_pt_send_location_some_params.json");

  // Will be called each time permissions are checked
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(application_id_))
      .Times(4)
      .WillRepeatedly(Return(device_id_1_));

  // Check RPC in each level
  ::policy::RPCParams input_params;
  InsertRpcParametersInList(input_params);

  ::policy::CheckPermissionResult output;
  // Rpc in FULL level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelFull, "SendLocation", input_params, output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);

  // Check list of allowed parameters is not empty
  ASSERT_FALSE(output.list_of_allowed_params.empty());
  // Check parameters that should be allowed
  CheckIsParamInList(output.list_of_allowed_params, "longitudeDegrees");
  CheckIsParamInList(output.list_of_allowed_params, "latitudeDegrees");
  CheckIsParamInList(output.list_of_allowed_params, "locationDescription");
  CheckIsParamInList(output.list_of_allowed_params, "phoneNumber");

  // Parameters that are missing in application assigned groups are considered
  // as disallowed by policy, i.e. w/o defined user consent, so they are put to
  // undefined container.
  ASSERT_FALSE(output.list_of_undefined_params.empty());
  CheckIsParamInList(output.list_of_undefined_params, "locationName");
  CheckIsParamInList(output.list_of_undefined_params, "addressLines");
  CheckIsParamInList(output.list_of_undefined_params, "locationImage");
  CheckIsParamInList(output.list_of_undefined_params, "deliveryMode");
  CheckIsParamInList(output.list_of_undefined_params, "timeStamp");
  CheckIsParamInList(output.list_of_undefined_params, "address");

  // Reset output
  output.hmi_level_permitted = ::policy::kRpcDisallowed;
  output.list_of_allowed_params.clear();
  output.list_of_undefined_params.clear();

  // Rpc in LIMITED level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelLimited, "SendLocation", input_params, output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);

  // Check list of allowed parameters is not empty
  ASSERT_FALSE(output.list_of_allowed_params.empty());
  // Check parameters that should be allowed
  CheckIsParamInList(output.list_of_allowed_params, "longitudeDegrees");
  CheckIsParamInList(output.list_of_allowed_params, "latitudeDegrees");
  CheckIsParamInList(output.list_of_allowed_params, "locationDescription");
  CheckIsParamInList(output.list_of_allowed_params, "phoneNumber");

  ASSERT_FALSE(output.list_of_undefined_params.empty());
  EXPECT_EQ(6u, output.list_of_undefined_params.size());
  // Reset output
  ResetOutputList(output);

  // Rpc in BACKGROUND level
  policy_manager_->CheckPermissions(application_id_,
                                    kHmiLevelBackground,
                                    "SendLocation",
                                    input_params,
                                    output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);

  // Check parameters that should be allowed
  ASSERT_FALSE(output.list_of_allowed_params.empty());
  CheckIsParamInList(output.list_of_allowed_params, "longitudeDegrees");
  CheckIsParamInList(output.list_of_allowed_params, "latitudeDegrees");
  CheckIsParamInList(output.list_of_allowed_params, "locationDescription");
  CheckIsParamInList(output.list_of_allowed_params, "phoneNumber");

  EXPECT_FALSE(output.list_of_undefined_params.empty());
  // Reset output
  ResetOutputList(output);

  // Rpc in NONE level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelNone, "SendLocation", input_params, output);
  // Check RPC is disallowed
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  // Check lists of parameters are  empty
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_TRUE(output.list_of_disallowed_params.empty());
  EXPECT_TRUE(output.list_of_undefined_params.empty());
}

TEST_F(PolicyManagerImplTest2, GetUpdateUrl) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  GetPTU(kValidSdlPtUpdateJson);
  // Check expectations
  const std::string update_url("http://x.x.x.x:3000/api/1/policies");
  EXPECT_EQ(update_url, policy_manager_->GetUpdateUrl(7));
  EXPECT_EQ("", policy_manager_->GetUpdateUrl(4));
}

// Related to manual test APPLINK-18789
TEST_F(PolicyManagerImplTest2, GetCorrectStatus_PTUSuccessful) {
  // Precondition
  CreateLocalPT(preloaded_pt_filename_);
  // Check
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());

  // Adding changes PT status
  policy_manager_->AddApplication(application_id_,
                                  HmiTypes(policy_table::DEFAULT));
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
  // Before load PT we should send notification about start updating
  policy_manager_->OnUpdateStarted();
  // Update
  GetPTU(kPtu3Json);
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2,
       CheckPermissions_NoParamsInPT_AddAppWithAllParams_CheckRpcsInDiffLvls) {
  // File does not have parameters, so they all are permitted
  CheckPermissions_AllParamsAllowed_CheckRpcsInDiffLvls(
      "json/sdl_update_pt_send_location.json");
}

TEST_F(PolicyManagerImplTest2,
       CheckPermissions_AllParamsAllowedInPT_CheckRpcsInDiffLvls) {
  // File has permissions for all params
  CheckPermissions_AllParamsAllowed_CheckRpcsInDiffLvls(
      "json/sdl_update_pt_send_location_all_params.json");
}

TEST_F(PolicyManagerImplTest2,
       CheckPermissions_DiffParamsAllowedInGroups_CheckRpcsInDiffLvls) {
  // Arrange
  AddSetDeviceData();
  // Load Json to cache
  // File have 2 functional groups: SendLocation and SendLocationOnly.
  // They have different parameters.
  // Allowed both groups of parameters
  LoadPTUFromJsonFile("json/sdl_update_pt_2_groups_have_params.json");

  // Will be called each time permissions are checked
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(application_id_))
      .Times(4)
      .WillRepeatedly(Return(device_id_1_));

  // Check RPC in each level
  ::policy::RPCParams input_params;
  InsertRpcParametersInList(input_params);

  ::policy::CheckPermissionResult output;
  // Rpc in FULL level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelFull, "SendLocation", input_params, output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  // Check list of allowed parameters is not empty
  EXPECT_FALSE(output.list_of_allowed_params.empty());
  EXPECT_EQ(7u, output.list_of_allowed_params.size());
  // Check parameters that should be allowed
  CheckIsParamInList(output.list_of_allowed_params, "longitudeDegrees");
  CheckIsParamInList(output.list_of_allowed_params, "latitudeDegrees");
  CheckIsParamInList(output.list_of_allowed_params, "locationDescription");
  CheckIsParamInList(output.list_of_allowed_params, "phoneNumber");
  CheckIsParamInList(output.list_of_allowed_params, "locationName");
  CheckIsParamInList(output.list_of_allowed_params, "locationImage");
  CheckIsParamInList(output.list_of_allowed_params, "deliveryMode");

  EXPECT_TRUE(output.list_of_disallowed_params.empty());
  ResetOutputList(output);

  // Rpc in LIMITED level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelLimited, "SendLocation", input_params, output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  // Check list of allowed parameters is not empty
  EXPECT_FALSE(output.list_of_allowed_params.empty());
  EXPECT_EQ(7u, output.list_of_allowed_params.size());
  // Check parameters that should be allowed
  CheckIsParamInList(output.list_of_allowed_params, "longitudeDegrees");
  CheckIsParamInList(output.list_of_allowed_params, "latitudeDegrees");
  CheckIsParamInList(output.list_of_allowed_params, "locationDescription");
  CheckIsParamInList(output.list_of_allowed_params, "phoneNumber");
  CheckIsParamInList(output.list_of_allowed_params, "locationName");
  CheckIsParamInList(output.list_of_allowed_params, "locationImage");
  CheckIsParamInList(output.list_of_allowed_params, "deliveryMode");

  EXPECT_TRUE(output.list_of_disallowed_params.empty());
  ResetOutputList(output);

  // Rpc in BACKGROUND level
  policy_manager_->CheckPermissions(application_id_,
                                    kHmiLevelBackground,
                                    "SendLocation",
                                    input_params,
                                    output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  // Check list of allowed parameters is not empty
  EXPECT_FALSE(output.list_of_allowed_params.empty());
  EXPECT_EQ(7u, output.list_of_allowed_params.size());
  // Check parameters that should be allowed
  CheckIsParamInList(output.list_of_allowed_params, "longitudeDegrees");
  CheckIsParamInList(output.list_of_allowed_params, "latitudeDegrees");
  CheckIsParamInList(output.list_of_allowed_params, "locationDescription");
  CheckIsParamInList(output.list_of_allowed_params, "phoneNumber");
  CheckIsParamInList(output.list_of_allowed_params, "locationName");
  CheckIsParamInList(output.list_of_allowed_params, "locationImage");
  CheckIsParamInList(output.list_of_allowed_params, "deliveryMode");

  EXPECT_TRUE(output.list_of_disallowed_params.empty());
  // Reset output
  ResetOutputList(output);

  // Rpc in NONE level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelNone, "SendLocation", input_params, output);
  // Check RPC is disallowed
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  // Check lists of parameters are  empty
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_TRUE(output.list_of_disallowed_params.empty());
  EXPECT_TRUE(output.list_of_undefined_params.empty());
}

TEST_F(PolicyManagerImplTest2,
       CheckPermissions_ParamsAllowedInOneGroup_CheckRpcsInDiffLvls) {
  // Load Json to cache
  // File have 2 functional groups: SendLocation and SendLocationOnly.
  // They have different parameters. One has dissalowed all params, other -
  // allowed.

  // Arrange
  CreateLocalPT("json/sdl_preloaded_pt_send_location.json");
  policy_manager_->AddDevice(device_id_1_, "Bluetooth");
  policy::CacheManagerInterfaceSPtr cache = policy_manager_->GetCache();
  ASSERT_TRUE(cache->SetDeviceData(device_id_1_,
                                   "hardware IPX",
                                   "v.8.0.1",
                                   "Android",
                                   "4.4.2",
                                   "Life",
                                   2,
                                   "Bluetooth"));

  // Add app from consented device. App will be assigned with default policies
  policy_manager_->AddApplication(application_id_,
                                  HmiTypes(policy_table::DEFAULT));

  std::ifstream ifile("json/sdl_update_pt_2_groups_no_params_in1.json");
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open()) {
    reader.parse(ifile, root, true);
  }
  json = root.toStyledString();
  ifile.close();
  ::policy::BinaryMessage msg(json.begin(), json.end());
  EXPECT_TRUE(policy_manager_->LoadPT(kFilePtUpdateJson, msg));
  EXPECT_FALSE(cache->IsPTPreloaded());

  // Will be called each time permissions are checked
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(application_id_))
      .Times(4)
      .WillRepeatedly(Return(device_id_1_));

  // Check RPC in each level
  ::policy::RPCParams input_params;
  InsertRpcParametersInList(input_params);

  ::policy::CheckPermissionResult output;
  // Rpc in FULL level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelFull, "SendLocation", input_params, output);

  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_EQ(10u, output.list_of_undefined_params.size());
  ResetOutputList(output);

  // Rpc in LIMITED level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelLimited, "SendLocation", input_params, output);

  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_EQ(10u, output.list_of_undefined_params.size());
  ResetOutputList(output);

  // Rpc in BACKGROUND level
  policy_manager_->CheckPermissions(application_id_,
                                    kHmiLevelBackground,
                                    "SendLocation",
                                    input_params,
                                    output);

  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_EQ(10u, output.list_of_undefined_params.size());
  // Reset output
  ResetOutputList(output);

  // Rpc in NONE level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelNone, "SendLocation", input_params, output);

  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_TRUE(output.list_of_disallowed_params.empty());
}

TEST_F(PolicyManagerImplTest2,
       CheckPermissions_ParamOmmittedInOneGroup_CheckRpcsInDiffLvls) {
  // Load Json to cache
  // File have 2 functional groups: SendLocation and SendLocationOnly.
  // They have different parameters. One has dissalowed all params, other -
  // omitted.

  // Arrange
  CreateLocalPT("json/sdl_preloaded_pt_send_location.json");
  policy_manager_->AddDevice(device_id_1_, "Bluetooth");
  policy::CacheManagerInterfaceSPtr cache = policy_manager_->GetCache();
  ASSERT_TRUE(cache->SetDeviceData(device_id_1_,
                                   "hardware IPX",
                                   "v.8.0.1",
                                   "Android",
                                   "4.4.2",
                                   "Life",
                                   2,
                                   "Bluetooth"));

  // Add app from consented device. App will be assigned with default policies
  policy_manager_->AddApplication(application_id_,
                                  HmiTypes(policy_table::DEFAULT));

  std::ifstream ifile(
      "json/sdl_update_pt_2_groups_no_params_in1_omitted_in2.json");
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open()) {
    reader.parse(ifile, root, true);
  }
  json = root.toStyledString();
  ifile.close();
  ::policy::BinaryMessage msg(json.begin(), json.end());
  EXPECT_TRUE(policy_manager_->LoadPT(kFilePtUpdateJson, msg));
  EXPECT_FALSE(cache->IsPTPreloaded());

  // Will be called each time permissions are checked
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(application_id_))
      .Times(4)
      .WillRepeatedly(Return(device_id_1_));

  // Check RPC in each level
  ::policy::RPCParams input_params;
  InsertRpcParametersInList(input_params);

  ::policy::CheckPermissionResult output;
  // Rpc in FULL level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelFull, "SendLocation", input_params, output);

  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_EQ(10u, output.list_of_undefined_params.size());
  ResetOutputList(output);

  // Rpc in LIMITED level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelLimited, "SendLocation", input_params, output);
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_EQ(10u, output.list_of_undefined_params.size());
  ResetOutputList(output);

  // Rpc in BACKGROUND level
  policy_manager_->CheckPermissions(application_id_,
                                    kHmiLevelBackground,
                                    "SendLocation",
                                    input_params,
                                    output);
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_EQ(10u, output.list_of_undefined_params.size());
  // Reset output
  ResetOutputList(output);

  // Rpc in NONE level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelNone, "SendLocation", input_params, output);
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_TRUE(output.list_of_disallowed_params.empty());
  EXPECT_TRUE(output.list_of_undefined_params.empty());
}

TEST_F(PolicyManagerImplTest, LoadPT_SetInvalidUpdatePT_PTIsNotLoaded) {
  // Arrange
  policy_manager_->ForcePTExchange();
  policy_manager_->OnUpdateStarted();
  Json::Value table(Json::objectValue);

  policy_table::Table update(&table);
  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);

  // Assert update is invalid
  ASSERT_FALSE(IsValid(update));

  // Act
  std::string json = table.toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  // Assert
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).Times(0);
  EXPECT_CALL(*cache_manager_, ApplyUpdate(_)).Times(0);
  EXPECT_CALL(listener_, GetAppName(_)).Times(0);
  EXPECT_CALL(listener_, OnUpdateStatusChanged(_)).Times(1);
  EXPECT_CALL(*cache_manager_, SaveUpdateRequired(false)).Times(0);
  EXPECT_CALL(*cache_manager_, TimeoutResponse()).Times(0);
  EXPECT_CALL(*cache_manager_, SecondsBetweenRetries(_)).Times(0);
  EXPECT_FALSE(policy_manager_->LoadPT(kFilePtUpdateJson, msg));
  EXPECT_CALL(*cache_manager_, IsPTPreloaded());
  EXPECT_FALSE(policy_manager_->GetCache()->IsPTPreloaded());
}

TEST_F(
    PolicyManagerImplTest2,
    AddApplication_AddExistingApplicationFromDeviceWithoutConsent_ExpectNoUpdateRequired) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
  GetPTU(kValidSdlPtUpdateJson);
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
  // Try to add existing app
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  // Check no update required
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
}

uint32_t GetCurrentDaysCount() {
  TimevalStruct current_time = date_time::DateTime::getCurrentTime();
  const uint32_t kSecondsInDay = 60 * 60 * 24;
  return current_time.tv_sec / kSecondsInDay;
}

TEST_F(PolicyManagerImplTest2,
       PTUpdatedAt_DaysNotExceedLimit_ExpectNoUpdateRequired) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  const uint32_t days = GetCurrentDaysCount();
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());

  GetPTU(kValidSdlPtUpdateJson);
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());

  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
  ::policy::Counters counter = ::policy::Counters::DAYS_AFTER_EPOCH;
  // Set PT was updated 10 days ago (limit is 30 days for now)
  // So no limit exceeded
  policy_manager_->PTUpdatedAt(counter, days - 10);
  policy_manager_->OnAppRegisteredOnMobile(app_id_2_);
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2,
       PTUpdatedAt_DaysExceedLimit_ExpectUpdateRequired) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  const uint32_t days = GetCurrentDaysCount();
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
  GetPTU(kValidSdlPtUpdateJson);
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());

  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
  ::policy::Counters counter = ::policy::Counters::DAYS_AFTER_EPOCH;
  // Set PT was updated 50 days ago (limit is 30 days for now)
  policy_manager_->PTUpdatedAt(counter, days - 50);
  policy_manager_->OnAppRegisteredOnMobile(app_id_2_);
  EXPECT_EQ("UPDATE_NEEDED", policy_manager_->GetPolicyTableStatus());
}

TEST_F(
    PolicyManagerImplTest2,
    OnIgnitionCyclesExceeded_SetExceededIgnitionCycles_ExpectUpdateScheduled) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  const uint32_t days = GetCurrentDaysCount();
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
  GetPTU(kValidSdlPtUpdateJson);
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
  // Try to add existing app
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  EXPECT_EQ("UP_TO_DATE", policy_manager_->GetPolicyTableStatus());
  ::policy::Counters counter = ::policy::Counters::DAYS_AFTER_EPOCH;
  // Set PT was updated 10 days ago (limit is 30 days for now)
  // So no limit exceeded
  policy_manager_->PTUpdatedAt(counter, days - 10);
  int ign_cycles =
      (policy_manager_->GetCache())->IgnitionCyclesBeforeExchange();
  // Set ignition cycles to value = 99 (limit is 100 which initiates auto
  // PTExchange)
  for (int i = 0; i < ign_cycles; ++i) {
    policy_manager_->IncrementIgnitionCycles();
  }
  policy_manager_->OnAppRegisteredOnMobile(app_id_2_);
  // Check update required
  EXPECT_EQ("UPDATE_NEEDED", policy_manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2,
       GetUserConsentForApp_SetUserConsentForApp_ExpectReceivedConsentCorrect) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  ASSERT_TRUE(
      (policy_manager_->GetCache())->AddDevice(device_id_2_, "Bluetooth"));
  ASSERT_TRUE((policy_manager_->GetCache())
                  ->SetDeviceData(device_id_2_,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));

  ::policy::StringArray consented_groups;
  ::policy::StringArray disallowed_groups;
  consented_groups.push_back(std::string("Notifications"));
  consented_groups.push_back(std::string("Notifications"));
  (policy_manager_->GetCache())
      ->SetUserPermissionsForDevice(
          device_id_2_, consented_groups, disallowed_groups);
  policy_manager_->SetUserConsentForDevice(device_id_2_, true);
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_2_))
      .WillRepeatedly(Return(device_id_2_));
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  GetPTU(kValidSdlPtUpdateJson);

  ::policy::PermissionConsent perm_consent;
  perm_consent.device_id = device_id_2_;
  perm_consent.policy_app_id = app_id_2_;
  perm_consent.consent_source = "VR";

  ::policy::FunctionalGroupPermission group1_perm;
  group1_perm.group_alias = "Notifications";
  group1_perm.group_name = "Notifications";
  group1_perm.group_id = ::utils::Djb2HashFromString("Notifications");
  group1_perm.state = ::policy::GroupConsent::kGroupAllowed;

  std::vector< ::policy::FunctionalGroupPermission> groups_permissions;
  groups_permissions.push_back(group1_perm);
  perm_consent.group_permissions = groups_permissions;

  policy_manager_->SetUserConsentForApp(perm_consent,
                                        policy::PolicyManager::kSilentMode);
  policy_manager_->SendNotificationOnPermissionsUpdated(app_id_2_);
  std::vector< ::policy::FunctionalGroupPermission> actual_groups_permissions;
  std::vector< ::policy::FunctionalGroupPermission>::iterator it;
  policy_manager_->GetUserConsentForApp(
      device_id_2_, app_id_2_, actual_groups_permissions);
  uint32_t index = 0;
  for (; index < actual_groups_permissions.size(); ++index) {
    if (actual_groups_permissions[index].group_id == group1_perm.group_id) {
      break;
    }
  }
  // Check
  EXPECT_EQ(group1_perm.group_alias,
            actual_groups_permissions[index].group_alias);
  EXPECT_EQ(group1_perm.group_name,
            actual_groups_permissions[index].group_name);
  EXPECT_EQ(group1_perm.group_id, actual_groups_permissions[index].group_id);
  EXPECT_EQ(group1_perm.state, actual_groups_permissions[index].state);
}

TEST_F(PolicyManagerImplTest2,
       CanAppKeepContext_SetPoliciesForAppUpdated_ExpectAppCanKeepContext) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  GetPTU(kValidSdlPtUpdateJson);
  // Check keep context in updated policies for app
  EXPECT_TRUE(policy_manager_->CanAppKeepContext(app_id_2_));
}

TEST_F(PolicyManagerImplTest2,
       CanAppStealFocus_SetPoliciesForAppUpdated_ExpectAppCanStealFocus) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  GetPTU(kValidSdlPtUpdateJson);
  // Check keep context in updated policies for app
  EXPECT_TRUE(policy_manager_->CanAppKeepContext(app_id_2_));
}

TEST_F(PolicyManagerImplTest2,
       GetVehicleInfo_SetVehicleInfo_ExpectReceivedInfoCorrect) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  GetPTU(kValidSdlPtUpdateJson);
  utils::SharedPtr<policy_table::Table> pt =
      (policy_manager_->GetCache())->GetPT();
  policy_table::ModuleConfig& module_config = pt->policy_table.module_config;
  ::policy::VehicleInfo vehicle_info = policy_manager_->GetVehicleInfo();

  EXPECT_EQ(static_cast<std::string>(*module_config.vehicle_make),
            vehicle_info.vehicle_make);
  EXPECT_EQ(static_cast<std::string>(*module_config.vehicle_model),
            vehicle_info.vehicle_model);
  EXPECT_EQ(static_cast<std::string>(*module_config.vehicle_year),
            vehicle_info.vehicle_year);
}

TEST_F(
    PolicyManagerImplTest2,
    GetPermissionsForApp_SetUserConsentForApp_ExpectReceivedPermissionsCorrect) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);

  ASSERT_TRUE(
      (policy_manager_->GetCache())->AddDevice(device_id_2_, "Bluetooth"));
  ASSERT_TRUE((policy_manager_->GetCache())
                  ->SetDeviceData(device_id_2_,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));

  ::policy::StringArray consented_groups;
  ::policy::StringArray disallowed_groups;
  consented_groups.push_back(std::string("Notifications"));
  (policy_manager_->GetCache())
      ->SetUserPermissionsForDevice(
          device_id_2_, consented_groups, disallowed_groups);
  policy_manager_->SetUserConsentForDevice(device_id_2_, true);
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_2_))
      .WillRepeatedly(Return(device_id_2_));
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));

  GetPTU(kValidSdlPtUpdateJson);
  ::policy::PermissionConsent perm_consent;
  perm_consent.device_id = device_id_2_;
  perm_consent.policy_app_id = app_id_2_;
  perm_consent.consent_source = "VR";

  ::policy::FunctionalGroupPermission group1_perm;
  group1_perm.group_alias = "Notifications";
  group1_perm.group_name = "Notifications";
  group1_perm.group_id = ::utils::Djb2HashFromString("Notifications");
  group1_perm.state = ::policy::GroupConsent::kGroupAllowed;

  std::vector< ::policy::FunctionalGroupPermission> groups_permissions;
  groups_permissions.push_back(group1_perm);
  perm_consent.group_permissions = groups_permissions;

  policy_manager_->SetUserConsentForApp(perm_consent,
                                        policy::PolicyManager::kSilentMode);
  policy_manager_->SendNotificationOnPermissionsUpdated(app_id_2_);
  std::vector< ::policy::FunctionalGroupPermission> actual_groups_permissions;
  std::vector< ::policy::FunctionalGroupPermission>::iterator it;
  policy_manager_->GetPermissionsForApp(
      device_id_2_, app_id_2_, actual_groups_permissions);
  uint32_t index = 0;
  for (; index < actual_groups_permissions.size(); ++index) {
    if (actual_groups_permissions[index].group_id == group1_perm.group_id) {
      break;
    }
  }
  // Check
  EXPECT_EQ(group1_perm.group_alias,
            actual_groups_permissions[index].group_alias);
  EXPECT_EQ(group1_perm.group_name,
            actual_groups_permissions[index].group_name);
  EXPECT_EQ(group1_perm.group_id, actual_groups_permissions[index].group_id);
  EXPECT_EQ(group1_perm.state, actual_groups_permissions[index].state);
}

TEST_F(
    PolicyManagerImplTest2,
    GetAppRequestTypes_AddApp_UpdateAppPolicies_ExpectReceivedRequestTypesCorrect) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);

  policy_manager_->AddApplication(app_id_3_,
                                  HmiTypes(policy_table::DEFAULT));
  ::policy::StringArray app_requests =
      policy_manager_->GetAppRequestTypes(app_id_3_);
  EXPECT_EQ(1u, app_requests.size());

  Json::Value root = GetPTU(kPtuRequestTypeJson);
  Json::Value request_Types = Json::Value(Json::arrayValue);
  request_Types =
      root["policy_table"]["app_policies"][app_id_3_]["RequestType"];
  app_requests = policy_manager_->GetAppRequestTypes(app_id_3_);
  EXPECT_EQ(request_Types.size(), app_requests.size());
  // Check nicknames match
  for (uint32_t i = 0; i < request_Types.size(); ++i) {
    EXPECT_EQ(request_Types[i], app_requests[i]);
  }
}

TEST_F(
    PolicyManagerImplTest2,
    HertBeatTimeout_AddApp_UpdateAppPolicies_ExpectReceivedHertBeatTimeoutCorrect) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  utils::SharedPtr<policy_table::Table> pt =
      (policy_manager_->GetCache())->GetPT();
  ::policy_table::PolicyTableType type1 =
      ::policy_table::PolicyTableType::PT_PRELOADED;
  pt->SetPolicyTableType(type1);
  if (!pt->is_valid()) {
    std::cout << "\nPolicy table is not valid."
              << "\n";
    rpc::ValidationReport report("policy_table");
    pt->ReportErrors(&report);
  }
  // Add new app
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  uint32_t result = policy_manager_->HeartBeatTimeout(app_id_2_);
  // By default hertbeat timeout is 0
  EXPECT_EQ(0u, result);
  Json::Value root = GetPTU(kValidSdlPtUpdateJson);

  ::policy_table::PolicyTableType type2 =
      ::policy_table::PolicyTableType::PT_UPDATE;
  pt->SetPolicyTableType(type2);
  if (!pt->is_valid()) {
    std::cout << "\nPolicy table is not valid."
              << "\n";
    rpc::ValidationReport report("policy_table");
    pt->ReportErrors(&report);
  }

  Json::Value heart_beat_timeout = Json::Value(Json::uintValue);
  heart_beat_timeout =
      root["policy_table"]["app_policies"][app_id_2_]["heart_beat_timeout_ms"];
  result = policy_manager_->HeartBeatTimeout(app_id_2_);
  EXPECT_EQ(heart_beat_timeout.asUInt(), result);
}

TEST_F(PolicyManagerImplTest2,
       RemoveAppConsentForGroup_SetUserConsentForApp_ExpectAppConsentDeleted) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  ASSERT_TRUE(
      (policy_manager_->GetCache())->AddDevice(device_id_2_, "Bluetooth"));
  ASSERT_TRUE((policy_manager_->GetCache())
                  ->SetDeviceData(device_id_2_,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));

  ::policy::StringArray consented_groups;
  ::policy::StringArray disallowed_groups;
  consented_groups.push_back(std::string("Notifications"));
  (policy_manager_->GetCache())
      ->SetUserPermissionsForDevice(
          device_id_2_, consented_groups, disallowed_groups);
  policy_manager_->SetUserConsentForDevice(device_id_2_, true);
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_2_))
      .WillRepeatedly(Return(device_id_2_));
  policy_manager_->AddApplication(app_id_2_,
                                  HmiTypes(policy_table::DEFAULT));
  GetPTU(kValidSdlPtUpdateJson);

  ::policy::PermissionConsent perm_consent;
  perm_consent.device_id = device_id_2_;
  perm_consent.policy_app_id = app_id_2_;
  perm_consent.consent_source = "VR";

  ::policy::FunctionalGroupPermission group1_perm;
  group1_perm.group_alias = "Notifications";
  group1_perm.group_name = "Notifications";
  group1_perm.group_id = ::utils::Djb2HashFromString("Notifications");
  group1_perm.state = ::policy::GroupConsent::kGroupAllowed;

  std::vector< ::policy::FunctionalGroupPermission> groups_permissions;
  groups_permissions.push_back(group1_perm);
  perm_consent.group_permissions = groups_permissions;

  policy_manager_->SetUserConsentForApp(perm_consent,
                                        policy::PolicyManager::kSilentMode);
  policy_manager_->SendNotificationOnPermissionsUpdated(app_id_2_);
  std::vector< ::policy::FunctionalGroupPermission> actual_groups_permissions;
  std::vector< ::policy::FunctionalGroupPermission>::iterator it;
  policy_manager_->GetPermissionsForApp(
      device_id_2_, app_id_2_, actual_groups_permissions);
  uint32_t index = 0;
  for (; index < actual_groups_permissions.size(); ++index) {
    if (actual_groups_permissions[index].group_id == group1_perm.group_id) {
      break;
    }
  }
  // Check
  EXPECT_EQ(group1_perm.group_alias,
            actual_groups_permissions[index].group_alias);
  EXPECT_EQ(group1_perm.group_name,
            actual_groups_permissions[index].group_name);
  EXPECT_EQ(group1_perm.group_id, actual_groups_permissions[index].group_id);
  EXPECT_EQ(group1_perm.state, actual_groups_permissions[index].state);
  utils::SharedPtr<policy_table::Table> pt =
      (policy_manager_->GetCache())->GetPT();
  uint32_t ucr_size = 0;
  ::policy_table::DeviceData& device_data = *pt->policy_table.device_data;
  ::policy_table::DeviceData::const_iterator dev_data_iter =
      device_data.find(device_id_2_);
  if (dev_data_iter != device_data.end()) {
    const ::policy_table::DeviceParams& dev_params = dev_data_iter->second;
    const ::policy_table::UserConsentRecords& ucr =
        *(dev_params.user_consent_records);
    ucr_size = ucr.size();
    ASSERT_GT(ucr_size, 0u);
    ::policy_table::UserConsentRecords::const_iterator ucr_iter =
        ucr.find(app_id_2_);
    if (ucr_iter != ucr.end()) {
      EXPECT_TRUE((*(ucr_iter->second.consent_groups)).find("Notifications") !=
                  (*(ucr_iter->second.consent_groups)).end());
      policy_manager_->RemoveAppConsentForGroup(app_id_2_, "Notifications");
      EXPECT_TRUE((*(ucr_iter->second.consent_groups)).find("Notifications") ==
                  (*(ucr_iter->second.consent_groups)).end());
    }
  }
}

TEST_F(PolicyManagerImplTest2,
       SingleInvalidRequestTypeInPTU_ExpectReplaceWithDefaultValues) {
  // Arrange
  const std::string section_name = app_id_2_;

  // Arrange
  CreateLocalPT(preloaded_pt_filename_);

  // Setting device consent to 'true' in order to have defult application
  // permissions, request type etc.
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_2_))
      .WillRepeatedly(Return(device_id_1_));
  policy_manager_->SetUserConsentForDevice(device_id_1_, true);

  // Add app
  policy_manager_->AddApplication(section_name,
                                  HmiTypes(policy_table::DEFAULT));
  EXPECT_CALL(listener_, OnPendingPermissionChange(section_name));

  // PTU has single invalid RequestTypes, which must be dropped and replaced
  // with default RT
  GetPTU("json/PTU_with_one_invalid_requestType.json");

  // Get RequestTypes from <app_id> section of app policies after PT update
  ::policy::StringArray app_request_types_after =
      policy_manager_->GetAppRequestTypes(section_name);

  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(kDefaultId))
      .WillOnce(Return(device_id_1_));
  ::policy::StringArray default_request_types_after =
      policy_manager_->GetAppRequestTypes(kDefaultId);

  // Check sizes of Request types of PT and PTU
  EXPECT_EQ(4u, app_request_types_after.size());

  ::policy::AppPermissions permissions =
      policy_manager_->GetAppPermissionsChanges(section_name);
  EXPECT_TRUE(permissions.requestTypeChanged);

  policy_table::RequestType temp_res1;
  std::vector<policy_table::RequestType> result1;
  for (uint32_t i = 0; i < app_request_types_after.size(); ++i) {
    if (::rpc::policy_table_interface_base::EnumFromJsonString(
            app_request_types_after[i], &temp_res1)) {
      result1.push_back(temp_res1);
    }
  }
  policy_table::RequestType temp_res2;
  std::vector<policy_table::RequestType> result2;
  for (size_t i = 0; i < default_request_types_after.size(); ++i) {
    if (::rpc::policy_table_interface_base::EnumFromJsonString(
            default_request_types_after[i], &temp_res2)) {
      result2.push_back(temp_res2);
    }
  }
  ASSERT_EQ(result1.size(), result2.size());
  std::sort(result1.begin(), result1.end());
  std::sort(result2.begin(), result2.end());
  // Checks
  EXPECT_TRUE(std::equal(result1.begin(), result1.end(), result2.begin()));
}

TEST_F(PolicyManagerImplTest2,
       InitPT_LoadPT_ExpectIncrementedCountOfSamePrompts) {
  // Initializing policy_table
  CreateLocalPT(preloaded_pt_filename_);

  policy_table::FunctionalGroupings functional_groupings;
  GetFunctionalGroupingsFromManager(functional_groupings);

  UserConsentPromptToRpcsConnections initial_functional_groupings_map;
  UserConsentPromptToRpcsConnections updated_functional_groupings_map;
  // Filling initial map
  FillMultimapFromFunctionalGroupings(initial_functional_groupings_map,
                                      functional_groupings);

  // Updating policy_table
  GetPTU("json/sdl_pt_update.json");
  policy_table::FunctionalGroupings updated_functional_groupings;
  GetFunctionalGroupingsFromManager(updated_functional_groupings);
  // Filling updated map
  FillMultimapFromFunctionalGroupings(updated_functional_groupings_map,
                                      updated_functional_groupings);

  // Comparing two multimaps
  // (EXPECT increment count of functionalgroups
  // under key : user_consent_prompt)
  uint32_t count_before_update =
      initial_functional_groupings_map.count("Notifications");
  uint32_t count_after_update =
      updated_functional_groupings_map.count("Notifications");
  EXPECT_EQ(1u, count_before_update);
  EXPECT_EQ(2u, count_after_update);
}

TEST_F(PolicyManagerImplTest2,
       LoadPT_UpdatePT_ChangingCountsOfDifferentUserConsentPrompts) {
  // Initializing policy_table
  CreateLocalPT(preloaded_pt_filename_);

  // First update of policy table
  GetPTU("json/sdl_pt_first_update.json");
  // Geting functional groupings first time
  policy_table::FunctionalGroupings first_functional_groupings;
  GetFunctionalGroupingsFromManager(first_functional_groupings);
  // Filling map first time
  UserConsentPromptToRpcsConnections first_update_functional_groupings_map;
  FillMultimapFromFunctionalGroupings(first_update_functional_groupings_map,
                                      first_functional_groupings);

  // Second update of policy table
  GetPTU("json/sdl_pt_second_update.json");
  // Geting functional groupings second time
  policy_table::FunctionalGroupings second_functional_groupings;
  GetFunctionalGroupingsFromManager(second_functional_groupings);
  // Filling map second time
  UserConsentPromptToRpcsConnections second_update_functional_groupings_map;
  FillMultimapFromFunctionalGroupings(second_update_functional_groupings_map,
                                      second_functional_groupings);

  // Getting counts before second update
  uint32_t first_count_of_old_user_consent_prompt =
      first_update_functional_groupings_map.count("Old_Notifications");
  uint32_t first_count_of_new_user_consent_prompt =
      first_update_functional_groupings_map.count("New_Notifications");

  // Getting counts after second update
  uint32_t second_count_of_old_user_consent_prompt =
      second_update_functional_groupings_map.count("Old_Notifications");
  uint32_t second_count_of_new_user_consent_prompt =
      second_update_functional_groupings_map.count("New_Notifications");

  // Expect decrement count of old user_consent_prormpt
  EXPECT_GT(first_count_of_old_user_consent_prompt,
            second_count_of_old_user_consent_prompt);
  // Expect increment count of new user_consent_prormpt
  EXPECT_LT(first_count_of_new_user_consent_prompt,
            second_count_of_new_user_consent_prompt);
}

TEST_F(PolicyManagerImplTest_RequestTypes,
       LoadPT_PTWithOneInvalidRequestTypeValue_RequestTypeValueEQToDefault) {
  // Logic in another function
  CompareAppRequestTypesWithDefault(kAppId, kJsonFiles[0]);
}

TEST_F(PolicyManagerImplTest_RequestTypes,
       LoadPT_InvalidRequestTypeBetweenCorectValuesInPTU_EraseInvalidValue) {
  // Refresh policy table with invalid RequestType in application
  RefreshPT(preloaded_pt_filename_, kJsonFiles[1]);
  // Correct of Request Types
  policy_table::RequestTypes correct_types;
  correct_types.push_back(policy_table::RequestType::RT_HTTP);
  correct_types.push_back(policy_table::RequestType::RT_LAUNCH_APP);
  correct_types.push_back(policy_table::RequestType::RT_PROPRIETARY);
  // Get <app_id> Request Types
  policy_table::RequestTypes received_types =
      GetRequestTypesForApplication(kAppId);

  CompareRequestTypesContainers(correct_types, received_types);
}

TEST_F(
    PolicyManagerImplTest_RequestTypes,
    LoadPT_AppInUpdateFileHaventRequestTypeField_RequestTypeValueEQToDefault) {
  // Logic in another function
  CompareAppRequestTypesWithDefault(kAppId, kJsonFiles[2]);
}

TEST_F(PolicyManagerImplTest_RequestTypes,
       LoadPT_RequestTypeArrayHaveNoOneValues_AvalibleAllRequestTypes) {
  // Refresh policy table with invalid RequestType in application
  RefreshPT(preloaded_pt_filename_, kJsonFiles[3]);

  // Get <app_id> Request Types
  policy_table::RequestTypes received_types =
      GetRequestTypesForApplication(kAppId);

  // Expect
  const size_t correct_size = 0;
  const size_t received_size = received_types.size();
  EXPECT_EQ(correct_size, received_size);
}

TEST_F(PolicyManagerImplTest_RequestTypes,
       InitPT_DefaultRequestTypeHaveOneInvalidValue_False) {
  // PT have only invalid value in app_policies::default::RequestType
  ON_CALL(policy_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(app_storage_folder_));
  ASSERT_FALSE(
      policy_manager_impl_sptr_->InitPT(kJsonFiles[4], &policy_settings_));
  // Invalid table data with pt_preloaded=true
  EXPECT_TRUE(policy_manager_impl_sptr_->GetCache()->IsPTPreloaded());
}

TEST_F(PolicyManagerImplTest_RequestTypes,
       InitPT_DefaultRequestTypeHaveSeveralInvalidValues_False) {
  // PT have several only invalid values in app_policies::default::RequestType
  ON_CALL(policy_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(app_storage_folder_));
  ASSERT_FALSE(
      policy_manager_impl_sptr_->InitPT(kJsonFiles[5], &policy_settings_));
  // Invalid table data with pt_preloaded=true
  EXPECT_TRUE(policy_manager_impl_sptr_->GetCache()->IsPTPreloaded());
}

TEST_F(PolicyManagerImplTest_RequestTypes,
       InitPT_DefaultRequestTypeHaveInvalidValueBetweenCorrect_True) {
  // PT have ["QUERY_APPS", "IVSU", "PROPRIETARY"]
  // In app_policies::default::RequestType
  ON_CALL(policy_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(app_storage_folder_));
  ASSERT_TRUE(
      policy_manager_impl_sptr_->InitPT(kJsonFiles[6], &policy_settings_));
  EXPECT_TRUE(policy_manager_impl_sptr_->GetCache()->IsPTPreloaded());

  // Correct of Request Types
  policy_table::RequestTypes correct_types;
  correct_types.push_back(policy_table::RequestType::RT_QUERY_APPS);
  correct_types.push_back(policy_table::RequestType::RT_PROPRIETARY);

  // Get default Request Types
  policy_table::RequestTypes received_types =
      GetRequestTypesForApplication(kDefaultAppId);

  CompareRequestTypesContainers(correct_types, received_types);
}

TEST_F(PolicyManagerImplTest_RequestTypes,
       LoadPT_PTDefaultApp_RequestTypeValueEQToUpdate) {
  // Base values of Request Types
  policy_table::RequestTypes correct_types = CreateDefaultAppPTURequestValues();

  // Load valid values for RequestType
  RefreshPT(preloaded_pt_filename_, kJsonFiles[7]);

  // Get Request Types for "<default>"
  policy_table::RequestTypes received_types =
      GetRequestTypesForApplication(policy::kDefaultId);

  CompareRequestTypesContainers(correct_types, received_types);
}

TEST_F(
    PolicyManagerImplTest_RequestTypes,
    LoadPT_PTDefaultAppInvalidRequestTypeValues_RequestTypeValueEQToDatabase) {
  // Base values of Request Types
  policy_table::RequestTypes correct_types =
      CreateDefaultAppDatabaseRequestValues();

  // Load RequestType with invalid values
  RefreshPT(preloaded_pt_filename_, kJsonFiles[8]);

  // Get Request Types for "<default>"
  policy_table::RequestTypes received_types =
      GetRequestTypesForApplication(policy::kDefaultId);

  CompareRequestTypesContainers(correct_types, received_types);
}

TEST_F(PolicyManagerImplTest_RequestTypes,
       LoadPT_PTDefaultAppEmptyRequestTypeValues_RequestTypeValueEmpty) {
  // Load RequestType with empty values
  RefreshPT(preloaded_pt_filename_, kJsonFiles[9]);

  // Get Request Types for "<default>"
  policy_table::RequestTypes received_types =
      GetRequestTypesForApplication(policy::kDefaultId);

  // Expect
  const size_t received_size = received_types.size();
  EXPECT_EQ(0u, received_size);
}

TEST_F(PolicyManagerImplTest_RequestTypes,
       LoadPT_PTDefaultAppOmittedRequestType_RequestTypeValueEQToDatabase) {
  // Base values of Request Types
  policy_table::RequestTypes correct_types =
      CreateDefaultAppDatabaseRequestValues();

  // Load omitted RequestType values
  RefreshPT(preloaded_pt_filename_, kJsonFiles[10]);

  // Get Request Types for "<default>"
  policy_table::RequestTypes received_types =
      GetRequestTypesForApplication(policy::kDefaultId);

  CompareRequestTypesContainers(correct_types, received_types);
}

TEST_F(
    PolicyManagerImplTest_RequestTypes,
    LoadPT_PTDefaultAppOneInvalidRequestTypeValue_RequestTypeValueEQValidPT) {
  // Update values of Request Types
  policy_table::RequestTypes correct_types = CreateDefaultAppPTURequestValues();

  // Load RequestType with one invalid value
  RefreshPT(preloaded_pt_filename_, kJsonFiles[11]);

  // Get Request Types for "<default>"
  policy_table::RequestTypes received_types =
      GetRequestTypesForApplication(policy::kDefaultId);

  CompareRequestTypesContainers(correct_types, received_types);
}

TEST_F(PolicyManagerImplTest_RequestTypes,
       LoadPT_PTPreDataConsentApp_RequestTypeValueEQToUpdate) {
  // Update values of Request Types
  policy_table::RequestTypes correct_types =
      CreatePreDataConsentAppPTURequestValues();

  // Load valid values for RequestType
  RefreshPT(preloaded_pt_filename_, kJsonFiles[12]);

  // Get Request Types for "<pre_DataConsent>"
  policy_table::RequestTypes received_types =
      GetRequestTypesForApplication(policy::kPreDataConsentId);

  CompareRequestTypesContainers(correct_types, received_types);
}

TEST_F(
    PolicyManagerImplTest_RequestTypes,
    LoadPT_PTPreDataConsentAppInvalidRequestTypeValues_RequestTypeValueEQToDatabase) {
  // Update values of Request Types
  policy_table::RequestTypes correct_types;
  correct_types.push_back(policy_table::RequestType::RT_HTTP);

  // Load RequestType with invalid values
  RefreshPT(preloaded_pt_filename_, kJsonFiles[13]);

  // Get Request Types for "<pre_DataConsent>"
  policy_table::RequestTypes received_types =
      GetRequestTypesForApplication(policy::kPreDataConsentId);

  CompareRequestTypesContainers(correct_types, received_types);
}

TEST_F(PolicyManagerImplTest_RequestTypes,
       LoadPT_PTPreDataConsentAppEmptyRequestTypeValues_RequestTypeValueEmpty) {
  // Load RequestType with empty values
  RefreshPT(preloaded_pt_filename_, kJsonFiles[14]);

  // Get Request Types for "<pre_DataConsent>"
  policy_table::RequestTypes received_types =
      GetRequestTypesForApplication(policy::kPreDataConsentId);

  // Expect
  const size_t received_size = received_types.size();
  EXPECT_EQ(0u, received_size);
}

TEST_F(
    PolicyManagerImplTest_RequestTypes,
    LoadPT_PTPreDataConsentAppOmittedRequestType_RequestTypeValueEQToDatabase) {
  // Base values of Request Types
  policy_table::RequestTypes correct_types;
  correct_types.push_back(policy_table::RequestType::RT_HTTP);

  // Load omitted RequestType values
  RefreshPT(preloaded_pt_filename_, kJsonFiles[15]);

  // Get Request Types for "<pre_DataConsent>"
  policy_table::RequestTypes received_types =
      GetRequestTypesForApplication(policy::kPreDataConsentId);

  CompareRequestTypesContainers(correct_types, received_types);
}

TEST_F(
    PolicyManagerImplTest_RequestTypes,
    LoadPT_PTPreDataConsentAppOneInvalidRequestTypeValue_RequestTypeValueEQValidPT) {
  // Update values of Request Types
  policy_table::RequestTypes correct_types =
      CreatePreDataConsentAppPTURequestValues();

  // Load RequestType with one invalid value
  RefreshPT(preloaded_pt_filename_, kJsonFiles[16]);

  // Get Request Types for "<pre_DataConsent>"
  policy_table::RequestTypes received_types =
      GetRequestTypesForApplication(policy::kPreDataConsentId);

  CompareRequestTypesContainers(correct_types, received_types);
}

}  // namespace policy
}  // namespace components
}  // namespace test
