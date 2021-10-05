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

#include "application_manager/app_service_manager.h"

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

SDL_CREATE_LOG_VARIABLE("AppServiceManager")

namespace application_manager {

const char* kAppServiceSection = "AppServices";
const char* kDefaults = "defaults";

AppServiceManager::AppServiceManager(ApplicationManager& app_manager,
                                     resumption::LastStateWrapperPtr last_state)
    : app_manager_(app_manager)
    , last_state_(last_state)
    , rpc_passing_handler_(*this, app_manager_) {}

AppServiceManager::~AppServiceManager() {
  SDL_LOG_AUTO_TRACE();
}

smart_objects::SmartObject AppServiceManager::PublishAppService(
    const smart_objects::SmartObject& manifest,
    const bool mobile_service,
    const uint32_t connection_key) {
  SDL_LOG_AUTO_TRACE();
  std::string str_to_hash;
  std::string service_id = "";

  std::string service_type = manifest[strings::service_type].asString();

  AppService* found_service =
      FindServiceByProvider(connection_key, service_type);
  if (found_service) {
    // Check if there is a different existing service with the same updated
    // name.
    if (manifest.keyExists(strings::service_name)) {
      auto service_by_name =
          FindServiceByName(manifest[strings::service_name].asString());
      if (service_by_name) {
        auto service_by_name_id =
            service_by_name->record[strings::service_id].asString();
        auto found_service_id =
            found_service->record[strings::service_id].asString();
        if (service_by_name_id != found_service_id) {
          SDL_LOG_WARN("A service already exists with this name, rejecting");
          return smart_objects::SmartObject();
        }
      }
    }
    SDL_LOG_WARN("Service already exists for this provider, updating");
    published_services_lock_.Acquire();
    found_service->record[strings::service_manifest] = manifest;
    found_service->record[strings::service_published] = true;
    smart_objects::SmartObject updated_service_record = found_service->record;
    published_services_lock_.Release();

    smart_objects::SmartObject msg_params;
    msg_params[strings::system_capability][strings::system_capability_type] =
        mobile_apis::SystemCapabilityType::APP_SERVICES;

    AppServiceUpdated(updated_service_record,
                      mobile_apis::ServiceUpdateReason::MANIFEST_UPDATE,
                      msg_params);

    MessageHelper::BroadcastCapabilityUpdate(msg_params, app_manager_);

    return updated_service_record;
  }

  if (manifest.keyExists(strings::service_name) &&
      FindServiceByName(manifest[strings::service_name].asString())) {
    SDL_LOG_WARN("A service already exists with this name, rejecting");
    return smart_objects::SmartObject();
  }

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

  std::string default_app_id = DefaultServiceByType(service_type);
  if (default_app_id.empty() && !mobile_service) {
    auto embedded_services = app_manager_.get_settings().embedded_services();
    for (const auto& embedded_service : embedded_services) {
      if (embedded_service == service_type) {
        auto last_state_accessor = last_state_->get_accessor();
        Json::Value dictionary = last_state_accessor.GetData().dictionary();
        dictionary[kAppServiceSection][kDefaults][service_type] =
            kEmbeddedService;
        default_app_id = kEmbeddedService;
        last_state_accessor.GetMutableData().set_dictionary(dictionary);
      }
    }
  }
  app_service.default_service =
      !default_app_id.empty() && GetPolicyAppID(app_service) == default_app_id;

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

  // Activate the new service if it is the default for its service type, if
  // no service is active of its service type, or it is a mobile app in full.
  AppService* active_service = ActiveServiceForType(service_type);
  ApplicationSharedPtr app = NULL;
  if (mobile_service && connection_key) {
    app = app_manager_.application(connection_key);
  }

  if (!active_service || app_service.default_service ||
      (app && app->IsFullscreen())) {
    ActivateAppService(service_id);
  }

  return published_record;
}

bool AppServiceManager::UnpublishAppService(const std::string service_id) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    SDL_LOG_ERROR("Service id does not exist in published services");
    return false;
  }
  SDL_LOG_DEBUG("Unpublishing app service: " << service_id);

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
    if (embedded_service &&
        (embedded_service->record[strings::service_id].asString() !=
         service_id)) {
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
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("Unpublishing all app services: " << connection_key);

  std::list<std::string> app_published_services;
  {
    sync_primitives::AutoLock lock(published_services_lock_);
    for (auto it = published_services_.begin(); it != published_services_.end();
         ++it) {
      if (it->second.connection_key == connection_key) {
        app_published_services.push_back(it->first);
      }
    }
  }

  for (auto& service_id : app_published_services) {
    UnpublishAppService(service_id);
  }
}

