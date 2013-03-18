//
// Copyright (c) 2013 Ford Motor Company
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
