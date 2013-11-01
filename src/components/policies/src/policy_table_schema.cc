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

typedef utils::SharedPtr<ISchemaItem> SchemaItemPtr;


const std::string PolicyTableSchema::kStrPolicyTable("policy_table");
const std::string PolicyTableSchema::kStrModuleConfig("module_config");
const std::string PolicyTableSchema::kStrFunctionalGroupings("functional_groupings");
const std::string PolicyTableSchema::kStrAppPolicies("app_policies");


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

SchemaItemPtr PolicyTableSchema::CreateModuleConfig(void) {
  
  // TODO: implement 
  return CAlwaysTrueSchemaItem::create();
}

SchemaItemPtr PolicyTableSchema::CreateFunctionaGroupings(void) {
  
  // TODO: implement 
  return CAlwaysTrueSchemaItem::create();
}

SchemaItemPtr PolicyTableSchema::CreateAppPolicies(void) {
  
  // TODO: implement 
  return CAlwaysTrueSchemaItem::create();
}

}  // namespace NsSmartDeviceLink
}  // namespace policies
