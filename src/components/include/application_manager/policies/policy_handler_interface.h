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

#ifndef SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_POLICIES_POLICY_HANDLER_INTERFACE_H_
#define SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_POLICIES_POLICY_HANDLER_INTERFACE_H_

#include <string>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include "interfaces/MOBILE_API.h"
#include "application_manager/policies/policy_handler_observer.h"
#include "application_manager/core_service.h"
#include "application_manager/application.h"
#include "policy/usage_statistics/statistics_manager.h"
#include "utils/custom_string.h"
#include "utils/callable.h"
#include "policy/policy_settings.h"
#include "smart_objects/smart_object.h"
#include "policy/policy_types.h"
#include "policy/policy_table/types.h"

using namespace ::rpc::policy_table_interface_base;
namespace policy {
typedef utils::SharedPtr<utils::Callable> StatusNotifier;

class PolicyHandlerInterface {
 public:
  virtual ~PolicyHandlerInterface() {}

  virtual bool LoadPolicyLibrary() = 0;
  virtual bool PolicyEnabled() const = 0;
  virtual bool InitPolicyTable() = 0;
  virtual bool ResetPolicyTable() = 0;
  virtual bool ClearUserConsent() = 0;
  virtual bool SendMessageToSDK(const BinaryMessage& pt_string,
                                const std::string& url) = 0;
  virtual bool ReceiveMessageFromSDK(const std::string& file,
                                     const BinaryMessage& pt_string) = 0;
  virtual bool UnloadPolicyLibrary() = 0;
  virtual void OnPermissionsUpdated(const std::string& policy_app_id,
                                    const Permissions& permissions,
                                    const HMILevel& default_hmi) = 0;

  virtual void OnPermissionsUpdated(const std::string& policy_app_id,
                                    const Permissions& permissions) = 0;

#ifdef EXTERNAL_PROPRIETARY_MODE
  virtual void OnSnapshotCreated(const BinaryMessage& pt_string,
                                 const std::vector<int>& retry_delay_seconds,
                                 uint32_t timeout_exchange) = 0;
#else   // EXTERNAL_PROPRIETARY_MODE
  virtual void OnSnapshotCreated(const BinaryMessage& pt_string) = 0;
#endif  // EXTERNAL_PROPRIETARY_MODE

  virtual bool GetPriority(const std::string& policy_app_id,
                           std::string* priority) const = 0;
  virtual void CheckPermissions(
      const application_manager::ApplicationSharedPtr app,
      const PTString& rpc,
      const RPCParams& rpc_params,
      CheckPermissionResult& result) = 0;

  virtual uint32_t GetNotificationsNumber(
      const std::string& priority) const = 0;
  virtual DeviceConsent GetUserConsentForDevice(
      const std::string& device_id) const = 0;
  virtual bool GetDefaultHmi(const std::string& policy_app_id,
                             std::string* default_hmi) const = 0;
  virtual bool GetInitialAppData(const std::string& application_id,
                                 StringArray* nicknames = NULL,
                                 StringArray* app_hmi_types = NULL) = 0;
  virtual void GetUpdateUrls(const std::string& service_type,
                             EndpointUrls& out_end_points) = 0;
  virtual void GetUpdateUrls(const uint32_t service_type,
                             EndpointUrls& out_end_points) = 0;
  virtual std::string GetLockScreenIconUrl() const = 0;
  virtual uint32_t NextRetryTimeout() = 0;

  /**
   * Gets timeout to wait until receive response
   * @return timeout in seconds
   */
  virtual uint32_t TimeoutExchangeSec() const = 0;

  /**
   * Gets timeout to wait until receive response
   * @return timeout in miliseconds
   */
  virtual uint32_t TimeoutExchangeMSec() const = 0;
  virtual void OnExceededTimeout() = 0;
  virtual void OnSystemReady() = 0;
  virtual void PTUpdatedAt(Counters counter, int value) = 0;
  virtual void add_listener(PolicyHandlerObserver* listener) = 0;
  virtual void remove_listener(PolicyHandlerObserver* listener) = 0;

  virtual utils::SharedPtr<usage_statistics::StatisticsManager>
  GetStatisticManager() const = 0;

  virtual void SendOnAppPermissionsChanged(
      const AppPermissions& permissions,
      const std::string& policy_app_id) const = 0;

