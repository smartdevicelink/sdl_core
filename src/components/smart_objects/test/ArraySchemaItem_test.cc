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
#include "smart_objects/array_schema_item.h"
#include "smart_objects/enum_schema_item.h"
#include "smart_objects/object_schema_item.h"
#include "smart_objects/smart_object.h"
#include "smart_objects/string_schema_item.h"

namespace test {
namespace components {
namespace smart_object_test {

namespace ExampleEnum {
enum eType { INVALID_ENUM = -1, Value0 = 0, Value1, Value2 };
}  // namespace ExampleEnum

namespace Keys {
const char OPTIONAL_PARAM[] = "optionalParam";
const char MANDATORY_PARAM[] = "mandatoryParam";
}  // namespace Keys

/**
 * Test ArraySchemaItem no schema item, no min and max size
 **/
TEST(test_no_default_value, test_ArraySchemaItemTest) {
  using namespace ns_smart_device_link::ns_smart_objects;
  SmartObject obj;

  ISchemaItemPtr item =
      CArraySchemaItem::create();  // No schema item, no min and max size

  obj[0] = 38;
  obj[1] = true;
  obj[2] = "New String";
  obj[3][0] = 39;
  obj[3][1] = false;
  obj[3][2] = "Another String";

  EXPECT_EQ(38, obj[0].asInt());
  EXPECT_TRUE(obj[1].asBool());
  EXPECT_EQ(std::string("New String"), obj[2].asString());
  EXPECT_EQ(39, obj[3][0].asInt());
  EXPECT_FALSE(obj[3][1].asBool());
  EXPECT_EQ(std::string("Another String"), obj[3][2].asString());

  rpc::ValidationReport report("RPC");
  int resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OK, resultType);
  resultType = item->validate(obj[0], &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[1], &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[2], &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[3], &report);
  EXPECT_EQ(errors::OK, resultType);

  item->applySchema(obj, false);

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OK, resultType);
  resultType = item->validate(obj[0], &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[1], &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[2], &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[3], &report);
  EXPECT_EQ(errors::OK, resultType);

  EXPECT_EQ(38, obj[0].asInt());
  EXPECT_TRUE(obj[1].asBool());
  EXPECT_EQ(std::string("New String"), obj[2].asString());
  EXPECT_EQ(39, obj[3][0].asInt());
  EXPECT_FALSE(obj[3][1].asBool());
  EXPECT_EQ(std::string("Another String"), obj[3][2].asString());

  // Object - valid string
  obj = "New valid string";
  ASSERT_EQ(std::string("New valid string"), obj.asString());

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);

  // Obj - bool
  obj = true;

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);

  // Object - number
  obj = 3.1415926;

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);
}

/**
 * Test ArraySchemaItem with schema item
 *
 * Create ArraySchemaItem with schema item. Method validate should return true
 * only if all array elements are valid schema item objects
 **/
TEST(test_item_with_default_value, test_ArraySchemaItemTest) {
  using namespace ns_smart_device_link::ns_smart_objects;
  SmartObject obj;

  ISchemaItemPtr item = CArraySchemaItem::create();  // No min and max size

  obj[0] = "Some String";
  obj[1] = "true";
  obj[2] = "New String";

  EXPECT_EQ(std::string("Some String"), obj[0].asString());
  EXPECT_EQ(std::string("true"), obj[1].asString());
  EXPECT_EQ(std::string("New String"), obj[2].asString());

  rpc::ValidationReport report("RPC");
  int resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OK, resultType);
  resultType = item->validate(obj[0], &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[1], &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[2], &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);

  item->applySchema(obj, false);

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OK, resultType);
  resultType = item->validate(obj[0], &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[1], &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[2], &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);

  EXPECT_EQ(std::string("Some String"), obj[0].asString());
  EXPECT_EQ(std::string("true"), obj[1].asString());
  EXPECT_EQ(std::string("New String"), obj[2].asString());

  obj[3][0] = "39";
  obj[3][1] = "false";
  obj[3][2] = "Another String";

  EXPECT_EQ(std::string("39"), obj[3][0].asString());
  EXPECT_EQ(std::string("false"), obj[3][1].asString());
  EXPECT_EQ(std::string("Another String"), obj[3][2].asString());

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OK, resultType);
  resultType = item->validate(obj[3], &report);
  EXPECT_EQ(errors::OK, resultType);

  obj[3][3] = "Another very very loooooong String";

  resultType = item->validate(obj[3], &report);
  EXPECT_EQ(errors::OK, resultType);
}

