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

#include <algorithm>
#include <ctime>
#include <numeric>
#include <vector>

#include "rc_rpc_plugin/commands/mobile/get_interior_vehicle_data_consent_request.h"
#include "rc_rpc_plugin/rc_helpers.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/rc_rpc_types.h"
#include "smart_objects/enum_schema_item.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;
namespace commands {

GetInteriorVehicleDataConsentRequest::GetInteriorVehicleDataConsentRequest(
    const application_manager::commands::MessageSharedPtr& message,
    const RCCommandParams& params)
    : RCCommandRequest(message, params) {}

void GetInteriorVehicleDataConsentRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  auto& msg_params = (*message_)[app_mngr::strings::msg_params];

  const bool module_ids_exists =
      msg_params.keyExists(message_params::kModuleIds);
  if (!module_ids_exists) {
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "ModuleIds collection is absent in request message");
    return;
  }

  if (msg_params[message_params::kModuleIds].empty()) {
    LOG4CXX_DEBUG(logger_,
                  "ModuleIds collection is empty. Will be add default "
                  "module_id from capabilities");

    const auto module_id =
        rc_capabilities_manager_.GetDefaultModuleIdFromCapabilities(
            ModuleType());

    msg_params[message_params::kModuleIds][0] = module_id;
  }

  const std::string module_type = ModuleType();
  for (const auto module_id :
       *(msg_params[message_params::kModuleIds].asArray())) {
    const ModuleUid module(module_type, module_id.asString());
    if (!rc_capabilities_manager_.CheckIfModuleExistsInCapabilities(module)) {
      LOG4CXX_WARN(logger_,
                   "Accessing not supported module: " << module_type << " "
                                                      << module_id.asString());
      SetResourceState(module_type, ResourceState::FREE);
      SendResponse(false,
                   mobile_apis::Result::UNSUPPORTED_RESOURCE,
                   "Accessing not supported module data");
      return;
    }
  }

  smart_objects::SmartObject location_consents;
  GetLocationConsents(location_consents);

  smart_objects::SmartObject response_params;
  if (GetCalculatedVehicleDataConsent(location_consents, response_params)) {
    LOG4CXX_DEBUG(
        logger_,
        "No need to send request to HMI. Sending cached consents to mobile");
    SendResponse(true, mobile_apis::Result::SUCCESS, nullptr, &response_params);
    return;
  }

  (*message_)[application_manager::strings::msg_params]
             [application_manager::strings::app_id] = connection_key();

  LOG4CXX_DEBUG(logger_,
                "Filtering out module ids with serviceArea which does not "
                "cover userLocation");

  smart_objects::SmartObject hmi_msg_params(msg_params);
  auto module_ids_for_consent =
      hmi_msg_params[message_params::kModuleIds].asArray();
  module_ids_for_consent->clear();

  auto module_ids = msg_params[message_params::kModuleIds].asArray();
  for (uint32_t i = 0; i < module_ids->size(); i++) {
    // Only add modules whose serviceArea covers the userLocation
    if (location_consents[i].asBool()) {
      module_ids_for_consent->push_back((*module_ids)[i]);
    }
  }

  SendHMIRequest(hmi_apis::FunctionID::RC_GetInteriorVehicleDataConsent,
                 (&hmi_msg_params),
                 true);
}

