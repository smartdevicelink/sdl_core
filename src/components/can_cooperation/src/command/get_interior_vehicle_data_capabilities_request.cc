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

#include "can_cooperation/commands/get_interior_vehicle_data_capabilities_request.h"
#include "can_cooperation/validators/get_interior_vehicle_data_capabilities_request_validator.h"
#include "can_cooperation/validators/struct_validators/module_description_validator.h"
#include "can_cooperation/can_module_constants.h"
#include "can_cooperation/message_helper.h"
#include "can_cooperation/vehicle_capabilities.h"
#include "functional_module/function_ids.h"
#include "json/json.h"

namespace can_cooperation {

namespace commands {

using namespace json_keys;
using namespace message_params;

CREATE_LOGGERPTR_GLOBAL(logger_, "GetInteriorVehicleDataCapabiliesRequest")

GetInteriorVehicleDataCapabiliesRequest::
GetInteriorVehicleDataCapabiliesRequest(
  const application_manager::MessagePtr& message)
  : BaseCommandRequest(message) {
}

GetInteriorVehicleDataCapabiliesRequest::
~GetInteriorVehicleDataCapabiliesRequest() {
}

void GetInteriorVehicleDataCapabiliesRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value params;

  Json::Reader reader;
  reader.parse(message_->json_message(), params);

  SendRequest(
      functional_modules::hmi_api::get_interior_vehicle_data_capabilities,
      params, true);
}

void GetInteriorVehicleDataCapabiliesRequest::OnEvent(
    const event_engine::Event<application_manager::MessagePtr,
    std::string>& event) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (functional_modules::hmi_api::get_interior_vehicle_data_capabilities ==
      event.id()) {
    std::string result_code;
    std::string info;

    Json::Value value;
    Json::Reader reader;
    reader.parse(event.event_message()->json_message(), value);

    bool success = ParseResultCode(value, result_code, info);


    // TOD(VS): Create GetInteriorVehicleDataCapabiliesResponseValidator. Replace this code there and correct it
    validators::ValidationResult validation_result = validators::SUCCESS;

    const int capabilities_min_size = 1;
    const int capabilities_max_size = 1000;

    if (success) {
      if (value[kResult].isMember(kInteriorVehicleDataCapabilities)) {
        int capabilities_size =
            value[kResult][kInteriorVehicleDataCapabilities].size();
        if (value[kResult][kInteriorVehicleDataCapabilities].isArray() &&
            (capabilities_size >= capabilities_min_size)               &&
            (capabilities_size <= capabilities_max_size)) {
          for (int i = 0; i < capabilities_size; ++i) {
            validation_result =
              validators::ModuleDescriptionValidator::instance()->Validate(
                value[kResult][kInteriorVehicleDataCapabilities][i],
                response_params_[kInteriorVehicleDataCapabilities][i]);
          }
        } else {
          validation_result = validators::INVALID_DATA;
        }
      } else {
        validation_result = validators::INVALID_DATA;
      }

      if (validators::SUCCESS != validation_result) {
        success = false;
        info = "Invalid response from vehicle.";
        result_code = result_codes::kGenericError;
      }
    }

    if (!success) {
      //  Try to read capabilities from file.
      //  TODO(PV): move to separate method
      LOG4CXX_INFO(logger_, "Failed to get correct response from HMI; \
          trying to read from file");
      Json::Value request;

      Json::Reader reader;
      reader.parse(message_->json_message(), request);

      VehicleCapabilities file_caps;
      Json::Value zone_capabilities;
      if (request.isMember(kZone)) {
        zone_capabilities = file_caps.capabilities(request[kZone]);
      } else {
        zone_capabilities = file_caps.capabilities();
      }

      if (zone_capabilities.type() == Json::ValueType::arrayValue) {
        LOG4CXX_DEBUG(logger_, "Read vehicle capabilities from file "
            << zone_capabilities);
        if (request.isMember(kModuleTypes)) {
          response_params_[kInteriorVehicleDataCapabilities] = Json::Value(
            Json::ValueType::arrayValue);
          for (size_t i = 0; i < zone_capabilities.size(); ++i) {
            for (size_t j = 0; j < request[kModuleTypes].size(); ++j) {
              if (request[kModuleTypes][static_cast<int>(j)] ==
                    zone_capabilities[static_cast<int>(i)][kModuleType]) {
                response_params_[kInteriorVehicleDataCapabilities].append(
                  zone_capabilities[static_cast<int>(i)]);
              }
            }
          }
        } else {
          response_params_[kInteriorVehicleDataCapabilities] =
            zone_capabilities;
        }
        success = true;
        result_code = result_codes::kSuccess;
        info = "";
      } else {
        // Failed to read capabilities from file.
        LOG4CXX_ERROR(logger_, "Failed to read capabilities from file also");
        result_code = result_codes::kGenericError;
        info = "Invalid response from the the vehicle";
      }
    }

    SendResponse(success, result_code.c_str(), info);
  } else {
    LOG4CXX_ERROR(logger_, "Received unknown event: " << event.id());
  }
}

bool GetInteriorVehicleDataCapabiliesRequest::Validate() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value json;

  json = MessageHelper::StringToValue(message_->json_message());
  Json::Value outgoing_json;

  if (validators::ValidationResult::SUCCESS !=
    validators::GetInteriorVehicleDataCapabilitiesRequestValidator::instance()->
                                                Validate(json, outgoing_json)) {
    LOG4CXX_INFO(logger_,
                 "GetInteriorVehicleDataCapabiliesRequest validation failed!");
    SendResponse(false, result_codes::kInvalidData,
                 "Mobile request validation failed!");
    return false;
  } else {
    message_->set_json_message(MessageHelper::ValueToString(outgoing_json));
  }

  return true;
}

bool GetInteriorVehicleDataCapabiliesRequest::CheckAccess() {
  // TODO(KKolodiy): need to clarify how it must work
  return true;
}

}  // namespace commands

}  // namespace can_cooperation
