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

#include "cppgen/cpp_interface_code_generator.h"

#include "cppgen/module_manager.h"
#include "model/interface.h"
#include "model/type_registry.h"

namespace codegen {

CppInterfaceCodeGenerator::CppInterfaceCodeGenerator(
    const Interface* interface,
    ModuleManager* module_manager)
    : interface_(interface),
      module_manager_(module_manager),
      declaration_generator_(module_manager_),
      definition_generator_(module_manager_) {
}

CppInterfaceCodeGenerator::~CppInterfaceCodeGenerator() {
}

void CppInterfaceCodeGenerator::GenerateCode() {
  GenerateEnums();
  GenerateTypedefs();
  GenerateStructs();
  GenerateFunctions();
  GenerateResponses();
  GenerateNotifications();
}

void CppInterfaceCodeGenerator::GenerateEnums() {
  const Interface::EnumList& enums = interface_->enums();
  for (Interface::EnumList::const_iterator i = enums.begin(), end = enums.end();
      i != end; ++i) {
    const Enum* e = *i;
    declaration_generator_.GenerateCodeForEnum(e);
    definition_generator_.GenerateCodeForEnum(e);
  }
}

void CppInterfaceCodeGenerator::GenerateStructs() {
  const Interface::StructList& structs = interface_->structs();
  for (Interface::StructList::const_iterator i = structs.begin(), end = structs
      .end(); i != end; ++i) {
    const Struct* s = *i;
    declaration_generator_.GenerateCodeForStruct(s);
    definition_generator_.GenerateCodeForStruct(s);
  }
}

void CppInterfaceCodeGenerator::GenerateTypedefs() {
  const Interface::TypedefList& typedefs = interface_->typedefs();
  for (Interface::TypedefList::const_iterator i = typedefs.begin(), end =
      typedefs.end(); i != end; ++i) {
    const Typedef* tdef = *i;
    declaration_generator_.GenerateCodeForTypedef(tdef);
  }
}

void CppInterfaceCodeGenerator::GenerateFunctions() {
  const Interface::FunctionsList& functions = interface_->functions();
  for (Interface::FunctionsList::const_iterator i = functions.begin(), end =
      functions.end(); i != end; ++i) {
    const Function& function = *i;
    declaration_generator_.GenerateCodeForFunction(function);
    definition_generator_.GenerateCodeForFunction(function);
  }
}

void CppInterfaceCodeGenerator::GenerateResponses() {
  const Interface::ResponseList& responses = interface_->generic_responses();
  for (Interface::ResponseList::const_iterator i = responses.begin(), end =
      responses.end(); i != end; ++i) {
    const Response& response = **i;
    declaration_generator_.GenerateCodeForResponse(response);
    definition_generator_.GenerateCodeForResponse(response);
  }
}

void CppInterfaceCodeGenerator::GenerateNotifications() {
  const Interface::NotificationList& notifications =
      interface_->notifications();
  for (Interface::NotificationList::const_iterator i = notifications.begin(),
      end = notifications.end(); i != end; ++i) {
    const Notification& notification = **i;
    declaration_generator_.GenerateCodeForNotification(notification);
    definition_generator_.GenerateCodeForNotification(notification);
  }
}

}  // namespace codegen
