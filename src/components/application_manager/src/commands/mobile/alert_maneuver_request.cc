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

#include "application_manager/commands/mobile/alert_maneuver_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

AlertManeuverRequest::AlertManeuverRequest(const MessageSharedPtr& message)
 : CommandRequestImpl(message),
   result_(mobile_apis::Result::INVALID_ENUM) {
}

AlertManeuverRequest::~AlertManeuverRequest() {
}

void AlertManeuverRequest::Run() {
  LOG4CXX_INFO(logger_, "AlertManeuverRequest::Run");

  if ((!(*message_)[strings::msg_params].keyExists(strings::soft_buttons)) &&
      (!(*message_)[strings::msg_params].keyExists(strings::tts_chunks))) {
    LOG4CXX_ERROR(logger_, "AlertManeuverRequest::Request without parameters!");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  Application* app = ApplicationManagerImpl::instance()->application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  mobile_apis::Result::eType processing_result =
      MessageHelper::ProcessSoftButtons((*message_)[strings::msg_params], app);

  if (mobile_apis::Result::SUCCESS != processing_result) {
    if (mobile_apis::Result::INVALID_DATA == processing_result) {
      LOG4CXX_ERROR(logger_, "Wrong soft buttons parameters!");
      SendResponse(false, processing_result);
      return;
    }
    if (mobile_apis::Result::UNSUPPORTED_RESOURCE == processing_result) {
      LOG4CXX_ERROR(logger_, "UNSUPPORTED_RESOURCE!");
      result_ = processing_result;
    }
  }

  // Checking parameters and how many HMI requests should be sent
  bool tts_is_ok = false;

  // check TTSChunk parameter
  if ((*message_)[strings::msg_params].keyExists(strings::tts_chunks)) {
    if (0 < (*message_)[strings::msg_params][strings::tts_chunks].length()) {
      pending_requests_.Add(hmi_apis::FunctionID::TTS_Speak);
      tts_is_ok = true;
    }
  }

  bool soft_buttons_is_ok = false;

  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    pending_requests_.Add(hmi_apis::FunctionID::Navigation_AlertManeuver);
    soft_buttons_is_ok = true;
  }

  // Sending requests to HMI
  if (soft_buttons_is_ok) {
    smart_objects::SmartObject msg_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);

    msg_params[hmi_request::soft_buttons] =
            (*message_)[strings::msg_params][strings::soft_buttons];

    SendHMIRequest(hmi_apis::FunctionID::Navigation_AlertManeuver,
                   &msg_params,
                   true);
  }

  if (tts_is_ok) {
    smart_objects::SmartObject msg_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);

    msg_params[hmi_request::tts_chunks] = smart_objects::SmartObject(
        smart_objects::SmartType_Array);

    msg_params[hmi_request::tts_chunks] =
        (*message_)[strings::msg_params][strings::tts_chunks];

    msg_params[strings::app_id] = app->app_id();
    SendHMIRequest(hmi_apis::FunctionID::TTS_Speak, &msg_params, true);
  }
}

void AlertManeuverRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "AlertManeuverRequest::on_event");
  const smart_objects::SmartObject& message = event.smart_object();

  mobile_apis::Result::eType result_code;
  hmi_apis::FunctionID::eType id = event.id();
  switch (id) {
    case hmi_apis::FunctionID::Navigation_AlertManeuver: {
      LOG4CXX_INFO(logger_, "Received Navigation_AlertManeuver event");

      pending_requests_.Remove(id);

      navi_alert_maneuver_result_code_ =
          static_cast<mobile_apis::Result::eType>(
          message[strings::params][hmi_response::code].asInt());

      break;
    }
    case hmi_apis::FunctionID::TTS_Speak: {
      LOG4CXX_INFO(logger_, "Received TTS_Speak event");

      pending_requests_.Remove(id);

      tts_speak_result_code_ =
          static_cast<mobile_apis::Result::eType>(
          message[strings::params][hmi_response::code].asInt());

      break;
    }
    default: {
      LOG4CXX_ERROR(logger_,"Received unknown event" << event.id());
      SendResponse(false, result_code, "Received unknown event");
      return;
    }
  }

  if (pending_requests_.IsFinal(id)) {

    result_code = (mobile_apis::Result::SUCCESS != tts_speak_result_code_)
          ? tts_speak_result_code_
          : navi_alert_maneuver_result_code_;

    bool result = mobile_apis::Result::SUCCESS == result_code;

    if (mobile_apis::Result::INVALID_ENUM != result_code) {
      mobile_apis::Result::eType return_code =
      static_cast<mobile_apis::Result::eType>(result_code);

      const char* return_info = NULL;

      if (result) {
        if (hmi_apis::Common_Result::UNSUPPORTED_RESOURCE == result_code) {
          result_code = mobile_apis::Result::WARNINGS;
          return_info = std::string("Unsupported phoneme type sent in a prompt").c_str();
        }
      }

      SendResponse(result, static_cast<mobile_apis::Result::eType>(result_code),
                        return_info, &(message[strings::msg_params]));
    }
  } else {
    LOG4CXX_INFO(logger_,
                "There are some pending responses from HMI."
                "AlertManeuverRequest still waiting.");
  }
}

}  // namespace commands

}  // namespace application_manager
