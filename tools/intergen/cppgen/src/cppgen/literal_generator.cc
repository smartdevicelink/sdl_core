/**
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

#include "cppgen/literal_generator.h"

#include "cppgen/naming_convention.h"

namespace codegen {

LiteralGenerator::LiteralGenerator(const Constant& constant) {
  constant.Apply(this);
}

LiteralGenerator::~LiteralGenerator() {
}

std::string LiteralGenerator::result() const {
  return result_;
}

void LiteralGenerator::GenerateCodeForBooleanConstant(
    const Boolean::Constant* boolean) {
  result_ = boolean->value() ? "true" : "false";
}

void LiteralGenerator::GenerateCodeForIntegerConstant(
    const Integer::Constant* integer) {
  result_ = NumberToString(integer->value());
}

void LiteralGenerator::GenerateCodeForFloatConstant(
    const Float::Constant* flt) {
  result_ = NumberToString(flt->value());
}

void LiteralGenerator::GenerateCodeForEnumConstant(const Enum::Constant* enm) {
  std::string type_name = enm->type()->name();
  if (Enum::kFunctionIdEnumName == type_name) {
    result_ = "k" + enm->name();
  } else {
    std::string constant_name = enm->name();
    std::replace(constant_name.begin(), constant_name.end(), '-', '_');
    result_ = to_upper(
        WordList::FromCamelCase(type_name).Abbreviate() + "_" + constant_name);
  }
}

}  // namespace codegen
