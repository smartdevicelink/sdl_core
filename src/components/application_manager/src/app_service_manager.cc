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

#include "application_manager/app_service_manager.h"
#include "application_manager/application.h"
#include "application_manager/app_service_manager.h"
#include "application_manager/application_manager.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "encryption/hashing.h"
#include "resumption/last_state.h"
#include "utils/logger.h"

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

  std::string service_type =
      manifest[strings::app_service_manifest][strings::service_type].asString();
  Json::Value dictionary = last_state_.get_dictionary();
  app_service.default_service =
      (dictionary[kAppServiceSection][kDefaults][service_type].asString() ==
       manifest[strings::service_name].asString());

  published_services_.insert(
      std::pair<std::string, AppService>(service_id, app_service));

  smart_objects::SmartObject msg_params;
  AppServiceUpdated(
      service_record, mobile_apis::ServiceUpdateReason::PUBLISHED, msg_params);
  BroadcastAppServiceUpdate(msg_params);

  // Activate the new service if it is the default for its service type, or if
  // no service is active of its service type
  std::pair<std::string, AppService> active_service =
      ActiveServiceByType(service_type);
  if (active_service.first.empty() || app_service.default_service) {
    ActivateAppService(service_id);
  }

  return service_record;
}

bool AppServiceManager::UnpublishAppService(const std::string service_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Service id does not exist in published services");
    return false;
  }
  LOG4CXX_DEBUG(logger_, "Unpublishing app service: " << service_id);

  SetServicePublished(service_id, false);
  smart_objects::SmartObject msg_params;

  auto record = it->second.record;
  if (record[strings::service_active].asBool()) {
    record[strings::service_active] = false;

    // Activate embedded service, if available
    auto embedded_service = EmbeddedServiceForType(
        record[strings::service_manifest][strings::service_type].asString());
    if (!embedded_service.first.empty()) {
      embedded_service.second.record[strings::service_active] = true;
      AppServiceUpdated(embedded_service.second.record,
                        mobile_apis::ServiceUpdateReason::ACTIVATED,
                        msg_params);
    }
  }
  AppServiceUpdated(
      it->second.record, mobile_apis::ServiceUpdateReason::REMOVED, msg_params);
  BroadcastAppServiceUpdate(msg_params);
  published_services_.erase(it);

  return true;
}

void AppServiceManager::UnpublishServices(const uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Unpublishing all app services: " << connection_key);
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    if (it->second.connection_key == connection_key) {
      UnpublishAppService(it->first);
    }
  }
}

void AppServiceManager::OnAppActivated(ApplicationConstSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
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
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    services.push_back(it->second.record);
  }
  return services;
}

bool AppServiceManager::SetDefaultService(const std::string service_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Unable to find published service " << service_id);
    return false;
  }
  AppService& service = it->second;

  std::string service_type =
      service.record[strings::service_manifest][strings::service_type]
          .asString();
  std::string default_service_name = DefaultServiceByType(service_type);
  if (!default_service_name.empty()) {
    auto default_service = FindServiceByName(default_service_name);
    if (!default_service.first.empty()) {
      default_service.second.default_service = false;
    }
  }
  service.default_service = true;

  Json::Value dictionary = last_state_.get_dictionary();
  dictionary[kAppServiceSection][kDefaults][service_type] =
      service.record[strings::service_manifest][strings::service_name]
          .asString();
  return true;
}

bool AppServiceManager::RemoveDefaultService(const std::string service_id) {
  LOG4CXX_AUTO_TRACE(logger_);
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
  Json::Value dictionary = last_state_.get_dictionary();
  dictionary[kAppServiceSection][kDefaults].removeMember(service_type);
  return true;
}

bool AppServiceManager::ActivateAppService(const std::string service_id) {
  LOG4CXX_AUTO_TRACE(logger_);
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

  const std::string service_type =
      service[strings::service_manifest][strings::service_type].asString();
  auto active_service = ActiveServiceByType(service_type);
  if (!active_service.first.empty()) {
    active_service.second.record[strings::service_active] = false;
    AppServiceUpdated(active_service.second.record,
                      mobile_apis::ServiceUpdateReason::DEACTIVATED,
                      msg_params);
  }
  service[strings::service_active] = true;
  AppServiceUpdated(
      service, mobile_apis::ServiceUpdateReason::ACTIVATED, msg_params);

  BroadcastAppServiceUpdate(msg_params);
  return true;
}

bool AppServiceManager::DeactivateAppService(const std::string service_id) {
  LOG4CXX_AUTO_TRACE(logger_);
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
    if (!embedded_service.first.empty()) {
      embedded_service.second.record[strings::service_active] = true;
      AppServiceUpdated(embedded_service.second.record,
                        mobile_apis::ServiceUpdateReason::ACTIVATED,
                        msg_params);
    }
    send_update = true;
  }

  if (send_update) {
    BroadcastAppServiceUpdate(msg_params);
  }
  return true;
}

std::pair<std::string, AppService> AppServiceManager::ActiveServiceByType(
    std::string service_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    if (it->second.record[strings::service_manifest][strings::service_type]
                .asString() == service_type &&
        it->second.record[strings::service_published].asBool() &&
        it->second.record[strings::service_active].asBool()) {
      return *it;
    }
  }
  AppService empty;
  return std::make_pair(std::string(), empty);
}

std::pair<std::string, AppService> AppServiceManager::EmbeddedServiceForType(
    std::string service_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    if (it->second.record[strings::service_manifest][strings::service_type]
                .asString() == service_type &&
        !it->second.mobile_service) {
      return *it;
    }
  }
  AppService empty;
  return std::make_pair(std::string(), empty);
}

std::pair<std::string, AppService> AppServiceManager::FindServiceByName(
    std::string name) {
  LOG4CXX_AUTO_TRACE(logger_);
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    if (it->second.record[strings::service_manifest][strings::service_name]
            .asString() == name) {
      return *it;
    }
  }
  AppService empty;
  return std::make_pair(std::string(), empty);
}

std::string AppServiceManager::DefaultServiceByType(std::string service_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  Json::Value dictionary = last_state_.get_dictionary();
  if (dictionary[kAppServiceSection][kDefaults].isMember(service_type)) {
    return dictionary[kAppServiceSection][kDefaults][service_type].asString();
  }
  return std::string();
}

void AppServiceManager::SetServicePublished(const std::string service_id,
                                            bool service_published) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Service id does not exist in published services");
    return;
  }
  it->second.record[strings::service_published] = service_published;
}

void AppServiceManager::BroadcastAppServiceUpdate(
    smart_objects::SmartObject& msg_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject message(smart_objects::SmartType_Map);

  msg_params[strings::system_capability][strings::system_capability_type] =
      mobile_apis::SystemCapabilityType::APP_SERVICES;
  message[strings::params][strings::message_type] = MessageType::kNotification;
  message[strings::msg_params] = msg_params;

  // Construct and send mobile notification
  message[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnSystemCapabilityUpdatedID;
  smart_objects::SmartObjectSPtr notification =
      std::make_shared<smart_objects::SmartObject>(message);
  app_manager_.GetRPCService().ManageMobileCommand(
      notification, commands::Command::CommandSource::SOURCE_SDL);

  // Construct and send HMI notification
  message[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_OnSystemCapabilityUpdated;
  smart_objects::SmartObjectSPtr hmi_notification =
      std::make_shared<smart_objects::SmartObject>(message);
  app_manager_.GetRPCService().ManageHMICommand(hmi_notification);
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
