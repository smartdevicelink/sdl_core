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

#include "cppgen/definition_generator.h"

#include "cppgen/comment.h"
#include "cppgen/enum_from_json_value_function.h"
#include "cppgen/enum_to_json_value_function.h"
#include "cppgen/is_valid_enum_function.h"
#include "cppgen/message_handle_with_method.h"
#include "cppgen/module_manager.h"
#include "cppgen/struct_type_constructor.h"
#include "cppgen/struct_type_from_json_method.h"
#include "cppgen/struct_type_is_initialized_method.h"
#include "cppgen/struct_type_is_valid_method.h"
#include "model/composite_type.h"
#include "utils/safeformat.h"

using std::endl;
using std::ostream;
using typesafe_format::format;

namespace codegen {

DefinitionGenerator::DefinitionGenerator(ModuleManager* module_manager)
    : module_manager_(module_manager) {
}

DefinitionGenerator::~DefinitionGenerator() {
}

void DefinitionGenerator::GenerateCodeForEnum(const Enum* enm) {
  CppFile& source_file = module_manager_->SourceForEnum(*enm);
  ostream& o = source_file.types_ns().os();
  IsValidEnumFunction(enm).Define(&o, false);
  EnumToJsonValueFunction(enm).Define(&o, false);
  EnumFromJsonValueFunction(enm).Define(&o, false);
  o << endl;
}

void DefinitionGenerator::GenerateCodeForStruct(const Struct* strct) {
  CppFile& source_file = module_manager_->SourceForStruct(*strct);
  ostream& o = source_file.types_ns().os();
  o << Comment(format("{0} methods", strct->name())) << endl;
  StructTypeDefaultConstructor(strct).Define(&o, false);
  StructTypeMandatoryConstructor mandatory_constructor(strct);
  if (mandatory_constructor.has_mandatory_parameters()){
    mandatory_constructor.Define(&o, false);
  }
  CppStructDestructor(strct->name()).Define(&o, false);
  StructTypeIsValidMethod(strct).Define(&o, false);
  StructTypeIsInitializedMethod(strct).Define(&o, false);
  StructTypeFromJsonConstructor(strct).Define(&o , false);
  StructTypeToJsonMethod(strct).Define(&o , false);
  o << endl;

  Namespace& val_ns = module_manager_->SourceForValidator().types_ns();
  StructTypeAdditionalValidationMethod(strct).Define(&val_ns.os(), false);
}

void DefinitionGenerator::GenerateCodeForFunction(const Function& function) {
  CppFile& source_file = module_manager_->SourceForFunction(function);
  GenerateCodeForRequest(function.request(), &source_file);
  GenerateCodeForResponse(function.response());
}

void DefinitionGenerator::GenerateCodeForResponse(const Response& response) {
  CppFile& source_file = module_manager_->SourceForResponse(response);
  ostream& o = source_file.responses_ns().os();
  o << Comment(format("{0} response methods", response.name())) << endl;
  StructTypeDefaultConstructor(&response).Define(&o, false);
  StructTypeMandatoryConstructor mandatory_constructor(&response);
  if (mandatory_constructor.has_mandatory_parameters()){
    mandatory_constructor.Define(&o, false);
  }
  CppStructDestructor(response.name()).Define(&o, false);
  StructTypeIsValidMethod(&response).Define(&o, false);
  StructTypeIsInitializedMethod(&response).Define(&o, false);
  StructTypeFromJsonConstructor(&response).Define(&o , false);
  StructTypeToJsonMethod(&response).Define(&o , false);
  MessageHandleWithMethod(response.name()).Define(&o, false);
  o << endl;

  Namespace& val_ns = module_manager_->SourceForValidator().responses_ns();
  StructTypeAdditionalValidationMethod(&response).Define(&val_ns.os(), false);
}

void DefinitionGenerator::GenerateCodeForNotification(
    const Notification& notification) {
  CppFile& source_file = module_manager_->SourceForNotification(notification);
  ostream& o = source_file.notifications_ns().os();
  o << Comment(format("{0} notification methods", notification.name())) << endl;
  StructTypeDefaultConstructor(&notification).Define(&o, false);
  StructTypeMandatoryConstructor mandatory_constructor(&notification);
  if (mandatory_constructor.has_mandatory_parameters()){
    mandatory_constructor.Define(&o, false);
  }
  CppStructDestructor(notification.name()).Define(&o, false);
  StructTypeIsValidMethod(&notification).Define(&o, false);
  StructTypeIsInitializedMethod(&notification).Define(&o, false);
  StructTypeFromJsonConstructor(&notification).Define(&o , false);
  StructTypeToJsonMethod(&notification).Define(&o , false);
  MessageHandleWithMethod(notification.name()).Define(&o, false);
  o << endl;

  Namespace& val_ns = module_manager_->SourceForValidator().notifications_ns();
  StructTypeAdditionalValidationMethod(&notification).Define(&val_ns.os(),
                                                             false);
}

void DefinitionGenerator::GenerateCodeForRequest(const Request& request,
                                                 CppFile* source_file) {
  ostream& o = source_file->requests_ns().os();
  o << Comment(format("{0} request methods", request.name())) << endl;
  StructTypeDefaultConstructor(&request).Define(&o, false);
  StructTypeMandatoryConstructor mandatory_constructor(&request);
  if (mandatory_constructor.has_mandatory_parameters()){
    mandatory_constructor.Define(&o, false);
  }
  CppStructDestructor(request.name()).Define(&o, false);
  StructTypeIsValidMethod(&request).Define(&o, false);
  StructTypeIsInitializedMethod(&request).Define(&o, false);
  StructTypeFromJsonConstructor(&request).Define(&o , false);
  StructTypeToJsonMethod(&request).Define(&o , false);
  MessageHandleWithMethod(request.name()).Define(&o, false);
  o << endl;

  Namespace& val_ns = module_manager_->SourceForValidator().requests_ns();
  StructTypeAdditionalValidationMethod(&request).Define(&val_ns.os(), false);
}

}  // namespace codegen

