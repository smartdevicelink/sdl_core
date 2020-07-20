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

#include "rc_rpc_plugin/commands/mobile/set_interior_vehicle_data_request.h"
#include "interfaces/MOBILE_API.h"
#include "json/json.h"
#include "rc_rpc_plugin/rc_helpers.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/helpers.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

using namespace json_keys;
using namespace message_params;
CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")

SetInteriorVehicleDataRequest::SetInteriorVehicleDataRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    const RCCommandParams& params)
    : RCCommandRequest(message, params) {}

SetInteriorVehicleDataRequest::~SetInteriorVehicleDataRequest() {}

/**
 * @brief Clears unrelated module data parameters
 * @param module type in request
 * @param smart object of module_data
 * @return true if the correct module parameter is present, false otherwise
 */
bool ClearUnrelatedModuleData(const std::string& module_type,
                              smart_objects::SmartObject& module_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  const auto& all_module_types = RCHelpers::GetModuleTypesList();
  const auto& data_mapping = RCHelpers::GetModuleTypeToDataMapping();
  bool module_type_and_data_match = false;
  for (const auto& type : all_module_types) {
    const std::string module_key = data_mapping(type);
    if (type == module_type) {
      module_type_and_data_match = module_data.keyExists(module_key);
    } else if (module_data.keyExists(module_key)) {
      // Cutting unrelated module data
      module_data.erase(module_key);
    }
  }
  return module_type_and_data_match;
}

mobile_apis::Result::eType PrepareResultCodeAndInfo(
    const ModuleTypeCapability module_data_capabilities, std::string& info) {
  mobile_apis::Result::eType result_code =
      mobile_apis::Result::UNSUPPORTED_RESOURCE;
  if (message_params::kLightState == module_data_capabilities.first) {
    switch (module_data_capabilities.second) {
      case capabilitiesStatus::kMissedLightName:
        info = "The requested LightName is not supported by the vehicle.";
        break;
      case capabilitiesStatus::kMissedParam:
        info =
            "The requested parameter of the given LightName is not supported "
            "by the vehicle.";
        break;
      case capabilitiesStatus::kReadOnly:
        info = "The requested parameter is read-only.";
        result_code = mobile_apis::Result::READ_ONLY;
        break;
      default:
        break;
    }
  } else if (module_data_capabilities.second ==
             capabilitiesStatus::kInvalidStatus) {
    info = "The RC Capability is not available";
  } else {
    info = "Accessing not supported module data.";
  }
  return result_code;
  LOG4CXX_WARN(logger_, info);
}

void SetInteriorVehicleDataRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject& module_data =
      (*message_)[app_mngr::strings::msg_params][message_params::kModuleData];
  const std::string module_type = ModuleType();

  if (ClearUnrelatedModuleData(module_type, module_data)) {
    const std::string module_id = ModuleId();
    const ModuleUid module(module_type, module_id);
    if (!rc_capabilities_manager_.CheckIfModuleExistsInCapabilities(module)) {
      LOG4CXX_WARN(logger_,
                   "Accessing not supported module: " << module_type << " "
                                                      << module_id);
      SetResourceState(ModuleType(), ResourceState::FREE);
      SendResponse(false,
                   mobile_apis::Result::UNSUPPORTED_RESOURCE,
                   "Accessing not supported module data");
      return;
    }

    ModuleTypeCapability module_data_capabilities;
    module_data_capabilities =
        rc_capabilities_manager_.GetModuleDataCapabilities(module_data,
                                                           module_id);

    if (capabilitiesStatus::kSuccess != module_data_capabilities.second) {
      SetResourceState(ModuleType(), ResourceState::FREE);
      std::string info;
      mobile_apis::Result::eType result =
          PrepareResultCodeAndInfo(module_data_capabilities, info);
      SendResponse(false, result, info.c_str());
      return;
    }

    if (rc_capabilities_manager_.AreAllParamsReadOnly(module_data,
                                                      module_type)) {
      LOG4CXX_WARN(logger_, "All request params in module type are READ ONLY!");
      SetResourceState(ModuleType(), ResourceState::FREE);
      SendResponse(false,
                   mobile_apis::Result::READ_ONLY,
                   "All request params in module type are READ ONLY!");
      return;
    }

    module_data_capabilities = std::make_pair("", capabilitiesStatus::kSuccess);

    if (rc_capabilities_manager_.AreReadOnlyParamsPresent(
            module_data, module_type, module_data_capabilities)) {
      LOG4CXX_DEBUG(logger_, "Request module type has READ ONLY parameters");

      if (enums_value::kLight == module_data_capabilities.first &&
          capabilitiesStatus::kSuccess != module_data_capabilities.second) {
        SetResourceState(ModuleType(), ResourceState::FREE);
        SendResponse(
            false,
            mobile_apis::Result::READ_ONLY,
            "The LightStatus enum passed is READ ONLY and cannot be written.");
        return;
      }

      LOG4CXX_DEBUG(logger_, "Cutting-off READ ONLY parameters... ");

      CutOffReadOnlyParams(module_data);
    }

    application_manager_.RemoveHMIFakeParameters(
        message_, hmi_apis::FunctionID::RC_SetInteriorVehicleData);

    app_mngr::ApplicationSharedPtr app =
        application_manager_.application(connection_key());
    (*message_)[app_mngr::strings::msg_params][app_mngr::strings::app_id] =
        app->app_id();

    const bool app_wants_to_set_audio_src =
        module_data.keyExists(message_params::kAudioControlData) &&
        module_data[message_params::kAudioControlData].keyExists(
            message_params::kSource);

    if (app_wants_to_set_audio_src) {
      if (!app->IsAllowedToChangeAudioSource()) {
        LOG4CXX_WARN(logger_, "App is not allowed to change audio source");
        SetResourceState(ModuleType(), ResourceState::FREE);
        SendResponse(false,
                     mobile_apis::Result::REJECTED,
                     "App is not allowed to change audio source");
        return;
      } else if (module_data[message_params::kAudioControlData].keyExists(
                     message_params::kKeepContext)) {
        app->set_keep_context(module_data[message_params::kAudioControlData]
                                         [message_params::kKeepContext]
                                             .asBool());
      }
    }

    (*message_)[app_mngr::strings::msg_params][message_params::kModuleData]
               [message_params::kModuleId] = module_id;
    SendHMIRequest(hmi_apis::FunctionID::RC_SetInteriorVehicleData,
                   &(*message_)[app_mngr::strings::msg_params],
                   true);
  } else {
    LOG4CXX_WARN(logger_, "Request module type & data mismatch!");
    SetResourceState(ModuleType(), ResourceState::FREE);
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "Request module type & data mismatch!");
  }
}

void SetInteriorVehicleDataRequest::on_event(
    const app_mngr::event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  RCCommandRequest::on_event(event);

  if (hmi_apis::FunctionID::RC_SetInteriorVehicleData != event.id()) {
    return;
  }

  const smart_objects::SmartObject& hmi_response = event.smart_object();
  mobile_apis::Result::eType result_code =
      GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
          hmi_response[app_mngr::strings::params][app_mngr::hmi_response::code]
              .asUInt()));

  bool result =
      helpers::Compare<mobile_apis::Result::eType, helpers::EQ, helpers::ONE>(
          result_code,
          mobile_apis::Result::SUCCESS,
          mobile_apis::Result::WARNINGS);

  smart_objects::SmartObject response_params;
  bool is_resource_acquired = false;

  if (result) {
    if (!IsModuleIdProvided(hmi_response)) {
      LOG4CXX_WARN(logger_,
                   "conditional mandatory parameter "
                       << message_params::kModuleId
                       << " missed in hmi response");
      result = false;
      result_code = mobile_apis::Result::GENERIC_ERROR;
    }
    response_params = hmi_response[app_mngr::strings::msg_params];
    if (enums_value::kAudio == ModuleType()) {
      CheckAudioSource((
          *message_)[app_mngr::strings::msg_params][message_params::kModuleData]
                    [message_params::kAudioControlData]);
    }

    const std::string module_type = ModuleType();
    const std::string module_id = ModuleId();

    const rc_rpc_types::ModuleUid resource{module_type, module_id};
    auto app = application_manager_.application(connection_key());

    if (!app) {
      LOG4CXX_ERROR(logger_, "NULL pointer.");
      SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED, "");
      return;
    }

    const auto app_id = app->app_id();

    if (!resource_allocation_manager_.IsResourceAlreadyAcquiredByApp(resource,
                                                                     app_id)) {
      resource_allocation_manager_.SetResourceAcquired(
          module_type, module_id, app_id);
      is_resource_acquired = true;
    }

  } else {
    app_mngr::ApplicationSharedPtr app =
        application_manager_.application(connection_key());
    app->set_keep_context(false);
  }
  std::string info;
  GetInfo(hmi_response, info);
  SendResponse(
      result, result_code, info.c_str(), result ? &response_params : nullptr);

  if (is_resource_acquired) {
    resource_allocation_manager_.SendOnRCStatusNotifications(
        NotificationTrigger::MODULE_ALLOCATION,
        std::shared_ptr<application_manager::Application>());
  }
}

