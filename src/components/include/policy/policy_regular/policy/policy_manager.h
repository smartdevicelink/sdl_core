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

#ifndef SRC_COMPONENTS_INCLUDE_POLICY_POLICY_REGULAR_POLICY_POLICY_MANAGER_H_
#define SRC_COMPONENTS_INCLUDE_POLICY_POLICY_REGULAR_POLICY_POLICY_MANAGER_H_

#include <vector>
#include <cstdint>

#include "utils/callable.h"
#include "policy/policy_types.h"
#include "policy/policy_table/types.h"
#include "policy/policy_listener.h"
#include "policy/usage_statistics/statistics_manager.h"
#include "policy/cache_manager_interface.h"
#include "policy/access_remote.h"

namespace policy {
class PolicySettings;
typedef std::shared_ptr<utils::Callable> StatusNotifier;

class PolicyManager : public usage_statistics::StatisticsManager {
 public:
  virtual ~PolicyManager() {}

  /**
   * @brief set_listener set new policy listener instance
   * @param listener new policy listener
   */
  virtual void set_listener(PolicyListener* listener) = 0;

  /**
   * @brief Inits Policy Table
   * @param file_name path to preloaded PT file
   * @param settings pointer to policy init settings
   * @return true if init is successful
   */
  virtual bool InitPT(const std::string& file_name,
                      const PolicySettings* settings) = 0;

  /**
   * @brief Updates Policy Table from binary message received from
   * mobile device. Saves to Policy Table diff between Policy Table
   * sent in snapshot and received Policy Table.
   * @param file name of file with update policy table
   * @param pt_content PTU as binary string
   * @return true if successfully
   */
  virtual bool LoadPT(const std::string& file,
                      const BinaryMessage& pt_content) = 0;

  /**
   * @brief Resets Policy Table
   * @param file_name Path to preloaded PT file
   * @return true if successfully
   */
  virtual bool ResetPT(const std::string& file_name) = 0;

  /**
   * @brief GetLockScreenIcon allows to obtain lock screen icon url;
   * @return url which point to the resource where lock screen icon could be
   *obtained.
   */
  virtual std::string GetLockScreenIconUrl() const = 0;

  /**
   * @brief Get Icon Url used for showing a cloud apps icon before the initial
   *registration
   *
   * @return url which point to the resource where icon could be
   *obtained.
   */
  virtual std::string GetIconUrl(const std::string& policy_app_id) const = 0;

  /**
   * @brief Gets all URLs for sending PTS to from PT itself.
   * @param service_type Service specifies user of URL
   * @param out_end_points output vector of urls
   */
  virtual void GetUpdateUrls(const std::string& service_type,
                             EndpointUrls& out_end_points) = 0;
  virtual void GetUpdateUrls(const uint32_t service_type,
                             EndpointUrls& out_end_points) = 0;

  /**
   * @brief PTU is needed, for this PTS has to be formed and sent.
   */
  virtual bool RequestPTUpdate() = 0;

  /**
   * @brief Check if specified RPC for specified application
   * has permission to be executed in specified HMI Level
   * and also its permitted params.
   * @param device_id Id of device of application
   * @param app_id Id of application provided during registration
   * @param hmi_level Current HMI Level of application
   * @param rpc Name of RPC
   * @param rpc_params List of RPC params
   * @param result containing flag if HMI Level is allowed and list of
   * allowed params.
   */
  virtual void CheckPermissions(const PTString& device_id,
                                const PTString& app_id,
                                const PTString& hmi_level,
                                const PTString& rpc,
                                const RPCParams& rpc_params,
                                CheckPermissionResult& result) = 0;

  /**
   * @brief Changes isConsentNeeded for app pending permissions, in case
   * user set permissions before app activation.
   * @param Unique app id
   * @param Current permissions for app
   */
  virtual void CheckPendingPermissionsChanges(
      const std::string& policy_app_id,
      const std::vector<FunctionalGroupPermission>& current_permissions) = 0;

  /**
   * @brief Clear all record of user consents. Used during Factory Reset.
   * @return bool Success of operation
   */
  virtual bool ResetUserConsent() = 0;

  /**
   * @brief Returns current status of policy table for HMI
   * @return Current status of policy table
   */
  virtual std::string GetPolicyTableStatus() const = 0;

