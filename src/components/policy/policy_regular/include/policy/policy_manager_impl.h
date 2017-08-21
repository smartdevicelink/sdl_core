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

#ifndef SRC_COMPONENTS_POLICY_POLICY_REGULAR_INCLUDE_POLICY_POLICY_MANAGER_IMPL_H_
#define SRC_COMPONENTS_POLICY_POLICY_REGULAR_INCLUDE_POLICY_POLICY_MANAGER_IMPL_H_

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
#ifdef SDL_REMOTE_CONTROL
#include "policy/access_remote.h"
#include "policy/access_remote_impl.h"
#endif  // SDL_REMOTE_CONTROL

namespace policy_table = rpc::policy_table_interface_base;

namespace policy {
struct CheckAppPolicy;

class PolicyManagerImpl : public PolicyManager {
 public:
  PolicyManagerImpl();
  virtual void set_listener(PolicyListener* listener);
  PolicyListener* listener() const {
    return listener_;
  }
  virtual bool InitPT(const std::string& file_name,
                      const PolicySettings* settings);
  virtual bool LoadPT(const std::string& file, const BinaryMessage& pt_content);
  virtual bool ResetPT(const std::string& file_name);

  virtual void GetUpdateUrls(const uint32_t service_type,
                             EndpointUrls& out_end_points);
  virtual void GetUpdateUrls(const std::string& service_type,
                             EndpointUrls& out_end_points);

  virtual std::string GetLockScreenIconUrl() const;
  virtual bool RequestPTUpdate();
  virtual void CheckPermissions(const PTString& device_id,
                                const PTString& app_id,
                                const PTString& hmi_level,
                                const PTString& rpc,
                                const RPCParams& rpc_params,
                                CheckPermissionResult& result) OVERRIDE;
  virtual bool ResetUserConsent();
  virtual void KmsChanged(int kilometers);
  virtual void IncrementIgnitionCycles();
  virtual std::string ForcePTExchange();
  virtual std::string ForcePTExchangeAtUserRequest();
  virtual std::string GetPolicyTableStatus() const;
  virtual void ResetRetrySequence();
  virtual uint32_t NextRetryTimeout();
  virtual uint32_t TimeoutExchangeMSec();
  virtual const std::vector<int> RetrySequenceDelaysSeconds();
  virtual void OnExceededTimeout();
  virtual void OnUpdateStarted();
  virtual void PTUpdatedAt(Counters counter, int value);

  /**
   * Refresh data about retry sequence from policy table
   */
  virtual void RefreshRetrySequence();
  virtual DeviceConsent GetUserConsentForDevice(
      const std::string& device_id) const OVERRIDE;
  virtual void GetUserConsentForApp(
      const std::string& device_id,
      const std::string& policy_app_id,
      std::vector<FunctionalGroupPermission>& permissions);
  virtual void SetUserConsentForDevice(const std::string& device_id,
                                       bool is_allowed);
  virtual bool ReactOnUserDevConsentForApp(const std::string app_id,
                                           bool is_device_allowed);
  virtual bool GetInitialAppData(const std::string& application_id,
                                 StringArray* nicknames = NULL,
                                 StringArray* app_hmi_types = NULL);

  virtual void AddDevice(const std::string& device_id,
                         const std::string& connection_type);

  virtual void SetDeviceInfo(const std::string& device_id,
                             const DeviceInfo& device_info);

  virtual void SetUserConsentForApp(const PermissionConsent& permissions);

  virtual bool GetDefaultHmi(const std::string& policy_app_id,
                             std::string* default_hmi) const;

  virtual bool GetPriority(const std::string& policy_app_id,
                           std::string* priority) const;

  virtual std::vector<UserFriendlyMessage> GetUserFriendlyMessages(
      const std::vector<std::string>& message_code,
      const std::string& language);

  virtual bool IsApplicationRevoked(const std::string& app_id) const;

  virtual void GetPermissionsForApp(
      const std::string& device_id,
      const std::string& policy_app_id,
      std::vector<FunctionalGroupPermission>& permissions);

  virtual std::string& GetCurrentDeviceId(
      const std::string& policy_app_id) const;

  virtual void SetSystemLanguage(const std::string& language);

  virtual void SetSystemInfo(const std::string& ccpu_version,
                             const std::string& wers_country_code,
                             const std::string& language);
  virtual void OnSystemReady();

  virtual uint32_t GetNotificationsNumber(
      const std::string& priority) const OVERRIDE;

  virtual void SetVINValue(const std::string& value);

  // Interface StatisticsManager (begin)
  virtual void Increment(usage_statistics::GlobalCounterId type);
  virtual void Increment(const std::string& app_id,
                         usage_statistics::AppCounterId type);
  virtual void Set(const std::string& app_id,
                   usage_statistics::AppInfoId type,
                   const std::string& value);
  virtual void Add(const std::string& app_id,
                   usage_statistics::AppStopwatchId type,
                   int32_t timespan_seconds);
  // Interface StatisticsManager (end)

  AppPermissions GetAppPermissionsChanges(const std::string& policy_app_id);
  void RemovePendingPermissionChanges(const std::string& app_id);

  void SendNotificationOnPermissionsUpdated(const std::string& application_id);

  bool CleanupUnpairedDevices();

  bool CanAppKeepContext(const std::string& app_id) const;
  bool CanAppStealFocus(const std::string& app_id) const;
  void MarkUnpairedDevice(const std::string& device_id);

