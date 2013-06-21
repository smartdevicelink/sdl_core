/**
 * \file device_adapter_impl.cpp
 * \brief Class DeviceAdapterImpl.
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

#include <fcntl.h>
#include <memory.h>
#include <poll.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

//#include "DeviceAdapterListener.h"
//#include "HandleGenerator.h"
#include "device_adapter_impl.h"

namespace transport_manager {

log4cxx::LoggerPtr DeviceAdapterImpl::logger_ =
    log4cxx::LoggerPtr(log4cxx::Logger::getLogger( "TransportManager"));



DeviceAdapterImpl::Frame::Frame(int user_data, const uint8_t* data, const size_t data_size)
    : user_data_(user_data),
      data_(0),
      data_size_(0) {
  if ((0 != data) && (0u != data_size)) {
    data_ = new uint8_t[data_size];

    if (0 != data_) {
      data_size_ = data_size;
      memcpy(data_, data, data_size_);
    }
  }
}

DeviceAdapterImpl::Frame::~Frame(void)
{
    delete[] data_;
}

DeviceAdapterImpl::Device::Device(const char* name)
    : name_(name),
      unique_device_id_()
{
}

DeviceAdapterImpl::Device::~Device(void)
{
}

bool DeviceAdapterImpl::Device::isSameAs(const Device* other_device) const
{
    return true;
}

DeviceAdapterImpl::Connection::Connection(const DeviceHandle device_handle) :
    device_handle_(device_handle),
    connection_thread_(),
    notification_pipe_fds_(),
    connection_socket_(-1),
    frames_to_send_(),
    terminate_flag_(false)
{
}

DeviceAdapterImpl::Connection::~Connection(void)
{
  while (false == frames_to_send_.empty()) {
    delete frames_to_send_.front();
    frames_to_send_.pop();
  }
}

bool DeviceAdapterImpl::Connection::isSameAs(const Connection* other_connection) const
{
  bool result = false;

  if (0 != other_connection) {
    result = (device_handle_ == other_connection->device_handle_);
  }

  return result;
}

DeviceAdapterImpl::ConnectionThreadParameters::ConnectionThreadParameters(
    DeviceAdapterImpl& device_adapter, ConnectionHandle connection_handle)
    : device_adapter_(device_adapter),
      connection_handle_(connection_handle) {
}

DeviceAdapterImpl::DeviceAdapterImpl(
    DeviceAdapterListener& listener,
    DeviceHandleGenerator& handle_generator)
    : listener_(listener),
      handle_generator_(handle_generator),
      device_scan_requested_(false),
      device_scan_requested_mutex_(),
      device_scan_requested_cond_(),
      devices_(),
      devices_mutex_(),
      connections_(),
      connections_mutex_(),
      shutdownd_flag_(false),
      main_thread_(),
      main_thread_started_(false) {
  pthread_cond_init(&device_scan_requested_cond_, 0);
  pthread_mutex_init(&device_scan_requested_mutex_, 0);
  pthread_mutex_init(&devices_mutex_, 0);
  pthread_mutex_init(&connections_mutex_, 0);
}

DeviceAdapterImpl::~DeviceAdapterImpl() {
  pthread_mutex_destroy(&connections_mutex_);
  pthread_mutex_destroy(&devices_mutex_);
  pthread_mutex_destroy(&device_scan_requested_mutex_);
  pthread_cond_destroy(&device_scan_requested_cond_);
}

void DeviceAdapterImpl::run()
{
    LOG4CXX_INFO(logger_, "Initializing device adapter");

    int errorCode = pthread_create(&mMainThread, 0, &mainThreadStartRoutine, this);

    if (0 == errorCode)
    {
        mMainThreadStarted = true;
        LOG4CXX_INFO(logger_, "Device adapter main thread started");
    }
    else
    {
        LOG4CXX_ERROR(logger_, "Device adapter main thread start failed, error code " << errorCode);
    }
}

void NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::scanForNewDevices(void)
{
    pthread_mutex_lock(&mDeviceScanRequestedMutex);

    if (false == mDeviceScanRequested)
    {
        LOG4CXX_INFO(logger_, "Requesting device scan");

        mDeviceScanRequested = true;
        pthread_cond_signal(&mDeviceScanRequestedCond);
    }
    else
    {
        LOG4CXX_INFO(logger_, "Device scan is currently in progress");
    }

    pthread_mutex_unlock(&mDeviceScanRequestedMutex);
}

void NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::connectDevice(const NsSmartDeviceLink::NsTransportManager::tDeviceHandle DeviceHandle)
{
    bool isDeviceValid = false;

    pthread_mutex_lock(&mDevicesMutex);

    if (mDevices.end() != mDevices.find(DeviceHandle))
    {
        isDeviceValid = true;
    }
    else
    {
        LOG4CXX_ERROR(logger_, "Device handle " << DeviceHandle << " is invalid");
    }

    pthread_mutex_unlock(&mDevicesMutex);

    if (true == isDeviceValid)
    {
        std::vector<SConnection*> connections;
        createConnectionsListForDevice(DeviceHandle, connections);

        if (false == connections.empty())
        {
            LOG4CXX_INFO(logger_, "Connecting device " << DeviceHandle);

            for (std::vector<SConnection*>::iterator connectionIterator = connections.begin(); connectionIterator != connections.end(); ++connectionIterator)
            {
                startConnection(*connectionIterator);
            }
        }
        else
        {
            LOG4CXX_WARN(logger_, "No connections to establish on device " << DeviceHandle);
        }
    }
}

void NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::disconnectDevice(const NsSmartDeviceLink::NsTransportManager::tDeviceHandle DeviceHandle)
{
    bool isDeviceValid = false;

    pthread_mutex_lock(&mDevicesMutex);

    if (mDevices.end() != mDevices.find(DeviceHandle))
    {
        isDeviceValid = true;
    }
    else
    {
        LOG4CXX_ERROR(logger_, "Device handle " << DeviceHandle << " is invalid");
    }

    pthread_mutex_unlock(&mDevicesMutex);

    if (true == isDeviceValid)
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
                LOG4CXX_ERROR(logger_, "Connection " << connectionIterator->first << " is null");
            }
        }

        pthread_mutex_unlock(&mConnectionsMutex);

        for (std::vector<tConnectionHandle>::const_iterator connectionHandleIterator = connectionsToTerminate.begin(); connectionHandleIterator != connectionsToTerminate.end(); ++connectionHandleIterator)
        {
            stopConnection(*connectionHandleIterator);
        }
    }
}

void NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::sendFrame(NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize, int UserData)
{
    if (0u == DataSize)
    {
        LOG4CXX_WARN(logger_, "DataSize=0");
    }
    else if (0 == Data)
    {
        LOG4CXX_WARN(logger_, "Data is null");
    }
    else
    {
        pthread_mutex_lock(&mConnectionsMutex);

        tConnectionMap::iterator connectionIterator = mConnections.find(ConnectionHandle);

        if (mConnections.end() == connectionIterator)
        {
            LOG4CXX_ERROR(logger_, "Connection " << ConnectionHandle << " does not exist");
        }
        else
        {
            SConnection * connection = connectionIterator->second;

            if (0 != connection)
            {
                connection->mFramesToSend.push(new SFrame(UserData, Data, DataSize));

                if (-1 != connection->mNotificationPipeFds[1])
                {
                    uint8_t c = 0;
                    if (1 != write(connection->mNotificationPipeFds[1], &c, 1))
                    {
                        LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to wake up connection thread for connection " << connectionIterator->first);
                    }
                }
            }
        }

        pthread_mutex_unlock(&mConnectionsMutex);
    }
}

void NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::waitForThreadsTermination(void)
{
    mShutdownFlag = true;

    if (true == mMainThreadStarted)
    {
        LOG4CXX_INFO(logger_, "Waiting for device adapter main thread termination");
        pthread_join(mMainThread, 0);
        LOG4CXX_INFO(logger_, "Device adapter main thread terminated");
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
                    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to wake up connection thread for connection " << connectionIterator->first);
                }
            }
            connectionThreads.push_back(connection->mConnectionThread);
        }
        else
        {
            LOG4CXX_ERROR(logger_, "Connection " << connectionIterator->first << " is null");
        }
    }

    pthread_mutex_unlock(&mConnectionsMutex);

    LOG4CXX_INFO(logger_, "Waiting for connection threads termination");

    for (std::vector<pthread_t>::iterator connectionThreadIterator = connectionThreads.begin(); connectionThreadIterator != connectionThreads.end(); ++connectionThreadIterator)
    {
        pthread_join(*connectionThreadIterator, 0);
    }

    LOG4CXX_INFO(logger_, "Connection threads terminated");
}

bool DeviceAdapterImpl::startConnection(Connection* connection) {
  bool isConnectionThreadStarted = false;

  if (0 != connection) {
    ConnectionHandle newConnectionHandle = mHandleGenerator.generateNewConnectionHandle();

    pthread_mutex_lock(&connections_mutex_);

    if (!shutdown_flag_) {
      ConnectionMap::const_iterator connectionIterator;
      for (connectionIterator = connections_.begin();
          connectionIterator != connections_.end(); ++connectionIterator) {
        const Connection* existingConnection = connectionIterator->second;

        if (0 != existingConnection) {
          if (existingConnection->isSameAs(connection)) {
            LOG4CXX_WARN(
                logger_,
                "Connection is already opened (" << connectionIterator->first << ")");
            break;
          }
        } else {
          LOG4CXX_ERROR(
              logger_, "Connection " << connectionIterator->first << " is null");
        }
      }

      if (connections_.end() == connectionIterator) {
        std::pair<ConnectionMap::iterator, bool> insertResult = connections_
            .insert(std::make_pair(newConnectionHandle, connection));

        if (true == insertResult.second) {
          Connection* newConnection = insertResult.first->second;

          if (0 != newConnection) {
            ConnectionThreadParameters* connectionThreadParameters =
                ConnectionThreadParameters(*this, newConnectionHandle);

            int errorCode = pthread_create(
                &newConnection->connection_thread_, 0,
                &connectionThreadStartRoutine,
                static_cast<void*>(connectionThreadParameters));

            if (0 == errorCode) {
              LOG4CXX_INFO(
                  logger_,
                  "Connection thread started for connection " << newConnectionHandle << " (device " << newConnection->device_handle_ << ")");

              isConnectionThreadStarted = true;
            } else {
              LOG4CXX_ERROR(
                  logger_,
                  "Connection thread start failed for connection " << newConnectionHandle << " (device " << newConnection->device_handle_ << ")");

              delete connectionThreadParameters;
              connections_.erase(insertResult.first);
              delete newConnection;
            }
          } else {
            LOG4CXX_ERROR(
                logger_,
                "Failed to allocate connection " << newConnectionHandle);

            connections_.erase(newConnectionHandle);
          }
        } else {
          LOG4CXX_ERROR(
              logger_,
              "Connection handle " << newConnectionHandle << " already exists");
        }
      } else {
        delete Connection;
      }
    }

    pthread_mutex_unlock(&mConnectionsMutex);
  } else {
    LOG4CXX_ERROR(logger_, "Connection is null");
  }

  return isConnectionThreadStarted;
}

void NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::stopConnection(NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle)
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
                        LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to wake up connection thread for connection " << connectionIterator->first);
                    }
                }

                LOG4CXX_INFO(logger_, "Connection " << ConnectionHandle << "(device " << connection->mDeviceHandle << ") has been marked for termination");
            }
            else
            {
                LOG4CXX_WARN(logger_, "Connection " << ConnectionHandle << " is already terminating");
            }
        }
        else
        {
            LOG4CXX_ERROR(logger_, "Connection " << ConnectionHandle << " is null");
        }
    }
    else
    {
        LOG4CXX_WARN(logger_, "Connection " << ConnectionHandle << " does not exist");
    }

    pthread_mutex_unlock(&mConnectionsMutex);
}

bool NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::waitForDeviceScanRequest(const time_t Timeout)
{
    bool deviceScanRequested = false;

    pthread_mutex_lock(&mDeviceScanRequestedMutex);

    if (false == mDeviceScanRequested)
    {
        if (0 == Timeout)
        {
            if (0 != pthread_cond_wait(&mDeviceScanRequestedCond, &mDeviceScanRequestedMutex))
            {
                LOG4CXX_ERROR_WITH_ERRNO(logger_, "pthread_cond_wait failed");
            }
        }
        else
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
                LOG4CXX_ERROR_WITH_ERRNO(logger_, "clock_gettime failed");

                sleep(Timeout);
            }
        }
    }

    deviceScanRequested = mDeviceScanRequested;

    pthread_mutex_unlock(&mDeviceScanRequestedMutex);

    return deviceScanRequested;
}

void NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::handleCommunication(const NsSmartDeviceLink::NsTransportManager::tConnectionHandle ConnectionHandle)
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
                    if (true == isPipeCreated)
                    {
                        if (0 == fcntl(notificationPipeReadFd, F_SETFL, fcntl(notificationPipeReadFd, F_GETFL) | O_NONBLOCK))
                        {
                            LOG4CXX_INFO(logger_, "Connection " << ConnectionHandle << " to remote device " << clientDeviceInfo.mUniqueDeviceId << " established");

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
                                        LOG4CXX_INFO(logger_, "Connection " << ConnectionHandle << " terminated");

                                        connection->mTerminateFlag = true;
                                    }
                                    else if (0 != (pollFds[1].revents & (POLLERR | POLLHUP | POLLNVAL)))
                                    {
                                        LOG4CXX_ERROR(logger_, "Notification pipe for connection " << ConnectionHandle << " terminated");

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

                                                if (bytesRead > 0)
                                                {
                                                    LOG4CXX_INFO(logger_, "Received " << bytesRead << " bytes for connection " << ConnectionHandle);

                                                    mListener.onFrameReceived(this, ConnectionHandle, buffer, static_cast<size_t>(bytesRead));
                                                }
                                                else if (bytesRead < 0)
                                                {
                                                    if ((EAGAIN != errno) &&
                                                        (EWOULDBLOCK != errno))
                                                    {
                                                        LOG4CXX_ERROR_WITH_ERRNO(logger_, "recv() failed for connection " << ConnectionHandle);

                                                        connection->mTerminateFlag = true;
                                                    }
                                                }
                                                else
                                                {
                                                    LOG4CXX_INFO(logger_, "Connection " << ConnectionHandle << " closed by remote peer");

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
                                                LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to clear notification pipe for connection " << ConnectionHandle);

                                                connection->mTerminateFlag = true;
                                            }

                                            tFrameQueue framesToSend;

                                            pthread_mutex_lock(&mConnectionsMutex);
                                            framesToSend.swap(connection->mFramesToSend);
                                            pthread_mutex_unlock(&mConnectionsMutex);

                                            for (; false == framesToSend.empty(); framesToSend.pop())
                                            {
                                                SFrame * frame = framesToSend.front();
                                                ESendStatus frameSendStatus = SendStatusUnknownError;

                                                if (0 != frame)
                                                {
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
                                                                LOG4CXX_ERROR(logger_, "Sent " << bytesSent << " bytes while " << frame->mDataSize << " had been requested for connection " << ConnectionHandle);
                                                            }
                                                            else
                                                            {
                                                                LOG4CXX_ERROR_WITH_ERRNO(logger_, "Send failed for connection " << ConnectionHandle);
                                                            }

                                                            frameSendStatus = SendStatusFailed;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        LOG4CXX_ERROR(logger_, "Frame data is invalid for connection " << ConnectionHandle);

                                                        frameSendStatus = SendStatusInternalError;
                                                    }

                                                    delete frame;
                                                }
                                                else
                                                {
                                                    LOG4CXX_ERROR(logger_, "Frame data is null for connection " << ConnectionHandle);

                                                    frameSendStatus = SendStatusInternalError;
                                                }

                                                mListener.onFrameSendCompleted(this, ConnectionHandle, frame->mUserData, frameSendStatus);
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    LOG4CXX_ERROR_WITH_ERRNO(logger_, "poll() failed for connection " << ConnectionHandle);

                                    connection->mTerminateFlag = true;
                                }
                            }

                            mListener.onApplicationDisconnected(this, clientDeviceInfo, ConnectionHandle);
                        }
                        else
                        {
                            LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to set O_NONBLOCK for notification pipe for connection " << ConnectionHandle);
                        }
                    }
                    else
                    {
                        LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to create notification pipe for connection " << ConnectionHandle);
                    }
                }
                else
                {
                    LOG4CXX_ERROR(logger_, "Device for connection " << ConnectionHandle << " is invalid");
                }
            }
            else
            {
                LOG4CXX_ERROR(logger_, "Device handle for connection " << ConnectionHandle << " is invalid");
            }

            close(connectionSocket);
        }
        else
        {
            LOG4CXX_ERROR(logger_, "Socket is invalid for connection " << ConnectionHandle);
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
        LOG4CXX_ERROR(logger_, "Connection " << ConnectionHandle << " is not valid");
    }
}

void NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::updateClientDeviceList(void )
{
    LOG4CXX_INFO(logger_, "Updating client device list");

    tInternalDeviceList clientDeviceList;

    pthread_mutex_lock(&mDevicesMutex);

    for (tDeviceMap::const_iterator di = mDevices.begin(); di != mDevices.end(); ++di)
    {
        const SDevice * device = di->second;

        if (0 != device)
        {
            clientDeviceList.push_back(SInternalDeviceInfo(di->first, device->mName, device->mUniqueDeviceId));
        }
    }

    pthread_mutex_unlock(&mDevicesMutex);

    mListener.onDeviceListUpdated(this, clientDeviceList);
}

void NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::createConnectionsListForDevice(const NsSmartDeviceLink::NsTransportManager::tDeviceHandle DeviceHandle, std::vector<NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::SConnection *> & ConnectionsList)
{
    ConnectionsList.clear();
}

void * NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::mainThreadStartRoutine(void * Data)
{
    CDeviceAdapter * deviceAdapter = static_cast<CDeviceAdapter*>(Data);

    if (0 != deviceAdapter)
    {
        deviceAdapter->mainThread();
    }

    return 0;
}

void* DeviceAdapterImpl::connectionThreadStartRoutine(void* data)
{
    ConnectionThreadParameters* connectionThreadParameters = static_cast<ConnectionThreadParameters*>(data);

    if (0 != connectionThreadParameters)
    {
        DeviceAdapterImpl& deviceAdapter(connectionThreadParameters->device_adapter_);
        ConnectionHandle connectionHandle(connectionThreadParameters->connection_handle_);

        delete connectionThreadParameters;
        connectionThreadParameters = 0;

        deviceAdapter.connectionThread(connectionHandle);
    }

    return 0;
}

void DeviceAdapterImpl::connectDevice(const DeviceHandle device_handle) {
  bool is_device_valid = false;

  pthread_mutex_lock(&devices_mutex_);

  if (devices_.end() != devices_.find(device_handle)) {
    is_device_valid = true;
  } else {
    LOG4CXX_ERROR(logger_, "Device handle " << device_handle << " is invalid");
    listener_.onDeviceConnectFailed(this, device_handle, ConnectDeviceError());
  }

  pthread_mutex_unlock(&devices_mutex_);

  if (is_device_valid) {
    std::vector<Connection*> connections;
    createConnectionsListForDevice(device_handle, connections);

    if (! connections.empty()) {
      LOG4CXX_INFO(logger_, "Connecting device " << device_handle);

      for (std::vector<Connection*>::iterator connectionIterator = connections
          .begin(); connectionIterator != connections.end();
          ++connectionIterator) {
        startConnection(*connectionIterator);
      }
    } else {
      LOG4CXX_WARN(logger_,
                   "No connections to establish on device " << device_handle);
    }
  }
}

void DeviceAdapterImpl::disconnectDevice(const DeviceHandle device_handle) {
}

} // namespace transport_manager
