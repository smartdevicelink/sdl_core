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
#include "application_manager/event_engine/event_observer.h"
#include "utils/macro.h"

namespace application_manager {

class Application;

namespace commands {

/**
 * @brief PerformInteractionRequest command class
 **/
class PerformInteractionRequest : public CommandRequestImpl,
  public event_engine::EventObserver {

 public:

  /*
   * @brief Typedef for InteractionMode
   */
  typedef enum {
    MANUAL_ONLY = 0,
    VR_ONLY,
    BOTH,
    INVALID_ENUM
  } InteractionMode;

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
   * @brief Execute command
   **/
  virtual void Run();

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  void on_event(const event_engine::Event& event);

 private:
  /*
   * @brief Sends VR AddCommand request to HMI
   *
   * @param app_id Application ID
   *
   */
  void SendVRAddCommandRequest(Application* const app);

  /*
   * @brief Sends UI PerformInteraction request to HMI
   *
   * @param app_id Application ID
   *
   */
  void SendUIPerformInteractionRequest(Application* const app);

  /*
   * @brief Sends TTS Speak request to HMI
   *
   * @param app_id Application ID
   */
  void SendTTSSpeakRequest(Application* const app);

  /*
   * @brief Sends UI Show VR help request to HMI
   *
   * @param app_id Application ID
   */
  void SendUIShowVRHelpRequest(Application* const app);

  /**
   * @brief Creates and Sends Perform interaction to UI.
   */
  void CreateUIPerformInteraction(const smart_objects::SmartObject& msg_params,
                                  Application* const app);

  /*
   * @brief Checks if incoming choice set doesn't has similar menu names.
   *
   * @param app_id Application ID
   *
   * return Return TRUE if there are no similar menu names in choice set,
   * otherwise FALSE
   */
  bool CheckChoiceSetMenuNames(Application* const app);

  /*
   * @brief Checks if incoming choice set doesn't has similar VR synonyms.
   *
   * @param app_id Application ID
   *
   * return Return TRUE if there are no similar VR synonyms in choice set,
   * otherwise FALSE
   */
  bool CheckChoiceSetVRSynonyms(Application* const app);

  /*
   * @brief Checks if request with non-sequential positions of vrHelpItems
   * SDLAQ-CRS-466
   *
   * @param app_id Application ID
   *
   * @return TRUE if vrHelpItems positions are sequential,
   * otherwise FALSE
   */
  bool CheckVrHelpItemPositions(Application* const app);

  DISALLOW_COPY_AND_ASSIGN(PerformInteractionRequest);
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_PERFORM_INTERACTION_REQUEST_H_
