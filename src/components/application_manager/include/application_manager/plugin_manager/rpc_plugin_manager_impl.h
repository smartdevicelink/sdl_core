#ifndef PLUGIN_MANAGER_IMPLEMENTATION_H
#define PLUGIN_MANAGER_IMPLEMENTATION_H
#include "application_manager/plugin_manager/rpc_plugin_manager.h"
#include "utils/optional.h"
namespace application_manager {
namespace plugin_manager {

class RPCPluginManagerImpl : public RPCPluginManager {
  // RPCPluginManager interface
 public:
  uint32_t LoadPlugins(const std::string& plugins_path) OVERRIDE;
  std::vector<RPCPluginPtr>& GetPlugins() OVERRIDE;
  utils::Optional<RPCPlugin> FindPluginToProcess(
      const int32_t function_id,
      const commands::Command::CommandOrigin message_source) OVERRIDE;

 private:
  std::vector<RPCPluginPtr> loaded_plugins_;
};
}  // namespace plugin_manager
}  // namespace application_manager
#endif  // PLUGIN_MANAGER_IMPLEMENTATION_H
