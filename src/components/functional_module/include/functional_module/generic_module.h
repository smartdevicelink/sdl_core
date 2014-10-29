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

#ifndef SRC_COMPONENTS_FUNCTINAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_GENERIC_MODULE_H_
#define SRC_COMPONENTS_FUNCTINAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_GENERIC_MODULE_H_

#include "functional_module/module_observer.h"
#include "json/json.h"
#include "utils/shared_ptr.h"
#include "utils/singleton.h"
#include "application_manager/service.h"

namespace functional_modules {

enum ProcessResult {
  NONE = -1,
  PROCESSED,
  PARTIALLY_PROCESSED,
  CANNOT_PROCESS,
  FAILED
};

typedef int ModuleID;

enum ModuleState {
  NORMAL = 0,
  SUSPENDED,
  LOWVOLTAGE
};

typedef std::string HMIFunctionID;
typedef int MobileFunctionID;

class GenericModule;
typedef utils::SharedPtr<GenericModule> ModulePtr;

struct PluginInfo {
  std::string name;
  int version;
  ModuleID id;
  std::deque<MobileFunctionID> mobile_function_list;
  ModulePtr plugin;
};

class GenericModule : public utils::Singleton<GenericModule> {
  public:
    virtual ~GenericModule();
    ModuleID GetModuleID() const {
      return kModuleId_;
    }
    virtual PluginInfo GetPluginInfo() const = 0;

    void SetServiceHandler(application_manager::ServicePtr service) {
      service_ = service;
    }

    virtual ProcessResult ProcessMessage(const Json::Value& msg) = 0;
    virtual void ChangeModuleState(ModuleState state) {
      state_ = state;
    }
    void AddObserver(utils::SharedPtr<ModuleObserver> observer);
    void RemoveObserver(utils::SharedPtr<ModuleObserver> observer);
  protected:
    explicit GenericModule(ModuleID module_id);
    void NotifyObservers(ModuleObserver::Errors error);
    virtual void RemoveAppExtensions() = 0;
  private:
    DISALLOW_COPY_AND_ASSIGN(GenericModule);
    FRIEND_BASE_SINGLETON_CLASS(GenericModule);
    const ModuleID kModuleId_;
    std::deque<utils::SharedPtr<ModuleObserver> > observers_;
    ModuleState state_;
    application_manager::ServicePtr service_;
};

}  //  namespace functional_modules

#endif  //  SRC_COMPONENTS_FUNCTINAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_GENERIC_MODULE_H_