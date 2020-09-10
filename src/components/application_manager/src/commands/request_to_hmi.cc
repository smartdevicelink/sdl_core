/*
 Copyright (c) 2014, Ford Motor Company
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

#include "application_manager/commands/request_to_hmi.h"
#include "application_manager/message_helper.h"
#include "application_manager/rpc_service.h"
#include "utils/helpers.h"

namespace application_manager {

namespace {
static const std::set<hmi_apis::FunctionID::eType> tts_request_ids{
    hmi_apis::FunctionID::TTS_GetLanguage,
    hmi_apis::FunctionID::TTS_GetCapabilities,
    hmi_apis::FunctionID::TTS_GetSupportedLanguages};

static const std::set<hmi_apis::FunctionID::eType> vr_request_ids{
    hmi_apis::FunctionID::VR_GetLanguage,
    hmi_apis::FunctionID::VR_GetCapabilities,
    hmi_apis::FunctionID::VR_GetSupportedLanguages};

static const std::set<hmi_apis::FunctionID::eType> ui_request_ids{
    hmi_apis::FunctionID::UI_GetLanguage,
    hmi_apis::FunctionID::UI_GetCapabilities,
    hmi_apis::FunctionID::UI_GetSupportedLanguages};

static const std::set<hmi_apis::FunctionID::eType> rc_request_ids{
    hmi_apis::FunctionID::RC_GetCapabilities};

static const std::set<hmi_apis::FunctionID::eType> vehicle_info_request_ids{
    hmi_apis::FunctionID::VehicleInfo_GetVehicleType};

static std::map<std::string, std::set<hmi_apis::FunctionID::eType> >
    interface_requests{
        {std::string(hmi_interface::ui), ui_request_ids},
        {std::string(hmi_interface::vr), vr_request_ids},
        {std::string(hmi_interface::tts), tts_request_ids},
        {std::string(hmi_interface::rc), rc_request_ids},
        {std::string(hmi_interface::vehicle_info), vehicle_info_request_ids}};

}  // namespace

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

bool CheckAvailabilityHMIInterfaces(ApplicationManager& application_manager,
                                    HmiInterfaces::InterfaceID interface) {
  const HmiInterfaces::InterfaceState state =
      application_manager.hmi_interfaces().GetInterfaceState(interface);
  return HmiInterfaces::STATE_NOT_AVAILABLE != state;
}

bool IsResponseCodeSuccess(
    const smart_objects::SmartObject& response_from_hmi) {
  auto response_code = static_cast<hmi_apis::Common_Result::eType>(
      response_from_hmi[strings::params][hmi_response::code].asInt());

  using helpers::Compare;
  using helpers::EQ;
  using helpers::ONE;

  const bool is_result_success =
      Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
          response_code,
          hmi_apis::Common_Result::SUCCESS,
          hmi_apis::Common_Result::WARNINGS,
          hmi_apis::Common_Result::WRONG_LANGUAGE,
          hmi_apis::Common_Result::RETRY,
          hmi_apis::Common_Result::SAVED);
  return is_result_success;
}

bool ChangeInterfaceState(ApplicationManager& application_manager,
                          const smart_objects::SmartObject& response_from_hmi,
                          HmiInterfaces::InterfaceID interface) {
  if (response_from_hmi[strings::msg_params].keyExists(strings::available)) {
    const bool is_available =
        response_from_hmi[strings::msg_params][strings::available].asBool();

    if (!is_available) {
      application_manager.hmi_interfaces().SetInterfaceState(
          interface, HmiInterfaces::STATE_NOT_AVAILABLE);
      return false;
    }

    // Process response with result
    if (response_from_hmi[strings::params].keyExists(hmi_response::code) &&
        !IsResponseCodeSuccess(response_from_hmi)) {
      application_manager.hmi_interfaces().SetInterfaceState(
          interface, HmiInterfaces::STATE_NOT_AVAILABLE);
      return false;
    }

    application_manager.hmi_interfaces().SetInterfaceState(
        interface, HmiInterfaces::STATE_AVAILABLE);
    return true;
  }

  // Process response with error
  if (response_from_hmi[strings::params].keyExists(strings::error_msg)) {
    application_manager.hmi_interfaces().SetInterfaceState(
        interface, HmiInterfaces::STATE_NOT_AVAILABLE);
    return false;
  }

  return false;
}

RequestToHMI::RequestToHMI(const MessageSharedPtr& message,
                           ApplicationManager& application_manager,
                           rpc_service::RPCService& rpc_service,
                           HMICapabilities& hmi_capabilities,
                           policy::PolicyHandlerInterface& policy_handler)
    : CommandImpl(message,
                  application_manager,
                  rpc_service,
                  hmi_capabilities,
                  policy_handler) {}

RequestToHMI::~RequestToHMI() {}

bool RequestToHMI::Init() {
  // Replace Mobile connection id with HMI app id
  return ReplaceMobileWithHMIAppId(*message_);
}

bool RequestToHMI::CleanUp() {
  return true;
}

void RequestToHMI::Run() {
  SendRequest();
}

void RequestToHMI::SendRequest() {
  (*message_)[strings::params][strings::protocol_type] = hmi_protocol_type_;
  (*message_)[strings::params][strings::protocol_version] = protocol_version_;
  rpc_service_.SendMessageToHMI(message_);
}

void RequestToHMI::RequestInterfaceCapabilities(const char* interface_name) {
  SDL_LOG_DEBUG("Request capabilities for the " << interface_name
                                                << " interface");

  const auto& request_ids = interface_requests[std::string(interface_name)];
  RequestCapabilities(request_ids);
}

void RequestToHMI::UpdateRequestsRequiredForCapabilities(
    const std::set<hmi_apis::FunctionID::eType>& requests_to_send_to_hmi) {
  for (auto request_id : requests_to_send_to_hmi) {
    hmi_capabilities_.UpdateRequestsRequiredForCapabilities(request_id);
  }
}

void RequestToHMI::UpdateRequiredInterfaceCapabilitiesRequests(
    const std::string& interface_name) {
  SDL_LOG_DEBUG("Update requests required for the " << interface_name
                                                    << " interface");

  const auto& request_ids = interface_requests[std::string(interface_name)];
  UpdateRequestsRequiredForCapabilities(request_ids);
}

void RequestToHMI::RequestCapabilities(
    const std::set<hmi_apis::FunctionID::eType>& requests_to_send_to_hmi) {
  SDL_LOG_DEBUG("There are " << requests_to_send_to_hmi.size()
                             << " requests to send to the HMI");

  for (const auto& function_id : requests_to_send_to_hmi) {
    if (hmi_capabilities_.IsRequestsRequiredForCapabilities(function_id)) {
      std::shared_ptr<smart_objects::SmartObject> request_so(
          MessageHelper::CreateModuleInfoSO(function_id, application_manager_));

      switch (function_id) {
        case hmi_apis::FunctionID::UI_GetLanguage:
        case hmi_apis::FunctionID::VR_GetLanguage:
        case hmi_apis::FunctionID::TTS_GetLanguage:
          hmi_capabilities_.set_handle_response_for(*request_so);
          break;
        default:
          break;
      }
      rpc_service_.ManageHMICommand(request_so);
    }
  }
}

}  // namespace commands

}  // namespace application_manager
