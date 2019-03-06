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

#ifndef SRC_COMPONENTS_INCLUDE_TEST_POLICY_POLICY_EXTERNAL_POLICY_MOCK_POLICY_MANAGER_H_
#define SRC_COMPONENTS_INCLUDE_TEST_POLICY_POLICY_EXTERNAL_POLICY_MOCK_POLICY_MANAGER_H_

#include <string>
#include <vector>
#include "gmock/gmock.h"
#include "policy/policy_listener.h"
#include "policy/policy_types.h"
#include "policy/usage_statistics/statistics_manager.h"

#include "rpc_base/rpc_base.h"
#include "policy/policy_table/types.h"
#include "policy/policy_manager.h"

namespace policy_table = ::rpc::policy_table_interface_base;

using namespace policy;

namespace test {
namespace components {
namespace policy_manager_test {

class MockPolicyManager : public PolicyManager {
 public:
  MOCK_METHOD1(set_listener, void(PolicyListener* listener));
  MOCK_METHOD2(InitPT,
               bool(const std::string& file_name,
                    const PolicySettings* settings));
  MOCK_METHOD2(LoadPT,
               bool(const std::string& file, const BinaryMessage& pt_content));
  MOCK_METHOD1(ResetPT, bool(const std::string& file_name));
  MOCK_METHOD1(GetUpdateUrl, std::string(int service_type));
  MOCK_METHOD2(GetUpdateUrls,
               void(const uint32_t service_type, EndpointUrls& out_end_points));
  MOCK_METHOD2(GetUpdateUrls,
               void(const std::string& service_type,
                    EndpointUrls& out_end_points));
  MOCK_METHOD0(RequestPTUpdate, void());
  MOCK_METHOD5(CheckPermissions,
               void(const PTString& app_id,
                    const PTString& hmi_level,
                    const PTString& rpc,
                    const RPCParams& rpc_params,
                    CheckPermissionResult& result));
  MOCK_METHOD0(ResetUserConsent, bool());
  MOCK_CONST_METHOD0(GetPolicyTableStatus, std::string());
  MOCK_METHOD1(KmsChanged, void(int kilometers));
  MOCK_METHOD0(IncrementIgnitionCycles, void());
  MOCK_METHOD0(ForcePTExchange, std::string());
  MOCK_METHOD0(ForcePTExchangeAtUserRequest, std::string());
  MOCK_METHOD0(ResetRetrySequence, void());
  MOCK_METHOD0(NextRetryTimeout, int());
  MOCK_METHOD0(TimeoutExchangeMSec, uint32_t());
  MOCK_METHOD0(RetrySequenceDelaysSeconds, const std::vector<int>());
  MOCK_METHOD0(OnExceededTimeout, void());
  MOCK_METHOD0(OnUpdateStarted, void());
  MOCK_CONST_METHOD1(GetUserConsentForDevice,
                     DeviceConsent(const std::string& device_id));
  MOCK_METHOD3(
      GetUserConsentForApp,
      void(const std::string& device_id,
           const std::string& policy_app_id,
           std::vector<policy::FunctionalGroupPermission>& permissions));
  MOCK_METHOD2(SetUserConsentForDevice,
               void(const std::string& device_id, const bool is_allowed));
  MOCK_METHOD2(ReactOnUserDevConsentForApp,
               bool(const std::string& app_id, bool is_device_allowed));
  MOCK_METHOD2(PTUpdatedAt, void(policy::Counters counter, int value));

  MOCK_METHOD3(GetInitialAppData,
               bool(const std::string&,
                    policy::StringArray*,
                    policy::StringArray*));

