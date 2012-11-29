#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../../../src/components/TransportManager/src/IDeviceAdapter.hpp"
#include "../../../src/components/TransportManager/src/CTransportManager.hpp"
#include "TransportManager/ITransportManagerDataListener.hpp"
#include "TransportManager/ITransportManagerDeviceListener.hpp"

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

using namespace NsAppLink::NsTransportManager;

namespace test { namespace components { namespace TransportManager { namespace TestWithMultiThreadedClient {

    // ---------------- TEST DATA ---------------- //
    namespace Data
    {
        static const tDeviceHandle DeviceHandle = 123;
        static const EDeviceType DeviceType = DeviceBluetooth;
        static const std::string UserFriendlyName("MY USER FRIENDLY NAME");
        static const std::string UniqueDeviceId("MY_UNIQUE_DEVICE_ID");

        static const tConnectionHandle ConnectionHandle = 666;

        static const int BaseSequenceNumber = 123;
        static const int NumberOfThreads    = 256;
        static const int NumberOfFramesPerThread   = 64;
        static const int TotalNumberOfFrames = NumberOfThreads * NumberOfFramesPerThread;
    }

    // ---------------- TEST CLASSES ---------------- //

    /**
     * @brief Class that represents custom device adapter that will send known data
     *        and check it's methods calls
     **/
    class MockDeviceAdapter : public IDeviceAdapter
    {
    public:
        MockDeviceAdapter(IDeviceAdapterListener & Listener, IHandleGenerator & HandleGenerator)
        : mListener(Listener)
        , mHandleGenerator(HandleGenerator)
        , mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManagerTest")))
        {
        }

        virtual EDeviceType getDeviceType(void ) const
        {
            return DeviceBluetooth;
        }
        MOCK_METHOD1(connectDevice, void (const tDeviceHandle DeviceHandle));
        MOCK_METHOD1(disconnectDevice, void (const tDeviceHandle DeviceHandle));
        MOCK_METHOD0(run, void());
        MOCK_METHOD0(scanForNewDevices, void());
        MOCK_METHOD3(sendFrame, int(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize));

        void doScanForNewDevices()
        {
            LOG4CPLUS_INFO_EXT(mLogger, "-------------- Scanning new devices -----------------");
            SInternalDeviceInfo deviceInfo;
            deviceInfo.mDeviceHandle = Data::DeviceHandle;
            deviceInfo.mUniqueDeviceId = Data::UniqueDeviceId;
            deviceInfo.mUserFriendlyName = Data::UserFriendlyName;

            tInternalDeviceList list;
            list.push_back(deviceInfo);

            LOG4CPLUS_INFO_EXT(mLogger, "-------------- Sending device list update -----------------");
            mListener.onDeviceListUpdated(this, list);
        }

        void doConnectDevice(const tDeviceHandle DeviceHandle)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "-------------- Connecting device -----------------");
            // Application connect

            SDeviceInfo deviceInfo;
            deviceInfo.mDeviceHandle = Data::DeviceHandle;
            deviceInfo.mUniqueDeviceId = Data::UniqueDeviceId;
            deviceInfo.mUserFriendlyName = Data::UserFriendlyName;

            LOG4CPLUS_INFO_EXT(mLogger, "-------------- Sending ApplicationConnected -----------------");
            mListener.onApplicationConnected(this, deviceInfo, Data::ConnectionHandle);
        }

        int doSendFrame(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "-------------- doSendFrame called. Sending FrameSendCompleted -----------------");
            
            // Loop back. Each recevied frame is sent back.
            mListener.onFrameReceived(this, ConnectionHandle, Data, DataSize);
            
            // TODO: start the sequence number from some well known point and increment it each time
            // so it can be check later          
            
            mListener.onFrameSendCompleted(this, Data::ConnectionHandle, Data::BaseSequenceNumber, SendStatusOK);
            
            return Data::BaseSequenceNumber;
        }

        void doDisconnectDevice(const tDeviceHandle DeviceHandle)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "-------------- doDisconnectDevice -----------------");
            SDeviceInfo deviceInfo;
            deviceInfo.mDeviceHandle = Data::DeviceHandle;
            deviceInfo.mUniqueDeviceId = Data::UniqueDeviceId;
            deviceInfo.mUserFriendlyName = Data::UserFriendlyName;

