#include <errno.h>
#include <fcntl.h>
#include <iomanip>
#include <poll.h>
#include <set>
#include <unistd.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "IDeviceAdapterListener.hpp"
#include "CBluetoothAdapter.hpp"

#define LOG4CPLUS_ERROR_EXT_WITH_ERRNO(logger, message) LOG4CPLUS_ERROR_EXT(logger, message << ", error code " << errno << " (" << strerror(errno) << ")")

NsAppLink::NsTransportManager::CBluetoothAdapter::SFrame::SFrame(const uint8_t* Data, const size_t DataSize):
mData(0),
mDataSize(0)
{
    if ((0 != mData) &&
        (0u != mDataSize))
    {
        mData = new uint8_t[DataSize];

        if (0 != mData)
        {
            mDataSize = DataSize;
            memcpy(mData, Data, DataSize);
        }
    }
}

NsAppLink::NsTransportManager::CBluetoothAdapter::SFrame::~SFrame(void)
{
    if (0 != mData)
    {
        delete [] mData;
    }
}

NsAppLink::NsTransportManager::CBluetoothAdapter::SBluetoothDevice::SBluetoothDevice(void):
mAddress(),
mName(),
mUniqueDeviceId(),
mAppLinkRFCOMMChannels(),
mIsConnected(false)
{
}

NsAppLink::NsTransportManager::CBluetoothAdapter::SBluetoothDevice::SBluetoothDevice(const NsAppLink::NsTransportManager::CBluetoothAdapter::SBluetoothDevice & Other):
mAddress(Other.mAddress),
mName(Other.mName),
mUniqueDeviceId(Other.mUniqueDeviceId),
mAppLinkRFCOMMChannels(Other.mAppLinkRFCOMMChannels),
mIsConnected(Other.mIsConnected)
{
}

NsAppLink::NsTransportManager::CBluetoothAdapter::SBluetoothDevice::SBluetoothDevice(const bdaddr_t & Address, const char * Name, const NsAppLink::NsTransportManager::CBluetoothAdapter::tRFCOMMChannelVector & AppLinkRFCOMMChannels):
mAddress(Address),
mName(Name),
mUniqueDeviceId(getUniqueDeviceId(Address)),
mAppLinkRFCOMMChannels(AppLinkRFCOMMChannels),
mIsConnected(false)
{
}

NsAppLink::NsTransportManager::CBluetoothAdapter::SRFCOMMConnection::SRFCOMMConnection(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle, const uint8_t RFCOMMChannel):
mDeviceHandle(DeviceHandle),
mRFCOMMChannel(RFCOMMChannel),
mConnectionThread(),
mNotificationPipeFds(),
mNextFrameSequenceNumber(0),
mFramesToSend(),
mTerminateFlag(false)
{
    mNotificationPipeFds[0] = mNotificationPipeFds[1] = -1;
}

NsAppLink::NsTransportManager::CBluetoothAdapter::SRFCOMMConnection::~SRFCOMMConnection(void)
{
    for (tFrameMap::iterator frameIterator = mFramesToSend.begin(); frameIterator != mFramesToSend.end(); ++frameIterator)
    {
        delete frameIterator->second;
    }
    mFramesToSend.clear();
}

NsAppLink::NsTransportManager::CBluetoothAdapter::SRFCOMMConnectionParameters::SRFCOMMConnectionParameters(NsAppLink::NsTransportManager::CBluetoothAdapter & BluetoothAdapter, NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle):
mBluetoothAdapter(BluetoothAdapter),
mConnectionHandle(ConnectionHandle)
{
}

NsAppLink::NsTransportManager::CBluetoothAdapter::CBluetoothAdapter(NsAppLink::NsTransportManager::IDeviceAdapterListener & Listener, IHandleGenerator & HandleGenerator):
mListener(Listener),
mHandleGenerator(HandleGenerator),
mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("BluetoothAdapter"))),
mShutdownFlag(false),
mDeviceDiscoveryThread(),
mDeviceDiscoveryThreadStarted(false),
mDeviceScanRequested(false),
mDeviceScanRequestedMutex(),
mDeviceScanRequestedCond(),
mDevices(),
mDevicesMutex(),
mRFCOMMConnections(),
mRFCOMMConnectionsMutex(),
mAppLinkServiceUUID()
{
    LOG4CPLUS_INFO_EXT(mLogger, "BluetoothAdapter constructed");

    uint8_t appLinkServiceUUIDData[] = {0x93, 0x6D, 0xA0, 0x1F, 0x9A, 0xBD, 0x4D, 0x9D, 0x80, 0xC7, 0x02, 0xAF, 0x85, 0xC8, 0x22, 0xA8};
    sdp_uuid128_create(&mAppLinkServiceUUID, appLinkServiceUUIDData);

    pthread_cond_init(&mDeviceScanRequestedCond, 0);
    pthread_mutex_init(&mDeviceScanRequestedMutex, 0);
    pthread_mutex_init(&mDevicesMutex, 0);
    pthread_mutex_init(&mRFCOMMConnectionsMutex, 0);
}

