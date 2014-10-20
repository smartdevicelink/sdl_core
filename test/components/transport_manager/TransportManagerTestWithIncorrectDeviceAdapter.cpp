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
#include "../../../src/components/TransportManager/src/ITransportAdapter.hpp"

namespace test
{
    namespace components
    {
        namespace TransportManger
        {
            namespace IncorrectTransportAdapterTest
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

                class MockTransportAdapter: public NsSmartDeviceLink::NsTransportManager::ITransportAdapter
                {
                public:
                    virtual NsSmartDeviceLink::NsTransportManager::EDeviceType GetDeviceType(void) const;

                    MOCK_METHOD1(connectDevice, void (const NsSmartDeviceLink::NsTransportManager::tDeviceHandle DeviceHandle));
                    MOCK_METHOD1(disconnectDevice, void (const NsSmartDeviceLink::NsTransportManager::tDeviceHandle DeviceHandle));
                    MOCK_METHOD0(run, void());
                    MOCK_METHOD0(scanForNewDevices, void());
                    MOCK_METHOD4(sendFrame, void(NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize, int UserData));
                };

                class TestTransportManager: public NsSmartDeviceLink::NsTransportManager::CTransportManager
                {
                public:
                    TestTransportManager(MockTransportAdapter * TransportAdapter);
                    ~TestTransportManager(void);
                protected:
                    virtual void initializeTransportAdapters(void);

                private:
                    MockTransportAdapter * mMockTransportAdapter;
                };
            }
        }
    }
}

NsSmartDeviceLink::NsTransportManager::EDeviceType test::components::TransportManger::IncorrectTransportAdapterTest::MockTransportAdapter::GetDeviceType(void) const
{
    return NsSmartDeviceLink::NsTransportManager::DeviceBluetooth;
}

test::components::TransportManger::IncorrectTransportAdapterTest::TestTransportManager::TestTransportManager(test::components::TransportManger::IncorrectTransportAdapterTest::MockTransportAdapter* TransportAdapter):
CTransportManager(),
mMockTransportAdapter(TransportAdapter)
{
}

test::components::TransportManger::IncorrectTransportAdapterTest::TestTransportManager::~TestTransportManager(void)
{
}

void test::components::TransportManger::IncorrectTransportAdapterTest::TestTransportManager::initializeTransportAdapters(void)
{
    addTransportAdapter(mMockTransportAdapter);
}

TEST(TransportManager, IncorrectTransportAdapter)
{
    test::components::TransportManger::IncorrectTransportAdapterTest::MockTransportAdapter* mockTransportAdapter = new test::components::TransportManger::IncorrectTransportAdapterTest::MockTransportAdapter();
    test::components::TransportManger::IncorrectTransportAdapterTest::MockDataListener mockDataListener;
    test::components::TransportManger::IncorrectTransportAdapterTest::MockDeviceListener mockDeviceListener;
    test::components::TransportManger::IncorrectTransportAdapterTest::TestTransportManager transportManager(mockTransportAdapter);

    EXPECT_CALL(*mockTransportAdapter, run()).Times(1);

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

    transportManager.onFrameReceived(mockTransportAdapter, validConnectionHandle, 0, 5);

    transportManager.onFrameReceived(mockTransportAdapter, validConnectionHandle, validFrameData, 0);

    transportManager.onFrameReceived(mockTransportAdapter, validConnectionHandle, validFrameData, sizeof(validFrameData));

    transportManager.onFrameSendCompleted(mockTransportAdapter, validConnectionHandle, 20, NsSmartDeviceLink::NsTransportManager::SendStatusOK);

    transportManager.onApplicationDisconnected(mockTransportAdapter, deviceInfo, validConnectionHandle);

    transportManager.onApplicationConnected(mockTransportAdapter, deviceInfo, validConnectionHandle);

    EXPECT_CALL(mockDeviceListener, onDeviceListUpdated(::testing::ContainerEq(deviceList))).Times(1).RetiresOnSaturation();
    transportManager.onDeviceListUpdated(mockTransportAdapter, internalDeviceList);

    transportManager.onApplicationDisconnected(mockTransportAdapter, deviceInfo, validConnectionHandle);

    EXPECT_CALL(mockDeviceListener, onApplicationConnected(::testing::Eq(deviceInfo), validConnectionHandle)).Times(1).RetiresOnSaturation();
    transportManager.onApplicationConnected(mockTransportAdapter, deviceInfo, validConnectionHandle);

    transportManager.onFrameReceived(mockTransportAdapter, validConnectionHandle, 0, 5);

    transportManager.onFrameReceived(mockTransportAdapter, validConnectionHandle, validFrameData, 0);

    EXPECT_CALL(mockDataListener, onFrameReceived(validConnectionHandle, ::testing::_, sizeof(validFrameData))).Times(1).RetiresOnSaturation();
    transportManager.onFrameReceived(mockTransportAdapter, validConnectionHandle, validFrameData, sizeof(validFrameData));

    transportManager.onFrameReceived(mockTransportAdapter, validConnectionHandle, invalidFrameData, sizeof(invalidFrameData));

    // Frame result is now expected even if we do not send any frame before
    EXPECT_CALL(mockDataListener, onFrameSendCompleted(validConnectionHandle, 20, NsSmartDeviceLink::NsTransportManager::SendStatusOK)).Times(1).RetiresOnSaturation();
    transportManager.onFrameSendCompleted(mockTransportAdapter, validConnectionHandle, 20, NsSmartDeviceLink::NsTransportManager::SendStatusOK);

    int userData = 123;
    transportManager.sendFrame(validConnectionHandle, validFrameData, sizeof(validFrameData), userData);
    EXPECT_CALL(mockDataListener, onFrameSendCompleted(validConnectionHandle, userData, NsSmartDeviceLink::NsTransportManager::SendStatusOK)).Times(1).RetiresOnSaturation();
    transportManager.onFrameSendCompleted(mockTransportAdapter, validConnectionHandle, userData, NsSmartDeviceLink::NsTransportManager::SendStatusOK);

    sleep(1); //We must wait for sending previous callbacks while applicationDisconnect can shutdown connection thread
    
    EXPECT_CALL(mockDeviceListener, onApplicationDisconnected(::testing::Eq(deviceInfo), validConnectionHandle)).Times(1).RetiresOnSaturation();
    transportManager.onApplicationDisconnected(mockTransportAdapter, deviceInfo, validConnectionHandle);

    transportManager.onApplicationDisconnected(mockTransportAdapter, deviceInfo, validConnectionHandle);

    
    deviceList.clear();
    internalDeviceList.clear();

    EXPECT_CALL(mockDeviceListener, onDeviceListUpdated(::testing::ContainerEq(deviceList))).Times(1).RetiresOnSaturation();
    transportManager.onDeviceListUpdated(mockTransportAdapter, internalDeviceList);

    transportManager.onApplicationConnected(mockTransportAdapter, deviceInfo, validConnectionHandle);

    //sleep(2); return;
    sleep(1);
}

int main(int argc, char **argv)
{
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));

    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
