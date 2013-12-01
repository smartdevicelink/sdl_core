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
#include "formatters/generic_json_formatter.h"
#include "policies/policy_table_schema.h"

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;
namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;

//---------------------------------------------------------------------------

policies::PolicyTable::PolicyTable(
    const std::string& policy_table_string,
    const PolicyTableType pt_type)
  : pt_validation_result_(PTValidationResult::VALIDATION_FAILED)
  , pt_type_(pt_type)
  , schema_(policies::PolicyTableSchema::Create())
  , pt_smart_object_() {
//
  if (false == formatters::GenericJsonFormatter::FromString(
      policy_table_string, pt_smart_object_)) {
    pt_validation_result_ = PTValidationResult::VALIDATION_FAILED_BAD_JSON;
  } else {
    // By applying schema the enums are converted from strings into ints
    pt_smart_object_.setSchema(schema_);
    schema_.applySchema(pt_smart_object_);
  }
}

//---------------------------------------------------------------------------

policies::PolicyTable::~PolicyTable() {
}

//---------------------------------------------------------------------------

smart_objects::SmartObject& policies::PolicyTable::AsSmartObject() {
  return pt_smart_object_;
}

//---------------------------------------------------------------------------

const std::string policies::PolicyTable::AsString() {
  std::string ret_val;
  if (PTValidationResult::VALIDATION_FAILED_BAD_JSON != pt_validation_result_) {
    formatters::GenericJsonFormatter::ToString(pt_smart_object_, ret_val);
  }
  return ret_val;
}

//---------------------------------------------------------------------------

policies::PTValidationResult policies::PolicyTable::Validate() {
  // TODO(_): distinct between PT and Preload PolicyTable types (use pt_type_)
  if (PTValidationResult::VALIDATION_FAILED_BAD_JSON == pt_validation_result_) {
    return pt_validation_result_;
  }

  if (smart_objects::Errors::OK == schema_.validate(pt_smart_object_)) {
    pt_validation_result_ = PTValidationResult::VALIDATION_OK;
    return pt_validation_result_;
  }

  pt_validation_result_ = PTValidationResult::VALIDATION_FAILED;
  return pt_validation_result_;
}

//---------------------------------------------------------------------------

bool policies::PolicyTable::IsPTPreload() {
  // evaluate 'preloaded_pt' in 'module_config'
  return false;
}

