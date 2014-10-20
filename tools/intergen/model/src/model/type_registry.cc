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

#include "model/api.h"
#include "model/builtin_type_registry.h"
#include "model/constant.h"
#include "model/interface.h"
#include "model/model_filter.h"
#include "model/scope.h"
#include "pugixml.hpp"
#include "utils/safeformat.h"
#include "utils/string_utils.h"
#include "utils/xml_utils.h"

using typesafe_format::strmfmt;

namespace codegen {

TypeRegistry::TypeRegistry(const Interface* interface,
                           BuiltinTypeRegistry* builtin_type_registry,
                           Enum* function_ids_enum,
                           const ModelFilter* model_filter,
                           bool create_function_id_enum)
    : interface_(interface),
      builtin_type_registry_(builtin_type_registry),
      function_ids_enum_(function_ids_enum),
      model_filter_(model_filter),
      enums_deleter_(&enums_),
      structs_deleter_(&structs_),
      typedefs_deleter_(&typedefs_) {
  assert(builtin_type_registry_);
  assert(function_ids_enum_);
  assert(model_filter_);
}

TypeRegistry::~TypeRegistry() {
}

bool TypeRegistry::init(const pugi::xml_node& xml) {
  if (!AddEnums(xml) || !AddStructsAndTypedefs(xml)) {
    return false;
  }
  return true;
}

bool TypeRegistry::GetCompositeType(const pugi::xml_node& params, const Type** type) {
  bool is_array = params.attribute("array").as_bool(false);
  bool is_map = params.attribute("map").as_bool(false);
  bool is_nullable = params.attribute("nullable").as_bool(false);
  if (is_array && is_map) {
    strmfmt(std::cerr, "Entity {0} has both map and array attributes specified",
            params.attribute("name").as_string(""));
    return false;
  }
  if (is_map) {
    return GetContainer(params, type, false, is_nullable);
  } else if (is_array) {
    return GetContainer(params, type, true, is_nullable);
  } else {
    return GetNonArray(params, type, is_nullable);
  }
}

const Type* TypeRegistry::GetType(const std::string& name) const {
  const Type* type = NULL;
  if (IsRegisteredEnum(name)
      && GetEnum(name, &type)) {
    return type;
  } else if (IsRegisteredStruct(name)
             && GetStruct(name, &type)) {
    return type;
  } else if (IsRegisteredTypedef(name)
             && GetTypedef(name, &type)) {
    return type;
  }
  return NULL;
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

// static
bool TypeRegistry::IsMandatoryParam(const pugi::xml_node& param) {
  return param.attribute("mandatory").as_bool("true");
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
  for (pugi::xml_node i = xml.first_child(); i;
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
  Scope scope = ScopeFromLiteral(xml_enum.attribute("scope").value());
  if (model_filter_->ShouldFilterScope(scope)) {
    return true;
  }
  std::string name = xml_enum.attribute("name").value();
  InternalScope internal_scope = InternalScopeFromLiteral(
      xml_enum.attribute("internal_scope").value());
  Description description = CollectDescription(xml_enum);
  if (IsRegisteredEnum(name)) {
    std::cerr << "Duplicate enum: " << name << std::endl;
    return false;
  }
  Enum* this_enum = NULL;
  // FunctionID enum is already created, avoid creation of duplicate FunctionID enum
  if (name == Enum::kFunctionIdEnumName) {
    this_enum = function_ids_enum_;
  } else {
    enums_.push_back(new Enum(interface_, name, scope, internal_scope, description));
    this_enum = enums_.back();
    enum_by_name_[name] = this_enum;
  }
  if (!AddEnumConstants(this_enum, xml_enum)) {
    return false;
  }
  return true;
}

bool TypeRegistry::AddStruct(const pugi::xml_node& xml_struct) {
  Scope scope = ScopeFromLiteral(xml_struct.attribute("scope").value());
  if (model_filter_->ShouldFilterScope(scope)) {
    return true;
  }
  std::string name = xml_struct.attribute("name").value();
  const Type* frankenmap = NULL;
  if (!xml_struct.attribute("type").empty()) {
    if (!GetContainer(xml_struct,
                      &frankenmap,
                      false,
                      false)) {
      std::cerr << "Invalid frankenstruct: " << name << std::endl;
      return false;
    }
  }
  Description description = CollectDescription(xml_struct);
  if (IsRegisteredStruct(name)) {
    std::cerr << "Duplicate structure: " << name << std::endl;
    return false;
  }
  structs_.push_back(new Struct(interface_, name, frankenmap,
                                scope, description));
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
  if (!GetCompositeType(xml_typedef, &type)) {
    std::cerr<< "While parsing typedef " << name << '\n';
    return false;
  }
  Description description = CollectDescription(xml_typedef);
  if (IsRegisteredTypedef(name)) {
    std::cerr << "Duplicate typedef: " << name << std::endl;
    return false;
  }
  typedefs_.push_back(new Typedef(interface_, name, type, description));
  typedef_by_name_[name] = typedefs_.back();

  return true;
}

bool TypeRegistry::GetContainer(const pugi::xml_node& params, const Type** type,
                                bool get_array, bool container_nullable) {
  const Type* element_type = NULL;
  bool null_values_allowed =
      params.attribute("null_values_allowed").as_bool(false);
  if (GetNonArray(params, &element_type, null_values_allowed)) {
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
      if (container_nullable) {
        return GetNullable(*type, type);
      } else {
        return true;
      }
    } else {
      std::cerr << "Incorrect container size range: " << minsize_str << ", "
                << maxsize_str << std::endl;
    }
  }
  return false;
}

bool TypeRegistry::GetNonArray(const pugi::xml_node& params,
                               const Type** type, bool nullable) {
  bool type_found = false;
  pugi::xml_attribute type_name = params.attribute("type");
  if (type_name) {
    std::string type_name_str = type_name.value();
    BuiltinTypeRegistry::BuiltInType builtin_type = builtin_type_registry_
        ->BuiltInTypeByName(type_name_str);
    if (BuiltinTypeRegistry::kNotABuiltInType != builtin_type) {
      type_found = builtin_type_registry_->GetType(builtin_type, params, type);
    } else if (IsExternalType(type_name_str)) {
      type_found = GetExternalType(type_name_str, type);
    } else if (*type = GetType(type_name_str)) {
      type_found = true;
    } else {
      std::cerr << "Unregistered type: " << type_name_str << std::endl;
      type_found = false;
    }
  } else {
    std::cerr << "Absent type name" << std::endl;
    type_found = false;
  }

  if (type_found && nullable) {
    type_found = GetNullable(*type, type);
  }
  return type_found;
}

bool TypeRegistry::GetNullable(const Type* original_type,
                               const Type** type) {
  std::set<NullableType>::const_iterator inserted =
      nullables_.insert(NullableType(original_type)).first;
  *type = &*inserted;
  return true;
}

bool TypeRegistry::GetEnum(const std::string& name, const Type** type) const {
  EnumByName::const_iterator i = enum_by_name_.find(name);
  if (i != enum_by_name_.end()) {
    *type = i->second;
    return true;
  } else {
    std::cerr << "Unregistered enum " << name << std::endl;
    return false;
  }
}

bool TypeRegistry::GetStruct(const std::string& name, const Type** type) const {
  StructByName::const_iterator i = struct_by_name_.find(name);
  if (i != struct_by_name_.end()) {
    *type = i->second;
    return true;
  } else {
    std::cerr << "Unregistered struct " << name << std::endl;
    return false;
  }
}

bool TypeRegistry::GetTypedef(const std::string& name, const Type** type) const {
  TypedefByName::const_iterator i = typedef_by_name_.find(name);
  if (i != typedef_by_name_.end()) {
    *type = i->second;
    return true;
  } else {
    std::cerr<< "Unregistered typedef " << name << std::endl;
    return false;
  }
}

bool TypeRegistry::GetExternalType(const std::string& full_type_name,
                                   const Type** type) const {
  size_t dotpos = full_type_name.find('.');
  if (dotpos == full_type_name.npos) {
    std::cerr << full_type_name << " is not valid fully qualified type name" << '\n';
    return false;
  }
  std::string interface_name = full_type_name.substr(0, dotpos);
  std::string type_name = full_type_name.substr(dotpos + 1);
  const API& api = interface_->api();
  const Interface* that_interface =
      interface_name == interface_->name() ?
        interface_ : api.InterfaceByName(interface_name);
  if (!that_interface) {
    std::cerr << "Unknown interface " << interface_name << '\n';
    return false;
  }
  const Type* found_type = that_interface->GetNamedType(type_name);
  if (!found_type) {
    strmfmt(std::cerr, "Unknown type {0} in interface {1}",
            type_name, interface_name) << '\n';
    return false;
  }
  *type = found_type;
  return true;
}

bool TypeRegistry::IsRegisteredEnum(const std::string& enum_name) const {
  return enum_by_name_.find(enum_name) != enum_by_name_.end();
}

bool TypeRegistry::IsRegisteredStruct(const std::string& struct_name) const {
  return struct_by_name_.find(struct_name) != struct_by_name_.end();
}

bool TypeRegistry::IsExternalType(const std::string& full_type_name) const {
  return full_type_name.find('.') != full_type_name.npos;
}

bool TypeRegistry::AddEnumConstants(Enum* enm, const pugi::xml_node& xml_enum) {
  for (pugi::xml_node i = xml_enum.child("element"); i;
      i = i.next_sibling("element")) {
    Scope scope = ScopeFromLiteral(i.attribute("scope").value());
    if (model_filter_->ShouldFilterScope(scope)) {
      continue;
    }
    std::string name = i.attribute("name").value();
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
    Scope scope = ScopeFromLiteral(i.attribute("scope").value());
    if (model_filter_->ShouldFilterScope(scope)) {
      continue;
    }
    std::string type_name = i.attribute("type").value();
    const Type* type = NULL;
    if (!GetCompositeType(i, &type)) {
      std::cerr << "While parsing struct " << strct->name()
                << std::endl;
      return false;
    }
    std::string name = i.attribute("name").value();
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

bool TypeRegistry::IsRegisteredTypedef(const std::string& typedef_name) const {
  return typedef_by_name_.find(typedef_name) != typedef_by_name_.end();
}

}  // namespace codegen

