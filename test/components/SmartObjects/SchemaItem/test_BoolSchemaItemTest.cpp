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

#include "SmartObjects/CSmartObject.hpp"
#include "SmartObjects/CBoolSchemaItem.hpp"

#include <string>


namespace test { namespace components { namespace SmartObjects { namespace SchemaItem { namespace SchemaItemDraftTest {

    /**
     * Test BoolSchemaItem no default value
     *
     * Create SchemaItem without default value. Method setDefaultValue should always return false
     * and leave SmartObject in previous state.
     **/
    TEST(test_no_default_value, test_BoolSchemaItemTest)
    {
        NsSmartDeviceLink::NsSmartObjects::CSmartObject obj;

        utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem> item =
            NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem::create(); // No default value

        obj = 5;
        ASSERT_EQ(5, (int)obj);

        int resultType = item->validate(obj);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE, resultType);

        obj = true;
        ASSERT_TRUE((bool)obj);

        resultType = item->validate(obj);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, resultType);
        bool resDefault = item->setDefaultValue(obj);
        EXPECT_FALSE(resDefault);
        EXPECT_TRUE((bool)obj);

        obj = "Test";
        ASSERT_EQ(std::string("Test"), (std::string)obj);

        resultType = item->validate(obj);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE, resultType);
        resDefault = item->setDefaultValue(obj);
        EXPECT_FALSE(resDefault);
        resultType = item->validate(obj);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE, resultType);
    }

    /**
     * Test BoolSchemaItem with default value
     *
     * Create SchemaItem with default value. Method setDefaultValue should return true,
     * Bool SmartObject should contain default value.
     * Not bool SmartObject should converted to BoolObject and setted up by default value.
     **/
    TEST(test_item_with_default_value, test_BoolSchemaItemTest)
    {
        NsSmartDeviceLink::NsSmartObjects::CSmartObject obj;

        utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem> item =
            NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem::create(false); // Default value = false

        obj = 5;
        ASSERT_EQ(5, (int)obj);

        int resultType = item->validate(obj);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE, resultType);

        obj = true;
        ASSERT_TRUE((bool)obj);

        resultType = item->validate(obj);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, resultType);
        bool resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_FALSE((bool)obj);

        obj = "Test";
        ASSERT_EQ(std::string("Test"), (std::string)obj);

        resultType = item->validate(obj);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE, resultType);
        resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_FALSE((bool)obj);

        resultType = item->validate(obj);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, resultType);
        EXPECT_FALSE((bool)obj);
    }

    TEST(test_map_validate, test_BoolSchemaItemTest)
    {
        NsSmartDeviceLink::NsSmartObjects::CSmartObject obj;
        utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem> item =
            NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem::create(false); // Default value = false

        obj["aa"] = true;
        ASSERT_TRUE((bool)obj["aa"]);

        int resultType = item->validate(obj["aa"]);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, resultType);

        resultType = item->validate(obj);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE, resultType);

        bool resDefault = item->setDefaultValue(obj["aa"]);
        EXPECT_TRUE(resDefault);
        EXPECT_FALSE((bool)obj["aa"]);

        resDefault = item->setDefaultValue(obj);
        EXPECT_TRUE(resDefault);
        EXPECT_FALSE((bool)obj);

        resultType = item->validate(obj);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, resultType);

        obj["ind"] = true;
        resultType = item->validate(obj);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE, resultType);
    }

    TEST(test_array_validate, test_BoolSchemaItemTest)
    {
        NsSmartDeviceLink::NsSmartObjects::CSmartObject obj;
        utils::SharedPtr<NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem> item =
            NsSmartDeviceLink::NsSmartObjects::CBoolSchemaItem::create();

        obj[0] = true;
        obj[1] = false;

        ASSERT_TRUE((bool)obj[0]);
        ASSERT_FALSE((bool)obj[1]);

        int resultType = item->validate(obj[0]);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, resultType);

        resultType = item->validate(obj[1]);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, resultType);

        resultType = item->validate(obj);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE, resultType);

        bool resDefault = item->setDefaultValue(obj[0]);
        EXPECT_FALSE(resDefault);
        EXPECT_TRUE((bool)obj[0]);

        resDefault = item->setDefaultValue(obj);
        EXPECT_FALSE(resDefault);
        EXPECT_FALSE((bool)obj[1]);

        resultType = item->validate(obj);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::INVALID_VALUE, resultType);

        obj = false;

        resultType = item->validate(obj);
        EXPECT_EQ(NsSmartDeviceLink::NsSmartObjects::Errors::OK, resultType);
    }

}}}}}

int main(int argc, char **argv) {
  //PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
