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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_PERFORM_AUDIO_PASS_THRU_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_PERFORM_AUDIO_PASS_THRU_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"
#include "utils/macro.h"
#include "interfaces/MOBILE_API.h"

namespace application_manager {

namespace commands {

/**
 * @brief PerformAudioPassThruRequest command class
 **/
class PerformAudioPassThruRequest : public CommandRequestImpl {
 public:
  /**
   * @brief PerformAudioPassThruRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  PerformAudioPassThruRequest(const MessageSharedPtr& message,
                              ApplicationManager& application_manager);

  /**
   * @brief PerformAudioPassThruRequest class destructor
   **/
  virtual ~PerformAudioPassThruRequest();

  /**
   * @brief Function is called by RequestController when request execution time
   * has exceed it's limit
   *
   */
  virtual void onTimeOut();

  /**
   * @brief Init required by command resources
   **/
  bool Init();

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
  /**
   * @brief Prepare result code, result and info for sending to mobile
   * application
   * @param result_code contains result code for sending to mobile application
   * @return result for sending to mobile application.
   */
  bool PrepareResponseParameters(mobile_apis::Result::eType& result_code,
                                 std::string& info);

  /**
   * @brief Checks result code from HMI for splitted RPC
   * and returns parameter for sending to mobile app.
   * @param ui_perform_info contains result_code from HMI response and
   * interface that returns ui response
   * @param tts_perform_info contains result_code from HMI response and
   * interface that returns tts response
   * @return true if result code complies successful result code
   * otherwise returns false
   */
  bool PrepareResultForMobileResponse(
      ResponseInfo& ui_perform_info,
      ResponseInfo& tts_perform_info) const FINAL;

  /**
   * @brief Prepare result code for sending to mobile application
   * @param ui_perform_info contains result_code from HMI response and
   * interface that returns ui response
   * @param tts_perform_info contains result_code from HMI response and
   * interface that returns tts response.
   * @return resulting code for sending to mobile application.
   */
  mobile_apis::Result::eType PrepareResultCodeForResponse(
      const ResponseInfo& ui_perform_info,
      const ResponseInfo& tts_perform_info) FINAL;

  /**
   * @brief Sends TTS Speak request
   */
  void SendSpeakRequest();

  /**
   * @brief Sends UI PerformAudioPassThru request
   */
  void SendPerformAudioPassThruRequest();

  /**
   * @brief Sends UI RecordStart notification after TTS Speak response received.
   * Indicates that capturing mic data should be started
   */
  void SendRecordStartNotification();

  /**
   * @brief Starts microphone recording
   */
  void StartMicrophoneRecording();

  /**
   * @brief Checks perform audio pass thru params(initialPrompt, ...).
   * When type is String there is a check on the contents \t\n \\t \\n
   * @return if perform audio pass thru contains \t\n \\t \\n return TRUE,
   * FALSE otherwise
   */
  bool IsWhiteSpaceExist();

  /**
   * @brief If is_active_tts_speak_ TRUE - set up to FALSE and send request
   * TTS_StopSpeaking to HMI
   */
  void FinishTTSSpeak();

  /*
   * @brief Tells if there are sent requests without responses
   */
  bool IsWaitingHMIResponse();

  /* flag display state of speak and ui perform audio
  during perform audio pass thru*/
  bool awaiting_tts_speak_response_;
  bool awaiting_ui_response_;

  hmi_apis::Common_Result::eType result_tts_speak_;
  hmi_apis::Common_Result::eType result_ui_;
  std::string ui_info_;
  std::string tts_info_;

  DISALLOW_COPY_AND_ASSIGN(PerformAudioPassThruRequest);
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_PERFORM_AUDIO_PASS_THRU_REQUEST_H_
