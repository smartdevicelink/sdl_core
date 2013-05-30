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

#include "application_manager/commands/alert_command.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_chaining.h"
#include "application_manager/application_impl.h"
#include "JSONHandler/SDLRPCObjects/V2/HMILevel.h"
#include "utils/logger.h"

namespace application_manager {

namespace commands {

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

log4cxx::LoggerPtr logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Commands"));

AlertCommandRequest::AlertCommandRequest(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

AlertCommandRequest::~AlertCommandRequest() {
}

void AlertCommandRequest::Run() {
  LOG4CXX_INFO(logger_, "AlertCommandRequest::Run ");

  ApplicationImpl* app = static_cast<ApplicationImpl*>(
      ApplicationManagerImpl::instance()->
      application((*message_)[strings::params][strings::connection_key]));

  if (NULL == app) {
    LOG4CXX_ERROR_EXT(logger_, "No application associated with session key ");
    SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  SendAlertRequest();
  SendPlayToneRequest();
  SendSpeekRequest();
}

void AlertCommandRequest::SendAlertRequest() const {
  const int corellation_id =
      (*message_)[strings::params][strings::correlation_id];
  const int connection_key =
      (*message_)[strings::params][strings::connection_key];

  // create HMI alert request
  smart_objects::CSmartObject* ui_alert  = new smart_objects::CSmartObject();
  // TODO(DK): HMI Request Id
  const int ui_cmd_id = 31;
  (*ui_alert)[strings::params][strings::function_id] =
      ui_cmd_id;

  (*ui_alert)[strings::params][strings::message_type] =
      MessageType::kRequest;

  // create Common.TextFieldStruct type
  // alert1
  (*ui_alert)[strings::msg_params][hmi_request::alert_strings][0] =
      smart_objects::CSmartObject();
  (*ui_alert)[strings::msg_params][hmi_request::alert_strings][0]
      [hmi_request::field_name] = TextFieldName::ALERT_TEXT1;
  (*ui_alert)[strings::msg_params][hmi_request::alert_strings][0]
      [hmi_request::field_text] =
          (*message_)[strings::msg_params][strings::alert_text1];

  // alert2
  (*ui_alert)[strings::msg_params][hmi_request::alert_strings][1] =
      smart_objects::CSmartObject();
  (*ui_alert)[strings::msg_params][hmi_request::alert_strings][1]
      [hmi_request::field_name] = TextFieldName::ALERT_TEXT2;
  (*ui_alert)[strings::msg_params][hmi_request::alert_strings][1]
      [hmi_request::field_text] =
          (*message_)[strings::msg_params][strings::alert_text2];
  // alert3
  (*ui_alert)[strings::msg_params][hmi_request::alert_strings][2] =
      smart_objects::CSmartObject();
  (*ui_alert)[strings::msg_params][hmi_request::alert_strings][2]
      [hmi_request::field_name] = TextFieldName::ALERT_TEXT3;
  (*ui_alert)[strings::msg_params][hmi_request::alert_strings][2]
      [hmi_request::field_text] =
          (*message_)[strings::msg_params][strings::alert_text3];
  // duration
  (*ui_alert)[strings::msg_params][hmi_request::duration] =
      (*message_)[strings::msg_params][strings::duration];
  // softButtons
  (*ui_alert)[strings::msg_params][hmi_request::soft_buttons] =
      (*message_)[strings::msg_params][strings::soft_buttons];
  // app_id
  (*ui_alert)[strings::msg_params][strings::app_id] =
      (*message_)[strings::params][strings::connection_key];

  MessageChaining * chain = NULL;
  chain = ApplicationManagerImpl::instance()->AddMessageChain(chain,
      connection_key, corellation_id, ui_cmd_id);

  ApplicationManagerImpl::instance()->SendMessageToHMI(ui_alert);
}

void AlertCommandRequest::SendSpeekRequest() const {
  // check TTSChunk parameter
  if ((*message_)[strings::msg_params].keyExists(strings::tts_chunks)) {
    if (0 < (*message_)[strings::msg_params][strings::tts_chunks].length()) {
      // crate HMI basic communication playtone request
      smart_objects::CSmartObject* tts_speak =
          new smart_objects::CSmartObject();
      // TODO(DK): HMI tts request Id
      const int tts_cmd_id = 51;
      (*tts_speak)[strings::params][strings::function_id] =
          tts_cmd_id;

      (*tts_speak)[strings::params][strings::message_type] =
          MessageType::kRequest;
      (*tts_speak)[strings::msg_params][hmi_request::tts_chunks] =
         (*message_)[strings::msg_params][strings::tts_chunks];
      ApplicationManagerImpl::instance()->SendMessageToHMI(tts_speak);
    }
  }
}

void AlertCommandRequest::SendPlayToneRequest() const {
  // check playtone parameter
  if ((*message_)[strings::msg_params].keyExists(strings::play_tone)) {
    if ((*message_)[strings::msg_params][strings::play_tone].asBool()) {
      // crate HMI basic communication playtone request
      smart_objects::CSmartObject* bc_play =
          new smart_objects::CSmartObject();
      // TODO(DK): HMI Basic communication request Id
      const int bc_cmd_id = 41;
      (*bc_play)[strings::params][strings::function_id] =
          bc_cmd_id;

      (*bc_play)[strings::params][strings::message_type] =
          MessageType::kNotification;
      ApplicationManagerImpl::instance()->SendMessageToHMI(bc_play);
    }
  }
}

}  // namespace commands

}  // namespace application_manager
