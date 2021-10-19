#include "rc_rpc_plugin/rc_pending_resumption_handler.h"
#include <sstream>
#include "rc_rpc_plugin/rc_helpers.h"
#include "rc_rpc_plugin/rc_module_constants.h"

namespace rc_rpc_plugin {

SDL_CREATE_LOG_VARIABLE("RemoteControlModule")

RCPendingResumptionHandler::RCPendingResumptionHandler(
    application_manager::ApplicationManager& application_manager,
    InteriorDataCache& interior_data_cache)
    : PendingResumptionHandler(application_manager)
    , rpc_service_(application_manager.GetRPCService())
    , interior_data_cache_(interior_data_cache) {}

void RCPendingResumptionHandler::HandleOnEvent(
    const application_manager::event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  namespace am_strings = application_manager::strings;
  sync_primitives::AutoLock lock(pending_resumption_lock_);
  const auto cid = event.smart_object_correlation_id();
  SDL_LOG_TRACE("Received event with function id: "
                << event.id() << " and correlation id: " << cid);

  const auto request_optional = GetPendingRequest(cid);
  const auto app_id_optional = GetPendingApp(cid);
  if (!request_optional) {
    SDL_LOG_ERROR("Not waiting for message with correlation id: " << cid);
    return;
  }

  auto current_request = *request_optional;
  const auto app_id = *app_id_optional;
  RemovePendingRequest(cid);
  auto module_uid = GetModuleUid(current_request);

  auto& response = event.smart_object();
  if (RCHelpers::IsResponseSuccessful(response)) {
    SDL_LOG_DEBUG("Resumption of subscriptions is successful"
                  << " module type: " << module_uid.first
                  << " module id: " << module_uid.second);

    auto app = application_manager_.application(app_id);
    if (app) {
      auto rc_app_extension = RCHelpers::GetRCExtension(*app);
      rc_app_extension->SubscribeToInteriorVehicleData(module_uid);
    }

    if (response[am_strings::msg_params].keyExists(
            message_params::kModuleData)) {
      const auto module_data =
          response[am_strings::msg_params][message_params::kModuleData];
      const auto& data_mapping = RCHelpers::GetModuleTypeToDataMapping();
      if (module_data.keyExists(data_mapping(module_uid.first))) {
        const auto control_data = module_data[data_mapping(module_uid.first)];
        interior_data_cache_.Add(module_uid, control_data);
      }
    }

    HandleSuccessfulResponse(event, module_uid);
  } else {
    SDL_LOG_DEBUG("Resumption of subscriptions is NOT successful"
                  << " module type: " << module_uid.first
                  << " module id: " << module_uid.second);

    ProcessNextPausedResumption(module_uid);
  }
}

template <typename RCModulesCollection>
std::string Stringify(RCModulesCollection& collection) {
  std::stringstream ss;
  for (const auto& module : collection) {
    ss << "{type : " << module.first << " id " << module.second << "} ";
  }
  return ss.str();
}

void RCPendingResumptionHandler::HandleResumptionSubscriptionRequest(
    application_manager::AppExtension& extension,
    application_manager::Application& app) {
  UNUSED(extension);
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(pending_resumption_lock_);

  auto rc_extension = RCHelpers::GetRCExtension(app);
  auto subscriptions = rc_extension->PendingSubscriptions();
  rc_extension->RemovePendingSubscriptions();
  SDL_LOG_TRACE("app id " << app.app_id() << " " << Stringify(subscriptions));

  std::vector<ModuleUid> ignored;
  std::vector<ModuleUid> already_pending;
  std::vector<ModuleUid> need_to_subscribe;
  for (const auto& subscription : subscriptions) {
    bool is_another_app_subscribed =
        IsOtherAppsSubscribed(app.app_id(), subscription);
    bool is_pending_response = IsPendingForResponse(subscription);
    if (is_another_app_subscribed && !is_pending_response) {
      rc_extension->SubscribeToInteriorVehicleData(subscription);
      ignored.push_back(subscription);
    } else if (is_pending_response) {
      already_pending.push_back(subscription);
    } else {
      need_to_subscribe.push_back(subscription);
    }
  }
  SDL_LOG_TRACE("ignored: " << Stringify(ignored));
  SDL_LOG_TRACE("already_pending: " << Stringify(already_pending));
  SDL_LOG_TRACE("need_to_subscribe: " << Stringify(need_to_subscribe));

  for (auto subscription : already_pending) {
    const auto cid = application_manager_.GetNextHMICorrelationID();
    const auto subscription_request =
        CreateSubscriptionRequest(subscription, cid);
    const auto fid = GetFunctionId(*subscription_request);
    paused_resumptions_[subscription].push(
        {app.app_id(), *subscription_request});
    const auto resumption_request =
        MakeResumptionRequest(cid, fid, *subscription_request);
    resumption_data_processor().SubscribeToResponse(app.app_id(),
                                                    resumption_request);
    SDL_LOG_DEBUG("Paused request with correlation_id: "
                  << cid << " module type: " << subscription.first
                  << " module id: " << subscription.second);
  }

  for (auto module : need_to_subscribe) {
    const auto cid = application_manager_.GetNextHMICorrelationID();
    const auto subscription_request = CreateSubscriptionRequest(module, cid);
    const auto fid = GetFunctionId(*subscription_request);
    const auto resumption_request =
        MakeResumptionRequest(cid, fid, *subscription_request);
    AddPendingRequest(app.app_id(), *subscription_request);
    subscribe_on_event(fid, cid);
    resumption_data_processor().SubscribeToResponse(app.app_id(),
                                                    resumption_request);
    SDL_LOG_DEBUG("Sending request with correlation id: "
                  << cid << " module type: " << module.first
                  << " module id: " << module.second);
    application_manager_.GetRPCService().ManageHMICommand(subscription_request);
  }
}

void RCPendingResumptionHandler::OnResumptionRevert() {
  SDL_LOG_AUTO_TRACE();
}

void RCPendingResumptionHandler::HandleSuccessfulResponse(
    const application_manager::event_engine::Event& event,
    const ModuleUid& module_uid) {
  SDL_LOG_AUTO_TRACE();

  auto& response = event.smart_object();
  auto cid = event.smart_object_correlation_id();

  const auto& it = paused_resumptions_.find(module_uid);
  if (it != paused_resumptions_.end()) {
    SDL_LOG_DEBUG("Paused resumptions found");
    auto& queue_paused = it->second;
    while (!queue_paused.empty()) {
      const auto& resumption_request = queue_paused.front();
      cid = resumption_request.correlation_id();
      RaiseEventForResponse(response, cid);
      auto app = application_manager_.application(resumption_request.app_id);
      if (app) {
        auto rc_app_extension = RCHelpers::GetRCExtension(*app);
        rc_app_extension->SubscribeToInteriorVehicleData(module_uid);
      }
      queue_paused.pop();
    }
    paused_resumptions_.erase(it);
  }
}

void RCPendingResumptionHandler::ProcessNextPausedResumption(
    const ModuleUid& module_uid) {
  SDL_LOG_AUTO_TRACE();

  auto pop_front_paused_resumptions = [this](const ModuleUid& module_uid) {
    const auto& it = paused_resumptions_.find(module_uid);
    if (it == paused_resumptions_.end()) {
      return std::shared_ptr<PendingRequestQueue::value_type>(nullptr);
    }
    auto& queue_paused = it->second;
    if (queue_paused.empty()) {
      paused_resumptions_.erase(it);
      return std::shared_ptr<PendingRequestQueue::value_type>(nullptr);
    }
    auto paused_resumption =
        std::make_shared<PendingRequestQueue::value_type>(queue_paused.front());
    queue_paused.pop();
    if (queue_paused.empty()) {
      paused_resumptions_.erase(it);
    }
    return paused_resumption;
  };

  auto paused_resumption = pop_front_paused_resumptions(module_uid);
  if (!paused_resumption) {
    SDL_LOG_DEBUG("No paused resumptions found");
    return;
  }

  auto& resumption_request = *paused_resumption;
  auto subscription_request =
      std::make_shared<smart_objects::SmartObject>(resumption_request.message);
  const auto fid = GetFunctionId(*subscription_request);
  const auto cid =
      resumption_request
          .message[app_mngr::strings::params][app_mngr::strings::correlation_id]
          .asInt();
  subscribe_on_event(fid, cid);
  AddPendingRequest(resumption_request.app_id, *subscription_request);
  SDL_LOG_DEBUG("Sending request with correlation id: "
                << cid << " module type: " << module_uid.first
                << " module id: " << module_uid.second);
  application_manager_.GetRPCService().ManageHMICommand(subscription_request);
}

void RCPendingResumptionHandler::RaiseEventForResponse(
    const smart_objects::SmartObject& subscription_response,
    const uint32_t correlation_id) const {
  smart_objects::SmartObject event_message = subscription_response;
  event_message[app_mngr::strings::params][app_mngr::strings::correlation_id] =
      correlation_id;

  app_mngr::event_engine::Event event(
      hmi_apis::FunctionID::RC_GetInteriorVehicleData);
  event.set_smart_object(event_message);
  event.raise(application_manager_.event_dispatcher());
}

bool RCPendingResumptionHandler::IsPendingForResponse(
    const ModuleUid& module_uid) const {
  auto is_module_exists = [&module_uid](const PendingRequest& pending) {
    return module_uid == GetModuleUid(pending.message);
  };
  auto it = std::find_if(
      pending_requests_.begin(), pending_requests_.end(), is_module_exists);
  return it != pending_requests_.end();
}

bool RCPendingResumptionHandler::IsOtherAppsSubscribed(
    const uint32_t app_id, const ModuleUid& module_uid) const {
  auto get_subscriptions = [](application_manager::ApplicationSharedPtr app) {
    std::set<ModuleUid> result;
    auto rc_app_extension = RCHelpers::GetRCExtension(*app);
    if (rc_app_extension) {
      result = rc_app_extension->InteriorVehicleDataSubscriptions();
    }
    return result;
  };

  auto app_subscribed = [module_uid, &get_subscriptions](
                            application_manager::ApplicationSharedPtr app) {
    auto subscriptions = get_subscriptions(app);
    auto it = subscriptions.find(module_uid);
    return subscriptions.end() != it;
  };

  std::set<uint32_t> subscribed_apps;
  for (auto& app : application_manager_.applications().GetData()) {
    if (app_subscribed(app)) {
      SDL_LOG_DEBUG("APP " << app->app_id() << " subscribed "
                           << module_uid.first << " " << module_uid.second);
      subscribed_apps.insert(app->app_id());
    }
  }

  subscribed_apps.erase(app_id);
  return !subscribed_apps.empty();
}

utils::Optional<smart_objects::SmartObject>
RCPendingResumptionHandler::GetPendingRequest(const uint32_t corr_id) {
  auto corr_id_match = [corr_id](const PendingRequest& item) {
    return corr_id == item.correlation_id();
  };
  auto it = std::find_if(
      pending_requests_.begin(), pending_requests_.end(), corr_id_match);
  if (it == pending_requests_.end()) {
    return utils::Optional<smart_objects::SmartObject>::EMPTY;
  }
  return it->message;
}

utils::Optional<uint32_t> RCPendingResumptionHandler::GetPendingApp(
    const uint32_t corr_id) {
  auto corr_id_match = [corr_id](const PendingRequest& item) {
    return corr_id == item.correlation_id();
  };
  auto it = std::find_if(
      pending_requests_.begin(), pending_requests_.end(), corr_id_match);
  if (it == pending_requests_.end()) {
    return utils::Optional<uint32_t>::EMPTY;
  }
  return it->app_id;
}

void RCPendingResumptionHandler::RemovePendingRequest(const uint32_t corr_id) {
  auto corr_id_match = [corr_id](const PendingRequest& item) {
    return corr_id == item.correlation_id();
  };
  const auto it = std::find_if(
      pending_requests_.begin(), pending_requests_.end(), corr_id_match);
  if (it == pending_requests_.end()) {
    SDL_LOG_WARN("Pending request with corr_id " << corr_id << " not found");
    return;
  }
  pending_requests_.erase(it);
}

void RCPendingResumptionHandler::AddPendingRequest(
    const uint32_t app_id, const smart_objects::SmartObject request_so) {
  pending_requests_.push_back({app_id, request_so});
}

smart_objects::SmartObjectSPtr
RCPendingResumptionHandler::CreateSubscriptionRequest(
    const ModuleUid& module, const uint32_t correlation_id) {
  return RCHelpers::CreateGetInteriorVDRequestToHMI(
      module, correlation_id, RCHelpers::InteriorDataAction::SUBSCRIBE);
}

hmi_apis::FunctionID::eType RCPendingResumptionHandler::GetFunctionId(
    const smart_objects::SmartObject& subscription_request) {
  const auto function_id = static_cast<hmi_apis::FunctionID::eType>(
      subscription_request[app_mngr::strings::params]
                          [app_mngr::strings::function_id]
                              .asInt());
  return function_id;
}

ModuleUid RCPendingResumptionHandler::GetModuleUid(
    const smart_objects::SmartObject& subscription_request) {
  const smart_objects::SmartObject& msg_params =
      subscription_request[app_mngr::strings::msg_params];
  return ModuleUid(msg_params[message_params::kModuleType].asString(),
                   msg_params[message_params::kModuleId].asString());
}

}  // namespace rc_rpc_plugin
