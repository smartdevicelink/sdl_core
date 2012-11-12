#include <fcntl.h>
#include <memory.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "IDeviceAdapterListener.hpp"
#include "IHandleGenerator.hpp"
#include "CDeviceAdapter.hpp"

NsAppLink::NsTransportManager::CDeviceAdapter::SFrame::SFrame(int SequenceNumber, const uint8_t* Data, const size_t DataSize):
mSequenceNumber(SequenceNumber),
mData(0),
mDataSize(0)
{
    if ((0 != Data) &&
        (0u != DataSize))
    {
        mData = new uint8_t[DataSize];

        if (0 != mData)
        {
            mDataSize = DataSize;
            memcpy(mData, Data, DataSize);
        }
    }
}

NsAppLink::NsTransportManager::CDeviceAdapter::SFrame::~SFrame(void)
{
    if (0 != mData)
    {
        delete [] mData;
    }
}

NsAppLink::NsTransportManager::CDeviceAdapter::SDevice::SDevice(const char * Name):
mName(Name),
mUniqueDeviceId(),
mIsConnected(false)
{
}

NsAppLink::NsTransportManager::CDeviceAdapter::SDevice::~SDevice(void)
{
}

bool NsAppLink::NsTransportManager::CDeviceAdapter::SDevice::isSameAs(const NsAppLink::NsTransportManager::CDeviceAdapter::SDevice * OtherDevice) const
{
    return true;
}

NsAppLink::NsTransportManager::CDeviceAdapter::SConnection::SConnection(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle):
mDeviceHandle(DeviceHandle),
mConnectionThread(),
mNotificationPipeFds(),
mConnectionSocket(-1),
mNextFrameSequenceNumber(0),
mFramesToSend(),
mTerminateFlag(false)
{
}

NsAppLink::NsTransportManager::CDeviceAdapter::SConnection::~SConnection(void)
{
    while (false == mFramesToSend.empty())
    {
        delete mFramesToSend.front();
        mFramesToSend.pop();
    }
}

bool NsAppLink::NsTransportManager::CDeviceAdapter::SConnection::isSameAs(const NsAppLink::NsTransportManager::CDeviceAdapter::SConnection * OtherConnection) const
{
    bool result = false;

    if (0 != OtherConnection)
    {
        result = (mDeviceHandle == OtherConnection->mDeviceHandle);
    }

    return result;
}

NsAppLink::NsTransportManager::CDeviceAdapter::SConnectionThreadParameters::SConnectionThreadParameters(NsAppLink::NsTransportManager::CDeviceAdapter & DeviceAdapter, NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle):
mDeviceAdapter(DeviceAdapter),
mConnectionHandle(ConnectionHandle)
{
}

NsAppLink::NsTransportManager::CDeviceAdapter::CDeviceAdapter(const char* LoggerName, NsAppLink::NsTransportManager::IDeviceAdapterListener& Listener, IHandleGenerator& HandleGenerator):
mLogger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(LoggerName))),
mListener(Listener),
mHandleGenerator(HandleGenerator),
mDeviceScanRequested(false),
mDeviceScanRequestedMutex(),
mDeviceScanRequestedCond(),
mDevices(),
mDevicesMutex(),
mConnections(),
mConnectionsMutex(),
mShutdownFlag(false),
mMainThread(),
mMainThreadStarted(false)
{
    pthread_cond_init(&mDeviceScanRequestedCond, 0);
    pthread_mutex_init(&mDeviceScanRequestedMutex, 0);
    pthread_mutex_init(&mDevicesMutex, 0);
    pthread_mutex_init(&mConnectionsMutex, 0);
}

NsAppLink::NsTransportManager::CDeviceAdapter::~CDeviceAdapter(void)
{
    pthread_mutex_destroy(&mConnectionsMutex);
    pthread_mutex_destroy(&mDevicesMutex);
    pthread_mutex_destroy(&mDeviceScanRequestedMutex);
    pthread_cond_destroy(&mDeviceScanRequestedCond);
}

void NsAppLink::NsTransportManager::CDeviceAdapter::run(void)
{
    LOG4CPLUS_INFO_EXT(mLogger, "Initializing device adapter");

    int errorCode = pthread_create(&mMainThread, 0, &mainThreadStartRoutine, this);

    if (0 == errorCode)
    {
        mMainThreadStarted = true;
        LOG4CPLUS_INFO_EXT(mLogger, "Device adapter main thread started");
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Device adapter main thread start failed, error code " << errorCode);
    }
}

void NsAppLink::NsTransportManager::CDeviceAdapter::scanForNewDevices(void)
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

