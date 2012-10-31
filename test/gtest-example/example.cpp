#include "gtest/gtest.h"

namespace test
{
    namespace gtest_example
    {
        TEST(gtest_example, initial_test) 
        {
            EXPECT_TRUE(1 == 1);
            EXPECT_TRUE(1 == 2);
            ASSERT_FALSE(0 == 1);
            ASSERT_FALSE(0 == 0);
        }
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}