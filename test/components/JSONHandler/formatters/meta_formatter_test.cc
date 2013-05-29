/**
 * @file meta_formatter_test.cc
 * @brief implementation of test for class CMetaFormatter
 */
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
// POSSIBILITY OF SUCH DAMAGE.auto

#include "gtest/gtest.h"

#include <iostream>
#include <stdio.h>

#include "JSONHandler/formatters/meta_formatter.h"
#include "JSONHandler/CSmartFactory.hpp"
#include "JSONHandler/formatters/CFormatterJsonSDLRPCv1.hpp"

#include "meta_formatter_test_helper.h"

using test::components::JSONHandler::formatters::CMetaFormatterTestHelper;
namespace formatter_ns = NsSmartDeviceLink::NsJSONHandler::Formatters;
namespace generated_ns = Gen::test::components::JSONHandler2;
namespace strings_ns = NsSmartDeviceLink::NsJSONHandler::strings;

TEST_F(CMetaFormatterTestHelper, test_inputObjectIdenticalToSchemaWithAndWithoutMandatoryParams) {
  Json::Value value;  // just a quick workaround to avoid undefined reference to Json
  Json::Reader reader;    // the same thing
  
  NsSmartDeviceLink::NsSmartObjects::CSmartObject object1 = factory.CreateSmartObject(
      generated_ns::FunctionID::RegisterAppInterfaceID,
      generated_ns::messageType::request);

  NsSmartDeviceLink::NsSmartObjects::CSmartObject object2 = factory.CreateSmartObject(
      generated_ns::FunctionID::RegisterAppInterfaceID,
      generated_ns::messageType::request);
  
  NsSmartDeviceLink::NsSmartObjects::CSmartObject result_object1;
  NsSmartDeviceLink::NsSmartObjects::CSmartObject result_object2;
  // get schema
  NsSmartDeviceLink::NsSmartObjects::CSmartSchema schema;  
  bool get_schema_result = factory.GetSchema(
      generated_ns::FunctionID::RegisterAppInterfaceID,
      generated_ns::messageType::request,
      schema);

  ASSERT_TRUE(get_schema_result);
  
  FillObjectIdenticalToSchema(object1);
  FillObjectIdenticalToSchemaWithoutNoMandatoriesParams(object2);
  
  formatter_ns::CMetaFormatter::CreateObjectByPattern(object1, schema, result_object1);
  formatter_ns::CMetaFormatter::CreateObjectByPattern(object2, schema, result_object2);

  if (1) { // printing out
    std::string formatted_string;
    formatter_ns::CFormatterJsonSDLRPCv1::toString(object1, formatted_string);
    printf("object1 %s\n", formatted_string.c_str());

    formatter_ns::CFormatterJsonSDLRPCv1::toString(result_object1, formatted_string);
    printf("result_object1 %s\n", formatted_string.c_str());

    formatter_ns::CFormatterJsonSDLRPCv1::toString(object2, formatted_string);
    printf("object2 %s\n", formatted_string.c_str());

    formatter_ns::CFormatterJsonSDLRPCv1::toString(result_object2, formatted_string);
    printf("result_object2 %s\n", formatted_string.c_str());
  }

  CompareObjects(object1, result_object1);
  CompareObjects(result_object1, object1);
  CompareObjects(object2, result_object2);
  CompareObjects(result_object2, object2);

  // enums must be unapplied (converted to string) in order to be compared against strings
  result_object1.getSchema().unapplySchema(result_object1);
  EXPECT_EQ("request",
            result_object1[strings_ns::S_PARAMS][strings_ns::S_MESSAGE_TYPE].asString());
  EXPECT_EQ("RegisterAppInterfaceID",
            result_object1[strings_ns::S_PARAMS][strings_ns::S_FUNCTION_ID].asString());

  result_object2.getSchema().unapplySchema(result_object2);
  EXPECT_EQ("request",
            result_object2[strings_ns::S_PARAMS][strings_ns::S_MESSAGE_TYPE].asString());
  EXPECT_EQ("RegisterAppInterfaceID",
            result_object2[strings_ns::S_PARAMS][strings_ns::S_FUNCTION_ID].asString());
}


int main(int argc, char **argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