  /**
   * @brief CheckSystemAction allows to check whether certain system
   * action is enabled.
   *
   * @param system_action system action to check.
   *
   * @return true if specified system action is enabled, false otherwise.
   */
  virtual bool CheckSystemAction(mobile_apis::SystemAction::eType system_action,
                                 const std::string& policy_app_id) const = 0;

  /**
   * Lets client to notify PolicyHandler that more kilometers expired
   * @param kms New value of odometer
   */
  virtual void KmsChanged(int kms) = 0;

  /**
   * @brief Gather information for application and sends it to HMI
   * @param connection_key Connection key for application
   */
  virtual void OnActivateApp(uint32_t connection_key,
                             uint32_t correlation_id) = 0;

  /**
   * @brief Process user consent on mobile data connection access
   * @param is_allowed - user consent from response
   * @param device_mac - mac adress of device
   */
  virtual void OnAllowSDLFunctionalityNotification(
      bool is_allowed, const std::string& device_mac) = 0;

  /**
   * @brief Increment counter for ignition cycles
   */
  virtual void OnIgnitionCycleOver() = 0;

  virtual void OnPendingPermissionChange(const std::string& policy_app_id) = 0;

  /**
   * Initializes PT exchange at user request
   * @param correlation_id correlation id of request
   */
  virtual void PTExchangeAtUserRequest(uint32_t correlation_id) = 0;

  /**
   * @brief Add's device to policy table
   * @param device_id        Device mac address
   * @param connection_type  Device connection type
   */
  virtual void AddDevice(const std::string& device_id,
                         const std::string& connection_type) = 0;

  /**
   * @brief Save device info for specific device to policy table
   * @param device_id Device mac address
   * @param device_info Device params
   */
  virtual void SetDeviceInfo(const std::string& device_id,
                             const DeviceInfo& device_info) = 0;

/**
*@brief Processes data from OnAppPermissionConsent notification with
*permissions changes and ExternalConsent status changes done by user
*@param connection_key Connection key of application, 0 if no key has been
*provided
*@param permissions Groups permissions changes
*@param external_consent_status Customer connectivity settings status changes
*/
#ifdef EXTERNAL_PROPRIETARY_MODE
  virtual void OnAppPermissionConsent(
      const uint32_t connection_key,
      const PermissionConsent& permissions,
      const ExternalConsentStatus& external_consent_status) = 0;
#else
  virtual void OnAppPermissionConsent(const uint32_t connection_key,
                                      const PermissionConsent& permissions) = 0;
#endif

  /**
   * @brief Get appropriate message parameters and send them with response
   * to HMI
   * @param message_codes RPC message codes
   * @param language Language
   * @param correlation_id correlation id of request
   */
  virtual void OnGetUserFriendlyMessage(
      const std::vector<std::string>& message_codes,
      const std::string& language,
      uint32_t correlation_id) = 0;

  /**
   * @brief Get list of permissions for application/device binded to
   * connection key from request and send response
   * @param connection_key Connection key for specific application or 0 for all
   * currently registered applications
   * @param correlation_id Correlation id from request
   */
  virtual void OnGetListOfPermissions(const uint32_t connection_key,
                                      const uint32_t correlation_id) = 0;

  /**
   * @brief Get current policy table update state and send response
   * @param correlation_id Correlation id from request
   */
  virtual void OnGetStatusUpdate(const uint32_t correlation_id) = 0;

  /**
   * @brief Send notification to HMI with changed policy update status
   * @param status Current policy update state
   */
  virtual void OnUpdateStatusChanged(const std::string& status) = 0;

  /**
   * @brief Update currently used device id in policies manager for given
   * application
   * @param policy_app_id Application id
   */
  virtual std::string OnCurrentDeviceIdUpdateRequired(
      const std::string& policy_app_id) = 0;

  /**
   * @brief Set parameters from OnSystemInfoChanged to policy table
   * @param language System language
   */
  virtual void OnSystemInfoChanged(const std::string& language) = 0;

  /**
   * @brief Save data from GetSystemInfo request to policy table
   * @param ccpu_version CCPU version
   * @param wers_country_code WERS country code
   * @param language System language
   */
  virtual void OnGetSystemInfo(const std::string& ccpu_version,
                               const std::string& wers_country_code,
                               const std::string& language) = 0;

  /**
   * @brief Send request to HMI to get update on system parameters
   */
  virtual void OnSystemInfoUpdateRequired() = 0;

