/*
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

#include "formatters/formatter_json_rpc.h"
#include "utils/convert_utils.h"

namespace NsSmartDeviceLink {
namespace NsJSONHandler {
namespace Formatters {

namespace strings = NsJSONHandler::strings;

const char* FormatterJsonRpc::kRequest = "request";
const char* FormatterJsonRpc::kResponse = "response";
const char* FormatterJsonRpc::kNotification = "notification";
const char* FormatterJsonRpc::kErrorResponse = "error_response";
const char* FormatterJsonRpc::kJsonRpc = "jsonrpc";
const char* FormatterJsonRpc::kJsonRpcExpectedValue = "2.0";
const char* FormatterJsonRpc::kId = "id";
const char* FormatterJsonRpc::kMethod = "method";
const char* FormatterJsonRpc::kParams = "params";
const char* FormatterJsonRpc::kResult = "result";
const char* FormatterJsonRpc::kError = "error";
const char* FormatterJsonRpc::kCode = "code";
const char* FormatterJsonRpc::kData = "data";
const char* FormatterJsonRpc::kMessage = "message";

bool FormatterJsonRpc::ToString(const NsSmartObjects::SmartObject& obj,
                                std::string& out_str) {
  bool result = true;
  try {
    Json::Value root(Json::objectValue);

    root[kJsonRpc] = kJsonRpcExpectedValue;

    NsSmartObjects::SmartObject formatted_object(obj);
    Json::Value msg_params_json(Json::objectValue);
    formatted_object.getSchema().unapplySchema(formatted_object);

    bool is_message_params = formatted_object.keyExists(strings::S_MSG_PARAMS);
    bool empty_message_params = true;
    if (true == is_message_params) {
      const NsSmartObjects::SmartObject& msg_params =
          formatted_object.getElement(strings::S_MSG_PARAMS);

      result = (NsSmartObjects::SmartType_Map == msg_params.getType());
      if (true == result) {
        objToJsonValue(msg_params, msg_params_json);
      }
      if (0 < msg_params.length()) {
        empty_message_params = false;
      }
    }

    if (false == formatted_object.keyExists(strings::S_PARAMS)) {
      result = false;
    } else {
      const NsSmartObjects::SmartObject& params =
          formatted_object.getElement(strings::S_PARAMS);
      if (NsSmartObjects::SmartType_Map != params.getType()) {
        result = false;
      } else {
        const NsSmartObjects::SmartObject& message_type_object =
            params.getElement(strings::S_MESSAGE_TYPE);

        if (NsSmartObjects::SmartType_String != message_type_object.getType()) {
          result = false;
        } else {
          const std::string message_type = message_type_object.asString();

          if (kRequest == message_type) {
            if (false == empty_message_params) {
              root[kParams] = msg_params_json;
            }
            result = result && SetMethod(params, root);
            result = result && SetId(params, root);
          } else if (kResponse == message_type) {
            root[kResult] = msg_params_json;
            result = result && SetMethod(params, root[kResult]);
            result = result && SetId(params, root);

            if (false == params.keyExists(strings::kCode)) {
              result = false;
            } else {
              const NsSmartObjects::SmartObject& code =
                  params.getElement(strings::kCode);

              if (NsSmartObjects::SmartType_Integer != code.getType()) {
                result = false;
              } else {
                root[kResult][kCode] =
                    utils::ConvertInt64ToLongLongInt(code.asInt());
              }
            }
          } else if (kNotification == message_type) {
            root[kParams] = msg_params_json;
            result = result && SetMethod(params, root);
          } else if (kErrorResponse == message_type) {
            result = result && SetId(params, root);
            result = result && SetMethod(params, root[kError][kData]);
            result = result && SetMessage(params, root[kError]);

            const NsSmartObjects::SmartObject& code =
                params.getElement(strings::kCode);
            if (NsSmartObjects::SmartType_Integer != code.getType()) {
              result = false;
            } else {
              root[kError][kCode] =
                  utils::ConvertInt64ToLongLongInt(code.asInt());
            }
          }
        }
      }
    }
    out_str = root.toStyledString();
  } catch (...) {
    result = false;
  }

  return result;
}

bool FormatterJsonRpc::SetMethod(const NsSmartObjects::SmartObject& params,
                                 Json::Value& method_container) {
  bool result = false;

  if (true == params.keyExists(strings::S_FUNCTION_ID)) {
    const NsSmartObjects::SmartObject& function_id =
        params.getElement(strings::S_FUNCTION_ID);

    if (NsSmartObjects::SmartType_String == function_id.getType()) {
      method_container[kMethod] = function_id.asString();
      result = true;
    }
  }

  return result;
}

bool FormatterJsonRpc::SetId(const NsSmartObjects::SmartObject& params,
                             Json::Value& id_container) {
  bool result = false;

  if (true == params.keyExists(strings::S_CORRELATION_ID)) {
    const NsSmartObjects::SmartObject& id =
        params.getElement(strings::S_CORRELATION_ID);

    if (NsSmartObjects::SmartType_Integer == id.getType()) {
      id_container[kId] = utils::ConvertUInt64ToLongLongUInt(id.asUInt());
      result = true;
    }
  }

  return result;
}

bool FormatterJsonRpc::SetMessage(const NsSmartObjects::SmartObject& params,
                                  Json::Value& message_container) {
  bool result = false;

  if (true == params.keyExists(strings::kMessage)) {
    const NsSmartObjects::SmartObject& message =
        params.getElement(strings::kMessage);

    if (NsSmartObjects::SmartType_String == message.getType()) {
      message_container[kMessage] = message.asString();
      result = true;
    }
  }

  return result;
}

}  // namespace Formatters
}  // namespace NsJSONHandler
}  // namespace NsSmartDeviceLink
