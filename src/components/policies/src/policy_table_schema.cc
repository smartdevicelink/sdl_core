/**
 * @file policy_table_schema.cc
 * @brief Policy table schema source file.
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
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "policies/policy_table_schema.h"

#include "smart_objects/object_schema_item.h"
#include "smart_objects/always_true_schema_item.h"
#include "smart_objects/object_optional_schema_item.h"
#include "smart_objects/array_schema_item.h"
#include "smart_objects/string_schema_item.h"
#include "utils/shared_ptr.h"

#include <map>
#include <string>

namespace NsSmartDeviceLink {
namespace policies {

namespace so_ns = NsSmartDeviceLink::NsSmartObjects;

using ::NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::CSmartSchema;
using ::NsSmartDeviceLink::NsSmartObjects::ISchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::CAlwaysTrueSchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::ObjectOptionalSchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem;     
using ::NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem;

typedef utils::SharedPtr<ISchemaItem> SchemaItemPtr;

//-----------------------------------------------------------------------------

const std::string PolicyTableSchema::kStrPolicyTable("policy_table");
const std::string PolicyTableSchema::kStrModuleConfig("module_config");
const std::string PolicyTableSchema::kStrFunctionalGroupings("functional_groupings");
const std::string PolicyTableSchema::kStrAppPolicies("app_policies");
const std::string PolicyTableSchema::kStrEndpoints("endpoints");
const std::string PolicyTableSchema::kStrDefault("default");
const std::string PolicyTableSchema::kStrUserConsentPrompt("user_consent_prompt");
const std::string PolicyTableSchema::kStrRpcs("rpcs");
const std::string PolicyTableSchema::kStrHmiLevels("hmi_levels");
const std::string PolicyTableSchema::kStrParameters("parameters");
const std::string PolicyTableSchema::kStrGroups("groups");
const std::string PolicyTableSchema::kStrNicknames("nicknames");
const std::string PolicyTableSchema::kStrPriority("priority");
//-----------------------------------------------------------------------------

CSmartSchema PolicyTableSchema::CreateSchema(void) {         
  
  SchemaItemPtr root_schema_item;
  std::map<std::string, CObjectSchemaItem::SMember> root_member_map;
  std::map<std::string, CObjectSchemaItem::SMember> policy_table_member_map;
  
  policy_table_member_map[kStrModuleConfig] = CObjectSchemaItem::SMember(
    CreateModuleConfig(),
    true);
  
  policy_table_member_map[kStrFunctionalGroupings] = CObjectSchemaItem::SMember(
    CreateFunctionaGroupings(),
    true);
  
  policy_table_member_map[kStrAppPolicies] = CObjectSchemaItem::SMember(
    CreateAppPolicies(),
    true);
  
  root_member_map[kStrPolicyTable] = CObjectSchemaItem::SMember(
    CObjectSchemaItem::create(policy_table_member_map), 
    true);
  
  root_schema_item = CObjectSchemaItem::create(root_member_map);
  
  return CSmartSchema(root_schema_item);
}

//-----------------------------------------------------------------------------

SchemaItemPtr PolicyTableSchema::CreateModuleConfig(void) {
  
  std::map<std::string, CObjectSchemaItem::SMember> service_type_map;
  service_type_map[kStrDefault] = CObjectSchemaItem::SMember(
    CArraySchemaItem::create(CStringSchemaItem::create()), true);
  
  std::map<std::string, CObjectSchemaItem::SMember> endpoint_map;
  
  endpoint_map[ObjectOptionalSchemaItem::sOptionalGenericFieldName1] = 
    CObjectSchemaItem::SMember(CObjectSchemaItem::create(service_type_map), true);
  
  std::map<std::string, CObjectSchemaItem::SMember> module_config_map;
  module_config_map[kStrEndpoints] = CObjectSchemaItem::SMember(
    ObjectOptionalSchemaItem::Create(endpoint_map), true);
  
  return CObjectSchemaItem::create(module_config_map);
}

//-----------------------------------------------------------------------------

SchemaItemPtr PolicyTableSchema::CreateFunctionaGroupings(void) {
  
  std::map<std::string, CObjectSchemaItem::SMember> rpc_map;
  
  rpc_map[kStrHmiLevels] = CObjectSchemaItem::SMember(
    CArraySchemaItem::create(CStringSchemaItem::create()) );
  
  rpc_map[kStrParameters] = CObjectSchemaItem::SMember(
    CArraySchemaItem::create(CStringSchemaItem::create()) );
  
  std::map<std::string, CObjectSchemaItem::SMember> rpcs_map;
  rpcs_map[ObjectOptionalSchemaItem::sOptionalGenericFieldName1] = 
    CObjectSchemaItem::SMember(CObjectSchemaItem::create(rpc_map), false);
  
  std::map<std::string, CObjectSchemaItem::SMember> functional_grouping_map;
  
  functional_grouping_map[kStrUserConsentPrompt] = 
    CObjectSchemaItem::SMember(CStringSchemaItem::create(), false);
  functional_grouping_map[kStrRpcs] = CObjectSchemaItem::SMember(
    ObjectOptionalSchemaItem::Create(rpcs_map), true);
  
  std::map<std::string, CObjectSchemaItem::SMember> functional_groupings_map;
  
  functional_groupings_map[ObjectOptionalSchemaItem::sOptionalGenericFieldName1] =
    CObjectSchemaItem::SMember(CObjectSchemaItem::create(functional_grouping_map),
                               false);
  
  return ObjectOptionalSchemaItem::Create(functional_groupings_map);
}

//-----------------------------------------------------------------------------

SchemaItemPtr PolicyTableSchema::CreateAppPolicies(void) {
  
  std::map<std::string, CObjectSchemaItem::SMember> app_id_map;
  app_id_map[kStrGroups] = CObjectSchemaItem::SMember(
    CArraySchemaItem::create(CStringSchemaItem::create()), true);

  app_id_map[kStrNicknames] = CObjectSchemaItem::SMember(
    CArraySchemaItem::create(CStringSchemaItem::create()), true);
  
  SchemaItemPtr app_id = CObjectSchemaItem::create(app_id_map);
  
  std::map<std::string, CObjectSchemaItem::SMember> app_policies_map;
  
  std::map<std::string, CObjectSchemaItem::SMember> default_map;
  
  default_map[kStrPriority] = CObjectSchemaItem::SMember(
    CStringSchemaItem::create(), true);
  default_map[kStrGroups] = CObjectSchemaItem::SMember(
    CArraySchemaItem::create(CStringSchemaItem::create()), true);
  
  SchemaItemPtr default_section = CObjectSchemaItem::create(default_map);
  
  app_policies_map[kStrDefault] = 
    CObjectSchemaItem::SMember(default_section, false);
  app_policies_map[ObjectOptionalSchemaItem::sOptionalGenericFieldName1] = 
    CObjectSchemaItem::SMember(app_id, false);
  
  return ObjectOptionalSchemaItem::Create(app_policies_map);
}

}  // namespace NsSmartDeviceLink
}  // namespace policies
