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
#ifndef SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SMART_SCHEMA_H_
#define SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SMART_SCHEMA_H_

#include "utils/macro.h"
#include "smart_objects/schema_item.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class SmartObject;

/**
 * @brief Smart Schema.
 */
class CSmartSchema FINAL {
 public:
  /**
   * @brief Constructor.
   *
   * Default constructor. Creates empty schema.
   */
  CSmartSchema();

  /**
   * @brief Constructor.
   *
   * Creates schema with given root schema item.
   *
   * @param SchemaItem Root schema item.
   */
  explicit CSmartSchema(const ISchemaItemPtr SchemaItem);

  /**
   * @brief Validate smart object.
   *
   * @param Object SmartObject to validate.
   *
   * @return Result of validation.
   */
  Errors::eType validate(const SmartObject& Object) const;

  /**
   * @brief Set new root schema item.
   *
   * @param SchemaItem Root schema item.
   */
  void setSchemaItem(const ISchemaItemPtr SchemaItem);

  /**
   * @brief Apply schema.
   *
   * @param Object Object to apply schema.
   **/
  void applySchema(SmartObject& Object);

  /**
   * @brief The reverse SmartObject conversion using schema.
   *
   * @param object Object to convert.
   */
  // TODO(cpplint): Is this a non-const reference?
  // If so, make const or use a pointer.
  void unapplySchema(SmartObject& object);

  /**
   * @brief Build smart object by smart schema having copied matched
   *        parameters from pattern smart object
   *
   * @param pattern_object pattern object
   * @param result_object object to build
   */
  void BuildObjectBySchema(const SmartObject& pattern_object,
                           SmartObject& result_object) const;

 protected:
  /**
   * @brief Root schema item.
   */
  ISchemaItemPtr mSchemaItem;
};
}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SMART_SCHEMA_H_
