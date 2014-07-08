// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_NUMBER_SCHEMA_ITEM_H_
#define SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_NUMBER_SCHEMA_ITEM_H_

#include <typeinfo>
#include <string>

#include "utils/shared_ptr.h"

#include "smart_objects/smart_object.h"
#include "smart_objects/schema_item.h"
#include "smart_objects/schema_item_parameter.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

/**
 * @brief Number schema item.
 *
 * @tparam NumberType Number type.
 **/
template<typename NumberType>
class TNumberSchemaItem : public ISchemaItem {
 public:
  /**
   * @brief Create a new schema item.
   *
   * @param MinValue Minimum allowed value.
   * @param MaxValue Maximum allowed value.
   * @param DefaultValue Default value.
   *
   * @return Shared pointer to a new schema item.
   **/
  static utils::SharedPtr<TNumberSchemaItem> create(
      const TSchemaItemParameter<NumberType>& MinValue = TSchemaItemParameter<
          NumberType>(),
      const TSchemaItemParameter<NumberType>& MaxValue = TSchemaItemParameter<
          NumberType>(),
      const TSchemaItemParameter<NumberType>& DefaultValue =
          TSchemaItemParameter<NumberType>());

  /**
   * @brief Validate smart object.
   *
   * @param Object Object to validate.
   *
   * @return NsSmartObjects::Errors::eType
   **/
  virtual Errors::eType validate(const SmartObject& Object);

  /**
   * @brief Set default value to an object.
   *
   * @param Object Object to set default value.
   *
   * @return true if default value was successfully set, false otherwise.
   **/
  // TODO(cpplint): Is this a non-const reference?
  // If so, make const or use a pointer.
  virtual bool setDefaultValue(SmartObject& Object);

  /**
   * @brief Check if object has default value.
   *
   * @param Object Object to set default value.
   *
   * @return true if default value was successfully set, false otherwise.
   **/
  virtual bool hasDefaultValue(SmartObject& Object);

  /**
   * @brief Build smart object by smart schema having copied matched
   *        parameters from pattern smart object
   *
   * @param pattern_object pattern object
   * @param result_object object to build
   */
  virtual void BuildObjectBySchema(const SmartObject& pattern_object,
                                   SmartObject& result_object);

  virtual ~TNumberSchemaItem() {
  }

 private:
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
   * @brief Get smart type for this NumberType.
   *
   * @return Smart type for this NumberType.
   **/
  static SmartType getSmartType();

  bool isNumberType(SmartType type);

  /**
   * @brief Copy constructor.
   *
   * Not implemented to prevent misuse.
   *
   * @param Other Other schema item.
   **/
  TNumberSchemaItem(const TNumberSchemaItem<NumberType>& Other);

  /**
   * @brief Assignment operator.
   *
   * Not implemented to prevent misuse.
   *
   * @param Other Other schema item.
   *
   * @return Not implemented.
   **/
  TNumberSchemaItem & operator =(const TNumberSchemaItem<NumberType>& Other);

  /**
   * @brief Minimum allowed value.
   **/
  const TSchemaItemParameter<NumberType> mMinValue;

  /**
   * @brief Maximum allowed value.
   **/
  const TSchemaItemParameter<NumberType> mMaxValue;

  /**
   * @brief Default value.
   **/
  const TSchemaItemParameter<NumberType> mDefaultValue;
};

/**
 * @brief Specialization of getSmartType for int32_t.
 *
 * @return SmartType_Integer.
 **/
template<>
SmartType TNumberSchemaItem<int32_t>::getSmartType();

/**
 * @brief Specialization of getSmartType for long.
 *
 * @return SmartType_Integer.
 **/
template<>
SmartType TNumberSchemaItem<uint32_t>::getSmartType();

/**
 * @brief Specialization of getSmartType for double.
 *
 * @return SmartType_Double.
 **/
template<>
SmartType TNumberSchemaItem<double>::getSmartType();

template<typename NumberType>
utils::SharedPtr<TNumberSchemaItem<NumberType> >
TNumberSchemaItem<NumberType>::create(
    const TSchemaItemParameter<NumberType>& MinValue,
    const TSchemaItemParameter<NumberType>& MaxValue,
    const TSchemaItemParameter<NumberType>& DefaultValue) {
  return new TNumberSchemaItem<NumberType>(MinValue, MaxValue, DefaultValue);
}

template<typename NumberType>
bool TNumberSchemaItem<NumberType>::isNumberType(SmartType type) {
  if (SmartType_Integer == type || SmartType_Double == type) {
    return true;
  }

  return false;
}

template<typename NumberType>
Errors::eType TNumberSchemaItem<NumberType>::validate(
    const SmartObject& Object) {
  Errors::eType result = Errors::ERROR;

  if (isNumberType(Object.getType())) {
    result = Errors::OK;
    NumberType value;

    if (std::string("i") == typeid(value).name()) {
      value = Object.asInt();
    } else if (std::string("j") == typeid(value).name()) {
      value = Object.asUInt();
    } else if (std::string("d") == typeid(value).name()) {
      value = Object.asDouble();
    } else {
      NOTREACHED();
    }

    NumberType rangeLimit;

    if (true == mMinValue.getValue(rangeLimit)) {
      if (value < rangeLimit) {
        result = Errors::OUT_OF_RANGE;
      }
    }

    if ((Errors::OK == result) && (true == mMaxValue.getValue(rangeLimit))) {
      if (value > rangeLimit) {
        result = Errors::OUT_OF_RANGE;
      }
    }
  } else {
    result = Errors::INVALID_VALUE;
  }

  return result;
}

template<typename NumberType>
bool TNumberSchemaItem<NumberType>::setDefaultValue(SmartObject& Object) {
  bool result = false;
  NumberType value;

  if (true == mDefaultValue.getValue(value)) {
    Object = value;
    result = true;
  }

  return result;
}

template<typename NumberType>
bool TNumberSchemaItem<NumberType>::hasDefaultValue(SmartObject& Object) {
  bool result = false;
  NumberType value;

  if (true == mDefaultValue.getValue(value)) {
    Object = value;
    result = true;
  }

  return result;
}

template<typename NumberType>
void TNumberSchemaItem<NumberType>::BuildObjectBySchema(
    const SmartObject& pattern_object, SmartObject& result_object) {

  if (getSmartType() == pattern_object.getType()) {
    result_object = pattern_object;  // TODO(AK): check this..
  } else {
    bool result = setDefaultValue(result_object);
    if (false == result) {
      result_object = static_cast<NumberType>(0);
    }
  }
}

template<typename NumberType>
TNumberSchemaItem<NumberType>::TNumberSchemaItem(
    const TSchemaItemParameter<NumberType>& MinValue,
    const TSchemaItemParameter<NumberType>& MaxValue,
    const TSchemaItemParameter<NumberType>& DefaultValue)
    : mMinValue(MinValue),
      mMaxValue(MaxValue),
      mDefaultValue(DefaultValue) {
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink

#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_NUMBER_SCHEMA_ITEM_H_
