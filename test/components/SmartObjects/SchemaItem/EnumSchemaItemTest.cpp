#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SmartObjects/CSmartObject.hpp"
#include "SmartObjects/TEnumSchemaItem.hpp"
#include "SmartObjects/CStringSchemaItem.hpp"

#include <string>


namespace test { namespace components { namespace SmartObjects { namespace SchemaItem {

    using namespace NsSmartDeviceLink::NsSmartObjects;

    class EnumSchemaItemTest : public ::testing::Test {
    public:
        enum eTestType {
                USER_EXIT = 0,
                IGNITION_OFF,
                BLUETOOTH_OFF,
                USB_DISCONNECTED,
                TOO_MANY_REQUESTS,
                MASTER_RESET,
                FACTORY_DEFAULTS,
                APP_UNAUTHORIZED
            };
    protected:
        EnumSchemaItemTest() {
            testEnum.insert(USER_EXIT);
            testEnum.insert(IGNITION_OFF);
            testEnum.insert(BLUETOOTH_OFF);
            testEnum.insert(USB_DISCONNECTED);
            testEnum.insert(TOO_MANY_REQUESTS);
            testEnum.insert(MASTER_RESET);
            testEnum.insert(FACTORY_DEFAULTS);
            testEnum.insert(APP_UNAUTHORIZED);
        }

        virtual void SetUp() {
        }

        std::set<eTestType> testEnum;
    };

    /**
     * Test EnumSchemaItem
     *
     * Create SchemaItem with default value. Method setDefaultValue should return true,
     * SmartObject should contain default value.
     * Not Enum SmartObject should converted to intObject and setted up by the default value.
     **/
    TEST_F(EnumSchemaItemTest, test_item_with_default_value)
    {
        CSmartObject obj;
        TSharedPtr<ISchemaItem> item = TEnumSchemaItem<eTestType>::create(testEnum,
            TSchemaItemParameter<eTestType>(FACTORY_DEFAULTS));

        //Object - valid enum
        obj = eTestType::BLUETOOTH_OFF;
        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        bool resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(eTestType::FACTORY_DEFAULTS, (int)obj);

        //Obj - bool
        obj = true;

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(eTestType::FACTORY_DEFAULTS, (int)obj);

        //Object - number
        obj = 3.1415926;

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(eTestType::FACTORY_DEFAULTS, (int)obj);

        //Object - string
        obj = "Some string";
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(eTestType::FACTORY_DEFAULTS, (int)obj);
    }

    /**
     * Test EnumSchemaItem with default value
     *
     * Create SchemaItem without default value. Method setDefaultValue should return false,
     * SmartObject should contain previous value.
     **/
    TEST_F(EnumSchemaItemTest, test_item_without_default_value)
    {
        CSmartObject obj;

        TSharedPtr<ISchemaItem> item = TEnumSchemaItem<eTestType>::create(testEnum,
            TSchemaItemParameter<eTestType>());

        //Object - valid enum
        obj = eTestType::BLUETOOTH_OFF;
        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        bool resDefault = item->setDefaultValue(obj);
        EXPECT_FALSE(resDefault);
        EXPECT_EQ(eTestType::BLUETOOTH_OFF, (int)obj);

        //Obj - bool
        obj = true;

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resDefault = item->setDefaultValue(obj);
        EXPECT_FALSE(resDefault);
        EXPECT_TRUE((bool)obj);

        //Object - number
        obj = 3.1415926;

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        resDefault = item->setDefaultValue(obj);
        EXPECT_FALSE(resDefault);
        EXPECT_EQ(3.1415926, (double)obj);

        //Object - string
        obj = "Some string";
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        resDefault = item->setDefaultValue(obj);
        EXPECT_FALSE(resDefault);
        EXPECT_EQ(std::string("Some string"), (std::string)obj);

        //Object - int in range of enum
        obj = 6;
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);

        //Object - int out of enum range
        obj = 15;
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OUT_OF_RANGE, resultType);
    }

    /**
     * Test apply and unapply EnumSchemaItem
     **/
    TEST_F(EnumSchemaItemTest, test_apply_unapply_schema)
    {
        CSmartObject obj;

        TSharedPtr<ISchemaItem> item = TEnumSchemaItem<eTestType>::create(testEnum,
            TSchemaItemParameter<eTestType>(FACTORY_DEFAULTS));

        //Object - valid enum
        obj = eTestType::BLUETOOTH_OFF;
        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        bool resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(eTestType::FACTORY_DEFAULTS, (int)obj);

        item->unapplySchema(obj);
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        EXPECT_EQ(std::string("FACTORY_DEFAULTS"), (std::string)obj);

        item->applySchema(obj);
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        EXPECT_EQ(eTestType::FACTORY_DEFAULTS, (int)obj);

        obj = "TOO_MANY_REQUESTS";
        item->applySchema(obj);
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        EXPECT_EQ(eTestType::TOO_MANY_REQUESTS, (int)obj);

        obj = "ENOUGH_REQUESTS";
        item->applySchema(obj);
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        EXPECT_EQ(std::string("ENOUGH_REQUESTS"), (std::string)obj);
    }

}}}}

namespace NsSmartDeviceLink { namespace NsSmartObjects {
    template<>
    const std::map<test::components::SmartObjects::SchemaItem::EnumSchemaItemTest::eTestType, std::string> & TEnumSchemaItem<test::components::SmartObjects::SchemaItem::EnumSchemaItemTest::eTestType>::getEnumElementsStringRepresentation(void)
    {
        static bool isInitialized = false;
        static std::map<test::components::SmartObjects::SchemaItem::EnumSchemaItemTest::eTestType, std::string> enumStringRepresentationMap;

        if (false == isInitialized)
        {
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::EnumSchemaItemTest::eTestType::USER_EXIT, "USER_EXIT"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::EnumSchemaItemTest::eTestType::IGNITION_OFF, "IGNITION_OFF"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::EnumSchemaItemTest::eTestType::BLUETOOTH_OFF, "BLUETOOTH_OFF"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::EnumSchemaItemTest::eTestType::USB_DISCONNECTED, "USB_DISCONNECTED"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::EnumSchemaItemTest::eTestType::TOO_MANY_REQUESTS, "TOO_MANY_REQUESTS"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::EnumSchemaItemTest::eTestType::MASTER_RESET, "MASTER_RESET"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::EnumSchemaItemTest::eTestType::FACTORY_DEFAULTS, "FACTORY_DEFAULTS"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::EnumSchemaItemTest::eTestType::APP_UNAUTHORIZED, "APP_UNAUTHORIZED"));

            isInitialized = true;
        }

        return enumStringRepresentationMap;
    }
}}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}