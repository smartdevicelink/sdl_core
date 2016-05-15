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

#include <string>

#include "gmock/gmock.h"
#include "smart_objects/smart_object.h"
#include "smart_objects/smart_schema.h"
#include "smart_objects/array_schema_item.h"
#include "smart_objects/bool_schema_item.h"
#include "smart_objects/string_schema_item.h"
#include "smart_objects/enum_schema_item.h"
#include "smart_objects/number_schema_item.h"
#include "smart_objects/schema_item_parameter.h"
#include "smart_objects/object_schema_item.h"
#include "formatters/generic_json_formatter.h"
#include "formatters/CSmartFactory.h"

namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace smartobj = NsSmartDeviceLink::NsSmartObjects;

using namespace NsSmartDeviceLink::NsSmartObjects;
using namespace NsSmartDeviceLink::NsJSONHandler::strings;

namespace test {
namespace components {
namespace SmartObjects {
namespace SchemaItem {

namespace FunctionID {
enum eType {
  INVALID_ENUM = -1,
  Function0 = 0,
  Function1,
  Function2,
  Function3,
  Function4,
  Function5,
  Function6
};
}  // namespace FunctionID

namespace ResultType {
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
}  // namespace ResultType

namespace Keys {
const char RESULT_CODE[] = "resultCode";
const char INFO[] = "info";
const char SUCCESS[] = "success";
}

class ObjectSchemaItemTest : public ::testing::Test {
 protected:
  ISchemaItemPtr schema_item;
  // Create SmartObjectSchema for test object
  void SetUp() {
    std::set<FunctionID::eType> function_values;
    function_values.insert(FunctionID::Function0);
    function_values.insert(FunctionID::Function1);
    function_values.insert(FunctionID::Function2);
    function_values.insert(FunctionID::Function3);
    function_values.insert(FunctionID::Function4);
    function_values.insert(FunctionID::Function5);
    function_values.insert(FunctionID::Function6);

    std::set<ResultType::eType> resultCode_values;
    resultCode_values.insert(ResultType::APPLICATION_NOT_REGISTERED);
    resultCode_values.insert(ResultType::SUCCESS);
    resultCode_values.insert(ResultType::TOO_MANY_PENDING_REQUESTS);
    resultCode_values.insert(ResultType::REJECTED);
    resultCode_values.insert(ResultType::INVALID_DATA);
    resultCode_values.insert(ResultType::OUT_OF_MEMORY);
    resultCode_values.insert(ResultType::ABORTED);
    resultCode_values.insert(ResultType::USER_DISALLOWED);
    resultCode_values.insert(ResultType::GENERIC_ERROR);
    resultCode_values.insert(ResultType::DISALLOWED);

    CObjectSchemaItem::Members paramsMembersMap;
    paramsMembersMap[S_FUNCTION_ID] = CObjectSchemaItem::SMember(
        TEnumSchemaItem<FunctionID::eType>::create(function_values), true);
    paramsMembersMap[S_CORRELATION_ID] = CObjectSchemaItem::SMember(
        TNumberSchemaItem<int>::create(), true);
    paramsMembersMap[S_PROTOCOL_VERSION] = CObjectSchemaItem::SMember(
        TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1),
                                       TSchemaItemParameter<int>(2)),
        true);

    CObjectSchemaItem::Members schemaMembersMap;
    schemaMembersMap[Keys::RESULT_CODE] = CObjectSchemaItem::SMember(
        TEnumSchemaItem<ResultType::eType>::create(resultCode_values), false);
    schemaMembersMap[Keys::INFO] = CObjectSchemaItem::SMember(
        CStringSchemaItem::create(TSchemaItemParameter<size_t>(0),
                                  TSchemaItemParameter<size_t>(10)),
        false);
    schemaMembersMap[Keys::SUCCESS] = CObjectSchemaItem::SMember(
        CBoolSchemaItem::create(), false);

