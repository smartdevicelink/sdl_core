/*
 * Copyright (c) 2018, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_COMMAND_FACTORY_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_COMMAND_FACTORY_H

#include <memory>
#include "application_manager/command_factory.h"
#include "application_manager/application_manager.h"
#include "application_manager/command_factory.h"
#include "application_manager/rpc_service.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "rc_rpc_plugin/resource_allocation_manager.h"
#include "rc_rpc_plugin/interior_data_cache.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;
using policy::PolicyHandlerInterface;
class RCCommandFactory : public application_manager::CommandFactory {
 public:
  /**
   * @brief RCCommandFactory class constructor
   * @param app_manager ApplicationManager
   * @param rpc_service RPCService
   * @param hmi_capabilities HMICapabilities
   * @param policy_handler PolicyHandlerInterface
   * @param allocation_manager ResourceAllocationManager
   **/
  RCCommandFactory(app_mngr::ApplicationManager& app_manager,
                   app_mngr::rpc_service::RPCService& rpc_service,
                   app_mngr::HMICapabilities& hmi_capabilities,
                   policy::PolicyHandlerInterface& policy_handler,
                   ResourceAllocationManager& allocation_manager,
                   InteriorDataCache& interior_data_cache);
  application_manager::CommandSharedPtr CreateCommand(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::commands::Command::CommandSource source) OVERRIDE;

  /**
  * @param int32_t command id
  * @param CommandSource source
  * @return return true if command can be create, else return false
  **/
  virtual bool IsAbleToProcess(
      const int32_t,
      const application_manager::commands::Command::CommandSource)
      const OVERRIDE;

 private:
  app_mngr::CommandCreator& get_mobile_creator_factory(
      mobile_apis::FunctionID::eType id,
      mobile_apis::messageType::eType message_type) const;

  app_mngr::CommandCreator& get_hmi_creator_factory(
      hmi_apis::FunctionID::eType id,
      hmi_apis::messageType::eType message_type) const;

  app_mngr::ApplicationManager& app_manager_;
  app_mngr::rpc_service::RPCService& rpc_service_;
  app_mngr::HMICapabilities& hmi_capabilities_;
  PolicyHandlerInterface& policy_handler_;
  ResourceAllocationManager& allocation_manager_;
  InteriorDataCache& interior_data_cache_;
};
}  // namespace rc_rpc_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_COMMAND_FACTORY_H
