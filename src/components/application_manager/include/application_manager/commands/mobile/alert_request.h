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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_ALERT_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_ALERT_REQUEST_H_

#include <string>

#include "application_manager/commands/command_request_impl.h"
#include "interfaces/MOBILE_API.h"
#include "utils/macro.h"

namespace application_manager {

namespace commands {

/**
 * @brief AlertRequest command class
 **/
class AlertRequest : public CommandRequestImpl {
 public:
  /**
   * @brief AlertRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  AlertRequest(const MessageSharedPtr& message,
               ApplicationManager& application_manager);

  /**
   * @brief AlertRequest class destructor
   **/
  virtual ~AlertRequest();

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
  void on_event(const event_engine::Event& event);

 protected:
 private:
  /*
   * @brief Checks if request parameters are valid
   * @param app_id Id of application requested this RPC
   * @returns true if request is valid and should be processed
   */
  bool Validate(uint32_t app_id);
  /*
   * @brief Sends UI Alert request
   *
   * @param app_id Id of application requested this RPC
   */
  void SendAlertRequest(int32_t app_id);

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
  bool CheckStringsOfAlertRequest();

  /*
   * @brief Prepare result code and result for sending to mobile application
   * @param result_code contains result code for sending to mobile application
   * @param info contains info for mobile app.
   * @return result for sending to mobile application.
   */
  bool PrepareResponseParameters(mobile_apis::Result::eType& result_code,
                                 std::string& info);

  bool awaiting_ui_alert_response_;
  bool awaiting_tts_speak_response_;
  bool awaiting_tts_stop_speaking_response_;
  bool is_alert_succeeded_;
  bool is_ui_alert_sent_;
  hmi_apis::Common_Result::eType alert_result_;
  smart_objects::SmartObject alert_response_params_;
  hmi_apis::Common_Result::eType tts_speak_result_;
  std::string ui_response_info_;
  std::string tts_response_info_;

  DISALLOW_COPY_AND_ASSIGN(AlertRequest);
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_ALERT_REQUEST_H_
