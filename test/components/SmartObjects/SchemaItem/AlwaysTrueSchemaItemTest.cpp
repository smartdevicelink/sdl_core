#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SmartObjects/CSmartObject.hpp"
#include "SmartObjects/CAlwaysTrueSchemaItem.hpp"

#include <string>


namespace test { namespace components { namespace SmartObjects { namespace SchemaItem { namespace SchemaItemDraftTest {

    using namespace NsSmartDeviceLink::NsSmartObjects;

    /**
     * Test AlwaysTrueSchemaItem
     **/
    TEST(test_AlwaysTrueSchemaItemTest, simple_test)
    {
        CSmartObject obj;

        TSharedPtr<ISchemaItem> item = CAlwaysTrueSchemaItem::create();

        obj = 5;
        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        EXPECT_EQ(5, (int)obj);

        obj = true;
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        EXPECT_TRUE((bool)obj);

        obj = "Test";
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        EXPECT_EQ(std::string("Test"), (std::string)obj);

        obj["First"] = "Some string";
        obj["Second"] = 555;
        resultType = item->validate(obj["First"]);
        EXPECT_EQ(Errors::OK, resultType);
        resultType = item->validate(obj["Second"]);
        EXPECT_EQ(Errors::OK, resultType);
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        EXPECT_EQ(std::string("Some string"),(std::string)obj["First"]);
        EXPECT_EQ(555, (int)obj["Second"]);


        obj[0] = true;
        obj[1] = false;
        resultType = item->validate(obj[0]);
        EXPECT_EQ(Errors::OK, resultType);
        resultType = item->validate(obj[1]);
        EXPECT_EQ(Errors::OK, resultType);
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        EXPECT_TRUE((bool)obj[0]);
        EXPECT_FALSE((bool)obj[1]);

    }
}}}}}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}