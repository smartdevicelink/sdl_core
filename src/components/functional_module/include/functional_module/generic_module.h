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

namespace functional_modules {

enum ProcessResult {
  NONE = -1,
  PROCESSED,
  PARTIALLY_PROCESSED,
  CANNOT_PROCESS,
  FAILED
};

typedef int ModuleID;

class GenericModule {
  public:
    explicit GenericModule(ModuleID module_id);
    virtual ~GenericModule();
    ModuleID GetModuleID() const {
      return kModuleId_;
    }
    virtual ProcessResult ProcessMessage(const Json::Value& msg) = 0;
    void AddObserver(utils::SharedPtr<ModuleObserver> observer);
    void RemoveObserver(utils::SharedPtr<ModuleObserver> observer);
  protected:
    void NotifyObservers(ModuleObserver::Errors error);
  private:
    DISALLOW_COPY_AND_ASSIGN(GenericModule);
    const ModuleID kModuleId_;
    std::deque<utils::SharedPtr<ModuleObserver> > observers_;
};

typedef utils::SharedPtr<GenericModule> ModulePtr;

}  //  namespace functional_modules

#endif  //  SRC_COMPONENTS_FUNCTINAL_MODULE_INCLUDE_FUNCTIONAL_MODULE_GENERIC_MODULE_H_