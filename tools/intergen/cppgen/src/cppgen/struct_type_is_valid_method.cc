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

#include "cppgen/struct_type_is_valid_method.h"

#include <ostream>

#include "cppgen/naming_convention.h"
#include "cppgen/generator_preferences.h"
#include "model/composite_type.h"
#include "utils/safeformat.h"

using std::endl;
using typesafe_format::strmfmt;

namespace codegen {

StructTypeIsValidMethod::StructTypeIsValidMethod(const Struct* strct)
    : CppFunction(strct->name(), "is_valid", "bool", kConst),
      strct_(strct) {
}

StructTypeIsValidMethod::~StructTypeIsValidMethod() {
}

void StructTypeIsValidMethod::DefineBody(std::ostream* os) const {
  if (strct_->frankenstruct()) {
    strmfmt(*os, "if (!Frankenbase::{0}()) return false;\n",
            name_);
  }
  const Struct::FieldsList& fields = strct_->fields();
  bool struct_can_be_valid_empty = true;
  for (size_t i = 0; i != fields.size(); ++i) {
    if (fields[i].is_mandatory()) {
      struct_can_be_valid_empty = false;
    }
  }
  if (struct_can_be_valid_empty) {
    *os << "if (struct_empty()) return initialization_state__ == kInitialized;\n";
  }
  for (size_t i = 0; i != fields.size(); ++i) {
    strmfmt(*os, "if (!{0}.is_valid()) return false;\n",
            AvoidKeywords(fields[i].name()));
  }
  *os << "return "<< func_names::kAdditionalValidation << "();\n";
}

StructTypeAdditionalValidationMethod::StructTypeAdditionalValidationMethod(
    const Struct* strct)
    : CppFunction(strct->name(), func_names::kAdditionalValidation, "bool",
                  kConst),
      strct_(strct) {
}

StructTypeAdditionalValidationMethod::~StructTypeAdditionalValidationMethod() {
}

void StructTypeAdditionalValidationMethod::DefineBody(std::ostream* os) const {
  *os << "return true;" << endl;
}

}
// namespace codegen
