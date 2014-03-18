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

#ifndef CPP_INTERFACE_CODE_GENERATOR_H_
#define CPP_INTERFACE_CODE_GENERATOR_H_

#include "cppgen/declaration_generator.h"
#include "cppgen/definition_generator.h"

namespace codegen {
class Interface;
class ModuleManager;
class TypePreferences;

/*
 * Generates code for all the entities of single interface
 */
class CppInterfaceCodeGenerator {
 public:
  // Creates code generator for |interface|, using
  // |module_manager| to find where to output the code
  CppInterfaceCodeGenerator(const Interface* interface,
                            const TypePreferences* preferences,
                            ModuleManager* module_manager);
  ~CppInterfaceCodeGenerator();
  // Generate all the interface code
  void GenerateCode();
 private:
  // Generate code for different interface entities
  void GenerateEnums();
  void GenerateStructs();
  void GenerateTypedefs();
  void GenerateFunctions();
  void GenerateResponses();
  void GenerateNotifications();
  void GenerateHandlerInterfaces();
  void GenerateMessageBaseClasses();
  void GenerateMessageFactories();
private:
  // Fields
  const Interface* interface_;
  const TypePreferences* preferences_;
  ModuleManager* module_manager_;
  DeclarationGenerator declaration_generator_;
  DefinitionGenerator definition_generator_;
};

}  // namespace codegen

#endif /* CPP_INTERFACE_CODE_GENERATOR_H_ */