            LOG4CPLUS_INFO_EXT(mLogger, "-------------- sending ApplicationDisconnected -----------------");
            mListener.onApplicationDisconnected(this, deviceInfo, Data::ConnectionHandle);
        }

    protected:
        IDeviceAdapterListener & mListener;
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
        , mFrameSequenceNumber(-1)
        , mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManagerTest")))
        {
            
        }
        MOCK_METHOD2(onApplicationConnected, void(const SDeviceInfo& ConnectedDevice, const tConnectionHandle Connection));
        MOCK_METHOD2(onApplicationDisconnected, void(const SDeviceInfo& DisconnectedDevice, const tConnectionHandle Connection));
        MOCK_METHOD1(onDeviceListUpdated, void(const tDeviceList& DeviceList));
        MOCK_METHOD3(onFrameReceived, void(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize));
        MOCK_METHOD3(onFrameSendCompleted, void(tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus));

        void doDeviceListUpdated(const tDeviceList& DeviceList)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "-------------- doDeviceListUpdated -----------------");
            mDeviceList = DeviceList;

            tDeviceList::const_iterator device;
            for(device = mDeviceList.begin(); device != mDeviceList.end(); ++device)
            {
                mTransportManager.connectDevice(device->mDeviceHandle);
            }
        }
        
        void doApplicationConnected(const SDeviceInfo& ConnectedDevice, const tConnectionHandle Connection)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "-------------- doApplicationConnected -----------------");
            
            // TODO: Start multiple threads and send data
            // mTransportManager.sendFrame(Connection, /* some data */, /* data size */);
        }

        void doFrameSendCompleted(tConnectionHandle ConnectionHandle, int FrameSequenceNumber, ESendStatus SendStatus)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "-------------- doFrameSendCompleted -----------------");
            
            // TODO: Check if the correct is sent (optional)
            
            /*
            tDeviceList::const_iterator device;
            for(device = mDeviceList.begin(); device != mDeviceList.end(); ++device)
            {
                mTransportManager.disconnectDevice(device->mDeviceHandle);
            }
            */
        }
        
        void doFrameReceived(tConnectionHandle ConnectionHandle, const uint8_t* Data, size_t DataSize)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "-------------- doFrameReceived -----------------");
            
            // TODO: Check if the correct frame is received
            
            // when all frames are received disconnectDevice
            /*
            tDeviceList::const_iterator device;
            for(device = mDeviceList.begin(); device != mDeviceList.end(); ++device)
            {
                mTransportManager.disconnectDevice(device->mDeviceHandle);
            }
            */
        }

    protected:
        ITransportManager & mTransportManager;
        tDeviceList mDeviceList;
        int mFrameSequenceNumber;
        Logger mLogger;
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
        , mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManagerTest")))
        {
        }

        virtual ~TestTransportManager(void )
        {

        }

        virtual void initializeDeviceAdapters()
        {
            // Preparing custom device adapter
            mpDeviceAdapter = new MockDeviceAdapter(*this, *this);

            EXPECT_CALL(*mpDeviceAdapter, run()).Times(1);
            EXPECT_CALL(*mpDeviceAdapter, scanForNewDevices())
                .Times(1)
                .WillOnce(Invoke(mpDeviceAdapter, &MockDeviceAdapter::doScanForNewDevices))
            ;

            EXPECT_CALL(*mpDeviceAdapter, connectDevice(Data::DeviceHandle))
                .Times(1)
                .WillOnce(Invoke(mpDeviceAdapter, &MockDeviceAdapter::doConnectDevice))
            ;

            EXPECT_CALL(*mpDeviceAdapter, sendFrame(Data::ConnectionHandle, _, _))
                .Times(Data::TotalNumberOfFrames)
                .WillRepeatedly(Invoke(mpDeviceAdapter, &MockDeviceAdapter::doSendFrame))
            ;

            EXPECT_CALL(*mpDeviceAdapter, disconnectDevice(Data::DeviceHandle))
                .Times(1)
                .WillOnce(Invoke(mpDeviceAdapter, &MockDeviceAdapter::doDisconnectDevice))
            ;

            addDeviceAdapter(mpDeviceAdapter);
            LOG4CPLUS_INFO_EXT(mLogger, "Device adapters initialized");
        }

    protected:
        MockDeviceAdapter *mpDeviceAdapter;
        Logger mLogger;
    };

    // ----------------------- TESTS ----------------------- //

    TEST(test_TestWithMultiThreadedClient, MultiThreadedClientTest)
    {
        Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("TransportManagerTest"));

        LOG4CPLUS_INFO_EXT(logger, "*************************** Starting test *****************************");
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

        EXPECT_CALL(tmClient, onFrameReceived(Data::ConnectionHandle, _, _))
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

        LOG4CPLUS_INFO_EXT(logger, "*************************** Calling RUN *****************************");
        pTm->run();

        sleep(1);

        LOG4CPLUS_INFO_EXT(logger, "*************************** Calling SCAN FOR DEVICES *****************************");
        pTm->scanForNewDevices();

        sleep(2);

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