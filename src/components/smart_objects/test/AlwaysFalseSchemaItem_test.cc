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
#include "smart_objects/always_false_schema_item.h"

namespace test {
namespace components {
namespace SmartObjects {
namespace SchemaItem {
using namespace NsSmartDeviceLink::NsSmartObjects;

/**
 * Test AlwaysFalseSchemaItem
 **/
TEST(test_AlwaysFalseSchemaItemTest, simple_test) {
  SmartObject obj;

  ISchemaItemPtr item = CAlwaysFalseSchemaItem::create();

  obj = 5;
  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::ERROR, resultType);
  EXPECT_EQ(5, obj.asInt());

  obj = true;
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::ERROR, resultType);
  EXPECT_TRUE(obj.asBool());

  obj = "Test";
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::ERROR, resultType);
  EXPECT_EQ(std::string("Test"), obj.asString());

  obj["First"] = "Some string";
  obj["Second"] = 555;
  resultType = item->validate(obj["First"]);
  EXPECT_EQ(Errors::ERROR, resultType);
  resultType = item->validate(obj["Second"]);
  EXPECT_EQ(Errors::ERROR, resultType);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::ERROR, resultType);
  EXPECT_EQ(std::string("Some string"), obj["First"].asString());
  EXPECT_EQ(555, obj["Second"].asInt());

  obj[0] = true;
  obj[1] = false;
  resultType = item->validate(obj[0]);
  EXPECT_EQ(Errors::ERROR, resultType);
  resultType = item->validate(obj[1]);
  EXPECT_EQ(Errors::ERROR, resultType);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::ERROR, resultType);
  EXPECT_TRUE(obj[0].asBool());
  EXPECT_FALSE(obj[1].asBool());
}
}  // namespace SchemaItem
}  // namespace SmartObjects
}  // namespace components
}  // namespace test
