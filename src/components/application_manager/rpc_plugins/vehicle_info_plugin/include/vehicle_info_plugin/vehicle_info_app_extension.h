/*
 Copyright (c) 2018, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_APP_EXTENSION_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_APP_EXTENSION_H

#include <application_manager/application_manager.h>

#include <set>
#include <string>

namespace vehicle_info_plugin {
class VehicleInfoPlugin;

namespace app_mngr = application_manager;

typedef mobile_apis::VehicleDataType::eType VehicleDataType;
/**
 * @brief Defines set of vehicle info types
 */
typedef std::set<std::string> VehicleInfoSubscriptions;

class VehicleInfoAppExtension : public app_mngr::AppExtension {
 public:
  /**
   * @brief VehicleInfoAppExtension constructor
   * @param plugin vehicle info plugin
   * @param app application that contains this plugin
   */
  VehicleInfoAppExtension(VehicleInfoPlugin& plugin,
                          app_mngr::Application& app);
  VehicleInfoAppExtension(const VehicleInfoAppExtension&) = delete;
  VehicleInfoAppExtension& operator=(const VehicleInfoAppExtension&) = delete;
  virtual ~VehicleInfoAppExtension();

  /**
   * @brief subscribeToVehicleInfo add vehicle_data to list of subscriptions of
   * application extension
   * @param vehicle_data data to subscribe
   * @return true in case if subscription is successful
   */
  bool subscribeToVehicleInfo(const std::string& vehicle_data);

  /**
   * @brief unsubscribeFromVehicleInfo remove vehicle_data from list of
   * subscriptions of application extension
   * @param vehicle_data data to unsubscribe
   * @return true in case if unsubscription is successful
   */
  bool unsubscribeFromVehicleInfo(const std::string& vehicle_data);
  /**
   * @brief unsubscribeFromVehicleInfo unsubscribe from all vehicle info data
   */
  void unsubscribeFromVehicleInfo();

  /**
   * @brief isSubscribedToVehicleInfo checks if extension is subscribed to
   * vehicle data
   * @param vehicle_data_type data type  to check subscription
   * @return true if extension is subscribed this vehicle_data_type, otherwise
   * return false
   */
  bool isSubscribedToVehicleInfo(const std::string& vehicle_data_type) const;

  /**
   * @brief isPendingSubscriptionToVehicleInfo checks if there any extension
   * with pending subscription to vehicle data
   * @param vehicle_data vehicle data to check
   * @return true if extension is subscribed to this vehicle_data, otherwise
   * returns false
   */
  bool isPendingSubscriptionToVehicleInfo(
      const std::string& vehicle_data) const;

  /**
   * @brief Subscriptions get list of subscriptions for application extension
   * @return list of subscriptions for application extension
   */
  const DataAccessor<VehicleInfoSubscriptions> Subscriptions();

  /**
   * @brief AddPendingSubscription add pending subscription
   * @param vehicle_data subscription to add
   * @return
   */
  bool AddPendingSubscription(const std::string& vehicle_data);

  /**
   * @brief RemovePendingSubscription remove some paticular pending subscription
   * @param vehicle_data subscription to remove
   * @return
   */
  bool RemovePendingSubscription(const std::string& vehicle_data);

  /**
   * @brief RemovePendingSubscriptions removed all pending subscriptions
   * @return
   */
  void RemovePendingSubscriptions();

  /**
   * @brief PendingSubscriptions list of preliminary subscriptoins
   * That will be moved to subscriptions as soon as HMI will respond with
   * success.
   * Used for resumption to keep list of preliminary subcriptions and wait for
   * HMI response
   * @return
   */
  const DataAccessor<VehicleInfoSubscriptions> PendingSubscriptions();

  /**
   * @brief SaveResumptionData saves vehicle info data
   * @param resumption_data plase to store resumption data
   */
  void SaveResumptionData(smart_objects::SmartObject& resumption_data) OVERRIDE;

  void ProcessResumption(const smart_objects::SmartObject& saved_app) OVERRIDE;

  void RevertResumption(
      const smart_objects::SmartObject& resumption_data) OVERRIDE;

  /**
   * @brief VehicleInfoAppExtensionUID unique identifier of VehicleInfo
   * aplication extension
   */
  static const app_mngr::AppExtensionUID VehicleInfoAppExtensionUID = 146;

  /**
   * @brief ExtractVIExtension utility function to extract application extension
   * from application
   * @param app : applicaiton that contains vehicle info app_extension
   * @return application extension extracted from application
   */
  static VehicleInfoAppExtension& ExtractVIExtension(
      application_manager::Application& app);

 private:
  mutable std::shared_ptr<sync_primitives::Lock> subscribed_data_lock_;
  VehicleInfoSubscriptions subscribed_data_;

  mutable std::shared_ptr<sync_primitives::Lock> pending_subscriptions_lock_;
  VehicleInfoSubscriptions pending_subscriptions_;
  VehicleInfoPlugin& plugin_;
  app_mngr::Application& app_;
};
}  // namespace vehicle_info_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_APP_EXTENSION_H
