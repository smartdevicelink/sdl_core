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
}}}}

int main(int argc, char **argv) {
  //PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}