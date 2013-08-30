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
    Tests Transport Manager functionality with multiple (256) device adapters that connects/disconnects/sents data
 */
#include <algorithm>
#include <pthread.h>
#include <time.h>

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
        namespace TransportManager
        {
            namespace MultipleTransportAdaptersTest
            {
                const size_t cNumberOfMockTransportAdapters = 256u;

                class MockDataListener: public NsSmartDeviceLink::NsTransportManager::ITransportManagerDataListener
                {
                public:
                    MockDataListener(pthread_mutex_t & Mutex);

                    MOCK_METHOD3(onFrameReceived, void(NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize));
                    MOCK_METHOD3(onFrameSendCompleted, void(NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle, int UserData, NsSmartDeviceLink::NsTransportManager::ESendStatus SendStatus));

                    void unlockMutexAfterFrameReceived(NsSmartDeviceLink::NsTransportManager::tConnectionHandle, const uint8_t *, size_t);
                    void unlockMutexAfterFrameSendCompleted(NsSmartDeviceLink::NsTransportManager::tConnectionHandle, int, NsSmartDeviceLink::NsTransportManager::ESendStatus);

                private:
                    pthread_mutex_t & mMutex;
                };

                class MockDeviceListener: public NsSmartDeviceLink::NsTransportManager::ITransportManagerDeviceListener
                {
                public:
                    MockDeviceListener(pthread_mutex_t & Mutex);

                    MOCK_METHOD1(onDeviceListUpdated, void(const NsSmartDeviceLink::NsTransportManager::tDeviceList & DeviceList));
                    MOCK_METHOD2(onApplicationConnected, void(const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & ConnectedDevice, const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection));
                    MOCK_METHOD2(onApplicationDisconnected, void(const NsSmartDeviceLink::NsTransportManager::SDeviceInfo & DisconnectedDevice, const NsSmartDeviceLink::NsTransportManager::tConnectionHandle Connection));

                    void unlockMutexAfterDeviceListUpdate(const NsSmartDeviceLink::NsTransportManager::tDeviceList &);
                    void unlockMutexAfterApplicationConnectedDisconnected(const NsSmartDeviceLink::NsTransportManager::SDeviceInfo &, const NsSmartDeviceLink::NsTransportManager::tConnectionHandle);

                private:
                    pthread_mutex_t & mMutex;
                };

                class MockTransportAdapter: public NsSmartDeviceLink::NsTransportManager::ITransportAdapter
                {
                public:
                    MockTransportAdapter(pthread_mutex_t & Mutex);
                    virtual NsSmartDeviceLink::NsTransportManager::EDeviceType GetDeviceType(void) const;

                    MOCK_METHOD1(connectDevice, void (const NsSmartDeviceLink::NsTransportManager::tDeviceHandle DeviceHandle));
                    MOCK_METHOD1(disconnectDevice, void (const NsSmartDeviceLink::NsTransportManager::tDeviceHandle DeviceHandle));
                    MOCK_METHOD0(run, void());
                    MOCK_METHOD0(scanForNewDevices, void());
                    MOCK_METHOD4(sendFrame, void(NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize, int UserData));

                    void unlockMutexAfterSendFrame(NsSmartDeviceLink::NsTransportManager::tConnectionHandle, const uint8_t *, size_t, int UserData);

                    NsSmartDeviceLink::NsTransportManager::SDeviceInfo mMockDeviceInfo;
                    NsSmartDeviceLink::NsTransportManager::tConnectionHandle mMockConnectionHandle;
                    int mMockUserData;

                private:
                    pthread_mutex_t & mMutex;
                };

                class TestTransportManager: public NsSmartDeviceLink::NsTransportManager::CTransportManager
                {
                public:
                    TestTransportManager(test::components::TransportManager::MultipleTransportAdaptersTest::MockTransportAdapter * (& TransportAdapters)[cNumberOfMockTransportAdapters]);
                    ~TestTransportManager(void);
                protected:
                    virtual void initializeTransportAdapters(void);

                private:
                    MockTransportAdapter * (& mMockTransportAdapters)[cNumberOfMockTransportAdapters];
                };

                template <typename Type>
                struct TRemoveReference
                {
                    typedef Type tType;
                };

                template <typename Type>
                struct TRemoveReference<Type &>
                {
                    typedef Type tType;
                };
            }
        }
    }
}

