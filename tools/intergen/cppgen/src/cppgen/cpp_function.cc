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

#include "cppgen/cpp_function.h"

#include <cassert>
#include <ostream>

#include "utils/safeformat.h"
#include "utils/string_utils.h"

using std::endl;
using std::ostream;
using std::string;
using std::vector;
using typesafe_format::strmfmt;

namespace codegen {

CppFunction::CppFunction(const string& class_name, const string& name,
                         const string& return_type_name, int qualifiers)
    : qualifiers_(qualifiers),
      class_name_(class_name),
      name_(name),
      return_type_name_(return_type_name) {
  assert(!((qualifiers & kVirtual) && (qualifiers & kStatic)));
}

CppFunction::~CppFunction() {
}

CppFunction::Parameter::Parameter(const string& name, const string& type_name)
    : name(name),
      type_name(type_name) {
}

CppFunction::Parameter::~Parameter() {
}

CppFunction::OptionalParameter::OptionalParameter(
    const std::string& name, const std::string& type_name,
    const std::string& default_value)
    : Parameter(name, type_name),
      default_value(default_value) {
}

CppFunction::OptionalParameter::~OptionalParameter() {
}

codegen::CppFunction::Initializer::Initializer(const std::string& field_name,
                                               const std::string& initializer)
    : field_name(field_name),
      initializer(initializer) {
}

codegen::CppFunction::Initializer::~Initializer() {
}

void CppFunction::Add(const Parameter& parameter) {
  parameters_.push_back(parameter);
}

void CppFunction::Add(const OptionalParameter& parameter) {
  optional_params_.push_back(parameter);
}

void CppFunction::Declare(ostream* os, bool in_class) const {
  WriteFunctionPrototype(os, in_class, true);
  if (qualifiers_ & kAbstract) {
    *os << " = 0";
  }
  *os << ";" << endl;
}

void CppFunction::Define(ostream* os, bool in_class) const {
  // No definitions for abstract functions
  if (qualifiers_ & kAbstract)
    return;
  WriteFunctionPrototype(os, in_class, false);
  if (!initializers_.empty()) {
    *os << endl;
    WriteInitializerList(os);
  }
  *os << " {" << endl;
  {
    Indent indent(*os);
    DefineBody(os);
  }
  *os << "}" << endl;
}

void CppFunction::Add(const Initializer& initializer) {
  initializers_.push_back(initializer);
}

bool CppFunction::has_mandatory_parameters() const {
  return !parameters_.empty();
}

void CppFunction::WriteInitializerList(std::ostream* os) const {
  bool first = true;
  Indent indent1(*os), indent2(*os);
  for (std::vector<Initializer>::const_iterator i = initializers_.begin(), end =
      initializers_.end(); i != end; ++i) {
    bool last = i == end - 1;
    *os << (first ? ": " : "  ");
    strmfmt(*os, "{0}({1})", i->field_name, i->initializer);
    if (!last) {
      *os << "," << endl;
    }
    first = false;
  }
}

void CppFunction::DefineBody(std::ostream *os) const {
}

void CppFunction::WriteFunctionPrototype(ostream* os, bool in_class,
                                         bool default_values) const {
  if (in_class && (qualifiers_ & kExplicit)) {
    if (parameters_.size() == 1) {
      *os << "explicit ";
    }
  } else if (in_class && (qualifiers_ & kStatic)) {
    *os << "static ";
  } else if (in_class && (qualifiers_ & kVirtual)) {
    *os << "virtual ";
  }
  if (!return_type_name_.empty()) {
    *os << return_type_name_ << " ";
  }
  if (!in_class && !class_name_.empty()) {
    *os << class_name_ << "::";
  }
  *os << name_ << "(";
  bool separate = false;
  for (vector<Parameter>::const_iterator i = parameters_.begin();
      i != parameters_.end(); ++i) {
    if (separate) {
      *os << ", ";
    }
    *os << i->type_name << " " << i->name;
    separate = true;
  }
  for (vector<OptionalParameter>::const_iterator i = optional_params_.begin();
      i != optional_params_.end(); ++i) {
    if (separate) {
      *os << ", ";
    }
    *os << i->type_name << " " << i->name;
    if (default_values) {
      *os << " = " << i->default_value;
    }
    separate = true;
  }
  *os << ")";
  if (qualifiers_ & kConst) {
    *os << " const";
  }
  if (qualifiers_ & kVolatile) {
    *os << " volatile";
  }
}

CppStructConstructor::CppStructConstructor(const std::string& type_name)
    : CppFunction(type_name, type_name, "", kExplicit) {
}

CppStructConstructor::~CppStructConstructor() {
}

CppStructDestructor::CppStructDestructor(const std::string& type_name,
                                         bool abstract)
    : CppFunction(type_name, "~" + type_name, "", abstract ? kAbstract : 0) {
}

CppStructDestructor::~CppStructDestructor() {
}

}  // namespace codegen
