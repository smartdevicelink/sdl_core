/*
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
#include "utils/json_utils.h"

namespace rpc {

// static
inline PrimitiveType::ValueState PrimitiveType::InitHelper(
    const utils::json::JsonValueRef& value,
    bool (utils::json::JsonValueRef::*type_check)() const) {
  if (!value.IsValid()) {
    return kUninitialized;
  } else if ((value.*type_check)()) {
    return kValid;
  }
  return kInvalid;
}

inline policy_table_interface_base::PolicyTableType
PrimitiveType::GetPolicyTableType() const {
  return policy_table_type_;
}

inline void PrimitiveType::SetPolicyTableType(
    policy_table_interface_base::PolicyTableType pt_type) {
  policy_table_type_ = pt_type;
}

// static
inline CompositeType::InitializationState CompositeType::InitHelper(
    const utils::json::JsonValueRef& value,
    bool (utils::json::JsonValueRef::*type_check)() const) {
  if (!value.IsValid()) {
    return kUninitialized;
  } else if ((value.*type_check)()) {
    // Some value type checks return true when initialized with null
    if (value.IsNull()) {
      return kInvalidInitialized;
    } else {
      return kInitialized;
    }
  }
  return kInvalidInitialized;
}

inline policy_table_interface_base::PolicyTableType
CompositeType::GetPolicyTableType() const {
  return policy_table_type_;
}

inline void CompositeType::SetPolicyTableType(
    policy_table_interface_base::PolicyTableType pt_type) {
  policy_table_type_ = pt_type;
}

namespace impl {
// static
inline const utils::json::JsonValueRef ValueMember(
    const utils::json::JsonValueRef value, const char* member_name) {
  if (value.IsValid() && value.IsObject() && value.HasMember(member_name)) {
    return value[member_name];
  }
  return utils::json::JsonValueRef();
}

template <class T>
inline void WriteJsonField(const char* field_name,
                           const T& field,
                           utils::json::JsonValueRef json_value) {
  if (field.is_initialized()) {
    json_value[field_name] = field.ToJsonValue();
  }
}

}  // namespace impl

inline Boolean::Boolean(const utils::json::JsonValueRef& value)
    : PrimitiveType(InitHelper(value, &utils::json::JsonValueRef::IsBool))
    , value_(is_valid() ? value.AsBool() : bool()) {}

inline Boolean::Boolean(const utils::json::JsonValueRef& value, bool def_value)
    : PrimitiveType(InitHelper(value, &utils::json::JsonValueRef::IsBool))
    , value_(is_valid() ? value.AsBool() : def_value) {
  // If there is no value, mark it as valid and use def_value
  if (!is_initialized()) {
    value_state_ = kValid;
  }
}

inline utils::json::JsonValue Boolean::ToJsonValue() const {
  return utils::json::JsonValue(value_);
}

template <typename T, T minval, T maxval>
Integer<T, minval, maxval>::Integer(const utils::json::JsonValueRef& value)
    : PrimitiveType(InitHelper(value, &utils::json::JsonValueRef::IsInt))
    , value_() {
  if (is_valid()) {
    utils::json::JsonValue::Int intval = value.AsInt();
    if (range_.Includes(intval)) {
      value_ = IntType(intval);
    } else {
      value_state_ = kInvalid;
    }
  }
}

template <typename T, T minval, T maxval>
Integer<T, minval, maxval>::Integer(const Integer& val)
    : PrimitiveType(range_.Includes(val.value_) ? kValid : kInvalid)
    , value_(val.value_) {}

template <typename T, T minval, T maxval>
Integer<T, minval, maxval>::Integer(const utils::json::JsonValueRef& value,
                                    IntType def_value)
    : PrimitiveType(InitHelper(value, &utils::json::JsonValueRef::IsInt))
    , value_(def_value) {
  if (!is_initialized()) {
    value_state_ = kValid;
  } else if (is_valid()) {
    utils::json::JsonValue::Int intval = value.AsInt();
    if (range_.Includes(intval)) {
      value_ = IntType(intval);
    } else {
      value_state_ = kInvalid;
    }
  }
}

template <typename T, T minval, T maxval>
utils::json::JsonValue Integer<T, minval, maxval>::ToJsonValue() const {
  return utils::json::JsonValue(utils::json::JsonValue::Int(value_));
}

template <int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
Float<minnum, maxnum, minden, maxden>::Float(
    const utils::json::JsonValueRef& value)
    : PrimitiveType(InitHelper(value, &utils::json::JsonValueRef::IsDouble))
    , value_() {
  if (is_valid()) {
    value_ = value.AsDouble();
    value_state_ = range_.Includes(value_) ? kValid : kInvalid;
  }
}

template <int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
Float<minnum, maxnum, minden, maxden>::Float(
    const utils::json::JsonValueRef& value, double def_value)
    : PrimitiveType(InitHelper(value, &utils::json::JsonValue::IsDouble))
    , value_(def_value) {
  if (!is_initialized()) {
    value_state_ = kValid;
  } else if (is_valid()) {
    value_ = value.AsDouble();
    value_state_ = range_.Includes(value_) ? kValid : kInvalid;
  }
}

template <int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
utils::json::JsonValue Float<minnum, maxnum, minden, maxden>::ToJsonValue()
    const {
  return utils::json::JsonValue(value_);
}

template <size_t minlen, size_t maxlen>
String<minlen, maxlen>::String(const utils::json::JsonValueRef& value)
    : PrimitiveType(InitHelper(value, &utils::json::JsonValueRef::IsString))
    , value_(is_valid() ? value.AsString() : std::string()) {
  if (is_valid()) {
    value_state_ = length_range_.Includes(value_.length()) ? kValid : kInvalid;
  }
}

template <size_t minlen, size_t maxlen>
String<minlen, maxlen>::String(const utils::json::JsonValueRef& value,
                               const std::string& def_value)
    : PrimitiveType(InitHelper(value, &utils::json::JsonValueRef::IsString))
    , value_(def_value) {
  if (!is_initialized()) {
    value_state_ = kValid;
  } else if (is_valid()) {
    value_state_ = length_range_.Includes(value_.length()) ? kValid : kInvalid;
  }
}

template <size_t minlen, size_t maxlen>
utils::json::JsonValue String<minlen, maxlen>::ToJsonValue() const {
  return utils::json::JsonValue(value_);
}

template <typename T>
Enum<T>::Enum(const utils::json::JsonValueRef& value)
    : PrimitiveType(InitHelper(value, &utils::json::JsonValueRef::IsString))
    , value_(EnumType()) {
  if (is_valid()) {
    value_state_ =
        EnumFromJsonString(value.AsString(), &value_) ? kValid : kInvalid;
  }
}

template <typename T>
Enum<T>::Enum(const utils::json::JsonValueRef& value, EnumType def_value)
    : PrimitiveType(InitHelper(value, &utils::json::JsonValueRef::IsString))
    , value_(def_value) {
  if (!is_initialized()) {
    value_state_ = kValid;
  } else if (is_valid()) {
    value_state_ =
        EnumFromJsonString(value.AsString(), &value_) ? kValid : kInvalid;
  }
}

template <typename T>
utils::json::JsonValue Enum<T>::ToJsonValue() const {
  return utils::json::JsonValue(EnumToJsonString(value_));
}

// Non-const version
template <typename T, size_t minsize, size_t maxsize>
Array<T, minsize, maxsize>::Array(utils::json::JsonValueRef& value)
    : CompositeType(InitHelper(value, &utils::json::JsonValueRef::IsArray)) {
  if (value.IsValid()) {
    if (value.IsArray()) {
      this->reserve(value.Size());
      for (utils::json::JsonValue::iterator i = value.begin(),
                                            end = value.end();
           i != end;
           ++i) {
        push_back(*i);
      }
    } else {
      // Array is empty, empty initialized or uninitialized
      // depending on InitHelper result
    }
  }
}

// Const version, must be identical to the non-const version
template <typename T, size_t minsize, size_t maxsize>
Array<T, minsize, maxsize>::Array(const utils::json::JsonValueRef& value)
    : CompositeType(InitHelper(value, &utils::json::JsonValueRef::IsArray)) {
  if (value.IsValid()) {
    if (value.IsArray()) {
      this->reserve(value.Size());
      for (utils::json::JsonValue::const_iterator i = value.begin(),
                                                  end = value.end();
           i != end;
           ++i) {
        push_back(*i);
      }
    } else {
      // Array is empty, empty initialized or uninitialized
      // depending on InitHelper result
    }
  }
}

template <typename T, size_t minsize, size_t maxsize>
utils::json::JsonValue Array<T, minsize, maxsize>::ToJsonValue() const {
  utils::json::JsonValue array(utils::json::ValueType::ARRAY_VALUE);
  for (size_t i = 0; i != this->size(); ++i) {
    array[utils::json::JsonValue::ArrayIndex(i)] =
        (this->operator[](i)).ToJsonValue();
  }
  return array;
}

// Non-const version
template <typename T, size_t minsize, size_t maxsize>
Map<T, minsize, maxsize>::Map(utils::json::JsonValueRef& value)
    : CompositeType(InitHelper(value, &utils::json::JsonValueRef::IsObject)) {
  if (value.IsValid()) {
    if (value.IsObject()) {
      for (utils::json::JsonValue::iterator i = value.begin(),
                                            end = value.end();
           i != end;
           ++i) {
        this->insert(typename MapType::value_type(i.key().AsString(), T(&*i)));
      }
    } else {
      // Map is empty, empty initialized or uninitialized
      // depending on InitHelper result
    }
  }
}

template <typename T, size_t minsize, size_t maxsize>
Map<T, minsize, maxsize>::Map(const utils::json::JsonValueRef& value)
    : CompositeType(InitHelper(value, &utils::json::JsonValueRef::IsObject)) {
  if (value.IsValid()) {
    if (value.IsObject()) {
      for (utils::json::JsonValue::const_iterator i = value.begin(),
                                                  end = value.end();
           i != end;
           ++i) {
        this->insert(typename MapType::value_type(i.key().AsString(), T(*i)));
      }
    } else {
      // Map is empty, empty initialized or uninitialized
      // depending on InitHelper result
    }
  }
}

template <typename T, size_t minsize, size_t maxsize>
utils::json::JsonValue Map<T, minsize, maxsize>::ToJsonValue() const {
  utils::json::JsonValue map(utils::json::ValueType::OBJECT_VALUE);
  for (typename MapType::const_iterator i = this->begin(), end = this->end();
       i != end;
       ++i) {
    map[i->first] = i->second.ToJsonValue();
  }
  return map;
}

template <typename T>
Nullable<T>::Nullable(const utils::json::JsonValueRef& value)
    : T(value), marked_null_(value.IsValid() && value.IsNull()) {}

template <typename T>
Nullable<T>::Nullable(utils::json::JsonValueRef& value)
    : T(value), marked_null_(value.IsValid() && value.IsNull()) {}

template <typename T>
template <typename U>
Nullable<T>::Nullable(const utils::json::JsonValueRef& value,
                      const U& def_value)
    : T(value, def_value), marked_null_(value.IsValid() && value.IsNull()) {}

template <typename T>
inline utils::json::JsonValue Nullable<T>::ToJsonValue() const {
  return marked_null_ ? utils::json::JsonValue() : T::ToJsonValue();
}

template <typename T>
template <typename U>
Optional<T>::Optional(const utils::json::JsonValueRef& value,
                      const U& def_value)
    : value_(value, def_value) {}

template <typename T>
inline utils::json::JsonValue Optional<T>::ToJsonValue() const {
  return value_.ToJsonValue();
}

template <typename T>
Stringifyable<T>::Stringifyable(const utils::json::JsonValueRef& value)
    : T(value.IsValid() && !value.IsString() ? value
                                             : utils::json::JsonValueRef())
    , predefined_string_(value.IsValid() && value.IsString() ? value.AsString()
                                                             : "") {}

template <typename T>
Stringifyable<T>::Stringifyable(utils::json::JsonValueRef& value)
    : T(value.IsValid() && !value.IsString() ? value
                                             : utils::json::JsonValueRef())
    , predefined_string_(value.IsValid() && value.IsString() ? value.AsString()
                                                             : "") {}

template <typename T>
template <typename U>
Stringifyable<T>::Stringifyable(const utils::json::JsonValueRef& value,
                                const U& def_value)
    : T(value.IsValid() && !value.IsString() ? (value, def_value)
                                             : utils::json::JsonValueRef())
    , predefined_string_(value.IsValid() && value.IsString() ? value.AsString()
                                                             : "") {}

template <typename T>
inline utils::json::JsonValue Stringifyable<T>::ToJsonValue() const {
  return predefined_string_.empty() ? T::ToJsonValue() : predefined_string_;
}

}  // namespace rpc

#endif /* VALIDATED_TYPES_JSON_INL_H_ */
