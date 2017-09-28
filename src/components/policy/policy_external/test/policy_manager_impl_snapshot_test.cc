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

#include "gtest/gtest.h"

#include "policy/policy_manager_impl_test_base.h"

#include "json/json.h"

namespace test {
namespace components {
namespace policy_test {

using ::testing::Return;

TEST_F(PolicyManagerImplTest2, UpdatedPreloadedPT_ExpectLPT_IsUpdated) {
  // Arrange necessary pre-conditions
  StringsForUpdate new_data;
  new_data.new_field_name_ = "Notifications-";
  CreateNewRandomData(new_data);
  // Create Initial LocalPT from preloadedPT
  CreateLocalPT(preloaded_pt_filename_);
  // Update preloadedPT
  std::ifstream ifile(preloaded_pt_filename_);
  Json::Reader reader;
  Json::Value root(Json::objectValue);

  if (ifile.is_open() && reader.parse(ifile, root, true)) {
    root["policy_table"]["module_config"]["preloaded_date"] =
        new_data.new_date_;
    Json::Value val(Json::objectValue);
    Json::Value val2(Json::arrayValue);
    val2[0] = hmi_level_[index_];
    val[new_data.new_field_value_]["hmi_levels"] = val2;
    root["policy_table"]["functional_groupings"][new_data
                                                     .new_field_name_]["rpcs"] =
        val;
    root["policy_table"]["functional_groupings"][new_data.new_field_name_]
        ["user_consent_prompt"] = new_data.new_field_name_;
  }
  ifile.close();

  Json::StyledStreamWriter writer;
  std::ofstream ofile(preloaded_pt_filename_);
  writer.write(ofile, root);
  ofile.flush();
  ofile.close();

  //  Make PolicyManager to update LocalPT
  policy::CacheManagerInterfaceSPtr cache = policy_manager_->GetCache();
  EXPECT_TRUE(
      policy_manager_->InitPT(preloaded_pt_filename_, &policy_settings_));
  EXPECT_TRUE(cache->IsPTPreloaded());

  // Arrange
  utils::SharedPtr<policy_table::Table> table = cache->GenerateSnapshot();
  // Get FunctionalGroupings
  policy_table::FunctionalGroupings& fc =
      table->policy_table.functional_groupings;
  // Get RPCs for new added field in functional_group
  policy_table::Rpcs& rpcs = fc[new_data.new_field_name_];
  // Get user consent prompt
  const std::string& ucp = *(rpcs.user_consent_prompt);
  // Get Rpcs
  policy_table::Rpc& rpc = rpcs.rpcs;
  // Get RPC's parameters
  policy_table::RpcParameters& rpc_param = rpc[new_data.new_field_value_];

  // Check preloaded date
  EXPECT_EQ(static_cast<std::string>(
                *(table->policy_table.module_config.preloaded_date)),
            new_data.new_date_);
  // Check if new field exists in Local PT
  EXPECT_TRUE(fc.find(new_data.new_field_name_) != fc.end());
  // Check if user_consent_propmp is correct
  EXPECT_EQ(new_data.new_field_name_, ucp);
  // Check if new RPC exists
  EXPECT_TRUE(rpc.find(new_data.new_field_value_) != rpc.end());
  // Check HMI level of new RPC
  EXPECT_EQ(index_, static_cast<uint32_t>(rpc_param.hmi_levels[0]));
  // Check if new field matches field added to preloaded PT
  EXPECT_EQ(std::string((*(fc.find(new_data.new_field_name_))).first),
            new_data.new_field_name_);
}

TEST_F(PolicyManagerImplTest2,
       SetSystemLanguage_ExpectSystemLanguageSetSuccessfully) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  policy_manager_->SetSystemLanguage("it-it");
  utils::SharedPtr<policy_table::Table> pt =
      (policy_manager_->GetCache())->GetPT();
  ::policy_table::ModuleMeta& ModuleMeta = *(pt->policy_table.module_meta);
  EXPECT_EQ("it-it", static_cast<std::string>(*(ModuleMeta.language)));
}

TEST_F(PolicyManagerImplTest2, SetVINValue_ExpectVINSetSuccessfully) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  std::string vin_code("1FAPP6242VH100001");
  policy_manager_->SetVINValue(vin_code);
  utils::SharedPtr<policy_table::Table> pt =
      (policy_manager_->GetCache())->GetPT();
  ::policy_table::ModuleMeta& ModuleMeta = *(pt->policy_table.module_meta);
  EXPECT_EQ(vin_code, static_cast<std::string>(*(ModuleMeta.vin)));
}

TEST_F(PolicyManagerImplTest2, SetSystemInfo_ExpectSystemInfoSetSuccessfully) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  policy_manager_->SetSystemInfo("4.1.3.B_EB355B", "WAEGB", "ru-ru");
  policy::CacheManagerInterfaceSPtr cache = policy_manager_->GetCache();
  utils::SharedPtr<policy_table::Table> pt = cache->GetPT();
  ::policy_table::ModuleMeta& ModuleMeta = *(pt->policy_table.module_meta);
  EXPECT_EQ("ru-ru", static_cast<std::string>(*(ModuleMeta.language)));
  EXPECT_EQ("4.1.3.B_EB355B",
            static_cast<std::string>(*(ModuleMeta.ccpu_version)));
  EXPECT_EQ("WAEGB", static_cast<std::string>(*(ModuleMeta.wers_country_code)));
  EXPECT_FALSE(cache->IsPTPreloaded());
}

