/**
 * \file CTCPAdapter.cpp
 * \brief Class CTCPAdapter.
 * Copyright (c) 2013 Ford Motor Company
 */

#include <memory.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "CTCPAdapter.hpp"
#include "IHandleGenerator.hpp"

NsSmartDeviceLink::NsTransportManager::CTCPAdapter::STCPDevice::STCPDevice(const char* Name, const in_addr_t & Address):
SDevice(Name),
mAddress(Address)
{
}

NsSmartDeviceLink::NsTransportManager::CTCPAdapter::STCPDevice::~STCPDevice(void)
{
}

bool NsSmartDeviceLink::NsTransportManager::CTCPAdapter::STCPDevice::isSameAs(const NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::SDevice * OtherDevice) const
{
    bool result = false;

    if (true == SDevice::isSameAs(OtherDevice))
    {
        const STCPDevice * otherTCPDevice = dynamic_cast<const STCPDevice*>(OtherDevice);

        if (0 != otherTCPDevice)
        {
            result = (mAddress == otherTCPDevice->mAddress);
        }
    }

    return result;
}

NsSmartDeviceLink::NsTransportManager::CTCPAdapter::STCPConnection::STCPConnection(const NsSmartDeviceLink::NsTransportManager::tDeviceHandle DeviceHandle, int ConnectionSocket, const in_port_t Port):
SConnection(DeviceHandle),
mPort(Port)
{
    mConnectionSocket = ConnectionSocket;
}

NsSmartDeviceLink::NsTransportManager::CTCPAdapter::STCPConnection::~STCPConnection(void)
{
}

bool NsSmartDeviceLink::NsTransportManager::CTCPAdapter::STCPConnection::isSameAs(const NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::SConnection* OtherConnection) const
{
    bool result = false;

    if (true == SConnection::isSameAs(OtherConnection))
    {
        const STCPConnection * otherTCPConnection = dynamic_cast<const STCPConnection*>(OtherConnection);

        if (0 != otherTCPConnection)
        {
            result = (mPort == otherTCPConnection->mPort);
        }
    }

    return result;
}

NsSmartDeviceLink::NsTransportManager::CTCPAdapter::CTCPAdapter(NsSmartDeviceLink::NsTransportManager::IDeviceAdapterListener & Listener, NsSmartDeviceLink::NsTransportManager::IHandleGenerator & HandleGenerator):
CDeviceAdapter(Listener, HandleGenerator)
{
}

NsSmartDeviceLink::NsTransportManager::CTCPAdapter::~CTCPAdapter(void)
{
    waitForThreadsTermination();
}

NsSmartDeviceLink::NsTransportManager::EDeviceType NsSmartDeviceLink::NsTransportManager::CTCPAdapter::getDeviceType(void) const
{
    return DeviceWiFi;
}

