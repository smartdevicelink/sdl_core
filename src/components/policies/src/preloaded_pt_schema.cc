/**
 * @file preloaded_pt_schema.cc
 * @brief Policy table preloaded schema source file.
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

#include "policies/preloaded_pt_schema.h"

#include <map>
#include <string>

#include "smart_objects/object_optional_schema_item.h"
#include "smart_objects/array_schema_item.h"
#include "smart_objects/string_schema_item.h"


namespace policies {

using ::NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::CSmartSchema;
using ::NsSmartDeviceLink::NsSmartObjects::ISchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::ObjectOptionalSchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::CArraySchemaItem;
using ::NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem;

typedef utils::SharedPtr<ISchemaItem> SchemaItemPtr;

//-----------------------------------------------------------------------------

CSmartSchema PreloadedPTSchema::Create(void) {
  static PreloadedPTSchema schema;

  return schema.GetSchema();
}

//-----------------------------------------------------------------------------

SchemaItemPtr PreloadedPTSchema::CreateAppId(void) {
  std::map<std::string, CObjectSchemaItem::SMember> app_id_map;

  app_id_map[kStrGroups] = CObjectSchemaItem::SMember(
    CArraySchemaItem::create(CStringSchemaItem::create()), true);
  app_id_map[kStrNicknames] = CObjectSchemaItem::SMember(
    CArraySchemaItem::create(CStringSchemaItem::create()), false);
  app_id_map[kStrPriority] = CObjectSchemaItem::SMember(
    CStringSchemaItem::create(), true);

  return CObjectSchemaItem::create(app_id_map);
}

//-----------------------------------------------------------------------------

SchemaItemPtr PreloadedPTSchema::CreateAppPolicies(void) {
  std::map<std::string, CObjectSchemaItem::SMember> app_policies_map;

  app_policies_map[kStrDefault] =
    CObjectSchemaItem::SMember(CreateAppPoliciesDefault(), true);
  app_policies_map[ObjectOptionalSchemaItem::kOptionalGenericFieldName] =
    CObjectSchemaItem::SMember(CreateAppId(), false);

  return ObjectOptionalSchemaItem::create(app_policies_map);
}

}  // namespace policies