void AppServiceManager::OnAppActivated(ApplicationConstSharedPtr app) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.begin();
  // Activate all services published by the app
  for (; it != published_services_.end(); ++it) {
    if (it->second.connection_key == app->app_id()) {
      ActivateAppService(it->first);
    }
  }
}

std::vector<smart_objects::SmartObject>
AppServiceManager::GetAllServiceRecords() {
  SDL_LOG_AUTO_TRACE();
  std::vector<smart_objects::SmartObject> services;
  sync_primitives::AutoLock lock(published_services_lock_);
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    services.push_back(it->second.record);
  }
  return services;
}

void AppServiceManager::GetProviderByType(const std::string& service_type,
                                          const bool mobile_consumer,
                                          ApplicationSharedPtr& app,
                                          bool& hmi_service) {
  SDL_LOG_AUTO_TRACE();
  auto active_service = ActiveServiceForType(service_type);
  if (!active_service) {
    SDL_LOG_ERROR("There is no active service for the given service type: "
                  << service_type);
    return;
  }

  SDL_LOG_DEBUG("Found provider for service type: " << service_type);
  GetProviderFromService(*active_service, mobile_consumer, app, hmi_service);
}

void AppServiceManager::GetProviderByID(const std::string& service_id,
                                        const bool mobile_consumer,
                                        ApplicationSharedPtr& app,
                                        bool& hmi_service) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    SDL_LOG_ERROR("Service id does not exist in published services");
    return;
  }

  SDL_LOG_DEBUG("Found provider with service ID: " << service_id);
  GetProviderFromService(it->second, mobile_consumer, app, hmi_service);
}

void AppServiceManager::GetProviderFromService(const AppService& service,
                                               const bool mobile_consumer,
                                               ApplicationSharedPtr& app,
                                               bool& hmi_service) {
  SDL_LOG_AUTO_TRACE();
  if (mobile_consumer &&
      !service.record[strings::service_manifest][strings::allow_app_consumers]
           .asBool()) {
    SDL_LOG_ERROR("Service does not support app consumers");
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
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    SDL_LOG_ERROR("Unable to find published service " << service_id);
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

  auto last_state_accessor = last_state_->get_accessor();
  Json::Value dictionary = last_state_accessor.GetData().dictionary();
  dictionary[kAppServiceSection][kDefaults][service_type] =
      GetPolicyAppID(service);
  last_state_accessor.GetMutableData().set_dictionary(dictionary);
  return true;
}

bool AppServiceManager::RemoveDefaultService(const std::string service_id) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    SDL_LOG_ERROR("Unable to find published service " << service_id);
    return false;
  }

  AppService& service = it->second;
  if (!service.default_service) {
    SDL_LOG_ERROR("Service was not default " << service_id);
    return false;
  }
  service.default_service = false;

  std::string service_type =
      service.record[strings::service_manifest][strings::service_type]
          .asString();

  auto last_state_accessor = last_state_->get_accessor();
  Json::Value dictionary = last_state_accessor.GetData().dictionary();
  dictionary[kAppServiceSection][kDefaults].removeMember(service_type);
  last_state_accessor.GetMutableData().set_dictionary(dictionary);
  return true;
}

bool AppServiceManager::ActivateAppService(const std::string service_id) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    SDL_LOG_ERROR("Unable to find published service " << service_id);
    return false;
  }

  smart_objects::SmartObject& service = it->second.record;
  if (service[strings::service_active].asBool()) {
    SDL_LOG_DEBUG("Service was already active " << service_id);
    return true;
  }

  smart_objects::SmartObject msg_params;
  msg_params[strings::system_capability][strings::system_capability_type] =
      mobile_apis::SystemCapabilityType::APP_SERVICES;

  const std::string service_type =
      service[strings::service_manifest][strings::service_type].asString();
  auto active_service = ActiveServiceForType(service_type);
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
    smart_objects::SmartObject params;
    params[strings::system_capability][strings::system_capability_type] =
        mobile_apis::SystemCapabilityType::NAVIGATION;
    MessageHelper::BroadcastCapabilityUpdate(params, app_manager_);
  }
  return true;
}

