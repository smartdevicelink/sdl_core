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

 Neither the name of the copyright holders nor the names of their contributors
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

#include "rc_rpc_plugin/commands/mobile/get_interior_vehicle_data_request.h"
#include "interfaces/MOBILE_API.h"
#include "rc_rpc_plugin/rc_helpers.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

using namespace json_keys;
using namespace message_params;

SDL_CREATE_LOG_VARIABLE("Commands")

GetInteriorVehicleDataRequest::GetInteriorVehicleDataRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    const RCCommandParams& params)
    : RCCommandRequest(message, params)
    , excessive_subscription_occured_(false) {}

bool GetInteriorVehicleDataRequest::ProcessCapabilities() {
  SDL_LOG_AUTO_TRACE();
  const auto rc_capability = hmi_capabilities_.rc_capability();

  const std::string module_type = ModuleType();
  const std::string module_id = ModuleId();
  const ModuleUid module(module_type, module_id);
  if (rc_capability &&
      !rc_capabilities_manager_.CheckIfModuleExistsInCapabilities(module)) {
    SDL_LOG_WARN("Accessing not supported module: " << module_type << " "
                                                    << module_id);
    SetResourceState(ModuleType(), ResourceState::FREE);
    SendResponse(false,
                 mobile_apis::Result::UNSUPPORTED_RESOURCE,
                 "Accessing not supported module data");
    return false;
  }
  return true;
}

void GetInteriorVehicleDataRequest::FilterDisabledModuleData(
    smart_objects::SmartObject& module_data) {
  // If radioEnable is false, remove all other radio parameters from the
  // message.
  if (module_data.keyExists(message_params::kRadioEnable) &&
      module_data[message_params::kRadioEnable].asBool() == false) {
    for (auto data = module_data.map_begin(); data != module_data.map_end();) {
      auto key = data->first;
      ++data;
      if (key != message_params::kRadioEnable) {
        module_data.erase(key);
      }
    }
  }
  // If hdRadioEnable is false, find and remove the HDChannel if parameter is
  // present.
  if (module_data.keyExists(message_params::kHdRadioEnable) &&
      module_data[message_params::kHdRadioEnable].asBool() == false) {
    module_data.erase(message_params::kHdChannel);
    module_data.erase(message_params::kAvailableHDs);
    module_data.erase(message_params::kAvailableHdChannels);
    module_data.erase(message_params::kSisData);
  }
}

void GetInteriorVehicleDataRequest::ProcessResponseToMobileFromCache(
    app_mngr::ApplicationSharedPtr app) {
  SDL_LOG_AUTO_TRACE();
  const auto& data_mapping = RCHelpers::GetModuleTypeToDataMapping();
  const std::string module_type = ModuleType();
  const std::string module_id = ModuleId();
  const ModuleUid module(module_type, module_id);
  auto data = interior_data_cache_.Retrieve(module);
  FilterDisabledModuleData(data);
  auto response_msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  response_msg_params[message_params::kModuleData][data_mapping(module_type)] =
      data;
  response_msg_params[message_params::kModuleData]
                     [message_params::kModuleType] = module_type;

  response_msg_params[message_params::kModuleData][message_params::kModuleId] =
      module_id;

  const auto& request_msg_params = (*message_)[app_mngr::strings::msg_params];
  SDL_LOG_DEBUG("kSubscribe exist"
                << request_msg_params.keyExists(message_params::kSubscribe));

  mobile_apis::Result::eType result_code = mobile_apis::Result::SUCCESS;
  if (request_msg_params.keyExists(message_params::kSubscribe)) {
    response_msg_params[message_params::kIsSubscribed] =
        request_msg_params[message_params::kSubscribe].asBool();
    if (request_msg_params[message_params::kSubscribe].asBool()) {
      auto extension = RCHelpers::GetRCExtension(*app);
      DCHECK(extension);
      const bool is_app_already_subscribed =
          extension->IsSubscribedToInteriorVehicleData(module);
      if (is_app_already_subscribed) {
        response_msg_params[app_mngr::strings::info] =
            "App is already subscribed to the provided module";
        result_code = mobile_apis::Result::WARNINGS;
      } else {
        extension->SubscribeToInteriorVehicleData(module);
        app->UpdateHash();
      }
    }
  }
  SendResponse(true, result_code, nullptr, &response_msg_params);
  if (AppShouldBeUnsubscribed()) {
    auto extension = RCHelpers::GetRCExtension(*app);
    DCHECK(extension);
    if (extension->IsSubscribedToInteriorVehicleData(module)) {
      extension->UnsubscribeFromInteriorVehicleData(module);
      app->UpdateHash();
    }
  }
}

