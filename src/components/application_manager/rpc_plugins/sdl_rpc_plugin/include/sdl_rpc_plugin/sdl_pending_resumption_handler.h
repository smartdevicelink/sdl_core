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
#include "application_manager/resumption/extension_pending_resumption_handler.h"
#include "application_manager/resumption/resumption_data_processor.h"
#include "sdl_rpc_plugin/sdl_app_extension.h"

namespace sdl_rpc_plugin {

namespace app_mngr = application_manager;

class SDLPendingResumptionHandler
    : public resumption::ExtensionPendingResumptionHandler {
 public:
  SDLPendingResumptionHandler(
      app_mngr::ApplicationManager& application_manager);

  // EventObserver interface
  void on_event(const app_mngr::event_engine::Event& event) OVERRIDE;

  void HandleResumptionSubscriptionRequest(
      app_mngr::AppExtension& extension,
      application_manager::Application& app) OVERRIDE;

  void OnResumptionRevert() OVERRIDE;

 private:
  /**
   * @brief RaiseFakeSuccessfulResponse raise event for the subscriber that
   * contains emulated successful response from HMI To avoid double subscription
   * SDLPendingResumptionHandler freezes sending requests to HMI. But
   * resumption_data_processor().SubscribeOnResponse() need to be called to
   * provide information that some data need to be resumed. So if pending
   * request exists, SDL creates preliminary requests to HMI, subscribe the
   * subscriber to this request but do not send it to HMI. It freezes this
   * requests to precess the one by one when a response to the current request
   * will be received When SDL receives a response from HMI it may satisfy some
   * frozen requests. If it does SDL will create faked HMI response(based on the
   * real one with corr_id replacement) and raise event. So that
   * subscriber::on_event will be called with an appropriate response to the
   * request that SDL was not sent to HMI.
   * @param response message that will be raised with corr_id replacement
   * @param corr_id correlation id that will be replaced in response to notify
   * the subscriber
   */
  void RaiseFakeSuccessfulResponse(smart_objects::SmartObject response,
                                   const int32_t corr_id);
  smart_objects::SmartObjectSPtr CreateSubscriptionRequest();

  struct ResumptionAwaitingHandling {
    const uint32_t app_id;
    SDLAppExtension& ext;
    resumption::ResumptionRequest request_to_send_;
  };

  std::vector<ResumptionAwaitingHandling> frozen_resumptions_;
  std::map<uint32_t, smart_objects::SmartObject> pending_requests_;
  std::queue<uint32_t> app_ids_;
};
}  // namespace sdl_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_SDL_PENDING_RESUMPTION_HANDLER_H_
