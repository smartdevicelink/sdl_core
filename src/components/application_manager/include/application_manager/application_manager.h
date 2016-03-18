/*
 * Copyright (c) 2015, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_H_

#include <string>
#include <vector>
#include <set>

#include "application_manager/application.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/commands/command.h"

#include "utils/data_accessor.h"
#include "utils/shared_ptr.h"
#include "telemetry_monitor/telemetry_observable.h"

// Other compomnents class declaration
namespace hmi_message_handler {
class HMIMessageHandler;
}
namespace protocol_handler {
class ProtocolHandler;
}
namespace connection_handler {
class ConnectionHandler;
}

namespace application_manager {

struct ApplicationsAppIdSorter {
  bool operator()(const ApplicationSharedPtr lhs,
                  const ApplicationSharedPtr rhs) const {
    return lhs->app_id() < rhs->app_id();
  }
};
typedef std::set<ApplicationSharedPtr, ApplicationsAppIdSorter> ApplicationSet;

// typedef for Applications list iterator
typedef ApplicationSet::iterator ApplicationSetIt;

// typedef for Applications list const iterator
typedef ApplicationSet::const_iterator ApplicationSetConstIt;

class ApplicationManager {
 public:
  virtual ~ApplicationManager() {}

  /**
   * Inits application manager
   */
  virtual bool Init() = 0;

  /**
   * @brief Stop work.
   *
   * @return TRUE on success otherwise FALSE.
   **/
  virtual bool Stop() = 0;

  virtual void set_hmi_message_handler(
      hmi_message_handler::HMIMessageHandler* handler) = 0;
  virtual void set_protocol_handler(
      protocol_handler::ProtocolHandler* handler) = 0;
  virtual void set_connection_handler(
      connection_handler::ConnectionHandler* handler) = 0;

  virtual DataAccessor<ApplicationSet> applications() const = 0;

  virtual ApplicationSharedPtr application(uint32_t app_id) const = 0;
  virtual ApplicationSharedPtr active_application() const = 0;
  //

  /**
   * Function used only by HMI request/response/notification base classes
   * to change HMI app id to Mobile app id and vice versa.
   * Don't use it inside Core
   */
  virtual ApplicationSharedPtr application_by_hmi_app(
      uint32_t hmi_app_id) const = 0;

  virtual ApplicationSharedPtr application_by_policy_id(
      const std::string& policy_app_id) const = 0;

  virtual std::vector<ApplicationSharedPtr> applications_by_button(
      uint32_t button) = 0;
  virtual std::vector<ApplicationSharedPtr> applications_with_navi() = 0;

  /**
   * @brief Returns media application with LIMITED HMI Level if exists
   *
   * @return Shared pointer to application if application does not
   * exist returns empty shared pointer.
   */
  virtual ApplicationSharedPtr get_limited_media_application() const = 0;

  /**
   * @brief Returns navigation application with LIMITED HMI Level if exists
   *
   * @return Shared pointer to application if application does not
   * exist returns empty shared pointer
   */
  virtual ApplicationSharedPtr get_limited_navi_application() const = 0;

  /**
   * @brief Returns voice communication application with
   * LIMITED HMI Level if exists
   *
   * @return Shared pointer to application if application does not
   * exist returns empty shared pointer
   */
  virtual ApplicationSharedPtr get_limited_voice_application() const = 0;

  /**
   * @brief Retrieves application id associated with correlation id
   *
   * @param correlation_id Correlation ID of the HMI request
   *
   * @return application id associated with correlation id
   */
  virtual const uint32_t application_id(const int32_t correlation_id) = 0;

  /**
   * @brief Sets application id correlation id
   *
   * @param correlation_id Correlation ID of the HMI request
   * @param app_id Application ID
   */
  virtual void set_application_id(const int32_t correlation_id,
                                  const uint32_t app_id) = 0;

  /**
   * @brief OnHMILevelChanged the callback that allows SDL to react when
   * application's HMI level has been changed.
   *
   * @param app_id application identifier for which HMILevel has been chaned.
   *
   * @param from previous HMILevel.
   * @param to current HMILevel.
   */
  virtual void OnHMILevelChanged(uint32_t app_id,
                                 mobile_apis::HMILevel::eType from,
                                 mobile_apis::HMILevel::eType to) = 0;

  /**
   * @brief Sends HMI status notification to mobile
   *
   * @param application_impl application with changed HMI status
   *
   **/
  virtual void SendHMIStatusNotification(
      const utils::SharedPtr<Application> app) = 0;

  virtual void SendMessageToMobile(const commands::MessageSharedPtr message,
                                   bool final_message = false) = 0;

  virtual void SendMessageToHMI(const commands::MessageSharedPtr message) = 0;

  virtual bool ManageHMICommand(const commands::MessageSharedPtr message) = 0;
  virtual bool ManageMobileCommand(const commands::MessageSharedPtr message,
                                   commands::Command::CommandOrigin origin) = 0;


  virtual mobile_api::HMILevel::eType GetDefaultHmiLevel(
      ApplicationConstSharedPtr application) const = 0;
  /**
   * @brief hmi_capabilities return capabilities of hmi
   * @return capabilities of hmi
   */
  virtual HMICapabilities& hmi_capabilities() = 0;

  virtual bool is_attenuated_supported() = 0;

  /**
   * @brief Checks if application with the same HMI type
   *        (media, voice communication or navi) exists
   *        in HMI_FULL or HMI_LIMITED level.
   *
   * @param app Pointer to application to compare with
   *
   * @return true if exist otherwise false
   */
  virtual bool IsAppTypeExistsInFullOrLimited(
      ApplicationConstSharedPtr app) const = 0;

  /**
   * @brief Sets default HMI level and configure application after its
   * registration
   * @param app Application
   */
  virtual void OnApplicationRegistered(ApplicationSharedPtr app) = 0;

  virtual connection_handler::ConnectionHandler& connection_handler() const = 0;
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_MANAGER_H_
