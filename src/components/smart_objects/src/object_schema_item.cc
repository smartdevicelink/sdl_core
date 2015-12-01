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
#include "smart_objects/object_schema_item.h"

#include <algorithm>

#include "smart_objects/always_false_schema_item.h"
#include "smart_objects/smart_object.h"

namespace {
const char connection_key[] = "connection_key";
const char binary_data[] = "binary_data";
const char app_id[] = "appID";
}
namespace NsSmartDeviceLink {
namespace NsSmartObjects {

CObjectSchemaItem::SMember::SMember()
  : mSchemaItem(CAlwaysFalseSchemaItem::create()),
    mIsMandatory(true) {
}

CObjectSchemaItem::SMember::SMember(const ISchemaItemPtr SchemaItem,
                                    const bool IsMandatory)
  : mSchemaItem(SchemaItem),
    mIsMandatory(IsMandatory) {
}

utils::SharedPtr<CObjectSchemaItem> CObjectSchemaItem::create(const Members& members) {
  return new CObjectSchemaItem(members);
}

Errors::eType CObjectSchemaItem::validate(const SmartObject& object) {
  if (SmartType_Map != object.getType()) {
    return Errors::INVALID_VALUE;
  }

  std::set<std::string> object_keys = object.enumerate();

  for (Members::const_iterator it = mMembers.begin(); it != mMembers.end(); ++it) {
    const std::string& key = it->first;
    const SMember& member = it->second;

    std::set<std::string>::const_iterator key_it = object_keys.find(key);
    if (object_keys.end() == key_it) {
      if (member.mIsMandatory) {
        return Errors::MISSING_MANDATORY_PARAMETER;
      }
      continue;
    }
    const SmartObject& field = object.getElement(key);
    const Errors::eType result = member.mSchemaItem->validate(field);
    if (Errors::OK != result) {
      return result;
    }
    object_keys.erase(key_it);
  }
  return Errors::OK;
}

void CObjectSchemaItem::applySchema(SmartObject& Object,
                                    const bool RemoveFakeParameters) {
  if (SmartType_Map != Object.getType()) {
    return;
  }

  if (RemoveFakeParameters) {
    RemoveFakeParams(Object);
  }

  SmartObject default_value;
  for (Members::const_iterator it = mMembers.begin(); it != mMembers.end(); ++it) {
    const std::string& key = it->first;
    const SMember& member = it->second;
    if (!Object.keyExists(key)) {
      if (member.mSchemaItem->setDefaultValue(default_value)) {
        Object[key] = default_value;
        member.mSchemaItem->applySchema(Object[key], RemoveFakeParameters);
      }
    } else {
      member.mSchemaItem->applySchema(Object[key], RemoveFakeParameters);
    }
  }
}

void CObjectSchemaItem::unapplySchema(SmartObject& Object) {
  if (SmartType_Map != Object.getType()) {
    return;
  }
  for (SmartMap::const_iterator it = Object.map_begin();
       it != Object.map_end();) {
    const std::string& key = it->first;
    // move next to avoid wrong iterator on erase
    ++it;
    if (mMembers.end() == mMembers.find(key)) {
      // remove fake params
      Object.erase(key);
    }
  }

  for (Members::const_iterator it = mMembers.begin(); it != mMembers.end(); ++it) {
    const std::string& key = it->first;
    const SMember& member = it->second;
    if (Object.keyExists(key)) {
      member.mSchemaItem->unapplySchema(Object[key]);
    }
  }
}

void CObjectSchemaItem::BuildObjectBySchema(
  const SmartObject& pattern_object, SmartObject& result_object) {
  result_object = SmartObject(SmartType_Map);
  const bool pattern_is_map = SmartType_Map == pattern_object.getType();

  for (Members::const_iterator it = mMembers.begin(); it != mMembers.end(); ++it) {
    const std::string& key = it->first;
    const SMember& member = it->second;
    const bool pattern_exists = pattern_is_map && pattern_object.keyExists(key);
    member.mSchemaItem->BuildObjectBySchema(
          pattern_exists ? pattern_object.getElement(key) : SmartObject(),
          result_object[key]);
  }
}

size_t CObjectSchemaItem::GetMemberSize() {
  return mMembers.size();
}

CObjectSchemaItem::CObjectSchemaItem(const Members& members)
  : mMembers(members) {}

void CObjectSchemaItem::RemoveFakeParams(SmartObject& Object) {
  for (SmartMap::const_iterator it = Object.map_begin(); it != Object.map_end(); ) {
    const std::string& key = it->first;
    if (mMembers.end() == mMembers.find(key)
        // FIXME(EZamakhov): Remove illegal usage of filed in AM
        && key.compare(connection_key) != 0
        && key.compare(binary_data) != 0
        && key.compare(app_id) != 0
    ) {
      ++it;
      Object.erase(key);
    } else {
      it++;
    }
  }
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
