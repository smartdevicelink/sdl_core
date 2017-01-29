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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POLICY_HANDLER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POLICY_HANDLER_H_

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
typedef std::vector<uint32_t> DeviceHandles;
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

  virtual void OnSnapshotCreated(const BinaryMessage& pt_string) OVERRIDE;

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
#ifdef SDL_REMOTE_CONTROL
  /**
   * Checks access to equipment of vehicle for application by RPC
   * @param device_id unique identifier of device
   * @param app_id policy id application
   * @param zone interior zone
   * @param module type
   * @param rpc name of rpc
   * @param params parameters list
   */
  application_manager::TypeAccess CheckAccess(
      const PTString& device_id,
      const PTString& app_id,
      const application_manager::SeatLocation& zone,
      const PTString& module,
      const std::string& rpc,
      const std::vector<PTString>& params);

  /**
   * Checks access to module for application
   * @param app_id policy id application
   * @param module
   * @return true if module is allowed for application
   */
  bool CheckModule(const PTString& app_id, const PTString& module);

  /**
   * Sets access to equipment of vehicle for application by RPC
   * @param device_id unique identifier of device
   * @param app_id policy id application
   * @param zone interior zone
   * @param module type
   * @param allowed true if access is allowed
   */
  void SetAccess(const PTString& device_id,
                 const PTString& app_id,
                 const application_manager::SeatLocation& zone,
                 const PTString& module,
                 bool allowed);

  /**
   * Resets access application to all resources
   * @param device_id unique identifier of device
   * @param app_id policy id application
   */
  void ResetAccess(const PTString& device_id, const PTString& app_id);

  /**
   * Resets access by group name for all applications
   * @param zone interior zone
   * @param module type
   */
  void ResetAccess(const application_manager::SeatLocation& zone,
                   const std::string& module);

  /**
   * Sets device as primary device
   * @param dev_id ID device
   */
  void SetPrimaryDevice(const PTString& dev_id);

  /**
   * Resets driver's device
   */
  void ResetPrimaryDevice();

  /*
   * Return id of primary device
   */
  uint32_t PrimaryDevice() const;

  /**
   * Sets device zone
   * @param device_id unique identifier of device
   * @param zone device zone
   */
  void SetDeviceZone(const std::string& device_id,
                     const application_manager::SeatLocation& zone);

  /**
   * Gets device zone
   * @param dev_id ID device
   * @return device zone is unknown otherwise 0
   */
  const application_manager::SeatLocationPtr GetDeviceZone(
      const std::string& device_id) const;

  /**
   * Sets mode of remote control (on/off)
   * @param enabled true if remote control is turned on
   */
  void SetRemoteControl(bool enabled);

  /*
   * @brief If remote control is enabled
   * by User and by Policy
   */
  bool GetRemoteControl() const;

  /*
   * @brief Notifies passengers' apps about change
   * @param new_consent New value of remote permission
   */
  void OnRemoteAllowedChanged(bool new_consent);

  /*
   * @brief Notifies Remote apps about change in permissions
   * @param device_id Device on which app is running
   * @param application_id ID of app whose permissions are changed
   */
  void OnRemoteAppPermissionsChanged(const std::string& device_id,
                                     const std::string& application_id);

  virtual void OnUpdateHMIStatus(const std::string& device_id,
                                 const std::string& policy_app_id,
                                 const std::string& hmi_level);

  virtual void OnUpdateHMIStatus(const std::string& device_id,
                                 const std::string& policy_app_id,
                                 const std::string& hmi_level,
                                 const std::string& device_rank);

  /**
   * Gets all allowed module types
   * @param app_id unique identifier of application
   * @param list of allowed module types
   * @return true if application has allowed modules
   */
  bool GetModuleTypes(const std::string& policy_app_id,
                      std::vector<std::string>* modules) const;
