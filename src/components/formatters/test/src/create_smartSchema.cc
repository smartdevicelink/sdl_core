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

#include "create_smartSchema.h"
namespace test {
namespace components {
namespace formatters {

using namespace NsSmartDeviceLink::NsJSONHandler::strings;
using namespace NsSmartDeviceLink::NsJSONHandler::Formatters;
using namespace NsSmartDeviceLink::NsSmartObjects;

template<>
const EnumConversionHelper<FunctionIDTest::eType>::EnumToCStringMap EnumConversionHelper<
    test::components::formatters::FunctionIDTest::eType>::enum_to_cstring_map_ =
    EnumConversionHelper<test::components::formatters::FunctionIDTest::eType>::InitEnumToCStringMap();

template<>
const EnumConversionHelper<FunctionIDTest::eType>::CStringToEnumMap EnumConversionHelper<
    test::components::formatters::FunctionIDTest::eType>::cstring_to_enum_map_ =
    EnumConversionHelper<test::components::formatters::FunctionIDTest::eType>::InitCStringToEnumMap();

template<>
const char* const EnumConversionHelper<FunctionIDTest::eType>::cstring_values_[] =
    { "RegisterAppInterface", "UnregisterAppInterface", "SetGlobalProperties" };

template<>
const FunctionIDTest::eType EnumConversionHelper<FunctionIDTest::eType>::enum_values_[] =
    { test::components::formatters::FunctionIDTest::RegisterAppInterface,
        test::components::formatters::FunctionIDTest::UnregisterAppInterface,
        test::components::formatters::FunctionIDTest::SetGlobalProperties };

template<>
const EnumConversionHelper<Language::eType>::EnumToCStringMap EnumConversionHelper<
    test::components::formatters::Language::eType>::enum_to_cstring_map_ =
    EnumConversionHelper<test::components::formatters::Language::eType>::InitEnumToCStringMap();

template<>
const EnumConversionHelper<Language::eType>::CStringToEnumMap EnumConversionHelper<
    test::components::formatters::Language::eType>::cstring_to_enum_map_ =
    EnumConversionHelper<test::components::formatters::Language::eType>::InitCStringToEnumMap();

template<>
const char* const EnumConversionHelper<Language::eType>::cstring_values_[] =
    { "EN_EU", "RU_RU"};

template<>
const Language::eType EnumConversionHelper<Language::eType>::enum_values_[] =
    { test::components::formatters::Language::EN_EU,
        test::components::formatters::Language::RU_RU};

template<>
const EnumConversionHelper<SpeechCapabilities::eType>::EnumToCStringMap EnumConversionHelper<
    test::components::formatters::SpeechCapabilities::eType>::enum_to_cstring_map_ =
    EnumConversionHelper<test::components::formatters::SpeechCapabilities::eType>::InitEnumToCStringMap();

template<>
const EnumConversionHelper<SpeechCapabilities::eType>::CStringToEnumMap EnumConversionHelper<
    test::components::formatters::SpeechCapabilities::eType>::cstring_to_enum_map_ =
    EnumConversionHelper<test::components::formatters::SpeechCapabilities::eType>::InitCStringToEnumMap();

template<>
const char* const EnumConversionHelper<SpeechCapabilities::eType>::cstring_values_[] =
    { "SC_TEXT"};

template<>
const SpeechCapabilities::eType EnumConversionHelper<SpeechCapabilities::eType>::enum_values_[] =
    { test::components::formatters::SpeechCapabilities::SC_TEXT};

template<>
const EnumConversionHelper<AppTypeTest::eType>::EnumToCStringMap EnumConversionHelper<
    test::components::formatters::AppTypeTest::eType>::enum_to_cstring_map_ =
    EnumConversionHelper<test::components::formatters::AppTypeTest::eType>::InitEnumToCStringMap();

template<>
const EnumConversionHelper<AppTypeTest::eType>::CStringToEnumMap EnumConversionHelper<
    test::components::formatters::AppTypeTest::eType>::cstring_to_enum_map_ =
    EnumConversionHelper<test::components::formatters::AppTypeTest::eType>::InitCStringToEnumMap();

template<>
const char* const EnumConversionHelper<AppTypeTest::eType>::cstring_values_[] =
    { "SYSTEM", "MEDIA"};

template<>
const AppTypeTest::eType EnumConversionHelper<AppTypeTest::eType>::enum_values_[] =
    { test::components::formatters::AppTypeTest::SYSTEM,
      test::components::formatters::AppTypeTest::MEDIA,
    };

CSmartSchema initObjectSchema() {
  std::set<TestType::eType> resultCode_allowedEnumSubsetValues;
  resultCode_allowedEnumSubsetValues.insert(
      TestType::APPLICATION_NOT_REGISTERED);
  resultCode_allowedEnumSubsetValues.insert(TestType::SUCCESS);
  resultCode_allowedEnumSubsetValues.insert(
      TestType::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowedEnumSubsetValues.insert(TestType::REJECTED);
  resultCode_allowedEnumSubsetValues.insert(TestType::INVALID_DATA);
  resultCode_allowedEnumSubsetValues.insert(TestType::OUT_OF_MEMORY);
  resultCode_allowedEnumSubsetValues.insert(TestType::ABORTED);
  resultCode_allowedEnumSubsetValues.insert(TestType::USER_DISALLOWED);
  resultCode_allowedEnumSubsetValues.insert(TestType::GENERIC_ERROR);
  resultCode_allowedEnumSubsetValues.insert(TestType::DISALLOWED);

  // Possible functions in this test scheme
  std::set<FunctionIDTest::eType> functionId_allowedEnumSubsetValues;
  functionId_allowedEnumSubsetValues.insert(
      FunctionIDTest::RegisterAppInterface);
  functionId_allowedEnumSubsetValues.insert(
      FunctionIDTest::UnregisterAppInterface);
  functionId_allowedEnumSubsetValues.insert(
      FunctionIDTest::SetGlobalProperties);

  // Possible message types
  std::set<MessageTypeTest::eType> messageType_allowedEnumSubsetValues;
  messageType_allowedEnumSubsetValues.insert(MessageTypeTest::request);
  messageType_allowedEnumSubsetValues.insert(MessageTypeTest::response);
  messageType_allowedEnumSubsetValues.insert(MessageTypeTest::notification);

  // Create result item
  ISchemaItemPtr success_SchemaItem = CBoolSchemaItem::create(
      TSchemaItemParameter<bool>());
  ISchemaItemPtr resultCode_SchemaItem =
      TEnumSchemaItem<TestType::eType>::create(
          resultCode_allowedEnumSubsetValues,
          TSchemaItemParameter<TestType::eType>());

  // Create info value with min 0 length and max 1000
  ISchemaItemPtr info_SchemaItem = CStringSchemaItem::create(
      TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000),
      TSchemaItemParameter<std::string>());

  ISchemaItemPtr tryAgainTime_SchemaItem = TNumberSchemaItem<int>::create(
      TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000),
      TSchemaItemParameter<int>());