void NsAppLink::NsTransportManager::CDeviceAdapter::connectDevice(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle)
{
    bool isConnectionRequired = false;

    pthread_mutex_lock(&mDevicesMutex);

    tDeviceMap::iterator deviceIterator = mDevices.find(DeviceHandle);

    if (mDevices.end() != deviceIterator)
    {
        SDevice * device = deviceIterator->second;

        if (0 != device)
        {
            if (false == device->mIsConnected)
            {
                device->mIsConnected = true;
                isConnectionRequired = true;
            }
            else
            {
                LOG4CPLUS_WARN_EXT(mLogger, "Device " << DeviceHandle << " is already connected");
            }
        }
        else
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Device " << DeviceHandle << " is invalid");
        }
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Device handle " << DeviceHandle << " is invalid");
    }

    pthread_mutex_unlock(&mDevicesMutex);

    if (true == isConnectionRequired)
    {
        std::vector<SConnection*> connections;
        createConnectionsListForDevice(DeviceHandle, connections);

        if (false == connections.empty())
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Connecting device " << DeviceHandle);

            for (std::vector<SConnection*>::iterator connectionIterator = connections.begin(); connectionIterator != connections.end(); ++connectionIterator)
            {
                startConnection(*connectionIterator);
            }
        }
        else
        {
            LOG4CPLUS_WARN_EXT(mLogger, "No connections to establish on device " << DeviceHandle);
        }
    }
}

void NsAppLink::NsTransportManager::CDeviceAdapter::disconnectDevice(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle)
{
    bool isDisconnectionRequired = false;

    pthread_mutex_lock(&mDevicesMutex);

    tDeviceMap::iterator deviceIterator = mDevices.find(DeviceHandle);

    if (mDevices.end() != deviceIterator)
    {
        SDevice * device = deviceIterator->second;

        if (0 != device)
        {
            if (true == device->mIsConnected)
            {
                device->mIsConnected = false;
                isDisconnectionRequired = true;
            }
            else
            {
                LOG4CPLUS_WARN_EXT(mLogger, "Device " << DeviceHandle << " is already disconnected");
            }
        }
        else
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Device " << DeviceHandle << " is invalid");
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

        pthread_mutex_lock(&mConnectionsMutex);

        for (tConnectionMap::const_iterator connectionIterator = mConnections.begin(); connectionIterator != mConnections.end(); ++connectionIterator)
        {
            SConnection * connection = connectionIterator->second;

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

        pthread_mutex_unlock(&mConnectionsMutex);

        for (std::vector<tConnectionHandle>::const_iterator connectionHandleIterator = connectionsToTerminate.begin(); connectionHandleIterator != connectionsToTerminate.end(); ++connectionHandleIterator)
        {
            stopConnection(*connectionHandleIterator);
        }
    }
}

int NsAppLink::NsTransportManager::CDeviceAdapter::sendFrame(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize)
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
        pthread_mutex_lock(&mConnectionsMutex);

        tConnectionMap::iterator connectionIterator = mConnections.find(ConnectionHandle);

        if (mConnections.end() == connectionIterator)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << ConnectionHandle << " does not exist");
        }
        else
        {
            SConnection * connection = connectionIterator->second;

            if (0 != connection)
            {
                frameSequenceNumber = connection->mNextFrameSequenceNumber++;

                connection->mFramesToSend.push(new SFrame(frameSequenceNumber, Data, DataSize));

                if (-1 != connection->mNotificationPipeFds[1])
                {
                    uint8_t c = 0;
                    if (1 != write(connection->mNotificationPipeFds[1], &c, 1))
                    {
                        LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to wake up connection thread for connection " << connectionIterator->first);
                    }
                }
            }
        }

        pthread_mutex_unlock(&mConnectionsMutex);
    }

    return frameSequenceNumber;
}

void NsAppLink::NsTransportManager::CDeviceAdapter::waitForThreadsTermination(void)
{
    mShutdownFlag = true;

    if (true == mMainThreadStarted)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Waiting for device adapter main thread termination");
        pthread_join(mMainThread, 0);
        LOG4CPLUS_INFO_EXT(mLogger, "Device adapter main thread terminated");
    }

    std::vector<pthread_t> connectionThreads;

    pthread_mutex_lock(&mConnectionsMutex);

    for (tConnectionMap::iterator connectionIterator = mConnections.begin(); connectionIterator != mConnections.end(); ++connectionIterator)
    {
        SConnection * connection = connectionIterator->second;

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

    pthread_mutex_unlock(&mConnectionsMutex);

    LOG4CPLUS_INFO_EXT(mLogger, "Waiting for connection threads termination");

    for (std::vector<pthread_t>::iterator connectionThreadIterator = connectionThreads.begin(); connectionThreadIterator != connectionThreads.end(); ++connectionThreadIterator)
    {
        pthread_join(*connectionThreadIterator, 0);
    }

    LOG4CPLUS_INFO_EXT(mLogger, "Connection threads terminated");
}

