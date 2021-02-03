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

#include "sdl_rpc_plugin/commands/mobile/set_media_clock_timer_request.h"

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/enum_schema_item.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

SetMediaClockRequest::SetMediaClockRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler) {}

SetMediaClockRequest::~SetMediaClockRequest() {}

void SetMediaClockRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    SDL_LOG_ERROR("Application is not registered");
    return;
  }

  if (!app->is_media_application()) {
    SDL_LOG_ERROR("Application is not media application");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  std::string info;
  if (isDataValid(info)) {
    // copy entirely msg
    smart_objects::SmartObject msg_params = (*message_)[strings::msg_params];
    msg_params[strings::app_id] = app->app_id();
    StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);

    SendHMIRequest(
        hmi_apis::FunctionID::UI_SetMediaClockTimer, &msg_params, true);
  } else {
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 info.empty() ? NULL : info.c_str());
  }
}

void SetMediaClockRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_SetMediaClockTimer: {
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
      hmi_apis::Common_Result::eType result_code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      const bool result = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_UI);
      std::string response_info;
      GetInfo(message, response_info);

      SendResponse(result,
                   MessageHelper::HMIToMobileResult(result_code),
                   response_info.empty() ? NULL : response_info.c_str(),
                   &(message[strings::msg_params]));
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event " << event.id());
      return;
    }
  }
}

bool SetMediaClockRequest::isDataValid(std::string& info) {
  smart_objects::SmartObject msg_params = (*message_)[strings::msg_params];
  mobile_apis::UpdateMode::eType update_mode =
      static_cast<mobile_apis::UpdateMode::eType>(
          msg_params[strings::update_mode].asUInt());

  if (update_mode == mobile_apis::UpdateMode::COUNTUP ||
      update_mode == mobile_apis::UpdateMode::COUNTDOWN) {
    if (!msg_params.keyExists(strings::start_time)) {
      info =
          "Start time must be provided for \"COUNTUP\" and \"COUNTDOWN\" "
          "update modes";
      SDL_LOG_INFO("Invalid data: " << info);
      return false;
    }

    if (msg_params.keyExists(strings::end_time)) {
      unsigned int start_time_in_seconds = 0;
      start_time_in_seconds =
          (msg_params[strings::start_time][strings::hours].asUInt()) * 3600;
      start_time_in_seconds +=
          (msg_params[strings::start_time][strings::minutes].asUInt()) * 60;
      start_time_in_seconds +=
          (msg_params[strings::start_time][strings::seconds].asUInt());

      unsigned int end_time_in_seconds = 0;
      end_time_in_seconds =
          (msg_params[strings::end_time][strings::hours].asUInt()) * 3600;
      end_time_in_seconds +=
          (msg_params[strings::end_time][strings::minutes].asUInt()) * 60;
      end_time_in_seconds +=
          (msg_params[strings::end_time][strings::seconds].asUInt());

      if (((end_time_in_seconds > start_time_in_seconds) &&
           (update_mode == mobile_apis::UpdateMode::COUNTDOWN)) ||
          ((end_time_in_seconds < start_time_in_seconds) &&
           (update_mode == mobile_apis::UpdateMode::COUNTUP))) {
        std::string update_mode_name;
        smart_objects::EnumConversionHelper<
            mobile_apis::UpdateMode::eType>::EnumToString(update_mode,
                                                          &update_mode_name);
        info = "Start time must be " +
               std::string((update_mode == mobile_apis::UpdateMode::COUNTUP)
                               ? "before"
                               : "after") +
               " the end time for update mode " + update_mode_name;
        SDL_LOG_INFO("Invalid data: " << info);
        return false;
      }
    }
  }

  std::vector<std::string> indicator_keys{strings::forward_seek_indicator,
                                          strings::back_seek_indicator};
  for (auto& key : indicator_keys) {
    if (msg_params.keyExists(key)) {
      mobile_apis::SeekIndicatorType::eType seek_indicator_type =
          static_cast<mobile_apis::SeekIndicatorType::eType>(
              msg_params[key][strings::type].asUInt());
      if (seek_indicator_type == mobile_apis::SeekIndicatorType::TRACK &&
          msg_params[key].keyExists(strings::seek_time)) {
        info =
            "The seekTime parameter is not applicable for indicator type TRACK";
        SDL_LOG_INFO("Invalid data: " << info);
        return false;
      }
    }
  }

  SDL_LOG_INFO("Data is valid");
  return true;
}

}  // namespace commands

}  // namespace sdl_rpc_plugin
