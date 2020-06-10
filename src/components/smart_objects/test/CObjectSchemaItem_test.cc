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

#include "formatters/CSmartFactory.h"
#include "formatters/generic_json_formatter.h"
#include "gmock/gmock.h"
#include "smart_objects/array_schema_item.h"
#include "smart_objects/bool_schema_item.h"
#include "smart_objects/enum_schema_item.h"
#include "smart_objects/number_schema_item.h"
#include "smart_objects/object_schema_item.h"
#include "smart_objects/schema_item_parameter.h"
#include "smart_objects/smart_object.h"
#include "smart_objects/smart_schema.h"
#include "smart_objects/string_schema_item.h"
#include "utils/semantic_version.h"

namespace formatters = ns_smart_device_link::ns_json_handler::formatters;
namespace smartobj = ns_smart_device_link::ns_smart_objects;

using namespace ns_smart_device_link::ns_smart_objects;
using namespace ns_smart_device_link::ns_json_handler::strings;

namespace test {
namespace components {
namespace smart_object_test {

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
const char STRUCT[] = "struct";
const char OPTIONAL_PARAM[] = "optionalParam";
const char MANDATORY_PARAM[] = "mandatoryParam";
}  // namespace Keys

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

    Members paramsMembersMap;
    paramsMembersMap[S_FUNCTION_ID] = SMember(
        TEnumSchemaItem<FunctionID::eType>::create(function_values), true);
    paramsMembersMap[S_CORRELATION_ID] =
        SMember(TNumberSchemaItem<int>::create(), true);
    paramsMembersMap[S_PROTOCOL_VERSION] =
        SMember(TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(1),
                                               TSchemaItemParameter<int>(2)),
                true);

    Members structMembersMap;
    structMembersMap[Keys::OPTIONAL_PARAM] = SMember(
        TEnumSchemaItem<FunctionID::eType>::create(function_values), false);
    structMembersMap[Keys::MANDATORY_PARAM] = SMember(
        TEnumSchemaItem<FunctionID::eType>::create(function_values), true);

    Members schemaMembersMap;
    schemaMembersMap[Keys::RESULT_CODE] = SMember(
        TEnumSchemaItem<ResultType::eType>::create(resultCode_values), false);
    schemaMembersMap[Keys::INFO] =
        SMember(CStringSchemaItem::create(TSchemaItemParameter<size_t>(0),
                                          TSchemaItemParameter<size_t>(10)),
                false);
    schemaMembersMap[Keys::SUCCESS] = SMember(CBoolSchemaItem::create(), false);
    schemaMembersMap[Keys::STRUCT] =
        SMember(CObjectSchemaItem::create(structMembersMap), false);

    // Create fake param that has breaking history changes
    std::vector<SMember> fake_param_history_vector;

    std::shared_ptr<ISchemaItem> fake_param_SchemaItem =
        CArraySchemaItem::create(
            CStringSchemaItem::create(TSchemaItemParameter<size_t>(1),
                                      TSchemaItemParameter<size_t>(99),
                                      TSchemaItemParameter<std::string>()),
            TSchemaItemParameter<size_t>(1),
            TSchemaItemParameter<size_t>(100));

    std::shared_ptr<ISchemaItem> fake_param_history_v1_SchemaItem =
        CArraySchemaItem::create(
            CStringSchemaItem::create(TSchemaItemParameter<size_t>(1),
                                      TSchemaItemParameter<size_t>(99),
                                      TSchemaItemParameter<std::string>()),
            TSchemaItemParameter<size_t>(1),
            TSchemaItemParameter<size_t>(100));

    fake_param_history_vector.push_back(SMember(
        fake_param_history_v1_SchemaItem, true, "", "4.5.0", false, false));
    schemaMembersMap["fakeParam"] = SMember(fake_param_SchemaItem,
                                            false,
                                            "4.5.0",
                                            "",
                                            false,
                                            false,
                                            fake_param_history_vector);

    Members rootMembersMap;
    rootMembersMap[S_PARAMS] =
        SMember(CObjectSchemaItem::create(paramsMembersMap), true);
    rootMembersMap[S_MSG_PARAMS] =
        SMember(CObjectSchemaItem::create(schemaMembersMap), true);

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

