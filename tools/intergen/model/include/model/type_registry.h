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

#ifndef TYPE_REGISTRY_H_
#define TYPE_REGISTRY_H_

#include <map>
#include <set>
#include <string>
#include <list>

#include "model/composite_type.h"
#include "model/type.h"
#include "utils/macro.h"
#include "utils/stl_utils.h"

namespace pugi {
class xml_node;
}  // namespace pugi

namespace codegen {
class BuiltinTypeRegistry;

/*
 * Type registry is a container holding definitions of all the structs and
 * enums found in a single interface xml document element.
 */
class TypeRegistry {
 public:
  // Types
  typedef std::list<Enum*> EnumList;
  typedef std::map<std::string, const Enum*> EnumByName;
  typedef std::list<Struct*> StructList;
  typedef std::map<std::string, const Struct*> StructByName;
 public:
  // Methods
  TypeRegistry(BuiltinTypeRegistry* builtin_type_registry);
  ~TypeRegistry();
  // Follows given xml_node collectin all the struct and enum definitons
  // Returns false and prints to cerr if invalid type definition was found
  bool init(const pugi::xml_node& xml);
  // Finds (or creates) Type object corresponding to type name and parameters
  // found in |params|.
  // Returns false if unknown type or invalid params were found
  bool GetType(const pugi::xml_node& params, const Type** type);
  // Get reserved enum that contains function ID's
  // returns NULL if FunctionID is not registered
  const Enum* GetFunctionIDEnum() const;
  // Returns list of all enums keeping order of definitions in xml
  const EnumList& enums() const;
  // Returns list of all structs keeping order of definitions in xml
  const StructList& structs() const;

  static bool IsMandatoryParam(const pugi::xml_node& param);

 private:
  // methods
  bool AddEnums(const pugi::xml_node& xml);
  bool AddStructs(const pugi::xml_node& xml);
  bool AddEnumConstants(Enum* enm, const pugi::xml_node& xml_enum);
  bool AddStructureFields(Struct* strct, const pugi::xml_node& xml_struct);
  // Create container (map or array) |type| object based on xml |params|
  // if |get_array| is true array is created, otherwise map is created
  // Returns false and prints to cerr on error
  bool GetContainer(const pugi::xml_node& params, const Type** type,
                    bool get_array);
  bool GetNonArray(const pugi::xml_node& params, const Type** type);
  bool GetEnum(const std::string& name, const Type** type);
  bool GetStruct(const std::string& name, const Type** type);
  bool IsRegisteredEnum(const std::string& enum_name);
  bool IsRegisteredStruct(const std::string& struct_name);
 private:
  // fields
  BuiltinTypeRegistry* builtin_type_registry_;
  std::set<Array> arrays_;
  std::set<Map> maps_;
  EnumList enums_;
  utils::StdContainerDeleter<EnumList> enums_deleter_;
  EnumByName enum_by_name_;
  StructList structs_;
  utils::StdContainerDeleter<StructList> structs_deleter_;
  StructByName struct_by_name_;
 private:
  DISALLOW_COPY_AND_ASSIGN(TypeRegistry);
};

}  // namespace codegen

#endif /* TYPE_REGISTRY_H_ */
