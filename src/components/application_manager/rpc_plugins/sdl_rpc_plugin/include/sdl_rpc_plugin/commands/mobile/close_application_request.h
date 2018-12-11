/*

 Copyright (c) 2018, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_CLOSE_APPLICATION_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_CLOSE_APPLICATION_REQUEST_H_

#include <cstdint>

#include "application_manager/commands/command_request_impl.h"
#include "application_manager/event_engine/event_observer.h"
#include "utils/macro.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {

/**
  * @brief CloseApplicationRequest command class
  */
class CloseApplicationRequest : public app_mngr::commands::CommandRequestImpl {
 public:
  /**
    * @brief CloseApplicationRequest class constructor
    *
    * @param message Incoming SmartObject message
    */
  CloseApplicationRequest(const app_mngr::commands::MessageSharedPtr& message,
                          app_mngr::ApplicationManager& application_manager,
                          app_mngr::rpc_service::RPCService& rpc_service,
                          app_mngr::HMICapabilities& hmi_capabilities,
                          policy::PolicyHandlerInterface& policy_handler);
  /**
    * @brief CloseApplicationRequest class destructor
    */
  ~CloseApplicationRequest() FINAL;

  /**
    * @brief Execute command
    */
  void Run() FINAL;

  /**
    * @brief Interface method that is called whenever new event received
    *
    * @param event The received event
    */
  void on_event(const app_mngr::event_engine::Event& event) FINAL;

 private:
  DISALLOW_COPY_AND_ASSIGN(CloseApplicationRequest);

  /**
    * @brief Send Basic Communication Request to HMI.
    *
    * @param app - Application which have to set specified HMI level.
    * @param hmi_level - New HMI level for specified application.
    * @param send_policy_priority - Defines whether to send "priority" field
    * with request
    * @param activate_app_corr_id - Variable for detting ActivateAppRequest
    * correlation id.
    * @return - True if command is executed, otherwise return false.
    */
  bool SendBCActivateApp(app_mngr::ApplicationConstSharedPtr app,
                         const hmi_apis::Common_HMILevel::eType hmi_level,
                         const bool send_policy_priority);
};

}  // namespace commands
}  // namespace sdl_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_CLOSE_APPLICATION_REQUEST_H_
