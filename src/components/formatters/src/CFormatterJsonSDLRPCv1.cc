/*
 * Copyright (c) 2015, Ford Motor Company
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

#include "formatters/CFormatterJsonSDLRPCv1.h"
#include "formatters/meta_formatter.h"
#include "utils/convert_utils.h"

namespace strings = NsSmartDeviceLink::NsJSONHandler::strings;
namespace smart_objects_ns = NsSmartDeviceLink::NsSmartObjects;

namespace NsSmartDeviceLink {
namespace NsJSONHandler {
namespace Formatters {

// ----------------------------------------------------------------------------

const std::string CFormatterJsonSDLRPCv1::S_REQUEST("request");
const std::string CFormatterJsonSDLRPCv1::S_RESPONSE("response");
const std::string CFormatterJsonSDLRPCv1::S_NOTIFICATION("notification");
const std::string CFormatterJsonSDLRPCv1::S_PARAMETERS("parameters");
const std::string CFormatterJsonSDLRPCv1::S_NAME("name");
const std::string CFormatterJsonSDLRPCv1::S_CORRELATION_ID("correlationID");

const int32_t CFormatterJsonSDLRPCv1::kSuccess = 0;
const int32_t CFormatterJsonSDLRPCv1::kParsingError = 1 << 0;
const int32_t CFormatterJsonSDLRPCv1::kFunctionIdNotFound = 1 << 1;
const int32_t CFormatterJsonSDLRPCv1::kMessageTypeNotFound = 1 << 2;
const int32_t CFormatterJsonSDLRPCv1::kCorrelationIdNotFound = 1 << 3;
// ----------------------------------------------------------------------------

const std::string CFormatterJsonSDLRPCv1::getMessageType(
    const smart_objects_ns::SmartObject& obj) {
  return obj.getElement(strings::S_PARAMS).getElement(strings::S_MESSAGE_TYPE)
      .asString();
}

// ----------------------------------------------------------------------------

const std::string CFormatterJsonSDLRPCv1::getMessageType(
    const Json::Value& root) {
  std::string type;

  if (true == root.isMember(S_REQUEST)) {
    type = S_REQUEST;
  } else if (true == root.isMember(S_RESPONSE)) {
    type = S_RESPONSE;
  } else if (true == root.isMember(S_NOTIFICATION)) {
    type = S_NOTIFICATION;
  } else {
  }

  return type;
}

// ----------------------------------------------------------------------------

bool CFormatterJsonSDLRPCv1::toString(const smart_objects_ns::SmartObject& obj,
                                      std::string& outStr) {
  bool result = false;
  try {
    Json::Value root(Json::objectValue);
    Json::Value params(Json::objectValue);

    smart_objects_ns::SmartObject formattedObj(obj);
    formattedObj.getSchema().unapplySchema(formattedObj);  // converts enums(as int32_t) to strings

    objToJsonValue(formattedObj.getElement(strings::S_MSG_PARAMS), params);

    std::string type = getMessageType(formattedObj);
    root[type] = Json::Value(Json::objectValue);
    root[type][S_PARAMETERS] = params;

    if (formattedObj[strings::S_PARAMS].keyExists(strings::S_CORRELATION_ID)) {
      root[type][S_CORRELATION_ID] = utils::ConvertInt64ToLongLongInt(
          formattedObj[strings::S_PARAMS][strings::S_CORRELATION_ID].asInt());
    }

    root[type][S_NAME] = formattedObj[strings::S_PARAMS][strings::S_FUNCTION_ID]
        .asString();

    outStr = root.toStyledString();

    result = true;
  } catch (...) {
    result = false;
  }

  return result;
}

// ----------------------------------------------------------------------------

CFormatterJsonSDLRPCv1::tMetaFormatterErrorCode CFormatterJsonSDLRPCv1::MetaFormatToString(
    const smart_objects_ns::SmartObject& object,
    const smart_objects_ns::CSmartSchema& schema, std::string& outStr) {
  meta_formatter_error_code::tMetaFormatterErrorCode result_code =
      meta_formatter_error_code::kErrorOk;

  smart_objects_ns::SmartObject tmp_object;

  if (false
      == CMetaFormatter::CreateObjectByPattern(object, schema, tmp_object)) {
    result_code |= meta_formatter_error_code::kErrorFailedCreateObjectBySchema;
    return result_code;
  }

  // determine whether smart objects are functions
  // (in terms of SDLRPC communication)
  bool is_root_object_created_by_schema = ((tmp_object.getType()
      == smart_objects_ns::SmartType_Map)
      && tmp_object.keyExists(strings::S_PARAMS)
      && tmp_object.keyExists(strings::S_MSG_PARAMS));

  bool is_root_object = ((object.getType() == smart_objects_ns::SmartType_Map)
      && object.keyExists(strings::S_PARAMS)
      && object.keyExists(strings::S_MSG_PARAMS));

  if (false == is_root_object) {
    result_code |= meta_formatter_error_code::kErrorObjectIsNotFunction;
  }
  if (false == is_root_object_created_by_schema) {
    result_code |= meta_formatter_error_code::kErrorSchemaIsNotFunction;
  }

  CFormatterJsonSDLRPCv1::toString(tmp_object, outStr);

  return result_code;
}

}
}
}
