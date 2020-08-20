#pragma once

#include <map>
#include <queue>
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/resumption/extension_pending_resumption_handler.h"
#include "application_manager/resumption/resumption_data_processor.h"
#include "application_manager/rpc_service.h"
#include "rc_rpc_plugin/interior_data_cache.h"
#include "rc_rpc_plugin/rc_app_extension.h"

namespace rc_rpc_plugin {

/**
 * @brief The RCPendingResumptionHandler class
 * responsibility to avoid duplication of subscription requests to HMI
 * if multiple applications are registering
 */
class RCPendingResumptionHandler
    : public resumption::ExtensionPendingResumptionHandler {
 public:
  RCPendingResumptionHandler(
      application_manager::ApplicationManager& application_manager,
      rc_rpc_plugin::InteriorDataCache& interior_data_cache);

  void on_event(const application_manager::event_engine::Event& event) override;

  void HandleResumptionSubscriptionRequest(
      application_manager::AppExtension& extension,
      resumption::Subscriber& subscriber,
      application_manager::Application& app) override;

  void OnResumptionRevert() override;

  static smart_objects::SmartObjectSPtr CreateSubscriptionRequest(
      const ModuleUid& module, const uint32_t correlation_id);

  static hmi_apis::FunctionID::eType GetFunctionId(
      const smart_objects::SmartObject& subscription_request);

  static ModuleUid GetModuleUid(
      const smart_objects::SmartObject& subscription_request);

 private:
  struct PendingRequest {
    uint32_t app_id;
    smart_objects::SmartObject message;
    uint32_t correlation_id() const {
      namespace am_strings = app_mngr::strings;
      const auto cid =
          message[am_strings::params][am_strings::correlation_id].asInt();
      return cid;
    }
  };

  void HandleSuccessfulResponse(
      const application_manager::event_engine::Event& event,
      const ModuleUid& module_uid);

  void ProcessNextFreezedResumption(const ModuleUid& module);

  void RaiseEventForResponse(
      const smart_objects::SmartObject& subscription_response,
      const uint32_t correlation_id) const;

  bool IsPendingForResponse(const ModuleUid& module) const;

  bool IsAnotherAppsSubscribedOnTheSameModule(const uint32_t app_id,
                                              const ModuleUid& module) const;

  utils::Optional<smart_objects::SmartObject> GetPendingRequest(
      const uint32_t corr_id);

  utils::Optional<uint32_t> GetPendingApp(const uint32_t corr_id);

  void RemovePendingRequest(const uint32_t corr_id);

  void AddPendingRequest(const uint32_t app_id,
                         const smart_objects::SmartObject request_so);
  using QueueFreezedResumptions = std::queue<PendingRequest>;
  std::map<ModuleUid, QueueFreezedResumptions> freezed_resumptions_;
  sync_primitives::Lock pending_resumption_lock_;

  std::vector<PendingRequest> pending_requests_;

  application_manager::rpc_service::RPCService& rpc_service_;
  rc_rpc_plugin::InteriorDataCache& interior_data_cache_;
};

}  // namespace rc_rpc_plugin
