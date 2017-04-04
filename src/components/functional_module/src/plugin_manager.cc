/*
 * Copyright (c) 2013, Ford Motor Company
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

#include <dlfcn.h>
#include <algorithm>
#include "functional_module/plugin_manager.h"
#include "functional_module/function_ids.h"
#include "utils/file_system.h"
#include "utils/logger.h"
#include "json/json.h"

namespace functional_modules {

CREATE_LOGGERPTR_GLOBAL(logger_, "PluginManager")

typedef std::map<ModuleID, ModulePtr>::iterator PluginsIterator;
typedef std::map<MobileFunctionID, ModulePtr>::iterator PluginFunctionsIterator;
typedef std::map<HMIFunctionID, ModulePtr>::iterator PluginHMIFunctionsIterator;

PluginManager::PluginManager() : service_() {
  LOG4CXX_DEBUG(logger_, "Creating plugin mgr");
}

PluginManager::~PluginManager() {
  // TODO(PV): unsubscribe plugins from functions
  mobile_subscribers_.clear();
  hmi_subscribers_.clear();
  UnloadPlugins();
}

int PluginManager::LoadPlugins(const std::string& plugin_path) {
  LOG4CXX_INFO(logger_, "Loading plugins from " << plugin_path);
  std::vector<std::string> plugin_files = file_system::ListFiles(plugin_path);
  for (size_t i = 0; i < plugin_files.size(); ++i) {
    size_t pos = plugin_files[i].find_last_of(".");
    if (std::string::npos != pos) {
      if (plugin_files[i].substr(pos + 1).compare("so") != 0) {
        continue;
      }
    } else {
      continue;
    }
    std::string full_name = plugin_path + '/' + plugin_files[i];
    void* generic_plugin_dll = dlopen(full_name.c_str(), RTLD_LAZY);
    if (NULL == generic_plugin_dll) {
      LOG4CXX_ERROR(logger_,
                    "Failed to open dll " << plugin_files[i] << "\n"
                                          << dlerror());
      continue;
    }
    typedef GenericModule* (*Create)();
    Create create_manager =
        reinterpret_cast<Create>(dlsym(generic_plugin_dll, "Create"));
    char* error_string = dlerror();
    if (NULL != error_string) {
      LOG4CXX_ERROR(logger_,
                    "Failed to export dll's " << plugin_files[i] << " symbols\n"
                                              << error_string);
      dlclose(generic_plugin_dll);
      continue;
    }
    ModulePtr module = create_manager();
    if (!module) {
      LOG4CXX_ERROR(logger_,
                    "Failed to create plugin main class " << plugin_files[i]);
      dlclose(generic_plugin_dll);
      continue;
    } else {
      LOG4CXX_DEBUG(logger_,
                    "Opened and working plugin from " << plugin_files[i]
                                                      << " with id "
                                                      << module->GetModuleID());
      dlls_.insert(std::pair<ModuleID, void*>(module->GetModuleID(),
                                              generic_plugin_dll));
      plugins_.insert(
          std::pair<ModuleID, ModulePtr>(module->GetModuleID(), module));
      std::deque<MobileFunctionID> subscribers =
          module->GetPluginInfo().mobile_function_list;
      for (size_t i = 0; i < subscribers.size(); ++i) {
        mobile_subscribers_.insert(
            std::pair<MobileFunctionID, ModulePtr>(subscribers[i], module));
      }

      std::deque<HMIFunctionID> hmi_subscribers =
          module->GetPluginInfo().hmi_function_list;
      for (size_t i = 0; i < hmi_subscribers.size(); ++i) {
        hmi_subscribers_.insert(
            std::pair<HMIFunctionID, ModulePtr>(hmi_subscribers[i], module));
      }
      module->set_service(service_);
      module->AddObserver(this);
    }
  }
  return plugins_.size();
}

void PluginManager::UnloadPlugins() {
  for (Modules::iterator it = plugins_.begin(); plugins_.end() != it; ++it) {
    it->second->RemoveObserver(this);
  }
  plugins_.clear();

  for (std::map<ModuleID, void*>::iterator it = dlls_.begin();
       dlls_.end() != it;
       ++it) {
    dlclose(it->second);
  }
  dlls_.clear();
}

// TODO(VS): Optimize similar code in ProcessMessage, IsMessageForPlugin,
// ProcessHMIMessage, IsHMIMessageForPlugin methods
// (also we have similar code in can module)
void PluginManager::ProcessMessage(application_manager::MessagePtr msg) {
  DCHECK(msg);
  if (!msg) {
    LOG4CXX_ERROR(logger_, "Null pointer message was received.");
    return;
  }
  if (application_manager::ProtocolVersion::kUnknownProtocol !=
          msg->protocol_version() &&
      application_manager::ProtocolVersion::kHMI != msg->protocol_version()) {
    PluginFunctionsIterator subscribed_plugin_itr = mobile_subscribers_.find(
        static_cast<MobileFunctionID>(msg->function_id()));
    if (mobile_subscribers_.end() != subscribed_plugin_itr) {
      if (subscribed_plugin_itr->second->ProcessMessage(msg) !=
          ProcessResult::PROCESSED) {
        LOG4CXX_ERROR(logger_, "Failed process HMI message!");
      }
    }
  }
}

ProcessResult PluginManager::ProcessHMIMessage(
    application_manager::MessagePtr msg) {
  DCHECK(msg);
  if (!msg) {
    LOG4CXX_ERROR(logger_, "Null pointer message was received.");
    return ProcessResult::CANNOT_PROCESS;
  }

  Json::Value value;
  Json::Reader reader;
  reader.parse(msg->json_message(), value);

  if (application_manager::ProtocolVersion::kHMI == msg->protocol_version()) {
    std::string msg_method;
    // Request or notification from HMI
    if (value.isMember("method") && value["method"].isString()) {
      msg_method = value["method"].asCString();
      // Response from HMI
    } else if (value.isMember("result") && value["result"].isMember("method") &&
               value["result"]["method"].isString()) {
      msg_method = value["result"]["method"].asCString();
      // Error response from HMI
    } else if (value.isMember("error") && value["error"].isMember("data") &&
               value["error"]["data"].isMember("method") &&
               value["error"]["data"]["method"].isString()) {
      msg_method = value["error"]["data"]["method"].asCString();
    } else {
      LOG4CXX_WARN(logger_,
                   "Message with HMI protocol version can not be handled by "
                   "plugin manager, because required 'method' field was not "
                   "found, or was containing an invalid string.");
      return ProcessResult::CANNOT_PROCESS;
    }

    PluginHMIFunctionsIterator subscribed_plugin_itr =
        hmi_subscribers_.find(msg_method);
    if (hmi_subscribers_.end() != subscribed_plugin_itr) {
      return subscribed_plugin_itr->second->ProcessHMIMessage(msg);
    }
  }

  return ProcessResult::CANNOT_PROCESS;
}

bool PluginManager::IsMessageForPlugin(application_manager::MessagePtr msg) {
  DCHECK(msg);
  if (!msg) {
    LOG4CXX_ERROR(logger_, "Null pointer message was received.");
    return false;
  }
  if (application_manager::ProtocolVersion::kUnknownProtocol !=
          msg->protocol_version() &&
      application_manager::ProtocolVersion::kHMI != msg->protocol_version()) {
    MobileFunctionID id = static_cast<MobileFunctionID>(msg->function_id());
    return (mobile_subscribers_.find(id) != mobile_subscribers_.end());
  } else {
    return false;
  }
}

bool PluginManager::IsHMIMessageForPlugin(application_manager::MessagePtr msg) {
  DCHECK(msg);
  if (!msg) {
    LOG4CXX_ERROR(logger_, "Null pointer message was received.");
    return false;
  }

  Json::Value value;
  Json::Reader reader;
  reader.parse(msg->json_message(), value);

  if (application_manager::ProtocolVersion::kHMI == msg->protocol_version()) {
    std::string msg_method;
    // Request or notification from HMI
    if (value.isMember("method") && value["method"].isString()) {
      msg_method = value["method"].asCString();
      // Response from HMI
    } else if (value.isMember("result") && value["result"].isMember("method") &&
               value["result"]["method"].isString()) {
      msg_method = value["result"]["method"].asCString();
      // Error response from HMI
    } else if (value.isMember("error") && value["error"].isMember("data") &&
               value["error"]["data"].isMember("method") &&
               value["error"]["data"]["method"].isString()) {
      msg_method = value["error"]["data"]["method"].asCString();
    } else {
      LOG4CXX_WARN(logger_,
                   "Message with HMI protocol version can not be handled by "
                   "plugin manager, because required 'method' field was not "
                   "found, or was containing an invalid string.");
      return false;
    }

    return (hmi_subscribers_.find(msg_method) != hmi_subscribers_.end());
  }

  return false;
}

void PluginManager::OnServiceStateChanged(ServiceState state) {
  for (PluginsIterator it = plugins_.begin(); plugins_.end() != it; ++it) {
    it->second->OnServiceStateChanged(state);
  }
}

void PluginManager::OnHMIResponse(application_manager::MessagePtr msg) {
  // TODO(PV)
}

void PluginManager::OnError(ModuleObserver::Errors error, ModuleID module_id) {
  std::string error_string;
  switch (error) {
    case ModuleObserver::Errors::OUT_OF_MEMORY:
      error_string = "Module run out of memory.";
      break;
    case ModuleObserver::Errors::FS_FAILURE:
      error_string = "Plugin failed to run file system operation.";
      break;
    default:
      break;
  }
  LOG4CXX_ERROR(logger_,
                "Error " << error_string << " was received from module "
                         << module_id);
  // TODO(PV)
}

void PluginManager::RemoveAppExtension(uint32_t app_id) {
  for (PluginsIterator it = plugins_.begin(); plugins_.end() != it; ++it) {
    it->second->RemoveAppExtension(app_id);
  }
}

bool PluginManager::IsAppForPlugins(
    application_manager::ApplicationSharedPtr app) {
  DCHECK(app);
  if (!app) {
    return false;
  }

  bool res = false;
  for (PluginsIterator it = plugins_.begin(); plugins_.end() != it; ++it) {
    res = res || it->second->IsAppForPlugin(app);
  }
  return res;
}

bool PluginManager::IsAppForPlugin(
    application_manager::ApplicationSharedPtr app, ModuleID module_id) const {
  Modules::const_iterator i = plugins_.find(module_id);
  return i != plugins_.end() ? i->second->IsAppForPlugin(app) : false;
}

void PluginManager::OnAppHMILevelChanged(
    application_manager::ApplicationSharedPtr app,
    mobile_apis::HMILevel::eType old_level) {
  DCHECK(app);
  if (!app) {
    return;
  }
  for (PluginsIterator it = plugins_.begin(); plugins_.end() != it; ++it) {
    if (it->second->IsAppForPlugin(app)) {
      LOG4CXX_DEBUG(logger_,
                    "Application " << app->name().AsMBString() << " of plugin "
                                   << it->second->GetModuleID()
                                   << " has changed level from " << old_level
                                   << " to " << app->hmi_level());
      it->second->OnAppHMILevelChanged(app, old_level);
    }
  }
}

bool PluginManager::CanAppChangeHMILevel(
    application_manager::ApplicationSharedPtr app,
    mobile_apis::HMILevel::eType new_level) {
  DCHECK(app);
  if (!app) {
    return false;
  }
  bool result = true;
  for (PluginsIterator it = plugins_.begin(); plugins_.end() != it; ++it) {
    if (it->second->IsAppForPlugin(app)) {
      result = result && it->second->CanAppChangeHMILevel(app, new_level);
      LOG4CXX_DEBUG(logger_,
                    "Application " << app->name().AsMBString() << " of plugin "
                                   << it->second->GetModuleID() << " is "
                                   << (result ? "allowed" : "not allowed")
                                   << " to change level to " << new_level);
    }
  }
  return result;
}

typedef std::map<ModuleID, ModulePtr>::value_type PluginsValueType;
struct HandleDeviceRemoved {
 private:
  const connection_handler::DeviceHandle& device_;

 public:
  explicit HandleDeviceRemoved(const connection_handler::DeviceHandle& device)
      : device_(device) {}
  void operator()(PluginsValueType& x) {
    x.second->OnDeviceRemoved(device_);
  }
};

void PluginManager::OnDeviceRemoved(
    const connection_handler::DeviceHandle& device) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::for_each(plugins_.begin(), plugins_.end(), HandleDeviceRemoved(device));
}

PluginManager::Modules& PluginManager::plugins() {
  return plugins_;
}

}  //  namespace functional_modules
