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

#include "policy/access_remote.h"
#include "policy/access_remote_impl.h"
#include "policy/cache_manager_interface.h"
#include "policy/policy_manager.h"
#include "policy/policy_table.h"
#include "policy/policy_table/functions.h"
#include "policy/update_status_manager.h"
#include "policy/usage_statistics/statistics_manager.h"
#include "utils/lock.h"
#include "utils/timer.h"

namespace policy_table = rpc::policy_table_interface_base;

namespace policy {
struct CheckAppPolicy;

class PolicyManagerImpl : public PolicyManager {
 public:
  PolicyManagerImpl();
  explicit PolicyManagerImpl(bool in_memory);
  /*
   * \param policy_app_id policy app id
   * \return true if the app need encryption
   */
  bool AppNeedEncryption(const std::string& policy_app_id) const OVERRIDE;

  /*
   * \param policy_app_id policy app id
   * \return Optional app need encryption
   */
  const rpc::Optional<rpc::Boolean> GetAppEncryptionRequired(
      const std::string& policy_app_id) const OVERRIDE;

  /*
   * \param policy_app_id policy app id
   * \return groups that exist for app
   */
  const std::vector<std::string> GetFunctionalGroupsForApp(
      const std::string& policy_app_id) const OVERRIDE;

  const std::vector<std::string> GetApplicationPolicyIDs() const OVERRIDE;

  /*
   * \param policy_group group
   * \return true if the group need encryption
   */
  bool FunctionGroupNeedEncryption(
      const std::string& policy_group) const OVERRIDE;

  /*
   * \param policy_group group\
   * \return RPCs that exists in group
   */
  const std::vector<std::string> GetRPCsForFunctionGroup(
      const std::string& group) const OVERRIDE;
  /*
   * \param function_id function id
   * \return policy function name
   */
  const std::string GetPolicyFunctionName(
      const uint32_t function_id) const OVERRIDE;
  /**
   * @brief set_listener set new policy listener instance
   * @param listener new policy listener
   */
  void set_listener(PolicyListener* listener) OVERRIDE;

  /**
   * @brief listener get current policy listener instance
   * @return current policy listener
   */
  PolicyListener* listener() const {
    return listener_;
  }

  /**
   * @brief Inits Policy Table
   * @param file_name path to preloaded PT file
   * @param settings pointer to policy init settings
   * @return true if init is successful
   */
  bool InitPT(const std::string& file_name,
              const PolicySettings* settings) OVERRIDE;

  /**
   * @brief Updates Policy Table from binary message received from
   * mobile device. Saves to Policy Table diff between Policy Table
   * sent in snapshot and received Policy Table.
   * @param file name of file with update policy table
   * @param pt_content PTU as binary string
   * @return true if successfully
   */
  PtProcessingResult LoadPT(const std::string& file,
                            const BinaryMessage& pt_content) OVERRIDE;

  void OnPTUFinished(const PtProcessingResult ptu_result) OVERRIDE;

  /**
   * @brief Resets Policy Table
   * @param file_name Path to preloaded PT file
   * @return true if successfully
   */
  bool ResetPT(const std::string& file_name) OVERRIDE;

  /**
   * @brief Gets last URL for sending PTS to from PT itself
   * @param service_type Service specifies user of URL
   * @return last URL or empty string if endpoint entry is empty
   */
  std::string GetUpdateUrl(int service_type) OVERRIDE;

  /**
   * @brief Gets all URLs for sending PTS to from PT itself.
   * @param service_type Service specifies user of URL
   * @param out_end_points output vector of urls
   */
  void GetUpdateUrls(const uint32_t service_type,
                     EndpointUrls& out_end_points) const OVERRIDE;
  void GetUpdateUrls(const std::string& service_type,
                     EndpointUrls& out_end_points) const OVERRIDE;

  /**
   * @brief PTU is needed, for this PTS has to be formed and sent.
   */
  void RequestPTUpdate() OVERRIDE;

  /**
   * @brief Check if specified RPC for specified application
   * has permission to be executed in specified HMI Level
   * and also its permitted params.
   * @param device_id device identifier
   * @param app_id Id of application provided during registration
   * @param hmi_level Current HMI Level of application
   * @param rpc Name of RPC
   * @param rpc_params List of RPC params
   * @param result containing flag if HMI Level is allowed
   * and list of allowed params.
   */
  void CheckPermissions(const PTString& device_id,
                        const PTString& app_id,
                        const PTString& hmi_level,
                        const PTString& rpc,
                        const RPCParams& rpc_params,
                        CheckPermissionResult& result) OVERRIDE;

