#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SmartObjects/CSmartObject.hpp"
#include <string>


namespace test { namespace components { namespace SmartObjects { namespace SmartObjectDraftTest {

    TEST(test_primitive_types, test_SmartObjectsDraftTest)
    {
        NsAppLink::NsSmartObjects::CSmartObject obj;

        obj = 5;
        ASSERT_EQ(5, (int)obj);

        obj = true;
        ASSERT_TRUE((bool)obj);

        obj = "Test";
        ASSERT_EQ(std::string("Test"), (std::string)obj);
    }

    TEST(test_map_access, test_SmartObjectsDraftTest)
    {
        NsAppLink::NsSmartObjects::CSmartObject obj;

        obj["aa"] = true;
        ASSERT_TRUE((bool)obj["aa"]);

        obj["aa"]["fds"]["Fsdf"] = 123;
        ASSERT_EQ(123, (int)obj["aa"]["fds"]["Fsdf"]);
    }

    TEST(test_array_access, test_SmartObjectsDraftTest)
    {
        NsAppLink::NsSmartObjects::CSmartObject obj;

        obj[0] = 5;
        obj[-1] = 6; // Appending new item to array

        ASSERT_EQ(5, (int)obj[0]);
        ASSERT_EQ(6, (int)obj[1]);
    }

    TEST(test_public_interface, test_SmartObjectsDraftTest)
    {
        NsAppLink::NsSmartObjects::CSmartObject obj;

        // ---- INTEGER ---- //
        obj = 1;
        ASSERT_EQ(1, (int)obj);

        // ---- LONG ---- //
        obj = 100l;
        ASSERT_EQ(100l, (long)obj);

        // ---- DOUBLE ---- //
        obj = 3.14;
        ASSERT_EQ(3.14, (double)obj);

        // ---- CHAR ---- //
        obj = 'a';
        ASSERT_EQ('a', (char)obj);

        // ---- BOOL ---- //
        obj = true;
        ASSERT_TRUE((bool)obj);

        // ---- CHAR* ---- //
        obj = "Hello, world";
        ASSERT_EQ(std::string("Hello, world"), (std::string)obj);

        // ---- STD::STRING ---- //
        obj = std::string("Hello, world");
        ASSERT_EQ(std::string("Hello, world"), (std::string)obj);

        // ---- ARRAY ---- //
        obj[0] = 1;
        obj[1] = true;
        obj[2] = 'a';
        obj[3] = 3.14;

        ASSERT_EQ(1, (int)obj[0]);
        ASSERT_TRUE((bool)obj[1]);
        ASSERT_EQ('a', (char)obj[2]);
        ASSERT_EQ(3.14, (double)obj[3]);

        // ---- DEEP ARRAY ---- //
        obj[0] = 1;
        obj[1][0] = 3.14;
        obj[1][1][0] = true;

        ASSERT_EQ(1, (int)obj[0]);
        ASSERT_EQ(3.14, (double)obj[1][0]);
        ASSERT_TRUE((bool)obj[1][1][0]);

        // ---- MAP ---- //
        obj["name"] = "My name";
        obj["count"] = 10;
        obj["isValid"] = true;

        ASSERT_EQ(std::string("My name"), (std::string)obj["name"]);
        ASSERT_EQ(10, (int)obj["count"]);
        ASSERT_TRUE((bool)obj["isValid"]);

        // ---- DEEP MAP ---- //
        obj["request"]["name"] = "My Request";
        obj["request"]["id"] = 123;
        obj["response"]["name"] = "My Response";
        obj["response"]["id"] = 456;
        obj["we"]["need"]["to"]["go"]["deeper"] = true;

        ASSERT_EQ(std::string("My Request"), (std::string)obj["request"]["name"]);
        ASSERT_EQ(123, (int)obj["request"]["id"]);
        ASSERT_EQ(std::string("My Response"), (std::string)obj["response"]["name"]);
        ASSERT_EQ(456, (int)obj["response"]["id"]);
        ASSERT_TRUE((bool)obj["we"]["need"]["to"]["go"]["deeper"]);
    }

    TEST(test_helper_methods, test_SmartObjectsDraftTest)
    {
        NsAppLink::NsSmartObjects::CSmartObject obj;

        // ---- INTEGER ---- //
        obj = 1;
        ASSERT_EQ(1, obj.asInt());

        // ---- LONG ---- //
        obj = 100l;
        ASSERT_EQ(100l, obj.asLong());

        // ---- DOUBLE ---- //
        obj = 3.14;
        ASSERT_EQ(3.14, obj.asDouble());

        // ---- CHAR ---- //
        obj = 'a';
        ASSERT_EQ('a', obj.asChar());

        // ---- BOOL ---- //
        obj = true;
        ASSERT_TRUE(obj.asBool());

        // ---- STD::STRING ---- //
        obj = std::string("Hello, world");
        ASSERT_EQ(std::string("Hello, world"), obj.asString());
    }
}}}}

int main(int argc, char **argv) {
  //PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}