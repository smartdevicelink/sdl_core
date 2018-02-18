#ifndef SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_RPC_PLUGIN_MANAGER_H_
#define SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_RPC_PLUGIN_MANAGER_H_
#include <string>

#include "gmock/gmock.h"
#include "application_manager/plugin_manager/rpc_plugin_manager.h"

namespace application_manager {
namespace plugin_manager {

class MockRPCPluginManager : public RPCPluginManager {
 public:
  MOCK_METHOD1(LoadPlugins, uint32_t(const std::string& plugins_path));
  MOCK_METHOD0(GetPlugins, std::vector<RPCPluginPtr>&());
  MOCK_METHOD2(FindPluginToProcess,
               utils::Optional<RPCPlugin>(
                   const int32_t function_id,
                   const commands::Command::CommandSource message_source));
  MOCK_METHOD1(ForEachPlugin, void(std::function<void(RPCPlugin&)> functor));
};

}  // namespace plugin_manager
}  // namespace application_manager
#endif  // SRC_COMPONENTS_INCLUDE_TEST_APPLICATION_MANAGER_MOCK_RPC_PLUGIN_MANAGER_H_
