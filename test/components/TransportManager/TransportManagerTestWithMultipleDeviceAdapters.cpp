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
#include "../../../src/components/TransportManager/src/IDeviceAdapter.hpp"

namespace test
{
    namespace components
    {
        namespace TransportManager
        {
            namespace MultipleDeviceAdaptersTest
            {
                const size_t cNumberOfMockDeviceAdapters = 256u;

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

                class MockDeviceAdapter: public NsSmartDeviceLink::NsTransportManager::IDeviceAdapter
                {
                public:
                    MockDeviceAdapter(pthread_mutex_t & Mutex);
                    virtual NsSmartDeviceLink::NsTransportManager::EDeviceType getDeviceType(void) const;

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
                    TestTransportManager(test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceAdapter * (& DeviceAdapters)[cNumberOfMockDeviceAdapters]);
                    ~TestTransportManager(void);
                protected:
                    virtual void initializeDeviceAdapters(void);

                private:
                    MockDeviceAdapter * (& mMockDeviceAdapters)[cNumberOfMockDeviceAdapters];
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

test::components::TransportManager::MultipleDeviceAdaptersTest::MockDataListener::MockDataListener(pthread_mutex_t & Mutex):
mMutex(Mutex)
{
}

void test::components::TransportManager::MultipleDeviceAdaptersTest::MockDataListener::unlockMutexAfterFrameReceived(NsSmartDeviceLink::NsTransportManager::tConnectionHandle, const uint8_t *, size_t)
{
    pthread_mutex_unlock(&mMutex);
}

void test::components::TransportManager::MultipleDeviceAdaptersTest::MockDataListener::unlockMutexAfterFrameSendCompleted(NsSmartDeviceLink::NsTransportManager::tConnectionHandle, int, NsSmartDeviceLink::NsTransportManager::ESendStatus)
{
    pthread_mutex_unlock(&mMutex);
}

test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceListener::MockDeviceListener(pthread_mutex_t & Mutex):
mMutex(Mutex)
{
}

void test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceListener::unlockMutexAfterDeviceListUpdate(const NsSmartDeviceLink::NsTransportManager::tDeviceList &)
{
    pthread_mutex_unlock(&mMutex);
}

void test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceListener::unlockMutexAfterApplicationConnectedDisconnected(const NsSmartDeviceLink::NsTransportManager::SDeviceInfo &, const NsSmartDeviceLink::NsTransportManager::tConnectionHandle)
{
    pthread_mutex_unlock(&mMutex);
}

test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceAdapter::MockDeviceAdapter(pthread_mutex_t & Mutex):
mMockDeviceInfo(),
mMockConnectionHandle(NsSmartDeviceLink::NsTransportManager::InvalidConnectionHandle),
mMockUserData(-1),
mMutex(Mutex)
{
    mMockDeviceInfo.mDeviceHandle = NsSmartDeviceLink::NsTransportManager::InvalidDeviceHandle;
    mMockDeviceInfo.mDeviceType = NsSmartDeviceLink::NsTransportManager::DeviceBluetooth;
}

NsSmartDeviceLink::NsTransportManager::EDeviceType test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceAdapter::getDeviceType(void) const
{
    return mMockDeviceInfo.mDeviceType;
}

void test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceAdapter::unlockMutexAfterSendFrame(NsSmartDeviceLink::NsTransportManager::tConnectionHandle, const uint8_t *, size_t, int UserData)
{
    pthread_mutex_unlock(&mMutex);
}

test::components::TransportManager::MultipleDeviceAdaptersTest::TestTransportManager::TestTransportManager(test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceAdapter * (& DeviceAdapters)[test::components::TransportManager::MultipleDeviceAdaptersTest::cNumberOfMockDeviceAdapters]):
CTransportManager(),
mMockDeviceAdapters(DeviceAdapters)
{
}

test::components::TransportManager::MultipleDeviceAdaptersTest::TestTransportManager::~TestTransportManager(void)
{
}

void test::components::TransportManager::MultipleDeviceAdaptersTest::TestTransportManager::initializeDeviceAdapters(void)
{
    std::for_each(&mMockDeviceAdapters[0], &mMockDeviceAdapters[cNumberOfMockDeviceAdapters], [this] (MockDeviceAdapter * DeviceAdapter) {addDeviceAdapter(DeviceAdapter);});
}

MATCHER_P(ContainsSameElements, value, "Matches two containers if they contain the same set of elements regardless of order")
{
    bool result = false;

    typedef typename test::components::TransportManager::MultipleDeviceAdaptersTest::TRemoveReference<arg_type>::tType::const_iterator tIteratorType;

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

TEST(TransportManager, MultipleDeviceAdapters)
{
    pthread_mutex_t callbacksMutex; // Mutex restricting access to deviceList, mockDataListener and mockDeviceListener

    pthread_mutex_init(&callbacksMutex, 0);

    test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceAdapter * mockDeviceAdapters[test::components::TransportManager::MultipleDeviceAdaptersTest::cNumberOfMockDeviceAdapters];
    std::for_each(&mockDeviceAdapters[0], &mockDeviceAdapters[test::components::TransportManager::MultipleDeviceAdaptersTest::cNumberOfMockDeviceAdapters], [&callbacksMutex] (test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceAdapter *& DeviceAdapter) {DeviceAdapter = new test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceAdapter(callbacksMutex);});

    test::components::TransportManager::MultipleDeviceAdaptersTest::MockDataListener mockDataListener(callbacksMutex);
    test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceListener mockDeviceListener(callbacksMutex);
    test::components::TransportManager::MultipleDeviceAdaptersTest::TestTransportManager transportManager(mockDeviceAdapters);

    std::for_each(&mockDeviceAdapters[0], &mockDeviceAdapters[test::components::TransportManager::MultipleDeviceAdaptersTest::cNumberOfMockDeviceAdapters], [] (test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceAdapter * DeviceAdapter) {EXPECT_CALL(*DeviceAdapter, run()).Times(1);});

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
        size_t deviceAdapterIndex = static_cast<size_t>(rand()) % test::components::TransportManager::MultipleDeviceAdaptersTest::cNumberOfMockDeviceAdapters;
        test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceAdapter & mockDeviceAdapter = *mockDeviceAdapters[deviceAdapterIndex];

        if (NsSmartDeviceLink::NsTransportManager::InvalidDeviceHandle == mockDeviceAdapter.mMockDeviceInfo.mDeviceHandle)
        {
            mockDeviceAdapter.mMockDeviceInfo.mDeviceHandle = ++lastUsedDeviceHandle;

            char deviceNumberString[16];
            sprintf(deviceNumberString, "%d", mockDeviceAdapter.mMockDeviceInfo.mDeviceHandle);

            mockDeviceAdapter.mMockDeviceInfo.mUniqueDeviceId = std::string("id-") + deviceNumberString;
            mockDeviceAdapter.mMockDeviceInfo.mUserFriendlyName = std::string("Name ") + deviceNumberString;

            NsSmartDeviceLink::NsTransportManager::tInternalDeviceList internalDeviceList;
            internalDeviceList.push_back(NsSmartDeviceLink::NsTransportManager::SInternalDeviceInfo(mockDeviceAdapter.mMockDeviceInfo.mDeviceHandle, mockDeviceAdapter.mMockDeviceInfo.mUserFriendlyName, mockDeviceAdapter.mMockDeviceInfo.mUniqueDeviceId));

            TRY_LOCK_AND_FAIL_ON_TIMEOUT(callbacksMutex);

            deviceList.push_back(mockDeviceAdapter.mMockDeviceInfo);

            EXPECT_CALL(mockDeviceListener, onDeviceListUpdated(ContainsSameElements(deviceList)))
                .Times(1)
                .WillOnce(::testing::Invoke(&mockDeviceListener, &test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceListener::unlockMutexAfterDeviceListUpdate))
                .RetiresOnSaturation();

            transportManager.onDeviceListUpdated(&mockDeviceAdapter, internalDeviceList);
        }
        else if (NsSmartDeviceLink::NsTransportManager::InvalidConnectionHandle == mockDeviceAdapter.mMockConnectionHandle)
        {
            if (0 == rand() % 2)
            {
                TRY_LOCK_AND_FAIL_ON_TIMEOUT(callbacksMutex);

                for (auto i = deviceList.begin(); i != deviceList.end(); ++i)
                {
                    if (i->mDeviceHandle == mockDeviceAdapter.mMockDeviceInfo.mDeviceHandle)
                    {
                        deviceList.erase(i);
                        break;
                    }
                }

                mockDeviceAdapter.mMockDeviceInfo.mDeviceHandle = NsSmartDeviceLink::NsTransportManager::InvalidDeviceHandle;

                NsSmartDeviceLink::NsTransportManager::tInternalDeviceList internalDeviceList;

                EXPECT_CALL(mockDeviceListener, onDeviceListUpdated(ContainsSameElements(deviceList)))
                    .Times(1)
                    .WillOnce(::testing::Invoke(&mockDeviceListener, &test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceListener::unlockMutexAfterDeviceListUpdate))
                    .RetiresOnSaturation();

                transportManager.onDeviceListUpdated(&mockDeviceAdapter, internalDeviceList);
            }
            else
            {
                mockDeviceAdapter.mMockConnectionHandle = ++lastUsedConnectionHandle;

                TRY_LOCK_AND_FAIL_ON_TIMEOUT(callbacksMutex);

                EXPECT_CALL(mockDeviceListener, onApplicationConnected(mockDeviceAdapter.mMockDeviceInfo, mockDeviceAdapter.mMockConnectionHandle))
                    .Times(1)
                    .WillOnce(::testing::Invoke(&mockDeviceListener, &test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceListener::unlockMutexAfterApplicationConnectedDisconnected))
                    .RetiresOnSaturation();

                transportManager.onApplicationConnected(&mockDeviceAdapter, mockDeviceAdapter.mMockDeviceInfo, mockDeviceAdapter.mMockConnectionHandle);
            }
        }
        else
        {
            static const uint8_t frameData[16] = {0x11, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08};

            if (0 == rand() % 2)
            {
                TRY_LOCK_AND_FAIL_ON_TIMEOUT(callbacksMutex);

                EXPECT_CALL(mockDataListener, onFrameReceived(mockDeviceAdapter.mMockConnectionHandle, ::testing::_, ::testing::_))
                    .With(::testing::Args<1, 2>(BuffersSame(frameData, sizeof(frameData))))
                    .Times(1)
                    .WillOnce(::testing::Invoke(&mockDataListener, &test::components::TransportManager::MultipleDeviceAdaptersTest::MockDataListener::unlockMutexAfterFrameReceived))
                    .RetiresOnSaturation();

                transportManager.onFrameReceived(&mockDeviceAdapter, mockDeviceAdapter.mMockConnectionHandle, frameData, sizeof(frameData));
            }
            else if (0 == rand() % 2)
            {
                TRY_LOCK_AND_FAIL_ON_TIMEOUT(callbacksMutex);

                mockDeviceAdapter.mMockUserData = rand();

                EXPECT_CALL(mockDeviceAdapter, sendFrame(mockDeviceAdapter.mMockConnectionHandle, ::testing::_, ::testing::_, mockDeviceAdapter.mMockUserData))
                    .With(::testing::Args<1, 2>(BuffersSame(frameData, sizeof(frameData))))
                    .Times(1)
                    .WillOnce(::testing::Invoke(&mockDeviceAdapter, &test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceAdapter::unlockMutexAfterSendFrame))
                    .RetiresOnSaturation();

                transportManager.sendFrame(mockDeviceAdapter.mMockConnectionHandle, frameData, sizeof(frameData), mockDeviceAdapter.mMockUserData);

                TRY_LOCK_AND_FAIL_ON_TIMEOUT(callbacksMutex);

                EXPECT_CALL(mockDataListener, onFrameSendCompleted(mockDeviceAdapter.mMockConnectionHandle, mockDeviceAdapter.mMockUserData, NsSmartDeviceLink::NsTransportManager::SendStatusOK))
                    .Times(1)
                    .WillOnce(::testing::Invoke(&mockDataListener, &test::components::TransportManager::MultipleDeviceAdaptersTest::MockDataListener::unlockMutexAfterFrameSendCompleted))
                    .RetiresOnSaturation();

                transportManager.onFrameSendCompleted(&mockDeviceAdapter, mockDeviceAdapter.mMockConnectionHandle, mockDeviceAdapter.mMockUserData, NsSmartDeviceLink::NsTransportManager::SendStatusOK);
            }
            else
            {
                TRY_LOCK_AND_FAIL_ON_TIMEOUT(callbacksMutex);

                EXPECT_CALL(mockDeviceListener, onApplicationDisconnected(mockDeviceAdapter.mMockDeviceInfo, mockDeviceAdapter.mMockConnectionHandle))
                    .Times(1)
                    .WillOnce(::testing::Invoke(&mockDeviceListener, &test::components::TransportManager::MultipleDeviceAdaptersTest::MockDeviceListener::unlockMutexAfterApplicationConnectedDisconnected))
                    .RetiresOnSaturation();

                transportManager.onApplicationDisconnected(&mockDeviceAdapter, mockDeviceAdapter.mMockDeviceInfo, mockDeviceAdapter.mMockConnectionHandle);

                mockDeviceAdapter.mMockConnectionHandle = NsSmartDeviceLink::NsTransportManager::InvalidConnectionHandle;
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
