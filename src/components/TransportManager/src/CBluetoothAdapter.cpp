/**
 * \file CBluetoothAdapter.cpp
 * \brief Class CBluetoothAdapter.
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <iomanip>
#include <set>
#include <algorithm>
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

NsSmartDeviceLink::NsTransportManager::CBluetoothAdapter::SBluetoothDevice::SBluetoothDevice(const bdaddr_t & Address, const char * Name, const NsSmartDeviceLink::NsTransportManager::CBluetoothAdapter::tRFCOMMChannelVector & SmartDeviceLinkRFCOMMChannels):
SDevice(Name),
mAddress(Address),
mSmartDeviceLinkRFCOMMChannels(SmartDeviceLinkRFCOMMChannels)
{
    mUniqueDeviceId = getUniqueDeviceId(Address);
}

bool NsSmartDeviceLink::NsTransportManager::CBluetoothAdapter::SBluetoothDevice::isSameAs(const NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::SDevice * OtherDevice) const
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

NsSmartDeviceLink::NsTransportManager::CBluetoothAdapter::SRFCOMMConnection::SRFCOMMConnection(const NsSmartDeviceLink::NsTransportManager::tDeviceHandle DeviceHandle, const uint8_t RFCOMMChannel):
SConnection(DeviceHandle),
mRFCOMMChannel(RFCOMMChannel)
{
}

bool NsSmartDeviceLink::NsTransportManager::CBluetoothAdapter::SRFCOMMConnection::isSameAs(const NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::SConnection * OtherConnection) const
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

NsSmartDeviceLink::NsTransportManager::CBluetoothAdapter::CBluetoothAdapter(NsSmartDeviceLink::NsTransportManager::IDeviceAdapterListener & Listener, IHandleGenerator & HandleGenerator):
CDeviceAdapter(Listener, HandleGenerator),
mSmartDeviceLinkServiceUUID()
{
    LOG4CXX_INFO(logger_, "BluetoothAdapter constructed");

    uint8_t SmartDeviceLinkServiceUUIDData[] = {0x93, 0x6D, 0xA0, 0x1F, 0x9A, 0xBD, 0x4D, 0x9D, 0x80, 0xC7, 0x02, 0xAF, 0x85, 0xC8, 0x22, 0xA8};
    sdp_uuid128_create(&mSmartDeviceLinkServiceUUID, SmartDeviceLinkServiceUUIDData);
}

NsSmartDeviceLink::NsTransportManager::CBluetoothAdapter::~CBluetoothAdapter(void)
{
    LOG4CXX_INFO(logger_, "BluetoothAdapter destructor");

    waitForThreadsTermination();
}

NsSmartDeviceLink::NsTransportManager::EDeviceType NsSmartDeviceLink::NsTransportManager::CBluetoothAdapter::getDeviceType(void) const
{
    return DeviceBluetooth;
}

void NsSmartDeviceLink::NsTransportManager::CBluetoothAdapter::createConnectionsListForDevice(const NsSmartDeviceLink::NsTransportManager::tDeviceHandle DeviceHandle, std::vector< NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::SConnection* >& ConnectionsList)
{
    bdaddr_t deviceAddress;
    tRFCOMMChannelVector rfcommChannels;
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
            rfcommChannels = device->mSmartDeviceLinkRFCOMMChannels;
            isDeviceValid = true;
        }
        else
        {
            LOG4CXX_WARN(logger_, "Device " << DeviceHandle << " is invalid");
        }
    }
    else
    {
        LOG4CXX_ERROR(logger_, "Device handle " << DeviceHandle << " is invalid");
    }

    pthread_mutex_unlock(&mDevicesMutex);

    if (true == isDeviceValid)
    {
        if (false == rfcommChannels.empty())
        {
            pthread_mutex_lock(&mConnectionsMutex);

            for (tConnectionMap::const_iterator connectionIterator = mConnections.begin(); connectionIterator != mConnections.end(); ++connectionIterator)
            {
                const SRFCOMMConnection * connection = dynamic_cast<SRFCOMMConnection*>(connectionIterator->second);

                if (0 != connection)
                {
                    rfcommChannels.erase(std::remove_if(rfcommChannels.begin(),
                                                        rfcommChannels.end(),
                                                        [&DeviceHandle, &connection] (const uint8_t & Channel)
                                                        {
                                                            return (connection->mDeviceHandle == DeviceHandle) &&
                                                                   (connection->mRFCOMMChannel == Channel);
                                                        }
                                                       ),
                                         rfcommChannels.end());
                }
            }

            pthread_mutex_unlock(&mConnectionsMutex);

            for (tRFCOMMChannelVector::const_iterator channelIterator = rfcommChannels.begin(); channelIterator != rfcommChannels.end(); ++channelIterator)
            {
                ConnectionsList.push_back(new SRFCOMMConnection(DeviceHandle, *channelIterator));
            }
        }
        else
        {
            LOG4CXX_WARN(logger_, "SmartDeviceLink service was not discovered on device " << DeviceHandle);
        }
    }
}

void NsSmartDeviceLink::NsTransportManager::CBluetoothAdapter::mainThread(void)
{
    LOG4CXX_INFO(logger_, "Bluetooth adapter main thread initialized");

    const size_t maxDevices = 256u;
    inquiry_info * inquiryInfoList = new inquiry_info[maxDevices];

    while (false == mShutdownFlag)
    {
        tDeviceMap newDevices;
        tDeviceVector discoveredDevices;

        bool deviceScanRequested = waitForDeviceScanRequest(0);

        if (true == deviceScanRequested)
        {
            int deviceID = hci_get_route(0);

            if (deviceID >= 0)
            {
                int deviceHandle = hci_open_dev(deviceID);

                if (deviceHandle >= 0)
                {
                    const uint8_t inquiryTime = 8u; // Time unit is 1.28 seconds

                    LOG4CXX_INFO(logger_, "Starting hci_inquiry on device " << deviceID);

                    int numberOfDevices = hci_inquiry(deviceID, inquiryTime, maxDevices, 0, &inquiryInfoList, IREQ_CACHE_FLUSH);

                    if (numberOfDevices >= 0)
                    {
                        LOG4CXX_INFO(logger_, "hci_inquiry: found " << numberOfDevices << " devices");

                        for (int i = 0; i < numberOfDevices; ++i)
                        {
                            tRFCOMMChannelVector SmartDeviceLinkRFCOMMChannels;
                            discoverSmartDeviceLinkRFCOMMChannels(inquiryInfoList[i].bdaddr, SmartDeviceLinkRFCOMMChannels);

                            if (false == SmartDeviceLinkRFCOMMChannels.empty())
                            {
                                char deviceName[256];

                                if (0 != hci_read_remote_name(deviceHandle, &inquiryInfoList[i].bdaddr, sizeof(deviceName) / sizeof(deviceName[0]), deviceName, 0))
                                {
                                    LOG4CXX_ERROR_WITH_ERRNO(logger_, "hci_read_remote_name failed");
                                    strncpy(deviceName, getUniqueDeviceId(inquiryInfoList[i].bdaddr).c_str(), sizeof(deviceName) / sizeof(deviceName[0]));
                                }

                                discoveredDevices.push_back(new SBluetoothDevice(inquiryInfoList[i].bdaddr, deviceName, SmartDeviceLinkRFCOMMChannels));
                            }
                        }
                    }
                    else
                    {
                        LOG4CXX_ERROR(logger_, "hci_inquiry failed");
                    }

                    close(deviceHandle);
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

                        LOG4CXX_INFO(logger_, "Adding new device " << deviceHandle << " (\"" << discoveredDevice->mName << "\")");
                    }

                    newDevices[deviceHandle] = discoveredDevice;
                }
            }

            pthread_mutex_lock(&mConnectionsMutex);

            std::set<tDeviceHandle> connectedDevices;

            for (tConnectionMap::const_iterator connectionIterator = mConnections.begin(); connectionIterator != mConnections.end(); ++connectionIterator)
            {
                const SConnection * connection = connectionIterator->second;

                if (0 != connection)
                {
                    if (connectedDevices.end() == connectedDevices.find(connection->mDeviceHandle))
                    {
                        connectedDevices.insert(connection->mDeviceHandle);
                    }
                }
            }

            pthread_mutex_unlock(&mConnectionsMutex);

            pthread_mutex_lock(&mDevicesMutex);

            for (tDeviceMap::iterator deviceIterator = mDevices.begin(); deviceIterator != mDevices.end(); ++deviceIterator)
            {
                SDevice * device = deviceIterator->second;

                if (0 != device)
                {
                    if (newDevices.end() == newDevices.find(deviceIterator->first))
                    {
                        if (connectedDevices.end() != connectedDevices.find(deviceIterator->first))
                        {
                            newDevices[deviceIterator->first] = device;
                            device = 0;
                        }
                    }

                    if (0 != device)
                    {
                        delete device;
                    }
                }
            }

            mDevices = newDevices;

            pthread_mutex_unlock(&mDevicesMutex);

            LOG4CXX_INFO(logger_, "Discovered " << newDevices.size() << " device" << ((1u == newDevices.size()) ? "" : "s") << " with SmartDeviceLink service. New devices map:");

            for (tDeviceMap::iterator deviceIterator = newDevices.begin(); deviceIterator != newDevices.end(); ++deviceIterator)
            {
                SDevice * device = deviceIterator->second;

                if (0 != device)
                {
                    LOG4CXX_INFO(logger_, std::setw(10) << deviceIterator->first << std::setw(0) << ": " << device->mUniqueDeviceId << ", " << device->mName.c_str());
                }
                else
                {
                    LOG4CXX_ERROR(logger_, std::setw(10) << deviceIterator->first << std::setw(0) << ": Device is null");
                }
            }

            mDeviceScanRequested = false;

            updateClientDeviceList();
        }
    }

    delete [] inquiryInfoList;

    LOG4CXX_INFO(logger_, "Bluetooth adapter main thread finished");
}

void NsSmartDeviceLink::NsTransportManager::CBluetoothAdapter::connectionThread(const NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle)
{
    LOG4CXX_INFO(logger_, "Connection thread started for connection " << ConnectionHandle);

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
            LOG4CXX_ERROR(logger_, "Connection " << ConnectionHandle << " is not valid");
        }
    }
    else
    {
        LOG4CXX_ERROR(logger_, "Connection " << ConnectionHandle << " does not exist");
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
                    LOG4CXX_ERROR(logger_, "Device " << deviceHandle << " is not valid");
                }
            }
            else
            {
                LOG4CXX_ERROR(logger_, "Device " << deviceHandle << " does not exist");
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
                        LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to connect to remote device " << getUniqueDeviceId(remoteSocketAddress.rc_bdaddr) << " for connection " << ConnectionHandle);
                    }
                }
                else
                {
                    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to create RFCOMM socket for connection " << ConnectionHandle);
                }
            }
        }
        else
        {
            LOG4CXX_ERROR(logger_, "Device handle for connection " << ConnectionHandle << " is invalid");
        }
    }
    else
    {
        LOG4CXX_ERROR(logger_, "Connection " << ConnectionHandle << " is null");
    }

    LOG4CXX_INFO(logger_, "Removing connection " << ConnectionHandle << " from connection map");

    pthread_mutex_lock(&mConnectionsMutex);
    mConnections.erase(ConnectionHandle);
    pthread_mutex_unlock(&mConnectionsMutex);

    delete connection;

    LOG4CXX_INFO(logger_, "Connection thread finished for connection " << ConnectionHandle);
}

std::string NsSmartDeviceLink::NsTransportManager::CBluetoothAdapter::getUniqueDeviceId(const bdaddr_t & DeviceAddress)
{
    char deviceAddressString[32];

    ba2str(&DeviceAddress, deviceAddressString);

    return std::string("BT-") + deviceAddressString;
}

void NsSmartDeviceLink::NsTransportManager::CBluetoothAdapter::discoverSmartDeviceLinkRFCOMMChannels(const bdaddr_t & DeviceAddress, NsSmartDeviceLink::NsTransportManager::CBluetoothAdapter::tRFCOMMChannelVector & SmartDeviceLinkRFCOMMChannels)
{
    SmartDeviceLinkRFCOMMChannels.clear();

    static bdaddr_t anyAddress = {{0, 0, 0, 0, 0, 0}};

    sdp_session_t * sdpSession = sdp_connect(&anyAddress, &DeviceAddress, SDP_RETRY_IF_BUSY);

    if (0 != sdpSession)
    {
        sdp_list_t * searchList = sdp_list_append(0, &mSmartDeviceLinkServiceUUID);
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
                                            SmartDeviceLinkRFCOMMChannels.push_back(d->val.uint8);
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
        LOG4CXX_ERROR(logger_, "Service discovery failed for " << getUniqueDeviceId(DeviceAddress));
    }

    if (false == SmartDeviceLinkRFCOMMChannels.empty())
    {
        std::stringstream rfcommChannelsString;

        for (tRFCOMMChannelVector::const_iterator channelIterator = SmartDeviceLinkRFCOMMChannels.begin(); channelIterator != SmartDeviceLinkRFCOMMChannels.end(); ++channelIterator)
        {
            if (channelIterator != SmartDeviceLinkRFCOMMChannels.begin())
            {
                rfcommChannelsString << ", ";
            }

            rfcommChannelsString << static_cast<uint32_t>(*channelIterator);
        }

        LOG4CXX_INFO(logger_, "SmartDeviceLink service was discovered on device " << getUniqueDeviceId(DeviceAddress) << " at channel(s): " << rfcommChannelsString.str().c_str());
    }
    else
    {
        LOG4CXX_INFO(logger_, "SmartDeviceLink service was not discovered on device " << getUniqueDeviceId(DeviceAddress));
    }
}
