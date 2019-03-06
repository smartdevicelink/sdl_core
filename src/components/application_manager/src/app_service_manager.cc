/*
 * Copyright (c) 2018, Ford Motor Company
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

#include "application_manager/help_prompt_manager_impl.h"

#include <algorithm>
#include <iterator>

#include "application_manager/application.h"
#include "application_manager/application_manager.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "encryption/hashing.h"
#include "resumption/last_state.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/logger.h"

#include "smart_objects/enum_schema_item.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "AppServiceManager")

namespace application_manager {

const char* kAppServiceSection = "AppServices";
const char* kDefaults = "defaults";

AppServiceManager::AppServiceManager(ApplicationManager& app_manager,
                                     resumption::LastState& last_state)
    : app_manager_(app_manager), last_state_(last_state) {}

AppServiceManager::~AppServiceManager() {
  LOG4CXX_AUTO_TRACE(logger_);
}

smart_objects::SmartObject AppServiceManager::PublishAppService(
    const smart_objects::SmartObject& manifest,
    const bool mobile_service,
    const uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::string str_to_hash = "";
  std::string service_id = "";

  published_services_lock_.Acquire();
  do {
    str_to_hash = manifest[strings::service_type].asString() +
                  std::to_string(std::rand());
    service_id = encryption::MakeHash(str_to_hash);
  } while (published_services_.find(service_id) != published_services_.end());

  AppService app_service;
  app_service.connection_key = connection_key;
  app_service.mobile_service = mobile_service;

  smart_objects::SmartObject service_record;
  service_record[strings::service_manifest] = manifest;
  service_record[strings::service_id] = service_id;
  service_record[strings::service_published] = true;
  service_record[strings::service_active] = false;
  app_service.record = service_record;

  std::string service_type = manifest[strings::service_type].asString();

  std::string default_app_id = DefaultServiceByType(service_type);
  if (default_app_id.empty() && !mobile_service) {
    auto embedded_services = app_manager_.get_settings().embedded_services();
    for (auto it = embedded_services.begin(); it != embedded_services.end();
         ++it) {
      if (*it == service_type) {
        Json::Value& dictionary = last_state_.get_dictionary();
        dictionary[kAppServiceSection][kDefaults][service_type] =
            kEmbeddedService;
        default_app_id = kEmbeddedService;
      }
    }
  }
  app_service.default_service = GetPolicyAppID(app_service) == default_app_id;

  auto ret = published_services_.insert(
      std::pair<std::string, AppService>(service_id, app_service));
  smart_objects::SmartObject& published_record = ret.first->second.record;
  published_services_lock_.Release();

  smart_objects::SmartObject msg_params;
  msg_params[strings::system_capability][strings::system_capability_type] =
      mobile_apis::SystemCapabilityType::APP_SERVICES;
  AppServiceUpdated(
      service_record, mobile_apis::ServiceUpdateReason::PUBLISHED, msg_params);

  MessageHelper::BroadcastCapabilityUpdate(msg_params, app_manager_);

  // Activate the new service if it is the default for its service type, or if
  // no service is active of its service type
  AppService* active_service = ActiveServiceByType(service_type);
  if (!active_service || app_service.default_service) {
    ActivateAppService(service_id);
  }

  return published_record;
}

bool AppServiceManager::UnpublishAppService(const std::string service_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Service id does not exist in published services");
    return false;
  }
  LOG4CXX_DEBUG(logger_, "Unpublishing app service: " << service_id);

  SetServicePublished(service_id, false);
  smart_objects::SmartObject msg_params;
  msg_params[strings::system_capability][strings::system_capability_type] =
      mobile_apis::SystemCapabilityType::APP_SERVICES;

  auto& record = it->second.record;
  if (record[strings::service_active].asBool()) {
    record[strings::service_active] = false;

    // Activate embedded service, if available
    auto embedded_service = EmbeddedServiceForType(
        record[strings::service_manifest][strings::service_type].asString());
    if (embedded_service) {
      embedded_service->record[strings::service_active] = true;
      AppServiceUpdated(embedded_service->record,
                        mobile_apis::ServiceUpdateReason::ACTIVATED,
                        msg_params);
    }
  }
  AppServiceUpdated(
      record, mobile_apis::ServiceUpdateReason::REMOVED, msg_params);
  MessageHelper::BroadcastCapabilityUpdate(msg_params, app_manager_);
  published_services_.erase(it);

  return true;
}

void AppServiceManager::UnpublishServices(const uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Unpublishing all app services: " << connection_key);
  sync_primitives::AutoLock lock(published_services_lock_);
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    if (it->second.connection_key == connection_key) {
      UnpublishAppService(it->first);
    }
  }
}

void AppServiceManager::OnAppActivated(ApplicationConstSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.begin();
  // Activate all services published by the app
  for (; it != published_services_.end(); ++it) {
    if (it->second.connection_key == app->app_id()) {
      ActivateAppService(it->first);
    }
  }
}

std::vector<smart_objects::SmartObject> AppServiceManager::GetAllServices() {
  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<smart_objects::SmartObject> services;
  sync_primitives::AutoLock lock(published_services_lock_);
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    services.push_back(it->second.record);
  }
  return services;
}

void AppServiceManager::GetProviderByType(const std::string& service_type,
                                          bool mobile_consumer,
                                          ApplicationSharedPtr& app,
                                          bool& hmi_service) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto active_service = ActiveServiceByType(service_type);
  if (!active_service) {
    LOG4CXX_ERROR(logger_,
                  "There is no active service for the given service type: "
                      << service_type);
    return;
  }

  LOG4CXX_DEBUG(logger_, "Found provider for service type: " << service_type);
  GetProviderFromService(*active_service, mobile_consumer, app, hmi_service);
}

void AppServiceManager::GetProviderByID(const std::string& service_id,
                                        bool mobile_consumer,
                                        ApplicationSharedPtr& app,
                                        bool& hmi_service) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Service id does not exist in published services");
    return;
  }

  LOG4CXX_DEBUG(logger_, "Found provider with service ID: " << service_id);
  GetProviderFromService(it->second, mobile_consumer, app, hmi_service);
}

void AppServiceManager::GetProviderFromService(const AppService& service,
                                               bool mobile_consumer,
                                               ApplicationSharedPtr& app,
                                               bool& hmi_service) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (mobile_consumer &&
      !service.record[strings::service_manifest][strings::allow_app_consumers]
           .asBool()) {
    LOG4CXX_ERROR(logger_, "Service does not support app consumers");
    return;
  }
  bool mobile_service = service.mobile_service;
  if (mobile_service) {
    app = app_manager_.application(service.connection_key);
    hmi_service = false;
    return;
  }
  hmi_service = true;
}

bool AppServiceManager::SetDefaultService(const std::string service_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Unable to find published service " << service_id);
    return false;
  }
  AppService& service = it->second;

  std::string service_type =
      service.record[strings::service_manifest][strings::service_type]
          .asString();
  std::string default_app_id = DefaultServiceByType(service_type);
  if (!default_app_id.empty()) {
    auto default_service =
        FindServiceByPolicyAppID(default_app_id, service_type);
    if (default_service) {
      default_service->default_service = false;
    }
  }
  service.default_service = true;

  Json::Value& dictionary = last_state_.get_dictionary();
  dictionary[kAppServiceSection][kDefaults][service_type] =
      GetPolicyAppID(service);
  return true;
}

bool AppServiceManager::RemoveDefaultService(const std::string service_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Unable to find published service " << service_id);
    return false;
  }

  AppService& service = it->second;
  if (!service.default_service) {
    LOG4CXX_ERROR(logger_, "Service was not default " << service_id);
    return false;
  }
  service.default_service = false;

  std::string service_type =
      service.record[strings::service_manifest][strings::service_type]
          .asString();
  Json::Value& dictionary = last_state_.get_dictionary();
  dictionary[kAppServiceSection][kDefaults].removeMember(service_type);
  return true;
}

bool AppServiceManager::ActivateAppService(const std::string service_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Unable to find published service " << service_id);
    return false;
  }

  smart_objects::SmartObject& service = it->second.record;
  if (service[strings::service_active].asBool()) {
    LOG4CXX_DEBUG(logger_, "Service was already active " << service_id);
    return true;
  }

  smart_objects::SmartObject msg_params;
  msg_params[strings::system_capability][strings::system_capability_type] =
      mobile_apis::SystemCapabilityType::APP_SERVICES;

  const std::string service_type =
      service[strings::service_manifest][strings::service_type].asString();
  auto active_service = ActiveServiceByType(service_type);
  if (active_service) {
    active_service->record[strings::service_active] = false;
    AppServiceUpdated(active_service->record,
                      mobile_apis::ServiceUpdateReason::DEACTIVATED,
                      msg_params);
  }
  service[strings::service_active] = true;
  AppServiceUpdated(
      service, mobile_apis::ServiceUpdateReason::ACTIVATED, msg_params);

  MessageHelper::BroadcastCapabilityUpdate(msg_params, app_manager_);

  std::string navi_service_type;
  ns_smart_device_link::ns_smart_objects::
      EnumConversionHelper<mobile_apis::AppServiceType::eType>::EnumToString(
          mobile_apis::AppServiceType::NAVIGATION, &navi_service_type);
  if (service_type == navi_service_type) {
    smart_objects::SmartObject msg_params;
    msg_params[strings::system_capability][strings::system_capability_type] =
        mobile_apis::SystemCapabilityType::NAVIGATION;
    MessageHelper::BroadcastCapabilityUpdate(msg_params, app_manager_);
  }
  return true;
}

bool AppServiceManager::DeactivateAppService(const std::string service_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Unable to find published service " << service_id);
    return false;
  }
  smart_objects::SmartObject& service = it->second.record;
  smart_objects::SmartObject msg_params;
  bool send_update = false;

  if (service[strings::service_active].asBool()) {
    service[strings::service_active] = false;
    AppServiceUpdated(
        service, mobile_apis::ServiceUpdateReason::DEACTIVATED, msg_params);

    // Activate embedded service, if available
    const std::string service_type =
        service[strings::service_manifest][strings::service_type].asString();
    auto embedded_service = EmbeddedServiceForType(service_type);
    if (embedded_service) {
      embedded_service->record[strings::service_active] = true;
      AppServiceUpdated(embedded_service->record,
                        mobile_apis::ServiceUpdateReason::ACTIVATED,
                        msg_params);
    }
    send_update = true;
  }

  if (send_update) {
    msg_params[strings::system_capability][strings::system_capability_type] =
        mobile_apis::SystemCapabilityType::APP_SERVICES;
    MessageHelper::BroadcastCapabilityUpdate(msg_params, app_manager_);
  }
  return true;
}

AppService* AppServiceManager::ActiveServiceByType(std::string service_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(published_services_lock_);
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    if (it->second.record[strings::service_manifest][strings::service_type]
                .asString() == service_type &&
        it->second.record[strings::service_published].asBool() &&
        it->second.record[strings::service_active].asBool()) {
      return &(it->second);
    }
  }
  return NULL;
}

AppService* AppServiceManager::EmbeddedServiceForType(
    std::string service_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(published_services_lock_);
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    if (it->second.record[strings::service_manifest][strings::service_type]
                .asString() == service_type &&
        !it->second.mobile_service) {
      return &(it->second);
    }
  }
  return NULL;
}

AppService* AppServiceManager::FindServiceByName(std::string name) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(published_services_lock_);
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    if (it->second.record[strings::service_manifest][strings::service_name]
            .asString() == name) {
      return &(it->second);
    }
  }
  return NULL;
}

AppService* AppServiceManager::FindServiceByPolicyAppID(
    std::string policy_app_id, std::string type) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(published_services_lock_);
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    if (it->second.record[strings::service_manifest][strings::service_type]
            .asString() != type) {
      continue;
    }

    if (policy_app_id == GetPolicyAppID(it->second)) {
      return &(it->second);
    }
  }
  return NULL;
}

AppService* AppServiceManager::FindServiceByID(std::string service_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(published_services_lock_);

  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Service id does not exist in published services");
    return NULL;
  }
  return &(it->second);
}

std::string AppServiceManager::DefaultServiceByType(std::string service_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  Json::Value& dictionary = last_state_.get_dictionary();
  if (dictionary[kAppServiceSection][kDefaults].isMember(service_type)) {
    return dictionary[kAppServiceSection][kDefaults][service_type].asString();
  }
  return std::string();
}

void AppServiceManager::SetServicePublished(const std::string service_id,
                                            bool service_published) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Service id does not exist in published services");
    return;
  }
  it->second.record[strings::service_published] = service_published;
}

std::string AppServiceManager::GetPolicyAppID(AppService service) {
  if (service.mobile_service) {
    auto app = app_manager_.application(service.connection_key);
    return app ? app->policy_app_id() : std::string();
  }
  return kEmbeddedService;
}

bool AppServiceManager::UpdateNavigationCapabilities(
    smart_objects::SmartObject& out_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::string navi_service_type;
  ns_smart_device_link::ns_smart_objects::
      EnumConversionHelper<mobile_apis::AppServiceType::eType>::EnumToString(
          mobile_apis::AppServiceType::NAVIGATION, &navi_service_type);
  auto service = ActiveServiceByType(navi_service_type);
  if (!service) {
    return false;
  }

  if (!out_params.keyExists(strings::send_location_enabled)) {
    out_params[strings::send_location_enabled] = false;
  }
  if (!out_params.keyExists(strings::get_way_points_enabled)) {
    out_params[strings::get_way_points_enabled] = false;
  }

  if (!service->record[strings::service_manifest].keyExists(
          strings::handled_rpcs)) {
    return true;
  }

  smart_objects::SmartObject& handled_rpcs =
      service->record[strings::service_manifest][strings::handled_rpcs];
  for (size_t i = 0; i < handled_rpcs.length(); ++i) {
    if (handled_rpcs[i].asInt() == mobile_apis::FunctionID::SendLocationID) {
      out_params[strings::send_location_enabled] = true;
    } else if (handled_rpcs[i].asInt() ==
               mobile_apis::FunctionID::GetWayPointsID) {
      out_params[strings::get_way_points_enabled] = true;
    }
  }
  return true;
}

void AppServiceManager::AppServiceUpdated(
    const smart_objects::SmartObject& service_record,
    const mobile_apis::ServiceUpdateReason::eType update_reason,
    smart_objects::SmartObject& msg_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject& services =
      msg_params[strings::system_capability][strings::app_services_capabilities]
                [strings::app_services];
  smart_objects::SmartObject service(smart_objects::SmartType_Map);
  service[strings::update_reason] = update_reason;
  service[strings::updated_app_service_record] = service_record;
  services[-1] = service;
}

}  //  namespace application_manager