    CObjectSchemaItem::Members rootMembersMap;
    rootMembersMap[S_PARAMS] = CObjectSchemaItem::SMember(
        CObjectSchemaItem::create(paramsMembersMap), true);
    rootMembersMap[S_MSG_PARAMS] = CObjectSchemaItem::SMember(
        CObjectSchemaItem::create(schemaMembersMap), true);

    schema_item = CObjectSchemaItem::create(rootMembersMap);
  }
};

TEST_F(ObjectSchemaItemTest, validation_correct) {
  SmartObject obj;
  obj[S_PARAMS][S_FUNCTION_ID] = 0;
  obj[S_PARAMS][S_CORRELATION_ID] = 0XFF0;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 0;
  obj[S_MSG_PARAMS][Keys::INFO] = "0123456789";
  obj[S_MSG_PARAMS][Keys::SUCCESS] = true;

  EXPECT_EQ(Errors::OK, schema_item->validate(obj));
}

TEST_F(ObjectSchemaItemTest, validation_correct_skip_not_mandatory) {
  SmartObject obj;
  obj[S_PARAMS][S_FUNCTION_ID] = 1;
  obj[S_PARAMS][S_CORRELATION_ID] = -0xFF1;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 1;
  // skip non-mandatory obj[S_MSG_PARAMS][Keys::INFO]
  obj[S_MSG_PARAMS][Keys::SUCCESS] = false;

  EXPECT_EQ(Errors::OK, schema_item->validate(obj));
}

TEST_F(ObjectSchemaItemTest, validation_invalid_param) {
  SmartObject obj;
  obj[S_PARAMS] = "some parameters";
  obj[S_MSG_PARAMS] = "some message parameters";

  EXPECT_EQ(Errors::INVALID_VALUE, schema_item->validate(obj));

  obj[S_PARAMS][S_FUNCTION_ID] = "some function";
  obj[S_PARAMS][S_CORRELATION_ID] = "some correlation id";
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 3;
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = "some result";
  obj[S_MSG_PARAMS][Keys::SUCCESS] = 0xABC;

  EXPECT_EQ(Errors::INVALID_VALUE, schema_item->validate(obj));

  obj[S_PARAMS][S_FUNCTION_ID] = 1;
  EXPECT_EQ(Errors::INVALID_VALUE, schema_item->validate(obj));

  obj[S_PARAMS][S_CORRELATION_ID] = -0xFF1;
  EXPECT_EQ(Errors::INVALID_VALUE, schema_item->validate(obj));

  obj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  EXPECT_EQ(Errors::INVALID_VALUE, schema_item->validate(obj));

  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 1;
  EXPECT_EQ(Errors::INVALID_VALUE, schema_item->validate(obj));

  obj[S_MSG_PARAMS][Keys::SUCCESS] = false;

  EXPECT_EQ(Errors::OK, schema_item->validate(obj));
}
TEST_F(ObjectSchemaItemTest, validation_invalid_not_mandatory_param) {
  SmartObject obj;
  obj[S_PARAMS][S_FUNCTION_ID] = 0;
  obj[S_PARAMS][S_CORRELATION_ID] = 0XFF0;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 0;
  obj[S_MSG_PARAMS][Keys::SUCCESS] = true;

  // invalid non-mandatory obj[S_MSG_PARAMS][Keys::INFO]
  obj[S_MSG_PARAMS][Keys::INFO] = 0x10;
  EXPECT_EQ(Errors::INVALID_VALUE, schema_item->validate(obj));

  // invalid non-mandatory obj[S_MSG_PARAMS][Keys::INFO]
  obj[S_MSG_PARAMS][Keys::INFO] = true;
  EXPECT_EQ(Errors::INVALID_VALUE, schema_item->validate(obj));

  // invalid non-mandatory obj[S_MSG_PARAMS][Keys::INFO]
  obj[S_MSG_PARAMS][Keys::INFO] = SmartObject();
  EXPECT_EQ(Errors::INVALID_VALUE, schema_item->validate(obj));

  obj[S_MSG_PARAMS][Keys::INFO] = "info";
  EXPECT_EQ(Errors::OK, schema_item->validate(obj));
}

TEST_F(ObjectSchemaItemTest, validation_missing_mandatory) {
  SmartObject obj;
  // missed obj[S_PARAMS][S_FUNCTION_ID]
  // missed obj[S_PARAMS][S_CORRELATION_ID]
  // missed obj[S_PARAMS][S_PROTOCOL_VERSION]
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 2;
  obj[S_MSG_PARAMS][Keys::INFO] = "123";
  obj[S_MSG_PARAMS][Keys::SUCCESS] = false;

  EXPECT_EQ(Errors::MISSING_MANDATORY_PARAMETER, schema_item->validate(obj));

  obj[S_PARAMS][S_FUNCTION_ID] = 2;
  // S_CORRELATION_ID and S_PROTOCOL_VERSION is still missed
  EXPECT_EQ(Errors::MISSING_MANDATORY_PARAMETER, schema_item->validate(obj));

  obj[S_PARAMS][S_CORRELATION_ID] = 0XFF2;
  // S_PROTOCOL_VERSION is still missed
  EXPECT_EQ(Errors::MISSING_MANDATORY_PARAMETER, schema_item->validate(obj));

  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  EXPECT_EQ(Errors::OK, schema_item->validate(obj));
}

TEST_F(ObjectSchemaItemTest, validation_unexpected_param) {
  const char* fake1 = "FAKE_PARAM1";
  const char* fake2 = "FAKE_PARAM2";
  const char* fake3 = "FAKE_PARAM3";

  SmartObject obj;
  obj[S_PARAMS][S_FUNCTION_ID] = 0;
  obj[S_PARAMS][S_CORRELATION_ID] = 0XFF;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 2;
  obj[S_MSG_PARAMS][Keys::INFO] = "123";
  obj[S_MSG_PARAMS][Keys::SUCCESS] = true;

  obj[fake1] = SmartObject(static_cast<int64_t>(0));
  // any fake parameter is OK
  EXPECT_EQ(Errors::OK, schema_item->validate(obj));

  obj[S_PARAMS][fake2] = SmartObject("123");
  // any fake parameters are OK
  EXPECT_EQ(Errors::OK, schema_item->validate(obj));

  obj[S_MSG_PARAMS][fake3] = true;
  // any fake parameters are OK
  EXPECT_EQ(Errors::OK, schema_item->validate(obj));
}

TEST_F(ObjectSchemaItemTest, validation_unexpected_param_remove) {
  const char* fake1 = "FAKE_PARAM1";
  const char* fake2 = "FAKE_PARAM2";
  const char* fake3 = "FAKE_PARAM3";

  SmartObject obj = SmartObject(SmartType::SmartType_Map);
  obj[S_PARAMS][S_FUNCTION_ID] = 0;
  obj[S_PARAMS][S_CORRELATION_ID] = 0XFF;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 2;
  obj[S_MSG_PARAMS][Keys::INFO] = "123";
  obj[S_MSG_PARAMS][Keys::SUCCESS] = true;

  obj[fake1] = SmartObject(static_cast<int64_t>(0));
  obj[S_PARAMS][fake2] = SmartObject("123");
  obj[S_MSG_PARAMS][fake3] = true;

  // Check apply schema, does not remove fake parameter
  schema_item->applySchema(obj, false);

  EXPECT_TRUE(obj.keyExists(fake1));
  EXPECT_TRUE(obj[S_PARAMS].keyExists(fake2));
  EXPECT_TRUE(obj[S_MSG_PARAMS].keyExists(fake3));
  EXPECT_EQ(Errors::OK, schema_item->validate(obj));

  // Check apply schema, remove fake parameter
  schema_item->applySchema(obj, true);
  EXPECT_FALSE(obj.keyExists(fake1));
  EXPECT_FALSE(obj[S_PARAMS].keyExists(fake2));
  EXPECT_FALSE(obj[S_MSG_PARAMS].keyExists(fake3));
  EXPECT_TRUE(obj[S_PARAMS].keyExists(S_FUNCTION_ID));
  EXPECT_TRUE(obj[S_PARAMS].keyExists(S_CORRELATION_ID));
  EXPECT_TRUE(obj[S_PARAMS].keyExists(S_PROTOCOL_VERSION));
  EXPECT_TRUE(obj[S_MSG_PARAMS].keyExists(Keys::RESULT_CODE));
  EXPECT_TRUE(obj[S_MSG_PARAMS].keyExists(Keys::INFO));
  EXPECT_TRUE(obj[S_MSG_PARAMS].keyExists(Keys::SUCCESS));
  EXPECT_EQ(Errors::OK, schema_item->validate(obj));


  obj[fake1] = SmartObject(static_cast<int64_t>(0));
  obj[S_PARAMS][fake2] = SmartObject("123");
  obj[S_MSG_PARAMS][fake3] = true;

  // all fake parameters are removed on unapply schema
  schema_item->unapplySchema(obj);

  // all fake parameters are removed on unapply schema
  EXPECT_FALSE(obj.keyExists(fake1));
  EXPECT_FALSE(obj[S_PARAMS].keyExists(fake2));
  EXPECT_FALSE(obj[S_MSG_PARAMS].keyExists(fake3));
  // Invalide state after enum convertion
  EXPECT_EQ(Errors::INVALID_VALUE, schema_item->validate(obj));
}

TEST_F(ObjectSchemaItemTest, validation_empty_params) {
  SmartObject obj;
  obj[S_PARAMS][S_FUNCTION_ID] = 1;
  obj[S_PARAMS][S_CORRELATION_ID] = 0xFF;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
  // S_MSG_PARAMS has only fake parameter
  obj[S_MSG_PARAMS]["FAKE_PARAM1"] = SmartObject();
  obj[S_MSG_PARAMS]["FAKE_PARAM2"] = SmartObject(0x1);
  obj[S_MSG_PARAMS]["FAKE_PARAM3"] = SmartObject("2");

  EXPECT_EQ(Errors::OK, schema_item->validate(obj));

  schema_item->applySchema(obj, false);
  EXPECT_EQ(Errors::OK, schema_item->validate(obj));

  schema_item->unapplySchema(obj);
  // Invalide state after enum convertion
  EXPECT_EQ(Errors::INVALID_VALUE, schema_item->validate(obj));
}

TEST_F(ObjectSchemaItemTest, test_strings_to_enum_conversion) {
  SmartObject object;
  object[S_PARAMS][S_FUNCTION_ID] = SmartObject();
  object[S_PARAMS][S_CORRELATION_ID] = 0XFF0;
  object[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  object[S_MSG_PARAMS][Keys::RESULT_CODE] = SmartObject();
  object[S_MSG_PARAMS][Keys::INFO] = "0123456789";
  object[S_MSG_PARAMS][Keys::SUCCESS] = true;

  typedef EnumConversionHelper<ResultType::eType>::CStringToEnumMap Results;
  const Results results =
      EnumConversionHelper<ResultType::eType>::cstring_to_enum_map();

  typedef EnumConversionHelper<FunctionID::eType>::CStringToEnumMap Functions;
  const Functions functions =
      EnumConversionHelper<FunctionID::eType>::cstring_to_enum_map();

  for (Results::const_iterator res_it = results.begin();
      res_it != results.end(); ++res_it) {
    for (Functions::const_iterator func_it = functions.begin();
        func_it != functions.end(); ++func_it) {
      const char* const function_str = func_it->first;
      const char* const result_type_str = res_it->first;
      const FunctionID::eType function_type = func_it->second;
      const ResultType::eType result_type = res_it->second;

      object[S_PARAMS][S_FUNCTION_ID] = function_str;
      object[S_MSG_PARAMS][Keys::RESULT_CODE] = result_type_str;

      // S_FUNCTION_ID and RESULT_CODE are not converted to int
      EXPECT_NE(Errors::OK, schema_item->validate(object));

      schema_item->applySchema(object, false);
      EXPECT_EQ(Errors::OK, schema_item->validate(object));

      // check conversion result
      EXPECT_EQ(function_type, object[S_PARAMS][S_FUNCTION_ID].asInt());
      EXPECT_EQ(result_type, object[S_MSG_PARAMS][Keys::RESULT_CODE].asInt());

      schema_item->unapplySchema(object);
      // S_FUNCTION_ID and RESULT_CODE are string
      EXPECT_NE(Errors::OK, schema_item->validate(object));

      // check conversion result
      EXPECT_EQ(function_str, object[S_PARAMS][S_FUNCTION_ID].asString());
      EXPECT_EQ(result_type_str,
                object[S_MSG_PARAMS][Keys::RESULT_CODE].asString());
    }
  }
}
// ----------------------------------------------------------------------------
}// namespace SchemaItem
}  // namespace SmartObjects
}  // namespace components
}  // namespace test

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

