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

#ifndef SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_GENERIC_MODULE_H_
#define SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_GENERIC_MODULE_H_

#include <deque>
#include <string>
#include "functional_module/module_observer.h"
#include "functional_module/function_ids.h"
#include "utils/shared_ptr_helpers.h"
#include "application_manager/service.h"

namespace functional_modules {

/**
 * @brief The PolicyEvent enum defines events related to policy
 */
enum PolicyEvent { kApplicationPolicyUpdated = 0, kApplicationsDisabled };

/**
 * @brief The ApplicationEvent enum defines events related to single application
 */
enum ApplicationEvent { kApplicationExit = 0, kApplicationUnregistered };

enum ProcessResult {
  NONE = -1,
  PROCESSED,
  PARTIALLY_PROCESSED,
  CANNOT_PROCESS,
  FAILED
};

enum ServiceState { IDLE = 0, SUSPENDED, LOWVOLTAGE, HMI_ADAPTER_INITIALIZED };

typedef std::string HMIFunctionID;

class GenericModule;
typedef std::shared_ptr<GenericModule> ModulePtr;

struct PluginInfo {
  std::string name;
  int version;
  std::deque<RCFunctionID> rc_function_list;
  std::deque<HMIFunctionID> hmi_function_list;
};

class GenericModule {
 public:
  typedef std::deque<ModuleObserver*> Observers;

  virtual ~GenericModule();
  ModuleID GetModuleID() const {
    return kModuleId_;
  }
  virtual PluginInfo GetPluginInfo() const = 0;

  /**
   * @brief Setter for service (interface for plugin t access SDL functionality)
   * @param service pointer to new service instance
   */
  virtual void set_service(application_manager::ServicePtr service);

  /**
   * @brief Returns pointer to SDL core service interface
   * @return pointer to core service interface
   */
  virtual application_manager::ServicePtr service();

  virtual ProcessResult ProcessMessage(application_manager::MessagePtr msg) = 0;
  virtual ProcessResult ProcessHMIMessage(
      application_manager::MessagePtr msg) = 0;
  virtual void OnServiceStateChanged(ServiceState state);

  /**
    * @brief Adds pointer to observer of module to be notified about
    * exceptional sutiations in module.
    * Raw pointer is passed to avoid circular dependencies.
    * Module is not responsible for freeing observer's memory.
    */
  void AddObserver(ModuleObserver* const observer);

  /**
    * @brief Removes pointer to observer of module when it's no loger
    * wants to be notified about exceptional sutiations in module.
    * Raw pointer is passed to avoid circular dependencies.
    * Module is not responsible for freeing observer's memory.
    */
  void RemoveObserver(ModuleObserver* const observer);

  /**
   * @brief Remove extension created for specified application
   * @param app_id application id
   */
  virtual void RemoveAppExtension(uint32_t app_id) = 0;

  /**
   * @brief Check registering app can be handled by plugin
   * @param msg Registration message
   * @param app Application basis already create by Core
   */
  virtual bool IsAppForPlugin(
      application_manager::ApplicationSharedPtr app) = 0;

  /**
   * @brief Notify about change of HMILevel of plugin's app
   * @param app App with new HMILevel
   * @param old_level Old HMILevel of app
   */
  virtual void OnAppHMILevelChanged(
      application_manager::ApplicationSharedPtr app,
      mobile_apis::HMILevel::eType old_level) = 0;

  /**
   * @brief OnApplicationEvent Processes application related events
   * @param event Event
   * @param application Pointer to application struct
   */
  virtual void OnApplicationEvent(
      ApplicationEvent event,
      application_manager::ApplicationSharedPtr application) = 0;

  /**
   * @brief OnPolicyEvent Processes policy related events
   * @param event Policy event
   */
  virtual void OnPolicyEvent(PolicyEvent event) = 0;

 protected:
  explicit GenericModule(ModuleID module_id);
  void NotifyObservers(ModuleObserver::Errors error);

  /**
   * @brief Remove extension for all applications
   */
  virtual void RemoveAppExtensions() = 0;

 private:
  application_manager::ServicePtr service_;
  const ModuleID kModuleId_;

  Observers observers_;
  ServiceState state_;

  friend class DriverGenericModuleTest;
  DISALLOW_COPY_AND_ASSIGN(GenericModule);
};

}  //  namespace functional_modules

#endif  // SRC_COMPONENTS_FUNCTIONAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_GENERIC_MODULE_H_
