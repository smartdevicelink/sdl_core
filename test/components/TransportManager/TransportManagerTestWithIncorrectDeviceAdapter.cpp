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
                class MockDataListener: public NsAppLink::NsTransportManager::ITransportManagerDataListener
                {
                public:
                    MOCK_METHOD3(onFrameReceived, void(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize));
                    MOCK_METHOD3(onFrameSendCompleted, void(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, int FrameSequenceNumber, NsAppLink::NsTransportManager::ESendStatus SendStatus));
                };

                class MockDeviceListener: public NsAppLink::NsTransportManager::ITransportManagerDeviceListener
                {
                public:
                    MOCK_METHOD1(onDeviceListUpdated, void(const NsAppLink::NsTransportManager::tDeviceList & DeviceList));
                    MOCK_METHOD2(onApplicationConnected, void(const NsAppLink::NsTransportManager::SDeviceInfo & ConnectedDevice, const NsAppLink::NsTransportManager::tConnectionHandle Connection));
                    MOCK_METHOD2(onApplicationDisconnected, void(const NsAppLink::NsTransportManager::SDeviceInfo & DisconnectedDevice, const NsAppLink::NsTransportManager::tConnectionHandle Connection));
                };

                class MockDeviceAdapter: public NsAppLink::NsTransportManager::IDeviceAdapter
                {
                public:
                    virtual NsAppLink::NsTransportManager::EDeviceType getDeviceType(void) const;

                    MOCK_METHOD1(connectDevice, void (const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle));
                    MOCK_METHOD1(disconnectDevice, void (const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle));
                    MOCK_METHOD0(run, void());
                    MOCK_METHOD0(scanForNewDevices, void());
                    MOCK_METHOD3(sendFrame, int(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize));
                };

                class TestTransportManager: public NsAppLink::NsTransportManager::CTransportManager
                {
                public:
                    TestTransportManager(MockDeviceAdapter & DeviceAdapter);
                    ~TestTransportManager(void);
                protected:
                    virtual void initializeDeviceAdapters(void);

                private:
                    MockDeviceAdapter & mMockDeviceAdapter;
                };
            }
        }
    }
}

NsAppLink::NsTransportManager::EDeviceType test::components::TransportManger::IncorrectDeviceAdapterTest::MockDeviceAdapter::getDeviceType(void) const
{
    return NsAppLink::NsTransportManager::DeviceBluetooth;
}

test::components::TransportManger::IncorrectDeviceAdapterTest::TestTransportManager::TestTransportManager(test::components::TransportManger::IncorrectDeviceAdapterTest::MockDeviceAdapter& DeviceAdapter):
CTransportManager(),
mMockDeviceAdapter(DeviceAdapter)
{
}

test::components::TransportManger::IncorrectDeviceAdapterTest::TestTransportManager::~TestTransportManager(void)
{
}

void test::components::TransportManger::IncorrectDeviceAdapterTest::TestTransportManager::initializeDeviceAdapters(void)
{
    addDeviceAdapter(&mMockDeviceAdapter);
}

