#include "sdl_rpc_plugin/sdl_pending_resumption_handler.h"
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/resumption/resumption_data_processor.h"
#include "application_manager/message_helper.h"
#include "utils/helpers.h"

namespace sdl_rpc_plugin {

CREATE_LOGGERPTR_GLOBAL(logger_, "VehicleInfoPendingResumptionHandler")

SDLPendingResumptionHandler::SDLPendingResumptionHandler(
    application_manager::ApplicationManager& application_manager)
    : ExtensionPendingResumptionHandler(application_manager) {}

smart_objects::SmartObjectSPtr
SDLPendingResumptionHandler::CreateSubscriptionRequest() {
  LOG4CXX_AUTO_TRACE(logger_);
  auto subscribe_waypoints_msg =
      application_manager::MessageHelper::CreateMessageForHMI(
          hmi_apis::FunctionID::Navigation_SubscribeWayPoints,
          application_manager_.GetNextHMICorrelationID());
  (*subscribe_waypoints_msg)[application_manager::strings::params]
                            [application_manager::strings::message_type] =
                                hmi_apis::messageType::request;
  return subscribe_waypoints_msg;
}

void SDLPendingResumptionHandler::ClearPendingResumptionRequests() {
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

    auto request = CreateSubscriptionRequest();
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

void SDLPendingResumptionHandler::on_event(
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

  const hmi_apis::Common_Result::eType result_code =
      static_cast<hmi_apis::Common_Result::eType>(
          response[strings::params][application_manager::hmi_response::code]
              .asInt());
  const bool succesfull_response =
      (result_code == hmi_apis::Common_Result::SUCCESS ||
       result_code == hmi_apis::Common_Result::WARNINGS);
  if (succesfull_response) {
    LOG4CXX_DEBUG(logger_, "Resumption of subscriptions is successful");
  } else {
    LOG4CXX_DEBUG(logger_, "Resumption of subscriptions is NOT successful");
    uint32_t app_id = 0;
    if (app_ids_.empty()) {
      LOG4CXX_ERROR(logger_, "app_ids is empty");
      return;
    }
    app_id = app_ids_.front();
    auto app = application_manager_.application(app_id);
    if (!app) {
      LOG4CXX_ERROR(logger_, "Application NOT found");
      return;
    }
    application_manager_.UnsubscribeAppFromWayPoints(app);
    if (freezed_resumptions_.empty()) {
      LOG4CXX_DEBUG(logger_, "freezed resumptions is empty");
      return;
    }

    ResumptionAwaitingHandling freezed_resumption =
        freezed_resumptions_.front();
    freezed_resumptions_.pop();
    resumption::Subscriber subscriber = freezed_resumption.subscriber;

    auto request = CreateSubscriptionRequest();
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
}

void SDLPendingResumptionHandler::HandleResumptionSubscriptionRequest(
    application_manager::AppExtension& extension,
    resumption::Subscriber& subscriber,
    application_manager::Application& app) {
  LOG4CXX_AUTO_TRACE(logger_);
  SDLAppExtension& ext = dynamic_cast<SDLAppExtension&>(extension);
  smart_objects::SmartObjectSPtr request = CreateSubscriptionRequest();
  smart_objects::SmartObject& request_ref = *request;
  const auto function_id = static_cast<hmi_apis::FunctionID::eType>(
      request_ref[application_manager::strings::params]
                 [application_manager::strings::function_id].asInt());
  const uint32_t corr_id =
      request_ref[application_manager::strings::params]
                 [application_manager::strings::correlation_id].asUInt();

  auto resumption_request =
      MakeResumptionRequest(corr_id, function_id, *request);
  app_ids_.push(app.app_id());
  if (pending_requests_.empty()) {
    LOG4CXX_DEBUG(logger_,
                  "There are no pending requests for app_id: " << app.app_id());
    pending_requests_[corr_id] = request_ref;
    subscribe_on_event(function_id, corr_id);
    subscriber(app.app_id(), resumption_request);
    LOG4CXX_DEBUG(logger_,
                  "Sending request with function id: "
                      << function_id << " and correlation_id: " << corr_id);
    application_manager_.GetRPCService().ManageHMICommand(request);
    return;
  }
  LOG4CXX_DEBUG(logger_,
                "There are pending requests for app_id: " << app.app_id());
  ResumptionAwaitingHandling frozen_res(app.app_id(), ext, subscriber);
  freezed_resumptions_.push(frozen_res);
}
}
