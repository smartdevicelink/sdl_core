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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_PLUGIN_MANAGER_RPC_PLUGIN_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_PLUGIN_MANAGER_RPC_PLUGIN_H
#include <memory>
#include "utils/macro.h"
#include "application_manager/commands/command.h"
#include "application_manager/application.h"
#include "application_manager/rpc_service.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/policies/policy_handler_interface.h"

namespace application_manager {
class CommandFactory;

namespace plugin_manager {

/**
 * @brief The PolicyEvent enum defines events related to policy
 */
enum PolicyEvent { kApplicationPolicyUpdated = 0, kApplicationsDisabled };

/**
 * @brief The ApplicationEvent enum defines events related to single application
 */
enum ApplicationEvent {
  kApplicationExit = 0,
  kApplicationRegistered,
  kApplicationUnregistered,
  kDeleteApplicationData
};

class RPCPlugin {
 public:
  /**
    * @brief Command initialization function
    * @param app_manager ApplicationManager
    * @param rpc_service RPCService
    * @param hmi_capabilities HMICapabilities
    * @param policy_handler PolicyHandlerInterface
    * @return true in case initialization was succesful, false otherwise.
    **/
  virtual bool Init(ApplicationManager& app_manager,
                    rpc_service::RPCService& rpc_service,
                    HMICapabilities& hmi_capabilities,
                    policy::PolicyHandlerInterface& policy_handler) = 0;
  /**
   * @brief IsAbleToProcess check if plugin is able to process function
   * @param function_id RPC identifier
   * @param message_source source of message to process (from HMI from mobile
   * ...)
   * @return true if plugin is able to process this RPC, false if not
   */
  virtual bool IsAbleToProcess(
      const int32_t function_id,
      const commands::Command::CommandSource message_source) = 0;

  /**
   * @brief PluginName plugin name
   * @return plugin name
   */
  virtual std::string PluginName() = 0;

  /**
   * @brief GetCommandFactory get that is able to generate command
   * from message
   * @return plugins command factory
   */
  virtual CommandFactory& GetCommandFactory() = 0;
  /**
   * @brief OnPolicyEvent Notifies modules on certain events from policy
   * @param event Policy event
   */
  virtual void OnPolicyEvent(PolicyEvent event) = 0;

  /**
   * @brief OnApplicationEvent Processes application related events
   * @param event Event
   * @param application Pointer to application struct
   */
  virtual void OnApplicationEvent(
      ApplicationEvent event,
      application_manager::ApplicationSharedPtr application) = 0;
};
typedef std::unique_ptr<RPCPlugin> RPCPluginPtr;

}  // namespace plugin_manager
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_PLUGIN_MANAGER_RPC_PLUGIN_H
