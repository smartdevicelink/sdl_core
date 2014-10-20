/* Copyright (c) 2014, Ford Motor Company
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

#include "cppgen/struct_type_dbus_serializer.h"

#include "cppgen/naming_convention.h"
#include "cppgen/type_name_code_generator.h"
#include "model/composite_type.h"
#include "utils/safeformat.h"

using typesafe_format::strmfmt;

namespace codegen {

StructTypeDbusMessageSignatureMethod::StructTypeDbusMessageSignatureMethod(
    const TypePreferences* preferences,
    const Struct* strct,
    bool substructure)
    : CppFunction(strct->name(), "GetDbusSignature", "void", kStatic),
      preferences_(preferences),
      substructure_(substructure),
      strct_(strct) {
  Add(Parameter("signature", "std::string*"));
}

StructTypeDbusMessageSignatureMethod::~StructTypeDbusMessageSignatureMethod() {
}

void StructTypeDbusMessageSignatureMethod::DefineBody(std::ostream* os) const {
  const Struct::FieldsList& fields = strct_->fields();
  if (!fields.empty()) {
    if (substructure_) {
      *os << "(*signature) += DBUS_STRUCT_BEGIN_CHAR;\n";
    }
    for (Struct::FieldsList::const_iterator i = fields.begin(),
         end = fields.end(); i != end; ++i) {
      // Field is considered optional if it has mandatory=false attribute and
      // if it does NOT have default values. Fields that have default values are
      // always available no mater if they present in input or not
      bool field_is_optional = false;
      if (!i->is_mandatory()) {
        if (i->default_value() == NULL) {
          field_is_optional = true;
        }
      }
      std::string field_type = RpcTypeNameGenerator(&strct_->interface(),
                                                    preferences_,
                                                    i->type(),
                                                    field_is_optional).result();
      strmfmt(*os, "rpc::DbusSignature< {0} >({1});\n",
              field_type, parameters_[0].name);

    }
    if (substructure_) {
      *os << "(*signature) += DBUS_STRUCT_END_CHAR;\n";
    }
  }
}

StructTypeFromDbusReaderConstructor::StructTypeFromDbusReaderConstructor(
    const TypePreferences* preferences,
    const Struct* strct,
    bool substructure,
    const std::string& base_class_name)
    : CppStructConstructor(strct->name()),
      preferences_(preferences),
      strct_(strct),
      substructure_(substructure) {
  Add(Parameter("reader__", "dbus::MessageReader*"));
  std::string base_initializer = "reader__";
  if (!strct->frankenstruct()) {
    base_initializer = "InitHelper(true)";
  }
  Add(Initializer(base_class_name, base_initializer));
  // In case of non-substructure use initializer list to initialize fields
  // From MessageReader passed in
  if (!substructure_) {
    const Struct::FieldsList& fields = strct->fields();
    for (Struct::FieldsList::const_iterator i = fields.begin(), end = fields.end();
         i != end; ++i) {
      Add(Initializer(AvoidKeywords(i->name()),
                      "reader__"));
    }
  }
}

StructTypeFromDbusReaderConstructor::~StructTypeFromDbusReaderConstructor() {
}

void StructTypeFromDbusReaderConstructor::DefineBody(std::ostream* os) const {
  const Struct::FieldsList& fields = strct_->fields();
  // If initializing substructure (a structure that is used as
  // a field of other structure) additional structure reading iterator
  // should be created
  if (substructure_ && !fields.empty()) {
    *os << "dbus::MessageReader subreader__ = reader__->TakeStructReader();\n";
    for (Struct::FieldsList::const_iterator i = fields.begin(), end = fields.end();
         i != end; ++i) {
      // Field is considered optional if it has mandatory=false attribute and
      // if it does NOT have default values. Fields that have default values are
      // always available no mater if they present in input or not
      bool field_is_optional = false;
      if (!i->is_mandatory()) {
        if (i->default_value() == NULL) {
          field_is_optional = true;
        }
      }
      std::string field_type = RpcTypeNameGenerator(&strct_->interface(),
                                                    preferences_,
                                                    i->type(),
                                                    field_is_optional).result();
      strmfmt(*os, "{0} = {1}(&subreader__);\n", i->name(), field_type);
    }
  }
}

StructTypeToDbusWriterMethod::StructTypeToDbusWriterMethod(
    const Struct* strct,
    bool substructure)
    : CppFunction(strct->name(), "ToDbusWriter", "void", kConst),
      substructure_(substructure),
      strct_(strct) {
  Add(Parameter("writer__", "dbus::MessageWriter*"));
}

StructTypeToDbusWriterMethod::~StructTypeToDbusWriterMethod() {

}

void StructTypeToDbusWriterMethod::DefineBody(std::ostream* os) const {
  const Struct::FieldsList& fields = strct_->fields();
  if (!fields.empty()) {
    std::string writer_ptr_name = parameters_[0].name;
    if (substructure_) {
      strmfmt(*os, "dbus::MessageWriter subwriter__({0}, dbus::kStruct, NULL);\n",
              writer_ptr_name);
      writer_ptr_name = "&subwriter__";
    }
    for (Struct::FieldsList::const_iterator i = fields.begin(), end = fields.end();
         i != end; ++i) {
      strmfmt(*os, "{0}.ToDbusWriter({1});\n",
              i->name(),
              writer_ptr_name);
    }
  }
}


} // namespace codegen
