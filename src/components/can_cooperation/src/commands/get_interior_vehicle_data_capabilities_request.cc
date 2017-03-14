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

#include <string>
#include <vector>

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
        const application_manager::MessagePtr& message,
        CANModuleInterface& can_module)
    : BaseCommandRequest(message, can_module) {}

GetInteriorVehicleDataCapabiliesRequest::
    ~GetInteriorVehicleDataCapabiliesRequest() {}

void GetInteriorVehicleDataCapabiliesRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value params;

  Json::Reader reader;
  reader.parse(message_->json_message(), params);
  UpdateModules(&params);

  SendRequest(
      functional_modules::hmi_api::get_interior_vehicle_data_capabilities,
      params,
      true);
}

void GetInteriorVehicleDataCapabiliesRequest::UpdateModules(
    Json::Value* params) {
  DCHECK(params);
  if (IsDriverDevice()) {
    DCHECK(allowed_modules_.isArray());
    if (allowed_modules_.empty()) {
      // Empty allowed modules list is all modules
      params->removeMember(message_params::kModuleTypes);
    } else {
      (*params)[message_params::kModuleTypes] = allowed_modules_;
    }
  } else {
    // The permissions of passenger's device  were checked by GetPermission.
    // This request can contain only one module
    // so we don't have to exclude any module.
    DCHECK(params->get(message_params::kModuleTypes,
                       Json::Value(Json::arrayValue)).size() == 1);
  }
}

