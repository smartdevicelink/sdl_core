/*

 Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_PERFORM_INTERACTION_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_PERFORM_INTERACTION_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"
#include "application_manager/application.h"
#include "utils/macro.h"

namespace application_manager {

class Application;

namespace commands {

/**
 * @brief PerformInteractionRequest command class
 **/
class PerformInteractionRequest : public CommandRequestImpl  {

 public:
  /**
   * @brief PerformInteractionRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  explicit PerformInteractionRequest(const MessageSharedPtr& message);

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
  virtual void on_event(const event_engine::Event& event);

 private:
  /*
   * @brief Function is called by RequestController when request execution time
   * has exceed it's limit
   *
   */
    virtual void onTimeOut();

  /*
   * @brief Function will be called when VR_OnCommand event
   * comes
   *
   * @param message which should send to mobile side
   *
   */
  void ProcessVRResponse(const smart_objects::SmartObject& message, smart_objects::SmartObject& msg_params);

  /*
   * @brief Sends PerformInteraction response to mobile side
   *
   * @param message which should send to mobile side
   *
   */
  void ProcessPerformInteractionResponse
  (const smart_objects::SmartObject& message, smart_objects::SmartObject &msg_params);


  /*
   * @brief Sends UI PerformInteraction request to HMI
   *
   * @param app_id Application ID
   *
   */
  void SendUIPerformInteractionRequest(
      application_manager::ApplicationSharedPtr const app);

  /*
   * @brief Sends TTS PerformInteraction request to HMI
   *
   * @param app_id Application ID
   *
   */
  void SendVRPerformInteractionRequest(
      application_manager::ApplicationSharedPtr const app);

  /*
   * @brief Sends UI Show VR help request to HMI
   *
   * @param app_id Application ID
   */
  void SendUIShowVRHelpRequest(ApplicationSharedPtr const app);

  /*
   * @brief Checks if incoming choice set doesn't has similar menu names.
   *
   * @param app_id Application ID
   *
   * return Return TRUE if there are no similar menu names in choice set,
   * otherwise FALSE
   */
  bool CheckChoiceSetMenuNames(application_manager::ApplicationSharedPtr const app);

  /*
   * @brief Checks if incoming choice set doesn't has similar VR synonyms.
   *
   * @param app_id Application ID
   *
   * return Return TRUE if there are no similar VR synonyms in choice set,
   * otherwise FALSE
   */
  bool CheckChoiceSetVRSynonyms(application_manager::ApplicationSharedPtr const app);

  /*
   * @brief Checks if request with non-sequential positions of vrHelpItems
   * SDLAQ-CRS-466
   *
   * @param app_id Application ID
   *
   * @return TRUE if vrHelpItems positions are sequential,
   * otherwise FALSE
   */
  bool CheckVrHelpItemPositions(application_manager::ApplicationSharedPtr const app);

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
  bool CheckChoiceIDFromResponse(ApplicationSharedPtr app, int32_t choice_id);

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
  bool CheckChoiceIDFromRequest(ApplicationSharedPtr app,
                                const size_t choice_set_id_list_length,
                                const smart_objects::SmartObject& choice_set_id_list) const;

  /**
   * @brief Tells if there are sent requests without responses
   * @return If there is request without response method returns TRUE
   * otherwise returns FALSE
   */
  const bool HasHMIResponsesToWait() const;

  /**
   * @brief Check VR response result code, in case GENERIC_ERROR, REJECTED,
   * send resultCode FALSE, in case WARNINGS send resultCode TRUE
   */
  void CheckResponseResultCode();

  /**
   * @brief Check UI & VR result codes, send response to mobile
   * @param msg_param Message params to send
   */
  void SendBothModeResponse(const smart_objects::SmartObject &msg_param);

  mobile_apis::InteractionMode::eType interaction_mode_;
  bool                                ui_response_recived_;
  bool                                vr_response_recived_;
  bool                                ui_result_;
  bool                                vr_result_;
  bool                                app_pi_was_active_before_;
  static uint32_t                     pi_requests_count_;
  mobile_apis::Result::eType          vr_resultCode_;
  mobile_apis::Result::eType          ui_resultCode_;

  DISALLOW_COPY_AND_ASSIGN(PerformInteractionRequest);
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_PERFORM_INTERACTION_REQUEST_H_
