/*
 Copyright (c) 2016, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_MANAGER_IMPL_H_
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_MANAGER_IMPL_H_

#include <string>
#include <list>
#include <cstdint>

#include "utils/shared_ptr.h"
#include "utils/lock.h"
#include "policy/policy_manager.h"
#include "policy/policy_table.h"
#include "policy/cache_manager_interface.h"
#include "policy/update_status_manager.h"
#include "policy/policy_table/functions.h"
#include "policy/usage_statistics/statistics_manager.h"
#include "policy/policy_helper.h"
#include "utils/timer.h"

namespace policy_table = rpc::policy_table_interface_base;

namespace policy {
struct CheckAppPolicy;

class PolicyManagerImpl : public PolicyManager {
 public:
  PolicyManagerImpl(const std::string& app_storage_folder,
                    uint16_t attempts_to_open_policy_db,
                    uint16_t open_attempt_timeout_ms);
  void set_listener(PolicyListener* listener) OVERRIDE;
  PolicyListener* listener() const {
    return listener_;
  }
  bool InitPT(const std::string& file_name,
              const PolicySettings* settings) OVERRIDE;
  bool LoadPT(const std::string& file,
              const BinaryMessage& pt_content) OVERRIDE;
  bool ResetPT(const std::string& file_name) OVERRIDE;

  void GetServiceUrls(const std::string& service_type,
                      EndpointUrls& end_points) OVERRIDE;

  std::string GetLockScreenIconUrl() const OVERRIDE;
  bool RequestPTUpdate() OVERRIDE;
  void CheckPermissions(const PTString& app_id,
                        const PTString& hmi_level,
                        const PTString& rpc,
                        const RPCParams& rpc_params,
                        CheckPermissionResult& result) OVERRIDE;
  bool ResetUserConsent() OVERRIDE;
  void KmsChanged(int kilometers) OVERRIDE;
  void IncrementIgnitionCycles() OVERRIDE;
  std::string ForcePTExchange() OVERRIDE;
  std::string GetPolicyTableStatus() const OVERRIDE;
  void ResetRetrySequence() OVERRIDE;
  uint32_t NextRetryTimeout() OVERRIDE;
  int TimeoutExchange() OVERRIDE;
  const std::vector<int> RetrySequenceDelaysSeconds() OVERRIDE;
  void OnExceededTimeout() OVERRIDE;
  void OnUpdateStarted() OVERRIDE;
  void PTUpdatedAt(Counters counter, int value) OVERRIDE;

  /**
   * Refresh data about retry sequence from policy table
   */
  void RefreshRetrySequence();
  DeviceConsent GetUserConsentForDevice(
      const std::string& device_id) const OVERRIDE;
  void GetUserConsentForApp(
      const std::string& device_id,
      const std::string& policy_app_id,
      std::vector<FunctionalGroupPermission>& permissions) OVERRIDE;
  void SetUserConsentForDevice(const std::string& device_id,
                               bool is_allowed) OVERRIDE;
  bool ReactOnUserDevConsentForApp(const std::string app_id,
                                   bool is_device_allowed) OVERRIDE;
  bool GetInitialAppData(const std::string& application_id,
                         StringArray* nicknames = NULL,
                         StringArray* app_hmi_types = NULL) OVERRIDE;

  void AddDevice(const std::string& device_id,
                 const std::string& connection_type) OVERRIDE;

  void SetDeviceInfo(const std::string& device_id,
                     const DeviceInfo& device_info) OVERRIDE;

  void SetUserConsentForApp(const PermissionConsent& permissions) OVERRIDE;

  bool GetDefaultHmi(const std::string& policy_app_id,
                     std::string* default_hmi) const OVERRIDE;

  bool GetPriority(const std::string& policy_app_id,
                   std::string* priority) const OVERRIDE;

  std::vector<UserFriendlyMessage> GetUserFriendlyMessages(
      const std::vector<std::string>& message_code,
      const std::string& language) OVERRIDE;

  bool IsApplicationRevoked(const std::string& app_id) const OVERRIDE;

  void GetPermissionsForApp(
      const std::string& device_id,
      const std::string& policy_app_id,
      std::vector<FunctionalGroupPermission>& permissions) OVERRIDE;

  std::string& GetCurrentDeviceId(
      const std::string& policy_app_id) const OVERRIDE;

  void SetSystemLanguage(const std::string& language) OVERRIDE;

  void SetSystemInfo(const std::string& ccpu_version,
                     const std::string& wers_country_code,
                     const std::string& language) OVERRIDE;
  void OnSystemReady() OVERRIDE;

  uint32_t GetNotificationsNumber(const std::string& priority) const OVERRIDE;

  void SetVINValue(const std::string& value) OVERRIDE;

  // Interface StatisticsManager (begin)
  void Increment(usage_statistics::GlobalCounterId type) OVERRIDE;
  void Increment(const std::string& app_id,
                 usage_statistics::AppCounterId type) OVERRIDE;
  void Set(const std::string& app_id,
           usage_statistics::AppInfoId type,
           const std::string& value) OVERRIDE;
  void Add(const std::string& app_id,
           usage_statistics::AppStopwatchId type,
           int32_t timespan_seconds) OVERRIDE;
  // Interface StatisticsManager (end)

  AppPermissions GetAppPermissionsChanges(
      const std::string& policy_app_id) OVERRIDE;
  void RemovePendingPermissionChanges(const std::string& app_id) OVERRIDE;

  void SendNotificationOnPermissionsUpdated(
      const std::string& application_id) OVERRIDE;

  bool CleanupUnpairedDevices() OVERRIDE;

  bool CanAppKeepContext(const std::string& app_id) const OVERRIDE;
  bool CanAppStealFocus(const std::string& app_id) const OVERRIDE;
  void MarkUnpairedDevice(const std::string& device_id) OVERRIDE;

  void AddApplication(const std::string& application_id) OVERRIDE;

  void RemoveAppConsentForGroup(const std::string& app_id,
                                const std::string& group_name);

  uint32_t HeartBeatTimeout(const std::string& app_id) const OVERRIDE;

  void SaveUpdateStatusRequired(bool is_update_needed) OVERRIDE;

  bool IsPredataPolicy(const std::string& policy_app_id) OVERRIDE;
  void set_cache_manager(CacheManagerInterface* cache_manager);

  void OnAppsSearchStarted() OVERRIDE;

  void OnAppsSearchCompleted() OVERRIDE;

