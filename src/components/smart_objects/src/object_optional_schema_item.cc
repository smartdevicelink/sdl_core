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
#include "smart_objects/object_optional_schema_item.h"
#include "smart_objects/smart_object.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

const std::string ObjectOptionalSchemaItem::kOptionalGenericFieldName
  = std::string("[<-- @@@@$$$&&& unique uancdm,vn 991188 --->]");

//----------------------------------------------------------------------------

utils::SharedPtr<ObjectOptionalSchemaItem> ObjectOptionalSchemaItem::create(
  const Members& members) {
  return new ObjectOptionalSchemaItem(members);
}

//----------------------------------------------------------------------------

Errors::eType ObjectOptionalSchemaItem::validate(const SmartObject& object) {

  if (NsSmartDeviceLink::NsSmartObjects::SmartType_Map != object.getType()) {
    return Errors::INVALID_VALUE;
  }
  std::set<std::string> object_keys = object.enumerate();

  for (Members::const_iterator it = mMembers.begin(); it != mMembers.end(); ++it) {
    const std::string& key = it->first;
    if (key == kOptionalGenericFieldName) {
      continue;
    }
    const SMember& member = it->second;

    Key_Iterator key_it = object_keys.find(key);
    if (object_keys.end() != key_it) {
      const Errors::eType result = member.mSchemaItem->validate(object.getElement(key));
      if (Errors::OK != result) {
        return result;
      }
      // remove handled key
      object_keys.erase(key_it);
    } else if (member.mIsMandatory && !IsOptionalName(key)) {
      return Errors::MISSING_MANDATORY_PARAMETER;
    }
  }
  // Smart object has unhandled keys
  const bool has_keys = !object_keys.empty();

  // this key is supposed as optional
  Members::const_iterator optional_iterator = mMembers.find(kOptionalGenericFieldName);
  if (mMembers.end() == optional_iterator) {
    if (has_keys) {
      // all remaining keys are unexpected
      return Errors::UNEXPECTED_PARAMETER;
    }
    return Errors::OK;
  }

  const SMember& optional_member = optional_iterator->second;

  if (optional_member.mIsMandatory
      && !has_keys) {
    return Errors::MISSING_MANDATORY_UNTITLED_PARAMETER;
  }

  // for all remaining keys
  for (Key_Iterator k = object_keys.begin(); k != object_keys.end(); ++k) {
    const std::string& key = *k;
    // validate optional (having any name) object member
    const Errors::eType result =
      optional_member.mSchemaItem->validate(object.getElement(key));
    if (Errors::OK != result) {
      return result;
    }
  }
  return Errors::OK;
}

//----------------------------------------------------------------------------

bool ObjectOptionalSchemaItem::IsOptionalName(const std::string& name) const {
  return (0 == name.compare(kOptionalGenericFieldName));
}

//----------------------------------------------------------------------------

ObjectOptionalSchemaItem::ObjectOptionalSchemaItem(const Members& members):
  CObjectSchemaItem(members) {
}

//----------------------------------------------------------------------------

std::set<std::string>
ObjectOptionalSchemaItem::GetOptionalObjectKeys(const SmartObject& root_obj) const {
  std::set<std::string> optional_objects;

  const std::set<std::string> object_keys = root_obj.enumerate();
  for (Key_Iterator k = object_keys.begin(); k != object_keys.end(); ++k) {
    // if an object is not among the schema items then it is optional
    if (mMembers.end() == mMembers.find(*k)) {
      optional_objects.insert(*k);
    }
  }

  return optional_objects;
}

//----------------------------------------------------------------------------

void ObjectOptionalSchemaItem::IterateOverOptionalItems(SmartObject& object,
    void (ISchemaItem::* action)(SmartObject&) ) {
  Members::const_iterator it = mMembers.find(kOptionalGenericFieldName);
  if (it == mMembers.end()) {
    return;             // There are no optional items
  }

  const SMember& member = it->second;
  ISchemaItem* schema = member.mSchemaItem.get();

  // Then apply schema for all the optional objects
  const std::set<std::string> optionals = GetOptionalObjectKeys(object);
  for (Key_Iterator key = optionals.begin(); key != optionals.end(); ++key) {
    (schema->*action)(object[*key]);
  }
}

//----------------------------------------------------------------------------

void ObjectOptionalSchemaItem::applySchema(SmartObject& object) {
  // At first apply schema for the regular objects
  CObjectSchemaItem::applySchema(object);

  IterateOverOptionalItems(object, &ISchemaItem::applySchema);
}

//----------------------------------------------------------------------------

void ObjectOptionalSchemaItem::unapplySchema(SmartObject& object) {
  CObjectSchemaItem::unapplySchema(object);

  IterateOverOptionalItems(object, &ISchemaItem::unapplySchema);
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink

