
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SmartObjects/CSmartObject.hpp"

namespace test { namespace components { namespace SmartObjects { namespace SmartObjectBasicTest {

    using namespace NsAppLink::NsSmartObjects;

    /*
     * Tests different types sequentially
     */
    TEST(BasicMixtedTypes, test_SmartObjectBasicTest)
    {
        CSmartObject obj;

        ASSERT_EQ(0, static_cast<int>(obj)) << "Wrong cast to int just after construction";

        obj = 10;
        ASSERT_EQ(10, static_cast<int>(obj)) << "Wrong cast to int";

        obj = "some string";
        ASSERT_EQ("some string", static_cast<std::string>(obj)) << "Wrong cast to std::string";

        obj = false;
        ASSERT_FALSE(static_cast<bool>(obj)) << "Wrong cast to bool";

        obj = 'A';
        ASSERT_EQ('A', static_cast<char>(obj)) << "Wrong cast to char";

        obj = 3.14;
        ASSERT_EQ(3.14, static_cast<double>(obj)) << "Wrong cast to double";

        // array test
        for (int i = 0; i < 100; i++)
        {
            obj[i] = i;
            ASSERT_EQ(i, static_cast<int>(obj[i]));
        }

        // map test
        for (int i = 0; i < 100; i++)
        {
            char key[8];
            sprintf(key, "%d", i);
            obj[key] = i;
            ASSERT_EQ(i, static_cast<int>(obj[key]));
        }
    }

    int main(int argc, char **argv)
    {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }

}}}}

