/*
 * Copyright (c) 2019, Ford Motor Company
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
 * POSSIBILITY OF SUCH DAMAGE. */

#include "vehicle_info_plugin/vehicle_data_item_schema.h"
#include <limits>
#include <set>
#include <type_traits>
#include "formatters/formatter_json_rpc.h"
#include "formatters/generic_json_formatter.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "interfaces/HMI_API.h"
#include "smart_objects/smart_object.h"

namespace vehicle_info_plugin_test {
using namespace vehicle_info_plugin;
using namespace rpc;
namespace smart_objects = ns_smart_device_link::ns_smart_objects;
namespace policy_table = rpc::policy_table_interface_base;

using namespace ns_smart_device_link::ns_json_handler::strings;
using namespace hmi_apis;

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

using ErrorCode = smart_objects::errors::eType;

namespace {
const int kInvalidValue = std::numeric_limits<int32_t>::max();
const int kDefaultInteger = 20;
const double kDefaultFloat = 10.05;
const bool kDefaultBoolean = true;
const std::string kDefaultString = "string value";
const std::string kVehicleDataKey = "key";
}  // namespace

typedef Common_ComponentVolumeStatus::eType TestEnumType;

class VehicleDataItemSchemaTest : public ::testing::Test {
 public:
  /*
   * @brief
   * Specifies whether user wants to generate valid value of SmartObject or not.
   */
  enum class SetValidItemValue { NO = 0, YES = 1 };

  /*
   * @brief
   * VehicleDataTestObject struct combines schema and data structures for test
   * object.
   */
  struct VehicleDataTestObject {
    VehicleDataTestObject(const int& key_number) {
      schema.mark_initialized();
      schema.params->mark_initialized();
      schema.mandatory = true;
      *schema.array = false;
      // default value bounds
      *schema.minvalue = 10;
      *schema.maxvalue = 100;
      // default string length
      *schema.minlength = 5;
      *schema.maxlength = 30;
      // default array length
      *schema.minsize = 5;
      *schema.maxsize = 30;
      // generate schema unique key
      schema.key = kVehicleDataKey + std::to_string(key_number);
    }

    policy_table::VehicleDataItem schema;
    smart_objects::SmartObject data;
  };

 public:
  VehicleDataItemSchemaTest() : key_number_(1) {
    test_enum_.insert(TestEnumType::CVS_UNKNOWN);
    test_enum_.insert(TestEnumType::CVS_NORMAL);
    test_enum_.insert(TestEnumType::CVS_LOW);
    test_enum_.insert(TestEnumType::CVS_FAULT);
    test_enum_.insert(TestEnumType::CVS_ALERT);
    test_enum_.insert(TestEnumType::CVS_NOT_SUPPORTED);
  }

  void assertSmartObjectSize(const size_t& expected_size,
                             const size_t& actual_size) {
    ASSERT_EQ(expected_size, actual_size);
  }

  std::vector<VehicleDataTestObject> generatePODObjects(
      const SetValidItemValue& set_valid_item) {
    std::vector<VehicleDataTestObject> generated_objects;
    VehicleDataTestObject custom_object(key_number_++);

    // Integer item
    custom_object.schema.type = policy_table::VehicleDataItem::kInteger;
    if (set_valid_item == SetValidItemValue::YES) {
      custom_object.data = kDefaultInteger;
    } else {
      custom_object.data = kInvalidValue;
    }
    generated_objects.push_back(custom_object);

    // Float item
    custom_object.schema.type = policy_table::VehicleDataItem::kFloat;
    if (set_valid_item == SetValidItemValue::YES) {
      custom_object.data = kDefaultFloat;
    } else {
      custom_object.data = kInvalidValue;
    }
    generated_objects.push_back(custom_object);

    // String item
    custom_object.schema.type = policy_table::VehicleDataItem::kString;
    if (set_valid_item == SetValidItemValue::YES) {
      custom_object.data = kDefaultString;
    } else {
      custom_object.data = kInvalidValue;
    }
    generated_objects.push_back(custom_object);

    // Boolean item
    custom_object.schema.type = policy_table::VehicleDataItem::kBoolean;
    if (set_valid_item == SetValidItemValue::YES) {
      custom_object.data = kDefaultBoolean;
    } else {
      custom_object.data = kInvalidValue;
    }
    generated_objects.push_back(custom_object);

    return generated_objects;
  }

