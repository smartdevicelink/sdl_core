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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_PENDING_RESUMPTION_HANDLER_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_PENDING_RESUMPTION_HANDLER_H
#include <queue>
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/resumption/extension_pending_resumption_handler.h"
#include "application_manager/resumption/resumption_data_processor.h"

#include "utils/optional.h"
#include "vehicle_info_app_extension.h"
namespace vehicle_info_plugin {

namespace app_mngr = application_manager;

class VehicleInfoPendingResumptionHandler
    : public resumption::ExtensionPendingResumptionHandler {
 public:
  VehicleInfoPendingResumptionHandler(
      app_mngr::ApplicationManager& application_manager);

  void on_event(const app_mngr::event_engine::Event& event) OVERRIDE;

  void HandleResumptionSubscriptionRequest(app_mngr::AppExtension& extension,
                                           resumption::Subscriber& subscriber,
                                           app_mngr::Application& app) OVERRIDE;

  std::map<std::string, bool> ExtractSubscribeResults(
      const smart_objects::SmartObject& response,
      const smart_objects::SmartObject& request) const;

  smart_objects::SmartObjectSPtr CreateSubscribeRequestToHMI(
      const std::set<std::string>& subscriptions);

  ns_smart_device_link::ns_smart_objects::SmartObject CreateFakeResponseFromHMI(
      const std::map<std::string, smart_objects::SmartObject>& subscriptions,
      const uint32_t fake_corrlation_id);
  void ClearPendingResumptionRequests() OVERRIDE;

 private:
  void ProcessNextPendingResumption(
      const smart_objects::SmartObject& response_message);
  void TriggerPendingResumption();

  struct PendingSubscriptionsResumption {
    PendingSubscriptionsResumption(
        const uint32_t app_id,
        const uint32_t corr_id,
        smart_objects::SmartObject request_message,
        const std::set<std::string>& requested_vehicle_data)
        : app_id_(app_id)
        , fake_corr_id_(corr_id)
        , requested_vehicle_data_(requested_vehicle_data) {}
    uint32_t app_id_;
    uint32_t fake_corr_id_;
    std::set<std::string> requested_vehicle_data_;
    std::set<std::string> restored_vehicle_data_;

    std::map<std::string, smart_objects::SmartObject> subscription_results_;

    bool Successfull() const;

    bool DataWasRequested(const std::string& vd) const;
    std::set<std::string> NotSubscribedData() const;

    void FillSubscriptionResults(const smart_objects::SmartObject& response);
    void FillSubscriptionResults();

    void FillRestoredData(
        const std::set<std::string>& successful_subscriptions);
  };

  void SendHMIRequestForNotSubscribed(
      const PendingSubscriptionsResumption& next_pending);
  void RaiseFinishedPendingResumption(
      const PendingSubscriptionsResumption& next_pending);

  std::deque<PendingSubscriptionsResumption> pending_requests_;

  /**
   * @brief SubscribeToFakeRequest will create fake subscription for subscriber
   * ( resumption_data_processor)
   * @param app_id applicaiton to pass into subscriber
   * @param subscriptions list of requested subscriptions
   * @param subscriber function to subscribe caller for ready resumption
   * @return pending_resumption with information about fake correlation id for
   * subscriber, list of vehicle data to subscribe
   */
  PendingSubscriptionsResumption SubscribeToFakeRequest(
      const uint32_t app,
      const std::set<std::string>& subscriptions,
      resumption::Subscriber& subscriber);
};
}  // namespace vehicle_info_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_PENDING_RESUMPTION_HANDLER_H
