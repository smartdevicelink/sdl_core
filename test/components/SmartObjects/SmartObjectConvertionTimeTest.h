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

#ifndef TEST_COMPONENTS_SMARTOBJECTS_SMARTOBJECTCONVERTIONTIMETEST_H_
#define TEST_COMPONENTS_SMARTOBJECTS_SMARTOBJECTCONVERTIONTIMETEST_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "utils/shared_ptr.h"

#include "SmartObjects/CSmartObject.hpp"
#include "SmartObjects/CSmartSchema.hpp"
#include "SmartObjects/ISchemaItem.hpp"
#include "formatters/CFormatterJsonSDLRPCv2.hpp"
#include "formatters/CFormatterJsonSDLRPCv1.hpp"
#include "SmartObjects/CArraySchemaItem.hpp"
#include "SmartObjects/CBoolSchemaItem.hpp"
#include "SmartObjects/CObjectSchemaItem.hpp"
#include "SmartObjects/CStringSchemaItem.hpp"
#include "SmartObjects/TEnumSchemaItem.hpp"
#include "SmartObjects/TNumberSchemaItem.hpp"
#include "SmartObjects/TSchemaItemParameter.hpp"


#include <string>
#include <time.h>



namespace test {
namespace components {
namespace SmartObjects {
namespace SmartObjectConvertionTimeTest {

using namespace NsSmartDeviceLink::NsJSONHandler::strings;

namespace TestType {
enum eType {
  INVALID_ENUM = -1,
  APPLICATION_NOT_REGISTERED = 0,
  SUCCESS,
  TOO_MANY_PENDING_REQUESTS,
  REJECTED,
  INVALID_DATA,
  OUT_OF_MEMORY,
  ABORTED,
  USER_DISALLOWED,
  GENERIC_ERROR,
  DISALLOWED
};
}

namespace FunctionIdTest {
enum eType {
  INVALID_ENUM = -1,
  RegisterAppInterface,
  UnregisterAppInterface,
  SetGlobalProperties,
};
}

namespace MessageTypeTest {
enum eType {
  INVALID_ENUM = -1,
  request,
  response,
  notification
};
}

class SmartObjectConvertionTimeTest : public ::testing::Test {
  protected:

    double getConvertionTimeToJsonV2Format(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& srcObj
                                           , std::string& jsonString) {
      timespec convertionStartTime;
      timespec convertionEndTime;
      double convertionTime;

      clock_gettime(CLOCK_REALTIME, &convertionStartTime);

      // SmartObjects --> JSON
      NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::toString(srcObj, jsonString);

      clock_gettime(CLOCK_REALTIME, &convertionEndTime);

      convertionTime = static_cast<double>(convertionEndTime.tv_sec - convertionStartTime.tv_sec)
                       + static_cast<double>(convertionEndTime.tv_nsec - convertionStartTime.tv_nsec) * 1e-9;

      return convertionTime;
    }

    double getConvertionTimeFromJsonV2Format(std::string& jsonString
        , NsSmartDeviceLink::NsSmartObjects::CSmartObject& dstObj) {
      timespec convertionStartTime;
      timespec convertionEndTime;
      double convertionTime;

      clock_gettime(CLOCK_REALTIME, &convertionStartTime);

      // JSON --> SmartObjects
      NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::
      fromString<FunctionIdTest::eType, MessageTypeTest::eType>(jsonString, dstObj, FunctionIdTest::RegisterAppInterface, MessageTypeTest::request, 13);

      clock_gettime(CLOCK_REALTIME, &convertionEndTime);

      convertionTime = static_cast<double>(convertionEndTime.tv_sec - convertionStartTime.tv_sec)
                       + static_cast<double>(convertionEndTime.tv_nsec - convertionStartTime.tv_nsec) * 1e-9;

      return convertionTime;
    }

    double getConvertionTimeToJsonV1Format(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& srcObj
                                           , std::string& jsonString) {
      timespec convertionStartTime;
      timespec convertionEndTime;
      double convertionTime;

      clock_gettime(CLOCK_REALTIME, &convertionStartTime);

      // SmartObjects --> JSON
      NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv1::toString(srcObj, jsonString);

      clock_gettime(CLOCK_REALTIME, &convertionEndTime);

      convertionTime = static_cast<double>(convertionEndTime.tv_sec - convertionStartTime.tv_sec)
                       + static_cast<double>(convertionEndTime.tv_nsec - convertionStartTime.tv_nsec) * 1e-9;

      return convertionTime;
    }

