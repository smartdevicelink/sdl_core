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

#ifndef SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_POLICIES_MOCK_POLICY_HANDLER_INTERFACE_H_
#define SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_POLICIES_MOCK_POLICY_HANDLER_INTERFACE_H_

#include "application_manager/application_manager.h"
#include "application_manager/policies/policy_encryption_flag_getter.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "gmock/gmock.h"
#include "policy/policy_types.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace policy_test {

class MockPolicyHandlerInterface : public policy::PolicyHandlerInterface {
 public:
  MOCK_METHOD0(LoadPolicyLibrary, bool());
  MOCK_CONST_METHOD0(PolicyEncryptionFlagGetter,
                     policy::PolicyEncryptionFlagGetterInterfaceSPtr());
  MOCK_CONST_METHOD0(PolicyEnabled, bool());
  MOCK_METHOD0(InitPolicyTable, bool());
  MOCK_METHOD0(ResetPolicyTable, bool());
  MOCK_METHOD0(ClearUserConsent, bool());
  MOCK_METHOD2(SendMessageToSDK,
               bool(const policy::BinaryMessage& pt_string,
                    const std::string& url));
  MOCK_METHOD2(ReceiveMessageFromSDK,
               bool(const std::string& file,
                    const policy::BinaryMessage& pt_string));
  MOCK_METHOD0(UnloadPolicyLibrary, bool());
  MOCK_METHOD4(OnPermissionsUpdated,
               void(const std::string& device_id,
                    const std::string& policy_app_id,
                    const policy::Permissions& permissions,
                    const policy::HMILevel& default_hmi));
  MOCK_METHOD3(OnPermissionsUpdated,
               void(const std::string& device_id,
                    const std::string& policy_app_id,
                    const policy::Permissions& permissions));
  MOCK_CONST_METHOD0(GetPolicyTableData, Json::Value());
  MOCK_CONST_METHOD0(
      GetVehicleDataItems,
      const std::vector<rpc::policy_table_interface_base::VehicleDataItem>());
  MOCK_CONST_METHOD0(
      GetRemovedVehicleDataItems,
      std::vector<rpc::policy_table_interface_base::VehicleDataItem>());

#ifdef EXTERNAL_PROPRIETARY_MODE
  MOCK_METHOD3(OnSnapshotCreated,
               void(const policy::BinaryMessage& pt_string,
                    const std::vector<int>& retry_delay_seconds,
                    uint32_t timeout_exchange));
#else   // EXTERNAL_PROPRIETARY_MODE
  MOCK_METHOD2(OnSnapshotCreated,
               void(const policy::BinaryMessage& pt_string,
                    const policy::PTUIterationType iteration_type));
  MOCK_METHOD2(GetNextUpdateUrl,
               std::string(const policy::PTUIterationType iteration_type,
                           uint32_t& app_id));
#endif  // EXTERNAL_PROPRIETARY_MODE

  MOCK_CONST_METHOD2(GetPriority,
                     bool(const std::string& policy_app_id,
                          std::string* priority));
  MOCK_METHOD5(CheckPermissions,
               void(const application_manager::ApplicationSharedPtr app,
                    const application_manager::WindowID window_id,
                    const policy::PTString& rpc,
                    const application_manager::RPCParams& rpc_params,
                    policy::CheckPermissionResult& result));
  MOCK_CONST_METHOD1(GetNotificationsNumber,
                     uint32_t(const std::string& priority));
  MOCK_CONST_METHOD1(GetUserConsentForDevice,
                     policy::DeviceConsent(const std::string& device_id));
  MOCK_CONST_METHOD3(GetDefaultHmi,
                     bool(const std::string& device_id,
                          const std::string& policy_app_id,
                          std::string* default_hmi));