void NsSmartDeviceLink::NsTransportManager::CTCPAdapter::mainThread(void)
{
    LOG4CPLUS_INFO(mLogger, "Main thread initialized");

    int socketFd = socket(AF_INET, SOCK_STREAM, 0);

    if (-1 != socketFd)
    {
        sockaddr_in serverAddress;

        memset(&serverAddress, 0, sizeof(serverAddress));

        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(cTCPAdapterPort);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        if (0 == bind(socketFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)))
        {
            if (0 == listen(socketFd, 128))
            {
                while (false == mShutdownFlag)
                {
                    sockaddr_in clientAddress;
                    socklen_t clientAddressSize = sizeof(clientAddress);

                    int connectionFd = accept(socketFd, (struct sockaddr*)&clientAddress, &clientAddressSize);

                    if (connectionFd >= 0)
                    {
                        bool isConnectionThreadStarted = false;

                        if ((AF_INET == clientAddress.sin_family) &&
                            (sizeof(clientAddress) == clientAddressSize))
                        {
                            char deviceName[32];

                            strncpy(deviceName, inet_ntoa(clientAddress.sin_addr), sizeof(deviceName) / sizeof(deviceName[0]));

                            LOG4CPLUS_INFO(mLogger, "Connected client " << deviceName);

                            tDeviceHandle deviceHandle = InvalidDeviceHandle;

                            pthread_mutex_lock(&mDevicesMutex);

                            for (tDeviceMap::const_iterator deviceIterator = mDevices.begin(); deviceIterator != mDevices.end(); ++deviceIterator)
                            {
                                const STCPDevice * device = dynamic_cast<const STCPDevice *>(deviceIterator->second);

                                if (0 != device)
                                {
                                    if (clientAddress.sin_addr.s_addr == device->mAddress)
                                    {
                                        deviceHandle = deviceIterator->first;
                                        break;
                                    }
                                }
                            }

                            pthread_mutex_unlock(&mDevicesMutex);

                            if (InvalidDeviceHandle == deviceHandle)
                            {
                                deviceHandle = mHandleGenerator.generateNewDeviceHandle();
                                STCPDevice * device = new STCPDevice(deviceName, clientAddress.sin_addr.s_addr);

                                pthread_mutex_lock(&mDevicesMutex);

                                if (true == mDevices.insert(std::make_pair(deviceHandle, device)).second)
                                {
                                    device->mUniqueDeviceId = std::string("TCP-") + deviceName;

                                    LOG4CPLUS_INFO(mLogger, "Added new device " << deviceHandle << ": " << device->mName << " (" << device->mUniqueDeviceId << ")");
                                }
                                else
                                {
                                    LOG4CPLUS_ERROR(mLogger, "Device handle " << deviceHandle << " is already present in devices map");

                                    deviceHandle = InvalidDeviceHandle;
                                    delete device;
                                    device = 0;
                                }

                                pthread_mutex_unlock(&mDevicesMutex);

                                if (0 != device)
                                {
                                    updateClientDeviceList();
                                }
                            }

                            if (InvalidDeviceHandle != deviceHandle)
                            {
                                isConnectionThreadStarted = startConnection(new STCPConnection(deviceHandle, connectionFd, clientAddress.sin_port));
                            }
                            else
                            {
                                LOG4CPLUS_ERROR(mLogger, "Failed to insert new device into devices map");
                            }
                        }
                        else
                        {
                            LOG4CPLUS_ERROR(mLogger, "Address of connected client is invalid");
                        }

                        if (false == isConnectionThreadStarted)
                        {
                            close(connectionFd);
                        }
                    }
                    else
                    {
                        LOG4CPLUS_ERROR_WITH_ERRNO(mLogger, "accept() failed");
                    }
                }
            }
            else
            {
                LOG4CPLUS_ERROR_WITH_ERRNO(mLogger, "listen() failed");
            }
        }
        else
        {
            LOG4CPLUS_ERROR_WITH_ERRNO(mLogger, "bind() failed");
        }

        close(socketFd);
    }
    else
    {
        LOG4CPLUS_ERROR_WITH_ERRNO(mLogger, "Failed to create socket");
    }

    LOG4CPLUS_INFO(mLogger, "Main thread finished");
}

void NsSmartDeviceLink::NsTransportManager::CTCPAdapter::connectionThread(const NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle)
{
    handleCommunication(ConnectionHandle);

    tDeviceHandle deviceHandle = InvalidDeviceHandle;

    pthread_mutex_lock(&mConnectionsMutex);

    tConnectionMap::iterator connectionIterator = mConnections.find(ConnectionHandle);

    if (mConnections.end() != connectionIterator)
    {
        STCPConnection * connection = dynamic_cast<STCPConnection*>(connectionIterator->second);

        if (0 != connection)
        {
            deviceHandle = connection->mDeviceHandle;

            delete connection;
        }
        else
        {
            LOG4CPLUS_ERROR(mLogger, "Connection " << ConnectionHandle << " is invalid");
        }

        mConnections.erase(connectionIterator);
    }
    else
    {
        LOG4CPLUS_ERROR(mLogger, "Connection " << ConnectionHandle << " was not found in connections map");
    }

    for (connectionIterator = mConnections.begin(); connectionIterator != mConnections.end(); ++connectionIterator)
    {
        SConnection * connection = connectionIterator->second;

        if (0 != connection)
        {
            if (deviceHandle == connection->mDeviceHandle)
            {
                deviceHandle = InvalidDeviceHandle;
                break;
            }
        }
    }

    pthread_mutex_unlock(&mConnectionsMutex);

    if (InvalidDeviceHandle != deviceHandle)
    {
        LOG4CPLUS_INFO(mLogger, "No connections left for device " << deviceHandle << ". Deleting device");

        pthread_mutex_lock(&mDevicesMutex);

        tDeviceMap::iterator deviceIterator = mDevices.find(deviceHandle);

        if (mDevices.end() != deviceIterator)
        {
            delete deviceIterator->second;

            mDevices.erase(deviceIterator);
        }

        pthread_mutex_unlock(&mDevicesMutex);

        updateClientDeviceList();
    }
}
