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
#include "can_cooperation/validators/struct_validators/module_data_validator.h"
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
  const application_manager::MessagePtr& message)
  : BaseCommandRequest(message) {
}

GetInteriorVehicleDataRequest::~GetInteriorVehicleDataRequest() {
}

void GetInteriorVehicleDataRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value params;

  Json::Reader reader;
  reader.parse(message_->json_message(), params);

  if (params.isMember(kSubscribe)){
    application_manager::ApplicationSharedPtr app =
        service_->GetApplication(message_->connection_key());

    if (!app) {
      LOG4CXX_ERROR(logger_, "Application doesn't registered!");
      SendResponse(false, result_codes::kApplicationNotRegistered, "");
      return;
    }

    CANAppExtensionPtr extension = application_manager::AppExtensionPtr::
        static_pointer_cast<CANAppExtension>(app->QueryInterface(
            CANModule::instance()->GetModuleID()));

    if (params[kSubscribe].asBool()){
      extension->SubscribeToInteriorVehicleData(params[kModuleDescription]);
    } else {
      extension->UnsubscribeFromInteriorVehicleData(params[kModuleDescription]);
    }
  }

  SendRequest(
      functional_modules::hmi_api::get_interior_vehicle_data, params, true);
}

void GetInteriorVehicleDataRequest::OnEvent(
    const event_engine::Event<application_manager::MessagePtr,
    std::string>& event) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (functional_modules::hmi_api::get_interior_vehicle_data == event.id()) {
    std::string result_code;
    std::string info;

    Json::Value value;
    Json::Reader reader;
    reader.parse(event.event_message()->json_message(), value);

    bool success = ParseResultCode(value, result_code, info);

    // TOD(VS): Create GetInteriorVehicleDataResponseValidator
    validators::ValidationResult validation_result = validators::SUCCESS;

    if (success) {
      if (value[kResult].isMember(kModuleData)) {
        validation_result =
            validators::ModuleDataValidator::instance()->Validate(
                                              value[kResult][kModuleData],
                                              response_params_[kModuleData]);
      } else {
        validation_result = validators::INVALID_DATA;
      }

      if (validators::SUCCESS != validation_result) {
        success = false;
        info = "Invalid response from the vehicle";
        result_code = result_codes::kGenericError;
      }
    }

    SendResponse(success, result_code.c_str(), info);
  } else {
    LOG4CXX_ERROR(logger_, "Received unknown event: " << event.id());
  }
}

bool GetInteriorVehicleDataRequest::Validate() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value json;
  if (!this->ParseJsonString(&json)) {
    return false;
  }

  Json::Value outgoing_json;

  if (validators::ValidationResult::SUCCESS !=
    validators::GetInteriorVehicleDataRequestValidator::instance()->
                                                Validate(json, outgoing_json)) {
    LOG4CXX_INFO(logger_,
                 "GetInteriorVehicleDataRequest validation failed!");
    SendResponse(false, result_codes::kInvalidData,
                 "Mobile request validation failed!");
    return false;
  }  else {
    message_->set_json_message(MessageHelper::ValueToString(outgoing_json));
  }

  return true;
}

std::string GetInteriorVehicleDataRequest::ModuleType(
    const Json::Value& message) {
  return message.get(message_params::kModuleDescription,
                     Json::Value(Json::objectValue)).get(
      message_params::kModuleType, Json::Value("")).asString();
}

Json::Value GetInteriorVehicleDataRequest::GetInteriorZone(
  const Json::Value& message) {
  return message.get(message_params::kModuleDescription,
                                 Json::Value(Json::objectValue)).get(
      message_params::kModuleZone, Json::Value(Json::objectValue));
}

SeatLocation GetInteriorVehicleDataRequest::InteriorZone(
    const Json::Value& message) {
  Json::Value zone = message.get(message_params::kModuleDescription,
                                 Json::Value(Json::objectValue)).get(
      message_params::kModuleZone, Json::Value(Json::objectValue));
  return CreateInteriorZone(zone);
}

}  // namespace commands

}  // namespace can_cooperation
