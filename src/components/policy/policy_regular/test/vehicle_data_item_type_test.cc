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

#include "gtest/gtest.h"
#include "json/reader.h"
#include "policy/policy_table/types.h"

#include <type_traits>

namespace test {
namespace vehicle_info_plugin_test {

using namespace rpc::policy_table_interface_base;

namespace {
/*
 * class TestString should be used for compact string creation with json value
 */
class TestString {
  /**
   * @brief Field contains json structure
   */
  std::string text_;
  /**
   * @brief Need for validate should not be returned closed json or empty string
   */
  bool is_initialized_;

 public:
  TestString() : is_initialized_(false) {}

  /**
   * @brief This method handling all types except string
   * the name:value will be added to json string
   * @param std::string& name - name of json filed
   * @param const T& value - value of json filed
   */
  template <class T>
  void AddField(const std::string& name, const T& value) {
    const bool is_bool = std::is_same<T, bool>::value;
    // Should be writed to json true/false not 1/0
    const std::string text_value =
        (is_bool ? (value ? "true" : "false") : std::to_string(value));
    if (text_.empty()) {
      is_initialized_ = true;
      text_ = "{\"" + name + "\":" + text_value;
      return;
    }
    text_ += ",\"" + name + "\":" + text_value;
  }

  /**
   * @brief This method handling only string values
   * the name:"value" will be added to json string
   * this method is coverring value in quotes
   * @param std::string& name - name of json filed
   * @param const T& value - value of json filed
   */
  void AddField(const std::string& name, const char* value) {
    if (text_.empty()) {
      is_initialized_ = true;
      text_ = "{\"" + name + "\":";
    } else {
      text_ += ",\"" + name + "\":";
    }

    text_ += value[0] == '{' || value[0] == '['
                 ? std::string(value)
                 : "\"" + std::string(value) + "\"";
  }

  /**
   * @brief This method close json string before return
   * @return all string and close json form by right bracket "}"
   * or empty string if json structure is empty
   */
  std::string GetValue() {
    return is_initialized_ ? text_ + "}" : "";
  }
};

}  // namespace

class VehicleDataItemTypeTest : public ::testing::Test {
 public:
  std::string GetFullJsonString(const std::string& params_array = "") {
    TestString str;
    str.AddField("name", "VehicleDataItem");
    str.AddField("type", "String");
    str.AddField("key", "OEMDataRef");
    str.AddField("array", true);
    str.AddField("mandatory", true);
    str.AddField("params", ("[" + params_array + "]").c_str());
    str.AddField("since", "4.0");
    str.AddField("until", "5.0");
    str.AddField("removed", true);
    str.AddField("deprecated", true);
    str.AddField("minvalue", 1);
    str.AddField("maxvalue", 2);
    str.AddField("minsize", 10);
    str.AddField("maxsize", 20);
    str.AddField("minlength", 100);
    str.AddField("maxlength", 200);

    return str.GetValue();
  }

 protected:
  virtual void TearDown() {
    json_.clear();
  }

  Json::Value json_;
  Json::Reader reader_;
};

TEST_F(VehicleDataItemTypeTest, Initialize_Success) {
  reader_.parse(GetFullJsonString(), json_);
  VehicleDataItem vdi(&json_);

  EXPECT_TRUE(vdi.is_initialized());
}

TEST_F(VehicleDataItemTypeTest, Initialize_Failed) {
  VehicleDataItem vdi;

  EXPECT_FALSE(vdi.is_initialized());
}

TEST_F(VehicleDataItemTypeTest, CheckConvertFromJsonToVehicleDataItem_Success) {
  reader_.parse(GetFullJsonString(), json_);
  VehicleDataItem vdi(&json_);

  EXPECT_TRUE((std::string)vdi.name == "VehicleDataItem");
  EXPECT_TRUE((std::string)vdi.type == "String");
  EXPECT_TRUE((std::string)vdi.key == "OEMDataRef");
  EXPECT_TRUE(*vdi.array == true);
  EXPECT_TRUE(vdi.mandatory == true);
  EXPECT_TRUE(vdi.params->empty());
  EXPECT_TRUE((std::string)*vdi.since == "4.0");
  EXPECT_TRUE((std::string)*vdi.until == "5.0");
  EXPECT_TRUE(*vdi.removed == true);
  EXPECT_TRUE(*vdi.deprecated == true);
  EXPECT_TRUE(*vdi.minvalue == 1);
  EXPECT_TRUE(*vdi.maxvalue == 2);
  EXPECT_TRUE(*vdi.minsize == 10);
  EXPECT_TRUE(*vdi.maxsize == 20);
  EXPECT_TRUE(*vdi.minlength == 100);
  EXPECT_TRUE(*vdi.maxlength == 200);
}

TEST_F(VehicleDataItemTypeTest, CheckConvertFromVehicleDataItemToJson_Success) {
  reader_.parse(GetFullJsonString(), json_);
  VehicleDataItem vdi(&json_);

  auto jsonFrom = vdi.ToJsonValue();
  VehicleDataItem vdi2(&jsonFrom);
  EXPECT_TRUE(vdi == vdi2);
}

TEST_F(VehicleDataItemTypeTest, CheckIsValid_Failed) {
  VehicleDataItem vdi;

  EXPECT_FALSE(vdi.is_valid());
}

TEST_F(VehicleDataItemTypeTest, CheckIsValid_Struct_Success) {
  reader_.parse(GetFullJsonString(GetFullJsonString()), json_);
  VehicleDataItem vdi(&json_);

  vdi.type = "Struct";
  EXPECT_TRUE(vdi.is_valid());
}

TEST_F(VehicleDataItemTypeTest, CheckIsValid_Struct_EmptyParams_Failed) {
  reader_.parse(GetFullJsonString(), json_);
  VehicleDataItem vdi(&json_);

  vdi.type = "Struct";
  EXPECT_FALSE(vdi.is_valid());
}

TEST_F(VehicleDataItemTypeTest, CheckIsValid_PODTypes_Success) {
  reader_.parse(GetFullJsonString(), json_);
  VehicleDataItem vdi(&json_);

  vdi.type = "Integer";
  EXPECT_TRUE(vdi.is_valid());

  vdi.type = "Float";
  EXPECT_TRUE(vdi.is_valid());

  vdi.type = "String";
  EXPECT_TRUE(vdi.is_valid());

  vdi.type = "Boolean";
  EXPECT_TRUE(vdi.is_valid());
}

TEST_F(VehicleDataItemTypeTest, CheckEmptiness_True) {
  VehicleDataItem vdi;
  EXPECT_FALSE(vdi.struct_not_empty());
}

TEST_F(VehicleDataItemTypeTest, CheckEmptiness_False) {
  reader_.parse(GetFullJsonString(), json_);

  VehicleDataItem vdi(&json_);
  EXPECT_TRUE(vdi.struct_not_empty());
}

TEST_F(VehicleDataItemTypeTest, CheckCopyConstructor) {
  reader_.parse(GetFullJsonString(), json_);

  VehicleDataItem vdi1(&json_);
  VehicleDataItem vdi2(vdi1);

  EXPECT_TRUE(vdi1 == vdi2);
}

}  // namespace vehicle_info_plugin_test
}  // namespace test
