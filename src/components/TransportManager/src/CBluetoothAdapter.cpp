#include <errno.h>
#include <iomanip>
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
#include "IHandleGenerator.hpp"
#include "CBluetoothAdapter.hpp"

NsAppLink::NsTransportManager::CBluetoothAdapter::SBluetoothDevice::SBluetoothDevice(const bdaddr_t & Address, const char * Name, const NsAppLink::NsTransportManager::CBluetoothAdapter::tRFCOMMChannelVector & AppLinkRFCOMMChannels):
SDevice(Name),
mAddress(Address),
mAppLinkRFCOMMChannels(AppLinkRFCOMMChannels)
{
    mUniqueDeviceId = getUniqueDeviceId(Address);
}

bool NsAppLink::NsTransportManager::CBluetoothAdapter::SBluetoothDevice::isSameAs(const NsAppLink::NsTransportManager::CDeviceAdapter::SDevice * OtherDevice) const
{
    bool result = false;

    if (true == SDevice::isSameAs(OtherDevice))
    {
        const SBluetoothDevice * otherBluetoothDevice = dynamic_cast<const SBluetoothDevice *>(OtherDevice);

        if (0 != otherBluetoothDevice)
        {
            result = (0 == memcmp(&mAddress, &otherBluetoothDevice->mAddress, sizeof(bdaddr_t)));
        }
    }

    return result;
}

NsAppLink::NsTransportManager::CBluetoothAdapter::SRFCOMMConnection::SRFCOMMConnection(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle, const uint8_t RFCOMMChannel):
SConnection(DeviceHandle),
mRFCOMMChannel(RFCOMMChannel)
{
}

bool NsAppLink::NsTransportManager::CBluetoothAdapter::SRFCOMMConnection::isSameAs(const NsAppLink::NsTransportManager::CDeviceAdapter::SConnection * OtherConnection) const
{
    bool result = false;

    if (true == SConnection::isSameAs(OtherConnection))
    {
        const SRFCOMMConnection * otherRFCOMMConnection = dynamic_cast<const SRFCOMMConnection *>(OtherConnection);

        if (0 != otherRFCOMMConnection)
        {
            result = (mRFCOMMChannel == otherRFCOMMConnection->mRFCOMMChannel);
        }
    }

    return result;
}

NsAppLink::NsTransportManager::CBluetoothAdapter::CBluetoothAdapter(NsAppLink::NsTransportManager::IDeviceAdapterListener & Listener, IHandleGenerator & HandleGenerator):
CDeviceAdapter("BluetoothAdapter", Listener, HandleGenerator),
mAppLinkServiceUUID()
{
    LOG4CPLUS_INFO_EXT(mLogger, "BluetoothAdapter constructed");

    uint8_t appLinkServiceUUIDData[] = {0x93, 0x6D, 0xA0, 0x1F, 0x9A, 0xBD, 0x4D, 0x9D, 0x80, 0xC7, 0x02, 0xAF, 0x85, 0xC8, 0x22, 0xA8};
    sdp_uuid128_create(&mAppLinkServiceUUID, appLinkServiceUUIDData);
}

NsAppLink::NsTransportManager::CBluetoothAdapter::~CBluetoothAdapter(void)
{
    LOG4CPLUS_INFO_EXT(mLogger, "BluetoothAdapter destructor");

    waitForThreadsTermination();
}

