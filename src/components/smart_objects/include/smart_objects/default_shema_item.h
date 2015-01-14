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
#ifndef SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_DEFAULT_SHEMA_ITEM_H_
#define SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_DEFAULT_SHEMA_ITEM_H_

#include "utils/macro.h"
#include "utils/shared_ptr.h"

#include "smart_objects/schema_item.h"
#include "smart_objects/schema_item_parameter.h"
#include "smart_objects/smart_object.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

/**
 * @brief Default schema item.
 **/
template<typename Type>
class CDefaultSchemaItem : public ISchemaItem {
 public:
  typedef TSchemaItemParameter<Type> ParameterType;
  /**
   * @brief Validate smart object.
   * @param Object Object to validate.
   * @return NsSmartObjects::Errors::eType
   **/
  Errors::eType validate(const SmartObject& Object) OVERRIDE;

  /**
   * @brief Set default value to an object.
   * @param Object Object to set default value.
   * @return true if default value was successfully set, false otherwise.
   **/
  bool setDefaultValue(SmartObject& Object) OVERRIDE;

  /**
   * @brief Build smart object by smart schema having copied matched
   *        parameters from pattern smart object
   * @param pattern_object pattern object
   * @param result_object object to build
   */
  void BuildObjectBySchema(const SmartObject& pattern_object,
                           SmartObject& result_object) OVERRIDE;

 protected:
  explicit CDefaultSchemaItem(const ParameterType& DefaultValue);
  /**
   * @brief SmartType getter for primitive types common logic
   * @return SmartType for ShemaItem
   */
  virtual SmartType getSmartType() const = 0;
  /**
   * @brief Default value getter for primitive types common logic
   * @return Default value for selected type
   */
  virtual Type getDefaultValue() const = 0;
  /**
   * @param DefaultValue Default value of a parameter.
   */
  const ParameterType mDefaultValue;

 private:
  DISALLOW_COPY_AND_ASSIGN(CDefaultSchemaItem<Type>);
};

template<typename Type>
CDefaultSchemaItem<Type>::CDefaultSchemaItem(const ParameterType& DefaultValue)
  : mDefaultValue(DefaultValue) {
}

template<typename Type>
Errors::eType CDefaultSchemaItem<Type>::validate(const SmartObject& Object) {
  return (getSmartType() == Object.getType()) ? Errors::OK : Errors::INVALID_VALUE;
}

template<typename Type>
bool CDefaultSchemaItem<Type>::setDefaultValue(SmartObject& Object) {
  Type value;
  if (mDefaultValue.getValue(value)) {
    Object = value;
    return true;
  }
  return false;
}

template<typename Type>
void CDefaultSchemaItem<Type>::BuildObjectBySchema(
  const SmartObject& pattern_object, SmartObject& result_object) {
  if (getSmartType() == pattern_object.getType()) {
    result_object = pattern_object;
  } else {
    if (!setDefaultValue(result_object)) {
      result_object = getDefaultValue();
    }
  }
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_DEFAULT_SHEMA_ITEM_H_