void NsAppLink::NsTransportManager::CDeviceAdapter::startConnection(NsAppLink::NsTransportManager::CDeviceAdapter::SConnection * Connection)
{
    if (0 != Connection)
    {
        tConnectionHandle newConnectionHandle = mHandleGenerator.generateNewConnectionHandle();

        pthread_mutex_lock(&mConnectionsMutex);

        if (false == mShutdownFlag)
        {
            tConnectionMap::const_iterator connectionIterator;
            for (connectionIterator = mConnections.begin(); connectionIterator != mConnections.end(); ++connectionIterator)
            {
                const SConnection * existingConnection = connectionIterator->second;

                if (0 != existingConnection)
                {
                    if (existingConnection->isSameAs(Connection))
                    {
                        LOG4CPLUS_WARN_EXT(mLogger, "Connection is already opened (" << connectionIterator->first << ")");

                        break;
                    }
                }
                else
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << connectionIterator->first << " is null");
                }
            }

            if (mConnections.end() == connectionIterator)
            {
                std::pair<tConnectionMap::iterator, bool> insertResult = mConnections.insert(std::make_pair(newConnectionHandle, Connection));

                if (true == insertResult.second)
                {
                    SConnection * newConnection = insertResult.first->second;

                    if (0 != newConnection)
                    {
                        SConnectionThreadParameters * connectionThreadParameters = new SConnectionThreadParameters(*this, newConnectionHandle);

                        int errorCode = pthread_create(&newConnection->mConnectionThread, 0, &CDeviceAdapter::connectionThreadStartRoutine, static_cast<void*>(connectionThreadParameters));

                        if (0 == errorCode)
                        {
                            LOG4CPLUS_INFO_EXT(mLogger, "Connection thread started for connection " << newConnectionHandle << " (device " << newConnection->mDeviceHandle << ")");
                        }
                        else
                        {
                            LOG4CPLUS_ERROR_EXT(mLogger, "Connection thread start failed for connection " << newConnectionHandle << " (device " << newConnection->mDeviceHandle << ")");

                            delete connectionThreadParameters;
                            mConnections.erase(insertResult.first);
                            delete newConnection;
                        }
                    }
                    else
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, "Failed to allocate connection " << newConnectionHandle);

                        mConnections.erase(newConnectionHandle);
                    }
                }
                else
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "Connection handle " << newConnectionHandle << " already exists");
                }
            }
            else
            {
                delete Connection;
            }
        }

        pthread_mutex_unlock(&mConnectionsMutex);
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Connection is null");
    }
}

