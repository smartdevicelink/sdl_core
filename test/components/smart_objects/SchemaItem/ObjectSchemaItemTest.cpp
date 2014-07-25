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

#include "gtest/gtest.h"
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

namespace test {
namespace components {
namespace SmartObjects {
namespace SchemaItem {

using namespace NsSmartDeviceLink::NsSmartObjects;
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

class ObjectSchemaItemTest : public ::testing::Test {
  protected:

    //Create SmartObjectSchema for test object
    ISchemaItemPtr initObjectSchemaItem(void) {
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

      ISchemaItemPtr success_SchemaItem =
        CBoolSchemaItem::create(TSchemaItemParameter<bool>());

      ISchemaItemPtr resultCode_SchemaItem =
        TEnumSchemaItem<TestType::eType>::create(resultCode_allowedEnumSubsetValues
            , TSchemaItemParameter<TestType::eType>());

      ISchemaItemPtr info_SchemaItem =
        CStringSchemaItem::create(TSchemaItemParameter<size_t>(0)
                                  , TSchemaItemParameter<size_t>(10)
                                  , TSchemaItemParameter<std::string>());

      ISchemaItemPtr tryAgainTime_SchemaItem =
        TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0)
                                       , TSchemaItemParameter<int>(2000)
                                       , TSchemaItemParameter<int>());

      CObjectSchemaItem::Members schemaMembersMap;

      schemaMembersMap["success"] = CObjectSchemaItem::SMember(success_SchemaItem, true);
      schemaMembersMap["resultCode"] = CObjectSchemaItem::SMember(resultCode_SchemaItem, true);
      schemaMembersMap["info"] = CObjectSchemaItem::SMember(info_SchemaItem, false);
      schemaMembersMap["tryAgainTime"] = CObjectSchemaItem::SMember(tryAgainTime_SchemaItem, true);

      CObjectSchemaItem::Members paramsMembersMap;
      paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] = CObjectSchemaItem::SMember(TEnumSchemaItem<TestType::eType>::create(resultCode_allowedEnumSubsetValues), true);
      paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] = CObjectSchemaItem::SMember(TEnumSchemaItem<TestType::eType>::create(resultCode_allowedEnumSubsetValues), true);
      paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);
    paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] =
        CObjectSchemaItem::SMember(
            TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1),
                                           TSchemaItemParameter<int>(2)),
            true);
      paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] = CObjectSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);

      CObjectSchemaItem::Members rootMembersMap;
      rootMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(schemaMembersMap), true);
      rootMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] = CObjectSchemaItem::SMember(CObjectSchemaItem::create(paramsMembersMap), true);

      return CObjectSchemaItem::create(rootMembersMap);
    }
};

TEST_F(ObjectSchemaItemTest, test_too_few_object_params) {
  SmartObject obj;
  ISchemaItemPtr item = initObjectSchemaItem();

  obj[S_PARAMS][S_MESSAGE_TYPE] = "request";
  obj[S_PARAMS][S_FUNCTION_ID] = "some function";
  obj[S_PARAMS][S_CORRELATION_ID] = 13;
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  obj[S_MSG_PARAMS]["value"] = 5;

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::MISSING_MANDATORY_PARAMETER, resultType);
}

TEST_F(ObjectSchemaItemTest, test_too_many_object_params) {
  SmartObject srcObj;
  ISchemaItemPtr item = initObjectSchemaItem();

  srcObj[S_PARAMS][S_MESSAGE_TYPE] = 1;
  srcObj[S_PARAMS][S_FUNCTION_ID] = 3;
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
  srcObj[S_MSG_PARAMS]["null"] = NsSmartDeviceLink::NsSmartObjects::SmartObject();
  srcObj[S_MSG_PARAMS]["double"] = -0.1234;
  srcObj[S_MSG_PARAMS]["success"] = true;
  srcObj[S_MSG_PARAMS]["resultCode"] = 2;
  srcObj[S_MSG_PARAMS]["info"] = "String";
  srcObj[S_MSG_PARAMS]["tryAgainTime"] = 322;

  int resultType = item->validate(srcObj);
  EXPECT_EQ(Errors::UNEXPECTED_PARAMETER, resultType);
}


