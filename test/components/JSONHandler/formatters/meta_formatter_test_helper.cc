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


#include "meta_formatter_test_helper.h"
#include <JSONHandler/formatters/CFormatterJsonBase.hpp>

namespace testhelper_ns = test::components::JSONHandler::formatters;
using test::components::JSONHandler::formatters::CMetaFormatterTestHelper;
namespace formatter_ns = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace smart_objects_ns = NsSmartDeviceLink::NsSmartObjects;
namespace jsonhandler_ns = NsSmartDeviceLink::NsJSONHandler;
namespace strings_ns = NsSmartDeviceLink::NsJSONHandler::strings;


void CMetaFormatterTestHelper::SetUp() {
  
  function_id_items_.insert(testhelper_ns::function_id::kRegisterAppInterfaceID);
  function_id_items_.insert(testhelper_ns::function_id::kUnregisterAppInterfaceID);
  function_id_items_.insert(testhelper_ns::function_id::kSetGlobalPropertiesID);
  
  message_type_items_.insert(testhelper_ns::message_type::kRequest);
  message_type_items_.insert(testhelper_ns::message_type::kResponse);
  message_type_items_.insert(testhelper_ns::message_type::kNotification);
}

void CMetaFormatterTestHelper::TearDown() {
  function_id_items_.clear();
  message_type_items_.clear();
}

//-----------------------------------------------------------

void CMetaFormatterTestHelper::AnyObjectToJsonString(
    const smart_objects_ns::SmartObject& obj, std::string& result_string) {  
  
    Json::Value params(Json::objectValue);

    smart_objects_ns::SmartObject formattedObj(obj);

    formatter_ns::CFormatterJsonBase::objToJsonValue(formattedObj, params);

    result_string = params.toStyledString();
}

//-----------------------------------------------------------