NsAppLink::NsTransportManager::CBluetoothAdapter::~CBluetoothAdapter(void)
{
    LOG4CPLUS_INFO_EXT(mLogger, "BluetoothAdapter destructor");

    mShutdownFlag = true;

    if (true == mDeviceDiscoveryThreadStarted)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Waiting for discovery thread termination");
        pthread_join(mDeviceDiscoveryThread, 0);
        LOG4CPLUS_INFO_EXT(mLogger, "Discovery thread terminated");
    }

    std::vector<pthread_t> connectionThreads;

    pthread_mutex_lock(&mRFCOMMConnectionsMutex);

    for (tRFCOMMConnectionMap::iterator connectionIterator = mRFCOMMConnections.begin(); connectionIterator != mRFCOMMConnections.end(); ++connectionIterator)
    {
        SRFCOMMConnection * connection = connectionIterator->second;

        if (0 != connection)
        {
            connection->mTerminateFlag = true;
            if (-1 != connection->mNotificationPipeFds[1])
            {
                uint8_t c = 0;
                if (1 != write(connection->mNotificationPipeFds[1], &c, 1))
                {
                    LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to wake up connection thread for connection " << connectionIterator->first);
                }
            }
            connectionThreads.push_back(connection->mConnectionThread);
        }
        else
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << connectionIterator->first << " is null");
        }
    }

    pthread_mutex_unlock(&mRFCOMMConnectionsMutex);

    LOG4CPLUS_INFO_EXT(mLogger, "Waiting for connection threads termination");

    for (std::vector<pthread_t>::iterator connectionThreadIterator = connectionThreads.begin(); connectionThreadIterator != connectionThreads.end(); ++connectionThreadIterator)
    {
        pthread_join(*connectionThreadIterator, 0);
    }

    LOG4CPLUS_INFO_EXT(mLogger, "Connection threads terminated");

    pthread_mutex_destroy(&mRFCOMMConnectionsMutex);
    pthread_mutex_destroy(&mDevicesMutex);
    pthread_mutex_destroy(&mDeviceScanRequestedMutex);
    pthread_cond_destroy(&mDeviceScanRequestedCond);
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::run(void)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Initializing bluetooth adapter");

    int errorCode = pthread_create(&mDeviceDiscoveryThread, 0, &deviceDiscoveryThreadStartRoutine, this);

    if (0 == errorCode)
    {
        mDeviceDiscoveryThreadStarted = true;
        LOG4CPLUS_INFO_EXT(mLogger, "Device discovery thread started");
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Device discovery thread start failed, error code " << errorCode);
    }
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::scanForNewDevices(void)
{
    pthread_mutex_lock(&mDeviceScanRequestedMutex);

    if (false == mDeviceScanRequested)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Requesting device scan");

        mDeviceScanRequested = true;
        pthread_cond_signal(&mDeviceScanRequestedCond);
    }
    else
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Device scan is currently in progress");
    }

    pthread_mutex_unlock(&mDeviceScanRequestedMutex);
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::connectDevice(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle)
{
    bool isConnectionRequired = false;
    bdaddr_t deviceAddress;

    pthread_mutex_lock(&mDevicesMutex);

    tBluetoothDevicesMap::iterator deviceIterator = mDevices.find(DeviceHandle);

    if (mDevices.end() != deviceIterator)
    {
        SBluetoothDevice & device = deviceIterator->second;

        if (false == device.mIsConnected)
        {
            device.mIsConnected = true;
            isConnectionRequired = true;
            memcpy(&deviceAddress, &device.mAddress, sizeof(bdaddr_t));
        }
        else
        {
            LOG4CPLUS_WARN_EXT(mLogger, "Device " << DeviceHandle << " is already connected");
        }
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Device handle " << DeviceHandle << " is invalid");
    }

    pthread_mutex_unlock(&mDevicesMutex);

    if (true == isConnectionRequired)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Connecting device " << DeviceHandle);

        tRFCOMMChannelVector rfcommChannels;

        discoverAppLinkRFCOMMChannels(deviceAddress, rfcommChannels);

        if (false == rfcommChannels.empty())
        {
            for (tRFCOMMChannelVector::const_iterator channelIterator = rfcommChannels.begin(); channelIterator != rfcommChannels.end(); ++channelIterator)
            {
                startRFCOMMConnection(DeviceHandle, *channelIterator);
            }
        }
        else
        {
            LOG4CPLUS_WARN_EXT(mLogger, "AppLink service was not discovered on device " << DeviceHandle);
        }
    }
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::disconnectDevice(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle)
{
    bool isDisconnectionRequired = false;

    pthread_mutex_lock(&mDevicesMutex);

    tBluetoothDevicesMap::iterator deviceIterator = mDevices.find(DeviceHandle);

    if (mDevices.end() != deviceIterator)
    {
        SBluetoothDevice & device = deviceIterator->second;

        if (true == device.mIsConnected)
        {
            device.mIsConnected = false;
            isDisconnectionRequired = true;
        }
        else
        {
            LOG4CPLUS_WARN_EXT(mLogger, "Device " << DeviceHandle << " is already disconnected");
        }
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Device handle " << DeviceHandle << " is invalid");
    }

    pthread_mutex_unlock(&mDevicesMutex);

    if (true == isDisconnectionRequired)
    {
        std::vector<tConnectionHandle> connectionsToTerminate;

        pthread_mutex_lock(&mRFCOMMConnectionsMutex);

        for (tRFCOMMConnectionMap::const_iterator connectionIterator = mRFCOMMConnections.begin(); connectionIterator != mRFCOMMConnections.end(); ++connectionIterator)
        {
            SRFCOMMConnection * connection = connectionIterator->second;

            if (0 != connection)
            {
                if (connection->mDeviceHandle == DeviceHandle)
                {
                    connectionsToTerminate.push_back(connectionIterator->first);
                }
            }
            else
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << connectionIterator->first << " is null");
            }
        }

        pthread_mutex_unlock(&mRFCOMMConnectionsMutex);

        for (std::vector<tConnectionHandle>::const_iterator connectionHandleIterator = connectionsToTerminate.begin(); connectionHandleIterator != connectionsToTerminate.end(); ++connectionHandleIterator)
        {
            stopRFCOMMConnection(*connectionHandleIterator);
        }
    }
}

NsAppLink::NsTransportManager::EDeviceType NsAppLink::NsTransportManager::CBluetoothAdapter::getDeviceType(void) const
{
    return DeviceBluetooth;
}

int NsAppLink::NsTransportManager::CBluetoothAdapter::sendFrame(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize)
{
    int frameSequenceNumber = -1;

    if (0u == DataSize)
    {
        LOG4CPLUS_WARN_EXT(mLogger, "DataSize=0");
    }
    else if (0 == Data)
    {
        LOG4CPLUS_WARN_EXT(mLogger, "Data is null");
    }
    else
    {
        pthread_mutex_lock(&mRFCOMMConnectionsMutex);

        tRFCOMMConnectionMap::iterator connectionIterator = mRFCOMMConnections.find(ConnectionHandle);

        if (mRFCOMMConnections.end() == connectionIterator)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << ConnectionHandle << " does not exist");
        }
        else
        {
            SRFCOMMConnection * connection = connectionIterator->second;

            if (0 != connection)
            {
                frameSequenceNumber = connection->mNextFrameSequenceNumber++;

                if (connection->mFramesToSend.find(frameSequenceNumber) == connection->mFramesToSend.end())
                {
                    connection->mFramesToSend[frameSequenceNumber] = new SFrame(Data, DataSize);

                    if (-1 != connection->mNotificationPipeFds[1])
                    {
                        uint8_t c = 0;
                        if (1 != write(connection->mNotificationPipeFds[1], &c, 1))
                        {
                            LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to wake up connection thread for connection " << connectionIterator->first);
                        }
                    }
                }
                else
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "Sequence number " << frameSequenceNumber << " is already present in the map");

                    frameSequenceNumber = -1;
                }
            }
        }

        pthread_mutex_unlock(&mRFCOMMConnectionsMutex);
    }

    return frameSequenceNumber;
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::startRFCOMMConnection(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle, const uint8_t RFCOMMChannel)
{
    tConnectionHandle newConnectionHandle = mHandleGenerator.generateNewConnectionHandle();

    pthread_mutex_lock(&mRFCOMMConnectionsMutex);

    if (false == mShutdownFlag)
    {
        tRFCOMMConnectionMap::const_iterator connectionIterator;
        for (connectionIterator = mRFCOMMConnections.begin(); connectionIterator != mRFCOMMConnections.end(); ++connectionIterator)
        {
            const SRFCOMMConnection * connection = connectionIterator->second;

            if (0 != connection)
            {
                if ((connection->mDeviceHandle == DeviceHandle) &&
                    (connection->mRFCOMMChannel == RFCOMMChannel))
                {
                    LOG4CPLUS_WARN_EXT(mLogger, "Connection for device " << DeviceHandle << " channel " << static_cast<uint32_t>(RFCOMMChannel) << " is already opened (" << connectionIterator->first << ")");
                    break;
                }
            }
            else
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << connectionIterator->first << " is null");
            }
        }

        if (mRFCOMMConnections.end() == connectionIterator)
        {
            std::pair<tRFCOMMConnectionMap::iterator, bool> insertResult = mRFCOMMConnections.insert(std::make_pair(newConnectionHandle, new SRFCOMMConnection(DeviceHandle, RFCOMMChannel)));

            if (true == insertResult.second)
            {
                SRFCOMMConnection * newConnection = insertResult.first->second;

                if (0 != newConnection)
                {
                    SRFCOMMConnectionParameters * connectionParameters = new SRFCOMMConnectionParameters(*this, newConnectionHandle);

                    int errorCode = pthread_create(&newConnection->mConnectionThread, 0, &CBluetoothAdapter::connectionThreadStartRoutine, static_cast<void*>(connectionParameters));

                    if (0 == errorCode)
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, "Connection thread started for connection " << newConnectionHandle << " (device " << DeviceHandle << " channel " << static_cast<uint32_t>(RFCOMMChannel) << ")");
                    }
                    else
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, "Connection thread start failed for connection " << newConnectionHandle << " (device " << DeviceHandle << " channel " << static_cast<uint32_t>(RFCOMMChannel) << ")");

                        delete connectionParameters;
                        mRFCOMMConnections.erase(insertResult.first);
                        delete newConnection;
                    }
                }
                else
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "Failed to allocate connection " << newConnectionHandle);

                    mRFCOMMConnections.erase(newConnectionHandle);
                }
            }
            else
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "Connection handle " << newConnectionHandle << " already exists");
            }
        }
    }

    pthread_mutex_unlock(&mRFCOMMConnectionsMutex);
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::stopRFCOMMConnection(const NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle)
{
    pthread_mutex_lock(&mRFCOMMConnectionsMutex);

    tRFCOMMConnectionMap::iterator connectionIterator = mRFCOMMConnections.find(ConnectionHandle);

    if (mRFCOMMConnections.end() != connectionIterator)
    {
        SRFCOMMConnection * connection = connectionIterator->second;

        if (0 != connection)
        {
            if (false == connection->mTerminateFlag)
            {
                connection->mTerminateFlag = true;
                if (-1 != connection->mNotificationPipeFds[1])
                {
                    uint8_t c = 0;
                    if (1 != write(connection->mNotificationPipeFds[1], &c, 1))
                    {
                        LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to wake up connection thread for connection " << connectionIterator->first);
                    }
                }

                LOG4CPLUS_INFO_EXT(mLogger, "Connection " << ConnectionHandle << "(device " << connection->mDeviceHandle << " channel " << static_cast<uint32_t>(connection->mRFCOMMChannel) << ") has been marked for termination");
            }
            else
            {
                LOG4CPLUS_WARN_EXT(mLogger, "Connection " << ConnectionHandle << " is already terminating");
            }
        }
        else
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << ConnectionHandle << " is null");
        }
    }
    else
    {
        LOG4CPLUS_WARN_EXT(mLogger, "Connection " << ConnectionHandle << " does not exist");
    }

    pthread_mutex_unlock(&mRFCOMMConnectionsMutex);
}