  MOCK_METHOD3(GetInitialAppData,
               bool(const std::string& application_id,
                    policy::StringArray* nicknames,
                    policy::StringArray* app_hmi_types));
  MOCK_METHOD2(GetInitialAppData,
               bool(const std::string& application_id,
                    policy::StringArray* nicknames));
  MOCK_METHOD1(GetInitialAppData, bool(const std::string& application_id));
  MOCK_CONST_METHOD2(GetUpdateUrls,
                     void(const uint32_t service_type,
                          policy::EndpointUrls& end_points));
  MOCK_CONST_METHOD1(GetLockScreenIconUrl,
                     std::string(const std::string& policy_app_id));
  MOCK_CONST_METHOD1(GetIconUrl, std::string(const std::string& policy_app_id));
  MOCK_METHOD0(ResetRetrySequence, void());
  MOCK_METHOD0(NextRetryTimeout, uint32_t());
  MOCK_CONST_METHOD0(TimeoutExchangeSec, uint32_t());
  MOCK_CONST_METHOD0(TimeoutExchangeMSec, uint32_t());
  MOCK_METHOD0(OnExceededTimeout, void());
  MOCK_CONST_METHOD0(LockScreenDismissalEnabledState,
                     const boost::optional<bool>());
  MOCK_CONST_METHOD1(LockScreenDismissalWarningMessage,
                     const boost::optional<std::string>(const std::string&));
  MOCK_METHOD2(PTUpdatedAt, void(policy::Counters counter, int value));
  MOCK_METHOD1(add_listener, void(policy::PolicyHandlerObserver* listener));
  MOCK_METHOD1(remove_listener, void(policy::PolicyHandlerObserver* listener));
  MOCK_CONST_METHOD0(GetStatisticManager,
                     std::shared_ptr<usage_statistics::StatisticsManager>());
  MOCK_CONST_METHOD2(CheckSystemAction,
                     bool(mobile_apis::SystemAction::eType system_action,
                          const std::string& policy_app_id));
  MOCK_METHOD1(KmsChanged, void(int kms));
  MOCK_METHOD2(OnActivateApp,
               void(uint32_t connection_key, uint32_t correlation_id));
  MOCK_METHOD2(OnAllowSDLFunctionalityNotification,
               void(bool is_allowed, const std::string& device_id));
  MOCK_METHOD0(OnIgnitionCycleOver, void());
  MOCK_METHOD2(OnPendingPermissionChange,
               void(const std::string& device_id,
                    const std::string& policy_app_id));
  MOCK_METHOD1(PTExchangeAtUserRequest, void(uint32_t correlation_id));
  MOCK_METHOD2(AddDevice,
               void(const std::string& device_id,
                    const std::string& connection_type));
  MOCK_METHOD2(SetDeviceInfo,
               void(const std::string& device_id,
                    const policy::DeviceInfo& device_info));
#ifdef EXTERNAL_PROPRIETARY_MODE
  MOCK_METHOD3(
      OnAppPermissionConsent,
      void(const uint32_t connection_key,
           const policy::PermissionConsent& permissions,
           const policy::ExternalConsentStatus& external_consent_status));
#else
  MOCK_METHOD2(OnAppPermissionConsent,
               void(const uint32_t connection_key,
                    const policy::PermissionConsent& permissions));
#endif
  MOCK_METHOD3(OnGetUserFriendlyMessage,
               void(const std::vector<std::string>& message_codes,
                    const std::string& language,
                    uint32_t correlation_id));
  MOCK_METHOD2(OnGetListOfPermissions,
               void(const uint32_t connection_key,
                    const uint32_t correlation_id));
  MOCK_METHOD1(OnGetStatusUpdate, void(const uint32_t correlation_id));
  MOCK_METHOD1(OnUpdateStatusChanged, void(const std::string& status));
  MOCK_METHOD2(OnCurrentDeviceIdUpdateRequired,
               std::string(const transport_manager::DeviceHandle& device_handle,
                           const std::string& policy_app_id));
  MOCK_METHOD1(OnSystemInfoChanged, void(const std::string& language));
  MOCK_METHOD3(OnGetSystemInfo,
               void(const std::string& ccpu_version,
                    const std::string& wers_country_code,
                    const std::string& language));
  MOCK_METHOD0(OnVIIsReady, void());
  MOCK_METHOD1(OnVehicleDataUpdated,
               void(const smart_objects::SmartObject& message));
  MOCK_METHOD1(RemoveDevice, void(const std::string& device_id));
  MOCK_METHOD1(AddStatisticsInfo, void(int type));
  MOCK_METHOD1(OnSystemError, void(int code));
#ifndef EXTERNAL_PROPRIETARY_MODE
  MOCK_METHOD1(ChoosePTUApplication,
               uint32_t(const policy::PTUIterationType iteration_type));
  MOCK_METHOD3(CacheRetryInfo,
               void(const uint32_t app_id,
                    const std::string url,
                    const std::string snapshot_path));
#endif
  MOCK_CONST_METHOD0(GetAppIdForSending, uint32_t());
  MOCK_METHOD1(
      GetAppName,
      utils::custom_string::CustomString(const std::string& policy_app_id));
  MOCK_METHOD1(OnUpdateHMIAppType,
               void(std::map<std::string, policy::StringArray> app_hmi_types));
  MOCK_METHOD1(OnCertificateUpdated, void(const std::string& certificate_data));
  MOCK_METHOD1(OnPTUFinished, void(const bool ptu_result));
  MOCK_METHOD0(OnPTInited, void());
  MOCK_METHOD0(StopRetrySequence, void());
  MOCK_METHOD1(OnCertificateDecrypted, void(bool is_succeeded));
  MOCK_METHOD0(CanUpdate, bool());
  MOCK_METHOD2(OnDeviceConsentChanged,
               void(const std::string& device_id, bool is_allowed));
  MOCK_CONST_METHOD3(SendOnAppPermissionsChanged,
                     void(const policy::AppPermissions& permissions,
                          const std::string& device_id,
                          const std::string& policy_app_id));
  MOCK_CONST_METHOD1(SendOnAppPropertiesChangeNotification,
                     void(const std::string& policy_app_id));
  MOCK_METHOD0(OnPTExchangeNeeded, void());
  MOCK_METHOD0(TriggerPTUOnStartupIfRequired, void());
  MOCK_METHOD1(GetAvailableApps, void(std::queue<std::string>& apps));
  MOCK_METHOD3(
      AddApplication,
      policy::StatusNotifier(
          const std::string& device_id,
          const std::string& application_id,
          const rpc::policy_table_interface_base::AppHmiTypes& hmi_types));
  MOCK_METHOD1(IsApplicationRevoked, bool(const std::string& app_id));
  MOCK_METHOD0(OnUpdateRequestSentToMobile, void());
  MOCK_CONST_METHOD1(HeartBeatTimeout, uint32_t(const std::string& app_id));
  MOCK_METHOD0(OnAppsSearchStarted, void());
  MOCK_METHOD1(OnAppsSearchCompleted, void(const bool trigger_ptu));
  MOCK_METHOD2(OnAddedNewApplicationToAppList,
               void(const uint32_t new_app_id, const std::string& policy_id));
  MOCK_METHOD2(OnAppRegisteredOnMobile,
               void(const std::string& device_id,
                    const std::string& application_id));
  MOCK_CONST_METHOD3(IsRequestTypeAllowed,
                     bool(const transport_manager::DeviceHandle& device_handle,
                          const std::string& policy_app_id,
                          mobile_apis::RequestType::eType type));
  MOCK_CONST_METHOD2(IsRequestSubTypeAllowed,
                     bool(const std::string& policy_app_id,
                          const std::string& request_subtype));
  MOCK_CONST_METHOD1(
      GetAppRequestTypeState,
      policy::RequestType::State(const std::string& policy_app_id));
  MOCK_CONST_METHOD1(
      GetAppRequestSubTypeState,
      policy::RequestSubType::State(const std::string& policy_app_id));
  MOCK_CONST_METHOD1(
      GetAppRequestSubTypes,
      const std::vector<std::string>(const std::string& policy_app_id));
  MOCK_CONST_METHOD2(GetAppRequestTypes,
                     const std::vector<std::string>(
                         const transport_manager::DeviceHandle& device_handle,
                         const std::string& policy_app_id));
  MOCK_CONST_METHOD0(GetApplicationPolicyIDs, std::vector<std::string>());
  MOCK_CONST_METHOD1(GetEnabledCloudApps,
                     void(std::vector<std::string>& enabled_apps));
  MOCK_CONST_METHOD1(CheckCloudAppEnabled,
                     const bool(const std::string& policy_app_id));
  MOCK_CONST_METHOD0(GetEnabledLocalApps, std::vector<std::string>());
  MOCK_CONST_METHOD2(GetAppProperties,
                     bool(const std::string& policy_app_id,
                          policy::AppProperties& out_app_properties));

