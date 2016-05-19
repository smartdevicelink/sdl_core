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
#include <cstdint>
#include "policy/policy_manager.h"
#include "application_manager/policies/policy_event_observer.h"
#include "application_manager/policies/delegates/statistics_delegate.h"
#include "utils/logger.h"
#include "utils/singleton.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "utils/conditional_variable.h"
#include "utils/rwlock.h"
#include "usage_statistics/statistics_manager.h"
#include "application_manager/policies/policy_handler_observer.h"
#include "utils/threads/async_runner.h"
#include "application_manager/application_manager_impl.h"

namespace Json {
class Value;
}

namespace policy {
typedef std::vector<uint32_t> AppIds;
typedef std::vector<uint32_t> DeviceHandles;

class PolicyHandler
    : public utils::Singleton<PolicyHandler,
                              utils::deleters::Deleter<PolicyHandler>>,
      public PolicyListener,
      public threads::AsyncRunner {
public:
  virtual ~PolicyHandler();
  bool LoadPolicyLibrary();
  bool PolicyEnabled();
  bool InitPolicyTable();
  bool ResetPolicyTable();
  bool ClearUserConsent();
  bool SendMessageToSDK(const BinaryMessage &pt_string, const std::string &url);
  bool ReceiveMessageFromSDK(const std::string &file,
                             const BinaryMessage &pt_string);
  bool UnloadPolicyLibrary();
  virtual void OnPermissionsUpdated(const std::string &policy_app_id,
                                    const Permissions &permissions,
                                    const HMILevel &default_hmi);

  virtual void OnPermissionsUpdated(const std::string &policy_app_id,
                                    const Permissions &permissions);

  void OnSnapshotCreated(const BinaryMessage &pt_string) OVERRIDE;

  bool GetPriority(const std::string &policy_app_id, std::string *priority);
  void CheckPermissions(const PTString &app_id, const PTString &hmi_level,
                        const PTString &rpc, const RPCParams &rpc_params,
                        CheckPermissionResult &result);

  uint32_t GetNotificationsNumber(const std::string &priority);
  DeviceConsent GetUserConsentForDevice(const std::string &device_id);
  bool GetDefaultHmi(const std::string &policy_app_id,
                     std::string *default_hmi);
  bool GetInitialAppData(const std::string &application_id,
                         StringArray *nicknames = NULL,
                         StringArray *app_hmi_types = NULL);
  void GetServiceUrls(const std::string &service_type,
                      EndpointUrls &end_points);

  std::string GetLockScreenIconUrl() const;
  void ResetRetrySequence();
  uint32_t NextRetryTimeout();
  int TimeoutExchange();
  void OnExceededTimeout();
  void OnSystemReady();
  void PTUpdatedAt(Counters counter, int value);
  void add_listener(PolicyHandlerObserver *listener);
  void remove_listener(PolicyHandlerObserver *listener);

  utils::SharedPtr<usage_statistics::StatisticsManager> GetStatisticManager();

  /**
   * @brief CheckSystemAction allows to check whether certain system
   * action is enabled.
   *
   * @param system_action system action to check.
   *
   * @return true if specified system action is enabled, false otherwise.
   */
  bool CheckSystemAction(mobile_apis::SystemAction::eType system_action,
                         const std::string &policy_app_id);

  /**
   * Lets client to notify PolicyHandler that more kilometers expired
   * @param kms New value of odometer
   */
  void KmsChanged(int kms);

  /**
   * @brief Gather information for application and sends it to HMI
   * @param connection_key Connection key for application
   */
  void OnActivateApp(uint32_t connection_key, uint32_t correlation_id);

  /**
   * @brief Process user consent on mobile data connection access
   * @param Device id or 0, if concern to all SDL functionality
   * @param User consent from response
   */
  void OnAllowSDLFunctionalityNotification(bool is_allowed,
                                           uint32_t device_id = 0);

  /**
   * @brief Increment counter for ignition cycles
   */
  void OnIgnitionCycleOver();

  void OnPendingPermissionChange(const std::string &policy_app_id);

  /**
   * Initializes PT exchange at user request
   * @param correlation_id correlation id of request
   */
  void PTExchangeAtUserRequest(uint32_t correlation_id);

  /**
   * @brief Save device info for specific device to policy table
   * @param device_id Device mac address
   * @param device_info Device params
   */
  void SetDeviceInfo(std::string &device_id, const DeviceInfo &device_info);

  /**
   * @brief Store user-changed permissions consent to DB
   * @param connection_key Connection key of application or 0, if permissions
   * should be applied to all applications
   * @param permissions User-changed group permissions consent
   */
  void OnAppPermissionConsent(const uint32_t connection_key,
                              const PermissionConsent &permissions);

  /**
   * @brief Get appropriate message parameters and send them with response
   * to HMI
   * @param message_codes RPC message codes
   * @param language Language
   * @param correlation_id correlation id of request
   */
  void OnGetUserFriendlyMessage(const std::vector<std::string> &message_codes,
                                const std::string &language,
                                uint32_t correlation_id);

  /**
   * @brief Get list of permissions for application/device binded to
   * connection key from request and send response
   * @param connection_key Connection key for specific application or 0 for all
   * currently registered applications
   * @param correlation_id Correlation id from request
   */
  void OnGetListOfPermissions(const uint32_t connection_key,
                              const uint32_t correlation_id);

  /**
   * @brief Get current policy table update state and send response
   * @param correlation_id Correlation id from request
   */
  void OnGetStatusUpdate(const uint32_t correlation_id);

  /**
   * @brief Send notification to HMI with changed policy update status
   * @param status Current policy update state
   */
  void OnUpdateStatusChanged(const std::string &status);

  /**
   * @brief Update currently used device id in policies manager for given
   * application
   * @param policy_app_id Application id
   */
  std::string OnCurrentDeviceIdUpdateRequired(const std::string &policy_app_id);

  /**
   * @brief Set parameters from OnSystemInfoChanged to policy table
   * @param language System language
   */
  void OnSystemInfoChanged(const std::string &language);

  /**
   * @brief Save data from GetSystemInfo request to policy table
   * @param ccpu_version CCPU version
   * @param wers_country_code WERS country code
   * @param language System language
   */
  void OnGetSystemInfo(const std::string &ccpu_version,
                       const std::string &wers_country_code,
                       const std::string &language);

  /**
   * @brief Send request to HMI to get update on system parameters
   */
  void OnSystemInfoUpdateRequired() OVERRIDE;

  /**
   * @brief Sends GetVehicleData request in case when Vechicle info is ready.
   */
  virtual void OnVIIsReady();

  /**
   * @brief Allows to update vechicle data info.
   * @param SmartObject which contains all needed information.
   */
  virtual void OnVehicleDataUpdated(const smart_objects::SmartObject &message);

  /**
   * Removes device
   * @param device_id id of device
   */
  void RemoveDevice(const std::string &device_id);

  /**
   * Adds statistics info
   * @param type type of info
   */
  void AddStatisticsInfo(int type);

  /**
   * Handles system error
   * @param code code of error
   */
  void OnSystemError(int code);

  /**
   * @brief Choose application id to be used for snapshot sending
   * @return Application id or 0, if there are no applications registered
   */
  uint32_t GetAppIdForSending();

  std::string GetAppName(const std::string &policy_app_id);

  void
  OnUpdateHMIAppType(std::map<std::string, StringArray> app_hmi_types) OVERRIDE;

  void OnCertificateUpdated(const std::string &certificate_data) OVERRIDE;

  bool CanUpdate() OVERRIDE;

  void OnDeviceConsentChanged(const std::string &device_id,
                              bool is_allowed) OVERRIDE;

  virtual void OnPTExchangeNeeded();

  void GetAvailableApps(std::queue<std::string> &apps) OVERRIDE;

  /**
   * @brief Allows to add new or update existed application during
   * registration process
   * @param application_id The policy aplication id.
   */
  void AddApplication(const std::string &application_id);

  /**
   * Checks whether application is revoked
   * @param app_id id application
   * @return true if application is revoked
   */
  bool IsApplicationRevoked(const std::string &app_id);

  /**
   * @brief Notifies policy manager, that PTS was sent out
   */
  void OnUpdateRequestSentToMobile();

  /**
   * Returns heart beat timeout
   * @param app_id application id
   * @return if timeout was set then value in seconds greater zero
   * otherwise heart beat for specific application isn't set
   */
  uint16_t HeartBeatTimeout(const std::string &app_id) const;

  /**
   * @brief Returns URL for querying list of remote apps
   */
  const std::string RemoteAppsUrl() const;

  /**
   * @brief Handler on applications search started
   */
  void OnAppsSearchStarted();

  /**
   * @brief Handler on applications search completed
   */
  void OnAppsSearchCompleted();

  /**
   * @brief OnAppRegisteredOnMobile alows to handle event when application were
   * succesfully registered on mobile device.
   * It will send OnAppPermissionSend notification and will try to start PTU.
   *
   * @param application_id registered application.
   */
  void OnAppRegisteredOnMobile(const std::string &application_id);

  /**
   * @brief Checks if certain request type is allowed for application
   * @param policy_app_id Unique applicaion id
   * @param type Request type
   * @return true, if allowed, otherwise - false
   */
  bool IsRequestTypeAllowed(const std::string &policy_app_id,
                            mobile_apis::RequestType::eType type) const;

  /**
   * @brief Gets application request types
   * @param policy_app_id Unique application id
   * @return request types
   */
  const std::vector<std::string>
  GetAppRequestTypes(const std::string &policy_app_id) const;

  // TODO(AKutsan) REMOVE THIS UGLY HOTFIX
  virtual void Increment(usage_statistics::GlobalCounterId type);
  virtual void Increment(const std::string &app_id,
                         usage_statistics::AppCounterId type);
  virtual void Set(const std::string &app_id, usage_statistics::AppInfoId type,
                   const std::string &value);
  virtual void Add(const std::string &app_id,
                   usage_statistics::AppStopwatchId type,
                   int32_t timespan_seconds);

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
  bool CheckKeepContext(const std::string &policy_app_id);

  /**
   * Checks system action of application for permission of steal focus
   * @param system_action system action (see mobile api)
   * @param policy_app_id unique application id
   * @return false if system_action is STEAL_FOCUS and it isn't allowed by
   * policy
   * otherwise true
   */
  bool CheckStealFocus(const std::string &policy_app_id);

  /**
   * @brief OnAppPermissionConsentInternal reacts on permission changing
   *
   * @param connection_key connection key
   *
   * @param permissions new permissions.
   */
  void OnAppPermissionConsentInternal(const uint32_t connection_key,
                                      PermissionConsent &permissions);

  /**
   * @brief Sets days after epoch on successful policy update
   */
  void SetDaysAfterEpoch();

private:
  class StatisticManagerImpl : public usage_statistics::StatisticsManager {
    // TODO(AKutsan) REMOVE THIS UGLY HOTFIX
    virtual void Increment(usage_statistics::GlobalCounterId type) {

      PolicyHandler::instance()->AsyncRun(new StatisticsDelegate(type));
    }

    virtual void Increment(const std::string &app_id,
                           usage_statistics::AppCounterId type) {

      PolicyHandler::instance()->AsyncRun(new StatisticsDelegate(app_id, type));
    }

    virtual void Set(const std::string &app_id,
                     usage_statistics::AppInfoId type,
                     const std::string &value) {

      PolicyHandler::instance()->AsyncRun(
          new StatisticsDelegate(app_id, type, value));
    }

    virtual void Add(const std::string &app_id,
                     usage_statistics::AppStopwatchId type,
                     int32_t timespan_seconds) {

      PolicyHandler::instance()->AsyncRun(
          new StatisticsDelegate(app_id, type, timespan_seconds));
    }
  };
  // TODO(AKutsan) REMOVE THIS UGLY HOTFIX

  PolicyHandler();
  bool SaveSnapshot(const BinaryMessage &pt_string, std::string &snap_path);
  static PolicyHandler *instance_;
  static const std::string kLibrary;
  mutable sync_primitives::RWLock policy_manager_lock_;
  utils::SharedPtr<PolicyManager> policy_manager_;
  void *dl_handle_;
  AppIds last_used_app_ids_;
  utils::SharedPtr<PolicyEventObserver> event_observer_;
  uint32_t last_activated_app_id_;

  /**
   * @brief Contains device handles, which were sent for user consent to HMI
   */
  DeviceHandles pending_device_handles_;

  inline bool CreateManager();

  typedef std::list<PolicyHandlerObserver *> HandlersCollection;
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

  friend class AppPermissionDelegate;

  DISALLOW_COPY_AND_ASSIGN(PolicyHandler);
  FRIEND_BASE_SINGLETON_CLASS_WITH_DELETER(
      PolicyHandler, utils::deleters::Deleter<PolicyHandler>);
  FRIEND_DELETER_DESTRUCTOR(PolicyHandler);
};

} //  namespace policy

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POLICY_HANDLER_H_
