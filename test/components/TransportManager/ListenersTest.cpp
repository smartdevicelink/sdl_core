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
                class MockDataListener: public NsAppLink::NsTransportManager::ITransportManagerDataListener
                {
                public:
                    //MOCK_METHOD2(onFrameReceived, void(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const Blob & Data));
                    MOCK_METHOD3(onFrameSendCompleted, void(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, int FrameSequenceNumber, NsAppLink::NsTransportManager::ESendStatus SendStatus));
                };

                /**
                 * @brief Class to check device listener callbacks calling
                 **/
                class MockDeviceListener: public NsAppLink::NsTransportManager::ITransportManagerDeviceListener
                {
                public:
                    MOCK_METHOD1(onDeviceListUpdated, void(const NsAppLink::NsTransportManager::tDeviceList & DeviceList));
                    MOCK_METHOD1(onDeviceConnected, void(const NsAppLink::NsTransportManager::SDeviceInfo & ConnectedDevice));
                    MOCK_METHOD1(onDeviceDisconnected, void(const NsAppLink::NsTransportManager::SDeviceInfo & DisconnectedDevice));
                };

                class TestTransportManager: public NsAppLink::NsTransportManager::CTransportManager
                {
                public:
                    void test()
                    {
                        //Calling callbacks on all listeners

                        NsAppLink::NsTransportManager::tConnectionHandle dummyConnectionHandle;
                        std::vector<NsAppLink::NsTransportManager::ITransportManagerDataListener*>::const_iterator dataListenersIterator;                        

                        for (dataListenersIterator = mDataListeners.begin(); dataListenersIterator != mDataListeners.end(); ++dataListenersIterator)
                        {
                            (*dataListenersIterator)->onFrameSendCompleted(dummyConnectionHandle, 0, NsAppLink::NsTransportManager::SendStatusOK);
                        }

                        NsAppLink::NsTransportManager::SDeviceInfo dummyDeviceInfo;
                        std::vector<NsAppLink::NsTransportManager::ITransportManagerDeviceListener*>::const_iterator deviceListenersIterator;

                        for (deviceListenersIterator = mDeviceListeners.begin(); deviceListenersIterator != mDeviceListeners.end(); ++deviceListenersIterator)
                        {
                            (*deviceListenersIterator)->onDeviceConnected(dummyDeviceInfo);
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

    EXPECT_CALL(mockDeviceListener1, onDeviceConnected(_))
        .Times(1)
    ;

    EXPECT_CALL(mockDeviceListener2, onDeviceConnected(_))
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

    EXPECT_CALL(mockDeviceListener1, onDeviceConnected(_))
        .Times(0)
    ;

    EXPECT_CALL(mockDeviceListener2, onDeviceConnected(_))
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

    EXPECT_CALL(mockDeviceListener1, onDeviceConnected(_))
        .Times(1)
    ;

    EXPECT_CALL(mockDeviceListener2, onDeviceConnected(_))
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