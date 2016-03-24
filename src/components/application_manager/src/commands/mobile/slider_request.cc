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

#include "application_manager/commands/mobile/slider_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "utils/helpers.h"
#include "config_profile/profile.h"

namespace application_manager {

namespace commands {

SliderRequest::SliderRequest(const MessageSharedPtr& message)
 : CommandRequestImpl(message) {
  subscribe_on_event(hmi_apis::FunctionID::UI_OnResetTimeout);
}

SliderRequest::~SliderRequest() {
}

bool SliderRequest::Init() {

  /* Timeout in milliseconds.
     If omitted a standard value of 10000 milliseconds is used.*/
  if ((*message_)[strings::msg_params].keyExists(strings::timeout)) {
    default_timeout_ = profile::Profile::instance()->default_timeout() +
        (*message_)[strings::msg_params][strings::timeout].asUInt();
  }

  return true;
}

void SliderRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr application =
      application_manager::ApplicationManagerImpl::instance()->application(
          (*message_)[strings::params][strings::connection_key].asUInt());

  if (!application) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if ((*message_)[strings::msg_params][strings::num_ticks].asInt() <
      (*message_)[strings::msg_params][strings::position].asInt()) {
    LOG4CXX_ERROR(logger_, "INVALID_DATA");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::slider_footer)) {
    if (1 < (*message_)[strings::msg_params][strings::slider_footer].length()) {
      if ((*message_)[strings::msg_params][strings::num_ticks].asUInt()
          != (*message_)[strings::msg_params]
                         [strings::slider_footer].length()) {
        LOG4CXX_ERROR(logger_, "INVALID_DATA");
        SendResponse(false, mobile_apis::Result::INVALID_DATA);
        return;
      }
    }
  }

  if (IsWhiteSpaceExist()) {
    LOG4CXX_ERROR(logger_, "Incoming slider has contains \t\n \\t \\n");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);
  msg_params = (*message_)[strings::msg_params];
  msg_params[strings::app_id] = application->app_id();

  if (!(*message_)[strings::msg_params].keyExists(strings::timeout)) {
    msg_params[strings::timeout] = default_timeout_;
  }

  SendHMIRequest(hmi_apis::FunctionID::UI_Slider, &msg_params, true);
}

void SliderRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;
  using namespace smart_objects;
  using namespace hmi_apis;

  const SmartObject& message = event.smart_object();

  const event_engine::Event::EventID event_id = event.id();
  if (event_id == FunctionID::UI_OnResetTimeout) {
    LOG4CXX_INFO(logger_, "Received UI_OnResetTimeout event");
    ApplicationManagerImpl::instance()->updateRequestTimeout(connection_key(),
      correlation_id(),
      default_timeout());
    return;
  }

  if (event_id != FunctionID::UI_Slider) {
    LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
    return;
  }

  LOG4CXX_DEBUG(logger_, "Received UI_Slider event");

  const Common_Result::eType response_code = static_cast<Common_Result::eType>(
      message[strings::params][hmi_response::code].asInt());

  SmartObject response_msg_params = message[strings::msg_params];

  const bool is_timeout_aborted =
      Compare<Common_Result::eType, EQ, ONE>(
        response_code,
        Common_Result::TIMED_OUT,
        Common_Result::ABORTED);

  if (is_timeout_aborted) {
    if (message[strings::params][strings::data]
        .keyExists(strings::slider_position)) {
      //Copy slider_position info to msg_params section
      response_msg_params[strings::slider_position] =
          message[strings::params][strings::data][strings::slider_position];
    } else {
      LOG4CXX_ERROR(logger_, strings::slider_position << " field is absent"
                    " in response.");
      response_msg_params[strings::slider_position] = 0;
    }
  }

  const bool is_response_success =
      Compare<Common_Result::eType, EQ, ONE>(
        response_code,
        Common_Result::SUCCESS,
        Common_Result::WARNINGS);

  SendResponse(is_response_success,
               MessageHelper::HMIToMobileResult(response_code),
               0,
               &response_msg_params);
}

bool SliderRequest::IsWhiteSpaceExist() {
  LOG4CXX_AUTO_TRACE(logger_);
  const char* str = NULL;

  str = (*message_)[strings::msg_params][strings::slider_header].asCharArray();
  if (!CheckSyntax(str)) {
    LOG4CXX_ERROR(logger_, "Invalid slider_header value syntax check failed");
    return true;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::slider_footer)) {
    const smart_objects::SmartArray* sf_array =
        (*message_)[strings::msg_params][strings::slider_footer].asArray();

    smart_objects::SmartArray::const_iterator it_sf = sf_array->begin();
    smart_objects::SmartArray::const_iterator it_sf_end = sf_array->end();

    for (; it_sf != it_sf_end; ++it_sf) {
      str = (*it_sf).asCharArray();
      if (!CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_, "Invalid slider_footer syntax check failed");
        return true;
      }
    }
  }
  return false;
}

}  // namespace commands
}  // namespace application_manager

