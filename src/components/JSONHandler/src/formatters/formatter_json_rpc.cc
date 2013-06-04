/**
 * @file formatter_json_rpc.cc
 * @brief formatter_json_rpc source file.
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

#include "JSONHandler/formatters/formatter_json_rpc.h"

namespace NsSmartDeviceLink {
namespace NsJSONHandler {
namespace Formatters {

namespace strings = NsJSONHandler::strings;

const char *FormatterJsonRpc::kRequest = "request";
const char *FormatterJsonRpc::kResponse = "response";
const char *FormatterJsonRpc::kNotification = "notification";
const char *FormatterJsonRpc::kJsonRpc = "jsonrpc";
const char *FormatterJsonRpc::kJsonRpcExpectedValue = "2.0";
const char *FormatterJsonRpc::kId = "id";
const char *FormatterJsonRpc::kMethod = "method";
const char *FormatterJsonRpc::kParams = "params";
const char *FormatterJsonRpc::kResult = "result";
const char *FormatterJsonRpc::kError = "error";
const char *FormatterJsonRpc::kCode = "code";
const char *FormatterJsonRpc::kData = "data";

bool FormatterJsonRpc::ToString(const NsSmartObjects::CSmartObject &obj,
                                std::string &out_str) {
  Json::Value root(Json::objectValue);

  root[kJsonRpc] = kJsonRpcExpectedValue;

  NsSmartObjects::CSmartObject formatted_object(obj);
  Json::Value msg_params_json(Json::objectValue);
  formatted_object.getSchema().unapplySchema(formatted_object);

  bool result = formatted_object.keyExists(strings::S_MSG_PARAMS);
  if (true == result) {
    const NsSmartObjects::CSmartObject &msg_params =
        formatted_object.getElement(strings::S_MSG_PARAMS);

    result = (NsSmartObjects::SmartType_Map == msg_params.getType());
    if (true == result) {
      objToJsonValue(msg_params, msg_params_json);
    }
  }

  if (false == formatted_object.keyExists(strings::S_PARAMS)) {
    result = false;
  } else {
    const NsSmartObjects::CSmartObject &params = formatted_object.getElement(
        strings::S_PARAMS);
    if (NsSmartObjects::SmartType_Map != params.getType()) {
      result = false;
    } else {
      const NsSmartObjects::CSmartObject &message_type_object =
          formatted_object.getElement(strings::S_MESSAGE_TYPE);

      if (NsSmartObjects::SmartType_String != message_type_object.getType()) {
        result = false;
      } else {
        const std::string message_type = message_type_object;

        if (kRequest == message_type) {
          root[kParams] = msg_params_json;
          result = result && SetMethod(params, root);
          result = result && SetId(params, root);
        } else if (kResponse == message_type) {
          root[kResult] = msg_params_json;
          result = result && SetMethod(params, root[kResult]);
          result = result && SetId(params, root);

          if (false == params.keyExists(strings::kCode)) {
            result = false;
          } else {
            const NsSmartObjects::CSmartObject &code = params.getElement(
                strings::kCode);

            if (NsSmartObjects::SmartType_Integer != code.getType()) {
              result = false;
            } else {
              root[kResult][kCode] = code.asInt();
            }
          }
        } else if (kNotification == message_type) {
          root[kParams] = msg_params_json;
          result = result && SetMethod(params, root);
        } else {
          result = false;
        }
      }
    }
  }
  out_str = root.toStyledString();

  return result;
}

bool FormatterJsonRpc::SetMethod(const NsSmartObjects::CSmartObject &params,
                                 Json::Value &method_container) {
  bool result = false;

  if (true == params.keyExists(strings::S_FUNCTION_ID)) {
    const NsSmartObjects::CSmartObject &function_id =
        params.getElement(strings::S_FUNCTION_ID);

    if (NsSmartObjects::SmartType_String == function_id.getType()) {
      method_container[kMethod] = function_id.asString();
      result = true;
    }
  }

  return result;
}

bool FormatterJsonRpc::SetId(const NsSmartObjects::CSmartObject &params,
                             Json::Value &id_container) {
  bool result = false;

  if (true == params.keyExists(strings::S_CORRELATION_ID)) {
    const NsSmartObjects::CSmartObject &id = params.getElement(
        strings::S_CORRELATION_ID);

    if (NsSmartObjects::SmartType_Integer == id.getType()) {
      id_container[kId] = id.asString();
      result = true;
    }
  }

  return result;
}

} // namespace Formatters
} // namespace NsJSONHandler
} // namespace NsSmartDeviceLink
