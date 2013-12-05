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

#include <string>

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
#include "smart_objects/object_optional_schema_item.h"
#include "smart_objects/string_schema_item.h"
#include "smart_objects/enum_schema_item.h"
#include "smart_objects/number_schema_item.h"
#include "smart_objects/schema_item_parameter.h"
#include "formatters/generic_json_formatter.h"

namespace formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace smartobj = NsSmartDeviceLink::NsSmartObjects;

using namespace NsSmartDeviceLink::NsSmartObjects;  // NOLINT
using namespace NsSmartDeviceLink::NsJSONHandler::strings;  // NOLINT

namespace test {
namespace components {
namespace SmartObjects {
namespace SchemaItem {

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
}  // namespace TestType

namespace Priority {
enum eType {
  INVALID_ENUM = -1,
  PRIORITY_NONE = 0,
  PRIORITY_NORMAL,
  PRIORITY_COMMUNICATION,
  PRIORITY_NAVIGATION,
  PRIORITY_EMERGENCY,
};
}  // namespace Priority


class ObjectSchemaItemTest : public ::testing::Test {
  protected:
  // Create SmartObjectSchema for test object
  utils::SharedPtr<ISchemaItem> initObjectSchemaItem(void) {
    std::set<TestType::eType> resultCode_allowedEnumSubsetValues;
    resultCode_allowedEnumSubsetValues.insert(
        TestType::APPLICATION_NOT_REGISTERED);
    resultCode_allowedEnumSubsetValues.insert(
        TestType::SUCCESS);
    resultCode_allowedEnumSubsetValues.insert(
        TestType::TOO_MANY_PENDING_REQUESTS);
    resultCode_allowedEnumSubsetValues.insert(TestType::REJECTED);
    resultCode_allowedEnumSubsetValues.insert(TestType::INVALID_DATA);
    resultCode_allowedEnumSubsetValues.insert(TestType::OUT_OF_MEMORY);
    resultCode_allowedEnumSubsetValues.insert(TestType::ABORTED);
    resultCode_allowedEnumSubsetValues.insert(TestType::USER_DISALLOWED);
    resultCode_allowedEnumSubsetValues.insert(TestType::GENERIC_ERROR);
    resultCode_allowedEnumSubsetValues.insert(TestType::DISALLOWED);

    utils::SharedPtr<ISchemaItem> success_SchemaItem =
      CBoolSchemaItem::create(TSchemaItemParameter<bool>());

    utils::SharedPtr<ISchemaItem> resultCode_SchemaItem =
      TEnumSchemaItem<TestType::eType>::create(
        resultCode_allowedEnumSubsetValues
        , TSchemaItemParameter<TestType::eType>());

    utils::SharedPtr<ISchemaItem> info_SchemaItem =
      CStringSchemaItem::create(TSchemaItemParameter<size_t>(0)
                                , TSchemaItemParameter<size_t>(10)
                                , TSchemaItemParameter<std::string>());

    utils::SharedPtr<ISchemaItem> tryAgainTime_SchemaItem =
      TNumberSchemaItem<int>::create(TSchemaItemParameter<int>(0)
                                      , TSchemaItemParameter<int>(2000)
                                      , TSchemaItemParameter<int>());

    std::map<std::string, ObjectOptionalSchemaItem::SMember> schemaMembersMap;

    schemaMembersMap["success"] =
        ObjectOptionalSchemaItem::SMember(success_SchemaItem, true);
    schemaMembersMap["resultCode"] =
        ObjectOptionalSchemaItem::SMember(resultCode_SchemaItem, true);
    schemaMembersMap["info"] =
        ObjectOptionalSchemaItem::SMember(info_SchemaItem, false);
    schemaMembersMap["tryAgainTime"] =
        ObjectOptionalSchemaItem::SMember(tryAgainTime_SchemaItem, true);

    std::map<std::string, ObjectOptionalSchemaItem::SMember> paramsMembersMap;
    paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_FUNCTION_ID] =  // NOLINT
      ObjectOptionalSchemaItem::SMember(TEnumSchemaItem<TestType::eType>::create(resultCode_allowedEnumSubsetValues), true);  // NOLINT
    paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_MESSAGE_TYPE] =  // NOLINT
      ObjectOptionalSchemaItem::SMember(TEnumSchemaItem<TestType::eType>::create(resultCode_allowedEnumSubsetValues), true);  // NOLINT
    paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_CORRELATION_ID] =  // NOLINT
      ObjectOptionalSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);  // NOLINT
    paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_VERSION] =  // NOLINT
      ObjectOptionalSchemaItem::SMember(TNumberSchemaItem<int>::create(
          TSchemaItemParameter<int>(1),
          TSchemaItemParameter<int>(2)),
        true);

    paramsMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_PROTOCOL_TYPE] =  // NOLINT
      ObjectOptionalSchemaItem::SMember(TNumberSchemaItem<int>::create(), true);  // NOLINT

    std::map<std::string, ObjectOptionalSchemaItem::SMember> rootMembersMap;
    rootMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS] =
      ObjectOptionalSchemaItem::SMember(ObjectOptionalSchemaItem::create(schemaMembersMap), true);  // NOLINT
    rootMembersMap[NsSmartDeviceLink::NsJSONHandler::strings::S_PARAMS] =
      ObjectOptionalSchemaItem::SMember(ObjectOptionalSchemaItem::create(paramsMembersMap), true);  // NOLINT

    return ObjectOptionalSchemaItem::create(rootMembersMap);
  }

  //------------------------------------------------------------------------

  utils::SharedPtr<ISchemaItem> initObjectOptonalSchemaItem(
      bool isOtptionalMandatory = false) {
  // NOLINT
    utils::SharedPtr<ISchemaItem> priority_item =
      CStringSchemaItem::create(TSchemaItemParameter<size_t>(0),
                                TSchemaItemParameter<size_t>(100),
                                TSchemaItemParameter<std::string>("NONE"));

    utils::SharedPtr<ISchemaItem> groups_item =
      CArraySchemaItem::create(CStringSchemaItem::create());

    utils::SharedPtr<ISchemaItem> nicknames_item =
      CArraySchemaItem::create(CStringSchemaItem::create());

    std::map<std::string, CObjectSchemaItem::SMember> app_id_map;
    app_id_map["priority"] = CObjectSchemaItem::SMember(priority_item, true);
    app_id_map["groups"] = CObjectSchemaItem::SMember(groups_item, true);
    app_id_map["nicknames"] = CObjectSchemaItem::SMember(nicknames_item, true);

    std::map<std::string, CObjectSchemaItem::SMember> app_policies_map;
    // non mandatory optional parameter
    app_policies_map[ObjectOptionalSchemaItem::kOptionalGenericFieldName] =
      CObjectSchemaItem::SMember(CObjectSchemaItem::create(app_id_map), isOtptionalMandatory); // NOLINT

    std::map<std::string, CObjectSchemaItem::SMember> policy_table_map;
    policy_table_map["app_policies"] =
      CObjectSchemaItem::SMember(
        ObjectOptionalSchemaItem::create(app_policies_map), true);

    std::map<std::string, CObjectSchemaItem::SMember> root_map;
    root_map["policy_table"] =
    CObjectSchemaItem::SMember(
        CObjectSchemaItem::create(policy_table_map), true);

    return CObjectSchemaItem::create(root_map);
  }

  //------------------------------------------------------------------------

  utils::SharedPtr<ISchemaItem> initObjectOptonalSchemaItemAlongWithStaticMember(  // NOLINT
      bool isOtptionalMandatory = false) {
  // NOLINT
    utils::SharedPtr<ISchemaItem> priority_item =
      CStringSchemaItem::create(TSchemaItemParameter<size_t>(0),
                                TSchemaItemParameter<size_t>(100),
                                TSchemaItemParameter<std::string>("NONE"));

    utils::SharedPtr<ISchemaItem> groups_item =
      CArraySchemaItem::create(CStringSchemaItem::create());

    utils::SharedPtr<ISchemaItem> nicknames_item =
      CArraySchemaItem::create(CStringSchemaItem::create());

    std::map<std::string, CObjectSchemaItem::SMember> app_id_map;
    std::map<std::string, CObjectSchemaItem::SMember> default_map;
    app_id_map["priority"] = CObjectSchemaItem::SMember(priority_item, true);
    app_id_map["groups"] = CObjectSchemaItem::SMember(groups_item, true);
    app_id_map["nicknames"] = CObjectSchemaItem::SMember(nicknames_item, true);
    default_map["priority"] = CObjectSchemaItem::SMember(priority_item, true);
    default_map["groups"] = CObjectSchemaItem::SMember(groups_item, true);

    std::map<std::string, CObjectSchemaItem::SMember> app_policies_map;
    // optional parameter
    app_policies_map[ObjectOptionalSchemaItem::kOptionalGenericFieldName] =
      CObjectSchemaItem::SMember(CObjectSchemaItem::create(app_id_map),
                                 isOtptionalMandatory);
    app_policies_map["default"] =
      CObjectSchemaItem::SMember(CObjectSchemaItem::create(default_map), true);

    std::map<std::string, CObjectSchemaItem::SMember> policy_table_map;
    policy_table_map["app_policies"] =
      CObjectSchemaItem::SMember(
        ObjectOptionalSchemaItem::create(app_policies_map), true);

    std::map<std::string, CObjectSchemaItem::SMember> root_map;
    root_map["policy_table"] =
    CObjectSchemaItem::SMember(
        CObjectSchemaItem::create(policy_table_map), true);

    return CObjectSchemaItem::create(root_map);
  }

  //------------------------------------------------------------------------

  utils::SharedPtr<ISchemaItem>
    initTestSchemaWithEnum(bool isOtptionalMandatory = false) {
  // NOLINT
    std::set<Priority::eType> allowed_priority;
    allowed_priority.insert(Priority::PRIORITY_COMMUNICATION);
    allowed_priority.insert(Priority::PRIORITY_EMERGENCY);
    allowed_priority.insert(Priority::PRIORITY_NAVIGATION);
    allowed_priority.insert(Priority::PRIORITY_NONE);
    allowed_priority.insert(Priority::PRIORITY_NORMAL);

    utils::SharedPtr<ISchemaItem> priority_item =
      TEnumSchemaItem<Priority::eType>::create(allowed_priority);

    utils::SharedPtr<ISchemaItem> groups_item =
      CArraySchemaItem::create(CStringSchemaItem::create());

    utils::SharedPtr<ISchemaItem> nicknames_item =
      CArraySchemaItem::create(CStringSchemaItem::create());

    std::map<std::string, CObjectSchemaItem::SMember> app_id_map;
    std::map<std::string, CObjectSchemaItem::SMember> default_map;
    app_id_map["priority"] = CObjectSchemaItem::SMember(priority_item, true);
    app_id_map["groups"] = CObjectSchemaItem::SMember(groups_item, true);
    app_id_map["nicknames"] = CObjectSchemaItem::SMember(nicknames_item, true);
    default_map["priority"] = CObjectSchemaItem::SMember(priority_item, true);
    default_map["groups"] = CObjectSchemaItem::SMember(groups_item, true);

    std::map<std::string, CObjectSchemaItem::SMember> app_policies_map;
    // optional parameter
    app_policies_map[ObjectOptionalSchemaItem::kOptionalGenericFieldName] =
      CObjectSchemaItem::SMember(CObjectSchemaItem::create(app_id_map),
                                  isOtptionalMandatory);
    app_policies_map["default"] =
      CObjectSchemaItem::SMember(CObjectSchemaItem::create(default_map), true);

    std::map<std::string, CObjectSchemaItem::SMember> policy_table_map;
    policy_table_map["app_policies"] =
      CObjectSchemaItem::SMember(
        ObjectOptionalSchemaItem::create(app_policies_map), true);

    std::map<std::string, CObjectSchemaItem::SMember> root_map;
    root_map["policy_table"] =
    CObjectSchemaItem::SMember(
        CObjectSchemaItem::create(policy_table_map), true);

    return CObjectSchemaItem::create(root_map);
  }
};