  /**
   * @brief Clear all record of user consents. Used during Factory Reset.
   * @return bool Success of operation
   */
  bool ResetUserConsent() OVERRIDE;

  /**
   * @brief Checks is PT exceeded kilometers
   * @param kilometers current kilometers at odometer
   * @return true if exceeded
   */
  void KmsChanged(int kilometers) OVERRIDE;

  const boost::optional<bool> LockScreenDismissalEnabledState() const OVERRIDE;

  const boost::optional<std::string> LockScreenDismissalWarningMessage(
      const std::string& language) const OVERRIDE;

  /**
   * @brief Increments counter of ignition cycles
   */
  void IncrementIgnitionCycles() OVERRIDE;

  /**
   * @brief Exchange by hmi or mobile request
   * @return Current status of policy table
   */
  std::string ForcePTExchange() OVERRIDE;

  void StopRetrySequence() OVERRIDE;

  /**
   * @brief Exchange by user request
   * @return Current status of policy table
   */
  std::string ForcePTExchangeAtUserRequest() OVERRIDE;

  /**
   * @brief Returns current status of policy table for HMI
   * @return Current status of policy table
   */
  std::string GetPolicyTableStatus() const OVERRIDE;

  /**
   * @brief Resets retry sequence
   * @param send_event - if true corresponding event is sent to
   * UpdateStatusManager
   */
  void ResetRetrySequence(const ResetRetryCountType reset_type) OVERRIDE;

  /**
   * @brief Gets timeout to wait before next retry updating PT
   * If timeout is less or equal to zero then the retry sequence is not need.
   * @return timeout in seconds
   */
  int NextRetryTimeout() OVERRIDE;

  /**
   * @brief Gets timeout to wait until receive response
   * @return timeout in seconds
   */
  uint32_t TimeoutExchangeMSec() OVERRIDE;

  /**
   * @brief List of timeouts in seconds between retries
   *        when attempt to update PT fails
   * @return List of delays between attempts.
   */
  const std::vector<int> RetrySequenceDelaysSeconds() OVERRIDE;

  /**
   * @brief Handler of exceeding timeout of exchanging policy table
   */
  void OnExceededTimeout() OVERRIDE;

