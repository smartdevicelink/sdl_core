/*
 Copyright (c) 2020, Ford Motor Company, Livio
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the the copyright holders nor the names of their
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SUBTLE_ALERT_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SUBTLE_ALERT_REQUEST_H_

#include "application_manager/commands/request_from_mobile_impl.h"
#include "sdl_rpc_plugin/sdl_rpc_plugin.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {

/**
 * @brief SubtleAlertRequest command class
 **/
class SubtleAlertRequest : public app_mngr::commands::RequestFromMobileImpl {
 public:
  /**
   * @brief SubtleAlertRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  SubtleAlertRequest(const app_mngr::commands::MessageSharedPtr& message,
                     app_mngr::ApplicationManager& application_manager,
                     app_mngr::rpc_service::RPCService& rpc_service,
                     app_mngr::HMICapabilities& hmi_capabilities,
                     policy::PolicyHandlerInterface& policy_handle);

  /**
   * @brief SubtleAlertRequest class destructor
   **/
  virtual ~SubtleAlertRequest();

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
  void on_event(const app_mngr::event_engine::Event& event);

 protected:
 private:
  /*
   * @brief Checks if request parameters are valid
   * @param app_id Id of application requested this RPC
   * @returns true if request is valid and should be processed
   */
  bool Validate(uint32_t app_id);

  /*
   * @brief Sends UI SubtleAlert request
   *
   * @param app_id Id of application requested this RPC
   */
  void SendSubtleAlertRequest(int32_t app_id);

  /*
   * @brief Sends TTS Speak request
   *
   * @param app_id Id of application requested this RPC
   *
   * @param tts_chunks_exists if tts chunks exists in
   * message contains true, otherwise contains false
   *
   * @param length_tts_chunks contains length of array
   * tts chunks.
   */
  void SendSpeakRequest(int32_t app_id,
                        bool tts_chunks_exists,
                        size_t length_tts_chunks);

  /*
   * @brief Tells if there are sent requests without responses
   */
  bool HasHmiResponsesToWait();

  /*
   * @brief Check if all strings have valid syntax in request
   *
   * @return TRUE on success, otherwise FALSE
   */
  bool CheckStrings();

  /*
   * @brief Prepare result code and result for sending to mobile application
   * @param result_code contains result code for sending to mobile application
   * @param info contains info for mobile app.
   * @return result for sending to mobile application.
   */
  bool PrepareResponseParameters(mobile_apis::Result::eType& result_code,
                                 std::string& info);

  bool awaiting_ui_subtle_alert_response_;
  bool awaiting_tts_speak_response_;
  bool awaiting_tts_stop_speaking_response_;
  bool is_ui_subtle_alert_sent_;
  bool is_tts_stop_speaking_sent_;
  hmi_apis::Common_Result::eType subtle_alert_result_;
  smart_objects::SmartObject subtle_alert_response_params_;
  hmi_apis::Common_Result::eType tts_speak_result_;
  std::string ui_response_info_;
  std::string tts_response_info_;

  DISALLOW_COPY_AND_ASSIGN(SubtleAlertRequest);
};

}  // namespace commands

}  // namespace sdl_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SUBTLE_ALERT_REQUEST_H_