/**
 * Test ArraySchemaItem with min size
 **/
TEST(test_array_with_min_size, test_ArraySchemaItemTest) {
  using namespace ns_smart_device_link::ns_smart_objects;
  SmartObject obj;

  ISchemaItemPtr item = CArraySchemaItem::create(
      CStringSchemaItem::create(TSchemaItemParameter<size_t>(25)),
      TSchemaItemParameter<size_t>(3));

  obj[0] = "Some String";

  rpc::ValidationReport report("RPC");
  int resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OUT_OF_RANGE, resultType);

  obj[1] = "true";

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OUT_OF_RANGE, resultType);

  obj[2] = "New String";

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OUT_OF_RANGE, resultType);

  EXPECT_EQ(std::string("Some String"), obj[0].asString());
  EXPECT_EQ(std::string("true"), obj[1].asString());
  EXPECT_EQ(std::string("New String"), obj[2].asString());
}

/**
 * Test ArraySchemaItem with min size
 **/
TEST(test_array_with_max_size, test_ArraySchemaItemTest) {
  using namespace ns_smart_device_link::ns_smart_objects;
  SmartObject obj;

  ISchemaItemPtr item = CArraySchemaItem::create(
      CStringSchemaItem::create(TSchemaItemParameter<size_t>(),
                                TSchemaItemParameter<size_t>(25)),
      TSchemaItemParameter<size_t>(),
      TSchemaItemParameter<size_t>(3));  // No min size

  obj[0] = "Some String";

  rpc::ValidationReport report("RPC");
  int resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OK, resultType);

  obj[1] = "true";

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OK, resultType);

  obj[2] = "New String";

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OK, resultType);

  obj[3] = "Another String";

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OUT_OF_RANGE, resultType);

  EXPECT_EQ(std::string("Some String"), obj[0].asString());
  EXPECT_EQ(std::string("true"), obj[1].asString());
  EXPECT_EQ(std::string("New String"), obj[2].asString());
  EXPECT_EQ(std::string("Another String"), obj[3].asString());
}

/**
 * Test ArraySchemaItem with min and max size
 **/
TEST(test_array_with_min_and_max_size, test_ArraySchemaItemTest) {
  using namespace ns_smart_device_link::ns_smart_objects;
  SmartObject obj;

  ISchemaItemPtr item = CArraySchemaItem::create(
      CStringSchemaItem::create(TSchemaItemParameter<size_t>(),
                                TSchemaItemParameter<size_t>(25)),
      TSchemaItemParameter<size_t>(2),
      TSchemaItemParameter<size_t>(4));

  obj[0] = "Some String";

  rpc::ValidationReport report("RPC");
  int resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OUT_OF_RANGE, resultType);

  obj[1] = "true";

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OK, resultType);

  obj[2] = "New String";

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OK, resultType);

  obj[3] = "Another String";

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OK, resultType);

  obj[4] = "Out of array";

  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::OUT_OF_RANGE, resultType);

  EXPECT_EQ(std::string("Some String"), obj[0].asString());
  EXPECT_EQ(std::string("true"), obj[1].asString());
  EXPECT_EQ(std::string("New String"), obj[2].asString());
  EXPECT_EQ(std::string("Another String"), obj[3].asString());
  EXPECT_EQ(std::string("Out of array"), obj[4].asString());
}

