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

#include "can_cooperation/validators/struct_validators/rds_data_validator.h"
#include "can_cooperation/can_module_constants.h"

namespace can_cooperation {

namespace validators {

CREATE_LOGGERPTR_GLOBAL(logger_, "RdsDataValidator")

using namespace message_params;

RdsDataValidator::RdsDataValidator() {
  // name="PS"
  ps_[ValidationParams::TYPE] = ValueType::STRING;
  ps_[ValidationParams::MIN_LENGTH] = 0;
  ps_[ValidationParams::MAX_LENGTH] = 8;
  ps_[ValidationParams::ARRAY] = 0;
  ps_[ValidationParams::MANDATORY] = 1;

  // name="RT"
  rt_[ValidationParams::TYPE] = ValueType::STRING;
  rt_[ValidationParams::MIN_LENGTH] = 0;
  rt_[ValidationParams::MAX_LENGTH] = 64;
  rt_[ValidationParams::ARRAY] = 0;
  rt_[ValidationParams::MANDATORY] = 1;

  // name="CT"
  ct_[ValidationParams::TYPE] = ValueType::STRING;
  ct_[ValidationParams::MIN_LENGTH] = 24;
  ct_[ValidationParams::MAX_LENGTH] = 24;
  ct_[ValidationParams::ARRAY] = 0;
  ct_[ValidationParams::MANDATORY] = 1;

  // name="PI"
  pi_[ValidationParams::TYPE] = ValueType::STRING;
  pi_[ValidationParams::MIN_LENGTH] = 0;
  pi_[ValidationParams::MAX_LENGTH] = 6;
  pi_[ValidationParams::ARRAY] = 0;
  pi_[ValidationParams::MANDATORY] = 1;

  // name="PTY"
  pty_[ValidationParams::TYPE] = ValueType::INT;
  pty_[ValidationParams::MIN_VALUE] = 0;
  pty_[ValidationParams::MAX_VALUE] = 31;
  pty_[ValidationParams::ARRAY] = 0;
  pty_[ValidationParams::MANDATORY] = 1;

  // name="TA"
  ta_[ValidationParams::TYPE] = ValueType::BOOL;
  ta_[ValidationParams::ARRAY] = 0;
  ta_[ValidationParams::MANDATORY] = 1;

  // name="TP"
  tp_[ValidationParams::TYPE] = ValueType::BOOL;
  tp_[ValidationParams::ARRAY] = 0;
  tp_[ValidationParams::MANDATORY] = 1;

  // name="REG"
  reg_[ValidationParams::TYPE] = ValueType::STRING;
  reg_[ValidationParams::MIN_LENGTH] = 0;
  reg_[ValidationParams::MAX_LENGTH] = 255;
  reg_[ValidationParams::ARRAY] = 0;
  reg_[ValidationParams::MANDATORY] = 1;

  validation_scope_map_[kPS] = &ps_;
  validation_scope_map_[kRT] = &rt_;
  validation_scope_map_[kCT] = &ct_;
  validation_scope_map_[kPI] = &pi_;
  validation_scope_map_[kPTY] = &pty_;
  validation_scope_map_[kTA] = &ta_;
  validation_scope_map_[kTP] = &tp_;
  validation_scope_map_[kREG] = &reg_;
}

ValidationResult RdsDataValidator::Validate(const Json::Value& json,
                                            Json::Value& outgoing_json) {
  LOG4CXX_AUTO_TRACE(logger_);
  return ValidateSimpleValues(json, outgoing_json);
}

}  // namespace valdiators

}  // namespace can_cooperation
