#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_PENDING_RESUMPTION_HANDLER_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_PENDING_RESUMPTION_HANDLER_H
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/resumption/extension_pending_resumption_handler.h"
#include "vehicle_info_app_extension.h"

namespace vehicle_info_plugin {

namespace app_mngr = application_manager;

class VehicleInfoPendingResumptionHandler
    : public resumption::ExtensionPendingResumptionHandler {
 public:
  VehicleInfoPendingResumptionHandler(
      app_mngr::ApplicationManager& application_manager);

  // EventObserver interface
  void on_event(const app_mngr::event_engine::Event& event) OVERRIDE;

  void HandleResumptionSubscriptionRequest(
      app_mngr::AppExtension& extension,
      app_mngr::Application& app,
      resumption::ResumptionHandlingCallbacks callbacks) OVERRIDE;

  std::map<std::string, bool> ExtractSubscribeResults(
      const smart_objects::SmartObject& response,
      const smart_objects::SmartObject& request) const;

  bool IsResumptionResultSuccessful(
      std::map<std::string, bool>& subscription_results);

  void RemoveSucessfulSubscriptions(
      std::set<std::string>& subscriptions,
      std::set<std::string>& successful_subscriptions);

  std::set<std::string> GetExtensionSubscriptions(
      VehicleInfoAppExtension& extension);

  smart_objects::SmartObjectSPtr CreateSubscribeRequestToHMI(
      const std::set<std::string>& subscriptions);

  void ClearPendingResumptionRequests() OVERRIDE;

 private:
  struct ResumptionAwaitingHandling {
    const uint32_t app_id;
    VehicleInfoAppExtension& ext;
    resumption::Subscriber subscriber;

    ResumptionAwaitingHandling(const uint32_t application_id,
                               VehicleInfoAppExtension& extension,
                               resumption::Subscriber subscriber_callback)
        : app_id(application_id)
        , ext(extension)
        , subscriber(subscriber_callback) {}
  };

  typedef std::pair<VehicleInfoAppExtension, resumption::Subscriber>
      FreezedResumption;
  std::queue<ResumptionAwaitingHandling> freezed_resumptions_;
  std::map<uint32_t, smart_objects::SmartObject> pending_requests_;
};
}  // namespace vehicle_info_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_VEHICLE_INFO_PLUGIN_INCLUDE_VEHICLE_INFO_PLUGIN_PENDING_RESUMPTION_HANDLER_H