test::components::TransportManager::MultipleTransportAdaptersTest::MockDataListener::MockDataListener(pthread_mutex_t & Mutex):
mMutex(Mutex)
{
}

void test::components::TransportManager::MultipleTransportAdaptersTest::MockDataListener::unlockMutexAfterFrameReceived(NsSmartDeviceLink::NsTransportManager::tConnectionHandle, const uint8_t *, size_t)
{
    pthread_mutex_unlock(&mMutex);
}

void test::components::TransportManager::MultipleTransportAdaptersTest::MockDataListener::unlockMutexAfterFrameSendCompleted(NsSmartDeviceLink::NsTransportManager::tConnectionHandle, int, NsSmartDeviceLink::NsTransportManager::ESendStatus)
{
    pthread_mutex_unlock(&mMutex);
}

test::components::TransportManager::MultipleTransportAdaptersTest::MockDeviceListener::MockDeviceListener(pthread_mutex_t & Mutex):
mMutex(Mutex)
{
}

void test::components::TransportManager::MultipleTransportAdaptersTest::MockDeviceListener::unlockMutexAfterDeviceListUpdate(const NsSmartDeviceLink::NsTransportManager::tDeviceList &)
{
    pthread_mutex_unlock(&mMutex);
}

void test::components::TransportManager::MultipleTransportAdaptersTest::MockDeviceListener::unlockMutexAfterApplicationConnectedDisconnected(const NsSmartDeviceLink::NsTransportManager::SDeviceInfo &, const NsSmartDeviceLink::NsTransportManager::tConnectionHandle)
{
    pthread_mutex_unlock(&mMutex);
}

test::components::TransportManager::MultipleTransportAdaptersTest::MockTransportAdapter::MockTransportAdapter(pthread_mutex_t & Mutex):
mMockDeviceInfo(),
mMockConnectionHandle(NsSmartDeviceLink::NsTransportManager::InvalidConnectionHandle),
mMockUserData(-1),
mMutex(Mutex)
{
    mMockDeviceInfo.mDeviceHandle = NsSmartDeviceLink::NsTransportManager::InvalidDeviceHandle;
    mMockDeviceInfo.mDeviceType = NsSmartDeviceLink::NsTransportManager::DeviceBluetooth;
}

NsSmartDeviceLink::NsTransportManager::EDeviceType test::components::TransportManager::MultipleTransportAdaptersTest::MockTransportAdapter::GetDeviceType(void) const
{
    return mMockDeviceInfo.mDeviceType;
}

void test::components::TransportManager::MultipleTransportAdaptersTest::MockTransportAdapter::unlockMutexAfterSendFrame(NsSmartDeviceLink::NsTransportManager::tConnectionHandle, const uint8_t *, size_t, int UserData)
{
    pthread_mutex_unlock(&mMutex);
}

test::components::TransportManager::MultipleTransportAdaptersTest::TestTransportManager::TestTransportManager(test::components::TransportManager::MultipleTransportAdaptersTest::MockTransportAdapter * (& TransportAdapters)[test::components::TransportManager::MultipleTransportAdaptersTest::cNumberOfMockTransportAdapters]):
CTransportManager(),
mMockTransportAdapters(TransportAdapters)
{
}

test::components::TransportManager::MultipleTransportAdaptersTest::TestTransportManager::~TestTransportManager(void)
{
}

void test::components::TransportManager::MultipleTransportAdaptersTest::TestTransportManager::initializeTransportAdapters(void)
{
    std::for_each(&mMockTransportAdapters[0], &mMockTransportAdapters[cNumberOfMockTransportAdapters], [this] (MockTransportAdapter * TransportAdapter) {addTransportAdapter(TransportAdapter);});
}

