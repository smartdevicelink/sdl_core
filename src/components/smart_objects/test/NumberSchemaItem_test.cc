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
#include "smart_objects/number_schema_item.h"

namespace test {
namespace components {
namespace SmartObjects {
namespace SchemaItem {

using NsSmartDeviceLink::NsSmartObjects::ISchemaItemPtr;

/**
 * Test NumberSchemaItem as INT with no default value
 *
 * Create SchemaItem without default value. Method setDefaultValue should always return false
 * and leave SmartObject in previous state.
 **/
TEST(test_int_no_default_value, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<int>::create();  // No default value, no min, no max

  //Object int
  obj = 5;
  ASSERT_EQ(5, obj.asInt());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Obj bool
  obj = true;
  ASSERT_TRUE(obj.asBool());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  //Set default value
  bool resDefault = item->setDefaultValue(obj);
  EXPECT_FALSE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  EXPECT_TRUE(obj.asBool());

  //Obj string
  obj = "Test";
  ASSERT_EQ(std::string("Test"), obj.asString());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resDefault = item->setDefaultValue(obj);
  EXPECT_FALSE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
}

/**
 * Test NumberSchemaItem as INT with setted min value
 **/
TEST(test_int_min_value, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<int>::create(
      TSchemaItemParameter<int>(10));  // No default value, no max value

  //Object int correct
  obj = 15;
  ASSERT_EQ(15, obj.asInt());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object int incorrect
  obj = 9;
  ASSERT_EQ(9, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  //Object int correct
  obj = 10;
  ASSERT_EQ(10, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
}

/**
 * Test NumberSchemaItem as INT with setted max value
 **/
TEST(test_int_max_value, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<int>::create(
      TSchemaItemParameter<int>(), TSchemaItemParameter<int>(749));  // No default value, no min value

  //Object int correct
  obj = 749;
  ASSERT_EQ(749, obj.asInt());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object int incorrect
  obj = 750;
  ASSERT_EQ(750, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  //Object int correct
  obj = -750;
  ASSERT_EQ(-750, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
}

/**
 * Test NumberSchemaItem as INT with setted min and max value
 **/
TEST(test_int_min_max_value, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<int>::create(
      TSchemaItemParameter<int>(-949), TSchemaItemParameter<int>(749));  // No default value

  //Object int correct
  obj = 749;
  ASSERT_EQ(749, obj.asInt());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object int incorrect
  obj = 750;
  ASSERT_EQ(750, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  //Object int correct
  obj = -949;
  ASSERT_EQ(-949, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object int incorrect
  obj = -950;
  ASSERT_EQ(-950, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

}

/**
 * Test NumberSchemaItem as INT with correct default value
 **/
TEST(test_int_correct_default_value, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<int>::create(
      TSchemaItemParameter<int>(-12000), TSchemaItemParameter<int>(100),
      TSchemaItemParameter<int>(-38));

  //Object int correct
  obj = -12000;
  ASSERT_EQ(-12000, obj.asInt());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object int incorrect
  obj = -12001;
  ASSERT_EQ(-12001, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  //Object int correct
  obj = 100;
  ASSERT_EQ(100, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object int incorrect
  obj = 101;
  ASSERT_EQ(101, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  //Set default value
  bool resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
  EXPECT_EQ(-38, obj.asInt());

  //Object string
  obj = "string";
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
  EXPECT_EQ(-38, obj.asInt());
}

/**
 * Test NumberSchemaItem as INT with default value out of range
 **/
TEST(test_int_default_value_out_of_range, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<int>::create(
      TSchemaItemParameter<int>(90), TSchemaItemParameter<int>(100),
      TSchemaItemParameter<int>(50));  // Default value out of range

  //Object int correct
  obj = 90;
  ASSERT_EQ(90, obj.asInt());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object int incorrect
  obj = 89;
  ASSERT_EQ(89, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  //Object int correct
  obj = 100;
  ASSERT_EQ(100, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object int incorrect
  obj = 101;
  ASSERT_EQ(101, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  //Set default value
  bool resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
  EXPECT_EQ(50, obj.asInt());

  //Object string
  obj = "string";
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
  EXPECT_EQ(50, obj.asInt());
}

TEST(test_int_map_validate, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<int>::create(
      TSchemaItemParameter<int>(-120), TSchemaItemParameter<int>(100),
      TSchemaItemParameter<int>(-38));

  obj["min"] = -120;
  obj["out_of_min"] = -121;
  obj["max"] = 100;
  obj["out_of_max"] = 101;

  int resultType = item->validate(obj["min"]);
  EXPECT_EQ(Errors::OK, resultType);
  resultType = item->validate(obj["max"]);
  EXPECT_EQ(Errors::OK, resultType);
  resultType = item->validate(obj["out_of_min"]);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
  resultType = item->validate(obj["out_of_max"]);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  //Set default value
  bool resDefault = item->setDefaultValue(obj["aa"]);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(-38, obj["aa"].asInt());

  resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(-38, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  resultType = item->validate(obj["min"]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
}

TEST(test_int_array_validate, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<int>::create(
      TSchemaItemParameter<int>(-120), TSchemaItemParameter<int>(100),
      TSchemaItemParameter<int>(-38));

  obj[0] = -121;
  obj[1] = -120;
  obj[2] = 100;
  obj[3] = 101;

  int resultType = item->validate(obj[0]);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OUT_OF_RANGE,
            resultType);

  resultType = item->validate(obj[1]);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, resultType);

  resultType = item->validate(obj[2]);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, resultType);

  resultType = item->validate(obj[3]);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OUT_OF_RANGE,
            resultType);

  resultType = item->validate(obj);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE,
            resultType);

  bool resDefault = item->setDefaultValue(obj[0]);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(-38, obj[0].asInt());

  resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(-38, obj.asInt());

  resultType = item->validate(obj);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, resultType);

  resultType = item->validate(obj[0]);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE,
            resultType);

  resultType = item->validate(obj);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE,
            resultType);
}

/**
 * Test NumberSchemaItem as DOUBLE with no default value
 *
 * Create SchemaItem without default value. Method setDefaultValue should always return false
 * and leave SmartObject in previous state.
 **/
TEST(test_double_no_default_value, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<double>::create();  // No default value, no min, no max

  //Object int
  obj = 5.79;
  ASSERT_EQ(5.79, obj.asDouble());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Obj bool
  obj = true;
  ASSERT_TRUE(obj.asBool());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  //Set default value
  bool resDefault = item->setDefaultValue(obj);
  EXPECT_FALSE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  EXPECT_TRUE(obj.asBool());

  //Obj string
  obj = "Test";
  ASSERT_EQ(std::string("Test"), obj.asString());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  //Set default value
  resDefault = item->setDefaultValue(obj);
  EXPECT_FALSE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
}

/**
 * Test NumberSchemaItem as DOUBLE with setted min value
 **/
TEST(test_double_min_value, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<double>::create(
      TSchemaItemParameter<double>(10.0));  // No default value, no max value

  //Object double correct
  obj = 10.000001;
  ASSERT_EQ(10.000001, obj.asDouble());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object double incorrect
  obj = 9.999999;
  ASSERT_EQ(9.999999, obj.asDouble());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
  /*
   //Object int
   obj = 10;
   ASSERT_EQ(10, obj.asInt());

   resultType = item->validate(obj);
   EXPECT_EQ(Errors::INVALID_VALUE, resultType);*/
}

/**
 * Test NumberSchemaItem as DOUBLE with setted max value
 **/
TEST(test_double_max_value, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<double>::create(
      TSchemaItemParameter<double>(), TSchemaItemParameter<double>(749.0));  // No default value, no min value

  //Object double correct
  obj = 749.0;
  ASSERT_EQ(749.0, obj.asDouble());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object double incorrect
  obj = 749.0001;
  ASSERT_EQ(749.0001, obj.asDouble());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  //Object double correct
  obj = -750.0;
  ASSERT_EQ(-750.0, obj.asDouble());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
}

/**
 * Test NumberSchemaItem as DOUBLE with setted min and max value
 **/
TEST(test_double_min_max_value, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<double>::create(
      TSchemaItemParameter<double>(-949.0),
      TSchemaItemParameter<double>(749.0));  // No default value

  //Object double correct
  obj = 749.0;
  ASSERT_EQ(749.0, obj.asDouble());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object double incorrect
  obj = 749.001;
  ASSERT_EQ(749.001, obj.asDouble());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  //Object double correct
  obj = -949.0;
  ASSERT_EQ(-949.0, obj.asDouble());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object double incorrect
  obj = -949.00001;
  ASSERT_EQ(-949.00001, obj.asDouble());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

}

/**
 * Test NumberSchemaItem as DOUBLE with correct default value
 **/
TEST(test_double_correct_default_value, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<double>::create(
      TSchemaItemParameter<double>(-12000.0),
      TSchemaItemParameter<double>(100.0), TSchemaItemParameter<double>(-38.0));

  //Object double correct
  obj = -12000.0;
  ASSERT_EQ(-12000.0, obj.asDouble());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object double incorrect
  obj = -12000.01;
  ASSERT_EQ(-12000.01, obj.asDouble());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  //Object double correct
  obj = 100.0;
  ASSERT_EQ(100.0, obj.asDouble());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object double incorrect
  obj = 100.001;
  ASSERT_EQ(100.001, obj.asDouble());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  //Set default value
  bool resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
  EXPECT_EQ(-38.0, obj.asDouble());

  //Object string
  obj = "string";
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
  EXPECT_EQ(-38.0, obj.asDouble());
}

/**
 * Test NumberSchemaItem as DOUBLE with default value out of range
 **/
TEST(test_double_default_value_out_of_range, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<double>::create(
      TSchemaItemParameter<double>(90.0), TSchemaItemParameter<double>(100.0),
      TSchemaItemParameter<double>(50.0));  // Default value out of range

  //Object double correct
  obj = 90.0;
  ASSERT_EQ(90.0, obj.asDouble());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object double incorrect
  obj = 89.999;
  ASSERT_EQ(89.999, obj.asDouble());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  //Object double correct
  obj = 100.0;
  ASSERT_EQ(100.0, obj.asDouble());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  //Object double incorrect
  obj = 100.001;
  ASSERT_EQ(100.001, obj.asDouble());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);

  //Set default value
  bool resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
  EXPECT_EQ(50.0, obj.asDouble());

  //Object string
  obj = "string";
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
  resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
  EXPECT_EQ(50.0, obj.asDouble());
}

TEST(test_double_map_validate, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<double>::create(
      TSchemaItemParameter<double>(-120.0), TSchemaItemParameter<double>(100.0),
      TSchemaItemParameter<double>(-38.0));

  obj["min"] = -120.0;
  obj["out_of_min"] = -120.001;
  obj["max"] = 100.0;
  obj["out_of_max"] = 100.001;

  int resultType = item->validate(obj["min"]);
  EXPECT_EQ(Errors::OK, resultType);
  resultType = item->validate(obj["max"]);
  EXPECT_EQ(Errors::OK, resultType);
  resultType = item->validate(obj["out_of_min"]);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
  resultType = item->validate(obj["out_of_max"]);
  EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  bool resDefault = item->setDefaultValue(obj["aa"]);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(-38.0, obj["aa"].asDouble());

  resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(-38.0, obj.asDouble());

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);

  resultType = item->validate(obj["min"]);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);

  resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
}

TEST(test_double_array_validate, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  SmartObject obj;

  ISchemaItemPtr item = TNumberSchemaItem<double>::create(
      TSchemaItemParameter<double>(-120.0), TSchemaItemParameter<double>(100.0),
      TSchemaItemParameter<double>(-38.0));

  obj[0] = -120.001;
  obj[1] = -120.0;
  obj[2] = 100.0;
  obj[3] = 100.000001;

  int resultType = item->validate(obj[0]);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OUT_OF_RANGE,
            resultType);

  resultType = item->validate(obj[1]);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, resultType);

  resultType = item->validate(obj[2]);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, resultType);

  resultType = item->validate(obj[3]);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OUT_OF_RANGE,
            resultType);

  resultType = item->validate(obj);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE,
            resultType);

