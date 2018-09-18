/*
 Copyright (c) 2018, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_PLUGIN_INCLUDE_SDL_PLUGIN_SDL_PENDING_RESUMPTION_HANDLER_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_PLUGIN_INCLUDE_SDL_PLUGIN_SDL_PENDING_RESUMPTION_HANDLER_H

#include "application_manager/event_engine/event_observer.h"
#include "application_manager/resumption/extension_pending_resumption_handler.h"
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
      resumption::Subscriber& subscriber,
      application_manager::Application& app) OVERRIDE;

  void ClearPendingResumptionRequests() OVERRIDE;

 private:
  smart_objects::SmartObjectSPtr CreateSubscriptionRequest();

  struct ResumptionAwaitingHandling {
    const uint32_t app_id;
    SDLAppExtension& ext;
    resumption::Subscriber subscriber;

    ResumptionAwaitingHandling(const uint32_t application_id,
                               SDLAppExtension& extension,
                               resumption::Subscriber subscriber_callback)
        : app_id(application_id)
        , ext(extension)
        , subscriber(subscriber_callback) {}
  };

  typedef std::pair<SDLAppExtension, resumption::Subscriber> FreezedResumption;
  std::queue<ResumptionAwaitingHandling> freezed_resumptions_;
  std::map<uint32_t, smart_objects::SmartObject> pending_requests_;
  std::queue<uint32_t> app_ids_;
};
}

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_PLUGIN_INCLUDE_SDL_PLUGIN_SDL_PENDING_RESUMPTION_HANDLER_H