void CMetaFormatterTestHelper::FillObjectIdenticalToSchema(
    NsSmartDeviceLink::NsSmartObjects::SmartObject& obj) {
  
  obj[strings_ns::S_PARAMS][strings_ns::S_MESSAGE_TYPE] =
      generated_ns::messageType::request;
  obj[strings_ns::S_PARAMS][strings_ns::S_FUNCTION_ID] =
      generated_ns::FunctionID::RegisterAppInterfaceID;
  obj[strings_ns::S_PARAMS][strings_ns::S_CORRELATION_ID] = 12;
  obj[strings_ns::S_PARAMS][strings_ns::S_PROTOCOL_VERSION] = 1;
  obj[strings_ns::S_PARAMS][strings_ns::S_PROTOCOL_TYPE] = 0;

  obj[strings_ns::S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"] = 2;
  obj[strings_ns::S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"] = 10;
  obj[strings_ns::S_MSG_PARAMS]["appName"] = "APP NAME";
  obj[strings_ns::S_MSG_PARAMS]["ttsName"][0]["text"] = "ABC";
  obj[strings_ns::S_MSG_PARAMS]["ttsName"][0]["type"] =
      generated_ns::SpeechCapabilities::SC_TEXT;
  obj[strings_ns::S_MSG_PARAMS]["ngnMediaScreenAppName"] = "SCREEN NAME";
  obj[strings_ns::S_MSG_PARAMS]["vrSynonyms"][0] = "Synonym1";
  obj[strings_ns::S_MSG_PARAMS]["vrSynonyms"][1] = "Synonym2";
  obj[strings_ns::S_MSG_PARAMS]["isMediaApplication"] = true;
  obj[strings_ns::S_MSG_PARAMS]["languageDesired"] =
      generated_ns::Language::EN_EU;
  obj[strings_ns::S_MSG_PARAMS]["hmiDisplayLanguageDesired"] =
      generated_ns::Language::RU_RU;
  obj[strings_ns::S_MSG_PARAMS]["appType"][0] = generated_ns::AppType::SYSTEM;
  obj[strings_ns::S_MSG_PARAMS]["appType"][1] = generated_ns::AppType::MEDIA;
  obj[strings_ns::S_MSG_PARAMS]["appID"] = "APP ID";
}

//-----------------------------------------------------------

void CMetaFormatterTestHelper::FillObjectIdenticalToSchemaWithoutNoMandatoriesParams(
        NsSmartDeviceLink::NsSmartObjects::SmartObject& obj) {
  obj[strings_ns::S_PARAMS][strings_ns::S_MESSAGE_TYPE] =
      generated_ns::messageType::request;
  obj[strings_ns::S_PARAMS][strings_ns::S_FUNCTION_ID] =
      generated_ns::FunctionID::RegisterAppInterfaceID;
  obj[strings_ns::S_PARAMS][strings_ns::S_CORRELATION_ID] = 12;
  obj[strings_ns::S_PARAMS][strings_ns::S_PROTOCOL_VERSION] = 1;
  obj[strings_ns::S_PARAMS][strings_ns::S_PROTOCOL_TYPE] = 0;

  obj[strings_ns::S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"] = 2;
  obj[strings_ns::S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"] = 10;
  obj[strings_ns::S_MSG_PARAMS]["appName"] = "APP NAME";
//   obj[strings_ns::S_MSG_PARAMS]["ttsName"][0]["text"] = "ABC"; // not mandatory
//   obj[strings_ns::S_MSG_PARAMS]["ttsName"][0]["type"] =
//       generated_ns::SpeechCapabilities::SC_TEXT;
  obj[strings_ns::S_MSG_PARAMS]["ngnMediaScreenAppName"] = "SCREEN NAME";
//   obj[strings_ns::S_MSG_PARAMS]["vrSynonyms"][0] = "Synonym1"; // not mandatory
//   obj[strings_ns::S_MSG_PARAMS]["vrSynonyms"][1] = "Synonym2"; 
  obj[strings_ns::S_MSG_PARAMS]["isMediaApplication"] = true;
  obj[strings_ns::S_MSG_PARAMS]["languageDesired"] =
      generated_ns::Language::EN_EU;
  obj[strings_ns::S_MSG_PARAMS]["hmiDisplayLanguageDesired"] =
      generated_ns::Language::RU_RU;
//   obj[strings_ns::S_MSG_PARAMS]["appType"][0] = generated_ns::AppType::SYSTEM; // not mandatory
//   obj[strings_ns::S_MSG_PARAMS]["appType"][1] = generated_ns::AppType::MEDIA; 
  obj[strings_ns::S_MSG_PARAMS]["appID"] = "APP ID"; 
}

void CMetaFormatterTestHelper::FillObjectWithoutSomeMandatoryFields(
      NsSmartDeviceLink::NsSmartObjects::SmartObject& obj) {
obj[strings_ns::S_PARAMS][strings_ns::S_MESSAGE_TYPE] =
      generated_ns::messageType::request;
  obj[strings_ns::S_PARAMS][strings_ns::S_FUNCTION_ID] =
      generated_ns::FunctionID::RegisterAppInterfaceID;
//   obj[strings_ns::S_PARAMS][strings_ns::S_CORRELATION_ID] = 12;
  obj[strings_ns::S_PARAMS][strings_ns::S_PROTOCOL_VERSION] = 1;
  obj[strings_ns::S_PARAMS][strings_ns::S_PROTOCOL_TYPE] = 0;

//   obj[strings_ns::S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"] = 2;
//   obj[strings_ns::S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"] = 10;
  obj[strings_ns::S_MSG_PARAMS]["appName"] = "APP NAME";
  obj[strings_ns::S_MSG_PARAMS]["ttsName"][0]["text"] = "ABC";
  obj[strings_ns::S_MSG_PARAMS]["ttsName"][0]["type"] =
      generated_ns::SpeechCapabilities::SC_TEXT;
  obj[strings_ns::S_MSG_PARAMS]["ngnMediaScreenAppName"] = "SCREEN NAME";
  obj[strings_ns::S_MSG_PARAMS]["vrSynonyms"][0] = "Synonym1";
  obj[strings_ns::S_MSG_PARAMS]["vrSynonyms"][1] = "Synonym2";
  obj[strings_ns::S_MSG_PARAMS]["isMediaApplication"] = true;
  obj[strings_ns::S_MSG_PARAMS]["languageDesired"] =
      generated_ns::Language::EN_EU;
  obj[strings_ns::S_MSG_PARAMS]["hmiDisplayLanguageDesired"] =
      generated_ns::Language::RU_RU;
  obj[strings_ns::S_MSG_PARAMS]["appType"][0] = generated_ns::AppType::SYSTEM;
  obj[strings_ns::S_MSG_PARAMS]["appType"][1] = generated_ns::AppType::MEDIA;
  obj[strings_ns::S_MSG_PARAMS]["appID"] = "APP ID";   
}

//-----------------------------------------------------------

void CMetaFormatterTestHelper::CompareObjects(
        const NsSmartDeviceLink::NsSmartObjects::SmartObject& first,
        const NsSmartDeviceLink::NsSmartObjects::SmartObject& second) {
  
  if (smart_objects_ns::SmartType_Array == first.getType()) {
    ASSERT_EQ(smart_objects_ns::SmartType_Array, second.getType());
    for (int i = 0; i < first.length(); i++) {
      CompareObjects(first.getElement(i), second.getElement(i));
    }
  }
  else if (smart_objects_ns::SmartType_Map == first.getType())
  {
    ASSERT_EQ(smart_objects_ns::SmartType_Map, second.getType());
    std::set<std::string> keys = first.enumerate();

    for (std::set<std::string>::const_iterator key = keys.begin();
          key != keys.end(); key++){
      CompareObjects(first.getElement(*key), second.getElement(*key));
    }
  }
  else if (smart_objects_ns::SmartType_Boolean == first.getType())  {
      ASSERT_EQ((bool)first, (bool)second);
  }
  else if (smart_objects_ns::SmartType_Integer == first.getType()) {
      ASSERT_EQ((int)first, (int)second);
  }
  else if (smart_objects_ns::SmartType_Double == first.getType())  {
      ASSERT_EQ((double)first, (double)second);
  }
  else if (smart_objects_ns::SmartType_String == first.getType()) {
      ASSERT_EQ((std::string)first, (std::string)second);
  }
  else if (smart_objects_ns::SmartType_Null == first.getType())  {
      ASSERT_EQ(smart_objects_ns::SmartType_Null, second.getType());
  }
  else
  {
      FAIL() << "Unknown SmartObject type: " << first.getType();
  }
}

//-----------------------------------------------------------

void CMetaFormatterTestHelper::FillObjectWithDefaultValues(
      NsSmartDeviceLink::NsSmartObjects::SmartObject& obj) {

  obj[strings_ns::S_PARAMS][strings_ns::S_MESSAGE_TYPE] = -1;      
  obj[strings_ns::S_PARAMS][strings_ns::S_FUNCTION_ID] = -1;
  obj[strings_ns::S_PARAMS][strings_ns::S_CORRELATION_ID] = 0;
  obj[strings_ns::S_PARAMS][strings_ns::S_PROTOCOL_VERSION] = 0;
  obj[strings_ns::S_PARAMS][strings_ns::S_PROTOCOL_TYPE] = 0;

  obj[strings_ns::S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"] = 0;
  obj[strings_ns::S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"] = 0;
  obj[strings_ns::S_MSG_PARAMS]["appName"] = "";
//   obj[strings_ns::S_MSG_PARAMS]["ttsName"][0]["text"] = "ABC"; // not mandatory
//   obj[strings_ns::S_MSG_PARAMS]["ttsName"][0]["type"] =
//       generated_ns::SpeechCapabilities::SC_TEXT;
  obj[strings_ns::S_MSG_PARAMS]["ngnMediaScreenAppName"] = "";
//   obj[strings_ns::S_MSG_PARAMS]["vrSynonyms"][0] = "Synonym1"; // not mandatory
//   obj[strings_ns::S_MSG_PARAMS]["vrSynonyms"][1] = "Synonym2";
  obj[strings_ns::S_MSG_PARAMS]["isMediaApplication"] = false;
  obj[strings_ns::S_MSG_PARAMS]["languageDesired"] = -1;
  obj[strings_ns::S_MSG_PARAMS]["hmiDisplayLanguageDesired"] = -1;      
//   obj[strings_ns::S_MSG_PARAMS]["appType"][0] = generated_ns::AppType::SYSTEM; // not mandatory
//   obj[strings_ns::S_MSG_PARAMS]["appType"][1] = generated_ns::AppType::MEDIA;
  obj[strings_ns::S_MSG_PARAMS]["appID"] = "";
}