    double getConvertionTimeFromJsonV1Format(std::string& jsonString
        , NsSmartDeviceLink::NsSmartObjects::CSmartObject& dstObj) {
      timespec convertionStartTime;
      timespec convertionEndTime;
      double convertionTime;

      clock_gettime(CLOCK_REALTIME, &convertionStartTime);

      // JSON --> SmartObjects
      NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv1::fromString<FunctionIdTest::eType, MessageTypeTest::eType>(jsonString, dstObj);

      clock_gettime(CLOCK_REALTIME, &convertionEndTime);

      convertionTime = static_cast<double>(convertionEndTime.tv_sec - convertionStartTime.tv_sec)
                       + static_cast<double>(convertionEndTime.tv_nsec - convertionStartTime.tv_nsec) * 1e-9;

      return convertionTime;
    }

    void calculateConvertionTime(NsSmartDeviceLink::NsSmartObjects::CSmartObject& srcObj
                                 , NsSmartDeviceLink::NsSmartObjects::CSmartObject& dstObj) {
      std::string jsonString;
      double convertionToTime = 0.0;
      double convertionFromTime = 0.0;
      int cycles = 10;

      for (int i = 0; i < cycles; i++) {
        convertionToTime += getConvertionTimeToJsonV1Format(srcObj, jsonString);
        convertionFromTime += getConvertionTimeFromJsonV1Format(jsonString, dstObj);
      }
      printf("\nFormat V1. Convertion TO time = %.8f, Convertion FROM time = %.8f\n"
             , (convertionToTime / (double)cycles)
             , (convertionFromTime / (double)cycles));

      srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;       // adjust protocol version

      dstObj.getSchema().applySchema(dstObj);

      EXPECT_TRUE(srcObj == dstObj) << "The objects are different after V1 conversion";

      convertionToTime = 0.0;
      convertionFromTime = 0.0;
      for (int i = 0; i < cycles; i++) {
        convertionToTime += getConvertionTimeToJsonV2Format(srcObj, jsonString);
        convertionFromTime += getConvertionTimeFromJsonV2Format(jsonString, dstObj);
      }
      printf("Format V2. Convertion TO time = %.8f, Convertion FROM time = %.8f\n\n"
             , (convertionToTime / (double)cycles)
             , (convertionFromTime / (double)cycles));

      srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;       // adjust protocol version

      dstObj.getSchema().applySchema(dstObj);

      EXPECT_TRUE(srcObj == dstObj) << "The objects are different after V2 conversion";
    }

    void calculateConvertionTimeWithJsonStringOutput(const NsSmartDeviceLink::NsSmartObjects::CSmartObject& srcObj
        , NsSmartDeviceLink::NsSmartObjects::CSmartObject& dstObj) {
      std::string jsonString;
      double convertionToTime;
      double convertionFromTime;

      convertionToTime = getConvertionTimeToJsonV1Format(srcObj, jsonString);
      convertionFromTime = getConvertionTimeFromJsonV1Format(jsonString, dstObj);
      printf("\nJSON string V1 = %s", jsonString.c_str());
      printf("\nFormat V1. Convertion TO time = %.8f, Convertion FROM time = %.8f\n"
             , (convertionToTime)
             , (convertionFromTime));

      convertionToTime = getConvertionTimeToJsonV2Format(srcObj, jsonString);
      convertionFromTime = getConvertionTimeFromJsonV2Format(jsonString, dstObj);
      printf("\nJSON string V2 = %s", jsonString.c_str());
      printf("\nFormat V2. Convertion TO time = %.8f, Convertion FROM time = %.8f\n"
             , (convertionToTime)
             , (convertionFromTime));
    }

