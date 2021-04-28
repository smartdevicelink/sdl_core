/**
 * @file CFormatterJsonBase.cpp
 * @brief CFormatterJsonBase source file.
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
#include "formatters/CFormatterJsonBase.h"
#include "json/json.h"
#include "utils/convert_utils.h"

void ns_smart_device_link::ns_json_handler::formatters::CFormatterJsonBase::
    jsonValueToObj(const Json::Value& value,
                   ns_smart_device_link::ns_smart_objects::SmartObject& obj) {
  try {
    if (value.type() == Json::objectValue) {
      obj = ns_smart_device_link::ns_smart_objects::SmartObject(
          ns_smart_device_link::ns_smart_objects::SmartType_Map);

      Json::Value::Members members = value.getMemberNames();

      for (uint32_t i = 0; i < members.size(); ++i) {
        jsonValueToObj(value[members[i]], obj[members[i]]);
      }
    } else if (value.type() == Json::arrayValue) {
      obj = ns_smart_device_link::ns_smart_objects::SmartObject(
          ns_smart_device_link::ns_smart_objects::SmartType_Array);

      for (uint32_t i = 0; i < value.size(); ++i) {
        jsonValueToObj(value[i], obj[i]);
      }
    } else if (value.type() == Json::intValue) {
      obj = utils::ConvertLongLongIntToInt64(value.asInt64());
    } else if (value.type() == Json::uintValue) {
      obj = utils::ConvertLongLongUIntToUInt64(value.asUInt64());
    } else if (value.type() == Json::realValue) {
      obj = value.asDouble();
    } else if (value.type() == Json::booleanValue) {
      obj = value.asBool();
    } else if (value.type() == Json::stringValue) {
      obj = value.asString();
    }
  } catch (...) {
  }
}

// ----------------------------------------------------------------------------

void ns_smart_device_link::ns_json_handler::formatters::CFormatterJsonBase::
    objToJsonValue(
        const ns_smart_device_link::ns_smart_objects::SmartObject& obj,
        Json::Value& item) {
  try {
    if (ns_smart_device_link::ns_smart_objects::SmartType_Array ==
        obj.getType()) {
      item = Json::arrayValue;

      for (uint32_t i = 0; i < obj.length(); ++i) {
        Json::Value value(Json::nullValue);

        objToJsonValue(obj.getElement(i), value);

        item.append(value);
      }
    } else if (ns_smart_device_link::ns_smart_objects::SmartType_Map ==
               obj.getType()) {
      item = Json::objectValue;
      std::set<std::string> keys = obj.enumerate();

      for (std::set<std::string>::const_iterator key = keys.begin();
           key != keys.end();
           ++key) {
        Json::Value value(Json::nullValue);

        objToJsonValue(obj.getElement(*key), value);

        item[*key] = value;
      }
    } else if (ns_smart_device_link::ns_smart_objects::SmartType_Boolean ==
               obj.getType()) {
      item = obj.asBool();
    } else if (ns_smart_device_link::ns_smart_objects::SmartType_Integer ==
               obj.getType()) {
      item = obj.asInt();
    } else if (ns_smart_device_link::ns_smart_objects::SmartType_UInteger ==
               obj.getType()) {
      item = obj.asUInt();
    } else if (ns_smart_device_link::ns_smart_objects::SmartType_Double ==
               obj.getType()) {
      item = obj.asDouble();
    } else if (ns_smart_device_link::ns_smart_objects::SmartType_Null ==
               obj.getType()) {
      item = Json::nullValue;
    } else {
      item = obj.asString();
    }
  } catch (...) {
  }
}
