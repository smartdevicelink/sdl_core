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
#include "functional_module/plugin_manager.h"
#include "functional_module/function_ids.h"
#include "utils/file_system.h"
#include "utils/logger.h"

namespace functional_modules {

CREATE_LOGGERPTR_GLOBAL(logger_, "PluginManager");

typedef std::map<ModuleID, ModulePtr>::iterator PluginsIterator;
typedef std::map<MobileFunctionID, ModulePtr>::iterator PluginFunctionsIterator;

PluginManager::PluginManager()
: service_() {
  LOG4CXX_DEBUG(logger_, "Creating plugin mgr");
}

PluginManager::~PluginManager() {
  //  TODO(PV): unsubscribe plugins from functions
  mobile_subscribers_.clear();
  hmi_subscribers_.clear();
  UnloadPlugins();
  plugins_.clear();
}

int PluginManager::LoadPlugins(const std::string& plugin_path) {
  LOG4CXX_INFO(logger_, "Loading plugins from " << plugin_path);
  std::vector<std::string> plugin_files = file_system::ListFiles(
    plugin_path);
  for(size_t i = 0; i < plugin_files.size(); ++i) {
    size_t pos = plugin_files[i].find_last_of(".");
    if (std::string::npos != pos) {
      if (plugin_files[i].substr(pos+1).compare("so") != 0)
        continue; 
    } else {
      continue;
    }
    void* generic_plugin_dll = dlopen(plugin_files[i].c_str(), RTLD_LAZY);
    if (NULL == generic_plugin_dll) {
      char* failed_to_open = dlerror();
      LOG4CXX_ERROR(logger_, "Failed to open dll " << plugin_files[i] << "\n"
        << failed_to_open);
      continue;
    }
    typedef GenericModule* (*Create)();
    Create create_manager = reinterpret_cast<Create>(dlsym(generic_plugin_dll, "Create"));
    char* error_string = dlerror();
    if (NULL != error_string) {
      LOG4CXX_ERROR(logger_, "Failed to export dll's " << plugin_files[i] << " symbols\n"
        << error_string);
      dlclose(generic_plugin_dll);
      continue;
    }
    GenericModule* module = create_manager();
    if (!module) {
      LOG4CXX_ERROR(logger_, "Failed to create plugin main class " << plugin_files[i]);
      dlclose(generic_plugin_dll);
      continue; 
    } else {
      LOG4CXX_DEBUG(logger_, "Opened and working plugin from "
        << plugin_files[i] << " with id " << module->GetModuleID());
      dlls_.insert(std::pair<ModuleID, void*>(
        module->GetModuleID(), generic_plugin_dll));
      plugins_.insert(std::pair<ModuleID, ModulePtr>(
        module->GetModuleID(), module));
      std::deque<MobileFunctionID> subscribers = module->GetPluginInfo().mobile_function_list;
      for(size_t i = 0; i < subscribers.size(); ++i) {
        mobile_subscribers_.insert(std::pair<MobileFunctionID, ModulePtr>(subscribers[i], module));
      }
      module->SetServiceHandler(service_);
    }
  }
  return plugins_.size();
}

void PluginManager::UnloadPlugins() {
  plugins_.clear();
  for(std::map<ModuleID, void*>::iterator it = dlls_.begin();
    dlls_.end() != it; ++it) {
    dlclose(it->second);
  }
}

void PluginManager::ProcessMessage(application_manager::MessagePtr msg) {
  DCHECK(msg);
  if (application_manager::ProtocolVersion::kUnknownProtocol != msg->protocol_version()
    && application_manager::ProtocolVersion::kHMI != msg->protocol_version()) {
    PluginFunctionsIterator subscribed_plugin_itr = mobile_subscribers_.find(
        static_cast<MobileFunctionID>(msg->function_id()));
    if (mobile_subscribers_.end() != subscribed_plugin_itr) {
      subscribed_plugin_itr->second->ProcessMessage(msg);
    }
  }
}

bool PluginManager::IsMessageForPlugin(application_manager::MessagePtr msg) {
  DCHECK(msg);
  if (application_manager::ProtocolVersion::kUnknownProtocol != msg->protocol_version()
    && application_manager::ProtocolVersion::kHMI != msg->protocol_version()) {
    MobileFunctionID id = static_cast<MobileFunctionID>(msg->function_id());
    return (mobile_subscribers_.find(id) != mobile_subscribers_.end());
  } else {
    return false;
  }
}

void PluginManager::ChangePluginsState(ModuleState state) {
  for(PluginsIterator it = plugins_.begin();
    plugins_.end() != it; ++it) {
    it->second->ChangeModuleState(state);
  }
}

void PluginManager::SubscribeOnHMIFunction(ModuleID module_id,
  const HMIFunctionID& function_id) {
  // TODO(PV)
}

void PluginManager::OnHMIResponse(application_manager::MessagePtr msg) {
  // TODO(PV)
}

}  //  namespace functional_modules
