/*
 * Copyright (c) 2015, Ford Motor Company
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


#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_MOCK_POLICY_HANDLER_INTERFACE_MOCK_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_MOCK_POLICY_HANDLER_INTERFACE_MOCK_H_

#include "application_manager/policies/policy_handler_interface.h"
#include "gmock/gmock.h"
#include "policy/policy_types.h"


namespace policy {

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;
class MockPolicyHandlerInterface : public policy::PolicyHandlerInterface {
 public:
  MOCK_METHOD0(LoadPolicyLibrary, bool());
  MOCK_CONST_METHOD0(PolicyEnabled, bool());
  MOCK_METHOD0(InitPolicyTable, bool());
  MOCK_METHOD0(ResetPolicyTable, bool());
  MOCK_METHOD0(ClearUserConsent, bool());
  MOCK_METHOD2(SendMessageToSDK, bool(const policy::BinaryMessage& pt_string,
                                      const std::string& url));
  MOCK_METHOD2(ReceiveMessageFromSDK,
               bool(const std::string& file,
                    const policy::BinaryMessage& pt_string));
  MOCK_METHOD0(UnloadPolicyLibrary, bool());
  MOCK_METHOD3(OnPermissionsUpdated,
               void(const std::string& policy_app_id,
                    const policy::Permissions& permissions,
                    const policy::HMILevel& default_hmi));
  MOCK_METHOD2(OnPermissionsUpdated,
               void(const std::string& policy_app_id,
                    const policy::Permissions& permissions));
  MOCK_METHOD3(OnSnapshotCreated,
               void(const policy::BinaryMessage& pt_string,
                    const std::vector<int>& retry_delay_seconds,
                    int timeout_exchange));
  MOCK_CONST_METHOD2(GetPriority, bool(const std::string& policy_app_id,
                                       std::string* priority));
  MOCK_METHOD5(CheckPermissions,
               void(const policy::PTString& app_id,
                    const policy::PTString& hmi_level,
                    const policy::PTString& rpc, const policy::RPCParams& rpc_params,
                    policy::CheckPermissionResult& result));
  MOCK_CONST_METHOD1(GetNotificationsNumber,
                     uint32_t(const std::string& priority));
  MOCK_CONST_METHOD1(GetUserConsentForDevice,
                     policy::DeviceConsent(const std::string& device_id));
  MOCK_CONST_METHOD2(GetDefaultHmi, bool(const std::string& policy_app_id,
                                         std::string* default_hmi));

  MOCK_METHOD3(GetInitialAppData,
               bool(const std::string& application_id, policy::StringArray* nicknames,
                    policy::StringArray* app_hmi_types));
  MOCK_METHOD2(GetInitialAppData,
               bool(const std::string& application_id, policy::StringArray* nicknames));
  MOCK_METHOD1(GetInitialAppData,
               bool(const std::string& application_id));
  MOCK_METHOD2(GetUpdateUrls,
               void(int service_type, policy::EndpointUrls& end_points));
  MOCK_METHOD0(ResetRetrySequence, void());
  MOCK_METHOD0(NextRetryTimeout, int());
  MOCK_METHOD0(TimeoutExchange, int());
  MOCK_METHOD0(OnExceededTimeout, void());
  MOCK_METHOD0(OnSystemReady, void());
  MOCK_METHOD2(PTUpdatedAt, void(policy::Counters counter, int value));
  MOCK_METHOD1(add_listener, void(policy::PolicyHandlerObserver* listener));
  MOCK_METHOD1(remove_listener, void(policy::PolicyHandlerObserver* listener));
  MOCK_CONST_METHOD0(GetStatisticManager,
                     utils::SharedPtr<usage_statistics::StatisticsManager>());
  MOCK_CONST_METHOD2(CheckSystemAction,
                     bool(mobile_apis::SystemAction::eType system_action,
                          const std::string& policy_app_id));
  MOCK_METHOD1(KmsChanged, void(int kms));
  MOCK_METHOD2(OnActivateApp,
               void(uint32_t connection_key, uint32_t correlation_id));
  MOCK_METHOD2(OnAllowSDLFunctionalityNotification,
               void(bool is_allowed, const std::string& device_id));
  MOCK_METHOD0(OnIgnitionCycleOver, void());
  MOCK_METHOD1(OnPendingPermissionChange,
               void(const std::string& policy_app_id));
  MOCK_METHOD1(PTExchangeAtUserRequest, void(uint32_t correlation_id));
  MOCK_METHOD2(AddDevice, void(const std::string& device_id,
                               const std::string& connection_type));
  MOCK_METHOD2(SetDeviceInfo, void(const std::string& device_id,
                                   const policy::DeviceInfo& device_info));
  MOCK_METHOD2(OnAppPermissionConsent,
               void(const uint32_t connection_key,
                    const policy::PermissionConsent& permissions));
  MOCK_METHOD3(OnGetUserFriendlyMessage,
               void(const std::vector<std::string>& message_codes,
                    const std::string& language, uint32_t correlation_id));
  MOCK_METHOD2(OnGetListOfPermissions, void(const uint32_t connection_key,
                                            const uint32_t correlation_id));
  MOCK_METHOD1(OnGetStatusUpdate, void(const uint32_t correlation_id));
  MOCK_METHOD1(OnUpdateStatusChanged, void(const std::string& status));
  MOCK_METHOD1(OnCurrentDeviceIdUpdateRequired,
               std::string(const std::string& policy_app_id));
  MOCK_METHOD1(OnSystemInfoChanged, void(const std::string& language));
  MOCK_METHOD3(OnGetSystemInfo, void(const std::string& ccpu_version,
                                     const std::string& wers_country_code,
                                     const std::string& language));
  MOCK_METHOD0(OnSystemInfoUpdateRequired, void());
  MOCK_METHOD0(OnVIIsReady, void());
  MOCK_METHOD1(OnVehicleDataUpdated,
               void(const smart_objects::SmartObject& message));
  MOCK_METHOD1(RemoveDevice, void(const std::string& device_id));
  MOCK_METHOD1(AddStatisticsInfo, void(int type));
  MOCK_METHOD1(OnSystemError, void(int code));
  MOCK_CONST_METHOD0(GetAppIdForSending, uint32_t());
  MOCK_METHOD1(GetAppName,
               utils::custom_string::CustomString(const std::string& policy_app_id));
  MOCK_METHOD1(OnUpdateHMIAppType,
               void(std::map<std::string, policy::StringArray> app_hmi_types));
  MOCK_METHOD1(OnCertificateUpdated, void(const std::string& certificate_data));
  MOCK_METHOD1(OnCertificateDecrypted, void(bool is_succeeded));
  MOCK_METHOD0(CanUpdate, bool());
  MOCK_METHOD2(OnDeviceConsentChanged,
               void(const std::string& device_id, bool is_allowed));
  MOCK_CONST_METHOD2(SendOnAppPermissionsChanged,
                     void(const policy::AppPermissions& permissions,
                          const std::string& policy_app_id));
  MOCK_METHOD0(OnPTExchangeNeeded, void());
  MOCK_METHOD1(GetAvailableApps, void(std::queue<std::string>& apps));
  MOCK_METHOD1(AddApplication, void(const std::string& application_id));
  MOCK_METHOD1(IsApplicationRevoked, bool(const std::string& app_id));
  MOCK_METHOD0(OnUpdateRequestSentToMobile, void());
  MOCK_CONST_METHOD1(HeartBeatTimeout, uint32_t(const std::string& app_id));
  MOCK_METHOD0(OnAppsSearchStarted, void());
  MOCK_METHOD0(OnAppsSearchCompleted, void());
  MOCK_METHOD1(OnAppRegisteredOnMobile,
               void(const std::string& application_id));
  MOCK_CONST_METHOD2(IsRequestTypeAllowed,
                     bool(const std::string& policy_app_id,
                          mobile_apis::RequestType::eType type));
  MOCK_CONST_METHOD1(GetAppRequestTypes, const std::vector<std::string>(
                                             const std::string& policy_app_id));
  MOCK_CONST_METHOD0(GetVehicleInfo, const policy::VehicleInfo());
  MOCK_METHOD1(Increment, void(usage_statistics::GlobalCounterId type));
  MOCK_METHOD2(Increment, void(const std::string& app_id,
                               usage_statistics::AppCounterId type));
  MOCK_METHOD3(Set,
               void(const std::string& app_id, usage_statistics::AppInfoId type,
                    const std::string& value));
  MOCK_METHOD3(Add, void(const std::string& app_id,
                         usage_statistics::AppStopwatchId type,
                         int32_t timespan_seconds));
  MOCK_CONST_METHOD0(RetrieveCertificate, std::string());
  MOCK_CONST_METHOD0(get_settings, const policy::PolicySettings&());

 private:
  MOCK_METHOD2(OnAppPermissionConsentInternal,
               void(const uint32_t connection_key,
                    policy::PermissionConsent& permissions));
};

}  // namespace policy

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_MOCK_POLICY_HANDLER_INTERFACE_MOCK_H_