void SetInteriorVehicleDataRequest::CheckAudioSource(
    const smart_objects::SmartObject& audio_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (audio_data.keyExists(message_params::kSource)) {
    application_manager_.set_current_audio_source(
        audio_data[message_params::kSource].asUInt());
  }
}

void SetInteriorVehicleDataRequest::CutOffReadOnlyParams(
    smart_objects::SmartObject& module_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string module_type = ModuleType();
  const auto& module_type_params =
      rc_capabilities_manager_.ControlDataForType(module_data, module_type);

  if (enums_value::kAudio == module_type) {
    auto& equalizer_settings = module_data[message_params::kAudioControlData]
                                          [message_params::kEqualizerSettings];
    auto it = equalizer_settings.asArray()->begin();
    for (; it != equalizer_settings.asArray()->end(); ++it) {
      if (it->keyExists(message_params::kChannelName)) {
        it->erase(message_params::kChannelName);
        LOG4CXX_DEBUG(logger_,
                      "Cutting-off READ ONLY parameter: "
                          << message_params::kChannelName);
      }
    }
  }

  std::vector<std::string> ro_params =
      RCHelpers::GetModuleReadOnlyParams(module_type);
  const auto& data_mapping = RCHelpers::GetModuleTypeToDataMapping();
  for (const auto& param : ro_params) {
    if (module_type_params.keyExists(param)) {
      module_data[data_mapping(module_type)].erase(param);
      LOG4CXX_DEBUG(logger_, "Cutting-off READ ONLY parameter: " << param);
    }
  }
}

std::string SetInteriorVehicleDataRequest::ModuleType() const {
  LOG4CXX_AUTO_TRACE(logger_);
  mobile_apis::ModuleType::eType module_type =
      static_cast<mobile_apis::ModuleType::eType>(
          (*message_)[app_mngr::strings::msg_params]
                     [message_params::kModuleData][message_params::kModuleType]
                         .asUInt());
  const char* str;
  const bool ok = ns_smart_device_link::ns_smart_objects::EnumConversionHelper<
      mobile_apis::ModuleType::eType>::EnumToCString(module_type, &str);
  return ok ? str : "unknown";
}

std::string SetInteriorVehicleDataRequest::ModuleId() const {
  LOG4CXX_AUTO_TRACE(logger_);
  auto msg_params = (*message_)[app_mngr::strings::msg_params];
  if (msg_params[message_params::kModuleData].keyExists(
          message_params::kModuleId)) {
    return msg_params[message_params::kModuleData][message_params::kModuleId]
        .asString();
  }
  if (enums_value::kSeat == ModuleType()) {
    const auto id = static_cast<mobile_apis::SupportedSeat::eType>(
        msg_params[message_params::kModuleData]
                  [message_params::kSeatControlData][message_params::kId]
                      .asUInt());
    return rc_capabilities_manager_.GetModuleIdForSeatLocation(id);
  }
  const std::string module_id =
      rc_capabilities_manager_.GetDefaultModuleIdFromCapabilities(ModuleType());
  return module_id;
}

AcquireResult::eType SetInteriorVehicleDataRequest::AcquireResource(
    const app_mngr::commands::MessageSharedPtr& message) {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string module_type = ModuleType();
  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());

  return resource_allocation_manager_.AcquireResource(
      module_type, ModuleId(), app->app_id());
}

bool SetInteriorVehicleDataRequest::IsResourceFree(
    const std::string& module_type, const std::string& module_id) const {
  return resource_allocation_manager_.IsResourceFree(module_type, module_id);
}

void SetInteriorVehicleDataRequest::SetResourceState(
    const std::string& module_type, const ResourceState::eType state) {
  LOG4CXX_AUTO_TRACE(logger_);
  app_mngr::ApplicationSharedPtr app =
      application_manager_.application(CommandRequestImpl::connection_key());
  resource_allocation_manager_.SetResourceState(
      module_type, ModuleId(), app->app_id(), state);
}

}  // namespace commands
}  // namespace rc_rpc_plugin