  VehicleDataTestObject generateTestObject(
      const SetValidItemValue& set_valid_item) {
    // Enum parameter
    VehicleDataTestObject enum_object(key_number_++);
    enum_object.schema.type = "ComponentVolumeStatus";
    enum_object.data = (set_valid_item == SetValidItemValue::YES)
                           ? TestEnumType::CVS_LOW
                           : kInvalidValue;

    // Integer parameter
    VehicleDataTestObject int_object(key_number_++);
    int_object.schema.type = policy_table::VehicleDataItem::kInteger;
    int_object.data = (set_valid_item == SetValidItemValue::YES)
                          ? kDefaultInteger
                          : kInvalidValue;

    // Main object
    VehicleDataTestObject main_object(key_number_++);
    main_object.schema.type = policy_table::VehicleDataItem::kStruct;
    *main_object.schema.params = std::vector<policy_table::VehicleDataItem>{
        enum_object.schema, int_object.schema};
    main_object.data[enum_object.schema.key] = enum_object.data;
    main_object.data[int_object.schema.key] = int_object.data;

    assertSmartObjectSize(2u, main_object.data.length());
    return main_object;
  }

  VehicleDataTestObject generateRecursiveTestObject(
      const SetValidItemValue& set_valid_item) {
    // Float item
    VehicleDataTestObject float_object(key_number_++);
    float_object.schema.type = policy_table::VehicleDataItem::kFloat;
    if (set_valid_item == SetValidItemValue::YES) {
      float_object.data = kDefaultFloat;
    } else {
      float_object.data = kInvalidValue;
    }

    // String item
    VehicleDataTestObject string_object(key_number_++);
    string_object.schema.type = policy_table::VehicleDataItem::kString;
    if (set_valid_item == SetValidItemValue::YES) {
      string_object.data = kDefaultString;
    } else {
      string_object.data = kInvalidValue;
    }

    // Sub Object
    VehicleDataTestObject sub_object = generateTestObject(set_valid_item);

    // Main Object
    VehicleDataTestObject main_object(key_number_++);
    main_object.schema.type = policy_table::VehicleDataItem::kStruct;
    *main_object.schema.params = std::vector<policy_table::VehicleDataItem>{
        float_object.schema, string_object.schema, sub_object.schema};
    main_object.data[float_object.schema.key] = float_object.data;
    main_object.data[string_object.schema.key] = string_object.data;
    main_object.data[sub_object.schema.key] = sub_object.data;

    assertSmartObjectSize(3u, main_object.data.length());
    return main_object;
  }

