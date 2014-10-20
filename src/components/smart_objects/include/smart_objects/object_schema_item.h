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
#ifndef SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_OBJECT_SCHEMA_ITEM_H_
#define SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_OBJECT_SCHEMA_ITEM_H_

#include <map>
#include <string>
#include <set>

#include "utils/macro.h"
#include "utils/shared_ptr.h"

#include "smart_objects/schema_item.h"
#include "smart_objects/schema_item_parameter.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
/**
 * @brief Object schema item.
 **/
class CObjectSchemaItem : public ISchemaItem {
 public:
  /**
   * @brief Object member.
   **/
  struct SMember {
    /**
     * @brief Default constructor.
     **/
    SMember();
    /**
     * @brief Constructor.
     * @param SchemaItem Member schema item.
     * @param IsMandatory true if member is mandatory, false
     *                    otherwise. Defaults to true.
     **/
    SMember(const ISchemaItemPtr SchemaItem,
            const bool IsMandatory = true);
    /**
     * @brief Member schema item.
     **/
    ISchemaItemPtr mSchemaItem;
    /**
     * @brief true if member is mandatory, false otherwise.
     **/
    bool mIsMandatory;
  };
  typedef std::map<std::string, SMember> Members;
  /**
   * @brief Create a new schema item.
   *
   * @param Members Map of member name to SMember structure
   *                describing the object member.
   *
   * @return Shared pointer to a new schema item.
   **/
  static utils::SharedPtr<CObjectSchemaItem> create(const Members& Members);
  /**
   * @brief Validate smart object.
   * @param Object Object to validate.
   * @return NsSmartObjects::Errors::eType
   **/
  Errors::eType validate(const SmartObject& Object) OVERRIDE;
  /**
   * @brief Apply schema.
   * @param Object Object to apply schema.
   **/
  void applySchema(SmartObject& Object) OVERRIDE;
  /**
   * @brief Unapply schema.
   * @param Object Object to unapply schema.
   **/
  void unapplySchema(SmartObject& Object) OVERRIDE;
  /**
   * @brief Build smart object by smart schema having copied matched
   *        parameters from pattern smart object
   * @param pattern_object pattern object
   * @param result_object object to build
   */
  void BuildObjectBySchema(const SmartObject& pattern_object,
                           SmartObject& result_object) OVERRIDE;
  /**
   * @brief Get size mMembers map
   * @return Size of mMembers
   */
  size_t GetMemberSize() OVERRIDE;

 protected:
  /**
   * @brief Constructor.
   * @param Members Map of member name to SMember structure
   *                describing the object member.
   **/
  CObjectSchemaItem(const Members& Members);
  /**
   * @brief Map of member name to SMember structure describing the object member.
   **/
  const Members mMembers;
  DISALLOW_COPY_AND_ASSIGN(CObjectSchemaItem);
};
}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_OBJECT_SCHEMA_ITEM_H_
