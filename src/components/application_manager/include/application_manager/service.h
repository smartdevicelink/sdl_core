/*
 * Copyright (c) 2017, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SERVICE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SERVICE_H_

#include <string>
#include <sstream>
#include <vector>
#include "application_manager/application.h"
#include "application_manager/application_manager_settings.h"
#include "application_manager/message.h"
#include "application_manager/hmi_interfaces.h"
#include "application_manager/hmi_capabilities.h"

namespace application_manager {

enum TypeAccess { kDisallowed, kAllowed };

enum MessageValidationResult {
  SUCCESS = 0,
  INVALID_JSON,
  INVALID_METADATA,
  SCHEMA_MISMATCH,
  UNSUPPORTED_PROTOCOL
};

typedef std::string PluginFunctionID;

/**
 * @brief Interface to core service
 */
class Service {
 public:
  virtual ~Service() {}

  /**
   * @brief Checks message permissions and cuts parameters according
   * to policy table permissions
   * @param msg message to cut disallowed parameters
   * @return result according by mobile API
   */
  virtual mobile_apis::Result::eType CheckPolicyPermissions(MessagePtr msg) = 0;

  /**
   * Checks if module for application is present in policy table
   * @param app_id id of application
   * @param module type
   * @return true if module is present, otherwise - false
   */
  virtual bool CheckModule(const ApplicationId& app_id,
                           const std::string& module) = 0;

  /**
   * @brief Get pointer to application by application id
   * @param app_id application id
   * return pointer to application
   */
  virtual ApplicationSharedPtr GetApplication(ApplicationId app_id) = 0;

  /**
   * Removes parameters not listed in HMI API from HMI request
   * @param message message to handle
   */
  virtual void RemoveHMIFakeParameters(
      application_manager::MessagePtr& message) = 0;
  /**
   * @brief Send message to HMI
   * @param message Message to HMI
   */
  virtual void SendMessageToHMI(const MessagePtr& message) = 0;

  /**
   * @brief Send message to mobile device
   * @param message Message to mobile
   */
  virtual void SendMessageToMobile(const MessagePtr& message) = 0;

  /**
   * @brief Returns unique correlation ID for next HMI request
   *
   * @return Unique correlation ID
   */
  virtual uint32_t GetNextCorrelationID() = 0;

  /**
   * @brief Returns all applications related to plugin
   * @param uid ID provided by plugin to its extension to app.
   * @return List with shared pointers to applications
   */
  virtual std::vector<ApplicationSharedPtr> GetApplications(
      AppExtensionUID uid) = 0;

  /**
   * @brief Change hmi level of app and notify it
   * @param app Application to be changed and notified
   * @param level New HMI level of app
   */
  virtual void ChangeNotifyHMILevel(ApplicationSharedPtr app,
                                    mobile_apis::HMILevel::eType level) = 0;

  /**
   * @brief Notify HMI about app changing HMI Level
   * only NONE, BACKGROUND and LIMITED levels are sent
   * @param app Application to be changed and notified
   * @param level New HMI level of app
   */
  virtual void NotifyHMIAboutHMILevel(ApplicationSharedPtr app,
                                      mobile_apis::HMILevel::eType level) = 0;

  virtual const DataAccessor<application_manager::HMICapabilities> GetHMICapabilities() const = 0;
  /**
   * Checks if application has remote control functions
   * @param app application
   * @return true if application has remote control functions
   */
  virtual bool IsRemoteControlApplication(ApplicationSharedPtr app) const = 0;

  /**
   * @brief Gets current state of the specified interface
   * @param interface which state to get
   * @return true if specified interface available otherwise false
   */
  virtual bool IsInterfaceAvailable(
      const HmiInterfaces::InterfaceID interface) const = 0;

  /**
   * Gets all allowed module types
   * @param app_id unique identifier of application
   * @param list of allowed module types
   * @return true if application has allowed modules
   */
  virtual bool GetModuleTypes(const std::string& policy_app_id,
                              std::vector<std::string>* modules) const = 0;

  /**
   * @brief ValidateMessageBySchema validates message by xml schema
   * @param message message for validation
   * @return true if message is valid according to schema, otherwise false
   */
  virtual MessageValidationResult ValidateMessageBySchema(
      const Message& message) = 0;

  /**
   * @brief Gets application manager settings structure
   * @return reference to application manager settings structure
   */
  virtual const ApplicationManagerSettings& GetSettings() const = 0;
};

typedef utils::SharedPtr<Service> ServicePtr;

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SERVICE_H_
