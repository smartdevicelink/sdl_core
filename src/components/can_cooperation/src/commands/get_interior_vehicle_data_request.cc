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

#include <map>

#include "can_cooperation/commands/get_interior_vehicle_data_request.h"
#include "can_cooperation/can_module_constants.h"
#include "can_cooperation/message_helper.h"
#include "can_cooperation/can_module.h"
#include "functional_module/function_ids.h"
#include "json/json.h"

namespace can_cooperation {

namespace commands {

using namespace enums_value;
using namespace json_keys;
using namespace message_params;

CREATE_LOGGERPTR_GLOBAL(logger_, "GetInteriorVehicleDataRequest")

GetInteriorVehicleDataRequest::GetInteriorVehicleDataRequest(
    const application_manager::MessagePtr& message,
    CANModuleInterface& can_module)
    : BaseCommandRequest(message, can_module) {}

void GetInteriorVehicleDataRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);
  const Json::Value request_params =
      MessageHelper::StringToValue(message_->json_message());

  SendRequest(functional_modules::hmi_api::get_interior_vehicle_data,
              request_params,
              true);
}

void GetInteriorVehicleDataRequest::OnEvent(
    const can_event_engine::Event<application_manager::MessagePtr, std::string>&
        event) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(
      (functional_modules::hmi_api::get_interior_vehicle_data == event.id()));

  application_manager::Message& hmi_response = *(event.event_message());
  const bool validate_result = application_manager::SUCCESS ==
                               service()->ValidateMessageBySchema(hmi_response);
  LOG4CXX_DEBUG(logger_,
                "HMI response validation result is " << validate_result);
  const Json::Value value =
      MessageHelper::StringToValue(hmi_response.json_message());
  std::string result_code;
  std::string info;
  bool success = validate_result && ParseResultCode(value, result_code, info);

  if (!validate_result) {
    success = false;
    result_code = result_codes::kGenericError;
    info = "Invalid message received from vehicle";
  }

  if (success) {
    ProccessSubscription(value);
    response_params_[kModuleData] = value[kResult][kModuleData];
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
      IsMember(hmi_response[kResult], kIsSubscribed);

  if (!is_subscribe_present_in_request && !isSubscribed_present_in_response) {
    return;
  }

  CANAppExtensionPtr extension = GetAppExtension(app());
  if (is_subscribe_present_in_request && !isSubscribed_present_in_response) {
    LOG4CXX_WARN(logger_,
                 "conditional mandatory parameter "
                     << kIsSubscribed << " missed in hmi response");
    response_params_[kIsSubscribed] =
        extension->IsSubscibedToInteriorVehicleData(
            request_params[kModuleDescription]);
    return;
  }

  if (!is_subscribe_present_in_request && isSubscribed_present_in_response) {
    LOG4CXX_WARN(logger_,
                 "Parameter " << kIsSubscribed << " is ignored due to absence '"
                              << kSubscribe << "' parameter in request");
    return;
  }

  const bool request_subscribe = request_params[kSubscribe].asBool();
  const bool response_subscribe = hmi_response[kResult][kIsSubscribed].asBool();
  response_params_[kIsSubscribed] = response_subscribe;
  if (request_subscribe == response_subscribe) {
    if (response_subscribe) {
      extension->SubscribeToInteriorVehicleData(
          request_params[kModuleDescription]);
    } else {
      extension->UnsubscribeFromInteriorVehicleData(
          request_params[kModuleDescription]);
    }
  }
}

std::string GetInteriorVehicleDataRequest::ModuleType(
    const Json::Value& message) {
  return message.get(message_params::kModuleDescription,
                     Json::Value(Json::objectValue))
      .get(message_params::kModuleType, Json::Value(""))
      .asString();
}

}  // namespace commands

}  // namespace can_cooperation