  /**
   * @brief Checks is PT exceeded kilometers
   * @param kilometers current kilometers at odometer
   * @return true if exceeded
   */
  virtual void KmsChanged(int kilometers) = 0;

  /**
   * @brief Increments counter of ignition cycles
   */
  virtual void IncrementIgnitionCycles() = 0;

  /**
   * @brief Exchange by hmi or mobile request
   * @return Current status of policy table
   */
  virtual std::string ForcePTExchange() = 0;

  /**
   * @brief Exchange by user request
   * @return Current status of policy table
   */
  virtual std::string ForcePTExchangeAtUserRequest() = 0;

  /**
   * @brief Gets timeout to wait before next retry updating PT
   * If timeout is less or equal to zero then the retry sequence is not need.
   * @return timeout in seconds
   */
  virtual uint32_t NextRetryTimeout() = 0;

  /**
   * @brief Gets timeout to wait until receive response
   * @return timeout in seconds
   */
  virtual uint32_t TimeoutExchangeMSec() = 0;

  /**
   * @brief List of timeouts in seconds between retries
   *        when attempt to update PT fails
   * @return List of delays between attempts.
   */
  virtual const std::vector<int> RetrySequenceDelaysSeconds() = 0;

  /**
   * @brief Handler of exceeding timeout of exchanging policy table
   */
  virtual void OnExceededTimeout() = 0;

  /**
   * @brief Handler of PTS sending out
   */
  virtual void OnUpdateStarted() = 0;

  /**
   * @brief Gets user consent for mobile device data connection
   * @param device_id Unique device identifier
   * @return status of device consent
   */
  virtual DeviceConsent GetUserConsentForDevice(
      const std::string& device_id) const = 0;

  /**
   * @brief Gets user consent for application
   * @param device_id Device id
   * @param policy_app_id Unique application id
   * @param permissions Array of functional groups permissions
   */
  virtual void GetUserConsentForApp(
      const std::string& device_id,
      const std::string& policy_app_id,
      std::vector<FunctionalGroupPermission>& permissions) = 0;

  /**
   * @brief Set user consent for mobile device data connection
   * @param device_id Unique device identifier
   * @param is_allowed User consent for usage device data connection
   */
  virtual void SetUserConsentForDevice(const std::string& device_id,
                                       bool is_allowed) = 0;

  /**
   * @brief Update Application Policies as reaction
   * on User allowing/disallowing device this app is running on.
   * @param app_id Unique application id
   * @param is_device_allowed true if user allowing device otherwise false
   * @return true if operation was successful
   */
  virtual bool ReactOnUserDevConsentForApp(const std::string app_id,
                                           bool is_device_allowed) = 0;

  /**
   * @brief Sets counter value that passed for receiving PT UPdate.
   */
  virtual void PTUpdatedAt(Counters counter, int value) = 0;

  /**
   * @brief Retrieves data from app_policies about app on its registration:
   * @param application_id - id of registered app
   * @param nicknames Synonyms for application
   * @param app_hmi_types Section on HMI where app can appear (Navigation, Phone
   * etc)
   */
  virtual bool GetInitialAppData(const std::string& application_id,
                                 StringArray* nicknames = NULL,
                                 StringArray* app_hmi_types = NULL) = 0;

  /**
   * @brief Add's device to policy table
   * @param device_id        Device mac address
   * @param connection_type  Device connection type
   */
  virtual void AddDevice(const std::string& device_id,
                         const std::string& connection_type) = 0;

  /**
   * @brief Stores device parameters received during application registration
   * to policy table
   * @param device_id Device mac address
   * @param device_info Received device parameters
   */
  virtual void SetDeviceInfo(const std::string& device_id,
                             const DeviceInfo& device_info) = 0;

  /**
   * @brief Set user consent for application functional groups
   * @param permissions User-defined application group pemissions.
   * The permissions is not const reference because it may contains
   * valid data as well as invalid. So we will remove all invalid data
   * from this structure.
   */
  virtual void SetUserConsentForApp(const PermissionConsent& permissions) = 0;

  /**
   * @brief Get default HMI level for application
   * @param policy_app_id Unique application id
   * @param default_hmi Default HMI level for application or empty, if value
   * was not set
   * @return true, if succedeed, otherwise - false
   */
  virtual bool GetDefaultHmi(const std::string& policy_app_id,
                             std::string* default_hmi) const = 0;

