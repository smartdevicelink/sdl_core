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
    Tests transport manager with multiple TM clients.

    Each client lives within it's own thread. During his live each client sends a lot of data.
    Also a lot of data also sent from device adapter to all TM clients.
 */
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../../../src/components/TransportManager/src/ITransportAdapter.hpp"
#include "../../../src/components/TransportManager/src/CTransportManager.hpp"
#include "TransportManager/ITransportManagerDataListener.hpp"
#include "TransportManager/ITransportManagerDeviceListener.hpp"

#include <stddef.h>
#include <stdio.h>
#include <pthread.h>


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

namespace test { namespace components { namespace TransportManager { namespace TestWithMultipleClients {

    // ---------------- TEST DATA ---------------- //
    namespace Data
    {
        static const tDeviceHandle DeviceHandle = 123;
        static const EDeviceType DeviceType = DeviceBluetooth;
        static const std::string UserFriendlyName("MY USER FRIENDLY NAME");
        static const std::string UniqueDeviceId("MY_UNIQUE_DEVICE_ID");

        static const tConnectionHandle ConnectionHandle = 666;

        static const int NumberOfClients = 100;
        static const int NumberOfIterations = 6;

        static const int TotalNumberOfCalls = NumberOfClients * NumberOfIterations;
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
        , mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManagerTest")))
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
            LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- Scanning new devices -----------------");
            SInternalDeviceInfo deviceInfo;
            deviceInfo.mDeviceHandle = Data::DeviceHandle;
            deviceInfo.mUniqueDeviceId = Data::UniqueDeviceId;
            deviceInfo.mUserFriendlyName = Data::UserFriendlyName;

            tInternalDeviceList list;
            list.push_back(deviceInfo);

            LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- Sending device list update -----------------");
            mListener.onDeviceListUpdated(this, list);
        }

        void doConnectDevice(const tDeviceHandle DeviceHandle)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- Connecting device -----------------");
            // Application connect

            SDeviceInfo deviceInfo;
            deviceInfo.mDeviceHandle = Data::DeviceHandle;
            deviceInfo.mUniqueDeviceId = Data::UniqueDeviceId;
            deviceInfo.mUserFriendlyName = Data::UserFriendlyName;

            LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- Sending ApplicationConnected -----------------");
            mListener.onApplicationConnected(this, deviceInfo, Data::ConnectionHandle);

            // Send three frames to transport manager

            uint8_t raw_data[] = {
                0x22, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC8,
                0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF
            };

            for (int i = 0; i < Data::NumberOfIterations; ++i)
            {
                // Sending only header first
                uint8_t *pSendBuff = raw_data;

                LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- Sending Frame #" << 3 * i + 1 << " -----------------");
                mListener.onFrameReceived(this, Data::ConnectionHandle, pSendBuff, 12);

                // Sending first part of the data
                pSendBuff+=12;
                LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- Sending Frame #" << 3 * i + 2 << " -----------------");
                mListener.onFrameReceived(this, Data::ConnectionHandle, pSendBuff, static_cast<size_t>(100));

                // Sending last part of the data
                pSendBuff+=100;
                LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- Sending Frame #" << 3 * i + 3 << " -----------------");
                mListener.onFrameReceived(this, Data::ConnectionHandle, pSendBuff, static_cast<size_t>(100));
            }
        }

        void doSendFrame(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize, int UserData)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- doSendFrame called. Sending FrameSendCompleted -----------------");
            mListener.onFrameSendCompleted(this, Data::ConnectionHandle, UserData, SendStatusOK);
        }

        void doDisconnectDevice(const tDeviceHandle DeviceHandle)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- doDisconnectDevice -----------------");
            SDeviceInfo deviceInfo;
            deviceInfo.mDeviceHandle = Data::DeviceHandle;
            deviceInfo.mUniqueDeviceId = Data::UniqueDeviceId;
            deviceInfo.mUserFriendlyName = Data::UserFriendlyName;

            LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- sending ApplicationDisconnected -----------------");
            mListener.onApplicationDisconnected(this, deviceInfo, Data::ConnectionHandle);
        }

