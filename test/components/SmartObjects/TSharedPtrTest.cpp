#include <stdlib.h>
#include <vector>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SmartObjects/TSharedPtr.hpp"

namespace test
{
    namespace components
    {
        namespace SmartObjects
        {
            namespace TSharedPtrTest
            {
                class CMockObject
                {
                public:
                    CMockObject(int id);
                    ~CMockObject(void);
                    int getId(void) const;

                    MOCK_METHOD0(destructor, void ());

                private:
                    int mId;
                };

                class CExtendedMockObject: public CMockObject
                {
                public:
                    CExtendedMockObject(int id);
                };
            }
        }
    }
}

test::components::SmartObjects::TSharedPtrTest::CMockObject::CMockObject(int id):
mId(id)
{
}

test::components::SmartObjects::TSharedPtrTest::CMockObject::~CMockObject(void)
{
    destructor();
}

int test::components::SmartObjects::TSharedPtrTest::CMockObject::getId(void) const
{
    return mId;
}

test::components::SmartObjects::TSharedPtrTest::CExtendedMockObject::CExtendedMockObject(int id):
CMockObject(id)
{
}

typedef NsAppLink::NsSmartObjects::TSharedPtr<test::components::SmartObjects::TSharedPtrTest::CMockObject> tMockObjectPtr;
typedef NsAppLink::NsSmartObjects::TSharedPtr<test::components::SmartObjects::TSharedPtrTest::CExtendedMockObject> tExtendedMockObjectPtr;

TEST(TSharedPtrTest, Constructor)
{
    test::components::SmartObjects::TSharedPtrTest::CMockObject * object1 = new test::components::SmartObjects::TSharedPtrTest::CMockObject(1);
    test::components::SmartObjects::TSharedPtrTest::CMockObject * object2 = new test::components::SmartObjects::TSharedPtrTest::CMockObject(2);

    EXPECT_CALL(*object1, destructor()).Times(0);
    EXPECT_CALL(*object2, destructor()).Times(0);

    tMockObjectPtr p1(object1);
    ASSERT_EQ(1, p1->getId());

    tMockObjectPtr p2(p1);
    ASSERT_EQ(1, p2->getId());

    tMockObjectPtr p3 = p2;
    ASSERT_EQ(1, p3->getId());

    tMockObjectPtr p4 = object2;
    ASSERT_EQ(2, p4->getId());

    p3 = p4;
    ASSERT_EQ(2, p3->getId());

    EXPECT_CALL(*object1, destructor()).Times(1);
    EXPECT_CALL(*object2, destructor()).Times(1);
}

TEST(TSharedPtrTest, PointerTypeCast)
{
    test::components::SmartObjects::TSharedPtrTest::CExtendedMockObject * object1 = new test::components::SmartObjects::TSharedPtrTest::CExtendedMockObject(1);
    test::components::SmartObjects::TSharedPtrTest::CExtendedMockObject * object2 = new test::components::SmartObjects::TSharedPtrTest::CExtendedMockObject(2);

    EXPECT_CALL(*object1, destructor()).Times(0);
    EXPECT_CALL(*object2, destructor()).Times(0);

    tExtendedMockObjectPtr ep1(object1);
    ASSERT_EQ(1, ep1->getId());

    tMockObjectPtr p1(ep1);
    ASSERT_EQ(1, p1->getId());

    tExtendedMockObjectPtr ep2(object2);
    ASSERT_EQ(2, ep2->getId());

    p1 = ep2;
    ASSERT_EQ(2, p1->getId());

    EXPECT_CALL(*object1, destructor()).Times(1);
    EXPECT_CALL(*object2, destructor()).Times(1);
}

TEST(TSharedPtrTest, StressTest)
{
    const size_t cNumIterations = 1024U * 1024U;

    size_t objectCreated = 0U;
    size_t pointersCopied = 0U;

    std::vector<tMockObjectPtr> objects;

    for (size_t i = 0U; i < cNumIterations; ++i)
    {
        if ((true == objects.empty()) ||
            (0 == rand() % 256))
        {
            test::components::SmartObjects::TSharedPtrTest::CMockObject * object = new test::components::SmartObjects::TSharedPtrTest::CMockObject(0);
            EXPECT_CALL(*object, destructor()).Times(1);

            objects.push_back(object);

            ++objectCreated;
        }
        else
        {
            size_t objectIndex = static_cast<size_t>(rand()) % objects.size();

            if (rand() % 2)
            {
                objects.push_back(objects[objectIndex]);

                ++pointersCopied;
            }
            else
            {
                objects.erase(objects.begin() + objectIndex);
            }
        }
    }

    printf("%zu objects created, %zu pointers copied\n", objectCreated, pointersCopied);
}
