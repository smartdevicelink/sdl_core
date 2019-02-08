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

  return service_record;
}

bool AppServiceManager::UnpublishAppService(const std::string service_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  // TODO
  return false;
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

}  //  namespace application_manager