TEST_F(PolicyManagerImplTest2, CleanUnpairedDevice_ExpectDevicesDeleted) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  // Add first device
  ::policy::DeviceInfo dev_info1;
  dev_info1.hardware = "hardware IPX";
  dev_info1.firmware_rev = "v.8.0.1";
  dev_info1.os = "Android";
  dev_info1.os_ver = "4.4.2";
  dev_info1.carrier = "Life";
  dev_info1.max_number_rfcom_ports = 2;
  dev_info1.connection_type = "Bluetooth";
  policy_manager_->AddDevice(device_id_1_, "Bluetooth");
  policy_manager_->SetDeviceInfo(device_id_1_, dev_info1);

  // Add second device
  ::policy::DeviceInfo dev_info2;
  dev_info2.hardware = "hardware SPX";
  dev_info2.firmware_rev = "v.6.0.1";
  dev_info2.os = "Android";
  dev_info2.os_ver = "4.1.1";
  dev_info2.carrier = "MTS";
  dev_info2.max_number_rfcom_ports = 2;
  dev_info2.connection_type = "Bluetooth";
  policy_manager_->AddDevice("ZZZ123456789YYY", "Bluetooth");
  policy_manager_->SetDeviceInfo("ZZZ123456789YYY", dev_info2);

  // Add third device
  ::policy::DeviceInfo dev_info3;
  dev_info3.hardware = "hardware DNPX";
  dev_info3.firmware_rev = "v.4.0.1";
  dev_info3.os = "Android";
  dev_info3.os_ver = "5.0.1 Lollipop";
  dev_info3.carrier = "Kyivstar";
  dev_info3.max_number_rfcom_ports = 2;
  dev_info3.connection_type = "Bluetooth";
  policy_manager_->AddDevice("AAA123456789RRR", "Bluetooth");
  policy_manager_->SetDeviceInfo("AAA123456789RRR", dev_info3);

  utils::SharedPtr<policy_table::Table> pt =
      (policy_manager_->GetCache())->GetPT();
  // Try to find first device in PT
  policy_table::DeviceData::const_iterator iter =
      (*(pt->policy_table.device_data)).find(device_id_1_);
  // Check first device successfully added to PT
  ASSERT_TRUE(iter != (*(pt->policy_table.device_data)).end());

  // Try to find second device in PT
  iter = (*(pt->policy_table.device_data)).find("ZZZ123456789YYY");
  // Check second device successfully added to PT
  ASSERT_TRUE(iter != (*(pt->policy_table.device_data)).end());

  // Try to find third device in PT
  iter = (*(pt->policy_table.device_data)).find("AAA123456789RRR");
  // Check third device successfully added to PT
  ASSERT_TRUE(iter != (*(pt->policy_table.device_data)).end());

  policy_manager_->MarkUnpairedDevice(device_id_1_);
  policy_manager_->MarkUnpairedDevice("ZZZ123456789YYY");
  policy_manager_->MarkUnpairedDevice("AAA123456789RRR");
  policy_manager_->CleanupUnpairedDevices();

  // Try to find first device in PT
  iter = (*(pt->policy_table.device_data)).find(device_id_1_);
  // Check first device successfully deleted from PT
  ASSERT_TRUE(iter == (*(pt->policy_table.device_data)).end());

  // Try to find second device in PT
  iter = (*(pt->policy_table.device_data)).find("ZZZ123456789YYY");
  // Check second device successfully deleted from PT
  ASSERT_TRUE(iter == (*(pt->policy_table.device_data)).end());

  // Try to find third device in PT
  iter = (*(pt->policy_table.device_data)).find("AAA123456789RRR");
  // Check third device successfully deleted from PT
  ASSERT_TRUE(iter == (*(pt->policy_table.device_data)).end());
}