TEST_F(ObjectSchemaItemTest, test_too_few_object_params) {
  SmartObject obj;
  utils::SharedPtr<ISchemaItem> item = initObjectSchemaItem();

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
  utils::SharedPtr<ISchemaItem> item = initObjectSchemaItem();

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
  srcObj[S_MSG_PARAMS]["null"] =
    NsSmartDeviceLink::NsSmartObjects::SmartObject();
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
  utils::SharedPtr<ISchemaItem> item = initObjectSchemaItem();

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
  utils::SharedPtr<ISchemaItem> item = initObjectSchemaItem();

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

TEST_F(ObjectSchemaItemTest, test_object_with_optional_params) {
  std::string json_str(
  "{\n"
    "  \"policy_table\":{\n"
    "    \"app_policies\": {\n"
    "        \"789\":{\n"
    "            \"priority\":\"NORMAL\",\n"
    "            \"groups\":[\n"
    "               \"Notifications\",\n"
    "               \"Base-4\"\n"
    "            ],\n"
    "            \"nicknames\":[\n"
    "               \"Music App\"\n"
    "            ]\n"
    "        }\n"
    "    }\n"
    "}\n"
  "}");

  smartobj::SmartObject object;
  bool fromstring_result =
    formatters::GenericJsonFormatter::FromString(json_str, object);

  ASSERT_EQ(true, fromstring_result);

  utils::SharedPtr<ISchemaItem> schema1 = initObjectOptonalSchemaItem(false);
  int resultType = schema1->validate(object);
  EXPECT_EQ(Errors::OK, resultType);

  utils::SharedPtr<ISchemaItem> schema2 = initObjectOptonalSchemaItem(true);
  resultType = schema2->validate(object);
  EXPECT_EQ(Errors::OK, resultType);

  // store priority value
  std::string priority =
      object["policy_table"]["app_policies"]["789"]["priority"].asString();
  // set empty string. Validation should pass successfuly.
  object["policy_table"]["app_policies"]["789"]["priority"] = "";
  resultType = schema1->validate(object);
  EXPECT_EQ(Errors::OK, resultType);
  resultType = schema2->validate(object);
  EXPECT_EQ(Errors::OK, resultType);
  // restore priority value
  object["policy_table"]["app_policies"]["789"]["priority"] = priority;

  // remove non-mandatory (for schema1) section "789"
  object["policy_table"]["app_policies"].erase("789");
  resultType = schema1->validate(object);
  EXPECT_EQ(Errors::OK, resultType);
}

TEST_F(ObjectSchemaItemTest, test_object_with_optional_params_failed_validation) {  // NOLINT
  std::string json_str(
  "{\n"
    "  \"policy_table\":{\n"
    "    \"app_policies\": {\n"
    "        \"789\":{\n"
    "            \"priority\":\"NORMAL\",\n"
    "            \"groups\":[\n"
    "               \"Notifications\",\n"
    "               \"Base-4\"\n"
    "            ],\n"
    "            \"nicknames\":[\n"
    "               \"Music App\"\n"
    "            ]\n"
    "        }\n"
    "    }\n"
    "}\n"
  "}");

  smartobj::SmartObject object;
  bool fromstring_result =
    formatters::GenericJsonFormatter::FromString(json_str, object);

  ASSERT_EQ(true, fromstring_result);

  utils::SharedPtr<ISchemaItem> schema1 = initObjectOptonalSchemaItem(false);
  utils::SharedPtr<ISchemaItem> schema2 = initObjectOptonalSchemaItem(true);

  /////////////////////////////////////////////
  // Below tests with failed validation.
  /////////////////////////////////////////////

  // Schema is created in initObjectOptonalSchemaItemAlongWithStaticMember()
  // that describes another object (object from TEST_F(test_object_with_optional_and_static_param))  // NOLINT
  utils::SharedPtr<ISchemaItem> schema_item_another_object =
    initObjectOptonalSchemaItemAlongWithStaticMember();
  int resultType = schema_item_another_object->validate(object);
  // mising mandatory parameter policy_table|app_policies|default
  EXPECT_EQ(Errors::MISSING_MANDATORY_PARAMETER, resultType);

  // set value of type int. Schema expects string value.
  object["policy_table"]["app_policies"]["789"]["priority"] = 25;
  resultType = schema1->validate(object);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  // Remove optional parameter.
  // Set optional parameter as mandatory in schema.
  object["policy_table"]["app_policies"].erase("789");
  utils::SharedPtr<ISchemaItem> schema_optional_mandatory =
    initObjectOptonalSchemaItem(true);
  resultType = schema_optional_mandatory->validate(object);
  EXPECT_EQ(Errors::MISSING_MANDATORY_UNTITLED_PARAMETER, resultType);
}

TEST_F(ObjectSchemaItemTest, test_object_with_optional_and_static_param) {
  std::string json_str(
  "{\n"
    "  \"policy_table\":{\n"
    "    \"app_policies\": {\n"
    "        \"789\":{\n"
    "            \"priority\":\"NORMAL\",\n"
    "            \"groups\":[\n"
    "               \"Notifications\",\n"
    "               \"Base-4\"\n"
    "            ],\n"
    "            \"nicknames\":[\n"
    "               \"Music App\"\n"
    "            ]\n"
    "        },\n"
    "       \"default\":{\n"
    "            \"priority\":\"NONE\",\n"
    "            \"groups\":[\n"
    "               \"Base-4\"\n"
    "            ]\n"
    "        }\n"
    "    }\n"
    "}\n"
  "}");

  smartobj::SmartObject object;
  bool fromstring_result =
    formatters::GenericJsonFormatter::FromString(json_str, object);

  ASSERT_EQ(true, fromstring_result);

  utils::SharedPtr<ISchemaItem> schema1 =
    initObjectOptonalSchemaItemAlongWithStaticMember(false);
  int resultType = schema1->validate(object);
  EXPECT_EQ(Errors::OK, resultType);

  utils::SharedPtr<ISchemaItem> schema2 =
    initObjectOptonalSchemaItemAlongWithStaticMember(true);
  resultType = schema2->validate(object);
  EXPECT_EQ(Errors::OK, resultType);
}

// ----------------------------------------------------------------------------

TEST_F(ObjectSchemaItemTest, test_strings_to_enum_convertion) {
  std::string json_str(
  "{\n"
    "  \"policy_table\":{\n"
    "    \"app_policies\": {\n"
    "        \"789\":{\n"
    "            \"priority\":\"NORMAL\",\n"
    "            \"groups\":[\n"
    "               \"Notifications\",\n"
    "               \"Base-4\"\n"
    "            ],\n"
    "            \"nicknames\":[\n"
    "               \"Music App\"\n"
    "            ]\n"
    "        },\n"
    "       \"default\":{\n"
    "            \"priority\":\"NONE\",\n"
    "            \"groups\":[\n"
    "               \"Base-4\"\n"
    "            ]\n"
    "        }\n"
    "    }\n"
    "}\n"
  "}");

  smartobj::SmartObject object;
  bool fromstring_result =
    formatters::GenericJsonFormatter::FromString(json_str, object);

  ASSERT_EQ(true, fromstring_result);

  utils::SharedPtr<ISchemaItem> schema1 = initTestSchemaWithEnum(false);
  schema1->applySchema(object);
  int resultType = schema1->validate(object);
  EXPECT_EQ(Errors::OK, resultType);
  EXPECT_EQ(Priority::PRIORITY_NORMAL,
    object["policy_table"]["app_policies"]["789"]["priority"].asInt());

  EXPECT_EQ(Priority::PRIORITY_NONE,
    object["policy_table"]["app_policies"]["default"]["priority"].asInt());

  schema1->unapplySchema(object);
  EXPECT_EQ("NORMAL",
    object["policy_table"]["app_policies"]["789"]["priority"].asString());

  EXPECT_EQ("NONE",
    object["policy_table"]["app_policies"]["default"]["priority"].asString());


//   utils::SharedPtr<ISchemaItem> schema2 = initTestSchemaWithEnum(true);
//   resultType = schema2->validate(object);
//   EXPECT_EQ(Errors::OK, resultType);
}

// ----------------------------------------------------------------------------


}  // namespace SchemaItem
}  // namespace SmartObjects
}  // namespace components
}  // namespace test

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

