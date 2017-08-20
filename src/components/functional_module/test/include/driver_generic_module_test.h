/*
 * Copyright (c) 2015, Ford Motor Company
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
#ifndef SRC_COMPONENTS_FUNCTIONAL_MODULE_TEST_INCLUDE_DRIVER_GENERIC_MODULE_TEST_H_
#define SRC_COMPONENTS_FUNCTIONAL_MODULE_TEST_INCLUDE_DRIVER_GENERIC_MODULE_TEST_H_

#include "functional_module/generic_module.h"

namespace functional_modules {

class DriverGenericModuleTest : public GenericModule {
 public:
  explicit DriverGenericModuleTest(ModuleID module_id)
      : GenericModule(module_id) {}
  virtual ~DriverGenericModuleTest() {}
  virtual PluginInfo GetPluginInfo() const {
    PluginInfo info;
    info.name = "DriverGenericModuleTest";
    info.version = 1;
    return info;
  }
  virtual ProcessResult ProcessMessage(application_manager::MessagePtr msg) {
    NotifyObservers(ModuleObserver::FS_FAILURE);
    return ProcessResult::FAILED;
  }
  virtual ProcessResult ProcessHMIMessage(application_manager::MessagePtr msg) {
    return ProcessResult::PROCESSED;
  }
  virtual void RemoveAppExtension(uint32_t app_id) {}
  virtual void RemoveAppExtensions() {}
  bool IsAppForPlugin(application_manager::ApplicationSharedPtr app) {
    return true;
  }

  void OnAppHMILevelChanged(application_manager::ApplicationSharedPtr,
                            mobile_apis::HMILevel::eType) {}

  const Observers& observers() {
    return observers_;
  }

  void OnUnregisterApplication(const uint32_t app_id) {}
};

}  // namespace functional_modules

#endif  // SRC_COMPONENTS_FUNCTIONAL_MODULE_TEST_INCLUDE_DRIVER_GENERIC_MODULE_TEST_H_
