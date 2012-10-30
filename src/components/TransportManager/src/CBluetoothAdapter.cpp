#include <errno.h>
#include <iomanip>
#include <set>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

#include "IDeviceAdapterListener.hpp"
#include "CBluetoothAdapter.hpp"

NsAppLink::NsTransportManager::CBluetoothAdapter::SBluetoothDevice::SBluetoothDevice(void):
mAddress(),
mName(),
mAppLinkRFCOMMChannels()
{
}

NsAppLink::NsTransportManager::CBluetoothAdapter::SBluetoothDevice::SBluetoothDevice(const NsAppLink::NsTransportManager::CBluetoothAdapter::SBluetoothDevice & Other):
mAddress(Other.mAddress),
mName(Other.mName),
mAppLinkRFCOMMChannels(Other.mAppLinkRFCOMMChannels)
{
}

NsAppLink::NsTransportManager::CBluetoothAdapter::SBluetoothDevice::SBluetoothDevice(const bdaddr_t & Address, const char * Name, const std::vector<uint8_t> & AppLinkRFCOMMChannels):
mAddress(Address),
mName(Name),
mAppLinkRFCOMMChannels(AppLinkRFCOMMChannels)
{
}

NsAppLink::NsTransportManager::CBluetoothAdapter::SRFCOMMConnection::SRFCOMMConnection(void):
mDeviceHandle(InvalidDeviceHandle),
mDeviceAddress(),
mRFCOMMChannel(0u),
mConnectionThread()
{
}

NsAppLink::NsTransportManager::CBluetoothAdapter::SRFCOMMConnection::SRFCOMMConnection(const NsAppLink::NsTransportManager::CBluetoothAdapter::SRFCOMMConnection & Other):
mDeviceHandle(Other.mDeviceHandle),
mDeviceAddress(Other.mDeviceAddress),
mRFCOMMChannel(Other.mRFCOMMChannel),
mConnectionThread(Other.mConnectionThread)
{
}

NsAppLink::NsTransportManager::CBluetoothAdapter::SRFCOMMConnection::SRFCOMMConnection(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle, const bdaddr_t & DeviceAddress, const uint8_t RFCOMMChannel):
mDeviceHandle(DeviceHandle),
mDeviceAddress(DeviceAddress),
mRFCOMMChannel(RFCOMMChannel),
mConnectionThread()
{
}

NsAppLink::NsTransportManager::CBluetoothAdapter::SRFCOMMConnectionParameters::SRFCOMMConnectionParameters(NsAppLink::NsTransportManager::CBluetoothAdapter & BluetoothAdapter, NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle, uint8_t RFCOMMChannel):
mBluetoothAdapter(BluetoothAdapter),
mDeviceHandle(DeviceHandle),
mRFCOMMChannel(RFCOMMChannel)
{
}

