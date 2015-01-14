#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace test
{
    namespace gtest_example
    {
        TEST(gtest_example, initial_test) 
        {
            EXPECT_TRUE(1 == 1);
            ASSERT_FALSE(0 == 1);
            //ASSERT_FALSE(0 == 0); // Uncomment this and test will fail
        }
    }

    namespace gmock_example
    {
        // Class that we test
        class ClassToTest
        {
        public:
            virtual ~ClassToTest(){}
            virtual void doAction() = 0; // We will test that doAction will be called
            virtual void run()
            {
                doAction();
                //doAction(); // Uncomment this and test will fail
            }
        };

        // Our mock class
        class MockClassToTest: public ClassToTest
        {
        public:
            MOCK_METHOD0(doAction, void());
        };

        TEST(gmock_example, test)
        {
            MockClassToTest c;
            EXPECT_CALL(c, doAction())
                .Times(1)
            ;

            c.run();
        }
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}