    // The basic Schema just for enum conversion (FunctionId and MessageType)
    NsSmartDeviceLink::NsSmartObjects::CSmartSchema initBasicObjectSchema(void) {
      std::set<FunctionIdTest::eType> functionId_allowedEnumSubsetValues;
      functionId_allowedEnumSubsetValues.insert(FunctionIdTest::RegisterAppInterface);
      functionId_allowedEnumSubsetValues.insert(FunctionIdTest::UnregisterAppInterface);
      functionId_allowedEnumSubsetValues.insert(FunctionIdTest::SetGlobalProperties);

      std::set<MessageTypeTest::eType> messageType_allowedEnumSubsetValues;
      messageType_allowedEnumSubsetValues.insert(MessageTypeTest::request);
      messageType_allowedEnumSubsetValues.insert(MessageTypeTest::response);
      messageType_allowedEnumSubsetValues.insert(MessageTypeTest::notification);

      utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> functionId_SchemaItem =
        NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<FunctionIdTest::eType>::create(functionId_allowedEnumSubsetValues);

      utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> messageType_SchemaItem =
        NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<MessageTypeTest::eType>::create(messageType_allowedEnumSubsetValues);

      std::map<std::string, NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember> paramsMembersMap;
      paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember(functionId_SchemaItem, true);
      paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember(messageType_SchemaItem, true);

      std::map<std::string, NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember> rootMembersMap;
      rootMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember(NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::create(paramsMembersMap), true);

      return NsSmartDeviceLink::NsSmartObjects::CSmartSchema(NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::create(rootMembersMap));
    }

    //Create SmartObjectSchema for test object
    NsSmartDeviceLink::NsSmartObjects::CSmartSchema initObjectSchema(void) {
      std::set<TestType::eType> resultCode_allowedEnumSubsetValues;
      resultCode_allowedEnumSubsetValues.insert(TestType::APPLICATION_NOT_REGISTERED);
      resultCode_allowedEnumSubsetValues.insert(TestType::SUCCESS);
      resultCode_allowedEnumSubsetValues.insert(TestType::TOO_MANY_PENDING_REQUESTS);
      resultCode_allowedEnumSubsetValues.insert(TestType::REJECTED);
      resultCode_allowedEnumSubsetValues.insert(TestType::INVALID_DATA);
      resultCode_allowedEnumSubsetValues.insert(TestType::OUT_OF_MEMORY);
      resultCode_allowedEnumSubsetValues.insert(TestType::ABORTED);
      resultCode_allowedEnumSubsetValues.insert(TestType::USER_DISALLOWED);
      resultCode_allowedEnumSubsetValues.insert(TestType::GENERIC_ERROR);
      resultCode_allowedEnumSubsetValues.insert(TestType::DISALLOWED);

      std::set<FunctionIdTest::eType> functionId_allowedEnumSubsetValues;
      functionId_allowedEnumSubsetValues.insert(FunctionIdTest::RegisterAppInterface);
      functionId_allowedEnumSubsetValues.insert(FunctionIdTest::UnregisterAppInterface);
      functionId_allowedEnumSubsetValues.insert(FunctionIdTest::SetGlobalProperties);

      std::set<MessageTypeTest::eType> messageType_allowedEnumSubsetValues;
      messageType_allowedEnumSubsetValues.insert(MessageTypeTest::request);
      messageType_allowedEnumSubsetValues.insert(MessageTypeTest::response);
      messageType_allowedEnumSubsetValues.insert(MessageTypeTest::notification);

      utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> success_SchemaItem =
        NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem::create(NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<bool>());

      utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> resultCode_SchemaItem =
        NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<TestType::eType>::create(resultCode_allowedEnumSubsetValues
            , NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<TestType::eType>());

      utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> info_SchemaItem =
        NsSmartDeviceLink::NsSmartObjects::CStringSchemaItem::create(NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<size_t>(1000)
            , NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<std::string>());

      utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::ISchemaItem> tryAgainTime_SchemaItem =
        NsSmartDeviceLink::NsSmartObjects::TNumberSchemaItem<int>::create(NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<int>(0)
            , NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<int>(2000000000)
            , NsSmartDeviceLink::NsSmartObjects::TSchemaItemParameter<int>());

      std::map<std::string, NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember> schemaMembersMap;

      schemaMembersMap["success"] = NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember(success_SchemaItem, true);
      schemaMembersMap["resultCode"] = NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
      schemaMembersMap["info"] = NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember(info_SchemaItem, false);
      schemaMembersMap["tryAgainTime"] = NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember(tryAgainTime_SchemaItem, true);

      std::map<std::string, NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember> paramsMembersMap;
      paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember(NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<FunctionIdTest::eType>::create(functionId_allowedEnumSubsetValues), true);
      paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember(NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<MessageTypeTest::eType>::create(messageType_allowedEnumSubsetValues), true);
      paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember(NsSmartDeviceLink::NsSmartObjects::TNumberSchemaItem<int>::create(), true);
      paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] = NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember(NsSmartDeviceLink::NsSmartObjects::TNumberSchemaItem<int>::create(1, 2), true);
      paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember(NsSmartDeviceLink::NsSmartObjects::TNumberSchemaItem<int>::create(), true);