NsAppLink::NsTransportManager::CBluetoothAdapter::CBluetoothAdapter(NsAppLink::NsTransportManager::IDeviceAdapterListener & Listener, IHandleGenerator & HandleGenerator):
mListener(Listener),
mHandleGenerator(HandleGenerator),
mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("BluetoothAdapter"))),
mShutdownFlag(false),
mDeviceDiscoveryThread(),
mDeviceDiscoveryThreadStarted(false),
mDevices(),
mDevicesMutex()
{
    LOG4CPLUS_INFO_EXT(mLogger, "BluetoothAdapter constructed");

    pthread_mutex_init(&mDevicesMutex, 0);
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

    pthread_mutex_destroy(&mDevicesMutex);
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

void NsAppLink::NsTransportManager::CBluetoothAdapter::getDeviceList(NsAppLink::NsTransportManager::tInternalDeviceList & DeviceList) const
{
    pthread_mutex_lock(&mDevicesMutex);

    for (tBluetoothDevicesMap::const_iterator di = mDevices.begin(); di != mDevices.end(); ++di)
    {
        DeviceList.push_back(SInternalDeviceInfo(di->first, di->second.mName));
    }

    pthread_mutex_unlock(&mDevicesMutex);
}

NsAppLink::NsTransportManager::EDeviceType NsAppLink::NsTransportManager::CBluetoothAdapter::getDeviceType(void) const
{
    return DeviceBluetooth;
}

int NsAppLink::NsTransportManager::CBluetoothAdapter::sendFrame(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const NsAppLink::NsTransportManager::Blob& Data)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");

    return -1;
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::startRFCOMMConnection(const NsAppLink::NsTransportManager::tDeviceHandle& DeviceHandle, const uint8_t RFCOMMChannel)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::stopRFCOMMConnection(const NsAppLink::NsTransportManager::tConnectionHandle& ConnectionHandle)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");
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
    const uint8_t inquiryTime = 8u; // Time unit is 1.28 seconds
    inquiry_info * inquiryInfoList = new inquiry_info[maxDevices];
    uint8_t appLinkServiceUUIDData[] = {0x93, 0x6D, 0xA0, 0x1F, 0x9A, 0xBD, 0x4D, 0x9D, 0x80, 0xC7, 0x02, 0xAF, 0x85, 0xC8, 0x22, 0xA8};
    uuid_t appLinkServiceUUID;
    sdp_uuid128_create(&appLinkServiceUUID, appLinkServiceUUIDData);

    while (false == mShutdownFlag)
    {
        int deviceID = hci_get_route(0);

        if (deviceID >= 0)
        {
            bdaddr_t adapterAddress = {{0, 0, 0, 0, 0, 0}};

            if (0 != hci_devba(deviceID, &adapterAddress))
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "Address of bluetooth adapter " << deviceID << " can't be determined, error code " << errno << " (" << strerror(errno) << ")");
            }
            else
            {
                int deviceHandle = hci_open_dev(deviceID);

                if (deviceHandle >= 0)
                {
                    char adapterAddressString[16];
                    ba2str(&adapterAddress, adapterAddressString);
                    LOG4CPLUS_INFO_EXT(mLogger, "Starting hci_inquiry on device " << deviceID << " (" << adapterAddressString << ")");

                    int numberOfDevices = hci_inquiry(deviceID, inquiryTime, maxDevices, 0, &inquiryInfoList, IREQ_CACHE_FLUSH);
                    std::vector<SBluetoothDevice> discoveredDevices;

                    if (numberOfDevices >= 0)
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, "hci_inquiry: found " << numberOfDevices << " devices");

                        for (int i = 0; i < numberOfDevices; ++i)
                        {
                            bdaddr_t anyAddress = {{0, 0, 0, 0, 0, 0}};
                            std::vector<uint8_t> appLinkRFCOMMChannels;
                            sdp_session_t * sdpSession = sdp_connect(&anyAddress, &inquiryInfoList[i].bdaddr, SDP_RETRY_IF_BUSY);

                            if (0 != sdpSession)
                            {
                                sdp_list_t * searchList = sdp_list_append(0, &appLinkServiceUUID);
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
                                                                    appLinkRFCOMMChannels.push_back(d->val.uint8);
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

                                    if (false == appLinkRFCOMMChannels.empty())
                                    {
                                        char deviceAddressString[16];
                                        char deviceName[256];
                                        ba2str(&inquiryInfoList[i].bdaddr, deviceAddressString);
                                        std::stringstream rfcommChannelsString;

                                        for (std::vector<uint8_t>::const_iterator pi = appLinkRFCOMMChannels.begin(); pi != appLinkRFCOMMChannels.end(); ++pi)
                                        {
                                            if (pi != appLinkRFCOMMChannels.begin())
                                            {
                                                rfcommChannelsString << ", ";
                                            }

                                            rfcommChannelsString << static_cast<int>(*pi);
                                        }

                                        if (0 != hci_read_remote_name(deviceHandle, &inquiryInfoList[i].bdaddr, sizeof(deviceName) / sizeof(deviceName[0]), deviceName, 0))
                                        {
                                            LOG4CPLUS_WARN_EXT(mLogger, "hci_read_remote_name failed, error code " << errno << " (" << strerror(errno) << ")");
                                            strncpy(deviceName, deviceAddressString, sizeof(deviceName) / sizeof(deviceName[0]));
                                        }

                                        LOG4CPLUS_INFO_EXT(mLogger, "hci_inquiry: device " << i << ": " << deviceAddressString << " (\"" << deviceName << "\") service found at channel(s): " << rfcommChannelsString.str().c_str());

                                        discoveredDevices.push_back(SBluetoothDevice(inquiryInfoList[i].bdaddr, deviceName, appLinkRFCOMMChannels));
                                    }
                                }

                                sdp_list_free(searchList, 0);
                                sdp_list_free(attrList, 0);
                                sdp_list_free(responseList, 0);
                                sdp_close(sdpSession);
                            }
                        }
                    }
                    else
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, "hci_inquiry failed");
                    }

                    tBluetoothDevicesMap newDevices;
                    pthread_mutex_lock(&mDevicesMutex);
                    for (std::vector<SBluetoothDevice>::const_iterator discoveredDeviceIterator = discoveredDevices.begin(); discoveredDeviceIterator != discoveredDevices.end(); ++discoveredDeviceIterator)
                    {
                        const SBluetoothDevice & discoveredDevice = *discoveredDeviceIterator;
                        tDeviceHandle deviceHandle = InvalidDeviceHandle;

                        for (tBluetoothDevicesMap::iterator deviceIterator = mDevices.begin(); deviceIterator != mDevices.end(); ++deviceIterator)
                        {
                            SBluetoothDevice & exisingDevice = deviceIterator->second;

                            if (0 == memcmp(&exisingDevice.mAddress, &discoveredDevice.mAddress, sizeof(bdaddr_t)))
                            {
                                deviceHandle = deviceIterator->first;

                                LOG4CPLUS_INFO_EXT(mLogger, "Updating exising device " << deviceHandle << " (\"" << discoveredDevice.mName << "\")");
                            }
                        }

                        if (InvalidDeviceHandle == deviceHandle)
                        {
                            deviceHandle = mHandleGenerator.generateNewDeviceHandle();

                            LOG4CPLUS_INFO_EXT(mLogger, "Adding new device " << deviceHandle << " (\"" << discoveredDevice.mName << "\")");
                        }

                        newDevices[deviceHandle] = discoveredDevice;
                    }

                    bool needUpdateClientDeviceList = (newDevices.size() != mDevices.size());

                    if (false == needUpdateClientDeviceList)
                    {
                        for (tBluetoothDevicesMap::const_iterator di = mDevices.begin(); di != mDevices.end(); ++di)
                        {
                            tBluetoothDevicesMap::const_iterator ndi = newDevices.find(di->first);
                            bool matchingDeviceFound = (ndi != newDevices.end());
                            if (true == matchingDeviceFound)
                            {
                                matchingDeviceFound = (di->second.mName == ndi->second.mName);
                            }

                            if (false == matchingDeviceFound)
                            {
                                needUpdateClientDeviceList = true;
                                break;
                            }
                        }
                    }

                    mDevices = newDevices;

                    LOG4CPLUS_INFO_EXT(mLogger, "Discovered " << mDevices.size() << " device" << ((1u == mDevices.size()) ? "" : "s") << " with AppLink service. New devices map:");

                    for (tBluetoothDevicesMap::const_iterator deviceIterator = mDevices.begin(); deviceIterator != mDevices.end(); ++deviceIterator)
                    {
                        char deviceAddressString[16];
                        ba2str(&deviceIterator->second.mAddress, deviceAddressString);
                        LOG4CPLUS_INFO_EXT(mLogger, std::setw(10) << deviceIterator->first << std::setw(0) << ": " << deviceAddressString << ", " << deviceIterator->second.mName.c_str());
                    }

                    pthread_mutex_unlock(&mDevicesMutex);

                    close(deviceHandle);

                    if (true == needUpdateClientDeviceList)
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, "Client device list updated");

                        tInternalDeviceList clientDeviceList;
                        getDeviceList(clientDeviceList);
                        mListener.onDeviceListUpdated(this, clientDeviceList);
                    }

                    std::vector<tConnectionHandle> connectionsToTerminate;
                    std::set<std::pair<tDeviceHandle, uint8_t> > connectionsToEstablish;

                    for (tBluetoothDevicesMap::const_iterator newDeviceIterator = newDevices.begin(); newDeviceIterator != newDevices.end(); ++newDeviceIterator)
                    {
                        const SBluetoothDevice & device = newDeviceIterator->second;

                        for (std::vector<uint8_t>::const_iterator channelIterator = device.mAppLinkRFCOMMChannels.begin(); channelIterator != device.mAppLinkRFCOMMChannels.end(); ++channelIterator)
                        {
                            connectionsToEstablish.insert(std::make_pair(newDeviceIterator->first, *channelIterator));
                        }
                    }

                    pthread_mutex_lock(&mRFCOMMConnectionsMutex);

                    for (tRFCOMMConnectionMap::const_iterator connectionIterator = mRFCOMMConnections.begin(); connectionIterator != mRFCOMMConnections.end(); ++connectionIterator)
                    {
                        const SRFCOMMConnection connection = connectionIterator->second;
                        tBluetoothDevicesMap::const_iterator newDeviceIterator = newDevices.find(connection.mDeviceHandle);

                        if (newDevices.end() == newDeviceIterator)
                        {
                            LOG4CPLUS_INFO_EXT(mLogger, "Connection " << connectionIterator->first << " must be terminated (device " << connectionIterator->second.mDeviceHandle << " is lost)");

                            connectionsToTerminate.push_back(connectionIterator->first);
                        }
                        else
                        {
                            const std::vector<uint8_t> & appLinkRFCOMMChannels = newDeviceIterator->second.mAppLinkRFCOMMChannels;

                            if (appLinkRFCOMMChannels.end() == std::find(appLinkRFCOMMChannels.begin(), appLinkRFCOMMChannels.end(), connectionIterator->second.mRFCOMMChannel))
                            {
                                LOG4CPLUS_INFO_EXT(mLogger, "Connection " << connectionIterator->first << " must be terminated (no AppLink service found on channel " << connectionIterator->second.mRFCOMMChannel << ")");

                                connectionsToTerminate.push_back(connectionIterator->first);
                            }
                            else
                            {
                                const std::pair<tDeviceHandle, uint8_t> newConnection(connection.mDeviceHandle, connection.mRFCOMMChannel);

                                if (connectionsToEstablish.find(newConnection) != connectionsToEstablish.end())
                                {
                                    connectionsToEstablish.erase(newConnection);
                                }
                            }
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
                else
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "Failed to open bluetooth adapter device");
                }
            }
        }
        else
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "No bluetooth adapter device found");
        }
    }

    delete [] inquiryInfoList;

    LOG4CPLUS_INFO_EXT(mLogger, "Device discovery thread finished");
}

void * NsAppLink::NsTransportManager::CBluetoothAdapter::connectionThreadStartRoutine(void * Data)
{
    //LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");
}

void NsAppLink::NsTransportManager::CBluetoothAdapter::connectionThread(const NsAppLink::NsTransportManager::tConnectionHandle & ConnectionHandle)
{
    LOG4CPLUS_ERROR_EXT(mLogger, "Not implemented");
}