TEST_F(PolicyManagerImplTest2,
       AddValidRequestTypesToPT_default_Section_ExpectRTAdded) {
  // Arrange
  AddRTtoPT(kPtuJson, policy::kDefaultId, 1u, 0);
  CheckResultForValidRT();
}

TEST_F(PolicyManagerImplTest2,
       AddValidRequestTypeToPT_preDataConsentSection_ExpectRTAdded) {
  // Arrange
  AddRTtoPT(kPtuJson, policy::kPreDataConsentId, 1u, 0u);
  CheckResultForValidRT();
}

TEST_F(PolicyManagerImplTest2,
       AddInvalidRequestTypeToPT_defaultSection_ExpectIgnored) {
  // Arrange
  AddRTtoPT(kPtu3Json, policy::kDefaultId, 1u, 0u);
  CheckResultForInvalidRT();
}

TEST_F(PolicyManagerImplTest2,
       AddInvalidRequestTypeToPT_pre_DataConsentSection_ExpectIgnored) {
  // Arrange
  AddRTtoPT(kPtu3Json, policy::kPreDataConsentId, 1u, 1u);
  CheckResultForInvalidRT();
}

TEST_F(
    PolicyManagerImplTest2,
    AddValidRequestTypeToPT_GetNewAppWithSpecificPoliciesViaPTU_ExpectRTAdded) {
  const std::string& app_id = application_id_;

  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  // Add app
  policy_manager_->AddApplication(app_id, HmiTypes(policy_table::AHT_DEFAULT));
  // Check app gets RequestTypes from pre_DataConsent of app_policies
  // section
  pt_request_types_ = policy_manager_->GetAppRequestTypes(app_id);

  // Only single item as in pre_DataConsent in preloaded PT
  EXPECT_EQ(1u, pt_request_types_.size());
  EXPECT_CALL(listener_, OnPendingPermissionChange(app_id)).Times(1);

  // PTU has RequestTypes as [] - means 'all allowed'
  Json::Value root = GetPTU("json/PTU2.json");

  // Setting device consent to 'true' in order to have appropriate application
  // permissions, request type etc.
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(application_id_))
      .WillRepeatedly(Return(device_id_1_));
  policy_manager_->SetUserConsentForDevice(device_id_1_, true);

  // Get App Request Types from PTU
  ptu_request_types_ =
      root["policy_table"]["app_policies"][app_id]["RequestType"];
  ptu_request_types_size_ = ptu_request_types_.size();

  pt_request_types_.clear();
  // Get RequestTypes from <app_id> section of app policies after PT update
  pt_request_types_ = policy_manager_->GetAppRequestTypes(app_id);

  // Check sizes of Request types of PT and PTU
  ASSERT_EQ(ptu_request_types_size_, pt_request_types_.size());

  ::policy::AppPermissions permissions =
      policy_manager_->GetAppPermissionsChanges(app_id);
  EXPECT_TRUE(permissions.requestTypeChanged);

  ::policy::StringArray result;
  for (uint32_t i = 0; i < ptu_request_types_size_; ++i) {
    result.push_back(ptu_request_types_[i].asString());
  }
  std::sort(pt_request_types_.begin(), pt_request_types_.end());
  std::sort(result.begin(), result.end());
  // Checks
  ASSERT_EQ(pt_request_types_.size(), result.size());
  EXPECT_TRUE(std::equal(
      pt_request_types_.begin(), pt_request_types_.end(), result.begin()));
}

TEST_F(PolicyManagerImplTest2, AddDevice_RegisterDevice_TRUE) {
  const std::string connection_type = "Bluetooth";

  const bool result =
      (policy_manager_->GetCache())->AddDevice(device_id_1_, connection_type);
  // Get Policy table
  const utils::SharedPtr<policy_table::Table> policy_table =
      policy_manager_->GetCache()->GetPT();
  // Get preloaded_pt flag from Policy table
  const bool is_preloaded_pt =
      *policy_table->policy_table.module_config.preloaded_pt;
  // Get connection_type from policy_table
  const policy_table::DeviceParams& params =
      (*policy_table->policy_table.device_data)[device_id_1_];
  const std::string expected_connection_type = *params.connection_type;

  // Expect
  EXPECT_EQ(connection_type, expected_connection_type);
  // After adding device preloaded_pt must be false
  EXPECT_FALSE(is_preloaded_pt);
  EXPECT_TRUE(result);
}

}  // namespace policy
}  // namespace components
}  // namespace test
