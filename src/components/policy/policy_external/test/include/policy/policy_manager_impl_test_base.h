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
#ifndef SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_TEST_INCLUDE_POLICY_POLICY_MANAGER_IMPL_TEST_BASE_H_
#define SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_TEST_INCLUDE_POLICY_POLICY_MANAGER_IMPL_TEST_BASE_H_

#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "policy/policy_manager_impl.h"

#include "policy/mock_cache_manager.h"
#include "policy/mock_update_status_manager.h"
#include "policy/mock_policy_listener.h"
#include "policy/mock_policy_settings.h"

namespace test {
namespace components {
namespace policy_test {

using ::testing::NiceMock;
using ::policy::PolicyManagerImpl;

typedef std::multimap<std::string, policy_table::Rpcs&>
    UserConsentPromptToRpcsConnections;

typedef utils::SharedPtr<policy_table::Table> PolicyTableSPtr;

namespace {
const std::string kSdlPreloadedPtJson = "json/sdl_preloaded_pt.json";
const std::string kSdlPreloadedPtJson2 = "json/sdl_preloaded_pt1.json";
const std::string kFilePtUpdateJson = "json/file_pt_update.json";
const std::string kHmiLevelFull = "FULL";
const std::string kHmiLevelLimited = "LIMITED";
const std::string kHmiLevelBackground = "BACKGROUND";
const std::string kHmiLevelNone = "None";

const std::string kPtuJson = "json/PTU.json";
const std::string kPtu2Json = "json/PTU2.json";
const std::string kPtu3Json = "json/PTU3.json";
const std::string kValidSdlPtUpdateJson = "json/valid_sdl_pt_update.json";
const std::string kPtuRequestTypeJson = "json/ptu_requestType.json";
const std::string kPtu2RequestTypeJson = "json/ptu2_requestType.json";
const std::string kDummyUpdateFileName = "DummyName";
}  // namespace

struct StringsForUpdate {
  std::string new_field_value_;
  std::string new_field_name_;
  std::string new_date_;
};

char GenRandomChar(char range_from, char range_to);
struct StringsForUpdate CreateNewRandomData(StringsForUpdate& str);
void CheckIsParamInList(const ::policy::RPCParams& list,
                        const std::string& parameter);
Json::Value createPTforLoad();
void InsertRpcParametersInList(::policy::RPCParams& input_params);
policy_table::AppHmiTypes HmiTypes(const policy_table::AppHMIType hmi_type);

template <typename T>
void SortAndCheckEquality(std::vector<T> first, std::vector<T> second) {
  ASSERT_EQ(first.size(), second.size());
  std::sort(first.begin(), first.end());
  std::sort(second.begin(), second.end());

  EXPECT_TRUE(std::equal(first.begin(), first.end(), second.begin()));
}

template <typename T>
std::string NumberToString(T Number) {
  std::ostringstream ss;
  ss << Number;
  return ss.str();
}

class PolicyManagerImplTest : public ::testing::Test {
 public:
  PolicyManagerImplTest();

 protected:
  const std::string unpaired_device_id_;

  PolicyManagerImpl* manager_;
  MockCacheManagerInterface* cache_manager_;
  MockUpdateStatusManager update_manager_;
  NiceMock<MockPolicyListener> listener_;

  void SetUp() OVERRIDE;

  void TearDown() OVERRIDE;

  ::testing::AssertionResult IsValid(const policy_table::Table& table);
};

class PolicyManagerImplTest2 : public ::testing::Test {
 public:
  PolicyManagerImplTest2();

 protected:
  const std::string app_id_1_;
  const std::string app_id_2_;
  const std::string app_id_3_;
  const std::string device_id_1_;
  const std::string device_id_2_;
  const std::string application_id_;
  const std::string app_storage_folder_;
  const std::string preloaded_pt_filename_;
  const bool in_memory_;

  PolicyManagerImpl* policy_manager_;
  NiceMock<MockPolicyListener> listener_;
  ::policy::StringArray hmi_level_;
  ::policy::StringArray pt_request_types_;
  size_t ptu_request_types_size_;
  uint32_t index_;
  Json::Value ptu_request_types_;
  NiceMock<policy_handler_test::MockPolicySettings> policy_settings_;

  void SetUp() OVERRIDE;

  ::policy::StringArray JsonToVectorString(
      const Json::Value& PTU_request_types);

  const Json::Value GetPTU(const std::string& file_name);

  void CreateLocalPT(const std::string& file_name);

  void AddRTtoPT(const std::string& update_file_name,
                 const std::string& section_name,
                 const uint32_t rt_number,
                 const uint32_t invalid_rt_number);

  void AddRTtoAppSectionPT(const std::string& update_file_name,
                           const std::string& section_name,
                           const uint32_t rt_number,
                           const uint32_t invalid_rt_number);

