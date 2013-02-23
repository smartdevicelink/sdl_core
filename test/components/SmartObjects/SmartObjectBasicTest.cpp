
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


    // TODO: Figure out why the test fails
    TEST(BasicArray, test_SmartObjectBasicTest)
    {
        CSmartObject obj;

        ASSERT_EQ(0, static_cast<int>(obj[0])) << "Wrong value at accessing non existent index";
        ASSERT_EQ(0, static_cast<int>(obj["non_existent_key"])) << "Wrong value at accessing non existent key";

        obj[0] = 1;
        ASSERT_EQ(1, static_cast<int>(obj[0])) << "Wrong value at 0 index";
        obj[1] = 2;
        ASSERT_EQ(2, static_cast<int>(obj[1])) << "Wrong value at 1 index";

        obj[0][0] = 3;
        obj[1][0] = 1;
        ASSERT_EQ(3, static_cast<int>(obj[0][0])) << "Wrong value at index 0, 0";

        obj[0][0][0] = 4;
        obj[0][1][0] = 5;   // FIXME: Segmentation fault
        ASSERT_EQ(4, static_cast<int>(obj[0][0][0])) << "Wrong value at index 0, 0, 0";

        const int size = 2;
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                for (int k = 0; k < size; k++)
                {
                    std::cout << "i, j, k: " << i << ", " << j << ", " << k << std::endl;
                    obj[i][j][k] = i + j + k;
                }

        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                for (int k = 0; k < size; k++)
                {
                    ASSERT_EQ(i + j + k, static_cast<int>(obj[i][j][k])) <<
                            "Wrong value in the array at index: " << i << ", " << j << ", " << k;
                }
    }


    int main(int argc, char **argv)
    {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }

}}}}

