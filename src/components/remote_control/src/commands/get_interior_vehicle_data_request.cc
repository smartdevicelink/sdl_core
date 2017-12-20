/*
 Copyright (c) 2017, Ford Motor Company
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

#include <map>

#include "remote_control/commands/get_interior_vehicle_data_request.h"
#include "remote_control/rc_module_constants.h"
#include "remote_control/message_helper.h"
#include "remote_control/remote_control_plugin.h"
#include "functional_module/function_ids.h"
#include "json/json.h"

namespace remote_control {

namespace commands {

using namespace enums_value;
using namespace json_keys;
using namespace message_params;

CREATE_LOGGERPTR_GLOBAL(logger_, "GetInteriorVehicleDataRequest")

GetInteriorVehicleDataRequest::GetInteriorVehicleDataRequest(
    const application_manager::MessagePtr& message,
    RemotePluginInterface& rc_module)
    : BaseCommandRequest(message, rc_module) {}

bool CheckIfModuleTypeExistInCapabilities(
    const smart_objects::SmartObject& rc_capabilities,
    const std::string& module_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (enums_value::kRadio == module_type &&
      !rc_capabilities.keyExists(strings::kradioControlCapabilities)) {
    LOG4CXX_DEBUG(logger_, " Radio control capabilities not present");
    return false;
  }
  if (enums_value::kClimate == module_type &&
      !rc_capabilities.keyExists(strings::kclimateControlCapabilities)) {
    LOG4CXX_DEBUG(logger_, " Climate control capabilities not present");
    return false;
  }

  return true;
}

void GetInteriorVehicleDataRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);
  Json::Value request_params =
      MessageHelper::StringToValue(message_->json_message());

  {  // A local scope to limit accessor's lifetime and release app list lock.
    const DataAccessor<const smart_objects::SmartObject *> rc_capabilities_accessor = service()->GetRCCapabilities();
    const smart_objects::SmartObject* capabilities = rc_capabilities_accessor.GetData();

    if (capabilities &&
        !CheckIfModuleTypeExistInCapabilities(*capabilities,
                                              ModuleType(request_params))) {
      LOG4CXX_WARN(logger_, "Accessing not supported module data");
      SendResponse(false,
                   result_codes::kUnsupportedResource,
                   "Accessing not supported module data");
      return;
    }
  }
  if (HasRequestExcessiveSubscription(request_params)) {
    RemoveExcessiveSubscription(request_params);
  }

  SendRequest(functional_modules::hmi_api::get_interior_vehicle_data,
              request_params);
}

void GetInteriorVehicleDataRequest::OnEvent(
    const rc_event_engine::Event<application_manager::MessagePtr, std::string>&
        event) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(
      (functional_modules::hmi_api::get_interior_vehicle_data == event.id()));

  application_manager::Message& hmi_response = *(event.event_message());
  const bool validate_result =
      application_manager::MessageValidationResult::SUCCESS ==
      service()->ValidateMessageBySchema(hmi_response);
  LOG4CXX_DEBUG(logger_,
                "HMI response validation result is " << validate_result);
  const Json::Value value =
      MessageHelper::StringToValue(hmi_response.json_message());
  std::string result_code;
  std::string info;
  bool success = validate_result && ParseResultCode(value, result_code, info);

  if (!validate_result ||
      remote_control::result_codes::kReadOnly == result_code) {
    success = false;
    result_code = result_codes::kGenericError;
  }

  if (success) {
    ProccessSubscription(value);
    response_params_[kModuleData] =
        value[json_keys::kResult][message_params::kModuleData];
  }
  SendResponse(success, result_code.c_str(), info);
}

void GetInteriorVehicleDataRequest::ProccessSubscription(
    const Json::Value& hmi_response) {
  LOG4CXX_AUTO_TRACE(logger_);
  Json::Value request_params;
  Json::Reader reader;
  reader.parse(message_->json_message(), request_params);

  const bool is_subscribe_present_in_request =
      IsMember(request_params, kSubscribe);
  const bool isSubscribed_present_in_response =
      IsMember(hmi_response[json_keys::kResult], message_params::kIsSubscribed);

  if (!is_subscribe_present_in_request && !isSubscribed_present_in_response) {
    return;
  }

  RCAppExtensionPtr extension = GetAppExtension(app());
  if (is_subscribe_present_in_request && !isSubscribed_present_in_response) {
    LOG4CXX_WARN(logger_,
                 "conditional mandatory parameter "
                     << kIsSubscribed << " missed in hmi response");
    response_params_[kIsSubscribed] =
        extension->IsSubscibedToInteriorVehicleData(
            request_params[kModuleType]);
    return;
  }

  if (!is_subscribe_present_in_request && isSubscribed_present_in_response) {
    LOG4CXX_WARN(logger_,
                 "Parameter " << message_params::kIsSubscribed
                              << " is ignored due to absence '"
                              << message_params::kSubscribe
                              << "' parameter in request");
    return;
  }

  const bool request_subscribe = request_params[kSubscribe].asBool();
  const bool response_subscribe =
      hmi_response[json_keys::kResult][message_params::kIsSubscribed].asBool();
  response_params_[message_params::kIsSubscribed] = response_subscribe;
  LOG4CXX_TRACE(logger_, "request_subscribe = " << request_subscribe);
  LOG4CXX_TRACE(logger_, "response_subscribe = " << response_subscribe);
  if (request_subscribe == response_subscribe) {
    if (response_subscribe) {
      LOG4CXX_DEBUG(logger_,
                    "SubscribeToInteriorVehicleData "
                        << app()->app_id() << " "
                        << request_params[kModuleType].asString());
      extension->SubscribeToInteriorVehicleData(request_params[kModuleType]);
    } else {
      LOG4CXX_DEBUG(logger_,
                    "UnsubscribeFromInteriorVehicleData "
                        << app()->app_id() << " "
                        << request_params[kModuleType].asString());
      extension->UnsubscribeFromInteriorVehicleData(
          request_params[kModuleType]);
    }
  }
}

bool GetInteriorVehicleDataRequest::HasRequestExcessiveSubscription(
    const Json::Value& request_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  const bool is_subscribe_present_in_request =
      IsMember(request_params, kSubscribe);
  if (is_subscribe_present_in_request) {
    RCAppExtensionPtr extension = GetAppExtension(app());
    const bool is_app_already_subscribed =
        extension->IsSubscibedToInteriorVehicleData(
            request_params[kModuleType]);
    const bool app_wants_to_subscribe = request_params[kSubscribe].asBool();
    if (!app_wants_to_subscribe && !is_app_already_subscribed) {
      return true;
    }
    return app_wants_to_subscribe && is_app_already_subscribed;
  }
  return false;
}

void GetInteriorVehicleDataRequest::RemoveExcessiveSubscription(
    Json::Value& request_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  request_params.removeMember(kSubscribe);
}

std::string GetInteriorVehicleDataRequest::ModuleType(
    const Json::Value& message) {
  return message.get(message_params::kModuleType, "").asString();
}

}  // namespace commands

}  // namespace remote_control
