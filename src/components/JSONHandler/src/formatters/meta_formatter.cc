/**
 * @file meta_fromatter.cc
 * @brief implementation of class CMetaFormatter which is designed to format
 *        the smart object against given schema for given formatter
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
// POSSIBILITY OF SUCH DAMAGE.auto

#include "JSONHandler/formatters/meta_formatter.h"
#include "JSONHandler/CSmartFactory.hpp"
#include "JSONHandler/formatters/CFormatterJsonALRPCv1.hpp"
#include "JSONHandler/formatters/CFormatterJsonALRPCv2.hpp"

namespace formatter_ns = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace smart_objects_ns = NsSmartDeviceLink::NsSmartObjects;
namespace jsonhandler_ns = NsSmartDeviceLink::NsJSONHandler;

//---------------------------------------------------------------
formatter_ns::meta_formatter_error_code::tMetaFormatterErrorCode
  formatter_ns::CMetaFormatter::FormatToString(
                const NsSmartDeviceLink::NsSmartObjects::CSmartObject& object,
                NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema,
                const e_formatter_version::Type formatter_version,
                std::string& result_string) {
  meta_formatter_error_code::tMetaFormatterErrorCode result_code
                                    = meta_formatter_error_code::ERROR_OK;
  NsSmartDeviceLink::NsSmartObjects::CSmartObject tmp_object(
                                            smart_objects_ns::SmartType_Map);

  // build temporary object by schema filled with default values
  schema.BuildObjectBySchema(tmp_object);

  // copy parameters from source object to tmp object
  bool is_root_schema = (
    (tmp_object.getType() == smart_objects_ns::SmartType_Map)
      && tmp_object.keyExists(jsonhandler_ns::strings::S_PARAMS)
      && tmp_object.keyExists(jsonhandler_ns::strings::S_MSG_PARAMS));

  bool is_root_object = (
    (object.getType() == smart_objects_ns::SmartType_Map)
      && object.keyExists(jsonhandler_ns::strings::S_PARAMS)
      && object.keyExists(jsonhandler_ns::strings::S_MSG_PARAMS));

  if (true == is_root_schema && true == is_root_object) {
    CMetaFormatter::CopyServiceParams(object, tmp_object);
    
    CMetaFormatter::FillOutputObject(
                      object.getElement(jsonhandler_ns::strings::S_MSG_PARAMS),
                      tmp_object[jsonhandler_ns::strings::S_MSG_PARAMS]);
  } else {
    if (false == is_root_object) {
      result_code |= meta_formatter_error_code::ERROR_OBJECT_IS_NOT_FUNCTION;
    }
    if (false == is_root_schema) {
      result_code |= meta_formatter_error_code::ERROR_SCHEMA_IS_NOT_FUNCTION;
    }
  }

  // call respective formatter->toString()
  if (e_formatter_version::FORMATTER_SDLRPCv1 == formatter_version) {
    formatter_ns::CFormatterJsonALRPCv1::toString(tmp_object, result_string);
  } else if (e_formatter_version::FORMATTER_SDLRPCv2 == formatter_version) {
    formatter_ns::CFormatterJsonALRPCv2::toString(tmp_object, result_string);
  }
  
  return result_code;
}

//---------------------------------------------------------------
void formatter_ns::CMetaFormatter::CopyServiceParams(
        NsSmartDeviceLink::NsSmartObjects::CSmartObject in_object,
        NsSmartDeviceLink::NsSmartObjects::CSmartObject& out_object) {
  
  if (true == in_object.keyExists(jsonhandler_ns::strings::S_PARAMS)) {
    out_object[jsonhandler_ns::strings::S_PARAMS]
                [jsonhandler_ns::strings::S_FUNCTION_ID] =
    in_object[jsonhandler_ns::strings::S_PARAMS]
               [jsonhandler_ns::strings::S_FUNCTION_ID];

    out_object[jsonhandler_ns::strings::S_PARAMS]
                [jsonhandler_ns::strings::S_MESSAGE_TYPE] =
    in_object[jsonhandler_ns::strings::S_PARAMS]
               [jsonhandler_ns::strings::S_MESSAGE_TYPE];

    out_object[jsonhandler_ns::strings::S_PARAMS]
                [jsonhandler_ns::strings::S_CORRELATION_ID] =
    in_object[jsonhandler_ns::strings::S_PARAMS]
               [jsonhandler_ns::strings::S_CORRELATION_ID];

    out_object[jsonhandler_ns::strings::S_PARAMS]
                [jsonhandler_ns::strings::S_PROTOCOL_TYPE] =
    in_object[jsonhandler_ns::strings::S_PARAMS]
               [jsonhandler_ns::strings::S_PROTOCOL_TYPE];

    out_object[jsonhandler_ns::strings::S_PARAMS]
                [jsonhandler_ns::strings::S_PROTOCOL_VERSION] =
    in_object[jsonhandler_ns::strings::S_PARAMS]
               [jsonhandler_ns::strings::S_PROTOCOL_VERSION];
  }  
}

//---------------------------------------------------------------
void formatter_ns::CMetaFormatter::FillOutputObject(
        const NsSmartDeviceLink::NsSmartObjects::CSmartObject& in_object,
        NsSmartDeviceLink::NsSmartObjects::CSmartObject& out_object) {
  
  if (in_object.getType() == smart_objects_ns::SmartType_Invalid
      || in_object.getType() == smart_objects_ns::SmartType_Null
      || in_object.getType() != out_object.getType()) {
    return;
  }

  if (in_object.getType() == smart_objects_ns::SmartType_Map) {
    
    const std::set<std::string> in_object_keys = in_object.enumerate();
    for(std::set<std::string>::const_iterator i = in_object_keys.begin();
        i != in_object_keys.end(); ++i) {
      if (out_object.keyExists(*i)) {
        CMetaFormatter::FillOutputObject(in_object.getElement(*i),
                                          out_object[*i]);
      }      
    }      
  } else if (in_object.getType() == smart_objects_ns::SmartType_Array) {
    
    for(int i = 0; i < in_object.length(); i++) {
      CMetaFormatter::FillOutputObject(in_object.getElement(i), out_object[i]);      
    }    
  } else {

    out_object = in_object;
  }
}  
  