void GetInteriorVehicleDataCapabiliesRequest::OnEvent(
    const can_event_engine::Event<application_manager::MessagePtr, std::string>&
        event) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (functional_modules::hmi_api::get_interior_vehicle_data_capabilities ==
      event.id()) {
    std::string result_code;
    std::string info;

    Json::Value value;
    Json::Reader reader;
    reader.parse(event.event_message()->json_message(), value);

    bool success = ParseResultCode(value, result_code, info);

    // TOD(VS): Create GetInteriorVehicleDataCapabiliesResponseValidator.
    // Replace this code there and correct it
    validators::ValidationResult validation_result = validators::SUCCESS;

    const int capabilities_min_size = 1;
    const int capabilities_max_size = 1000;

    if (success) {
      if (IsMember(value[kResult], kInteriorVehicleDataCapabilities)) {
        int capabilities_size =
            value[kResult][kInteriorVehicleDataCapabilities].size();
        if (value[kResult][kInteriorVehicleDataCapabilities].isArray() &&
            (capabilities_size >= capabilities_min_size) &&
            (capabilities_size <= capabilities_max_size)) {
          validators::ModuleDescriptionValidator module_description_validator;

          for (int i = 0; i < capabilities_size; ++i) {
            validation_result = module_description_validator.Validate(
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
        info = "Invalid response from the vehicle.";
        result_code = result_codes::kGenericError;
      }
    }

    if (!success) {
      //  Try to read capabilities from file.
      if (ReadCapabilitiesFromFile()) {
        success = true;
        result_code = result_codes::kSuccess;
        info = "";
      }
    }

    SendResponse(success, result_code.c_str(), info);
  } else {
    LOG4CXX_ERROR(logger_, "Received unknown event: " << event.id());
  }
}

bool GetInteriorVehicleDataCapabiliesRequest::ReadCapabilitiesFromFile() {
  LOG4CXX_INFO(logger_,
               "Failed to get correct response from HMI; \
      trying to read from file");
  Json::Value request;
  Json::Reader reader;
  if (!reader.parse(message_->json_message(), request)) {
    LOG4CXX_ERROR(logger_, "Failed to read capabilities from file also");
    return false;
  }

  VehicleCapabilities file_caps;
  Json::Value zone_capabilities;
  if (IsMember(request, kZone)) {
    zone_capabilities = file_caps.capabilities(request[kZone]);
  } else {
    zone_capabilities = file_caps.capabilities();
  }

  if (!zone_capabilities.isArray()) {
    // Failed to read capabilities from file.
    LOG4CXX_ERROR(logger_, "Failed to read capabilities from file also");
    return false;
  }
  LOG4CXX_DEBUG(logger_,
                "Read vehicle capabilities from file " << zone_capabilities);
  const Json::Value& modules =
      IsDriverDevice() ? allowed_modules_ : request[kModuleTypes];
  DCHECK(modules.isArray());
  CreateCapabilities(zone_capabilities, modules);
  return !response_params_[kInteriorVehicleDataCapabilities].empty();
}

void GetInteriorVehicleDataCapabiliesRequest::CreateCapabilities(
    const Json::Value& capabilities, const Json::Value& modules) {
  response_params_[kInteriorVehicleDataCapabilities] =
      Json::Value(Json::ValueType::arrayValue);
  for (Json::ValueConstIterator i = capabilities.begin();
       i != capabilities.end();
       ++i) {
    for (Json::ValueConstIterator j = modules.begin(); j != modules.end();
         ++j) {
      const Json::Value& row = *i;
      if (row[kModuleType] == *j) {
        response_params_[kInteriorVehicleDataCapabilities].append(row);
      }
    }
  }
}

bool GetInteriorVehicleDataCapabiliesRequest::Validate() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value json;
  if (!this->ParseJsonString(&json)) {
    return false;
  }
  Json::Value outgoing_json;
  validators::GetInteriorVehicleDataCapabilitiesRequestValidator validator;
  if (validators::ValidationResult::SUCCESS !=
      validator.Validate(json, outgoing_json)) {
    LOG4CXX_INFO(logger_,
                 "GetInteriorVehicleDataCapabiliesRequest validation failed!");
    SendResponse(
        false, result_codes::kInvalidData, "Mobile request validation failed!");
    return false;
  } else {
    message_->set_json_message(MessageHelper::ValueToString(outgoing_json));
  }

  return true;
}

application_manager::TypeAccess
GetInteriorVehicleDataCapabiliesRequest::CheckAccess(
    const Json::Value& message) {
  if (IsDriverDevice()) {
    return CheckModuleTypes(message);
  }
  return BaseCommandRequest::CheckAccess(message);
}

application_manager::TypeAccess
GetInteriorVehicleDataCapabiliesRequest::CheckModuleTypes(
    const Json::Value& message) {
  if (IsMember(message, message_params::kModuleTypes)) {
    return ProcessRequestedModuleTypes(message[message_params::kModuleTypes]);
  } else {
    return GetModuleTypes();
  }
}

application_manager::TypeAccess
GetInteriorVehicleDataCapabiliesRequest::ProcessRequestedModuleTypes(
    const Json::Value& modules) {
  DCHECK(!modules.empty());
  DCHECK(allowed_modules_.isNull());
  Json::Value moduleTypes(Json::arrayValue);
  for (Json::ValueConstIterator i = modules.begin(); i != modules.end(); ++i) {
    const Json::Value& module = *i;
    if (service()->CheckModule(app()->app_id(), module.asString())) {
      moduleTypes.append(module);
    }
  }
  allowed_modules_ = moduleTypes;
  DCHECK(allowed_modules_.isArray());
  // Empty allowed modules list means requested modules didn't match with
  // allowed by policy
  return allowed_modules_.empty() ? application_manager::kDisallowed
                                  : application_manager::kAllowed;
}

application_manager::TypeAccess
GetInteriorVehicleDataCapabiliesRequest::GetModuleTypes() {
  DCHECK(allowed_modules_.isNull());
  typedef std::vector<std::string> ModuleTypes;
  ModuleTypes modules;
  if (service()->GetModuleTypes(app()->policy_app_id(), &modules)) {
    Json::Value moduleTypes(Json::arrayValue);
    for (ModuleTypes::iterator i = modules.begin(); i != modules.end(); ++i) {
      const std::string& name = *i;
      moduleTypes.append(Json::Value(name));
    }
    allowed_modules_ = moduleTypes;
    DCHECK(allowed_modules_.isArray());
    // Empty allowed modules list is all modules
    return application_manager::kAllowed;
  }
  // There aren't allowed modules
  DCHECK(allowed_modules_.isNull());
  return application_manager::kDisallowed;
}

Json::Value GetInteriorVehicleDataCapabiliesRequest::GetInteriorZone(
    const Json::Value& message) {
  // This is used only for passenger's devices
  DCHECK(!IsDriverDevice());
  return message.get(message_params::kZone, Json::Value(Json::objectValue));
}

SeatLocation GetInteriorVehicleDataCapabiliesRequest::InteriorZone(
    const Json::Value& message) {
  // This is used only for passenger's devices
  DCHECK(!IsDriverDevice());
  Json::Value zone =
      message.get(message_params::kZone, Json::Value(Json::objectValue));
  if (zone.empty()) {
    // if SDL-RC has not received app's device location from the vehicle
    // we will use this message to send response "DISALLOWED"
    set_disallowed_info("Information: zone must be provided");
  }
  return CreateInteriorZone(zone);
}

std::string GetInteriorVehicleDataCapabiliesRequest::ModuleType(
    const Json::Value& message) {
  // This is used only for passenger's devices
  DCHECK(!IsDriverDevice());
  Json::Value modules =
      message.get(message_params::kModuleTypes, Json::Value(Json::arrayValue));
  if (modules.size() == 1) {
    return modules.get(Json::ArrayIndex(0), Json::Value("")).asString();
  }
  set_disallowed_info("Information: one moduleType must be provided");
  return "";
}

bool GetInteriorVehicleDataCapabiliesRequest::IsDriverDevice() {
  return app()->device() == service()->PrimaryDevice();
}
}  // namespace commands

}  // namespace can_cooperation
