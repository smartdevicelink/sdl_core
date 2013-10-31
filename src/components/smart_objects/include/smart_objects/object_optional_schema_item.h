/**
 * @file CObjectSchemaItem.hpp
 * @brief CObjectSchemaItem header file.
 */
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
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THEORY

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
 * @brief Object schema item for object which can have optional
 *        parameter with any(unknwon when describing scema) name.
 *
 * Please not that optioanl parameter can be also as mandatory.
 */
class ObjectOptionalSchemaItem : public CObjectSchemaItem {
  public:
     /**
      * @brief Generic field names for optional members of smart obejct.
      *
      * Names should be uniq, not to iterfere with all possible regualr names
      */
    static const char* sOptionalGenericFieldName1;

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
    static utils::SharedPtr<ObjectOptionalSchemaItem> Create(
          const std::map<std::string, CObjectSchemaItem::SMember> & members);

     /**
      * @brief Validate smart object.
      *
      * @param Object Object to validate.
      *
      * @return NsSmartObjects::Errors::eType
      **/
    virtual Errors::eType Validate(const SmartObject & object);

  private:
    /**
      * @brief Constructor.
      *
      * @param Members Map of member name to SMember structure
      *                describing the object member.
      * @param ObjectHasOptioanalMembers Smart object can have
      *                                  optioanal members
      **/
    ObjectOptionalSchemaItem(const std::map<std::string,
                             CObjectSchemaItem::SMember> & members);

    /**
      * @brief Copy constructor.
      *
      * Not implemented to prevent misuse.
      *
      * @param Other Other schema item.
      **/
    ObjectOptionalSchemaItem(const ObjectOptionalSchemaItem & Other);

    /**
      * @brief Assignment operator.
      *
      * Not implemented to prevent misuse.
      *
      * @param Other Other schema item.
      *
      * @return Not implemented.
      **/
    ObjectOptionalSchemaItem & operator =(
        const ObjectOptionalSchemaItem & Other);

    /**
      * @brief Tells whether Name is generic optional
      *
      * @param name Name field (key) in object
      *
      * @return Whether FieldName is generic optional
      */
    bool IsOptionalName(std::string name);
};

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink

#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_OBJECT_OPTIONAL_SCHEMA_ITEM_H_