void * NsAppLink::NsTransportManager::CBluetoothAdapter::deviceDiscoveryThreadStartRoutine(void * Data)
{
    if (0 != Data)
    {
        static_cast<CBluetoothAdapter*>(Data)->deviceDiscoveryThread();
    }

    return 0;
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::deviceDiscoveryThread(void)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Device discovery thread initialized");

    const size_t maxDevices = 256u;
    inquiry_info * inquiryInfoList = new inquiry_info[maxDevices];

    while (false == mShutdownFlag)
    {
        tBluetoothDevicesMap newDevices;
        bool deviceScanRequested = false;
        std::vector<SBluetoothDevice> discoveredDevices;

        pthread_mutex_lock(&mDeviceScanRequestedMutex);

        if (false == mDeviceScanRequested)
        {
            timespec timeout;

            if (0 == clock_gettime(CLOCK_REALTIME, &timeout))
            {
                timeout.tv_sec += cConnectedDevicesServiceDiscoveryInterval;

                while (0 == pthread_cond_timedwait(&mDeviceScanRequestedCond, &mDeviceScanRequestedMutex, &timeout))
                {
                    if (true == mDeviceScanRequested)
                    {
                        break;
                    }
                }
            }
            else
            {
                LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "clock_gettime failed");

                sleep(cConnectedDevicesServiceDiscoveryInterval);
            }
        }

        deviceScanRequested = mDeviceScanRequested;

        pthread_mutex_unlock(&mDeviceScanRequestedMutex);

        if (true == deviceScanRequested)
        {
            int deviceID = hci_get_route(0);

            if (deviceID >= 0)
            {
                int deviceHandle = hci_open_dev(deviceID);

                if (deviceHandle >= 0)
                {
                    const uint8_t inquiryTime = 8u; // Time unit is 1.28 seconds

                    LOG4CPLUS_INFO_EXT(mLogger, "Starting hci_inquiry on device " << deviceID);

                    int numberOfDevices = hci_inquiry(deviceID, inquiryTime, maxDevices, 0, &inquiryInfoList, IREQ_CACHE_FLUSH);

                    if (numberOfDevices >= 0)
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, "hci_inquiry: found " << numberOfDevices << " devices");

                        for (int i = 0; i < numberOfDevices; ++i)
                        {
                            tRFCOMMChannelVector appLinkRFCOMMChannels;
                            discoverAppLinkRFCOMMChannels(inquiryInfoList[i].bdaddr, appLinkRFCOMMChannels);

                            if (false == appLinkRFCOMMChannels.empty())
                            {
                                char deviceName[256];

                                if (0 != hci_read_remote_name(deviceHandle, &inquiryInfoList[i].bdaddr, sizeof(deviceName) / sizeof(deviceName[0]), deviceName, 0))
                                {
                                    LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "hci_read_remote_name failed");
                                    strncpy(deviceName, getUniqueDeviceId(inquiryInfoList[i].bdaddr).c_str(), sizeof(deviceName) / sizeof(deviceName[0]));
                                }

                                discoveredDevices.push_back(SBluetoothDevice(inquiryInfoList[i].bdaddr, deviceName, appLinkRFCOMMChannels));
                            }
                        }
                    }
                    else
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, "hci_inquiry failed");
                    }

                    close(deviceHandle);
                }
            }
        }
        else
        {
            pthread_mutex_lock(&mDevicesMutex);

            for (tBluetoothDevicesMap::iterator deviceIterator = mDevices.begin(); deviceIterator != mDevices.end(); ++deviceIterator)
            {
                discoveredDevices.push_back(deviceIterator->second);
            }

            pthread_mutex_unlock(&mDevicesMutex);

            for (std::vector<SBluetoothDevice>::iterator discoveredDeviceIterator = discoveredDevices.begin(); discoveredDeviceIterator != discoveredDevices.end(); ++discoveredDeviceIterator)
            {
                if (true == discoveredDeviceIterator->mIsConnected)
                {
                    discoverAppLinkRFCOMMChannels(discoveredDeviceIterator->mAddress, discoveredDeviceIterator->mAppLinkRFCOMMChannels);
                }
            }
        }

        for (std::vector<SBluetoothDevice>::const_iterator discoveredDeviceIterator = discoveredDevices.begin(); discoveredDeviceIterator != discoveredDevices.end(); ++discoveredDeviceIterator)
        {
            const SBluetoothDevice & discoveredDevice = *discoveredDeviceIterator;
            tDeviceHandle deviceHandle = InvalidDeviceHandle;

            pthread_mutex_lock(&mDevicesMutex);

            for (tBluetoothDevicesMap::iterator deviceIterator = mDevices.begin(); deviceIterator != mDevices.end(); ++deviceIterator)
            {
                SBluetoothDevice & exisingDevice = deviceIterator->second;

                if (0 == memcmp(&exisingDevice.mAddress, &discoveredDevice.mAddress, sizeof(bdaddr_t)))
                {
                    deviceHandle = deviceIterator->first;
                }
            }

            pthread_mutex_unlock(&mDevicesMutex);

            if (InvalidDeviceHandle == deviceHandle)
            {
                deviceHandle = mHandleGenerator.generateNewDeviceHandle();

                LOG4CPLUS_INFO_EXT(mLogger, "Adding new device " << deviceHandle << " (\"" << discoveredDevice.mName << "\")");
            }

            newDevices[deviceHandle] = discoveredDevice;
        }

        pthread_mutex_lock(&mDevicesMutex);
        mDevices = newDevices;
        pthread_mutex_unlock(&mDevicesMutex);

        if (true == deviceScanRequested)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Discovered " << newDevices.size() << " device" << ((1u == newDevices.size()) ? "" : "s") << " with AppLink service. New devices map:");

            for (tBluetoothDevicesMap::const_iterator deviceIterator = newDevices.begin(); deviceIterator != newDevices.end(); ++deviceIterator)
            {
                LOG4CPLUS_INFO_EXT(mLogger, std::setw(10) << deviceIterator->first << std::setw(0) << ": " << deviceIterator->second.mUniqueDeviceId << ", " << deviceIterator->second.mName.c_str());
            }

            LOG4CPLUS_INFO_EXT(mLogger, "Updating client device list");

            tInternalDeviceList clientDeviceList;

            for (tBluetoothDevicesMap::const_iterator di = newDevices.begin(); di != newDevices.end(); ++di)
            {
                clientDeviceList.push_back(SInternalDeviceInfo(di->first, di->second.mName, di->second.mUniqueDeviceId));
            }

            mDeviceScanRequested = false;

            mListener.onDeviceListUpdated(this, clientDeviceList);
        }

        std::vector<tConnectionHandle> connectionsToTerminate;
        std::set<std::pair<tDeviceHandle, uint8_t> > connectionsToEstablish;

        for (tBluetoothDevicesMap::const_iterator newDeviceIterator = newDevices.begin(); newDeviceIterator != newDevices.end(); ++newDeviceIterator)
        {
            const SBluetoothDevice & device = newDeviceIterator->second;

            if (true == device.mIsConnected)
            {
                for (tRFCOMMChannelVector::const_iterator channelIterator = device.mAppLinkRFCOMMChannels.begin(); channelIterator != device.mAppLinkRFCOMMChannels.end(); ++channelIterator)
                {
                    connectionsToEstablish.insert(std::make_pair(newDeviceIterator->first, *channelIterator));
                }
            }
        }

        pthread_mutex_lock(&mRFCOMMConnectionsMutex);

        for (tRFCOMMConnectionMap::const_iterator connectionIterator = mRFCOMMConnections.begin(); connectionIterator != mRFCOMMConnections.end(); ++connectionIterator)
        {
            const SRFCOMMConnection * connection = connectionIterator->second;

            if (0 != connection)
            {
                tBluetoothDevicesMap::const_iterator newDeviceIterator = newDevices.find(connection->mDeviceHandle);

                if (newDevices.end() == newDeviceIterator)
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "Connection " << connectionIterator->first << " must be terminated (device " << connection->mDeviceHandle << " is lost)");

                    connectionsToTerminate.push_back(connectionIterator->first);
                }
                else if (false == newDeviceIterator->second.mIsConnected)
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "Connection " << connectionIterator->first << " must be terminated (device " << connection->mDeviceHandle << " is not connected)");

                    connectionsToTerminate.push_back(connectionIterator->first);
                }
                else
                {
                    const tRFCOMMChannelVector & appLinkRFCOMMChannels = newDeviceIterator->second.mAppLinkRFCOMMChannels;

                    if (appLinkRFCOMMChannels.end() == std::find(appLinkRFCOMMChannels.begin(), appLinkRFCOMMChannels.end(), connection->mRFCOMMChannel))
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, "Connection " << connectionIterator->first << " must be terminated (no AppLink service found on channel " << static_cast<uint32_t>(connection->mRFCOMMChannel) << ")");

                        connectionsToTerminate.push_back(connectionIterator->first);
                    }
                    else
                    {
                        const std::pair<tDeviceHandle, uint8_t> newConnection(connection->mDeviceHandle, connection->mRFCOMMChannel);

                        if (connectionsToEstablish.find(newConnection) != connectionsToEstablish.end())
                        {
                            connectionsToEstablish.erase(newConnection);
                        }
                    }
                }
            }
            else
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << connectionIterator->first << " is null");
            }
        }

        pthread_mutex_unlock(&mRFCOMMConnectionsMutex);

        for (std::vector<tConnectionHandle>::const_iterator terminatingConnectionsIterator = connectionsToTerminate.begin(); terminatingConnectionsIterator != connectionsToTerminate.end(); ++terminatingConnectionsIterator)
        {
            stopRFCOMMConnection(*terminatingConnectionsIterator);
        }

        for (std::set<std::pair<tDeviceHandle, uint8_t> >::const_iterator newConnectionsIterator = connectionsToEstablish.begin(); newConnectionsIterator != connectionsToEstablish.end(); ++newConnectionsIterator)
        {
            startRFCOMMConnection(newConnectionsIterator->first, newConnectionsIterator->second);
        }
    }

    delete [] inquiryInfoList;

    LOG4CPLUS_INFO_EXT(mLogger, "Device discovery thread finished");
}