TEST(test_map_validate, test_ArraySchemaItemTest) {
  using namespace ns_smart_device_link::ns_smart_objects;
  SmartObject obj;

  ISchemaItemPtr item = CArraySchemaItem::create(
      CStringSchemaItem::create(TSchemaItemParameter<size_t>(),
                                TSchemaItemParameter<size_t>(25)),
      TSchemaItemParameter<size_t>(2),
      TSchemaItemParameter<size_t>(4));

  obj["array"][0] = "Some String";

  rpc::ValidationReport report("RPC");
  int resultType = item->validate(obj["array"], &report);
  EXPECT_EQ(errors::OUT_OF_RANGE, resultType);

  obj["array"][1] = "true";

  resultType = item->validate(obj["array"], &report);
  EXPECT_EQ(errors::OK, resultType);

  obj["array"][2] = "New String";

  resultType = item->validate(obj["array"], &report);
  EXPECT_EQ(errors::OK, resultType);

  obj["array"][3] = "Another String";

  resultType = item->validate(obj["array"], &report);
  EXPECT_EQ(errors::OK, resultType);

  obj["array"][4] = "Out of array";

  resultType = item->validate(obj["array"], &report);
  EXPECT_EQ(errors::OUT_OF_RANGE, resultType);
  resultType = item->validate(obj, &report);
  EXPECT_EQ(errors::INVALID_VALUE, resultType);

  EXPECT_EQ(std::string("Some String"), obj["array"][0].asString());
  EXPECT_EQ(std::string("true"), obj["array"][1].asString());
  EXPECT_EQ(std::string("New String"), obj["array"][2].asString());
  EXPECT_EQ(std::string("Another String"), obj["array"][3].asString());
  EXPECT_EQ(std::string("Out of array"), obj["array"][4].asString());
}

