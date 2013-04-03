#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SmartObjects/CSmartObject.hpp"
#include <string>

using namespace NsSmartDeviceLink::NsSmartObjects;


namespace test { namespace components { namespace SmartObjects { namespace SmartObjectInvalidTest {

    TEST(test_simple_type_can_be_set_in_constructor, test_SmartObjectInvalidTest)
    {
        CSmartObject objInt(SmartType_Integer);
        ASSERT_EQ(SmartType_Integer, objInt.getType());

        CSmartObject objDouble(SmartType_Double);
        ASSERT_EQ(SmartType_Double, objDouble.getType());

        CSmartObject objBoolean(SmartType_Boolean);
        ASSERT_EQ(SmartType_Boolean, objBoolean.getType());

        CSmartObject objChar(SmartType_Character);
        ASSERT_EQ(SmartType_Character, objChar.getType());

        CSmartObject objString(SmartType_String);
        ASSERT_EQ(SmartType_String, objString.getType());

        CSmartObject objBinary(SmartType_Binary);
        ASSERT_EQ(SmartType_Binary, objBinary.getType());

        CSmartObject objInvalid(SmartType_Invalid);
        ASSERT_EQ(SmartType_Invalid, objInvalid.getType());

        CSmartObject objNullConstructor(SmartType_Null);
        ASSERT_EQ(SmartType_Null, objNullConstructor.getType());

        CSmartObject objNullDefault;
        ASSERT_EQ(SmartType_Null, objNullDefault.getType());
    }

    TEST(test_some_types_cannot_be_set_in_constructor, test_SmartObjectInvalidTest)
    {
        CSmartObject objMap(SmartType_Map);
        ASSERT_EQ(SmartType_Null, objMap.getType());

        CSmartObject objArray(SmartType_Array);
        ASSERT_EQ(SmartType_Null, objArray.getType());
    }

    TEST(test_invalid_object_remains_invalid, test_SmartObjectInvalidTest)
    {
        CSmartObject obj(SmartType_Invalid);
        ASSERT_EQ(SmartType_Invalid, obj.getType());

        obj = 1;
        ASSERT_EQ(SmartType_Invalid, obj.getType());
        ASSERT_EQ(invalid_int_value, (int)obj);

        // ---- LONG ---- //
        obj = 100l;
        ASSERT_EQ(SmartType_Invalid, obj.getType());
        ASSERT_EQ(invalid_int_value, (long)obj);

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

int main(int argc, char **argv) {
    //PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}