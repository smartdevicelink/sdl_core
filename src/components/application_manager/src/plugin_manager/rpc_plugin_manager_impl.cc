#include "application_manager/plugin_manager/rpc_plugin_manager_impl.h"

#include <dlfcn.h>

#include "utils/file_system.h"

namespace application_manager {
namespace plugin_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "PluginManager")

bool IsLibraryFile(const std::string& file_path) {
  size_t pos = file_path.find_last_of(".");
  if (std::string::npos == pos) {
    return false;
  }
  if (file_path.substr(pos + 1).compare("so") != 0) {
    return false;
  }
  return true;
}

RPCPluginPtr LoadPlugin(const std::string& full_plugin_path) {
  if (!IsLibraryFile(full_plugin_path)) {
    LOG4CXX_DEBUG(logger_, "Skip loading " << full_plugin_path);
    return RPCPluginPtr();
  }

  void* plugin_dll = dlopen(full_plugin_path.c_str(), RTLD_LAZY);
  if (nullptr == plugin_dll) {
    LOG4CXX_ERROR(logger_,
                  "Failed to open dll " << full_plugin_path << " : "
                                        << dlerror());
    return RPCPluginPtr();
  }

  typedef RPCPlugin* (*Create)();
  Create create_plugin = reinterpret_cast<Create>(dlsym(plugin_dll, "Create"));
  char* error_string = dlerror();
  if (nullptr != error_string) {
    LOG4CXX_ERROR(logger_,
                  "Failed to export dll's " << full_plugin_path
                                            << " symbols : " << error_string);
    dlclose(plugin_dll);
    return RPCPluginPtr();
  }
  RPCPlugin* plugin = create_plugin();
  return RPCPluginPtr(plugin);
}

uint32_t RPCPluginManagerImpl::LoadPlugins(
    const std::string& plugins_path) {
  LOG4CXX_INFO(logger_, "Loading plugins from " << plugins_path);
  std::vector<std::string> plugin_files = file_system::ListFiles(plugins_path);
  for (auto& plugin_file : plugin_files) {
    std::string full_name = plugins_path + '/' + plugin_file;
    auto plugin = LoadPlugin(full_name);
    if (!plugin) {
      continue;
    }
    LOG4CXX_DEBUG(logger_,
                  "Loaded " << plugin->PluginName() << " plugin from "
                            << full_name);
    if (plugin->Init()) {
      loaded_plugins_.push_back(std::move(plugin));
    } else {
      LOG4CXX_ERROR(logger_,
                    "Initialisation of " << plugin->PluginName()
                                         << " plugin from " << full_name
                                         << " failed");
    }
  }
  return loaded_plugins_.size();
}

std::vector<RPCPluginPtr>& RPCPluginManagerImpl::GetPlugins() {
  return loaded_plugins_;
}

utils::Optional<RPCPlugin> RPCPluginManagerImpl::FindPluginToProcess(
    const int32_t function_id,
    const commands::Command::CommandOrigin message_source) {
  typedef utils::Optional<RPCPlugin> PluginOptional;
  for (auto& plugin : loaded_plugins_) {
    if (plugin->IsAbleToProcess(function_id, message_source)) {
      return PluginOptional(*plugin);
    }
  }
  return PluginOptional(PluginOptional::EMPTY,
                        std::string("Plugin for processing RPC not found"));
}
}  // namespace plugin_manager
}  // namespace application_manager