  MOCK_METHOD2(AddDevice,
               void(const std::string& device_id,
                    const std::string& connection_type));
  MOCK_METHOD2(SetDeviceInfo,
               void(const std::string& device_id,
                    const policy::DeviceInfo& device_info));
  MOCK_METHOD2(SetUserConsentForApp,
               void(const policy::PermissionConsent& permissions,
                    const policy::PolicyManager::NotificationMode mode));
  MOCK_CONST_METHOD2(GetDefaultHmi,
                     bool(const std::string& policy_app_id,
                          std::string* default_hmi));
  MOCK_CONST_METHOD2(GetPriority,
                     bool(const std::string& policy_app_id,
                          std::string* priority));
  MOCK_METHOD3(GetUserFriendlyMessages,
               std::vector<policy::UserFriendlyMessage>(
                   const std::vector<std::string>& message_code,
                   const std::string& language,
                   const std::string& active_hmi_language));
  MOCK_CONST_METHOD1(IsApplicationRevoked, bool(const std::string& app_id));
  MOCK_METHOD3(
      GetPermissionsForApp,
      void(const std::string& device_id,
           const std::string& policy_app_id,
           std::vector<policy::FunctionalGroupPermission>& permissions));
  MOCK_METHOD1(GetAppPermissionsChanges,
               policy::AppPermissions(const std::string& policy_app_id));
  MOCK_METHOD1(RemovePendingPermissionChanges, void(const std::string& app_id));
  MOCK_CONST_METHOD1(GetCurrentDeviceId,
                     std::string&(const std::string& policy_app_id));
  MOCK_METHOD1(SetSystemLanguage, void(const std::string& language));
  MOCK_METHOD3(SetSystemInfo,
               void(const std::string& ccpu_version,
                    const std::string& wers_country_code,
                    const std::string& language));
  MOCK_METHOD1(SendNotificationOnPermissionsUpdated,
               void(const std::string& application_id));
  MOCK_METHOD1(MarkUnpairedDevice, void(const std::string& device_id));
  MOCK_METHOD2(
      AddApplication,
      StatusNotifier(
          const std::string& application_id,
          const rpc::policy_table_interface_base::AppHmiTypes& hmi_types));
  MOCK_METHOD2(SetDefaultHmiTypes,
               void(const std::string& application_id,
                    const std::vector<int>& hmi_types));
  MOCK_METHOD2(GetHMITypes,
               bool(const std::string& application_id,
                    std::vector<int>* app_types));
  MOCK_METHOD2(CheckModule,
               bool(const PTString& app_id, const PTString& module));
  MOCK_METHOD2(SendAppPermissionsChanged,
               void(const std::string& device_id,
                    const std::string& application_id));
  MOCK_CONST_METHOD2(GetModuleTypes,
                     bool(const std::string& policy_app_id,
                          std::vector<std::string>* modules));
  MOCK_METHOD1(set_access_remote,
               void(std::shared_ptr<AccessRemote> access_remote));

