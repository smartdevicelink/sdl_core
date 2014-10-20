/**
 * Copyright (c) 2014, Ford Motor Company
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

#include "cppgen/cpp_api_code_generator.h"

#include <iostream>

#include "cppgen/cpp_file.h"
#include "cppgen/cpp_interface_code_generator.h"
#include "cppgen/module_manager.h"
#include "cppgen/naming_convention.h"
#include "model/api.h"
#include "model/interface.h"

using std::cout;

namespace codegen {

CppApiCodeGenerator::CppApiCodeGenerator(const API* api)
    : api_(api) {
}

CppApiCodeGenerator::~CppApiCodeGenerator() {
}

std::set<std::string> codegen::CppApiCodeGenerator::Generate(
    const codegen::Preferences& preferences) {
  std::set<std::string> problematic_interafces = preferences.requested_interfaces;
  const std::vector<Interface*>& interfaces = api_->interfaces();
  for (std::vector<Interface*>::const_iterator i = interfaces.begin(), end =
      interfaces.end(); i != end; ++i) {
    const Interface* intf = *i;
    std::string interface_name = LowercaseIntefaceName(*intf);
    if (!preferences.requested_interfaces.empty()) {
      // If interface list provided, skip unneeded interfaces
      if (preferences.requested_interfaces.count(interface_name) == 0) {
        continue;
      }
    }

    if (GenerateInterface(intf, preferences.type_preferences)) {
      // Mark this interface as sucessfully generated
      problematic_interafces.erase(interface_name);
    }
  }
  return problematic_interafces;
}

bool CppApiCodeGenerator::GenerateInterface(const Interface* interface,
                                            const TypePreferences& preferences) {
  ModuleManager mgr(LowercaseIntefaceName(*interface), preferences);
  CppInterfaceCodeGenerator interface_generator(interface,
                                                &preferences,
                                                &mgr);
  interface_generator.GenerateCode();
  return mgr.Write();
}

}  // namespace codegen