  MOCK_METHOD1(OnSetAppProperties,
               void(const smart_objects::SmartObject& message));
  MOCK_CONST_METHOD2(
      GetAppPropertiesStatus,
      AppPropertiesState(const smart_objects::SmartObject& properties,
                         const std::string& app_id));
  MOCK_CONST_METHOD1(IsNewApplication, bool(const std::string& application_id));
  MOCK_METHOD0(OnLocalAppAdded, void());
  MOCK_METHOD1(OnSetCloudAppProperties,
               void(const smart_objects::SmartObject& message));
  MOCK_CONST_METHOD2(
      GetAppServiceParameters,
      void(const std::string& policy_app_id,
           policy_table::AppServiceParameters* app_service_parameters));
  MOCK_CONST_METHOD4(CheckAppServiceParameters,
                     bool(const std::string& policy_app_id,
                          const std::string& requested_service_name,
                          const std::string& requested_service_type,
                          smart_objects::SmartArray* requested_handled_rpcs));
  MOCK_CONST_METHOD1(UnknownRPCPassthroughAllowed,
                     bool(const std::string& policy_app_id));

#ifdef EXTERNAL_PROPRIETARY_MODE
  MOCK_CONST_METHOD0(GetMetaInfo, const policy::MetaInfo());
  MOCK_METHOD0(IncrementRetryIndex, void());
  MOCK_CONST_METHOD0(ptu_retry_handler, policy::PTURetryHandler&());
#endif  // EXTERNAL_PROPRIETARY_MODE

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
#ifdef ENABLE_SECURITY
  MOCK_CONST_METHOD0(RetrieveCertificate, std::string());
#endif  // ENABLE_SECURITY
  MOCK_CONST_METHOD0(get_settings, const policy::PolicySettings&());
  MOCK_CONST_METHOD0(RemoteAppsUrl, const std::string());
  MOCK_CONST_METHOD2(GetUpdateUrls,
                     void(const std::string& service_type,
                          policy::EndpointUrls& end_points));

