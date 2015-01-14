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
    Tests Transport Manager behavior with single TM client which sents a lot of data from many different threads (more than 200).
    Sent data was sent back from device adapter to TM client and validated
 */
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../../../src/components/TransportManager/src/ITransportAdapter.hpp"
#include "../../../src/components/TransportManager/src/CTransportManager.hpp"
#include "TransportManager/ITransportManagerDataListener.hpp"
#include "TransportManager/ITransportManagerDeviceListener.hpp"

#include <stddef.h>
#include <stdio.h>
#include <vector>
#include <map>

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

using namespace NsSmartDeviceLink::NsTransportManager;

namespace test { namespace components { namespace TransportManager { namespace TestWithMultiThreadedClient {

    // ---------------- TEST DATA ---------------- //
    namespace Data
    {
        static const tDeviceHandle DeviceHandle = 123;
        static const EDeviceType DeviceType = DeviceBluetooth;
        static const std::string UserFriendlyName("MY USER FRIENDLY NAME");
        static const std::string UniqueDeviceId("MY_UNIQUE_DEVICE_ID");

        static const tConnectionHandle ConnectionHandle = 666;

        static const int BaseSequenceNumber = 1;
        static const int NumberOfThreads    = 113;
        static const int NumberOfFramesPerThread = 53;
        static const int TotalNumberOfFrames = NumberOfThreads * NumberOfFramesPerThread;
        static const int FrameSize = 323;
    }

    // ---------------- TEST CLASSES ---------------- //

    /**
     * @brief Class that represents custom device adapter that will send known data
     *        and check it's methods calls
     **/
    class MockTransportAdapter : public ITransportAdapter
    {
    public:
        MockTransportAdapter(ITransportAdapterListener & Listener, IHandleGenerator & HandleGenerator)
        : mListener(Listener)
        , mHandleGenerator(HandleGenerator)
        {
        }

        virtual EDeviceType GetDeviceType(void ) const
        {
            return DeviceBluetooth;
        }
        MOCK_METHOD1(connectDevice, void (const tDeviceHandle DeviceHandle));
        MOCK_METHOD1(disconnectDevice, void (const tDeviceHandle DeviceHandle));
        MOCK_METHOD0(run, void());
        MOCK_METHOD0(scanForNewDevices, void());
        MOCK_METHOD4(sendFrame, void(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize, int UserData));

        void doScanForNewDevices()
        {
            LOG4CXX_INFO_EXT(mLogger, "\n-------------- Scanning new devices -----------------");
            SInternalDeviceInfo deviceInfo;
            deviceInfo.mDeviceHandle = Data::DeviceHandle;
            deviceInfo.mUniqueDeviceId = Data::UniqueDeviceId;
            deviceInfo.mUserFriendlyName = Data::UserFriendlyName;

            tInternalDeviceList list;
            list.push_back(deviceInfo);

            LOG4CXX_INFO_EXT(mLogger, "\n-------------- Sending device list update -----------------");
            mListener.onDeviceListUpdated(this, list);
        }

        void doConnectDevice(const tDeviceHandle DeviceHandle)
        {
            LOG4CXX_INFO_EXT(mLogger, "\n-------------- Connecting device -----------------");
            // Application connect

            SDeviceInfo deviceInfo;
            deviceInfo.mDeviceHandle = Data::DeviceHandle;
            deviceInfo.mUniqueDeviceId = Data::UniqueDeviceId;
            deviceInfo.mUserFriendlyName = Data::UserFriendlyName;

            LOG4CXX_INFO_EXT(mLogger, "\n-------------- Sending ApplicationConnected -----------------");
            mListener.onApplicationConnected(this, deviceInfo, Data::ConnectionHandle);
        }

        void doSendFrame(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize, int UserData)
        {
            LOG4CXX_INFO_EXT(mLogger, "\n-------------- doSendFrame called -----------------");

            LOG4CXX_INFO_EXT(mLogger, "\n-------------- Sending frame back to TransportManager. DataSize: " << DataSize << "--------------");

            // Loop back. Each recevied frame is sent back.
            mListener.onFrameReceived(this, ConnectionHandle, Data, DataSize);

            LOG4CXX_INFO_EXT(mLogger, "\n-------------- Calling onFrameSendCompleted. UserData: " << UserData << "--------------");

            mListener.onFrameSendCompleted(this, Data::ConnectionHandle, UserData, SendStatusOK);
        }

