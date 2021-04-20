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

#include "rpc_base/validation_report.h"

#include "smart_objects/errors.h"

#include <memory>
#include <vector>
#include "boost/optional/optional.hpp"
#include "utils/macro.h"
#include "utils/semantic_version.h"

namespace ns_smart_device_link {
namespace ns_smart_objects {
class SmartObject;
class SMember;

enum TypeID {
  TYPE_NONE,
  TYPE_OBJECT,
  TYPE_ARRAY,
  TYPE_STRING,
  TYPE_NUMBER,
  TYPE_ENUM,
  TYPE_BOOLEAN
};

/**
 * @brief Base schema item.
 **/
class ISchemaItem {
 public:
  /**
   * @brief Validate smart object.
   *
   * @param Object Object to validate.
   * @param report object for reporting errors during validation
   * message if an error occurs
   * @param MessageVersion to check mobile RPC version against RPC Spec History
   * @param allow_unknown_enums
   *   false - unknown enum values (left as string values after applySchema)
   *   will be considered invalid.
   *   true - such values will be considered valid.
   * @return ns_smart_objects::errors::eType
   **/
  virtual errors::eType validate(
      const SmartObject& Object,
      rpc::ValidationReport* report,
      const utils::SemanticVersion& MessageVersion = utils::SemanticVersion(),
      const bool allow_unknown_enums = false);

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
   * @brief Filter invalid enum values
   *
   * @param Object Object to check for invalid enum values
   * @param MessageVersion the version of the schema to use for validation
   * @param report object for reporting enums which were removed during the
   * process
   *
   * @return true if the value being checked should be filtered, false otherwise
   **/
  virtual bool filterInvalidEnums(SmartObject& Object,
                                  const utils::SemanticVersion& MessageVersion,
                                  rpc::ValidationReport* report);

  /**
   * @brief Apply schema.
   *
   * @param Object Object to apply schema.
   * @param remove_unknown_parameters contains true if need to remove unknown
   * parameters from smart object, otherwise contains false.
   * @param MessageVersion the version of the schema to be applied
   **/
  virtual void applySchema(
      ns_smart_device_link::ns_smart_objects::SmartObject& Object,
      const bool remove_unknown_parameters,
      const utils::SemanticVersion& MessageVersion = utils::SemanticVersion());

  /**
   * @brief Unapply schema.
   *
   * @param Object Object to unapply schema.
   * @param remove_unknown_parameters contains true if need to remove unknown
   *parameters
   **/
  virtual void unapplySchema(
      ns_smart_device_link::ns_smart_objects::SmartObject& Object,
      const bool remove_unknown_parameters = true);

  /**
   * @brief Build smart object by smart schema having copied matched
   *        parameters from pattern smart object
   *
   * @param pattern_object pattern object
   * @param result_object object to build
   */
  virtual void BuildObjectBySchema(const SmartObject& pattern_object,
                                   SmartObject& result_object);

  virtual boost::optional<SMember&> GetMemberSchemaItem(
      const std::string& member_key) {
    UNUSED(member_key);
    return boost::optional<SMember&>();
  }

  virtual void AddMemberSchemaItem(const std::string& member_key,
                                   SMember& member) {
    UNUSED(member_key);
    UNUSED(member);
  }
  /**
   * @brief Get value param, depends of children
   *
   * @return value of any parameter
   */
  virtual size_t GetMemberSize();

  /**
   * @brief Get type ID of schema
   *
   * @return The type ID of this schema
   */
  virtual TypeID GetType();

  virtual ~ISchemaItem() {}
};
typedef std::shared_ptr<ISchemaItem> ISchemaItemPtr;
}  // namespace ns_smart_objects
}  // namespace ns_smart_device_link
#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SCHEMA_ITEM_H_
