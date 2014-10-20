//
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
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

/*
    Tests correct generation of Device and Connection handles used in TransportManager.
        - tests that generated handles are valid
        - generates 10000 handles
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "TransportManager/ITransportManager.hpp"

using ::testing::_;

namespace test
{
    namespace components
    {
        namespace TransportManager
        {
            namespace HandleGeneratorsTest
            {

                class TestTransportManager: public NsSmartDeviceLink::NsTransportManager::ITransportManager
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
