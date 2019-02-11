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
#include "application_manager/app_service_manager.h"
#include "application_manager/application_manager.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "encryption/hashing.h"
#include "resumption/last_state.h"
#include "smart_objects/smart_object.h"
#include "utils/logger.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "AppServiceManager")

namespace {
const size_t kLimitCommand = 30;
}

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
  service_record[strings::service_active] = true;
  app_service.record = service_record;

  std::string service_type =
      manifest[strings::app_service_manifest][strings::service_type].asString();
  Json::Value dictionary = last_state_.get_dictionary();
  app_service.default_service =
      (dictionary[kAppServiceSection][kDefaults][service_type].asString() ==
       manifest[strings::service_name].asString());

  published_services_.insert(
      std::pair<std::string, AppService>(service_id, app_service));

  std::pair<std::string, AppService> active_service =
      ActiveServiceByType(service_type);
  if (active_service.first.empty() || app_service.default_service) {
    ActivateAppService(service_id);
  }

  smart_objects::SmartObjectSPtr notification =
      std::make_shared<smart_objects::SmartObject>(
          CreateMobileSystemCapabilityNotification(
              service_id, mobile_apis::ServiceUpdateReason::PUBLISHED));
  app_manager_.GetRPCService().ManageMobileCommand(
      notification, commands::Command::CommandSource::SOURCE_SDL);
  smart_objects::SmartObjectSPtr hmi_notification =
      std::make_shared<smart_objects::SmartObject>(
          CreateHMISystemCapabilityNotification(
              service_id, mobile_apis::ServiceUpdateReason::PUBLISHED));
  app_manager_.GetRPCService().ManageHMICommand(hmi_notification);
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

  setServicePublished(service_id, false);
  smart_objects::SmartObjectSPtr notification =
      std::make_shared<smart_objects::SmartObject>(
          CreateMobileSystemCapabilityNotification(
              service_id, mobile_apis::ServiceUpdateReason::REMOVED));
  app_manager_.GetRPCService().ManageMobileCommand(
      notification, commands::Command::CommandSource::SOURCE_SDL);
  smart_objects::SmartObjectSPtr hmi_notification =
      std::make_shared<smart_objects::SmartObject>(
          CreateHMISystemCapabilityNotification(
              service_id, mobile_apis::ServiceUpdateReason::REMOVED));
  app_manager_.GetRPCService().ManageHMICommand(hmi_notification);

  published_services_.erase(it);

  return true;
}

void AppServiceManager::UnpublishAppServices(const uint32_t connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Unpublishing all app services: " << connection_key);
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    if (it->second.connection_key == connection_key) {
      UnpublishAppService(it->first);
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
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Unable to find published service " << service_id);
    return false;
  }
  AppService service = it->second;

  std::string service_type =
      service.record[strings::app_service_manifest][strings::service_type]
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
      service.record[strings::app_service_manifest][strings::service_name]
          .asString();
  return true;
}

bool AppServiceManager::RemoveDefaultService(const std::string service_id) {
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Unable to find published service " << service_id);
    return false;
  }

  AppService service = it->second;
  if (!service.default_service) {
    LOG4CXX_ERROR(logger_, "Service was not default " << service_id);
    return false;
  }
  service.default_service = false;

  std::string service_type =
      service.record[strings::app_service_manifest][strings::service_type]
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
  smart_objects::SmartObject service = it->second.record;

  std::string service_type =
      service[strings::app_service_manifest][strings::service_type].asString();
  auto active_service = ActiveServiceByType(service_type);
  if (!active_service.first.empty()) {
    active_service.second.record[strings::service_active] = false;
  }
  service[strings::service_active] = true;

  return true;
}

bool AppServiceManager::DeactivateAppService(const std::string service_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Unable to find published service " << service_id);
    return false;
  }
  smart_objects::SmartObject service = it->second.record;
  service[strings::service_active] = false;
  return true;
}

