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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POLICIES_POLICY_HANDLER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POLICIES_POLICY_HANDLER_H_

#include <string>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <stdint.h>

#include "policy/policy_manager.h"
#include "application_manager/application.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "application_manager/policies/policy_event_observer.h"
#include "application_manager/policies/delegates/statistics_delegate.h"
#include "application_manager/policies/policy_handler_observer.h"
#include "utils/logger.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "utils/conditional_variable.h"
#include "utils/rwlock.h"
#include "utils/custom_string.h"
#include "policy/usage_statistics/statistics_manager.h"
#include "utils/threads/async_runner.h"
#include "policy/policy_settings.h"

namespace Json {
class Value;
}

namespace application_manager {
class ApplicationManager;
}

namespace policy {

typedef std::vector<uint32_t> AppIds;
typedef std::vector<transport_manager::DeviceHandle> DeviceHandles;
namespace custom_str = utils::custom_string;

class PolicyHandler : public PolicyHandlerInterface,
                      public PolicyListener,
                      public threads::AsyncRunner {
 public:
  PolicyHandler(const policy::PolicySettings& get_settings,
                application_manager::ApplicationManager& application_manager);
  virtual ~PolicyHandler();
  bool LoadPolicyLibrary() OVERRIDE;
  bool PolicyEnabled() const OVERRIDE;
  bool InitPolicyTable() OVERRIDE;
  bool ResetPolicyTable() OVERRIDE;
  bool ClearUserConsent() OVERRIDE;
  bool SendMessageToSDK(const BinaryMessage& pt_string,
                        const std::string& url) OVERRIDE;
  bool ReceiveMessageFromSDK(const std::string& file,
                             const BinaryMessage& pt_string) OVERRIDE;
  bool UnloadPolicyLibrary() OVERRIDE;
  virtual void OnPermissionsUpdated(const std::string& policy_app_id,
                                    const Permissions& permissions,
                                    const HMILevel& default_hmi) OVERRIDE;

  virtual void OnPermissionsUpdated(const std::string& policy_app_id,
                                    const Permissions& permissions) OVERRIDE;

#ifdef EXTERNAL_PROPRIETARY_MODE
  void OnSnapshotCreated(const BinaryMessage& pt_string,
                         const std::vector<int>& retry_delay_seconds,
                         uint32_t timeout_exchange) OVERRIDE;
#else   // EXTERNAL_PROPRIETARY_MODE
  void OnSnapshotCreated(const BinaryMessage& pt_string) OVERRIDE;
#endif  // EXTERNAL_PROPRIETARY_MODE
  virtual bool GetPriority(const std::string& policy_app_id,
                           std::string* priority) const OVERRIDE;
  virtual void CheckPermissions(
      const application_manager::ApplicationSharedPtr app,
      const PTString& rpc,
      const RPCParams& rpc_params,
      CheckPermissionResult& result) OVERRIDE;

  uint32_t GetNotificationsNumber(const std::string& priority) const OVERRIDE;
  virtual DeviceConsent GetUserConsentForDevice(
      const std::string& device_id) const OVERRIDE;

  /**
   * @brief Sets HMI default type for specified application
   * @param application_id ID application
   * @param app_types list of HMI types
   */
  void SetDefaultHmiTypes(const std::string& application_id,
                          const smart_objects::SmartObject* app_types) OVERRIDE;

  /**
   * Checks if application has HMI type
   * @param application_id ID application
   * @param hmi HMI type to check
   * @param app_types additional list of HMI type to search in it
   * @return true if hmi is contained in policy or app_types
   */
  bool CheckHMIType(const std::string& application_id,
                    mobile_apis::AppHMIType::eType hmi,
                    const smart_objects::SmartObject* app_types) OVERRIDE;

  /**
   * Notifies about changing HMI level
   * @param device_id unique identifier of device
   * @param policy_app_id unique identifier of application in policy
   * @param hmi_level default HMI level for this application
   */
  void OnUpdateHMILevel(const std::string& device_id,
                        const std::string& policy_app_id,
                        const std::string& hmi_level) OVERRIDE;

  /**
   * Checks if module for application is present in policy table
   * @param app_id id of application
   * @param module type
   * @return true if module is present, otherwise - false
   */
  bool CheckModule(const PTString& app_id, const PTString& module) OVERRIDE;

  /**
   * @brief Notifies Remote apps about change in permissions
   * @param device_id Device on which app is running
   * @param application_id ID of app whose permissions are changed
   */
  void OnRemoteAppPermissionsChanged(
      const std::string& device_id, const std::string& application_id) OVERRIDE;

  /**
   * @brief Notifies Remote apps about change in HMI status
   * @param device_id Device on which app is running
   * @param policy_app_id ID of application
   * @param hmi_level new HMI level for this application
   */
  void OnUpdateHMIStatus(const std::string& device_id,
                         const std::string& policy_app_id,
                         const std::string& hmi_level) OVERRIDE;

  /**
   * Gets all allowed module types
   * @param app_id unique identifier of application
   * @param list of allowed module types
   * @return true if application has allowed modules
   */
  bool GetModuleTypes(const std::string& policy_app_id,
                      std::vector<std::string>* modules) const OVERRIDE;

  bool GetDefaultHmi(const std::string& policy_app_id,
                     std::string* default_hmi) const OVERRIDE;
  bool GetInitialAppData(const std::string& application_id,
                         StringArray* nicknames = NULL,
                         StringArray* app_hmi_types = NULL) OVERRIDE;
  void GetUpdateUrls(const std::string& service_type,
                     EndpointUrls& out_end_points) OVERRIDE;
  void GetUpdateUrls(const uint32_t service_type,
                     EndpointUrls& out_end_points) OVERRIDE;
  virtual std::string GetLockScreenIconUrl() const OVERRIDE;
  uint32_t NextRetryTimeout() OVERRIDE;

  /**
   * Gets timeout to wait until receive response
   * @return timeout in seconds
   */
  uint32_t TimeoutExchangeSec() const OVERRIDE;

  /**
   * Gets timeout to wait until receive response
   * @return timeout in miliseconds
   */
  uint32_t TimeoutExchangeMSec() const OVERRIDE;
  void OnExceededTimeout() OVERRIDE;
  void OnSystemReady() OVERRIDE;
  void PTUpdatedAt(Counters counter, int value) OVERRIDE;
  void add_listener(PolicyHandlerObserver* listener) OVERRIDE;
  void remove_listener(PolicyHandlerObserver* listener) OVERRIDE;

  std::shared_ptr<usage_statistics::StatisticsManager> GetStatisticManager()
      const OVERRIDE;

  /**
   * @brief CheckSystemAction allows to check whether certain system
   * action is enabled.
   *
   * @param system_action system action to check.
   *
   * @return true if specified system action is enabled, false otherwise.
   */
  bool CheckSystemAction(mobile_apis::SystemAction::eType system_action,
                         const std::string& policy_app_id) const OVERRIDE;

  /**
   * Lets client to notify PolicyHandler that more kilometers expired
   * @param kms New value of odometer
   */
  void KmsChanged(int kms) OVERRIDE;

  /**
   * @brief Gather information for application and sends it to HMI
   * @param connection_key Connection key for application
   */
  void OnActivateApp(uint32_t connection_key, uint32_t correlation_id) OVERRIDE;

  /**
   * @brief Process user consent on mobile data connection access
   * @param is_allowed - user consent from response
   * @param device_mac - mac adress of device
   */
  void OnAllowSDLFunctionalityNotification(
      bool is_allowed, const std::string& device_mac) OVERRIDE;

  /**
   * @brief Increment counter for ignition cycles
   */
  void OnIgnitionCycleOver() OVERRIDE;

  void OnPendingPermissionChange(const std::string& policy_app_id) OVERRIDE;

  /**
   * Initializes PT exchange at user request
   * @param correlation_id correlation id of request
   */
  void PTExchangeAtUserRequest(uint32_t correlation_id) OVERRIDE;

  /**
   * @brief Add's device to policy table
   * @param device_id        Device mac address
   * @param connection_type  Device connection type
   */
  void AddDevice(const std::string& device_id,
                 const std::string& connection_type) OVERRIDE;

  /**
   * @brief Save device info for specific device to policy table
   * @param device_id Device mac address
   * @param device_info Device params
   */
  void SetDeviceInfo(const std::string& device_id,
                     const DeviceInfo& device_info) OVERRIDE;

/**
 * @brief Store user-changed permissions consent to DB
 * @param connection_key Connection key of application or 0, if permissions
 * should be applied to all applications
 * @param permissions User-changed group permissions consent
 */

/**
 * @brief Processes permissions changes received from system via
 * OnAppPermissionConsent notification
 * @param connection_key Connection key of application, 0 if no key has been
 * provided by notification
 * @param permissions Structure containing group permissions changes
 * @param external_consent_status Structure containig customer connectivity
 * settings
 * changes
 */
#ifdef EXTERNAL_PROPRIETARY_MODE
  void OnAppPermissionConsent(
      const uint32_t connection_key,
      const PermissionConsent& permissions,
      const ExternalConsentStatus& external_consent_status) OVERRIDE;
#else
  void OnAppPermissionConsent(const uint32_t connection_key,
                              const PermissionConsent& permissions) OVERRIDE;
#endif

  /**
   * @brief Get appropriate message parameters and send them with response
   * to HMI
   * @param message_codes RPC message codes
   * @param language Language
   * @param correlation_id correlation id of request
   */
  void OnGetUserFriendlyMessage(const std::vector<std::string>& message_codes,
                                const std::string& language,
                                uint32_t correlation_id) OVERRIDE;

  /**
   * @brief Get list of permissions for application/device binded to
   * connection key from request and send response
   * @param connection_key Connection key for specific application or 0 for all
   * currently registered applications
   * @param correlation_id Correlation id from request
   */
  void OnGetListOfPermissions(const uint32_t connection_key,
                              const uint32_t correlation_id) OVERRIDE;

  /**
   * @brief Get current policy table update state and send response
   * @param correlation_id Correlation id from request
   */
  void OnGetStatusUpdate(const uint32_t correlation_id) OVERRIDE;

  /**
   * @brief Send notification to HMI with changed policy update status
   * @param status Current policy update state
   */
  void OnUpdateStatusChanged(const std::string& status) OVERRIDE;

  /**
   * @brief Update currently used device id in policies manager for given
   * application
   * @param policy_app_id Application id
   */
  std::string OnCurrentDeviceIdUpdateRequired(
      const std::string& policy_app_id) OVERRIDE;

  /**
   * @brief Set parameters from OnSystemInfoChanged to policy table
   * @param language System language
   */
  void OnSystemInfoChanged(const std::string& language) OVERRIDE;

  /**
   * @brief Save data from GetSystemInfo request to policy table
   * @param ccpu_version CCPU version
   * @param wers_country_code WERS country code
   * @param language System language
   */
  void OnGetSystemInfo(const std::string& ccpu_version,
                       const std::string& wers_country_code,
                       const std::string& language) OVERRIDE;

  /**
   * @brief Send request to HMI to get update on system parameters
   */
  virtual void OnSystemInfoUpdateRequired() OVERRIDE;

  /**
   * @brief Sends GetVehicleData request in case when Vechicle info is ready.
   */
  virtual void OnVIIsReady() OVERRIDE;

  /**
   * @brief Allows to update vechicle data info.
   * @param SmartObject which contains all needed information.
   */
  virtual void OnVehicleDataUpdated(
      const smart_objects::SmartObject& message) OVERRIDE;

  /**
   * Removes device
   * @param device_id id of device
   */
  void RemoveDevice(const std::string& device_id) OVERRIDE;

  /**
   * Adds statistics info
   * @param type type of info
   */
  void AddStatisticsInfo(int type) OVERRIDE;

  /**
   * Handles system error
   * @param code code of error
   */
  void OnSystemError(int code) OVERRIDE;

  /**
   * @brief Chooses random application id to be used for snapshot sending
   * considering HMI level
   * @return Application id or 0, if there are no suitable applications
   */
  uint32_t GetAppIdForSending() const OVERRIDE;

  custom_str::CustomString GetAppName(
      const std::string& policy_app_id) OVERRIDE;

  /**
   * @brief Get a list of enabled cloud applications
   * @param enabled_apps List filled with the policy app id of each enabled
   * cloud application
   */
  void GetEnabledCloudApps(
      std::vector<std::string>& enabled_apps) const OVERRIDE;

  /**
   * @brief Checks if a given application is an enabled cloud application
   * @param policy_app_id Unique application id
   * @return true, if the application is an enabled cloud application,
   * otherwise - false
   */
  const bool CheckCloudAppEnabled(
      const std::string& policy_app_id) const OVERRIDE;

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
  void GetCloudAppParameters(const std::string& policy_app_id,
                             bool& enabled,
                             std::string& endpoint,
                             std::string& certificate,
                             std::string& auth_token,
                             std::string& cloud_transport_type,
                             std::string& hybrid_app_preference) const OVERRIDE;

  /**
   * @brief Callback for when a SetCloudAppProperties message is received from a
   * mobile app
   * @param message The SetCloudAppProperties message
   */
  void OnSetCloudAppProperties(
      const smart_objects::SmartObject& message) OVERRIDE;

  virtual void OnUpdateHMIAppType(
      std::map<std::string, StringArray> app_hmi_types) OVERRIDE;

  virtual void OnCertificateUpdated(
      const std::string& certificate_data) OVERRIDE;
#ifdef EXTERNAL_PROPRIETARY_MODE
  void OnCertificateDecrypted(bool is_succeeded) OVERRIDE;
#endif  // EXTERNAL_PROPRIETARY_MODE
  void OnAuthTokenUpdated(const std::string& policy_app_id,
                          const std::string& auth_token);

  virtual bool CanUpdate() OVERRIDE;

  virtual void OnDeviceConsentChanged(const std::string& device_id,
                                      const bool is_allowed) OVERRIDE;

  virtual void SendOnAppPermissionsChanged(
      const AppPermissions& permissions,
      const std::string& policy_app_id) const OVERRIDE;

  virtual void OnPTExchangeNeeded() OVERRIDE;

  virtual void GetAvailableApps(std::queue<std::string>& apps) OVERRIDE;

  /**
   * @brief Allows to add new or update existed application during
   * registration process
   * @param application_id The policy aplication id.
   * @param hmi_types list of hmi types
   * @return function that will notify update manager about new application
   */
  StatusNotifier AddApplication(
      const std::string& application_id,
      const rpc::policy_table_interface_base::AppHmiTypes& hmi_types) OVERRIDE;

  /**
   * Checks whether application is revoked
   * @param app_id id application
   * @return true if application is revoked
   */
  bool IsApplicationRevoked(const std::string& app_id) OVERRIDE;

  /**
   * @brief Notifies policy manager, that PTS was sent out
   */
  void OnUpdateRequestSentToMobile() OVERRIDE;

  /**
   * Returns heart beat timeout
   * @param app_id application id
   * @return if timeout was set then value in milliseconds greater zero
   * otherwise heart beat for specific application isn't set
   */
  uint32_t HeartBeatTimeout(const std::string& app_id) const OVERRIDE;

  /**
   * @brief Returns URL for querying list of remote apps
   */
  const std::string RemoteAppsUrl() const OVERRIDE;

  /**
   * @brief Handler on applications search started
   */
  void OnAppsSearchStarted() OVERRIDE;

  /**
   * @brief Handler on applications search completed
   */
  void OnAppsSearchCompleted(const bool trigger_ptu) OVERRIDE;

  /**
   * @brief OnAppRegisteredOnMobile allows to handle event when application were
   * succesfully registered on mobile device.
   * It will send OnAppPermissionSend notification and will try to start PTU.
   *
   * @param application_id registered application.
   */
  void OnAppRegisteredOnMobile(const std::string& application_id) OVERRIDE;

  /**
   * @brief Checks if certain request type is allowed for application
   * @param policy_app_id Unique applicaion id
   * @param type Request type
   * @return true, if allowed, otherwise - false
   */
  bool IsRequestTypeAllowed(
      const std::string& policy_app_id,
      mobile_apis::RequestType::eType type) const OVERRIDE;

  /**
   * @brief Checks if certain request subtype is allowed for application
   * @param policy_app_id Unique applicaion id
   * @param request_subtype Request subtype
   * @return true, if allowed, otherwise - false
   */
  bool IsRequestSubTypeAllowed(
      const std::string& policy_app_id,
      const std::string& request_subtype) const OVERRIDE;

  /**
   * @brief Gets application request types state
   * @param policy_app_id Unique application id
   * @return request types state
   */
  RequestType::State GetAppRequestTypeState(
      const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief Gets application request subtypes state
   * @param policy_app_id Unique application id
   * @return request subtypes state
   */
  RequestSubType::State GetAppRequestSubTypeState(
      const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief Gets application request types
   * @param policy_app_id Unique application id
   * @return request types
   */
  const std::vector<std::string> GetAppRequestTypes(
      const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief Gets application request subtypes
   * @param policy_app_id Unique application id
   * @return app request subtypes
   */
  const std::vector<std::string> GetAppRequestSubTypes(
      const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief Gets vehicle information
   * @return Structure with vehicle information
   */
  const VehicleInfo GetVehicleInfo() const OVERRIDE;

#ifdef EXTERNAL_PROPRIETARY_MODE
  /**
   * @brief Gets meta information
   * @return meta information
   */
  const policy::MetaInfo GetMetaInfo() const OVERRIDE;
#endif  // EXTERNAL_PROPRIETARY_MODE

  // TODO(AKutsan) REMOVE THIS UGLY HOTFIX
  void Increment(usage_statistics::GlobalCounterId type) OVERRIDE;
  void Increment(const std::string& app_id,
                 usage_statistics::AppCounterId type) OVERRIDE;
  void Set(const std::string& app_id,
           usage_statistics::AppInfoId type,
           const std::string& value) OVERRIDE;
  void Add(const std::string& app_id,
           usage_statistics::AppStopwatchId type,
           int32_t timespan_seconds) OVERRIDE;

#ifdef BUILD_TESTS
  void SetPolicyManager(std::shared_ptr<PolicyManager> pm) {
    policy_manager_ = pm;
  }
#endif  // BUILD_TESTS

#ifdef ENABLE_SECURITY
  std::string RetrieveCertificate() const OVERRIDE;
#endif  // ENABLE_SECURITY

  const PolicySettings& get_settings() const OVERRIDE;

  virtual void OnPTUFinished(const bool ptu_result) OVERRIDE;

  /**
   * @brief OnDeviceSwitching Notifies policy manager on device switch event so
   * policy permissions should be processed accordingly
   * @param device_id_from Id of device being switched
   * @param device_id_to Id of device on the new transport
   */
  void OnDeviceSwitching(const std::string& device_id_from,
                         const std::string& device_id_to) FINAL;

 protected:
  /**
   * Starts next retry exchange policy table
   */
  void StartNextRetry();

 private:
  /**
   * Checks system action of application for permission of keep context
   * @param system_action system action (see mobile api)
   * @param policy_app_id unique application id
   * @return false if system_action is KEEP_CONTEXT and it isn't allowed by
   * policy
   * otherwise true
   */
  bool CheckKeepContext(const std::string& policy_app_id) const;

  /**
   * Checks system action of application for permission of steal focus
   * @param system_action system action (see mobile api)
   * @param policy_app_id unique application id
   * @return false if system_action is STEAL_FOCUS and it isn't allowed by
   * policy
   * otherwise true
   */
  bool CheckStealFocus(const std::string& policy_app_id) const;

/**
 * @brief Processes data received via OnAppPermissionChanged notification
 * from. Being started asyncronously from AppPermissionDelegate class.
 * Sets updated permissions and ExternalConsent for registered applications
 * and
 * applications which already have appropriate group assigned which related to
 * devices already known by policy
 * @param connection_key Connection key of application, 0 if no key has been
 * provided within notification
 * @param external_consent_status Customer connectivity settings changes to
 * process
 * @param permissions Permissions changes to process
 */
#ifdef EXTERNAL_PROPRIETARY_MODE
  void OnAppPermissionConsentInternal(
      const uint32_t connection_key,
      const ExternalConsentStatus& external_consent_status,
      PermissionConsent& out_permissions) OVERRIDE;
#else
  void OnAppPermissionConsentInternal(
      const uint32_t connection_key,
      PermissionConsent& out_permissions) OVERRIDE;
#endif

  /**
   * @brief Updates HMI level for specified application and send notification
   * @param app application where HMI level was changed
   * @param level new HMI level
   */
  void UpdateHMILevel(application_manager::ApplicationSharedPtr app,
                      mobile_apis::HMILevel::eType level);
  std::vector<std::string> GetDevicesIds(
      const std::string& policy_app_id) OVERRIDE;

  /**
   * @brief Sets days after epoch on successful policy update
   */
  void SetDaysAfterEpoch();

  /**
   * @brief Link all currently registered applications
   */
  void LinkAppsToDevice();

  typedef std::vector<application_manager::ApplicationSharedPtr> Applications;

  /**
   * @brief Checks application registration status (SDL4.0) and device consent
   * to find out whether application is suitable
   * @param value Item from applications collection
   * @return true if application is suitable, otherwise - false
   */
  bool IsAppSuitableForPolicyUpdate(const Applications::value_type value) const;

  /**
   * @brief Chooses random application from list using
   * IsAppSuitableForPolicyUpdate
   * @param app_list Application collection
   * @return Application id if suitable is found, otherwise - zero
   */
  uint32_t ChooseRandomAppForPolicyUpdate(Applications& app_list) const;

 private:
  class StatisticManagerImpl : public usage_statistics::StatisticsManager {
   public:
    StatisticManagerImpl(PolicyHandler* policy_handler)
        : policy_handler_(policy_handler) {
      DCHECK(policy_handler_);
    }
    // TODO(AKutsan) REMOVE THIS UGLY HOTFIX
    void Increment(usage_statistics::GlobalCounterId type) OVERRIDE {
      policy_handler_->AsyncRun(new StatisticsDelegate(*policy_handler_, type));
    }

    void Increment(const std::string& app_id,
                   usage_statistics::AppCounterId type) OVERRIDE {
      policy_handler_->AsyncRun(
          new StatisticsDelegate(*policy_handler_, app_id, type));
    }

    void Set(const std::string& app_id,
             usage_statistics::AppInfoId type,
             const std::string& value) OVERRIDE {
      policy_handler_->AsyncRun(
          new StatisticsDelegate(*policy_handler_, app_id, type, value));
    }

    void Add(const std::string& app_id,
             usage_statistics::AppStopwatchId type,
             int32_t timespan_seconds) OVERRIDE {
      policy_handler_->AsyncRun(new StatisticsDelegate(
          *policy_handler_, app_id, type, timespan_seconds));
    }

   private:
    PolicyHandler* policy_handler_;
  };
#ifdef EXTERNAL_PROPRIETARY_MODE
  void OnEmptyCertificateArrived() const;
#endif  // EXTERNAL_PROPRIETARY_MODE
  bool SaveSnapshot(const BinaryMessage& pt_string, std::string& snap_path);

  /**
   * @brief Collects permissions for all currently registered applications on
   * all devices
   * @return consolidated permissions list or empty list if no
   * applications/devices currently present
   */
  std::vector<FunctionalGroupPermission> CollectRegisteredAppsPermissions();

  /**
   * @brief Collects permissions for application with certain connection key
   * @param connection_key Connection key of application to look for
   * @return list of application permissions or empty list if no such
   * application found
   */
  std::vector<FunctionalGroupPermission> CollectAppPermissions(
      const uint32_t connection_key);

 private:
  static const std::string kLibrary;

  /**
 * @brief Collects currently registered applications ids linked to their
 * device id
 * @param out_links Collection of device_id-to-app_id links
 */
  void GetRegisteredLinks(std::map<std::string, std::string>& out_links) const;

 private:
  mutable sync_primitives::RWLock policy_manager_lock_;
  std::shared_ptr<PolicyManager> policy_manager_;
  void* dl_handle_;
  std::shared_ptr<PolicyEventObserver> event_observer_;
  uint32_t last_activated_app_id_;

  /**
   * @brief Contains device handles, which were sent for user consent to HMI
   */
  DeviceHandles pending_device_handles_;

  inline bool CreateManager();

  typedef std::list<PolicyHandlerObserver*> HandlersCollection;
  HandlersCollection listeners_;
  mutable sync_primitives::Lock listeners_lock_;

  /**
   * @brief Application-to-device links are used for collecting their current
   * consents to provide for HMI request and process response with possible
   * changes done by user
   */
  std::map<std::string, std::string> app_to_device_link_;

  // Lock for app to device list
  sync_primitives::RecursiveLock app_to_device_link_lock_;

  std::shared_ptr<StatisticManagerImpl> statistic_manager_impl_;
  const PolicySettings& settings_;
  application_manager::ApplicationManager& application_manager_;
  friend class AppPermissionDelegate;

  /**
   * @brief Checks if the application with the given policy
   * application id is registered or it is default id
   * @param app_idx Application idx from EndpointUrls vector
   * @param urls EndpointUrls vector
   * @return TRUE if the vector with URLs with given idx is not empty
   * and is related to a registered application or these are default URLs,
   * otherwise FALSE
   */
  bool IsUrlAppIdValid(const uint32_t app_idx, const EndpointUrls& urls) const;
  DISALLOW_COPY_AND_ASSIGN(PolicyHandler);
};

}  //  namespace policy

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POLICIES_POLICY_HANDLER_H_