  bool resDefault = item->setDefaultValue(obj[0]);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(-38.0, obj[0].asDouble());

  resDefault = item->setDefaultValue(obj);
  EXPECT_TRUE(resDefault);
  EXPECT_EQ(-38.0, obj.asDouble());

  resultType = item->validate(obj);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, resultType);

  resultType = item->validate(obj[0]);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE,
            resultType);

  resultType = item->validate(obj);
  EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE,
            resultType);
}

TEST(test_int_double_value, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  ISchemaItemPtr item = TNumberSchemaItem<int>::create(
      TSchemaItemParameter<int>(10),
      TSchemaItemParameter<int>(100));  // No default value

  SmartObject obj;
  const double value = 10.0001;
  obj = value;
  ASSERT_EQ(value, obj.asDouble());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::INVALID_VALUE, resultType);
}

TEST(test_double_int_value, test_NumberSchemaItemTest) {
  using namespace NsSmartDeviceLink::NsSmartObjects;

  ISchemaItemPtr item = TNumberSchemaItem<double>::create(
      TSchemaItemParameter<double>(10.0),
      TSchemaItemParameter<double>(100.0));  // No default value

  SmartObject obj;
  const int value = 99;
  obj = value;
  ASSERT_EQ(value, obj.asInt());

  int resultType = item->validate(obj);
  EXPECT_EQ(Errors::OK, resultType);
}

}  // namespace SchemaItem
}  // namespace SmartObjects
}  // namespace components
}  // namespace test
