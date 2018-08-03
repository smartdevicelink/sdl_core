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

#include "sdl_rpc_plugin/commands/mobile/reset_global_properties_request.h"

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"

#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

ResetGlobalPropertiesRequest::ResetGlobalPropertiesRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler)
    , ui_result_(hmi_apis::Common_Result::INVALID_ENUM)
    , tts_result_(hmi_apis::Common_Result::INVALID_ENUM) {}

ResetGlobalPropertiesRequest::~ResetGlobalPropertiesRequest() {}

void ResetGlobalPropertiesRequest::Run() {
  ResetGlobalProperties();
}

void ResetGlobalPropertiesRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_SetGlobalProperties: {
      LOG4CXX_INFO(logger_, "Received UI_SetGlobalProperties event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
      ui_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, ui_response_info_);
      break;
    }
    case hmi_apis::FunctionID::TTS_SetGlobalProperties: {
      LOG4CXX_INFO(logger_, "Received TTS_SetGlobalProperties event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
      tts_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, tts_response_info_);
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      return;
    }
  }

  if (IsPendingResponseExist()) {
    LOG4CXX_DEBUG(logger_, "Waiting for remaining responses");
    return;
  }

  mobile_apis::Result::eType result_code = mobile_apis::Result::INVALID_ENUM;
  std::string response_info;
  const bool result = PrepareResponseParameters(result_code, response_info);

  SendResponse(result,
               static_cast<mobile_apis::Result::eType>(result_code),
               response_info.empty() ? NULL : response_info.c_str(),
               &(message[strings::msg_params]));
}

bool ResetGlobalPropertiesRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

bool ResetGlobalPropertiesRequest::PrepareResponseParameters(
    mobile_apis::Result::eType& out_result_code,
    std::string& out_response_info) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;

  bool result = false;
  app_mngr::commands::ResponseInfo ui_properties_info(
      ui_result_, HmiInterfaces::HMI_INTERFACE_UI, application_manager_);
  app_mngr::commands::ResponseInfo tts_properties_info(
      tts_result_, HmiInterfaces::HMI_INTERFACE_TTS, application_manager_);

  HmiInterfaces::InterfaceState tts_interface_state =
      application_manager_.hmi_interfaces().GetInterfaceState(
          HmiInterfaces::HMI_INTERFACE_TTS);

  if (hmi_apis::Common_Result::SUCCESS == ui_result_ &&
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE == tts_result_ &&
      HmiInterfaces::STATE_AVAILABLE == tts_interface_state) {
    result = true;
    out_result_code = mobile_apis::Result::WARNINGS;
    out_response_info = "Unsupported phoneme type sent in a prompt";
  } else {
    result =
        PrepareResultForMobileResponse(ui_properties_info, tts_properties_info);
    out_result_code =
        PrepareResultCodeForResponse(ui_properties_info, tts_properties_info);
    out_response_info = app_mngr::commands::MergeInfos(tts_properties_info,
                                                       tts_response_info_,
                                                       ui_properties_info,
                                                       ui_response_info_);
  }

  return result;
}

bool ResetGlobalPropertiesRequest::IsPendingResponseExist() {
  return IsInterfaceAwaited(HmiInterfaces::HMI_INTERFACE_TTS) ||
         IsInterfaceAwaited(HmiInterfaces::HMI_INTERFACE_UI);
}

void ResetGlobalPropertiesRequest::ResetGlobalProperties() {
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t app_id =
      (*message_)[strings::params][strings::connection_key].asUInt();
  ApplicationSharedPtr app = application_manager_.application(app_id);

  const smart_objects::SmartObject& global_properties =
      (*message_)[strings::msg_params][strings::properties];

  if (!app) {
    LOG4CXX_ERROR(logger_, "No application associated with session key");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  auto result =
      application_manager_.ResetGlobalProperties(global_properties, app_id);

  if (result.HasUIPropertiesReset()) {
    StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  }

  if (result.HasTTSPropertiesReset()) {
    StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
  }

  app->set_reset_global_properties_active(true);

  if (result.HasUIPropertiesReset()) {
    // create ui request
    smart_objects::SmartObjectSPtr msg_params =
        MessageHelper::CreateUIResetGlobalPropertiesRequest(result, app);
    if (msg_params.get()) {
      SendHMIRequest(
          hmi_apis::FunctionID::UI_SetGlobalProperties, msg_params.get(), true);
    }
  }

  if (result.HasTTSPropertiesReset()) {
    smart_objects::SmartObjectSPtr msg_params =
        MessageHelper::CreateTTSResetGlobalPropertiesRequest(result, app);

    SendHMIRequest(
        hmi_apis::FunctionID::TTS_SetGlobalProperties, msg_params.get(), true);
  }
}

}  // namespace commands

}  // namespace application_manager
