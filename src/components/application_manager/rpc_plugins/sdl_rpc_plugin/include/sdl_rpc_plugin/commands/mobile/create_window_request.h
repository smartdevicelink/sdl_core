/*
 Copyright (c) 2019, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_CREATE_WINDOW_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_CREATE_WINDOW_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"
#include "application_manager/hmi_state.h"
#include "utils/macro.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {

/**
 * @brief CreateWindow request command class
 **/
class CreateWindowRequest : public app_mngr::commands::CommandRequestImpl {
 public:
  CreateWindowRequest(const app_mngr::commands::MessageSharedPtr& message,
                      app_mngr::ApplicationManager& application_manager,
                      app_mngr::rpc_service::RPCService& rpc_service,
                      app_mngr::HMICapabilities& hmi_capabilities,
                      policy::PolicyHandlerInterface& policy_handler);

  ~CreateWindowRequest() FINAL;

  app_mngr::WindowID window_id() const FINAL;

  void Run() FINAL;

  void on_event(const app_mngr::event_engine::Event& event) FINAL;

  /**
   * @brief Init sets hash update mode for request
   */
  bool Init() FINAL;

 private:
  /**
   * @brief CheckWindowName checks if provided window_name exists and allowed to
   * be created
   * @param app pointer to application owns affected window
   * @param window_name window name to check
   * @return true if window_name is valid, otherwise returns false
   */
  bool CheckWindowName(app_mngr::ApplicationSharedPtr app,
                       const app_mngr::WindowID window_id,
                       const std::string& window_name) const;

  /**
   * @brief ValidateWindowCreation checks whether window can be created
   * @return true if window can be created, otherwise returns false
   */
  bool ValidateWindowCreation(app_mngr::ApplicationSharedPtr app,
                              const app_mngr::WindowID window_id);

  /**
   * @brief IsWindowForAssociatedServiceCreated check whether a window with
   * current associated type has already been created
   * @return true if window has been created, otherwise returns false
   */
  bool IsWindowForAssociatedServiceCreated(
      app_mngr::ApplicationSharedPtr app) const;

  /**
   * @brief DoesExceedMaxAllowedWindows check wheter max allowed amount of
   * windows is exceeded
   * @return true if amount us exceeded, otherwise returns false
   */
  bool DoesExceedMaxAllowedWindows(app_mngr::ApplicationSharedPtr app) const;

  /**
   * @brief ApplyWindowInitialState apply changes related to window HMI state
   * initialization
   * @param app pointer to application owns affected window
   */
  void ApplyWindowInitialState(app_mngr::ApplicationSharedPtr app) const;

  DISALLOW_COPY_AND_ASSIGN(CreateWindowRequest);
};

}  // namespace commands

}  // namespace sdl_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_CREATE_WINDOW_REQUEST_H_
