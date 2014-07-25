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

#include "smart_objects/always_false_schema_item.h"
#include "smart_objects/object_schema_item.h"
#include "smart_objects/smart_object.h"

namespace smart_objects_ns = NsSmartDeviceLink::NsSmartObjects;

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

const char *kMsgParams = "msg_params";

CObjectSchemaItem::SMember::SMember()
    : mSchemaItem(CAlwaysFalseSchemaItem::create()),
      mIsMandatory(true) {
}

CObjectSchemaItem::SMember::SMember(const ISchemaItemPtr SchemaItem, const bool IsMandatory)
    : mSchemaItem(SchemaItem),
      mIsMandatory(IsMandatory) {
}

utils::SharedPtr<CObjectSchemaItem> CObjectSchemaItem::create(const Members& members) {
  return new CObjectSchemaItem(members);
}

Errors::eType CObjectSchemaItem::validate(const SmartObject& Object) {
  static bool is_valid = false;
  Errors::eType result = Errors::ERROR;

  if (SmartType_Map == Object.getType()) {
    result = Errors::OK;

    for (Members::const_iterator i = mMembers.begin(); i != mMembers.end();
         ++i) {
      if (Object.keyExists(i->first)) {
        if (kMsgParams == i->first) {
          is_valid = false;
        }

        result = i->second.mSchemaItem->validate(Object.getElement(i->first));

        if (kMsgParams == i->first) {
          if ((!is_valid) && ( 0 < (*i->second.mSchemaItem).GetMemberSize())) {
            result = Errors::ERROR;
          }
        }

      } else {
        if (i->second.mIsMandatory) {
          result = Errors::MISSING_MANDATORY_PARAMETER;
        }
      }

      if ((Errors::OK != result) && (Errors::UNEXPECTED_PARAMETER != result)) {
        break;
      }
    }

    if (Errors::OK == result) {
      for (SmartMap::const_iterator k = Object.map_begin();
           k != Object.map_end(); ++k) {
        if (mMembers.end() == mMembers.find(k->first)) {
          result = Errors::UNEXPECTED_PARAMETER;
        } else {
          is_valid = true;
        }
      }
    }

  } else {
    result = Errors::INVALID_VALUE;
  }

  if (Errors::OK == result) {
    is_valid = true;
  }

  return result;
}

void CObjectSchemaItem::applySchema(SmartObject& Object) {
  if (SmartType_Map != Object.getType()) {
    return;
  }
  for (Members::const_iterator it = mMembers.end(); it != mMembers.end(); ++it) {
    const std::string& key = it->first;
    const SMember& member = it->second;
    SmartObject def_value;
    if (member.mSchemaItem->setDefaultValue(def_value) &&
        !Object.keyExists(key)) {
      Object[key] = SmartObject(def_value.getType());
      if (SmartType_Boolean == def_value.getType()) {
        Object[key] = def_value.asBool();
      } else if (SmartType_Integer == def_value.getType()) {
        Object[key] = def_value.asUInt();
      } else if (SmartType_Double == def_value.getType()) {
        Object[key] = def_value.asDouble();
      }
    }
  }

  for (Members::const_iterator i = mMembers.begin(); i != mMembers.end(); ++i) {
    if (Object.keyExists(i->first)) {
      i->second.mSchemaItem->applySchema(Object[i->first]);
    }
  }
}

void CObjectSchemaItem::unapplySchema(SmartObject& Object) {
  if (SmartType_Map != Object.getType()) {
    return;
  }
  for (SmartMap::const_iterator it = Object.map_begin(); it != Object.map_end(); ++it) {
    const std::string& key = it->first;
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

void CObjectSchemaItem::BuildObjectBySchema(const SmartObject& pattern_object,
                                            SmartObject& result_object) {
  result_object = SmartObject(SmartType_Map);
  const bool pattern_is_map = SmartType_Map == pattern_object.getType();

  for (Members::const_iterator it = mMembers.begin(); it != mMembers.end(); ++it) {
    const std::string& key = it->first;
    const SMember& member = it->second;
    member.mSchemaItem->BuildObjectBySchema(
          (pattern_object.keyExists(key) && pattern_is_map) ? pattern_object.getElement(key) : SmartObject(),
          result_object[key]);
  }
}

size_t CObjectSchemaItem::GetMemberSize() {
  return mMembers.size();
}

CObjectSchemaItem::CObjectSchemaItem(const Members& members)
    : mMembers(members) {}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
