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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_PLUGIN_MANAGER_RPC_PLUGIN_MANAGER_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_PLUGIN_MANAGER_RPC_PLUGIN_MANAGER_H
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <functional>

#include "application_manager/plugin_manager/rpc_plugin.h"
#include "utils/optional.h"
namespace application_manager {
namespace plugin_manager {

class RPCPluginManager {
 public:
  virtual ~RPCPluginManager() {}
  /**
   * @brief LoadPlugins load plugins from plugin folder.
   * Already loaded plugins should continue be loaded.
   *
   * @param plugin_folder folder to search for plugins
   * @return amount of loaded plugins
   */
  virtual uint32_t LoadPlugins(const std::string& plugins_path) = 0;

  /**
   * @brief GetPlugins get list of plugins
   * @return list of loaded plugins
   */
  virtual std::vector<RPCPluginPtr>& GetPlugins() = 0;

  /**
   * @brief FindPluginToProcess find plugin to process message
   * @param function_id RPC identifier to process
   * @param message_source message_source source of message to process
   * @return plugin that is able to process the message
   */
  virtual utils::Optional<RPCPlugin> FindPluginToProcess(
      const int32_t function_id,
      const commands::Command::CommandSource message_source) = 0;

  /**
   * @brief OnPolicyEvent Notifies modules on certain events from policy
   * @param event Policy event
   */
  virtual void ForEachPlugin(std::function<void(RPCPlugin&)> functor) = 0;
};
}  // namespace plugin_manager
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_PLUGIN_MANAGER_RPC_PLUGIN_MANAGER_H
