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

#ifndef BUILTIN_TYPE_REGISTRY_H_
#define BUILTIN_TYPE_REGISTRY_H_

#include <stdint.h>
#include <set>
#include <string>

#include "model/builtin_type.h"
#include "model/type.h"

namespace pugi {
class xml_node;
}  // namespace pugi

namespace codegen {

/*
 * Factory and owner of all built-in types graph.
 * Types have restrictions which are considered to be part of type
 * (not part of struct field / function parameter definition)
 */
class BuiltinTypeRegistry {
 public:
  // List of all built-in types managed by this type registry
  enum BuiltInType {
    kBoolean,
    kString,
    kInteger,
    kFloat,
    kNotABuiltInType
  };
  BuiltinTypeRegistry();
  ~BuiltinTypeRegistry();
  // Checks whether |type_name| is a built-in type name and returns
  // appropriate BuiltInType value.
  // Returns kNotABuiltInType if |type_name| contains unknown type name.
  BuiltInType BuiltInTypeByName(const std::string& type_name);
  // Creates type object based on it's |type_id| and type-dependent
  // xml |params| and stores it into |type|.
  // If |params| doesn't contain all required parameters,
  // |type| is left untouched and false is returned.
  bool GetType(BuiltInType type_id, const pugi::xml_node& params,
               const Type** type);
 private:
  // Caching factories for supported types
  const Boolean* GetBoolean();
  const Integer* GetInteger(const Integer::Range& range);
  const Float* GetFloat(const Float::Range& range);
  const String* GetString(const String::Range& length_range);
 private:
  Boolean boolean_;
  std::set<Integer> integers_;
  std::set<Float> floats_;
  std::set<String> strings_;
};

}  // namespace codegen

#endif /* BUILTIN_TYPE_REGISTRY_H_ */