  /**
   * @brief Get priority for application
   * @param policy_app_id Unique application id
   * @param priority Priority for application or empty, if value was not set
   * @return true, if succedeed, otherwise - false
   */
  virtual bool GetPriority(const std::string& policy_app_id,
                           std::string* priority) const = 0;

  /**
   * @brief Get user friendly messages for given RPC messages and language
   * @param message_codes RPC message codes
   * @param language Language
   * @return Array of structs with appropriate message parameters
   */
  virtual std::vector<UserFriendlyMessage> GetUserFriendlyMessages(
      const std::vector<std::string>& message_code,
      const std::string& language) = 0;

  /**
   * @brief Checks if the application is revoked
   * @param app_id application id
   * @return true if application is revoked
   */
  virtual bool IsApplicationRevoked(const std::string& app_id) const = 0;

  /**
   * @brief Get resulting RPCs permissions for application which started on
   * specific device
   * @param device_id Device id
   * @param policy_app_id Unique application id
   * @param permissions Array of functional groups permissions
   */
  virtual void GetPermissionsForApp(
      const std::string& device_id,
      const std::string& policy_app_id,
      std::vector<FunctionalGroupPermission>& permissions) = 0;

  /**
   * @brief Gets specific application permissions changes since last policy
   * table update
   * @param policy_app_id Unique application id
   * @return Permissions changes
   */
  virtual AppPermissions GetAppPermissionsChanges(
      const std::string& policy_app_id) = 0;

  /**
   * @brief Removes specific application permissions changes
   * @param app_id Unique application id
   */
  virtual void RemovePendingPermissionChanges(const std::string& app_id) = 0;

  /**
   * @brief Return device id, which hosts specific application
   * @param policy_app_id Application id, which is required to update device id
   */
  virtual std::string& GetCurrentDeviceId(
      const std::string& policy_app_id) const = 0;

  /**
   * @brief Set current system language
   * @param language Language
   */
  virtual void SetSystemLanguage(const std::string& language) = 0;

  /**
   * @brief Set data from GetSystemInfo response to policy table
   * @param ccpu_version CCPU version
   * @param wers_country_code WERS country code
   * @param language System language
   */
  virtual void SetSystemInfo(const std::string& ccpu_version,
                             const std::string& wers_country_code,
                             const std::string& language) = 0;

  /**
   * @brief Send OnPermissionsUpdated for choosen application
   * @param application_id Unique application id
   */
  virtual void SendNotificationOnPermissionsUpdated(
      const std::string& application_id) = 0;

  /**
   * @brief Marks device as upaired
   * @param device_id id device
   */
  virtual void MarkUnpairedDevice(const std::string& device_id) = 0;

  /**
   * @brief Adds, application to the db or update existed one
   * run PTU if policy update is necessary for application.
   * @param application_id Unique application id
   * @param hmi_types application HMI types
   * @return function that will notify update manager about new application
   */
  virtual StatusNotifier AddApplication(
      const std::string& application_id,
      const rpc::policy_table_interface_base::AppHmiTypes& hmi_types) = 0;

  /**
   * @brief Removes unpaired device records and related records from DB
   * @return true, if succedeed, otherwise - false
   */
  virtual bool CleanupUnpairedDevices() = 0;

  /**
   * @brief Check if app can keep context.
   * @param app_id Unique application id
   * @return true if app can keep context, otherwise - false
   */
  virtual bool CanAppKeepContext(const std::string& app_id) const = 0;

  /**
   * @brief Check if app can steal focus.
   * @param app_id Unique application id
   * @return true if app can steal focus, otherwise - false
   */
  virtual bool CanAppStealFocus(const std::string& app_id) const = 0;

  /**
   * @brief Runs necessary operations, which is depends on external system
   * state, e.g. getting system-specific parameters which are need to be
   * filled into policy table
   */
  virtual void OnSystemReady() = 0;

  /**
   * @brief Get number of notification by priority
   * @param priority Specified priority
   * @return notification number
   */
  virtual uint32_t GetNotificationsNumber(
      const std::string& priority) const = 0;

  /**
   * @brief Allows to update Vehicle Identification Number in policy table.
   * @param new value for the parameter.
   */
  virtual void SetVINValue(const std::string& value) = 0;

  /**
   * @brief Checks, if application has policy assigned w/o data consent
   * @param policy_app_id Unique application id
   * @return true, if policy assigned w/o data consent, otherwise -false
   */
  virtual bool IsPredataPolicy(const std::string& policy_app_id) const = 0;

