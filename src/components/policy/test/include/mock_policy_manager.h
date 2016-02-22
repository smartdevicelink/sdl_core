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

#ifndef SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_POLICY_MANAGER_H_
#define SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_POLICY_MANAGER_H_

#include <string>
#include <vector>
#include "gmock/gmock.h"
#include "policy/policy_listener.h"
#include "policy/policy_types.h"
#include "usage_statistics/statistics_manager.h"

#include "rpc_base/rpc_base.h"
#include "./types.h"

namespace policy_table = ::rpc::policy_table_interface_base;

namespace policy_manager {

using namespace policy;

class MockPolicyManager : public PolicyManager {
 public:
  MOCK_METHOD1(set_listener, void(PolicyListener* listener));
  MOCK_METHOD1(InitPT, bool(const std::string& file_name));
  MOCK_METHOD2(LoadPT,
               bool(const std::string& file, const BinaryMessage& pt_content));
  MOCK_METHOD1(ResetPT, bool(const std::string& file_name));
  MOCK_CONST_METHOD1(GetUpdateUrl, std::string(int service_type));
  MOCK_METHOD2(GetUpdateUrls, void(int service_type, EndpointUrls& end_points));
  MOCK_METHOD0(RequestPTUpdate, bool());
  MOCK_METHOD5(CheckPermissions,
               void(const PTString& app_id, const PTString& hmi_level,
                    const PTString& rpc, const RPCParams& rpc_params,
                    CheckPermissionResult& result));
  MOCK_METHOD0(ResetUserConsent, bool());
  MOCK_CONST_METHOD0(GetPolicyTableStatus, std::string());
  MOCK_METHOD1(KmsChanged, void(int kilometers));
  MOCK_METHOD0(IncrementIgnitionCycles, void());
  MOCK_METHOD0(ForcePTExchange, std::string());
  MOCK_METHOD0(ResetRetrySequence, void());
  MOCK_METHOD0(NextRetryTimeout, int());
  MOCK_METHOD0(TimeoutExchange, int());
  MOCK_METHOD0(RetrySequenceDelaysSeconds, const std::vector<int>());
  MOCK_METHOD0(OnExceededTimeout, void());
  MOCK_METHOD0(OnUpdateStarted, void());
  MOCK_METHOD1(GetUserConsentForDevice,
               DeviceConsent(const std::string& device_id));
  MOCK_METHOD3(GetUserConsentForApp,
               void(const std::string& device_id,
                    const std::string& policy_app_id,
                    std::vector<policy::FunctionalGroupPermission>& permissions));
  MOCK_METHOD2(SetUserConsentForDevice,
               void(const std::string& device_id, bool is_allowed));
  MOCK_METHOD2(ReactOnUserDevConsentForApp,
               bool(const std::string app_id, bool is_device_allowed));
  MOCK_METHOD2(PTUpdatedAt, void(policy::Counters counter, int value));

  MOCK_METHOD3(GetInitialAppData,
               bool(const std::string&, policy::StringArray*, policy::StringArray*));

  MOCK_METHOD2(AddDevice, void(const std::string& device_id,
                               const std::string& connection_type));
  MOCK_METHOD2(SetDeviceInfo, void(const std::string& device_id,
                                   const policy::DeviceInfo& device_info));
  MOCK_METHOD1(SetUserConsentForApp,
               void(const policy::PermissionConsent& permissions));
  MOCK_METHOD2(GetDefaultHmi, bool(const std::string& policy_app_id,
                                   std::string* default_hmi));
  MOCK_METHOD2(GetPriority,
               bool(const std::string& policy_app_id, std::string* priority));
  MOCK_METHOD2(GetUserFriendlyMessages,
               std::vector<policy::UserFriendlyMessage>(
                   const std::vector<std::string>& message_code,
                   const std::string& language));
  MOCK_CONST_METHOD1(IsApplicationRevoked, bool(const std::string& app_id));
  MOCK_METHOD3(GetPermissionsForApp,
               void(const std::string& device_id,
                    const std::string& policy_app_id,
                    std::vector<policy::FunctionalGroupPermission>& permissions));
  MOCK_METHOD1(GetAppPermissionsChanges,
               policy::AppPermissions(const std::string& policy_app_id));
  MOCK_METHOD1(RemovePendingPermissionChanges, void(const std::string& app_id));
  MOCK_METHOD1(GetCurrentDeviceId,
               std::string&(const std::string& policy_app_id));
  MOCK_METHOD1(SetSystemLanguage, void(const std::string& language));
  MOCK_METHOD3(SetSystemInfo, void(const std::string& ccpu_version,
                                   const std::string& wers_country_code,
                                   const std::string& language));
  MOCK_METHOD1(SendNotificationOnPermissionsUpdated,
               void(const std::string& application_id));
  MOCK_METHOD1(MarkUnpairedDevice, void(const std::string& device_id));
  MOCK_METHOD1(AddApplication, void(const std::string& application_id));
  MOCK_METHOD0(CleanupUnpairedDevices, bool());
  MOCK_METHOD1(CanAppKeepContext, bool(const std::string& app_id));
  MOCK_METHOD1(CanAppStealFocus, bool(const std::string& app_id));
  MOCK_METHOD0(OnSystemReady, void());
  MOCK_METHOD1(GetNotificationsNumber, uint32_t(const std::string& priority));
  MOCK_METHOD1(SetVINValue, void(const std::string& value));
  MOCK_METHOD1(IsPredataPolicy, bool(const std::string& policy_app_id));
  MOCK_CONST_METHOD1(HeartBeatTimeout, uint32_t(const std::string& app_id));
  MOCK_METHOD1(SaveUpdateStatusRequired, void(bool is_update_needed));
  MOCK_METHOD0(OnAppsSearchStarted, void());
  MOCK_METHOD0(OnAppsSearchCompleted, void());
  MOCK_METHOD1(OnAppRegisteredOnMobile,
               void(const std::string& application_id));
  MOCK_CONST_METHOD1(GetAppRequestTypes, const std::vector<std::string>(
                                             const std::string policy_app_id));
  MOCK_CONST_METHOD0(GetVehicleInfo, const policy::VehicleInfo());
  MOCK_CONST_METHOD0(RetrieveCertificate, std::string());
  MOCK_METHOD1 (SetDecryptedCertificate, void(const std::string&));
  MOCK_METHOD0(ExceededIgnitionCycles, bool());
  MOCK_METHOD0(ExceededDays, bool());
  MOCK_METHOD0(StartPTExchange, void());
  MOCK_METHOD1(Increment, void(usage_statistics::GlobalCounterId type));
  MOCK_METHOD2(Increment, void(const std::string& app_id,
                               usage_statistics::AppCounterId type));
  MOCK_METHOD3(Set,
               void(const std::string& app_id, usage_statistics::AppInfoId type,
                    const std::string& value));
  MOCK_METHOD3(Add, void(const std::string& app_id,
                         usage_statistics::AppStopwatchId type,
                         int32_t timespan_seconds));
};

}  // namespace policy_manager

#endif  // SRC_COMPONENTS_POLICY_TEST_POLICY_INCLUDE_MOCK_POLICY_MANAGER_H_
