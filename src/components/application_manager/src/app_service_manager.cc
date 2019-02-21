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
#include "utils/logger.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "AppServiceManager")

namespace application_manager {

AppServiceManager::AppServiceManager(ApplicationManager& app_manager)
    : app_manager_(app_manager) {}

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
  service_record[strings::service_active] = true;
  app_service.record = service_record;

  published_services_.insert(
      std::pair<std::string, AppService>(service_id, app_service));

  auto all_services = GetAllServices();
  smart_objects::SmartObjectSPtr notification =
      std::make_shared<smart_objects::SmartObject>(
          MessageHelper::CreateMobileSystemCapabilityNotification(
              all_services,
              service_id,
              mobile_apis::ServiceUpdateReason::PUBLISHED));
  app_manager_.GetRPCService().ManageMobileCommand(
      notification, commands::Command::CommandSource::SOURCE_SDL);
  smart_objects::SmartObjectSPtr hmi_notification =
      std::make_shared<smart_objects::SmartObject>(
          MessageHelper::CreateHMISystemCapabilityNotification(
              all_services,
              service_id,
              mobile_apis::ServiceUpdateReason::PUBLISHED));
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

  SetServicePublished(service_id, false);

  auto all_services = GetAllServices();
  smart_objects::SmartObjectSPtr notification =
      std::make_shared<smart_objects::SmartObject>(
          MessageHelper::CreateMobileSystemCapabilityNotification(
              all_services,
              service_id,
              mobile_apis::ServiceUpdateReason::REMOVED));
  app_manager_.GetRPCService().ManageMobileCommand(
      notification, commands::Command::CommandSource::SOURCE_SDL);
  smart_objects::SmartObjectSPtr hmi_notification =
      std::make_shared<smart_objects::SmartObject>(
          MessageHelper::CreateHMISystemCapabilityNotification(
              all_services,
              service_id,
              mobile_apis::ServiceUpdateReason::REMOVED));
  app_manager_.GetRPCService().ManageHMICommand(hmi_notification);

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

std::vector<smart_objects::SmartObject> AppServiceManager::GetAllServices() {
  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<smart_objects::SmartObject> services;
  for (auto it = published_services_.begin(); it != published_services_.end();
       ++it) {
    services.push_back(it->second.record);
  }
  return services;
}

void AppServiceManager::SetServicePublished(const std::string service_id,
                                            bool service_published) {
  auto it = published_services_.find(service_id);
  if (it == published_services_.end()) {
    LOG4CXX_ERROR(logger_, "Service id does not exist in published services");
    return;
  }
  it->second.record[strings::service_published] = service_published;
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
}  //  namespace application_manager
