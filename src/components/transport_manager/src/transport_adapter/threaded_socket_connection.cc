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

#include <errno.h>
#include <fcntl.h>
#include <memory.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>

#include "utils/logger.h"
#include "utils/threads/thread.h"

#include "transport_manager/transport_adapter/threaded_socket_connection.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {
SDL_CREATE_LOG_VARIABLE("TransportManager")

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
    , thread_(nullptr) {
  const std::string thread_name = std::string("Socket ") + device_handle();
  thread_ = threads::CreateThread(thread_name.c_str(),
                                  new SocketConnectionDelegate(this));
}

ThreadedSocketConnection::~ThreadedSocketConnection() {
  SDL_LOG_AUTO_TRACE();
  DCHECK(nullptr == thread_);

  if (-1 != read_fd_) {
    close(read_fd_);
  }
  if (-1 != write_fd_) {
    close(write_fd_);
  }
}

void ThreadedSocketConnection::StopAndJoinThread() {
  Disconnect();
  if (thread_) {
    thread_->Stop(threads::Thread::kThreadSoftStop);
    delete thread_->GetDelegate();
    threads::DeleteThread(thread_);
    thread_ = nullptr;
  }
}

void ThreadedSocketConnection::Abort() {
  SDL_LOG_AUTO_TRACE();
  unexpected_disconnect_ = true;
  terminate_flag_ = true;
}

TransportAdapter::Error ThreadedSocketConnection::Start() {
  SDL_LOG_AUTO_TRACE();
  int fds[2];
  const int pipe_ret = pipe(fds);
  if (0 == pipe_ret) {
    SDL_LOG_DEBUG("pipe created");
    read_fd_ = fds[0];
    write_fd_ = fds[1];
  } else {
    SDL_LOG_ERROR("pipe creation failed");
    return TransportAdapter::FAIL;
  }
  const int fcntl_ret =
      fcntl(read_fd_, F_SETFL, fcntl(read_fd_, F_GETFL) | O_NONBLOCK);
  if (0 != fcntl_ret) {
    SDL_LOG_ERROR("fcntl failed");
    return TransportAdapter::FAIL;
  }

  if (!thread_->Start()) {
    SDL_LOG_ERROR("thread creation failed");
    return TransportAdapter::FAIL;
  }
  SDL_LOG_INFO("thread created");
  return TransportAdapter::OK;
}

void ThreadedSocketConnection::Finalize() {
  SDL_LOG_AUTO_TRACE();
  if (unexpected_disconnect_) {
    SDL_LOG_DEBUG("unexpected_disconnect");
    controller_->ConnectionAborted(
        device_handle(), application_handle(), CommunicationError());
  } else {
    SDL_LOG_DEBUG("not unexpected_disconnect");
    controller_->ConnectionFinished(device_handle(), application_handle());
  }

  ShutdownAndCloseSocket();
}

TransportAdapter::Error ThreadedSocketConnection::Notify() const {
  SDL_LOG_AUTO_TRACE();
  if (-1 == write_fd_) {
    SDL_LOG_ERROR_WITH_ERRNO(
        "Failed to wake up connection thread for connection " << this);
    SDL_LOG_TRACE("exit with TransportAdapter::BAD_STATE");
    return TransportAdapter::BAD_STATE;
  }
  uint8_t c = 0;
  if (1 != write(write_fd_, &c, 1)) {
    SDL_LOG_ERROR_WITH_ERRNO(
        "Failed to wake up connection thread for connection " << this);
    return TransportAdapter::FAIL;
  }
  return TransportAdapter::OK;
}

TransportAdapter::Error ThreadedSocketConnection::SendData(
    ::protocol_handler::RawMessagePtr message) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
  frames_to_send_.push(message);
  return Notify();
}

TransportAdapter::Error ThreadedSocketConnection::Disconnect() {
  SDL_LOG_AUTO_TRACE();
  terminate_flag_ = true;
  ShutdownAndCloseSocket();
  return Notify();
}

void ThreadedSocketConnection::Terminate() {
  SDL_LOG_AUTO_TRACE();
  StopAndJoinThread();
}

