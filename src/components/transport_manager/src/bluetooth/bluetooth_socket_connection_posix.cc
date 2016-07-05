/*
 *
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

#include "transport_manager/bluetooth/bluetooth_socket_connection.h"

#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>
#include <poll.h>
#include <fcntl.h>

#include "transport_manager/bluetooth/bluetooth_device.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

#include "utils/logger.h"
#include "utils/threads/thread.h"

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOGGER("TransportManager")

namespace {

bool CloseSocket(int& socket) {
  SDL_AUTO_TRACE();
  if (0 == socket) {
    SDL_DEBUG("Socket " << socket << " is not valid. Skip closing.");
    return true;
  }
  errno = 0;
  if (-1 != close(socket)) {
    SDL_WARN_WITH_ERRNO("Failed recvto close socket " << socket);
    return false;
  }
  socket = 0;
  return true;
}

}  // namespace

BluetoothSocketConnection::BluetoothSocketConnection(
    const DeviceUID& device_uid,
    const ApplicationHandle& app_handle,
    TransportAdapterController* controller)
    : controller_(controller)
    , frames_to_send_()
    , frames_to_send_mutex_()
    , terminate_flag_(false)
    , unexpected_disconnect_(false)
    , device_uid_(device_uid)
    , app_handle_(app_handle)
    , rfcomm_socket_(0)
    , read_fd_(0)
    , write_fd_(0) {
  const std::string thread_name = std::string("Socket ") + device_handle();
  thread_ = threads::CreateThread(thread_name.c_str(),
                                  new BthConnectionDelegate(this));
}

BluetoothSocketConnection::~BluetoothSocketConnection() {
  SDL_AUTO_TRACE();
  Disconnect();
  thread_->join();
  delete thread_->delegate();
  threads::DeleteThread(thread_);
}

TransportAdapter::Error BluetoothSocketConnection::Start() {
  SDL_AUTO_TRACE();
  if (!thread_->start()) {
    SDL_ERROR("thread creation failed");
    return TransportAdapter::FAIL;
  }
  SDL_INFO("thread created");
  return TransportAdapter::OK;
}

void BluetoothSocketConnection::threadMain() {
  SDL_AUTO_TRACE();
  controller_->ConnectionCreated(this, device_uid_, app_handle_);
  ConnectError* connect_error = NULL;
  if (!Establish(&connect_error)) {
    SDL_ERROR("Connection Establish failed");
    delete connect_error;
  }
  SDL_DEBUG("Connection established");
  controller_->ConnectDone(device_handle(), application_handle());
  while (!terminate_flag_) {
    Transmit();
  }
  SDL_DEBUG("Connection is to finalize");
  Finalize();
  sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
  while (!frames_to_send_.empty()) {
    SDL_INFO("removing message");
    ::protocol_handler::RawMessagePtr message = frames_to_send_.front();
    frames_to_send_.pop();
    controller_->DataSendFailed(
        device_handle(), application_handle(), message, DataSendError());
  }
}

void BluetoothSocketConnection::Transmit() {
  SDL_AUTO_TRACE();
  SDL_DEBUG("Waiting for connection events. " << this);

  const nfds_t kPollFdsSize = 2;
  pollfd poll_fds[kPollFdsSize];
  poll_fds[0].fd = rfcomm_socket_;
  // TODO: Fix data race. frames_to_send_ should be protected
  poll_fds[0].events = POLLIN | POLLPRI;
  poll_fds[1].fd = read_fd_;
  poll_fds[1].events = POLLIN | POLLPRI;
  errno = 0;
  if (-1 == poll(poll_fds, kPollFdsSize, -1)) {
    SDL_ERROR_WITH_ERRNO("poll failed for the socket " << rfcomm_socket_);
    OnError(errno);
    return;
  }
  SDL_DEBUG("poll is ok for the socket "
            << rfcomm_socket_ << " revents0: " << std::hex
            << poll_fds[0].revents << " revents1:" << std::hex
            << poll_fds[1].revents);
  // error check
  if (0 != (poll_fds[1].revents & (POLLERR | POLLHUP | POLLNVAL))) {
    SDL_ERROR("Notification pipe for socket " << rfcomm_socket_
                                              << " terminated.");
    OnError(errno);
    return;
  }
  if (poll_fds[0].revents & (POLLERR | POLLHUP | POLLNVAL)) {
    SDL_DEBUG("Socket " << rfcomm_socket_ << " has terminated.");
    OnClose();
    return;
  }

  // clear notifications in the notification pipe
  char buffer[256];
  ssize_t bytes_read = -1;
  do {
    errno = 0;
    bytes_read = read(read_fd_, buffer, sizeof(buffer));
  } while (bytes_read > 0);
  if ((bytes_read < 0) && (EAGAIN != errno)) {
    SDL_ERROR_WITH_ERRNO(
        "Failed to clear notification pipe. Poll failed for socket "
        << rfcomm_socket_);
    OnError(errno);
    return;
  }

  // send data if possible
  if (poll_fds[1].revents & (POLLIN | POLLPRI)) {
    OnWrite();
    return;
  }

  // receive data
  if (poll_fds[0].revents & (POLLIN | POLLPRI)) {
    OnRead();
  }
}

void BluetoothSocketConnection::Send() {
  SDL_AUTO_TRACE();
  SDL_DEBUG("Trying to send data if available");
  FrameQueue frames_to_send;
  {
    sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
    std::swap(frames_to_send, frames_to_send_);
  }

  size_t offset = 0;
  while (!frames_to_send.empty()) {
    ::protocol_handler::RawMessagePtr frame = frames_to_send.front();
    std::size_t bytes_sent = 0u;
    const bool sent =
        Send(reinterpret_cast<const char*>(frame->data() + offset),
             frame->data_size() - offset,
             bytes_sent);
    if (!sent) {
      SDL_ERROR("Send failed for connection " << this);
      frames_to_send.pop();
      offset = 0;
      controller_->DataSendFailed(
          device_handle(), application_handle(), frame, DataSendError());
      Abort();
      return;
    }
    offset += bytes_sent;
    if (offset == frame->data_size()) {
      frames_to_send.pop();
      offset = 0;
      controller_->DataSendDone(device_handle(), application_handle(), frame);
    }
  }
}

bool BluetoothSocketConnection::Send(const char* const buffer,
                                     std::size_t size,
                                     std::size_t& bytes_written) {
  bytes_written = 0u;
  if (!IsValid()) {
    SDL_ERROR("Failed to send data socket is not valid");
    return false;
  }
  const int flags = MSG_NOSIGNAL;
  errno = 0;
  int written = send(rfcomm_socket_, buffer, size, flags);
  int socket_error = errno;
  if (-1 == written) {
    if (EAGAIN != socket_error && EWOULDBLOCK != socket_error) {
      SDL_ERROR_WITH_ERRNO("Failed to send data.");
      return false;
    } else {
      return true;
    }
  }
  // Lets double chek written because we have signed to unsigned conversion
  DCHECK(written >= 0);
  bytes_written = static_cast<std::size_t>(written);
  SDL_DEBUG("Sent " << written << " bytes to socket " << rfcomm_socket_);
  return true;
}

bool BluetoothSocketConnection::Close() {
  if (!IsValid()) {
    SDL_DEBUG("Connection is not valid. Nothing to close.");
    return true;
  }
  SDL_DEBUG("Closing connection.");

  // Possibly we're waiting on Wait. We have to interrupt this.
  Notify();

  if (-1 != read_fd_) {
    close(read_fd_);
  }
  if (-1 != write_fd_) {
    close(write_fd_);
  }

  return CloseSocket(rfcomm_socket_);
}

bool BluetoothSocketConnection::IsValid() const {
  return rfcomm_socket_ != 0;
}

void BluetoothSocketConnection::OnError(int error) {
  SDL_ERROR("Connection error: " << error);
  Abort();
}

void BluetoothSocketConnection::OnData(const uint8_t* const buffer,
                                       std::size_t buffer_size) {
  protocol_handler::RawMessagePtr frame(
      new protocol_handler::RawMessage(0, 0, buffer, buffer_size));
  controller_->DataReceiveDone(device_handle(), application_handle(), frame);
}

void BluetoothSocketConnection::OnCanWrite() {
  SDL_DEBUG("OnCanWrite event. Trying to send data.");
  Send();
}

bool BluetoothSocketConnection::Establish(ConnectError** error) {
  SDL_AUTO_TRACE();
  SDL_DEBUG("error: " << error);
  DeviceSptr device = controller()->FindDevice(device_handle());

  if (!device.valid()) {
    SDL_ERROR("Device is not valid.");
    return false;
  }

  BluetoothDevice* bluetooth_device =
      static_cast<BluetoothDevice*>(device.get());

  uint8_t rfcomm_channel = 0;
  if (!bluetooth_device->GetRfcommChannel(application_handle(),
                                          &rfcomm_channel)) {
    SDL_DEBUG("Application " << application_handle() << " not found");
    *error = new ConnectError();
    SDL_TRACE("exit with FALSE");
    return false;
  }

  SDL_DEBUG("Connecting to "
            << "bluetooth device on port"
            << ":" << rfcomm_channel);

  sockaddr_rc remoteSocketAddress = {0};
  remoteSocketAddress.rc_family = AF_BLUETOOTH;
  remoteSocketAddress.rc_channel = rfcomm_channel;
  bacpy(&remoteSocketAddress.rc_bdaddr, &bluetooth_device->address());

  int rfcomm_socket;

  uint8_t attempts = 4;
  int connect_status = 0;
  SDL_DEBUG("start rfcomm Connect attempts");
  do {
    SDL_DEBUG("Attempts left: " << attempts);
    errno = 0;
    rfcomm_socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    if (-1 == rfcomm_socket) {
      SDL_ERROR_WITH_ERRNO("Failed to create RFCOMM socket for device.");
      Close();
      SDL_TRACE("exit with FALSE");
      return false;
    }
    errno = 0;
    connect_status =
        ::connect(rfcomm_socket,
                  reinterpret_cast<sockaddr*>(&remoteSocketAddress),
                  sizeof(remoteSocketAddress));
    if (0 == connect_status) {
      SDL_DEBUG("rfcomm Connect ok");
      break;
    }
    if (errno != 111 && errno != 104) {
      SDL_ERROR_WITH_ERRNO("rfcomm connect error.");
    }
    if (errno) {
      SDL_ERROR_WITH_ERRNO("rfcomm connect error.");
      CloseSocket(rfcomm_socket);
    }
    sleep(2);
  } while (--attempts > 0);

  SDL_INFO("rfcomm Connect attempts finished");
  if (0 != connect_status) {
    SDL_DEBUG("Failed to Connect to remote bluetooth device for session "
              << this);
    CloseSocket(rfcomm_socket);
    SDL_TRACE("exit with FALSE");
    return false;
  }

  if (!CreateNotifictionPipes()) {
    Close();
    return false;
  }

  rfcomm_socket_ = rfcomm_socket;
  SDL_TRACE("exit with TRUE");
  return true;
}

TransportAdapter::Error BluetoothSocketConnection::Notify() {
  if (-1 == write_fd_) {
    SDL_ERROR(
        "File descriptior for writing is invalid. "
        "Failed to wake up connection thread for connection "
        << this);
    return TransportAdapter::FAIL;
  }
  uint8_t buffer = 0;
  errno = 0;
  if (1 != write(write_fd_, &buffer, 1)) {
    SDL_ERROR_WITH_ERRNO("Failed to wake up connection thread for connection "
                         << this);
    return TransportAdapter::FAIL;
  }
  return TransportAdapter::OK;
}

void BluetoothSocketConnection::Abort() {
  SDL_AUTO_TRACE();
  unexpected_disconnect_ = true;
  terminate_flag_ = true;
}

void BluetoothSocketConnection::Finalize() {
  SDL_AUTO_TRACE();
  if (unexpected_disconnect_) {
    SDL_DEBUG("unexpected_disconnect");
    controller_->ConnectionAborted(
        device_handle(), application_handle(), CommunicationError());
  } else {
    SDL_DEBUG("not unexpected_disconnect");
    controller_->ConnectionFinished(device_handle(), application_handle());
  }
  CloseSocket(rfcomm_socket_);
}

TransportAdapter::Error BluetoothSocketConnection::SendData(
    ::protocol_handler::RawMessagePtr message) {
  SDL_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
  frames_to_send_.push(message);
  return Notify();
}

TransportAdapter::Error BluetoothSocketConnection::Disconnect() {
  SDL_AUTO_TRACE();
  terminate_flag_ = true;
  return Notify();
}

TransportAdapterController* BluetoothSocketConnection::controller() {
  return controller_;
}

DeviceUID BluetoothSocketConnection::device_handle() const {
  return device_uid_;
}

ApplicationHandle BluetoothSocketConnection::application_handle() const {
  return app_handle_;
}

void BluetoothSocketConnection::OnRead() {
  SDL_AUTO_TRACE();
  const std::size_t buffer_size = 4096u;
  char buffer[buffer_size];
  int bytes_read = -1;

  do {
    errno = 0;
    bytes_read = recv(rfcomm_socket_, buffer, sizeof(buffer), MSG_DONTWAIT);
    if (bytes_read > 0) {
      SDL_DEBUG("Received " << bytes_read << " bytes from socket "
                            << rfcomm_socket_);
      uint8_t* casted_buffer = reinterpret_cast<uint8_t*>(buffer);
      OnData(casted_buffer, bytes_read);
    } else if (bytes_read < 0) {
      int socket_error = errno;
      if (EAGAIN != socket_error && EWOULDBLOCK != socket_error) {
        SDL_ERROR_WITH_ERRNO("recv() failed for connection " << rfcomm_socket_);
        OnError(socket_error);
        return;
      }
    } else {
      SDL_WARN("Socket " << rfcomm_socket_ << " closed by remote peer");
      OnError(errno);
      return;
    }
  } while (bytes_read > 0);
}

void BluetoothSocketConnection::OnWrite() {
  OnCanWrite();
}

bool BluetoothSocketConnection::CreateNotifictionPipes() {
  int fds[2];
  errno = 0;
  const int result = pipe(fds);

  if (0 != result) {
    SDL_ERROR_WITH_ERRNO("Pipe creation failed.");
    return false;
  }

  SDL_DEBUG("Pipe has been created.");
  read_fd_ = fds[0];
  write_fd_ = fds[1];

  errno = 0;
  const int fcntl_ret =
      fcntl(read_fd_, F_SETFL, fcntl(read_fd_, F_GETFL) | O_NONBLOCK);
  if (0 != fcntl_ret) {
    SDL_ERROR_WITH_ERRNO("fcntl failed.");
    return false;
  }
  return true;
}

void BluetoothSocketConnection::OnClose() {
  Abort();
}

////////////////////////////////////////////////////////////////////////////////
/// BthConnectionDelegate::BthConnectionDelegate
////////////////////////////////////////////////////////////////////////////////

BluetoothSocketConnection::BthConnectionDelegate::BthConnectionDelegate(
    BluetoothSocketConnection* connection)
    : connection_(connection) {}

void BluetoothSocketConnection::BthConnectionDelegate::threadMain() {
  SDL_AUTO_TRACE();
  DCHECK(connection_);
  connection_->threadMain();
}

void BluetoothSocketConnection::BthConnectionDelegate::exitThreadMain() {
  SDL_AUTO_TRACE();
}

}  // namespace transport_adapter
}  // namespace transport_manager
