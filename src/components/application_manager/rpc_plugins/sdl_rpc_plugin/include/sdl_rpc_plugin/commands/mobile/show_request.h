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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SHOW_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SHOW_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"
#include "interfaces/MOBILE_API.h"
#include "utils/macro.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {

/**
 * @brief show request command class
 **/
class ShowRequest : public app_mngr::commands::CommandRequestImpl {
 public:
  /**
   * \brief ShowRequest class constructor
   **/
  ShowRequest(const app_mngr::commands::MessageSharedPtr& message,
              app_mngr::ApplicationManager& application_manager,
              app_mngr::rpc_service::RPCService& rpc_service,
              app_mngr::HMICapabilities& hmi_capabilities,
              policy::PolicyHandlerInterface& policy_handler);

  /**
   * \brief ShowRequest class destructor
   **/
  virtual ~ShowRequest();

  /**
   * @brief Execute command
   **/
  virtual void Run();

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  virtual void on_event(const app_mngr::event_engine::Event& event);

 private:
  /*
   * @brief Check if all strings have valid syntax in request
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool CheckStringsOfShowRequest();

  /**
   * @brief Handle the conversion of metadata information from the incoming
   * mobile json message format to the outgoing hmi json message format
   */
  void HandleMetadata(const char* field_id,
                      int32_t field_index,
                      smart_objects::SmartObject& msg_params);

  mobile_apis::Result::eType core_result_code_;
  std::string core_response_info_;

  DISALLOW_COPY_AND_ASSIGN(ShowRequest);
};

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SHOW_REQUEST_H_
