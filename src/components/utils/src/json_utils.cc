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
#include "utils/json_utils.h"

#include <iterator>

namespace {

Json::Value CreateStorage(const utils::json::ValueType::Type type) {
  using namespace utils::json;
  switch (type) {
    case ValueType::OBJECT_VALUE:
      return Json::Value(Json::objectValue);
    case ValueType::ARRAY_VALUE:
      return Json::Value(Json::arrayValue);
    case ValueType::BOOL_VALUE:
      return Json::Value(Json::booleanValue);
    case ValueType::STRING_VALUE:
      return Json::Value(Json::stringValue);
    case ValueType::INT_VALUE:
      return Json::Value(Json::intValue);
    case ValueType::UINT_VALUE:
      return Json::Value(Json::uintValue);
    case ValueType::REAL_VALUE:
      return Json::Value(Json::realValue);
    default:
      return Json::Value();
  }
  DCHECK(type == ValueType::NULL_VALUE);
  return Json::Value();
}

utils::json::ValueType::Type GetType(const Json::Value& value) {
  using namespace utils::json;
  const Json::ValueType type = value.type();
  switch (type) {
    case Json::objectValue:
      return ValueType::OBJECT_VALUE;
    case Json::arrayValue:
      return ValueType::ARRAY_VALUE;
    case Json::booleanValue:
      return ValueType::BOOL_VALUE;
    case Json::stringValue:
      return ValueType::STRING_VALUE;
    case Json::intValue:
      return ValueType::INT_VALUE;
    case Json::uintValue:
      return ValueType::UINT_VALUE;
    case Json::realValue:
      return ValueType::REAL_VALUE;
    default:
      return ValueType::NULL_VALUE;
  }
  DCHECK(type == Json::nullValue);
  return ValueType::NULL_VALUE;
}

utils::json::JsonValue::Storage& GetObjectItemByIndex(
    utils::json::JsonValue::Storage& storage,
    utils::json::JsonValue::ArrayIndex index) {
  DCHECK(storage.isObject());
  DCHECK(storage.isValidIndex(index));
  utils::json::JsonValue::Storage::iterator itr = storage.begin();
  for (utils::json::JsonValue::ArrayIndex cnt = 0; cnt < index; ++cnt) {
    ++itr;
  }
  return *itr;
}

utils::json::JsonValue GetKeyByIndex(
    const utils::json::JsonValue::Storage& storage,
    utils::json::JsonValue::ArrayIndex index) {
  DCHECK(storage.isObject());
  DCHECK(storage.isValidIndex(index));
  utils::json::JsonValue::Storage::iterator itr = storage.begin();
  for (utils::json::JsonValue::ArrayIndex cnt = 0; cnt < index; ++cnt) {
    ++itr;
  }
  return itr.key();
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////
/// class utils::json::JsonValueRef
////////////////////////////////////////////////////////////////////////////////

utils::json::JsonValueRef::JsonValueRef(JsonValue::Storage& storage,
                                        const char* key)
    : storage_(&storage), kind_(Key) {
  storage_ = &(storage[key]);
}

utils::json::JsonValueRef::JsonValueRef(JsonValue::Storage& storage,
                                        JsonValue::ArrayIndex index)
    : storage_(&storage), kind_(Index) {
  storage_ = &(storage[index]);
}

utils::json::JsonValueRef::JsonValueRef(JsonValue::Storage& storage,
                                        JsonValue::ArrayIndex index,
                                        Kind kind)
    : storage_(&storage), kind_(kind) {
  // This ctor is used from iterators, thus Map\List should be already present
  if (kind_ == Key) {
    storage_ = &GetObjectItemByIndex(storage, index);
  } else if (kind_ == Index) {
    storage_ = &(storage[index]);
  } else {
    DCHECK(kind_ == None);
  }
}

void utils::json::JsonValueRef::Clear() {
  DCHECK(IsValid());
  storage_->clear();
}

utils::json::JsonValueRef::Kind utils::json::JsonValueRef::GetKind(
    const ValueType::Type type) {
  JsonValueRef::Kind kind = JsonValueRef::None;
  if (type == ValueType::ARRAY_VALUE) {
    kind = JsonValueRef::Index;
  } else if (type == ValueType::OBJECT_VALUE) {
    kind = JsonValueRef::Key;
  }
  return kind;
}

////////////////////////////////////////////////////////////////////////////////
/// class utils::json::JsonValue
////////////////////////////////////////////////////////////////////////////////

utils::json::JsonValue::JsonValue(const std::string& value) : storage_(value) {}

utils::json::JsonValue::JsonValue(ValueType::Type type)
    : storage_(CreateStorage(type)) {}

utils::json::JsonValue::ParseResult utils::json::JsonValue::Parse(
    const std::string& document) {
  Storage storage;
  Json::Reader reader;
  bool is_parsed = reader.parse(document, storage, false);
  if (!is_parsed) {
    return std::make_pair(JsonValue(ValueType::NULL_VALUE), false);
  }
  return std::make_pair(JsonValue(storage), true);
}

bool utils::json::JsonValue::HasMember(const char* key) const {
  return storage_.isMember(key);
}

utils::json::JsonValue utils::json::JsonValue::Get(
    const char* key, const JsonValue& default_value) const {
  return storage_.get(key, default_value.storage_);
}

std::string utils::json::JsonValue::AsString() const {
  return storage_.asString();
}

utils::json::JsonValue::Int utils::json::JsonValue::AsInt() const {
  return storage_.asInt();
}

utils::json::JsonValue::UInt utils::json::JsonValue::AsUInt() const {
  return storage_.asUInt();
}

bool utils::json::JsonValue::AsBool() const {
  return storage_.asBool();
}

double utils::json::JsonValue::AsDouble() const {
  return storage_.asDouble();
}

utils::json::JsonValue::ArrayIndex utils::json::JsonValue::Size() const {
  return storage_.size();
}

utils::json::ValueType::Type utils::json::JsonValue::Type() const {
  return GetType(storage_);
}

utils::json::JsonValue::Members utils::json::JsonValue::GetMemberNames() const {
  return storage_.getMemberNames();
}

bool utils::json::JsonValue::IsEmpty() const {
  return storage_.empty();
}

std::string utils::json::JsonValue::ToJson(const bool styled) const {
  if (styled) {
    return storage_.toStyledString();
  }
  Json::FastWriter writer;
  std::string result = writer.write(storage_);
  // cut off the last '\n' which is added be jsoncpp
  if (result[result.size() - 1] == '\n') {
    result.erase(result.size() - 1, 1);
  }
  return result;
}

void utils::json::JsonValue::Clear() {
  storage_.clear();
}

////////////////////////////////////////////////////////////////////////////////
/// class utils::json::JsonValue::iterator
////////////////////////////////////////////////////////////////////////////////

utils::json::JsonValue utils::json::JsonValue::iterator::key() const {
  return GetKeyByIndex(*storage_, index_);
}

////////////////////////////////////////////////////////////////////////////////
/// class utils::json::JsonValue::const_iterator
////////////////////////////////////////////////////////////////////////////////

utils::json::JsonValue utils::json::JsonValue::const_iterator::key() const {
  return GetKeyByIndex(*storage_, index_);
}