void GetInteriorVehicleDataConsentRequest::on_event(
    const app_mngr::event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (event.id() != hmi_apis::FunctionID::RC_GetInteriorVehicleDataConsent) {
    LOG4CXX_ERROR(logger_, "Received wrong event. FunctionID: " << event.id());
    return;
  }

  auto temp_response = event.smart_object();

  auto result_code =
      GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
          temp_response[app_mngr::strings::params][app_mngr::hmi_response::code]
              .asUInt()));

  const bool success_result =
      helpers::Compare<mobile_apis::Result::eType, helpers::EQ, helpers::ONE>(
          result_code,
          mobile_apis::Result::SUCCESS,
          mobile_apis::Result::WARNINGS);

  if (!success_result) {
    std::string info;
    GetInfo(temp_response, info);
    SendResponse(false, result_code, info.c_str(), nullptr);
    return;
  }

  if (!temp_response[app_mngr::strings::msg_params].keyExists(
          message_params::kAllowed) ||
      temp_response[app_mngr::strings::msg_params][message_params::kAllowed]
          .empty()) {
    std::string info_out =
        "Collection of consents is absent in HMI response or empty";
    LOG4CXX_ERROR(logger_, info_out);
    SendResponse(false, mobile_apis::Result::GENERIC_ERROR, info_out.c_str());
    return;
  }

  LOG4CXX_DEBUG(logger_,
                "Adding back filtered out module ids for response to mobile");

  smart_objects::SmartObject all_consents;
  GetLocationConsents(all_consents);
  uint32_t number_of_expected_response_consents = std::accumulate(
      all_consents.asArray()->begin(),
      all_consents.asArray()->end(),
      uint32_t(0),
      [](uint32_t num_consents, smart_objects::SmartObject& consent) {
        // Only module ids with valid location consents were sent to the hmi
        return (consent.asBool()) ? num_consents + 1 : num_consents;
      });

  const auto response_consents =
      temp_response[app_mngr::strings::msg_params][message_params::kAllowed]
          .asArray();

  if (number_of_expected_response_consents != response_consents->size()) {
    std::string info_out =
        "HMI response has incorrect number of consents expected: " +
        std::to_string(number_of_expected_response_consents) +
        ", actual: " + std::to_string(response_consents->size());
    LOG4CXX_ERROR(logger_, info_out);
    SendResponse(false, mobile_apis::Result::GENERIC_ERROR, info_out.c_str());
    return;
  }
  uint32_t response_consents_counter = 0;

  for (auto& consent : *(all_consents.asArray())) {
    // Only modify consent for moduleIds allowed by location constraints
    if (consent.asBool()) {
      consent = (*response_consents)[response_consents_counter];
      response_consents_counter++;
    }
  }

  temp_response[app_mngr::strings::msg_params][message_params::kAllowed] =
      all_consents;

  const auto hmi_response = temp_response;
  std::string response_info;
  const bool result_of_saving = SaveModuleIdConsents(
      response_info, hmi_response[app_mngr::strings::msg_params]);

  if (!result_of_saving) {
    LOG4CXX_DEBUG(logger_, "Consent saving failed");
    SendResponse(
        false, mobile_apis::Result::GENERIC_ERROR, response_info.c_str());
    return;
  }

  smart_objects::SmartObject response_params =
      hmi_response[app_mngr::strings::msg_params];
  std::string info;
  GetInfo(hmi_response, info);
  SendResponse(success_result,
               result_code,
               info.c_str(),
               success_result ? &response_params : nullptr);
}

std::string GetInteriorVehicleDataConsentRequest::ModuleType() const {
  mobile_apis::ModuleType::eType module_type =
      static_cast<mobile_apis::ModuleType::eType>(
          (*message_)[app_mngr::strings::msg_params]
                     [message_params::kModuleType]
                         .asUInt());

  const char* str;
  const bool ok = smart_objects::EnumConversionHelper<
      mobile_apis::ModuleType::eType>::EnumToCString(module_type, &str);
  return ok ? str : "unknown";
}

std::string GetInteriorVehicleDataConsentRequest::ModuleId() const {
  return std::string();
}

GetInteriorVehicleDataConsentRequest::~GetInteriorVehicleDataConsentRequest() {}

