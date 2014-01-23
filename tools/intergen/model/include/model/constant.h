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

#ifndef CONSTANT_H_
#define CONSTANT_H_

#include <string>

#include "model/builtin_type.h"
#include "model/composite_type.h"
#include "utils/common_types.h"

namespace codegen {
class Type;
class ConstantCodeGenerator;

class Constant {
 public:
  virtual const Type* type() const = 0;
  virtual void Apply(ConstantCodeGenerator* generator) const = 0;
  virtual ~Constant();
};

class Boolean::Constant : public codegen::Constant {
 public:
  ~Constant();
  bool value() const;
  bool is_valid() const;
  bool operator==(const Boolean::Constant& that) const;
  // codegen::Constant methods
  virtual const Boolean* type() const;
  virtual void Apply(ConstantCodeGenerator* generator) const;
 private:
  // Methods
  Constant(const Boolean* type, const std::string& literal);

 private:
  const Boolean* type_;
  bool value_;
  bool valid_;
  friend class Boolean;
};

class Integer::Constant : public codegen::Constant {
 public:
  ~Constant();
  // Methods
  int64_t value() const;
  bool is_valid() const;
  bool operator<(const Constant& that) const;
  // codegen::Constant methods
  virtual const Integer* type() const;
  virtual void Apply(ConstantCodeGenerator* generator) const;

 private:
  // Methods
  Constant(const Integer* type, const std::string& literal);

 private:
  // Fields
  const Integer* type_;
  int64_t value_;
  bool valid_;
  friend class Integer;
};

class Float::Constant : public codegen::Constant {
 public:
  ~Constant();
  // Methods
  double value() const;
  bool is_valid() const;
  bool operator<(const Constant& that) const;
  // codegen::Constant methods
  virtual const Float* type() const;
  virtual void Apply(ConstantCodeGenerator* generator) const;

 private:
  // Methods
  Constant(const Float* type, const std::string& literal);

 private:
  // Fields
  const Float* type_;
  double value_;
  bool valid_;
  friend class Float;
};

class Enum::Constant : public codegen::Constant {
 public:
  // Methods
  ~Constant();
  const std::string& name() const;
  int64_t value() const;
  bool is_value_explicit() const;
  const Scope& scope() const;
  const std::string& internal_name() const;
  const Description& description() const;
  const std::string& design_description() const;

  // codegen::Constant methods
  virtual const Enum* type() const;
  virtual void Apply(ConstantCodeGenerator* generator) const;

 private:
  // Methods
  Constant(Enum* enm, const std::string& name, const int64_t value,
           bool value_explicit, Scope scope, const std::string& internal_name,
           const Description& description, const std::string& design_description);
 private:
  // Fields
  Enum* type_;
  std::string name_;
  int64_t value_;
  bool value_explicit_;
  Scope scope_;
  std::string internal_name_;
  Description description_;
  std::string design_description_;
  friend class Enum;
};

class ConstantCodeGenerator {
 public:
  virtual void GenerateCodeForBooleanConstant(const Boolean::Constant* boolean);
  virtual void GenerateCodeForIntegerConstant(const Integer::Constant* integer);
  virtual void GenerateCodeForFloatConstant(const Float::Constant* flt);
  virtual void GenerateCodeForEnumConstant(const Enum::Constant* enm);
  virtual ~ConstantCodeGenerator();
};

}  // namespace codegen

#endif /* CONSTANT_H_ */
