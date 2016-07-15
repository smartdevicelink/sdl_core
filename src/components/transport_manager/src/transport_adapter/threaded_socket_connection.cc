/*
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
#include "transport_manager/transport_adapter/threaded_socket_connection.h"

#include "transport_manager/transport_adapter/transport_adapter_controller.h"

#include "utils/logger.h"
#include "utils/threads/thread.h"

SDL_CREATE_LOGGER("TransportManager")

namespace transport_manager {
namespace transport_adapter {
////////////////////////////////////////////////////////////////////////////////
/// SocketConnectionDelegate:
////////////////////////////////////////////////////////////////////////////////

ThreadedSocketConnection::ThreadedSocketConnection(
    const DeviceUID& device_id,
    const ApplicationHandle& app_handle,
    TransportAdapterController* controller)
    : controller_(controller)
    , frames_to_send_()
    , frames_to_send_mutex_()
    , socket_connection_()
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
  SDL_AUTO_TRACE();
  Disconnect();
  thread_->join(threads::Thread::kForceStop);
  delete thread_->delegate();
  threads::DeleteThread(thread_);
}

void ThreadedSocketConnection::Abort() {
  SDL_AUTO_TRACE();
  unexpected_disconnect_ = true;
  terminate_flag_ = true;
}

void ThreadedSocketConnection::SetSocket(
    utils::TcpSocketConnection& socket_connection) {
  socket_connection_ = socket_connection;
  socket_connection_.SetEventHandler(this);
}

TransportAdapter::Error ThreadedSocketConnection::Start() {
  SDL_AUTO_TRACE();
  if (!thread_->start()) {
    SDL_ERROR("thread creation failed");
    return TransportAdapter::FAIL;
  }
  SDL_INFO("thread created");
  return TransportAdapter::OK;
}

void ThreadedSocketConnection::Finalize() {
  SDL_AUTO_TRACE();
  if (unexpected_disconnect_) {
    SDL_DEBUG("unexpected_disconnect");
    controller_->ConnectionAborted(
        device_handle(), application_handle(), CommunicationError());
  } else {
    SDL_DEBUG("not unexpected_disconnect");
    controller_->ConnectionFinished(device_handle(), application_handle());
  }
  socket_connection_.Close();
}

TransportAdapter::Error ThreadedSocketConnection::Notify() {
  SDL_AUTO_TRACE();
  return socket_connection_.Notify() ? TransportAdapter::OK
                                     : TransportAdapter::FAIL;
}

TransportAdapter::Error ThreadedSocketConnection::SendData(
    ::protocol_handler::RawMessagePtr message) {
  SDL_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
  frames_to_send_.push(message);
  return Notify();
}

TransportAdapter::Error ThreadedSocketConnection::Disconnect() {
  SDL_AUTO_TRACE();
  terminate_flag_ = true;
  return Notify();
}

void ThreadedSocketConnection::threadMain() {
  SDL_AUTO_TRACE();
  controller_->ConnectionCreated(this, device_handle(), application_handle());
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

void ThreadedSocketConnection::Transmit() {
  SDL_AUTO_TRACE();
  SDL_DEBUG("Waiting for connection events. " << this);
  socket_connection_.Wait();

  SDL_DEBUG("Waited for connection events: " << this);
}

void ThreadedSocketConnection::Send() {
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
    const bool sent = socket_connection_.Send(
        frame->data() + offset, frame->data_size() - offset, bytes_sent);
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

void ThreadedSocketConnection::OnError(int error) {
  SDL_ERROR("Connection error: " << error);
  Abort();
}

void ThreadedSocketConnection::OnData(const uint8_t* const buffer,
                                      std::size_t buffer_size) {
  protocol_handler::RawMessagePtr frame(
      new protocol_handler::RawMessage(0, 0, buffer, buffer_size));
  controller_->DataReceiveDone(device_handle(), application_handle(), frame);
}

void ThreadedSocketConnection::OnCanWrite() {
  SDL_DEBUG("OnCanWrite event. Trying to send data.");
  Send();
}

void ThreadedSocketConnection::OnClose() {
  SDL_DEBUG("Connection has been closed");
  Abort();
}

////////////////////////////////////////////////////////////////////////////////
/// SocketConnectionDelegate::SocketConnectionDelegate
////////////////////////////////////////////////////////////////////////////////

ThreadedSocketConnection::SocketConnectionDelegate::SocketConnectionDelegate(
    ThreadedSocketConnection* connection)
    : connection_(connection) {}

void ThreadedSocketConnection::SocketConnectionDelegate::threadMain() {
  SDL_AUTO_TRACE();
  DCHECK(connection_);
  connection_->threadMain();
}

void ThreadedSocketConnection::SocketConnectionDelegate::exitThreadMain() {
  SDL_AUTO_TRACE();
  connection_->Abort();
}

}  // namespace transport_adapter
}  // namespace transport_manager
