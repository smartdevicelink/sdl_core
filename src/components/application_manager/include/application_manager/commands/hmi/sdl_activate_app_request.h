/*
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_SDL_ACTIVATE_APP_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_SDL_ACTIVATE_APP_REQUEST_H_

#include "application_manager/commands/hmi/request_from_hmi.h"
#include "application_manager/application_manager.h"

namespace application_manager {

namespace commands {

/**
 * @brief SDLActivateAppRequest command class
 **/
class SDLActivateAppRequest : public RequestFromHMI {
 public:
  /**
   * @brief Applications registered over protocol v4
   */
  typedef std::vector<application_manager::ApplicationSharedPtr> V4ProtoApps;

  /**
   * @brief SDLActivateAppRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  SDLActivateAppRequest(const MessageSharedPtr& message,
                        ApplicationManager& application_manager);

  /**
   * @brief SDLActivateAppRequest class destructor
   **/
  ~SDLActivateAppRequest() OVERRIDE;

  /**
   * @brief Execute command
   **/
  void Run() OVERRIDE;

  /**
   * @brief onTimeOut allows to process case when timeout has appeared
   * during request execution.
   */
  void onTimeOut() OVERRIDE;

  /**
   * @brief on_event allows to handle events
   *
   * @param event event type that current request subscribed on.
   */
  void on_event(const event_engine::Event& event) OVERRIDE;

 private:
  uint32_t app_id() const;
  uint32_t hmi_app_id(const smart_objects::SmartObject& so) const;

  /**
   * @brief Retrieves all v4 protocol applications for particular device
   * @param handle Device handle
   * @return List of applications registered over v4 protocol
   */
  V4ProtoApps get_v4_proto_apps(
      const connection_handler::DeviceHandle handle) const;

  /**
   * @brief Get v4 protocol application reported as forgrounded on device
   * @param handle Device
   * @return Pointer to application or empty pointer
   */
  ApplicationSharedPtr get_foreground_app(
      const connection_handler::DeviceHandle handle) const;

  DISALLOW_COPY_AND_ASSIGN(SDLActivateAppRequest);
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_SDL_ACTIVATE_APP_REQUEST_H_