#endif  // SDL_REMOTE_CONTROL

  bool GetDefaultHmi(const std::string& policy_app_id,
                     std::string* default_hmi) const OVERRIDE;
  bool GetInitialAppData(const std::string& application_id,
                         StringArray* nicknames = NULL,
                         StringArray* app_hmi_types = NULL) OVERRIDE;
  void GetServiceUrls(const std::string& service_type,
                      EndpointUrls& end_points) OVERRIDE;
  virtual std::string GetLockScreenIconUrl() const OVERRIDE;
  void ResetRetrySequence() OVERRIDE;
  uint32_t NextRetryTimeout() OVERRIDE;
  int TimeoutExchange() OVERRIDE;
  void OnExceededTimeout() OVERRIDE;
  void OnSystemReady() OVERRIDE;
  void PTUpdatedAt(Counters counter, int value) OVERRIDE;
  void add_listener(PolicyHandlerObserver* listener) OVERRIDE;
  void remove_listener(PolicyHandlerObserver* listener) OVERRIDE;

  utils::SharedPtr<usage_statistics::StatisticsManager> GetStatisticManager()
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
   * @param Device id or empty string, if concern to all SDL functionality
   * @param User consent from response
   */
  void OnAllowSDLFunctionalityNotification(
      bool is_allowed, const std::string& device_id) OVERRIDE;

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
  void OnAppPermissionConsent(const uint32_t connection_key,
                              const PermissionConsent& permissions) OVERRIDE;

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

#ifdef SDL_REMOTE_CONTROL
  /**
   * @brief Update currently used device id in policies manager for given
   * application
   * @param policy_app_id Application id
   * @deprecated see
   */
  std::vector<std::string> GetDevicesIds(const std::string&) OVERRIDE;
#endif
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
   * @brief Choose application id to be used for snapshot sending
   * @return Application id or 0, if there are no applications registered
   */
  uint32_t GetAppIdForSending() const OVERRIDE;

  custom_str::CustomString GetAppName(
      const std::string& policy_app_id) OVERRIDE;

  virtual void OnUpdateHMIAppType(
      std::map<std::string, StringArray> app_hmi_types) OVERRIDE;

#ifdef SDL_REMOTE_CONTROL
  virtual void OnUpdateHMILevel(const std::string& device_id,
                                const std::string& policy_app_id,
                                const std::string& hmi_level) OVERRIDE;
#endif

  virtual void OnCertificateUpdated(
      const std::string& certificate_data) OVERRIDE;

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
   * @param device_id unique identifier of device
   * @param application_id The policy aplication id.
   * @param app_types list of hmi types
   */
  void AddApplication(const std::string& application_id) OVERRIDE;

#ifdef SDL_REMOTE_CONTROL
  /**
     * Checks if application has HMI type
     * @param application_id ID application
     * @param hmi HMI type to check
     * @param app_types additional list of HMI type to search in it
     * @return true if hmi is contained in policy or app_types
     */
  bool CheckHMIType(const std::string& application_id,
                    mobile_apis::AppHMIType::eType hmi,
                    const smart_objects::SmartObject* app_types);
#endif
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
  void OnAppsSearchCompleted() OVERRIDE;

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
   * @brief Gets application request types
   * @param policy_app_id Unique application id
   * @return request types
   */
  const std::vector<std::string> GetAppRequestTypes(
      const std::string& policy_app_id) const OVERRIDE;

  /**
   * @brief Gets vehicle information
   * @return Structure with vehicle information
   */
  const VehicleInfo GetVehicleInfo() const OVERRIDE;

  /**
   * @brief OnAppRegisteredOnMobile allows to handle event when application were
   * succesfully registered on mobile device.
   * It will send OnAppPermissionSend notification and will try to start PTU.
   * @param device_handle unique indetifier of device
   * @param application_id registered application.
   */
  void OnAppRegisteredOnMobile(const std::string& application_id) OVERRIDE;

  // TODO(AKutsan) REMOVE THIS UGLY HOTFIX
  virtual void Increment(usage_statistics::GlobalCounterId type) OVERRIDE;
  virtual void Increment(const std::string& app_id,
                         usage_statistics::AppCounterId type) OVERRIDE;
  virtual void Set(const std::string& app_id,
                   usage_statistics::AppInfoId type,
                   const std::string& value) OVERRIDE;
  virtual void Add(const std::string& app_id,
                   usage_statistics::AppStopwatchId type,
                   int32_t timespan_seconds) OVERRIDE;

