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
#include "smart_objects/array_schema_item.h"
#include "smart_objects/string_schema_item.h"

namespace test {
namespace components {
namespace SmartObjects {
namespace SchemaItem {

/**
 * Test ArraySchemaItem no schema item, no min and max size
 **/
TEST(test_no_default_value, test_ArraySchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject obj;

  ISchemaItemPtr item = CArraySchemaItem::create();  // No schema item, no min and max size

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

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
  resultType = item->validate(obj[0]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[1]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[2]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[3]);
  EXPECT_EQ(Errors::OK, resultType);

  item->applySchema(obj, false);

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
  resultType = item->validate(obj[0]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[1]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[2]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[3]);
  EXPECT_EQ(Errors::OK, resultType);

  EXPECT_EQ(38, obj[0].asInt());
  EXPECT_TRUE(obj[1].asBool());
  EXPECT_EQ(std::string("New String"), obj[2].asString());
  EXPECT_EQ(39, obj[3][0].asInt());
  EXPECT_FALSE(obj[3][1].asBool());
  EXPECT_EQ(std::string("Another String"), obj[3][2].asString());

  //Object - valid string
  obj = "New valid string";
  ASSERT_EQ(std::string("New valid string"), obj.asString());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  //Obj - bool
  obj = true;

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  //Object - number
  obj = 3.1415926;

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
}

/**
 * Test ArraySchemaItem with schema item
 *
 * Create ArraySchemaItem with schema item. Method validate should return true
 * only if all array elements are valid schema item objects
 **/
TEST(test_item_with_default_value, test_ArraySchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject obj;

  ISchemaItemPtr item = CArraySchemaItem::create();  // No min and max size

  obj[0] = "Some String";
  obj[1] = "true";
  obj[2] = "New String";

  EXPECT_EQ(std::string("Some String"), obj[0].asString());
  EXPECT_EQ(std::string("true"), obj[1].asString());
  EXPECT_EQ(std::string("New String"), obj[2].asString());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
  resultType = item->validate(obj[0]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[1]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[2]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  item->applySchema(obj, false);

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
  resultType = item->validate(obj[0]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[1]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resultType = item->validate(obj[2]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  EXPECT_EQ(std::string("Some String"), obj[0].asString());
  EXPECT_EQ(std::string("true"), obj[1].asString());
  EXPECT_EQ(std::string("New String"), obj[2].asString());

  obj[3][0] = "39";
  obj[3][1] = "false";
  obj[3][2] = "Another String";

  EXPECT_EQ(std::string("39"), obj[3][0].asString());
  EXPECT_EQ(std::string("false"), obj[3][1].asString());
  EXPECT_EQ(std::string("Another String"), obj[3][2].asString());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
  resultType = item->validate(obj[3]);
  EXPECT_EQ(Errors::OK, resultType);

  obj[3][3] = "Another very very loooooong String";

  resultType = item->validate(obj[3]);
  EXPECT_EQ(Errors::OK, resultType);
}

/**
 * Test ArraySchemaItem with min size
 **/
TEST(test_array_with_min_size, test_ArraySchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject obj;

  ISchemaItemPtr item = CArraySchemaItem::create(
      CStringSchemaItem::create(TSchemaItemParameter<size_t>(25)),
      TSchemaItemParameter<size_t>(3));

  obj[0] = "Some String";

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  obj[1] = "true";

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  obj[2] = "New String";

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  EXPECT_EQ(std::string("Some String"), obj[0].asString());
  EXPECT_EQ(std::string("true"), obj[1].asString());
  EXPECT_EQ(std::string("New String"), obj[2].asString());
}

/**
 * Test ArraySchemaItem with min size
 **/
TEST(test_array_with_max_size, test_ArraySchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject obj;

  ISchemaItemPtr item = CArraySchemaItem::create(
      CStringSchemaItem::create(TSchemaItemParameter<size_t>(),
                                TSchemaItemParameter<size_t>(25)),
      TSchemaItemParameter<size_t>(), TSchemaItemParameter<size_t>(3));  // No min size

  obj[0] = "Some String";

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  obj[1] = "true";

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  obj[2] = "New String";

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  obj[3] = "Another String";

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  EXPECT_EQ(std::string("Some String"), obj[0].asString());
  EXPECT_EQ(std::string("true"), obj[1].asString());
  EXPECT_EQ(std::string("New String"), obj[2].asString());
  EXPECT_EQ(std::string("Another String"), obj[3].asString());
}

/**
 * Test ArraySchemaItem with min and max size
 **/
TEST(test_array_with_min_and_max_size, test_ArraySchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject obj;

  ISchemaItemPtr item = CArraySchemaItem::create(
      CStringSchemaItem::create(TSchemaItemParameter<size_t>(),
                                TSchemaItemParameter<size_t>(25)),
      TSchemaItemParameter<size_t>(2), TSchemaItemParameter<size_t>(4));

  obj[0] = "Some String";

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  obj[1] = "true";

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  obj[2] = "New String";

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  obj[3] = "Another String";

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  obj[4] = "Out of array";

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  EXPECT_EQ(std::string("Some String"), obj[0].asString());
  EXPECT_EQ(std::string("true"), obj[1].asString());
  EXPECT_EQ(std::string("New String"), obj[2].asString());
  EXPECT_EQ(std::string("Another String"), obj[3].asString());
  EXPECT_EQ(std::string("Out of array"), obj[4].asString());
}

TEST(test_map_validate, test_ArraySchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;
  SmartObject obj;

  ISchemaItemPtr item = CArraySchemaItem::create(
      CStringSchemaItem::create(TSchemaItemParameter<size_t>(),
                                TSchemaItemParameter<size_t>(25)),
      TSchemaItemParameter<size_t>(2), TSchemaItemParameter<size_t>(4));

  obj["array"][0] = "Some String";

  int resultType = item->validate(obj["array"]);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  obj["array"][1] = "true";

  resultType = item->validate(obj["array"]);
  EXPECT_EQ(Errors::OK, resultType);

  obj["array"][2] = "New String";

  resultType = item->validate(obj["array"]);
  EXPECT_EQ(Errors::OK, resultType);

  obj["array"][3] = "Another String";

  resultType = item->validate(obj["array"]);
  EXPECT_EQ(Errors::OK, resultType);

  obj["array"][4] = "Out of array";

  resultType = item->validate(obj["array"]);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  EXPECT_EQ(std::string("Some String"), obj["array"][0].asString());
  EXPECT_EQ(std::string("true"), obj["array"][1].asString());
  EXPECT_EQ(std::string("New String"), obj["array"][2].asString());
  EXPECT_EQ(std::string("Another String"), obj["array"][3].asString());
  EXPECT_EQ(std::string("Out of array"), obj["array"][4].asString());
}
}  // namespace SchemaItem
}  // namespace SmartObjects
}  // namespace components
}  // namespace test