void * NsAppLink::NsTransportManager::CBluetoothAdapter::connectionThreadStartRoutine(void * Data)
{
    SRFCOMMConnectionParameters * connectionParameters = static_cast<SRFCOMMConnectionParameters*>(Data);

    if (0 != connectionParameters)
    {
        CBluetoothAdapter & bluetoothAdapter(connectionParameters->mBluetoothAdapter);
        tConnectionHandle connectionHandle(connectionParameters->mConnectionHandle);

        delete connectionParameters;
        connectionParameters = 0;

        bluetoothAdapter.connectionThread(connectionHandle);
    }

    return 0;
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::connectionThread(const NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Connection thread started for connection " << ConnectionHandle);

    struct sockaddr_rc remoteSocketAddress = {0};
    remoteSocketAddress.rc_family = AF_BLUETOOTH;

    tDeviceHandle deviceHandle = InvalidDeviceHandle;

    pthread_mutex_lock(&mRFCOMMConnectionsMutex);

    SRFCOMMConnection * connection = 0;
    tRFCOMMConnectionMap::const_iterator connectionIterator = mRFCOMMConnections.find(ConnectionHandle);

    if (connectionIterator != mRFCOMMConnections.end())
    {
        connection = connectionIterator->second;

        if (0 != connection)
        {
            deviceHandle = connection->mDeviceHandle;
            remoteSocketAddress.rc_channel = connection->mRFCOMMChannel;
        }
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << ConnectionHandle << " does not exist");
    }

    pthread_mutex_unlock(&mRFCOMMConnectionsMutex);

    if (0 != connection)
    {
        if (InvalidDeviceHandle != deviceHandle)
        {
            bool isDeviceValid = false;
            SDeviceInfo clientDeviceInfo;

            pthread_mutex_lock(&mDevicesMutex);

            tBluetoothDevicesMap::const_iterator deviceIterator = mDevices.find(deviceHandle);

            if (deviceIterator != mDevices.end())
            {
                isDeviceValid = true;
                memcpy(&remoteSocketAddress.rc_bdaddr, &deviceIterator->second.mAddress, sizeof(bdaddr_t));

                clientDeviceInfo.mDeviceHandle = deviceHandle;
                clientDeviceInfo.mDeviceType = getDeviceType();
                clientDeviceInfo.mUserFriendlyName = deviceIterator->second.mName;
                clientDeviceInfo.mUniqueDeviceId = deviceIterator->second.mUniqueDeviceId;
            }
            else
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "Device " << deviceHandle << " does not exist");
            }

            pthread_mutex_unlock(&mDevicesMutex);

            if (true == isDeviceValid)
            {
                pthread_mutex_lock(&mRFCOMMConnectionsMutex);
                bool isPipeCreated = (0 == pipe(connection->mNotificationPipeFds));
                int notificationPipeReadFd = connection->mNotificationPipeFds[0];
                pthread_mutex_unlock(&mRFCOMMConnectionsMutex);

                if (true == isPipeCreated)
                {
                    isPipeCreated = (0 == fcntl(notificationPipeReadFd, F_SETFL, fcntl(notificationPipeReadFd, F_GETFL) | O_NONBLOCK));

                    if (false == isPipeCreated)
                    {
                        LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to set O_NONBLOCK for notification pipe for connection " << ConnectionHandle);
                    }
                }

                if (true == isPipeCreated)
                {
                    int rfcommSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

                    if (-1 != rfcommSocket)
                    {
                        if (0 == connect(rfcommSocket, (struct sockaddr *)&remoteSocketAddress, sizeof(remoteSocketAddress)))
                        {
                            LOG4CPLUS_INFO_EXT(mLogger, "Connection " << ConnectionHandle << " to remote device " << clientDeviceInfo.mUniqueDeviceId << " established");

                            mListener.onApplicationConnected(this, clientDeviceInfo, ConnectionHandle);

                            pollfd pollFds[2];
                            pollFds[0].fd = rfcommSocket;
                            pollFds[0].events = POLLIN | POLLPRI;
                            pollFds[1].fd = connection->mNotificationPipeFds[0];
                            pollFds[1].events = POLLIN | POLLPRI;

                            while (false == connection->mTerminateFlag)
                            {
                                if (-1 != poll(pollFds, sizeof(pollFds) / sizeof(pollFds[0]), -1))
                                {
                                    if (0 != (pollFds[0].revents & (POLLERR | POLLHUP | POLLNVAL)))
                                    {
                                        LOG4CPLUS_INFO_EXT(mLogger, "Connection " << ConnectionHandle << " terminated");

                                        connection->mTerminateFlag = true;
                                    }
                                    else if (0 != (pollFds[1].revents & (POLLERR | POLLHUP | POLLNVAL)))
                                    {
                                        LOG4CPLUS_ERROR_EXT(mLogger, "Notification pipe for connection " << ConnectionHandle << " terminated");

                                        connection->mTerminateFlag = true;
                                    }
                                    else
                                    {
                                        uint8_t buffer[4096];
                                        ssize_t bytesRead = -1;

                                        if (0 != pollFds[0].revents)
                                        {
                                            do
                                            {
                                                bytesRead = recv(rfcommSocket, buffer, sizeof(buffer), MSG_DONTWAIT);

                                                if ((bytesRead < 0) &&
                                                    ((EAGAIN == errno) ||
                                                     (EWOULDBLOCK == errno)))
                                                {
                                                    bytesRead = 0;
                                                }

                                                if (bytesRead >= 0)
                                                {
                                                    if (bytesRead > 0)
                                                    {
                                                        LOG4CPLUS_INFO_EXT(mLogger, "Received " << bytesRead << " bytes for connection " << ConnectionHandle);

                                                        mListener.onFrameReceived(this, ConnectionHandle, buffer, static_cast<size_t>(bytesRead));
                                                    }
                                                }
                                                else
                                                {
                                                    LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "recv() failed for connection " << ConnectionHandle);

                                                    connection->mTerminateFlag = true;
                                                }
                                            } while (bytesRead > 0);
                                        }

                                        if ((false == connection->mTerminateFlag) &&
                                            (0 != pollFds[1].revents))
                                        {
                                            do
                                            {
                                                bytesRead = read(notificationPipeReadFd, buffer, sizeof(buffer));
                                            } while (bytesRead > 0);

                                            if ((bytesRead < 0) &&
                                                (EAGAIN != errno))
                                            {
                                                LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to clear notification pipe for connection " << ConnectionHandle);

                                                connection->mTerminateFlag = true;
                                            }

                                            tFrameMap framesToSend;

                                            pthread_mutex_lock(&mRFCOMMConnectionsMutex);
                                            framesToSend.swap(connection->mFramesToSend);
                                            pthread_mutex_unlock(&mRFCOMMConnectionsMutex);

                                            for (tFrameMap::iterator frameIterator = framesToSend.begin(); frameIterator != framesToSend.end(); ++frameIterator)
                                            {
                                                int frameSequenceNumber = frameIterator->first;
                                                SFrame * frame = frameIterator->second;
                                                ESendStatus frameSendStatus = SendStatusUnknownError;

                                                if (0 != frame)
                                                {
                                                    if ((0 != frame->mData) &&
                                                        (0u != frame->mDataSize))
                                                    {
                                                        ssize_t bytesSent = send(rfcommSocket, frame->mData, frame->mDataSize, 0);

                                                        if (static_cast<size_t>(bytesSent) == frame->mDataSize)
                                                        {
                                                            frameSendStatus = SendStatusOK;
                                                        }
                                                        else
                                                        {
                                                            if (bytesSent >= 0)
                                                            {
                                                                LOG4CPLUS_ERROR_EXT(mLogger, "Sent " << bytesSent << " bytes while " << frame->mDataSize << " had been requested for connection " << ConnectionHandle << " frame " << frameSequenceNumber);
                                                            }
                                                            else
                                                            {
                                                                LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Send failed for connection " << ConnectionHandle << " frame " << frameSequenceNumber);
                                                            }

                                                            frameSendStatus = SendStatusFailed;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        LOG4CPLUS_ERROR_EXT(mLogger, "Frame data is invalid for connection " << ConnectionHandle << " frame " << frameSequenceNumber);

                                                        frameSendStatus = SendStatusInternalError;
                                                    }

                                                    delete frame;
                                                }
                                                else
                                                {
                                                    LOG4CPLUS_ERROR_EXT(mLogger, "Frame data is null for connection " << ConnectionHandle << " frame " << frameSequenceNumber);

                                                    frameSendStatus = SendStatusInternalError;
                                                }

                                                mListener.onFrameSendCompleted(this, ConnectionHandle, frameSequenceNumber, frameSendStatus);
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "poll() failed for connection " << ConnectionHandle);

                                    connection->mTerminateFlag = true;
                                }
                            }

                            mListener.onApplicationDisconnected(this, clientDeviceInfo, ConnectionHandle);
                        }
                        else
                        {
                            LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to connect to remote device " << clientDeviceInfo.mUniqueDeviceId << " for connection " << ConnectionHandle);
                        }

                        if (0 != close(rfcommSocket))
                        {
                            LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to close RFCOMM socket for connection " << ConnectionHandle);
                        }
                    }
                    else
                    {
                        LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to create RFCOMM socket for connection " << ConnectionHandle);
                    }

                    pthread_mutex_lock(&mRFCOMMConnectionsMutex);

                    close(connection->mNotificationPipeFds[0]);
                    close(connection->mNotificationPipeFds[1]);

                    connection->mNotificationPipeFds[0] = connection->mNotificationPipeFds[1] = -1;

                    pthread_mutex_unlock(&mRFCOMMConnectionsMutex);
                }
                else
                {
                    LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to create notification pipe for connection " << ConnectionHandle);
                }
            }
        }
        else
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Device handle for connection " << ConnectionHandle << " is invalid");
        }
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << ConnectionHandle << " is null");
    }

    LOG4CPLUS_INFO_EXT(mLogger, "Removing connection " << ConnectionHandle << " from connection map");

    pthread_mutex_lock(&mRFCOMMConnectionsMutex);
    mRFCOMMConnections.erase(ConnectionHandle);
    pthread_mutex_unlock(&mRFCOMMConnectionsMutex);

    delete connection;

    LOG4CPLUS_INFO_EXT(mLogger, "Connection thread finished for connection " << ConnectionHandle);
}

