/*
 * @file formatter_json_rpc.h
 * @brief FormatterJsonRpc header file.
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

#ifndef SRC_COMPONENTS_FORMATTERS_INCLUDE_FORMATTERS_FORMATTER_JSON_RPC_H_
#define SRC_COMPONENTS_FORMATTERS_INCLUDE_FORMATTERS_FORMATTER_JSON_RPC_H_

#include <string>
#include <sys/stat.h>

#include "smart_objects/enum_schema_item.h"
#include "smart_objects/smart_object.h"
#include "utils/json_utils.h"

#include "CFormatterJsonBase.h"
#include "formatters/CSmartFactory.h"

namespace NsSmartDeviceLink {
namespace NsJSONHandler {
namespace Formatters {

/**
 * @brief Formatter for JSON RPC format.
 */
class FormatterJsonRpc : public CFormatterJsonBase {
 public:
  /**
   * @brief No errors occurred during the parsing of the input JSON string.
   */
  static const int32_t kSuccess = 0;

  /**
   * @brief Input JSON string cannot be parsed.
   */
  static const int32_t kParsingError = 1;

  /**
   * @brief Input JSON string has invalid format.
   */
  static const int32_t kInvalidFormat = 2;

  /**
   * @brief Method in input JSON string is not
   *        specified or specified incorrectly.
   */
  static const int32_t kMethodNotSpecified = 4;

  /**
   * @brief Method is unknown.
   */
  static const int32_t kUnknownMethod = 8;

  /**
   * @brief Message type is unknown.
   */
  static const int32_t kUnknownMessageType = 16;

  /**
   * @brief Id of request or response is invalid.
   */
  static const int32_t kInvalidId = 32;

  /**
   * @brief Response code is not available.
   */
  static const int32_t kResponseCodeNotAvailable = 64;

  /**
   * @brief Message for error response is not available.
   */
  static const int32_t kErrorResponseMessageNotAvailable = 128;

  /**
   * @brief Creates a JSON string from a SmartObject.
   *
   * @param obj Input SmartObject.
   * @param out_str Resulting JSON string.
   *
   * @return true if success, false otherwise.
   */
  static bool ToString(const NsSmartObjects::SmartObject& obj,
                       std::string& out_str);

  /**
   * @brief Creates a SmartObject from a JSON string.
   *
   * @tparam FunctionId Type of function id enumeration.
   * @tparam MessageType Type of message type enumeration.
   *
   * @param str input JSON string.
   * @param out The resulting SmartObject.
   *
   * @return An integer that is a bitwise-or of all error codes occurred
   *         during the parsing of the input string. 0 if no errors occurred.
   */
  template <typename FunctionId, typename MessageType>
  static int32_t FromString(const std::string& str,
                            NsSmartObjects::SmartObject& out);

 private:
  /**
   * @brief Request.
   */
  static const char* kRequest;

  /**
   * @brief Response.
   */
  static const char* kResponse;

  /**
   * @brief Notification.
   */
  static const char* kNotification;

  /**
   * @brief Error reponse.
   */
  static const char* kErrorResponse;

  /**
   * @brief Name of "jsonrpc" message field.
   */
  static const char* kJsonRpc;

  /**
   * @brief Expected value of "jsonrpc" message field ("2.0").
   */
  static const char* kJsonRpcExpectedValue;

  /**
   * @brief Name of "id" message field.
   */
  static const char* kId;

  /**
   * @brief Name of "method" field.
   */
  static const char* kMethod;

  /**
   * @brief Name of "params" field.
   */
  static const char* kParams;

  /**
   * @brief Name of "result" field.
   */
  static const char* kResult;

  /**
   * @brief Name of "error" field.
   */
  static const char* kError;

  /**
   * @brief Name of "code" field.
   */
  static const char* kCode;

  /**
   * @brief Name of "data" field.
   */
  static const char* kData;

  /**
   * @brief Name of "message" field.
   */
  static const char* kMessage;

  /**
   * @brief Constructor.
   */
  FormatterJsonRpc();

  /**
   * @brief Copy constructor.
   *
   * @param unused Unused parameter.
   */
  FormatterJsonRpc(const FormatterJsonRpc& unused);

  /**
   * @brief Parse function id.
   *
   * @tparam FunctionId Type of function id enumeration.
   *
   * @param method_value JSON value with function id.
   * @param out The resulting SmartObject.
   *
   * @return An integer that is a bitwise-or of all error codes occurred
   *         during the parsing of the function id. 0 if no errors occurred.
   */
  template <typename FunctionId>
  static int32_t ParseFunctionId(const utils::json::JsonValueRef method_value,
                                 NsSmartObjects::SmartObject& out);