  // Map of parameters
  std::map<std::string, CObjectSchemaItem::SMember> schemaMembersMap;

  schemaMembersMap["success"] = CObjectSchemaItem::SMember(success_SchemaItem,
                                                           false);
  schemaMembersMap["resultCode"] = CObjectSchemaItem::SMember(
      resultCode_SchemaItem, false);
  schemaMembersMap["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);
  schemaMembersMap["tryAgainTime"] = CObjectSchemaItem::SMember(
      tryAgainTime_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> paramsMembersMap;
  paramsMembersMap[S_FUNCTION_ID] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<FunctionIDTest::eType>::create(
          functionId_allowedEnumSubsetValues),
      true);
  paramsMembersMap[S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<MessageTypeTest::eType>::create(
          messageType_allowedEnumSubsetValues),
      true);
  paramsMembersMap[S_CORRELATION_ID] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  paramsMembersMap[S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1),
                                     TSchemaItemParameter<int>(2)),
      true);
  paramsMembersMap[S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> rootMembersMap;
  rootMembersMap[S_MSG_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(schemaMembersMap), true);
  rootMembersMap[S_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(paramsMembersMap), true);
  return CSmartSchema(CObjectSchemaItem::create(rootMembersMap));
};


CSmartSchema initSchemaForMetaFormatter() {
  std::set<TestType::eType> resultCode_allowedEnumSubsetValues;
  resultCode_allowedEnumSubsetValues.insert(
      TestType::APPLICATION_NOT_REGISTERED);
  resultCode_allowedEnumSubsetValues.insert(TestType::SUCCESS);
  resultCode_allowedEnumSubsetValues.insert(
      TestType::TOO_MANY_PENDING_REQUESTS);
  resultCode_allowedEnumSubsetValues.insert(TestType::REJECTED);
  resultCode_allowedEnumSubsetValues.insert(TestType::INVALID_DATA);
  resultCode_allowedEnumSubsetValues.insert(TestType::OUT_OF_MEMORY);
  resultCode_allowedEnumSubsetValues.insert(TestType::ABORTED);
  resultCode_allowedEnumSubsetValues.insert(TestType::USER_DISALLOWED);
  resultCode_allowedEnumSubsetValues.insert(TestType::GENERIC_ERROR);
  resultCode_allowedEnumSubsetValues.insert(TestType::DISALLOWED);

  // Possible functions in this test scheme
  std::set<FunctionIDTest::eType> functionId_allowedEnumSubsetValues;
  functionId_allowedEnumSubsetValues.insert(
      FunctionIDTest::RegisterAppInterface);
  functionId_allowedEnumSubsetValues.insert(
      FunctionIDTest::UnregisterAppInterface);
  functionId_allowedEnumSubsetValues.insert(
      FunctionIDTest::SetGlobalProperties);

  std::set<Language::eType> languageDesired_allowedEnumSubsetValues;
  languageDesired_allowedEnumSubsetValues.insert(Language::RU_RU);
  languageDesired_allowedEnumSubsetValues.insert(Language::EN_EU);


  std::set<AppTypeTest::eType> appType_allowedEnumSubsetValues;
  appType_allowedEnumSubsetValues.insert(AppTypeTest::SYSTEM);
  appType_allowedEnumSubsetValues.insert(AppTypeTest::MEDIA);

  std::set<SpeechCapabilities::eType> speechCapabilities_allowedEnumSubsetValues;
  speechCapabilities_allowedEnumSubsetValues.insert(SpeechCapabilities::SC_TEXT);

  // Possible message types
  std::set<MessageTypeTest::eType> messageType_allowedEnumSubsetValues;
  messageType_allowedEnumSubsetValues.insert(MessageTypeTest::request);
  messageType_allowedEnumSubsetValues.insert(MessageTypeTest::response);
  messageType_allowedEnumSubsetValues.insert(MessageTypeTest::notification);

  // Create param items
  ISchemaItemPtr appID_SchemaItem = CStringSchemaItem::create(
      TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000),
      TSchemaItemParameter<std::string>());
  ISchemaItemPtr appName_SchemaItem = CStringSchemaItem::create(
      TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000),
      TSchemaItemParameter<std::string>());
  ISchemaItemPtr isMediaApplication_SchemaItem = CBoolSchemaItem::create(
       TSchemaItemParameter<bool>());
  ISchemaItemPtr ngnMediaScreenAppName_SchemaItem = CStringSchemaItem::create(
       TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000),
       TSchemaItemParameter<std::string>());

  ISchemaItemPtr ttsNameItem_SchemaItem = CStringSchemaItem::create(
        TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000),
        TSchemaItemParameter<std::string>());

  ISchemaItemPtr ttstype_SchemaItem =
      TEnumSchemaItem<SpeechCapabilities::eType>::create(
          speechCapabilities_allowedEnumSubsetValues,
          TSchemaItemParameter<SpeechCapabilities::eType>());

  std::map<std::string, CObjectSchemaItem::SMember> ttsMap;
  ttsMap["text"]=CObjectSchemaItem::SMember(ttsNameItem_SchemaItem,
                                                                false);
  ttsMap["type"]=CObjectSchemaItem::SMember(ttstype_SchemaItem,
                                                                false);;

  ISchemaItemPtr hmiDisplayLanguageDesired_SchemaItem =
      TEnumSchemaItem<Language::eType>::create(
          languageDesired_allowedEnumSubsetValues,
          TSchemaItemParameter<Language::eType>());

  ISchemaItemPtr languageDesired_SchemaItem =
      TEnumSchemaItem<Language::eType>::create(
          languageDesired_allowedEnumSubsetValues,
          TSchemaItemParameter<Language::eType>());

  ISchemaItemPtr vrElementSchemaItem = CStringSchemaItem::create(
      TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000),
      TSchemaItemParameter<std::string>());

  ISchemaItemPtr appTypeElementSchemaItem =
      TEnumSchemaItem<AppTypeTest::eType>::create(
          appType_allowedEnumSubsetValues,
          TSchemaItemParameter<AppTypeTest::eType>());

  ISchemaItemPtr ttsElementSchemaItem = CObjectSchemaItem::create(ttsMap);

  ISchemaItemPtr ttsName_SchemaItem =
      CArraySchemaItem::create(ttsElementSchemaItem,
                               TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000));

  ISchemaItemPtr vrSynonyms_SchemaItem =
      CArraySchemaItem::create(vrElementSchemaItem,
                               TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000));

  ISchemaItemPtr appType_SchemaItem =
      CArraySchemaItem::create(appTypeElementSchemaItem,
                               TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000));


  ISchemaItemPtr majorVersion_SchemaItem = TNumberSchemaItem<int>::create();
  ISchemaItemPtr minorVersion_SchemaItem = TNumberSchemaItem<int>::create();

  ISchemaItemPtr syncMsg_SchemaItem =CStringSchemaItem::create(
      TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000),
      TSchemaItemParameter<std::string>());

  ISchemaItemPtr syncMsgVersion_SchemaItem =
      CArraySchemaItem::create(syncMsg_SchemaItem,
                               TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000));

  // Creation map for syncMsgVersion
  std::map<std::string, CObjectSchemaItem::SMember> schemaSyncMsgVersionMap;
  schemaSyncMsgVersionMap["majorVersion"]=CObjectSchemaItem::SMember(majorVersion_SchemaItem,
                                                              false);
  schemaSyncMsgVersionMap["minorVersion"]=CObjectSchemaItem::SMember(minorVersion_SchemaItem,
                                                              false);;

  // Map of parameters
  std::map<std::string, CObjectSchemaItem::SMember> schemaMembersMap;

  schemaMembersMap["appID"] = CObjectSchemaItem::SMember(appID_SchemaItem,
                                                           false);
  schemaMembersMap["appName"] = CObjectSchemaItem::SMember(appName_SchemaItem,
                                                            false);
  schemaMembersMap["appType"] = CObjectSchemaItem::SMember(appType_SchemaItem,
                                                           false);
  schemaMembersMap["hmiDisplayLanguageDesired"] = CObjectSchemaItem::SMember(hmiDisplayLanguageDesired_SchemaItem,
                                                            false);
  schemaMembersMap["isMediaApplication"] = CObjectSchemaItem::SMember(isMediaApplication_SchemaItem,
                                                            false);
  schemaMembersMap["languageDesired"] = CObjectSchemaItem::SMember(languageDesired_SchemaItem,
                                                            false);
  schemaMembersMap["ngnMediaScreenAppName"] = CObjectSchemaItem::SMember(ngnMediaScreenAppName_SchemaItem,
                                                            false);
  schemaMembersMap["syncMsgVersion"] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schemaSyncMsgVersionMap),
                                                            false);
  schemaMembersMap["ttsName"] = CObjectSchemaItem::SMember(ttsName_SchemaItem,
                                                            false);
  schemaMembersMap["vrSynonyms"] = CObjectSchemaItem::SMember(vrSynonyms_SchemaItem, false);

  std::map<std::string, CObjectSchemaItem::SMember> paramsMembersMap;
  paramsMembersMap[S_FUNCTION_ID] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<FunctionIDTest::eType>::create(
          functionId_allowedEnumSubsetValues),
      true);
  paramsMembersMap[S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(
      TEnumSchemaItem<MessageTypeTest::eType>::create(
          messageType_allowedEnumSubsetValues),
      true);
  paramsMembersMap[S_CORRELATION_ID] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);
  paramsMembersMap[S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1),
                                     TSchemaItemParameter<int>(2)),
      true);
  paramsMembersMap[S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(
      TNumberSchemaItem<int>::create(), true);

  std::map<std::string, CObjectSchemaItem::SMember> rootMembersMap;
  rootMembersMap[S_MSG_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(schemaMembersMap), true);
  rootMembersMap[S_PARAMS] = CObjectSchemaItem::SMember(
      CObjectSchemaItem::create(paramsMembersMap), true);
  return CSmartSchema(CObjectSchemaItem::create(rootMembersMap));
};



}  // namespace formatters
}  // namespace components
}  // namespace test
