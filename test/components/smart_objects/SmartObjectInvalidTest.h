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

#ifndef TEST_COMPONENTS_SMARTOBJECTS_SMARTOBJECTINVALIDTEST_H_
#define TEST_COMPONENTS_SMARTOBJECTS_SMARTOBJECTINVALIDTEST_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "smart_objects/smart_object.h"
#include <string>

using namespace NsSmartDeviceLink::NsSmartObjects;


namespace test { namespace components { namespace SmartObjects { namespace SmartObjectInvalidTest {

    TEST(test_simple_type_can_be_set_in_constructor, test_SmartObjectInvalidTest)
    {
      SmartObject objMap(SmartType_Map);
      ASSERT_EQ(SmartType_Map, objMap.getType());

      SmartObject objArray(SmartType_Array);
      ASSERT_EQ(SmartType_Array, objArray.getType());

        SmartObject objInt(SmartType_Integer);
        ASSERT_EQ(SmartType_Integer, objInt.getType());

        SmartObject objDouble(SmartType_Double);
        ASSERT_EQ(SmartType_Double, objDouble.getType());

        SmartObject objBoolean(SmartType_Boolean);
        ASSERT_EQ(SmartType_Boolean, objBoolean.getType());

        SmartObject objChar(SmartType_Character);
        ASSERT_EQ(SmartType_Character, objChar.getType());

        SmartObject objString(SmartType_String);
        ASSERT_EQ(SmartType_String, objString.getType());

        SmartObject objBinary(SmartType_Binary);
        ASSERT_EQ(SmartType_Binary, objBinary.getType());

        SmartObject objInvalid(SmartType_Invalid);
        ASSERT_EQ(SmartType_Invalid, objInvalid.getType());

        SmartObject objNullConstructor(SmartType_Null);
        ASSERT_EQ(SmartType_Null, objNullConstructor.getType());

        SmartObject objNullDefault;
        ASSERT_EQ(SmartType_Null, objNullDefault.getType());
    }

    TEST(test_invalid_object_remains_invalid, test_SmartObjectInvalidTest)
    {
        SmartObject obj(SmartType_Invalid);
        ASSERT_EQ(SmartType_Invalid, obj.getType());

        obj = 1;
        ASSERT_EQ(SmartType_Invalid, obj.getType());
        ASSERT_EQ(invalid_int_value, (int)obj);

        // ---- unsigned int ---- //
        obj = static_cast<unsigned int>(100);
        ASSERT_EQ(SmartType_Invalid, obj.getType());
        ASSERT_EQ(invalid_int_value, (unsigned int)obj);

        // ---- DOUBLE ---- //
        obj = 3.14;
        ASSERT_EQ(SmartType_Invalid, obj.getType());
        ASSERT_EQ(invalid_double_value, (double)obj);

        // ---- CHAR ---- //
        obj = 'a';
        ASSERT_EQ(SmartType_Invalid, obj.getType());
        ASSERT_EQ(invalid_char_value, (char)obj);

        // ---- BOOL ---- //
        obj = true;
        ASSERT_EQ(SmartType_Invalid, obj.getType());
        ASSERT_EQ(invalid_bool_value, (bool)obj);

        // ---- CHAR* ---- //
        obj = "Hello, world";
        ASSERT_EQ(SmartType_Invalid, obj.getType());
        ASSERT_EQ(invalid_string_value, (std::string)obj);

        // ---- STD::STRING ---- //
        obj = std::string("Hello, world");
        ASSERT_EQ(SmartType_Invalid, obj.getType());
        ASSERT_EQ(invalid_string_value, (std::string)obj);

        // ---- BINARY ---- //
        NsSmartDeviceLink::NsSmartObjects::SmartBinary binaryData;
        binaryData.push_back('\0');
        binaryData.push_back('a');
        obj = binaryData;
        ASSERT_EQ(SmartType_Invalid, obj.getType());
        ASSERT_EQ(invalid_binary_value, obj.asBinary());

        // ---- ARRAY ---- //
        obj[0] = 1;
        obj[1] = true;
        obj[2] = 'a';
        obj[3] = 3.14;

        ASSERT_EQ(SmartType_Invalid, obj.getType());
        ASSERT_EQ(invalid_int_value, (int)obj[0]);
        ASSERT_EQ(invalid_bool_value, (bool)obj[1]);
        ASSERT_EQ(invalid_char_value, (char)obj[2]);
        ASSERT_EQ(invalid_double_value, (double)obj[3]);

        // ---- DEEP ARRAY ---- //
        obj[0] = 1;
        obj[1][0] = 3.14;
        obj[1][1][0] = true;

        ASSERT_EQ(SmartType_Invalid, obj.getType());
        ASSERT_EQ(invalid_int_value, (int)obj[0]);
        ASSERT_EQ(invalid_double_value, (double)obj[1][0]);
        ASSERT_EQ(invalid_bool_value, (bool)obj[1][1][0]);

        // ---- MAP ---- //
        obj["name"] = "My name";
        obj["count"] = 10;
        obj["isValid"] = true;

        ASSERT_EQ(SmartType_Invalid, obj.getType());
        ASSERT_EQ(invalid_string_value, (std::string)obj["name"]);
        ASSERT_EQ(invalid_int_value, (int)obj["count"]);
        ASSERT_EQ(invalid_bool_value, (bool)obj["isValid"]);

        // ---- DEEP MAP ---- //
        obj["request"]["name"] = "My Request";
        obj["request"]["id"] = 123;
        obj["response"]["name"] = "My Response";
        obj["response"]["id"] = 456;
        obj["we"]["need"]["to"]["go"]["deeper"] = true;

        ASSERT_EQ(SmartType_Invalid, obj.getType());

        ASSERT_EQ(invalid_string_value, (std::string)obj["request"]["name"]);
        ASSERT_EQ(invalid_int_value, (int)obj["request"]["id"]);
        ASSERT_EQ(invalid_string_value, (std::string)obj["response"]["name"]);
        ASSERT_EQ(invalid_int_value, (int)obj["response"]["id"]);
        ASSERT_EQ(invalid_bool_value, (bool)obj["we"]["need"]["to"]["go"]["deeper"]);
    }
}}}}

#endif  // TEST_COMPONENTS_SMARTOBJECTS_SMARTOBJECTINVALIDTEST_H_
