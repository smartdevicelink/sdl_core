#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <vector>

#include "application_manager/plugin_manager/rpc_plugin.h"
#include "utils/optional.h"
namespace application_manager {
namespace plugin_manager {

class RPCPluginManager {
 public:
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
      const commands::Command::CommandOrigin message_source) = 0;
};
}  // namespace plugin_manager
}  // namespace application_manager
