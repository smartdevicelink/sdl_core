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

#ifndef TEST_COMPONENTS_SMARTOBJECTS_SMARTOBJECTDRAFTTEST_H_
#define TEST_COMPONENTS_SMARTOBJECTS_SMARTOBJECTDRAFTTEST_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "smart_objects/smart_object.h"
#include <string>

using ::testing::ElementsAre;
using ::testing::ContainerEq;

namespace test { namespace components { namespace SmartObjects { namespace SmartObjectDraftTest {

    TEST(test_primitive_types, test_SmartObjectsDraftTest)
    {
        NsSmartDeviceLink::NsSmartObjects::SmartObject obj;

        obj = 5;
        ASSERT_EQ(5, obj.asInt());

        obj = true;
        ASSERT_TRUE(obj.asBool());

        obj = "Test";
        ASSERT_EQ(std::string("Test"), obj.asString());
    }

    TEST(test_map_access, test_SmartObjectsDraftTest)
    {
        NsSmartDeviceLink::NsSmartObjects::SmartObject obj;

        obj["aa"] = true;
        ASSERT_TRUE(obj["aa"].asInt());

        obj["aa"]["fds"]["Fsdf"] = 123;
        ASSERT_EQ(123, obj["aa"]["fds"]["Fsdf"].asInt());
    }

    TEST(test_array_access, test_SmartObjectsDraftTest)
    {
        NsSmartDeviceLink::NsSmartObjects::SmartObject obj;

        obj[0] = 5;
        obj[-1] = 6; // Appending new item to array

        ASSERT_EQ(5, obj[0].asInt());
        ASSERT_EQ(6, obj[1].asInt());
    }

    TEST(test_public_interface, test_SmartObjectsDraftTest)
    {
        NsSmartDeviceLink::NsSmartObjects::SmartObject obj;

        // ---- INTEGER ---- //
        obj = 1;
        ASSERT_EQ(1, obj.asInt());

        // ---- unsigned int ---- //
        obj = static_cast<unsigned int>(100);
        ASSERT_EQ(100u, obj.asUInt());

        // ---- DOUBLE ---- //
        obj = 3.14;
        ASSERT_EQ(3.14, obj.asDouble());

        // ---- CHAR ---- //
        obj = 'a';
        ASSERT_EQ('a', obj.asChar());

        // ---- BOOL ---- //
        obj = true;
        ASSERT_TRUE(obj.asBool());

        // ---- CHAR* ---- //
        obj = "Hello, world";
        ASSERT_EQ(std::string("Hello, world"), obj.asString());

        // ---- STD::STRING ---- //
        obj = std::string("Hello, world");
        ASSERT_EQ(std::string("Hello, world"), obj.asString());

        // ---- Binary ---- //
        NsSmartDeviceLink::NsSmartObjects::SmartBinary binaryData;
        binaryData.push_back('\0');
        binaryData.push_back('a');
        obj = binaryData;
        ASSERT_THAT(obj.asBinary(), ElementsAre('\0', 'a'));

        // ---- ARRAY ---- //
        obj[0] = 1;
        obj[1] = true;
        obj[2] = 'a';
        obj[3] = 3.14;

        ASSERT_EQ(1, obj[0].asInt());
        ASSERT_TRUE(obj[1].asBool());
        ASSERT_EQ('a', obj[2].asChar());
        ASSERT_EQ(3.14, obj[3].asDouble());

        // ---- DEEP ARRAY ---- //
        obj[0] = 1;
        obj[1][0] = 3.14;
        obj[1][1][0] = true;

        ASSERT_EQ(1, obj[0].asInt());
        ASSERT_EQ(3.14, obj[1][0].asDouble());
        ASSERT_TRUE(obj[1][1][0].asBool());

        // ---- MAP ---- //
        obj["name"] = "My name";
        obj["count"] = 10;
        obj["isValid"] = true;

        ASSERT_EQ(std::string("My name"), obj["name"].asString());
        ASSERT_EQ(10, obj["count"].asInt());
        ASSERT_TRUE(obj["isValid"].asBool());

        // ---- DEEP MAP ---- //
        obj["request"]["name"] = "My Request";
        obj["request"]["id"] = 123;
        obj["response"]["name"] = "My Response";
        obj["response"]["id"] = 456;
        obj["we"]["need"]["to"]["go"]["deeper"] = true;

        ASSERT_EQ(std::string("My Request"), obj["request"]["name"].asString());
        ASSERT_EQ(123, obj["request"]["id"].asInt());
        ASSERT_EQ(std::string("My Response"), obj["response"]["name"].asString());
        ASSERT_EQ(456, obj["response"]["id"].asInt());
        ASSERT_TRUE(obj["we"]["need"]["to"]["go"]["deeper"].asBool());
    }

    TEST(test_helper_methods, test_SmartObjectsDraftTest)
    {
        NsSmartDeviceLink::NsSmartObjects::SmartObject obj;

        // ---- INTEGER ---- //
        obj = 1;
        ASSERT_EQ(1, obj.asInt());

        // ---- unsigned int ---- //
        obj = static_cast<unsigned int>(100);
        ASSERT_EQ(100u, obj.asUInt());

        // ---- DOUBLE ---- //
        obj = 3.14;
        ASSERT_EQ(3.14, obj.asDouble());
        TEST_COMPONENTS_SMARTOBJECTS_SMARTOBJECTDRAFTTEST_H_
        // ---- CHAR ---- //
        obj = 'a';
        ASSERT_EQ('a', obj.asChar());

        // ---- BOOL ---- //
        obj = true;
        ASSERT_TRUE(obj.asBool());

        // ---- STD::STRING ---- //
        obj = std::string("Hello, world");
        ASSERT_EQ(std::string("Hello, world"), obj.asString());

        // ---- Binary ---- //
        NsSmartDeviceLink::NsSmartObjects::SmartBinary binaryData;
        binaryData.push_back('\0');
        binaryData.push_back('a');
        obj = binaryData;
        ASSERT_THAT(obj.asBinary(), ElementsAre('\0', 'a'));
    }

}}}}

#endif // TEST_COMPONENTS_SMARTOBJECTS_SMARTOBJECTDRAFTTEST_H_