template<>
const std::map<test::components::SmartObjects::SchemaItem::TestType::eType, std::string>&  // NOLINT
  TEnumSchemaItem<test::components::SmartObjects::SchemaItem::TestType::eType>::getEnumElementsStringRepresentation(void) {  // NOLINT
  // NOLINT
  static bool isInitialized = false;
  static std::map<test::components::SmartObjects::SchemaItem::TestType::eType, std::string> enumStringRepresentationMap;  // NOLINT

  if (false == isInitialized) {
    enumStringRepresentationMap.insert(std::make_pair(
      test::components::SmartObjects::SchemaItem::TestType::APPLICATION_NOT_REGISTERED, "APPLICATION_NOT_REGISTERED"));  // NOLINT
    enumStringRepresentationMap.insert(std::make_pair(
      test::components::SmartObjects::SchemaItem::TestType::SUCCESS, "SUCCESS"));  // NOLINT
    enumStringRepresentationMap.insert(std::make_pair(
      test::components::SmartObjects::SchemaItem::TestType::TOO_MANY_PENDING_REQUESTS, "TOO_MANY_PENDING_REQUESTS"));  // NOLINT
    enumStringRepresentationMap.insert(std::make_pair(
      test::components::SmartObjects::SchemaItem::TestType::REJECTED, "REJECTED"));  // NOLINT
    enumStringRepresentationMap.insert(std::make_pair(
      test::components::SmartObjects::SchemaItem::TestType::INVALID_DATA, "INVALID_DATA"));  // NOLINT
    enumStringRepresentationMap.insert(std::make_pair(
      test::components::SmartObjects::SchemaItem::TestType::OUT_OF_MEMORY, "OUT_OF_MEMORY"));  // NOLINT
    enumStringRepresentationMap.insert(std::make_pair(
      test::components::SmartObjects::SchemaItem::TestType::ABORTED, "ABORTED"));  // NOLINT
    enumStringRepresentationMap.insert(std::make_pair(
      test::components::SmartObjects::SchemaItem::TestType::USER_DISALLOWED, "USER_DISALLOWED"));  // NOLINT
    enumStringRepresentationMap.insert(std::make_pair(
      test::components::SmartObjects::SchemaItem::TestType::GENERIC_ERROR, "GENERIC_ERROR"));  // NOLINT
    enumStringRepresentationMap.insert(std::make_pair(
      test::components::SmartObjects::SchemaItem::TestType::DISALLOWED, "DISALLOWED"));  // NOLINT

    isInitialized = true;
  }
  return enumStringRepresentationMap;
}

// ----------------------------------------------------------------------------

namespace TestPriority = test::components::SmartObjects::SchemaItem::Priority;

template <>
const std::map<TestPriority::eType, std::string>&
  TEnumSchemaItem<TestPriority::eType>::
    getEnumElementsStringRepresentation() {
  // NOLINT
  static bool is_initialized = false;
  static std::map<TestPriority::eType, std::string>
    enum_string_representation;

  if (false == is_initialized) {
    enum_string_representation.insert(
      std::make_pair(TestPriority::PRIORITY_NONE, "NONE"));
    enum_string_representation.insert(
      std::make_pair(TestPriority::PRIORITY_NORMAL, "NORMAL"));
    enum_string_representation.insert(
      std::make_pair(TestPriority::PRIORITY_COMMUNICATION,
                     "COMMUNICATION"));
    enum_string_representation.insert(
      std::make_pair(TestPriority::PRIORITY_NAVIGATION, "NAVIGATION"));
    enum_string_representation.insert(
      std::make_pair(TestPriority::PRIORITY_EMERGENCY, "EMERGENCY"));

    is_initialized = true;
  }

  return enum_string_representation;
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink

int main(int argc, char** argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
