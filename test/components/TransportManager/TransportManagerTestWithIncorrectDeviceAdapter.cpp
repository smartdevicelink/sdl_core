/*
    Tests TransportManager behavior when device adapter behaves completely incorrectly.
    Tested cases are:
        - receiving data without available devices
        - receiving frameSendCompleted statuses without available devices/connections
        - receiving application disconnects without connected applications
        - receiving new frames with invalid/null data or it's size
        - receiving application disconnects for already disconnected applications
        - receiving application connect without available devices
 */
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "TransportManager/ITransportManagerDataListener.hpp"
#include "TransportManager/ITransportManagerDeviceListener.hpp"
#include "../../../src/components/TransportManager/src/CTransportManager.hpp"
#include "../../../src/components/TransportManager/src/IDeviceAdapter.hpp"

namespace test
{
    namespace components
    {
        namespace TransportManger
        {
            namespace IncorrectDeviceAdapterTest
            {
                class MockDataListener: public NsSmartDeviceLink::NsTransportManager::ITransportManagerDataListener
                {
                public:
                    MOCK_METHOD3(onFrameReceived, void(NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize));
                    MOCK_METHOD3(onFrameSendCompleted, void(NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle, int UserData, NsSmartDeviceLink::NsTransportManager::ESendStatus SendStatus));
                };

                class MockDeviceListener: public NsSmartDeviceLink::NsTransportManager::ITransportManagerDeviceListener
                {
                public:
                    MOCK_METHOD1(onDeviceListUpdated, void(const NsSmartDeviceLink::NsTransportManager::tDeviceList & DeviceList));
                    MOCK_METHOD2(onApplicationConnected, void(const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & ConnectedDevice, const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection));
                    MOCK_METHOD2(onApplicationDisconnected, void(const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & DisconnectedDevice, const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection));
                };

                class MockDeviceAdapter: public NsSmartDeviceLink::NsTransportManager::IDeviceAdapter
                {
                public:
                    virtual NsSmartDeviceLink::NsTransportManager::EDeviceType getDeviceType(void) const;

                    MOCK_METHOD1(connectDevice, void (const NsSmartDeviceLink::NsTransportManager::tDeviceHandle DeviceHandle));
                    MOCK_METHOD1(disconnectDevice, void (const NsSmartDeviceLink::NsTransportManager::tDeviceHandle DeviceHandle));
                    MOCK_METHOD0(run, void());
                    MOCK_METHOD0(scanForNewDevices, void());
                    MOCK_METHOD4(sendFrame, void(NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize, int UserData));
                };

                class TestTransportManager: public NsSmartDeviceLink::NsTransportManager::CTransportManager
                {
                public:
                    TestTransportManager(MockDeviceAdapter * DeviceAdapter);
                    ~TestTransportManager(void);
                protected:
                    virtual void initializeDeviceAdapters(void);

                private:
                    MockDeviceAdapter * mMockDeviceAdapter;
                };
            }
        }
    }
}

NsSmartDeviceLink::NsTransportManager::EDeviceType test::components::TransportManger::IncorrectDeviceAdapterTest::MockDeviceAdapter::getDeviceType(void) const
{
    return NsSmartDeviceLink::NsTransportManager::DeviceBluetooth;
}

test::components::TransportManger::IncorrectDeviceAdapterTest::TestTransportManager::TestTransportManager(test::components::TransportManger::IncorrectDeviceAdapterTest::MockDeviceAdapter* DeviceAdapter):
CTransportManager(),
mMockDeviceAdapter(DeviceAdapter)
{
}

test::components::TransportManger::IncorrectDeviceAdapterTest::TestTransportManager::~TestTransportManager(void)
{
}

void test::components::TransportManger::IncorrectDeviceAdapterTest::TestTransportManager::initializeDeviceAdapters(void)
{
    addDeviceAdapter(mMockDeviceAdapter);
}