  StatusNotifier AddApplication(
      const std::string& application_id,
      const rpc::policy_table_interface_base::AppHmiTypes& hmi_types);
#ifdef SDL_REMOTE_CONTROL
  void SetDefaultHmiTypes(const std::string& application_id,
                          const std::vector<int>& hmi_types);
  /**
   * Gets HMI types
   * @param application_id ID application
   * @param app_types list to save HMI types
   * @return true if policy has specific policy for this application
   */
  virtual bool GetHMITypes(const std::string& application_id,
                           std::vector<int>* app_types) OVERRIDE;
  virtual void set_access_remote(utils::SharedPtr<AccessRemote> access_remote);
  TypeAccess CheckDriverConsent(const Subject& who,
                                const Object& what,
                                const std::string& rpc,
                                const RemoteControlParams& params);
  void CheckPTUUpdatesChange(
      const utils::SharedPtr<policy_table::Table> pt_update,
      const utils::SharedPtr<policy_table::Table> snapshot);

  void CheckRemoteGroupsChange(
      const utils::SharedPtr<policy_table::Table> pt_update,
      const utils::SharedPtr<policy_table::Table> snapshot);

  void SendHMILevelChanged(const Subject& who);

  void OnPrimaryGroupsChanged(const std::string& application_id);
#endif  // SDL_REMOTE_CONTROL

  virtual void RemoveAppConsentForGroup(const std::string& app_id,
                                        const std::string& group_name);

  virtual uint32_t HeartBeatTimeout(const std::string& app_id) const;

  virtual void SaveUpdateStatusRequired(bool is_update_needed);

  virtual bool IsPredataPolicy(const std::string& policy_app_id) const OVERRIDE;
  void set_cache_manager(CacheManagerInterface* cache_manager);

  virtual void OnAppsSearchStarted();

  virtual void OnAppsSearchCompleted(const bool trigger_ptu);

#ifdef BUILD_TESTS
  inline CacheManagerInterfaceSPtr GetCache() {
    return cache_;
  }

  inline void SetSendOnUpdateSentOut(const bool send_on_update_sent_out) {
    send_on_update_sent_out_ = send_on_update_sent_out;
  }
#endif  // BUILD_TESTS
  virtual const std::vector<std::string> GetAppRequestTypes(
      const std::string policy_app_id) const;

  virtual const VehicleInfo GetVehicleInfo() const;

  virtual void OnAppRegisteredOnMobile(
      const std::string& application_id) OVERRIDE;

  virtual std::string RetrieveCertificate() const OVERRIDE;

  virtual bool HasCertificate() const OVERRIDE;

  AppIdURL GetNextUpdateUrl(const EndpointUrls& urls) OVERRIDE;

  AppIdURL RetrySequenceUrl(const struct RetrySequenceURL& rs,
                            const EndpointUrls& urls) const OVERRIDE;

 protected:
#ifdef USE_HMI_PTU_DECRYPTION
  virtual utils::SharedPtr<policy_table::Table> Parse(
      const BinaryMessage& pt_content);
#else
  virtual utils::SharedPtr<policy_table::Table> ParseArray(
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
  virtual bool IsConsentNeeded(const std::string& app_id);

  /**
   * @brief Changes isConsentNeeded for app pending permissions, in case
   * user set permissions before app activation.
   * @param Unique app id
   * @param Current permissions for app
   */
  void CheckPendingPermissionsChanges(
      const std::string& policy_app_id,
      const std::vector<FunctionalGroupPermission>& current_permissions);

  virtual void StartPTExchange();
  virtual bool ExceededDays();
  virtual bool ExceededIgnitionCycles();
  bool IsPTValid(utils::SharedPtr<policy_table::Table> policy_table,
                 policy_table::PolicyTableType type) const;

  void RetrySequence();

 private:
#ifdef SDL_REMOTE_CONTROL
  void GetPermissions(const std::string device_id,
                      const std::string application_id,
                      Permissions* data);
  virtual TypeAccess CheckAccess(const PTString& device_id,
                                 const PTString& app_id,
                                 const PTString& module);
  virtual bool CheckModule(const PTString& app_id, const PTString& module);
  virtual void SetAccess(const PTString& dev_id,
                         const PTString& app_id,
                         const PTString& module,
                         bool allowed);
  virtual void SendAppPermissionsChanged(const std::string& device_id,
                                         const std::string& application_id);
  virtual bool GetModuleTypes(const std::string& policy_app_id,
                              std::vector<std::string>* modules) const;
#endif  // SDL_REMOTE_CONTROL
  PolicyListener* listener_;

  UpdateStatusManager update_status_manager_;
  CacheManagerInterfaceSPtr cache_;
#ifdef SDL_REMOTE_CONTROL
  utils::SharedPtr<AccessRemote> access_remote_;
#endif
  sync_primitives::Lock apps_registration_lock_;
  sync_primitives::Lock app_permissions_diff_lock_;
  std::map<std::string, AppPermissions> app_permissions_diff_;

  /**
   * Timeout to wait response with UpdatePT (msec)
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
  friend struct ProccessAppGroups;

  /**
   * @brief Pair of app index and url index from Endpoints vector
   * that contains all application URLs
   */
  RetrySequenceURL retry_sequence_url_;

  bool wrong_ptu_update_received_;
  bool send_on_update_sent_out_;
  bool trigger_ptu_;
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_POLICY_REGULAR_INCLUDE_POLICY_POLICY_MANAGER_IMPL_H_