NsAppLink::NsTransportManager::EDeviceType NsAppLink::NsTransportManager::CBluetoothAdapter::getDeviceType(void) const
{
    return DeviceBluetooth;
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::createConnectionsListForDevice(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle, std::vector< NsAppLink::NsTransportManager::CDeviceAdapter::SConnection* >& ConnectionsList)
{
    bdaddr_t deviceAddress;
    bool isDeviceValid = false;

    ConnectionsList.clear();

    pthread_mutex_lock(&mDevicesMutex);

    tDeviceMap::iterator deviceIterator = mDevices.find(DeviceHandle);

    if (mDevices.end() != deviceIterator)
    {
        SBluetoothDevice * device = dynamic_cast<SBluetoothDevice*>(deviceIterator->second);

        if (0 != device)
        {
            memcpy(&deviceAddress, &device->mAddress, sizeof(bdaddr_t));
            isDeviceValid = true;
        }
        else
        {
            LOG4CPLUS_WARN_EXT(mLogger, "Device " << DeviceHandle << " is invalid");
        }
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Device handle " << DeviceHandle << " is invalid");
    }

    pthread_mutex_unlock(&mDevicesMutex);

    if (true == isDeviceValid)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Searching for AppLink service on device " << DeviceHandle);

        tRFCOMMChannelVector rfcommChannels;

        discoverAppLinkRFCOMMChannels(deviceAddress, rfcommChannels);

        if (false == rfcommChannels.empty())
        {
            for (tRFCOMMChannelVector::const_iterator channelIterator = rfcommChannels.begin(); channelIterator != rfcommChannels.end(); ++channelIterator)
            {
                ConnectionsList.push_back(new SRFCOMMConnection(DeviceHandle, *channelIterator));
            }
        }
        else
        {
            LOG4CPLUS_WARN_EXT(mLogger, "AppLink service was not discovered on device " << DeviceHandle);
        }
    }
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::mainThread(void)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Bluetooth adapter main thread initialized");

    const size_t maxDevices = 256u;
    inquiry_info * inquiryInfoList = new inquiry_info[maxDevices];

    while (false == mShutdownFlag)
    {
        tDeviceMap newDevices;
        tDeviceVector discoveredDevices;

        bool deviceScanRequested = waitForDeviceScanRequest(cConnectedDevicesServiceDiscoveryInterval);

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

                                discoveredDevices.push_back(new SBluetoothDevice(inquiryInfoList[i].bdaddr, deviceName, appLinkRFCOMMChannels));
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

            for (tDeviceMap::iterator deviceIterator = mDevices.begin(); deviceIterator != mDevices.end(); ++deviceIterator)
            {
                discoveredDevices.push_back(deviceIterator->second);
            }

            pthread_mutex_unlock(&mDevicesMutex);

            for (tDeviceVector::iterator discoveredDeviceIterator = discoveredDevices.begin(); discoveredDeviceIterator != discoveredDevices.end(); ++discoveredDeviceIterator)
            {
                SBluetoothDevice * device = dynamic_cast<SBluetoothDevice*>(*discoveredDeviceIterator);

                if (0 != device)
                {
                    if (true == device->mIsConnected)
                    {
                        discoverAppLinkRFCOMMChannels(device->mAddress, device->mAppLinkRFCOMMChannels);
                    }
                }
            }
        }

        for (tDeviceVector::iterator discoveredDeviceIterator = discoveredDevices.begin(); discoveredDeviceIterator != discoveredDevices.end(); ++discoveredDeviceIterator)
        {
            SDevice * discoveredDevice = *discoveredDeviceIterator;

            if (0 != discoveredDevice)
            {
                tDeviceHandle deviceHandle = InvalidDeviceHandle;

                pthread_mutex_lock(&mDevicesMutex);

                for (tDeviceMap::iterator deviceIterator = mDevices.begin(); deviceIterator != mDevices.end(); ++deviceIterator)
                {
                    SDevice * exisingDevice = deviceIterator->second;

                    if (true == discoveredDevice->isSameAs(exisingDevice))
                    {
                        deviceHandle = deviceIterator->first;
                        break;
                    }
                }

                pthread_mutex_unlock(&mDevicesMutex);

                if (InvalidDeviceHandle == deviceHandle)
                {
                    deviceHandle = mHandleGenerator.generateNewDeviceHandle();

                    LOG4CPLUS_INFO_EXT(mLogger, "Adding new device " << deviceHandle << " (\"" << discoveredDevice->mName << "\")");
                }

                newDevices[deviceHandle] = discoveredDevice;
            }
        }

        pthread_mutex_lock(&mDevicesMutex);

        if (true == deviceScanRequested)
        {
            for (tDeviceMap::iterator deviceIterator = mDevices.begin(); deviceIterator != mDevices.end(); ++deviceIterator)
            {
                delete deviceIterator->second;
            }
        }

        mDevices = newDevices;

        pthread_mutex_unlock(&mDevicesMutex);

        if (true == deviceScanRequested)
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Discovered " << newDevices.size() << " device" << ((1u == newDevices.size()) ? "" : "s") << " with AppLink service. New devices map:");

            for (tDeviceMap::iterator deviceIterator = newDevices.begin(); deviceIterator != newDevices.end(); ++deviceIterator)
            {
                SDevice * device = deviceIterator->second;

                if (0 != device)
                {
                    LOG4CPLUS_INFO_EXT(mLogger, std::setw(10) << deviceIterator->first << std::setw(0) << ": " << device->mUniqueDeviceId << ", " << device->mName.c_str());
                }
                else
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, std::setw(10) << deviceIterator->first << std::setw(0) << ": Device is null");
                }
            }

            mDeviceScanRequested = false;

            updateClientDeviceList();
        }

        std::vector<tConnectionHandle> connectionsToTerminate;
        std::set<std::pair<tDeviceHandle, uint8_t> > connectionsToEstablish;

        for (tDeviceMap::const_iterator newDeviceIterator = newDevices.begin(); newDeviceIterator != newDevices.end(); ++newDeviceIterator)
        {
            const SBluetoothDevice * device = dynamic_cast<const SBluetoothDevice*>(newDeviceIterator->second);

            if (0 != device)
            {
                if (true == device->mIsConnected)
                {
                    for (tRFCOMMChannelVector::const_iterator channelIterator = device->mAppLinkRFCOMMChannels.begin(); channelIterator != device->mAppLinkRFCOMMChannels.end(); ++channelIterator)
                    {
                        connectionsToEstablish.insert(std::make_pair(newDeviceIterator->first, *channelIterator));
                    }
                }
            }
        }

        pthread_mutex_lock(&mConnectionsMutex);

        for (tConnectionMap::const_iterator connectionIterator = mConnections.begin(); connectionIterator != mConnections.end(); ++connectionIterator)
        {
            const SRFCOMMConnection * connection = dynamic_cast<SRFCOMMConnection*>(connectionIterator->second);

            if (0 != connection)
            {
                tDeviceMap::const_iterator newDeviceIterator = newDevices.find(connection->mDeviceHandle);

                if (newDevices.end() == newDeviceIterator)
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "Connection " << connectionIterator->first << " must be terminated (device " << connection->mDeviceHandle << " is lost)");

                    connectionsToTerminate.push_back(connectionIterator->first);
                }
                else
                {
                    const SBluetoothDevice * newDevice = dynamic_cast<const SBluetoothDevice*>(newDeviceIterator->second);

                    if (0 == newDevice)
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << connectionIterator->first << " must be terminated because device is null");

                        connectionsToTerminate.push_back(connectionIterator->first);
                    }
                    else if (false == newDevice->mIsConnected)
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, "Connection " << connectionIterator->first << " must be terminated (device " << connection->mDeviceHandle << " is not connected)");

                        connectionsToTerminate.push_back(connectionIterator->first);
                    }
                    else
                    {
                        const tRFCOMMChannelVector & appLinkRFCOMMChannels = newDevice->mAppLinkRFCOMMChannels;

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
            }
            else
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << connectionIterator->first << " is null");
            }
        }

        pthread_mutex_unlock(&mConnectionsMutex);

        for (std::vector<tConnectionHandle>::const_iterator terminatingConnectionsIterator = connectionsToTerminate.begin(); terminatingConnectionsIterator != connectionsToTerminate.end(); ++terminatingConnectionsIterator)
        {
            stopConnection(*terminatingConnectionsIterator);
        }

        for (std::set<std::pair<tDeviceHandle, uint8_t> >::const_iterator newConnectionsIterator = connectionsToEstablish.begin(); newConnectionsIterator != connectionsToEstablish.end(); ++newConnectionsIterator)
        {
            startConnection(new SRFCOMMConnection(newConnectionsIterator->first, newConnectionsIterator->second));
        }
    }

    delete [] inquiryInfoList;

    LOG4CPLUS_INFO_EXT(mLogger, "Bluetooth adapter main thread finished");
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::connectionThread(const NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Connection thread started for connection " << ConnectionHandle);

    tDeviceHandle deviceHandle = InvalidDeviceHandle;
    struct sockaddr_rc remoteSocketAddress = {0};
    remoteSocketAddress.rc_family = AF_BLUETOOTH;

    pthread_mutex_lock(&mConnectionsMutex);

    SRFCOMMConnection * connection = 0;
    tConnectionMap::const_iterator connectionIterator = mConnections.find(ConnectionHandle);

    if (connectionIterator != mConnections.end())
    {
        connection = dynamic_cast<SRFCOMMConnection*>(connectionIterator->second);

        if (0 != connection)
        {
            deviceHandle = connection->mDeviceHandle;
            remoteSocketAddress.rc_channel = connection->mRFCOMMChannel;
        }
        else
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << ConnectionHandle << " is not valid");
        }
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << ConnectionHandle << " does not exist");
    }

    pthread_mutex_unlock(&mConnectionsMutex);

    if (0 != connection)
    {
        if (InvalidDeviceHandle != deviceHandle)
        {
            bool isDeviceValid = false;

            pthread_mutex_lock(&mDevicesMutex);

            tDeviceMap::const_iterator deviceIterator = mDevices.find(deviceHandle);

            if (deviceIterator != mDevices.end())
            {
                const SBluetoothDevice * device = dynamic_cast<const SBluetoothDevice*>(deviceIterator->second);

                if (0 != device)
                {
                    isDeviceValid = true;
                    memcpy(&remoteSocketAddress.rc_bdaddr, &device->mAddress, sizeof(bdaddr_t));
                }
                else
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "Device " << deviceHandle << " is not valid");
                }
            }
            else
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "Device " << deviceHandle << " does not exist");
            }

            pthread_mutex_unlock(&mDevicesMutex);

            if (true == isDeviceValid)
            {
                int rfcommSocket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

                if (-1 != rfcommSocket)
                {
                    if (0 == connect(rfcommSocket, (struct sockaddr *)&remoteSocketAddress, sizeof(remoteSocketAddress)))
                    {
                        connection->mConnectionSocket = rfcommSocket;
                        handleCommunication(ConnectionHandle);
                    }
                    else
                    {
                        LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to connect to remote device " << getUniqueDeviceId(remoteSocketAddress.rc_bdaddr) << " for connection " << ConnectionHandle);
                    }
                }
                else
                {
                    LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to create RFCOMM socket for connection " << ConnectionHandle);
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

    pthread_mutex_lock(&mConnectionsMutex);
    mConnections.erase(ConnectionHandle);
    pthread_mutex_unlock(&mConnectionsMutex);

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
