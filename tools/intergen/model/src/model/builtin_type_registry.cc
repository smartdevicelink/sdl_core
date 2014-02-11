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

#include "model/builtin_type_registry.h"

#include <stdint.h>
#include <iostream>
#include <map>
#include <utility>

#include "pugixml.hpp"
#include "model/constant.h"
#include "utils/string_utils.h"

namespace codegen {

BuiltinTypeRegistry::BuiltinTypeRegistry() {
}

BuiltinTypeRegistry::~BuiltinTypeRegistry() {
}

const Boolean* BuiltinTypeRegistry::GetBoolean() {
  return &boolean_;
}

const Integer* BuiltinTypeRegistry::GetInteger(const Integer::Range& range) {
  Integer integer(range);
  return &*integers_.insert(integer).first;
}

const Float* BuiltinTypeRegistry::GetFloat(const Float::Range& range) {
  Float flt(range);
  return &*floats_.insert(flt).first;
}

const String* BuiltinTypeRegistry::GetString(const String::Range& length_range) {
  String string(length_range);
  return &*strings_.insert(string).first;
}

BuiltinTypeRegistry::BuiltInType BuiltinTypeRegistry::BuiltInTypeByName(
    const std::string& type_name) {
  if (type_name == "Boolean") {
    return kBoolean;
  } else if (type_name == "Integer") {
    return kInteger;
  } else if (type_name == "String") {
    return kString;
  } else if (type_name == "Float") {
    return kFloat;
  } else {
    return kNotABuiltInType;
  }
}

bool BuiltinTypeRegistry::GetType(BuiltInType type_id,
                                  const pugi::xml_node& params,
                                  const Type** type) {
  switch (type_id) {
    case kBoolean: {
      *type = GetBoolean();
      return true;
    }
    case kString: {
      std::string min_length_str = params.attribute("minlength").as_string("1");
      std::string max_length_str = params.attribute("maxlength").as_string("0");
      int64_t min_length, max_length;
      if (StringToNumber(min_length_str, &min_length)
          && StringToNumber(max_length_str, &max_length)) {
        *type = GetString(String::Range(min_length, max_length));
        return true;
      } else {
        std::cerr << "Incorrect length value provided for string: "
                  << max_length_str << std::endl;
        *type = NULL;
        return false;
      }
    }
    case kInteger: {
      std::string min_val_str = params.attribute("minvalue").as_string("0"),
          max_val_str = params.attribute("maxvalue").as_string("0");
      int64_t min_val, max_val;
      if (StringToNumber(min_val_str, &min_val) && StringToNumber(max_val_str, &max_val)) {
        *type = GetInteger(Integer::Range(min_val, max_val));
        return true;
      } else {
        std::cerr << "Incorrect range for integer provided: " << min_val_str
                  << ", " << max_val_str << std::endl;
        return false;
      }
    }
    case kFloat: {
      std::string min_val_str = params.attribute("minvalue").as_string("0"),
          max_val_str = params.attribute("maxvalue").as_string("0");
      double min_val, max_val;
      if (StringToNumber(min_val_str, &min_val) && StringToNumber(max_val_str, &max_val)) {
        *type = GetFloat(Float::Range(min_val, max_val));
        return true;
      } else {
        std::cerr << "Incorrect range for float provided: " << min_val_str
                  << ", " << max_val_str << std::endl;
        return false;
      }
    }
    default: {
      std::cerr << "Unknown built-in type";
      *type = NULL;
      return false;
    }
  }
}

}  // namespace codegen

