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

#include "can_cooperation/commands/get_interior_vehicle_data_request.h"
#include "can_cooperation/validators/get_interior_vehicle_data_request_validator.h"
#include "can_cooperation/validators/get_interior_vehicle_data_response_validator.h"
#include "can_cooperation/can_module_constants.h"
#include "can_cooperation/message_helper.h"
#include "can_cooperation/can_module.h"
#include "functional_module/function_ids.h"
#include "json/json.h"

namespace can_cooperation {

namespace commands {

using namespace json_keys;
using namespace message_params;

CREATE_LOGGERPTR_GLOBAL(logger_, "GetInteriorVehicleDataRequest")

GetInteriorVehicleDataRequest::GetInteriorVehicleDataRequest(
    const application_manager::MessagePtr& message,
    CANModuleInterface& can_module)
    : BaseCommandRequest(message, can_module) {}

void GetInteriorVehicleDataRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  SendRequest(functional_modules::hmi_api::get_interior_vehicle_data,
              MessageHelper::StringToValue(message_->json_message()),
              true);
}

void GetInteriorVehicleDataRequest::OnEvent(
    const can_event_engine::Event<application_manager::MessagePtr, std::string>&
        event) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (functional_modules::hmi_api::get_interior_vehicle_data == event.id()) {
    std::string result_code;
    std::string info;

    Json::Value value =
        MessageHelper::StringToValue(event.event_message()->json_message());

    bool success = ParseResultCode(value, result_code, info);

    validators::ValidationResult validation_result = validators::SUCCESS;
    validators::GetInteriorVehicleDataResponseValidator validator;
    if (success) {
      validation_result = validator.Validate(value[kResult], response_params_);

      if (validators::SUCCESS != validation_result) {
        success = false;
        info = "Invalid response from the vehicle";
        result_code = result_codes::kGenericError;
      }

      ProccessSubscription();
    }

    SendResponse(success, result_code.c_str(), info);
  } else {
    LOG4CXX_ERROR(logger_, "Received unknown event: " << event.id());
  }
}

void GetInteriorVehicleDataRequest::ProccessSubscription() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value params;

  Json::Reader reader;
  reader.parse(message_->json_message(), params);

  if (IsMember(params, kSubscribe)) {
    bool subscribe = params[kSubscribe].asBool();

    if (!response_params_.isMember(kIsSubscribed)) {
      if (subscribe) {
        response_params_[kIsSubscribed] = false;
      } else {
        response_params_[kIsSubscribed] = true;
      }
    } else {
      CANAppExtensionPtr extension = GetAppExtension(app());

      bool isSubscribed = response_params_[kIsSubscribed].asBool();

      if (subscribe && isSubscribed) {
        extension->SubscribeToInteriorVehicleData(params[kModuleDescription]);
      } else if ((!subscribe) && (!isSubscribed)) {
        extension->UnsubscribeFromInteriorVehicleData(
            params[kModuleDescription]);
      }
    }
  } else {
    response_params_.removeMember(kIsSubscribed);
  }
}

bool GetInteriorVehicleDataRequest::Validate() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value json;
  if (!this->ParseJsonString(&json)) {
    return false;
  }

  Json::Value outgoing_json;
  validators::GetInteriorVehicleDataRequestValidator validator;
  if (validators::ValidationResult::SUCCESS !=
      validator.Validate(json, outgoing_json)) {
    LOG4CXX_INFO(logger_, "GetInteriorVehicleDataRequest validation failed!");
    SendResponse(
        false, result_codes::kInvalidData, "Mobile request validation failed!");
    return false;
  } else {
    message_->set_json_message(MessageHelper::ValueToString(outgoing_json));
  }

  return true;
}

std::string GetInteriorVehicleDataRequest::ModuleType(
    const Json::Value& message) {
  return message.get(message_params::kModuleDescription,
                     Json::Value(Json::objectValue))
      .get(message_params::kModuleType, Json::Value(""))
      .asString();
}

Json::Value GetInteriorVehicleDataRequest::GetInteriorZone(
    const Json::Value& message) {
  return message.get(message_params::kModuleDescription,
                     Json::Value(Json::objectValue))
      .get(message_params::kModuleZone, Json::Value(Json::objectValue));
}

SeatLocation GetInteriorVehicleDataRequest::InteriorZone(
    const Json::Value& message) {
  Json::Value zone =
      message.get(message_params::kModuleDescription,
                  Json::Value(Json::objectValue))
          .get(message_params::kModuleZone, Json::Value(Json::objectValue));
  return CreateInteriorZone(zone);
}

}  // namespace commands

}  // namespace can_cooperation
