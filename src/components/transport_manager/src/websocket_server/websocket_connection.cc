/*
Copyright (c) 2020 Livio, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

* Neither the name of SmartDeviceLink Consortium, Inc. nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "transport_manager/websocket_server/websocket_connection.h"
#include <unistd.h>
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(wsc_logger_, "WebSocketConnection")

using namespace boost::beast::websocket;

template <>
WebSocketConnection<WebSocketSession<> >::WebSocketConnection(
    const DeviceUID& device_uid,
    const ApplicationHandle& app_handle,
    boost::asio::ip::tcp::socket socket,
    TransportAdapterController* controller)
    : device_uid_(device_uid)
    , app_handle_(app_handle)
    , session_(new WebSocketSession<>(
          std::move(socket),
          [this](Message frame) { DataReceive(frame); },
          [this](Message frame) { DataSendDone(frame); },
          [this](Message frame) { DataSendFailed(frame); },
          [this]() { OnError(); }))
    , controller_(controller)
    , shutdown_(false)
    , thread_delegate_(new LoopThreadDelegate(
          &message_queue_,
          [this](Message frame) { session_->WriteDown(frame); }))
    , thread_(threads::CreateThread("WS Async Send", thread_delegate_)) {
  thread_->start(threads::ThreadOptions());
}

#ifdef ENABLE_SECURITY
template <>
WebSocketConnection<WebSocketSecureSession<> >::WebSocketConnection(
    const DeviceUID& device_uid,
    const ApplicationHandle& app_handle,
    boost::asio::ip::tcp::socket socket,
    ssl::context& ctx,
    TransportAdapterController* controller)
    : device_uid_(device_uid)
    , app_handle_(app_handle)
    , session_(new WebSocketSecureSession<>(
          std::move(socket),
          ctx,
          [this](Message frame) { DataReceive(frame); },
          [this](Message frame) { DataSendDone(frame); },
          [this](Message frame) { DataSendFailed(frame); },
          [this]() { OnError(); }))
    , controller_(controller)
    , shutdown_(false)
    , thread_delegate_(new LoopThreadDelegate(
          &message_queue_,
          [this](Message frame) { session_->WriteDown(frame); }))
    , thread_(threads::CreateThread("WS Async Send", thread_delegate_)) {
  thread_->start(threads::ThreadOptions());
}
template class WebSocketConnection<WebSocketSecureSession<> >;
#endif  // ENABLE_SECURITY

template <typename Session>
WebSocketConnection<Session>::~WebSocketConnection() {
  if (!IsShuttingDown()) {
    Shutdown();
  }
}

template <typename Session>
void WebSocketConnection<Session>::OnError() {
  LOG4CXX_AUTO_TRACE(wsc_logger_);

  if (IsShuttingDown()) {
    LOG4CXX_DEBUG(wsc_logger_, "Session is shutting down...");
    return;
  }

  Message message_ptr;
  while (message_queue_.pop(message_ptr)) {
    DataSendFailed(message_ptr);
  }

  controller_->ConnectionAborted(
      device_uid_, app_handle_, CommunicationError());

  session_->Shutdown();
}

template <typename Session>
TransportAdapter::Error WebSocketConnection<Session>::Disconnect() {
  LOG4CXX_AUTO_TRACE(wsc_logger_);
  if (!IsShuttingDown()) {
    Shutdown();
    controller_->DisconnectDone(device_uid_, app_handle_);
    return TransportAdapter::OK;
  }
  return TransportAdapter::BAD_STATE;
}

template <typename Session>
TransportAdapter::Error WebSocketConnection<Session>::SendData(
    Message message) {
  if (IsShuttingDown()) {
    return TransportAdapter::BAD_STATE;
  }

  message_queue_.push(message);

  return TransportAdapter::OK;
}

template <typename Session>
void WebSocketConnection<Session>::DataReceive(Message frame) {
  controller_->DataReceiveDone(device_uid_, app_handle_, frame);
}

template <typename Session>
void WebSocketConnection<Session>::DataSendDone(Message frame) {
  controller_->DataSendDone(device_uid_, app_handle_, frame);
}

template <typename Session>
void WebSocketConnection<Session>::DataSendFailed(Message frame) {
  controller_->DataSendFailed(device_uid_, app_handle_, frame, DataSendError());
}

template <typename Session>
void WebSocketConnection<Session>::Run() {
  LOG4CXX_AUTO_TRACE(wsc_logger_);
  session_->AsyncAccept();
}

template <typename Session>
void WebSocketConnection<Session>::Shutdown() {
  LOG4CXX_AUTO_TRACE(wsc_logger_);
  shutdown_ = true;
  if (thread_delegate_) {
    session_->Shutdown();
    thread_delegate_->SetShutdown();
    thread_->join();
    delete thread_delegate_;
    thread_delegate_ = nullptr;
    threads::DeleteThread(thread_);
    thread_ = nullptr;
  }
}

template <typename Session>
bool WebSocketConnection<Session>::IsShuttingDown() {
  return shutdown_;
}

template <typename Session>
WebSocketConnection<Session>::LoopThreadDelegate::LoopThreadDelegate(
    MessageQueue<Message, AsyncQueue>* message_queue,
    DataWriteCallback data_write)
    : message_queue_(*message_queue), data_write_(data_write) {}

template <typename Session>
void WebSocketConnection<Session>::LoopThreadDelegate::threadMain() {
  while (!message_queue_.IsShuttingDown()) {
    DrainQueue();
    message_queue_.wait();
  }
  DrainQueue();
}

template <typename Session>
void WebSocketConnection<Session>::LoopThreadDelegate::exitThreadMain() {
  SetShutdown();
}

template <typename Session>
void WebSocketConnection<Session>::LoopThreadDelegate::DrainQueue() {
  Message message_ptr;
  while (!message_queue_.IsShuttingDown() && message_queue_.pop(message_ptr)) {
    data_write_(message_ptr);
  }
}

template <typename Session>
void WebSocketConnection<Session>::LoopThreadDelegate::SetShutdown() {
  if (!message_queue_.IsShuttingDown()) {
    message_queue_.Shutdown();
  }
}

template class WebSocketConnection<WebSocketSession<> >;

}  // namespace transport_adapter
}  // namespace transport_manager