  /**
   * @brief Set method.
   *
   * Try to extract function id from the message object and set the "method"
   * field of the container.
   *
   * @param params Message parameters object.
   * @param method_container Container for the "method" field.
   *
   * @return true if function id was extracted successfully and set as a
   *         value of "method" field.
   */
  static bool SetMethod(const NsSmartObjects::SmartObject& params,
                        utils::json::JsonValueRef method_container);

  /**
   * @brief Set id.
   *
   * Try to extract request/response id from the message object and set the
   * "id" field of the container.
   *
   * @param params Message parameters object.
   * @param id_container Container for the "id" field.
   *
   * @return true if request/response id was extracted successfully and set
   *         as a value of "id" field.
   */
  static bool SetId(const NsSmartObjects::SmartObject& params,
                    utils::json::JsonValueRef id_container);

  /**
   * @brief Set message
   *
 * Try to extract message from response error object and set "message" field
 * for the container
   *
   * @param params Message parameters object.
   * @param id_container Container of the "message" field
   *
   * @return true if message string was extracted successfully and set
   *         as a value of "message" field.
   */
  static bool SetMessage(const NsSmartObjects::SmartObject& params,
                         utils::json::JsonValueRef id_container);
};

template <typename FunctionId, typename MessageType>
int32_t FormatterJsonRpc::FromString(const std::string& str,
                                     NsSmartObjects::SmartObject& out) {
  using namespace utils::json;
  int32_t result = kSuccess;
  try {
    namespace strings = NsSmartDeviceLink::NsJSONHandler::strings;

    JsonValue::ParseResult parse_result = JsonValue::Parse(str);
    if (!parse_result.second) {
      result = kParsingError | kMethodNotSpecified | kUnknownMethod |
               kUnknownMessageType;
    } else {
      JsonValue& root_json = parse_result.first;
      if (!root_json.HasMember(kJsonRpc)) {
        result |= kInvalidFormat;
      } else {
        const JsonValueRef jsonRpcValue = root_json[kJsonRpc];

        if (!jsonRpcValue.IsString() ||
            (jsonRpcValue.AsString() != kJsonRpcExpectedValue)) {
          result |= kInvalidFormat;
        }
      }

      std::string message_type_string;
      JsonValue response_value;
      bool response_value_found = false;
      bool is_error_response = false;

      if (!root_json.HasMember(kId)) {
        message_type_string = kNotification;

        if (!root_json.HasMember(kMethod)) {
          result |= kMethodNotSpecified | kUnknownMethod;
        } else {
          result |= ParseFunctionId<FunctionId>(root_json[kMethod], out);
        }
        out[strings::S_MSG_PARAMS] =
            NsSmartObjects::SmartObject(NsSmartObjects::SmartType_Map);
      } else {
        const JsonValueRef id_value = root_json[kId];

        if (id_value.IsString()) {
          out[strings::S_PARAMS][strings::S_CORRELATION_ID] =
              id_value.AsString();
        } else if (id_value.IsInt()) {
          out[strings::S_PARAMS][strings::S_CORRELATION_ID] =
              static_cast<int64_t>(id_value.AsInt());
        } else if (id_value.IsDouble()) {
          out[strings::S_PARAMS][strings::S_CORRELATION_ID] =
              id_value.AsDouble();
        } else if (id_value.IsNull()) {
          out[strings::S_PARAMS][strings::S_CORRELATION_ID] =
              NsSmartObjects::SmartObject(NsSmartObjects::SmartType_Null);
        } else {
          result |= kInvalidFormat | kInvalidId;
        }

        if (root_json.HasMember(kMethod)) {
          message_type_string = kRequest;
          result |= ParseFunctionId<FunctionId>(root_json[kMethod], out);
          out[strings::S_MSG_PARAMS] =
              NsSmartObjects::SmartObject(NsSmartObjects::SmartType_Map);
        } else {
          JsonValue method_container;
          bool method_container_found = false;

          if (root_json.HasMember(kResult)) {
            out[strings::S_MSG_PARAMS] =
                NsSmartObjects::SmartObject(NsSmartObjects::SmartType_Map);

            message_type_string = kResponse;
            response_value = root_json[kResult];
            response_value_found = true;
            method_container = root_json[kResult];
            method_container_found = true;
          } else if (root_json.HasMember(kError)) {
            out[strings::S_MSG_PARAMS] =
                NsSmartObjects::SmartObject(NsSmartObjects::SmartType_Map);
            message_type_string = kErrorResponse;
            response_value = root_json[kError];
            response_value_found = true;
            is_error_response = true;

            if (response_value.IsObject()) {
              if (response_value.HasMember(kData)) {
                method_container = response_value[kData];
                method_container_found = true;
              }
            }
          } else {
            result |= kUnknownMessageType;
          }

          if (!method_container_found) {
            result |= kMethodNotSpecified | kUnknownMethod;
          } else if (!method_container.IsObject()) {
            result |= kInvalidFormat | kMethodNotSpecified | kUnknownMethod;
          } else {
            if (!method_container.HasMember(kMethod)) {
              result |= kMethodNotSpecified | kUnknownMethod;
            } else {
              result |=
                  ParseFunctionId<FunctionId>(method_container[kMethod], out);
            }
          }
        }
      }

      if (0 == (result & kUnknownMessageType)) {
        MessageType message_type;

        if (!NsSmartObjects::EnumConversionHelper<MessageType>::StringToEnum(
                message_type_string, &message_type)) {
          result |= kUnknownMessageType;
        } else {
          out[strings::S_PARAMS][strings::S_MESSAGE_TYPE] = message_type;
        }
      }

      if (root_json.HasMember(kParams)) {
        const JsonValueRef params_value = root_json[kParams];

        if (!params_value.IsObject()) {
          result |= kInvalidFormat;
        } else {
          jsonValueToObj(root_json[kParams], out[strings::S_MSG_PARAMS]);
        }
      } else if (root_json.HasMember(kResult)) {
        const JsonValueRef result_value = root_json[kResult];

        if (!result_value.IsObject()) {
          result |= kInvalidFormat;
        } else {
          jsonValueToObj(root_json[kResult], out[strings::S_MSG_PARAMS]);
        }
      } else if (is_error_response) {
        jsonValueToObj(response_value[kData], out[strings::S_PARAMS][kData]);
      }

      if ((kResponse == message_type_string) ||
          (kErrorResponse == message_type_string)) {
        if (out.keyExists(strings::S_MSG_PARAMS)) {
          out[strings::S_MSG_PARAMS].erase(kMethod);
          out[strings::S_MSG_PARAMS].erase(kCode);
        }

        if (!response_value_found) {
          result |= kResponseCodeNotAvailable;
        } else {
          if (!response_value.IsObject()) {
            result |= kInvalidFormat | kResponseCodeNotAvailable;

            if (is_error_response) {
              result |= kErrorResponseMessageNotAvailable;
            }
          } else {
            if (!response_value.HasMember(kCode)) {
              result |= kResponseCodeNotAvailable;
            } else {
              const JsonValueRef code_value = response_value[kCode];

              if (!code_value.IsInt()) {
                result |= kInvalidFormat | kResponseCodeNotAvailable;
              } else {
                out[strings::S_PARAMS][strings::kCode] =
                    static_cast<int64_t>(code_value.AsInt());
              }
            }

            if (true == is_error_response) {
              if (!response_value.HasMember(kMessage)) {
                result |= kErrorResponseMessageNotAvailable;
              } else {
                const JsonValueRef message_value = response_value[kMessage];

                if (!message_value.IsString()) {
                  result |= kErrorResponseMessageNotAvailable;
                } else {
                  out[strings::S_PARAMS][strings::kMessage] =
                      message_value.AsString();
                }
              }
            }
          }
        }
      }
    }

    out[strings::S_PARAMS][strings::S_PROTOCOL_TYPE] = 1;
    out[strings::S_PARAMS][strings::S_PROTOCOL_VERSION] = 2;
  } catch (...) {
    result = kParsingError;
  }

  return result;
}

template <typename FunctionId>
int32_t FormatterJsonRpc::ParseFunctionId(
    const utils::json::JsonValueRef method_value,
    NsSmartObjects::SmartObject& out) {
  int32_t result = kSuccess;

  if (!method_value.IsString()) {
    result |= kInvalidFormat | kUnknownMethod;
  } else {
    FunctionId function_id;

    if (!NsSmartObjects::EnumConversionHelper<FunctionId>::CStringToEnum(
            method_value.AsString().c_str(), &function_id)) {
      result |= kUnknownMethod;
    } else {
      namespace strings = NsSmartDeviceLink::NsJSONHandler::strings;

      out[strings::S_PARAMS][strings::S_FUNCTION_ID] = function_id;
    }
  }

  return result;
}

}  // namespace Formatters
}  // namespace NsJSONHandler
}  // namespace NsSmartDeviceLink

#endif  // SRC_COMPONENTS_FORMATTERS_INCLUDE_FORMATTERS_FORMATTER_JSON_RPC_H_
