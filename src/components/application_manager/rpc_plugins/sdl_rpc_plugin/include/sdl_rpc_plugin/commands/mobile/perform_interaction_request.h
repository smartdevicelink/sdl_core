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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_PERFORM_INTERACTION_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_PERFORM_INTERACTION_REQUEST_H_

#include <string>

#include "application_manager/application.h"
#include "application_manager/commands/command_request_impl.h"
#include "utils/macro.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

class Application;

namespace commands {

/**
 * @brief PerformInteractionRequest command class
 **/
class PerformInteractionRequest
    : public app_mngr::commands::CommandRequestImpl {
 public:
  /**
   * @brief PerformInteractionRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  PerformInteractionRequest(const app_mngr::commands::MessageSharedPtr& message,
                            app_mngr::ApplicationManager& application_manager,
                            app_mngr::rpc_service::RPCService& rpc_service,
                            app_mngr::HMICapabilities& hmi_capabilities,
                            policy::PolicyHandlerInterface& policy_handler);

  /**
   * @brief PerformInteractionRequest class destructor
   **/
  virtual ~PerformInteractionRequest();

  /**
   * @brief Initialize request params
   **/
  virtual bool Init();

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

  /*
   * @brief Function is called by RequestController when request execution time
   * has exceed it's limit
   *
   */
  virtual void onTimeOut();

 protected:
  /**
   * @brief Prepare result code for sending to mobile application
   * @param ui_response contains result_code from HMI response and
   * interface that returns response
   * @param vr_response contains result_code from HMI response and
   * interface that returns response.
   * @return resulting code for sending to mobile application.
   */
  mobile_apis::Result::eType PrepareResultCodeForResponse(
      const app_mngr::commands::ResponseInfo& ui_response,
      const app_mngr::commands::ResponseInfo& vr_response);

  /**
   * @brief Checks result code from HMI for splitted RPC
   * and returns parameter for sending to mobile app.
   * @param ui_response contains result_code from HMI response and
   * interface that returns response
   * @param vr_response contains result_code from HMI response and
   * interface that returns response
   * @return true if result code complies successful result code
   * otherwise returns false
   */
  bool PrepareResultForMobileResponse(
      app_mngr::commands::ResponseInfo& out_first,
      app_mngr::commands::ResponseInfo& out_second) const;

 private:
  /**
   * @brief Function will be called when VR_OnCommand event
   * comes
   * @param message which should send to mobile side
   * @return true if send response to mobile application otherwise
   * return false.
   */
  bool ProcessVRResponse(const smart_objects::SmartObject& message);

  /**
   * @brief Sends PerformInteraction response to mobile side
   * @param message which should send to mobile side
   */
  void ProcessUIResponse(const smart_objects::SmartObject& message);

  /*
   * @brief Sends UI PerformInteraction request to HMI
   *
   * @param app_id Application ID
   *
   */
  void SendUIPerformInteractionRequest(
      app_mngr::ApplicationSharedPtr const app);

  /*
   * @brief Sends TTS PerformInteraction request to HMI
   *
   * @param app_id Application ID
   *
   */
  void SendVRPerformInteractionRequest(
      app_mngr::ApplicationSharedPtr const app);

  /*
   * @brief Sends UI Show VR help request to HMI
   *
   * @param app_id Application ID
   */
  void SendUIShowVRHelpRequest(app_mngr::ApplicationSharedPtr const app);

  /*
   * @brief Checks if incoming choice set doesn't has similar VR synonyms.
   *
   * @param app_id Application ID
   *
   * return Return TRUE if there are no similar VR synonyms in choice set,
   * otherwise FALSE
   */
  bool CheckChoiceSetVRSynonyms(app_mngr::ApplicationSharedPtr const app);

  /*
   * @brief Checks if request with non-sequential positions of vrHelpItems
   * SDLAQ-CRS-466
   *
   * @param app_id Application ID
   *
   * @return TRUE if vrHelpItems positions are sequential,
   * otherwise FALSE
   */
  bool CheckVrHelpItemPositions(app_mngr::ApplicationSharedPtr const app);

  /*
   * @brief Disable PerformInteraction state in application and
   * delete VR commands from HMI
   */
  void DisablePerformInteraction();

  /**
   * @brief Checks perform interaction params(initialText, initialPrompt, ...).
   * When type is String there is a check on the contents \t\n \\t \\n
   * @return if perform interaction contains \t\n \\t \\n return TRUE,
   * FALSE otherwise
   */
  bool IsWhiteSpaceExist();

  /**
   * @brief Send HMI close PopUp and call DisablePerformInteraction
   */
  void TerminatePerformInteraction();

  /**
   * @brief Checks parameter ChoiceID from response.
   * @param app contains pointer to application
   * @param choice_id contains ChoiceID from response.
   * @return if ChoiceID from response is correct method returns TRUE
   * otherwise returns FALSE.
   */
  bool CheckChoiceIDFromResponse(app_mngr::ApplicationSharedPtr app,
                                 const int32_t choice_id);

  /**
   * @brief Checks for a match of choice ID, in
   * choice sets.
   * @param app contains pointer to application.
   * @param choice_set_id_list_length contains amount
   * of choice set ids.
   * @param choice_set_id_list array of choice set ids
   * @return If request contains several choice sets with
   * same choice id returns false, otherwise returns
   * true.
   */
  bool CheckChoiceIDFromRequest(
      app_mngr::ApplicationSharedPtr app,
      const size_t choice_set_id_list_length,
      const smart_objects::SmartObject& choice_set_id_list) const;

  /**
   * @brief Checks each choice in each set for having a VRcommands parameter
   * @param app contains pointer to application.
   * @return returns false if request has choice sets with no vrCommands
   */
  bool CheckChoiceSetListVRCommands(app_mngr::ApplicationSharedPtr app);

  /**
   * @brief Tells if there are sent requests without responses
   * @return If there is request without response method returns TRUE
   * otherwise returns FALSE
   */
  const bool HasHMIResponsesToWait() const;

  /**
   * @brief Check UI & VR result codes, send response to mobile
   * @param msg_param Message params to send
   */
  void SendBothModeResponse(const smart_objects::SmartObject& msg_param);

  /**
   * @brief Sends UiClosePopUp request to HMI
   */
  void SendClosePopupRequestToHMI();

  /**
   * @brief Sets the choice according to the current interaction mode and first
   * received choice id (UI or VR).
   *
   * @param msg_param Message parameters which will be included in the response
   * to mobile device.
   * @return Returns false, if choice_id received from UI and VR are valid and
   * not equal. Otherwise returns true.
   */
  bool SetChoiceIdToResponseMsgParams(
      smart_objects::SmartObject& msg_param) const;

  mobile_apis::InteractionMode::eType interaction_mode_;
  std::int32_t ui_choice_id_received_;
  std::int32_t vr_choice_id_received_;
  std::string ui_text_entry_received_;

  bool ui_response_received_;
  bool vr_response_received_;
  bool app_pi_was_active_before_;
  static uint32_t pi_requests_count_;
  hmi_apis::Common_Result::eType vr_result_code_;
  hmi_apis::Common_Result::eType ui_result_code_;
  std::string ui_info_;
  std::string vr_info_;
  smart_objects::SmartObject response_msg_params;

  DISALLOW_COPY_AND_ASSIGN(PerformInteractionRequest);
};

}  // namespace commands
}  // namespace sdl_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_PERFORM_INTERACTION_REQUEST_H_
