#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SmartObjects/CSmartObject.hpp"
#include "SmartObjects/CArraySchemaItem.hpp"
#include "SmartObjects/CStringSchemaItem.hpp"

#include <string>


namespace test { namespace components { namespace SmartObjects { namespace SchemaItem { namespace ArraySchemaItemTest {

    /**
     * Test ArraySchemaItem no schema item, no min and max size
     **/
    TEST(test_no_default_value, test_ArraySchemaItemTest)
    {
        using namespace NsAppLink::NsSmartObjects;
        CSmartObject obj;

        TSharedPtr<CArraySchemaItem> item = CArraySchemaItem::create(); // No schema item, no min and max size

        obj[0] = 38;
        obj[1] = true;
        obj[2] = "New String";
        obj[3][0] = 39;
        obj[3][1] = false;
        obj[3][2] = "Another String";
        
        EXPECT_EQ(38, (int)obj[0]);
        EXPECT_TRUE((bool)obj[1]);
        EXPECT_EQ(std::string("New String"), (std::string)obj[2]);
        EXPECT_EQ(39, (int)obj[3][0]);
        EXPECT_FALSE((bool)obj[3][1]);
        EXPECT_EQ(std::string("Another String"), (std::string)obj[3][2]);
        
        
        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        resultType = item->validate(obj[0]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resultType = item->validate(obj[1]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resultType = item->validate(obj[2]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resultType = item->validate(obj[3]);
        EXPECT_EQ(Errors::OK, resultType);
        
        item->applySchema(obj);
        
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        resultType = item->validate(obj[0]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resultType = item->validate(obj[1]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resultType = item->validate(obj[2]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resultType = item->validate(obj[3]);
        EXPECT_EQ(Errors::OK, resultType);

        EXPECT_EQ(38, (int)obj[0]);
        EXPECT_TRUE((bool)obj[1]);
        EXPECT_EQ(std::string("New String"), (std::string)obj[2]);
        EXPECT_EQ(39, (int)obj[3][0]);
        EXPECT_FALSE((bool)obj[3][1]);
        EXPECT_EQ(std::string("Another String"), (std::string)obj[3][2]);

        //Object - valid string
        obj = "New valid string";
        ASSERT_EQ(std::string("New valid string"), (std::string)obj);

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        //Obj - bool
        obj = true;

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        //Object - number
        obj = 3.1415926;
        
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
    }

    /** 
     * Test ArraySchemaItem with schema item
     * 
     * Create ArraySchemaItem with schema item. Method validate should return true
     * only if all array elements are valid schema item objects
     **/
    TEST(test_item_with_default_value, test_ArraySchemaItemTest)
    {
        using namespace NsAppLink::NsSmartObjects;
        CSmartObject obj;

        TSharedPtr<CArraySchemaItem> item = CArraySchemaItem::create(
            CStringSchemaItem::create(TSchemaItemParameter<size_t>(25))); // No min and max size

        obj[0] = "Some String";
        obj[1] = "true";
        obj[2] = "New String";
        
        EXPECT_EQ(std::string("Some String"), (std::string)obj[0]);
        EXPECT_EQ(std::string("true"), (std::string)obj[1]);
        EXPECT_EQ(std::string("New String"), (std::string)obj[2]);
        
        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        resultType = item->validate(obj[0]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resultType = item->validate(obj[1]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resultType = item->validate(obj[2]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        
        item->applySchema(obj);
        
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        resultType = item->validate(obj[0]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resultType = item->validate(obj[1]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resultType = item->validate(obj[2]);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        EXPECT_EQ(std::string("Some String"), (std::string)obj[0]);
        EXPECT_EQ(std::string("true"), (std::string)obj[1]);
        EXPECT_EQ(std::string("New String"), (std::string)obj[2]);

        obj[3][0] = "39";
        obj[3][1] = "false";
        obj[3][2] = "Another String";

        EXPECT_EQ(std::string("39"), (std::string)obj[3][0]);
        EXPECT_EQ(std::string("false"), (std::string)obj[3][1]);
        EXPECT_EQ(std::string("Another String"), (std::string)obj[3][2]);
        
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resultType = item->validate(obj[3]);
        EXPECT_EQ(Errors::OK, resultType);
        
        obj[3][3] = "Another very very loooooong String";
        
        resultType = item->validate(obj[3]);
        EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
    }
    
    /** 
     * Test ArraySchemaItem with min size
     **/
    TEST(test_array_with_min_size, test_ArraySchemaItemTest)
    {
        using namespace NsAppLink::NsSmartObjects;
        CSmartObject obj;

        TSharedPtr<CArraySchemaItem> item = CArraySchemaItem::create(
                CStringSchemaItem::create(TSchemaItemParameter<size_t>(25)),
            TSchemaItemParameter<size_t>(3)); // No max size

        obj[0] = "Some String";
        
        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
        
        obj[1] = "true";
        
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
        
        obj[2] = "New String";
        
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        
        EXPECT_EQ(std::string("Some String"), (std::string)obj[0]);
        EXPECT_EQ(std::string("true"), (std::string)obj[1]);
        EXPECT_EQ(std::string("New String"), (std::string)obj[2]);
    }

    /** 
     * Test ArraySchemaItem with min size
     **/
    TEST(test_array_with_max_size, test_ArraySchemaItemTest)
    {
        using namespace NsAppLink::NsSmartObjects;
        CSmartObject obj;

        TSharedPtr<CArraySchemaItem> item = CArraySchemaItem::create(
                CStringSchemaItem::create(TSchemaItemParameter<size_t>(25)),
            TSchemaItemParameter<size_t>(),
            TSchemaItemParameter<size_t>(3)); // No min size

        obj[0] = "Some String";
        
        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        
        obj[1] = "true";
        
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        
        obj[2] = "New String";
        
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        
        obj[3] = "Another String";
        
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
        
        EXPECT_EQ(std::string("Some String"), (std::string)obj[0]);
        EXPECT_EQ(std::string("true"), (std::string)obj[1]);
        EXPECT_EQ(std::string("New String"), (std::string)obj[2]);
        EXPECT_EQ(std::string("Another String"), (std::string)obj[3]);
    }

    /** 
     * Test ArraySchemaItem with min and max size
     **/
    TEST(test_array_with_min_and_max_size, test_ArraySchemaItemTest)
    {
        using namespace NsAppLink::NsSmartObjects;
        CSmartObject obj;

        TSharedPtr<CArraySchemaItem> item = CArraySchemaItem::create(
                CStringSchemaItem::create(TSchemaItemParameter<size_t>(25)),
            TSchemaItemParameter<size_t>(2),
            TSchemaItemParameter<size_t>(4));
        
        obj[0] = "Some String";
        
        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
        
        obj[1] = "true";
        
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        
        obj[2] = "New String";
        
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);

        obj[3] = "Another String";
        
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        
        obj[4] = "Out of array";
        
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
        
        EXPECT_EQ(std::string("Some String"), (std::string)obj[0]);
        EXPECT_EQ(std::string("true"), (std::string)obj[1]);
        EXPECT_EQ(std::string("New String"), (std::string)obj[2]);
        EXPECT_EQ(std::string("Another String"), (std::string)obj[3]);
        EXPECT_EQ(std::string("Out of array"), (std::string)obj[4]);
    }
    
    TEST(test_map_validate, test_ArraySchemaItemTest)
    {
        using namespace NsAppLink::NsSmartObjects;
        CSmartObject obj;

        TSharedPtr<CArraySchemaItem> item = CArraySchemaItem::create(
                CStringSchemaItem::create(TSchemaItemParameter<size_t>(25)),
            TSchemaItemParameter<size_t>(2),
            TSchemaItemParameter<size_t>(4));
        
        obj["array"][0] = "Some String";
        
        int resultType = item->validate(obj["array"]);
        EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
        
        obj["array"][1] = "true";
        
        resultType = item->validate(obj["array"]);
        EXPECT_EQ(Errors::OK, resultType);
        
        obj["array"][2] = "New String";
        
        resultType = item->validate(obj["array"]);
        EXPECT_EQ(Errors::OK, resultType);

        obj["array"][3] = "Another String";
        
        resultType = item->validate(obj["array"]);
        EXPECT_EQ(Errors::OK, resultType);
        
        obj["array"][4] = "Out of array";
        
        resultType = item->validate(obj["array"]);
        EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        
        EXPECT_EQ(std::string("Some String"), (std::string)obj["array"][0]);
        EXPECT_EQ(std::string("true"), (std::string)obj["array"][1]);
        EXPECT_EQ(std::string("New String"), (std::string)obj["array"][2]);
        EXPECT_EQ(std::string("Another String"), (std::string)obj["array"][3]);
        EXPECT_EQ(std::string("Out of array"), (std::string)obj["array"][4]);
    }
}}}}}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}