#ifdef BUILD_TESTS
  inline CacheManagerInterfaceSPtr GetCache() {
    return cache_;
  }
#endif  // BUILD_TESTS
  const std::vector<std::string> GetAppRequestTypes(
      const std::string policy_app_id) const OVERRIDE;

  const VehicleInfo GetVehicleInfo() const OVERRIDE;

  void OnAppRegisteredOnMobile(const std::string& application_id) OVERRIDE;

  std::string RetrieveCertificate() const OVERRIDE;

 protected:
#ifdef USE_HMI_PTU_DECRYPTION
  utils::SharedPtr<policy_table::Table> Parse(const BinaryMessage& pt_content);
#else
  utils::SharedPtr<policy_table::Table> ParseArray(
      const BinaryMessage& pt_content);
#endif

  const PolicySettings& get_settings() const OVERRIDE;

 private:
  void CheckTriggers();
  /*
   * @brief Checks policy table update along with current data for any changes
   * in assigned functional group list of application
   *
   * @param Policy table update struct
   */
  void CheckPermissionsChanges(
      const utils::SharedPtr<policy_table::Table> update,
      const utils::SharedPtr<policy_table::Table> snapshot);

  /**
   * @brief Fill structure to be sent with OnPermissionsChanged notification
   *
   * @param Policy table struct, which contains rpc functional groups data
   * @param List of rpc functional group names, which should be checked
   * @param group_permission User permissions for functional groups
   * @param Notification struct to be filled and sent
   */
  void PrepareNotificationData(
      const policy_table::FunctionalGroupings& groups,
      const policy_table::Strings& group_names,
      const std::vector<FunctionalGroupPermission>& group_permission,
      Permissions& notification_data);

  /**
   * @brief Validate PermissionConsent structure according to currently
   * assigned groups
   * @param permissions PermissionConsent structure that should be validated.
   * @return PermissonConsent struct, which contains no foreign groups
   */
  PermissionConsent EnsureCorrectPermissionConsent(
      const PermissionConsent& permissions_to_check);

  /**
   * @brief Allows to process case when added application is not present in
   * policy db.
   * @param policy application id.
   * @param cuuren consent for application's device.
   */
  void AddNewApplication(const std::string& application_id,
                         DeviceConsent device_consent);

  /**
   * @brief Allows to process case when added application is already
   * in policy db.
   * @param policy application id.
   * @param cuuren consent for application's device.
   */
  void PromoteExistedApplication(const std::string& application_id,
                                 DeviceConsent device_consent);

  /**
   * @brief Check if certain application already in policy db.
   * @param policy application id.
   * @return true if application presents false otherwise.
   */
  bool IsNewApplication(const std::string& application_id) const;

  /**
   * Checks existing and permissions of AppStorageFolder
   * @return true if AppStorageFolder exists and has permissions read/write
   */
  bool CheckAppStorageFolder() const;

  /**
   * @brief Checks whether need ask the permission of users
   * @return true if user consent is needed
   */
  bool IsConsentNeeded(const std::string& app_id);

  /**
   * @brief Changes isConsentNeeded for app pending permissions, in case
   * user set permissions before app activation.
   * @param Unique app id
   * @param Current permissions for app
   */
  void CheckPendingPermissionsChanges(
      const std::string& policy_app_id,
      const std::vector<FunctionalGroupPermission>& current_permissions);

  void StartPTExchange() OVERRIDE;
  bool ExceededDays() OVERRIDE;
  bool ExceededIgnitionCycles() OVERRIDE;
  bool IsPTValid(utils::SharedPtr<policy_table::Table> policy_table,
                 policy_table::PolicyTableType type) const;

  void RetrySequence();

 private:
  PolicyListener* listener_;

  UpdateStatusManager update_status_manager_;
  CacheManagerInterfaceSPtr cache_;
  sync_primitives::Lock apps_registration_lock_;
  sync_primitives::Lock app_permissions_diff_lock_;
  std::map<std::string, AppPermissions> app_permissions_diff_;

  /**
   * Timeout to wait response with UpdatePT
   */
  uint32_t retry_sequence_timeout_;

  /**
   * Seconds between retries to update PT
   */
  std::vector<int> retry_sequence_seconds_;

  /**
   * Current index trying of retry sequence
   */
  uint32_t retry_sequence_index_;

  /**
   * Lock for guarding retry sequence
   */
  sync_primitives::Lock retry_sequence_lock_;

  /**
    * Timer to retry UpdatePT
    */
  timer::Timer timer_retry_sequence_;

  /**
   * @brief Device id, which is used during PTU handling for specific
   * application
   */
  mutable std::string last_device_id_;

  bool ignition_check;

  const PolicySettings* settings_;
  friend struct CheckAppPolicy;
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_MANAGER_IMPL_H_
