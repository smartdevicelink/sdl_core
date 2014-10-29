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

#ifndef SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_PLUGIN_MANANGER_H_
#define SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_PLUGIN_MANANGER_H_

#include <set>
#include <map>
#include "functional_module/generic_module.h"
#include "application_manager/service.h"
#include "application_manager/message.h"
#include "utils/singleton.h"

namespace functional_modules {

class PluginManager : public utils::Singleton<PluginManager> {
public:
  ~PluginManager();
  int LoadPlugins();
  void UnloadPlugins();
  void ProcessMessage(application_manager::MobileMessage msg);
  void SetServiceHandler(application_manager::ServicePtr service) {
    service_ = service;
  }
  bool IsMessageForPlugin(application_manager::MobileMessage msg);
  void ChangePluginsState(ModuleState state);
  void SubscribeOnHMIFunction(ModuleID module_id, const HMIFunctionID& function_id);
  void OnHMIResponse(application_manager::MobileMessage msg);
private:
  PluginManager();
  DISALLOW_COPY_AND_ASSIGN(PluginManager);
  FRIEND_BASE_SINGLETON_CLASS(PluginManager);
  std::set<PluginInfo> plugins_;
  std::map<MobileFunctionID, ModulePtr> mobile_subscribers_;
  std::map<HMIFunctionID, ModulePtr> hmi_subscribers_;
  application_manager::ServicePtr service_;
};
}  //  namespace functional_modules

#endif  //  SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_PLUGIN_MANANGER_H_