/*
 * Copyright (c) 2016, Ford Motor Company
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
#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_JSON_UTILS_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_JSON_UTILS_H_

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#if defined(QT_PORT)
#include <QVariant>
#else
#include "json/json.h"
#endif

#include "utils/macro.h"

namespace utils {
namespace json {

namespace ValueType {
enum Type {
  NULL_VALUE = 0,  ///< 'null' value
  INT_VALUE,       ///< signed integer value
  UINT_VALUE,      ///< unsigned integer value
  REAL_VALUE,      ///< double value
  STRING_VALUE,    ///< UTF-8 string value
  BOOL_VALUE,      ///< bool value
  ARRAY_VALUE,     ///< array value (ordered list)
  OBJECT_VALUE     ///< object value (collection of name/value pairs).
};
}  // namespace ValueType

class JsonValueRef;

class JsonValue {
 public:
  typedef int32_t Int;
  typedef uint32_t UInt;
  typedef uint32_t ArrayIndex;
  typedef std::vector<std::string> Members;
  typedef std::pair<JsonValue, bool> ParseResult;

#if defined(QT_PORT)
  typedef QVariant Storage;
#else
  typedef Json::Value Storage;
#endif

  JsonValue(const char* value);

  JsonValue(const std::string& value);

  JsonValue(Int value);

  JsonValue(UInt value);

  JsonValue(bool value);

  JsonValue(double value);

  JsonValue(ValueType::Type type = ValueType::NULL_VALUE);

  JsonValue(const Storage& value);

  static ParseResult Parse(const std::string& document);

  std::string ToJson(const bool styled = true) const;

  bool HasMember(const char* key) const;

  bool HasMember(const std::string& key) const;

  JsonValue Get(const char* key, const JsonValue& default_value) const;

  JsonValue Get(const std::string& key, const JsonValue& default_value) const;

  std::string AsString() const;

  Int AsInt() const;

  UInt AsUInt() const;

  bool AsBool() const;

  double AsDouble() const;

  bool IsBool() const;

  bool IsDouble() const;

  bool IsInt() const;

  bool IsUInt() const;

  bool IsString() const;

  bool IsObject() const;

  bool IsNull() const;

  bool IsEmpty() const;

  bool IsArray() const;

  ArrayIndex Size() const;

  ValueType::Type Type() const;

  Members GetMemberNames() const;

  friend class JsonValueRef;

  operator JsonValueRef();

  operator const JsonValueRef() const;

  JsonValueRef operator[](ArrayIndex index);

  const JsonValueRef operator[](ArrayIndex index) const;

  JsonValueRef operator[](const char* key);

  JsonValueRef operator[](const std::string& key);

  const JsonValueRef operator[](const char* key) const;

  const JsonValueRef operator[](const std::string& key) const;

  class iterator;
  friend class iterator;

  class const_iterator;
  friend class const_iterator;

  iterator begin();

  iterator end();

  const_iterator begin() const;

  const_iterator end() const;

  JsonValueRef Append(const JsonValue& value);

  void Clear();

 private:
  Storage storage_;
};

////////////////////////////////////////////////////////////////////////////////
/// class JsonValueRef declaration
////////////////////////////////////////////////////////////////////////////////

class JsonValueRef {
 public:
  JsonValueRef();

  JsonValueRef& operator=(const JsonValue& rhs);

  JsonValueRef& operator=(const JsonValueRef& rhs);

  bool IsString() const;

  bool IsBool() const;

  bool IsDouble() const;

  bool IsInt() const;

  bool IsUInt() const;

  bool IsObject() const;

  bool IsNull() const;

  bool IsEmpty() const;

  bool IsArray() const;

  std::string AsString() const;

  JsonValue::Int AsInt() const;

  JsonValue::UInt AsUInt() const;

  bool AsBool() const;

  double AsDouble() const;

  operator JsonValue() const;

  JsonValueRef operator[](const char* key);

  JsonValueRef operator[](const std::string& key);

  JsonValueRef operator[](JsonValue::ArrayIndex index);

  const JsonValueRef operator[](const char* key) const;

  const JsonValueRef operator[](const std::string& key) const;

  const JsonValueRef operator[](JsonValue::ArrayIndex index) const;

  JsonValue::ArrayIndex Size() const;

  ValueType::Type Type() const;

  bool HasMember(const char* key) const;

  bool HasMember(const std::string& key) const;

  JsonValue::Members GetMemberNames() const;

  std::string ToJson(const bool styled = false) const;

  JsonValue::iterator begin();

  JsonValue::iterator end();

  JsonValue::const_iterator begin() const;

  JsonValue::const_iterator end() const;

  JsonValueRef Append(const JsonValue& value);

  void Clear();

  bool IsValid() const;

 private:
  friend class JsonValue;
  friend class const_iterator;
  friend class iterator;

  enum Kind { None, Key, Index };

  explicit JsonValueRef(JsonValue::Storage& storage);

  JsonValueRef(JsonValue::Storage& storage, const char* key);

  JsonValueRef(JsonValue::Storage& storage, JsonValue::ArrayIndex index);

  JsonValueRef(JsonValue::Storage& storage,
               JsonValue::ArrayIndex index,
               Kind kind);

  static JsonValueRef::Kind GetKind(const ValueType::Type type);

  JsonValue::Storage* storage_;

  Kind kind_;
};

////////////////////////////////////////////////////////////////////////////////
/// class JsonValue::iterator declaration
////////////////////////////////////////////////////////////////////////////////

class JsonValue::iterator {
 public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef int32_t difference_type;
  typedef JsonValue value_type;
  typedef JsonValueRef reference;

  reference operator*() const;

  bool operator==(const iterator& other) const;

  bool operator!=(const iterator& other) const;

  bool operator==(const const_iterator& other) const;

  bool operator!=(const const_iterator& other) const;

  iterator& operator++();

  iterator operator++(difference_type);

  iterator& operator--();

  iterator operator--(difference_type);

  iterator operator+(difference_type j) const;

  iterator operator-(difference_type j) const;

  iterator& operator+=(difference_type j);

  iterator& operator-=(difference_type j);

  JsonValue key() const;

 private:
  iterator();

  iterator(Storage* storage, ArrayIndex index, JsonValueRef::Kind kind);

  friend class const_iterator;
  friend class JsonValue;
  friend class JsonValueRef;

  JsonValueRef::Kind kind_;

  Storage* storage_;

  ArrayIndex index_;
};

////////////////////////////////////////////////////////////////////////////////
/// class JsonValue::const_iterator
////////////////////////////////////////////////////////////////////////////////

class JsonValue::const_iterator {
 public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef int32_t difference_type;
  typedef JsonValue value_type;
  typedef JsonValueRef reference;

  const reference operator*() const;

  bool operator==(const iterator& other) const;

  bool operator!=(const iterator& other) const;

  bool operator==(const const_iterator& other) const;

  bool operator!=(const const_iterator& other) const;

  const_iterator& operator++();

  const_iterator operator++(difference_type);

  const_iterator& operator--();

  const_iterator operator--(difference_type);

  const_iterator operator+(difference_type j) const;

  const_iterator operator-(difference_type j) const;

  const_iterator& operator+=(difference_type j);

  const_iterator& operator-=(difference_type j);

  JsonValue key() const;

 private:
  const_iterator();

  const_iterator(const JsonValue::Storage* storage,
                 JsonValue::ArrayIndex index,
                 JsonValueRef::Kind kind);

  friend class iterator;
  friend class JsonValue;
  friend class JsonValueRef;

  JsonValueRef::Kind kind_;

  const JsonValue::Storage* storage_;

  JsonValue::ArrayIndex index_;
};

inline JsonValue::const_iterator::const_iterator()
    : kind_(JsonValueRef::None), storage_(NULL), index_(0) {}

inline JsonValue::const_iterator::const_iterator(
    const JsonValue::Storage* storage,
    ArrayIndex index,
    JsonValueRef::Kind kind)
    : kind_(kind), storage_(storage), index_(index) {}

inline const JsonValue::const_iterator::reference JsonValue::const_iterator::
operator*() const {
  // Result is const thus it's safe to cact out the const
  return JsonValueRef(const_cast<Storage&>(*storage_), index_, kind_);
}

inline bool JsonValue::const_iterator::operator==(
    const JsonValue::iterator& other) const {
  return index_ == other.index_;
}

inline bool JsonValue::const_iterator::operator!=(
    const JsonValue::iterator& other) const {
  return index_ != other.index_;
}

inline bool JsonValue::const_iterator::operator==(
    const JsonValue::const_iterator& other) const {
  return index_ == other.index_;
}

inline bool JsonValue::const_iterator::operator!=(
    const JsonValue::const_iterator& other) const {
  return index_ != other.index_;
}

inline JsonValue::const_iterator& JsonValue::const_iterator::operator++() {
  ++index_;
  return *this;
}

inline JsonValue::const_iterator JsonValue::const_iterator::operator++(
    JsonValue::const_iterator::difference_type) {
  const_iterator result = *this;
  ++index_;
  return result;
}

inline JsonValue::const_iterator& JsonValue::const_iterator::operator--() {
  --index_;
  return *this;
}

inline JsonValue::const_iterator JsonValue::const_iterator::operator--(
    JsonValue::const_iterator::difference_type) {
  const_iterator result = *this;
  --index_;
  return result;
}

inline JsonValue::const_iterator JsonValue::const_iterator::operator+(
    JsonValue::const_iterator::difference_type j) const {
  const_iterator result = *this;
  result.index_ += j;
  return result;
}

inline JsonValue::const_iterator JsonValue::const_iterator::operator-(
    JsonValue::const_iterator::difference_type j) const {
  return operator+(-j);
}

inline JsonValue::const_iterator& JsonValue::const_iterator::operator+=(
    JsonValue::const_iterator::difference_type j) {
  index_ += j;
  return *this;
}

inline JsonValue::const_iterator& JsonValue::const_iterator::operator-=(
    JsonValue::const_iterator::difference_type j) {
  index_ -= j;
  return *this;
}

////////////////////////////////////////////////////////////////////////////////
/// class JsonValue implementation
////////////////////////////////////////////////////////////////////////////////

inline JsonValue::JsonValue(const char* value) : storage_(value) {}

inline JsonValue::JsonValue(Int value) : storage_(value) {}

inline JsonValue::JsonValue(UInt value) : storage_(value) {}

inline JsonValue::JsonValue(bool value) : storage_(value) {}

inline JsonValue::JsonValue(double value) : storage_(value) {}

inline JsonValue::JsonValue(const JsonValue::Storage& value)
    : storage_(value) {}

inline bool JsonValue::HasMember(const std::string& key) const {
  return HasMember(key.c_str());
}

inline JsonValue JsonValue::Get(const std::string& key,
                                const JsonValue& default_value) const {
  return Get(key.c_str(), default_value);
}

inline JsonValueRef JsonValue::operator[](ArrayIndex index) {
  return JsonValueRef(storage_, index);
}

inline const JsonValueRef JsonValue::operator[](ArrayIndex index) const {
  // It's safe to cast out since the result is const
  return JsonValueRef(const_cast<Storage&>(storage_), index);
}

inline JsonValueRef JsonValue::operator[](const char* key) {
  return JsonValueRef(storage_, key);
}

inline JsonValueRef JsonValue::operator[](const std::string& key) {
  return (*this)[key.c_str()];
}

inline const JsonValueRef JsonValue::operator[](const char* key) const {
  // It's safe to cast out since the result is const
  return JsonValueRef(const_cast<Storage&>(storage_), key);
}

inline const JsonValueRef JsonValue::operator[](const std::string& key) const {
  return (*this)[key.c_str()];
}

inline bool JsonValue::IsBool() const {
  return Type() == ValueType::BOOL_VALUE;
}

inline bool JsonValue::IsDouble() const {
  return Type() == ValueType::REAL_VALUE;
}

inline bool JsonValue::IsInt() const {
  return Type() == ValueType::INT_VALUE;
}

inline bool JsonValue::IsUInt() const {
  return Type() == ValueType::UINT_VALUE;
}

inline bool JsonValue::IsString() const {
  return Type() == ValueType::STRING_VALUE;
}

inline bool JsonValue::IsObject() const {
  const ValueType::Type type = Type();
  return type == ValueType::NULL_VALUE || type == ValueType::OBJECT_VALUE;
}

inline bool JsonValue::IsNull() const {
  return Type() == ValueType::NULL_VALUE;
}

inline bool JsonValue::IsArray() const {
  const ValueType::Type type = Type();
  return type == ValueType::NULL_VALUE || type == ValueType::ARRAY_VALUE;
}

inline JsonValue::iterator JsonValue::begin() {
  return iterator(&storage_, 0, JsonValueRef::GetKind(Type()));
}

inline JsonValue::iterator JsonValue::end() {
  return iterator(&storage_, Size(), JsonValueRef::GetKind(Type()));
}

inline JsonValue::const_iterator JsonValue::begin() const {
  return const_iterator(&storage_, 0, JsonValueRef::GetKind(Type()));
}

inline JsonValue::const_iterator JsonValue::end() const {
  return const_iterator(&storage_, Size(), JsonValueRef::GetKind(Type()));
}

inline JsonValue::operator JsonValueRef() {
  return JsonValueRef(storage_);
}

inline JsonValue::operator const JsonValueRef() const {
  // It's safe to cast out since the result is const
  return JsonValueRef(const_cast<Storage&>(storage_));
}

inline JsonValueRef JsonValue::Append(const JsonValue& value) {
  return (*this)[Size()] = value;
}

////////////////////////////////////////////////////////////////////////////////
/// class JsonValueRef implementation
////////////////////////////////////////////////////////////////////////////////

inline JsonValueRef::JsonValueRef() : storage_(NULL), kind_(None) {}

inline JsonValueRef::JsonValueRef(JsonValue::Storage& storage)
    : storage_(&storage), kind_(None) {}

inline JsonValueRef& JsonValueRef::operator=(const JsonValue& rhs) {
  *storage_ = rhs.storage_;
  return *this;
}

inline JsonValueRef& JsonValueRef::operator=(const JsonValueRef& rhs) {
  *storage_ = *rhs.storage_;
  return *this;
}

inline JsonValueRef JsonValueRef::operator[](const char* key) {
  DCHECK(IsValid());
  return JsonValueRef(*storage_, key);
}

inline JsonValueRef JsonValueRef::operator[](const std::string& key) {
  DCHECK(IsValid());
  return (*this)[key.c_str()];
}

inline JsonValueRef JsonValueRef::operator[](
    utils::json::JsonValue::ArrayIndex index) {
  DCHECK(IsValid());
  return JsonValueRef(*storage_, index);
}

inline const JsonValueRef JsonValueRef::operator[](
    utils::json::JsonValue::ArrayIndex index) const {
  DCHECK(IsValid());
  return JsonValueRef(*storage_, index);
}

inline const JsonValueRef JsonValueRef::operator[](const char* key) const {
  DCHECK(IsValid());
  return JsonValueRef(*storage_, key);
}

inline const JsonValueRef JsonValueRef::operator[](
    const std::string& key) const {
  DCHECK(IsValid());
  return (*this)[key.c_str()];
}

inline JsonValue::Members JsonValueRef::GetMemberNames() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).GetMemberNames();
}

inline bool JsonValueRef::HasMember(const char* key) const {
  DCHECK(IsValid());
  return JsonValue(*storage_).HasMember(key);
}

inline bool JsonValueRef::HasMember(const std::string& key) const {
  DCHECK(IsValid());
  return HasMember(key.c_str());
}

inline std::string JsonValueRef::ToJson(const bool styled) const {
  DCHECK(IsValid());
  return JsonValue(*storage_).ToJson(styled);
}

inline JsonValueRef JsonValueRef::Append(const JsonValue& value) {
  DCHECK(IsValid());
  return (*this)[Size()] = value;
}

inline JsonValue::ArrayIndex JsonValueRef::Size() const {
  return JsonValue(*storage_).Size();
}

inline ValueType::Type JsonValueRef::Type() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).Type();
}

inline bool JsonValueRef::IsString() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).IsString();
}

inline bool JsonValueRef::IsBool() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).IsBool();
}

inline bool JsonValueRef::IsDouble() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).IsDouble();
}

inline bool JsonValueRef::IsInt() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).IsInt();
}

inline bool JsonValueRef::IsUInt() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).IsUInt();
}

inline bool JsonValueRef::IsObject() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).IsObject();
}

inline bool JsonValueRef::IsNull() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).IsNull();
}

inline bool JsonValueRef::IsEmpty() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).IsEmpty();
}

inline bool JsonValueRef::IsArray() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).IsArray();
}

inline std::string JsonValueRef::AsString() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).AsString();
}

inline JsonValue::Int JsonValueRef::AsInt() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).AsInt();
}

inline JsonValue::UInt JsonValueRef::AsUInt() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).AsUInt();
}

inline bool JsonValueRef::AsBool() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).AsBool();
}

inline double JsonValueRef::AsDouble() const {
  DCHECK(IsValid());
  return JsonValue(*storage_).AsDouble();
}

inline JsonValueRef::operator JsonValue() const {
  DCHECK(IsValid());
  return JsonValue(*storage_);
}

inline bool JsonValueRef::IsValid() const {
  return storage_ != NULL;
}

inline JsonValue::iterator JsonValueRef::begin() {
  DCHECK(IsValid());
  return JsonValue::iterator(storage_, 0, JsonValueRef::GetKind(Type()));
}

inline JsonValue::iterator JsonValueRef::end() {
  DCHECK(IsValid());
  return JsonValue::iterator(storage_, Size(), JsonValueRef::GetKind(Type()));
}

inline JsonValue::const_iterator JsonValueRef::begin() const {
  DCHECK(IsValid());
  return JsonValue::const_iterator(storage_, 0, JsonValueRef::GetKind(Type()));
}

inline JsonValue::const_iterator JsonValueRef::end() const {
  DCHECK(IsValid());
  return JsonValue::const_iterator(
      storage_, Size(), JsonValueRef::GetKind(Type()));
}

////////////////////////////////////////////////////////////////////////////////
/// class JsonValue::iterator implementation
////////////////////////////////////////////////////////////////////////////////

inline JsonValue::iterator::iterator()
    : kind_(JsonValueRef::None), storage_(NULL), index_(0) {}

inline JsonValue::iterator::iterator(JsonValue::Storage* storage,
                                     JsonValue::ArrayIndex index,
                                     JsonValueRef::Kind kind)
    : kind_(kind), storage_(storage), index_(index) {}

inline JsonValue::iterator::reference JsonValue::iterator::operator*() const {
  return JsonValueRef(*storage_, index_, kind_);
}

inline bool JsonValue::iterator::operator==(
    const JsonValue::iterator& other) const {
  return index_ == other.index_;
}

inline bool JsonValue::iterator::operator!=(
    const JsonValue::iterator& other) const {
  return index_ != other.index_;
}

inline bool JsonValue::iterator::operator==(
    const JsonValue::const_iterator& other) const {
  return index_ == other.index_;
}

inline bool JsonValue::iterator::operator!=(
    const JsonValue::const_iterator& other) const {
  return index_ != other.index_;
}

inline JsonValue::iterator& JsonValue::iterator::operator++() {
  ++index_;
  return *this;
}

inline JsonValue::iterator JsonValue::iterator::operator++(
    JsonValue::iterator::difference_type) {
  iterator result = *this;
  ++index_;
  return result;
}

inline JsonValue::iterator& JsonValue::iterator::operator--() {
  --index_;
  return *this;
}

inline JsonValue::iterator JsonValue::iterator::operator--(
    JsonValue::iterator::difference_type) {
  iterator result = *this;
  --index_;
  return result;
}

inline JsonValue::iterator JsonValue::iterator::operator+(
    JsonValue::iterator::difference_type j) const {
  iterator result = *this;
  result.index_ += j;
  return result;
}

inline JsonValue::iterator JsonValue::iterator::operator-(
    JsonValue::iterator::difference_type j) const {
  return operator+(-j);
}

inline JsonValue::iterator& JsonValue::iterator::operator+=(
    JsonValue::iterator::difference_type j) {
  index_ += j;
  return *this;
}

inline JsonValue::iterator& JsonValue::iterator::operator-=(
    JsonValue::iterator::difference_type j) {
  index_ -= j;
  return *this;
}

}  // namespace json
}  // namespace utils

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_JSON_UTILS_H_
