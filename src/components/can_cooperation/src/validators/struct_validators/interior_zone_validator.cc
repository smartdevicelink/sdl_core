/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "can_cooperation/validators/struct_validators/interior_zone_validator.h"
#include "can_cooperation/can_module_constants.h"

namespace can_cooperation {

namespace validators {

CREATE_LOGGERPTR_GLOBAL(logger_, "InteriorZoneValidator")

using namespace message_params;

InteriorZoneValidator::InteriorZoneValidator() {
  // name="col"
  col_[ValidationParams::TYPE] = ValueType::INT;
  col_[ValidationParams::MIN_VALUE] = 0;
  col_[ValidationParams::MAX_VALUE] = 100;
  col_[ValidationParams::ARRAY] = 0;
  col_[ValidationParams::MANDATORY] = 1;

  // name="row"
  row_[ValidationParams::TYPE] = ValueType::INT;
  row_[ValidationParams::MIN_VALUE] = 0;
  row_[ValidationParams::MAX_VALUE] = 100;
  row_[ValidationParams::ARRAY] = 0;
  row_[ValidationParams::MANDATORY] = 1;

  // name="level"
  level_[ValidationParams::TYPE] = ValueType::INT;
  level_[ValidationParams::MIN_VALUE] = 0;
  level_[ValidationParams::MAX_VALUE] = 100;
  level_[ValidationParams::ARRAY] = 0;
  level_[ValidationParams::MANDATORY] = 1;

  // name="colspan"
  colspan_[ValidationParams::TYPE] = ValueType::INT;
  colspan_[ValidationParams::MIN_VALUE] = 0;
  colspan_[ValidationParams::MAX_VALUE] = 100;
  colspan_[ValidationParams::ARRAY] = 0;
  colspan_[ValidationParams::MANDATORY] = 1;

  // name="rowspan"
  rowspan_[ValidationParams::TYPE] = ValueType::INT;
  rowspan_[ValidationParams::MIN_VALUE] = 0;
  rowspan_[ValidationParams::MAX_VALUE] = 100;
  rowspan_[ValidationParams::ARRAY] = 0;
  rowspan_[ValidationParams::MANDATORY] = 1;

  // name="levelspan"
  levelspan_[ValidationParams::TYPE] = ValueType::INT;
  levelspan_[ValidationParams::MIN_VALUE] = 0;
  levelspan_[ValidationParams::MAX_VALUE] = 100;
  levelspan_[ValidationParams::ARRAY] = 0;
  levelspan_[ValidationParams::MANDATORY] = 1;

  validation_scope_map_[kCol] = &col_;
  validation_scope_map_[kRow] = &row_;
  validation_scope_map_[kLevel] = &level_;
  validation_scope_map_[kColspan] = &colspan_;
  validation_scope_map_[kRowspan] = &rowspan_;
  validation_scope_map_[kLevelspan] = &levelspan_;
}

ValidationResult InteriorZoneValidator::Validate(const Json::Value& json,
                                                 Json::Value& outgoing_json) {
  LOG4CXX_AUTO_TRACE(logger_);
  return ValidateSimpleValues(json, outgoing_json);
}

}  // namespace valdiators

}  // namespace can_cooperation
