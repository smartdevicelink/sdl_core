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
#include <assert.h>

#include "transport_manager/device_adapter_impl.h"
#include "transport_manager/device_adapter_listener.h"

namespace transport_manager {

log4cxx::LoggerPtr DeviceAdapterImpl::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("TransportManager"));

DeviceAdapterImpl::Device::Device(const char* name)
    : name_(name),
      unique_device_id_() {
}

DeviceAdapterImpl::Device::~Device(void) {
}

bool DeviceAdapterImpl::Device::isSameAs(const Device* other_device) const {
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
      terminate_flag_(false) {
}

DeviceAdapterImpl::Connection::~Connection(void) {
}

DeviceAdapterImpl::DeviceAdapterImpl()
    : listeners_(0),
      device_scan_requested_(false),
      device_scan_requested_mutex_(),
      device_scan_requested_cond_(),
      devices_(),
      devices_mutex_(),
      connections_(),
      connections_mutex_(),
      shutdown_flag_(false),
      main_thread_(),
      main_thread_started_(false),
      initialized_(false) {
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
  for (DeviceAdapterListenerList::iterator it = listeners_.begin();
      it != listeners_.end(); ++it) {
    delete (*it);
  }

}

DeviceAdapter::Error DeviceAdapterImpl::init(Configuration* configuration) {
  LOG4CXX_INFO(logger_, "Initializing device adapter");

  initialized_ = true;

  const int thread_start_error = pthread_create(&main_thread_, 0,
                                                &mainThreadStartRoutine, this);

  if (0 == thread_start_error) {
    main_thread_started_ = true;
    LOG4CXX_INFO(logger_, "Device adapter main thread started");
  } else {
    LOG4CXX_ERROR(
        logger_,
        "Device adapter main thread start failed, error code " << thread_start_error);
    return DeviceAdapter::FAIL;
  }

  return DeviceAdapter::OK;
}

void DeviceAdapterImpl::addListener(DeviceAdapterListener *listener) {
  listeners_.push_back(listener);
}
DeviceAdapter::Error DeviceAdapterImpl::searchDevices() {
  //TODO check if initialized and supported

  Error ret = OK;

  pthread_mutex_lock(&device_scan_requested_mutex_);

  if (false == device_scan_requested_) {
    LOG4CXX_INFO(logger_, "Requesting device scan");

    device_scan_requested_ = true;
    pthread_cond_signal(&device_scan_requested_cond_);
  } else {
    ret = BAD_STATE;
    LOG4CXX_INFO(logger_, "Device scan is currently in progress");
  }

  pthread_mutex_unlock(&device_scan_requested_mutex_);

  return ret;
}

void DeviceAdapterImpl::waitForThreadsTermination() {
  shutdown_flag_ = true;

  if (true == main_thread_started_) {
    LOG4CXX_INFO(logger_, "Waiting for device adapter main thread termination");
    pthread_join(main_thread_, 0);
    LOG4CXX_INFO(logger_, "Device adapter main thread terminated");
  }

  std::vector<pthread_t> connection_threads;

  pthread_mutex_lock(&connections_mutex_);

  for (ConnectionMap::iterator it = connections_.begin();
      it != connections_.end(); ++it) {
    SessionID session_id = it->first;
    Connection* connection = it->second;

    connection->set_terminate_flag(true);
    if (-1 != connection->getNotificationPipeWriteFd()) {
      uint8_t c = 0;
      if (1 != write(connection->getNotificationPipeWriteFd(), &c, 1)) {
        LOG4CXX_ERROR_WITH_ERRNO(
            logger_,
            "Failed to wake up connection thread for connection " << session_id);
      }
    }
    connection_threads.push_back(connection->connection_thread());
  }

  pthread_mutex_unlock(&connections_mutex_);

  LOG4CXX_INFO(logger_, "Waiting for connection threads termination");

  for (std::vector<pthread_t>::iterator it = connection_threads.begin();
      it != connection_threads.end(); ++it) {
    pthread_join(*it, 0);
  }

  LOG4CXX_INFO(logger_, "Connection threads terminated");
}

DeviceAdapter::Error DeviceAdapterImpl::createConnection(
    const DeviceHandle device_handle, const ApplicationHandle app_handle,
    const int session_id, Connection** connection) {
  assert(connection != NULL);
  *connection = NULL;

  if (shutdown_flag_) {
    return DeviceAdapter::BAD_STATE;
  }

  DeviceAdapter::Error error = DeviceAdapter::OK;

  pthread_mutex_lock(&connections_mutex_);

  for (ConnectionMap::const_iterator it = connections_.begin();
      it != connections_.end(); ++it) {
    const Connection* existing_connection = it->second;
    if (existing_connection->application_handle() == app_handle
        && existing_connection->device_handle() == device_handle) {
      error = DeviceAdapter::ALREADY_EXIST;
      break;
    }
    if (existing_connection->session_id() == session_id) {
      error = DeviceAdapter::ALREADY_EXIST;
      break;
    }
  }

  if (error == DeviceAdapter::OK) {
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

DeviceAdapter::Error DeviceAdapterImpl::startConnection(
    Connection* connection) {
  assert(connection != 0);

  if (shutdown_flag_) {
    return DeviceAdapter::BAD_STATE;
  }

  bool is_thread_started = false;

  if (!shutdown_flag_) {
    const int error_code = connection->start_connection_thread(this);
    if (0 == error_code) {
      LOG4CXX_INFO(
          logger_,
          "Connection thread started for session " << connection->session_id() << " (device " << connection->device_handle() << ")");

      is_thread_started = true;
    } else {
      LOG4CXX_ERROR(
          logger_,
          "Connection thread start failed for session " << connection->session_id() << " (device " << connection->device_handle() << ")");
    }
  }

  return is_thread_started ? DeviceAdapter::OK : DeviceAdapter::FAIL;
}

DeviceAdapter::Error DeviceAdapterImpl::stopConnection(Connection* connection) {
  assert(connection != 0);
  if (!connection->terminate_flag()) {
    connection->set_terminate_flag(true);
    if (-1 != connection->getNotificationPipeWriteFd()) {
      uint8_t c = 0;
      if (1 != ::write(connection->getNotificationPipeWriteFd(), &c, 1)) {
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

bool DeviceAdapterImpl::waitForDeviceScanRequest(const time_t Timeout) {
  bool deviceScanRequested = false;

  pthread_mutex_lock(&device_scan_requested_mutex_);

  if (false == device_scan_requested_) {
    if (0 == Timeout) {
      if (0
          != pthread_cond_wait(&device_scan_requested_cond_,
                               &device_scan_requested_mutex_)) {
        LOG4CXX_ERROR_WITH_ERRNO(logger_, "pthread_cond_wait failed");
      }
    } else {
      timespec timeoutTime;

      if (0 == clock_gettime(CLOCK_REALTIME, &timeoutTime)) {
        timeoutTime.tv_sec += Timeout;

        while (0
            == pthread_cond_timedwait(&device_scan_requested_cond_,
                                      &device_scan_requested_mutex_,
                                      &timeoutTime)) {
          if (true == device_scan_requested_) {
            break;
          }
        }
      } else {
        LOG4CXX_ERROR_WITH_ERRNO(logger_, "clock_gettime failed");

        sleep(Timeout);
      }
    }
  }

  deviceScanRequested = device_scan_requested_;

  pthread_mutex_unlock(&device_scan_requested_mutex_);

  return deviceScanRequested;
}

void DeviceAdapterImpl::handleCommunication(Connection* connection) {
  assert(connection != 0);

  DeviceHandle device_handle = connection->device_handle();
  const bool is_pipe_created = connection->createNotificationPipe();
  const int notification_pipe_read_fd = connection->getNotificationPipeReadFd();
  const int connection_socket = connection->connection_socket();

  bool is_connection_succeeded = false;

  if (-1 == connection_socket) {
    LOG4CXX_ERROR(
        logger_,
        "Socket is invalid for connection session " << connection->session_id());
  } else {
    bool is_device_valid = false;
    //DeviceInfo clientDeviceInfo; TODO

    pthread_mutex_lock(&devices_mutex_);

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
              "Connection " << connection->session_id() << " to remote device " << device->unique_device_id() << " established");
          for (DeviceAdapterListenerList::iterator it = listeners_.begin();
              it != listeners_.end(); ++it) {
            (*it)->onConnectDone(this, connection->session_id());
          }
          is_connection_succeeded = true;

          pollfd poll_fds[2];
          poll_fds[0].fd = connection_socket;
          poll_fds[0].events = POLLIN | POLLPRI;
          poll_fds[1].fd = notification_pipe_read_fd;
          poll_fds[1].events = POLLIN | POLLPRI;

          while (!connection->terminate_flag()) {
            if (-1
                != poll(poll_fds, sizeof(poll_fds) / sizeof(poll_fds[0]), -1)) {
              if (0 != (poll_fds[0].revents & (POLLERR | POLLHUP | POLLNVAL))) {
                LOG4CXX_INFO(
                    logger_,
                    "Connection " << connection->session_id() << " terminated");

                connection->set_terminate_flag(true);
              } else if (0
                  != (poll_fds[1].revents & (POLLERR | POLLHUP | POLLNVAL))) {
                LOG4CXX_ERROR(
                    logger_,
                    "Notification pipe for connection " << connection->session_id() << " terminated");

                connection->set_terminate_flag(true);
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
                      unsigned char* data = new unsigned char[bytes_read];
                      if (data) {
                        memcpy(data, buffer, bytes_read);
                        RawMessageSptr frame(
                            new protocol_handler::RawMessage(
                                connection->session_id(), 0, data, bytes_read));
                        for (DeviceAdapterListenerList::iterator it = listeners_
                            .begin(); it != listeners_.end(); ++it) {
                          (*it)->onDataReceiveDone(this,
                                                   connection->session_id(),
                                                   frame);
                        }
                      } else {
                        for (DeviceAdapterListenerList::iterator it = listeners_
                            .begin(); it != listeners_.end(); ++it) {
                          (*it)->onDataReceiveFailed(this,
                                                     connection->session_id(),
                                                     DataReceiveError());
                        }
                      }
                    } else if (bytes_read < 0) {
                      if (EAGAIN != errno && EWOULDBLOCK != errno) {
                        LOG4CXX_ERROR_WITH_ERRNO(
                            logger_,
                            "recv() failed for connection " << connection->session_id());

                        connection->set_terminate_flag(true);
                      }
                    } else {
                      LOG4CXX_INFO(
                          logger_,
                          "Connection " << connection->session_id() << " closed by remote peer");

                      connection->set_terminate_flag(true);
                    }
                  } while (bytes_read > 0);
                }

                if ((!connection->terminate_flag())
                    && (0 != poll_fds[1].revents)) {
                  do {
                    bytes_read = read(notification_pipe_read_fd, buffer,
                                      sizeof(buffer));
                  } while (bytes_read > 0);

                  if ((bytes_read < 0) && (EAGAIN != errno)) {
                    LOG4CXX_ERROR_WITH_ERRNO(
                        logger_,
                        "Failed to clear notification pipe for connection " << connection->session_id());

                    connection->set_terminate_flag(true);
                  }

                  FrameQueue frames_to_send;
                  pthread_mutex_lock(&connections_mutex_);
                  std::swap(frames_to_send, connection->frames_to_send());
                  pthread_mutex_unlock(&connections_mutex_);

                  bool frame_sent = false;
                  for (; false == frames_to_send.empty();
                      frames_to_send.pop()) {
                    RawMessageSptr frame = frames_to_send.front();

                    ssize_t bytes_sent = send(connection_socket, frame->data(),
                                              frame->data_size(), 0);

                    if (static_cast<size_t>(bytes_sent) == frame->data_size()) {
                      frame_sent = true;
                    } else {
                      if (bytes_sent >= 0) {
                        //TODO isn't it OK?
                        LOG4CXX_ERROR(
                            logger_,
                            "Sent " << bytes_sent << " bytes while " << frame->data_size() << " had been requested for connection " << connection->session_id());
                      } else {
                        LOG4CXX_ERROR_WITH_ERRNO(
                            logger_,
                            "Send failed for connection " << connection->session_id());
                      }
                    }
                    if (frame_sent) {
                      for (DeviceAdapterListenerList::iterator it = listeners_
                          .begin(); it != listeners_.end(); ++it) {
                        (*it)->onDataSendDone(this, connection->session_id(),
                                              frame);
                      }
                    } else {
                      for (DeviceAdapterListenerList::iterator it = listeners_
                          .begin(); it != listeners_.end(); ++it) {
                        (*it)->onDataSendFailed(this, connection->session_id(),
                                                frame, DataSendError());
                      }
                    }
                  }
                }
              }
            } else {
              LOG4CXX_ERROR_WITH_ERRNO(
                  logger_,
                  "poll() failed for connection " << connection->session_id());

              connection->set_terminate_flag(true);
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

    if (is_connection_succeeded) {
      if (close_status == 0) {
        for (DeviceAdapterListenerList::iterator it = listeners_.begin();
            it != listeners_.end(); ++it) {
          (*it)->onDisconnectDone(this, connection->session_id());
        }
      } else {
        for (DeviceAdapterListenerList::iterator it = listeners_.begin();
            it != listeners_.end(); ++it) {
          (*it)->onDisconnectFailed(this, connection->session_id(),
                                    DisconnectError());
        }
      }
    }
  }

  if (!is_connection_succeeded) {
    for (DeviceAdapterListenerList::iterator it = listeners_.begin();
        it != listeners_.end(); ++it) {
      (*it)->onConnectFailed(this, connection->session_id(), ConnectError());
    }
  }

  if (true == is_pipe_created) {
    pthread_mutex_lock(&connections_mutex_);
    connection->closeNotificationPipe();
    pthread_mutex_unlock(&connections_mutex_);
  }
}

void* DeviceAdapterImpl::mainThreadStartRoutine(void* data) {
  DeviceAdapterImpl* deviceAdapter = static_cast<DeviceAdapterImpl*>(data);

  if (0 != deviceAdapter) {
    deviceAdapter->mainThread();
  }

  return 0;
}

DeviceList DeviceAdapterImpl::getDeviceList() const {
  DeviceList result;

  pthread_mutex_lock(&devices_mutex_);
  for (DeviceMap::const_iterator it = devices_.begin(); it != devices_.end();
      ++it) {
    result.push_back(it->first);
  }
  pthread_mutex_unlock(&devices_mutex_);

  return result;
}

DeviceAdapter::Error DeviceAdapterImpl::sendData(const int session_id,
                                                 const RawMessageSptr data) {
//TODO check if initialized

//TODO check 0
  if (0u == data->data_size()) {
    LOG4CXX_WARN(logger_, "DataSize=0");
  } else if (0 == data->data()) {
    LOG4CXX_WARN(logger_, "Data is null");
  } else {
    pthread_mutex_lock(&connections_mutex_);

    ConnectionMap::iterator connection_it = connections_.find(session_id);

    if (connections_.end() == connection_it) {
      LOG4CXX_ERROR(logger_, "Connection " << session_id << " does not exist");
    } else {
      Connection* connection = connection_it->second;

      connection->frames_to_send().push(data);

      if (-1 != connection->getNotificationPipeWriteFd()) {
        uint8_t c = 0;
        if (1 != write(connection->getNotificationPipeWriteFd(), &c, 1)) {
          LOG4CXX_ERROR_WITH_ERRNO(
              logger_,
              "Failed to wake up connection thread for connection " << session_id);
        }
      }
    }

    pthread_mutex_unlock(&connections_mutex_);
  }

  return OK;
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

DeviceAdapter::Error DeviceAdapterImpl::connect(
    const DeviceHandle device_handle, const ApplicationHandle app_handle,
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
  Error error = createConnection(device_handle, app_handle, session_id,
                                 &connection);
  if (error != OK) {
    return error;
  }

  error = startConnection(connection);
  if (error != OK) {
    deleteConnection(connection);
    return error;
  }

  return OK;
}

DeviceAdapter::Error DeviceAdapterImpl::disconnect(const SessionID session_id) {
//TODO check if initialized and supported

  Error error = OK;
  Connection* connection = 0;
  pthread_mutex_lock(&connections_mutex_);
  ConnectionMap::const_iterator it = connections_.find(session_id);
  if (it != connections_.end()) {
    connection = it->second;
  } else {
    error = BAD_PARAM;
  }
  pthread_mutex_unlock(&connections_mutex_);
  if (error != OK) {
    return error;
  }
  return stopConnection(connection);
}

DeviceAdapter::Error DeviceAdapterImpl::disconnectDevice(
    const DeviceHandle device_handle) {
  //TODO check if initialized and supported

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

bool DeviceAdapterImpl::Connection::createNotificationPipe() {
  return (0 == pipe(notification_pipe_fds_));
}

void DeviceAdapterImpl::Connection::closeNotificationPipe() {
  close(notification_pipe_fds_[0]);
  close(notification_pipe_fds_[1]);

  notification_pipe_fds_[0] = -1;
  notification_pipe_fds_[1] = -1;
}

int DeviceAdapterImpl::Connection::getNotificationPipeReadFd() const {
  return notification_pipe_fds_[0];
}

int DeviceAdapterImpl::Connection::getNotificationPipeWriteFd() const {
  return notification_pipe_fds_[1];
}

int DeviceAdapterImpl::Connection::start_connection_thread(
    DeviceAdapterImpl* device_adapter) {
  ConnectionThreadParameters* thread_params = new ConnectionThreadParameters;
  thread_params->device_adapter = device_adapter;
  thread_params->connection = this;

  const int error_code = pthread_create(&connection_thread_, 0,
                                        &connectionThreadStartRoutine,
                                        static_cast<void*>(thread_params));
  if (error_code != 0)
    delete thread_params;
  return error_code;
}
}  // namespace transport_manager