  std::vector<policy_table::RequestType> PushRequestTypesToContainer(
      const ::policy::StringArray& temp_result);

  void CheckResultForValidRT();

  void CheckResultForInvalidRT();

  void FillMultimapFromFunctionalGroupings(
      UserConsentPromptToRpcsConnections& input_multimap,
      policy_table::FunctionalGroupings& fg_table);

  void GetFunctionalGroupingsFromManager(
      policy_table::FunctionalGroupings& input_functional_groupings);

  void TearDown() OVERRIDE;

  void ResetOutputList(::policy::CheckPermissionResult& output);
  // To avoid duplicate test with different json files
  void CheckPermissions_AllParamsAllowed_CheckRpcsInDiffLvls(
      const std::string& update_file);

  // To avoid duplicates in tests
  void CheckRpcPermissions(const std::string& rpc_name,
                           const PermitResult& expected_permission);

  void CheckRpcPermissions(const std::string& app_id,
                           const std::string& rpc_name,
                           const policy::PermitResult& out_expected_permission);

  // To avoid duplicate arrange of test
  void AddSetDeviceData();

  // Load Json File and set it as PTU
  void LoadPTUFromJsonFile(const std::string& update_file);

  void EmulatePTAppRevoked(const std::string& ptu_name);

  utils::SharedPtr<policy_table::Table> PreconditionForBasicValidateSnapshot();

  template <typename ParentType, typename Value>
  bool IsKeyExisted(const ParentType& parent, const Value& value) const {
    return parent.end() != std::find(parent.begin(), parent.end(), value);
  }

  template <typename ParentType>
  bool IsKeyExisted(const ParentType& parent, const std::string& value) const {
    return parent.end() != parent.find(value);
  }

  template <typename ParentType, typename KeyType>
  const KeyType& GetKeyData(const ParentType& parent,
                            const std::string& key_name) const {
    DCHECK(IsKeyExisted<ParentType>(parent, key_name));
    return parent.find(key_name)->second;
  }

  bool CheckPolicyTimeStamp(const std::string& str) const;
};

class PolicyManagerImplTest_RequestTypes : public ::testing::Test {
 public:
  PolicyManagerImplTest_RequestTypes();

 protected:
  const ::policy::StringArray kJsonFiles;
  const std::string kAppId;
  const std::string kDefaultAppId;
  const std::string app_storage_folder_;
  const std::string preloaded_pt_filename_;

  utils::SharedPtr<PolicyManagerImpl> policy_manager_impl_sptr_;
  NiceMock<MockPolicyListener> listener_;
  NiceMock<policy_handler_test::MockPolicySettings> policy_settings_;

  void SetUp() OVERRIDE;

  const Json::Value GetPTU(const std::string& file_name);

  void RefreshPT(const std::string& preloaded_pt_file,
                 const std::string& update_pt_file);

  PolicyTableSPtr GetPolicyTableSnapshot();

  policy_table::RequestTypes GetRequestTypesForApplication(
      const std::string& app_id);

  void CompareAppRequestTypesWithDefault(const std::string& app_id,
                                         const std::string& ptu_file);

  policy_table::RequestTypes CreateDefaultAppPTURequestValues();

  policy_table::RequestTypes CreateDefaultAppDatabaseRequestValues();

  policy_table::RequestTypes CreatePreDataConsentAppPTURequestValues();

  void CompareRequestTypesContainers(
      const policy_table::RequestTypes& expected_data,
      const policy_table::RequestTypes& received_data);

  void TearDown() OVERRIDE;
};

class PolicyManagerImplTest_ExternalConsent : public PolicyManagerImplTest2 {
 public:
  PolicyManagerImplTest_ExternalConsent()
      : PolicyManagerImplTest2()
      , group_name_1_("Group1")
      , group_name_2_("Group2")
      , group_name_3_("Group3") {}

 protected:
  void PreconditionExternalConsentPreparePTWithAppGroupsAndConsents();

  void PreconditionExternalConsentPreparePTWithAppPolicy();

  policy_table::Table PreparePTWithGroupsHavingExternalConsent();

  std::string PreparePTUWithNewGroup(const uint32_t type,
                                     const uint32_t id,
                                     const std::string& group_name);

  const uint32_t type_1_ = 0;
  const uint32_t id_1_ = 1;
  const uint32_t type_2_ = 2;
  const uint32_t id_2_ = 3;
  const uint32_t type_3_ = 4;
  const uint32_t id_3_ = 5;

  const std::string group_name_1_;
  const std::string group_name_2_;
  const std::string group_name_3_;
};

}  // namespace policy_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_TEST_INCLUDE_POLICY_POLICY_MANAGER_IMPL_TEST_BASE_H_
