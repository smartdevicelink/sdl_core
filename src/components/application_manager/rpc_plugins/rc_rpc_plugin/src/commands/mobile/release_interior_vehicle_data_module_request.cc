/*
 * Copyright (c) 2019, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <string>

#include "application_manager/rpc_plugins/rc_rpc_plugin/include/rc_rpc_plugin/resource_allocation_manager.h"
#include "rc_rpc_plugin/commands/mobile/release_interior_vehicle_data_module_request.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "smart_objects/enum_schema_item.h"

namespace rc_rpc_plugin {
using namespace application_manager;
namespace commands {

namespace {
struct ResponseParams {
  std::string response_info;
  std::string module_type;
  std::string module_id;
  mobile_apis::Result::eType result_code;
  uint32_t app_id;
  bool success_result;
};

void PrepareResponseResult(
    ResponseParams& response_params_out,
    rc_rpc_plugin::ResourceReleasedState::eType& released_result) {
  std::stringstream ss;
  auto info_inserter = [&ss, response_params_out](std::string info) {
    ss << "Module [" << response_params_out.module_type << ":"
       << response_params_out.module_id << "] " << info;
  };
  switch (released_result) {
    case rc_rpc_plugin::ResourceReleasedState::eType::IS_RELEASED: {
      response_params_out.success_result = true;
      response_params_out.result_code = mobile_apis::Result::eType::SUCCESS;
      info_inserter("is released successfully.");
      break;
    }
    case rc_rpc_plugin::ResourceReleasedState::eType::IS_ALLOCATED: {
      response_params_out.success_result = false;
      response_params_out.result_code = mobile_apis::Result::eType::REJECTED;
      info_inserter("is allocated to a different application.");
      break;
    }
    case rc_rpc_plugin::ResourceReleasedState::eType::NOT_ALLOCATED: {
      response_params_out.success_result = false;
      response_params_out.result_code = mobile_apis::Result::eType::IGNORED;
      info_inserter("is not allocated to any application.");
      break;
    }
  }
  response_params_out.response_info = ss.str();
}
}  // namespace

ReleaseInteriorVehicleDataModuleRequest::
    ReleaseInteriorVehicleDataModuleRequest(
        const application_manager::commands::MessageSharedPtr& message,
        const RCCommandParams& params)
    : RCCommandRequest(message, params) {}

bool ReleaseInteriorVehicleDataModuleRequest::ProcessCapabilities() {
  LOG4CXX_AUTO_TRACE(logger_);
  const auto rc_capability = hmi_capabilities_.rc_capability();

  const std::string module_type = ModuleType();
  const std::string module_id = ModuleId();
  const ModuleUid module(module_type, module_id);
  if (rc_capability &&
      !rc_capabilities_manager_.CheckIfModuleExistsInCapabilities(module)) {
    LOG4CXX_WARN(
        logger_,
        "Accessing not supported module: " << module_type << " " << module_id);
    SendResponse(false,
                 mobile_apis::Result::UNSUPPORTED_RESOURCE,
                 "Accessing not supported module");
    return false;
  }
  return true;
}

void ReleaseInteriorVehicleDataModuleRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!ProcessCapabilities()) {
    return;
  }

  const std::string module_type = ModuleType();
  const std::string module_id = ModuleId();

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const uint32_t app_id = app->app_id();

  ResourceReleasedState::eType released_result =
      resource_allocation_manager_.ReleaseResource(
          module_type, module_id, app_id);

  bool success_result = true;
  mobile_apis::Result::eType result_code = mobile_apis::Result::eType::SUCCESS;
  std::string response_info;
  ResponseParams response_params{response_info,
                                 ModuleType(),
                                 module_id,
                                 result_code,
                                 app_id,
                                 success_result};
  PrepareResponseResult(response_params, released_result);

  if (response_params.success_result) {
    resource_allocation_manager_.SendOnRCStatusNotifications(
        NotificationTrigger::MODULE_ALLOCATION, app);
  }

  LOG4CXX_DEBUG(logger_,
                "Resource for module: "
                    << ModuleType() << " with id: " << module_id
                    << " was released with result " << std::boolalpha
                    << response_params.success_result
                    << " and result_code: " << response_params.result_code);

  SendResponse(response_params.success_result,
               response_params.result_code,
               response_params.response_info.c_str());
}

std::string ReleaseInteriorVehicleDataModuleRequest::ModuleType() const {
  LOG4CXX_AUTO_TRACE(logger_);
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

std::string ReleaseInteriorVehicleDataModuleRequest::ModuleId() const {
  LOG4CXX_AUTO_TRACE(logger_);
  auto msg_params = (*message_)[app_mngr::strings::msg_params];
  if (msg_params.keyExists(message_params::kModuleId)) {
    return msg_params[message_params::kModuleId].asString();
  }
  const std::string module_id =
      rc_capabilities_manager_.GetDefaultModuleIdFromCapabilities(ModuleType());
  return module_id;
}

ReleaseInteriorVehicleDataModuleRequest::
    ~ReleaseInteriorVehicleDataModuleRequest() {}

}  // namespace commands
}  // namespace rc_rpc_plugin
