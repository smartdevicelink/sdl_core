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

#include "sdl_rpc_plugin/waypoints_pending_resumption_handler.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/message_helper.h"
#include "application_manager/resumption/resumption_data_processor.h"
#include "utils/helpers.h"

namespace sdl_rpc_plugin {

SDL_CREATE_LOG_VARIABLE("SdlRPCPlugin")

using hmi_apis::FunctionID::Navigation_SubscribeWayPoints;

WayPointsPendingResumptionHandler::WayPointsPendingResumptionHandler(
    application_manager::ApplicationManager& application_manager)
    : PendingResumptionHandler(application_manager) {}

smart_objects::SmartObjectSPtr
WayPointsPendingResumptionHandler::CreateSubscriptionRequest() {
  SDL_LOG_AUTO_TRACE();

  auto subscribe_waypoints_msg =
      application_manager::MessageHelper::CreateMessageForHMI(
          Navigation_SubscribeWayPoints,
          application_manager_.GetNextHMICorrelationID());
  (*subscribe_waypoints_msg)[application_manager::strings::params]
                            [application_manager::strings::message_type] =
                                hmi_apis::messageType::request;
  return subscribe_waypoints_msg;
}

void WayPointsPendingResumptionHandler::SendPendingHMIRequest(
    PendingRequest& pending_request) {
  SDL_LOG_AUTO_TRACE();
  using namespace application_manager;

  SDL_LOG_DEBUG("Sending request with function id: "
                << Navigation_SubscribeWayPoints
                << " and correlation_id: " << pending_request.corr_id_);

  auto request = MessageHelper::CreateMessageForHMI(
      Navigation_SubscribeWayPoints, pending_request.corr_id_);
  (*request)[strings::params][strings::message_type] =
      hmi_apis::messageType::request;
  subscribe_on_event(Navigation_SubscribeWayPoints, pending_request.corr_id_);
  application_manager_.GetRPCService().ManageHMICommand(request);
  pending_request.waiting_for_hmi_response_ = true;
}

void WayPointsPendingResumptionHandler::HandleResumptionSubscriptionRequest(
    application_manager::AppExtension& extension,
    application_manager::Application& app) {
  SDL_LOG_AUTO_TRACE();
  using namespace application_manager;
  sync_primitives::AutoLock lock(pending_resumption_lock_);
  UNUSED(extension);

  if (application_manager_.IsAnyAppSubscribedForWayPoints()) {
    SDL_LOG_DEBUG(
        "Subscription to waypoint already exist, no need to send "
        "request to HMI");
    application_manager_.SubscribeAppForWayPoints(app.app_id(), false);
    return;
  }

  const auto request = CreateSubscriptionRequest();
  const auto corr_id =
      (*request)[strings::params][strings::correlation_id].asInt();
  auto resumption_request =
      MakeResumptionRequest(corr_id, Navigation_SubscribeWayPoints, *request);

  PendingRequest pending_request(app.app_id(), corr_id);
  pending_requests_.push_back(pending_request);
  SDL_LOG_DEBUG("Add to pending resumptins corr_id = " << corr_id);

  if (pending_requests_.size() == 1) {
    SendPendingHMIRequest(pending_requests_.front());
  }
  resumption_data_processor().SubscribeToResponse(app.app_id(),
                                                  resumption_request);
}

void WayPointsPendingResumptionHandler::OnResumptionRevert() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(pending_resumption_lock_);

  if (pending_requests_.empty()) {
    SDL_LOG_DEBUG("No pending resumptions");
    return;
  }

  auto& pending_request = pending_requests_.front();
  if (pending_request.waiting_for_hmi_response_) {
    SDL_LOG_DEBUG("Pending resumption for  "
                  << pending_request.app_id_
                  << " is already waiting for HMI response");
    return;
  }
  SendPendingHMIRequest(pending_request);
}

void WayPointsPendingResumptionHandler::RaiseFakeSuccessfulResponse(
    const int32_t corr_id) {
  using namespace application_manager;

  auto response = MessageHelper::CreateResponseMessageFromHmi(
      Navigation_SubscribeWayPoints, corr_id, hmi_apis::Common_Result::SUCCESS);

  event_engine::Event event(Navigation_SubscribeWayPoints);
  event.set_smart_object(*response);

  SDL_LOG_TRACE("Raise fake response for subscriber. corr_id : " << corr_id);
  event.raise(application_manager_.event_dispatcher());
}

void WayPointsPendingResumptionHandler::on_event(
    const application_manager::event_engine::Event& event) {
  using namespace application_manager;
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(pending_resumption_lock_);

  unsubscribe_from_event(Navigation_SubscribeWayPoints);

  if (pending_requests_.empty()) {
    SDL_LOG_DEBUG("Not waiting for any response");
    return;
  }

  const smart_objects::SmartObject& response = event.smart_object();

  SDL_LOG_TRACE("Received event with function id: "
                << event.id() << " and correlation id: "
                << event.smart_object_correlation_id());

  auto current_pending = pending_requests_.front();
  pending_requests_.pop_front();

  auto app = application_manager_.application(current_pending.app_id_);
  if (!app) {
    SDL_LOG_WARN("Application not found " << current_pending.app_id_);
    return;
  }

  if (resumption::IsResponseSuccessful(response)) {
    SDL_LOG_DEBUG("Resumption of waypoints is successful");
    application_manager_.SubscribeAppForWayPoints(app, true);
  }
  ProcessNextPendingResumption();
}

void WayPointsPendingResumptionHandler::ProcessNextPendingResumption() {
  SDL_LOG_AUTO_TRACE();
  if (pending_requests_.empty()) {
    SDL_LOG_DEBUG("No more pending resumptions");
    return;
  }
  auto& pending = pending_requests_.front();
  if (pending.waiting_for_hmi_response_) {
    SDL_LOG_DEBUG("Request was already sent to HMI for " << pending.app_id_);
    return;
  }

  if (!application_manager_.IsAnyAppSubscribedForWayPoints()) {
    SendPendingHMIRequest(pending);
    return;
  }

  auto pending_copy = pending;
  pending_requests_.pop_front();
  auto app = application_manager_.application(pending_copy.app_id_);
  application_manager_.SubscribeAppForWayPoints(app, false);
  RaiseFakeSuccessfulResponse(pending_copy.corr_id_);
  ProcessNextPendingResumption();
}

}  // namespace sdl_rpc_plugin
