/*
 * Copyright (c) 2014, Ford Motor Company
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
#ifndef SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_CACHE_MANAGER_H_
#define SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_CACHE_MANAGER_H_


#include <string>
#include <vector>

#include "gmock/gmock.h"

#include "policy/cache_manager_interface.h"

namespace policy_table = rpc::policy_table_interface_base;

namespace policy {

class MockCacheManagerInterface : public CacheManagerInterface {
 public:
  MOCK_METHOD4(CheckPermissions,
      void(const PTString& app_id, const PTString& hmi_level, const PTString& rpc, CheckPermissionResult& result));
  MOCK_METHOD0(IsPTPreloaded,
      bool());
  MOCK_METHOD0(IgnitionCyclesBeforeExchange,
      int());
  MOCK_METHOD1(KilometersBeforeExchange,
      int(int current));
  MOCK_METHOD2(SetCountersPassedForSuccessfulUpdate,
      bool(Counters counter, int value));
  MOCK_METHOD1(DaysBeforeExchange,
      int(int current));
  MOCK_METHOD0(IncrementIgnitionCycles,
      void());
  MOCK_METHOD0(ResetIgnitionCycles,
      void());
  MOCK_METHOD0(TimeoutResponse,
      int());
  MOCK_METHOD1(SecondsBetweenRetries,
      bool(std::vector<int> &seconds));
  MOCK_CONST_METHOD0(GetVehicleInfo,
      const VehicleInfo());
  MOCK_METHOD1(SetVINValue,
      bool(const std::string& value));
  MOCK_METHOD2(GetUserFriendlyMsg,
      std::vector<UserFriendlyMessage>(const std::vector<std::string>& msg_codes, const std::string& language));
  MOCK_METHOD2(GetUpdateUrls,
      void(int service_type, EndpointUrls& end_points));
  MOCK_METHOD1(GetNotificationsNumber,
      policy_table::NumberOfNotificationsType(const std::string& priority));
  MOCK_CONST_METHOD2(GetPriority,
      bool(const std::string& policy_app_id, std::string& priority));
  MOCK_METHOD2(GetServiceUrls,
      void(const std::string& service_type, EndpointUrls& end_points));
  MOCK_CONST_METHOD0(GetLockScreenIconUrl,
      std::string());
  MOCK_METHOD2(Init,
      bool(const std::string& file_name, const PolicySettings* settings));
  MOCK_METHOD0(GenerateSnapshot,
      utils::SharedPtr<policy_table::Table>());
  MOCK_METHOD1(ApplyUpdate,
      bool(const policy_table::Table& update_pt));
  MOCK_METHOD1(Save,
      bool(const policy_table::Table& table));
  MOCK_CONST_METHOD0(UpdateRequired,
      bool());
  MOCK_METHOD1(SaveUpdateRequired,
      void(bool status));
  MOCK_METHOD3(GetInitialAppData,
      bool(const std::string& app_id, StringArray& nicknames, StringArray& app_hmi_types));
  MOCK_CONST_METHOD1(IsApplicationRevoked,
      bool(const std::string& app_id));
  MOCK_METHOD1(GetFunctionalGroupings,
      bool(policy_table::FunctionalGroupings& groups));
  MOCK_CONST_METHOD1(IsApplicationRepresented,
      bool(const std::string& app_id));
  MOCK_METHOD1(IsDefaultPolicy,
      bool(const std::string& app_id));
  MOCK_METHOD1(SetIsDefault,
      bool(const std::string& app_id));
  MOCK_METHOD1(IsPredataPolicy,
      bool(const std::string& app_id));
  MOCK_METHOD1(SetDefaultPolicy,
      bool(const std::string& app_id));
  MOCK_CONST_METHOD1(CanAppKeepContext,
      bool(const std::string& app_id));
  MOCK_CONST_METHOD1(CanAppStealFocus,
      bool(const std::string& app_id));
  MOCK_CONST_METHOD2(GetDefaultHMI,
      bool(const std::string& app_id, std::string &default_hmi));
  MOCK_METHOD0(ResetUserConsent,
      bool());
  MOCK_CONST_METHOD3(GetUserPermissionsForDevice,
      bool(const std::string& device_id, StringArray &consented_groups, StringArray &disallowed_groups));
  MOCK_METHOD3(GetPermissionsForApp,
      bool(const std::string& device_id, const std::string& app_id, FunctionalIdType &group_types));
  MOCK_CONST_METHOD2(GetDeviceGroupsFromPolicies,
      bool(rpc::policy_table_interface_base::Strings &groups, rpc::policy_table_interface_base::Strings &preconsented_groups));
  MOCK_METHOD2(AddDevice,
      bool(const std::string& device_id, const std::string& connection_type));
  MOCK_METHOD8(SetDeviceData,
      bool(const std::string& device_id, const std::string& hardware, const std::string& firmware, const std::string& os, const std::string& os_version, const std::string& carrier, const uint32_t number_of_ports, const std::string& connection_type));
  MOCK_METHOD3(SetUserPermissionsForDevice,
      bool(const std::string& device_id, const StringArray& consented_groups, const StringArray& disallowed_groups));
  MOCK_METHOD2(ReactOnUserDevConsentForApp,
      bool(const std::string& app_id, bool is_device_allowed));
  MOCK_METHOD1(SetUserPermissionsForApp,
      bool(const PermissionConsent& permissions));
  MOCK_METHOD3(SetMetaInfo,
      bool(const std::string& ccpu_version, const std::string& wers_country_code, const std::string& language));
  MOCK_CONST_METHOD0(IsMetaInfoPresent,
      bool());
  MOCK_METHOD1(SetSystemLanguage,
      bool(const std::string& language));
  MOCK_METHOD1(Increment,
      void(usage_statistics::GlobalCounterId type));
  MOCK_METHOD2(Increment,
      void(const std::string& app_id, usage_statistics::AppCounterId type));
  MOCK_METHOD3(Set,
      void(const std::string& app_id, usage_statistics::AppInfoId type, const std::string& value));
  MOCK_METHOD3(Add,
      void(const std::string& app_id, usage_statistics::AppStopwatchId type, int seconds));
  MOCK_METHOD2(CountUnconsentedGroups,
      int(const std::string& policy_app_id, const std::string& device_id));
  MOCK_METHOD1(GetFunctionalGroupNames,
      bool(FunctionalGroupNames& names));
  MOCK_METHOD2(GetAllAppGroups,
      void(const std::string& app_id, FunctionalGroupIDs& all_group_ids));
  MOCK_METHOD2(GetPreConsentedGroups,
      void(const std::string &app_id, FunctionalGroupIDs& preconsented_groups));
  MOCK_METHOD4(GetConsentedGroups,
      void(const std::string &device_id, const std::string &app_id, FunctionalGroupIDs& allowed_groups, FunctionalGroupIDs& disallowed_groups));
  MOCK_METHOD3(GetUnconsentedGroups,
      void(const std::string& device_id, const std::string& policy_app_id, FunctionalGroupIDs& unconsented_groups));
  MOCK_METHOD2(RemoveAppConsentForGroup,
      void(const std::string& app_id, const std::string& group_name));
  MOCK_METHOD1(SetPredataPolicy,
      bool(const std::string& app_id));
  MOCK_METHOD0(CleanupUnpairedDevices,
      bool());
  MOCK_METHOD2(SetUnpairedDevice,
      bool(const std::string& device_id, bool unpaired));
  MOCK_METHOD1(UnpairedDevicesList,
      bool(DeviceIds& device_ids));
  MOCK_METHOD1(ResetPT,
      bool(const std::string& file_name));
  MOCK_METHOD0(LoadFromBackup,
      bool());
  MOCK_METHOD2(LoadFromFile,
      bool(const std::string& file_name, policy_table::Table&));
  MOCK_METHOD0(Backup,
      void());
  MOCK_CONST_METHOD1(HeartBeatTimeout,
      uint32_t(const std::string& app_id));
  MOCK_CONST_METHOD2(GetAppRequestTypes,
      void(const std::string& policy_app_id,
           std::vector<std::string>& request_types));
  MOCK_METHOD1(GetHMIAppTypeAfterUpdate,
        void(std::map<std::string, StringArray>& app_hmi_types));
  MOCK_METHOD0(ResetCalculatedPermissions,
               void());
  MOCK_METHOD3(AddCalculatedPermissions,
               void(const std::string& device_id, const std::string& policy_app_id, const policy::Permissions& permissions));
  MOCK_METHOD3(IsPermissionsCalculated,
               bool(const std::string& device_id, const std::string& policy_app_id, policy::Permissions& permission));
  MOCK_CONST_METHOD0(GetPT, utils::SharedPtr<policy_table::Table>());
  MOCK_CONST_METHOD0(GetCertificate, std::string());
  MOCK_METHOD1(SetDecryptedCertificate, void(const std::string&));
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_CACHE_MANAGER_H_
