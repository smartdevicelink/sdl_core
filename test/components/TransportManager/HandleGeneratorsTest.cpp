//
// Copyright (c) 2013 Ford Motor Company
//

/*
    Tests correct generation of Device and Connection handles used in TransportManager.
        - tests that generated handles are valid
        - generates 10000 handles
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../../../src/components/TransportManager/src/CTransportManager.hpp"

using ::testing::_;

namespace test
{
    namespace components
    {
        namespace TransportManager
        {
            namespace HandleGeneratorsTest
            {

                class TestTransportManager: public NsSmartDeviceLink::NsTransportManager::CTransportManager
                {
                public:
                    void run()
                    {
                        // Preventing parent function processing.
                        // Does not needed inside this test.
                    }
                };
            }
        }
    }
}

TEST(test_TransportManagerHandleGenerators, singleDeviceHandleCanBeGenerated)
{
    test::components::TransportManager::HandleGeneratorsTest::TestTransportManager tm;
    NsSmartDeviceLink::NsTransportManager::tDeviceHandle handle;

    handle = tm.generateNewDeviceHandle();
    ASSERT_NE(handle, NsSmartDeviceLink::NsTransportManager::InvalidDeviceHandle);
}

TEST(test_TransportManagerHandleGenerators, manyDeviceHandlesCanBeGenerated)
{
    test::components::TransportManager::HandleGeneratorsTest::TestTransportManager tm;
    NsSmartDeviceLink::NsTransportManager::tDeviceHandle handle;

    for (int i = 0; i < 10000; i++)
    {
        handle = tm.generateNewDeviceHandle();
        ASSERT_NE(handle, NsSmartDeviceLink::NsTransportManager::InvalidDeviceHandle);
    }
}

TEST(test_TransportManagerHandleGenerators, singleConnectionHandleCanBeGenerated)
{
    test::components::TransportManager::HandleGeneratorsTest::TestTransportManager tm;
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle handle;

    handle = tm.generateNewConnectionHandle();
    ASSERT_NE(handle, NsSmartDeviceLink::NsTransportManager::InvalidConnectionHandle);
}

TEST(test_TransportManagerHandleGenerators, manyConnectionHandlesCanBeGenerated)
{
    test::components::TransportManager::HandleGeneratorsTest::TestTransportManager tm;
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle handle;

    for (int i = 0; i < 10000; i++)
    {
        handle = tm.generateNewConnectionHandle();
        ASSERT_NE(handle, NsSmartDeviceLink::NsTransportManager::InvalidConnectionHandle);
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
