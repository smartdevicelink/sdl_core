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

#include "can_cooperation/commands/get_interior_vehicle_data_capabilities_request.h"

#include <string>
#include <vector>

#include "can_cooperation/can_module_constants.h"
#include "can_cooperation/message_helper.h"
#include "can_cooperation/vehicle_capabilities.h"
#include "functional_module/function_ids.h"
#include "json/json.h"
#include "utils/logger.h"

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
  DCHECK_OR_RETURN_VOID(
      functional_modules::hmi_api::get_interior_vehicle_data_capabilities ==
      event.id());

  const bool is_message_valid =
      service()->ValidateMessageBySchema(*(event.event_message()));
  LOG4CXX_DEBUG(logger_, "Response from HMI is valid: " << is_message_valid);

  Json::Value value;
  Json::Reader reader;
  reader.parse(event.event_message()->json_message(), value);

  std::string info;
  std::string result_code;
  bool success = is_message_valid && ParseResultCode(value, result_code, info);

  if (!success) {
    LOG4CXX_INFO(logger_, "Failed to get correct response from HMI!");
    if (ReadCapabilitiesFromFile(event)) {
      success = true;
      result_code = result_codes::kSuccess;
      info = "";
    } else {
      success = false;
      result_code = result_codes::kGenericError;
      info = "Invalid response from the vehicle.";
    }
  }
  SendResponse(success, result_code.c_str(), info);
}

bool GetInteriorVehicleDataCapabiliesRequest::ReadCapabilitiesFromFile(
    const can_event_engine::Event<application_manager::MessagePtr, std::string>&
        event) {
  LOG4CXX_AUTO_TRACE(logger_);
  VehicleCapabilities file_caps;
  LOG4CXX_DEBUG(logger_,
                "Read vehicle capabilities from file: "
                    << file_caps.default_capabilities_path());
  Json::Value interior_vehicle_data_caps_content = file_caps.capabilities();
  Json::Value interior_vehicle_data_caps =
      Json::Value(Json::ValueType::objectValue);
  interior_vehicle_data_caps[kInteriorVehicleDataCapabilities] =
      interior_vehicle_data_caps_content;

  Json::Value stringified_hmi_response;
  Json::Reader reader;
  if (!reader.parse(event.event_message()->json_message(),
                    stringified_hmi_response)) {
    LOG4CXX_ERROR(logger_, "Failed to read capabilities from hmi response");
    return false;
  }

  stringified_hmi_response[kResult][kInteriorVehicleDataCapabilities] =
      interior_vehicle_data_caps_content;
  event.event_message()->set_json_message(
      stringified_hmi_response.toStyledString());

  const bool is_file_contents_valid =
      service()->ValidateMessageBySchema(*(event.event_message()));
  if (!is_file_contents_valid) {
    LOG4CXX_ERROR(logger_, "Contents of default capabilities file is invalid");
    return false;
  }
  response_params_ = interior_vehicle_data_caps;
  return !response_params_[kInteriorVehicleDataCapabilities].empty();
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
