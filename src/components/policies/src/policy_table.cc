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
#include "formatters/generic_json_formatter.h"
#include "policies/policy_table_schema.h"

namespace policies_ns = NsSmartDeviceLink::policies;
namespace so_ns = NsSmartDeviceLink::NsSmartObjects;
namespace formatters_ns = NsSmartDeviceLink::NsJSONHandler::Formatters;

//---------------------------------------------------------------------------

policies_ns::PolicyTable::PolicyTable(
    const std::string policy_table_string, PTType::eType pt_type)
  : is_PT_valid_(PTValidationResult::VALIDATION_FAILED)
  , pt_type_(pt_type)
  , schema_(policies_ns::PolicyTableSchema::Create())
  , pt_smart_object_()
  , pt_default_smart_object_() {

  if (false == formatters_ns::GenericJsonFormatter::FromString(
      policy_table_string, pt_smart_object_)) {
    is_PT_valid_ = PTValidationResult::VALIDATION_FAILED_BAD_JSON;
  } else {
    
  } 
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
  std::string ret_val;
  if (PTValidationResult::VALIDATION_FAILED_BAD_JSON != is_PT_valid_) {
    formatters_ns::GenericJsonFormatter::ToString(pt_smart_object_, ret_val);
  }
  return ret_val;
}

//---------------------------------------------------------------------------

policies_ns::PTValidationResult::eType policies_ns::PolicyTable::Validate() {
  //TODO: distinct between PT and Preload PolicyTable types (use pt_type_)
  if (PTValidationResult::VALIDATION_FAILED_BAD_JSON == is_PT_valid_) {
    return is_PT_valid_;
  }

  if (so_ns::Errors::OK == schema_.validate(pt_smart_object_)) {
    is_PT_valid_ = PTValidationResult::VALIDATION_OK;
    return is_PT_valid_;
  } 

  is_PT_valid_ = PTValidationResult::VALIDATION_FAILED;
  return is_PT_valid_;
}

//---------------------------------------------------------------------------

bool policies_ns::PolicyTable::IsPTPreload() {
  // evaluate 'preloaded_pt' in 'module_config'
  return false;
}

//---------------------------------------------------------------------------

so_ns::SmartObject& policies_ns::PolicyTable::CreateDefaultPT() {
  return pt_default_smart_object_;
}
