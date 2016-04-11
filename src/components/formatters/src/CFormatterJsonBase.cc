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

#include "utils/json_utils.h"
#include "utils/convert_utils.h"
#include "formatters/CFormatterJsonBase.h"

void NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonBase::
    jsonValueToObj(const utils::json::JsonValueRef value,
                   NsSmartDeviceLink::NsSmartObjects::SmartObject& obj) {
  using namespace utils::json;
  try {
    const ValueType::Type json_type = value.Type();
    if (json_type == ValueType::OBJECT_VALUE) {
      obj = NsSmartDeviceLink::NsSmartObjects::SmartObject(
          NsSmartDeviceLink::NsSmartObjects::SmartType_Map);

      for (JsonValue::const_iterator itr = value.begin(), end = value.end();
           itr != end;
           ++itr) {
        jsonValueToObj(*itr, obj[itr.key().AsString()]);
      }
    } else if (json_type == ValueType::ARRAY_VALUE) {
      obj = NsSmartDeviceLink::NsSmartObjects::SmartObject(
          NsSmartDeviceLink::NsSmartObjects::SmartType_Array);

      for (JsonValue::ArrayIndex i = 0, size = value.Size(); i < size; ++i) {
        jsonValueToObj(value[i], obj[i]);
      }
    } else if (json_type == ValueType::INT_VALUE) {
      obj = utils::ConvertLongLongIntToInt64(value.AsInt());
    } else if (json_type == ValueType::UINT_VALUE) {
      obj = utils::ConvertLongLongUIntToUInt64(value.AsUInt());
    } else if (json_type == ValueType::REAL_VALUE) {
      obj = value.AsDouble();
    } else if (json_type == ValueType::BOOL_VALUE) {
      obj = value.AsBool();
    } else if (json_type == ValueType::STRING_VALUE) {
      obj = value.AsString();
    }
  } catch (...) {
  }
}

// ----------------------------------------------------------------------------

void NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonBase::
    objToJsonValue(const NsSmartDeviceLink::NsSmartObjects::SmartObject& obj,
                   utils::json::JsonValueRef item) {
  using namespace utils::json;
  try {
    if (NsSmartDeviceLink::NsSmartObjects::SmartType_Array == obj.getType()) {
      item = ValueType::ARRAY_VALUE;

      for (uint32_t i = 0; i < obj.length(); i++) {
        JsonValue value;

        objToJsonValue(obj.getElement(i), value);

        item.Append(value);
      }
    } else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Map ==
               obj.getType()) {
      item = ValueType::OBJECT_VALUE;
      std::set<std::string> keys = obj.enumerate();

      for (std::set<std::string>::const_iterator key_itr = keys.begin(),
                                                 end = keys.end();
           key_itr != end;
           ++key_itr) {
        JsonValue value;

        objToJsonValue(obj.getElement(*key_itr), value);

        item[*key_itr] = value;
      }
    } else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Boolean ==
               obj.getType()) {
      item = obj.asBool();
    } else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Integer ==
               obj.getType()) {
      item = utils::ConvertInt64ToLongLongInt(obj.asInt());
    } else if (NsSmartDeviceLink::NsSmartObjects::SmartType_UInteger ==
               obj.getType()) {
      item = utils::ConvertUInt64ToLongLongUInt(obj.asUInt());
    } else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Double ==
               obj.getType()) {
      item = obj.asDouble();
    } else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Null ==
               obj.getType()) {
      item = ValueType::NULL_VALUE;
    } else {
      item = obj.asString();
    }
  } catch (...) {
  }
}
