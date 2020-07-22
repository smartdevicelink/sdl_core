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

#include "generated_msg_version.h"
#include "smart_objects/always_false_schema_item.h"
#include "smart_objects/enum_schema_item.h"
#include "smart_objects/smart_object.h"

namespace {
const char connection_key[] = "connection_key";
const char binary_data[] = "binary_data";
const char app_id[] = "appID";
const utils::SemanticVersion kModuleVersion(application_manager::major_version,
                                            application_manager::minor_version,
                                            application_manager::patch_version);
}  // namespace
namespace ns_smart_device_link {
namespace ns_smart_objects {

SMember::SMember()
    : mSchemaItem(CAlwaysFalseSchemaItem::create())
    , mIsMandatory(true)
    , mIsDeprecated(false)
    , mIsRemoved(false) {}

SMember::SMember(const ISchemaItemPtr SchemaItem,
                 const bool IsMandatory,
                 const std::string& Since,
                 const std::string& Until,
                 const bool IsDeprecated,
                 const bool IsRemoved,
                 const std::vector<SMember>& history_vector)
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

bool SMember::CheckHistoryFieldVersion(
    const utils::SemanticVersion& MessageVersion) const {
  if (MessageVersion.isValid()) {
    if (mSince != boost::none) {
      if (MessageVersion < mSince.get()) {
        return false;
      }
    }
    if (mUntil != boost::none) {
      if (MessageVersion >= mUntil.get()) {
        return false;  // Msg version newer than `until` field
      }
    }
  }

  return true;  // All checks passed. Return true.
}

std::shared_ptr<CObjectSchemaItem> CObjectSchemaItem::create(
    const Members& members) {
  return std::shared_ptr<CObjectSchemaItem>(new CObjectSchemaItem(members));
}

errors::eType CObjectSchemaItem::validate(
    const SmartObject& object,
    rpc::ValidationReport* report,
    const utils::SemanticVersion& MessageVersion,
    const bool allow_unknown_enums) {
  if (SmartType_Map != object.getType()) {
    std::string validation_info =
        "Incorrect type, expected: " +
        SmartObject::typeToString(SmartType_Map) +
        ", got: " + SmartObject::typeToString(object.getType());
    report->set_validation_info(validation_info);
    return errors::INVALID_VALUE;
  }

  std::set<std::string> object_keys = object.enumerate();

  for (Members::const_iterator it = mMembers.begin(); it != mMembers.end();
       ++it) {
    const std::string& key = it->first;
    const SMember& member = it->second;
    const SMember* correct_member = GetCorrectMember(member, MessageVersion);

    std::set<std::string>::const_iterator key_it = object_keys.find(key);
    if (object_keys.end() == key_it) {
      if (correct_member && correct_member->mIsMandatory == true &&
          correct_member->mIsRemoved == false) {
        std::string validation_info = "Missing mandatory parameter: " + key;
        report->set_validation_info(validation_info);
        return errors::MISSING_MANDATORY_PARAMETER;
      }
      continue;
    }
    const SmartObject& field = object.getElement(key);

    errors::eType result = errors::OK;
    // Check if MessageVersion matches schema version
    if (correct_member) {
      result =
          correct_member->mSchemaItem->validate(field,
                                                &report->ReportSubobject(key),
                                                MessageVersion,
                                                allow_unknown_enums);
    } else {
      result = errors::ERROR;
    }

    if (errors::OK != result) {
      return result;
    }
    object_keys.erase(key_it);
  }

  return errors::OK;
}

bool CObjectSchemaItem::filterInvalidEnums(
    SmartObject& Object,
    const utils::SemanticVersion& MessageVersion,
    rpc::ValidationReport* report) {
  bool valid = true;
  for (const auto& key : Object.enumerate()) {
    auto members_it = mMembers.find(key);
    if (mMembers.end() == members_it) {
      // No members found for this key, skipping over
      continue;
    }

    const SMember* member =
        GetCorrectMember(members_it->second, MessageVersion);
    // Perform filtering recursively on this field
    if (member->mSchemaItem->filterInvalidEnums(
            Object[key], MessageVersion, &report->ReportSubobject(key))) {
      // Object is no longer valid if the member is mandatory.
      if (member->mIsMandatory) {
        valid = false;
      }

      // The member is safe to filter if it is non-mandatory, only leaf nodes
      // (individual enum values) should be filtered otherwise.
      bool should_erase = (member->mSchemaItem->GetType() == TYPE_ENUM ||
                           !member->mIsMandatory);
      if (should_erase) {
        Object.erase(key);
      }
    }
  }
  return !valid;
}

void CObjectSchemaItem::applySchema(
    SmartObject& Object,
    const bool remove_unknown_parameters,
    const utils::SemanticVersion& MessageVersion) {
  if (SmartType_Map != Object.getType()) {
    return;
  }

  SmartObject default_value;

  for (const auto& item : mMembers) {
    const std::string& key = item.first;
    const SMember& member = item.second;
    if (!Object.keyExists(key)) {
      if (member.mSchemaItem->setDefaultValue(default_value)) {
        Object[key] = default_value;
        member.mSchemaItem->applySchema(
            Object[key], remove_unknown_parameters, MessageVersion);
      }
    } else {
      member.mSchemaItem->applySchema(
          Object[key], remove_unknown_parameters, MessageVersion);
    }
  }

  if (remove_unknown_parameters) {
    RemoveUnknownParams(Object, MessageVersion);
  }
}

void CObjectSchemaItem::unapplySchema(SmartObject& Object,
                                      const bool remove_unknown_parameters) {
  if (SmartType_Map != Object.getType()) {
    return;
  }
  for (SmartMap::const_iterator it = Object.map_begin();
       it != Object.map_end();) {
    const std::string& key = it->first;
    // move next to avoid wrong iterator on erase
    ++it;
    if (mMembers.end() == mMembers.find(key) && remove_unknown_parameters) {
      // remove fake params
      Object.erase(key);
    }
  }
  for (Members::const_iterator it = mMembers.begin(); it != mMembers.end();
       ++it) {
    const std::string& key = it->first;
    const SMember& member = it->second;
    if (Object.keyExists(key)) {
      member.mSchemaItem->unapplySchema(Object[key], remove_unknown_parameters);
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

TypeID CObjectSchemaItem::GetType() {
  return TYPE_OBJECT;
}

boost::optional<SMember&> CObjectSchemaItem::GetMemberSchemaItem(
    const std::string& member_key) {
  auto it = mMembers.find(member_key);

  if (it != mMembers.end()) {
    return boost::optional<SMember&>(it->second);
  }
  return boost::optional<SMember&>();
}

void CObjectSchemaItem::AddMemberSchemaItem(const std::string& member_key,
                                            SMember& member) {
  mMembers[member_key] = member;
}

CObjectSchemaItem::CObjectSchemaItem(const Members& members)
    : mMembers(members) {}

void CObjectSchemaItem::RemoveUnknownParams(
    SmartObject& Object, const utils::SemanticVersion& MessageVersion) {
  for (const auto& key : Object.enumerate()) {
    std::map<std::string, SMember>::const_iterator members_it =
        mMembers.find(key);

    if (mMembers.end() != members_it) {
      const SMember* member =
          GetCorrectMember(members_it->second, MessageVersion);
      if (!member || member->mIsRemoved) {
        Object.erase(key);
      }
      continue;
    } else if (key.compare(connection_key) != 0 &&
               key.compare(binary_data) != 0 && key.compare(app_id) != 0) {
      Object.erase(key);
    }
  }
}

const SMember* CObjectSchemaItem::GetCorrectMember(
    const SMember& member, const utils::SemanticVersion& messageVersion) {
  // Check if member is the correct version
  if (member.CheckHistoryFieldVersion(messageVersion)) {
    return &member;
  }
  // Check for history tag items
  if (!member.mHistoryVector.empty()) {
    for (uint i = 0; i < member.mHistoryVector.size(); i++) {
      if (member.mHistoryVector[i].CheckHistoryFieldVersion(messageVersion)) {
        return &member.mHistoryVector[i];
      }
    }
  }

  // Return member as default
  return (member.mSince != boost::none && member.mSince.get() > kModuleVersion)
             ? &member
             : nullptr;
}

}  // namespace ns_smart_objects
}  // namespace ns_smart_device_link
