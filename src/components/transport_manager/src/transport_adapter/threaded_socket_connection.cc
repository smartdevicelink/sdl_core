/*
 * Copyright (c) 2017, Ford Motor Company
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

#include <algorithm>
#include <errno.h>
#include <fcntl.h>
#include <memory.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "utils/logger.h"
#include "utils/threads/thread.h"

#include "transport_manager/transport_adapter/threaded_socket_connection.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {
CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

ThreadedSocketConnection::ThreadedSocketConnection(
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle,
    TransportAdapterController* controller)
    : read_fd_(-1)
    , write_fd_(-1)
    , controller_(controller)
    , frames_to_send_()
    , frames_to_send_mutex_()
    , socket_(-1)
    , terminate_flag_(false)
    , unexpected_disconnect_(false)
    , device_uid_(device_id)
    , app_handle_(app_handle)
    , thread_(NULL) {
  const std::string thread_name = std::string("Socket ") + device_handle();
  thread_ = threads::CreateThread(thread_name.c_str(),
                                  new SocketConnectionDelegate(this));
}

ThreadedSocketConnection::~ThreadedSocketConnection() {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(NULL == thread_);

  if (-1 != read_fd_) {
    close(read_fd_);
  }
  if (-1 != write_fd_) {
    close(write_fd_);
  }
}

void ThreadedSocketConnection::StopAndJoinThread() {
  Disconnect();
  thread_->join();
  delete thread_->delegate();
  threads::DeleteThread(thread_);
  thread_ = NULL;
}

void ThreadedSocketConnection::Abort() {
  LOG4CXX_AUTO_TRACE(logger_);
  unexpected_disconnect_ = true;
  terminate_flag_ = true;
}

TransportAdapter::Error ThreadedSocketConnection::Start() {
  LOG4CXX_AUTO_TRACE(logger_);
  int fds[2];
  const int pipe_ret = pipe(fds);
  if (0 == pipe_ret) {
    LOG4CXX_DEBUG(logger_, "pipe created");
    read_fd_ = fds[0];
    write_fd_ = fds[1];
  } else {
    LOG4CXX_ERROR(logger_, "pipe creation failed");
    return TransportAdapter::FAIL;
  }
  const int fcntl_ret =
      fcntl(read_fd_, F_SETFL, fcntl(read_fd_, F_GETFL) | O_NONBLOCK);
  if (0 != fcntl_ret) {
    LOG4CXX_ERROR(logger_, "fcntl failed");
    return TransportAdapter::FAIL;
  }

  if (!thread_->start()) {
    LOG4CXX_ERROR(logger_, "thread creation failed");
    return TransportAdapter::FAIL;
  }
  LOG4CXX_INFO(logger_, "thread created");
  return TransportAdapter::OK;
}

void ThreadedSocketConnection::Finalize() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (unexpected_disconnect_) {
    LOG4CXX_DEBUG(logger_, "unexpected_disconnect");
    controller_->ConnectionAborted(
        device_handle(), application_handle(), CommunicationError());
  } else {
    LOG4CXX_DEBUG(logger_, "not unexpected_disconnect");
    controller_->ConnectionFinished(device_handle(), application_handle());
  }

  ShutdownAndCloseSocket();
}

TransportAdapter::Error ThreadedSocketConnection::Notify() const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (-1 == write_fd_) {
    LOG4CXX_ERROR_WITH_ERRNO(
        logger_, "Failed to wake up connection thread for connection " << this);
    LOG4CXX_TRACE(logger_, "exit with TransportAdapter::BAD_STATE");
    return TransportAdapter::BAD_STATE;
  }
  uint8_t c = 0;
  if (1 != write(write_fd_, &c, 1)) {
    LOG4CXX_ERROR_WITH_ERRNO(
        logger_, "Failed to wake up connection thread for connection " << this);
    return TransportAdapter::FAIL;
  }
  return TransportAdapter::OK;
}

TransportAdapter::Error ThreadedSocketConnection::SendData(
    ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
  frames_to_send_.push(message);
  return Notify();
}

TransportAdapter::Error ThreadedSocketConnection::Disconnect() {
  LOG4CXX_AUTO_TRACE(logger_);
  terminate_flag_ = true;
  ShutdownAndCloseSocket();
  return Notify();
}

void ThreadedSocketConnection::threadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
  ConnectError* connect_error = NULL;
  if (!Establish(&connect_error)) {
    LOG4CXX_ERROR(logger_, "Connection Establish failed");
    delete connect_error;
  }
  LOG4CXX_DEBUG(logger_, "Connection established");
  controller_->ConnectDone(device_handle(), application_handle());
  while (!terminate_flag_) {
    Transmit();
  }
  LOG4CXX_DEBUG(logger_, "Connection is to finalize");
  Finalize();
  sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
  while (!frames_to_send_.empty()) {
    LOG4CXX_INFO(logger_, "removing message");
    ::protocol_handler::RawMessagePtr message = frames_to_send_.front();
    frames_to_send_.pop();
    controller_->DataSendFailed(
        device_handle(), application_handle(), message, DataSendError());
  }
}

bool ThreadedSocketConnection::IsFramesToSendQueueEmpty() const {
  // Check Frames queue is empty or not
  sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
  return frames_to_send_.empty();
}

void ThreadedSocketConnection::ShutdownAndCloseSocket() {
  LOG4CXX_AUTO_TRACE(logger_);
  const int socket = socket_;
  socket_ = -1;
  if (socket != -1) {
    if (shutdown(socket, SHUT_RDWR) != 0) {
      LOG4CXX_WARN(logger_, "Socket was unable to be shutdowned");
    }
    if (close(socket) != 0) {
      LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to close socket");
    }
  } else {
    LOG4CXX_WARN(logger_, "Socket has been already closed or not created yet");
  }
}

void ThreadedSocketConnection::Transmit() {
  LOG4CXX_AUTO_TRACE(logger_);

  const nfds_t kPollFdsSize = 2;
  pollfd poll_fds[kPollFdsSize];
  poll_fds[0].fd = socket_;

  const bool is_queue_empty_on_poll = IsFramesToSendQueueEmpty();

  poll_fds[0].events =
      POLLIN | POLLPRI | (is_queue_empty_on_poll ? 0 : POLLOUT);
  poll_fds[1].fd = read_fd_;
  poll_fds[1].events = POLLIN | POLLPRI;

  LOG4CXX_DEBUG(logger_, "poll " << this);
  if (-1 == poll(poll_fds, kPollFdsSize, -1)) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "poll failed for connection " << this);
    Abort();
    return;
  }
  LOG4CXX_DEBUG(logger_,
                "poll is ok " << this << " revents0: " << std::hex
                              << poll_fds[0].revents << " revents1:" << std::hex
                              << poll_fds[1].revents);
  // error check
  if (0 != (poll_fds[1].revents & (POLLERR | POLLHUP | POLLNVAL))) {
    LOG4CXX_ERROR(logger_,
                  "Notification pipe for connection " << this << " terminated");
    Abort();
    return;
  }

  if (poll_fds[0].revents & (POLLERR | POLLHUP | POLLNVAL)) {
    LOG4CXX_WARN(logger_, "Connection " << this << " terminated");
    Abort();
    return;
  }

  // clear notifications
  char buffer[256];
  ssize_t bytes_read = -1;
  do {
    bytes_read = read(read_fd_, buffer, sizeof(buffer));
  } while (bytes_read > 0);
  if ((bytes_read < 0) && (EAGAIN != errno)) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to clear notification pipe");
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "poll failed for connection " << this);
    Abort();
    return;
  }

  const bool is_queue_empty = IsFramesToSendQueueEmpty();

  // Send data if possible
  if (!is_queue_empty && (poll_fds[0].revents & POLLOUT)) {
    LOG4CXX_DEBUG(logger_, "frames_to_send_ not empty() ");

    // send data
    const bool send_ok = Send();
    if (!send_ok) {
      LOG4CXX_ERROR(logger_, "Send() failed ");
      Abort();
      return;
    }
  }

  // receive data
  if (poll_fds[0].revents & (POLLIN | POLLPRI)) {
    const bool receive_ok = Receive();
    if (!receive_ok) {
      LOG4CXX_ERROR(logger_, "Receive() failed ");
      Abort();
      return;
    }
  }
}

bool ThreadedSocketConnection::Receive() {
  LOG4CXX_AUTO_TRACE(logger_);
  uint8_t buffer[4096];
  ssize_t bytes_read = -1;

  do {
    bytes_read = recv(socket_, buffer, sizeof(buffer), MSG_DONTWAIT);

    if (bytes_read > 0) {
      LOG4CXX_DEBUG(logger_,
                    "Received " << bytes_read << " bytes for connection "
                                << this);
      ::protocol_handler::RawMessagePtr frame(
          new protocol_handler::RawMessage(0, 0, buffer, bytes_read));
      controller_->DataReceiveDone(
          device_handle(), application_handle(), frame);
    } else if (bytes_read < 0) {
      if (EAGAIN != errno && EWOULDBLOCK != errno) {
        LOG4CXX_ERROR_WITH_ERRNO(logger_,
                                 "recv() failed for connection " << this);
        return false;
      }
    } else {
      LOG4CXX_WARN(logger_, "Connection " << this << " closed by remote peer");
      return false;
    }
  } while (bytes_read > 0);

  return true;
}

bool ThreadedSocketConnection::Send() {
  LOG4CXX_AUTO_TRACE(logger_);
  FrameQueue frames_to_send_local;

  {
    sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
    std::swap(frames_to_send_local, frames_to_send_);
  }

  size_t offset = 0;
  while (!frames_to_send_local.empty()) {
    LOG4CXX_INFO(logger_, "frames_to_send is not empty");
    ::protocol_handler::RawMessagePtr frame = frames_to_send_local.front();
    const ssize_t bytes_sent =
        ::send(socket_, frame->data() + offset, frame->data_size() - offset, 0);

    if (bytes_sent >= 0) {
      LOG4CXX_DEBUG(logger_, "bytes_sent >= 0");
      offset += bytes_sent;
      if (offset == frame->data_size()) {
        frames_to_send_local.pop();
        offset = 0;
        controller_->DataSendDone(device_handle(), application_handle(), frame);
      }
    } else {
      LOG4CXX_DEBUG(logger_, "bytes_sent < 0");
      LOG4CXX_ERROR_WITH_ERRNO(logger_, "Send failed for connection " << this);
      frames_to_send_local.pop();
      offset = 0;
      controller_->DataSendFailed(
          device_handle(), application_handle(), frame, DataSendError());
    }
  }

  return true;
}

ThreadedSocketConnection::SocketConnectionDelegate::SocketConnectionDelegate(
    ThreadedSocketConnection* connection)
    : connection_(connection) {}

void ThreadedSocketConnection::SocketConnectionDelegate::threadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(connection_);
  connection_->threadMain();
}

void ThreadedSocketConnection::SocketConnectionDelegate::exitThreadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
}

}  // namespace transport_adapter
}  // namespace transport_manager