  /**
   * @brief Sends GetVehicleData request in case when Vechicle info is ready.
   */
  virtual void OnVIIsReady() = 0;

  /**
   * @brief Allows to update vechicle data info.
   * @param SmartObject which contains all needed information.
   */
  virtual void OnVehicleDataUpdated(
      const smart_objects::SmartObject& message) = 0;

  /**
   * Removes device
   * @param device_id id of device
   */
  virtual void RemoveDevice(const std::string& device_id) = 0;

  /**
   * Adds statistics info
   * @param type type of info
   */
  virtual void AddStatisticsInfo(int type) = 0;

  /**
   * Handles system error
   * @param code code of error
   */
  virtual void OnSystemError(int code) = 0;

  /**
   * @brief Choose application id to be used for snapshot sending
   * @return Application id or 0, if there are no applications registered
   */
  virtual uint32_t GetAppIdForSending() const = 0;

  virtual utils::custom_string::CustomString GetAppName(
      const std::string& policy_app_id) = 0;

  virtual void OnUpdateHMIAppType(
      std::map<std::string, StringArray> app_hmi_types) = 0;

  virtual void OnCertificateUpdated(const std::string& certificate_data) = 0;

  virtual void OnPTUFinished(const bool ptu_result) = 0;

#ifdef EXTERNAL_PROPRIETARY_MODE
  virtual void OnCertificateDecrypted(bool is_succeeded) = 0;
#endif  // EXTERNAL_PROPRIETARY_MODE
  virtual bool CanUpdate() = 0;

  virtual void OnDeviceConsentChanged(const std::string& device_id,
                                      const bool is_allowed) = 0;

  virtual void OnPTExchangeNeeded() = 0;

  virtual void GetAvailableApps(std::queue<std::string>& apps) = 0;

  /**
   * @brief Allows to add new or update existed application during
   * registration process
   * @param application_id The policy aplication id.
   * @return function that will notify update manager about new application
   */
  virtual StatusNotifier AddApplication(
      const std::string& application_id,
      const rpc::policy_table_interface_base::AppHmiTypes& hmi_types) = 0;

  /**
   * Checks whether application is revoked
   * @param app_id id application
   * @return true if application is revoked
   */
  virtual bool IsApplicationRevoked(const std::string& app_id) = 0;

  /**
   * @brief Notifies policy manager, that PTS was sent out
   */
  virtual void OnUpdateRequestSentToMobile() = 0;

  /**
   * Returns heart beat timeout
   * @param app_id application id
   * @return if timeout was set then value in milliseconds greater zero
   * otherwise heart beat for specific application isn't set
   */
  virtual uint32_t HeartBeatTimeout(const std::string& app_id) const = 0;

  /**
   * @brief Handler on applications search started
   */
  virtual void OnAppsSearchStarted() = 0;

  /**
   * @brief Handler on applications search completed
   */
  virtual void OnAppsSearchCompleted(const bool trigger_ptu) = 0;

  /**
   * @brief OnAppRegisteredOnMobile allows to handle event when application were
   * succesfully registered on mobile device.
   * It will send OnAppPermissionSend notification and will try to start PTU.
   *
   * @param application_id registered application.
   */
  virtual void OnAppRegisteredOnMobile(const std::string& application_id) = 0;

  /**
   * @brief Checks if certain request type is allowed for application
   * @param policy_app_id Unique applicaion id
   * @param type Request type
   * @return true, if allowed, otherwise - false
   */
  virtual bool IsRequestTypeAllowed(
      const std::string& policy_app_id,
      mobile_apis::RequestType::eType type) const = 0;

  /**
   * @brief Gets application request types
   * @param policy_app_id Unique application id
   * @return request types
   */
  virtual const std::vector<std::string> GetAppRequestTypes(
      const std::string& policy_app_id) const = 0;

