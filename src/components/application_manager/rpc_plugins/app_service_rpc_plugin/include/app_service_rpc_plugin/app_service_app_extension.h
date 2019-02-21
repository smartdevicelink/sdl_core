/*
 Copyright (c) 2018, Livio
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_APP_SERVICE_RPC_PLUGIN_INCLUDE_APP_SERVICE_RPC_PLUGIN_APP_SERVICE_APP_EXTENSION_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_APP_SERVICE_RPC_PLUGIN_INCLUDE_APP_SERVICE_RPC_PLUGIN_APP_SERVICE_APP_EXTENSION_H

#include <application_manager/application_manager.h>
#include "application_manager/app_extension.h"

#include <set>
#include <string>

namespace app_service_rpc_plugin {
class AppServiceRpcPlugin;

namespace app_mngr = application_manager;

typedef int AppExtensionUID;
typedef mobile_apis::VehicleDataType::eType VehicleDataType;
/**
 * @brief Defines set of app service types
 */
typedef std::set<std::string> AppServiceSubscriptions;

class AppServiceAppExtension : public app_mngr::AppExtension {
 public:
  /**
   * @brief AppServiceAppExtension constructor
   * @param plugin app service plugin
   * @param app application that contains this plugin
   */
  AppServiceAppExtension(AppServiceRpcPlugin& plugin,
                         app_mngr::Application& app);
  virtual ~AppServiceAppExtension();

  /**
   * @brief subscribeToAppService add vehicle_data to list of subscriptions of
   * application extension
   * @param vehicle_data data to subscribe
   * @return true in case if subscription is successful
   */
  bool SubscribeToAppService(const std::string app_service_type);

  /**
   * @brief unsubscribeFromAppService remove vehicle_data from list of
   * subscriptions of application extension
   * @param vehicle_data data to unsubscribe
   * @return true in case if unsubscription is successful
   */
  bool UnsubscribeFromAppService(const std::string app_service_type);
  /**
   * @brief unsubscribeFromAppService unsubscribe from all app service types
   */
  void UnsubscribeFromAppService();

  /**
   * @brief isSubscribedToAppService checks if extension is subscribed to
   * app service type
   * @param app_service_type data type  to check subscription
   * @return true if extension is subscribed this app_service_type, otherwise
   * return false
   */
  bool IsSubscribedToAppService(const std::string app_service_type) const;

  /**
   * @brief Subscriptions get list of subscriptions for application extension
   * @return list of subscriptions for application extension
   */
  AppServiceSubscriptions Subscriptions();

  /**
   * @brief SaveResumptionData saves vehicle info data
   * @param resumption_data plase to store resumption data
   */
  void SaveResumptionData(ns_smart_device_link::ns_smart_objects::SmartObject&
                              resumption_data) OVERRIDE;

  /**
   * @brief ProcessResumption load resumtion data back to plugin during
   * resumption
   * @param resumption_data resumption data
   */
  void ProcessResumption(
      const smart_objects::SmartObject& resumption_data) OVERRIDE;

  /**
   * @brief ExtractVIExtension utility function to extract application extension
   * from application
   * @param app : applicaiton that contains vehicle info app_extension
   * @return application extension extracted from application
   */
  static AppServiceAppExtension& ExtractASExtension(
      application_manager::Application& app);

 private:
  AppServiceSubscriptions subscribed_data_;
  AppServiceRpcPlugin& plugin_;
  app_mngr::Application& app_;
};
}

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_APP_SERVICE_PLUGIN_INCLUDE_APP_SERVICE_PLUGIN_APP_SERVICE_APP_EXTENSION_H