TEST(test_array_with_unknown_enum, test_ArraySchemaItemTest) {
  using namespace ns_smart_device_link::ns_smart_objects;
  SmartObject obj;
  std::set<ExampleEnum::eType> enum_values;
  enum_values.insert(ExampleEnum::Value0);
  enum_values.insert(ExampleEnum::Value1);
  enum_values.insert(ExampleEnum::Value2);

  ISchemaItemPtr item = CArraySchemaItem::create(
      TEnumSchemaItem<ExampleEnum::eType>::create(enum_values),
      TSchemaItemParameter<size_t>(1));

  obj[0] = "Value0";
  obj[1] = "Value10";
  obj[2] = "Value1";

  item->applySchema(obj, true);

  rpc::ValidationReport report("RPC");
  EXPECT_FALSE(
      item->filterInvalidEnums(obj, utils::SemanticVersion(), &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  EXPECT_EQ((size_t)2, obj.length());
  EXPECT_EQ(0, obj[0].asInt());
  EXPECT_EQ(1, obj[1].asInt());

  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::OK, item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
}

TEST(test_array_with_unknown_enums, test_ArraySchemaItemTest) {
  using namespace ns_smart_device_link::ns_smart_objects;
  SmartObject obj;
  std::set<ExampleEnum::eType> enum_values;
  enum_values.insert(ExampleEnum::Value0);
  enum_values.insert(ExampleEnum::Value1);
  enum_values.insert(ExampleEnum::Value2);

  ISchemaItemPtr item = CArraySchemaItem::create(
      TEnumSchemaItem<ExampleEnum::eType>::create(enum_values),
      TSchemaItemParameter<size_t>(1));

  obj[0] = "Value10";
  obj[1] = "Value11";
  obj[2] = "Value12";

  item->applySchema(obj, true);

  rpc::ValidationReport report("RPC");
  EXPECT_TRUE(item->filterInvalidEnums(obj, utils::SemanticVersion(), &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  EXPECT_EQ((size_t)0, obj.length());

  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::OUT_OF_RANGE, item->validate(obj, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));
}

TEST(test_array_of_objects_with_unknown_enum, test_ArraySchemaItemTest) {
  using namespace ns_smart_device_link::ns_smart_objects;
  SmartObject obj;
  std::set<ExampleEnum::eType> enum_values;
  enum_values.insert(ExampleEnum::Value0);
  enum_values.insert(ExampleEnum::Value1);
  enum_values.insert(ExampleEnum::Value2);

  Members structMembersMap;
  structMembersMap[Keys::OPTIONAL_PARAM] =
      SMember(TEnumSchemaItem<ExampleEnum::eType>::create(enum_values), false);
  structMembersMap[Keys::MANDATORY_PARAM] =
      SMember(TEnumSchemaItem<ExampleEnum::eType>::create(enum_values), true);

  ISchemaItemPtr item =
      CArraySchemaItem::create(CObjectSchemaItem::create(structMembersMap),
                               TSchemaItemParameter<size_t>(1));

  SmartObject struct1;
  struct1[Keys::MANDATORY_PARAM] = "Value0";
  SmartObject struct2;
  struct2[Keys::MANDATORY_PARAM] = "Value11";
  SmartObject struct3;
  struct3[Keys::MANDATORY_PARAM] = "Value2";
  obj[0] = struct1;
  obj[1] = struct2;
  obj[2] = struct3;

  item->applySchema(obj, true);

  rpc::ValidationReport report("RPC");
  EXPECT_FALSE(
      item->filterInvalidEnums(obj, utils::SemanticVersion(), &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  EXPECT_EQ((size_t)2, obj.length());
  EXPECT_EQ(0, obj[0][Keys::MANDATORY_PARAM].asInt());
  EXPECT_EQ(2, obj[1][Keys::MANDATORY_PARAM].asInt());

  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::OK, item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
}

TEST(test_array_of_objects_with_unknown_optional_enums,
     test_ArraySchemaItemTest) {
  using namespace ns_smart_device_link::ns_smart_objects;
  SmartObject obj;
  std::set<ExampleEnum::eType> enum_values;
  enum_values.insert(ExampleEnum::Value0);
  enum_values.insert(ExampleEnum::Value1);
  enum_values.insert(ExampleEnum::Value2);

  Members structMembersMap;
  structMembersMap[Keys::OPTIONAL_PARAM] =
      SMember(TEnumSchemaItem<ExampleEnum::eType>::create(enum_values), false);
  structMembersMap[Keys::MANDATORY_PARAM] =
      SMember(TEnumSchemaItem<ExampleEnum::eType>::create(enum_values), true);

  ISchemaItemPtr item =
      CArraySchemaItem::create(CObjectSchemaItem::create(structMembersMap),
                               TSchemaItemParameter<size_t>(1));

  SmartObject struct1;
  struct1[Keys::OPTIONAL_PARAM] = "Value10";
  struct1[Keys::MANDATORY_PARAM] = "Value0";
  SmartObject struct2;
  struct2[Keys::OPTIONAL_PARAM] = "Value0";
  struct2[Keys::MANDATORY_PARAM] = "Value1";
  SmartObject struct3;
  struct3[Keys::OPTIONAL_PARAM] = "Value12";
  struct3[Keys::MANDATORY_PARAM] = "Value2";
  obj[0] = struct1;
  obj[1] = struct2;
  obj[2] = struct3;

  item->applySchema(obj, true);

  rpc::ValidationReport report("RPC");
  EXPECT_FALSE(
      item->filterInvalidEnums(obj, utils::SemanticVersion(), &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));

  // Unknown sub-parameter values should be filtered, but not array elements
  EXPECT_EQ((size_t)3, obj.length());
  EXPECT_EQ(0, obj[0][Keys::MANDATORY_PARAM].asInt());
  EXPECT_FALSE(obj[0].keyExists(Keys::OPTIONAL_PARAM));
  EXPECT_EQ(1, obj[1][Keys::MANDATORY_PARAM].asInt());
  EXPECT_EQ(0, obj[1][Keys::OPTIONAL_PARAM].asInt());
  EXPECT_EQ(2, obj[2][Keys::MANDATORY_PARAM].asInt());
  EXPECT_FALSE(obj[2].keyExists(Keys::OPTIONAL_PARAM));

  report = rpc::ValidationReport("RPC");
  EXPECT_EQ(errors::OK, item->validate(obj, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
}

}  // namespace smart_object_test
}  // namespace components
}  // namespace test

namespace ns_smart_device_link {
namespace ns_smart_objects {

namespace ExampleEnum = test::components::smart_object_test::ExampleEnum;
typedef EnumConversionHelper<ExampleEnum::eType> EnumConverter;

template <>
const EnumConverter::EnumToCStringMap EnumConverter::enum_to_cstring_map_ =
    EnumConverter::InitEnumToCStringMap();

template <>
const EnumConverter::CStringToEnumMap EnumConverter::cstring_to_enum_map_ =
    EnumConverter::InitCStringToEnumMap();

template <>
const char* const EnumConverter::cstring_values_[] = {
    "Value0", "Value1", "Value2"};

template <>
const ExampleEnum::eType EnumConverter::enum_values_[] = {
    ExampleEnum::Value0, ExampleEnum::Value1, ExampleEnum::Value2};

}  // namespace ns_smart_objects
}  // namespace ns_smart_device_link
