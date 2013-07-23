/**
 * \file device_adapter_socket_communication.cc
 * \brief Implementation of classes responsible for communication over sockets
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

#include "transport_manager/device_adapter_socket_communication.h"
#include "transport_manager/device_adapter_listener.h"

namespace transport_manager {

namespace device_adapter {

ThreadedSocketConnection::ThreadedSocketConnection(
    const DeviceHandle& device_handle, const ApplicationHandle& app_handle,
    DeviceAdapterController* controller)
    : controller_(controller),
      frames_to_send_(),
      notification_pipe_read_fd_(-1),
      notification_pipe_write_fd_(-1),
      terminate_flag_(false),
      unexpected_disconnect_(false),
      socket_(-1),
      device_handle_(device_handle),
      app_handle_(app_handle),
      thread_() {
  pthread_mutex_init(&frames_to_send_mutex_, 0);
}

ThreadedSocketConnection::~ThreadedSocketConnection() {
  terminate_flag_ = true;
  notify();
  pthread_join(thread_, 0);
  if (-1 != notification_pipe_read_fd_)
    close(notification_pipe_read_fd_);
  if (-1 != notification_pipe_write_fd_)
    close(notification_pipe_write_fd_);
  pthread_mutex_destroy(&frames_to_send_mutex_);
}

void ThreadedSocketConnection::abort() {
  unexpected_disconnect_ = true;
  terminate_flag_ = true;
}

void* startThreadedSocketConnection(void* v) {
  ThreadedSocketConnection* connection =
      static_cast<ThreadedSocketConnection*>(v);
  connection->thread();
  return 0;
}

DeviceAdapter::Error ThreadedSocketConnection::start() {
  int notification_pipe_fds[2];
  const int pipe_ret = pipe(notification_pipe_fds);
  if (0 == pipe_ret) {
    notification_pipe_read_fd_ = notification_pipe_fds[0];
    notification_pipe_write_fd_ = notification_pipe_fds[1];
  } else {
    return DeviceAdapter::FAIL;
  }

  const int fcntl_ret = fcntl(
      notification_pipe_read_fd_, F_SETFL,
      fcntl(notification_pipe_read_fd_, F_GETFL) | O_NONBLOCK);
  if (0 != fcntl_ret) {
    return DeviceAdapter::FAIL;
  }

  if (0 == pthread_create(&thread_, 0, &startThreadedSocketConnection, this))
    return DeviceAdapter::OK;
  else
    return DeviceAdapter::FAIL;
}

void ThreadedSocketConnection::finalise() {
  if (unexpected_disconnect_) {
    controller_->connectionAborted(device_handle(), application_handle(),
                                   CommunicationError());
  } else {
    controller_->connectionFinished(device_handle(), application_handle());
  }
  close(socket_);
}

DeviceAdapter::Error ThreadedSocketConnection::notify() const {
  if (-1 != notification_pipe_write_fd_) {
    uint8_t c = 0;
    if (1 == write(notification_pipe_write_fd_, &c, 1)) {
      return DeviceAdapter::OK;
    } else {
      LOG4CXX_ERROR_WITH_ERRNO(
          logger_,
          "Failed to wake up connection thread for connection " << this);
    }
  }
  return DeviceAdapter::FAIL;
}

DeviceAdapter::Error ThreadedSocketConnection::sendData(
    RawMessageSptr message) {
  pthread_mutex_lock(&frames_to_send_mutex_);
  frames_to_send_.push(message);
  pthread_mutex_unlock(&frames_to_send_mutex_);
  return notify();
}

DeviceAdapter::Error ThreadedSocketConnection::disconnect() {
  terminate_flag_ = true;
  return notify();
}

void ThreadedSocketConnection::thread() {
  controller_->connectionCreated(this, device_handle_, app_handle_);
  ConnectError* connect_error;
  if (establish(&connect_error)) {
    controller_->connectDone(device_handle(), application_handle());
    while (!terminate_flag_)
      transmit();
    finalise();
    while(!frames_to_send_.empty()) {
      RawMessageSptr message = frames_to_send_.front();
      frames_to_send_.pop();
      controller_->dataSendFailed(device_handle(), application_handle(), message, DataSendError());
    }
    controller_->disconnectDone(device_handle(), application_handle());
  } else {
    controller_->connectFailed(device_handle(), application_handle(),
                               *connect_error);
    delete connect_error;
  }
}

void ThreadedSocketConnection::transmit() {
  pollfd poll_fds[2];
  poll_fds[0].fd = socket_;
  poll_fds[0].events = POLLIN | POLLPRI;
  poll_fds[1].fd = notification_pipe_read_fd_;
  poll_fds[1].events = POLLIN | POLLPRI;

  const int poll_status = poll(poll_fds, sizeof(poll_fds) / sizeof(poll_fds[0]),
                               -1);
  if (poll_status == -1) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "poll() failed for connection " << this);
    abort();
    return;
  }

  if (0 != (poll_fds[0].revents & (POLLERR | POLLHUP | POLLNVAL))) {
    LOG4CXX_INFO(logger_, "Connection " << this << " terminated");
    abort();
    return;
  }

  if (0 != (poll_fds[1].revents & (POLLERR | POLLHUP | POLLNVAL))) {
    LOG4CXX_ERROR(logger_,
                  "Notification pipe for connection " << this << " terminated");
    abort();
    return;
  }

  if (0 != poll_fds[0].revents) {
    const bool receive_ok = receive();
    if (!receive_ok) {
      abort();
      return;
    }
  }

  if (0 != poll_fds[1].revents) {
    const bool clear_notification_pipe_ok = clearNotificationPipe();
    if (!clear_notification_pipe_ok) {
      abort();
      return;
    }

    const bool send_ok = send();
    if (!send_ok) {
      abort();
      return;
    }
  }
}

bool ThreadedSocketConnection::receive() {
  uint8_t buffer[4096];
  ssize_t bytes_read = -1;

  do {
    bytes_read = recv(socket_, buffer, sizeof(buffer), MSG_DONTWAIT);

    if (bytes_read > 0) {
      LOG4CXX_INFO(
          logger_,
          "Received " << bytes_read << " bytes for connection " << this);
      unsigned char* data = new unsigned char[bytes_read];
      if (data) {
        memcpy(data, buffer, bytes_read);
        RawMessageSptr frame(
            new protocol_handler::RawMessage(0, 0, data, bytes_read));
        controller_->dataReceiveDone(device_handle(), application_handle(),
                                     frame);
      } else {
        controller_->dataReceiveFailed(device_handle(), application_handle(),
                                       DataReceiveError());
      }
    } else if (bytes_read < 0) {
      if (EAGAIN != errno && EWOULDBLOCK != errno) {
        LOG4CXX_ERROR_WITH_ERRNO(logger_,
                                 "recv() failed for connection " << this);

        return false;
      }
    } else {
      LOG4CXX_INFO(logger_, "Connection " << this << " closed by remote peer");
      return false;
    }
  } while (bytes_read > 0);
  return true;
}

bool ThreadedSocketConnection::clearNotificationPipe() {
  uint8_t buffer[4096];
  ssize_t bytes_read = -1;
  do {
    bytes_read = read(notification_pipe_read_fd_, buffer, sizeof(buffer));
  } while (bytes_read > 0);

  if ((bytes_read < 0) && (EAGAIN != errno)) {
    LOG4CXX_ERROR_WITH_ERRNO(
        logger_, "Failed to clear notification pipe for connection " << this);
    return false;
  }
  return true;
}

bool ThreadedSocketConnection::send() {
  FrameQueue frames_to_send;
  pthread_mutex_lock(&frames_to_send_mutex_);
  std::swap(frames_to_send, frames_to_send_);
  pthread_mutex_unlock(&frames_to_send_mutex_);

  bool frame_sent = false;
  for (; false == frames_to_send.empty(); frames_to_send.pop()) {
    RawMessageSptr frame = frames_to_send.front();

    const ssize_t bytes_sent = ::send(socket_, frame->data(),
                                      frame->data_size(), 0);

    if (static_cast<size_t>(bytes_sent) == frame->data_size()) {
      frame_sent = true;
    } else {
      if (bytes_sent >= 0) {
        //TODO isn't it OK?
        LOG4CXX_ERROR(
            logger_,
            "Sent " << bytes_sent << " bytes while " << frame->data_size() << " had been requested for connection " << this);
      } else {
        LOG4CXX_ERROR_WITH_ERRNO(logger_, "Send failed for connection " << this);
      }
    }
    if (frame_sent) {
      controller_->dataSendDone(device_handle(), application_handle(), frame);
    } else {
      controller_->dataSendFailed(device_handle(), application_handle(), frame, DataSendError());
    }
  }
  return true;
}

}  // namespace

}  // namespace