MATCHER_P(ContainsSameElements, value, "Matches two containers if they contain the same set of elements regardless of order")
{
    bool result = false;

    typedef typename test::components::TransportManager::MultipleTransportAdaptersTest::TRemoveReference<arg_type>::tType::const_iterator tIteratorType;

    if (arg.size() == value.size())
    {
        std::set<tIteratorType> matchedValueIterators;
        tIteratorType ai;

        for (ai = arg.begin(); ai != arg.end(); ++ai)
        {
            tIteratorType vi;

            for (vi = value.begin(); vi != value.end(); ++vi)
            {
                if (matchedValueIterators.end() == matchedValueIterators.find(vi))
                {
                    if (*vi == *ai)
                    {
                        matchedValueIterators.insert(vi);
                        break;
                    }
                }
            }

            if (value.end() == vi)
            {
                break;
            }
        }

        result = (arg.end() == ai);
    }

    return result;
}

MATCHER_P2(BuffersSame, buffer, size, "Matches two buffers contents")
{
    bool result = false;

    if (size == std::tr1::get<1>(arg))
    {
        result = (0 == memcmp(buffer, std::tr1::get<0>(arg), size));
    }

    return result;
}

#define TRY_LOCK_AND_FAIL_ON_TIMEOUT(mutex)                     \
    {                                                           \
        timespec timeoutTime;                                   \
                                                                \
        clock_gettime(CLOCK_REALTIME, &timeoutTime);            \
        timeoutTime.tv_sec += 1;                                \
                                                                \
        if (0 != pthread_mutex_timedlock(&mutex, &timeoutTime)) \
        {                                                       \
            FAIL() << "Mutex lock timeout";                     \
        }                                                       \
    }

