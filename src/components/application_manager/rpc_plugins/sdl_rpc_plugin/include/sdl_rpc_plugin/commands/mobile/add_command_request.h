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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_ADD_COMMAND_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_ADD_COMMAND_REQUEST_H_

#include <string>

#include "application_manager/application.h"
#include "application_manager/commands/command_request_impl.h"
#include "utils/macro.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {

/**
 * @brief AddCommandRequest command class
 **/
class AddCommandRequest : public app_mngr::commands::CommandRequestImpl {
 public:
  /**
   * @brief AddCommandRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  AddCommandRequest(const app_mngr::commands::MessageSharedPtr& message,
                    app_mngr::ApplicationManager& application_manager,
                    app_mngr::rpc_service::RPCService& rpc_service,
                    app_mngr::HMICapabilities& hmi_capabilities,
                    policy::PolicyHandlerInterface& policy_handler);

  /**
   * @brief AddCommandRequest class destructor
   **/
  ~AddCommandRequest();

  /**
   * @brief Execute command
   **/
  void Run() FINAL;

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  void on_event(const app_mngr::event_engine::Event& event) FINAL;

  /**
   * @brief Function is called by RequestController when request execution time
   * has exceed it's limit
   */
  void onTimeOut() FINAL;

  /**
   * @brief Init sets hash update mode for request
   */
  bool Init() FINAL;

 private:
  /*
   * @brief Check if command name doesn't exist in application
   * Please see SDLAQ-CRS-407 for more information
   *
   * @param app Mobile application
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool CheckCommandName(app_mngr::ApplicationConstSharedPtr app);

  /*
   * @brief Check if command VR synonyms doesn't exist in application commands
   * Please see SDLAQ-CRS-407 for more information
   *
   * @param app Mobile application
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool CheckCommandVRSynonym(app_mngr::ApplicationConstSharedPtr app);

  /*
   * @brief Check if command parent ID exists in submenu map
   *
   * @param app Mobile application
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool CheckCommandParentId(app_mngr::ApplicationConstSharedPtr app);

  /**
   * @brief Removes command from list when HMI sends negative response or
   * HMI does not answer on addCommand request.
   */
  void RemoveCommand();

  DISALLOW_COPY_AND_ASSIGN(AddCommandRequest);

  /*
   * @brief Check if there some not delivered hmi responses exist
   *
   * @return true if all responses received
   */
  bool IsPendingResponseExist();

  /**
   * @brief Checks add command param
   * When type is String there is a check on the contents \t\n \\t \\n
   * @return if add command contains \t\n \\t \\n return TRUE,
   * FALSE otherwise
   */
  bool IsWhiteSpaceExist();

  inline bool BothSend() const;

  /**
   * @brief GenerateMobileResponseInfo generated info for mobile response
   * depends from UI and VR responses
   * @return info for mobile response
   */
  const std::string GenerateMobileResponseInfo();
  bool send_ui_;
  bool send_vr_;

  bool is_ui_received_;
  bool is_vr_received_;

  std::string ui_info_;
  std::string vr_info_;

  hmi_apis::Common_Result::eType ui_result_;
  hmi_apis::Common_Result::eType vr_result_;
};

}  // namespace commands
}  // namespace sdl_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_ADD_COMMAND_REQUEST_H_