      std::map<std::string, NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember> rootMembersMap;
      rootMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember(NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::create(schemaMembersMap), true);
      rootMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::SMember(NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::create(paramsMembersMap), true);

      return NsSmartDeviceLink::NsSmartObjects::CSmartSchema(NsSmartDeviceLink::NsSmartObjects::CObjectSchemaItem::create(rootMembersMap));
    }
};

TEST_F(SmartObjectConvertionTimeTest, test_int_object_convertion) {
  NsSmartDeviceLink::NsSmartObjects::CSmartObject srcObj, dstObj;
  NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema = initBasicObjectSchema();

  srcObj.setSchema(schema);
  dstObj.setSchema(schema);

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  srcObj[S_MSG_PARAMS]["value"] = 5;

  printf("\n INT value.\n");
  calculateConvertionTime(srcObj, dstObj);
}

TEST_F(SmartObjectConvertionTimeTest, test_double_object_convertion) {
  NsSmartDeviceLink::NsSmartObjects::CSmartObject srcObj, dstObj;
  NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema = initBasicObjectSchema();

  srcObj.setSchema(schema);
  dstObj.setSchema(schema);

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  srcObj[S_MSG_PARAMS]["value1"] = 3.1415926;
  srcObj[S_MSG_PARAMS]["value2"] = 32.6;
  srcObj[S_MSG_PARAMS]["value3"] = 33.945;
  srcObj[S_MSG_PARAMS]["value4"] = -12.5487698;
  srcObj[S_MSG_PARAMS]["value5"] = 0.61287346;

  printf("\n Double value.\n");
  calculateConvertionTime(srcObj, dstObj);
}

TEST_F(SmartObjectConvertionTimeTest, test_some_object_convertion) {
  NsSmartDeviceLink::NsSmartObjects::CSmartObject srcObj, dstObj;
  NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema = initBasicObjectSchema();

  srcObj.setSchema(schema);
  dstObj.setSchema(schema);

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  srcObj[S_MSG_PARAMS]["appId"] = "APP ID";
  srcObj[S_MSG_PARAMS]["appName"] = "APP NAME";
  srcObj[S_MSG_PARAMS]["appType"][0] = "SYSTEM";
  srcObj[S_MSG_PARAMS]["appType"][1] = "COMMUNICATION";
  srcObj[S_MSG_PARAMS]["hmiDisplayLanguageDesired"] = "RU-RU";
  srcObj[S_MSG_PARAMS]["isMediaApplication"] = true;
  srcObj[S_MSG_PARAMS]["languageDesired"] = "EN-US";
  srcObj[S_MSG_PARAMS]["ngnMediaScreenAppName"] = "SCREEN NAME";
  srcObj[S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"] = 2;
  srcObj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"] = 10;
  srcObj[S_MSG_PARAMS]["ttsName"][0]["text"] = "ABC";
  srcObj[S_MSG_PARAMS]["ttsName"][0]["type"] = "TEXT";
  srcObj[S_MSG_PARAMS]["vrSynonyms"][0] = "Synonym1";
  srcObj[S_MSG_PARAMS]["vrSynonyms"][1] = "Synonym2";
  srcObj[S_MSG_PARAMS]["null"] = NsSmartDeviceLink::NsSmartObjects::CSmartObject();
  srcObj[S_MSG_PARAMS]["double"] = -0.1234;

  printf("\n Random object.\n");
  calculateConvertionTime(srcObj, dstObj);
}

TEST_F(SmartObjectConvertionTimeTest, test_map_object_convertion) {
  NsSmartDeviceLink::NsSmartObjects::CSmartObject srcObj, dstObj, mapObj, innerObj;
  NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema = initBasicObjectSchema();

  srcObj.setSchema(schema);
  dstObj.setSchema(schema);

  // First iteration
  mapObj["request"]["name"] = "My Request";
  mapObj["request"]["id"] = 123;
  mapObj["response"]["name"] = "My Response";
  mapObj["response"]["id"] = 456;
  mapObj["we"]["need"]["to"]["go"]["deeper"] = true;

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  srcObj[S_MSG_PARAMS]["value"] = mapObj;

  printf("\n MAP object.\n");
  calculateConvertionTime(srcObj, dstObj);

  // Second iteration
  innerObj = mapObj;
  mapObj["request"]["value"] = innerObj;
  mapObj["response"]["value"] = innerObj;
  mapObj["we"]["need"]["to"]["go"]["deeper"]["value"] = innerObj;
  srcObj[S_MSG_PARAMS]["value"] = mapObj;

  printf("\n Complex MAP object.\n");
  calculateConvertionTime(srcObj, dstObj);

  // Third iteration
  innerObj = mapObj;
  mapObj["request"]["value"] = innerObj;
  mapObj["response"]["value"] = innerObj;
  mapObj["we"]["need"]["to"]["go"]["deeper"]["value"] = innerObj;
  srcObj[S_MSG_PARAMS]["value"] = mapObj;

  printf("\n Very Complex MAP object.\n");
  calculateConvertionTime(srcObj, dstObj);

  // Last iteration
  innerObj = mapObj;
  mapObj["request"]["value"] = innerObj;
  mapObj["response"]["value"] = innerObj;
  mapObj["we"]["need"]["to"]["go"]["deeper"]["value"] = innerObj;
  srcObj[S_MSG_PARAMS]["value"] = mapObj;

  printf("\n Very Very Complex MAP object.\n");
  calculateConvertionTime(srcObj, dstObj);
}

TEST_F(SmartObjectConvertionTimeTest, test_array_convertion) {
  NsSmartDeviceLink::NsSmartObjects::CSmartObject srcObj, dstObj, arrayObj, innerObj;
  NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema = initBasicObjectSchema();
  int arraySize = 10;

  srcObj.setSchema(schema);
  dstObj.setSchema(schema);

  // First iteration
  for (int i = 0; i < arraySize; i++) {
    arrayObj[i] = rand();
  }

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  srcObj[S_MSG_PARAMS]["array"] = arrayObj;

  printf("\n Array object [%d].\n", arraySize);
  calculateConvertionTime(srcObj, dstObj);

  // Second iteration
  printf("\n Array object [%d x %d].\n", arraySize, arraySize);
  innerObj = arrayObj;
  for (int i = 0; i < arraySize; i++) {
    arrayObj[i] = innerObj;
  }

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  srcObj[S_MSG_PARAMS]["array"] = arrayObj;

  calculateConvertionTime(srcObj, dstObj);

  // Third iteration
  printf("\n Array object [%d x %d x %d].\n"
         , arraySize, arraySize, arraySize);
  innerObj = arrayObj;
  for (int i = 0; i < arraySize; i++) {
    arrayObj[i] = innerObj;
  }

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  srcObj[S_MSG_PARAMS]["array"] = arrayObj;

  calculateConvertionTime(srcObj, dstObj);

  // Fourth iteration
  printf("\n Array object [%d x %d x %d x %d].\n"
         , arraySize, arraySize, arraySize, arraySize);
  innerObj = arrayObj;
  for (int i = 0; i < arraySize; i++) {
    arrayObj[i] = innerObj;
  }

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  srcObj[S_MSG_PARAMS]["array"] = arrayObj;

  calculateConvertionTime(srcObj, dstObj);

  // Last iteration
  printf("\n Array object [%d x %d x %d x %d x %d].\n"
         , arraySize, arraySize, arraySize, arraySize, arraySize);
  innerObj = arrayObj;
  for (int i = 0; i < arraySize; i++) {
    arrayObj[i] = innerObj;
  }

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  srcObj[S_MSG_PARAMS]["array"] = arrayObj;

  calculateConvertionTime(srcObj, dstObj);
}


TEST_F(SmartObjectConvertionTimeTest, test_object_with_enum_convertion) {
  NsSmartDeviceLink::NsSmartObjects::CSmartObject srcObj, dstObj;
  NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema = initObjectSchema();

  srcObj.setSchema(schema);
  dstObj.setSchema(schema);

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  srcObj[S_MSG_PARAMS]["success"] = true;
  srcObj[S_MSG_PARAMS]["resultCode"] = 2;
  srcObj[S_MSG_PARAMS]["info"] = "Some string";
  srcObj[S_MSG_PARAMS]["tryAgainTime"] = 322;
  srcObj.setSchema(schema);

  printf("\n Object with enum.\n");
  calculateConvertionTime(srcObj, dstObj);
}

TEST_F(SmartObjectConvertionTimeTest, test_object_without_enum_convertion) {
  NsSmartDeviceLink::NsSmartObjects::CSmartObject srcObj, dstObj;
  NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema = initBasicObjectSchema();

  srcObj.setSchema(schema);
  dstObj.setSchema(schema);

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = MessageTypeTest::request;
  srcObj[S_PARAMS][S_FUNCTION_ID] = FunctionIdTest::RegisterAppInterface;
  srcObj[S_PARAMS][S_CORRELATION_ID] = 13;
  srcObj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  srcObj[S_MSG_PARAMS]["success"] = true;
  srcObj[S_MSG_PARAMS]["resultCode"] = 2;
  srcObj[S_MSG_PARAMS]["info"] = "Some string";
  srcObj[S_MSG_PARAMS]["tryAgainTime"] = 322;

  printf("\n Object without enum.\n");
  calculateConvertionTime(srcObj, dstObj);
}

}
}
}
}

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
template<>
const std::map < test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType
, std::string > & TEnumSchemaItem<test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType>::getEnumElementsStringRepresentation(void) {
  static bool isInitialized = false;
  static std::map < test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType
  , std::string > enumStringRepresentationMap;

  if (false == isInitialized) {
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType::APPLICATION_NOT_REGISTERED, "APPLICATION_NOT_REGISTERED"));
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType::SUCCESS, "SUCCESS"));
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType::TOO_MANY_PENDING_REQUESTS, "TOO_MANY_PENDING_REQUESTS"));
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType::REJECTED, "REJECTED"));
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType::INVALID_DATA, "INVALID_DATA"));
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType::OUT_OF_MEMORY, "OUT_OF_MEMORY"));
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType::ABORTED, "ABORTED"));
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType::USER_DISALLOWED, "USER_DISALLOWED"));
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType::GENERIC_ERROR, "GENERIC_ERROR"));
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType::DISALLOWED, "DISALLOWED"));

    isInitialized = true;
  }
  return enumStringRepresentationMap;
}

template <>
const std::map<test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::eType, std::string>&
NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::eType>::getEnumElementsStringRepresentation(void) {
  static bool isInitialized = false;
  static std::map<test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::eType, std::string> enumStringRepresentationMap;

  if (false == isInitialized) {
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::RegisterAppInterface, "RegisterAppInterface"));
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::UnregisterAppInterface, "UnregisterAppInterface"));
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::SetGlobalProperties, "SetGlobalProperties"));

    isInitialized = true;
  }

  return enumStringRepresentationMap;
}

template <>
const std::map<test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::eType, std::string>&
NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::eType>::getEnumElementsStringRepresentation(void) {
  static bool isInitialized = false;
  static std::map<test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::eType, std::string> enumStringRepresentationMap;

  if (false == isInitialized) {
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::request, "request"));
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::response, "response"));
    enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::notification, "notification"));

    isInitialized = true;
  }

  return enumStringRepresentationMap;
}
}
}

#endif  // TEST_COMPONENTS_SMARTOBJECTS_SMARTOBJECTCONVERTIONTIMETEST_H_
