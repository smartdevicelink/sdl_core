/*
 * Copyright (c) 2021, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_BUTTON_NOTIFICATION_TO_MOBILE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_BUTTON_NOTIFICATION_TO_MOBILE_H_

#include "application_manager/application.h"
#include "command_notification_impl.h"
#include "interfaces/MOBILE_API.h"

namespace application_manager {
namespace commands {
namespace app_mngr = application_manager;

/**
 * @brief Class is intended to encapsulate shared button notification logic in
 * base class. Entities defined in this file do not conform to any version of
 * HMI or mobile API, and exist only to remove duplication in OnButtonPress and
 * OnButtonEvent notifications.
 **/
class ButtonNotificationToMobile
    : public app_mngr::commands::CommandNotificationImpl {
 public:
  /**
   * @brief ButtonNotificationToMobile class constructor
   **/
  ButtonNotificationToMobile(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handler);

  /**
   * @brief ButtonNotificationToMobile class destructor
   **/
  ~ButtonNotificationToMobile();

  /**
   * @brief Execute command
   **/
  void Run() OVERRIDE;

 protected:
  virtual void SendButtonNotification(app_mngr::ApplicationSharedPtr app) = 0;

  /**
   * @brief HandleCustomButton handle event for custom buttons
   * @param app pointer to application data.
   **/
  void HandleCustomButton(app_mngr::ApplicationSharedPtr app);

  /**
   * @brief HandleOKButton handle event for OK button
   * @param app pointer to application data.
   **/
  void HandleOKButton(app_mngr::ApplicationSharedPtr app);

  /**
   * @brief HandleMediaButton handle event for media buttons
   * @param app pointer to application data.
   **/
  void HandleMediaButton(app_mngr::ApplicationSharedPtr app);

  /**
   * @brief DoesParamExist check whether param is exists in msg_params
   * @param param_name name of parameter to find
   **/
  bool DoesParamExist(const std::string& param_name) const;

  /**
   * @brief SubscribedApps get subscribed apps for btn id received in message
   * @return Return applications list subscribed to current button
   **/
  std::vector<ApplicationSharedPtr> SubscribedApps() const;
};
}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_BUTTON_NOTIFICATION_TO_MOBILE_H_