std::string NsAppLink::NsTransportManager::CBluetoothAdapter::getUniqueDeviceId(const bdaddr_t & DeviceAddress)
{
    char deviceAddressString[32];

    ba2str(&DeviceAddress, deviceAddressString);

    return std::string("BT-") + deviceAddressString;
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::discoverAppLinkRFCOMMChannels(const bdaddr_t & DeviceAddress, NsAppLink::NsTransportManager::CBluetoothAdapter::tRFCOMMChannelVector & AppLinkRFCOMMChannels)
{
    AppLinkRFCOMMChannels.clear();

    static bdaddr_t anyAddress = {{0, 0, 0, 0, 0, 0}};

    sdp_session_t * sdpSession = sdp_connect(&anyAddress, &DeviceAddress, SDP_RETRY_IF_BUSY);

    if (0 != sdpSession)
    {
        sdp_list_t * searchList = sdp_list_append(0, &mAppLinkServiceUUID);
        uint32_t range = 0x0000ffff;
        sdp_list_t * attrList = sdp_list_append(0, &range);
        sdp_list_t * responseList = 0;

        if (0 == sdp_service_search_attr_req(sdpSession, searchList, SDP_ATTR_REQ_RANGE, attrList, &responseList))
        {
            for (sdp_list_t * r = responseList; 0 != r; r = r->next)
            {
                sdp_record_t * sdpRecord = static_cast<sdp_record_t*>(r->data);
                sdp_list_t * protoList = 0;

                if (0 == sdp_get_access_protos(sdpRecord, &protoList))
                {
                    for (sdp_list_t * p = protoList; 0 != p; p = p->next)
                    {
                        sdp_list_t * pdsList = static_cast<sdp_list_t*>(p->data);

                        for (sdp_list_t * pds = pdsList; 0 != pds; pds = pds->next)
                        {
                            sdp_data_t * sdpData = static_cast<sdp_data_t*>(pds->data);
                            int proto = 0;

                            for (sdp_data_t * d = sdpData; 0 != d; d = d->next)
                            {
                                switch (d->dtd)
                                {
                                    case SDP_UUID16:
                                    case SDP_UUID32:
                                    case SDP_UUID128:
                                        proto = sdp_uuid_to_proto(&d->val.uuid);
                                        break;

                                    case SDP_UINT8:
                                        if (RFCOMM_UUID == proto)
                                        {
                                            AppLinkRFCOMMChannels.push_back(d->val.uint8);
                                        }
                                        break;
                                }
                            }
                        }

                        sdp_list_free(pdsList, 0);
                    }

                    sdp_list_free(protoList, 0);
                }
            }
        }

        sdp_list_free(searchList, 0);
        sdp_list_free(attrList, 0);
        sdp_list_free(responseList, 0);
        sdp_close(sdpSession);
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Service discovery failed for " << getUniqueDeviceId(DeviceAddress));
    }

    if (false == AppLinkRFCOMMChannels.empty())
    {
        std::stringstream rfcommChannelsString;

        for (tRFCOMMChannelVector::const_iterator channelIterator = AppLinkRFCOMMChannels.begin(); channelIterator != AppLinkRFCOMMChannels.end(); ++channelIterator)
        {
            if (channelIterator != AppLinkRFCOMMChannels.begin())
            {
                rfcommChannelsString << ", ";
            }

            rfcommChannelsString << static_cast<uint32_t>(*channelIterator);
        }

        LOG4CPLUS_INFO_EXT(mLogger, "AppLink service was discovered on device " << getUniqueDeviceId(DeviceAddress) << " at channel(s): " << rfcommChannelsString.str().c_str());
    }
    else
    {
        LOG4CPLUS_INFO_EXT(mLogger, "AppLink service was not discovered on device " << getUniqueDeviceId(DeviceAddress));
    }
}