void GetInteriorVehicleDataConsentRequest::GetLocationConsents(
    smart_objects::SmartObject& location_consents) {
  location_consents =
      smart_objects::SmartObject(smart_objects::SmartType::SmartType_Array);

  auto modules_consent_array = location_consents.asArray();
  const auto module_ids =
      (*message_)[app_mngr::strings::msg_params][message_params::kModuleIds]
          .asArray();
  const std::string module_type = ModuleType();

  auto is_user_location_valid = [this](ModuleUid& module) {
    const auto app_ptr = application_manager_.application(connection_key());
    const auto extension = RCHelpers::GetRCExtension(*app_ptr);
    const auto user_location = extension->GetUserLocation();
    const auto module_service_area =
        rc_capabilities_manager_.GetModuleServiceArea(module);
    const auto driver =
        rc_capabilities_manager_.GetDriverLocationFromSeatLocationCapability();
    const bool is_driver = user_location == driver;
    if (is_driver || user_location.IntersectionExists(module_service_area)) {
      return true;
    }
    return false;
  };

  for (auto& module_id : (*module_ids)) {
    bool consent = true;
    if (rc_capabilities_manager_.IsSeatLocationCapabilityProvided()) {
      ModuleUid module(module_type, module_id.asString());
      consent = is_user_location_valid(module);
    }
    modules_consent_array->push_back(smart_objects::SmartObject(consent));
  }
}

bool GetInteriorVehicleDataConsentRequest::GetCalculatedVehicleDataConsent(
    const smart_objects::SmartObject& location_consents,
    smart_objects::SmartObject& out_response) const {
  LOG4CXX_AUTO_TRACE(logger_);

  out_response =
      smart_objects::SmartObject(smart_objects::SmartType::SmartType_Map);
  out_response[message_params::kAllowed] =
      smart_objects::SmartObject(smart_objects::SmartType::SmartType_Array);

  auto modules_consent_array = out_response[message_params::kAllowed].asArray();
  const auto module_ids =
      (*message_)[app_mngr::strings::msg_params][message_params::kModuleIds]
          .asArray();

  if (!MultipleAccessAllowed(
          (*module_ids), location_consents, (*modules_consent_array))) {
    return true;
  }

  auto fill_auto_allow_consents =
      [&module_ids,
       &location_consents](smart_objects::SmartArray& out_consents_array) {
        for (uint32_t i = 0; i < module_ids->size(); ++i) {
          out_consents_array.push_back(smart_objects::SmartObject(
              true && location_consents[i].asBool()));
        }
      };

  auto fill_auto_deny_consents =
      [this, &module_ids, &location_consents](
          smart_objects::SmartArray& out_consents_array) {
        const std::string module_type = ModuleType();
        auto app = application_manager_.application(connection_key());
        const uint32_t app_id = app->app_id();
        for (uint32_t i = 0; i < module_ids->size(); ++i) {
          const ModuleUid module_uid(module_type, (*module_ids)[i].asString());
          const bool is_resource_available =
              (resource_allocation_manager_.AcquireResource(
                   module_uid.first, module_uid.second, app_id) ==
               AcquireResult::ALLOWED);

          out_consents_array.push_back(smart_objects::SmartObject(
              is_resource_available && location_consents[i].asBool()));
        }
      };

  auto fill_ask_driver_consents =
      [this, &module_ids, &location_consents](
          smart_objects::SmartArray& out_consents_array) {
        auto app = application_manager_.application(connection_key());
        const std::string policy_app_id = app->policy_app_id();
        const std::string mac_address = app->mac_address();
        const std::string module_type = ModuleType();

        for (uint32_t i = 0; i < module_ids->size(); ++i) {
          const ModuleUid module_uid(module_type, (*module_ids)[i].asString());
          auto consent = rc_consent_manager_.GetModuleConsent(
              policy_app_id, mac_address, module_uid);

          if (rc_rpc_types::ModuleConsent::NOT_EXISTS == consent) {
            const bool is_resource_available =
                resource_allocation_manager_.AcquireResource(
                    module_uid.first, module_uid.second, app->app_id()) ==
                AcquireResult::ALLOWED;

            const bool is_resource_rejected =
                resource_allocation_manager_.AcquireResource(
                    module_uid.first, module_uid.second, app->app_id()) ==
                AcquireResult::REJECTED;

            if (!is_resource_available && !is_resource_rejected) {
              out_consents_array.clear();
              break;
            }

            out_consents_array.push_back(smart_objects::SmartObject(
                is_resource_available && location_consents[i].asBool()));
            continue;
          }

          const bool is_resource_available =
              rc_rpc_types::ModuleConsent::CONSENTED == consent;
          out_consents_array.push_back(smart_objects::SmartObject(
              is_resource_available && location_consents[i].asBool()));
        }
      };

  const auto access_mode = resource_allocation_manager_.GetAccessMode();
  if (hmi_apis::Common_RCAccessMode::AUTO_ALLOW == access_mode) {
    LOG4CXX_DEBUG(logger_,
                  "Current access mode is AUTO_ALLOW - returning successful "
                  "consents for all");
    fill_auto_allow_consents(*modules_consent_array);
    return true;
  }

  if (hmi_apis::Common_RCAccessMode::AUTO_DENY == access_mode) {
    LOG4CXX_DEBUG(logger_,
                  "Current access mode is AUTO_DENY - returning true only for "
                  "FREE resources");
    fill_auto_deny_consents(*modules_consent_array);
    return true;
  }

  if (hmi_apis::Common_RCAccessMode::ASK_DRIVER == access_mode) {
    LOG4CXX_DEBUG(
        logger_,
        "Current access mode is ASK_DRIVER - returning consents from cache");
    fill_ask_driver_consents(*modules_consent_array);
    if (!modules_consent_array->empty()) {
      LOG4CXX_DEBUG(logger_, "Returning consents from cache directly");
      return true;
    }
  }

  LOG4CXX_DEBUG(
      logger_,
      "Can't provide calculated consents - should send request to HMI");
  return false;
}

