/*
 Copyright (c) 2016, Ford Motor Company
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

#include <cstring>
#include <string>
#include "application_manager/commands/mobile/alert_maneuver_request.h"
#include "application_manager/application_impl.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/message_helper.h"

namespace application_manager {

namespace commands {

AlertManeuverRequest::AlertManeuverRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager)
    , tts_speak_result_code_(hmi_apis::Common_Result::INVALID_ENUM)
    , navi_alert_maneuver_result_code_(hmi_apis::Common_Result::INVALID_ENUM) {
  subscribe_on_event(hmi_apis::FunctionID::TTS_OnResetTimeout);
}

AlertManeuverRequest::~AlertManeuverRequest() {}

void AlertManeuverRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  if ((!(*message_)[strings::msg_params].keyExists(strings::soft_buttons)) &&
      (!(*message_)[strings::msg_params].keyExists(strings::tts_chunks))) {
    LOG4CXX_ERROR(logger_, "AlertManeuverRequest::Request without parameters!");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  ApplicationSharedPtr app = application_manager_.application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (NULL == app.get()) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (IsWhiteSpaceExist()) {
    LOG4CXX_ERROR(logger_,
                  "Incoming alert maneuver has contains \\t\\n \\\\t \\\\n"
                  "text contains only whitespace in ttsChunks");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  // ProcessSoftButtons checks strings on the contents incorrect character

  mobile_apis::Result::eType processing_result =
      MessageHelper::ProcessSoftButtons((*message_)[strings::msg_params],
                                        app,
                                        application_manager_.GetPolicyHandler(),
                                        application_manager_);

  if (mobile_apis::Result::SUCCESS != processing_result) {
    LOG4CXX_ERROR(logger_, "Wrong soft buttons parameters!");
    SendResponse(false, processing_result);
    return;
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

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[strings::app_id] = app->app_id();

  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    msg_params[hmi_request::soft_buttons] =
        (*message_)[strings::msg_params][strings::soft_buttons];
    MessageHelper::SubscribeApplicationToSoftButton(
        (*message_)[strings::msg_params], app, function_id());
  }

  pending_requests_.Add(hmi_apis::FunctionID::Navigation_AlertManeuver);
  SendHMIRequest(
      hmi_apis::FunctionID::Navigation_AlertManeuver, &msg_params, true);

  if (tts_is_ok) {
    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    msg_params[hmi_request::tts_chunks] =
        (*message_)[strings::msg_params][strings::tts_chunks];
    msg_params[hmi_request::speak_type] =
        hmi_apis::Common_MethodName::ALERT_MANEUVER;
    msg_params[strings::app_id] = app->app_id();

    SendHMIRequest(hmi_apis::FunctionID::TTS_Speak, &msg_params, true);
  }
}

void AlertManeuverRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& message = event.smart_object();
  hmi_apis::FunctionID::eType event_id = event.id();
  switch (event_id) {
    case hmi_apis::FunctionID::Navigation_AlertManeuver: {
      LOG4CXX_INFO(logger_, "Received Navigation_AlertManeuver event");
      pending_requests_.Remove(event_id);
      navi_alert_maneuver_result_code_ =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      GetInfo(message, info_navi_);
      break;
    }
    case hmi_apis::FunctionID::TTS_Speak: {
      LOG4CXX_INFO(logger_, "Received TTS_Speak event");
      pending_requests_.Remove(event_id);
      tts_speak_result_code_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, info_tts_);
      break;
    }
    case hmi_apis::FunctionID::TTS_OnResetTimeout: {
      LOG4CXX_INFO(logger_, "Received TTS_OnResetTimeout event");

      application_manager_.updateRequestTimeout(
          connection_key(), correlation_id(), default_timeout());
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      SendResponse(
          false, mobile_apis::Result::INVALID_ENUM, "Received unknown event");
      return;
    }
  }

  if (!pending_requests_.IsFinal(event_id)) {
    LOG4CXX_DEBUG(logger_,
                  "There are some pending responses from HMI."
                  "AlertManeuverRequest still waiting.");
    return;
  }
  std::string return_info;
  mobile_apis::Result::eType result_code;
  const bool result = PrepareResponseParameters(result_code, return_info);
  bool must_be_empty_info = false;
  if (return_info.find("\n") != std::string::npos ||
      return_info.find("\t") != std::string::npos) {
    must_be_empty_info = true;
  }
  SendResponse(result,
               result_code,
               (must_be_empty_info) ? NULL : return_info.c_str(),
               &(message[strings::msg_params]));
}

bool AlertManeuverRequest::PrepareResponseParameters(
    mobile_apis::Result::eType& result_code, std::string& return_info) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;

  application_manager::commands::ResponseInfo navigation_alert_info(
      navi_alert_maneuver_result_code_,
      HmiInterfaces::HMI_INTERFACE_Navigation);

  application_manager::commands::ResponseInfo tts_alert_info(
      tts_speak_result_code_, HmiInterfaces::HMI_INTERFACE_TTS);
  const bool result =
      PrepareResultForMobileResponse(navigation_alert_info, tts_alert_info);

  if (result && (hmi_apis::Common_Result::UNSUPPORTED_RESOURCE ==
                     tts_speak_result_code_ &&
                 (HmiInterfaces::STATE_AVAILABLE ==
                  application_manager_.hmi_interfaces().GetInterfaceState(
                      HmiInterfaces::HMI_INTERFACE_TTS)))) {
    result_code = mobile_apis::Result::WARNINGS;
    return_info = std::string("Unsupported phoneme type sent in a prompt");
    return result;
  }
  result_code =
      PrepareResultCodeForResponse(navigation_alert_info, tts_alert_info);
  return_info =
      MergeInfos(navigation_alert_info, info_navi_, tts_alert_info, info_tts_);
  return result;
}

bool AlertManeuverRequest::IsWhiteSpaceExist() {
  LOG4CXX_AUTO_TRACE(logger_);
  const char* str = NULL;

  if ((*message_)[strings::msg_params].keyExists(strings::tts_chunks)) {
    const smart_objects::SmartArray* tc_array =
        (*message_)[strings::msg_params][strings::tts_chunks].asArray();

    smart_objects::SmartArray::const_iterator it_tc = tc_array->begin();
    smart_objects::SmartArray::const_iterator it_tc_end = tc_array->end();

    for (; it_tc != it_tc_end; ++it_tc) {
      str = (*it_tc)[strings::text].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_, "Invalid tts_chunks syntax check failed");
        return true;
      }
    }
  }
  return false;
}

}  // namespace commands

}  // namespace application_manager