  /**
   * @brief Gets vehicle information
   * @return Structure with vehicle information
   */
  virtual const VehicleInfo GetVehicleInfo() const = 0;
#ifdef EXTERNAL_PROPRIETARY_MODE
  /**
   * @brief Gets meta information
   * @return meta information
   */
  virtual const policy::MetaInfo GetMetaInfo() const = 0;
#endif  // EXTERNAL_PROPRIETARY_MODE
  virtual void Increment(usage_statistics::GlobalCounterId type) = 0;
  virtual void Increment(const std::string& app_id,
                         usage_statistics::AppCounterId type) = 0;
  virtual void Set(const std::string& app_id,
                   usage_statistics::AppInfoId type,
                   const std::string& value) = 0;
  virtual void Add(const std::string& app_id,
                   usage_statistics::AppStopwatchId type,
                   int32_t timespan_seconds) = 0;

#ifdef ENABLE_SECURITY
  virtual std::string RetrieveCertificate() const = 0;
#endif  // ENABLE_SECURITY

  virtual const PolicySettings& get_settings() const = 0;
  virtual const std::string RemoteAppsUrl() const = 0;

  /**
   * @brief OnDeviceSwitching Notifies policy manager on device switch event so
   * policy permissions should be processed accordingly
   * @param device_id_from Id of device being switched
   * @param device_id_to Id of device on the new transport
   */
  virtual void OnDeviceSwitching(const std::string& device_id_from,
                                 const std::string& device_id_to) = 0;

#ifdef SDL_REMOTE_CONTROL
  /**
   * @brief Sets HMI default type for specified application
   * @param application_id ID application
   * @param app_types list of HMI types
   */
  virtual void SetDefaultHmiTypes(
      const std::string& application_id,
      const smart_objects::SmartObject* app_types) = 0;

  /**
   * Checks if application has HMI type
   * @param application_id ID application
   * @param hmi HMI type to check
   * @param app_types additional list of HMI type to search in it
   * @return true if hmi is contained in policy or app_types
   */
  virtual bool CheckHMIType(const std::string& application_id,
                            mobile_apis::AppHMIType::eType hmi,
                            const smart_objects::SmartObject* app_types) = 0;

  /**
   * Notifies about changing HMI level
   * @param device_id unique identifier of device
   * @param policy_app_id unique identifier of application in policy
   * @param hmi_level default HMI level for this application
   */
  virtual void OnUpdateHMILevel(const std::string& device_id,
                                const std::string& policy_app_id,
                                const std::string& hmi_level) = 0;

  /**
   * Checks if module for application is present in policy table
   * @param app_id id of application
   * @param module type
   * @return true if module is present, otherwise - false
   */
  virtual bool CheckModule(const PTString& app_id, const PTString& module) = 0;

  /**
   * @brief Notifies Remote apps about change in permissions
   * @param device_id Device on which app is running
   * @param application_id ID of app whose permissions are changed
   */
  virtual void OnRemoteAppPermissionsChanged(
      const std::string& device_id, const std::string& application_id) = 0;

  /**
   * @brief Notifies Remote apps about change in HMI status
   * @param device_id Device on which app is running
   * @param policy_app_id ID of application
   * @param hmi_level new HMI level for this application
   */
  virtual void OnUpdateHMIStatus(const std::string& device_id,
                                 const std::string& policy_app_id,
                                 const std::string& hmi_level) = 0;

  /**
   * Gets all allowed module types
   * @param app_id unique identifier of application
   * @param list of allowed module types
   * @return true if application has allowed modules
   */
  virtual bool GetModuleTypes(const std::string& policy_app_id,
                              std::vector<std::string>* modules) const = 0;
#endif  // SDL_REMOTE_CONTROL

 private:
/**
 * @brief Processes data received via OnAppPermissionChanged notification
 * from. Being started asyncronously from AppPermissionDelegate class.
 * Sets updated permissions and ExternalConsent for registered applications
*and
 * applications which already have appropriate group assigned which related to
 * devices already known by policy
 * @param connection_key Connection key of application, 0 if no key has been
 * provided within notification
 * @param external_consent_status Customer connectivity settings changes to
*process
*@param permissions Permissions changes to process
 */
#ifdef EXTERNAL_PROPRIETARY_MODE
  virtual void OnAppPermissionConsentInternal(
      const uint32_t connection_key,
      const ExternalConsentStatus& external_consent_status,
      PermissionConsent& out_permissions) = 0;
#else
  virtual void OnAppPermissionConsentInternal(
      const uint32_t connection_key, PermissionConsent& out_permissions) = 0;
#endif

  friend class AppPermissionDelegate;
};

}  //  namespace policy

#endif  // SRC_COMPONENTS_INCLUDE_APPLICATION_MANAGER_POLICIES_POLICY_HANDLER_INTERFACE_H_