bool GetInteriorVehicleDataConsentRequest::MultipleAccessAllowed(
    const smart_objects::SmartArray& module_ids,
    const smart_objects::SmartObject& location_consents,
    smart_objects::SmartArray& out_consents_array) const {
  LOG4CXX_AUTO_TRACE(logger_);
  for (uint32_t i = 0; i < module_ids.size(); ++i) {
    const std::string module_type = ModuleType();
    auto app = application_manager_.application(connection_key());
    const uint32_t app_id = app->app_id();
    const ModuleUid module_uid(module_type, module_ids[i].asString());
    const bool is_multiple_access_allowed =
        rc_capabilities_manager_.IsMultipleAccessAllowed(module_uid);
    if (!is_multiple_access_allowed) {
      const bool is_resource_free =
          (resource_allocation_manager_.AcquireResource(
               module_uid.first, module_uid.second, app_id) ==
           AcquireResult::ALLOWED);

      out_consents_array.push_back(smart_objects::SmartObject(
          is_resource_free && location_consents[i].asBool()));
    } else {
      out_consents_array.clear();
      break;
    }
  }

  if (!out_consents_array.empty()) {
    LOG4CXX_DEBUG(logger_,
                  "Multiple access disallowed, returning true only for "
                  "FREE resources");
    return false;
  }
  return true;
}

bool GetInteriorVehicleDataConsentRequest::SaveModuleIdConsents(
    std::string& info_out, const smart_objects::SmartObject& msg_params) {
  LOG4CXX_AUTO_TRACE(logger_);

  const auto& allowed = msg_params[message_params::kAllowed];
  const auto& moduleIds =
      (*message_)[app_mngr::strings::msg_params][message_params::kModuleIds];

  if (allowed.length() != moduleIds.length()) {
    info_out =
        "The received module_id collection from mobile and received consent "
        "collection from HMI are not equal by size.";
    LOG4CXX_ERROR(logger_, info_out);
    return false;
  }
  std::string module_type = ModuleType();
  auto module_ids = RCHelpers::RetrieveModuleIds(moduleIds);
  auto module_allowed = RCHelpers::RetrieveModuleConsents(allowed);

  auto module_consents =
      RCHelpers::FillModuleConsents(module_type, module_ids, module_allowed);

  auto application = application_manager_.application(connection_key());
  if (!application) {
    LOG4CXX_ERROR(logger_,
                  "Application with connection key:" << connection_key()
                                                     << " isn't registered");
    return false;
  }
  std::string policy_app_id = application->policy_app_id();
  const auto mac_address = application->mac_address();
  rc_consent_manager_.SaveModuleConsents(
      policy_app_id, mac_address, module_consents);
  return true;
}

}  // namespace commands
}  // namespace rc_rpc_plugin