bool GetInteriorVehicleDataRequest::CheckRateLimits() {
  SDL_LOG_AUTO_TRACE();
  const std::string module_type = ModuleType();
  const std::string module_id = ModuleId();
  const ModuleUid module(module_type, module_id);
  return interior_data_manager_.CheckRequestsToHMIFrequency(module);
}

bool GetInteriorVehicleDataRequest::AppShouldBeUnsubscribed() {
  SDL_LOG_AUTO_TRACE();
  const auto& msg_params = (*message_)[app_mngr::strings::msg_params];
  if (msg_params.keyExists(message_params::kSubscribe)) {
    return !(msg_params[message_params::kSubscribe].asBool());
  }
  return false;
}

bool GetInteriorVehicleDataRequest::TheLastAppShouldBeUnsubscribed(
    app_mngr::ApplicationSharedPtr app) {
  SDL_LOG_AUTO_TRACE();
  if (AppShouldBeUnsubscribed()) {
    const std::string module_type = ModuleType();
    const std::string module_id = ModuleId();
    const ModuleUid module(module_type, module_id);
    const auto subscribed_to_module_type =
        RCHelpers::AppsSubscribedToModule(application_manager_, module);
    if (subscribed_to_module_type.size() == 1 &&
        subscribed_to_module_type.front() == app) {
      SDL_LOG_DEBUG("The last application unsubscribes from "
                    << module_type << " " << module_id);
      return true;
    }
  }
  return false;
}

void GetInteriorVehicleDataRequest::Execute() {
  SDL_LOG_AUTO_TRACE();

  if (!ProcessCapabilities()) {
    return;
  }

  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(connection_key());
  const std::string module_type = ModuleType();
  const std::string module_id = ModuleId();
  const ModuleUid module(module_type, module_id);

  if (TheLastAppShouldBeUnsubscribed(app) ||
      !interior_data_cache_.Contains(module)) {
    if (HasRequestExcessiveSubscription()) {
      excessive_subscription_occured_ = true;
      is_subscribed =
          (*message_)[app_mngr::strings::msg_params][message_params::kSubscribe]
              .asBool();
      RemoveExcessiveSubscription();
    }
    if (!CheckRateLimits()) {
      SDL_LOG_WARN("GetInteriorVehicleData frequency is too high.");
      SendResponse(false, mobile_apis::Result::REJECTED);
      return;
    }
    interior_data_manager_.StoreRequestToHMITime(module);
    (*message_)[app_mngr::strings::msg_params][message_params::kModuleId] =
        ModuleId();
    SendHMIRequest(hmi_apis::FunctionID::RC_GetInteriorVehicleData,
                   &(*message_)[app_mngr::strings::msg_params],
                   true);
    return;
  }
  ProcessResponseToMobileFromCache(app);
}

