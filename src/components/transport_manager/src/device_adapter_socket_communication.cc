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
    const DeviceHandle device_handle, const ApplicationHandle app_handle,
    const SessionID session_id)
    : Connection(device_handle, app_handle, session_id),
      thread_(new Thread<Sptr>),
      notification_pipe_read_fd_(-1),
      notification_pipe_write_fd_(-1),
      socket_(-1) {
}

ThreadedSocketConnection::~ThreadedSocketConnection() {
}

void ThreadedSocketConnection::start(int socket) {
  int notification_pipe_fds[2];
  const int pipe_ret = pipe(notification_pipe_fds);
  if(0 == pipe_ret) {
    notification_pipe_read_fd_ = notification_pipe_fds[0];
    notification_pipe_write_fd_ = notification_pipe_fds[1];
  }
  socket_ = socket;
  thread_ = new Thread<Sptr>;
  if(thread_)
    thread_->start();
}

void ThreadedSocketConnection::notify() const {
  if (-1 != notification_pipe_write_fd_) {
    uint8_t c = 0;
    if (1 != write(notification_pipe_write_fd_, &c, 1)) {
      LOG4CXX_ERROR_WITH_ERRNO(
          logger_,
          "Failed to wake up connection thread for connection " << session_id());
    }
  }
}

int ThreadedSocketConnection::notification_pipe_read_fd() const {
  return notification_pipe_read_fd_;
}

SocketDataTransmitter::SocketDataTransmitter() {
}

Error SocketDataTransmitter::init() {
  return OK;
}

void SocketDataTransmitter::terminate() {
}

void SocketDataTransmitter::registerConnection(ConnectionSptr connection) {
  Thread<ThreadedSocketConnection::Sptr>* thread =
      static_cast<ThreadedSocketConnection*>(&*connection)->thread();  // FIXME
  if (thread)
    thread->setRepeatTask(this);
}

void SocketDataTransmitter::unregisterConnection(ConnectionSptr connection) {
  static_cast<ThreadedSocketConnection*>(connection)->notify();
}

void SocketDataTransmitter::notifyDataAvailable(ConnectionSptr connection) {
  static_cast<ThreadedSocketConnection*>(connection)->notify();
}

void SocketDataTransmitter::perform(ThreadedSocketConnection::Sptr connection) {
  pollfd poll_fds[2];
  poll_fds[0].fd = connection->socket();
  poll_fds[0].events = POLLIN | POLLPRI;
  poll_fds[1].fd = connection->notification_pipe_read_fd();
  poll_fds[1].events = POLLIN | POLLPRI;

  const int poll_status = poll(poll_fds, sizeof(poll_fds) / sizeof(poll_fds[0]),
                               -1);
  if (poll_status == -1) {
    LOG4CXX_ERROR_WITH_ERRNO(
        logger_, "poll() failed for connection " << connection->session_id());
    getController()->endConnection(connection);
    return;
  }

  if (0 != (poll_fds[0].revents & (POLLERR | POLLHUP | POLLNVAL))) {
    LOG4CXX_INFO(logger_,
                 "Connection " << connection->session_id() << " terminated");
    getController()->endConnection(connection);
    return;
  }

  if (0 != (poll_fds[1].revents & (POLLERR | POLLHUP | POLLNVAL))) {
    LOG4CXX_ERROR(
        logger_,
        "Notification pipe for connection " << connection->session_id() << " terminated");
    getController()->endConnection(connection);
    return;
  }

  if (0 != poll_fds[0].revents) {
    const bool receive_ok = receive(connection);
    if (!receive_ok) {
      getController()->endConnection(connection);
      return;
    }
  }

  if (0 != poll_fds[1].revents) {
    const bool clear_notification_pipe_ok = clearNotificationPipe(connection);
    if (!clear_notification_pipe_ok) {
      getController()->endConnection(connection);
      return;
    }

    const bool send_ok = send(connection);
    if (!send_ok) {
      getController()->endConnection(connection);
      return;
    }
  }
}

bool SocketDataTransmitter::receive(ThreadedSocketConnection::Sptr connection) {
  uint8_t buffer[4096];
  ssize_t bytes_read = -1;

  do {
    bytes_read = recv(connection->socket(), buffer, sizeof(buffer),
                      MSG_DONTWAIT);

    if (bytes_read > 0) {
      LOG4CXX_INFO(
          logger_,
          "Received " << bytes_read << " bytes for connection " << connection->session_id());
      unsigned char* data = new unsigned char[bytes_read];
      if (data) {
        memcpy(data, buffer, bytes_read);
        RawMessageSptr frame(
            new protocol_handler::RawMessage(connection->session_id(), 0, data,
                                             bytes_read));
        getController()->getListener()->onDataReceiveDone(
            getDeviceAdapter(), connection->session_id(), frame);
      } else {
        getController()->getListener()->onDataReceiveFailed(
            getDeviceAdapter(), connection->session_id(), DataReceiveError());
      }
    } else if (bytes_read < 0) {
      if (EAGAIN != errno && EWOULDBLOCK != errno) {
        LOG4CXX_ERROR_WITH_ERRNO(
            logger_,
            "recv() failed for connection " << connection->session_id());

        return false;
      }
    } else {
      LOG4CXX_INFO(
          logger_,
          "Connection " << connection->session_id() << " closed by remote peer");
      return false;
    }
  } while (bytes_read > 0);
  return true;
}

bool SocketDataTransmitter::clearNotificationPipe(
    ThreadedSocketConnection::Sptr connection) {
  uint8_t buffer[4096];
  ssize_t bytes_read = -1;
  do {
    bytes_read = read(connection->notification_pipe_read_fd(), buffer,
                      sizeof(buffer));
  } while (bytes_read > 0);

  if ((bytes_read < 0) && (EAGAIN != errno)) {
    LOG4CXX_ERROR_WITH_ERRNO(
        logger_,
        "Failed to clear notification pipe for connection " << connection->session_id());
    return false;
  }
  return true;
}

bool SocketDataTransmitter::send(ThreadedSocketConnection::Sptr connection) {
  FrameQueue frames_to_send = connection->frames_to_send();
  bool frame_sent = false;
  for (; false == frames_to_send.empty(); frames_to_send.pop()) {
    RawMessageSptr frame = frames_to_send.front();

    const ssize_t bytes_sent = ::send(connection->socket(), frame->data(),
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
            logger_, "Send failed for connection " << connection->session_id());
      }
    }
    if (frame_sent) {
      getController()->getListener()->onDataSendDone(getDeviceAdapter(),
                                                     connection->session_id(),
                                                     frame);
    } else {
      getController()->getListener()->onDataSendFailed(getDeviceAdapter(),
                                                       connection->session_id(),
                                                       frame, DataSendError());
    }
  }
  return true;
}

}  // namespace

}  // namespace

