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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_ALERT_MANEUVER_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_ALERT_MANEUVER_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"
#include "application_manager/commands/pending.h"
#include "interfaces/MOBILE_API.h"
#include "utils/macro.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {

/**
 * @brief AlertManeuverRequest command class
 **/
class AlertManeuverRequest : public app_mngr::commands::CommandRequestImpl {
 public:
  /**
   * @brief AlertManeuverRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  AlertManeuverRequest(const app_mngr::commands::MessageSharedPtr& message,
                       app_mngr::ApplicationManager& application_manager,
                       app_mngr::rpc_service::RPCService& rpc_service,
                       app_mngr::HMICapabilities& hmi_capabilities,
                       policy::PolicyHandlerInterface& policy_handler);

  /**
   * @brief AlertManeuverRequest class destructor
   **/
  virtual ~AlertManeuverRequest();

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
  /**
   * @brief Prepare parameters for  sending to mobile application
   * @param result_code contains result code for sending to mobile application
   * @param return_info contains resulting info for sending to mobile
   * application
   * @return result for sending to mobile application.
   */
  bool PrepareResponseParameters(mobile_apis::Result::eType& result_code,
                                 std::string& return_info);
  /**
   * @brief Checks alert maneuver params(ttsChunks, ...).
   * When type is String there is a check on the contents \t\n \\t \\n
   * @return if alert maneuver contains \t\n \\t \\n return TRUE,
   * FALSE otherwise
   */
  bool IsWhiteSpaceExist();

  hmi_apis::Common_Result::eType tts_speak_result_code_;
  hmi_apis::Common_Result::eType navi_alert_maneuver_result_code_;
  std::string info_navi_;
  std::string info_tts_;
  app_mngr::commands::Pending pending_requests_;

  DISALLOW_COPY_AND_ASSIGN(AlertManeuverRequest);
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_ALERT_MANEUVER_REQUEST_H_
