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

#include "cppgen/struct_type_from_json_method.h"


#include "cppgen/literal_generator.h"
#include "cppgen/naming_convention.h"
#include "model/composite_type.h"
#include "utils/safeformat.h"

using typesafe_format::strmfmt;
using typesafe_format::format;

namespace codegen {

StructTypeFromJsonConstructor::StructTypeFromJsonConstructor(
    const Struct* strct,
    const std::string& base_class_name)
    : CppStructConstructor(strct->name()),
      strct_(strct) {
  Add(Parameter("value__", "const Json::Value*"));
  std::string base_initializer = parameters_[0].name;
  if (!strct->frankenstruct()) {
    base_initializer =
      format("InitHelper({0}, &Json::Value::isObject)", parameters_[0].name);
  }
  Add(Initializer(base_class_name, base_initializer));
  const Struct::FieldsList& fields = strct_->fields();
  for (Struct::FieldsList::const_iterator i = fields.begin(), end =
      fields.end(); i != end; ++i) {
    std::string initializer =
        format("impl::ValueMember({0}, \"{1}\")",
               parameters_[0].name,
               i->name());
    if (i->default_value()) {
      std::string def_value = LiteralGenerator(*i->default_value()).result();
      initializer += (", " + def_value);
    }
    Add(Initializer(AvoidKeywords(i->name()), initializer));
  }
}

StructTypeFromJsonConstructor::~StructTypeFromJsonConstructor() {
}

void StructTypeFromJsonConstructor::DefineBody(std::ostream* os) const {
  if (strct_->frankenstruct()) {
    for (Struct::FieldsList::const_iterator i = strct_->fields().begin(),
         end = strct_->fields().end(); i != end; ++i) {
      const Struct::Field& field = *i;
      strmfmt(*os, "erase(\"{0}\");\n",
              field.name());
    }
  }
}

StructTypeToJsonMethod::StructTypeToJsonMethod(const Struct* strct)
    : CppFunction(strct->name(), "ToJsonValue", "Json::Value", kConst),
      strct_(strct) {
}

StructTypeToJsonMethod::~StructTypeToJsonMethod() {
}

void StructTypeToJsonMethod::DefineBody(std::ostream* os) const {
  if (strct_->frankenstruct()) {
    strmfmt(*os, "Json::Value result__(Frankenbase::{0}());\n",
            name_);
  } else {
    *os << "Json::Value result__(Json::objectValue);" << '\n';
  }
  const Struct::FieldsList& fields = strct_->fields();
  for (Struct::FieldsList::const_iterator i = fields.begin(), end =
      fields.end(); i != end; ++i) {
    const Struct::Field& field = *i;
    strmfmt(*os, "impl::WriteJsonField(\"{0}\", {1}, &result__);\n",
            field.name(), AvoidKeywords(field.name()));
  }
  *os << "return result__;" << '\n';
}

}  // namespace codegen
