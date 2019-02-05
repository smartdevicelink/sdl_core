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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_EXTENSIONS_SYSTEM_CAPABILITY_APP_EXTENSION_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_EXTENSIONS_SYSTEM_CAPABILITY_APP_EXTENSION_H
#include <application_manager/application_manager.h>
#include <set>
#include <string>
#include "sdl_rpc_plugin/sdl_rpc_plugin.h"

namespace sdl_rpc_plugin {
class SDLRPCPlugin;

namespace app_mngr_ = application_manager;

typedef mobile_apis::SystemCapabilityType::eType SystemCapabilityType;
typedef std::set<mobile_apis::SystemCapabilityType::eType>
    SystemCapabilitySubscriptions;

class SystemCapabilityAppExtension : public app_mngr_::AppExtension {
 public:
  SystemCapabilityAppExtension(SDLRPCPlugin& plugin,
                               app_mngr_::Application& app);
  virtual ~SystemCapabilityAppExtension();

  bool subscribeTo(const SystemCapabilityType system_capability_type);

  bool unsubscribeFrom(const SystemCapabilityType system_capability_type);

  void unsubscribeFromAll();

  bool isSubscribedTo(const SystemCapabilityType system_capability_type) const;

  SystemCapabilitySubscriptions Subscriptions();

  void SaveResumptionData(ns_smart_device_link::ns_smart_objects::SmartObject&
                              resumption_data) OVERRIDE;

  void ProcessResumption(
      const smart_objects::SmartObject& resumption_data) OVERRIDE;

  static unsigned SystemCapabilityAppExtensionUID;

  static SystemCapabilityAppExtension& ExtractExtension(
      app_mngr_::Application& app);

 private:
  SystemCapabilitySubscriptions subscribed_data_;
  SDLRPCPlugin& plugin_;
  app_mngr_::Application& app_;
};
}

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_EXTENSIONS_SYSTEM_CAPABILITY_APP_EXTENSION_H
