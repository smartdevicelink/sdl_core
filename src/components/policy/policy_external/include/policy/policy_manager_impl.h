/*
 Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_POLICY_MANAGER_IMPL_H_
#define SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_POLICY_MANAGER_IMPL_H_

#include <list>
#include "utils/shared_ptr.h"
#include "utils/lock.h"
#include "policy/policy_manager.h"
#include "policy/policy_table.h"
#include "policy/cache_manager_interface.h"
#include "policy/update_status_manager.h"
#include "policy/policy_table/functions.h"
#include "policy/usage_statistics/statistics_manager.h"

namespace policy_table = rpc::policy_table_interface_base;

namespace policy {
struct CheckAppPolicy;

class PolicyManagerImpl : public PolicyManager {
 public:
  PolicyManagerImpl();
  explicit PolicyManagerImpl(bool in_memory);
  virtual void set_listener(PolicyListener* listener);
  PolicyListener* listener() const {
    return listener_;
  }
  virtual bool InitPT(const std::string& file_name,
                      const PolicySettings* settings);
  virtual bool LoadPT(const std::string& file, const BinaryMessage& pt_content);
  virtual bool ResetPT(const std::string& file_name);

  virtual std::string GetUpdateUrl(int service_type);
  virtual void GetUpdateUrls(const uint32_t service_type,
                             EndpointUrls& out_end_points);
  virtual void GetUpdateUrls(const std::string& service_type,
                             EndpointUrls& out_end_points);
  virtual void RequestPTUpdate();
  virtual void CheckPermissions(const PTString& app_id,
                                const PTString& hmi_level,
                                const PTString& rpc,
                                const RPCParams& rpc_params,
                                CheckPermissionResult& result);
  virtual bool ResetUserConsent();
  virtual void KmsChanged(int kilometers);
  virtual void IncrementIgnitionCycles();
  virtual std::string ForcePTExchange();
  virtual std::string ForcePTExchangeAtUserRequest();
  virtual std::string GetPolicyTableStatus() const;
  virtual void ResetRetrySequence();
  virtual int NextRetryTimeout();
  virtual uint32_t TimeoutExchangeMSec();
  virtual const std::vector<int> RetrySequenceDelaysSeconds();
  virtual void OnExceededTimeout();
  virtual std::string GetLockScreenIconUrl() const OVERRIDE;
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
                                       const bool is_allowed);
  virtual bool ReactOnUserDevConsentForApp(const std::string& app_id,
                                           const bool is_device_allowed);
  virtual bool GetInitialAppData(const std::string& application_id,
                                 StringArray* nicknames = NULL,
                                 StringArray* app_hmi_types = NULL);

  virtual void AddDevice(const std::string& device_id,
                         const std::string& connection_type);

  virtual void SetDeviceInfo(const std::string& device_id,
                             const DeviceInfo& device_info);

  void SetUserConsentForApp(const PermissionConsent& permissions,
                            const NotificationMode mode) OVERRIDE;

  virtual bool GetDefaultHmi(const std::string& policy_app_id,
                             std::string* default_hmi) const;

  virtual bool GetPriority(const std::string& policy_app_id,
                           std::string* priority) const;

  virtual std::vector<UserFriendlyMessage> GetUserFriendlyMessages(
      const std::vector<std::string>& message_code,
      const std::string& language,
      const std::string& active_hmi_language);

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
  inline void SetSendOnUpdateFlags(const bool send_on_update_sent_out,
                                   const bool wrong_ptu_update_received) {
    send_on_update_sent_out_ = send_on_update_sent_out;
    wrong_ptu_update_received_ = wrong_ptu_update_received;
  }
#endif  // BUILD_TESTS

  virtual const std::vector<std::string> GetAppRequestTypes(
      const std::string policy_app_id) const;

  virtual const VehicleInfo GetVehicleInfo() const;

  virtual void OnAppRegisteredOnMobile(const std::string& application_id);

  virtual const MetaInfo GetMetaInfo() const OVERRIDE;

  virtual std::string RetrieveCertificate() const OVERRIDE;

  virtual bool HasCertificate() const OVERRIDE;

  virtual void SetDecryptedCertificate(const std::string& certificate) OVERRIDE;

  const PolicySettings& get_settings() const OVERRIDE;

  AppIdURL GetNextUpdateUrl(const EndpointUrls& urls) OVERRIDE;

  AppIdURL RetrySequenceUrl(const struct RetrySequenceURL& rs,
                            const EndpointUrls& urls) const OVERRIDE;

  /**
   * @brief  Checks, if SDL needs to update it's policy table section
             "external_consent_status"
   * @param  ExternalConsent status
   * @return true if such check is needed, false - if not.
   */
  bool IsNeedToUpdateExternalConsentStatus(
      const ExternalConsentStatus& status) const;

  /**
   * @brief Gets customer connectivity settings status
   * @return ExternalConsent status
   */
  bool SetExternalConsentStatus(const ExternalConsentStatus& status) OVERRIDE;
  ExternalConsentStatus GetExternalConsentStatus() OVERRIDE;

 protected:
  virtual utils::SharedPtr<policy_table::Table> Parse(
      const BinaryMessage& pt_content);

 private:
  void CheckTriggers();

  /**
   * @brief Compares current applications policies to the updated one and
   * returns apporopriate result codes per application, which that are being
   * processed by sending notification to applications registered and to the
   * system
   * @param update Shared pointer to policy table udpate
   * @param snapshot Shared pointer to current copy of policy table
   * @return Collection per-application results
   */
  CheckAppPolicyResults CheckPermissionsChanges(
      const utils::SharedPtr<policy_table::Table> update,
      const utils::SharedPtr<policy_table::Table> snapshot);

  /**
   * @brief Processes results from policy table update analysis done by
   * CheckPermissionsChanges() by sending OnPermissionChange and
   * OnAppPermissionChanged notifications
   * @param results Collection of per-application results
   * @param app_policies Reference to updated application policies section as
   * a data source for generating notifications data
   */
  void ProcessAppPolicyCheckResults(
      const CheckAppPolicyResults& results,
      const policy_table::ApplicationPolicies& app_policies);

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

  /**
   * @brief Notify application about its permissions changes by preparing and
   * sending OnPermissionsChanged notification
   * @param policy_app_id Application id to send notification to
   * @param app_group_permissons Current permissions for groups assigned to
   * application
   */
  void NotifyPermissionsChanges(
      const std::string& policy_app_id,
      const std::vector<FunctionalGroupPermission>& app_group_permissions);

  /**
   * @brief Processes updated ExternalConsent status received via
   * OnAppPermissionConsent
   * notification by updating user consents and ExternalConsent consents for
   * registered and
   * known before by policy table (must have any user consent records)
   * @param groups_by_status Collection of ExternalConsent entities with their
   * statuses
   * @param processing_policy Defines whether consents timestamps must be
   * considered or external consents take over
   */
  void ProcessExternalConsentStatusUpdate(
      const GroupsByExternalConsentStatus& groups_by_status,
      const ConsentProcessingPolicy processing_policy);

  /**
   * @brief Processes ExternalConsent status for application registered
   * afterward, so its
   * user consents (if any) and ExternalConsent consents (if any) will be
   * updated
   * appropiately to current ExternalConsent status stored by policy table
   * @param application_id Application id
   * @param processing_policy Defines whether consents timestamps must be
   * considered or external consents take over
   */
  void ProcessExternalConsentStatusForApp(
      const std::string& application_id,
      const ConsentProcessingPolicy processing_policy);
  /**
   * @brief Directly updates user consent and ExternalConsent consents (if any)
   * for
   * application if it has assigned any of group from allowed or disallowed
   * lists
   * @param device_id Device id which is linked to application id
   * @param application_id Application id
   * @param allowed_groups List of group names allowed by current
   * ExternalConsent status
   * @param disallowed_groups List of group names disallwed by current
   * ExternalConsent status
   * @param processing_policy Defines whether consents timestamps have to be
   * considered or external consents take over
   */
  void UpdateAppConsentWithExternalConsent(
      const std::string& device_id,
      const std::string& application_id,
      const GroupsNames& allowed_groups,
      const GroupsNames& disallowed_groups,
      const ConsentProcessingPolicy processing_policy);

  typedef policy_table::ApplicationPolicies::value_type AppPoliciesValueType;

  /**
   * @brief Notifies system by sending OnAppPermissionChanged notification
   * @param app_policy Reference to application policy
   */
  void NotifySystem(const AppPoliciesValueType& app_policy) const;

  /**
   * @brief Sends OnPermissionChange notification to application if its
   * currently registered
   * @param app_policy Reference to application policy
   */
  void SendPermissionsToApp(const AppPoliciesValueType& app_policy);

  /**
   * @brief Gets groups names from collection of groups permissions
   * @param app_group_permissions Collection of groups permissions
   * @return Collection of group names
   */
  policy_table::Strings GetGroupsNames(
      const std::vector<FunctionalGroupPermission>& app_group_permissions)
      const;

  /**
   * @brief Calculates consents for groups based on mapped ExternalConsent
   * entities statuses
   * and groups containers where entities have been found
   * @param groups_by_external_consent ExternalConsent entities mapped to
   * functional groups names and
   * their containters where this entity has been found
   * @param out_allowed_groups List of groups allowed by ExternalConsent status
   * @param out_disallowed_groups List of groups disallowed by ExternalConsent
   * status
   */
  void CalculateGroupsConsentFromExternalConsent(
      const GroupsByExternalConsentStatus& groups_by_external_consent,
      GroupsNames& out_allowed_groups,
      GroupsNames& out_disallowed_groups) const;

  PolicyListener* listener_;

  UpdateStatusManager update_status_manager_;
  CacheManagerInterfaceSPtr cache_;
  sync_primitives::Lock apps_registration_lock_;
  sync_primitives::Lock app_permissions_diff_lock_;

  /**
   * @brief Collection of parameters to be reported to the system with
   * SDL.ActivateApp response or OnAppPermissionsChanged notification
   * Being set during policy table update processing
   */
  typedef std::map<std::string, AppPermissions> PendingPermissions;
  PendingPermissions app_permissions_diff_;

  /**
   * Timeout to wait response with UpdatePT
   */
  int retry_sequence_timeout_;

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
   * @brief Device id, which is used during PTU handling for specific
   * application
   */
  mutable std::string last_device_id_;

  bool ignition_check;

  const PolicySettings* settings_;
  friend struct CheckAppPolicy;

  /**
   * @brief Pair of app index and url index from Endpoints vector
   * that contains all application URLs
   */
  RetrySequenceURL retry_sequence_url_;
  friend struct ProccessAppGroups;

  bool wrong_ptu_update_received_;
  bool send_on_update_sent_out_;
  bool trigger_ptu_;
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_POLICY_MANAGER_IMPL_H_