  MOCK_METHOD0(CleanupUnpairedDevices, bool());
  MOCK_CONST_METHOD1(CanAppKeepContext, bool(const std::string& app_id));
  MOCK_CONST_METHOD1(CanAppStealFocus, bool(const std::string& app_id));
  MOCK_METHOD0(OnSystemReady, void());
  MOCK_CONST_METHOD1(GetNotificationsNumber,
                     uint32_t(const std::string& priority));
  MOCK_METHOD1(SetVINValue, void(const std::string& value));
  MOCK_CONST_METHOD1(IsPredataPolicy, bool(const std::string& policy_app_id));
  MOCK_CONST_METHOD1(HeartBeatTimeout, uint32_t(const std::string& app_id));
  MOCK_METHOD1(SaveUpdateStatusRequired, void(bool is_update_needed));
  MOCK_METHOD0(OnAppsSearchStarted, void());
  MOCK_METHOD1(OnAppsSearchCompleted, void(const bool trigger_ptu));
  MOCK_METHOD1(OnAppRegisteredOnMobile,
               void(const std::string& application_id));
  MOCK_CONST_METHOD0(GetLockScreenIconUrl, std::string());
  MOCK_CONST_METHOD1(GetIconUrl, std::string(const std::string& policy_app_id));
  MOCK_CONST_METHOD1(
      GetAppRequestTypes,
      const std::vector<std::string>(const std::string policy_app_id));
  MOCK_CONST_METHOD0(GetVehicleInfo, const policy::VehicleInfo());
  MOCK_CONST_METHOD1(GetEnabledCloudApps,
                     void(std::vector<std::string>& enabled_apps));
  MOCK_CONST_METHOD7(GetCloudAppParameters,
                     void(const std::string& policy_app_id,
                          bool& enabled,
                          std::string& endpoint,
                          std::string& certificate,
                          std::string& auth_token,
                          std::string& cloud_transport_type,
                          std::string& hybrid_app_preference));
  MOCK_METHOD1(InitCloudApp, void(const std::string& policy_app_id));
  MOCK_METHOD2(SetCloudAppEnabled,
               void(const std::string& policy_app_id, const bool enabled));
  MOCK_METHOD2(SetAppAuthToken,
               void(const std::string& policy_app_id,
                    const std::string& auth_token));
  MOCK_METHOD2(SetAppCloudTransportType,
               void(const std::string& policy_app_id,
                    const std::string& cloud_transport_type));
  MOCK_METHOD2(SetAppEndpoint,
               void(const std::string& policy_app_id,
                    const std::string& endpoint));
  MOCK_METHOD2(SetAppNicknames,
               void(const std::string& policy_app_id,
                    const StringArray& nicknames));
  MOCK_METHOD2(SetHybridAppPreference,
               void(const std::string& policy_app_id,
                    const std::string& hybrid_app_preference));
  MOCK_CONST_METHOD0(GetMetaInfo, const policy::MetaInfo());
  MOCK_CONST_METHOD0(RetrieveCertificate, std::string());
  MOCK_CONST_METHOD0(HasCertificate, bool());
  MOCK_METHOD1(SetDecryptedCertificate, void(const std::string&));
  MOCK_METHOD0(ExceededIgnitionCycles, bool());
  MOCK_METHOD0(ExceededDays, bool());
  MOCK_METHOD0(StartPTExchange, void());
  MOCK_METHOD1(Increment, void(usage_statistics::GlobalCounterId type));
  MOCK_METHOD2(Increment,
               void(const std::string& app_id,
                    usage_statistics::AppCounterId type));
  MOCK_METHOD3(Set,
               void(const std::string& app_id,
                    usage_statistics::AppInfoId type,
                    const std::string& value));
  MOCK_METHOD3(Add,
               void(const std::string& app_id,
                    usage_statistics::AppStopwatchId type,
                    int32_t timespan_seconds));
  MOCK_CONST_METHOD0(get_settings, const PolicySettings&());
  MOCK_METHOD1(set_settings, void(const PolicySettings* get_settings));
  MOCK_METHOD1(GetNextUpdateUrl, AppIdURL(const EndpointUrls& urls));
  MOCK_CONST_METHOD2(RetrySequenceUrl,
                     AppIdURL(const struct RetrySequenceURL&,
                              const EndpointUrls& urls));
  MOCK_METHOD1(SetExternalConsentStatus, bool(const ExternalConsentStatus&));
  MOCK_METHOD0(GetExternalConsentStatus, ExternalConsentStatus());
  MOCK_CONST_METHOD1(IsNeedToUpdateExternalConsentStatus,
                     bool(const ExternalConsentStatus&));
  MOCK_METHOD2(OnDeviceSwitching,
               void(const std::string& device_id_from,
                    const std::string& device_id_to));
  MOCK_CONST_METHOD1(
      GetAppRequestSubTypes,
      const std::vector<std::string>(const std::string& policy_app_id));
  MOCK_CONST_METHOD1(GetAppRequestTypesState,
                     RequestType::State(const std::string& policy_app_id));
  MOCK_CONST_METHOD1(GetAppRequestSubTypesState,
                     RequestSubType::State(const std::string& policy_app_id));
};
}  // namespace policy_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_INCLUDE_TEST_POLICY_POLICY_EXTERNAL_POLICY_MOCK_POLICY_MANAGER_H_
