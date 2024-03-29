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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SET_GLOBAL_PROPERTIES_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SET_GLOBAL_PROPERTIES_REQUEST_H_
#include <string>

#include "application_manager/application.h"
#include "application_manager/commands/request_from_mobile_impl.h"
#include "utils/macro.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {

/**
 * @brief Register app interface request  command class
 **/
class SetGlobalPropertiesRequest
    : public app_mngr::commands::RequestFromMobileImpl {
 public:
  /**
   * @brief SetGlobalPropertiesRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  SetGlobalPropertiesRequest(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handler);

  /**
   * @brief SetGlobalPropertiesRequest class destructor
   **/
  ~SetGlobalPropertiesRequest();

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
   * @brief Init sets hash update mode for request
   */
  bool Init() FINAL;

  void OnTimeOut() OVERRIDE;

  /**
   * @brief Prepares total result for mobile according to three results:
   * ui_properties_result, tts_properties_result, rc_properties_result.
   * @param first ResponseInfo as first argument
   * @param second ResponseInfo as secondargument
   * @param third ResponseInfo as third argument
   * @return total result
   */
  bool PrepareResultForMobileResponse(
      const app_mngr::commands::ResponseInfo& first,
      const app_mngr::commands::ResponseInfo& second,
      const app_mngr::commands::ResponseInfo& third) const;

  /**
   * @brief Prepare result code for sending to mobile application
   * @param first contains result_code from HMI response and
   * interface that returns response
   * @param second contains result_code from HMI response and
   * interface that returns response.
   * * @param third contains result_code from HMI response and
   * interface that returns response.
   * @return resulting code for sending to mobile application.
   */
  mobile_apis::Result::eType PrepareResultCodeForResponse(
      const app_mngr::commands::ResponseInfo& first,
      const app_mngr::commands::ResponseInfo& second,
      const app_mngr::commands::ResponseInfo& third);

  /**
   * @brief Resolves if the return code must be
   * UNSUPPORTED_RESOURCE
   * @param first contains result_code from HMI response and
   * interface that returns response
   * @param second contains result_code from HMI response and
   * interface that returns response.
   * * @param third contains result_code from HMI response and
   * interface that returns response.
   * @return True, if the communication return code must be
   * UNSUPPORTED_RESOURCE, otherwise false.
   */
  bool IsResultCodeUnsupported(
      const app_mngr::commands::ResponseInfo& first,
      const app_mngr::commands::ResponseInfo& second,
      const app_mngr::commands::ResponseInfo& third) const;

 private:
  /**
   * @brief MergeInfos merge 2 infos into one string with info
   * @param first_info -contains result_code from HMI response and
   * interface that returns response
   * @param first_str - info string that should be first in result info
   * @param second_info -contains result_code from HMI response and
   * interface that returns response
   * @param second_str - info string that should be second in result info
   * @param third_info - contains result_code from HMI response and
   * interface that returns response
   * @param third_str - info string that should be third in result info
   * @return if first_info is not available and second_str and third_info not
   * empty return second if second_info is not available and first_str and
   * third_info not empty return first if third_info is not available and
   * first_str and second_str not empty return first other cases return result
   * MergeInfos for 2 params
   */
  std::string MergeInfos(const app_mngr::commands::ResponseInfo& first_info,
                         const std::string& first_str,
                         const app_mngr::commands::ResponseInfo& second_info,
                         const std::string& second_str,
                         const app_mngr::commands::ResponseInfo& third_info,
                         const std::string& third_str);

  // prepare UI sending data (VrHelps, Menus, Keyboard) to SmartObject
  static void PrepareUIRequestVRHelpData(
      const app_mngr::ApplicationSharedPtr app,
      const smart_objects::SmartObject& msg_params,
      smart_objects::SmartObject& out_params);

  static void PrepareUIRequestMenuAndKeyboardData(
      const app_mngr::ApplicationSharedPtr app,
      const smart_objects::SmartObject& msg_params,
      smart_objects::SmartObject& out_params);

  // Send TTS request to HMI
  void SendTTSRequest(const smart_objects::SmartObject& params,
                      bool use_events);

  // Send UI request to HMI
  void SendUIRequest(const smart_objects::SmartObject& params, bool use_events);

  // Send SetGlobalProperties with userLocation parameter
  void SendRCRequest(const smart_objects::SmartObject& params, bool use_events);

  // VRHelp shall contain sequential positions and start from 1
  static bool CheckVrHelpItemsOrder(const smart_objects::SmartObject& vr_help);

  /**
   * @brief Check if there some not delivered hmi responses exist
   *
   * @return true if all responses received
   */
  bool IsPendingResponseExist();

  /**
   * @brief Checks if request has at least one parameter
   *
   * @param params request parameters
   *
   * @returns true if at least one parameter is present in object
   */
  static bool ValidateConditionalMandatoryParameters(
      const smart_objects::SmartObject& params);

  /**
   * @brief Checks set global properties params(helpPrompt, timeoutPrompt, ...).
   * When type is String there is a check on the contents \t\n \\t \\n
   * @return if set global properties contains \t\n \\t \\n return TRUE,
   * FALSE otherwise
   */
  bool IsWhiteSpaceExist();

  /**
   * @brief helps to determine layout of interest. Returns keyboard layout,
   * mentioned in current request. If not, returns saved keyboard layout for
   * current app. If such layout wasn't saved, returns default keyboard layout
   * (QWERTY)
   * @return KeyboardLayout enum value
   */
  hmi_apis::Common_KeyboardLayout::eType GetKeyboardLayout() const;

  /**
   * @brief Returns allowed number of configurable keys for certain layout
   * @return allowed number of configurable keys, if provided, and zero
   * otherwise
   */
  uint32_t GetAllowedNumberOfConfigurableKeys() const;

  /**
   * @brief Checks provided custom keys against capabilities.
   * @return true if the specified keyboard layout supports the number of
   * custom keys provided.
   */
  bool ValidateCustomKeys() const;

  /**
   * @brief Prepare result code and result for sending to mobile application
   * @param result_code contains result code for sending to mobile application
   * @param info contains info for sending to mobile applicaion
   * @return result for sending to mobile application.
   */
  bool PrepareResponseParameters(mobile_apis::Result::eType& result_code,
                                 std::string& info);

  bool is_ui_send_;
  bool is_tts_send_;
  bool is_rc_send_;

  bool is_ui_received_;
  bool is_tts_received_;
  bool is_rc_received_;

  hmi_apis::Common_Result::eType ui_result_;
  hmi_apis::Common_Result::eType tts_result_;
  hmi_apis::Common_Result::eType rc_result_;
  std::string ui_response_info_;
  std::string tts_response_info_;
  std::string rc_response_info_;

  bool is_menu_layout_available_ = true;

  DISALLOW_COPY_AND_ASSIGN(SetGlobalPropertiesRequest);
};

}  // namespace commands
}  // namespace sdl_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SET_GLOBAL_PROPERTIES_REQUEST_H_
