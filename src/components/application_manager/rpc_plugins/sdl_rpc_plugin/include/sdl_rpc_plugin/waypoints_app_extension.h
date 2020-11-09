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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_WAYPOINTS_APP_EXTENSION_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_WAYPOINTS_APP_EXTENSION_H

#include <application_manager/application_manager.h>

namespace sdl_rpc_plugin {
class SDLRPCPlugin;

namespace app_mngr = application_manager;

/**
 * @brief Intended to handle subscriptions on way points
 */
class WayPointsAppExtension : public app_mngr::AppExtension {
 public:
  /**
   * @brief WayPointsAppExtension constructor
   * @param plugin sdl info plugin
   * @param app application that contains this plugin
   */
  WayPointsAppExtension(SDLRPCPlugin& plugin, app_mngr::Application& app);
  virtual ~WayPointsAppExtension();

  void SaveResumptionData(smart_objects::SmartObject& resumption_data) OVERRIDE;

  void ProcessResumption(const smart_objects::SmartObject& saved_app) OVERRIDE;

  void RevertResumption(
      const smart_objects::SmartObject& resumption_data) OVERRIDE;

  /**
   * @brief WayPointsAppExtensionUID unique identifier of waypoints
   * aplication extension
   */
  static unsigned WayPointsAppExtensionUID;

 private:
  SDLRPCPlugin& plugin_;
  app_mngr::Application& app_;
};
}  // namespace sdl_rpc_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_WAYPOINTS_APP_EXTENSION_H
