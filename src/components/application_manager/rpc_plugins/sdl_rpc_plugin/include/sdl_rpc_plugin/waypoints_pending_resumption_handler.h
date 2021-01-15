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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_SDL_PENDING_RESUMPTION_HANDLER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_SDL_PENDING_RESUMPTION_HANDLER_H_

#include <map>
#include <queue>
#include <vector>
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/resumption/pending_resumption_handler.h"
#include "application_manager/resumption/resumption_data_processor.h"
#include "sdl_rpc_plugin/waypoints_app_extension.h"

namespace sdl_rpc_plugin {

namespace app_mngr = application_manager;

class WayPointsPendingResumptionHandler
    : public resumption::PendingResumptionHandler {
 public:
  WayPointsPendingResumptionHandler(
      app_mngr::ApplicationManager& application_manager);

  // EventObserver interface
  void HandleOnEvent(const app_mngr::event_engine::Event& event) OVERRIDE;

  void HandleResumptionSubscriptionRequest(
      app_mngr::AppExtension& extension,
      application_manager::Application& app) OVERRIDE;

  void OnResumptionRevert() OVERRIDE;

 private:
  /**
   * @brief RaiseFakeSuccessfulResponse raise event for the subscriber that
   * contains emulated successful response from HMI. To avoid double
   * subscription WayPointsPendingResumptionHandler freezes sending requests to
   * HMI. But resumption_data_processor().SubscribeOnResponse() need to be
   * called to provide information that some data need to be resumed. So if
   * pending request exists, SDL creates preliminary requests to HMI, subscribe
   * the subscriber to this request but do not send it to HMI. It freezes this
   * requests to process the one by one when a response to the current request
   * will be received. When SDL receives a response from HMI it may satisfy some
   * frozen requests. If it does SDL will create fake successful HMI
   * response and raise event. So that subscriber::on_event will be called with
   * an appropriate response to the request that SDL was not sent to HMI.
   * @param corr_id correlation id of next pending request
   */
  void RaiseFakeSuccessfulResponse(const int32_t corr_id);
  smart_objects::SmartObjectSPtr CreateSubscriptionRequest();

  /**
   * @brief ProcessNextPendingResumption is responsible for processing of next
   * pending request. If any application is already subscribed to waypoints,
   * this method ensures that current application will be subscribed to
   * waypoints too or send request to HMI otherwise
   */
  void ProcessNextPendingResumption();

  struct PendingRequest {
    explicit PendingRequest(const uint32_t app_id, const uint32_t corr_id)
        : app_id_(app_id)
        , corr_id_(corr_id)
        , waiting_for_hmi_response_(false) {}
    uint32_t app_id_;
    uint32_t corr_id_;
    bool waiting_for_hmi_response_;
  };

  /**
   * @brief SendPendingHMIRequest is responsible for creating and sending of
   * next pending request to HMI. Also here this request is marked as waiting
   * for response.
   * @param pending_request Next pending request
   */
  void SendPendingHMIRequest(PendingRequest& pending_request);

  std::deque<PendingRequest> pending_requests_;
  sync_primitives::RecursiveLock pending_resumption_lock_;
};
}  // namespace sdl_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_SDL_PENDING_RESUMPTION_HANDLER_H_
