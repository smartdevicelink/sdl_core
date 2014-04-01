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
#include "utils/lock.h"
#include "utils/threads/thread.h"
#include "utils/singleton.h"

namespace Json {
class Value;
}

namespace policy {

typedef std::vector<uint32_t> AppIds;
typedef std::vector<uint32_t> DeviceHandles;

class PolicyHandler : public utils::Singleton<PolicyHandler>,
    public PolicyListener {
 public:
  virtual ~PolicyHandler();
  PolicyManager* LoadPolicyLibrary();
  PolicyManager* LoadPolicyLibrary(const std::string& path);
  PolicyManager* policy_manager() const {
    return policy_manager_;
  }
  bool InitPolicyTable();
  bool RevertPolicyTable();
  bool SendMessageToSDK(const BinaryMessage& pt_string);
  bool ReceiveMessageFromSDK(const BinaryMessage& pt_string);
  bool UnloadPolicyLibrary();
  void OnPTExchangeNeeded();
  void OnPermissionsUpdated(const std::string& policy_app_id,
                            const Permissions& permissions);
  /**
   * @brief Checks, if policy update is necessary for application
   */
  void CheckAppPolicyState(const std::string& application_id);

  /**
   * Starts proccess updating policy table
   */
  void StartPTExchange(bool skip_device_selection = false);

  /**
   * Lets client to notify PolicyHandler that more kilometers expired
   * @param kms New value of odometer
   */
  void KmsChanged(int kms);

  /**
   * @brief Gather information for application and sends it to HMI
   * @param connection_key Connection key for application
   */
  void OnActivateApp(const std::string& connection_key,
                     uint32_t correlation_id);

  /**
   * @brief Process user consent on mobile data connection access
   * @param Device id or 0, if concern to all SDL functionality
   * @param User consent from response
   */
  void OnAllowSDLFunctionalityNotification(bool is_allowed, uint32_t device_id =
                                               0);
  /**
   * @brief Send request to HMI for requesting the allowance for some
   * application functionality
   * @param List of permissions required by application
   * @param Unique appication id, if omitted - allow/disallow all applications
   */
  void SendAllowApp(const PermissionsList& list_of_permissions,
                    uint32_t application_id = 0);

  /**
   * @brief Process application permission, which was set by user
   * @param List of user-defined permissions for application
   */
  void OnAllowAppResponse(PermissionsList& list_of_permissions);

  /**
   * @brief Process application permission, which was set by user
   * @param List of user-defined permissions for application
   * @param Unique application id, if omitted - allow/disallow all
   * applications
   */
  void OnAllowAppNotification(PermissionsList& list_of_permissions,
                              uint32_t appication_id = 0);

  /**
   * @brief Increment counter for ignition cycles
   */
  void OnIgnitionCycleOver();

  /**
   * @brief Send notification to HMI concerning revocation of application
   * @param policy_app_id Unique identifier of application
   */
  void OnAppRevoked(const std::string& policy_app_id);

  /**
   * Initializes PT exchange at ignition if need
   */
  void PTExchangeAtIgnition();

  /**
   * @brief Save device info for specific device to policy table
   * @param device_id Device mac address
   * @param device_info Device params
   */
  void SetDeviceInfo(std::string& device_id, const DeviceInfo& device_info);

  /**
   * @brief Store user-changed permissions consent to DB
   * @param permissions User-changed group permissions consent
   */
  void OnAppPermissionConsent(const PermissionConsent& permissions);

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

 private:
  /**
   * @brief Choose application id to be used for snapshot sending
   * @return Application id or 0, if there are no applications registered
   */
  uint32_t GetAppIdForSending();

  /**
   * @brief Choose device according to app HMI status and user consent for
   * device
   * @param device_info Struct with selected device parameters
   * @return consent status for selected device
   */
   DeviceConsent GetDeviceForSending(DeviceParams& device_params);

 private:
  PolicyHandler();
  static PolicyHandler* instance_;
  static const std::string kLibrary;
  PolicyManager* policy_manager_;
  void* dl_handle_;
  AppIds last_used_app_ids_;
  static log4cxx::LoggerPtr logger_;
  threads::Thread retry_sequence_;
  sync_primitives::Lock retry_sequence_lock_;
  PTExchangeHandler* exchange_handler_;
  utils::SharedPtr<PolicyEventObserver> event_observer_;

  /**
   * @brief Contains device handles, which were sent for user consent to HMI
   */
  DeviceHandles pending_device_handles_;

  /**
   * @brief True, if PTS was sent, but PTU was not reseived yet,
   * otherwise - false
   * Used for limiting device consent request per PTS/PTU session
   */
  bool is_exchange_in_progress_;

  inline PolicyManager* CreateManager();

    DISALLOW_COPY_AND_ASSIGN(PolicyHandler); FRIEND_BASE_SINGLETON_CLASS(PolicyHandler);
    friend class RetrySequence;
};

}  //  namespace policy

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_POLICY_HANDLER_H_
