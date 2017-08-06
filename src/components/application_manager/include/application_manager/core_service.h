/**
 * Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_CORE_SERVICE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_CORE_SERVICE_H_

#include <string>
#include <vector>
#include "application_manager/service.h"
#include "application_manager/application.h"
#include "policy/policy_types.h"

namespace Json {
class Value;
}

namespace application_manager {

struct CommandParametersPermissions;

/**
 * @brief Class through which the plug-in can interact with the core
 */
class CoreService : public Service {
 public:
  /**
   * @brief CoreService class destructor
   */
  explicit CoreService(ApplicationManager& application_manager);

  /**
   * @brief CoreService class destructor
   */
  virtual ~CoreService();

  /**
   * @brief Checks message permissions and cuts parameters according
   * to policy table permissions
   * @param msg message to cut disallowed parameters
   * @return result according by mobile API
   */
  mobile_apis::Result::eType CheckPolicyPermissions(MessagePtr msg) FINAL;

  /**
   * Checks access to requested equipment of vehicle
   * @param app_id id of application
   * @param module type
   * @param rpc name of rpc
   * @param params parameters list
   * @return return allowed if access exist,
   * manual if need to send question to driver otherwise disallowed
   */
  TypeAccess CheckAccess(const ApplicationId& app_id,
                         const std::string& module,
                         const std::string& rpc,
                         const std::vector<std::string>& params) FINAL;

  /**
   * Checks access to module for application
   * @param app_id id of application
   * @param module type
   * @return true if module is allowed for application
   */
  bool CheckModule(const ApplicationId& app_id,
                   const std::string& module) FINAL;

  /**
   * Sets access to functional group which contains given RPC for application
   * @param app_id id of application
   * @param module type
   * @param allowed true if driver has given access
   */
  void SetAccess(const ApplicationId& app_id,
                 const std::string& module,
                 bool allowed) FINAL;

  /**
   * Resets access by group name for all applications
   * @param group_name group name
   */
  void ResetAccess(const ApplicationId& app_id) FINAL;

  /**
   * Resets access by module type for all applications
   * @param module type
   */
  void ResetAccess(const std::string& module) FINAL;

  /**
   * Gets device handler for device with certain ID
   * @param device_id the ID of the connected device
   * @return device handler if device with requested ID was found
   */
  uint32_t GetDeviceHandlerById(const std::string& device_id) FINAL;

  /**
   * Sets device as primary device
   * @param dev_id ID device
   */
  void SetPrimaryDevice(const uint32_t dev_id) FINAL;

  /**
   * Resets driver's device
   */
  void ResetPrimaryDevice() FINAL;

  /**
   * Return id of primary device
   */
  uint32_t PrimaryDevice() const FINAL;

  /**
   * Sets mode of remote control (on/off)
   * @param enabled true if remote control is turned on
   */
  void SetRemoteControl(bool enabled) FINAL;

  /**
   * @brief Is Remote Control allowed by Policy and User
   */
  bool IsRemoteControlAllowed() const FINAL;

  /**
   * Checks if application has remote control functions
   * @param app application
   * @return true if application has remote control functions
   */
  bool IsRemoteControlApplication(ApplicationSharedPtr app) const FINAL;

  /**
   * Removes fake parameters from request to HMI
   * @param message message to handle
   */
  void RemoveHMIFakeParameters(application_manager::MessagePtr& message) FINAL;

  /**
   * @brief Get pointer to application by application id
   * @param app_id application id
   * return pointer to application
   */
  ApplicationSharedPtr GetApplication(ApplicationId app_id) FINAL;

  /**
   * @brief Send message to HMI
   * @param message Message to HMI
   */
  void SendMessageToHMI(const MessagePtr& message) FINAL;

  /**
   * @brief Send message to mobile device
   * @param message Message to mobile
   */
  void SendMessageToMobile(const MessagePtr& message) FINAL;

  /**
   * @brief Returns unique correlation ID for next HMI request
   *
   * @return Unique correlation ID
   */
  uint32_t GetNextCorrelationID() FINAL;

  /**
   * @brief Returns all applications
   *
   * @return List with shared pointers to applications
   */
  std::vector<ApplicationSharedPtr> GetApplications(AppExtensionUID uid) FINAL;

  /**
   * @brief Subscribes to notification from HMI
   * @param hmi_notification string with notification name
   */
  void SubscribeToHMINotification(const std::string& hmi_notification) FINAL;

  /**
   * @brief Change hmi level of app and notify it
   * @param app Application to be changed and notified
   * @param level New HMI level of app
   */
  void ChangeNotifyHMILevel(ApplicationSharedPtr app,
                            mobile_apis::HMILevel::eType level) FINAL;

  const smart_objects::SmartObject* GetRCCapabilities() const FINAL;
  /**
   * @brief Notify HMI about app changing HMI Level
   * only NONE, BACKGROUND and LIMITED levels are sent
   * @param app Application to be changed and notified
   * @param level New HMI level of app
   */
  void NotifyHMIAboutHMILevel(ApplicationSharedPtr app,
                              mobile_apis::HMILevel::eType level) FINAL;

  /**
   * Gets all allowed module types
   * @param app_id unique identifier of application
   * @param list of allowed module types
   * @return true if application has allowed modules
   */
  bool GetModuleTypes(const std::string& policy_app_id,
                      std::vector<std::string>* modules) const FINAL;

  MessageValidationResult ValidateMessageBySchema(
      const Message& message) OVERRIDE;

 private:
  bool AreParametersAllowed(MessagePtr msg,
                            const CommandParametersPermissions& params);
  bool CheckParams(const Json::Value& object,
                   const policy::RPCParams& allowed_params);
  bool IsAllowed(const std::string& name,
                 const policy::RPCParams& allowed_params);

  ApplicationManager& application_manager_;

  DISALLOW_COPY_AND_ASSIGN(CoreService);
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_CORE_SERVICE_H_
