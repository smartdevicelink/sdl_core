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

#include "formatters/CFormatterJsonSDLRPCv2.h"
#include "formatters/meta_formatter.h"

namespace smart_objects_ns = NsSmartDeviceLink::NsSmartObjects;
namespace strings = NsSmartDeviceLink::NsJSONHandler::strings;

namespace NsSmartDeviceLink {
namespace NsJSONHandler {
namespace Formatters {

// ----------------------------------------------------------------------------

bool CFormatterJsonSDLRPCv2::toString(const smart_objects_ns::SmartObject& obj,
                                      std::string& outStr) {
  bool result = true;
  try {
    Json::Value root(Json::objectValue);

    smart_objects_ns::SmartObject formattedObj(obj);
    formattedObj.getSchema().unapplySchema(formattedObj);  // converts enums(as int32_t) to strings

    objToJsonValue(formattedObj.getElement(strings::S_MSG_PARAMS), root);

    outStr = root.toStyledString();

    result = true;
  } catch (...) {
    result = false;
  }

  return result;
}

// ----------------------------------------------------------------------------

CFormatterJsonSDLRPCv2::tMetaFormatterErrorCode CFormatterJsonSDLRPCv2::MetaFormatToString(
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

  CFormatterJsonSDLRPCv2::toString(tmp_object, outStr);

  return result_code;
}

}
}
}
