#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <string>
#include "test/components/JSONHandler/test_v4_protocol_v2_0_revP.hpp"

namespace test { namespace components { namespace JSONHandler { namespace SmartSchemaDraftTest {

    TEST(test_general, test_SmartSchemaDraftTest)
    {
        Gen::test::components::JSONHandler::test_v4_protocol_v2_0_revP factory;

        NsAppLink::NsSmartObjects::CSmartObject obj;
        obj["params"]["msg_type"] = Gen::test::components::JSONHandler::messageType::request;
        obj["params"]["function_id"] = Gen::test::components::JSONHandler::RegisterAppInterfaceID;

        ASSERT_TRUE(factory.attachSchema(obj));

        obj["params"]["msg_type"] = Gen::test::components::JSONHandler::messageType::notification;

        ASSERT_FALSE(factory.attachSchema(obj));
    }
}}}}

int main(int argc, char **argv) {
    //PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}