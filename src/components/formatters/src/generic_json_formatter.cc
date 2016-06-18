/*
 * @file generic_json_formatter.cc
 * @brief Generic JSON formatter source file.
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

#include "formatters/generic_json_formatter.h"
#include "utils/json_utils.h"

namespace NsSmartDeviceLink {
namespace NsJSONHandler {
namespace Formatters {

void GenericJsonFormatter::ToString(const NsSmartObjects::SmartObject& obj,
                                    std::string& out_str) {
  utils::json::JsonValue json_root(utils::json::ValueType::OBJECT_VALUE);
  objToJsonValue(obj, json_root);
  out_str = json_root.ToJson();
}

bool GenericJsonFormatter::FromString(const std::string& str,
                                      NsSmartObjects::SmartObject& out) {
  using namespace utils::json;

  JsonValue::ParseResult parse_result = JsonValue::Parse(str);
  if (!parse_result.second) {
    return false;
  }
  JsonValue& json_root = parse_result.first;

  jsonValueToObj(json_root, out);
  return true;
}

}  // namespace Formatters
}  // namespace NsJSONHandler
}  // namespace NsSmartDeviceLink
