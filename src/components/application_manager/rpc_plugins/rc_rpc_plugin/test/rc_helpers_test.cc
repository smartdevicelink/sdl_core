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

#include "rc_rpc_plugin/rc_helpers.h"
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>
#include "gtest/gtest.h"
#include "rc_rpc_plugin/rc_module_constants.h"

namespace {
const std::string kKeyId = "id";
const std::string kKeyStatus = "status";
const std::string kKeyDensity = "density";
const std::string kKeyValue = "value";
const std::string kArray = "array";
}  // namespace

namespace rc_rpc_plugin_test {

using namespace rc_rpc_plugin;
using namespace smart_objects;

class RCHelpersTest : public ::testing::Test {
 public:
  /**
   * @brief Generates smart object with next structure:
   * {
   *    "status": "status<n>",
   *    "value": "value<n>",
   *    "id": "id<n>"
   * }
   * where <n> is number specified with 'object_number' param.
   *
   * @param int object_number - number of object
   * @param bool use_id_key - defines, whether "id" key must be added to smart
   * object
   *
   * @return smart_objects::SmartObject - generated smart object
   */
  smart_objects::SmartObject gen_smart_object(int object_number,
                                              bool use_id_key) {
    smart_objects::SmartObject result(smart_objects::SmartType_Map);
    result[kKeyStatus] = kKeyStatus + std::to_string(object_number);
    result[kKeyValue] = kKeyValue + std::to_string(object_number);
    if (use_id_key) {
      result[kKeyId] = kKeyId + std::to_string(object_number);
    }
    return result;
  }
};

TEST_F(RCHelpersTest,
       MergeModuleData_OneOfParamsNotSmartMap_ExpectDataReplacedWithNewOne) {
  SmartObject data1(SmartType_Map);
  SmartObject data2(SmartType_Array);

  const auto result = RCHelpers::MergeModuleData(data1, data2);
  EXPECT_NE(result, data1);
  EXPECT_EQ(result, data2);
}

TEST_F(RCHelpersTest,
       MergeModuleData_FirstObjectDontContainKey_ExpectDataAppendedWithNewOne) {
  SmartObject data1(SmartType_Map);
  SmartObject data2(SmartType_Map);

  std::map<std::string, std::string> expected_keys_values = {
      {"key1", "value1"}, {"key2", "value2"}};
  auto it = expected_keys_values.begin();

  data1[it->first] = it->second;
  ++it;
  data2[it->first] = it->second;

  const auto result = RCHelpers::MergeModuleData(data1, data2);
  for (auto& item : expected_keys_values) {
    EXPECT_TRUE(result.keyExists(item.first));
    EXPECT_EQ(result[item.first], expected_keys_values[item.first]);
  }
}

TEST_F(
    RCHelpersTest,
    MergeModuleData_SecondObjectContainsDifferentValueType_ExpectDataReplacedWithNewOne) {
  SmartObject data1(SmartType_Map);
  SmartObject data2(SmartType_Map);

  data1["key1"] = "value1";
  data2["key1"] = 30;

  const auto result = RCHelpers::MergeModuleData(data1, data2);
  EXPECT_EQ(data2["key1"], result["key1"]);
}

TEST_F(RCHelpersTest, MergeModuleData_Recursive) {
  SmartObject data1(SmartType_Map);
  SmartObject data2(SmartType_Map);

  data1["key1"]["subkey1"] = "subvalue1";
  data2["key1"]["subkey2"] = 30;

  const auto result = RCHelpers::MergeModuleData(data1, data2);
  EXPECT_EQ(result["key1"]["subkey1"], data1["key1"]["subkey1"]);
  EXPECT_EQ(result["key1"]["subkey2"], data2["key1"]["subkey2"]);
}

TEST_F(
    RCHelpersTest,
    MergeModuleData_MergeObjectsThatContainArrays_ExpectDataReplacedWithNewOne) {
  SmartObject data1(SmartType_Map);
  SmartObject data1_array(SmartType_Array);
  (*data1_array.asArray()) =
      SmartArray{gen_smart_object(1, true), gen_smart_object(2, false)};
  data1[kArray] = data1_array;

  SmartObject data2(SmartType_Map);
  SmartObject data2_array(SmartType_Array);
  (*data2_array.asArray()) =
      SmartArray{gen_smart_object(3, true), gen_smart_object(4, false)};
  data2[kArray] = data2_array;

  SmartObject expected_result(SmartType_Map);
  SmartObject expected_result_array(SmartType_Array);
  (*expected_result_array.asArray()) =
      SmartArray{data2[kArray].getElement(0), data2[kArray].getElement(1)};

  expected_result[kArray] = expected_result_array;

  const auto result = RCHelpers::MergeModuleData(data1, data2);
  EXPECT_EQ(expected_result, result);
}

TEST_F(RCHelpersTest,
       MergeArray_OneOfParamsNotSmartArray_ExpectDataReplacedWithSecond) {
  SmartObject data1(SmartType_Array);
  SmartObject data2(SmartType_Map);

  const auto result = RCHelpers::MergeArray(data1, data2);
  EXPECT_NE(result, data1);
  EXPECT_EQ(result, data2);
}

TEST_F(RCHelpersTest, MergeArray_FirstArrayEmpty_ExpectDataReplacedWithSecond) {
  SmartObject data1(SmartType_Array);
  SmartObject data2(SmartType_Array);
  for (int object : {0, 1}) {
    data2.asArray()->emplace(data2.asArray()->begin(), SmartObject(object));
  }

  ASSERT_EQ(0u, data1.length());
  ASSERT_EQ(2u, data2.length());

  const auto result = RCHelpers::MergeArray(data1, data2);
  EXPECT_NE(result, data1);
  EXPECT_EQ(result, data2);
}

TEST_F(RCHelpersTest,
       MergeArray_SecondArrayEmpty_ExpectDataReplacedWithSecond) {
  SmartObject data1(SmartType_Array);
  for (int object : {0, 1}) {
    data1.asArray()->emplace(data1.asArray()->begin(), SmartObject(object));
  }
  SmartObject data2(SmartType_Array);

  ASSERT_EQ(2u, data1.length());
  ASSERT_EQ(0u, data2.length());

  const auto result = RCHelpers::MergeArray(data1, data2);
  EXPECT_NE(result, data1);
  EXPECT_EQ(result, data2);
}

TEST_F(RCHelpersTest, MergeArray_SimpleArrays_ExpectDataReplacedWithSecond) {
  SmartObject data1(SmartType_Array);
  for (int object : {0, 1, 2, 3, 4, 5}) {
    data1.asArray()->emplace(data1.asArray()->begin(), SmartObject(object));
  }

  SmartObject data2(SmartType_Array);
  for (int object : {0, 1}) {
    data2.asArray()->emplace(data2.asArray()->begin(), SmartObject(object));
  }

  ASSERT_EQ(6u, data1.length());
  ASSERT_EQ(2u, data2.length());

  const auto result = RCHelpers::MergeArray(data1, data2);
  EXPECT_NE(result, data1);
  EXPECT_EQ(result, data2);
}

TEST_F(RCHelpersTest,
       MergeArray_ObjectsHaveNotEqualIds_ExpectDataAppendedWithNewOne) {
  SmartObject data1 = gen_smart_object(1, true);
  SmartObject data2 = gen_smart_object(2, true);

  SmartObject array1(SmartType_Array);
  array1.asArray()->push_back(data1);
  SmartObject array2(SmartType_Array);
  array2.asArray()->push_back(data2);

  SmartObject expected_array(SmartType_Array);
  expected_array.asArray()->push_back(data1);
  expected_array.asArray()->push_back(data2);
  ASSERT_EQ(2u, expected_array.length());

  const auto result = RCHelpers::MergeArray(array1, array2);
  EXPECT_EQ(expected_array, result);
}

TEST_F(RCHelpersTest,
       MergeArray_SomeObjectsDontHaveIdKey_ExpectDataReplacedWithSecond) {
  int object_number = 1;

  SmartObject array1(SmartType_Array);
  array1.asArray()->push_back(gen_smart_object(object_number++, true));

  SmartObject array2(SmartType_Array);
  array2.asArray()->push_back(gen_smart_object(object_number++, false));
  array2.asArray()->push_back(gen_smart_object(object_number++, true));

  const auto result = RCHelpers::MergeArray(array1, array2);
  EXPECT_EQ(array2, result);
}

TEST_F(
    RCHelpersTest,
    MergeArray_SomeObjectsHaveEqualIds_ExpectDataWithSameIdUpdateValueOnlyNewDataAdded) {
  SmartObject temp = gen_smart_object(1, true);

  SmartObject data1(SmartType_Array);
  auto& data1_array = *data1.asArray();

  data1_array.push_back(temp);
  data1_array.push_back(gen_smart_object(2, true));

  SmartObject data2(SmartType_Array);
  auto& data2_array = *data2.asArray();

  temp[kKeyValue] = "CUSTOM VALUE";
  data2_array.push_back(temp);
  data2_array.push_back(gen_smart_object(3, true));

  SmartObject expected_result(SmartType_Array);
  expected_result.asArray()->push_back(temp);
  expected_result.asArray()->push_back(data1_array[1]);
  expected_result.asArray()->push_back(data2_array[1]);

  const auto result = RCHelpers::MergeArray(data1, data2);
  EXPECT_EQ(expected_result, result);
}

}  // namespace rc_rpc_plugin_test