  /**
   * @brief Get Icon Url used for showing a cloud apps icon before the intial
   *registration
   *
   * @return url which point to the resourse where icon could be
   *obtained.
   */
  std::string GetIconUrl(const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief Handler of PTS sending out
   */
  void OnUpdateStarted() OVERRIDE;

  /**
   * @brief Sets counter value that passed for receiving PT UPdate.
   */
  void PTUpdatedAt(Counters counter, int value) OVERRIDE;

  /**
   * @brief Refresh data about retry sequence from policy table
   */
  virtual void RefreshRetrySequence();

  /**
   * @brief Gets user consent for mobile device data connection
   * @param device_id Unique device identifier
   * @return status of device consent
   */
  DeviceConsent GetUserConsentForDevice(
      const std::string& device_id) const OVERRIDE;

  /**
   * @brief Gets user consent for application
   * @param device_id Device id
   * @param policy_app_id Unique application id
   * @param permissions Array of functional groups permissions
   */
  void GetUserConsentForApp(
      const std::string& device_id,
      const std::string& policy_app_id,
      std::vector<FunctionalGroupPermission>& permissions) OVERRIDE;

  /**
   * @brief Set user consent for mobile device data connection
   * @param device_id Unique device identifier
   * @param is_allowed User consent for usage device data connection
   */
  void SetUserConsentForDevice(const std::string& device_id,
                               const bool is_allowed) OVERRIDE;

  /**
   * @brief Update Application Policies as reaction
   * on User allowing/disallowing device this app is running on.
   * @param device_handle device identifier
   * @param app_id Unique application id
   * @param is_device_allowed true if user allowing device otherwise false
   * @return true if operation was successful
   */
  bool ReactOnUserDevConsentForApp(
      const transport_manager::DeviceHandle& device_handle,
      const std::string& app_id,
      const bool is_device_allowed) OVERRIDE;

  /**
   * @brief Retrieves data from app_policies about app on its registration:
   * @param application_id - id of registered app
   * @param nicknames Synonyms for application
   * @param app_hmi_types Section on HMI where app can appear (Navigation, Phone
   * etc)
   */
  bool GetInitialAppData(const std::string& application_id,
                         StringArray* nicknames = NULL,
                         StringArray* app_hmi_types = NULL) OVERRIDE;

  /**
   * @brief Add's device to policy table
   * @param device_id        Device mac address
   * @param connection_type  Device connection type
   */
  void AddDevice(const std::string& device_id,
                 const std::string& connection_type) OVERRIDE;

  /**
   * @brief Stores device parameters received during application registration
   * to policy table
   * @param device_id Device mac address
   * @param device_info Received device parameters
   */
  void SetDeviceInfo(const std::string& device_id,
                     const DeviceInfo& device_info) OVERRIDE;

  /**
   * @brief Set user consent for application functional groups
   * @param permissions User-defined application group pemissions.
   * The permissions is not const reference because it may contains
   * valid data as well as invalid. So we will remove all invalid data
   * from this structure.
   * @param mode notification mode
   */
  void SetUserConsentForApp(const PermissionConsent& permissions,
                            const NotificationMode mode) OVERRIDE;

  /**
   * @brief Get default HMI level for application
   * @param device_id device identifier
   * @param policy_app_id Unique application id
   * @param default_hmi Default HMI level for application or empty, if value
   * was not set
   * @return true, if succedeed, otherwise - false
   */
  bool GetDefaultHmi(const std::string& device_id,
                     const std::string& policy_app_id,
                     std::string* default_hmi) const OVERRIDE;

  /**
   * @brief Get priority for application
   * @param policy_app_id Unique application id
   * @param priority Priority for application or empty, if value was not set
   * @return true, if succedeed, otherwise - false
   */
  bool GetPriority(const std::string& policy_app_id,
                   std::string* priority) const OVERRIDE;

  /**
   * @brief Get user friendly messages for given RPC messages and language
   * @param message_codes RPC message codes
   * @param language Language
   * @param active_hmi_language Currently active language
   * @return Array of structs with appropriate message parameters
   */
  std::vector<UserFriendlyMessage> GetUserFriendlyMessages(
      const std::vector<std::string>& message_code,
      const std::string& language,
      const std::string& active_hmi_language) OVERRIDE;

  /**
   * @brief Checks if the application is revoked
   * @param app_id application id
   * @return true if application is revoked
   */
  bool IsApplicationRevoked(const std::string& app_id) const OVERRIDE;

  /**
   * @brief Get resulting RPCs permissions for application which started on
   * specific device
   * @param device_id Device id
   * @param policy_app_id Unique application id
   * @param permissions Array of functional groups permissions
   */
  void GetPermissionsForApp(
      const std::string& device_id,
      const std::string& policy_app_id,
      std::vector<FunctionalGroupPermission>& permissions) OVERRIDE;

  /**
   * @brief Return device id, which hosts specific application
   * @param device_id device identifier
   * @param policy_app_id Application id, which is required to update device id
   */
  std::string& GetCurrentDeviceId(
      const transport_manager::DeviceHandle& device_id,
      const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief Set current system language
   * @param language Language
   */
  void SetSystemLanguage(const std::string& language) OVERRIDE;

  /**
   * @brief Set data from GetSystemInfo response to policy table
   * @param ccpu_version CCPU version
   * @param wers_country_code WERS country code
   * @param language System language
   */
  void SetSystemInfo(const std::string& ccpu_version,
                     const std::string& wers_country_code,
                     const std::string& language) OVERRIDE;

  void SetHardwareVersion(const std::string& hardware_version) OVERRIDE;

  void SetPreloadedPtFlag(const bool is_preloaded) OVERRIDE;

  std::string GetCCPUVersionFromPT() const OVERRIDE;

  std::string GetHardwareVersionFromPT() const OVERRIDE;

  /**
   * @brief Get number of notification by priority
   * @param priority Specified priority
   * @param is_subtle If true, get the number of allowed subtle notifications
   * @return notification number
   */
  uint32_t GetNotificationsNumber(const std::string& priority,
                                  const bool is_subtle) const OVERRIDE;

  /**
   * @brief Allows to update Vehicle Identification Number in policy table.
   * @param new value for the parameter.
   */
  void SetVINValue(const std::string& value) OVERRIDE;

  /**
   * @brief Gets specific application permissions changes since last policy
   * table update
   * @param device_id device identifier
   * @param policy_app_id Unique application id
   * @return Permissions changes
   */
  AppPermissions GetAppPermissionsChanges(
      const std::string& device_id, const std::string& policy_app_id) OVERRIDE;

  /**
   * @brief Removes specific application permissions changes
   * @param app_id Unique application id
   */
  void RemovePendingPermissionChanges(const std::string& app_id) OVERRIDE;

  /**
   * @brief Send OnPermissionsUpdated for choosen application
   * @param device_id device identifier
   * @param application_id Unique application id
   */
  void SendNotificationOnPermissionsUpdated(
      const std::string& device_id, const std::string& application_id) OVERRIDE;

  /**
   * @brief Removes unpaired device records and related records from DB
   * @return true, if succedeed, otherwise - false
   */
  bool CleanupUnpairedDevices() OVERRIDE;

  /**
   * @brief Check if app can keep context.
   * @param app_id Unique application id
   * @return true if app can keep context, otherwise - false
   */
  bool CanAppKeepContext(const std::string& app_id) const OVERRIDE;

  /**
   * @brief Check if app can steal focus.
   * @param app_id Unique application id
   * @return true if app can steal focus, otherwise - false
   */
  bool CanAppStealFocus(const std::string& app_id) const OVERRIDE;

  /**
   * @brief Marks device as upaired
   * @param device_id id device
   */
  void MarkUnpairedDevice(const std::string& device_id) OVERRIDE;

  /**
   * @brief Adds, application to the db or update existed one
   * run PTU if policy update is necessary for application.
   * @param device_id device identifier
   * @param application_id Unique application id
   * @param hmi_types application HMI types
   * @return function that will notify update manager about new application
   */
  StatusNotifier AddApplication(
      const std::string& device_id,
      const std::string& application_id,
      const rpc::policy_table_interface_base::AppHmiTypes& hmi_types) OVERRIDE;

  /**
   * @brief Assigns new HMI types for specified application
   * @param device_handle device identifier
   * @param application_id Unique application id
   * @param hmi_types new HMI types list
   */
  void SetDefaultHmiTypes(const transport_manager::DeviceHandle& device_handle,
                          const std::string& application_id,
                          const std::vector<int>& hmi_types) OVERRIDE;

  /**
   * @brief Gets HMI types
   * @param application_id ID application
   * @param app_types list to save HMI types
   * @return true if policy has specific policy for this application
   */
  bool GetHMITypes(const std::string& application_id,
                   std::vector<int>* app_types) OVERRIDE;

  /**
   * @brief Setter for access_remote instance
   * @param access_remote pointer to new access_remote instance
   */
  void set_access_remote(std::shared_ptr<AccessRemote> access_remote) OVERRIDE;

  /**
   * @brief Sends notification about application HMI level changed
   * @param who application information structure
   */
  void SendHMILevelChanged(const ApplicationOnDevice& who);

  /**
   * @brief Sends notification if application permissions were changed
   * @param application_id Unique id of application
   */
  void OnPrimaryGroupsChanged(const std::string& application_id);

  /**
   * @brief Removes consent for application functional group
   * @param application_id Unique id of application
   * @param group_name application functional group name
   */
  virtual void RemoveAppConsentForGroup(const std::string& app_id,
                                        const std::string& group_name);

  /**
   * @brief Returns heart beat timeout
   * @param app_id application id
   * @return if timeout was set then value in milliseconds greater zero
   * otherwise heart beat for specific application isn't set
   */
  uint32_t HeartBeatTimeout(const std::string& app_id) const OVERRIDE;

  /**
   * @brief SaveUpdateStatusRequired allows to save update status.
   * @param is_update_needed true if update needed
   */
  void SaveUpdateStatusRequired(bool is_update_needed) OVERRIDE;

  /**
   * @brief Checks, if application has policy assigned w/o data consent
   * @param policy_app_id Unique application id
   * @return true, if policy assigned w/o data consent, otherwise -false
   */
  bool IsPredataPolicy(const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief Setter for cache_manager instance
   * @param cache_manager pointer to new cache_manager instance
   */
  void set_cache_manager(CacheManagerInterface* cache_manager);

  /**
   * @brief Handler on applications search started
   */
  void OnAppsSearchStarted() OVERRIDE;

  /**
   * @brief Handler on applications search completed
   * @param trigger_ptu contains true if PTU should be triggered
   */
  void OnAppsSearchCompleted(const bool trigger_ptu) OVERRIDE;

  void UpdatePTUReadyAppsCount(const uint32_t new_app_count) OVERRIDE;

  /**
   * @brief Get state of request types for given application
   * @param policy_app_id Unique application id
   * @return request type state
   */
  RequestType::State GetAppRequestTypesState(
      const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief Get state of request subtypes for given application
   * @param policy_app_id Unique application id
   * @return request subtype state
   */
  RequestSubType::State GetAppRequestSubTypesState(
      const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief Gets request types for application
   * @param device_handle device identifier
   * @param policy_app_id Unique application id
   * @return request types of application
   */
  const std::vector<std::string> GetAppRequestTypes(
      const transport_manager::DeviceHandle& device_handle,
      const std::string policy_app_id) const OVERRIDE;

  /**
   * @brief Gets request subtypes for application
   * @param policy_app_id Unique application id
   * @return request subtypes of application
   */
  const std::vector<std::string> GetAppRequestSubTypes(
      const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief Gets copy of current policy table data
   * @return policy_table as json object
   */
  Json::Value GetPolicyTableData() const OVERRIDE;

  /**
   * @brief Gets vehicle data items
   * @return Structure with vehicle data items
   */
  const std::vector<policy_table::VehicleDataItem> GetVehicleDataItems()
      const OVERRIDE;

  std::vector<policy_table::VehicleDataItem> GetRemovedVehicleDataItems()
      const OVERRIDE;

  /**
   * @brief Get a list of enabled cloud applications
   * @param enabled_apps List filled with the policy app id of each enabled
   * cloud application
   */
  void GetEnabledCloudApps(
      std::vector<std::string>& enabled_apps) const OVERRIDE;

  /**
   * @brief Get a list of enabled local applications
   * @return enabled_apps List filled with the policy app id of each enabled
   * local application
   */
  std::vector<std::string> GetEnabledLocalApps() const OVERRIDE;

  bool GetAppProperties(const std::string& policy_app_id,
                        AppProperties& out_app_properties) const OVERRIDE;

  /**
   * @ brief Initialize new cloud app in the policy table
   * @ param policy_app_id Application ID
   */
  void InitCloudApp(const std::string& policy_app_id) OVERRIDE;

  /**
   * @brief Enable or disable a cloud application in the HMI
   * @param enabled Cloud app enabled state
   */
  void SetCloudAppEnabled(const std::string& policy_app_id,
                          const bool enabled) OVERRIDE;

  /**
   * @brief Set an app's auth token
   * @param auth_token Cloud app authentication token
   */
  void SetAppAuthToken(const std::string& policy_app_id,
                       const std::string& auth_token) OVERRIDE;

  /**
   * @brief Set a cloud app's transport type
   * @param cloud_transport_type Cloud app transport type
   */
  void SetAppCloudTransportType(
      const std::string& policy_app_id,
      const std::string& cloud_transport_type) OVERRIDE;

  /**
   * @brief Set a cloud app's endpoint url
   * @param endpoint URL for websocket connection
   */
  void SetAppEndpoint(const std::string& policy_app_id,
                      const std::string& endpoint) OVERRIDE;

  /**
   * @brief Set a cloud app's nicknames
   * @param nicknames Nicknames for cloud app
   */
  void SetAppNicknames(const std::string& policy_app_id,
                       const StringArray& nicknames) OVERRIDE;

  /**
   * @brief Set the user preference for how a hybrid (cloud and mobile) app
   * should be used
   * @param hybrid_app_preference Hybrid app user preference
   */
  void SetHybridAppPreference(
      const std::string& policy_app_id,
      const std::string& hybrid_app_preference) OVERRIDE;

  /**
   * @brief Get app service parameters from the policy table
   * @param policy_app_id Unique application id
   * @param app_service_parameters Pointer to struct containing all app service
   * information
   */
  void GetAppServiceParameters(const std::string& policy_app_id,
                               policy_table::AppServiceParameters*
                                   app_service_parameters) const OVERRIDE;

  /**
   * @brief Check if an app can send unknown rpc requests to an app service
   * provider
   * @param policy_app_id Unique application id
   * @return bool true if allowed
   */
  bool UnknownRPCPassthroughAllowed(
      const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief OnAppRegisteredOnMobile allows to handle event when application were
   * succesfully registered on mobile device.
   * It will send OnAppPermissionSend notification and will try to start PTU.
   *
   * @param device_id device identifier
   * @param application_id registered application.
   */
  void OnAppRegisteredOnMobile(const std::string& device_id,
                               const std::string& application_id) OVERRIDE;

  void OnDeviceSwitching(const std::string& device_id_from,
                         const std::string& device_id_to) OVERRIDE;

  /**
   * @brief Gets meta information
   * @return meta information
   */
  const MetaInfo GetMetaInfo() const OVERRIDE;

  /**
   * @brief RetrieveCertificate Allows to obtain certificate in order
   * to start secure connection.
   * @return The certificate in PKCS#7 format.
   */
  std::string RetrieveCertificate() const OVERRIDE;

  /**
   * @brief HasCertificate check whether policy table has certificate
   * int module_config section.
   * @return true in case certificate exists, false otherwise
   */
  bool HasCertificate() const OVERRIDE;

  /**
   * @brief Sets decrypted certificate in policy table
   * @param certificate content of certificate
   */
  void SetDecryptedCertificate(const std::string& certificate) OVERRIDE;

  /**
   * @brief Getter for policy settings
   * @return policy settings instance
   */
  const PolicySettings& get_settings() const OVERRIDE;

  /**
   * @brief Checks if there is existing URL in the EndpointUrls vector with
   * index saved in the policy manager and if not, it moves to the next
   * application index
   * @param rs contains the application index and url index from the
   * urls vector that are to be sent on the next OnSystemRequest
   * @param urls vector of vectors that contain urls for each application
   * @return Pair of application index and url index
   */
  AppIdURL RetrySequenceUrl(const struct RetrySequenceURL& rs,
                            const EndpointUrls& urls) const OVERRIDE;

  /**
   * @brief  Checks, if SDL needs to update it's policy table section
             "external_consent_status"
   * @param  ExternalConsent status
   * @return true if such check is needed, false - if not.
   */
  bool IsNeedToUpdateExternalConsentStatus(
      const ExternalConsentStatus& status) const OVERRIDE;

  /**
   * @brief Saves customer connectivity settings status
   * @param status ExternalConsent status
   * @return true if succeeded, otherwise - false
   */
  bool SetExternalConsentStatus(const ExternalConsentStatus& status) OVERRIDE;

  /**
   * @brief Gets customer connectivity settings status
   * @return ExternalConsent status
   */
  ExternalConsentStatus GetExternalConsentStatus() OVERRIDE;

#ifdef BUILD_TESTS
  /**
   * @brief Getter for cache_manager instance
   * @return cache_manager instance
   */
  inline CacheManagerInterfaceSPtr GetCache() {
    return cache_;
  }

  /**
   * @brief Setter for send_on_update_sent_out
   * @param send_on_update_sent_out new value of this flag
   */
  inline void SetSendOnUpdateFlags(const bool send_on_update_sent_out) {
    send_on_update_sent_out_ = send_on_update_sent_out;
  }
#endif  // BUILD_TESTS

  // Interface StatisticsManager (begin)
  /**
   * @brief Increments global counter
   * @param type counter type
   */
  void Increment(usage_statistics::GlobalCounterId type) OVERRIDE;

  /**
   * @brief Increments specified application counter
   * @param app_id Unique ID of application
   * @param type application counter type
   */
  void Increment(const std::string& app_id,
                 usage_statistics::AppCounterId type) OVERRIDE;
  /**
   * @brief Sets specified application info value
   * @param app_id Unique ID of application
   * @param type application info type
   * @param value new value for counter
   */
  void Set(const std::string& app_id,
           usage_statistics::AppInfoId type,
           const std::string& value) OVERRIDE;

  /**
   * @brief Add seconds for specified application stopwatch
   * @param app_id Unique ID of application
   * @param type application stopwatch type
   * @param timespan_seconds seconds to add
   */
  void Add(const std::string& app_id,
           usage_statistics::AppStopwatchId type,
           int32_t timespan_seconds) OVERRIDE;

  // Interface StatisticsManager (end)

  /**
   * @brief Check whether allowed retry sequence count is exceeded
   * @return bool value - true is allowed count is exceeded, otherwise - false
   */
  bool IsAllowedRetryCountExceeded() const OVERRIDE;

  /**
   * @brief Finish PTU retry requence
   */
  void RetrySequenceFailed() OVERRIDE;

  /**
   * @brief In EXTERNAL_PROPRIETARY_MODE PTU sequence is driven by HMI and
   * begins with OnSystemRequest from HMI. Following function is called when
   * this notification is received to track number of PTU retries and react
   * accordingly once allowed retry count is exceeded
   */
  void OnSystemRequestReceived() OVERRIDE;

  void OnLocalAppAdded() OVERRIDE;

  bool IsNewApplication(const std::string& application_id) const OVERRIDE;

 protected:
  /**
   * @brief Parse policy table content and convert to PT object
   * @param pt_content binary content of PT
   * @return pointer to converted PT
   */
  virtual std::shared_ptr<policy_table::Table> Parse(
      const BinaryMessage& pt_content);

 private:
  /**
   * @brief Increments PTU retry index for external flow
   */
  void IncrementRetryIndex();

  /**
   * @brief Checks if PT update should be started and schedules it if needed
   */
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
      const std::shared_ptr<policy_table::Table> update,
      const std::shared_ptr<policy_table::Table> snapshot);

  /**
   * @brief Compares current policies to the updated one.
   * Trigger actions in case if certain fields after udate was changes.
   * This function should be called after PT update.
   * Actions require already updated policy table
   * @param update Shared pointer to policy table udpate
   * @param snapshot Shared pointer to old copy of policy table
   */
  void CheckPermissionsChangesAfterUpdate(const policy_table::Table& update,
                                          const policy_table::Table& snapshot);

  /**
   * @brief Processes results from policy table update analysis done by
   * CheckPermissionsChanges() by filling ApplicationsPoliciesActions struct
   * with actions which should be done for every application and passes them to
   * ProcessActionsForAppPolicies()
   * @param results Collection of per-application results
   * @param app_policies Reference to updated application policies section as
   * a data source for generating notifications data
   */
  void ProcessAppPolicyCheckResults(
      const CheckAppPolicyResults& results,
      const policy_table::ApplicationPolicies& app_policies);

  /**
   * @brief Processes actions filled in ProcessAppPolicyCheckResults() for every
   * application by sending OnPermissionChange and OnAppPermissionChanged
   * notifications and by checking consent where it needed
   * @param actions Reference to map with actions to be done or not for every
   * application
   * @param app_policies Reference to updated application policies section as
   * a data source for generating notifications data
   */
  void ProcessActionsForAppPolicies(
      const ApplicationsPoliciesActions& actions,
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
   * @param device_id device identifier
   * @param policy application id.
   * @param cuuren consent for application's device.
   */
  void AddNewApplication(const std::string& device_id,
                         const std::string& application_id,
                         DeviceConsent device_consent);

  /**
   * @brief Allows to process case when added application is already
   * in policy db.
   * @param device_id device identifier
   * @param policy application id.
   * @param cuuren consent for application's device.
   */
  void PromoteExistedApplication(const std::string& device_id,
                                 const std::string& application_id,
                                 DeviceConsent device_consent);

  /**
   * Checks existing and permissions of AppStorageFolder
   * @return true if AppStorageFolder exists and has permissions read/write
   */
  bool CheckAppStorageFolder() const;

  /**
   * @brief Checks whether need ask the permission of users
   * @param device_id device identifier
   * @param appid policy application id
   * @return true if user consent is needed
   */
  virtual bool IsConsentNeeded(const std::string& device_id,
                               const std::string& app_id);

  /**
   * @brief Changes isConsentNeeded for app pending permissions, in case
   * user set permissions before app activation.
   * @param Unique app id
   * @param Current permissions for app
   */
  void CheckPendingPermissionsChanges(
      const std::string& policy_app_id,
      const std::vector<FunctionalGroupPermission>& current_permissions);

  /**
   * @brief StartPTExchange allows to start PTU. The function will check
   * if one is required and starts the update flow in only case when previous
   * condition is true.
   */
  void StartPTExchange() OVERRIDE;

  /**
   * @brief Trigger a PTU once on startup if it is required
   */
  void TriggerPTUOnStartupIfRequired() OVERRIDE;

  /**
   * @brief Checks is PT exceeded days
   * @return true if exceeded
   */
  bool ExceededDays() OVERRIDE;

  /**
   * @brief Checks is PT exceeded IgnitionCycles
   * @return true if exceeded
   */
  bool ExceededIgnitionCycles() OVERRIDE;

  /**
   * @brief Checks is specified policy table valid
   * @param policy_table pointer to policy table to check
   * @param type policy table type
   * @return true if policy table valid, otherwise false
   */
  bool IsPTValid(std::shared_ptr<policy_table::Table> policy_table,
                 policy_table::PolicyTableType type) const;

  /**
   * @brief Check that new applications for PTU were registered
   */
  bool HasApplicationForPTU() const;

  /**
   * @brief Get resulting RPCs permissions for application which started on
   * specific device
   * @param device_id Device id
   * @param policy_app_id Unique application id
   * @param permissions Array of RPC param/HMI permissions
   */
  void GetPermissions(const std::string device_id,
                      const std::string application_id,
                      Permissions* data);

  /**
   * @brief Checks if module for application is present in policy table
   * @param app_id id of application
   * @param module type
   * @return true if module is present, otherwise - false
   */
  bool CheckModule(const PTString& app_id, const PTString& module) OVERRIDE;

  /**
   * @brief Send OnPermissionsChange notification to mobile app
   * when it's permissions are changed.
   * @param device_id Device on which app is running
   * @param application_id ID of app whose permissions are changed
   */
  void SendAppPermissionsChanged(const std::string& device_id,
                                 const std::string& application_id) OVERRIDE;

  void SendOnAppPropertiesChangeNotification(
      const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief notify listener of updated auth token for a given policy id
   * @param policy_app_id the app id that has an updated auth token
   */
  void SendAuthTokenUpdated(const std::string policy_app_id);

  /**
   * @brief Gets all allowed module types
   * @param policy_app_id unique identifier of application
   * @param modules list of allowed module types
   * @return true if application has allowed modules
   */
  bool GetModuleTypes(const std::string& policy_app_id,
                      std::vector<std::string>* modules) const OVERRIDE;

  void ResetTimeout() OVERRIDE;

  /**
   * @brief Notify application about its permissions changes by preparing and
   * sending OnPermissionsChanged notification
   * @param device_id device identifier
   * @param policy_app_id Application id to send notification to
   * @param app_group_permissons Current permissions for groups assigned to
   * application
   */
  void NotifyPermissionsChanges(
      const std::string& device_id,
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
   * @param device_id device identifier
   * @param application_id Application id
   * @param processing_policy Defines whether consents timestamps must be
   * considered or external consents take over
   */
  void ProcessExternalConsentStatusForApp(
      const std::string& device_id,
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
   * @param device_id device identifier
   * @param app_policy Reference to application policy
   */
  void NotifySystem(const std::string& device_id,
                    const AppPoliciesValueType& app_policy) const;

  /**
   * @brief Sends OnPermissionChange notification to application if its
   * currently registered
   * @param device_id device identifier
   * @param app_policy Reference to application policy
   */
  void SendPermissionsToApp(const std::string& device_id,
                            const AppPoliciesValueType& app_policy);

  /**
   * @brief Resumes all policy actions for all apps, suspended during
   * PTU applying
   */
  void ResumePendingAppPolicyActions();

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

  /**
   * @brief pointer to policy table listener for callbacks
   */
  PolicyListener* listener_;

  /**
   * @brief UpdateStatusManager instance for controlling PT status
   */
  UpdateStatusManager update_status_manager_;

  /**
   * @brief pointer to CacheManagerInterface instance for getting policy data
   */
  CacheManagerInterfaceSPtr cache_;
  /**
   * @brief pointer to AccessRemote instance for working with RC applications
   */
  std::shared_ptr<AccessRemote> access_remote_;

  /**
   * @brief lock guard for protecting applications list access
   */
  sync_primitives::Lock apps_registration_lock_;

  /**
   * @brief lock guard for protecting policy table snapshot
   */
  sync_primitives::Lock policy_table_lock_;

  /**
   * @brief lock guard for protecting policy table exchange
   */
  sync_primitives::Lock policy_table_exchange_lock_;

  /**
   * @brief lock guard for protecting application permissions access
   */
  sync_primitives::Lock app_permissions_diff_lock_;

  /**
   * @brief Collection of parameters to be reported to the system with
   * SDL.ActivateApp response or OnAppPermissionsChanged notification
   * Being set during policy table update processing
   */
  typedef std::map<std::string, AppPermissions> PendingPermissions;
  PendingPermissions app_permissions_diff_;

  /**
   * @brief Timeout to wait response with UpdatePT
   */
  uint32_t retry_sequence_timeout_;

  /**
   * @brief Seconds between retries to update PT
   */
  std::vector<int> retry_sequence_seconds_;

  /**
   * @brief Current index trying of retry sequence
   */
  uint32_t retry_sequence_index_;

  /**
   * @brief Applications pending count ready for PTU
   */
  uint32_t applications_pending_ptu_count_;

  /**
   * @brief Lock for guarding retry sequence
   */
  mutable sync_primitives::Lock retry_sequence_lock_;

  /**
   * @brief Device id, which is used during PTU handling for specific
   * application
   */
  mutable std::string last_device_id_;

  /**
   * @brief Flag for checking first ignition cycle
   */
  bool ignition_check;

  /**
   * @brief Pointer to current policy settings structure
   */
  const PolicySettings* settings_;
  friend struct CheckAppPolicy;

  /**
   * @brief Pair of app index and url index from Endpoints vector
   * that contains all application URLs
   */
  RetrySequenceURL retry_sequence_url_;
  friend struct ProccessAppGroups;

  /**
   * @brief Flag for notifying that PTU was started
   */
  bool send_on_update_sent_out_;

  std::vector<std::string> app_properties_changed_list_;

  /**
   * @brief Flag for notifying that invalid PTU should be triggered
   */
  bool trigger_ptu_;

  /**
   * @brief Flag for notifying that PTU was requested
   */
  bool ptu_requested_;

  /**
   * @brief Flag that indicates whether a PTU sequence (including retries) is in
   * progress
   */
  bool is_ptu_in_progress_;

  typedef std::list<std::pair<std::string, AppPoliciesValueType> >
      PendingAppPolicyActionsList;

  /**
   * @brief List containing device_id and pending permissions structure pairs
   * which can be processed later on demand
   */
  PendingAppPolicyActionsList notify_system_list_;

  /**
   * @brief List containing device_id and pending permissions structure pairs
   * which can be processed later on demand
   */
  PendingAppPolicyActionsList send_permissions_list_;
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_POLICY_MANAGER_IMPL_H_