TEST_F(ObjectSchemaItemTest, test_object_with_correct_params) {
  SmartObject obj, dstObj;
  ISchemaItemPtr item = initObjectSchemaItem();

  obj[S_PARAMS][S_MESSAGE_TYPE] = "APPLICATION_NOT_REGISTERED";
  obj[S_PARAMS][S_FUNCTION_ID] = "GENERIC_ERROR";
  obj[S_PARAMS][S_CORRELATION_ID] = 13;
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  obj[S_MSG_PARAMS]["success"] = true;
  obj[S_MSG_PARAMS]["resultCode"] = 2;
  obj[S_MSG_PARAMS]["tryAgainTime"] = 322;

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  item->applySchema(obj);

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
  bool resDefault = item->setDefaultValue(obj);
  EXPECT_FALSE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  // Adding not mandatory param
  obj[S_MSG_PARAMS]["info"] = "Long String";
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  obj[S_MSG_PARAMS]["info"] = "String";
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  obj[S_MSG_PARAMS]["success"] = "OK";
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
}

TEST_F(ObjectSchemaItemTest, test_object_with_incorrect_params) {
  SmartObject obj;
  ISchemaItemPtr item = initObjectSchemaItem();

  obj[S_PARAMS][S_MESSAGE_TYPE] = "request";
  obj[S_PARAMS][S_FUNCTION_ID] = "some function";
  obj[S_PARAMS][S_CORRELATION_ID] = 13;
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  obj[S_MSG_PARAMS]["success"] = true;
  obj[S_MSG_PARAMS]["resultCode"] = 2;
  obj[S_MSG_PARAMS]["info"] = "Long string";
  obj[S_MSG_PARAMS]["tryAgainTime"] = 322;

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
}

}
}
}
}

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
template<>
const EnumConversionHelper<test::components::SmartObjects::SchemaItem::TestType::eType>::EnumToCStringMap
EnumConversionHelper<test::components::SmartObjects::SchemaItem::TestType::eType>::enum_to_cstring_map_ =
  EnumConversionHelper<test::components::SmartObjects::SchemaItem::TestType::eType>::InitEnumToCStringMap();

template<>
const EnumConversionHelper<test::components::SmartObjects::SchemaItem::TestType::eType>::CStringToEnumMap
EnumConversionHelper<test::components::SmartObjects::SchemaItem::TestType::eType>::cstring_to_enum_map_ =
  EnumConversionHelper<test::components::SmartObjects::SchemaItem::TestType::eType>::InitCStringToEnumMap();

template<>
const char* const
EnumConversionHelper<test::components::SmartObjects::SchemaItem::TestType::eType>::cstring_values_[] = {
    "APPLICATION_NOT_REGISTERED",
    "SUCCESS",
    "TOO_MANY_PENDING_REQUESTS",
    "REJECTED",
    "INVALID_DATA",
    "OUT_OF_MEMORY",
    "ABORTED",
    "USER_DISALLOWED",
    "GENERIC_ERROR",
    "DISALLOWED"
};

template<>
const test::components::SmartObjects::SchemaItem::TestType::eType
EnumConversionHelper<test::components::SmartObjects::SchemaItem::TestType::eType>::enum_values_[] = {
    test::components::SmartObjects::SchemaItem::TestType::APPLICATION_NOT_REGISTERED,
    test::components::SmartObjects::SchemaItem::TestType::SUCCESS,
    test::components::SmartObjects::SchemaItem::TestType::TOO_MANY_PENDING_REQUESTS,
    test::components::SmartObjects::SchemaItem::TestType::REJECTED,
    test::components::SmartObjects::SchemaItem::TestType::INVALID_DATA,
    test::components::SmartObjects::SchemaItem::TestType::OUT_OF_MEMORY,
    test::components::SmartObjects::SchemaItem::TestType::ABORTED,
    test::components::SmartObjects::SchemaItem::TestType::USER_DISALLOWED,
    test::components::SmartObjects::SchemaItem::TestType::GENERIC_ERROR,
    test::components::SmartObjects::SchemaItem::TestType::DISALLOWED
};

}
}
