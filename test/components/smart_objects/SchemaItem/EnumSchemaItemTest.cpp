// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "smart_objects/smart_object.h"
#include "smart_objects/enum_schema_item.h"
#include "smart_objects/string_schema_item.h"

#include <string>


namespace test { namespace components { namespace SmartObjects { namespace SchemaItem {

    using namespace NsSmartDeviceLink::NsSmartObjects;
    namespace TestType {
        enum eType {
                INVALID_ENUM = -1,
                USER_EXIT = 0,
                IGNITION_OFF,
                BLUETOOTH_OFF,
                USB_DISCONNECTED,
                TOO_MANY_REQUESTS,
                MASTER_RESET,
                FACTORY_DEFAULTS,
                APP_UNAUTHORIZED
            };
        }

    class EnumSchemaItemTest : public ::testing::Test {
    public:

    protected:
        EnumSchemaItemTest() {
            testEnum.insert(TestType::USER_EXIT);
            testEnum.insert(TestType::IGNITION_OFF);
            testEnum.insert(TestType::BLUETOOTH_OFF);
            testEnum.insert(TestType::USB_DISCONNECTED);
            testEnum.insert(TestType::TOO_MANY_REQUESTS);
            testEnum.insert(TestType::MASTER_RESET);
            testEnum.insert(TestType::FACTORY_DEFAULTS);
            testEnum.insert(TestType::APP_UNAUTHORIZED);
        }

        virtual void SetUp() {
        }

        std::set<TestType::eType> testEnum;
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
        SmartObject obj;
        utils::SharedPtr<ISchemaItem> item = TEnumSchemaItem<TestType::eType>::create(testEnum,
            TSchemaItemParameter<TestType::eType>(TestType::FACTORY_DEFAULTS));

        //Object - valid enum
        obj = TestType::BLUETOOTH_OFF;
        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        bool resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(TestType::FACTORY_DEFAULTS, (int)obj);

        //Obj - bool
        obj = true;

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(TestType::FACTORY_DEFAULTS, (int)obj);

        //Object - number
        obj = 3.1415926;

        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(TestType::FACTORY_DEFAULTS, (int)obj);

        //Object - string
        obj = "Some string";
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);

        resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(TestType::FACTORY_DEFAULTS, (int)obj);
    }

    /**
     * Test EnumSchemaItem with default value
     *
     * Create SchemaItem without default value. Method setDefaultValue should return false,
     * SmartObject should contain previous value.
     **/
    TEST_F(EnumSchemaItemTest, test_item_without_default_value)
    {
        SmartObject obj;

        utils::SharedPtr<ISchemaItem> item = TEnumSchemaItem<TestType::eType>::create(testEnum,
            TSchemaItemParameter<TestType::eType>());

        //Object - valid enum
        obj = TestType::BLUETOOTH_OFF;
        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        bool resDefault = item->setDefaultValue(obj);
        EXPECT_FALSE(resDefault);
        EXPECT_EQ(TestType::BLUETOOTH_OFF, (int)obj);

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
        SmartObject obj;

        utils::SharedPtr<ISchemaItem> item = TEnumSchemaItem<TestType::eType>::create(testEnum,
            TSchemaItemParameter<TestType::eType>(TestType::FACTORY_DEFAULTS));

        //Object - valid enum
        obj = TestType::BLUETOOTH_OFF;
        int resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        bool resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_EQ(TestType::FACTORY_DEFAULTS, (int)obj);

        item->unapplySchema(obj);
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        EXPECT_EQ(std::string("FACTORY_DEFAULTS"), (std::string)obj);

        item->applySchema(obj);
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        EXPECT_EQ(TestType::FACTORY_DEFAULTS, (int)obj);

        obj = "TOO_MANY_REQUESTS";
        item->applySchema(obj);
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::OK, resultType);
        EXPECT_EQ(TestType::TOO_MANY_REQUESTS, (int)obj);

        obj = "ENOUGH_REQUESTS";
        item->applySchema(obj);
        resultType = item->validate(obj);
        EXPECT_EQ(Errors::INVALID_VALUE, resultType);
        EXPECT_EQ(std::string("ENOUGH_REQUESTS"), (std::string)obj);
    }

}}}}

namespace NsSmartDeviceLink { namespace NsSmartObjects {
    template<>
    const std::map<test::components::SmartObjects::SchemaItem::TestType::eType, std::string> & TEnumSchemaItem<test::components::SmartObjects::SchemaItem::TestType::eType>::getEnumElementsStringRepresentation(void)
    {
        static bool isInitialized = false;
        static std::map<test::components::SmartObjects::SchemaItem::TestType::eType, std::string> enumStringRepresentationMap;

        if (false == isInitialized)
        {
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::TestType::USER_EXIT, "USER_EXIT"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::TestType::IGNITION_OFF, "IGNITION_OFF"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::TestType::BLUETOOTH_OFF, "BLUETOOTH_OFF"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::TestType::USB_DISCONNECTED, "USB_DISCONNECTED"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::TestType::TOO_MANY_REQUESTS, "TOO_MANY_REQUESTS"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::TestType::MASTER_RESET, "MASTER_RESET"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::TestType::FACTORY_DEFAULTS, "FACTORY_DEFAULTS"));
            enumStringRepresentationMap.insert(std::make_pair(test::components::SmartObjects::SchemaItem::TestType::APP_UNAUTHORIZED, "APP_UNAUTHORIZED"));

            isInitialized = true;
        }

        return enumStringRepresentationMap;
    }
}}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
