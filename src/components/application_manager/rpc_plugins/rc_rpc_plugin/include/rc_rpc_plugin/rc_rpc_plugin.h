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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_RPC_PLUGIN_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_RPC_PLUGIN_H_

#include <memory>

#include "application_manager/plugin_manager/rpc_plugin.h"
#include "application_manager/command_factory.h"
#include "rc_rpc_plugin/resource_allocation_manager.h"
#include "rc_rpc_plugin/interior_data_cache.h"
#include "rc_rpc_plugin/interior_data_manager.h"

namespace rc_rpc_plugin {
namespace plugins = application_manager::plugin_manager;
namespace app_mngr = application_manager;

class RCRPCPlugin : public plugins::RPCPlugin {
 public:
  /**
    * @brief Command initialization function
    * @param app_manager ApplicationManager
    * @param rpc_service RPCService
    * @param hmi_capabilities HMICapabilities
    * @param policy_handler PolicyHandlerInterface
    * @return true in case initialization was succesful, false otherwise.
    **/
  bool Init(app_mngr::ApplicationManager& app_manager,
            app_mngr::rpc_service::RPCService& rpc_service,
            app_mngr::HMICapabilities& hmi_capabilities,
            policy::PolicyHandlerInterface& policy_handler) OVERRIDE;
  /**
  * @param int32_t command id
  * @param CommandSource source
  * @return return true if command can be create, else return false
  **/
  bool IsAbleToProcess(
      const int32_t function_id,
      const app_mngr::commands::Command::CommandSource message_source) OVERRIDE;
  /**
   * @brief PluginName plugin name
   * @return plugin name
   */
  std::string PluginName() OVERRIDE;
  /**
   * @brief GetCommandFactory get that is able to generate command
   * from message
   * @return plugins command factory
   */
  app_mngr::CommandFactory& GetCommandFactory() OVERRIDE;

  /**
   * @brief OnPolicyEvent Processes policy related events
   * @param event Policy event
   */
  void OnPolicyEvent(app_mngr::plugin_manager::PolicyEvent event) OVERRIDE;
  /**
    * @brief OnApplicationEvent Notifies modules on certain application events
    * @param event Event
    * @param application Pointer to application struct
    */
  void OnApplicationEvent(plugins::ApplicationEvent event,
                          app_mngr::ApplicationSharedPtr application) OVERRIDE;

  static const uint32_t kRCPluginID = 153;

  typedef std::vector<application_manager::ApplicationSharedPtr> Apps;
  static Apps GetRCApplications(
      application_manager::ApplicationManager& app_mngr);

 private:
  application_manager::rpc_service::RPCService* rpc_service_;
  application_manager::ApplicationManager* app_mngr_;
  std::unique_ptr<application_manager::CommandFactory> command_factory_;
  std::unique_ptr<ResourceAllocationManager> resource_allocation_manager_;
  std::unique_ptr<InteriorDataCache> interior_data_cache_;
  std::unique_ptr<InteriorDataManager> interior_data_manager_;
};
}  // namespace rc_rpc_plugin

extern "C" application_manager::plugin_manager::RPCPlugin* Create();

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_RPC_PLUGIN_H_
