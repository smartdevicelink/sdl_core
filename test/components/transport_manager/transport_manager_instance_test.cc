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
    Tests transport manager functionality with single device adapter that behaves correctly and single client
    Following sequence is tested:
        - TM created and runned
        - TM client registered as listener
        - TM client requests device scaning
        - single device was returned to TM client with onDeviceListUpdated callback
        - TM client calls "connect" on found device
        - device adapter sends onApplicationConnected
        - TM client receives onApplicationConnected
        - device adapter sends three data parts that represents single frame
        - TM client receives single frame with onFrameReceived callback
        - TM client calls sendFrame with some frame data and user data
        - TM client receives onFrameSendCompleted
        - TM client calls disconnectDevice
        - TM client receives onApplicationDisconnected
 */
#include "gtest/gtest.h"
#include "gmock/gmock.h"

//#include "../../../src/components/TransportManager/src/IDeviceAdapter.hpp"
//#include "../../../src/components/TransportManager/src/CTransportManager.hpp"
//#include "TransportManager/ITransportManagerDataListener.hpp"
//#include "TransportManager/ITransportManagerDeviceListener.hpp"

#include <stddef.h>
#include <stdio.h>

using ::testing::_;
using ::testing::Invoke;
using ::testing::StrEq;
using ::testing::Property;
using ::testing::Field;
using ::testing::Contains;
using ::testing::AllOf;
using ::testing::ElementsAre;
using ::testing::ContainerEq;
using ::testing::Eq;
using ::testing::InSequence;

//using namespace NsSmartDeviceLink::NsTransportManager;

