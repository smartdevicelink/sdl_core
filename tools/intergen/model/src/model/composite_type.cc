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

#include "model/composite_type.h"

#include <cassert>
#include <iostream>
#include <utility>

#include "model/constant.h"
#include "utils/safeformat.h"
#include "utils/common_types.h"

using typesafe_format::strmfmt;

namespace codegen {

/*
 * Array type
 */
Array::Array(const Type* type, const Range& range)
    : type_(type),
      range_(range) {
}

Array::~Array() {
}

const Type* Array::type() const {
  return type_;
}

const Array::Range& Array::range() const {
  return range_;
}

bool Array::operator <(const Array& that) const {
  if (type_ != that.type_) {
    return size_t(type_) < size_t(that.type_);
  }
  if (range_.min() == that.range_.min())
    return range_.min() < that.range_.min();
  return range_.max() < that.range_.max();
}

TypeCodeGenerator* Array::Apply(TypeCodeGenerator* code_generator) const {
  code_generator->GenerateCodeForArray(this);
  return code_generator;
}

const ConstantsCreator* Array::SupportsConstants() const {
  return NULL;
}

/*
 * Map type
 */
Map::Map(const Type* type, const Range& range)
    : type_(type),
      range_(range) {
}

Map::~Map() {
}

const Type* Map::type() const {
  return type_;
}

const Map::Range& Map::range() const {
  return range_;
}

bool Map::operator <(const Map& that) const {
  if (type_ != that.type_) {
    return size_t(type_) < size_t(that.type_);
  }
  if (range_.min() == that.range_.min())
    return range_.min() < that.range_.min();
  return range_.max() < that.range_.max();
}

TypeCodeGenerator* Map::Apply(TypeCodeGenerator* code_generator) const {
  code_generator->GenerateCodeForMap(this);
  return code_generator;
}

const ConstantsCreator* Map::SupportsConstants() const {
  return NULL;
}

/*
 * Enum type
 */

// static
const char* Enum::kFunctionIdEnumName = "FunctionID";

Enum::Enum(const Interface* interface,
           const std::string& name,
           Scope scope,
           InternalScope internal_scope,
           const Description& description)
    : interface_(interface),
      name_(name),
      scope_(scope),
      internal_scope_(internal_scope),
      description_(description),
      last_constant_value_(-1) {
  assert(interface);
}

Enum::~Enum() {
}

const Interface& Enum::interface() const {
  return *interface_;
}

const std::string& Enum::name() const {
  return name_;
}

const Enum::ConstantsList& Enum::constants() const {
  return constants_;
}

const Enum::ConstantsByName& Enum::constants_by_name() const {
  return constants_by_name_;
}

const Scope& Enum::scope() const {
  return scope_;
}

const InternalScope& Enum::internal_scope() const {
  return internal_scope_;
}

const Description& Enum::description() const {
  return description_;
}

TypeCodeGenerator* Enum::Apply(TypeCodeGenerator* code_generator) const {
  code_generator->GenerateCodeForEnum(this);
  return code_generator;
}

const ConstantsCreator* Enum::SupportsConstants() const {
  return this;
}

const codegen::Constant* Enum::ConstantFor(const std::string& literal) const {
  ConstantsByName::const_iterator i = constants_by_name_.find(literal);
  if (i != constants_by_name_.end())
    return i->second;
  else {
    return NULL;
  }
}

bool Enum::AddConstant(const std::string& name, Scope scope,
                       const std::string& internal_name,
                       const Description& description,
                       const std::string& design_description) {
  if (ConstantFor(name) != NULL) {
    strmfmt(std::cerr, "Constant {0} already defined in enum {1}\n", name,
            name_);
    return false;
  }
  constants_.push_back(
      Constant(this, name, last_constant_value_ + 1, false, scope, internal_name,
               description, design_description));
  constants_by_name_[name] = &constants_.back();
  ++last_constant_value_;
  return true;
}

bool Enum::AddConstant(const std::string& name, const int64_t value,
                       Scope scope, const std::string& internal_name,
                       const Description& description,
                       const std::string& design_description) {
  if (ConstantFor(name) != NULL) {
    strmfmt(std::cerr, "Constant {0} already defined in enum {1}\n", name,
            name_);
    return false;
  }
  constants_.push_back(
      Constant(this, name, value, true, scope, internal_name, description,
               design_description));
  constants_by_name_[name] = &constants_.back();
  last_constant_value_ = value;
  return true;
}


/*
 * Struct type
 */
Struct::Struct(const Interface* interface,
               const std::string& name,
               const Type* frankenmap,
               Scope scope,
               const Description& description)
    : interface_(interface),
      name_(name),
      frankenmap_(frankenmap),
      scope_(scope),
      description_(description) {
  assert(interface_);
}

Struct::~Struct() {
}

const std::string& Struct::Field::name() const {
  return name_;
}

bool Struct::Field::is_mandatory() const {
  return mandatory_;
}

const Description& Struct::description() const {
  return description_;
}

const Struct::FieldsList& Struct::fields() const {
  return fields_;
}

const Scope& Struct::scope() const {
  return scope_;
}

bool Struct::AddField(const Type* type, const std::string& name, bool mandatory,
                      Scope scope, const Constant* default_value,
                      const Description& description, Platform platform) {
  fields_.push_back(
      Field(type, name, mandatory, scope, default_value, description, platform));
  return true;
}

TypeCodeGenerator* Struct::Apply(TypeCodeGenerator* code_generator) const {
  code_generator->GenerateCodeForStruct(this);
  return code_generator;
}

const ConstantsCreator* Struct::SupportsConstants() const {
  return NULL;
}

const Type* Struct::Field::type() const {
  return type_;
}

const Interface& Struct::interface() const {
  return *interface_;
}

const Type* Struct::frankenstruct() const {
  return frankenmap_;
}

const std::string& Struct::name() const {
  return name_;
}

const Constant* Struct::Field::default_value() const {
  return default_value_;
}

const Scope& Struct::Field::scope() const {
  return scope_;
}

const Description& Struct::Field::description() const {
  return description_;
}

Struct::Field::Field(const Type* type, const std::string& name, bool mandatory,
                     Scope scope, const Constant* default_value,
                     const Description& description, Platform platform)
    : type_(type),
      name_(name),
      mandatory_(mandatory),
      default_value_(default_value),
      scope_(scope),
      description_(description),
      platform_(platform){
}

NullableType::NullableType(const Type* type)
    : type_(type) {
}

const Type* NullableType::type() const {
  return type_;
}

bool NullableType::operator<(const NullableType& that) const {
  // Simple pointer comparison should be enough here
  if (type_ != that.type_) {
    return type_ < that.type_;
  }
  return false;
}

TypeCodeGenerator* NullableType::Apply(
    TypeCodeGenerator* code_generator) const {
  code_generator->GenerateCodeForNullable(this);
  return code_generator;
}

const ConstantsCreator* NullableType::SupportsConstants() const {
  return type_->SupportsConstants();
}

Typedef::Typedef(const Interface* interface,
                 const std::string& name,
                 const Type* type,
                 const Description& description)
    : interface_(interface),
      name_(name),
      type_(type),
      description_(description) {
  assert(interface_);
}

const Description& Typedef::description() const {
  return description_;
}

const Interface& Typedef::interface() const {
  return *interface_;
}

const std::string& Typedef::name() const {
  return name_;
}

const Type* Typedef::type() const {
  return type_;
}

TypeCodeGenerator* Typedef::Apply(TypeCodeGenerator* code_generator) const {
  code_generator->GenerateCodeForTypedef(this);
  return code_generator;
}

const ConstantsCreator* Typedef::SupportsConstants() const {
  return NULL;
}

}  // namespace codegen
