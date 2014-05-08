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
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "smart_objects/always_false_schema_item.h"
#include "smart_objects/object_schema_item.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"

namespace smart_objects_ns = NsSmartDeviceLink::NsSmartObjects;

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

CObjectSchemaItem::SMember::SMember()
    : mSchemaItem(CAlwaysFalseSchemaItem::create()),
      mIsMandatory(true) {
}

CObjectSchemaItem::SMember::SMember(
    const utils::SharedPtr<CObjectSchemaItem::ISchemaItem>& SchemaItem,
    const bool IsMandatory)
    : mSchemaItem(SchemaItem),
      mIsMandatory(IsMandatory) {
}

utils::SharedPtr<CObjectSchemaItem> CObjectSchemaItem::create(
    const std::map<std::string, CObjectSchemaItem::SMember>& Members) {
  return new CObjectSchemaItem(Members);
}

Errors::eType CObjectSchemaItem::validate(const SmartObject& Object) {
  static bool is_valid = false;
  Errors::eType result = Errors::ERROR;

  if (SmartType_Map == Object.getType()) {
    result = Errors::OK;
    const std::set<std::string> objectKeys = Object.enumerate();

    for (std::map<std::string, CObjectSchemaItem::SMember>::const_iterator i =
        mMembers.begin(); i != mMembers.end(); ++i) {
      if (objectKeys.end() != objectKeys.find(i->first)) {
        if (i->first == application_manager::strings::msg_params) {
          is_valid = false;
        }

        result = i->second.mSchemaItem->validate(Object.getElement(i->first));

        if (i->first == application_manager::strings::msg_params) {
          if (!is_valid) {
            result = Errors::ERROR;
          }
        }

      } else {
        if (i->second.mIsMandatory) {
          result = Errors::MISSING_MANDATORY_PARAMETER;
        }
      }

      if (Errors::OK != result) {
        break;
      }
    }

    if (Errors::OK == result) {
      for (std::set<std::string>::const_iterator k = objectKeys.begin();
          k != objectKeys.end(); ++k) {
        if (mMembers.end() == mMembers.find(*k)) {
          result = Errors::UNEXPECTED_PARAMETER;
          break;
        }
      }
    }

    if (Errors::OK == result) {
      is_valid = true;
    }

  } else {
    result = Errors::INVALID_VALUE;
  }

  return result;
}

void CObjectSchemaItem::applySchema(SmartObject& Object) {
  if (SmartType_Map == Object.getType()) {

    SmartObject def_value;
    const std::set<std::string> objectKeys = Object.enumerate();
    for (std::map<std::string, CObjectSchemaItem::SMember>::const_iterator i =
        mMembers.begin(); i != mMembers.end(); ++i) {
      if ((objectKeys.end() == objectKeys.find(i->first)) &&
          (true == i->second.mSchemaItem->hasDefaultValue(def_value))) {
        // create default value
        Object[i->first] = SmartObject(def_value.getType());
        if (SmartType_Boolean == def_value.getType()) {
          Object[i->first] = def_value.asBool();
        } else if (SmartType_Integer == def_value.getType()) {
          Object[i->first] = def_value.asUInt();
        } else if (SmartType_Double == def_value.getType()) {
          Object[i->first] = def_value.asDouble();
        }
      }
    }

    for (std::map<std::string, CObjectSchemaItem::SMember>::const_iterator i =
        mMembers.begin(); i != mMembers.end(); ++i) {
      if (Object.keyExists(i->first)) {
        i->second.mSchemaItem->applySchema(Object[i->first]);
      }
    }
  }
}

void CObjectSchemaItem::unapplySchema(SmartObject& Object) {
  if (SmartType_Map == Object.getType()) {
    const std::set<std::string> objectKeys = Object.enumerate();
    for (std::set<std::string>::const_iterator k = objectKeys.begin();
         k != objectKeys.end(); ++k) {
        if (mMembers.end() == mMembers.find(*k)) {
            // remove fake params
            Object.erase(*k);
        }
    }

    for (std::map<std::string, CObjectSchemaItem::SMember>::const_iterator i =
        mMembers.begin(); i != mMembers.end(); ++i) {
      if (Object.keyExists(i->first)) {
        i->second.mSchemaItem->unapplySchema(Object[i->first]);
      }
    }
  }
}

void CObjectSchemaItem::BuildObjectBySchema(const SmartObject& pattern_object,
                                            SmartObject& result_object) {
  result_object = SmartObject(SmartType_Map);

  if (SmartType_Map == pattern_object.getType()) {
    for (std::map<std::string, CObjectSchemaItem::SMember>::const_iterator i =
        mMembers.begin(); i != mMembers.end(); ++i) {  // for
      if (pattern_object.keyExists(i->first)) {
        i->second.mSchemaItem->BuildObjectBySchema(
            pattern_object.getElement(i->first), result_object[i->first]);
      } else if (i->second.mIsMandatory) {
        i->second.mSchemaItem->BuildObjectBySchema(SmartObject(),
                                                   result_object[i->first]);
      }
    }  // for
  } else {
    bool is_any_mandatory_field = false;
    for (std::map<std::string, CObjectSchemaItem::SMember>::const_iterator i =
        mMembers.begin(); i != mMembers.end(); ++i) {  // for
      if (i->second.mIsMandatory) {
        is_any_mandatory_field = true;
        i->second.mSchemaItem->BuildObjectBySchema(SmartObject(),
                                                   result_object[i->first]);
      }
    }  // for
  }
}

CObjectSchemaItem::CObjectSchemaItem(
    const std::map<std::string, CObjectSchemaItem::SMember>& Members)
    : mMembers(Members) {
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
