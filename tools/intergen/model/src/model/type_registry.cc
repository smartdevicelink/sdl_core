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

#include "model/type_registry.h"

#include <iostream>
#include <utility>

#include "model/builtin_type_registry.h"
#include "model/constant.h"
#include "model/scope.h"
#include "pugixml.hpp"
#include "utils/safeformat.h"
#include "utils/string_utils.h"
#include "utils/xml_utils.h"

using typesafe_format::strmfmt;

namespace {
const char* kFunctionIdEnumName = "FunctionID";
}

namespace codegen {

TypeRegistry::TypeRegistry(BuiltinTypeRegistry* builtin_type_registry)
    : builtin_type_registry_(builtin_type_registry),
      enums_deleter_(&enums_),
      structs_deleter_(&structs_),
      typedefs_deleter_(&typedefs_) {
}

bool TypeRegistry::init(const pugi::xml_node& xml) {
  if (!AddEnums(xml) || !AddStructsAndTypedefs(xml))
    return false;
  return true;
}

bool TypeRegistry::GetType(const pugi::xml_node& params, const Type** type) {
  pugi::xml_attribute array = params.attribute("array");
  pugi::xml_attribute map = params.attribute("map");
  if (array && map) {
    strmfmt(std::cerr, "Entity {0} has both map and array attributes specified",
            params.attribute("name").as_string(""));
    return false;
  }
  if (map && map.as_bool(false)) {
    return GetContainer(params, type, false);
  } else if (array && array.as_bool(false)) {
    return GetContainer(params, type, true);
  } else {
    return GetNonArray(params, type);
  }
}

const TypeRegistry::EnumList& TypeRegistry::enums() const {
  return enums_;
}

const TypeRegistry::StructList& TypeRegistry::structs() const {
  return structs_;
}

const TypeRegistry::TypedefList& TypeRegistry::typedefs() const {
  return typedefs_;
}

const Enum* TypeRegistry::GetFunctionIDEnum() const {
  EnumByName::const_iterator res = enum_by_name_.find(kFunctionIdEnumName);
  if (res != enum_by_name_.end()) {
    return res->second;
  } else {
    std::cerr << "FunctionID enum is not defined" << std::endl;
    return NULL;
  }
}

TypeRegistry::~TypeRegistry() {
}

// static
bool TypeRegistry::IsMandatoryParam(const pugi::xml_node& param) {
  bool mandatory = param.attribute("mandatory").as_bool("true");
  if (param.attribute("array").as_bool(false)) {
    mandatory = param.attribute("minsize").as_int(0) > 0;
  }
  return mandatory;
}

bool TypeRegistry::AddEnums(const pugi::xml_node& xml) {
  for (pugi::xml_node i = xml.child("enum"); i; i = i.next_sibling("enum")) {
    if (!AddEnum(i)) {
      return false;
    }
  }
  return true;
}

bool TypeRegistry::AddStructsAndTypedefs(const pugi::xml_node& xml) {
  for (pugi::xml_node i = xml.child("struct"); i;
      i = i.next_sibling()) {
    if (std::string("struct") == i.name()) {
      if (!AddStruct(i)) {
        return false;
      }
    } else if (std::string("typedef") == i.name()) {
      if (!AddTypedef(i)) {
        return false;
      }
    }
  }
  return true;
}

bool TypeRegistry::AddEnum(const pugi::xml_node& xml_enum) {
  std::string name = xml_enum.attribute("name").value();
  Scope scope = ScopeFromLiteral(xml_enum.attribute("scope").value());
  InternalScope internal_scope = InternalScopeFromLiteral(
      xml_enum.attribute("internal_scope").value());
  Description description = CollectDescription(xml_enum);
  if (IsRegisteredEnum(name)) {
    std::cerr << "Duplicate enum: " << name << std::endl;
    return false;
  }
  enums_.push_back(new Enum(name, scope, internal_scope, description));
  enum_by_name_[name] = enums_.back();
  if (!AddEnumConstants(enums_.back(), xml_enum)) {
    return false;
  }
  return true;
}

bool TypeRegistry::AddStruct(const pugi::xml_node& xml_struct) {
  std::string name = xml_struct.attribute("name").value();
  Scope scope = ScopeFromLiteral(xml_struct.attribute("scope").value());
  Description description = CollectDescription(xml_struct);
  if (IsRegisteredStruct(name)) {
    std::cerr << "Duplicate structure: " << name << std::endl;
    return false;
  }
  structs_.push_back(new Struct(name, scope, description));
  struct_by_name_[name] = structs_.back();
  if (!AddStructureFields(structs_.back(), xml_struct)) {
    return false;
  }
  return true;
}

bool TypeRegistry::AddTypedef(const pugi::xml_node& xml_typedef) {
  std::string name = xml_typedef.attribute("name").value();
  if (name.empty()) {
    std::cerr << "Typedef with empty name found" << std::endl;
    return false;
  }
  const Type* type = NULL;
  if (!GetType(xml_typedef, &type)) {
    return false;
  }
  Description description = CollectDescription(xml_typedef);
  if (IsRegisteredTypedef(name)) {
    std::cerr << "Duplicate typedef: " << name << std::endl;
    return false;
  }
  typedefs_.push_back(new Typedef(name, type, description));
  typedef_by_name_[name] = typedefs_.back();

  return true;
}

bool TypeRegistry::GetContainer(const pugi::xml_node& params, const Type** type,
                                bool get_array) {
  const Type* element_type = NULL;
  if (GetNonArray(params, &element_type)) {
    assert(element_type);
    std::string minsize_str = params.attribute("minsize").as_string("0");
    std::string maxsize_str = params.attribute("maxsize").as_string("0");
    int64_t minsize, maxsize;
    if (StringToNumber(minsize_str, &minsize)
        && StringToNumber(maxsize_str, &maxsize)) {
      if (get_array) {
        *type = &*arrays_.insert(
            Array(element_type, Array::Range(minsize, maxsize))).first;
      } else {
        *type = &*maps_.insert(Map(element_type, Map::Range(minsize, maxsize)))
            .first;
      }
      return true;
    } else {
      std::cerr << "Incorrect array size range: " << minsize_str << ", "
                << maxsize_str << std::endl;
    }
  }
  return false;
}

bool TypeRegistry::GetNonArray(const pugi::xml_node& params,
                               const Type** type) {
  pugi::xml_attribute type_name = params.attribute("type");
  if (type_name) {
    std::string type_name_str = type_name.value();
    BuiltinTypeRegistry::BuiltInType builtin_type = builtin_type_registry_
        ->BuiltInTypeByName(type_name_str);
    if (BuiltinTypeRegistry::kNotABuiltInType != builtin_type) {
      return builtin_type_registry_->GetType(builtin_type, params, type);
    } else if (IsRegisteredEnum(type_name_str)) {
      return GetEnum(type_name_str, type);
    } else if (IsRegisteredStruct(type_name_str)) {
      return GetStruct(type_name_str, type);
    } else if (IsRegisteredTypedef(type_name_str)){
      return GetTypedef(type_name_str, type);
    } else {
      std::cerr << "Unregistered type: " << type_name_str << std::endl;
      return false;
    }
  } else {
    std::cerr << "Absent type name" << std::endl;
    return false;
  }
}

bool TypeRegistry::GetEnum(const std::string& name, const Type** type) {
  EnumByName::iterator i = enum_by_name_.find(name);
  if (i != enum_by_name_.end()) {
    *type = i->second;
    return true;
  } else {
    std::cerr << "Unregistered enum " << name << std::endl;
    return false;
  }
}

bool TypeRegistry::GetStruct(const std::string& name, const Type** type) {
  StructByName::iterator i = struct_by_name_.find(name);
  if (i != struct_by_name_.end()) {
    *type = i->second;
    return true;
  } else {
    std::cerr << "Unregistered struct " << name << std::endl;
    return false;
  }
}

bool TypeRegistry::GetTypedef(const std::string& name, const Type** type) {
  TypedefByName::const_iterator i = typedef_by_name_.find(name);
  if (i != typedef_by_name_.end()) {
    *type = i->second;
    return true;
  } else {
    std::cerr<< "Unregistered typedef " << name << std::endl;
    return false;
  }
}

bool TypeRegistry::IsRegisteredEnum(const std::string& enum_name) {
  return enum_by_name_.find(enum_name) != enum_by_name_.end();
}

bool TypeRegistry::IsRegisteredStruct(const std::string& struct_name) {
  return struct_by_name_.find(struct_name) != struct_by_name_.end();
}

bool TypeRegistry::AddEnumConstants(Enum* enm, const pugi::xml_node& xml_enum) {
  for (pugi::xml_node i = xml_enum.child("element"); i;
      i = i.next_sibling("element")) {
    std::string name = i.attribute("name").value();
    Scope scope = ScopeFromLiteral(i.attribute("scope").value());
    std::string internal_name = i.attribute("internal_name").value();
    Description description = CollectDescription(i);
    std::string design_description = i.child_value("designdescription");
    pugi::xml_attribute xml_value = i.attribute("value");
    if (xml_value) {
      int64_t val;
      if (!StringToNumber(xml_value.value(), &val)) {
        std::cerr << "Non-numeric value of enum constant " << name << std::endl;
      }
      if (!enm->AddConstant(name, val, scope, internal_name, description,
                            design_description)) {
        return false;
      }
    } else {
      if (!enm->AddConstant(name, scope, internal_name, description,
                            design_description)) {
        return false;
      }
    }
  }
  return true;
}

bool TypeRegistry::AddStructureFields(Struct* strct,
                                      const pugi::xml_node& xml_struct) {
  for (pugi::xml_node i = xml_struct.child("param"); i;
      i = i.next_sibling("param")) {
    std::string type_name = i.attribute("type").value();
    const Type* type = NULL;
    if (!GetType(i, &type)) {
      std::cerr << "Failed to get field type for struct " << strct->name()
                << std::endl;
      return false;
    }
    std::string name = i.attribute("name").value();
    Scope scope = ScopeFromLiteral(i.attribute("scope").value());
    std::string defvalue = i.attribute("defvalue").value();
    bool mandatory = IsMandatoryParam(i);
    const Constant* defvalue_constant = NULL;
    if (!defvalue.empty()) {
      const ConstantsCreator* const_creator = type->SupportsConstants();
      if (!const_creator) {
        std::cerr << "Default value can not be provided for " << type_name
                  << std::endl;
        return false;
      } else {
        defvalue_constant = const_creator->ConstantFor(defvalue);
      }
    }
    Description description = CollectDescription(i);
    Platform platform = PlatformFromLiteral(i.attribute("platform").value());
    strct->AddField(type, name, mandatory, scope, defvalue_constant,
                    description, platform);
  }
  return true;
}

bool TypeRegistry::IsRegisteredTypedef(const std::string& typedef_name) {
  return typedef_by_name_.find(typedef_name) != typedef_by_name_.end();
}

}  // namespace codegen

