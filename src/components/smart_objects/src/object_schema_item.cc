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

#include <boost/algorithm/string.hpp>

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
    utils::SemanticVersion since_struct;
    std::vector<std::string> since_fields;
    boost::split(since_fields, Since, boost::is_any_of("."));
    if (since_fields.size() == 3) {
      since_struct.major_version = atoi(since_fields[0].c_str());
      since_struct.minor_version = atoi(since_fields[1].c_str());
      since_struct.patch_version = atoi(since_fields[2].c_str());
      mSince = since_struct;
    }
  }
  if (Until.size() > 0) {
    utils::SemanticVersion until_struct;
    std::vector<std::string> until_fields;
    boost::split(until_fields, Until, boost::is_any_of("."));
    if (until_fields.size() == 3) {
      until_struct.major_version = atoi(until_fields[0].c_str());
      until_struct.minor_version = atoi(until_fields[1].c_str());
      until_struct.patch_version = atoi(until_fields[2].c_str());
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
    if (mSince.is_initialized()) {
      if (MessageVersion < mSince.get()) {
        return false;  // Msg version predates `since` field
      } else {
        if (mUntil.is_initialized() && (MessageVersion >= mUntil.get())) {
          return false;  // Msg version newer than `until` field
        } else {
          return true;  // Mobile msg version falls within specified version
                        // range
        }
      }
    }

    if (mUntil.is_initialized() && (MessageVersion >= mUntil.get())) {
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

Errors::eType CObjectSchemaItem::validate(const SmartObject& object) {
  rpc::ValidationReport report("RPC");
  return validate(object, &report);
}

Errors::eType CObjectSchemaItem::validate(const SmartObject& object,
                                          rpc::ValidationReport* report__) {
  if (SmartType_Map != object.getType()) {
    std::string validation_info = "Incorrect type, expected: " +
                                  SmartObject::typeToString(SmartType_Map) +
                                  ", got: " +
                                  SmartObject::typeToString(object.getType());
    report__->set_validation_info(validation_info);
    return Errors::INVALID_VALUE;
  }

  std::set<std::string> object_keys = object.enumerate();

  for (Members::const_iterator it = mMembers.begin(); it != mMembers.end();
       ++it) {
    const std::string& key = it->first;
    const SMember& member = it->second;

    std::set<std::string>::const_iterator key_it = object_keys.find(key);
    if (object_keys.end() == key_it) {
      if (member.mIsMandatory) {
        std::string validation_info = "Missing mandatory parameter: " + key;
        report__->set_validation_info(validation_info);
        return Errors::MISSING_MANDATORY_PARAMETER;
      }
      continue;
    }
    const SmartObject& field = object.getElement(key);
    const Errors::eType result =
        member.mSchemaItem->validate(field, &report__->ReportSubobject(key));
    if (Errors::OK != result) {
      return result;
    }
    object_keys.erase(key_it);
  }
  return Errors::OK;
}

Errors::eType CObjectSchemaItem::validate(
    const SmartObject& object,
    rpc::ValidationReport* report__,
    const utils::SemanticVersion& MessageVersion) {
  if (SmartType_Map != object.getType()) {
    std::string validation_info = "Incorrect type, expected: " +
                                  SmartObject::typeToString(SmartType_Map) +
                                  ", got: " +
                                  SmartObject::typeToString(object.getType());
    report__->set_validation_info(validation_info);
    return Errors::INVALID_VALUE;
  }

  std::set<std::string> object_keys = object.enumerate();

  for (Members::const_iterator it = mMembers.begin(); it != mMembers.end();
       ++it) {
    const std::string& key = it->first;
    const SMember& member = it->second;
    std::set<std::string>::const_iterator key_it = object_keys.find(key);
    if (object_keys.end() == key_it) {
      if (member.mSince.is_initialized() &&
          MessageVersion < member.mSince.get() &&
          member.mHistoryVector.size() > 0) {
        // Message version predates parameter and a history vector exists.
        for (uint i = 0; i < member.mHistoryVector.size(); i++) {
          if (member.mHistoryVector[i].mSince.is_initialized() &&
              MessageVersion >= member.mHistoryVector[i].mSince.get()) {
            if (member.mHistoryVector[i].mUntil.is_initialized() &&
                MessageVersion >= member.mHistoryVector[i].mUntil.get()) {
              // MessageVersion is newer than the specified "Until" version
              continue;
            } else {
              if (member.mHistoryVector[i].mIsMandatory == true &&
                  (member.mHistoryVector[i].mIsRemoved == false)) {
                std::string validation_info =
                    "Missing mandatory parameter since and until: " + key;
                report__->set_validation_info(validation_info);
                return Errors::MISSING_MANDATORY_PARAMETER;
              }
              break;
            }
          } else if (member.mHistoryVector[i].mSince.is_initialized() ==
                         false &&
                     member.mHistoryVector[i].mUntil.is_initialized() &&
                     MessageVersion < member.mHistoryVector[i].mUntil.get()) {
            if (member.mHistoryVector[i].mIsMandatory == true &&
                (member.mHistoryVector[i].mIsRemoved == false)) {
              std::string validation_info =
                  "Missing mandatory parameter until: " + key;
              report__->set_validation_info(validation_info);
              return Errors::MISSING_MANDATORY_PARAMETER;
            }
            break;
          }
        }
      } else if (member.mIsMandatory &&
                 member.CheckHistoryFieldVersion(MessageVersion) &&
                 (member.mIsMandatory == false)) {
        std::string validation_info = "Missing mandatory parameter: " + key;
        report__->set_validation_info(validation_info);
        return Errors::MISSING_MANDATORY_PARAMETER;
      }
      continue;
    }
    const SmartObject& field = object.getElement(key);

    Errors::eType result = Errors::OK;
    // Check if MessageVersion matches schema version
    if (member.CheckHistoryFieldVersion(MessageVersion) ||
        member.mHistoryVector.empty()) {
      result = member.mSchemaItem->validate(
          field, &report__->ReportSubobject(key), MessageVersion);
    } else if (member.mHistoryVector.size() > 0) {  // Check for history
      for (uint i = 0; i < member.mHistoryVector.size(); i++) {
        if (member.mHistoryVector[i].CheckHistoryFieldVersion(MessageVersion)) {
          // Found the correct history schema. Call validate
          result = member.mHistoryVector[i].mSchemaItem->validate(
              field, &report__->ReportSubobject(key), MessageVersion);
        }
      }
    }
    if (Errors::OK != result) {
      return result;
    }
    object_keys.erase(key_it);
  }
  return Errors::OK;
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
    } else if (members_it->second.mIsRemoved &&
               members_it->second.CheckHistoryFieldVersion(MessageVersion)) {
      ++it;
      Object.erase(key);
    } else if (members_it->second.mHistoryVector.size() > 0) {
      for (uint i = 0; i < members_it->second.mHistoryVector.size(); i++) {
        if (members_it->second.mHistoryVector[i].CheckHistoryFieldVersion(
                MessageVersion) &&
            members_it->second.mHistoryVector[i].mIsRemoved) {
          ++it;
          Object.erase(key);
          break;
        }
      }
      ++it;
    } else {
      ++it;
    }
  }
}

bool CObjectSchemaItem::IsMandatory(const SMember& member) {
  return true;
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