  MOCK_METHOD3(OnUpdateHMILevel,
               void(const std::string& device_id,
                    const std::string& policy_app_id,
                    const std::string& hmi_level));
  MOCK_METHOD3(CheckHMIType,
               bool(const std::string& application_id,
                    mobile_apis::AppHMIType::eType hmi,
                    const smart_objects::SmartObject* app_types));

  MOCK_METHOD2(CheckModule,
               bool(const policy::PTString& app_id,
                    const policy::PTString& module));

  MOCK_METHOD2(OnRemoteAppPermissionsChanged,
               void(const std::string& device_id,
                    const std::string& application_id));

  MOCK_METHOD3(OnUpdateHMIStatus,
               void(const std::string& device_id,
                    const std::string& policy_app_id,
                    const std::string& hmi_level));
  MOCK_METHOD0(OnPTUTimeOut, void());
  MOCK_CONST_METHOD2(GetModuleTypes,
                     bool(const std::string& policy_app_id,
                          std::vector<std::string>* modules));
  MOCK_METHOD3(SetDefaultHmiTypes,
               void(const transport_manager::DeviceHandle& device_handle,
                    const std::string& application_id,
                    const smart_objects::SmartObject* app_types));
  MOCK_METHOD2(OnDeviceSwitching,
               void(const std::string& device_id_from,
                    const std::string& device_id_to));

  MOCK_CONST_METHOD0(OnSystemRequestReceived, void());

 private:
#ifdef EXTERNAL_PROPRIETARY_MODE
  MOCK_METHOD3(OnAppPermissionConsentInternal,
               void(const uint32_t,
                    const policy::ExternalConsentStatus&,
                    policy::PermissionConsent&));
#else
  MOCK_METHOD2(OnAppPermissionConsentInternal,
               void(const uint32_t, policy::PermissionConsent&));
#endif
};

}  // namespace policy_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_POLICIES_MOCK_POLICY_HANDLER_INTERFACE_H_