TEST(TransportManager, IncorrectDeviceAdapter)
{
    test::components::TransportManger::IncorrectDeviceAdapterTest::MockDeviceAdapter* mockDeviceAdapter = new test::components::TransportManger::IncorrectDeviceAdapterTest::MockDeviceAdapter();
    test::components::TransportManger::IncorrectDeviceAdapterTest::MockDataListener mockDataListener;
    test::components::TransportManger::IncorrectDeviceAdapterTest::MockDeviceListener mockDeviceListener;
    test::components::TransportManger::IncorrectDeviceAdapterTest::TestTransportManager transportManager(mockDeviceAdapter);

    EXPECT_CALL(*mockDeviceAdapter, run()).Times(1);

    transportManager.run();

    transportManager.addDeviceListener(&mockDeviceListener);
    transportManager.addDataListener(&mockDataListener);

    NsSmartDeviceLink::NsTransportManager::tDeviceHandle validDeviceHandle = 123;
    NsSmartDeviceLink::NsTransportManager::tDeviceHandle validConnectionHandle = 321;

    NsSmartDeviceLink::NsTransportManager::SDeviceInfo deviceInfo;
    deviceInfo.mDeviceHandle = validDeviceHandle;
    deviceInfo.mUniqueDeviceId = "id";
    deviceInfo.mUserFriendlyName = "Name";

    NsSmartDeviceLink::NsTransportManager::tDeviceList deviceList;
    deviceList.push_back(deviceInfo);

    NsSmartDeviceLink::NsTransportManager::tInternalDeviceList internalDeviceList;
    internalDeviceList.push_back(NsSmartDeviceLink::NsTransportManager::SInternalDeviceInfo(deviceInfo.mDeviceHandle, deviceInfo.mUserFriendlyName, deviceInfo.mUniqueDeviceId));

    uint8_t validFrameData[16] = {0x11, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08};
    uint8_t invalidFrameData[16] = {0x51, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08};

    EXPECT_CALL(mockDeviceListener, onDeviceListUpdated(::testing::_)).Times(0);
    EXPECT_CALL(mockDeviceListener, onApplicationConnected(::testing::_, ::testing::_)).Times(0);
    EXPECT_CALL(mockDeviceListener, onApplicationDisconnected(::testing::_, ::testing::_)).Times(0);
    EXPECT_CALL(mockDataListener, onFrameReceived(::testing::_, ::testing::_, ::testing::_)).Times(0);
    EXPECT_CALL(mockDataListener, onFrameSendCompleted(::testing::_, ::testing::_, ::testing::_)).Times(0);

    transportManager.onFrameReceived(mockDeviceAdapter, validConnectionHandle, 0, 5);

    transportManager.onFrameReceived(mockDeviceAdapter, validConnectionHandle, validFrameData, 0);

    transportManager.onFrameReceived(mockDeviceAdapter, validConnectionHandle, validFrameData, sizeof(validFrameData));

    transportManager.onFrameSendCompleted(mockDeviceAdapter, validConnectionHandle, 20, NsSmartDeviceLink::NsTransportManager::SendStatusOK);

    transportManager.onApplicationDisconnected(mockDeviceAdapter, deviceInfo, validConnectionHandle);

    transportManager.onApplicationConnected(mockDeviceAdapter, deviceInfo, validConnectionHandle);

    EXPECT_CALL(mockDeviceListener, onDeviceListUpdated(::testing::ContainerEq(deviceList))).Times(1).RetiresOnSaturation();
    transportManager.onDeviceListUpdated(mockDeviceAdapter, internalDeviceList);

    transportManager.onApplicationDisconnected(mockDeviceAdapter, deviceInfo, validConnectionHandle);

    EXPECT_CALL(mockDeviceListener, onApplicationConnected(::testing::Eq(deviceInfo), validConnectionHandle)).Times(1).RetiresOnSaturation();
    transportManager.onApplicationConnected(mockDeviceAdapter, deviceInfo, validConnectionHandle);

    transportManager.onFrameReceived(mockDeviceAdapter, validConnectionHandle, 0, 5);

    transportManager.onFrameReceived(mockDeviceAdapter, validConnectionHandle, validFrameData, 0);

    EXPECT_CALL(mockDataListener, onFrameReceived(validConnectionHandle, ::testing::_, sizeof(validFrameData))).Times(1).RetiresOnSaturation();
    transportManager.onFrameReceived(mockDeviceAdapter, validConnectionHandle, validFrameData, sizeof(validFrameData));

    transportManager.onFrameReceived(mockDeviceAdapter, validConnectionHandle, invalidFrameData, sizeof(invalidFrameData));

    // Frame result is now expected even if we do not send any frame before
    EXPECT_CALL(mockDataListener, onFrameSendCompleted(validConnectionHandle, 20, NsSmartDeviceLink::NsTransportManager::SendStatusOK)).Times(1).RetiresOnSaturation();
    transportManager.onFrameSendCompleted(mockDeviceAdapter, validConnectionHandle, 20, NsSmartDeviceLink::NsTransportManager::SendStatusOK);

    int userData = 123;
    transportManager.sendFrame(validConnectionHandle, validFrameData, sizeof(validFrameData), userData);
    EXPECT_CALL(mockDataListener, onFrameSendCompleted(validConnectionHandle, userData, NsSmartDeviceLink::NsTransportManager::SendStatusOK)).Times(1).RetiresOnSaturation();
    transportManager.onFrameSendCompleted(mockDeviceAdapter, validConnectionHandle, userData, NsSmartDeviceLink::NsTransportManager::SendStatusOK);

    sleep(1); //We must wait for sending previous callbacks while applicationDisconnect can shutdown connection thread
    
    EXPECT_CALL(mockDeviceListener, onApplicationDisconnected(::testing::Eq(deviceInfo), validConnectionHandle)).Times(1).RetiresOnSaturation();
    transportManager.onApplicationDisconnected(mockDeviceAdapter, deviceInfo, validConnectionHandle);

    transportManager.onApplicationDisconnected(mockDeviceAdapter, deviceInfo, validConnectionHandle);

    
    deviceList.clear();
    internalDeviceList.clear();

    EXPECT_CALL(mockDeviceListener, onDeviceListUpdated(::testing::ContainerEq(deviceList))).Times(1).RetiresOnSaturation();
    transportManager.onDeviceListUpdated(mockDeviceAdapter, internalDeviceList);

    transportManager.onApplicationConnected(mockDeviceAdapter, deviceInfo, validConnectionHandle);

    //sleep(2); return;
    sleep(1);
}

int main(int argc, char **argv)
{
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));

    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