void ThreadedSocketConnection::threadMain() {
  SDL_LOG_AUTO_TRACE();
  ConnectError* connect_error = nullptr;
  if (!Establish(&connect_error)) {
    SDL_LOG_ERROR("Connection Establish failed");
    delete connect_error;
    Abort();
  } else {
    SDL_LOG_DEBUG("Connection established");
    controller_->ConnectDone(device_handle(), application_handle());
  }

  while (!terminate_flag_) {
    Transmit();
  }
  SDL_LOG_DEBUG("Connection is to finalize");
  Finalize();
  sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
  while (!frames_to_send_.empty()) {
    SDL_LOG_INFO("removing message");
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
  SDL_LOG_AUTO_TRACE();
  const int socket = socket_;
  socket_ = -1;
  if (socket != -1) {
    if (shutdown(socket, SHUT_RDWR) != 0) {
      SDL_LOG_WARN("Socket was unable to be shutdowned");
    }
    if (close(socket) != 0) {
      SDL_LOG_ERROR_WITH_ERRNO("Failed to close socket");
    }
  } else {
    SDL_LOG_WARN("Socket has been already closed or not created yet");
  }
}

void ThreadedSocketConnection::Transmit() {
  SDL_LOG_AUTO_TRACE();

  const nfds_t kPollFdsSize = 2;
  pollfd poll_fds[kPollFdsSize];
  poll_fds[0].fd = socket_;

  const bool is_queue_empty_on_poll = IsFramesToSendQueueEmpty();

  poll_fds[0].events =
      POLLIN | POLLPRI | (is_queue_empty_on_poll ? 0 : POLLOUT);
  poll_fds[1].fd = read_fd_;
  poll_fds[1].events = POLLIN | POLLPRI;

  SDL_LOG_TRACE("poll " << this);
  if (-1 == poll(poll_fds, kPollFdsSize, -1)) {
    SDL_LOG_ERROR_WITH_ERRNO("poll failed for connection " << this);
    Abort();
    return;
  }
  SDL_LOG_TRACE("poll is ok " << this << " revents0: " << std::hex
                              << poll_fds[0].revents << " revents1:" << std::hex
                              << poll_fds[1].revents);
  // error check
  if (0 != (poll_fds[1].revents & (POLLERR | POLLHUP | POLLNVAL))) {
    SDL_LOG_ERROR("Notification pipe for connection " << this << " terminated");
    Abort();
    return;
  }

  if (poll_fds[0].revents & (POLLERR | POLLHUP | POLLNVAL)) {
    SDL_LOG_WARN("Connection " << this << " terminated");
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
    SDL_LOG_ERROR_WITH_ERRNO("Failed to clear notification pipe");
    SDL_LOG_ERROR_WITH_ERRNO("poll failed for connection " << this);
    Abort();
    return;
  }

  const bool is_queue_empty = IsFramesToSendQueueEmpty();

  // Send data if possible
  if (!is_queue_empty && (poll_fds[0].revents & POLLOUT)) {
    SDL_LOG_DEBUG("frames_to_send_ not empty() ");

    // send data
    const bool send_ok = Send();
    if (!send_ok) {
      SDL_LOG_ERROR("Send() failed ");
      Abort();
      return;
    }
  }

  // receive data
  if (poll_fds[0].revents & (POLLIN | POLLPRI)) {
    const bool receive_ok = Receive();
    if (!receive_ok) {
      SDL_LOG_ERROR("Receive() failed ");
      Abort();
      return;
    }
  }
}

bool ThreadedSocketConnection::Receive() {
  SDL_LOG_AUTO_TRACE();
  uint8_t buffer[4096];
  ssize_t bytes_read = -1;

  do {
    bytes_read = recv(socket_, buffer, sizeof(buffer), MSG_DONTWAIT);

    if (bytes_read > 0) {
      SDL_LOG_TRACE("Received " << bytes_read << " bytes for connection "
                                << this);
      ::protocol_handler::RawMessagePtr frame(
          new protocol_handler::RawMessage(0, 0, buffer, bytes_read, false));
      controller_->DataReceiveDone(
          device_handle(), application_handle(), frame);
    } else if (bytes_read < 0) {
      if (EAGAIN != errno && EWOULDBLOCK != errno) {
        SDL_LOG_ERROR_WITH_ERRNO("recv() failed for connection " << this);
        return false;
      }
    } else {
      SDL_LOG_WARN("Connection " << this << " closed by remote peer");
      return false;
    }
  } while (bytes_read > 0);

  return true;
}

bool ThreadedSocketConnection::Send() {
  SDL_LOG_AUTO_TRACE();
  FrameQueue frames_to_send_local;

  {
    sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
    std::swap(frames_to_send_local, frames_to_send_);
  }

  size_t offset = 0;
  while (!frames_to_send_local.empty()) {
    SDL_LOG_INFO("frames_to_send is not empty");
    ::protocol_handler::RawMessagePtr frame = frames_to_send_local.front();
    const ssize_t bytes_sent =
        ::send(socket_, frame->data() + offset, frame->data_size() - offset, 0);

    if (bytes_sent >= 0) {
      SDL_LOG_DEBUG("bytes_sent >= 0");
      offset += bytes_sent;
      if (offset == frame->data_size()) {
        frames_to_send_local.pop();
        offset = 0;
        controller_->DataSendDone(device_handle(), application_handle(), frame);
      }
    } else {
      SDL_LOG_DEBUG("bytes_sent < 0");
      SDL_LOG_ERROR_WITH_ERRNO("Send failed for connection " << this);
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
  SDL_LOG_AUTO_TRACE();
  DCHECK(connection_);
  connection_->threadMain();
}

void ThreadedSocketConnection::SocketConnectionDelegate::exitThreadMain() {
  SDL_LOG_AUTO_TRACE();
}

}  // namespace transport_adapter
}  // namespace transport_manager
