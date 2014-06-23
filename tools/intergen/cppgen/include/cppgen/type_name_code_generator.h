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

#ifndef TYPE_NAME_CODE_GENERATOR_H_
#define TYPE_NAME_CODE_GENERATOR_H_

#include "model/type.h"

#include <sstream>

namespace codegen {
class Interface;
class Type;
class TypePreferences;

/*
 * These visitor classes are used to generate C++ type name for given
 * model types.
 */

/*
 * Generates primitive or STL type name for given model type.
 * Used to generate struct constructor parameters.
 */
class TypeNameGenerator: public TypeCodeGenerator {
 public:
  // Generates primitive type name for |type|.
  // |interface| specifies the interface where code is currently
  // being generated. Depending on that relative or fully qualified
  // type names are generated.
  // Generates type name can be accessed with result() method.
  TypeNameGenerator(const Interface* interface,
                    const TypePreferences* preferences,
                    const Type* type);
  ~TypeNameGenerator();
  // Generated type name
  std::string result() const;
 private:
  virtual void GenerateCodeForBoolean(const Boolean* boolean);
  virtual void GenerateCodeForInteger(const Integer* integer);
  virtual void GenerateCodeForFloat(const Float* flt);
  virtual void GenerateCodeForString(const String* string);
  virtual void GenerateCodeForEnum(const Enum* enm);
  virtual void GenerateCodeForArray(const Array* array);
  virtual void GenerateCodeForMap(const Map* map);
  virtual void GenerateCodeForNullable(const NullableType* nullable);
  virtual void GenerateCodeForStruct(const Struct* strct);
  virtual void GenerateCodeForTypedef(const Typedef* tdef);

 private:
  const Interface* interface_;
  const TypePreferences* preferences_;
  bool prefer_reference_type_;
  std::stringstream os_;
};

/*
 * Generates type name from rpc_base template library.
 * Used to define struct fields.
 */
class RpcTypeNameGenerator: public TypeCodeGenerator {
 public:
  // Types
 public:
  // Generates name of type that is able to validate given primitive value
  // |interface| specifies the interface where code is currently
  // being generated. Depending on that relative or fully qualified
  // type names are generated.
  // Depending on |availability| option optionally wraps declaration into
  // Mandatory or Optional template
  RpcTypeNameGenerator(const Interface* interface,
                       const TypePreferences* preferences,
                       const Type* type,
                       bool optional);
  ~RpcTypeNameGenerator();
  // Generated type name
  std::string result() const;
 private:
  // TypeCodeGenerator methods
  virtual void GenerateCodeForBoolean(const Boolean* boolean);
  virtual void GenerateCodeForInteger(const Integer* integer);
  virtual void GenerateCodeForFloat(const Float* flt);
  virtual void GenerateCodeForString(const String* string);
  virtual void GenerateCodeForEnum(const Enum* enm);
  virtual void GenerateCodeForArray(const Array* array);
  virtual void GenerateCodeForMap(const Map* map);
  virtual void GenerateCodeForNullable(const NullableType* nullable);
  virtual void GenerateCodeForStruct(const Struct* strct);
  virtual void GenerateCodeForTypedef(const Typedef* tdef);
 private:
  const Interface* interface_;
  const TypePreferences* preferences_;
  std::stringstream os_;
};

/*
 * Not a real code generator but helper class that is used to collect
 * properties of composite types
 */
class TypeProperties: public TypeCodeGenerator {
public:
  TypeProperties(const Type* type);
  // Tells whether type is map, array or typedef alias of map or array
  bool is_container() const;

private:
  // TypeCodeGenerator methods
  virtual void GenerateCodeForBoolean(const Boolean* boolean);
  virtual void GenerateCodeForInteger(const Integer* integer);
  virtual void GenerateCodeForFloat(const Float* flt);
  virtual void GenerateCodeForString(const String* string);
  virtual void GenerateCodeForEnum(const Enum* enm);
  virtual void GenerateCodeForArray(const Array* array);
  virtual void GenerateCodeForMap(const Map* map);
  virtual void GenerateCodeForNullable(const NullableType* nullable);
  virtual void GenerateCodeForStruct(const Struct* strct);
  virtual void GenerateCodeForTypedef(const Typedef* tdef);
private:
  bool container_;
};

}  // namespace codegen

#endif /* TYPE_NAME_CODE_GENERATOR_H_ */
