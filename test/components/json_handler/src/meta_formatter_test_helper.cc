/**
 * @file meta_formatter_test_helper.cc
 * @brief implementation of class CMetaFormatterTestHelper which is designed to
 *        create test environemnt to test class CMetaFormatter
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


#include "formatters/CFormatterJsonBase.hpp"
#include "json_handler/meta_formatter_test_helper.h"

// using test::components::JSONHandler::formatters;
using namespace NsSmartDeviceLink::NsJSONHandler::Formatters;
using namespace NsSmartDeviceLink::NsJSONHandler::strings;

namespace test {
namespace components {
namespace JSONHandler {
namespace formatters {

void CMetaFormatterTestHelper::SetUp() {

  function_id_items_.insert(function_id::kRegisterAppInterfaceID);
  function_id_items_.insert(function_id::kUnregisterAppInterfaceID);
  function_id_items_.insert(function_id::kSetGlobalPropertiesID);

  message_type_items_.insert(message_type::kRequest);
  message_type_items_.insert(message_type::kResponse);
  message_type_items_.insert(message_type::kNotification);
}

void CMetaFormatterTestHelper::TearDown() {
  function_id_items_.clear();
  message_type_items_.clear();
}

//-----------------------------------------------------------

void CMetaFormatterTestHelper::AnyObjectToJsonString(
    const NsSmartDeviceLink::NsSmartObjects::SmartObject& obj, std::string& result_string) {

    Json::Value params(Json::objectValue);

    NsSmartDeviceLink::NsSmartObjects::SmartObject formattedObj(obj);

    CFormatterJsonBase::objToJsonValue(formattedObj, params);

    result_string = params.toStyledString();
}

//-----------------------------------------------------------

void CMetaFormatterTestHelper::FillObjectIdenticalToSchema(
    NsSmartDeviceLink::NsSmartObjects::SmartObject& obj) {

  obj[S_PARAMS][S_MESSAGE_TYPE] =
      generated_ns::messageType::request;
  obj[S_PARAMS][S_FUNCTION_ID] =
      generated_ns::FunctionID::RegisterAppInterfaceID;
  obj[S_PARAMS][S_CORRELATION_ID] = 12;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;

  obj[S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"] = 2;
  obj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"] = 10;
  obj[S_MSG_PARAMS]["appName"] = "APP NAME";
  obj[S_MSG_PARAMS]["ttsName"][0]["text"] = "ABC";
  obj[S_MSG_PARAMS]["ttsName"][0]["type"] =
      generated_ns::SpeechCapabilities::SC_TEXT;
  obj[S_MSG_PARAMS]["ngnMediaScreenAppName"] = "SCREEN NAME";
  obj[S_MSG_PARAMS]["vrSynonyms"][0] = "Synonym1";
  obj[S_MSG_PARAMS]["vrSynonyms"][1] = "Synonym2";
  obj[S_MSG_PARAMS]["isMediaApplication"] = true;
  obj[S_MSG_PARAMS]["languageDesired"] =
      generated_ns::Language::EN_EU;
  obj[S_MSG_PARAMS]["hmiDisplayLanguageDesired"] =
      generated_ns::Language::RU_RU;
  obj[S_MSG_PARAMS]["appType"][0] = generated_ns::AppType::SYSTEM;
  obj[S_MSG_PARAMS]["appType"][1] = generated_ns::AppType::MEDIA;
  obj[S_MSG_PARAMS]["appID"] = "APP ID";
}

//-----------------------------------------------------------

void CMetaFormatterTestHelper::FillObjectIdenticalToSchemaWithoutNoMandatoriesParams(
        NsSmartDeviceLink::NsSmartObjects::SmartObject& obj) {
  obj[S_PARAMS][S_MESSAGE_TYPE] =
      generated_ns::messageType::request;
  obj[S_PARAMS][S_FUNCTION_ID] =
      generated_ns::FunctionID::RegisterAppInterfaceID;
  obj[S_PARAMS][S_CORRELATION_ID] = 12;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;

  obj[S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"] = 2;
  obj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"] = 10;
  obj[S_MSG_PARAMS]["appName"] = "APP NAME";
//   obj[S_MSG_PARAMS]["ttsName"][0]["text"] = "ABC"; // not mandatory
//   obj[S_MSG_PARAMS]["ttsName"][0]["type"] =
//       generated_ns::SpeechCapabilities::SC_TEXT;
  obj[S_MSG_PARAMS]["ngnMediaScreenAppName"] = "SCREEN NAME";
//   obj[S_MSG_PARAMS]["vrSynonyms"][0] = "Synonym1"; // not mandatory
//   obj[S_MSG_PARAMS]["vrSynonyms"][1] = "Synonym2";
  obj[S_MSG_PARAMS]["isMediaApplication"] = true;
  obj[S_MSG_PARAMS]["languageDesired"] =
      generated_ns::Language::EN_EU;
  obj[S_MSG_PARAMS]["hmiDisplayLanguageDesired"] =
      generated_ns::Language::RU_RU;
//   obj[S_MSG_PARAMS]["appType"][0] = generated_ns::AppType::SYSTEM; // not mandatory
//   obj[S_MSG_PARAMS]["appType"][1] = generated_ns::AppType::MEDIA;
  obj[S_MSG_PARAMS]["appID"] = "APP ID";
}

void CMetaFormatterTestHelper::FillObjectWithoutSomeMandatoryFields(
      NsSmartDeviceLink::NsSmartObjects::SmartObject& obj) {
obj[S_PARAMS][S_MESSAGE_TYPE] =
      generated_ns::messageType::request;
  obj[S_PARAMS][S_FUNCTION_ID] =
      generated_ns::FunctionID::RegisterAppInterfaceID;
//   obj[S_PARAMS][S_CORRELATION_ID] = 12;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;

//   obj[S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"] = 2;
//   obj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"] = 10;
  obj[S_MSG_PARAMS]["appName"] = "APP NAME";
  obj[S_MSG_PARAMS]["ttsName"][0]["text"] = "ABC";
  obj[S_MSG_PARAMS]["ttsName"][0]["type"] =
      generated_ns::SpeechCapabilities::SC_TEXT;
  obj[S_MSG_PARAMS]["ngnMediaScreenAppName"] = "SCREEN NAME";
  obj[S_MSG_PARAMS]["vrSynonyms"][0] = "Synonym1";
  obj[S_MSG_PARAMS]["vrSynonyms"][1] = "Synonym2";
  obj[S_MSG_PARAMS]["isMediaApplication"] = true;
  obj[S_MSG_PARAMS]["languageDesired"] =
      generated_ns::Language::EN_EU;
  obj[S_MSG_PARAMS]["hmiDisplayLanguageDesired"] =
      generated_ns::Language::RU_RU;
  obj[S_MSG_PARAMS]["appType"][0] = generated_ns::AppType::SYSTEM;
  obj[S_MSG_PARAMS]["appType"][1] = generated_ns::AppType::MEDIA;
  obj[S_MSG_PARAMS]["appID"] = "APP ID";
}

//-----------------------------------------------------------

void CMetaFormatterTestHelper::CompareObjects(
        const NsSmartDeviceLink::NsSmartObjects::SmartObject& first,
        const NsSmartDeviceLink::NsSmartObjects::SmartObject& second) {

  if (NsSmartDeviceLink::NsSmartObjects::SmartType_Array == first.getType()) {
    ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::SmartType_Array, second.getType());
    for (size_t i = 0; i < first.length(); i++) {
      CompareObjects(first.getElement(i), second.getElement(i));
    }
  }
  else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Map == first.getType())
  {
    ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::SmartType_Map, second.getType());
    std::set<std::string> keys = first.enumerate();

    for (std::set<std::string>::const_iterator key = keys.begin();
          key != keys.end(); key++){
      CompareObjects(first.getElement(*key), second.getElement(*key));
    }
  }
  else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Boolean == first.getType())  {
      ASSERT_EQ(first.asBool(), second.asBool());
  }
  else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Integer == first.getType()) {
      ASSERT_EQ(first.asInt(), second.asInt());
  }
  else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Double == first.getType())  {
      ASSERT_EQ(first.asDouble(), second.asDouble());
  }
  else if (NsSmartDeviceLink::NsSmartObjects::SmartType_String == first.getType()) {
      ASSERT_EQ(first.asString(), second.asString());
  }
  else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Null == first.getType())  {
      ASSERT_EQ(NsSmartDeviceLink::NsSmartObjects::SmartType_Null, second.getType());
  }
  else
  {
      FAIL() << "Unknown SmartObject type: " << first.getType();
  }
}

//-----------------------------------------------------------

void CMetaFormatterTestHelper::FillObjectWithDefaultValues(
      NsSmartDeviceLink::NsSmartObjects::SmartObject& obj) {

  obj[S_PARAMS][S_MESSAGE_TYPE] = -1;
  obj[S_PARAMS][S_FUNCTION_ID] = -1;
  obj[S_PARAMS][S_CORRELATION_ID] = 0;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 0;
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;

  obj[S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"] = 0;
  obj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"] = 0;
  obj[S_MSG_PARAMS]["appName"] = "";
//   obj[S_MSG_PARAMS]["ttsName"][0]["text"] = "ABC"; // not mandatory
//   obj[S_MSG_PARAMS]["ttsName"][0]["type"] =
//       generated_ns::SpeechCapabilities::SC_TEXT;
  obj[S_MSG_PARAMS]["ngnMediaScreenAppName"] = "";
//   obj[S_MSG_PARAMS]["vrSynonyms"][0] = "Synonym1"; // not mandatory
//   obj[S_MSG_PARAMS]["vrSynonyms"][1] = "Synonym2";
  obj[S_MSG_PARAMS]["isMediaApplication"] = false;
  obj[S_MSG_PARAMS]["languageDesired"] = -1;
  obj[S_MSG_PARAMS]["hmiDisplayLanguageDesired"] = -1;
//   obj[S_MSG_PARAMS]["appType"][0] = generated_ns::AppType::SYSTEM; // not mandatory
//   obj[S_MSG_PARAMS]["appType"][1] = generated_ns::AppType::MEDIA;
  obj[S_MSG_PARAMS]["appID"] = "";
}

}
}
}
}
