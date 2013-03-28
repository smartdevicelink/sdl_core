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
    Tests correct registration/removal of various TransportManager listeners.
        - tests that all registered device listeners received callbacks
        - tests that all registered data listeners received callbacks
        - tests that not registered device listeners was not received callbacks
        - tests that not registered data listeners was not received callbacks
        - tests that registered and then removed device listeners was not received callbacks
        - tests that registered and then removed data listeners was not received callbacks
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../../../src/components/TransportManager/src/CTransportManager.hpp"
#include "TransportManager/ITransportManagerDataListener.hpp"
#include "TransportManager/ITransportManagerDeviceListener.hpp"

using ::testing::_;

namespace test
{
    namespace components
    {
        namespace TransportManager
        {
            namespace ListenersTest
            {
                
                /**
                 * @brief Class to check data listener callbacks calling
                 **/
                class MockDataListener: public NsSmartDeviceLink::NsTransportManager::ITransportManagerDataListener
                {
                public:
                    //MOCK_METHOD2(onFrameReceived, void(NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle, const Blob & Data));
                    MOCK_METHOD3(onFrameSendCompleted, void(NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle, int FrameSequenceNumber, NsSmartDeviceLink::NsTransportManager::ESendStatus SendStatus));
                };

                /**
                 * @brief Class to check device listener callbacks calling
                 **/
                class MockDeviceListener: public NsSmartDeviceLink::NsTransportManager::ITransportManagerDeviceListener
                {
                public:
                    MOCK_METHOD1(onDeviceListUpdated, void(const NsSmartDeviceLink::NsTransportManager::tDeviceList & DeviceList));
                    MOCK_METHOD2(onApplicationConnected, void(const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & ConnectedDevice, const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection));
                    MOCK_METHOD2(onApplicationDisconnected, void(const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & DisconnectedDevice, const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection));
                };

                class TestTransportManager: public NsSmartDeviceLink::NsTransportManager::CTransportManager
                {
                public:
                    void run()
                    {
                        // Preventing parent function processing.
                        // Does not needed inside this test.
                    }

                    void test()
                    {
                        //Calling callbacks on all listeners

                        NsSmartDeviceLink::NsTransportManager::tConnectionHandle dummyConnectionHandle = 1;
                        std::vector<NsSmartDeviceLink::NsTransportManager::ITransportManagerDataListener*>::const_iterator dataListenersIterator;

                        for (dataListenersIterator = mDataListeners.begin(); dataListenersIterator != mDataListeners.end(); ++dataListenersIterator)
                        {
                            (*dataListenersIterator)->onFrameSendCompleted(dummyConnectionHandle, 0, NsSmartDeviceLink::NsTransportManager::SendStatusOK);
                        }

                        NsSmartDeviceLink::NsTransportManager::SDeviceInfo dummyDeviceInfo;
                        std::vector<NsSmartDeviceLink::NsTransportManager::ITransportManagerDeviceListener*>::const_iterator deviceListenersIterator;

                        for (deviceListenersIterator = mDeviceListeners.begin(); deviceListenersIterator != mDeviceListeners.end(); ++deviceListenersIterator)
                        {
                            (*deviceListenersIterator)->onApplicationConnected(dummyDeviceInfo, NsSmartDeviceLink::NsTransportManager::InvalidConnectionHandle);
                        }
                    }
                };
            }
        }
    }
}

TEST(test_TransportManagerListeners, allRegisteredDataListenersCalled)
{
    test::components::TransportManager::ListenersTest::MockDataListener mockDataListener1;
    test::components::TransportManager::ListenersTest::MockDataListener mockDataListener2;
    
    EXPECT_CALL(mockDataListener1, onFrameSendCompleted(_, _, _))
        .Times(1)
    ;

    EXPECT_CALL(mockDataListener2, onFrameSendCompleted(_, _, _))
        .Times(1)
    ;

    test::components::TransportManager::ListenersTest::TestTransportManager tm;
    tm.addDataListener(&mockDataListener1);
    tm.addDataListener(&mockDataListener2);

    tm.test();
}

TEST(test_TransportManagerListeners, allRegisteredDeviceListenersCalled)
{
    test::components::TransportManager::ListenersTest::MockDeviceListener mockDeviceListener1;
    test::components::TransportManager::ListenersTest::MockDeviceListener mockDeviceListener2;

    EXPECT_CALL(mockDeviceListener1, onApplicationConnected(_, _))
        .Times(1)
    ;

    EXPECT_CALL(mockDeviceListener2, onApplicationConnected(_, _))
        .Times(1)
    ;

    test::components::TransportManager::ListenersTest::TestTransportManager tm;
    tm.addDeviceListener(&mockDeviceListener1);
    tm.addDeviceListener(&mockDeviceListener2);

    tm.test();
}

TEST(test_TransportManagerListeners, notRegisteredDataListenersAreNotCalled)
{
    test::components::TransportManager::ListenersTest::MockDataListener mockDataListener1;
    test::components::TransportManager::ListenersTest::MockDataListener mockDataListener2;

    EXPECT_CALL(mockDataListener1, onFrameSendCompleted(_, _, _))
        .Times(0)
    ;

    EXPECT_CALL(mockDataListener2, onFrameSendCompleted(_, _, _))
        .Times(0)
    ;

    test::components::TransportManager::ListenersTest::TestTransportManager tm;
    tm.test();
}

TEST(test_TransportManagerListeners, notRegisteredDeviceListenersAreNotCalled)
{
    test::components::TransportManager::ListenersTest::MockDeviceListener mockDeviceListener1;
    test::components::TransportManager::ListenersTest::MockDeviceListener mockDeviceListener2;

    EXPECT_CALL(mockDeviceListener1, onApplicationConnected(_, _))
        .Times(0)
    ;

    EXPECT_CALL(mockDeviceListener2, onApplicationConnected(_, _))
        .Times(0)
    ;

    test::components::TransportManager::ListenersTest::TestTransportManager tm;
    tm.test();
}

TEST(test_TransportManagerListeners, dataListenersCanBeRemoved)
{
    test::components::TransportManager::ListenersTest::MockDataListener mockDataListener1;
    test::components::TransportManager::ListenersTest::MockDataListener mockDataListener2;

    EXPECT_CALL(mockDataListener1, onFrameSendCompleted(_, _, _))
        .Times(1)
    ;

    EXPECT_CALL(mockDataListener2, onFrameSendCompleted(_, _, _))
        .Times(0)
    ;

    test::components::TransportManager::ListenersTest::TestTransportManager tm;
    tm.addDataListener(&mockDataListener1);
    tm.addDataListener(&mockDataListener2);
    tm.removeDataListener(&mockDataListener2);

    tm.test();
}

TEST(test_TransportManagerListeners, deviceListenersCanBeRemoved)
{
    test::components::TransportManager::ListenersTest::MockDeviceListener mockDeviceListener1;
    test::components::TransportManager::ListenersTest::MockDeviceListener mockDeviceListener2;

    EXPECT_CALL(mockDeviceListener1, onApplicationConnected(_, _))
        .Times(1)
    ;

    EXPECT_CALL(mockDeviceListener2, onApplicationConnected(_, _))
        .Times(0)
    ;

    test::components::TransportManager::ListenersTest::TestTransportManager tm;
    tm.addDeviceListener(&mockDeviceListener1);
    tm.addDeviceListener(&mockDeviceListener2);
    tm.removeDeviceListener(&mockDeviceListener2);

    tm.test();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
