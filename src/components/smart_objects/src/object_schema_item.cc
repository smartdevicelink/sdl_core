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
namespace ns_smart_device_link {
namespace ns_smart_objects {

CObjectSchemaItem::SMember::SMember()
    : mSchemaItem(CAlwaysFalseSchemaItem::create()), mIsMandatory(true) {}

CObjectSchemaItem::SMember::SMember(
    const ISchemaItemPtr SchemaItem,
    const bool IsMandatory,
    const std::string& Since,
    const std::string& Until,
    const bool IsDeprecated,
    const bool IsRemoved,
    const std::vector<CObjectSchemaItem::SMember>& history_vector)
    : mSchemaItem(SchemaItem), mIsMandatory(IsMandatory) {
  if (Since.size() > 0) {
    utils::SemanticVersion since_struct(Since);
    if (since_struct.isValid()) {
      mSince = since_struct;
    }
  }
  if (Until.size() > 0) {
    utils::SemanticVersion until_struct(Until);
    if (until_struct.isValid()) {
      mUntil = until_struct;
    }
  }
  mIsDeprecated = IsDeprecated;
  mIsRemoved = IsRemoved;
  mHistoryVector = history_vector;
}

bool CObjectSchemaItem::SMember::CheckHistoryFieldVersion(
    const utils::SemanticVersion& MessageVersion) const {
  if (MessageVersion.isValid()) {
    if (mSince != boost::none) {
      if (MessageVersion < mSince.get()) {
        return false;  // Msg version predates `since` field
      } else {
        if (mUntil != boost::none && (MessageVersion >= mUntil.get())) {
          return false;  // Msg version newer than `until` field
        } else {
          return true;  // Mobile msg version falls within specified version
                        // range
        }
      }
    }

    if (mUntil != boost::none && (MessageVersion >= mUntil.get())) {
      return false;  // Msg version newer than `until` field
    } else {
      return true;  // Mobile msg version falls within specified version range
    }
  }

  return true;  // Not enough version information. Default true.
}

std::shared_ptr<CObjectSchemaItem> CObjectSchemaItem::create(
    const Members& members) {
  return std::shared_ptr<CObjectSchemaItem>(new CObjectSchemaItem(members));
}

errors::eType CObjectSchemaItem::validate(
    const SmartObject& object,
    rpc::ValidationReport* report__,
    const utils::SemanticVersion& MessageVersion) {
  if (SmartType_Map != object.getType()) {
    std::string validation_info = "Incorrect type, expected: " +
                                  SmartObject::typeToString(SmartType_Map) +
                                  ", got: " +
                                  SmartObject::typeToString(object.getType());
    report__->set_validation_info(validation_info);
    return errors::INVALID_VALUE;
  }

  std::set<std::string> object_keys = object.enumerate();

  for (Members::const_iterator it = mMembers.begin(); it != mMembers.end();
       ++it) {
    const std::string& key = it->first;
    const SMember& member = it->second;
    const SMember& correct_member = GetCorrectMember(member, MessageVersion);

    std::set<std::string>::const_iterator key_it = object_keys.find(key);
    if (object_keys.end() == key_it) {
      if (correct_member.mIsMandatory == true &&
          correct_member.mIsRemoved == false) {
        std::string validation_info = "Missing mandatory parameter: " + key;
        report__->set_validation_info(validation_info);
        return errors::MISSING_MANDATORY_PARAMETER;
      }
      continue;
    }
    const SmartObject& field = object.getElement(key);

    errors::eType result = errors::OK;
    // Check if MessageVersion matches schema version
    result = correct_member.mSchemaItem->validate(
        field, &report__->ReportSubobject(key), MessageVersion);
    if (errors::OK != result) {
      return result;
    }
    object_keys.erase(key_it);
  }
  return errors::OK;
}

void CObjectSchemaItem::applySchema(
    SmartObject& Object,
    const bool RemoveFakeParameters,
    const utils::SemanticVersion& MessageVersion) {
  if (SmartType_Map != Object.getType()) {
    return;
  }

  if (RemoveFakeParameters) {
    RemoveFakeParams(Object, MessageVersion);
  }

  SmartObject default_value;
  for (Members::const_iterator it = mMembers.begin(); it != mMembers.end();
       ++it) {
    const std::string& key = it->first;
    const SMember& member = it->second;
    if (!Object.keyExists(key)) {
      if (member.mSchemaItem->setDefaultValue(default_value)) {
        Object[key] = default_value;
        member.mSchemaItem->applySchema(
            Object[key], RemoveFakeParameters, MessageVersion);
      }
    } else {
      member.mSchemaItem->applySchema(
          Object[key], RemoveFakeParameters, MessageVersion);
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

  for (Members::const_iterator it = mMembers.begin(); it != mMembers.end();
       ++it) {
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

  for (Members::const_iterator it = mMembers.begin(); it != mMembers.end();
       ++it) {
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

void CObjectSchemaItem::RemoveFakeParams(
    SmartObject& Object, const utils::SemanticVersion& MessageVersion) {
  for (SmartMap::const_iterator it = Object.map_begin();
       it != Object.map_end();) {
    const std::string& key = it->first;
    std::map<std::string, SMember>::const_iterator members_it =
        mMembers.find(key);
    if (mMembers.end() == members_it
        // FIXME(EZamakhov): Remove illegal usage of filed in AM
        &&
        key.compare(connection_key) != 0 && key.compare(binary_data) != 0 &&
        key.compare(app_id) != 0) {
      ++it;
      Object.erase(key);

    } else if (mMembers.end() != members_it &&
               GetCorrectMember(members_it->second, MessageVersion)
                   .mIsRemoved) {
      ++it;
      Object.erase(key);
    } else {
      ++it;
    }
  }
}

const CObjectSchemaItem::SMember& CObjectSchemaItem::GetCorrectMember(
    const SMember& member, const utils::SemanticVersion& messageVersion) {
  // Check if member is the correct version
  if (member.CheckHistoryFieldVersion(messageVersion)) {
    return member;
  }
  // Check for history tag items
  if (!member.mHistoryVector.empty()) {
    for (uint i = 0; i < member.mHistoryVector.size(); i++) {
      if (member.mHistoryVector[i].CheckHistoryFieldVersion(messageVersion)) {
        return member.mHistoryVector[i];
      }
    }
  }
  // Return member as default
  return member;
}

}  // namespace ns_smart_objects
}  // namespace ns_smart_device_link
