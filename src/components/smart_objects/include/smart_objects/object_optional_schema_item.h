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
#ifndef SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_OBJECT_OPTIONAL_SCHEMA_ITEM_H_
#define SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_OBJECT_OPTIONAL_SCHEMA_ITEM_H_

#include <map>
#include <string>

#include "utils/shared_ptr.h"
#include "smart_objects/schema_item.h"
#include "smart_objects/object_schema_item.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
/**
 * @brief Object schema item for object which can have "optional"
 *        parameter with any(unknown when describing schema) name.
 * Please note that optional parameter can be also as mandatory.
 */
class ObjectOptionalSchemaItem : public CObjectSchemaItem {
 public:
  /**
   * @brief Generic field name for optional members of smart object.
   * Name should be unique, not to itersect with all possible regular names
   */
  static const std::string kOptionalGenericFieldName;

  /**
   * @brief Create a new schema item.
   *
   * @param Members Map of member name to SMember structure
   *                describing the object member.
   * @param ObjectHasOptioanalMembers Smart object can have
   *                                  optioanal members
   *
   * @return Shared pointer to a new schema item.
   **/
  static utils::SharedPtr<ObjectOptionalSchemaItem> create(const Members& members);

  /**
   * @brief Validate smart object.
   *
   * @param Object Object to validate.
   *
   * @return NsSmartObjects::Errors::eType
   **/
  Errors::eType validate(const SmartObject& object) OVERRIDE;

  /**
   * @brief Apply schema.
   *
   * @param object Object to apply schema.
   **/
  void applySchema(SmartObject& object) OVERRIDE;

  /**
   * @brief Unapply schema.
   *
   * @param object Object to unapply schema.
   **/
  void unapplySchema(SmartObject& object) OVERRIDE;

 private:
  /**
    * @brief Constructor.
    *
    * @param Members Map of member name to SMember structure
    *                describing the object member.
    * @param ObjectHasOptioanalMembers Smart object can have
    *                                  any name members
    **/
  ObjectOptionalSchemaItem(const Members& members);
  /**
    * @brief Tells whether name (normally schema item field name)
    *        may have any string value
    *
    * @param name Name of field (key) in object
    *
    * @return Whether schema item with name may have any string value as name
    */
  bool IsOptionalName(const std::string& name) const;

  /**
    * @brief Extracts a set of sub object keys from the root object that are
    * optional.
    *
    * @param Object Object from which the sub object keys will be extracted.
    *
    * @return A set of keys of sub-objects that are optional.
    */
  std::set<std::string> GetOptionalObjectKeys(const SmartObject& root_obj) const;

  /**
   * @brief Method iterates over optional sub-objects.
   *
   *  The method iterates over optional sub-objects in the root object
   *  and performs action on each object.
   *  The action is intended as either "applySchema" or "unapplySchema"
   *
   *  @param object The root object. Can be modified by "action".
   *  @param action Pointer to the method of ISchemaItem. Either "applySchema"
   *                or "unapplySchema"
   */
  void IterateOverOptionalItems(SmartObject& object,
                                void (ISchemaItem:: *action)(SmartObject&));
  DISALLOW_COPY_AND_ASSIGN(ObjectOptionalSchemaItem);
};

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_OBJECT_OPTIONAL_SCHEMA_ITEM_H_
