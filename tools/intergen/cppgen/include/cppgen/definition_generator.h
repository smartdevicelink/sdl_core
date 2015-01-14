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

#ifndef DEFINITION_GENERATOR_H_
#define DEFINITION_GENERATOR_H_

#include "model/builtin_type.h"
#include "model/function.h"

namespace codegen {
class CppFile;
class ModuleManager;
class Namespace;
class TypePreferences;

/*
 * Generates definition code of different entities that is to be
 * put into implementation (.cc) file
 */

class DefinitionGenerator {
 public:
  DefinitionGenerator(const TypePreferences* preferences,
                      ModuleManager* module_manager);
  ~DefinitionGenerator();

  void GenerateCodeForEnum(const Enum* enm);
  void GenerateCodeForStruct(const Struct* strct);
  void GenerateCodeForFunction(const Function& function);
  void GenerateCodeForResponse(const Response& response);
  void GenerateCodeForNotification(const Notification& notification);
 private:
  // Methods
  void GenerateCodeForRequest(const Request& request, CppFile* source_file);
private:
  // Fields
  const TypePreferences* preferences_;
  ModuleManager* module_manager_;
};

}  // namespace codegen

#endif /* DEFINITION_GENERATOR_H_ */
