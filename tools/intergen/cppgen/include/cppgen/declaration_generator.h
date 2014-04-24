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

#ifndef DECLARATION_GENERATOR_H_
#define DECLARATION_GENERATOR_H_

#include "model/composite_type.h"
#include "model/constant.h"
#include "model/function.h"

namespace codegen {
class CppFile;
class GeneratorPreferences;
class ModuleManager;
class Namespace;
class TypePreferences;

/*
 * Generates declaration code of different entities that is to be
 * put into header file
 */
class DeclarationGenerator {
 public:
  // Methods
  DeclarationGenerator(const TypePreferences* preferences,
                       ModuleManager* module_manager);
  ~DeclarationGenerator();

  // Methods that generate code for different code model entities
  void GenerateCodeForEnum(const Enum* enm);
  void GenerateCodeForStruct(const Struct* strct);
  void GenerateCodeForTypedef(const Typedef* tdef);
  void GenerateCodeForFunction(const Function& function);
  void GenerateCodeForResponse(const Response& response);
  void GenerateCodeForNotification(const Notification& notification);
 private:
  // Methods
  void GenerateCodeForRequest(const Request& response, CppFile* header_file);
  void GenerateCodeForEnumConstant(const Enum::Constant& enm,
                                   CppFile* header_file,
                                   bool skip_coma);
  void GenerateCodeForStructField(const Struct& strct,
                                  const Struct::Field& field,
                                  CppFile* header_file,
                                  Namespace* name_space);
  void GenerateCodeForStructFields(const Struct& strct,
                                   CppFile* header_file,
                                   Namespace* name_space);
 private:
  // Fields
  const TypePreferences* preferences_;
  ModuleManager* module_manager_;
};

/*
 * This class is used to add forward declaration of specific type
 * to file's namespace
 */
class TypeForwardDeclarator : public TypeCodeGenerator {
 public:
  // Creates objec and automatically forward declares |type| in namespace
  // |ns|
  TypeForwardDeclarator(Namespace* ns, const Type* type);
 private:
  // TypeCodeGenerator interface

  // Only structs can be forward declared but they can be part of array declaration
  // All other types don't need forward declaration
  virtual void GenerateCodeForNullable(const NullableType* nullable);
  virtual void GenerateCodeForArray(const Array* array);
  virtual void GenerateCodeForMap(const Map* map);
  virtual void GenerateCodeForStruct(const Struct* strct);
 private:
  // Fields
  Namespace* ns_;
};

}  // namespace codegen

#endif /* DECLARATION_GENERATOR_H_ */