std::pair<std::string, AppService> AppServiceManager::ActiveServiceByType(
    std::string service_type) {
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    if (it->second.record[strings::app_service_manifest][strings::service_type]
                .asString() == service_type &&
        it->second.record[strings::service_active].asBool()) {
      return *it;
    }
  }
  AppService empty;
  return std::make_pair(std::string(), empty);
}

std::pair<std::string, AppService> AppServiceManager::FindServiceByName(
    std::string name) {
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    if (it->second.record[strings::app_service_manifest][strings::service_name]
            .asString() == name) {
      return *it;
    }
  }
  AppService empty;
  return std::make_pair(std::string(), empty);
}

std::string AppServiceManager::DefaultServiceByType(std::string service_type) {
  Json::Value dictionary = last_state_.get_dictionary();
  if (dictionary[kAppServiceSection][kDefaults].isMember(service_type)) {
    return dictionary[kAppServiceSection][kDefaults][service_type].asString();
  }
  return std::string();
}

void AppServiceManager::setServicePublished(const std::string service_id,
                                            bool service_published) {
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Service id does not exist in published services");
    return;
  }
  it->second.record[strings::service_published] = service_published;
}

void AppServiceManager::setServiceActive(const std::string service_id,
                                         bool service_activated) {
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Service id does not exist in published services");
    return;
  }
  it->second.record[strings::service_active] = service_activated;
}

smart_objects::SmartObject
AppServiceManager::CreateMobileSystemCapabilityNotification(
    const std::string service_id,
    mobile_apis::ServiceUpdateReason::eType update_reason) {
  smart_objects::SmartObject message(smart_objects::SmartType_Map);

  message[strings::params][strings::message_type] = MessageType::kNotification;
  message[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnSystemCapabilityUpdatedID;

  smart_objects::SmartObject system_capability =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  system_capability[strings::system_capability_type] =
      mobile_apis::SystemCapabilityType::APP_SERVICES;

  smart_objects::SmartObject app_service_capabilities(
      smart_objects::SmartType_Map);
  smart_objects::SmartObject supported_types(smart_objects::SmartType_Array);
  smart_objects::SmartObject app_services(smart_objects::SmartType_Array);

  std::vector<smart_objects::SmartObject> service_records = GetAllServices();
  std::set<mobile_apis::AppServiceType::eType> service_types;

  for (auto& record : service_records) {
    // SUPPORTED TYPES
    mobile_apis::AppServiceType::eType service_type =
        static_cast<mobile_apis::AppServiceType::eType>(
            record[strings::service_manifest][strings::service_type].asUInt());
    service_types.insert(service_type);

    // APP SERVICES
    smart_objects::SmartObject app_services_capabilities(
        smart_objects::SmartType_Map);
    app_services_capabilities[strings::updated_app_service_record] = record;
    if (record[strings::service_id].asString() == service_id) {
      app_services_capabilities[strings::update_reason] = update_reason;
    }
    app_services.asArray()->push_back(app_services_capabilities);
  }

  int i = 0;
  for (auto type_ : service_types) {
    supported_types[i] = type_;
    i++;
  }

  app_service_capabilities[strings::services_supported] = supported_types;
  app_service_capabilities[strings::app_services] = app_services;
  system_capability[strings::app_services_capabilities] =
      app_service_capabilities;

  message[strings::msg_params][strings::system_capability] = system_capability;
  return message;
}

smart_objects::SmartObject
AppServiceManager::CreateHMISystemCapabilityNotification(
    const std::string service_id,
    mobile_apis::ServiceUpdateReason::eType update_reason) {
  auto message =
      CreateMobileSystemCapabilityNotification(service_id, update_reason);
  message[strings::params][strings::function_id] =
      hmi_apis::FunctionID::BasicCommunication_OnSystemCapabilityUpdated;
  return message;
}

}  //  namespace application_manager
