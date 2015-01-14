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

#include "model/constant.h"

#include "utils/string_utils.h"

namespace codegen {

Constant::~Constant() {
}

ConstantsCreator::~ConstantsCreator() {
}

Boolean::Constant::~Constant() {
}

bool Boolean::Constant::value() const {
  return value_;
}

bool Boolean::Constant::is_valid() const {
  return valid_;
}

bool Boolean::Constant::operator ==(const Boolean::Constant& that) const {
  return type_ == that.type_ && value_ == that.value_;
}

const Boolean* Boolean::Constant::type() const {
  return type_;
}

void Boolean::Constant::Apply(ConstantCodeGenerator* generator) const {
  generator->GenerateCodeForBooleanConstant(this);
}

Boolean::Constant::Constant(const Boolean* type, const std::string& literal)
    : type_(type),
      value_(literal == "true" ? true : false),
      valid_(literal == "true" || literal == "false") {
}

Integer::Constant::~Constant() {
}

int64_t Integer::Constant::value() const {
  return value_;
}

bool Integer::Constant::is_valid() const {
  return valid_;
}

bool Integer::Constant::operator <(const Integer::Constant& that) const {
  if (valid_ != that.valid_) {
    return int(valid_) < int(that.valid_);
  }
  if (type_ != that.type_) {
    return type_ < that.type_;
  }
  return value_ < that.value_;
}

const Integer* Integer::Constant::type() const {
  return type_;
}

void Integer::Constant::Apply(ConstantCodeGenerator* generator) const {
  generator->GenerateCodeForIntegerConstant(this);
}

Integer::Constant::Constant(const Integer* type, const std::string& literal)
    : type_(type),
      value_(0) {
  valid_ = StringToNumber(literal, &value_) && type_->range().Includes(value_);
}

Float::Constant::~Constant() {
}

double Float::Constant::value() const {
  return value_;
}

bool Float::Constant::is_valid() const {
  return valid_;
}

bool Float::Constant::operator <(const Float::Constant& that) const {
  if (valid_ != that.valid_) {
    return int(valid_) < int(that.valid_);
  }
  if (type_ != that.type_) {
    return type_ < that.type_;
  }
  return value_ < that.value_;
}

const Float* Float::Constant::type() const {
  return type_;
}

void Float::Constant::Apply(ConstantCodeGenerator* generator) const {
  generator->GenerateCodeForFloatConstant(this);
}

Float::Constant::Constant(const Float* type, const std::string& literal)
    : type_(type),
      value_(0) {
  valid_ = StringToNumber(literal, &value_) && type_->range().Includes(value_);
}

Enum::Constant::~Constant() {
}

const std::string& Enum::Constant::name() const {
  return name_;
}

int64_t Enum::Constant::value() const {
  return value_;
}

bool Enum::Constant::is_value_explicit() const {
  return value_explicit_;
}

const Scope& Enum::Constant::scope() const {
  return scope_;
}

const std::string& Enum::Constant::internal_name() const {
  return internal_name_;
}

const Description& Enum::Constant::description() const {
  return description_;
}

const std::string& Enum::Constant::design_description() const {
  return design_description_;
}

const Enum* Enum::Constant::type() const {
  return type_;
}

void Enum::Constant::Apply(ConstantCodeGenerator* generator) const {
  generator->GenerateCodeForEnumConstant(this);
}

Enum::Constant::Constant(Enum* enm, const std::string& name,
                         const int64_t value, bool value_explicit, Scope scope,
                         const std::string& internal_name,
                         const Description& description,
                         const std::string& design_description)
    : type_(enm),
      name_(name),
      value_(value),
      value_explicit_(value_explicit),
      scope_(scope),
      internal_name_(internal_name),
      description_(description),
      design_description_(design_description) {

}

void ConstantCodeGenerator::GenerateCodeForBooleanConstant(
    const Boolean::Constant* boolean) {
}

void ConstantCodeGenerator::GenerateCodeForIntegerConstant(
    const Integer::Constant* integer) {
}

void ConstantCodeGenerator::GenerateCodeForFloatConstant(
    const Float::Constant* flt) {
}

void ConstantCodeGenerator::GenerateCodeForEnumConstant(
    const Enum::Constant* enm) {
}

ConstantCodeGenerator::~ConstantCodeGenerator() {
}

}  // namespace codegen