        void doDisconnectDevice(const tDeviceHandle DeviceHandle)
        {
            LOG4CXX_INFO_EXT(mLogger, "\n-------------- doDisconnectDevice -----------------");
            SDeviceInfo deviceInfo;
            deviceInfo.mDeviceHandle = Data::DeviceHandle;
            deviceInfo.mUniqueDeviceId = Data::UniqueDeviceId;
            deviceInfo.mUserFriendlyName = Data::UserFriendlyName;

            LOG4CXX_INFO_EXT(mLogger, "\n-------------- sending ApplicationDisconnected -----------------");
            mListener.onApplicationDisconnected(this, deviceInfo, Data::ConnectionHandle);
        }

    protected:
        ITransportAdapterListener & mListener;
        IHandleGenerator & mHandleGenerator;
    };

    /**
     * @brief Custom transport manager client that will check data, sent by transport manager
     **/
    class MockTransportManagerClient : public ITransportManagerDataListener
                                     , public ITransportManagerDeviceListener
    {
    public:

        MockTransportManagerClient(ITransportManager & TransportManager)
        : mTransportManager(TransportManager)
        , mDeviceList()
        , mFrameSequenceNumber(-1)
        , mLogger(LOG4CXX::Logger::getInstance(LOG4CXX_TEXT("TransportManagerTest")))
        , mThreads()
        , mSendFrameMap()
        , mFrameReceivedNumberMutex()
        , mFrameSendCompletedMutex()
        , mNumberOfReceivedFrames(0)
        , mNumberOfCompletelySentFrames(0)
        , mSequenceNumber(Data::BaseSequenceNumber)
        , mSequenceNumberMutex()
        {
            pthread_mutex_init(&mFrameReceivedNumberMutex, 0);
            pthread_mutex_init(&mFrameSendCompletedMutex, 0);
            pthread_mutex_init(&mSequenceNumberMutex, 0);
        }

        MOCK_METHOD2(onApplicationConnected, void(const SDeviceInfo& ConnectedDevice, const tConnectionHandle Connection));
        MOCK_METHOD2(onApplicationDisconnected, void(const SDeviceInfo& DisconnectedDevice, const tConnectionHandle Connection));
        MOCK_METHOD1(onDeviceListUpdated, void(const tDeviceList& DeviceList));
        MOCK_METHOD3(onFrameReceived, void(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize));
        MOCK_METHOD3(onFrameSendCompleted, void(tConnectionHandle ConnectionHandle, int UserData, ESendStatus SendStatus));

        void doDeviceListUpdated(const tDeviceList& DeviceList)
        {
            LOG4CXX_INFO_EXT(mLogger, "\n-------------- doDeviceListUpdated -----------------");
            mDeviceList = DeviceList;

            tDeviceList::const_iterator device;
            for(device = mDeviceList.begin(); device != mDeviceList.end(); ++device)
            {
                mTransportManager.connectDevice(device->mDeviceHandle);
            }
        }

        void doApplicationConnected(const SDeviceInfo& ConnectedDevice, const tConnectionHandle Connection)
        {
            LOG4CXX_INFO_EXT(mLogger, "\n-------------- doApplicationConnected -----------------");

            for (int i = 0; i < Data::NumberOfThreads; ++i)
            {
                pthread_t thread;
                pthread_create(&thread, NULL, &threadSendFrames, static_cast<void*>(this));
                mThreads.push_back(thread);
            }
        }

        void doFrameSendCompleted(tConnectionHandle ConnectionHandle, int UserData, ESendStatus SendStatus)
        {
            LOG4CXX_INFO_EXT(mLogger, "\n-------------- doFrameSendCompleted -----------------");

            pthread_mutex_lock(&mFrameSendCompletedMutex);

            mNumberOfCompletelySentFrames++;

            uint8_t *pData = mSendFrameMap[UserData];

            delete pData;
            mSendFrameMap.erase(UserData);

            pthread_mutex_unlock(&mFrameSendCompletedMutex);

            LOG4CXX_INFO_EXT(mLogger, "\n-------------- Number of completely sent frames: "
                << mNumberOfCompletelySentFrames << " -----------------");
        }

        void doFrameReceived(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize)
        {
            LOG4CXX_INFO_EXT(mLogger, "\n-------------- doFrameReceived -----------------");

            pthread_mutex_lock(&mFrameReceivedNumberMutex);

            mNumberOfReceivedFrames++;

            pthread_mutex_unlock(&mFrameReceivedNumberMutex);

            LOG4CXX_INFO_EXT(mLogger, "\n-------------- Number of received frames: " << mNumberOfReceivedFrames << " -----------------");
        }

        void disconnectAllDevices(void)
        {
            tDeviceList::const_iterator device;

            for(device = mDeviceList.begin(); device != mDeviceList.end(); ++device)
            {
                mTransportManager.disconnectDevice(device->mDeviceHandle);
            }

            ASSERT_EQ(mNumberOfCompletelySentFrames, Data::TotalNumberOfFrames) << "Wrong number of send frame confirmation";
            ASSERT_EQ(mNumberOfReceivedFrames, Data::TotalNumberOfFrames) << "Wrong number of received frames";
        }

        void waitForAllThreads(void)
        {
            int timeOut = 60;   // sec
            bool finished = false;

            while(timeOut)
            {
                pthread_mutex_lock(&mFrameReceivedNumberMutex);
                finished = mNumberOfReceivedFrames == Data::TotalNumberOfFrames;
                pthread_mutex_unlock(&mFrameReceivedNumberMutex);

                if (finished)
                {
                    break;
                }
                else
                {
                    sleep(1);
                    timeOut--;
                }
            }

            ASSERT_NE(timeOut, 0) << "Timeout waiting to receive all the frames";

            for (int i = 0; i < Data::NumberOfThreads; ++i)
            {
                pthread_join(mThreads[i], NULL);
            }
        }

    private:

        static void* threadSendFrames(void* Data)
        {
            MockTransportManagerClient *pTMClient = static_cast<MockTransportManagerClient*>(Data);

            LOG4CXX_INFO_EXT(pTMClient->mLogger, "\n-------------- Frame Send Thread Started -----------------");

            for (int i = 0; i < Data::NumberOfFramesPerThread; ++i)
            {
                uint8_t *pFrameData = new uint8_t[Data::FrameSize];
                uint32_t dataSize = Data::FrameSize - 12;   // minus header size

                // make a valid packet
                pFrameData[0] = 0x22;       // version 2
                pFrameData[1] = 0x0F;
                pFrameData[2] = 0x00;
                pFrameData[3] = 0x00;
                pFrameData[4] = (dataSize>>24) & 0xFF;
                pFrameData[5] = (dataSize>>16) & 0xFF;
                pFrameData[6] = (dataSize>> 8) & 0xFF;
                pFrameData[7] = (dataSize>> 0) & 0xFF;

                int sequence = pTMClient->getNextSequenceNumber();

                pTMClient->mTransportManager.sendFrame(Data::ConnectionHandle, pFrameData, Data::FrameSize, sequence);

                pthread_mutex_lock(&pTMClient->mFrameSendCompletedMutex);
                pTMClient->mSendFrameMap[sequence] = pFrameData;
                pthread_mutex_unlock(&pTMClient->mFrameSendCompletedMutex);
            }

            return 0;
        }

    int getNextSequenceNumber(void)
    {
        int nextSeq;

        pthread_mutex_lock(&mSequenceNumberMutex);
        nextSeq = ++mSequenceNumber;
        pthread_mutex_unlock(&mSequenceNumberMutex);

        return nextSeq;
    }

    protected:
        ITransportManager & mTransportManager;
        tDeviceList mDeviceList;
        int mFrameSequenceNumber;
        Logger mLogger;

        std::vector<pthread_t> mThreads;
        std::map<int, uint8_t*> mSendFrameMap;

        int mNumberOfReceivedFrames;
        pthread_mutex_t mFrameReceivedNumberMutex;

        int mNumberOfCompletelySentFrames;
        pthread_mutex_t mFrameSendCompletedMutex;

        int mSequenceNumber;
        pthread_mutex_t mSequenceNumberMutex;
    };