    protected:
        ITransportAdapterListener & mListener;
        IHandleGenerator & mHandleGenerator;
        Logger mLogger;
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
        , mFrameSequenceNumber(0)
        , mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManagerTest")))
        , mCurrentIteration(0)
        , mConnectionHandle(Data::ConnectionHandle)  // should be invalid handle but currently it is valid because of the late applicationConnected callback
        , mConnected(false)
        , mNumberOfReceivedFrames(0)
        , mNumberOfReceivedFramesMutex()
        {
            pthread_mutex_init(&mNumberOfReceivedFramesMutex, 0);
        }
        MOCK_METHOD2(onApplicationConnected, void(const SDeviceInfo& ConnectedDevice, const tConnectionHandle Connection));
        MOCK_METHOD2(onApplicationDisconnected, void(const SDeviceInfo& DisconnectedDevice, const tConnectionHandle Connection));
        MOCK_METHOD1(onDeviceListUpdated, void(const tDeviceList& DeviceList));
        MOCK_METHOD3(onFrameReceived, void(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize));
        MOCK_METHOD3(onFrameSendCompleted, void(tConnectionHandle ConnectionHandle, int UserData, ESendStatus SendStatus));

        void doDeviceListUpdated(const tDeviceList& DeviceList)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- doDeviceListUpdated -----------------");
            mDeviceList = DeviceList;