TEST(TransportManager, IncorrectDeviceAdapter)
{
    test::components::TransportManger::IncorrectDeviceAdapterTest::MockDeviceAdapter mockDeviceAdapter;
    test::components::TransportManger::IncorrectDeviceAdapterTest::MockDataListener mockDataListener;
    test::components::TransportManger::IncorrectDeviceAdapterTest::MockDeviceListener mockDeviceListener;
    test::components::TransportManger::IncorrectDeviceAdapterTest::TestTransportManager transportManager(mockDeviceAdapter);

    EXPECT_CALL(mockDeviceAdapter, run()).Times(1);

    transportManager.run();

    transportManager.addDeviceListener(&mockDeviceListener);
    transportManager.addDataListener(&mockDataListener);

    NsAppLink::NsTransportManager::tDeviceHandle validDeviceHandle = 123;
    NsAppLink::NsTransportManager::tDeviceHandle validConnectionHandle = 321;

    NsAppLink::NsTransportManager::SDeviceInfo deviceInfo;
    deviceInfo.mDeviceHandle = validDeviceHandle;
    deviceInfo.mUniqueDeviceId = "id";
    deviceInfo.mUserFriendlyName = "Name";

    NsAppLink::NsTransportManager::tDeviceList deviceList;
    deviceList.push_back(deviceInfo);

    NsAppLink::NsTransportManager::tInternalDeviceList internalDeviceList;
    internalDeviceList.push_back(NsAppLink::NsTransportManager::SInternalDeviceInfo(deviceInfo.mDeviceHandle, deviceInfo.mUserFriendlyName, deviceInfo.mUniqueDeviceId));

    uint8_t validFrameData[16] = {0x11, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08};
    uint8_t invalidFrameData[16] = {0x51, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08};

    EXPECT_CALL(mockDeviceListener, onDeviceListUpdated(::testing::_)).Times(0);
    EXPECT_CALL(mockDeviceListener, onApplicationConnected(::testing::_, ::testing::_)).Times(0);
    EXPECT_CALL(mockDeviceListener, onApplicationDisconnected(::testing::_, ::testing::_)).Times(0);
    EXPECT_CALL(mockDataListener, onFrameReceived(::testing::_, ::testing::_, ::testing::_)).Times(0);
    EXPECT_CALL(mockDataListener, onFrameSendCompleted(::testing::_, ::testing::_, ::testing::_)).Times(0);

    transportManager.onFrameReceived(&mockDeviceAdapter, validConnectionHandle, 0, 5);

    transportManager.onFrameReceived(&mockDeviceAdapter, validConnectionHandle, validFrameData, 0);

    transportManager.onFrameReceived(&mockDeviceAdapter, validConnectionHandle, validFrameData, sizeof(validFrameData));

    transportManager.onFrameSendCompleted(&mockDeviceAdapter, validConnectionHandle, 20, NsAppLink::NsTransportManager::SendStatusOK);

    transportManager.onApplicationDisconnected(&mockDeviceAdapter, deviceInfo, validConnectionHandle);

    transportManager.onApplicationConnected(&mockDeviceAdapter, deviceInfo, validConnectionHandle);

    EXPECT_CALL(mockDeviceListener, onDeviceListUpdated(::testing::ContainerEq(deviceList))).Times(1).RetiresOnSaturation();
    transportManager.onDeviceListUpdated(&mockDeviceAdapter, internalDeviceList);

    transportManager.onApplicationDisconnected(&mockDeviceAdapter, deviceInfo, validConnectionHandle);

    EXPECT_CALL(mockDeviceListener, onApplicationConnected(::testing::Eq(deviceInfo), validConnectionHandle)).Times(1).RetiresOnSaturation();
    transportManager.onApplicationConnected(&mockDeviceAdapter, deviceInfo, validConnectionHandle);

    transportManager.onFrameReceived(&mockDeviceAdapter, validConnectionHandle, 0, 5);

    transportManager.onFrameReceived(&mockDeviceAdapter, validConnectionHandle, validFrameData, 0);

    EXPECT_CALL(mockDataListener, onFrameReceived(validConnectionHandle, ::testing::_, sizeof(validFrameData))).Times(1).RetiresOnSaturation();
    transportManager.onFrameReceived(&mockDeviceAdapter, validConnectionHandle, validFrameData, sizeof(validFrameData));

    transportManager.onFrameReceived(&mockDeviceAdapter, validConnectionHandle, invalidFrameData, sizeof(invalidFrameData));

    transportManager.onFrameSendCompleted(&mockDeviceAdapter, validConnectionHandle, 20, NsAppLink::NsTransportManager::SendStatusOK);

    int frameSequenceNumber = transportManager.sendFrame(validConnectionHandle, validFrameData, sizeof(validFrameData));
    EXPECT_CALL(mockDataListener, onFrameSendCompleted(validConnectionHandle, frameSequenceNumber, NsAppLink::NsTransportManager::SendStatusOK)).Times(1).RetiresOnSaturation();
    transportManager.onFrameSendCompleted(&mockDeviceAdapter, validConnectionHandle, frameSequenceNumber, NsAppLink::NsTransportManager::SendStatusOK);

    EXPECT_CALL(mockDeviceListener, onApplicationDisconnected(::testing::Eq(deviceInfo), validConnectionHandle)).Times(1).RetiresOnSaturation();
    transportManager.onApplicationDisconnected(&mockDeviceAdapter, deviceInfo, validConnectionHandle);

    transportManager.onApplicationDisconnected(&mockDeviceAdapter, deviceInfo, validConnectionHandle);

    deviceList.clear();
    internalDeviceList.clear();

    EXPECT_CALL(mockDeviceListener, onDeviceListUpdated(::testing::ContainerEq(deviceList))).Times(1).RetiresOnSaturation();
    transportManager.onDeviceListUpdated(&mockDeviceAdapter, internalDeviceList);

    transportManager.onApplicationConnected(&mockDeviceAdapter, deviceInfo, validConnectionHandle);

    sleep(1);
}

int main(int argc, char **argv)
{
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));

    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