    /**
     * @brief Inherited transport manager class used for some small preparation of class for
     *        testing (disabling another adapters etc.)
     **/
    class TestTransportManager : public CTransportManager
    {
    public:
        TestTransportManager(void )
        : CTransportManager()
        , mLogger(LOG4CXX::Logger::getInstance(LOG4CXX_TEXT("TransportManagerTest")))
        {
        }

        virtual ~TestTransportManager(void )
        {

        }

        virtual void initializeTransportAdapters()
        {
            // Preparing custom device adapter
            mpTransportAdapter = new MockTransportAdapter(*this, *this);

            EXPECT_CALL(*mpTransportAdapter, run()).Times(1);
            EXPECT_CALL(*mpTransportAdapter, scanForNewDevices())
                .Times(1)
                .WillOnce(Invoke(mpTransportAdapter, &MockTransportAdapter::doScanForNewDevices))
            ;

            EXPECT_CALL(*mpTransportAdapter, connectDevice(Data::DeviceHandle))
                .Times(1)
                .WillOnce(Invoke(mpTransportAdapter, &MockTransportAdapter::doConnectDevice))
            ;

            EXPECT_CALL(*mpTransportAdapter, sendFrame(Data::ConnectionHandle, _, _, _))
                .Times(Data::TotalNumberOfFrames)
                .WillRepeatedly(Invoke(mpTransportAdapter, &MockTransportAdapter::doSendFrame))
            ;

            EXPECT_CALL(*mpTransportAdapter, disconnectDevice(Data::DeviceHandle))
                .Times(1)
                .WillOnce(Invoke(mpTransportAdapter, &MockTransportAdapter::doDisconnectDevice))
            ;

            addTransportAdapter(mpTransportAdapter);
            LOG4CXX_INFO_EXT(mLogger, "Transport adapters initialized");
        }