TEST(TransportManager, MultipleTransportAdapters)
{
    pthread_mutex_t callbacksMutex; // Mutex restricting access to deviceList, mockDataListener and mockDeviceListener

    pthread_mutex_init(&callbacksMutex, 0);

    test::components::TransportManager::MultipleTransportAdaptersTest::MockTransportAdapter * mockTransportAdapters[test::components::TransportManager::MultipleTransportAdaptersTest::cNumberOfMockTransportAdapters];
    std::for_each(&mockTransportAdapters[0], &mockTransportAdapters[test::components::TransportManager::MultipleTransportAdaptersTest::cNumberOfMockTransportAdapters], [&callbacksMutex] (test::components::TransportManager::MultipleTransportAdaptersTest::MockTransportAdapter *& TransportAdapter) {TransportAdapter = new test::components::TransportManager::MultipleTransportAdaptersTest::MockTransportAdapter(callbacksMutex);});

    test::components::TransportManager::MultipleTransportAdaptersTest::MockDataListener mockDataListener(callbacksMutex);
    test::components::TransportManager::MultipleTransportAdaptersTest::MockDeviceListener mockDeviceListener(callbacksMutex);
    test::components::TransportManager::MultipleTransportAdaptersTest::TestTransportManager transportManager(mockTransportAdapters);

    std::for_each(&mockTransportAdapters[0], &mockTransportAdapters[test::components::TransportManager::MultipleTransportAdaptersTest::cNumberOfMockTransportAdapters], [] (test::components::TransportManager::MultipleTransportAdaptersTest::MockTransportAdapter * TransportAdapter) {EXPECT_CALL(*TransportAdapter, run()).Times(1);});

    transportManager.run();

    transportManager.addDeviceListener(&mockDeviceListener);
    transportManager.addDataListener(&mockDataListener);

    NsSmartDeviceLink::NsTransportManager::tDeviceHandle lastUsedDeviceHandle = 0;
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle lastUsedConnectionHandle = 0;

    NsSmartDeviceLink::NsTransportManager::tDeviceList deviceList;

    EXPECT_CALL(mockDeviceListener, onDeviceListUpdated(::testing::_)).Times(0);
    EXPECT_CALL(mockDeviceListener, onApplicationConnected(::testing::_, ::testing::_)).Times(0);
    EXPECT_CALL(mockDeviceListener, onApplicationDisconnected(::testing::_, ::testing::_)).Times(0);
    EXPECT_CALL(mockDataListener, onFrameReceived(::testing::_, ::testing::_, ::testing::_)).Times(0);
    EXPECT_CALL(mockDataListener, onFrameSendCompleted(::testing::_, ::testing::_, ::testing::_)).Times(0);

    for (int i = 0; i < 10000; ++i)
    {
        size_t deviceAdapterIndex = static_cast<size_t>(rand()) % test::components::TransportManager::MultipleTransportAdaptersTest::cNumberOfMockTransportAdapters;
        test::components::TransportManager::MultipleTransportAdaptersTest::MockTransportAdapter & mockTransportAdapter = *mockTransportAdapters[deviceAdapterIndex];

        if (NsSmartDeviceLink::NsTransportManager::InvalidDeviceHandle == mockTransportAdapter.mMockDeviceInfo.mDeviceHandle)
        {
            mockTransportAdapter.mMockDeviceInfo.mDeviceHandle = ++lastUsedDeviceHandle;

            char deviceNumberString[16];
            sprintf(deviceNumberString, "%d", mockTransportAdapter.mMockDeviceInfo.mDeviceHandle);

            mockTransportAdapter.mMockDeviceInfo.mUniqueDeviceId = std::string("id-") + deviceNumberString;
            mockTransportAdapter.mMockDeviceInfo.mUserFriendlyName = std::string("Name ") + deviceNumberString;

            NsSmartDeviceLink::NsTransportManager::tInternalDeviceList internalDeviceList;
            internalDeviceList.push_back(NsSmartDeviceLink::NsTransportManager::SInternalDeviceInfo(mockTransportAdapter.mMockDeviceInfo.mDeviceHandle, mockTransportAdapter.mMockDeviceInfo.mUserFriendlyName, mockTransportAdapter.mMockDeviceInfo.mUniqueDeviceId));

            TRY_LOCK_AND_FAIL_ON_TIMEOUT(callbacksMutex);

            deviceList.push_back(mockTransportAdapter.mMockDeviceInfo);

            EXPECT_CALL(mockDeviceListener, onDeviceListUpdated(ContainsSameElements(deviceList)))
                .Times(1)
                .WillOnce(::testing::Invoke(&mockDeviceListener, &test::components::TransportManager::MultipleTransportAdaptersTest::MockDeviceListener::unlockMutexAfterDeviceListUpdate))
                .RetiresOnSaturation();

            transportManager.onDeviceListUpdated(&mockTransportAdapter, internalDeviceList);
        }
        else if (NsSmartDeviceLink::NsTransportManager::InvalidConnectionHandle == mockTransportAdapter.mMockConnectionHandle)
        {
            if (0 == rand() % 2)
            {
                TRY_LOCK_AND_FAIL_ON_TIMEOUT(callbacksMutex);

                for (auto i = deviceList.begin(); i != deviceList.end(); ++i)
                {
                    if (i->mDeviceHandle == mockTransportAdapter.mMockDeviceInfo.mDeviceHandle)
                    {
                        deviceList.erase(i);
                        break;
                    }
                }

                mockTransportAdapter.mMockDeviceInfo.mDeviceHandle = NsSmartDeviceLink::NsTransportManager::InvalidDeviceHandle;

                NsSmartDeviceLink::NsTransportManager::tInternalDeviceList internalDeviceList;

                EXPECT_CALL(mockDeviceListener, onDeviceListUpdated(ContainsSameElements(deviceList)))
                    .Times(1)
                    .WillOnce(::testing::Invoke(&mockDeviceListener, &test::components::TransportManager::MultipleTransportAdaptersTest::MockDeviceListener::unlockMutexAfterDeviceListUpdate))
                    .RetiresOnSaturation();

                transportManager.onDeviceListUpdated(&mockTransportAdapter, internalDeviceList);
            }
            else
            {
                mockTransportAdapter.mMockConnectionHandle = ++lastUsedConnectionHandle;

                TRY_LOCK_AND_FAIL_ON_TIMEOUT(callbacksMutex);

                EXPECT_CALL(mockDeviceListener, onApplicationConnected(mockTransportAdapter.mMockDeviceInfo, mockTransportAdapter.mMockConnectionHandle))
                    .Times(1)
                    .WillOnce(::testing::Invoke(&mockDeviceListener, &test::components::TransportManager::MultipleTransportAdaptersTest::MockDeviceListener::unlockMutexAfterApplicationConnectedDisconnected))
                    .RetiresOnSaturation();

                transportManager.onApplicationConnected(&mockTransportAdapter, mockTransportAdapter.mMockDeviceInfo, mockTransportAdapter.mMockConnectionHandle);
            }
        }
        else
        {
            static const uint8_t frameData[16] = {0x11, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08};

            if (0 == rand() % 2)
            {
                TRY_LOCK_AND_FAIL_ON_TIMEOUT(callbacksMutex);

                EXPECT_CALL(mockDataListener, onFrameReceived(mockTransportAdapter.mMockConnectionHandle, ::testing::_, ::testing::_))
                    .With(::testing::Args<1, 2>(BuffersSame(frameData, sizeof(frameData))))
                    .Times(1)
                    .WillOnce(::testing::Invoke(&mockDataListener, &test::components::TransportManager::MultipleTransportAdaptersTest::MockDataListener::unlockMutexAfterFrameReceived))
                    .RetiresOnSaturation();

                transportManager.onFrameReceived(&mockTransportAdapter, mockTransportAdapter.mMockConnectionHandle, frameData, sizeof(frameData));
            }
            else if (0 == rand() % 2)
            {
                TRY_LOCK_AND_FAIL_ON_TIMEOUT(callbacksMutex);

                mockTransportAdapter.mMockUserData = rand();

                EXPECT_CALL(mockTransportAdapter, sendFrame(mockTransportAdapter.mMockConnectionHandle, ::testing::_, ::testing::_, mockTransportAdapter.mMockUserData))
                    .With(::testing::Args<1, 2>(BuffersSame(frameData, sizeof(frameData))))
                    .Times(1)
                    .WillOnce(::testing::Invoke(&mockTransportAdapter, &test::components::TransportManager::MultipleTransportAdaptersTest::MockTransportAdapter::unlockMutexAfterSendFrame))
                    .RetiresOnSaturation();

                transportManager.sendFrame(mockTransportAdapter.mMockConnectionHandle, frameData, sizeof(frameData), mockTransportAdapter.mMockUserData);

                TRY_LOCK_AND_FAIL_ON_TIMEOUT(callbacksMutex);

                EXPECT_CALL(mockDataListener, onFrameSendCompleted(mockTransportAdapter.mMockConnectionHandle, mockTransportAdapter.mMockUserData, NsSmartDeviceLink::NsTransportManager::SendStatusOK))
                    .Times(1)
                    .WillOnce(::testing::Invoke(&mockDataListener, &test::components::TransportManager::MultipleTransportAdaptersTest::MockDataListener::unlockMutexAfterFrameSendCompleted))
                    .RetiresOnSaturation();

                transportManager.onFrameSendCompleted(&mockTransportAdapter, mockTransportAdapter.mMockConnectionHandle, mockTransportAdapter.mMockUserData, NsSmartDeviceLink::NsTransportManager::SendStatusOK);
            }
            else
            {
                TRY_LOCK_AND_FAIL_ON_TIMEOUT(callbacksMutex);

                EXPECT_CALL(mockDeviceListener, onApplicationDisconnected(mockTransportAdapter.mMockDeviceInfo, mockTransportAdapter.mMockConnectionHandle))
                    .Times(1)
                    .WillOnce(::testing::Invoke(&mockDeviceListener, &test::components::TransportManager::MultipleTransportAdaptersTest::MockDeviceListener::unlockMutexAfterApplicationConnectedDisconnected))
                    .RetiresOnSaturation();

                transportManager.onApplicationDisconnected(&mockTransportAdapter, mockTransportAdapter.mMockDeviceInfo, mockTransportAdapter.mMockConnectionHandle);

                mockTransportAdapter.mMockConnectionHandle = NsSmartDeviceLink::NsTransportManager::InvalidConnectionHandle;
            }
        }
    }

    pthread_mutex_destroy(&callbacksMutex);

    sleep(1);
}

int main(int argc, char **argv)
{
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus_disabled.properties"));

    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