void GetInteriorVehicleDataRequest::on_event(
    const app_mngr::event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  RCCommandRequest::on_event(event);

  if (hmi_apis::FunctionID::RC_GetInteriorVehicleData != event.id()) {
    return;
  }

  smart_objects::SmartObject& hmi_response =
      const_cast<smart_objects::SmartObject&>(event.smart_object());

  mobile_apis::Result::eType result_code =
      GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
          hmi_response[app_mngr::strings::params][app_mngr::hmi_response::code]
              .asUInt()));

  bool result =
      helpers::Compare<mobile_apis::Result::eType, helpers::EQ, helpers::ONE>(
          result_code,
          mobile_apis::Result::SUCCESS,
          mobile_apis::Result::WARNINGS,
          mobile_apis::Result::WRONG_LANGUAGE,
          mobile_apis::Result::RETRY,
          mobile_apis::Result::SAVED);

  if (mobile_apis::Result::READ_ONLY == result_code) {
    result = false;
    result_code = mobile_apis::Result::GENERIC_ERROR;
  }

  if (result) {
    if (!IsModuleIdProvided(hmi_response)) {
      SDL_LOG_WARN("conditional mandatory parameter "
                   << message_params::kModuleId << " missed in hmi response");
      result = false;
      result_code = mobile_apis::Result::GENERIC_ERROR;
    }
    app_mngr::ApplicationSharedPtr app =
        application_manager_.application(connection_key());

    DCHECK_OR_RETURN_VOID(app);
    const std::string module_type = ModuleType();
    const std::string module_id = ModuleId();
    const ModuleUid module(module_type, module_id);

    if (TheLastAppShouldBeUnsubscribed(app)) {
      SDL_LOG_DEBUG("Removing module: [" << module.first << ":" << module.second
                                         << "] "
                                         << "from cache");
      interior_data_cache_.Remove(module);
    }
    ProccessSubscription(hmi_response);
    if (is_subscribed) {
      const auto& data_mapping = RCHelpers::GetModuleTypeToDataMapping();
      const auto module_data =
          hmi_response[app_mngr::strings::msg_params]
                      [message_params::kModuleData][data_mapping(module_type)];
      interior_data_cache_.Add(module, module_data);
    }
  } else {
    hmi_response[app_mngr::strings::msg_params].erase(
        message_params::kIsSubscribed);
  }
  std::string response_info;
  GetInfo(hmi_response, response_info);
  SetResourceState(ModuleType(), ResourceState::FREE);

  SendResponse(result,
               result_code,
               response_info.c_str(),
               &hmi_response[app_mngr::strings::msg_params]);
}

GetInteriorVehicleDataRequest::~GetInteriorVehicleDataRequest() {}

void GetInteriorVehicleDataRequest::ProccessSubscription(
    const ns_smart_device_link::ns_smart_objects::SmartObject& hmi_response) {
  SDL_LOG_AUTO_TRACE();

  const bool is_subscribe_present_in_request =
      (*message_)[app_mngr::strings::msg_params].keyExists(
          message_params::kSubscribe) ||
      excessive_subscription_occured_;

  const bool isSubscribed_present_in_response =
      hmi_response[app_mngr::strings::msg_params].keyExists(
          message_params::kIsSubscribed);

  smart_objects::SmartObject& temp_hmi_response =
      const_cast<smart_objects::SmartObject&>(hmi_response);

  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());
  const auto extension = RCHelpers::GetRCExtension(*app);
  const char* module_type;
  ns_smart_device_link::ns_smart_objects::
      EnumConversionHelper<mobile_apis::ModuleType::eType>::EnumToCString(
          static_cast<mobile_apis::ModuleType::eType>(
              hmi_response[app_mngr::strings::msg_params]
                          [message_params::kModuleData]
                          [message_params::kModuleType]
                              .asUInt()),
          &module_type);

  const std::string module_id =
      hmi_response[app_mngr::strings::msg_params][message_params::kModuleData]
                  [message_params::kModuleId]
                      .asString();
  const ModuleUid module(module_type, module_id);

  if (excessive_subscription_occured_) {
    is_subscribed = extension->IsSubscribedToInteriorVehicleData(module);
    temp_hmi_response[app_mngr::strings::msg_params]
                     [message_params::kIsSubscribed] = is_subscribed;
    return;
  }
  if (!is_subscribe_present_in_request && !isSubscribed_present_in_response) {
    return;
  }

  if (is_subscribe_present_in_request && !isSubscribed_present_in_response) {
    SDL_LOG_WARN("conditional mandatory parameter "
                 << message_params::kIsSubscribed << " missed in hmi response");

    is_subscribed = extension->IsSubscribedToInteriorVehicleData(module);
    temp_hmi_response[app_mngr::strings::msg_params]
                     [message_params::kIsSubscribed] = is_subscribed;
    return;
  }

  if (!is_subscribe_present_in_request && isSubscribed_present_in_response) {
    SDL_LOG_WARN("Parameter " << message_params::kIsSubscribed
                              << " is ignored due to absence '"
                              << message_params::kSubscribe
                              << "' parameter in request");
    smart_objects::SmartObject& temp_hmi_response =
        const_cast<smart_objects::SmartObject&>(hmi_response);
    temp_hmi_response[app_mngr::strings::msg_params].erase(
        message_params::kIsSubscribed);
    return;
  }

  const bool request_subscribe =
      (*message_)[app_mngr::strings::msg_params][message_params::kSubscribe]
          .asBool();
  const bool response_subscribe =
      hmi_response[app_mngr::strings::msg_params][message_params::kIsSubscribed]
          .asBool();
  is_subscribed = response_subscribe;

  SDL_LOG_TRACE("request_subscribe = " << request_subscribe);
  SDL_LOG_TRACE("response_subscribe = " << response_subscribe);
  if (request_subscribe == response_subscribe) {
    const std::string module_type = ModuleType();
    const std::string module_id = ModuleId();
    const ModuleUid module(module_type, module_id);

    if (response_subscribe &&
        !extension->IsSubscribedToInteriorVehicleData(module)) {
      SDL_LOG_DEBUG("SubscribeToInteriorVehicleData "
                    << app->app_id() << " " << module_type << " " << module_id);
      extension->SubscribeToInteriorVehicleData(module);
    } else if (!response_subscribe &&
               extension->IsSubscribedToInteriorVehicleData(module)) {
      SDL_LOG_DEBUG("UnsubscribeFromInteriorVehicleData "
                    << app->app_id() << " " << module_type << " " << module_id);
      extension->UnsubscribeFromInteriorVehicleData(module);
    }

    app->UpdateHash();
  }
}

