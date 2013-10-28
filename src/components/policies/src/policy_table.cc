/**
 * @file policy_table.cc
 * @brief Policy table source file.
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

#include "policies/policy_table.h"
#include "smart_objects/always_true_schema_item.h"

namespace policies_ns = NsSmartDeviceLink::policies;
namespace so_ns = NsSmartDeviceLink::NsSmartObjects;

//---------------------------------------------------------------------------

policies_ns::PolicyTable::PolicyTable(const std::string policy_table_string)
  : is_schema_set_(false)
  , schema_(so_ns::CSmartSchema(so_ns::CAlwaysTrueSchemaItem::create()))
  , pt_smart_object_()
  , pt_default_smart_object_() {
}

//---------------------------------------------------------------------------

policies_ns::PolicyTable::~PolicyTable() {
}

//---------------------------------------------------------------------------

so_ns::SmartObject& policies_ns::PolicyTable::AsSmartObject() {
  return pt_smart_object_;
}

//---------------------------------------------------------------------------

const std::string policies_ns::PolicyTable::AsString() {
  return std::string();
}

//---------------------------------------------------------------------------

void policies_ns::PolicyTable::SetSchema(
    NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema) {
  schema_ = schema;
}

//---------------------------------------------------------------------------

policies_ns::PTValidationResult::eType policies_ns::PolicyTable::Validate() {
  return PTValidationResult::VALIDATION_FAILED;
}

//---------------------------------------------------------------------------

/*static*/
so_ns::CSmartSchema policies_ns::PolicyTable::createSchemaSDL() {
  return
    so_ns::CSmartSchema(
        so_ns::CAlwaysTrueSchemaItem::create());
}

so_ns::SmartObject& policies_ns::PolicyTable::CreateDefaultPT() {
  return pt_default_smart_object_;
}
