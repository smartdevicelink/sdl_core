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
#include "smart_objects/smart_object.h"
#include "utils/logger.h"
#include "encryption/hashing.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "AppServiceManager")

namespace {
const size_t kLimitCommand = 30;
}

namespace application_manager {

AppServiceManager::AppServiceManager(ApplicationManager& app_manager)
    : app_manager_(app_manager) {}

AppServiceManager::~AppServiceManager() {
  LOG4CXX_AUTO_TRACE(logger_);
}

smart_objects::SmartObject AppServiceManager::PublishAppService(
    const smart_objects::SmartObject& manifest) {
  std::string str_to_hash = "";
  std::string service_id = "";

  do {
    str_to_hash = manifest[strings::service_type].asString() +
                  std::to_string(std::rand());
    service_id = encryption::MakeHash(str_to_hash);
  } while (published_services_.find(service_id) != published_services_.end());

  smart_objects::SmartObject service_record;
  service_record[strings::service_manifest] = manifest;
  service_record[strings::service_id] = service_id;
  service_record[strings::service_published] = true;
  service_record[strings::service_active] = true;
  published_services_.insert(std::pair<std::string, smart_objects::SmartObject>(
      service_id, service_record));

  smart_objects::SmartObjectSPtr notification =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::message_type] = MessageType::kNotification;
  message[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnSystemCapabilityUpdatedID;

  smart_objects::SmartObject system_capability =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  system_capability[strings::system_capability_type] =
      mobile_apis::SystemCapabilityType::APP_SERVICES;
  system_capability[strings::app_services_capabilities] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  smart_objects::SmartObject app_services =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  app_services[0] = smart_objects::SmartObject(smart_objects::SmartType_Map);
  app_services[0][strings::update_reason] =
      mobile_apis::ServiceUpdateReason::PUBLISHED;
  app_services[0][strings::updated_app_service_record] = service_record;

  system_capability[strings::app_services_capabilities][strings::app_services] =
      app_services;

  message[strings::msg_params][strings::system_capability] = system_capability;
  app_manager_.GetRPCService().ManageHMICommand(notification);
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
    services.push_back(it->second);
  }
  return services;
}

}  //  namespace application_manager
