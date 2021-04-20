#include <dlfcn.h>

#include "application_manager/plugin_manager/rpc_plugin_manager_impl.h"
#include "utils/file_system.h"
#include "utils/ilogger.h"

namespace application_manager {
namespace plugin_manager {

SDL_CREATE_LOG_VARIABLE("PluginManager")

RPCPluginManagerImpl::RPCPluginManagerImpl(
    ApplicationManager& app_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler,
    resumption::LastStateWrapperPtr last_state)
    : app_manager_(app_manager)
    , rpc_service_(rpc_service)
    , hmi_capabilities_(hmi_capabilities)
    , policy_handler_(policy_handler)
    , last_state_(last_state) {}

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

template <typename T>
T GetFuncFromLib(void* dl_handle, const std::string& function_name) {
  T exported_func =
      reinterpret_cast<T>(dlsym(dl_handle, function_name.c_str()));
  char* error_string = dlerror();
  if (nullptr != error_string) {
    SDL_LOG_ERROR("Failed to export symbols : " << error_string);
    return nullptr;
  }
  return exported_func;
}

RPCPluginManagerImpl::RPCPluginPtr RPCPluginManagerImpl::LoadPlugin(
    const std::string& full_plugin_path) const {
  if (!IsLibraryFile(full_plugin_path)) {
    SDL_LOG_DEBUG("Skip loading " << full_plugin_path);
    return RPCPluginPtr(nullptr, [](RPCPlugin*) {});
  }

  void* plugin_dll = dlopen(full_plugin_path.c_str(), RTLD_LAZY);
  if (nullptr == plugin_dll) {
    SDL_LOG_ERROR("Failed to open dll " << full_plugin_path << " : "
                                        << dlerror());
    return RPCPluginPtr(nullptr, [](RPCPlugin*) {});
  }

  typedef RPCPlugin* (*Create)(logger::Logger*);
  Create create_plugin = GetFuncFromLib<Create>(plugin_dll, "Create");
  if (!create_plugin) {
    SDL_LOG_ERROR("No Create function in " << full_plugin_path);
    dlclose(plugin_dll);
    return RPCPluginPtr(nullptr, [](RPCPlugin*) {});
  }

  typedef void (*Delete)(RPCPlugin*);
  Delete delete_plugin = GetFuncFromLib<Delete>(plugin_dll, "Delete");
  if (!delete_plugin) {
    SDL_LOG_ERROR("No Delete function in " << full_plugin_path);
    dlclose(plugin_dll);
    return RPCPluginPtr(nullptr, [](RPCPlugin*) {});
  }

  auto plugin_destroyer = [delete_plugin, plugin_dll](RPCPlugin* plugin) {
    SDL_LOG_DEBUG("Delete plugin " << plugin->PluginName());
    delete_plugin(plugin);
    dlclose(plugin_dll);
    return RPCPluginPtr(nullptr, [](RPCPlugin*) {});
  };
  RPCPlugin* plugin = create_plugin(&logger::Logger::instance());
  return RPCPluginPtr(plugin, plugin_destroyer);
}

uint32_t RPCPluginManagerImpl::LoadPlugins(const std::string& plugins_path) {
  SDL_LOG_INFO("Loading plugins from " << plugins_path);
  std::vector<std::string> plugin_files = file_system::ListFiles(plugins_path);
  for (auto& plugin_file : plugin_files) {
    std::string full_name = plugins_path + '/' + plugin_file;
    auto plugin = LoadPlugin(full_name);
    if (!plugin) {
      continue;
    }
    SDL_LOG_DEBUG("Loaded " << plugin->PluginName() << " plugin from "
                            << full_name);
    if (plugin->Init(app_manager_,
                     rpc_service_,
                     hmi_capabilities_,
                     policy_handler_,
                     last_state_)) {
      loaded_plugins_.push_back(std::move(plugin));
    } else {
      SDL_LOG_ERROR("Initialisation of " << plugin->PluginName()
                                         << " plugin from " << full_name
                                         << " failed");
    }
  }
  return loaded_plugins_.size();
}

utils::Optional<RPCPlugin> RPCPluginManagerImpl::FindPluginToProcess(
    const int32_t function_id,
    const commands::Command::CommandSource message_source) {
  typedef utils::Optional<RPCPlugin> PluginOptional;
  for (auto& plugin : loaded_plugins_) {
    if (plugin->IsAbleToProcess(function_id, message_source)) {
      return PluginOptional(*plugin);
    }
  }
  return PluginOptional(PluginOptional::EMPTY,
                        std::string("Plugin for processing RPC not found"));
}

void RPCPluginManagerImpl::ForEachPlugin(
    std::function<void(RPCPlugin&)> functor) {
  for (auto& plugin : loaded_plugins_) {
    functor(*plugin);
  }
}

}  // namespace plugin_manager
}  // namespace application_manager