//namespace test { namespace components { namespace TransportManager { namespace TestWithCorrectDeviceAdapter {
//
//    // ---------------- TEST DATA ---------------- //
//    namespace Data
//    {
//        static const tDeviceHandle DeviceHandle = 123;
//        static const EDeviceType DeviceType = DeviceBluetooth;
//        static const std::string UserFriendlyName("MY USER FRIENDLY NAME");
//        static const std::string UniqueDeviceId("MY_UNIQUE_DEVICE_ID");
//
//        static const tConnectionHandle ConnectionHandle = 666;
//
//        static const int UserData = 123;
//    }
//
//    // ---------------- TEST CLASSES ---------------- //
//
//    /**
//     * @brief Class that represents custom device adapter that will send known data
//     *        and check it's methods calls
//     **/
//    class MockDeviceAdapter : public IDeviceAdapter
//    {
//    public:
//        MockDeviceAdapter(IDeviceAdapterListener & Listener, IHandleGenerator & HandleGenerator)
//        : mListener(Listener)
//        , mHandleGenerator(HandleGenerator)
//        , mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManagerTest")))
//        {
//        }
//
//        virtual EDeviceType getDeviceType(void ) const
//        {
//            return DeviceBluetooth;
//        }
//        MOCK_METHOD1(connectDevice, void (const tDeviceHandle DeviceHandle));
//        MOCK_METHOD1(disconnectDevice, void (const tDeviceHandle DeviceHandle));
//        MOCK_METHOD0(run, void());
//        MOCK_METHOD0(scanForNewDevices, void());
//        MOCK_METHOD4(sendFrame, void(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize, int UserData));
//
//        void doScanForNewDevices()
//        {
//            LOG4CPLUS_INFO_EXT(mLogger, "-------------- Scanning new devices -----------------");
//            SInternalDeviceInfo deviceInfo;
//            deviceInfo.mDeviceHandle = Data::DeviceHandle;
//            deviceInfo.mUniqueDeviceId = Data::UniqueDeviceId;
//            deviceInfo.mUserFriendlyName = Data::UserFriendlyName;
//
//            tInternalDeviceList list;
//            list.push_back(deviceInfo);
//
//            LOG4CPLUS_INFO_EXT(mLogger, "-------------- Sending device list update -----------------");
//            mListener.onDeviceListUpdated(this, list);
//        }
//
//        void doConnectDevice(const tDeviceHandle DeviceHandle)
//        {
//            LOG4CPLUS_INFO_EXT(mLogger, "-------------- Connecting device -----------------");
//            // Application connect
//
//            SDeviceInfo deviceInfo;
//            deviceInfo.mDeviceHandle = Data::DeviceHandle;
//            deviceInfo.mUniqueDeviceId = Data::UniqueDeviceId;
//            deviceInfo.mUserFriendlyName = Data::UserFriendlyName;
//
//            LOG4CPLUS_INFO_EXT(mLogger, "-------------- Sending ApplicationConnected -----------------");
//            mListener.onApplicationConnected(this, deviceInfo, Data::ConnectionHandle);
//
//            // Send three frames to transport manager
//
//            uint8_t raw_data[] = {
//                0x22, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC8,
//                0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
//                0xFF, 0xFF, 0xFF, 0xFF
//            };
//
//            // Sending only header first
//            uint8_t *pSendBuff = raw_data;
//
//            LOG4CPLUS_INFO_EXT(mLogger, "-------------- Sending Frame #1 -----------------");
//            mListener.onFrameReceived(this, Data::ConnectionHandle, pSendBuff, 12);
//
//            // Sending first part of the data
//            pSendBuff+=12;
//            LOG4CPLUS_INFO_EXT(mLogger, "-------------- Sending Frame #2 -----------------");
//            mListener.onFrameReceived(this, Data::ConnectionHandle, pSendBuff, static_cast<size_t>(100));
//
//            // Sending last part of the data
//            pSendBuff+=100;
//            LOG4CPLUS_INFO_EXT(mLogger, "-------------- Sending Frame #3 -----------------");
//            mListener.onFrameReceived(this, Data::ConnectionHandle, pSendBuff, static_cast<size_t>(100));
//        }
//
//        void doSendFrame(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize, const int UserData)
//        {
//            LOG4CPLUS_INFO_EXT(mLogger, "-------------- doSendFrame called. Sending FrameSendCompleted -----------------");
//            mListener.onFrameSendCompleted(this, Data::ConnectionHandle, Data::UserData, SendStatusOK);
//        }
//
//        void doDisconnectDevice(const tDeviceHandle DeviceHandle)
//        {
//            LOG4CPLUS_INFO_EXT(mLogger, "-------------- doDisconnectDevice -----------------");
//            SDeviceInfo deviceInfo;
//            deviceInfo.mDeviceHandle = Data::DeviceHandle;
//            deviceInfo.mUniqueDeviceId = Data::UniqueDeviceId;
//            deviceInfo.mUserFriendlyName = Data::UserFriendlyName;
//
//            LOG4CPLUS_INFO_EXT(mLogger, "-------------- sending ApplicationDisconnected -----------------");
//            mListener.onApplicationDisconnected(this, deviceInfo, Data::ConnectionHandle);
//        }
//
//    protected:
//        IDeviceAdapterListener & mListener;
//        IHandleGenerator & mHandleGenerator;
//        Logger mLogger;
//    };
//
//    /**
//     * @brief Custom transport manager client that will check data, sent by transport manager
//     **/
//    class MockTransportManagerClient : public ITransportManagerDataListener
//                                     , public ITransportManagerDeviceListener
//    {
//    public:
//        MockTransportManagerClient(ITransportManager & TransportManager)
//        : mTransportManager(TransportManager)
//        , mDeviceList()
//        , mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManagerTest")))
//        {
//
//        }
//        MOCK_METHOD2(onApplicationConnected, void(const SDeviceInfo& ConnectedDevice, const tConnectionHandle Connection));
//        MOCK_METHOD2(onApplicationDisconnected, void(const SDeviceInfo& DisconnectedDevice, const tConnectionHandle Connection));
//        MOCK_METHOD1(onDeviceListUpdated, void(const tDeviceList& DeviceList));
//        MOCK_METHOD3(onFrameReceived, void(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize));
//        MOCK_METHOD3(onFrameSendCompleted, void(tConnectionHandle ConnectionHandle, int UserData, ESendStatus SendStatus));
//
//        void doDeviceListUpdated(const tDeviceList& DeviceList)
//        {
//            LOG4CPLUS_INFO_EXT(mLogger, "-------------- doDeviceListUpdated -----------------");
//            mDeviceList = DeviceList;
//
//            tDeviceList::const_iterator device;
//            for(device = mDeviceList.begin(); device != mDeviceList.end(); ++device)
//            {
//                mTransportManager.connectDevice(device->mDeviceHandle);
//            }
//        }
//
//        void doFrameReceived(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize)
//        {
//            LOG4CPLUS_INFO_EXT(mLogger, "-------------- doFrameReceived -----------------");
//            // Sending frame
//            uint8_t data[512]={1};
//            mTransportManager.sendFrame(ConnectionHandle, data, 512, Data::UserData);
//        }
//
//        void doFrameSendCompleted(tConnectionHandle ConnectionHandle, int UserData, ESendStatus SendStatus)
//        {
//            LOG4CPLUS_INFO_EXT(mLogger, "-------------- doFrameSendCompleted -----------------");
//
//            tDeviceList::const_iterator device;
//            for(device = mDeviceList.begin(); device != mDeviceList.end(); ++device)
//            {
//                mTransportManager.disconnectDevice(device->mDeviceHandle);
//            }
//        }
//
//    protected:
//        ITransportManager & mTransportManager;
//        tDeviceList mDeviceList;
//        Logger mLogger;
//    };
//
//    /**
//     * @brief Inherited transport manager class used for some small preparation of class for
//     *        testing (disabling another adapters etc.)
//     **/
//    class TestTransportManager : public CTransportManager
//    {
//    public:
//        TestTransportManager(void )
//        : CTransportManager()
//        , mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManagerTest")))
//        {
//        }
//
//        virtual ~TestTransportManager(void )
//        {
//
//        }
//
//        virtual void initializeDeviceAdapters()
//        {
//            // Preparing custom device adapter
//            mpDeviceAdapter = new MockDeviceAdapter(*this, *this);
//
//            EXPECT_CALL(*mpDeviceAdapter, run()).Times(1);
//            EXPECT_CALL(*mpDeviceAdapter, scanForNewDevices())
//                .Times(1)
//                .WillOnce(Invoke(mpDeviceAdapter, &MockDeviceAdapter::doScanForNewDevices))
//            ;
//
//            EXPECT_CALL(*mpDeviceAdapter, connectDevice(Data::DeviceHandle))
//                .Times(1)
//                .WillOnce(Invoke(mpDeviceAdapter, &MockDeviceAdapter::doConnectDevice))
//            ;
//
//            EXPECT_CALL(*mpDeviceAdapter, sendFrame(Data::ConnectionHandle, _, 512, Data::UserData))
//                .Times(1)
//                .WillOnce(Invoke(mpDeviceAdapter, &MockDeviceAdapter::doSendFrame))
//            ;
//
//            EXPECT_CALL(*mpDeviceAdapter, disconnectDevice(Data::DeviceHandle))
//                .Times(1)
//                .WillOnce(Invoke(mpDeviceAdapter, &MockDeviceAdapter::doDisconnectDevice))
//            ;
//
//            addDeviceAdapter(mpDeviceAdapter);
//            LOG4CPLUS_INFO_EXT(mLogger, "Device adapters initialized");
//        }
//
//    protected:
//        MockDeviceAdapter *mpDeviceAdapter;
//        Logger mLogger;
//    };
//
//    // ----------------------- TESTS ----------------------- //
//
//    TEST(test_TestWithCorrectDeviceAdapter, CorrectDeviceAdapterBehavior)
//    {
//        Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManagerTest"));
//
//        LOG4CPLUS_INFO_EXT(logger, "*************************** Starting test *****************************");
//        // All expectations must be sequenced
//        //InSequence dummy;
//
//        // Creating transport manager
//        TestTransportManager *pTm = new TestTransportManager();
//
//        // Preparing transport manage client
//        MockTransportManagerClient tmClient(*pTm);
//
//        // Expected device list
//        SDeviceInfo deviceInfo;
//        deviceInfo.mDeviceHandle = Data::DeviceHandle;
//        deviceInfo.mUniqueDeviceId = Data::UniqueDeviceId;
//        deviceInfo.mUserFriendlyName = Data::UserFriendlyName;
//        tDeviceList deviceList;
//        deviceList.push_back(deviceInfo);
//
//        EXPECT_CALL(tmClient, onDeviceListUpdated(ContainerEq(deviceList)))
//            .Times(1)
//            .WillOnce(Invoke(&tmClient, &MockTransportManagerClient::doDeviceListUpdated))
//        ;
//
//        EXPECT_CALL(tmClient, onApplicationConnected(deviceInfo, Data::ConnectionHandle))
//            .Times(1)
//        ;
//
//        EXPECT_CALL(tmClient, onFrameReceived(Data::ConnectionHandle, _, 212))
//            .Times(1)
//            .WillOnce(Invoke(&tmClient, &MockTransportManagerClient::doFrameReceived))
//        ;
//
//        EXPECT_CALL(tmClient, onFrameSendCompleted(Data::ConnectionHandle, Data::UserData, SendStatusOK))
//            .Times(1)
//            .WillOnce(Invoke(&tmClient, &MockTransportManagerClient::doFrameSendCompleted))
//        ;
//
//        EXPECT_CALL(tmClient, onApplicationDisconnected(deviceInfo, Data::ConnectionHandle))
//            .Times(1)
//        ;
//
//
//
//        // Running test
//
//        pTm->addDataListener(&tmClient);
//        pTm->addDeviceListener(&tmClient);
//
//        LOG4CPLUS_INFO_EXT(logger, "*************************** Calling RUN *****************************");
//        pTm->run();
//
//        sleep(1);
//
//        LOG4CPLUS_INFO_EXT(logger, "*************************** Calling SCAN FOR DEVICES *****************************");
//        pTm->scanForNewDevices();
//
//        sleep(2);
//
//        LOG4CPLUS_INFO_EXT(logger, "*************************** Deleting TM and shutting down *****************************");
//
//        // Shutdown transport manager
//        delete pTm;
//        pTm = 0;
//
//    }
//}}}} // End of namespaces



int main(int argc, char **argv) {
//  PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