void NsAppLink::NsTransportManager::CDeviceAdapter::stopConnection(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle)
{
    pthread_mutex_lock(&mConnectionsMutex);

    tConnectionMap::iterator connectionIterator = mConnections.find(ConnectionHandle);

    if (mConnections.end() != connectionIterator)
    {
        SConnection * connection = connectionIterator->second;

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

                LOG4CPLUS_INFO_EXT(mLogger, "Connection " << ConnectionHandle << "(device " << connection->mDeviceHandle << ") has been marked for termination");
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

    pthread_mutex_unlock(&mConnectionsMutex);
}

bool NsAppLink::NsTransportManager::CDeviceAdapter::waitForDeviceScanRequest(const time_t Timeout)
{
    bool deviceScanRequested = false;

    pthread_mutex_lock(&mDeviceScanRequestedMutex);

    if (false == mDeviceScanRequested)
    {
        timespec timeoutTime;

        if (0 == clock_gettime(CLOCK_REALTIME, &timeoutTime))
        {
            timeoutTime.tv_sec += Timeout;

            while (0 == pthread_cond_timedwait(&mDeviceScanRequestedCond, &mDeviceScanRequestedMutex, &timeoutTime))
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

            sleep(Timeout);
        }
    }

    deviceScanRequested = mDeviceScanRequested;

    pthread_mutex_unlock(&mDeviceScanRequestedMutex);

    return deviceScanRequested;
}

void NsAppLink::NsTransportManager::CDeviceAdapter::handleCommunication(const NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle)
{
    SConnection * connection = 0;
    bool isPipeCreated = false;
    int notificationPipeReadFd = -1;
    int connectionSocket = -1;
    tDeviceHandle deviceHandle = InvalidDeviceHandle;

    pthread_mutex_lock(&mConnectionsMutex);

    tConnectionMap::iterator connectionIterator = mConnections.find(ConnectionHandle);

    if (mConnections.end() != connectionIterator)
    {
        connection = connectionIterator->second;

        if (0 != connection)
        {
            isPipeCreated = (0 == pipe(connection->mNotificationPipeFds));
            notificationPipeReadFd = connection->mNotificationPipeFds[0];
            connectionSocket = connection->mConnectionSocket;
            deviceHandle = connection->mDeviceHandle;
        }
    }

    pthread_mutex_unlock(&mConnectionsMutex);

    if (0 != connection)
    {
        if (-1 != connectionSocket)
        {
            if (InvalidDeviceHandle != deviceHandle)
            {
                bool isDeviceValid = false;
                SDeviceInfo clientDeviceInfo;

                pthread_mutex_lock(&mDevicesMutex);

                tDeviceMap::const_iterator deviceIterator = mDevices.find(deviceHandle);

                if (deviceIterator != mDevices.end())
                {
                    const SDevice * device = deviceIterator->second;

                    if (0 != device)
                    {
                        isDeviceValid = true;

                        clientDeviceInfo.mDeviceHandle = deviceHandle;
                        clientDeviceInfo.mDeviceType = getDeviceType();
                        clientDeviceInfo.mUserFriendlyName = device->mName;
                        clientDeviceInfo.mUniqueDeviceId = device->mUniqueDeviceId;
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
                    if (true == isPipeCreated)
                    {
                        if (0 == fcntl(notificationPipeReadFd, F_SETFL, fcntl(notificationPipeReadFd, F_GETFL) | O_NONBLOCK))
                        {
                            LOG4CPLUS_INFO_EXT(mLogger, "Connection " << ConnectionHandle << " to remote device " << clientDeviceInfo.mUniqueDeviceId << " established");

                            mListener.onApplicationConnected(this, clientDeviceInfo, ConnectionHandle);

                            pollfd pollFds[2];
                            pollFds[0].fd = connectionSocket;
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
                                                bytesRead = recv(connectionSocket, buffer, sizeof(buffer), MSG_DONTWAIT);

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

                                            tFrameQueue framesToSend;

                                            pthread_mutex_lock(&mConnectionsMutex);
                                            framesToSend.swap(connection->mFramesToSend);
                                            pthread_mutex_unlock(&mConnectionsMutex);

                                            for (; false == framesToSend.empty(); framesToSend.pop())
                                            {
                                                int frameSequenceNumber = -1;
                                                SFrame * frame = framesToSend.front();
                                                ESendStatus frameSendStatus = SendStatusUnknownError;

                                                if (0 != frame)
                                                {
                                                    frameSequenceNumber = frame->mSequenceNumber;

                                                    if ((0 != frame->mData) &&
                                                        (0u != frame->mDataSize))
                                                    {
                                                        ssize_t bytesSent = send(connectionSocket, frame->mData, frame->mDataSize, 0);

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
                            LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to set O_NONBLOCK for notification pipe for connection " << ConnectionHandle);
                        }
                    }
                    else
                    {
                        LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to create notification pipe for connection " << ConnectionHandle);
                    }
                }
                else
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "Device for connection " << ConnectionHandle << " is invalid");
                }
            }
            else
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "Device handle for connection " << ConnectionHandle << " is invalid");
            }

            if (0 != close(connectionSocket))
            {
                LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Failed to close RFCOMM socket for connection " << ConnectionHandle);
            }
        }
        else
        {
            LOG4CPLUS_ERROR_EXT_WITH_ERRNO(mLogger, "Socket is invalid for connection " << ConnectionHandle);
        }

        if (true == isPipeCreated)
        {
            pthread_mutex_lock(&mConnectionsMutex);

            close(connection->mNotificationPipeFds[0]);
            close(connection->mNotificationPipeFds[1]);

            connection->mNotificationPipeFds[0] = connection->mNotificationPipeFds[1] = -1;

            pthread_mutex_unlock(&mConnectionsMutex);
        }
    }
    else
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Connection " << ConnectionHandle << " is not valid");
    }
}

void NsAppLink::NsTransportManager::CDeviceAdapter::createConnectionsListForDevice(const NsAppLink::NsTransportManager::tDeviceHandle DeviceHandle, std::vector<NsAppLink::NsTransportManager::CDeviceAdapter::SConnection *> & ConnectionsList)
{
    ConnectionsList.clear();
}

void * NsAppLink::NsTransportManager::CDeviceAdapter::mainThreadStartRoutine(void * Data)
{
    CDeviceAdapter * deviceAdapter = static_cast<CDeviceAdapter*>(Data);

    if (0 != deviceAdapter)
    {
        deviceAdapter->mainThread();
    }

    return 0;
}

void * NsAppLink::NsTransportManager::CDeviceAdapter::connectionThreadStartRoutine(void * Data)
{
    SConnectionThreadParameters * connectionThreadParameters = static_cast<SConnectionThreadParameters*>(Data);

    if (0 != connectionThreadParameters)
    {
        CDeviceAdapter & deviceAdapter(connectionThreadParameters->mDeviceAdapter);
        tConnectionHandle connectionHandle(connectionThreadParameters->mConnectionHandle);

        delete connectionThreadParameters;
        connectionThreadParameters = 0;

        deviceAdapter.connectionThread(connectionHandle);
    }

    return 0;
}
