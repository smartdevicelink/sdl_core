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

#ifndef VALIDATED_TYPES_JSON_INL_H_
#define VALIDATED_TYPES_JSON_INL_H_

#include "rpc_base/rpc_base.h"
#include "json/value.h"

namespace rpc {

// static
inline PrimitiveType::ValueState PrimitiveType::InitHelper(
  const Json::Value* value,
  bool (Json::Value::*type_check)() const) {
  if (!value) {
    return kUninitialized;
  } else if ((value->*type_check)()) {
    return kValid;
  } else {
    return kInvalid;
  }
}

inline policy_table_interface_base::PolicyTableType PrimitiveType::GetPolicyTableType() const  {
  return policy_table_type_;
}

inline void PrimitiveType::SetPolicyTableType(policy_table_interface_base::PolicyTableType pt_type) {
  policy_table_type_ = pt_type;
}

// static
inline CompositeType::InitializationState CompositeType::InitHelper(
  const Json::Value* value,
  bool (Json::Value::*type_check)() const) {
  if (!value) {
    return kUninitialized;
  } else if ((value->*type_check)()) {
    // Some value type checks return true when initialized with null
    if (value->isNull()) {
      return kInvalidInitialized;
    } else {
      return kInitialized;
    }
  } else {
    return kInvalidInitialized;
  }
}

inline policy_table_interface_base::PolicyTableType CompositeType::GetPolicyTableType() const {
  return policy_table_type_;
}

inline void CompositeType::SetPolicyTableType(policy_table_interface_base::PolicyTableType pt_type) {
  policy_table_type_ = pt_type;
}

namespace impl {
// static
inline const Json::Value* ValueMember(const Json::Value* value,
                                      const char* member_name) {
  if (value && value->isObject() && value->isMember(member_name)) {
    return &(*value)[member_name];
  }
  return NULL;
}

template<class T>
inline void WriteJsonField(const char* field_name,
                           const T& field,
                           Json::Value* json_value) {
  if (field.is_initialized()) {
    (*json_value)[field_name] = field.ToJsonValue();
  }
}

}  // namespace impl

inline Boolean::Boolean(const Json::Value* value)
  : PrimitiveType(InitHelper(value, &Json::Value::isBool)),
    value_(is_valid() ? value->asBool() : bool()) {
}

inline Boolean::Boolean(const Json::Value* value, bool def_value)
  : PrimitiveType(InitHelper(value, &Json::Value::isBool)),
    value_(is_valid() ? value->asBool() : def_value) {
  // If there is no value, mark it as valid and use def_value
  if (!is_initialized()) {
    value_state_ = kValid;
  }
}

inline Json::Value Boolean::ToJsonValue() const {
  return Json::Value(value_);
}

template<typename T, T minval, T maxval>
Integer<T, minval, maxval>::Integer(const Json::Value* value)
  : PrimitiveType(InitHelper(value, &Json::Value::isInt)),
    value_() {
  if (is_valid()) {
    Json::Value::Int64 intval = value->asInt64();
    if (range_.Includes(intval)) {
      value_ = IntType(intval);
    } else {
      value_state_ = kInvalid;
    }
  }
}

template<typename T, T minval, T maxval>
Integer<T, minval, maxval>::Integer(const Json::Value* value, IntType def_value)
  : PrimitiveType(InitHelper(value, &Json::Value::isInt)),
    value_(def_value) {
  if (!is_initialized()) {
    value_state_ = kValid;
  } else if (is_valid()) {
    Json::Value::Int64 intval = value->asInt64();
    if (range_.Includes(intval)) {
      value_ = IntType(intval);
    } else {
      value_state_ = kInvalid;
    }
  }
}

template<typename T, T minval, T maxval>
Json::Value Integer<T, minval, maxval>::ToJsonValue() const {
  return Json::Value(Json::Int64(value_));
}

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
Float<minnum, maxnum, minden, maxden>::Float(const Json::Value* value)
  : PrimitiveType(InitHelper(value, &Json::Value::isDouble)),
    value_() {
  if (is_valid()) {
    value_ = value->asDouble();
    value_state_ = range_.Includes(value_) ? kValid : kInvalid;
  }
}

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
Float<minnum, maxnum, minden, maxden>::Float(const Json::Value* value,
    double def_value)
  : PrimitiveType(InitHelper(value, &Json::Value::isDouble)),
    value_(def_value) {
  if (!is_initialized()) {
    value_state_ = kValid;
  } else if (is_valid()) {
    value_ = value->asDouble();
    value_state_ = range_.Includes(value_) ? kValid : kInvalid;
  }
}

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
Json::Value Float<minnum, maxnum, minden, maxden>::ToJsonValue() const {
  return Json::Value(value_);
}

template<size_t minlen, size_t maxlen>
String<minlen, maxlen>::String(const Json::Value* value)
  : PrimitiveType(InitHelper(value, &Json::Value::isString)),
    value_(is_valid() ? value->asString() : std::string()) {
  if (is_valid()) {
    value_state_ = length_range_.Includes(value_.length()) ? kValid : kInvalid;
  }
}

template<size_t minlen, size_t maxlen>
String<minlen, maxlen>::String(const Json::Value* value, const std::string& def_value)
  : PrimitiveType(InitHelper(value, &Json::Value::isString)),
    value_(def_value) {
  if (!is_initialized()) {
    value_state_ = kValid;
  } else if (is_valid()) {
    value_state_ = length_range_.Includes(value_.length()) ? kValid : kInvalid;
  }
}

template<size_t minlen, size_t maxlen>
Json::Value String<minlen, maxlen>::ToJsonValue() const {
  return Json::Value(value_);
}

template<typename T>
Enum<T>::Enum(const Json::Value* value)
  : PrimitiveType(InitHelper(value, &Json::Value::isString)),
    value_(EnumType()) {
  if (is_valid()) {
    value_state_ =
      EnumFromJsonString(value->asString(), &value_) ? kValid : kInvalid;
  }
}

template<typename T>
Enum<T>::Enum(const Json::Value* value, EnumType def_value)
  : PrimitiveType(InitHelper(value, &Json::Value::isString)),
    value_(def_value) {
  if (!is_initialized()) {
    value_state_ = kValid;
  } else if (is_valid()) {
    value_state_ =
      EnumFromJsonString(value->asString(), &value_) ? kValid : kInvalid;
  }
}

template<typename T>
Json::Value Enum<T>::ToJsonValue() const {
  return Json::Value(Json::StaticString(EnumToJsonString(value_)));
}

// Non-const version
template<typename T, size_t minsize, size_t maxsize>
Array<T, minsize, maxsize>::Array(Json::Value* value)
  : CompositeType(InitHelper(value, &Json::Value::isArray)) {
  if (value) {
    if (value->isArray()) {
      this->reserve(value->size());
      for (Json::Value::iterator i = value->begin(); i != value->end(); ++i) {
        push_back(&*i);
      }
    } else {
      // Array is empty, empty initialized or uninitialized
      // depending on InitHelper result
    }
  }
}

// Const version, must be identical to the non-const version
template<typename T, size_t minsize, size_t maxsize>
Array<T, minsize, maxsize>::Array(const Json::Value* value)
  : CompositeType(InitHelper(value, &Json::Value::isArray)) {
  if (value) {
    if (value->isArray()) {
      this->reserve(value->size());
      for (Json::Value::const_iterator i = value->begin(); i != value->end(); ++i) {
        push_back(&*i);
      }
    } else {
      // Array is empty, empty initialized or uninitialized
      // depending on InitHelper result
    }
  }
}

template<typename T, size_t minsize, size_t maxsize>
Json::Value Array<T, minsize, maxsize>::ToJsonValue() const {
  Json::Value array(Json::arrayValue);
  array.resize(this->size());
  for (size_t i = 0; i != this->size(); ++i) {
    array[Json::ArrayIndex(i)] = (this->operator [](i)).ToJsonValue();
  }
  return array;
}

// Non-const version
template<typename T, size_t minsize, size_t maxsize>
Map<T, minsize, maxsize>::Map(Json::Value* value)
  : CompositeType(InitHelper(value, &Json::Value::isObject)) {
  if (value) {
    if (value->isObject()) {
      for (Json::Value::iterator i = value->begin(); i != value->end(); ++i) {
        this->insert(typename MapType::value_type(i.key().asString(), T(&*i)));
      }
    } else {
      // Map is empty, empty initialized or uninitialized
      // depending on InitHelper result
    }
  }
}

template<typename T, size_t minsize, size_t maxsize>
Map<T, minsize, maxsize>::Map(const Json::Value* value)
  : CompositeType(InitHelper(value, &Json::Value::isObject)) {
  if (value) {
    if (value->isObject()) {
      for (Json::Value::const_iterator i = value->begin(); i != value->end(); ++i) {
        this->insert(typename MapType::value_type(i.key().asString(), T(&*i)));
      }
    } else {
      // Map is empty, empty initialized or uninitialized
      // depending on InitHelper result
    }
  }
}

template<typename T, size_t minsize, size_t maxsize>
Json::Value Map<T, minsize, maxsize>::ToJsonValue() const {
  Json::Value map(Json::objectValue);
  for (typename MapType::const_iterator i = this->begin(); i != this->end(); ++i) {
    map[i->first] = i->second.ToJsonValue();
  }
  return map;
}

template<typename T>
Nullable<T>::Nullable(const Json::Value* value)
  : T(value),
    marked_null_(value != NULL&&  value->isNull()) {
}

template<typename T>
Nullable<T>::Nullable(Json::Value* value)
  : T(value),
    marked_null_(value != NULL&&  value->isNull()) {
}

template<typename T>
template<typename U>
Nullable<T>::Nullable(const Json::Value* value, const U& def_value)
  : T(value, def_value),
    marked_null_(value != NULL&&  value->isNull()) {
}

template<typename T>
inline Json::Value Nullable<T>::ToJsonValue() const {
  return marked_null_ ? Json::Value::null : T::ToJsonValue();
}

template<typename T>
template<typename U>
Optional<T>::Optional(const Json::Value* value, const U& def_value)
  : value_(value, def_value) {
}

template<typename T>
inline Json::Value Optional<T>::ToJsonValue() const {
  return value_.ToJsonValue();
}

template<typename T>
Stringifyable<T>::Stringifyable(const Json::Value* value)
  : T(NULL != value&&  !value->isString() ? value : NULL),
    predefined_string_(NULL != value&&   value->isString() ? value->asString() : "") {
}

template<typename T>
Stringifyable<T>::Stringifyable(Json::Value* value)
  : T(NULL != value&&  !value->isString() ? value : NULL),
    predefined_string_(NULL != value&&   value->isString() ? value->asString() : "") {
}

template<typename T>
template<typename U>
Stringifyable<T>::Stringifyable(const Json::Value* value, const U& def_value)
  : T(NULL != value&&  !value->isString() ? (value, def_value) : NULL),
    predefined_string_(NULL != value&&   value->isString() ? value->asString() : "")  {
}

template<typename T>
inline Json::Value Stringifyable<T>::ToJsonValue() const {
  return predefined_string_.empty() ? T::ToJsonValue() : predefined_string_;
}

}  // namespace rpc

#endif /* VALIDATED_TYPES_JSON_INL_H_ */