bool AppServiceManager::DeactivateAppService(const std::string service_id) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    SDL_LOG_ERROR("Unable to find published service " << service_id);
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
    if (embedded_service &&
        embedded_service->record[strings::service_id].asString() !=
            service_id) {
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

AppService* AppServiceManager::ActiveServiceForType(
    const std::string service_type) {
  SDL_LOG_AUTO_TRACE();
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
    const std::string service_type) {
  SDL_LOG_AUTO_TRACE();
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

AppService* AppServiceManager::FindServiceByPolicyAppID(
    const std::string policy_app_id, const std::string type) {
  SDL_LOG_AUTO_TRACE();
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

AppService* AppServiceManager::FindServiceByID(const std::string service_id) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(published_services_lock_);

  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    SDL_LOG_ERROR("Service id does not exist in published services");
    return NULL;
  }
  return &(it->second);
}

AppService* AppServiceManager::FindServiceByProvider(
    const uint32_t connection_key, const std::string service_type) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(published_services_lock_);
  for (auto& service : published_services_) {
    if (service.second.connection_key == connection_key &&
        service.second.record[strings::service_manifest][strings::service_type]
                .asString() == service_type) {
      return &(service.second);
    }
  }
  return NULL;
}

AppService* AppServiceManager::FindServiceByName(std::string name) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(published_services_lock_);
  for (auto& service : published_services_) {
    if (service.second.record[strings::service_manifest][strings::service_name]
            .asString() == name) {
      return &(service.second);
    }
  }
  return NULL;
}

std::string AppServiceManager::DefaultServiceByType(
    const std::string service_type) {
  SDL_LOG_AUTO_TRACE();

  auto last_state_accessor = last_state_->get_accessor();
  Json::Value dictionary = last_state_accessor.GetData().dictionary();
  if (dictionary[kAppServiceSection][kDefaults].isMember(service_type)) {
    return dictionary[kAppServiceSection][kDefaults][service_type].asString();
  }
  return std::string();
}

void AppServiceManager::SetServicePublished(const std::string service_id,
                                            const bool service_published) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(published_services_lock_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    SDL_LOG_ERROR("Service id does not exist in published services");
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
  SDL_LOG_AUTO_TRACE();
  std::string navi_service_type;
  ns_smart_device_link::ns_smart_objects::
      EnumConversionHelper<mobile_apis::AppServiceType::eType>::EnumToString(
          mobile_apis::AppServiceType::NAVIGATION, &navi_service_type);
  auto service = ActiveServiceForType(navi_service_type);
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

AppService* AppServiceManager::FindWayPointsHandler() {
  auto service = ActiveServiceForType(
      EnumToString(mobile_apis::AppServiceType::NAVIGATION));
  if (!service || !service->mobile_service ||
      !service->record[strings::service_manifest].keyExists(
          strings::handled_rpcs)) {
    return nullptr;
  }

  smart_objects::SmartObject& handled_rpcs =
      service->record[strings::service_manifest][strings::handled_rpcs];
  for (size_t i = 0; i < handled_rpcs.length(); ++i) {
    if (handled_rpcs[i].asInt() == mobile_apis::FunctionID::GetWayPointsID) {
      return service;
    }
  }
  return nullptr;
}

void AppServiceManager::AppServiceUpdated(
    const smart_objects::SmartObject& service_record,
    const mobile_apis::ServiceUpdateReason::eType update_reason,
    smart_objects::SmartObject& msg_params) {
  SDL_LOG_AUTO_TRACE();
  smart_objects::SmartObject& services =
      msg_params[strings::system_capability][strings::app_services_capabilities]
                [strings::app_services];
  smart_objects::SmartObject service(smart_objects::SmartType_Map);
  service[strings::update_reason] = update_reason;
  service[strings::updated_app_service_record] = service_record;
  services[-1] = service;
}

std::vector<AppService> AppServiceManager::GetActiveServices() {
  std::vector<AppService> active_services;
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    if (it->second.record[strings::service_active].asBool()) {
      active_services.push_back(it->second);
    }
  }
  return active_services;
}

RPCPassingHandler& AppServiceManager::GetRPCPassingHandler() {
  return rpc_passing_handler_;
}

}  //  namespace application_manager
