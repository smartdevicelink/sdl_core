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
#ifndef SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SCHEMA_ITEM_H_
#define SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SCHEMA_ITEM_H_

#include <stdlib.h>
#include "utils/shared_ptr.h"

#include "smart_objects/errors.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class SmartObject;

/**
 * @brief Base schema item.
 **/
class ISchemaItem {
 public:
  /**
   * @brief Validate object.
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
   * @brief Apply schema.
   *
   * @param Object Object to apply schema.
   * @param RemoveFakeParameters contains true if need to remove fake parameters
   * from smart object otherwise contains false.
   **/
  virtual void applySchema(NsSmartDeviceLink::NsSmartObjects::SmartObject& Object,
                           const bool RemoveFakeParameters);

  /**
   * @brief Unapply schema.
   *
   * @param Object Object to unapply schema.
   **/
  virtual void unapplySchema(
      NsSmartDeviceLink::NsSmartObjects::SmartObject& Object);

  /**
   * @brief Build smart object by smart schema having copied matched
   *        parameters from pattern smart object
   *
   * @param pattern_object pattern object
   * @param result_object object to build
   */
  virtual void BuildObjectBySchema(
      const SmartObject& pattern_object, SmartObject& result_object);

  /**
   * @brief Get value param, depends of children
   *
   * @return value of any parameter
   */
  virtual size_t GetMemberSize();

  virtual ~ISchemaItem() { }
};
typedef utils::SharedPtr<ISchemaItem> ISchemaItemPtr;
}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SCHEMA_ITEM_H_
