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
#include "application_manager/rpc_passing_handler.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/smart_object.h"

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

class RPCPassingHandler;
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
  virtual ~AppServiceManager();

  /**
   * @brief Publish an app service. This service will be activated if it is the
   * default service for its type, or if it is the first service to register of
   * its type.
   * @param manifest - The app service manifest of the service to be published
   * @param mobile_service - True, if the service is being published by a mobile
   * app. False, if published by the embedded system.
   * @param connection_key - If mobile_service is true, the connection key of
   * the app publishing this service.
   * @return The app service record of the published app service
   */
  virtual smart_objects::SmartObject PublishAppService(
      const smart_objects::SmartObject& manifest,
      const bool mobile_service,
      const uint32_t connection_key = 0u);

  /**
   * @brief Unpublish an existing app service
   * @param service_id - The ID of the app service to be deleted.
   * @return True on success, false otherwise
   */
  virtual bool UnpublishAppService(const std::string service_id);

  /**
   * @brief Callback for when an application is activated. Will activate all
   * services which were published by this app.
   * @param app - The application that was activated.
   */
  virtual void OnAppActivated(ApplicationConstSharedPtr app);

  /**
   * @brief Unpublish any published services for a given mobile application
   * @param connection_key - The connection key of the application from which
   * all published services should be deleted.
   */
  virtual void UnpublishServices(const uint32_t connection_key);

  /**
   * @brief Sets a service to the default service for its type
   * @param service_id - The ID of the service to become the default
   * @return True on success, false otherwise
   */
  virtual bool SetDefaultService(const std::string service_id);

  /**
   * @brief Removes a service from the list of default services.
   * @param service_id - The ID of the existing default service.
   * @return True on success, false otherwise
   */
  virtual bool RemoveDefaultService(const std::string service_id);

  /**
   * @brief Activate an existing service
   * @param service_id - The ID of the service to be activated.
   * @return True if the service is active, false otherwise
   */
  virtual bool ActivateAppService(const std::string service_id);

  /**
   * @brief Deactivate an existing active service
   * @param service_id - The ID of the service to be deactivated.
   * @return True if the service was deactivated, false otherwise
   */
  virtual bool DeactivateAppService(const std::string service_id);

  /**
   * @brief Get a list of all published service records.
   * @return A list of all all published service records.
   */
  virtual std::vector<smart_objects::SmartObject> GetAllServiceRecords();
  
  /**
   * @brief Get a list of all active app services.
   * @return A list of all all active app services.
   */
  virtual std::vector<AppService> GetActiveServices();

  /**
   * @brief Retrieve the active service provider for a given service type
   * @param service_type - The service type of the provider to be retrieved
   * @param mobile_consumer - True, if the consumer trying to communicate with
   * the provider is a mobile application
   * @param app - Output parameter that will be filled with the application
   * providing the requested service on success, unchanged on failure
   * @param hmi_service - Output parameter set to true if the service
   * provider is the HMI, unchanged on failure
   */
  virtual void GetProviderByType(const std::string& service_type,
                                 const bool mobile_consumer,
                                 ApplicationSharedPtr& app,
                                 bool& hmi_service);

  /**
   * @brief Retrieve the service provider with a given service ID
   * @param service_type - The service type of the provider to be retrieved
   * @param mobile_consumer - True, if the consumer trying to communicate with
   * the provider is a mobile application
   * @param app - Output parameter that will be filled with the application
   * providing the requested service on success, unchanged on failure
   * @param hmi_service - Output parameter set to true if the service
   * provider is the HMI, unchanged on failure
   */
  virtual void GetProviderByID(const std::string& service_id,
                               const bool mobile_consumer,
                               ApplicationSharedPtr& app,
                               bool& hmi_service);

  /**
   * @brief Get the active service for a given service type.
   * @param service_type - The service type
   * @return A pointer to the active service for the given service type on
   * success, NULL on failure
   */
  virtual AppService* ActiveServiceForType(const std::string service_type);

  /**
   * @brief Get the embedded service for a given service type.
   * @param service_type - The service type
   * @return A pointer to the embedded service for the given service type on
   * success, NULL on failure
   */
  virtual AppService* EmbeddedServiceForType(const std::string service_type);

  /**
   * @brief Get the service with a given service ID.
   * @param service_type - The service ID
   * @return A pointer to requested service on success, NULL on failure
   */
  virtual AppService* FindServiceByID(const std::string service_id);

  /**
   * @brief Get the service ID of the default service for a given service type.
   * @param service_type - The service type
   * @return The service ID of the default service for the given service type on
   * success, "" on failure
   */
  virtual std::string DefaultServiceByType(const std::string service_type);

  /**
   * @brief Update a navigation system capability struct with the capabilities
   * of the active navigation service.
   * @param out_params - The struct to be modified
   * @return True on success, false otherwise (if there was no active NAVIGATION
   * service)
   */
  virtual bool UpdateNavigationCapabilities(
      smart_objects::SmartObject& out_params);

  /**
   * @brief Get the RPCPassingHandler tied to this object
   * @return The RPCPassingHandler tied to this object
   */
  virtual RPCPassingHandler& GetRPCPassingHandler();

 private:
  ApplicationManager& app_manager_;
  resumption::LastState& last_state_;

  sync_primitives::RecursiveLock published_services_lock_;
  std::map<std::string, AppService> published_services_;
  RPCPassingHandler rpc_passing_handler_;

  void SetServicePublished(const std::string service_id,
                           bool service_published);
  void AppServiceUpdated(
      const smart_objects::SmartObject& service_record,
      const mobile_apis::ServiceUpdateReason::eType update_reason,
      smart_objects::SmartObject& msg_params);
  void GetProviderFromService(const AppService& service,
                              const bool mobile_consumer,
                              ApplicationSharedPtr& app,
                              bool& hmi_service);
  AppService* FindServiceByPolicyAppID(const std::string policy_app_id,
                                       const std::string type);
  std::string GetPolicyAppID(AppService service);
};

}  //  namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_SERVICE_MANAGER_H_