bool GetInteriorVehicleDataRequest::HasRequestExcessiveSubscription() {
  SDL_LOG_AUTO_TRACE();
  const bool is_subscribe_present_in_request =
      (*message_)[app_mngr::strings::msg_params].keyExists(
          message_params::kSubscribe);

  if (is_subscribe_present_in_request) {
    app_mngr::ApplicationSharedPtr app =
        application_manager_.application(CommandRequestImpl::connection_key());
    const auto extension = RCHelpers::GetRCExtension(*app);

    const std::string module_type = ModuleType();
    const std::string module_id = ModuleId();
    const ModuleUid module(module_type, module_id);

    const bool is_app_already_subscribed =
        extension->IsSubscribedToInteriorVehicleData(module);
    const bool app_wants_to_subscribe =
        (*message_)[app_mngr::strings::msg_params][message_params::kSubscribe]
            .asBool();
    if (!app_wants_to_subscribe && !is_app_already_subscribed) {
      return true;
    }
    return app_wants_to_subscribe && is_app_already_subscribed;
  }
  return false;
}

void GetInteriorVehicleDataRequest::RemoveExcessiveSubscription() {
  SDL_LOG_AUTO_TRACE();
  (*message_)[app_mngr::strings::msg_params].erase(message_params::kSubscribe);
}

std::string GetInteriorVehicleDataRequest::ModuleType() const {
  SDL_LOG_AUTO_TRACE();
  mobile_apis::ModuleType::eType module_type =
      static_cast<mobile_apis::ModuleType::eType>(
          (*message_)[app_mngr::strings::msg_params]
                     [message_params::kModuleType]
                         .asUInt());
  const char* str;
  const bool ok = ns_smart_device_link::ns_smart_objects::EnumConversionHelper<
      mobile_apis::ModuleType::eType>::EnumToCString(module_type, &str);
  return ok ? str : "unknown";
}

std::string GetInteriorVehicleDataRequest::ModuleId() const {
  SDL_LOG_AUTO_TRACE();
  auto msg_params = (*message_)[app_mngr::strings::msg_params];
  if (msg_params.keyExists(message_params::kModuleId)) {
    return msg_params[message_params::kModuleId].asString();
  }
  const std::string module_id =
      rc_capabilities_manager_.GetDefaultModuleIdFromCapabilities(ModuleType());
  return module_id;
}

}  // namespace commands
}  // namespace rc_rpc_plugin
