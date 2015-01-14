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

#ifndef BUILTIN_TYPE_H_
#define BUILTIN_TYPE_H_

#include <stdint.h>
#include <set>
#include <string>
#include <vector>

#include "model/constants_creator.h"
#include "model/type.h"
#include "utils/common_types.h"

namespace codegen {

/*
 * Boolean type
 */
class Boolean : public Type, public ConstantsCreator {
 public:
  // Types
  class Constant;
 public:
  // Methods
  Boolean();
  ~Boolean();
  // codegen::Type methods
  virtual TypeCodeGenerator* Apply(TypeCodeGenerator* code_generator) const;
  virtual const ConstantsCreator* SupportsConstants() const;
  // codegen::SupportsLiterals methods
  virtual const codegen::Constant* ConstantFor(
      const std::string& literal) const;

 public:
  // Complete inner class declarations

 private:
  // Fields
  std::vector<Constant> constants_;
};

/*
 * Integer type
 */
class Integer : public Type, public ConstantsCreator {
 public:
  // Types
  typedef BasicRange<int64_t> Range;
  class Constant;
 public:
  // Methods
  Integer(const Range& range);
  ~Integer();
  const Range& range() const;
  bool operator<(const Integer& that) const;

  // codegen::Type methods
  virtual TypeCodeGenerator* Apply(TypeCodeGenerator* code_generator) const;
  virtual const ConstantsCreator* SupportsConstants() const;
  // codegen::SupportsLiterals methods
  virtual const codegen::Constant* ConstantFor(
      const std::string& literal) const;
 private:
  // Fields
  Range range_;
  mutable std::set<Constant> constants_;
};

/*
 * Float type
 */
class Float : public Type, public ConstantsCreator {
 public:
  // Types
  typedef BasicRange<double> Range;
  class Constant;
 public:
  // Methods
  Float(const Range& range);
  ~Float();
  const Range& range() const;
  bool operator<(const Float& that) const;

  // codegen::Type methods
  virtual TypeCodeGenerator* Apply(TypeCodeGenerator* code_generator) const;
  virtual const ConstantsCreator* SupportsConstants() const;
  // codegen::SupportsLiterals methods
  virtual const codegen::Constant* ConstantFor(
      const std::string& literal) const;
 private:
  // Fields
  Range range_;
  mutable std::set<Constant> constants_;
};

/*
 * String type
 */
class String : public Type {
 public:
  // Types
  typedef BasicRange<int64_t> Range;
public:
  // Methods
  String(const Range& range);
  bool operator<(const String& that) const;
  const Range& length_range() const;
  // codegen::Type methods
  virtual TypeCodeGenerator* Apply(TypeCodeGenerator* code_generator) const;
  virtual const ConstantsCreator* SupportsConstants() const;

private:
  // Fields
  Range length_range_;
};

}  // namespace codegen

#endif /* BUILTIN_TYPE_H_ */
