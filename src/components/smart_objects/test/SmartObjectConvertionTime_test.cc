//TODO
//this test file should be refactored. Now it doesn't test anything.
//Previously it tests equality of objects but should test time for object's conversion
/*
 * Copyright (c) 2014, Ford Motor Company
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

#include "gmock/gmock.h"

#include "utils/shared_ptr.h"

#include "smart_objects/smart_object.h"
#include "smart_objects/smart_schema.h"
#include "smart_objects/schema_item.h"
#include "formatters/CFormatterJsonSDLRPCv2.hpp"
#include "formatters/CFormatterJsonSDLRPCv1.hpp"
#include "smart_objects/array_schema_item.h"
#include "smart_objects/bool_schema_item.h"
#include "smart_objects/object_schema_item.h"
#include "smart_objects/string_schema_item.h"
#include "smart_objects/enum_schema_item.h"
#include "smart_objects/number_schema_item.h"
#include "smart_objects/schema_item_parameter.h"

#include <string>
#include <time.h>

namespace test {
namespace components {
namespace SmartObjects {
namespace SmartObjectConvertionTimeTest {

using namespace NsSmartDeviceLink::NsJSONHandler::strings;
using namespace NsSmartDeviceLink::NsSmartObjects;

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

  double getConvertionTimeToJsonV2Format(const SmartObject& srcObj,
                                         std::string& jsonString) {
    timespec convertionStartTime, convertionEndTime;
    clock_gettime(CLOCK_REALTIME, &convertionStartTime);

    // SmartObjects --> JSON
    NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::toString(
        srcObj, jsonString);

    clock_gettime(CLOCK_REALTIME, &convertionEndTime);

    return static_cast<double>(convertionEndTime.tv_sec
        - convertionStartTime.tv_sec)
        + static_cast<double>(convertionEndTime.tv_nsec
            - convertionStartTime.tv_nsec) * 1e-9;
  }

  double getConvertionTimeFromJsonV2Format(std::string& jsonString,
                                           SmartObject& dstObj) {
    timespec convertionStartTime, convertionEndTime;
    clock_gettime(CLOCK_REALTIME, &convertionStartTime);

    // JSON --> SmartObjects
    NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv2::fromString<
        FunctionIdTest::eType, MessageTypeTest::eType>(
        jsonString, dstObj, FunctionIdTest::RegisterAppInterface,
        MessageTypeTest::request, 13);

    clock_gettime(CLOCK_REALTIME, &convertionEndTime);
    return static_cast<double>(convertionEndTime.tv_sec
        - convertionStartTime.tv_sec)
        + static_cast<double>(convertionEndTime.tv_nsec
            - convertionStartTime.tv_nsec) * 1e-9;
  }

  double getConvertionTimeToJsonV1Format(const SmartObject& srcObj,
                                         std::string& jsonString) {
    timespec convertionStartTime, convertionEndTime;
    clock_gettime(CLOCK_REALTIME, &convertionStartTime);

    // SmartObjects --> JSON
    NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv1::toString(
        srcObj, jsonString);

    clock_gettime(CLOCK_REALTIME, &convertionEndTime);
    return static_cast<double>(convertionEndTime.tv_sec
        - convertionStartTime.tv_sec)
        + static_cast<double>(convertionEndTime.tv_nsec
            - convertionStartTime.tv_nsec) * 1e-9;
  }

  double getConvertionTimeFromJsonV1Format(std::string& jsonString,
                                           SmartObject& dstObj) {
    timespec convertionStartTime, convertionEndTime;
    clock_gettime(CLOCK_REALTIME, &convertionStartTime);

    // JSON --> SmartObjects
    NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonSDLRPCv1::fromString<
        FunctionIdTest::eType, MessageTypeTest::eType>(jsonString, dstObj);

    clock_gettime(CLOCK_REALTIME, &convertionEndTime);
    return static_cast<double>(convertionEndTime.tv_sec
        - convertionStartTime.tv_sec)
        + static_cast<double>(convertionEndTime.tv_nsec
            - convertionStartTime.tv_nsec) * 1e-9;
  }

  void calculateConvertionTime(SmartObject& srcObj, SmartObject& dstObj) {
    std::string jsonString;
    double convertionToTime = 0.0;
    double convertionFromTime = 0.0;
    const int cycles = 1;

    for (int i = 0; i < cycles; i++) {
      convertionToTime += getConvertionTimeToJsonV1Format(srcObj, jsonString);
//        printf("%s\n", jsonString.c_str());
      convertionFromTime += getConvertionTimeFromJsonV1Format(jsonString,
                                                              dstObj);
    }
    printf(
        "Format V1. Convertion TO time = %.8f, Convertion FROM time = %.8f\n",
        convertionToTime / cycles, convertionFromTime / cycles);

    srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 1;       // adjust protocol version

    srcObj.getSchema().applySchema(dstObj, false);
    // The objects are different after remove non-schemed fields
//      EXPECT_TRUE(srcObj == dstObj);

    convertionToTime = 0.0;
    convertionFromTime = 0.0;
    for (int i = 0; i < cycles; i++) {
      convertionToTime += getConvertionTimeToJsonV2Format(srcObj, jsonString);
      convertionFromTime += getConvertionTimeFromJsonV2Format(jsonString,
                                                              dstObj);
    }
    printf(
        "Format V2. Convertion TO time = %.8f, Convertion FROM time = %.8f\n",
        convertionToTime / cycles, convertionFromTime / cycles);

    srcObj[S_PARAMS][S_PROTOCOL_VERSION] = 2;       // adjust protocol version

    dstObj.getSchema().applySchema(dstObj, false);
    // The objects are different after remove non-schemed fields
//      EXPECT_TRUE(srcObj == dstObj);
  }

  void calculateConvertionTimeWithJsonStringOutput(const SmartObject& srcObj,
                                                   SmartObject& dstObj) {
    std::string jsonString;
    double convertionToTime = getConvertionTimeToJsonV1Format(srcObj,
                                                              jsonString);
    double convertionFromTime = getConvertionTimeFromJsonV1Format(jsonString,
                                                                  dstObj);
    printf("\nJSON string V1 = %s", jsonString.c_str());
    printf(
        "\nFormat V1. Convertion TO time = %.8f, Convertion FROM time = %.8f\n",
        convertionToTime, convertionFromTime);

    convertionToTime = getConvertionTimeToJsonV2Format(srcObj, jsonString);
    convertionFromTime = getConvertionTimeFromJsonV2Format(jsonString, dstObj);
    printf("\nJSON string V2 = %s", jsonString.c_str());
    printf(
        "\nFormat V2. Convertion TO time = %.8f, Convertion FROM time = %.8f\n",
        convertionToTime, convertionFromTime);
  }

  // The basic Schema just for enum conversion (FunctionId and MessageType)
  CSmartSchema initBasicObjectSchema() {
    std::set<FunctionIdTest::eType> functionId_allowedEnumSubsetValues;
    functionId_allowedEnumSubsetValues.insert(
        FunctionIdTest::RegisterAppInterface);
    functionId_allowedEnumSubsetValues.insert(
        FunctionIdTest::UnregisterAppInterface);
    functionId_allowedEnumSubsetValues.insert(
        FunctionIdTest::SetGlobalProperties);

    std::set<MessageTypeTest::eType> messageType_allowedEnumSubsetValues;
    messageType_allowedEnumSubsetValues.insert(MessageTypeTest::request);
    messageType_allowedEnumSubsetValues.insert(MessageTypeTest::response);
    messageType_allowedEnumSubsetValues.insert(MessageTypeTest::notification);

    ISchemaItemPtr functionId_SchemaItem =
        TEnumSchemaItem<FunctionIdTest::eType>::create(
            functionId_allowedEnumSubsetValues);

    ISchemaItemPtr messageType_SchemaItem = TEnumSchemaItem<
        MessageTypeTest::eType>::create(messageType_allowedEnumSubsetValues);

    CObjectSchemaItem::Members paramsMembersMap;
    paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] =
        CObjectSchemaItem::SMember(functionId_SchemaItem, true);
    paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] =
        CObjectSchemaItem::SMember(messageType_SchemaItem, true);
    paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] =
        CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

    std::map<std::string, CObjectSchemaItem::SMember> rootMembersMap;
    rootMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] =
        CObjectSchemaItem::SMember(CObjectSchemaItem::create(paramsMembersMap),
                                   true);

    return CSmartSchema(CObjectSchemaItem::create(rootMembersMap));
  }

  //Create SmartObjectSchema for test object
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

    std::set<FunctionIdTest::eType> functionId_allowedEnumSubsetValues;
    functionId_allowedEnumSubsetValues.insert(
        FunctionIdTest::RegisterAppInterface);
    functionId_allowedEnumSubsetValues.insert(
        FunctionIdTest::UnregisterAppInterface);
    functionId_allowedEnumSubsetValues.insert(
        FunctionIdTest::SetGlobalProperties);

    std::set<MessageTypeTest::eType> messageType_allowedEnumSubsetValues;
    messageType_allowedEnumSubsetValues.insert(MessageTypeTest::request);
    messageType_allowedEnumSubsetValues.insert(MessageTypeTest::response);
    messageType_allowedEnumSubsetValues.insert(MessageTypeTest::notification);

    ISchemaItemPtr success_SchemaItem = CBoolSchemaItem::create(
        TSchemaItemParameter<bool>());

    ISchemaItemPtr resultCode_SchemaItem =
        TEnumSchemaItem<TestType::eType>::create(
            resultCode_allowedEnumSubsetValues,
            TSchemaItemParameter<TestType::eType>());

    ISchemaItemPtr info_SchemaItem = CStringSchemaItem::create(
        TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(1000),
        TSchemaItemParameter<std::string>());

    ISchemaItemPtr tryAgainTime_SchemaItem = TNumberSchemaItem<int>::create(
        TSchemaItemParameter<int>(0), TSchemaItemParameter<int>(2000000000),
        TSchemaItemParameter<int>());

    std::map<std::string, CObjectSchemaItem::SMember> schemaMembersMap;

    schemaMembersMap["success"] = CObjectSchemaItem::SMember(success_SchemaItem,
                                                             true);
    schemaMembersMap["resultCode"] = CObjectSchemaItem::SMember(
        resultCode_SchemaItem, true);
    schemaMembersMap["info"] = CObjectSchemaItem::SMember(info_SchemaItem,
                                                          false);
    schemaMembersMap["tryAgainTime"] = CObjectSchemaItem::SMember(
        tryAgainTime_SchemaItem, true);

    std::map<std::string, CObjectSchemaItem::SMember> paramsMembersMap;
    paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] =
        CObjectSchemaItem::SMember(
            TEnumSchemaItem<FunctionIdTest::eType>::create(
                functionId_allowedEnumSubsetValues),
            true);
    paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] =
        CObjectSchemaItem::SMember(
            TEnumSchemaItem<MessageTypeTest::eType>::create(
                messageType_allowedEnumSubsetValues),
            true);
    paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] =
        CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
    paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] =
        CObjectSchemaItem::SMember(
            TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1),
                                           TSchemaItemParameter<int>(2)),
            true);
    paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] =
        CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

    std::map<std::string, CObjectSchemaItem::SMember> rootMembersMap;
    rootMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] =
        CObjectSchemaItem::SMember(CObjectSchemaItem::create(schemaMembersMap),
                                   true);
    rootMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] =
        CObjectSchemaItem::SMember(CObjectSchemaItem::create(paramsMembersMap),
                                   true);
    return CSmartSchema(CObjectSchemaItem::create(rootMembersMap));
  }
};

TEST_F(SmartObjectConvertionTimeTest, test_int_object_convertion) {
  SmartObject srcObj, dstObj;
  CSmartSchema schema = initObjectSchema();

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
  SmartObject srcObj, dstObj;
  CSmartSchema schema = initObjectSchema();

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
  SmartObject srcObj, dstObj;
  CSmartSchema schema = initObjectSchema();

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
  srcObj[S_MSG_PARAMS]["null"] = SmartObject();
  srcObj[S_MSG_PARAMS]["double"] = -0.1234;

  printf("\n Random object.\n");
  calculateConvertionTime(srcObj, dstObj);
}

TEST_F(SmartObjectConvertionTimeTest, test_map_object_convertion) {
  SmartObject srcObj, dstObj, mapObj, innerObj;
  CSmartSchema schema = initObjectSchema();

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
  SmartObject srcObj, dstObj, arrayObj, innerObj;
  CSmartSchema schema = initObjectSchema();
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
  printf("\n Array object [%d x %d x %d].\n", arraySize, arraySize, arraySize);
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
  printf("\n Array object [%d x %d x %d x %d].\n", arraySize, arraySize,
         arraySize, arraySize);
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
  printf("\n Array object [%d x %d x %d x %d x %d].\n", arraySize, arraySize,
         arraySize, arraySize, arraySize);
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
  SmartObject srcObj, dstObj;
  CSmartSchema schema = initObjectSchema();

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
  SmartObject srcObj, dstObj;
  CSmartSchema schema = initObjectSchema();

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
const EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType>::EnumToCStringMap EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType>::enum_to_cstring_map_ =
    EnumConversionHelper<
        test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType>::InitEnumToCStringMap();

template<>
const EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType>::CStringToEnumMap EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType>::cstring_to_enum_map_ =
    EnumConversionHelper<
        test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType>::InitCStringToEnumMap();

template<>
const char* const EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType>::cstring_values_[] =
    { "APPLICATION_NOT_REGISTERED", "SUCCESS", "TOO_MANY_PENDING_REQUESTS",
        "REJECTED", "INVALID_DATA", "OUT_OF_MEMORY", "ABORTED",
        "USER_DISALLOWED", "GENERIC_ERROR", "DISALLOWED" };

template<>
const test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::eType>::enum_values_[] =
    {
        test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::APPLICATION_NOT_REGISTERED,
        test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::SUCCESS,
        test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::TOO_MANY_PENDING_REQUESTS,
        test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::REJECTED,
        test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::INVALID_DATA,
        test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::OUT_OF_MEMORY,
        test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::ABORTED,
        test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::USER_DISALLOWED,
        test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::GENERIC_ERROR,
        test::components::SmartObjects::SmartObjectConvertionTimeTest::TestType::DISALLOWED };

template<>
const EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::eType>::EnumToCStringMap EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::eType>::enum_to_cstring_map_ =
    EnumConversionHelper<
        test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::eType>::InitEnumToCStringMap();

template<>
const EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::eType>::CStringToEnumMap EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::eType>::cstring_to_enum_map_ =
    EnumConversionHelper<
        test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::eType>::InitCStringToEnumMap();

template<>
const char* const EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::eType>::cstring_values_[] =
    { "RegisterAppInterface", "UnregisterAppInterface", "SetGlobalProperties" };

template<>
const test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::eType EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::eType>::enum_values_[] =
    {
        test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::RegisterAppInterface,
        test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::UnregisterAppInterface,
        test::components::SmartObjects::SmartObjectConvertionTimeTest::FunctionIdTest::SetGlobalProperties };

template<>
const EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::eType>::EnumToCStringMap EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::eType>::enum_to_cstring_map_ =
    EnumConversionHelper<
        test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::eType>::InitEnumToCStringMap();

template<>
const EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::eType>::CStringToEnumMap EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::eType>::cstring_to_enum_map_ =
    EnumConversionHelper<
        test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::eType>::InitCStringToEnumMap();

template<>
const char* const EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::eType>::cstring_values_[] =
    { "request", "response", "notification" };

template<>
const test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::eType EnumConversionHelper<
    test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::eType>::enum_values_[] =
    {
        test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::request,
        test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::response,
        test::components::SmartObjects::SmartObjectConvertionTimeTest::MessageTypeTest::notification };

}
}
