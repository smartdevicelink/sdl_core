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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_SDL_RPC_PLUGIN_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_SDL_RPC_PLUGIN_H
#include "application_manager/plugin_manager/rpc_plugin.h"
#include "application_manager/command_factory.h"
#include "application_manager/resumption/extension_pending_resumption_handler.h"

namespace sdl_rpc_plugin {
class SDLAppExtension;
namespace plugins = application_manager::plugin_manager;
class SDLRPCPlugin : public plugins::RPCPlugin {
  // RPCPlugin interface
 public:
  SDLRPCPlugin();

  bool Init(application_manager::ApplicationManager& app_manager,
            application_manager::rpc_service::RPCService& rpc_service,
            application_manager::HMICapabilities& hmi_capabilities,
            policy::PolicyHandlerInterface& policy_handler) OVERRIDE;

  bool IsAbleToProcess(
      const int32_t function_id,
      const application_manager::commands::Command::CommandSource
          message_source) OVERRIDE;
  std::string PluginName() OVERRIDE;

  application_manager::CommandFactory& GetCommandFactory() OVERRIDE;
  void OnPolicyEvent(
      application_manager::plugin_manager::PolicyEvent event) OVERRIDE;
  void OnApplicationEvent(
      application_manager::plugin_manager::ApplicationEvent event,
      application_manager::ApplicationSharedPtr application) OVERRIDE;

  /**
   * @brief ProcessResumptionSubscription send appropriate subscribe requests
   * to HMI
   * @param app application for subscription
   * @param ext application extension
   * @param subscriber callback for subscription
   */
  void ProcessResumptionSubscription(application_manager::Application& app,
                                     SDLAppExtension& ext,
                                     resumption::Subscriber subscriber);

  /**
   * @brief Revert the data to the state before Resumption.
   * @param subscriptions Subscriptions to be returned
   **/
  void RevertResumption(application_manager::Application& app);

  /**
   * @brief SaveResumptionData saves subscription data
   * @param resumption_data plase to store resumption data
   */
  void SaveResumptionData(application_manager::Application& app,
                          smart_objects::SmartObject& resumption_data);

 private:
  std::unique_ptr<application_manager::CommandFactory> command_factory_;
  application_manager::ApplicationManager* application_manager_;
  using ExtensionPendingResumptionHandlerSPtr =
      std::shared_ptr<resumption::ExtensionPendingResumptionHandler>;
  ExtensionPendingResumptionHandlerSPtr pending_resumption_handler_;
};
}

extern "C" application_manager::plugin_manager::RPCPlugin* Create();

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_SDL_RPC_PLUGIN_H