#ifdef BUILD_TESTS
  void SetPolicyManager(utils::SharedPtr<PolicyManager> pm) {
    policy_manager_ = pm;
  }

  AppIds& last_used_app_ids() {
    return last_used_app_ids_;
  }

#endif  // BUILD_TESTS

#ifdef ENABLE_SECURITY
  std::string RetrieveCertificate() const OVERRIDE;
#endif  // ENABLE_SECURITY

  const PolicySettings& get_settings() const OVERRIDE;

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
   * @brief OnAppPermissionConsentInternal reacts on permission changing
   *
   * @param connection_key connection key
   *
   * @param permissions new permissions.
   */
  void OnAppPermissionConsentInternal(const uint32_t connection_key,
                                      PermissionConsent& permissions) OVERRIDE;

#ifdef SDL_REMOTE_CONTROL
  void UpdateHMILevel(application_manager::ApplicationSharedPtr app,
                      mobile_apis::HMILevel::eType level);
#endif
  /**
   * @brief Sets days after epoch on successful policy update
   */
  void SetDaysAfterEpoch();

 private:
  class StatisticManagerImpl : public usage_statistics::StatisticsManager {
   public:
    StatisticManagerImpl(PolicyHandler* policy_handler)
        : policy_handler_(policy_handler) {
      DCHECK(policy_handler_);
    }
    // TODO(AKutsan) REMOVE THIS UGLY HOTFIX
    virtual void Increment(usage_statistics::GlobalCounterId type) {
      policy_handler_->AsyncRun(new StatisticsDelegate(*policy_handler_, type));
    }

    void Increment(const std::string& app_id,
                   usage_statistics::AppCounterId type) {
      policy_handler_->AsyncRun(
          new StatisticsDelegate(*policy_handler_, app_id, type));
    }

    void Set(const std::string& app_id,
             usage_statistics::AppInfoId type,
             const std::string& value) {
      policy_handler_->AsyncRun(
          new StatisticsDelegate(*policy_handler_, app_id, type, value));
    }

    void Add(const std::string& app_id,
             usage_statistics::AppStopwatchId type,
             int32_t timespan_seconds) {
      policy_handler_->AsyncRun(new StatisticsDelegate(
          *policy_handler_, app_id, type, timespan_seconds));
    }

   private:
    PolicyHandler* policy_handler_;
  };

  bool SaveSnapshot(const BinaryMessage& pt_string, std::string& snap_path);
  static const std::string kLibrary;
  mutable sync_primitives::RWLock policy_manager_lock_;
  utils::SharedPtr<PolicyManager> policy_manager_;
  void* dl_handle_;
  AppIds last_used_app_ids_;
  utils::SharedPtr<PolicyEventObserver> event_observer_;
  uint32_t last_activated_app_id_;

  /**
   * @brief Contains device handles, which were sent for user consent to HMI
   */
  DeviceHandles pending_device_handles_;

  inline bool CreateManager();

  typedef std::list<PolicyHandlerObserver*> HandlersCollection;
  HandlersCollection listeners_;
  sync_primitives::Lock listeners_lock_;

  /**
   * @brief Application-to-device map is used for getting/setting user consents
   * for all apps
   */
  std::map<std::string, std::string> app_to_device_link_;

  // Lock for app to device list
  sync_primitives::Lock app_to_device_link_lock_;

  utils::SharedPtr<StatisticManagerImpl> statistic_manager_impl_;
  const PolicySettings& settings_;
  application_manager::ApplicationManager& application_manager_;
  friend class AppPermissionDelegate;

  DISALLOW_COPY_AND_ASSIGN(PolicyHandler);
};

}  //  namespace policy

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POLICY_HANDLER_H_