  /**
   * @brief Returns heart beat timeout
   * @param app_id application id
   * @return if timeout was set then value in milliseconds greater zero
   * otherwise heart beat for specific application isn't set
   */
  virtual uint32_t HeartBeatTimeout(const std::string& app_id) const = 0;

  /**
   * @brief SaveUpdateStatusRequired allows to save update status.
   * @param is_update_needed true if update needed
   */
  virtual void SaveUpdateStatusRequired(bool is_update_needed) = 0;

  /**
   * @brief Handler on applications search started
   */
  virtual void OnAppsSearchStarted() = 0;

  /**
   * @brief Handler on applications search completed
   * @param trigger_ptu contains true if PTU should be triggered
   */
  virtual void OnAppsSearchCompleted(const bool trigger_ptu) = 0;

  /**
   * @brief Get state of request types for given application
   * @param policy_app_id Unique application id
   * @return request type state
   */
  virtual RequestType::State GetAppRequestTypesState(
      const std::string& policy_app_id) const = 0;

  /**
   * @brief Get state of request subtypes for given application
   * @param policy_app_id Unique application id
   * @return request subtype state
   */
  virtual RequestSubType::State GetAppRequestSubTypesState(
      const std::string& policy_app_id) const = 0;

  /**
   * @brief Gets request types for application
   * @param policy_app_id Unique application id
   * @return request types of application
   */
  virtual const std::vector<std::string> GetAppRequestTypes(
      const std::string policy_app_id) const = 0;

  /**
   * @brief Gets request subtypes for application
   * @param policy_app_id Unique application id
   * @return request subtypes of application
   */
  virtual const std::vector<std::string> GetAppRequestSubTypes(
      const std::string& policy_app_id) const = 0;

  /**
   * @brief Get information about vehicle
   * @return vehicle information
   */
  virtual const VehicleInfo GetVehicleInfo() const = 0;

  /**
   * @brief Get a list of enabled cloud applications
   * @param enabled_apps List filled with the policy app id of each enabled
   * cloud application
   */
  virtual void GetEnabledCloudApps(
      std::vector<std::string>& enabled_apps) const = 0;

  /**
   * @brief Get cloud app policy information, all fields that aren't set for a
   * given app will be filled with empty strings
   * @param policy_app_id Unique application id
   * @param enabled Whether or not the app is enabled
   * @param endpoint Filled with the endpoint used to connect to the cloud
   * application
   * @param certificate Filled with the certificate used to for creating a
   * secure connection to the cloud application
   * @param auth_token Filled with the token used for authentication when
   * reconnecting to the cloud app
   * @param cloud_transport_type Filled with the transport type used by the
   * cloud application (ex. "WSS")
   * @param hybrid_app_preference Filled with the hybrid app preference for the
   * cloud application set by the user
   */
  virtual bool GetCloudAppParameters(
      const std::string& policy_app_id,
      bool& enabled,
      std::string& endpoint,
      std::string& certificate,
      std::string& auth_token,
      std::string& cloud_transport_type,
      std::string& hybrid_app_preference) const = 0;

  /**
   * @ brief Initialize new cloud app in the policy table
   * @ param policy_app_id Application ID
   */

  virtual void InitCloudApp(const std::string& policy_app_id) = 0;

  /**
   * @brief Enable or disable a cloud application in the HMI
   * @param enabled Cloud app enabled state
   */
  virtual void SetCloudAppEnabled(const std::string& policy_app_id,
                                  const bool enabled) = 0;

  /**
   * @brief Set an app's auth token
   * @param auth_token Cloud app authentication token
   */
  virtual void SetAppAuthToken(const std::string& policy_app_id,
                               const std::string& auth_token) = 0;

  /**
   * @brief Set a cloud app's transport type
   * @param cloud_transport_type Cloud app transport type
   */
  virtual void SetAppCloudTransportType(
      const std::string& policy_app_id,
      const std::string& cloud_transport_type) = 0;

  /**
   * @brief Set a cloud app's endpoint url
   * @param endpoint URL for websocket connection
   */
  virtual void SetAppEndpoint(const std::string& policy_app_id,
                              const std::string& endpoint) = 0;

