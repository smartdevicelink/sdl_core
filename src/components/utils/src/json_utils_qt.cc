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

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

#include <iterator>
#include <limits>

namespace {

const QVariant kNullVariant = QVariant();

template <typename T>
T* GetValuePtr(QVariant& variant) {
  if (!variant.canConvert<T>()) {
    return NULL;
  }
  return reinterpret_cast<T*>(variant.data());
}

template <typename T>
const T* GetValuePtr(const QVariant& variant) {
  if (!variant.canConvert<T>()) {
    return NULL;
  }
  return reinterpret_cast<const T*>(variant.constData());
}

QVariantMap* AsObjectPtr(QVariant& value) {
  return GetValuePtr<QVariantMap>(value);
}

QVariantList* AsArrayPtr(QVariant& value) {
  return GetValuePtr<QVariantList>(value);
}

const QVariantMap* AsObjectPtr(const QVariant& value) {
  return GetValuePtr<QVariantMap>(value);
}

const QVariantList* AsArrayPtr(const QVariant& value) {
  return GetValuePtr<QVariantList>(value);
}

QVariant CreateVariant(utils::json::ValueType::Type type) {
  using namespace utils::json;
  switch (type) {
    case ValueType::OBJECT_VALUE:
      return QVariant(QVariantMap());
    case ValueType::ARRAY_VALUE:
      return QVariant(QVariantList());
    case ValueType::STRING_VALUE:
      return QVariant(QVariant::String);
    case ValueType::INT_VALUE:
      return QVariant(QVariant::Int);
    case ValueType::UINT_VALUE:
      return QVariant(QVariant::UInt);
    case ValueType::BOOL_VALUE:
      return QVariant(QVariant::Bool);
    case ValueType::REAL_VALUE:
      return QVariant(QVariant::Double);
  }
  return QVariant();
}

bool IsQVariantUInt(const QVariant& value) {
  bool converted = false;
  double double_value = value.toDouble(&converted);
  return converted && double_value >= 0;
}

bool IsQVariantInt(const QVariant& value) {
  bool converted = false;
  value.toLongLong(&converted);
  return converted;
}

std::pair<double, bool> GetIntegralPart(double value) {
  double integral_part = 0.;
  double fractional_part = std::modf(value, &integral_part);
  if (fractional_part <= std::numeric_limits<double>::epsilon()) {
    return std::make_pair(integral_part, true);
  }
  return std::make_pair(integral_part, false);
}

utils::json::ValueType::Type GetType(const QVariant& value) {
  using namespace utils::json;
  if (value.canConvert<QVariantMap>()) {
    return ValueType::OBJECT_VALUE;
  } else if (value.canConvert<QVariantList>()) {
    return ValueType::ARRAY_VALUE;
  } else if (!value.isValid()) {
    return ValueType::NULL_VALUE;
  } else {
    const QVariant::Type type = value.type();
    if (type == QVariant::String) {
      return ValueType::STRING_VALUE;
    } else if (type == QVariant::Bool) {
      return ValueType::BOOL_VALUE;
    } else if (type == QVariant::UInt) {
      return ValueType::UINT_VALUE;
    } else if (type == QVariant::Int) {
      return ValueType::INT_VALUE;
    } else if (type == QVariant::Double) {
      // Handling of the numeric types is tricky since Qt stores
      // all json number values as double during parsing
      std::pair<double, bool> intergal_result =
          GetIntegralPart(value.toDouble());
      if (intergal_result.second) {
        if (intergal_result.first < 0) {
          if (intergal_result.first >=
              std::numeric_limits<JsonValue::Int>::min()) {
            return ValueType::INT_VALUE;
          }
        } else {
          if (intergal_result.first <=
              std::numeric_limits<JsonValue::Int>::max()) {
            return ValueType::INT_VALUE;
          } else if (intergal_result.first <=
                     std::numeric_limits<JsonValue::UInt>::max()) {
            return ValueType::UINT_VALUE;
          }
        }
      }
      return ValueType::REAL_VALUE;
    }
  }
  DCHECK(!"Unsupported type inside QVariant");
  return ValueType::NULL_VALUE;
}

utils::json::JsonValue::Members GetMemberNamesFromObject(
    const QVariantMap& object) {
  using namespace utils::json;
  utils::json::JsonValue::Members result;
  result.reserve(object.size());
  QVariantMap::const_iterator itr = object.constBegin();
  QVariantMap::const_iterator end = object.constEnd();
  for (; itr != end; ++itr) {
    result.push_back(itr.key().toStdString());
  }
  return result;
}

QVariant& GetMapItemByIndex(QVariantMap& map,
                            utils::json::JsonValue::ArrayIndex index) {
  QVariantMap::iterator itr = map.begin();
  std::advance(itr, index);
  DCHECK(itr != map.end());
  return *itr;
}

utils::json::JsonValue GetKeyByIndex(const QVariantMap& map,
                                     utils::json::JsonValue::ArrayIndex index) {
  QVariantMap::const_iterator itr = map.begin();
  std::advance(itr, index);
  DCHECK(itr != map.end());
  return itr.key();
}

utils::json::JsonValue::ArrayIndex GetMapIndexByKey(QVariantMap& map,
                                                    const char* key) {
  QVariantMap::iterator itr = map.lowerBound(key);
  if (itr == map.end()) {
    itr = map.insert(key, kNullVariant);
  }
  return std::distance(map.begin(), itr);
}

QVariant& GetListItemByIndex(QVariantList& list,
                             utils::json::JsonValue::ArrayIndex index) {
  using namespace utils::json;
  JsonValue::ArrayIndex size = static_cast<JsonValue::ArrayIndex>(list.size());
  if (index >= size) {
    // Add item to the list;
    for (JsonValue::ArrayIndex i = size; i <= index; ++i) {
      list.append(kNullVariant);
    }
  }
  return list[index];
}

void EnsureKeyExists(QVariant& storage, const char* key) {
  if (!storage.isValid()) {
    storage = QVariantMap();
  }
  QVariantMap* object_ptr = AsObjectPtr(storage);
  DCHECK(object_ptr);
  if (!object_ptr->contains(key)) {
    object_ptr->insert(key, kNullVariant);
  }
}

void EnsureIndexExists(QVariant& storage,
                       utils::json::JsonValue::ArrayIndex index) {
  if (!storage.isValid()) {
    storage = QVariantList();
  }
  QVariantList* array_ptr = AsArrayPtr(storage);
  DCHECK(array_ptr);
  GetListItemByIndex(*array_ptr, index);
}

void ClearStorage(QVariant& storage) {
  if (QVariantMap* object_ptr = AsObjectPtr(storage)) {
    object_ptr->clear();
  } else if (QVariantList* array_ptr = AsArrayPtr(storage)) {
    array_ptr->clear();
  }
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////
/// class utils::json::JsonValueRef
////////////////////////////////////////////////////////////////////////////////

utils::json::JsonValueRef::JsonValueRef(JsonValue::Storage& storage,
                                        const char* key)
    : storage_(&storage), kind_(Key) {
  EnsureKeyExists(*storage_, key);
  QVariantMap* object_ptr = AsObjectPtr(*storage_);
  DCHECK(object_ptr);
  storage_ = &((*object_ptr)[key]);
}

utils::json::JsonValueRef::JsonValueRef(JsonValue::Storage& storage,
                                        JsonValue::ArrayIndex index)
    : storage_(&storage), kind_(Index) {
  EnsureIndexExists(*storage_, index);
  QVariantList* array_ptr = AsArrayPtr(*storage_);
  DCHECK(array_ptr);
  storage_ = &((*array_ptr)[index]);
}

utils::json::JsonValueRef::JsonValueRef(JsonValue::Storage& storage,
                                        JsonValue::ArrayIndex index,
                                        Kind kind)
    : storage_(&storage), kind_(kind) {
  // This ctor is used from iterators, thus Map\List should be already present
  if (kind_ == Key) {
    QVariantMap* object_ptr = AsObjectPtr(*storage_);
    DCHECK(object_ptr);
    storage_ = &GetMapItemByIndex(*object_ptr, index);
  } else if (kind_ == Index) {
    EnsureIndexExists(*storage_, index);
    QVariantList* array_ptr = AsArrayPtr(*storage_);
    DCHECK(array_ptr);
    storage_ = &((*array_ptr)[index]);
  } else {
    DCHECK(kind_ == None);
  }
}

void utils::json::JsonValueRef::Clear() {
  DCHECK(IsValid());
  ClearStorage(*storage_);
}

void utils::json::JsonValueRef::RemoveMember(const char* key) {
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

utils::json::JsonValue::JsonValue(const std::string& value)
    : storage_(value.c_str()) {}

utils::json::JsonValue::JsonValue(ValueType::Type type)
    : storage_(CreateVariant(type)) {}

utils::json::JsonValue::ParseResult utils::json::JsonValue::Parse(
    const std::string& document) {
  QJsonParseError parsing_error;
  QJsonDocument json_document =
      QJsonDocument::fromJson(document.c_str(), &parsing_error);
  if (parsing_error.error != QJsonParseError::NoError) {
    return std::make_pair(JsonValue(ValueType::NULL_VALUE), false);
  }
  return std::make_pair(JsonValue(json_document.toVariant()), true);
}

bool utils::json::JsonValue::HasMember(const char* key) const {
  if (const QVariantMap* object_ptr = AsObjectPtr(storage_)) {
    return object_ptr->contains(key);
  }
  return false;
}

utils::json::JsonValue utils::json::JsonValue::Get(
    const char* key, const JsonValue& default_value) const {
  if (const QVariantMap* object_ptr = AsObjectPtr(storage_)) {
    return JsonValue((*object_ptr)[key]);
  }
  return default_value;
}

std::string utils::json::JsonValue::AsString() const {
  return storage_.toString().toStdString();
}

utils::json::JsonValue::Int utils::json::JsonValue::AsInt() const {
  return storage_.toLongLong();
}

utils::json::JsonValue::UInt utils::json::JsonValue::AsUInt() const {
  return storage_.toULongLong();
}

bool utils::json::JsonValue::AsBool() const {
  return storage_.toBool();
}

double utils::json::JsonValue::AsDouble() const {
  return storage_.toDouble();
}

utils::json::JsonValue::ArrayIndex utils::json::JsonValue::Size() const {
  if (const QVariantMap* object_ptr = AsObjectPtr(storage_)) {
    return object_ptr->size();
  } else if (const QVariantList* array_ptr = AsArrayPtr(storage_)) {
    return array_ptr->size();
  }
  return 0;
}

utils::json::ValueType::Type utils::json::JsonValue::Type() const {
  return GetType(storage_);
}

utils::json::JsonValue::Members utils::json::JsonValue::GetMemberNames() const {
  DCHECK(IsNull() || IsObject());
  if (const QVariantMap* object_ptr = AsObjectPtr(storage_)) {
    return GetMemberNamesFromObject(*object_ptr);
  }
  return Members();
}

bool utils::json::JsonValue::IsEmpty() const {
  const ValueType::Type type = Type();
  if (type == ValueType::OBJECT_VALUE || type == ValueType::ARRAY_VALUE) {
    return Size() == 0;
  } else if (type == ValueType::NULL_VALUE) {
    return true;
  }
  return false;
}

std::string utils::json::JsonValue::ToJson(const bool styled) const {
  const QJsonDocument::JsonFormat format =
      styled ? QJsonDocument::Indented : QJsonDocument::Compact;
  if (const QVariantMap* object_ptr = AsObjectPtr(storage_)) {
    return QJsonDocument(QJsonObject::fromVariantMap(*object_ptr))
        .toJson(format)
        .toStdString();
  } else if (const QVariantList* array_ptr = AsArrayPtr(storage_)) {
    return QJsonDocument(QJsonArray::fromVariantList(*array_ptr))
        .toJson(format)
        .toStdString();
  }
  return std::string();
}

void utils::json::JsonValue::Clear() {
  ClearStorage(storage_);
}

////////////////////////////////////////////////////////////////////////////////
/// class utils::json::JsonValue::iterator
////////////////////////////////////////////////////////////////////////////////

utils::json::JsonValue utils::json::JsonValue::iterator::key() const {
  if (const QVariantMap* object_ptr = AsObjectPtr(*storage_)) {
    return GetKeyByIndex(*object_ptr, index_);
  }
  return JsonValue();
}

////////////////////////////////////////////////////////////////////////////////
/// class utils::json::JsonValue::const_iterator
////////////////////////////////////////////////////////////////////////////////

utils::json::JsonValue utils::json::JsonValue::const_iterator::key() const {
  if (const QVariantMap* object_ptr = AsObjectPtr(*storage_)) {
    return GetKeyByIndex(*object_ptr, index_);
  }
  return JsonValue();
}
