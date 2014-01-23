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

#include "cppgen/declaration_generator.h"

#include <ostream>
#include <memory>

#include "cppgen/comment.h"
#include "cppgen/cpp_file.h"
#include "cppgen/enum_from_json_value_function.h"
#include "cppgen/enum_to_json_value_function.h"
#include "cppgen/function_id_method.h"
#include "cppgen/generator_preferences.h"
#include "cppgen/is_valid_enum_function.h"
#include "cppgen/literal_generator.h"
#include "cppgen/module_manager.h"
#include "cppgen/struct_type_constructor.h"
#include "cppgen/struct_type_from_json_method.h"
#include "cppgen/struct_type_is_initialized_method.h"
#include "cppgen/struct_type_is_valid_method.h"
#include "cppgen/type_name_code_generator.h"
#include "model/composite_type.h"
#include "model/constant.h"
#include "model/function.h"
#include "utils/safeformat.h"
#include "utils/string_utils.h"

using std::endl;
using std::ostream;
using std::string;
using typesafe_format::strmfmt;

namespace codegen {

namespace {

class Section {
  struct SectionDeclarer {
    SectionDeclarer(const char* name, ostream* os) {
      *os << " " << name << ":" << endl;
    }
  };
 public:
  Section(const char* name, ostream* os)
      : declarer_(name, os),
        indent_(*os) {
  }
 private:
  SectionDeclarer declarer_;
  Indent indent_;
};

void DeclareStructureBegin(ostream& o, const string& name,
                           const string& base_type, const Comment& comment) {
  o << comment << endl;
  strmfmt(o, "struct {0} : {1} {", name, base_type) << endl;
}

}

DeclarationGenerator::DeclarationGenerator(ModuleManager* module_manager)
    : module_manager_(module_manager) {
}

DeclarationGenerator::~DeclarationGenerator() {
}

void DeclarationGenerator::GenerateCodeForEnum(const Enum* enm) {
  CppFile& header_file = module_manager_->HeaderForEnum(*enm);
  ostream& o = header_file.types_ns().os();
  o << Comment(enm->description()) << endl;
  o << "enum " << enm->name() << " {" << endl;
  const Enum::ConstantsList& constants = enm->constants();
  {
    Indent indent(o);
    for (Enum::ConstantsList::const_iterator i = constants.begin(), end =
        constants.end(); i != end; ++i) {
      const Enum::Constant& enum_constant = *i;
      GenerateCodeForEnumConstant(enum_constant, &header_file, false);
    }
  }
  o << "};" << endl;
  IsValidEnumFunction(enm).Declare(&o, false);
  EnumToJsonValueFunction(enm).Declare(&o, false);
  EnumFromJsonValueFunction(enm).Declare(&o, false);
  o << endl;
}

void DeclarationGenerator::GenerateCodeForStruct(const Struct* strct) {
  CppFile& header_file = module_manager_->HeaderForStruct(*strct);
  header_file.global_namespace().nested("Json").ForwardDeclare(
      Namespace::ForwardDeclaration(Namespace::ForwardDeclaration::kClass, "Value"));
  ostream& o = header_file.types_ns().os();
  DeclareStructureBegin(o, strct->name(), "CompositeType",
                        Comment(strct->description()));
  {
    Section pub("public", &o);
    GenerateCodeForStructFields(strct->fields(), &header_file.types_ns());
  }
  {
    Section pub("public", &o);
    StructTypeDefaultConstructor(strct).Declare(&o, true);
    StructTypeMandatoryConstructor mandatory_constructor(strct);
    StructTypeFromJsonConstructor(strct).Declare(&o , true);
    if (mandatory_constructor.has_mandatory_parameters()) {
      mandatory_constructor.Declare(&o, true);
    }
    CppStructDestructor(strct->name()).Declare(&o, true);
    StructTypeIsValidMethod(strct).Declare(&o, true);
    StructTypeIsInitializedMethod(strct).Declare(&o, true);
    StructTypeToJsonMethod(strct).Declare(&o , true);
  }
  {
    Section priv("private", &o);
    StructTypeAdditionalValidationMethod(strct).Declare(&o, true);

  }
  o << "};" << endl;
}

void DeclarationGenerator::GenerateCodeForEnumConstant(
    const Enum::Constant& enm, CppFile* header_file, bool skip_coma) {
  ostream& o = header_file->types_ns().os();
  o << LiteralGenerator(enm).result();
  if (enm.is_value_explicit()) {
    o << " = " << enm.value();
  }
  if (!skip_coma) {
    o << ",";
  }
  o << " " << Comment(enm.description()) << endl;
}

void DeclarationGenerator::GenerateCodeForStructField(
    const Struct::Field& field, Namespace* name_space) {
  ostream& o = name_space->os();
  o << RpcTypeNameGenerator(
      field.type(),
      field.default_value() || field.is_mandatory())
          .result();
  o << " " << field.name() << ";";
  if (!field.description().empty()) {
    o << " " << Comment(field.description());
  }
  o << endl;
}

void DeclarationGenerator::GenerateCodeForFunction(const Function& function) {
  CppFile& header_file = module_manager_->HeaderForFunction(function);
  header_file.responses_ns().ForwardDeclare(
      Namespace::ForwardDeclaration(Namespace::ForwardDeclaration::kStruct,
                                    function.response().name()));
  GenerateCodeForRequest(function.request(), &header_file);
  GenerateCodeForResponse(function.response());
}

void DeclarationGenerator::GenerateCodeForRequest(const Request& request,
                                                  CppFile* header_file) {
  header_file->global_namespace().nested("Json").ForwardDeclare(
      Namespace::ForwardDeclaration(Namespace::ForwardDeclaration::kClass, "Value"));
  ostream& o = header_file->requests_ns().os();
  DeclareStructureBegin(o, request.name(), "Request",
                        Comment(request.description()));
  {
    Section pub("public", &o);
    strmfmt(o, "typedef {0}::{1} ResponseType;",
            header_file->responses_ns().name(), request.name()) << endl;
    GenerateCodeForStructFields(request.parameters(),
                                &header_file->requests_ns());
  }
  {
    Section pub("public", &o);
    StructTypeDefaultConstructor(&request).Declare(&o, true);
    StructTypeMandatoryConstructor mandatory_constructor(&request);
    StructTypeFromJsonConstructor(&request).Declare(&o , true);
    if (mandatory_constructor.has_mandatory_parameters()) {
      mandatory_constructor.Declare(&o, true);
    }
    CppStructDestructor(request.name()).Declare(&o, true);
    StructTypeIsValidMethod(&request).Declare(&o, true);
    StructTypeIsInitializedMethod(&request).Declare(&o, true);
    StructTypeToJsonMethod(&request).Declare(&o , true);
    FunctionIdMethod(&request).Define(&o, true);
  }
  {
    Section priv("private", &o);
    StructTypeAdditionalValidationMethod(&request).Declare(&o, true);
  }

  o << "};" << endl;
}

void DeclarationGenerator::GenerateCodeForResponse(const Response& response) {
  CppFile& header_file = module_manager_->HeaderForResponse(response);
  header_file.global_namespace().nested("Json").ForwardDeclare(
      Namespace::ForwardDeclaration(Namespace::ForwardDeclaration::kClass, "Value"));
  ostream& o = header_file.responses_ns().os();
  DeclareStructureBegin(o, response.name(), "Response",
                        Comment(response.description()));
  {
    Section pub("public", &o);
    GenerateCodeForStructFields(response.parameters(),
                                &header_file.responses_ns());
  }
  {
    Section pub("public", &o);
    StructTypeDefaultConstructor(&response).Declare(&o, true);
    StructTypeMandatoryConstructor mandatory_constructor(&response);
    StructTypeFromJsonConstructor(&response).Declare(&o, true);
    if (mandatory_constructor.has_mandatory_parameters()) {
      mandatory_constructor.Declare(&o, true);
    }
    CppStructDestructor(response.name()).Declare(&o, true);
    StructTypeIsValidMethod(&response).Declare(&o, true);
    StructTypeIsInitializedMethod(&response).Declare(&o, true);
    StructTypeToJsonMethod(&response).Declare(&o , true);
    FunctionIdMethod(&response).Define(&o, true);
  }
  {
    Section priv("private", &o);
    StructTypeAdditionalValidationMethod(&response).Declare(&o, true);
  }

  o << "};" << endl;
}

void DeclarationGenerator::GenerateCodeForNotification(
    const Notification& notification) {
  CppFile& header_file = module_manager_->HeaderForNotification(notification);
  header_file.global_namespace().nested("Json").ForwardDeclare(
      Namespace::ForwardDeclaration(Namespace::ForwardDeclaration::kClass, "Value"));
  ostream& o = header_file.notifications_ns().os();
  DeclareStructureBegin(o, notification.name(), "Notification",
                        Comment(notification.description()));
  {
    Section pub("public", &o);
    GenerateCodeForStructFields(notification.parameters(),
                                &header_file.notifications_ns());
  }
  {
    Section pub("public", &o);
    StructTypeDefaultConstructor(&notification).Declare(&o, true);
    StructTypeMandatoryConstructor mandatory_constructor(&notification);
    StructTypeFromJsonConstructor(&notification).Declare(&o , true);
    if (mandatory_constructor.has_mandatory_parameters()) {
      mandatory_constructor.Declare(&o, true);
    }
    CppStructDestructor(notification.name()).Declare(&o, true);
    StructTypeIsValidMethod(&notification).Declare(&o, true);
    StructTypeIsInitializedMethod(&notification).Declare(&o, true);
    StructTypeToJsonMethod(&notification).Declare(&o , true);
    FunctionIdMethod(&notification).Define(&o, true);
  }
  {
    Section priv("private", &o);
    StructTypeAdditionalValidationMethod(&notification).Declare(&o, true);
  }

  o << "};" << endl;
}

void DeclarationGenerator::GenerateCodeForStructFields(
    const FunctionMessage::ParametersList& params, Namespace* name_space) {
  for (FunctionMessage::ParametersList::const_iterator i = params.begin(), end =
      params.end(); i != end; ++i) {
    const FunctionMessage::Parameter& param = *i;
    GenerateCodeForStructField(param, name_space);
  }
}

}  // namespace codegen
