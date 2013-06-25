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

#include <errno.h>
#include <fcntl.h>
#include <memory.h>
#include <poll.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

//#include "DeviceAdapterListener.h"
//#include "HandleGenerator.h"
#include "device_adapter_impl.h"
#include "transport_manager/device_adapter_listener.h"

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

DeviceAdapterImpl::Connection::Connection(const DeviceHandle device_handle,
                                          const ApplicationHandle app_handle,
                                          const int session_id)
    : device_handle_(device_handle),
      app_handle_(app_handle),
      session_id_(session_id),
      connection_thread_(),
      notification_pipe_fds_(),
      connection_socket_(-1),
      frames_to_send_(),
      terminate_flag_(false) {
}

DeviceAdapterImpl::Connection::~Connection(void) {
  while (false == frames_to_send_.empty()) {
    delete frames_to_send_.front();
    frames_to_send_.pop();
  }
}

bool DeviceAdapterImpl::Connection::isSameAs(
    const Connection* other_connection) const {
  bool result = false;

  if (0 != other_connection) {
    result = (device_handle_ == other_connection->device_handle_);
  }

  return result;
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
      shutdown_flag_(false),
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

DeviceAdapter::Error DeviceAdapterImpl::createConnection(
    const DeviceHandle device_handle, const ApplicationHandle app_handle,
    const int session_id, Connection** connection) {
  assert(connection != 0);
  *connection = 0;

  if(shutdown_flag_) {
    return DeviceAdapter::BAD_STATE;
  }

  DeviceAdapter::Error error = DeviceAdapter::OK;

  pthread_mutex_lock(&connections_mutex_);

    for (ConnectionMap::const_iterator it = connections_.begin();
        it != connections_.end(); ++it) {
      const Connection& existingConnection = *it;
      if (existingConnection.application_handle() == app_handle
          && existingConnection.device_handle() == device_handle) {
        error = DeviceAdapter::ALREADY_EXIST;
        break;
      }
      if (existingConnection.session_id() == session_id) {
        error = DeviceAdapter::ALREADY_EXIST;
        break;
      }
    }

  if(error == DeviceAdapter::OK) {
    *connection = new Connection(device_handle, app_handle, session_id);
    connections_[session_id] = *connection;
  }

  pthread_mutex_unlock(&connections_mutex_);

  return error;
}

void DeviceAdapterImpl::deleteConnection(Connection* connection) {
  assert(connection != 0);

  pthread_mutex_lock(&connections_mutex_);
  connections_.erase(connection->session_id());
  pthread_mutex_unlock(&connections_mutex_);
  delete connection;
}

DeviceAdapter::Error DeviceAdapterImpl::startConnection(Connection* connection) {
  assert(connection != 0);

  if (shutdown_flag_) {
    return DeviceAdapter::BAD_STATE;
  }

  bool is_thread_started = false;

  if (!shutdown_flag_) {
    ConnectionThreadParameters* thread_params = new ConnectionThreadParameters;
    thread_params->device_adapter = this;
    thread_params->connection = connection;

    int errorCode = pthread_create(
        &connection->connection_thread_, 0, &connectionThreadStartRoutine,
        static_cast<void*>(thread_params));

    if (0 == errorCode) {
      LOG4CXX_INFO(
          logger_,
          "Connection thread started for session " << connection->session_id() << " (device " << connection->device_handle() << ")");

      is_thread_started = true;
    } else {
      LOG4CXX_ERROR(
          logger_,
          "Connection thread start failed for session " << connection->session_id() << " (device " << connection->device_handle() << ")");

      delete thread_params;
    }
  }

  return is_thread_started ? DeviceAdapter::OK : DeviceAdapter::FAIL;
}

DeviceAdapter::Error DeviceAdapterImpl::stopConnection(Connection* connection) {
  assert(connection != 0);
  if (false == connection->terminate_flag_) {
    connection->terminate_flag_ = true;
    if (-1 != connection->notification_pipe_fds_[1]) {
      uint8_t c = 0;
      if (1 != ::write(connection->notification_pipe_fds_[1], &c, 1)) {
        LOG4CXX_ERROR_WITH_ERRNO(
            logger_,
            "Failed to wake up connection thread for connection " << connection->session_id());
      }
      return FAIL;
    }

    LOG4CXX_INFO(
        logger_,
        "Connection " << connection->session_id() << "(device " << connection->device_handle() << ") has been marked for termination");
  } else {
    LOG4CXX_WARN(
        logger_,
        "Connection " << connection->session_id() << " is already terminating");
  }
  return OK;
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

void DeviceAdapterImpl::handleCommunication(Connection* connection) {
  assert(connection != 0);

  DeviceHandle device_handle = connection->device_handle();
  const bool is_pipe_created = (0 == ::pipe(connection->notification_pipe_fds_));
  const int notification_pipe_read_fd = connection->notification_pipe_fds_[0];
  const int connection_socket = connection->connection_socket();

  bool is_connection_succeeded = false;

  if (-1 == connection_socket) {
    LOG4CXX_ERROR(
        logger_,
        "Socket is invalid for connection session " << connection->session_id());
  } else {
    bool is_device_valid = false;
    //DeviceInfo clientDeviceInfo; TODO

    pthread_mutex_lock (&mDevicesMutex);

    DeviceMap::const_iterator device_it = devices_.find(device_handle);

    const Device* device = 0;
    if (device_it != devices_.end()) {
      device = device_it->second;
      if (device != 0) {
        is_device_valid = true;
        /* TODO
         clientDeviceInfo.mDeviceHandle = deviceHandle;
         clientDeviceInfo.mDeviceType = getDeviceType();
         clientDeviceInfo.mUserFriendlyName = device->mName;
         clientDeviceInfo.mUniqueDeviceId = device->mUniqueDeviceId;
         */
      } else {
        LOG4CXX_ERROR(logger_, "Device " << device_handle << " is not valid");
      }
    } else {
      LOG4CXX_ERROR(logger_, "Device " << device_handle << " does not exist");
    }

    pthread_mutex_unlock(&devices_mutex_);

    if (true == is_device_valid) {
      if (true == is_pipe_created) {
        if (0
            == fcntl(notification_pipe_read_fd, F_SETFL,
                     fcntl(notification_pipe_read_fd, F_GETFL) | O_NONBLOCK)) {
          LOG4CXX_INFO(
              logger_,
              "Connection " << connection->session_id << " to remote device " << device->unique_device_id_ << " established");

          listener_.onConnectDone(this, connection->session_id());
          is_connection_succeeded = true;

          pollfd poll_fds[2];
          poll_fds[0].fd = connection_socket;
          poll_fds[0].events = POLLIN | POLLPRI;
          poll_fds[1].fd = connection->notification_pipe_fds_[0];
          poll_fds[1].events = POLLIN | POLLPRI;

          while (false == connection->terminate_flag_) {
            if (-1
                != poll(poll_fds, sizeof(poll_fds) / sizeof(poll_fds[0]), -1)) {
              if (0 != (poll_fds[0].revents & (POLLERR | POLLHUP | POLLNVAL))) {
                LOG4CXX_INFO(
                    logger_,
                    "Connection " << connection->session_id() << " terminated");

                connection->terminate_flag_ = true;
              } else if (0
                  != (poll_fds[1].revents & (POLLERR | POLLHUP | POLLNVAL))) {
                LOG4CXX_ERROR(
                    logger_,
                    "Notification pipe for connection " << connection->session_id() << " terminated");

                connection->terminate_flag_ = true;
              } else {
                uint8_t buffer[4096];
                ssize_t bytes_read = -1;

                if (0 != poll_fds[0].revents) {
                  do {
                    bytes_read = recv(connection_socket, buffer, sizeof(buffer),
                                      MSG_DONTWAIT);

                    if (bytes_read > 0) {
                      LOG4CXX_INFO(
                          logger_,
                          "Received " << bytes_read << " bytes for connection " << connection->session_id());
                      /* TODO
                       mListener.onFrameReceived(this, ConnectionHandle, buffer,
                       static_cast<size_t>(bytesRead));
                       */
                    } else if (bytes_read < 0) {
                      if ((EAGAIN != errno)&&
                      (EWOULDBLOCK != errno)){
                      LOG4CXX_ERROR_WITH_ERRNO(logger_, "recv() failed for connection " << connection->session_id());

                      connection->terminate_flag_ = true;
                    }
                  }
                  else
                  {
                    LOG4CXX_INFO(logger_, "Connection " << connection->session_id() << " closed by remote peer");

                    connection->terminate_flag_ = true;
                  }
                } while (bytes_read > 0);
              }

                if ((false == connection->terminate_flag_)
                    && (0 != poll_fds[1].revents)) {
                  do {
                    bytes_read = read(notification_pipe_read_fd, buffer,
                                      sizeof(buffer));
                  } while (bytes_read > 0);

                  if ((bytes_read < 0) && (EAGAIN != errno)) {
                    LOG4CXX_ERROR_WITH_ERRNO(
                        logger_,
                        "Failed to clear notification pipe for connection " << connection->session_id());

                    connection->terminate_flag_ = true;
                  }
                  /*
                   tFrameQueue framesToSend;

                   pthread_mutex_lock (&mConnectionsMutex);
                   framesToSend.swap(connection->mFramesToSend);
                   pthread_mutex_unlock(&mConnectionsMutex);

                   for (; false == framesToSend.empty(); framesToSend.pop()) {
                   SFrame * frame = framesToSend.front();
                   ESendStatus frameSendStatus = SendStatusUnknownError;

                   if (0 != frame) {
                   if ((0 != frame->mData) && (0u != frame->mDataSize)) {
                   ssize_t bytesSent = send(connectionSocket, frame->mData,
                   frame->mDataSize, 0);

                   if (static_cast<size_t>(bytesSent) == frame->mDataSize) {
                   frameSendStatus = SendStatusOK;
                   } else {
                   if (bytesSent >= 0) {
                   LOG4CXX_ERROR(
                   logger_,
                   "Sent " << bytesSent << " bytes while " << frame->mDataSize << " had been requested for connection " << ConnectionHandle);
                   } else {
                   LOG4CXX_ERROR_WITH_ERRNO(
                   logger_,
                   "Send failed for connection " << ConnectionHandle);
                   }

                   frameSendStatus = SendStatusFailed;
                   }
                   } else {
                   LOG4CXX_ERROR(
                   logger_,
                   "Frame data is invalid for connection " << ConnectionHandle);

                   frameSendStatus = SendStatusInternalError;
                   }

                   delete frame;
                   } else {
                   LOG4CXX_ERROR(
                   logger_,
                   "Frame data is null for connection " << ConnectionHandle);

                   frameSendStatus = SendStatusInternalError;
                   }

                   mListener.onFrameSendCompleted(this, ConnectionHandle,
                   frame->mUserData,
                   frameSendStatus);
                   }
                   */
                }
              }
            } else {
              LOG4CXX_ERROR_WITH_ERRNO(
                  logger_,
                  "poll() failed for connection " << connection->session_id());

              connection->terminate_flag_ = true;
            }
          }
        } else {
          LOG4CXX_ERROR_WITH_ERRNO(
              logger_,
              "Failed to set O_NONBLOCK for notification pipe for connection " << connection->session_id());
        }
      } else {
        LOG4CXX_ERROR_WITH_ERRNO(
            logger_,
            "Failed to create notification pipe for connection " << connection->session_id());
      }
    } else {
      LOG4CXX_ERROR(
          logger_,
          "Device for connection " << connection->session_id() << " is invalid");
    }

    const int close_status = close(connection_socket);

    if(is_connection_succeeded) {
    if(close_status == 0) {
      listener_.onDisconnectDone(this, connection->session_id());
    } else {
      listener_.onDisconnectFailed(this, connection->session_id(), DisconnectError());
    }
    }
  }

  if(!is_connection_succeeded) {
    listener_.onConnectFailed(this, connection->session_id(), ConnectError());
  }

  if (true == is_pipe_created) {
    pthread_mutex_lock(&connections_mutex_);

    close(connection->notification_pipe_fds_[0]);
    close(connection->notification_pipe_fds_[1]);

    connection->notification_pipe_fds_[0] =
        connection->notification_pipe_fds_[1] = -1;

    pthread_mutex_unlock(&connections_mutex_);
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

void * NsSmartDeviceLink::NsTransportManager::CDeviceAdapter::mainThreadStartRoutine(void * Data)
{
    CDeviceAdapter * deviceAdapter = static_cast<CDeviceAdapter*>(Data);

    if (0 != deviceAdapter)
    {
        deviceAdapter->mainThread();
    }

    return 0;
}

void* DeviceAdapterImpl::connectionThreadStartRoutine(void* data) {
  ConnectionThreadParameters* thread_params =
      static_cast<ConnectionThreadParameters*>(data);

  if (0 != thread_params) {
    DeviceAdapterImpl* deviceAdapter(thread_params->device_adapter);
    delete thread_params;
    if (deviceAdapter) {
      deviceAdapter->connectionThread(thread_params->connection);
    }
  }

  return 0;
}

DeviceAdapter::Error DeviceAdapterImpl::connect(const DeviceHandle device_handle,
                                const ApplicationHandle app_handle,
                                const int session_id) {

  //TODO check if initialized

  pthread_mutex_lock(&devices_mutex_);
  const bool is_device_valid = devices_.end() != devices_.find(device_handle);
  pthread_mutex_unlock(&devices_mutex_);

  if (!is_device_valid) {
    LOG4CXX_ERROR(logger_, "Device handle " << device_handle << " is invalid");
    return BAD_PARAM;
  }

  //TODO check app_handle validity???

  Connection* connection = 0;
  Error error = createConnection(device_handle, app_handle, session_id, &connection);
  if(error != OK) {
    return error;
  }

  error = startConnection(connection);
  if(error != OK) {
    deleteConnection(connection);
    return error;
  }

  return OK;
}

DeviceAdapter::Error DeviceAdapterImpl::disconnectDevice(
    const DeviceHandle device_handle) {
  //TODO check if initialized

  pthread_mutex_lock(&devices_mutex_);
  const bool is_device_valid = devices_.end() != devices_.find(device_handle);
  pthread_mutex_unlock(&devices_mutex_);

  if (!is_device_valid) {
    LOG4CXX_ERROR(logger_, "Device handle " << device_handle << " is invalid");
    return BAD_PARAM;
  }

  Error ret = OK;

  std::vector<Connection*> connections_to_terminate;
  pthread_mutex_lock(&connections_mutex_);
  for (ConnectionMap::iterator it = connections_.begin();
      it != connections_.end(); ++it) {
    Connection* connection = it->second;
    if (connection->device_handle() == device_handle) {
      connections_to_terminate.push_back(connection);
    }
  }
  pthread_mutex_unlock(&connections_mutex_);

  for (std::vector<Connection*>::const_iterator it = connections_to_terminate
      .begin(); it != connections_to_terminate.end(); ++it) {
    Error error = stopConnection(*it);
    if (error != OK) {
      ret = error;
    }
  }

  return ret;
}

} // namespace transport_manager
