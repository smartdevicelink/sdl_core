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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SERVICE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SERVICE_H_

#include <string>

#include "application_manager/application.h"
#include "application_manager/message.h"


namespace application_manager {

enum TypeGrant {
  kDisallowed,
  kAllowed,
  kManual
};

/**
 * @brief Interface to core service
 */
class Service {
  public:
  virtual ~Service() {
  }

  /**
   * @brief Checks message permissions and parameters according to policy table
   * permissions
   * @param json string with params in json format
   * @return false if message blocked by policy
   */
  virtual TypeGrant CheckPolicyPermissions(const std::string& json_message) = 0;

  /**
   * @brief Get pointer to application by application id
   * @param app_id application id
   * return pointer to application
   */
  virtual ApplicationSharedPtr GetApplication(ApplicationId app_id) = 0;

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

  /*
   * @brief Returns all applications related to plugin
   * @param uid ID provided by plugin to its extension to app.
   * @return List with shared pointers to applications
   */
  virtual std::vector<ApplicationSharedPtr> GetApplications(AppExtensionUID uid) = 0;

  /**
   * @brief Subscribes to notification from HMI
   * @param hmi_notification string with notification name
   */
  virtual void SubscribeToHMINotification(
      const std::string& hmi_notification) = 0;
};

typedef utils::SharedPtr<Service> ServicePtr;

}  // namespace application_manager


#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SERVICE_H_
