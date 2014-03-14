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

/*
 * Composite type
 */
// static
template<class T, size_t minsize, size_t maxsize>
void CompositeType::WriteJsonField(const char* field_name,
                    const Array<T, minsize, maxsize>& field,
                    Json::Value* json_value) {
  if (field.is_initialized()) {
    (*json_value)[field_name] = field.ToJsonValue();
  }
}

// static
template<class T, size_t minsize, size_t maxsize>
void CompositeType::WriteJsonField(const char* field_name,
                    const Map<T, minsize, maxsize>& field,
                    Json::Value* json_value) {
  if (field.is_initialized()) {
    (*json_value)[field_name] = field.ToJsonValue();
  }
}

// static
template<class T>
void CompositeType::WriteJsonField(const char* field_name,
                                   const Optional<T>& field,
                                   Json::Value* json_value) {
  if (field.is_initialized()) {
    (*json_value)[field_name] = field->ToJsonValue();
  }
}
// static
template<class T>
void CompositeType::WriteJsonField(const char* field_name,
                                   const Mandatory<T>& field,
                                   Json::Value* json_value) {
  (*json_value)[field_name] = field.ToJsonValue();
}


inline Boolean::Boolean(const Json::Value& value)
    : PrimitiveType(!value.isNull(), value.isBool()),
      value_(valid_ ? value.asBool() : bool()) {
}

inline Boolean::Boolean(const Json::Value& value, bool def_value)
    : PrimitiveType(true, value.isBool() || value.isNull()),
      value_(value.isBool() ? value.asBool() : def_value) {
}

inline Json::Value Boolean::ToJsonValue() const {
  return Json::Value(value_);
}

template<typename T, T minval, T maxval>
Integer<T, minval, maxval>::Integer(const Json::Value& value)
    : PrimitiveType(!value.isNull(),
                    value.isInt() && range_.Includes(value.asInt64())),
      value_(valid_ ? value.asInt64() : IntType()) {
}

template<typename T, T minval, T maxval>
Integer<T, minval, maxval>::Integer(const Json::Value& value, IntType def_value)
    : PrimitiveType(true, (value.isInt() && range_.Includes(value.asInt64()))
                           || value.isNull()),
      value_(value.isInt() ? value.asInt64() : def_value) {
}

template<typename T, T minval, T maxval>
Json::Value Integer<T, minval, maxval>::ToJsonValue() const {
  return Json::Value(Json::Int64(value_));
}

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
Float<minnum, maxnum, minden, maxden>::Float(const Json::Value& value)
    : PrimitiveType(!value.isNull(),
                    value.isDouble() && range_.Includes(value.asDouble())),
      value_(valid_ ? value.asDouble() : double()) {
}

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
Float<minnum, maxnum, minden, maxden>::Float(const Json::Value& value,
                                             double def_value)
    : PrimitiveType(true, (value.isDouble()
                             && range_.Includes(value.asDouble()))
                          || value.isNull()),
      value_(value.isDouble() ? value.asDouble() : def_value) {
}

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
Json::Value Float<minnum, maxnum, minden, maxden>::ToJsonValue() const {
  return Json::Value(value_);
}

template<size_t minlen, size_t maxlen>
String<minlen, maxlen>::String(const Json::Value& value)
    : PrimitiveType(!value.isNull(),
                    (value.isString() &&
                        length_range_.Includes(value.asString().length()))),
      value_(valid_ ? value.asString() : std::string()) {
}

template<size_t minlen, size_t maxlen>
String<minlen, maxlen>::String(const Json::Value& value, const std::string& def_value)
    : PrimitiveType(true, (value.isString()
                              && length_range_.Includes(value.asString().length()))
                          || value.isNull()),
      value_(value.isString() ? value.asString() : def_value) {
}

template<size_t minlen, size_t maxlen>
Json::Value String<minlen, maxlen>::ToJsonValue() const {
  return Json::Value(value_);
}

template<typename T>
Enum<T>::Enum(const Json::Value& value)
    : PrimitiveType(!value.isNull(), value.isString()),
      value_(EnumType()) {
  if (valid_) {
    valid_ = EnumFromJsonString(value.asString(), &value_);
  }
}

template<typename T>
Enum<T>::Enum(const Json::Value& value, EnumType def_value)
    : PrimitiveType(true, value.isString() || value.isNull()),
      value_(def_value) {
  if (value.isString()) {
    valid_ = EnumFromJsonString(value.asString(), &value_);
  }
}

template<typename T>
Json::Value Enum<T>::ToJsonValue() const {
  return Json::Value(Json::StaticString(EnumToJsonString(value_)));
}

template<typename T, size_t minsize, size_t maxsize>
Array<T, minsize, maxsize>::Array(const Json::Value& value) {
  if (value.isArray()) {
    this->reserve(value.size());
    for (Json::Value::const_iterator i = value.begin(); i != value.end(); ++i) {
      push_back(*i);
    }
  } else if (value.isNull()) {
    // Do nothing, keep array empty and uninitialized
  } else {
    // In case of non-array value initialize array with null value
    // so it handled as initialized but invalid
    push_back(Json::Value());
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

template<typename T, size_t minsize, size_t maxsize>
Map<T, minsize, maxsize>::Map(const Json::Value& value) {
  if (value.isObject()) {
    for (Json::Value::const_iterator i = value.begin(); i != value.end(); ++i) {
      this->insert(typename MapType::value_type(i.key().asString(), T(*i)));
    }
  } else if (value.isNull()) {
    // Do nothing, keep array empty and uninitialized
  } else {
    // In case of non-array value initialize array with null value
    // so it handled as initialized but invalid
    this->insert(typename MapType::value_type("", T(Json::Value())));
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
template<typename U>
Mandatory<T>::Mandatory(const Json::Value& value, const U& def_value)
    : T(value, def_value) {
}

template<typename T>
template<typename U>
Optional<T>::Optional(const Json::Value& value, const U& def_value)
    : value_(value, def_value) {
}

}  // namespace rpc

#endif /* VALIDATED_TYPES_JSON_INL_H_ */
