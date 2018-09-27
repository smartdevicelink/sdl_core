#include "vehicle_info_plugin/vehicle_info_pending_resumption_handler.h"
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/resumption/resumption_data_processor.h"
#include "application_manager/message_helper.h"
#include "utils/helpers.h"

namespace vehicle_info_plugin {

CREATE_LOGGERPTR_GLOBAL(logger_, "VehicleInfoPendingResumptionHandler")

VehicleInfoPendingResumptionHandler::VehicleInfoPendingResumptionHandler(
    application_manager::ApplicationManager& application_manager)
    : ExtensionPendingResumptionHandler(application_manager) {}

template <class Key, class Value>
std::set<Key> EnumerateKeys(std::map<Key, Value>& container) {
  std::set<std::string> keys;

  std::transform(
      container.begin(),
      container.end(),
      std::inserter(keys, keys.end()),
      [&](const std::pair<std::string, bool>& pair) { return pair.first; });

  return keys;
}

bool VehicleInfoPendingResumptionHandler::IsResumptionResultSuccessful(
    std::map<std::string, bool>& subscription_results) {
  for (auto ivi_status : subscription_results) {
    if (!ivi_status.second) {
      return false;
      break;
    }
  }

  return true;
}

void VehicleInfoPendingResumptionHandler::RemoveSucessfulSubscriptions(
    std::set<std::string>& subscriptions,
    std::set<std::string>& successful_subscriptions) {
  for (auto subscription : subscriptions) {
    if (helpers::in_range(successful_subscriptions, subscription)) {
      subscriptions.erase(subscription);
    }
  }
}

void VehicleInfoPendingResumptionHandler::ClearPendingResumptionRequests() {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace application_manager;
  const hmi_apis::FunctionID::eType timed_out_pending_request_fid =
      static_cast<hmi_apis::FunctionID::eType>(
          pending_requests_.begin()
              ->second[strings::params][strings::function_id]
              .asInt());
  unsubscribe_from_event(timed_out_pending_request_fid);
  pending_requests_.clear();

  if (!freezed_resumptions_.empty()) {
    ResumptionAwaitingHandling freezed_resumption =
        freezed_resumptions_.front();
    freezed_resumptions_.pop();

    std::set<std::string> subscriptions =
        GetExtensionSubscriptions(freezed_resumption.ext);

    auto request = CreateSubscribeRequestToHMI(subscriptions);
    const uint32_t cid =
        (*request)[strings::params][strings::correlation_id].asUInt();
    const hmi_apis::FunctionID::eType fid =
        static_cast<hmi_apis::FunctionID::eType>(
            (*request)[strings::params][strings::function_id].asInt());
    auto resumption_req = MakeResumptionRequest(cid, fid, *request);
    auto subscriber = freezed_resumption.subscriber;
    subscriber(freezed_resumption.app_id, resumption_req);
    LOG4CXX_DEBUG(logger_,
                  "Subscribing for event with function id: "
                      << fid << " correlation id: " << cid);
    subscribe_on_event(fid, cid);
    pending_requests_[cid] = *request;
    LOG4CXX_DEBUG(logger_,
                  "Sending request with fid: " << fid << " and cid: " << cid);
    application_manager_.GetRPCService().ManageHMICommand(request);
  }
}

void VehicleInfoPendingResumptionHandler::on_event(
    const application_manager::event_engine::Event& event) {
  using namespace application_manager;
  LOG4CXX_AUTO_TRACE(logger_);

  const smart_objects::SmartObject& response = event.smart_object();
  const uint32_t corr_id = event.smart_object_correlation_id();

  smart_objects::SmartObject pending_request;
  if (pending_requests_.find(corr_id) == pending_requests_.end()) {
    LOG4CXX_DEBUG(logger_, "corr id" << corr_id << " NOT found");
    return;
  }
  pending_request = pending_requests_[corr_id];
  pending_requests_.erase(corr_id);

  LOG4CXX_DEBUG(logger_,
                "Received event with function id: "
                    << event.id() << " and correlation id: " << corr_id);

  if (freezed_resumptions_.empty()) {
    LOG4CXX_DEBUG(logger_, "freezed resumptions is empty");
    return;
  }

  std::map<std::string, bool> subscription_results =
      ExtractSubscribeResults(pending_request, response);

  LOG4CXX_DEBUG(logger_,
                "pending_requests_.size()" << pending_requests_.size());

  std::set<std::string> successful_subscriptions =
      EnumerateKeys(subscription_results);

  ResumptionAwaitingHandling freezed_resumption = freezed_resumptions_.front();
  freezed_resumptions_.pop();
  resumption::Subscriber subscriber = freezed_resumption.subscriber;

  std::set<std::string> subscriptions =
      GetExtensionSubscriptions(freezed_resumption.ext);

  if (!IsResumptionResultSuccessful(subscription_results)) {
    LOG4CXX_DEBUG(logger_, "Resumption of subscriptions is NOT successful");
  } else {
    LOG4CXX_DEBUG(logger_, "Resumption of subscriptions is successful");
    RemoveSucessfulSubscriptions(subscriptions, successful_subscriptions);
  }

  auto request = CreateSubscribeRequestToHMI(subscriptions);
  const uint32_t cid =
      (*request)[strings::params][strings::correlation_id].asUInt();
  const hmi_apis::FunctionID::eType fid =
      static_cast<hmi_apis::FunctionID::eType>(
          (*request)[strings::params][strings::function_id].asInt());
  auto resumption_req = MakeResumptionRequest(cid, fid, *request);
  subscribe_on_event(fid, cid);
  subscriber(freezed_resumption.app_id, resumption_req);
  LOG4CXX_DEBUG(logger_,
                "Subscribing for event with function id: "
                    << fid << " correlation id: " << cid);
  pending_requests_[cid] = *request;
  LOG4CXX_DEBUG(logger_,
                "Sending request with fid: " << fid << " and cid: " << cid);
  application_manager_.GetRPCService().ManageHMICommand(request);
}

std::map<std::string, bool>
VehicleInfoPendingResumptionHandler::ExtractSubscribeResults(
    const smart_objects::SmartObject& response,
    const smart_objects::SmartObject& request) const {
  using namespace application_manager;
  const hmi_apis::Common_Result::eType result_code =
      static_cast<hmi_apis::Common_Result::eType>(
          response[strings::params][application_manager::hmi_response::code]
              .asInt());
  bool succesfull_response = (result_code == hmi_apis::Common_Result::SUCCESS ||
                              result_code == hmi_apis::Common_Result::WARNINGS);
  const auto response_keys =
      response[application_manager::strings::msg_params].enumerate();
  const auto request_keys =
      request[application_manager::strings::msg_params].enumerate();

  auto response_params = response[strings::msg_params];

  std::map<std::string, bool> subscription_results;

  if (!succesfull_response) {
    for (auto key : request_keys) {
      subscription_results[key] = false;
    }
  }

  if (succesfull_response) {
    for (auto key : request_keys) {
      if (!helpers::in_range(response_keys, key)) {
        subscription_results[key] = true;
      } else {
        const auto kSuccess =
            hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS;
        const auto vd_result_code =
            response_params[key][application_manager::strings::result_code]
                .asInt();
        subscription_results[key] = vd_result_code == kSuccess;
      }
    }
  }
  return subscription_results;
}

void VehicleInfoPendingResumptionHandler::HandleResumptionSubscriptionRequest(
    application_manager::AppExtension& extension,
    app_mngr::Application& app,
    resumption::ResumptionHandlingCallbacks callbacks) {
  LOG4CXX_AUTO_TRACE(logger_);

  VehicleInfoAppExtension& ext =
      dynamic_cast<VehicleInfoAppExtension&>(extension);

  std::set<std::string> subscriptions = GetExtensionSubscriptions(ext);

  smart_objects::SmartObjectSPtr request =
      CreateSubscribeRequestToHMI(subscriptions);

  smart_objects::SmartObject& request_ref = *request;
  const auto function_id = static_cast<hmi_apis::FunctionID::eType>(
      request_ref[application_manager::strings::params]
                 [application_manager::strings::function_id].asInt());
  const uint32_t corr_id =
      request_ref[application_manager::strings::params]
                 [application_manager::strings::correlation_id].asUInt();

  auto resumption_request =
      MakeResumptionRequest(corr_id, function_id, *request);

  if (pending_requests_.empty()) {
    LOG4CXX_DEBUG(logger_,
                  "There are no pending requests for app_id: " << app.app_id());
    pending_requests_[corr_id] = request_ref;
    subscribe_on_event(function_id, corr_id);
    callbacks.subscriber_(app.app_id(), resumption_request);
    LOG4CXX_DEBUG(logger_,
                  "Sending request with function id: "
                      << function_id << " and correlation_id: " << corr_id);
    application_manager_.GetRPCService().ManageHMICommand(request);
    return;
  } else {
    LOG4CXX_DEBUG(logger_,
                  "There are pending requests for app_id: " << app.app_id());
    ResumptionAwaitingHandling frozen_res(
        app.app_id(), ext, callbacks.subscriber_);
    freezed_resumptions_.push(frozen_res);
  }
}

std::set<std::string>
VehicleInfoPendingResumptionHandler::GetExtensionSubscriptions(
    VehicleInfoAppExtension& extension) {
  std::set<std::string> subscriptions;
  for (auto& ivi : application_manager::MessageHelper::vehicle_data()) {
    const auto it = extension.Subscriptions().find(ivi.second);
    if (extension.Subscriptions().end() != it) {
      subscriptions.insert(ivi.first);
    }
  }
  return subscriptions;
}

smart_objects::SmartObjectSPtr
VehicleInfoPendingResumptionHandler::CreateSubscribeRequestToHMI(
    const std::set<std::string>& subscriptions) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace application_manager;
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  for (const auto& ivi_data : subscriptions) {
    msg_params[ivi_data] = true;
  }

  smart_objects::SmartObjectSPtr request =
      application_manager::MessageHelper::CreateModuleInfoSO(
          hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData,
          application_manager_);
  (*request)[strings::msg_params] = msg_params;

  return request;
}
}
