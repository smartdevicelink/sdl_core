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

#ifndef SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_ENUM_SCHEMA_ITEM_H_
#define SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_ENUM_SCHEMA_ITEM_H_

#include <string.h>

#include <map>
#include <set>
#include <string>

#include "utils/shared_ptr.h"
#include "smart_objects/default_shema_item.h"


namespace NsSmartDeviceLink {
namespace NsSmartObjects {

template <typename EnumType> class EnumConversionHelper;
/**
 * @brief Enumeration schema item.
 * @tparam EnumType Enumeration type.
 **/
template<typename EnumType>
class TEnumSchemaItem : public CDefaultSchemaItem<EnumType> {
 public:
  /**
   * @brief Create a new schema item.
   * @param AllowedElements Set of allowed enumeration elements.
   * @param DefaultValue Default value.
   * @return Shared pointer to a new schema item.
   **/
  static utils::SharedPtr<TEnumSchemaItem> create(
    const std::set<EnumType>& AllowedElements,
    const TSchemaItemParameter<EnumType>& DefaultValue =
      TSchemaItemParameter<EnumType>());
  /**
   * @brief Validate smart object.
   * @param Object Object to validate.
   * @return NsSmartObjects::Errors::eType
   **/
  Errors::eType validate(const SmartObject& Object) OVERRIDE;
  /**
   * @brief Apply schema.
   * This implementation checks if enumeration is represented as string
   * and tries to convert it to integer according to element-to-string
   * map.
   * @param Object Object to apply schema.
   * @param RemoveFakeParameters contains true if need to remove fake parameters
   * from smart object otherwise contains false.
   **/
  void applySchema(SmartObject& Object, const bool RemoveFakeParameters) OVERRIDE;
  /**
   * @brief Unapply schema.
   * @param Object Object to unapply schema.
   **/
  void unapplySchema(SmartObject& Object) OVERRIDE;

 private:
  /**
   * @brief Constructor.
   * @param AllowedElements Set of allowed enumeration elements.
   * @param DefaultValue Default value.
   **/
  TEnumSchemaItem(const std::set<EnumType>& AllowedElements,
                  const TSchemaItemParameter<EnumType>& DefaultValue);
  SmartType getSmartType() const OVERRIDE;
  EnumType getDefaultValue() const OVERRIDE;
  /**
   * @brief Set of allowed enumeration elements.
   **/
  const std::set<EnumType> mAllowedElements;
  /**
   * @brief Default value.
   **/
  const TSchemaItemParameter<EnumType> mDefaultValue;
  typedef EnumConversionHelper<EnumType> ConversionHelper;
  DISALLOW_COPY_AND_ASSIGN(TEnumSchemaItem);
};

struct CStringComparator {
  bool operator()(const char* a, const char* b) const {
    return strcmp(a, b) < 0;
  }
};

template <typename EnumType>
class EnumConversionHelper {
 public:
  typedef std::map<EnumType, const char*> EnumToCStringMap;
  typedef std::map<const char*, EnumType, CStringComparator> CStringToEnumMap;

  static const EnumToCStringMap& enum_to_cstring_map() {
    return enum_to_cstring_map_;
  }

  static const CStringToEnumMap& cstring_to_enum_map() {
    return cstring_to_enum_map_;
  }

  static bool CStringToEnum(const char* str, EnumType* value) {
    typename CStringToEnumMap::const_iterator it = cstring_to_enum_map().find(str);
    if (it == cstring_to_enum_map().end()) {
      return false;
    }
    if (value) {
      *value = it->second;
    }
    return true;
  }

  static bool EnumToCString(EnumType value, const char** str) {
    typename EnumToCStringMap::const_iterator it = enum_to_cstring_map().find(value);
    if (it == enum_to_cstring_map().end()) {
      return false;
    }
    if (str) {
      *str = it->second;
    }
    return true;
  }

  static bool StringToEnum(const std::string& str, EnumType* value) {
    return CStringToEnum(str.c_str(), value);
  }

  static bool EnumToString(const EnumType value, std::string* str) {
    const char* cstr;
    if (EnumToCString(value, &cstr)) {
      return false;
    }
    if (str) {
      *str = cstr;
    }
    return false;
  }

 private:
  static const char* const cstring_values_[];
  static const EnumType enum_values_[];
  static const EnumToCStringMap enum_to_cstring_map_;
  static const CStringToEnumMap cstring_to_enum_map_;

  struct Size {
    enum {value = sizeof(cstring_values_) / sizeof(cstring_values_[0])};
  };

  static EnumToCStringMap InitEnumToCStringMap() {
    DCHECK(Size::value == sizeof(enum_values_) / sizeof(enum_values_[0]));
    EnumToCStringMap result;
    for (size_t i = 0; i < Size::value; ++i) {
      result[enum_values_[i]] = cstring_values_[i];
    }
    return result;
  }

  static CStringToEnumMap InitCStringToEnumMap() {
    DCHECK(Size::value == sizeof(enum_values_) / sizeof(enum_values_[0]));
    CStringToEnumMap result;
    for (size_t i = 0; i < Size::value; ++i) {
      result[cstring_values_[i]] = enum_values_[i];
    }
    return result;
  }
  DISALLOW_COPY_AND_ASSIGN(EnumConversionHelper<EnumType>);
};


template<typename EnumType>
utils::SharedPtr<TEnumSchemaItem<EnumType> > TEnumSchemaItem<EnumType>::create(
  const std::set<EnumType>& AllowedElements,
  const TSchemaItemParameter<EnumType>& DefaultValue) {
  return new TEnumSchemaItem<EnumType>(AllowedElements, DefaultValue);
}

template<typename EnumType>
Errors::eType TEnumSchemaItem<EnumType>::validate(const SmartObject& Object) {
  if (SmartType_Integer != Object.getType()) {
    return Errors::INVALID_VALUE;
  }
  if (mAllowedElements.find(static_cast<EnumType>(Object.asInt()))
      == mAllowedElements.end()) {
    return Errors::OUT_OF_RANGE;
  }
  return Errors::OK;
}

template<typename EnumType>
void TEnumSchemaItem<EnumType>::applySchema(SmartObject& Object, const bool RemoveFakeParameters) {
  if (SmartType_String == Object.getType()) {
    EnumType enum_val = static_cast<EnumType>(-1);
    if (ConversionHelper::StringToEnum(Object.asString(), &enum_val)) {
      Object = enum_val;
    }
  }
}

template<typename EnumType>
void TEnumSchemaItem<EnumType>::unapplySchema(SmartObject& Object) {
  if (SmartType_Integer == Object.getType()) {
    const char* str;
    if (ConversionHelper::EnumToCString(static_cast<EnumType>(Object.asInt()), &str)) {
      Object = str;
    }
  }
}

template<typename EnumType>
SmartType TEnumSchemaItem<EnumType>::getSmartType() const {
  return SmartType_Integer;
}
template<typename EnumType>
EnumType TEnumSchemaItem<EnumType>::getDefaultValue() const {
  return EnumType::INVALID_ENUM;
}

template<typename EnumType>
TEnumSchemaItem<EnumType>::TEnumSchemaItem(
  const std::set<EnumType>& AllowedElements,
  const TSchemaItemParameter<EnumType>& DefaultValue)
  : CDefaultSchemaItem<EnumType>(DefaultValue),
    mAllowedElements(AllowedElements) {
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_ENUM_SCHEMA_ITEM_H_
