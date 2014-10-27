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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POLICY_HANDLER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POLICY_HANDLER_H_

#include <string>
#include <map>
#include <set>
#include <vector>
#include "policy/policy_manager.h"
#include "application_manager/policies/policy_event_observer.h"
#include "application_manager/policies/pt_exchange_handler.h"
#include "utils/logger.h"
#include "utils/singleton.h"
#include "usage_statistics/statistics_manager.h"

namespace Json {
class Value;
}

namespace policy {
typedef std::vector<uint32_t> AppIds;
typedef std::vector<uint32_t> DeviceHandles;

class PolicyHandler :
    public utils::Singleton<PolicyHandler, utils::deleters::Deleter<PolicyHandler> >,
    public PolicyListener {
 public:
  virtual ~PolicyHandler();
  bool LoadPolicyLibrary();
  bool PolicyEnabled();
  bool InitPolicyTable();
  bool ResetPolicyTable();
  bool ClearUserConsent();
  bool SendMessageToSDK(const BinaryMessage& pt_string);
  bool ReceiveMessageFromSDK(const std::string& file,
                             const BinaryMessage& pt_string);
  bool UnloadPolicyLibrary();
  void OnPTExchangeNeeded();
  void OnPermissionsUpdated(const std::string& policy_app_id,
                            const Permissions& permissions,
                            const HMILevel& default_hmi);

  bool GetPriority(const std::string& policy_app_id, std::string* priority);
  void CheckPermissions(const PTString& app_id,
                   const PTString& hmi_level,
                   const PTString& rpc,
                   const RPCParams& rpc_params,
                   CheckPermissionResult& result);

  uint32_t GetNotificationsNumber(const std::string& priority);
  DeviceConsent GetUserConsentForDevice(const std::string& device_id);
  bool GetDefaultHmi(const std::string& policy_app_id,
                             std::string* default_hmi);
  bool GetInitialAppData(const std::string& application_id,
                                 StringArray* nicknames = NULL,
                                 StringArray* app_hmi_types = NULL);
  EndpointUrls GetUpdateUrls(int service_type);
  void ResetRetrySequence();
  int NextRetryTimeout();
  int TimeoutExchange();
  void OnExceededTimeout();
  BinaryMessageSptr RequestPTUpdate();
  const std::vector<int> RetrySequenceDelaysSeconds();

  utils::SharedPtr<usage_statistics::StatisticsManager> GetStatisticManager();

  /**
   * Checks system action of application for permission of keep context
   * @param system_action system action (see mobile api)
   * @param policy_app_id unique application id
   * @return false if system_action is KEEP_CONTEXT and it isn't allowed by policy
   * otherwise true
   */
  bool CheckKeepContext(int system_action, const std::string& policy_app_id);

  /**
   * Checks system action of application for permission of steal focus
   * @param system_action system action (see mobile api)
   * @param policy_app_id unique application id
   * @return false if system_action is STEAL_FOCUS and it isn't allowed by policy
   * otherwise true
   */
  bool CheckStealFocus(int system_action, const std::string& policy_app_id);

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

  /**
   * @brief Send notification to HMI concerning revocation of application
   * @param policy_app_id Unique identifier of application
   */
  void OnAppRevoked(const std::string& policy_app_id);

  void OnPendingPermissionChange(const std::string& policy_app_id);

  /**
   * Initializes PT exchange at ignition if need
   */
  void PTExchangeAtRegistration(const std::string& app_id);

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
  void SetDeviceInfo(std::string& device_id, const DeviceInfo& device_info);

  /**
   * @brief Store user-changed permissions consent to DB
   * @param connection_key Connection key of application or 0, if permissions
   * should be applied to all applications
   * @param permissions User-changed group permissions consent
   */
  void OnAppPermissionConsent(const uint32_t connection_key,
                              PermissionConsent& permissions);

  /**
   * @brief Get appropriate message parameters and send them with response
   * to HMI
   * @param message_codes RPC message codes
   * @param language Language
   * @param correlation_id correlation id of request
   */
  void OnGetUserFriendlyMessage(const std::vector<std::string>& message_codes,
                                const std::string& language,
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
  void OnUpdateStatusChanged(policy::PolicyTableStatus status);

  /**
   * @brief Update currently used device id in policies manager for given
   * application
   * @param policy_app_id Application id
   */
    std::string OnCurrentDeviceIdUpdateRequired(const std::string& policy_app_id);

  /**
   * @brief Set parameters from OnSystemInfoChanged to policy table
   * @param language System language
   */
  void OnSystemInfoChanged(const std::string& language);

  /**
   * @brief Save data from GetSystemInfo request to policy table
   * @param ccpu_version CCPU version
   * @param wers_country_code WERS country code
   * @param language System language
   */
  void OnGetSystemInfo(const std::string& ccpu_version,
                       const std::string& wers_country_code,
                       const std::string& language);

  /**
   * @brief Send request to HMI to get update on system parameters
   */
  virtual void OnSystemInfoUpdateRequired();

  /**
   * @brief Sends GetVehicleData request in case when Vechicle info is ready.
   */
  virtual void OnVIIsReady();

  /**
   * @brief Allows to update vechicle data info.
   * @param SmartObject which contains all needed information.
   */
  virtual void OnVehicleDataUpdated(const smart_objects::SmartObject& message);

  /**
   * Removes device
   * @param device_id id of device
   */
  void RemoveDevice(const std::string& device_id);

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

  std::string GetAppName(const std::string& policy_app_id);

  virtual void OnUserRequestedUpdateCheckRequired();

  virtual void OnDeviceConsentChanged(const std::string& device_id,
                                      bool is_allowed);

  /**
   * @brief Allows to add new or update existed application during
   * registration process
   * @param application_id The policy aplication id.
   */
  void AddApplication(const std::string& application_id);

  /**
   * Checks whether application is revoked
   * @param app_id id application
   * @return true if application is revoked
   */
  bool IsApplicationRevoked(const std::string& app_id);

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
  uint16_t HeartBeatTimeout(const std::string& app_id) const;

//TODO(AKutsan) REMOVE THIS UGLY HOTFIX
  virtual void Increment(usage_statistics::GlobalCounterId type);
  virtual void Increment(const std::string& app_id,
                         usage_statistics::AppCounterId type);
  virtual void Set(const std::string& app_id,
                   usage_statistics::AppInfoId type,
                   const std::string& value);
  virtual void Add(const std::string& app_id,
                   usage_statistics::AppStopwatchId type,
                   int32_t timespan_seconds);


protected:

  /**
   * Starts next retry exchange policy table
   */
  void StartNextRetry();

  /**
   * Initializes PT exchange at odometer if need
   * @param kilometers value from odometer in kilometers
   */
  void PTExchangeAtOdometer(int kilometers);

  /**
     * Starts proccess updating policy table
   */
    void StartPTExchange(bool skip_device_selection = false);

 private:
  /**
   * @brief Choose device according to app HMI status and user consent for
   * device
   * @param device_info Struct with selected device parameters
   * @return consent status for selected device
   */
  DeviceConsent GetDeviceForSending(DeviceParams& device_params);

  /**
   * @brief Convert internal policy update status to appropriate status for HMI
   * @param status Internal policy update status
   * @return Converted status for sending to HMI
   */
  const std::string ConvertUpdateStatus(policy::PolicyTableStatus status);

private:

  class StatisticManagerImpl: public usage_statistics::StatisticsManager {
      //TODO(AKutsan) REMOVE THIS UGLY HOTFIX
        virtual void Increment(usage_statistics::GlobalCounterId type) {
        return PolicyHandler::instance()->Increment(type);
      }

        virtual void Increment(const std::string& app_id,
                               usage_statistics::AppCounterId type) {
        return PolicyHandler::instance()->Increment(app_id, type);
      }

        virtual void Set(const std::string& app_id,
                         usage_statistics::AppInfoId type,
                         const std::string& value) {
        return PolicyHandler::instance()->Set(app_id, type, value);
      }

        virtual void Add(const std::string& app_id,
                         usage_statistics::AppStopwatchId type,
                         int32_t timespan_seconds) {
        return PolicyHandler::instance()->Add(app_id, type, timespan_seconds);
      }
  };
  //TODO(AKutsan) REMOVE THIS UGLY HOTFIX


  PolicyHandler();
  static PolicyHandler* instance_;
  static const std::string kLibrary;
  utils::SharedPtr<PolicyManager> policy_manager_;
  void* dl_handle_;
  AppIds last_used_app_ids_;
  utils::SharedPtr<PTExchangeHandler> exchange_handler_;
  utils::SharedPtr<PolicyEventObserver> event_observer_;
  bool on_ignition_check_done_;
  uint32_t last_activated_app_id_;
  bool registration_in_progress;

  /**
   * @brief Contains device handles, which were sent for user consent to HMI
   */
  DeviceHandles pending_device_handles_;

  inline bool CreateManager();

  bool is_user_requested_policy_table_update_;

  /**
   * @brief Application-to-device map is used for getting/setting user consents
   * for all apps
   */
  std::map<std::string, std::string> app_to_device_link_;


  utils::SharedPtr<StatisticManagerImpl> statistic_manager_impl_;


  DISALLOW_COPY_AND_ASSIGN(PolicyHandler);
  FRIEND_BASE_SINGLETON_CLASS_WITH_DELETER(PolicyHandler,
                                           utils::deleters::Deleter<PolicyHandler>);
  FRIEND_DELETER_DESTRUCTOR(PolicyHandler);
};

}  //  namespace policy

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POLICY_HANDLER_H_