    protected:
        MockTransportAdapter *mpTransportAdapter;
        Logger mLogger;
    };

    // ----------------------- TESTS ----------------------- //

    TEST(test_TestWithMultiThreadedClient, MultiThreadedClientTest)
    {
        Logger logger = LOG4CXX::Logger::getInstance(LOG4CXX_TEXT("TransportManagerTest"));

        LOG4CXX_INFO_EXT(logger, "\n*************************** Starting test *****************************");
        // All expectations must be sequenced
        //InSequence dummy;

        // Creating transport manager
        TestTransportManager *pTm = new TestTransportManager();

        // Preparing transport manage client
        MockTransportManagerClient tmClient(*pTm);

        // Expected device list
        SDeviceInfo deviceInfo;
        deviceInfo.mDeviceHandle = Data::DeviceHandle;
        deviceInfo.mUniqueDeviceId = Data::UniqueDeviceId;
        deviceInfo.mUserFriendlyName = Data::UserFriendlyName;
        tDeviceList deviceList;
        deviceList.push_back(deviceInfo);

        EXPECT_CALL(tmClient, onDeviceListUpdated(ContainerEq(deviceList)))
            .Times(1)
            .WillOnce(Invoke(&tmClient, &MockTransportManagerClient::doDeviceListUpdated))
        ;

        EXPECT_CALL(tmClient, onApplicationConnected(deviceInfo, Data::ConnectionHandle))
            .Times(1)
            .WillOnce(Invoke(&tmClient, &MockTransportManagerClient::doApplicationConnected))
        ;

        EXPECT_CALL(tmClient, onFrameReceived(Data::ConnectionHandle, _, Data::FrameSize))
            .Times(Data::TotalNumberOfFrames)
            .WillRepeatedly(Invoke(&tmClient, &MockTransportManagerClient::doFrameReceived))
        ;

        EXPECT_CALL(tmClient, onFrameSendCompleted(Data::ConnectionHandle, _, SendStatusOK))
            .Times(Data::TotalNumberOfFrames)
            .WillRepeatedly(Invoke(&tmClient, &MockTransportManagerClient::doFrameSendCompleted))
        ;

        EXPECT_CALL(tmClient, onApplicationDisconnected(deviceInfo, Data::ConnectionHandle))
            .Times(1)
        ;

        // Running test

        pTm->addDataListener(&tmClient);
        pTm->addDeviceListener(&tmClient);

        LOG4CXX_INFO_EXT(logger, "\n*************************** Calling RUN *****************************");
        pTm->run();

        sleep(1);

        LOG4CXX_INFO_EXT(logger, "\n*************************** Calling SCAN FOR DEVICES *****************************");
        pTm->scanForNewDevices();

        LOG4CXX_INFO_EXT(logger, "\n******************* Waiting for all client thread to finish ********************");
        tmClient.waitForAllThreads();

        LOG4CXX_INFO_EXT(logger, "\n******************* Disconnecting all devices ********************");
        tmClient.disconnectAllDevices();

        sleep(1);

        LOG4CXX_INFO_EXT(logger, "\n*************************** Deleting TM and shutting down *****************************");

        // Shutdown transport manager
        delete pTm;
        pTm = 0;

    }
}}}} // End of namespaces



int main(int argc, char **argv) {
  PropertyConfigurator::doConfigure(LOG4CXX_TEXT("log4cxx.properties"));
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
