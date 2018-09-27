#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_PENDING_RESUMPTION_HANDLER_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_PENDING_RESUMPTION_HANDLER_H

#include "application_manager/resumption/extension_pending_resumption_handler.h"
#include "application_manager/resumption/resumption_data_processor.h"
#include "rc_rpc_plugin.h"

namespace rc_rpc_plugin {

namespace app_mngr = application_manager;

using InteriorDataCacheSptr = std::shared_ptr<InteriorDataCache>;

class RCPendingResumptionHandler
    : public resumption::ExtensionPendingResumptionHandler {
 public:
  RCPendingResumptionHandler(app_mngr::ApplicationManager& application_manager,
                             InteriorDataCacheSptr interior_data_cache);

  ~RCPendingResumptionHandler() {}

  void on_event(const app_mngr::event_engine::Event& event) OVERRIDE;

  void HandleResumptionSubscriptionRequest(
      app_mngr::AppExtension& extension,
      app_mngr::Application& app,
      resumption::ResumptionHandlingCallbacks callbacks) OVERRIDE;

  void ClearPendingResumptionRequests() OVERRIDE;

 private:
  struct ResumptionAwaitingHandling {
    app_mngr::AppExtension& extension;
    const uint32_t application_id;
    resumption::ResumptionHandlingCallbacks callbacks;
    std::map<std::string, bool> handled_subscriptions;

    ResumptionAwaitingHandling(const uint32_t app_id,
                               app_mngr::AppExtension& ext,
                               resumption::ResumptionHandlingCallbacks cb);
  };

  smart_objects::SmartObjectList CreateSubscriptionRequests(
      const std::set<std::string> subscriptions, const uint32_t application_id);

  void ProcessSubscriptionRequests(
      const smart_objects::SmartObjectList& subscription_requests,
      const ResumptionAwaitingHandling& resumption);

  std::set<std::string> GetFrozenResumptionUnhandledSubscriptions(
      const ResumptionAwaitingHandling& frozen_resumption);

  bool NeedsToConcludeResumption(
      const ResumptionAwaitingHandling& resumption_awaiting_handling) const;

  std::shared_ptr<InteriorDataCache> interior_data_cache_;
  std::map<uint32_t, smart_objects::SmartObject> pending_subscription_requests_;
  std::deque<ResumptionAwaitingHandling> frozen_resumptions_;
};

}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_PENDING_RESUMPTION_HANDLER_H
