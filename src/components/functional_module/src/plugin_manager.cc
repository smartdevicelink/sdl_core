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

#include "functional_module/plugin_manager.h"

namespace functional_modules {

typedef std::set<PluginInfo>::iterator PluginInfoIterator;
typedef std::map<MobileFunctionID, ModulePtr>::iterator PluginFunctionsIterator;

PluginManager::PluginManager()
: service_(NULL) {
}

PluginManager::~PluginManager() {
  //  TODO(PV): unsubscribe plugins from functions
  mobile_subscribers_.clear();
  hmi_subscribers_.clear();
  UnloadPlugins();
  plugins_.clear();
}

int PluginManager::LoadPlugins() {
  return 0;
}

void PluginManager::UnloadPlugins() {
  for (PluginInfoIterator it = plugins_.begin();
    plugins_.end() != it; ++it) {

  }
}

void PluginManager::ProcessMessage(application_manager::MobileMessage msg) {
  DCHECK(msg);
  if (application_manager::ProtocolVersion::kUnknownProtocol != msg->protocol_version()
    && application_manager::ProtocolVersion::kHMI != msg->protocol_version()) {
    PluginFunctionsIterator subscribed_plugin_itr = mobile_subscribers_.find(msg->function_id());
    if (mobile_subscribers_.end() != subscribed_plugin_itr) {
      Json::Reader reader;
      Json::Value value;
      if (reader.parse(msg->json_message(), value, 0)) {
        subscribed_plugin_itr->second->ProcessMessage(value);
      }
    }
  }
}

bool PluginManager::IsMessageForPlugin(application_manager::MobileMessage msg) {
  DCHECK(msg);
  if (application_manager::ProtocolVersion::kUnknownProtocol != msg->protocol_version()
    && application_manager::ProtocolVersion::kHMI != msg->protocol_version()) {
    return (mobile_subscribers_.find(msg->function_id()) != mobile_subscribers_.end());
  } else {
    return false;
  }
}

void PluginManager::ChangePluginsState(ModuleState state) {
  for(PluginInfoIterator it = plugins_.begin();
    plugins_.end() != it; ++it) {
    it->plugin->ChangeModuleState(state);
  }
}

void PluginManager::SubscribeOnHMIFunction(ModuleID module_id,
  const HMIFunctionID& function_id) {
  // TODO(PV)
}

void PluginManager::OnHMIResponse(application_manager::MobileMessage msg) {
  // TODO(PV)
}

}  //  namespace functional_modules