            tDeviceList::const_iterator device;
            for(device = mDeviceList.begin(); device != mDeviceList.end(); ++device)
            {
                mTransportManager.connectDevice(device->mDeviceHandle);
            }
        }

        void doApplicationConnected(const SDeviceInfo& ConnectedDevice, const tConnectionHandle Connection)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- doApplicationConnected -----------------");

            mConnectionHandle = Connection;
            mConnected = true;
        }

        void sendTestFrame(void)
        {
            // Sending frame
            uint8_t data[512]={1};
            mTransportManager.sendFrame(mConnectionHandle, data, 512, ++mFrameSequenceNumber);
        }

        void doFrameReceived(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- doFrameReceived -----------------");

            bool finished = false;

            pthread_mutex_lock(&mNumberOfReceivedFramesMutex);
            mNumberOfReceivedFrames++;
            finished = mNumberOfReceivedFrames >= Data::TotalNumberOfCalls;
            pthread_mutex_unlock(&mNumberOfReceivedFramesMutex);

            if (finished)
            {
                checkIfFinished();
            }

            if (mCurrentIteration == 0)
            {   // start sending frame on first iteration
                mCurrentIteration++;
                sendTestFrame();
            }
        }

        void doFrameSendCompleted(tConnectionHandle ConnectionHandle, int UserData, ESendStatus SendStatus)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- doFrameSendCompleted, UserData: " << UserData << " -----------------");

            if (mCurrentIteration < Data::NumberOfIterations)
            {
                sendTestFrame();
            }
            else if (mCurrentIteration == Data::TotalNumberOfCalls)     // received all events
            {
                checkIfFinished();
            }

            mCurrentIteration++;
        }

        void doApplicationDisconnected(const SDeviceInfo& DisconnectedDevice, const tConnectionHandle Connection)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- Application disconnected -----------------");

            mConnected = false;
        }

        bool isConnected(void)
        {
            return mConnected;
        }

        void checkIfFinished(void)
        {
            if ( mCurrentIteration >= Data::TotalNumberOfCalls &&
                 mNumberOfReceivedFrames >= Data::TotalNumberOfCalls )
            {
                pthread_mutex_lock(&mFinishedMutex);
                mNumberOfClientsFinished++;
                pthread_mutex_unlock(&mFinishedMutex);

                LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- a client has finished -----------------");
            }
        }

        static bool isAllClientsFinished(void)
        {
            bool finished = false;

            pthread_mutex_lock(&mFinishedMutex);
            finished = mNumberOfClientsFinished >= Data::NumberOfClients;
            pthread_mutex_unlock(&mFinishedMutex);

            return finished;
        }


        void disconnectAllDevices(void)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "\n-------------- Disconnecting devices -----------------");

            tDeviceList::const_iterator device;

            for(device = mDeviceList.begin(); device != mDeviceList.end(); ++device)
            {
                mTransportManager.disconnectDevice(device->mDeviceHandle);
            }
        }

    protected:
        ITransportManager & mTransportManager;
        tDeviceList mDeviceList;
        int mFrameSequenceNumber;
        Logger mLogger;
        int mCurrentIteration;
        tConnectionHandle mConnectionHandle;
        static int mNumberOfClientsFinished;
        static pthread_mutex_t mFinishedMutex;
        bool mConnected;

        int mNumberOfReceivedFrames;
        pthread_mutex_t mNumberOfReceivedFramesMutex;
    };

    int MockTransportManagerClient::mNumberOfClientsFinished = 0;
    pthread_mutex_t MockTransportManagerClient::mFinishedMutex = PTHREAD_MUTEX_INITIALIZER;


    /**
     * @brief Inherited transport manager class used for some small preparation of class for
     *        testing (disabling another adapters etc.)
     **/
    class TestTransportManager : public CTransportManager
    {
    public:
        TestTransportManager(void )
        : CTransportManager()
        , mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManagerTest")))
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
                .Times(Data::NumberOfClients)
                .WillRepeatedly(Invoke(mpTransportAdapter, &MockTransportAdapter::doConnectDevice))
            ;

            EXPECT_CALL(*mpTransportAdapter, sendFrame(Data::ConnectionHandle, _, 512, _))
                .Times(Data::TotalNumberOfCalls)
                .WillRepeatedly(Invoke(mpTransportAdapter, &MockTransportAdapter::doSendFrame))
            ;

            EXPECT_CALL(*mpTransportAdapter, disconnectDevice(Data::DeviceHandle))
                .Times(1)
                .WillOnce(Invoke(mpTransportAdapter, &MockTransportAdapter::doDisconnectDevice))
            ;

            addTransportAdapter(mpTransportAdapter);
            LOG4CPLUS_INFO_EXT(mLogger, "Transport adapters initialized");
        }

    protected:
        MockTransportAdapter *mpTransportAdapter;
        Logger mLogger;
    };

    // ------------------------------------------------------------------------

    static void* startClientThread(void *pData)
    {
        Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManagerTest"));

        LOG4CPLUS_INFO_EXT(logger, "\n--------------------- Client thread started ----------------");

        TestTransportManager *pTm = static_cast<TestTransportManager*>(pData);

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
            .Times(Data::NumberOfClients)
            .WillRepeatedly(Invoke(&tmClient, &MockTransportManagerClient::doApplicationConnected))
        ;

        EXPECT_CALL(tmClient, onFrameReceived(Data::ConnectionHandle, _, 212))
            .Times(Data::TotalNumberOfCalls)
            .WillRepeatedly(Invoke(&tmClient, &MockTransportManagerClient::doFrameReceived))
        ;

        EXPECT_CALL(tmClient, onFrameSendCompleted(Data::ConnectionHandle, _, SendStatusOK))
            .Times(Data::TotalNumberOfCalls)
            .WillRepeatedly(Invoke(&tmClient, &MockTransportManagerClient::doFrameSendCompleted))
        ;

        EXPECT_CALL(tmClient, onApplicationDisconnected(deviceInfo, Data::ConnectionHandle))
            .Times(1)
            .WillOnce(Invoke(&tmClient, &MockTransportManagerClient::doApplicationDisconnected));
        ;

        pTm->addDataListener(&tmClient);
        pTm->addDeviceListener(&tmClient);

        while (!MockTransportManagerClient::isAllClientsFinished())
        {
            sleep(1);
        }

        while (tmClient.isConnected())  // wait for a client to receive onApplicationDisconnected
        {
            sleep(1);
        }

        pTm->removeDataListener(&tmClient);
        pTm->removeDeviceListener(&tmClient);

        return NULL;
    }

    // ----------------------- TESTS ----------------------- //

    TEST(test_TestWithCorrectTransportAdapter, CorrectTransportAdapterBehavior)
    {
        Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManagerTest"));

        LOG4CPLUS_INFO_EXT(logger, "*************************** Starting test *****************************");
        // All expectations must be sequenced
        //InSequence dummy;

        // Creating transport manager
        TestTransportManager *pTm = new TestTransportManager();

        LOG4CPLUS_INFO_EXT(logger, "\n--------------------- Calling RUN ----------------");
        pTm->run();

        std::vector<pthread_t> threads;

        for (int i = 0; i < Data::NumberOfClients; ++i)
        {
            pthread_t thread;
            pthread_create(&thread, NULL, &startClientThread, pTm);

            threads.push_back(thread);
        }

        sleep(1);

        LOG4CPLUS_INFO_EXT(logger, "\n--------------------- Calling SCAN FOR DEVICES ----------------");
        pTm->scanForNewDevices();

        while (!MockTransportManagerClient::isAllClientsFinished())
        {
            sleep(1);
        }

        LOG4CPLUS_INFO_EXT(logger, "\n--------------------- All clients have finished ----------------");

        pTm->disconnectDevice(Data::DeviceHandle);

        for (int i = 0; i < Data::NumberOfClients; ++i)
        {
            pthread_join(threads[i], NULL);
        }

        LOG4CPLUS_INFO_EXT(logger, "*************************** Deleting TM and shutting down *****************************");

        // Shutdown transport manager
        delete pTm;
        pTm = 0;
    }
}}}} // End of namespaces



int main(int argc, char **argv) {
  PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