  rpc::ValidationReport report("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
}

TEST_F(ObjectSchemaItemTest, validation_correct_with_new_version) {
  SmartObject obj;
  obj[S_PARAMS][S_FUNCTION_ID] = 0;
  obj[S_PARAMS][S_CORRELATION_ID] = 0XFF0;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 0;
  obj[S_MSG_PARAMS][Keys::INFO] = "0123456789";
  obj[S_MSG_PARAMS][Keys::SUCCESS] = true;

  utils::SemanticVersion messageVersion(4, 5, 0);
  rpc::ValidationReport report("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report, messageVersion));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
}

TEST_F(ObjectSchemaItemTest, validation_invalid_data_with_old_version) {
  SmartObject obj;
  obj[S_PARAMS][S_FUNCTION_ID] = 0;
  obj[S_PARAMS][S_CORRELATION_ID] = 0XFF0;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 0;
  obj[S_MSG_PARAMS][Keys::INFO] = "0123456789";
  obj[S_MSG_PARAMS][Keys::SUCCESS] = true;

  utils::SemanticVersion messageVersion(3, 0, 0);
  rpc::ValidationReport report("RPC");
  EXPECT_EQ(errors::MISSING_MANDATORY_PARAMETER,
            schema_item->validate(obj, &report, messageVersion));
}

TEST_F(ObjectSchemaItemTest, validation_correct_skip_not_mandatory) {
  SmartObject obj;
  obj[S_PARAMS][S_FUNCTION_ID] = 1;
  obj[S_PARAMS][S_CORRELATION_ID] = -0xFF1;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 1;
  // skip non-mandatory obj[S_MSG_PARAMS][Keys::INFO]
  obj[S_MSG_PARAMS][Keys::SUCCESS] = false;

  rpc::ValidationReport report("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
}

TEST_F(ObjectSchemaItemTest, validation_invalid_param) {
  SmartObject obj;
  obj[S_PARAMS] = "some parameters";
  obj[S_MSG_PARAMS] = "some message parameters";

  rpc::ValidationReport report("RPC");
  EXPECT_EQ(errors::INVALID_VALUE, schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  obj[S_PARAMS][S_FUNCTION_ID] = "some function";
  obj[S_PARAMS][S_CORRELATION_ID] = "some correlation id";
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 3;
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = "some result";
  obj[S_MSG_PARAMS][Keys::SUCCESS] = 0xABC;

  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::INVALID_VALUE, schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  obj[S_PARAMS][S_FUNCTION_ID] = 1;
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::INVALID_VALUE, schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  obj[S_PARAMS][S_CORRELATION_ID] = -0xFF1;
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::INVALID_VALUE, schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  obj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::INVALID_VALUE, schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 1;
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::INVALID_VALUE, schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  obj[S_MSG_PARAMS][Keys::SUCCESS] = false;

  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
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
  rpc::ValidationReport report("RPC");
  EXPECT_EQ(errors::INVALID_VALUE, schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  // invalid non-mandatory obj[S_MSG_PARAMS][Keys::INFO]
  obj[S_MSG_PARAMS][Keys::INFO] = true;
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::INVALID_VALUE, schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  // invalid non-mandatory obj[S_MSG_PARAMS][Keys::INFO]
  obj[S_MSG_PARAMS][Keys::INFO] = SmartObject();
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::INVALID_VALUE, schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  obj[S_MSG_PARAMS][Keys::INFO] = "info";
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
}

TEST_F(ObjectSchemaItemTest, validation_missing_mandatory) {
  SmartObject obj;
  // missed obj[S_PARAMS][S_FUNCTION_ID]
  // missed obj[S_PARAMS][S_CORRELATION_ID]
  // missed obj[S_PARAMS][S_PROTOCOL_VERSION]
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 2;
  obj[S_MSG_PARAMS][Keys::INFO] = "123";
  obj[S_MSG_PARAMS][Keys::SUCCESS] = false;

  rpc::ValidationReport report("RPC");
  EXPECT_EQ(errors::MISSING_MANDATORY_PARAMETER,
            schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  obj[S_PARAMS][S_FUNCTION_ID] = 2;
  // S_CORRELATION_ID and S_PROTOCOL_VERSION is still missed
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::MISSING_MANDATORY_PARAMETER,
            schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  obj[S_PARAMS][S_CORRELATION_ID] = 0XFF2;
  // S_PROTOCOL_VERSION is still missed
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::MISSING_MANDATORY_PARAMETER,
            schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  obj[S_PARAMS][S_PROTOCOL_VERSION] = 1;
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
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
  rpc::ValidationReport report("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));

  obj[S_PARAMS][fake2] = SmartObject("123");
  // any fake parameters are OK
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));

  obj[S_MSG_PARAMS][fake3] = true;
  // any fake parameters are OK
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
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
  rpc::ValidationReport report("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));

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
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));

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
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::INVALID_VALUE, schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));
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

  rpc::ValidationReport report("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));

  schema_item->applySchema(obj, false);
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));

  schema_item->unapplySchema(obj);
  // Invalide state after enum convertion
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::INVALID_VALUE, schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));
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
       res_it != results.end();
       ++res_it) {
    for (Functions::const_iterator func_it = functions.begin();
         func_it != functions.end();
         ++func_it) {
      const char* const function_str = func_it->first;
      const char* const result_type_str = res_it->first;
      const FunctionID::eType function_type = func_it->second;
      const ResultType::eType result_type = res_it->second;

      object[S_PARAMS][S_FUNCTION_ID] = function_str;
      object[S_MSG_PARAMS][Keys::RESULT_CODE] = result_type_str;

      // S_FUNCTION_ID and RESULT_CODE are not converted to int
      rpc::ValidationReport report("RPC");
      EXPECT_NE(errors::OK, schema_item->validate(object, &report));
      EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

      schema_item->applySchema(object, false);
      report = rpc::ValidationReport("RPC");
      EXPECT_EQ(errors::OK, schema_item->validate(object, &report));
      EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));

      // check conversion result
      EXPECT_EQ(function_type, object[S_PARAMS][S_FUNCTION_ID].asInt());
      EXPECT_EQ(result_type, object[S_MSG_PARAMS][Keys::RESULT_CODE].asInt());

      schema_item->unapplySchema(object);
      // S_FUNCTION_ID and RESULT_CODE are string
      report = rpc::ValidationReport("RPC");
      EXPECT_NE(errors::OK, schema_item->validate(object, &report));
      EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

      // check conversion result
      EXPECT_EQ(function_str, object[S_PARAMS][S_FUNCTION_ID].asString());
      EXPECT_EQ(result_type_str,
                object[S_MSG_PARAMS][Keys::RESULT_CODE].asString());
    }
  }
}