  /**
   * @brief Set the user preference for how a hybrid (cloud and mobile) app
   * should be used
   * @param hybrid_app_preference Hybrid app user preference
   */
  virtual void SetHybridAppPreference(
      const std::string& policy_app_id,
      const std::string& hybrid_app_preference) = 0;

  /**
   * @brief OnAppRegisteredOnMobile allows to handle event when application were
   * succesfully registered on mobile device.
   * It will send OnAppPermissionSend notification and will try to start PTU. *
   * @param application_id registered application.
   */
  virtual void OnAppRegisteredOnMobile(const std::string& application_id) = 0;

  virtual void OnDeviceSwitching(const std::string& device_id_from,
                                 const std::string& device_id_to) = 0;

  /**
   * @brief RetrieveCertificate Allows to obtain certificate in order
   * to start secure connection.
   * @return The certificate in PKCS#7 format.
   */
  virtual std::string RetrieveCertificate() const = 0;

  /**
   * @brief HasCertificate check whether policy table has certificate
   * int module_config section.
   * @return true in case certificate exists, false otherwise
   */
  virtual bool HasCertificate() const = 0;

  /**
   * @brief Getter for policy settings
   * @return policy settings instance
   */
  virtual const PolicySettings& get_settings() const = 0;

  /**
   * @brief Finds the next URL that must be sent on OnSystemRequest retry
   * @param urls vector of vectors that contain urls for each application
   * @return Pair of policy application id and application url id from the
   * urls vector
   */
  virtual AppIdURL GetNextUpdateUrl(const EndpointUrls& urls) = 0;

  /**
   * @brief Assigns new HMI types for specified application
   * @param application_id Unique application id
   * @param hmi_types new HMI types list
   */
  virtual void SetDefaultHmiTypes(const std::string& application_id,
                                  const std::vector<int>& hmi_types) = 0;

  /**
   * @brief Gets HMI types
   * @param application_id ID application
   * @param app_types list to save HMI types
   * @return true if policy has specific policy for this application
   */
  virtual bool GetHMITypes(const std::string& application_id,
                           std::vector<int>* app_types) = 0;

  /**
   * @brief Checks if module for application is present in policy table
   * @param app_id id of application
   * @param module type
   * @return true if module is present, otherwise - false
   */
  virtual bool CheckModule(const PTString& app_id, const PTString& module) = 0;

  /**
   * @brief Send OnPermissionsChange notification to mobile app
   * when it's permissions are changed.
   * @param device_id Device on which app is running
   * @param application_id ID of app whose permissions are changed
   */
  virtual void SendAppPermissionsChanged(const std::string& device_id,
                                         const std::string& application_id) = 0;

  /**
    * @brief Gets all allowed module types
    * @param policy_app_id unique identifier of application
    * @param modules list of allowed module types
    * @return true if application has allowed modules
    */
  virtual bool GetModuleTypes(const std::string& policy_app_id,
                              std::vector<std::string>* modules) const = 0;

  /**
   * @brief Setter for access_remote instance
   * @param access_remote pointer to new access_remote instance
   */
  virtual void set_access_remote(
      std::shared_ptr<AccessRemote> access_remote) = 0;

  /**
   * @brief Checks if there is existing URL in the EndpointUrls vector with
   * index saved in the policy manager and if not, it moves to the next
   * application index
   * @param rs contains the application index and url index from the
   * urls vector that are to be sent on the next OnSystemRequest
   * @param urls vector of vectors that contain urls for each application
   * @return Pair of application index and url index
   */
  virtual AppIdURL RetrySequenceUrl(const struct RetrySequenceURL& rs,
                                    const EndpointUrls& urls) const = 0;

 protected:
  /**
   * @brief Checks is PT exceeded IgnitionCycles
   * @return true if exceeded
   */
  virtual bool ExceededIgnitionCycles() = 0;

  /**
   * @brief Checks is PT exceeded days
   * @return true if exceeded
   */
  virtual bool ExceededDays() = 0;

  /**
   * @brief StartPTExchange allows to start PTU. The function will check
   * if one is required and starts the update flow in only case when previous
   * condition is true.
   */
  virtual void StartPTExchange() = 0;
};

}  // namespace policy

extern "C" policy::PolicyManager* CreateManager();
extern "C" void DeleteManager(policy::PolicyManager*);

#endif  // SRC_COMPONENTS_INCLUDE_POLICY_POLICY_REGULAR_POLICY_POLICY_MANAGER_H_
