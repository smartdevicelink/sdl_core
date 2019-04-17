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

#ifndef SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_NUMBER_SCHEMA_ITEM_H_
#define SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_NUMBER_SCHEMA_ITEM_H_

#include <typeinfo>
#include <limits>

#include "smart_objects/default_shema_item.h"
#include "smart_objects/schema_item_parameter.h"
#include "utils/convert_utils.h"
#include "utils/helpers.h"

namespace ns_smart_device_link {
namespace ns_smart_objects {
/**
 * @brief Number schema item.
 * @tparam NumberType Number type.
 **/
template <typename NumberType>
class TNumberSchemaItem : public CDefaultSchemaItem<NumberType> {
 public:
  /**
   * @brief Create a new schema item.
   * @param MinValue Minimum allowed value.
   * @param MaxValue Maximum allowed value.
   * @param DefaultValue Default value.
   * @return Shared pointer to a new schema item.
   **/
  static std::shared_ptr<TNumberSchemaItem> create(
      const TSchemaItemParameter<NumberType>& MinValue =
          TSchemaItemParameter<NumberType>(),
      const TSchemaItemParameter<NumberType>& MaxValue =
          TSchemaItemParameter<NumberType>(),
      const TSchemaItemParameter<NumberType>& DefaultValue =
          TSchemaItemParameter<NumberType>());

  /**
   * @brief Validate smart object.
   * @param Object Object to validate.
   * @param report__ object for reporting errors during validation
   * @param MessageVersion to check mobile RPC version against RPC Spec History
   * @param allow_unknown_enums
   *   false - unknown enum values (left as string values after applySchema)
   *   will be considered invalid.
   *   true - such values will be considered valid.
   * @return ns_smart_objects::errors::eType
   **/
  errors::eType validate(
      const SmartObject& Object,
      rpc::ValidationReport* report__,
      const utils::SemanticVersion& MessageVersion = utils::SemanticVersion(),
      const bool allow_unknown_enums = false) OVERRIDE;

 private:
  /**
   * @brief Get smart type for this NumberType.
   * @return Smart type for this NumberType.
   **/
  SmartType getSmartType() const OVERRIDE;
  NumberType getDefaultValue() const OVERRIDE;
  /**
   * @brief Constructor.
   *
   * @param MinValue Minimum allowed value.
   * @param MaxValue Maximum allowed value.
   * @param DefaultValue Default value.
   **/
  TNumberSchemaItem(const TSchemaItemParameter<NumberType>& MinValue,
                    const TSchemaItemParameter<NumberType>& MaxValue,
                    const TSchemaItemParameter<NumberType>& DefaultValue);
  /**
   * @brief Compares if param value type is correct
   **/
  bool isValidNumberType(SmartType type);

  /**
   * @brief Minimum and Maximum allowed values.
   **/
  const TSchemaItemParameter<NumberType> mMinValue;
  const TSchemaItemParameter<NumberType> mMaxValue;
  DISALLOW_COPY_AND_ASSIGN(TNumberSchemaItem);
};

template <typename NumberType>
std::shared_ptr<TNumberSchemaItem<NumberType> > TNumberSchemaItem<
    NumberType>::create(const TSchemaItemParameter<NumberType>& MinValue,
                        const TSchemaItemParameter<NumberType>& MaxValue,
                        const TSchemaItemParameter<NumberType>& DefaultValue) {
  return std::shared_ptr<TNumberSchemaItem<NumberType> >(
      new TNumberSchemaItem<NumberType>(MinValue, MaxValue, DefaultValue));
}

template <typename NumberType>
bool TNumberSchemaItem<NumberType>::isValidNumberType(SmartType type) {
  NumberType value(0);
  if ((SmartType_Double == type) && (typeid(double) == typeid(value))) {
    return true;
  } else if (((SmartType_Integer == type) || (SmartType_UInteger == type)) &&
             helpers::Compare<const std::type_info&, helpers::EQ, helpers::ONE>(
                 typeid(value),
                 typeid(int32_t),
                 typeid(uint32_t),
                 typeid(int64_t),
                 typeid(double))) {
    return true;
  }
  return false;
}

template <typename NumberType>
errors::eType TNumberSchemaItem<NumberType>::validate(
    const SmartObject& Object,
    rpc::ValidationReport* report__,
    const utils::SemanticVersion& MessageVersion,
    const bool allow_unknown_enums) {
  if (!isValidNumberType(Object.getType())) {
    SmartType expectedType = (typeid(double) == typeid(Object.getType()))
                                 ? SmartType_Double
                                 : SmartType_Integer;
    std::string validation_info =
        "Incorrect type, expected: " + SmartObject::typeToString(expectedType) +
        ", got: " + SmartObject::typeToString(Object.getType());
    report__->set_validation_info(validation_info);
    return errors::INVALID_VALUE;
  }
  NumberType value(0);
  if (typeid(int32_t) == typeid(value)) {
    value = utils::SafeStaticCast<int64_t, int32_t>(Object.asInt());
  } else if (typeid(uint32_t) == typeid(value)) {
    value = utils::SafeStaticCast<uint64_t, uint32_t>(Object.asUInt());
  } else if (typeid(double) == typeid(value)) {
    value = Object.asDouble();
  } else if (typeid(int64_t) == typeid(value)) {
    value = Object.asInt();
  } else if (typeid(uint64_t) == typeid(value)) {
    value = Object.asUInt();
  } else {
    NOTREACHED();
  }

  NumberType rangeLimit;
  if (mMinValue.getValue(rangeLimit) && (value < rangeLimit)) {
    std::stringstream stream;
    stream << "Value too small, got: " << value
           << ", minimum allowed: " << rangeLimit;
    std::string validation_info = stream.str();
    report__->set_validation_info(validation_info);
    return errors::OUT_OF_RANGE;
  }

  if (mMaxValue.getValue(rangeLimit) && (value > rangeLimit)) {
    std::stringstream stream;
    stream << "Value too large, got: " << value
           << ", maximum allowed: " << rangeLimit;
    std::string validation_info = stream.str();
    report__->set_validation_info(validation_info);
    return errors::OUT_OF_RANGE;
  }
  return errors::OK;
}

template <typename NumberType>
TNumberSchemaItem<NumberType>::TNumberSchemaItem(
    const TSchemaItemParameter<NumberType>& MinValue,
    const TSchemaItemParameter<NumberType>& MaxValue,
    const TSchemaItemParameter<NumberType>& DefaultValue)
    : CDefaultSchemaItem<NumberType>(DefaultValue)
    , mMinValue(MinValue)
    , mMaxValue(MaxValue) {}

template <typename NumberType>
NumberType TNumberSchemaItem<NumberType>::getDefaultValue() const {
  return NumberType(0);
}

/**
 * @brief Specialization of getSmartType for number types.
 * @return SmartType.
 **/
template <typename NumberType>
SmartType TNumberSchemaItem<NumberType>::getSmartType() const {
  DCHECK(!"Wrong type of template class");
  return SmartType_Invalid;
}
template <>
SmartType TNumberSchemaItem<int32_t>::getSmartType() const;

template <>
SmartType TNumberSchemaItem<uint32_t>::getSmartType() const;

template <>
SmartType TNumberSchemaItem<int64_t>::getSmartType() const;

template <>
SmartType TNumberSchemaItem<double>::getSmartType() const;

}  // namespace ns_smart_objects
}  // namespace ns_smart_device_link
#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_NUMBER_SCHEMA_ITEM_H_
