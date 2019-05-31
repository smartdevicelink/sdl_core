/*
 Copyright (c) 2019, Ford Motor Company
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

#include <algorithm>

#include "application_manager/application_manager.h"
#include "application_manager/smart_object_keys.h"
#include "json/json.h"
#include "rc_rpc_plugin/rc_consent_manager_impl.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "resumption/last_state.h"
#include "smart_objects/smart_object.h"
#include "utils/date_time.h"
#include "utils/logger.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;

CREATE_LOGGERPTR_GLOBAL(logger_, "RCConsentManager")

RCConsentManagerImpl::RCConsentManagerImpl(
    resumption::LastState& last_state,
    application_manager::ApplicationManager& application_manager,
    const uint32_t period_of_consent_expired)
    : app_manager_(application_manager)
    , last_state_(last_state)
    , period_of_consent_expired_(period_of_consent_expired) {}

void RCConsentManagerImpl::SaveModuleConsents(
    const std::string& policy_app_id,
    const std::string& mac_address,
    const rc_rpc_types::ModuleIdConsentVector& module_consents) {
  LOG4CXX_AUTO_TRACE(logger_);

  for (const auto& consent : module_consents) {
    std::string module_type = consent.module_id.first;

    auto& app_module_consents =
        GetModuleTypeConsentsOrAppend(policy_app_id, mac_address, module_type);
    SaveAppModuleConsent(app_module_consents, consent);
  }
}

rc_rpc_types::ModuleConsent RCConsentManagerImpl::GetModuleConsent(
    const std::string& app_id,
    const std::string& mac_address,
    const rc_rpc_types::ModuleUid& module_id) const {
  sync_primitives::AutoLock autolock(module_consents_lock_);

  auto& module_consents =
      GetModuleTypeConsentsOrAppend(app_id, mac_address, module_id.first);
  if (module_consents.empty()) {
    LOG4CXX_DEBUG(
        logger_,
        "For app: " << app_id << " module type doesn't have any consents");
    return rc_rpc_types::ModuleConsent::NOT_EXISTS;
  }

  for (const auto& conset_item : module_consents) {
    if (conset_item[message_params::kModuleId].asString() == module_id.second) {
      return static_cast<rc_rpc_types::ModuleConsent>(
          conset_item[message_params::kConsent].asUInt());
    }
  }

  LOG4CXX_DEBUG(logger_,
                "For app: " << app_id << " and module resource ["
                            << module_id.first << ":" << module_id.second
                            << "] consent is absent");

  return rc_rpc_types::ModuleConsent::NOT_EXISTS;
}

void RCConsentManagerImpl::RemoveExpiredConsents() {
  LOG4CXX_AUTO_TRACE(logger_);
  auto& remote_control = GetRemoteControlDataOrAppend();
  if (remote_control.empty()) {
    return;
  }

  auto& devices = GetAppsConsentsOrAppend();

  sync_primitives::AutoLock autolock(device_applications_lock_);

  for (auto& device_item : devices) {
    RemoveDeviceExpiredConsents(device_item);
  }
}

void RCConsentManagerImpl::RemoveAllConsents() {
  auto& remote_control = GetRemoteControlDataOrAppend();
  remote_control.removeMember(message_params::kAppConsents);
}

rc_rpc_types::ModuleConsentState RCConsentManagerImpl::CheckModuleConsentState(
    const Json::Value& module_consent) const {
  if (!module_consent.isMember(message_params::kConsentDate)) {
    LOG4CXX_DEBUG(logger_, "Date of consent is absent");
    return rc_rpc_types::ModuleConsentState::NOT_EXISTS;
  }

  const uint32_t period_for_expiring =
      app_manager_.get_settings().period_for_consent_expiration();

  const time_t consent_date = static_cast<time_t>(
      module_consent[message_params::kConsentDate].asUInt());

  const auto past_period_in_days =
      date_time::calculateAmountDaysFromDate(consent_date);

  LOG4CXX_DEBUG(
      logger_,
      "Keeping time of consent is: " << past_period_in_days << " days");
  LOG4CXX_DEBUG(logger_, "Period for expired: " << period_for_expiring);

  return (past_period_in_days >= period_for_expiring)
             ? rc_rpc_types::ModuleConsentState::EXPIRED
             : rc_rpc_types::ModuleConsentState::ACTIVE;
}

void RCConsentManagerImpl::RemoveDeviceExpiredConsents(Json::Value& device) {
  sync_primitives::AutoLock autolock(applications_lock_);

  if (device.isMember(message_params::kApplications)) {
    auto& applications = device[message_params::kApplications];

    for (auto& app : applications) {
      if (app.isMember(message_params::kAppConsentList)) {
        RemoveAppExpiredConsents(app[message_params::kAppConsentList]);
      }
    }
  }
}

void RCConsentManagerImpl::RemoveAppExpiredConsents(Json::Value& app_consents) {
  for (auto& module : app_consents) {
    if (module.isMember(message_params::kModuleConsents)) {
      auto& module_consents = module[message_params::kModuleConsents];
      RemoveModuleExpiredConsents(module_consents);
    }
  }
}

void RCConsentManagerImpl::RemoveModuleExpiredConsents(
    Json::Value& module_consents) {
  sync_primitives::AutoLock autolock(module_consents_lock_);
  Json::Value temp_consents;

  for (auto& consent : module_consents) {
    const bool is_module_id_exists =
        consent.isMember(message_params::kModuleId);

    const bool is_expired = rc_rpc_types::ModuleConsentState::EXPIRED ==
                            CheckModuleConsentState(consent);
    if (is_expired) {
      LOG4CXX_DEBUG(logger_,
                    "Consent for module resource ["
                        << consent[message_params::kModuleId].asString()
                        << "] is expired and will be removed");
    }

    if (is_module_id_exists && !is_expired) {
      LOG4CXX_DEBUG(logger_,
                    "Consent for module resource ["
                        << consent[message_params::kModuleId].asString()
                        << "] is actual.");
      temp_consents.append(consent);
    }
  }

  module_consents.clear();
  if (!temp_consents.empty()) {
    std::swap(module_consents, temp_consents);
  }
}

Json::Value& RCConsentManagerImpl::GetRemoteControlDataOrAppend() const {
  Json::Value& dictionary = last_state_.get_dictionary();

  sync_primitives::AutoLock autolock(dictionary_control_lock_);
  if (!dictionary.isMember(app_mngr::strings::remote_control)) {
    dictionary[app_mngr::strings::remote_control] =
        Json::Value(Json::objectValue);
    LOG4CXX_DEBUG(logger_, "remote_control section is missed");
  }

  Json::Value& remote_control = dictionary[app_mngr::strings::remote_control];

  if (!remote_control.isObject()) {
    LOG4CXX_ERROR(logger_, "remote_control type INVALID rewrite");
    remote_control = Json::Value(Json::objectValue);
  }
  return remote_control;
}

Json::Value& RCConsentManagerImpl::GetDeviceApplicationsOrAppend(
    const std::string& mac_address) const {
  sync_primitives::AutoLock autolock(device_applications_lock_);

  auto& apps_consents = GetAppsConsentsOrAppend();

  if (!apps_consents.isArray()) {
    LOG4CXX_DEBUG(logger_, "applications_consents type INVALID rewrite");
    apps_consents = Json::Value(Json::arrayValue);
  }

  for (auto& device_applications_item : apps_consents) {
    const bool is_device_section_exists =
        device_applications_item.isMember(message_params::kMacAddress);

    if (is_device_section_exists) {
      auto saved_mac_adress =
          device_applications_item[message_params::kMacAddress].asString();

      if (saved_mac_adress == mac_address) {
        return device_applications_item[message_params::kApplications];
      }
    }
  }

  auto device_applications = Json::Value(Json::objectValue);
  device_applications[message_params::kMacAddress] = Json::Value(mac_address);
  device_applications[message_params::kApplications] =
      Json::Value(Json::arrayValue);

  apps_consents.append(device_applications);
  return apps_consents[apps_consents.size() - 1][message_params::kApplications];
}

Json::Value& RCConsentManagerImpl::GetAppConsentsListOrAppend(
    const std::string& policy_app_id, const std::string& mac_address) const {
  auto& device_applications = GetDeviceApplicationsOrAppend(mac_address);

  sync_primitives::AutoLock autolock(applications_lock_);
  if (!device_applications.isArray()) {
    LOG4CXX_DEBUG(logger_, "applications_consents type INVALID rewrite");
    device_applications = Json::Value(Json::arrayValue);
  }

  for (auto& application : device_applications) {
    const bool is_app_id_section_exists =
        application.isMember(message_params::kAppId);

    if (is_app_id_section_exists) {
      auto saved_app_id = application[message_params::kAppId].asString();

      if (saved_app_id == policy_app_id) {
        return application[message_params::kAppConsentList];
      }
    }
  }

  // In case when specified application section is absent in json file,
  // will be created new section and added to dictionary.
  auto application = Json::Value(Json::objectValue);
  application[message_params::kAppId] = Json::Value(policy_app_id);
  application[message_params::kAppConsentList] = Json::Value(Json::arrayValue);
  device_applications.append(application);

  // Returns last appended object
  return device_applications[device_applications.size() - 1]
                            [message_params::kAppConsentList];
}

Json::Value& RCConsentManagerImpl::GetAppsConsentsOrAppend() const {
  Json::Value& remote_control = GetRemoteControlDataOrAppend();
  sync_primitives::AutoLock autolock(remote_control_lock_);

  if (!remote_control.isMember(message_params::kAppConsents)) {
    LOG4CXX_DEBUG(logger_, "app_consents section is missed");
    remote_control[message_params::kAppConsents] =
        Json::Value(Json::arrayValue);
  }

  auto& app_consents = remote_control[message_params::kAppConsents];
  if (!app_consents.isArray()) {
    LOG4CXX_DEBUG(logger_, "applications_consents type INVALID rewrite");
    app_consents = Json::Value(Json::arrayValue);
  }
  return app_consents;
}

Json::Value& RCConsentManagerImpl::GetModuleTypeConsentsOrAppend(
    const std::string& policy_app_id,
    const std::string& mac_address,
    const std::string& module_type) const {
  auto& app_consnets_list =
      GetAppConsentsListOrAppend(policy_app_id, mac_address);

  sync_primitives::AutoLock autolock(app_consents_lock_);
  for (auto& module_consents : app_consnets_list) {
    const bool module_exists =
        module_consents.isMember(message_params::kModuleType);

    if (module_exists &&
        (module_consents[message_params::kModuleType].asString() ==
         module_type)) {
      return module_consents[message_params::kModuleConsents];
    }
  }

  // In case of absent specified module_type in section of specified
  // application, will be added empty section with this module type.
  LOG4CXX_DEBUG(
      logger_,
      "Section module_type: " << module_type
                              << " is missed for app_id:" << policy_app_id);
  auto consent_item = Json::Value(Json::objectValue);
  consent_item[message_params::kModuleType] = module_type;
  consent_item[message_params::kModuleConsents] = Json::Value(Json::arrayValue);

  app_consnets_list.append(consent_item);

  // Returns last (appended) object
  return app_consnets_list[app_consnets_list.size() - 1]
                          [message_params::kModuleConsents];
}

RCConsentManagerImpl::~RCConsentManagerImpl() {}

void RCConsentManagerImpl::SaveAppModuleConsent(
    Json::Value& app_module_consents,
    const rc_rpc_types::ModuleIdConsent& consent_to_save) {
  bool is_found = false;

  sync_primitives::AutoLock autolock(app_consents_lock_);
  for (auto& consent : app_module_consents) {
    const bool is_module_id_exists =
        consent.isMember(message_params::kModuleId);

    // In case existing consent of specified ModuelResource (module_type +
    // module_id), old value will be rewritten by new value.

    if (is_module_id_exists && (consent[message_params::kModuleId].asString() ==
                                consent_to_save.module_id.second)) {
      is_found = true;
      consent[message_params::kConsent] =
          Json::Value(static_cast<uint32_t>(consent_to_save.consent));
      consent[message_params::kConsentDate] = Json::Value(
          static_cast<Json::UInt64>(consent_to_save.date_of_consent));
    }
  }

  // Otherwise, new item will be added to the "consents" collection

  if (!is_found) {
    auto new_consent = Json::Value(Json::objectValue);
    new_consent[message_params::kModuleId] =
        Json::Value(consent_to_save.module_id.second);
    new_consent[message_params::kConsent] =
        Json::Value(static_cast<uint32_t>(consent_to_save.consent));
    new_consent[message_params::kConsentDate] =
        Json::Value(static_cast<Json::UInt64>(consent_to_save.date_of_consent));

    app_module_consents.append(new_consent);
  }
}

}  // namespace rc_rpc_plugin
