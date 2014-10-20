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

#include "cppgen/struct_type_constructor.h"

#include <string>

#include "cppgen/literal_generator.h"
#include "cppgen/naming_convention.h"
#include "cppgen/type_name_code_generator.h"
#include "model/composite_type.h"
#include "model/constant.h"

using std::string;

namespace codegen {

StructTypeDefaultConstructor::StructTypeDefaultConstructor(
    const Struct* strct,
    const std::string& base_class_name)
    : CppStructConstructor(strct->name()) {
  if (!strct->frankenstruct()) {
    Add(Initializer(base_class_name, "kUninitialized"));
  }
}

StructTypeDefaultConstructor::~StructTypeDefaultConstructor() {
}

StructTypeMandatoryConstructor::StructTypeMandatoryConstructor(
    const TypePreferences* preferences,
    const Struct* strct,
    const std::string& base_class_name)
    : CppStructConstructor(strct->name()) {
  // Pass kUnitialized to CompositeType constructor
  // there is no actual difference which value to pick
  if (!strct->frankenstruct()) {
    Add(Initializer(base_class_name, "kUninitialized"));
  }
  const Struct::FieldsList& fields = strct->fields();
  for (Struct::FieldsList::const_iterator i = fields.begin(), end =
      fields.end(); i != end; ++i) {
    const Struct::Field& field = *i;
    if (field.default_value() || field.is_mandatory()) {
      Add(Parameter(AvoidKeywords(field.name()),
                    TypeNameGenerator(
                      &strct->interface(),
                      preferences,
                      field.type()).result()));
      Add(Initializer(AvoidKeywords(field.name()),
                      AvoidKeywords(field.name())));
    }
  }
}

StructTypeMandatoryConstructor::~StructTypeMandatoryConstructor() {
}

void StructTypeMandatoryConstructor::DefineBody(std::ostream* os) const {
}

}  // namespace codegen
