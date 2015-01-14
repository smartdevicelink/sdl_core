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

#ifndef COMPOSITE_TYPE_H_
#define COMPOSITE_TYPE_H_

#include <stdint.h>
#include <map>
#include <string>
#include <list>

#include "model/constants_creator.h"
#include "model/scope.h"
#include "model/type.h"
#include "utils/common_types.h"
#include "utils/macro.h"


namespace codegen {
class Interface;

class Array : public Type {
 public:
  // Types
  typedef BasicRange<int64_t> Range;
 public:
  // Methods
  Array(const Type* type, const Range& range);
  ~Array();
  const Type* type() const;
  const Range& range() const;
  bool operator<(const Array& that) const;
  // codegen::Type methods
  virtual TypeCodeGenerator* Apply(TypeCodeGenerator* code_generator) const;
  virtual const ConstantsCreator* SupportsConstants() const;

 private:
  // Fields
  const Type* type_;
  Range range_;
};

class Map : public Type {
 public:
  // Types
  typedef BasicRange<int64_t> Range;
 public:
  // Methods
  Map(const Type* type, const Range& range);
  ~Map();
  const Type* type() const;
  const Range& range() const;
  bool operator<(const Map& that) const;
  // codegen::Type methods
  virtual TypeCodeGenerator* Apply(TypeCodeGenerator* code_generator) const;
  virtual const ConstantsCreator* SupportsConstants() const;

 private:
  // Fields
  const Type* type_;
  Range range_;
};

class Enum : public Type, public ConstantsCreator {
 public:
  // Types
  class Constant;
  typedef std::list<Enum::Constant> ConstantsList;
  typedef std::map<std::string, Constant*> ConstantsByName;
public:
  // Constants
  // Name of enum that enlists IDs of all interface functions
  static const char* kFunctionIdEnumName;
 public:
  // Methods
  Enum(const Interface* interface,
       const std::string& name,
       Scope scope,
       InternalScope internal_scope,
       const Description& description);
  ~Enum();
  const Interface& interface() const;
  const std::string& name() const;
  const ConstantsList& constants() const;
  const ConstantsByName& constants_by_name() const;
  const Scope& scope() const;
  const InternalScope& internal_scope() const;
  const Description& description() const;

  // codegen::Type methods
  virtual TypeCodeGenerator* Apply(TypeCodeGenerator* code_generator) const;
  virtual const ConstantsCreator* SupportsConstants() const;
  // codegen::ConstantsCreator methods
  virtual const codegen::Constant* ConstantFor(
      const std::string& literal) const;
  bool AddConstant(const std::string& name, Scope scope,
                   const std::string& internal_name,
                   const Description& description,
                   const std::string& design_description);
  bool AddConstant(const std::string& name, const int64_t value, Scope scope,
                   const std::string& internal_name,
                   const Description& description,
                   const std::string& design_description);
 private:
  // Fields
  const Interface* interface_;
  std::string name_;
  ConstantsList constants_;
  Scope scope_;
  InternalScope internal_scope_;
  Description description_;

  ConstantsByName constants_by_name_;
  int64_t last_constant_value_;
  DISALLOW_COPY_AND_ASSIGN(Enum);
};

/*
 * Struct type
 */
class Struct : public Type {
 public:
  // Types
  class Field;
  typedef std::vector<Field> FieldsList;
 public:
  // Methods
  Struct(const Interface* interface,
         const std::string& name,
         const Type* frankenmap,
         Scope scope,
         const Description& description);
  ~Struct();
  const Interface& interface() const;
  // Returns frankenstruct type if this struct is frankenstruct
  const Type* frankenstruct() const;
  const std::string& name() const;
  const Description& description() const;
  const FieldsList& fields() const;
  const Scope& scope() const;
  bool AddField(const Type* type, const std::string& name, bool mandatory,
                Scope scope, const Constant* default_value,
                const Description& description, Platform platform);
  // codegen::Type methods
  virtual TypeCodeGenerator* Apply(TypeCodeGenerator* code_generator) const;
  virtual const ConstantsCreator* SupportsConstants() const;

 private:
  // Fields
  const Interface* interface_;
  std::string name_;
  const Type* frankenmap_;
  Scope scope_;
  Description description_;
  FieldsList fields_;
  DISALLOW_COPY_AND_ASSIGN(Struct);
};

class Struct::Field {
 public:
  // Methods
  const Type* type() const;
  const std::string& name() const;
  bool is_mandatory() const;
  const Constant* default_value() const;
  const Scope& scope() const;
  const Description& description() const;

 private:
  // Methods
  Field(const Type* type, const std::string& name, bool mandatory, Scope scope,
        const Constant* default_value, const Description& description, Platform platform);
 private:
  // Fields
  const Type* type_;
  std::string name_;
  bool mandatory_;
  const Constant* default_value_;
  Scope scope_;
  Description description_;
  Platform platform_;
  friend class Struct;
};

/*
 *  Nullable type decorator
 */
class NullableType : public Type {
 public:
  // Methods
  NullableType(const Type* type);
  const Type* type() const;
  bool operator<(const NullableType& that) const;
  // codegen::Type methods
  virtual TypeCodeGenerator* Apply(TypeCodeGenerator* code_generator) const;
  virtual const ConstantsCreator* SupportsConstants() const;
 private:
  const Type* type_;
};

/*
 * Typedef type
 */
class Typedef : public Type {
 public:
  Typedef(const Interface* interface,
          const std::string& name,
          const Type* type,
          const Description& description);

  // codegen::Type methods
  virtual TypeCodeGenerator* Apply(TypeCodeGenerator* code_generator) const;
  virtual const ConstantsCreator* SupportsConstants() const;
  const Description& description() const;
  const Interface& interface() const;
  const std::string& name() const;
  const Type* type() const;

 private:
  const Interface* interface_;
  std::string name_;
  const Type* type_;
  Description description_;
};

}  // namespace codegen

#endif /* COMPOSITE_TYPE_H_ */