namespace FunctionID = test::components::SmartObjects::SchemaItem::FunctionID;
typedef EnumConversionHelper<FunctionID::eType> FunctionConvertor;

template<>
const FunctionConvertor::EnumToCStringMap FunctionConvertor::enum_to_cstring_map_ =
    FunctionConvertor::InitEnumToCStringMap();

template<>
const FunctionConvertor::CStringToEnumMap FunctionConvertor::cstring_to_enum_map_ =
    FunctionConvertor::InitCStringToEnumMap();

template<>
const char* const FunctionConvertor::cstring_values_[] =
    { "Function0", "Function1", "Function2", "Function3", "Function4",
        "Function5", "Function6" };

template<>
const FunctionID::eType FunctionConvertor::enum_values_[] = {
    FunctionID::Function0, FunctionID::Function1, FunctionID::Function2,
    FunctionID::Function3, FunctionID::Function4, FunctionID::Function5,
    FunctionID::Function6 };

// ----------------------------------------------------------------------------

namespace ResultType = test::components::SmartObjects::SchemaItem::ResultType;
typedef EnumConversionHelper<ResultType::eType> ResultTypeConvertor;

template<>
const ResultTypeConvertor::EnumToCStringMap ResultTypeConvertor::enum_to_cstring_map_ =
    ResultTypeConvertor::InitEnumToCStringMap();

template<>
const ResultTypeConvertor::CStringToEnumMap ResultTypeConvertor::cstring_to_enum_map_ =
    ResultTypeConvertor::InitCStringToEnumMap();

template<>
const char* const ResultTypeConvertor::cstring_values_[] = {
    "APPLICATION_NOT_REGISTERED", "SUCCESS", "TOO_MANY_PENDING_REQUESTS",
    "REJECTED", "INVALID_DATA", "OUT_OF_MEMORY", "ABORTED", "USER_DISALLOWED",
    "GENERIC_ERROR", "DISALLOWED" };

template<>
const ResultType::eType ResultTypeConvertor::enum_values_[] = {
    ResultType::APPLICATION_NOT_REGISTERED, ResultType::SUCCESS,
    ResultType::TOO_MANY_PENDING_REQUESTS, ResultType::REJECTED,
    ResultType::INVALID_DATA, ResultType::OUT_OF_MEMORY, ResultType::ABORTED,
    ResultType::USER_DISALLOWED, ResultType::GENERIC_ERROR,
    ResultType::DISALLOWED };
}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
