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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_CHANGE_REGISTRATION_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_CHANGE_REGISTRATION_REQUEST_H_

#include <strings.h>

#include "application_manager/commands/command_request_impl.h"
#include "application_manager/commands/pending.h"
#include "utils/macro.h"
#include "utils/custom_string.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {

namespace custom_str = utils::custom_string;

/**
 * @brief ChangeRegistrationRequest command class
 **/
class ChangeRegistrationRequest
    : public app_mngr::commands::CommandRequestImpl {
 public:
  /**
   * @brief ChangeRegistrationRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  ChangeRegistrationRequest(const app_mngr::commands::MessageSharedPtr& message,
                            app_mngr::ApplicationManager& application_manager,
                            app_mngr::rpc_service::RPCService& rpc_service,
                            app_mngr::HMICapabilities& hmi_capabilities,
                            policy::PolicyHandlerInterface& policy_handler);

  /**
   * @brief ChangeRegistrationRequest class destructor
   **/
  virtual ~ChangeRegistrationRequest();

  /**
   * @brief Execute command
   **/
  virtual void Run();

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  void on_event(const app_mngr::event_engine::Event& event);

 private:
  /*
   * @brief Search for requested language in HMI UI supported languages
   *
   * @return true if language supported by UI, otherwise false
   */
  bool IsLanguageSupportedByUI(const int32_t& hmi_display_lang);

  /*
   * @brief Search for requested language in HMI VR supported languages
   *
   * @return true if language supported by VR, otherwise false
   */
  bool IsLanguageSupportedByVR(const int32_t& hmi_display_lang);

  /*
   * @brief Search for requested language in HMI TTS supported languages
   *
   * @return true if language supported by TTS, otherwise false
   */
  bool IsLanguageSupportedByTTS(const int32_t& hmi_display_lang);

  /*
   * @brief Check if there some not delivered hmi responses exist
   *
   * @return true if all responses received
   */
  bool IsPendingResponseExist();

  /**
   * @brief Checks change_registration params(ttsName, appname,
   * ngnMediaScreenAppName, vrSynonyms) on invalid characters.
   *
   * @return true if command contains \t\n \\t \\n of whitespace otherwise
   * returns false.
   */
  bool IsWhiteSpaceExist();

  /**
   * @brief Check parameters (name, vr) for
   * coincidence with already known parameters of registered applications
   *
   * @return SUCCESS if there is no coincidence of app.name/VR synonyms,
   * otherwise appropriate error code returns
   */
  mobile_apis::Result::eType CheckCoincidence();

  /**
   * @brief Checks if requested name is allowed by policy
   * @param app_name Application name
   * @return true, if allowed, otherwise - false
   */
  bool IsNicknameAllowed(const custom_str::CustomString& app_name) const;

  /**
   * @brief Prepare result code and result for sending to mobile application
   * @param result_code contains result code for sending to mobile application
   * @param response_info contains info for sending to mobile application
   * @return result for sending to mobile application.
   */
  bool PrepareResponseParameters(mobile_apis::Result::eType& result_code,
                                 std::string& ResponseInfo);

  /**
    * @brief Predicate for using with CheckCoincidence method to compare with VR
    * synonym SO
    *
    * @return TRUE if there is coincidence of VR, otherwise FALSE
    */
  struct CoincidencePredicateVR {
    CoincidencePredicateVR(const custom_str::CustomString& newItem)
        : newItem_(newItem) {}

    bool operator()(const smart_objects::SmartObject& obj) const {
      const custom_str::CustomString& vr_synonym = obj.asCustomString();
      return newItem_.CompareIgnoreCase(vr_synonym);
    };

    const custom_str::CustomString& newItem_;
  };

  app_mngr::commands::Pending pending_requests_;

  hmi_apis::Common_Result::eType ui_result_;
  hmi_apis::Common_Result::eType vr_result_;
  hmi_apis::Common_Result::eType tts_result_;
  std::string ui_response_info_;
  std::string vr_response_info_;
  std::string tts_response_info_;

  void SendVRRequest(app_mngr::ApplicationSharedPtr app,
                     smart_objects::SmartObject& msg_params);

  void SendTTSRequest(app_mngr::ApplicationSharedPtr app,
                      smart_objects::SmartObject& msg_params);

  void SendUIRequest(app_mngr::ApplicationSharedPtr app,
                     smart_objects::SmartObject& msg_params,
                     const int32_t hmi_language);

  DISALLOW_COPY_AND_ASSIGN(ChangeRegistrationRequest);
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_CHANGE_REGISTRATION_REQUEST_H_