 protected:
  std::set<TestEnumType> test_enum_;
  int key_number_;
};

TEST_F(VehicleDataItemSchemaTest, ValidatePOD_SUCCESS) {
  rpc::ValidationReport report("RPC");
  auto valid_pod_objects = generatePODObjects(SetValidItemValue::YES);

  for (auto& object : valid_pod_objects) {
    auto test_param = PolicyDataItem(object.schema);
    auto result = VehicleDataItemSchema::create(
        test_param, VehicleDataItemSchema::SchemaType::HMI);
    EXPECT_EQ(ErrorCode::OK, result->validate(object.data, &report));
    EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
  }
}

TEST_F(VehicleDataItemSchemaTest, ValidatePOD_FAIL) {
  rpc::ValidationReport report("RPC");
  std::vector<VehicleDataTestObject> valid_pod_objects =
      generatePODObjects(SetValidItemValue::NO);

  for (auto& object : valid_pod_objects) {
    auto test_param = PolicyDataItem(object.schema);
    auto result = VehicleDataItemSchema::create(
        test_param, VehicleDataItemSchema::SchemaType::MOBILE);
    EXPECT_NE(ErrorCode::OK, result->validate(object.data, &report));
    EXPECT_NE(std::string(""), rpc::PrettyFormat(report));
  }
}

TEST_F(VehicleDataItemSchemaTest, ValidateEnum_SUCCESS) {
  rpc::ValidationReport report("RPC");

  VehicleDataTestObject test_enum_object(key_number_++);
  test_enum_object.schema.type = "ComponentVolumeStatus";
  test_enum_object.data = TestEnumType::CVS_NORMAL;

  auto test_schema = PolicyDataItem(test_enum_object.schema);

  for (auto& enum_value : test_enum_) {
    test_enum_object.data = enum_value;
    auto result = VehicleDataItemSchema::create(
        test_schema, VehicleDataItemSchema::SchemaType::HMI);
    EXPECT_EQ(ErrorCode::OK, result->validate(test_enum_object.data, &report));
    EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
  }
}

TEST_F(VehicleDataItemSchemaTest, ValidateEnum_FAIL) {
  rpc::ValidationReport report("RPC");

  VehicleDataTestObject test_enum_object(key_number_++);
  test_enum_object.schema.type = "ComponentVolumeStatus";
  test_enum_object.data = kInvalidValue;

  auto test_schema = PolicyDataItem(test_enum_object.schema);
  auto result = VehicleDataItemSchema::create(
      test_schema, VehicleDataItemSchema::SchemaType::HMI);
  EXPECT_EQ(ErrorCode::OUT_OF_RANGE,
            result->validate(test_enum_object.data, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));
}

TEST_F(VehicleDataItemSchemaTest, ValidateObject_SUCCESS) {
  rpc::ValidationReport report("RPC");
  VehicleDataTestObject complex_object =
      generateTestObject(SetValidItemValue::YES);
  auto test_schema = PolicyDataItem(complex_object.schema);
  auto result = VehicleDataItemSchema::create(
      test_schema, VehicleDataItemSchema::SchemaType::HMI);
  EXPECT_EQ(ErrorCode::OK, result->validate(complex_object.data, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
}

TEST_F(VehicleDataItemSchemaTest, ValidateObject_FAIL) {
  rpc::ValidationReport report("RPC");
  VehicleDataTestObject complex_object =
      generateTestObject(SetValidItemValue::NO);
  auto test_schema = PolicyDataItem(complex_object.schema);
  auto result = VehicleDataItemSchema::create(
      test_schema, VehicleDataItemSchema::SchemaType::HMI);
  EXPECT_NE(ErrorCode::OK, result->validate(complex_object.data, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));
}

TEST_F(VehicleDataItemSchemaTest, ValidateRecursiveObject_SUCCESS) {
  rpc::ValidationReport report("RPC");
  VehicleDataTestObject complex_recursive_object =
      generateRecursiveTestObject(SetValidItemValue::YES);
  auto test_schema = PolicyDataItem(complex_recursive_object.schema);
  auto result = VehicleDataItemSchema::create(
      test_schema, VehicleDataItemSchema::SchemaType::HMI);
  EXPECT_EQ(ErrorCode::OK,
            result->validate(complex_recursive_object.data, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
}

TEST_F(VehicleDataItemSchemaTest, ValidateRecursiveObject_FAIL) {
  rpc::ValidationReport report("RPC");
  VehicleDataTestObject complex_recursive_object =
      generateRecursiveTestObject(SetValidItemValue::NO);
  auto test_schema = PolicyDataItem(complex_recursive_object.schema);
  auto result = VehicleDataItemSchema::create(
      test_schema, VehicleDataItemSchema::SchemaType::HMI);
  EXPECT_NE(ErrorCode::OK,
            result->validate(complex_recursive_object.data, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));
}

TEST_F(VehicleDataItemSchemaTest, ValidateArray_SUCCESS) {
  rpc::ValidationReport report("RPC");
  VehicleDataTestObject array_object(key_number_++);
  *array_object.schema.array = true;
  array_object.schema.type = policy_table::VehicleDataItem::kInteger;
  array_object.data =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  array_object.data.asArray()->push_back(smart_objects::SmartObject(10));
  array_object.data.asArray()->push_back(smart_objects::SmartObject(20));
  array_object.data.asArray()->push_back(smart_objects::SmartObject(30));
  array_object.data.asArray()->push_back(smart_objects::SmartObject(40));
  array_object.data.asArray()->push_back(smart_objects::SmartObject(50));
  array_object.data.asArray()->push_back(smart_objects::SmartObject(60));
  array_object.data.asArray()->push_back(smart_objects::SmartObject(70));

  auto test_schema = PolicyDataItem(array_object.schema);
  auto result = VehicleDataItemSchema::create(
      test_schema, VehicleDataItemSchema::SchemaType::HMI);
  EXPECT_EQ(ErrorCode::OK, result->validate(array_object.data, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
}

TEST_F(VehicleDataItemSchemaTest, ValidateArray_FAIL) {
  rpc::ValidationReport report("RPC");
  VehicleDataTestObject array_object(key_number_++);
  *array_object.schema.array = true;
  array_object.schema.type = policy_table::VehicleDataItem::kInteger;
  array_object.data =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  array_object.data.asArray()->push_back(smart_objects::SmartObject(10));
  array_object.data.asArray()->push_back(smart_objects::SmartObject(20));
  array_object.data.asArray()->push_back(smart_objects::SmartObject(30));

  auto test_schema = PolicyDataItem(array_object.schema);
  auto result = VehicleDataItemSchema::create(
      test_schema, VehicleDataItemSchema::SchemaType::HMI);
  EXPECT_NE(ErrorCode::OK, result->validate(array_object.data, &report));
  EXPECT_NE(std::string(""), rpc::PrettyFormat(report));
}

TEST_F(VehicleDataItemSchemaTest, ValidateUnknownType) {
  rpc::ValidationReport report("RPC");

  VehicleDataTestObject test_object_with_invalid_type(key_number_++);
  // Set invalid name of type
  test_object_with_invalid_type.schema.type = "omponentolumetatus";
  test_object_with_invalid_type.data = TestEnumType::CVS_NORMAL;

  auto test_schema = PolicyDataItem(test_object_with_invalid_type.schema);
  auto result = VehicleDataItemSchema::create(
      test_schema, VehicleDataItemSchema::SchemaType::HMI);
  test_object_with_invalid_type.data = "CVS_NORMAL";
  EXPECT_EQ(ErrorCode::OK,
            result->validate(test_object_with_invalid_type.data, &report));
  EXPECT_EQ(std::string(""), rpc::PrettyFormat(report));
}

}  // namespace vehicle_info_plugin_test
