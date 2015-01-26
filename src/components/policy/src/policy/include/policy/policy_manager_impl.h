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

#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_MANAGER_IMPL_H_
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_MANAGER_IMPL_H_

#include <list>
#include "utils/shared_ptr.h"
#include "utils/lock.h"
#include "policy/policy_manager.h"
#include "policy/policy_table.h"
#include "policy/cache_manager_interface.h"
#include "policy/update_status_manager_interface.h"
#include "./functions.h"
#include "usage_statistics/statistics_manager.h"

namespace policy_table = rpc::policy_table_interface_base;

namespace policy {
struct CheckAppPolicy;

class PolicyManagerImpl : public PolicyManager {
  public:
    PolicyManagerImpl();
    virtual ~PolicyManagerImpl();
    virtual void set_listener(PolicyListener* listener);
    PolicyListener* listener() const {
      return listener_;
    }
    virtual bool InitPT(const std::string& file_name);
    virtual bool LoadPT(const std::string& file, const BinaryMessage& pt_content);
    virtual bool ResetPT(const std::string& file_name);
    virtual std::string GetUpdateUrl(int service_type);
    virtual EndpointUrls GetUpdateUrls(int service_type);
    virtual BinaryMessageSptr RequestPTUpdate();
    virtual void CheckPermissions(const PTString& app_id,
        const PTString& hmi_level,
        const PTString& rpc,
        const RPCParams& rpc_params,
        CheckPermissionResult& result);
    virtual bool ResetUserConsent();
    virtual bool ExceededIgnitionCycles();
    virtual bool ExceededDays(int days);
    virtual bool ExceededKilometers(int kilometers);
    virtual void IncrementIgnitionCycles();
    virtual PolicyTableStatus GetPolicyTableStatus();
    virtual void ResetRetrySequence();
    virtual int NextRetryTimeout();
    virtual int TimeoutExchange();
    virtual const std::vector<int> RetrySequenceDelaysSeconds();
    virtual void OnExceededTimeout();
    virtual void OnUpdateStarted();
    virtual void PTUpdatedAt(int kilometers, int days_after_epoch);

    /**
     * Refresh data about retry sequence from policy table
     */
    virtual void RefreshRetrySequence();
    virtual DeviceConsent GetUserConsentForDevice(const std::string& device_id);
    virtual void GetUserConsentForApp(
      const std::string& device_id, const std::string& policy_app_id,
      std::vector<FunctionalGroupPermission>& permissions);
    virtual void SetUserConsentForDevice(const std::string& device_id,
                                         bool is_allowed);
    virtual bool ReactOnUserDevConsentForApp(const std::string app_id,
        bool is_device_allowed);
    virtual bool GetInitialAppData(const std::string& application_id,
                                   StringArray* nicknames = NULL,
                                   StringArray* app_hmi_types = NULL);

    virtual void SetDeviceInfo(const std::string& device_id,
                               const DeviceInfo& device_info);

    virtual void SetUserConsentForApp(const PermissionConsent& permissions);

    virtual bool GetDefaultHmi(const std::string& policy_app_id,
                               std::string* default_hmi);

    virtual bool GetPriority(const std::string& policy_app_id,
                             std::string* priority);

    virtual std::vector<UserFriendlyMessage> GetUserFriendlyMessages(
      const std::vector<std::string>& message_code, const std::string& language);

    virtual bool IsApplicationRevoked(const std::string& app_id) const;

    virtual void GetPermissionsForApp(
      const std::string& device_id, const std::string& policy_app_id,
      std::vector<FunctionalGroupPermission>& permissions);

    virtual std::string& GetCurrentDeviceId(const std::string& policy_app_id);

    virtual void SetSystemLanguage(const std::string& language);

    virtual void SetSystemInfo(const std::string& ccpu_version,
                               const std::string& wers_country_code,
                               const std::string& language);
    virtual void OnSystemReady();

    virtual uint32_t GetNotificationsNumber(const std::string& priority);

    virtual int IsConsentNeeded(const std::string& app_id);

    virtual void SetVINValue(const std::string& value);

    // Interface StatisticsManager (begin)
    virtual void Increment(usage_statistics::GlobalCounterId type);
    virtual void Increment(const std::string& app_id,
                           usage_statistics::AppCounterId type);
    virtual void Set(const std::string& app_id, usage_statistics::AppInfoId type,
                     const std::string& value);
    virtual void Add(const std::string& app_id,
                     usage_statistics::AppStopwatchId type,
                     int32_t timespan_seconds);
    // Interface StatisticsManager (end)

    AppPermissions GetAppPermissionsChanges(const std::string& device_id,
                                            const std::string& policy_app_id);
    void RemovePendingPermissionChanges(const std::string& app_id);

    void SendNotificationOnPermissionsUpdated(const std::string& application_id);

    bool CleanupUnpairedDevices();

    bool CanAppKeepContext(const std::string& app_id);
    bool CanAppStealFocus(const std::string& app_id);
    void MarkUnpairedDevice(const std::string& device_id);

    void AddApplication(const std::string& application_id);

    /**
     * @brief IsAppinUpdateList
     * @param app_id
     * @return
     */
    virtual bool IsAppInUpdateList(const std::string& app_id) const;

    virtual void RemoveAppConsentForGroup(const std::string& app_id,
                                          const std::string& group_name);

    virtual uint16_t HeartBeatTimeout(const std::string& app_id) const;

    virtual bool IsPredataPolicy(const std::string& policy_app_id);
    void set_cache_manager(CacheManagerInterface* cache_manager);
    void set_update_status_manager(
        UpdateStatusManagerInterface* update_manager);

  protected:
    virtual utils::SharedPtr<policy_table::Table> Parse(
        const BinaryMessage& pt_content);

  private:
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
     * @brief Add application id at the end of update permissions request list
     * @param Application id
     */
    void AddAppToUpdateList(const std::string& application_id);

    /**
     * @brief Remove first application in the update permissions request list
     */
    void RemoveAppFromUpdateList();

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

    PolicyListener* listener_;

    UpdateStatusManagerInterfaceSPtr update_status_manager_;
    CacheManagerInterfaceSPtr cache_;
    sync_primitives::Lock update_request_list_lock_;
    sync_primitives::Lock apps_registration_lock_;
    std::map<std::string, AppPermissions> app_permissions_diff_;

    /**
     * @brief List of application, which require update of permissions
     */
    std::list<std::string> update_requests_list_;

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
     * Lock for guarding recording statistics
     */
    sync_primitives::Lock statistics_lock_;

    /**
     * @brief Device id, which is used during PTU handling for specific
     * application
     */
    std::string last_device_id_;

    /**
     * @brief Holds device ids, which were unpaired
     */
    DeviceIds unpaired_device_ids_;

    friend struct CheckAppPolicy;
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_MANAGER_IMPL_H_