TEST_F(ObjectSchemaItemTest, filter_unknown_enums_non_mandatory_param) {
  SmartObject obj;
  obj[S_PARAMS][S_FUNCTION_ID] = 1;
  obj[S_PARAMS][S_CORRELATION_ID] = 0xFF;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = "FUTURE";
  obj[S_MSG_PARAMS][Keys::INFO] = "0123456789";

  schema_item->applySchema(obj, false);
  rpc::ValidationReport report("RPC");
  EXPECT_FALSE(
      schema_item->filterInvalidEnums(obj, utils::SemanticVersion(), &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  // The unknown enum value was filtered. Validation should pass in this case.
  EXPECT_FALSE(obj[S_MSG_PARAMS].keyExists(Keys::RESULT_CODE));
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
}

TEST_F(ObjectSchemaItemTest, filter_unknown_enums_mandatory) {
  SmartObject obj;
  obj[S_PARAMS][S_FUNCTION_ID] = "FUTURE";
  obj[S_PARAMS][S_CORRELATION_ID] = 0xFF;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 2;
  obj[S_MSG_PARAMS][Keys::INFO] = "0123456789";

  schema_item->applySchema(obj, false);
  rpc::ValidationReport report("RPC");
  EXPECT_TRUE(
      schema_item->filterInvalidEnums(obj, utils::SemanticVersion(), &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  EXPECT_FALSE(obj[S_PARAMS].keyExists(S_FUNCTION_ID));
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::MISSING_MANDATORY_PARAMETER,
            schema_item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));
}

TEST_F(ObjectSchemaItemTest, filter_unknown_enums_non_mandatory_subparam) {
  SmartObject obj;
  obj[S_PARAMS][S_FUNCTION_ID] = 1;
  obj[S_PARAMS][S_CORRELATION_ID] = 0xFF;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  obj[S_MSG_PARAMS][Keys::STRUCT][Keys::OPTIONAL_PARAM] = "FUTURE";
  obj[S_MSG_PARAMS][Keys::STRUCT][Keys::MANDATORY_PARAM] = 1;
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 2;
  obj[S_MSG_PARAMS][Keys::INFO] = "0123456789";

  schema_item->applySchema(obj, false);
  rpc::ValidationReport report("RPC");
  EXPECT_FALSE(
      schema_item->filterInvalidEnums(obj, utils::SemanticVersion(), &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  // The unknown enum value was filtered.
  // Validation should pass in this case.
  ASSERT_TRUE(obj[S_MSG_PARAMS].keyExists(Keys::STRUCT));
  EXPECT_FALSE(obj[S_MSG_PARAMS][Keys::STRUCT].keyExists(Keys::OPTIONAL_PARAM));
  EXPECT_TRUE(obj[S_MSG_PARAMS][Keys::STRUCT].keyExists(Keys::MANDATORY_PARAM));
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
}

TEST_F(ObjectSchemaItemTest, filter_unknown_enums_mandatory_subparam) {
  SmartObject obj;
  obj[S_PARAMS][S_FUNCTION_ID] = 1;
  obj[S_PARAMS][S_CORRELATION_ID] = 0xFF;
  obj[S_PARAMS][S_PROTOCOL_VERSION] = 2;
  obj[S_MSG_PARAMS][Keys::STRUCT][Keys::MANDATORY_PARAM] = "FUTURE";
  obj[S_MSG_PARAMS][Keys::RESULT_CODE] = 2;
  obj[S_MSG_PARAMS][Keys::INFO] = "0123456789";

  schema_item->applySchema(obj, false);
  rpc::ValidationReport report("RPC");
  EXPECT_FALSE(
      schema_item->filterInvalidEnums(obj, utils::SemanticVersion(), &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  // The struct containing the unknown enum value was filtered.
  // Validation should pass in this case.
  EXPECT_FALSE(obj[S_MSG_PARAMS].keyExists(Keys::STRUCT));
  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::OK, schema_item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
}

}  // namespace smart_object_test
}  // namespace components
}  // namespace test

namespace ns_smart_device_link {
namespace ns_smart_objects {

namespace FunctionID = test::components::smart_object_test::FunctionID;
typedef EnumConversionHelper<FunctionID::eType> FunctionConverter;

template <>
const FunctionConverter::EnumToCStringMap
    FunctionConverter::enum_to_cstring_map_ =
        FunctionConverter::InitEnumToCStringMap();

template <>
const FunctionConverter::CStringToEnumMap
    FunctionConverter::cstring_to_enum_map_ =
        FunctionConverter::InitCStringToEnumMap();

template <>
const char* const FunctionConverter::cstring_values_[] = {"Function0",
                                                          "Function1",
                                                          "Function2",
                                                          "Function3",
                                                          "Function4",
                                                          "Function5",
                                                          "Function6"};

template <>
const FunctionID::eType FunctionConverter::enum_values_[] = {
    FunctionID::Function0,
    FunctionID::Function1,
    FunctionID::Function2,
    FunctionID::Function3,
    FunctionID::Function4,
    FunctionID::Function5,
    FunctionID::Function6};

// ----------------------------------------------------------------------------

namespace ResultType = test::components::smart_object_test::ResultType;
typedef EnumConversionHelper<ResultType::eType> ResultTypeConverter;

template <>
const ResultTypeConverter::EnumToCStringMap
    ResultTypeConverter::enum_to_cstring_map_ =
        ResultTypeConverter::InitEnumToCStringMap();

template <>
const ResultTypeConverter::CStringToEnumMap
    ResultTypeConverter::cstring_to_enum_map_ =
        ResultTypeConverter::InitCStringToEnumMap();

template <>
const char* const ResultTypeConverter::cstring_values_[] = {
    "APPLICATION_NOT_REGISTERED",
    "SUCCESS",
    "TOO_MANY_PENDING_REQUESTS",
    "REJECTED",
    "INVALID_DATA",
    "OUT_OF_MEMORY",
    "ABORTED",
    "USER_DISALLOWED",
    "GENERIC_ERROR",
    "DISALLOWED"};

template <>
const ResultType::eType ResultTypeConverter::enum_values_[] = {
    ResultType::APPLICATION_NOT_REGISTERED,
    ResultType::SUCCESS,
    ResultType::TOO_MANY_PENDING_REQUESTS,
    ResultType::REJECTED,
    ResultType::INVALID_DATA,
    ResultType::OUT_OF_MEMORY,
    ResultType::ABORTED,
    ResultType::USER_DISALLOWED,
    ResultType::GENERIC_ERROR,
    ResultType::DISALLOWED};
}  // namespace ns_smart_objects
}  // namespace ns_smart_device_link
