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
#include "smart_objects/string_schema_item.h"

namespace test {
namespace components {
namespace SmartObjects {
namespace SchemaItem {

/**
 * Test StringSchemaItem no default value
 *
 * Create SchemaItem without default value. Method setDefaultValue should always return false
 * and leave SmartObject in previous state.
 **/
TEST(test_no_default_value, test_StringSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject obj;

  ISchemaItemPtr item = CStringSchemaItem::create();  // No default value, no max length

  //Object - valid string
  obj = "New valid string";
  ASSERT_EQ(std::string("New valid string"), obj.asString());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
  bool resDefault = item->setDefaultValue(obj);
  EXPECT_FALSE(resDefault);
  EXPECT_EQ(std::string("New valid string"), obj.asString());

  //Obj - bool
  obj = true;

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resDefault = item->setDefaultValue(obj);
  EXPECT_FALSE(resDefault);
  EXPECT_TRUE(obj.asBool());

  //Object - number
  obj = 3.1415926;

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  resDefault = item->setDefaultValue(obj);
  EXPECT_FALSE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  EXPECT_EQ(3.1415926, obj.asDouble());
}

/**
 * Test StringSchemaItem with default value
 *
 * Create SchemaItem with default value. Method setDefaultValue should return true,
 * String SmartObject should contain default value.
 * Not string SmartObject should converted to StringObject and setted up by the default value.
 **/
TEST(test_item_with_default_value, test_StringSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject obj;

  ISchemaItemPtr item = CStringSchemaItem::create(
      TSchemaItemParameter<size_t>(), TSchemaItemParameter<size_t>(),
      TSchemaItemParameter<std::string>("Default string"));  // Default value, no max length

  //Object - valid string
  obj = "New valid string";
  ASSERT_EQ(std::string("New valid string"), obj.asString());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
  bool resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(std::string("Default string"), obj.asString());

  //Obj - bool
  obj = true;

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(std::string("Default string"), obj.asString());

  //Object - number
  obj = 3.1415926;

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
  EXPECT_EQ(std::string("Default string"), obj.asString());
}

/**
 * Test StringSchemaItem with max length
 **/
TEST(test_item_with_max_length, test_StringSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject obj;

  ISchemaItemPtr item = CStringSchemaItem::create(
      TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(25),
      TSchemaItemParameter<std::string>("Default string"));

  //Object - valid string
  obj = "New valid string";
  ASSERT_EQ(std::string("New valid string"), obj.asString());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
  bool resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(std::string("Default string"), obj.asString());

  //Object - too long string
  obj = "New very very loooooong string";
  ASSERT_EQ(std::string("New very very loooooong string"), obj.asString());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(std::string("Default string"), obj.asString());
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
}

TEST(test_map_validate, test_StringSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject obj;

  ISchemaItemPtr item = CStringSchemaItem::create(
      TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(25),
      TSchemaItemParameter<std::string>("Default string"));

  obj["str"] = "New valid string";
  obj["long"] = "New very very loooooong string";
  obj["bool"] = true;
  obj["num"] = 3.14;

  int resultType = item->validate(obj["str"]);
  EXPECT_EQ(Errors::OK, resultType);

  resultType = item->validate(obj["long"]);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  resultType = item->validate(obj["bool"]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  resultType = item->validate(obj["num"]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  bool resDefault = item->setDefaultValue(obj["str"]);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(std::string("Default string"), obj["str"].asString());

  resDefault = item->setDefaultValue(obj["bool"]);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(std::string("Default string"), obj["bool"].asString());

  resDefault = item->setDefaultValue(obj["num"]);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(std::string("Default string"), obj["num"].asString());

  resultType = item->validate(obj["str"]);
  EXPECT_EQ(Errors::OK, resultType);

  resultType = item->validate(obj["long"]);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  resultType = item->validate(obj["bool"]);
  EXPECT_EQ(Errors::OK, resultType);

  resultType = item->validate(obj["num"]);
  EXPECT_EQ(Errors::OK, resultType);

  resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(std::string("Default string"), obj.asString());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
}

TEST(test_array_validate, test_StringSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject obj;

  ISchemaItemPtr item = CStringSchemaItem::create(
      TSchemaItemParameter<size_t>(0), TSchemaItemParameter<size_t>(25),
      TSchemaItemParameter<std::string>("Default string"));

  obj[0] = "New valid string";
  obj[1] = "New very very loooooong string";
  obj[2] = true;
  obj[3] = 3.14;
  obj[4] = "New valid string";

  int resultType = item->validate(obj[0]);
  EXPECT_EQ(Errors::OK, resultType);

  resultType = item->validate(obj[1]);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  resultType = item->validate(obj[2]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  resultType = item->validate(obj[3]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  resultType = item->validate(obj[4]);
  EXPECT_EQ(Errors::OK, resultType);

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  bool resDefault = item->setDefaultValue(obj[0]);
  EXPECT_TRUE(resDefault);
  resDefault = item->setDefaultValue(obj[2]);
  EXPECT_TRUE(resDefault);
  resDefault = item->setDefaultValue(obj[4]);
  EXPECT_TRUE(resDefault);

  //Set default value for non-initialized element
  resDefault = item->setDefaultValue(obj[5]);
  EXPECT_TRUE(resDefault);

  EXPECT_EQ(std::string("Default string"), obj[0].asString());
  EXPECT_EQ(std::string("Default string"), obj[2].asString());
  EXPECT_EQ(std::string("Default string"), obj[4].asString());
  EXPECT_EQ(std::string("Default string"), obj[5].asString());

  resultType = item->validate(obj[0]);
  EXPECT_EQ(Errors::OK, resultType);
  resultType = item->validate(obj[1]);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
  resultType = item->validate(obj[2]);
  EXPECT_EQ(Errors::OK, resultType);
  resultType = item->validate(obj[3]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[4]);
  EXPECT_EQ(Errors::OK, resultType);
  resultType = item->validate(obj[5]);
  EXPECT_EQ(Errors::OK, resultType);

  resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(std::string("Default string"), obj.asString());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
}
}  // namespace SchemaItem
}  // namespace SmartObjects
}  // namespace components
}  // namespace test
