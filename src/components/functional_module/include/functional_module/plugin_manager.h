/*
 * Copyright (c) 2017, Ford Motor Company
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

#ifndef SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_PLUGIN_MANAGER_H_
#define SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_PLUGIN_MANAGER_H_

#include <map>
#include <string>
#include "functional_module/generic_module.h"
#include "application_manager/service.h"
#include "application_manager/message.h"

namespace functional_modules {

class PluginManager : public ModuleObserver {
 public:
  PluginManager();
  ~PluginManager();
  typedef std::map<ModuleID, ModulePtr> Modules;
  int LoadPlugins(const std::string& plugin_path);
  void UnloadPlugins();

  /**
   * @brief ProcessMessage forwards modules message to modules if any is
   * subsribed for the message.
   * @param msg Mobile message to process
   * @return Result of processing
   */
  ProcessResult ProcessMessage(application_manager::MessagePtr msg);

  /**
   * @brief ProcessHMIMessage forwards HMI message to modules if any is
   * subscribed for the message
   * @param msg HMI message to process
   * @return Result of processing
   */
  ProcessResult ProcessHMIMessage(application_manager::MessagePtr msg);
  void SetServiceHandler(application_manager::ServicePtr service) {
    service_ = service;
  }
  void OnServiceStateChanged(ServiceState state);
  void OnError(ModuleObserver::Errors error, ModuleID module_id) FINAL;

  /**
   * @brief Remove extension created for specified application
   * @param app_id application id
   */
  void RemoveAppExtension(uint32_t app_id);

  /**
   * @brief Check if app cooperates with one or more plugins, init it
   * inside plugin accordingly if needed.
   * @param app Application in question.
   */
  bool IsAppForPlugins(application_manager::ApplicationSharedPtr app);

  /**
   * Check if app cooperates with plugin
   * @param app application
   * @param module_id unique identifier of plugin
   * @return true if application cooperates with this plugin
   */
  bool IsAppForPlugin(application_manager::ApplicationSharedPtr app,
                      ModuleID module_id) const;

  /**
   * @brief Notify plugins about change of HMILevel of app
   * if app is related to plugin
   * @param app App with new HMILevel
   * @param old_level Old HMILevel of app
   */
  void OnAppHMILevelChanged(application_manager::ApplicationSharedPtr app,
                            mobile_apis::HMILevel::eType old_level);

  /**
   * @brief OnSDLEvent Notifies modules about certain event happened in SDL
   * @param event Event
   * @param application_id Application id of particular application or zero if
   * event is common for all applications
   */
  void OnSDLEvent(functional_modules::SDLEvent event,
                  const uint32_t application_id = 0);

  Modules& plugins();

 private:
  Modules plugins_;
  std::map<ModuleID, void*> dlls_;
  std::map<RCFunctionID, ModulePtr> mobile_subscribers_;
  std::map<HMIFunctionID, ModulePtr> hmi_subscribers_;
  application_manager::ServicePtr service_;

  DISALLOW_COPY_AND_ASSIGN(PluginManager);
};

}  //  namespace functional_modules

#endif  // SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_PLUGIN_MANAGER_H_
