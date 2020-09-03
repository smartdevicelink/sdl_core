/*
 Copyright (c) 2020, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_PENDING_RESUMPTION_HANDLER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_PENDING_RESUMPTION_HANDLER_H_
#include <queue>
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/resumption/pending_resumption_handler.h"
#include "application_manager/resumption/resumption_data_processor.h"

#include "utils/optional.h"
#include "vehicle_info_app_extension.h"
namespace vehicle_info_plugin {
class CustomVehicleDataManager;

namespace app_mngr = application_manager;

class VehicleInfoPendingResumptionHandler
    : public resumption::PendingResumptionHandler {
 public:
  typedef std::set<std::string> VehicleDataList;
  VehicleInfoPendingResumptionHandler(
      app_mngr::ApplicationManager& application_manager,
      CustomVehicleDataManager& custom_vehicle_data_manager);

  void on_event(const app_mngr::event_engine::Event& event) OVERRIDE;

  void HandleResumptionSubscriptionRequest(app_mngr::AppExtension& extension,
                                           app_mngr::Application& app) OVERRIDE;
  void OnResumptionRevert() OVERRIDE;

  smart_objects::SmartObjectSPtr CreateSubscribeRequestToHMI(
      const VehicleDataList& subscriptions);

  ns_smart_device_link::ns_smart_objects::SmartObject CreateFakeResponseFromHMI(
      const std::map<std::string, smart_objects::SmartObject>& subscriptions,
      const uint32_t fake_corrlation_id);

 private:
  void ProcessNextPendingResumption(
      const smart_objects::SmartObject& response_message);
  void TriggerPendingResumption();

  struct PendingSubscriptionsResumption {
    PendingSubscriptionsResumption(
        const uint32_t app_id,
        const uint32_t corr_id,
        const VehicleDataList& requested_vehicle_data)
        : app_id_(app_id)
        , fake_corr_id_(corr_id)
        , waiting_for_hmi_response_(false)
        , requested_vehicle_data_(requested_vehicle_data) {}

    PendingSubscriptionsResumption(const PendingSubscriptionsResumption& copy) =
        default;

    /**
     * @brief IsSuccessfullyDone check if SDL restored all required data or not
     * @return
     */
    bool IsSuccessfullyDone() const;
    bool DataWasRequested(const std::string& vd) const;
    VehicleDataList NotSubscribedData() const;

    /**
     * @brief FillSubscriptionResults fill subscription results based on
     * response message
     * @param response
     */
    void FillSubscriptionResults(const smart_objects::SmartObject& response);

    /**
     * @brief FillSubscriptionResults fill subscription results based on list of
     * requested data and list of restored data
     *
     */
    void FillSubscriptionResults();

    void FillRestoredData(const VehicleDataList& successful_subscriptions);

    uint32_t app_id_;
    uint32_t fake_corr_id_;
    bool waiting_for_hmi_response_;
    VehicleDataList requested_vehicle_data_;
    VehicleDataList restored_vehicle_data_;
    std::map<std::string, smart_objects::SmartObject> subscription_results_;
  };

  void SendHMIRequestForNotSubscribed(
      const PendingSubscriptionsResumption& pending_resumption);
  void RaiseFinishedPendingResumption(
      const PendingSubscriptionsResumption& pending_resumption);
  /**
   * @brief SubscribeToFakeRequest will create fake subscription for subscriber
   * ( resumption_data_processor)
   * Fake request is required only for subscriber subscription.
   * This request will not be sen't to HMI so it named as fake request.
   * Fake request contains all data that need to be resumed for the application
   * When HMI will respond for any VehicleData request,
   * PendingSubscriptionsResumption will go through all pending resumptions and
   * fill them with received subscriptions. If certain pending resumption will
   * take all requested subscriptions PendingSubscriptionsResumption will take
   * this fake request correaltion id and create fake response based on it.
   * Within fake response it will notify subscriber about resumption status.
   *
   * @param app_id applicaiton to pass into subscriber
   * @param subscriptions list of requested subscriptions
   * @param subscriber function to subscribe caller for finished resumption
   * @return pending_resumption with information about fake correlation id for
   * subscriber, list of vehicle data to subscribe
   */
  PendingSubscriptionsResumption SubscribeToFakeRequest(
      const uint32_t app, const VehicleDataList& subscriptions);

  std::deque<PendingSubscriptionsResumption> pending_requests_;

  CustomVehicleDataManager& custom_vehicle_data_manager_;
  sync_primitives::RecursiveLock pending_resumption_lock_;
};
}  // namespace vehicle_info_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_VEHICLE_INFO_PENDING_RESUMPTION_HANDLER_H_
