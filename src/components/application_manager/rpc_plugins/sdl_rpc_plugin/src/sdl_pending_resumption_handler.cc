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

#include "sdl_rpc_plugin/sdl_pending_resumption_handler.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/message_helper.h"
#include "application_manager/resumption/resumption_data_processor.h"
#include "utils/helpers.h"

namespace sdl_rpc_plugin {

SDL_CREATE_LOG_VARIABLE("SdlRPCPlugin")

SDLPendingResumptionHandler::SDLPendingResumptionHandler(
    application_manager::ApplicationManager& application_manager)
    : ExtensionPendingResumptionHandler(application_manager) {}

smart_objects::SmartObjectSPtr
SDLPendingResumptionHandler::CreateSubscriptionRequest() {
  SDL_LOG_AUTO_TRACE();
  auto subscribe_waypoints_msg =
      application_manager::MessageHelper::CreateMessageForHMI(
          hmi_apis::FunctionID::Navigation_SubscribeWayPoints,
          application_manager_.GetNextHMICorrelationID());
  (*subscribe_waypoints_msg)[application_manager::strings::params]
                            [application_manager::strings::message_type] =
                                hmi_apis::messageType::request;
  return subscribe_waypoints_msg;
}

void SDLPendingResumptionHandler::HandleResumptionSubscriptionRequest(
    application_manager::AppExtension& extension,
    application_manager::Application& app) {
  SDL_LOG_AUTO_TRACE();
  SDLAppExtension& ext = dynamic_cast<SDLAppExtension&>(extension);
  smart_objects::SmartObjectSPtr request = CreateSubscriptionRequest();
  smart_objects::SmartObject& request_ref = *request;
  const auto function_id = static_cast<hmi_apis::FunctionID::eType>(
      request_ref[application_manager::strings::params]
                 [application_manager::strings::function_id]
                     .asInt());
  const uint32_t corr_id =
      request_ref[application_manager::strings::params]
                 [application_manager::strings::correlation_id]
                     .asUInt();

  auto resumption_request =
      MakeResumptionRequest(corr_id, function_id, *request);
  app_ids_.push(app.app_id());

  if (pending_requests_.empty()) {
    SDL_LOG_DEBUG("There are no pending requests for app_id: " << app.app_id());
    pending_requests_[corr_id] = request_ref;
    subscribe_on_event(function_id, corr_id);
    SDL_LOG_DEBUG("Sending request with function id: "
                  << function_id << " and correlation_id: " << corr_id);

    application_manager_.GetRPCService().ManageHMICommand(request);
  } else {
    SDL_LOG_DEBUG("There are pending requests. Freeze resumption for app id "
                  << app.app_id() << " corr id = " << corr_id);
    ResumptionAwaitingHandling frozen_res{
        app.app_id(), ext, resumption_request};
    frozen_resumptions_.push_back(frozen_res);
  }
  resumption_data_processor().SubscribeToResponse(app.app_id(),
                                                  resumption_request);
}

void SDLPendingResumptionHandler::OnResumptionRevert() {
  SDL_LOG_AUTO_TRACE();
  using namespace application_manager;

  if (!pending_requests_.empty()) {
    SDL_LOG_DEBUG("Still waiting for some response");
    return;
  }

  if (!frozen_resumptions_.empty()) {
    ResumptionAwaitingHandling freezed_resumption = frozen_resumptions_.back();
    frozen_resumptions_.pop_back();

    auto request = std::make_shared<smart_objects::SmartObject>(
        freezed_resumption.request_to_send_.message);
    const uint32_t cid =
        (*request)[strings::params][strings::correlation_id].asUInt();
    const auto fid = static_cast<hmi_apis::FunctionID::eType>(
        (*request)[strings::params][strings::function_id].asInt());

    SDL_LOG_DEBUG("Subscribing for event with function id: "
                  << fid << " correlation id: " << cid);
    subscribe_on_event(fid, cid);
    pending_requests_[cid] = *request;
    SDL_LOG_DEBUG("Sending request with fid: " << fid << " and cid: " << cid);
    application_manager_.GetRPCService().ManageHMICommand(request);
  }
}

void SDLPendingResumptionHandler::RaiseFakeSuccessfulResponse(
    ns_smart_device_link::ns_smart_objects::SmartObject response,
    const int32_t corr_id) {
  using namespace application_manager;
  response[strings::params][strings::correlation_id] = corr_id;
  auto fid = static_cast<hmi_apis::FunctionID::eType>(
      response[strings::params][strings::function_id].asInt());
  event_engine::Event event(fid);
  event.set_smart_object(response);

  SDL_LOG_TRACE("Raise fake response for subscriber. corr_id : " << corr_id);
  event.raise(application_manager_.event_dispatcher());
}

void SDLPendingResumptionHandler::on_event(
    const application_manager::event_engine::Event& event) {
  using namespace application_manager;
  SDL_LOG_AUTO_TRACE();

  const smart_objects::SmartObject& response = event.smart_object();
  const uint32_t corr_id = event.smart_object_correlation_id();

  SDL_LOG_TRACE("Received event with function id: "
                << event.id() << " and correlation id: " << corr_id);

  smart_objects::SmartObject pending_request;
  if (pending_requests_.find(corr_id) == pending_requests_.end()) {
    SDL_LOG_ERROR("corr id " << corr_id << " NOT found");
    return;
  }
  pending_request = pending_requests_[corr_id];
  pending_requests_.erase(corr_id);
  if (app_ids_.empty()) {
    SDL_LOG_ERROR("app_ids is empty");
    return;
  }
  uint32_t app_id = app_ids_.front();
  app_ids_.pop();
  auto app = application_manager_.application(app_id);
  if (!app) {
    SDL_LOG_ERROR("Application not found " << app_id);
    return;
  }

  if (resumption::IsResponseSuccessful(response)) {
    SDL_LOG_DEBUG("Resumption of subscriptions is successful");

    application_manager_.SubscribeAppForWayPoints(app);

    for (auto& freezed_resumption : frozen_resumptions_) {
      auto corr_id = freezed_resumption.request_to_send_
                         .message[strings::params][strings::correlation_id]
                         .asInt();
      RaiseFakeSuccessfulResponse(response, corr_id);
      application_manager_.SubscribeAppForWayPoints(freezed_resumption.app_id);
    }
    frozen_resumptions_.clear();
  } else {
    SDL_LOG_DEBUG("Resumption of subscriptions is NOT successful");

    if (frozen_resumptions_.empty()) {
      SDL_LOG_DEBUG("freezed resumptions is empty");
      return;
    }

    ResumptionAwaitingHandling freezed_resumption = frozen_resumptions_.back();
    frozen_resumptions_.pop_back();
    auto resumption_req = freezed_resumption.request_to_send_;
    const uint32_t cid =
        resumption_req.message[strings::params][strings::correlation_id]
            .asInt();
    const hmi_apis::FunctionID::eType fid =
        static_cast<hmi_apis::FunctionID::eType>(
            resumption_req.message[strings::params][strings::function_id]
                .asInt());
    subscribe_on_event(fid, cid);
    auto request =
        std::make_shared<smart_objects::SmartObject>(resumption_req.message);
    SDL_LOG_DEBUG("Subscribing for event with function id: "
                  << fid << " correlation id: " << cid);
    pending_requests_[cid] = *request;
    SDL_LOG_DEBUG("Sending request with fid: " << fid << " and cid: " << cid);
    application_manager_.GetRPCService().ManageHMICommand(request);
  }
}
}  // namespace sdl_rpc_plugin
