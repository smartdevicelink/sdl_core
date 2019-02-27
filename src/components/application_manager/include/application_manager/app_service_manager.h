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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_SERVICE_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_SERVICE_MANAGER_H_

#include "application_manager/application.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/smart_object.h"
#include "application_manager/application.h"

namespace resumption {
class LastState;
}

namespace application_manager {

struct AppService {
  uint32_t connection_key;
  bool mobile_service;
  bool default_service = false;
  smart_objects::SmartObject record;
};

class ApplicationManager;

/**
 * @brief The AppServiceManager is TODO.
 */
class AppServiceManager {
 public:
  const std::string kEmbeddedService = "EMBEDDED_SERVICE";

  /**
   * @brief Class constructor
   * @param app_manager
   */
  AppServiceManager(ApplicationManager& app_manager,
                    resumption::LastState& last_state);

  /**
   * @brief Class destructor
   */
  ~AppServiceManager();

  /**
   * @brief TODO
   * @param manifest
   */
  smart_objects::SmartObject PublishAppService(
      const smart_objects::SmartObject& manifest,
      const bool mobile_service,
      const uint32_t connection_key);

  /**
   * @brief TODO
   * @param service_id
   */
  bool UnpublishAppService(const std::string service_id);

  void OnAppActivated(ApplicationConstSharedPtr app);

  /**
   * @brief TODO
   * @param connection_key
   */
  void UnpublishServices(const uint32_t connection_key);

  /**
   * @brief TODO
   * @param service_id
   * @param service_published
   */
  void SetServicePublished(const std::string service_id,
                           bool service_published);

  /**
   * @brief TODO
   * @param service_id
   */
  bool SetDefaultService(const std::string service_id);

  /**
   * @brief TODO
   * @param service_id
   */
  bool RemoveDefaultService(const std::string service_id);

  /**
   * @brief TODO
   * @param service_id
   */
  bool ActivateAppService(const std::string service_id);

  /**
   * @brief TODO
   * @param service_id
   */
  bool DeactivateAppService(const std::string service_id);

  /**
   * @brief TODO
   * @param manifest
   */
  std::vector<smart_objects::SmartObject> GetAllServices();

  void GetProviderByType(const std::string& service_type,
                         ApplicationSharedPtr& app,
                         bool& hmi_service);

  void GetProviderByID(const std::string& service_id,
                       ApplicationSharedPtr& app,
                       bool& hmi_service);

  std::pair<std::string, AppService> ActiveServiceByType(
      std::string service_type);

  std::pair<std::string, AppService> EmbeddedServiceForType(
      std::string service_type);

  std::pair<std::string, AppService> FindServiceByName(std::string name);

  std::pair<std::string, AppService> FindServiceByID(std::string service_id);

  std::string DefaultServiceByType(std::string service_type);

 private:
  ApplicationManager& app_manager_;
  resumption::LastState& last_state_;
  std::map<std::string, AppService> published_services_;

  void BroadcastAppServiceUpdate(smart_objects::SmartObject& msg_params);
  void AppServiceUpdated(
      const smart_objects::SmartObject& service_record,
      const mobile_apis::ServiceUpdateReason::eType update_reason,
      smart_objects::SmartObject& msg_params);
  void GetProviderFromService(const AppService& service,
                              ApplicationSharedPtr& app,
                              bool& hmi_service);
  std::pair<std::string, AppService> FindServiceByAppID(std::string name,
                                                        std::string type);
  std::string GetServiceAppID(AppService service);
};

}  //  namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_SERVICE_MANAGER_H_
