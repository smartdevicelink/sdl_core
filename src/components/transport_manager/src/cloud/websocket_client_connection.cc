/*
 *
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

#include "transport_manager/cloud/websocket_client_connection.h"

#include "transport_manager/transport_adapter/transport_adapter_controller.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {
CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

WebsocketClientConnection::WebsocketClientConnection(
    const DeviceUID& device_uid,
    const ApplicationHandle& app_handle,
    TransportAdapterController* controller): controller_(controller)
        , resolver_(ioc_)
        , ws_(ioc_)
        , shutdown_(false)
        , thread_delegate_(new LoopThreadDelegate(&message_queue_, this))
        , write_thread_(threads::CreateThread("WS Async Send", thread_delegate_))
        , device_uid_(device_uid)
        , app_handle_(app_handle) {}

WebsocketClientConnection::~WebsocketClientConnection() {
  ioc_.stop();
  if(io_service_thread_.joinable()) {
    io_service_thread_.join();
  }
}

TransportAdapter::Error WebsocketClientConnection::Start() {
  LOG4CXX_AUTO_TRACE(logger_);
  printf("Calling websocket start\n");
  auto const host = "192.168.1.69";
  auto const port = "8080";
  boost::system::error_code ec;
  auto const results = resolver_.resolve(host, port, ec);
  if (ec) {
    std::string str_err = "ErrorMessage1: " + ec.message();
    printf("%s\n", str_err.c_str());
    Shutdown();
    return TransportAdapter::FAIL;
  }
  boost::asio::connect(ws_.next_layer(), results.begin(), results.end(), ec);
  if (ec) {
    std::string str_err = "ErrorMessage2: " + ec.message();
    printf("%s\n", str_err.c_str());
    Shutdown();
    return TransportAdapter::FAIL;
  }
  ws_.handshake(host, "/", ec);
  if (ec) {
    std::string str_err = "ErrorMessage3: " + ec.message();
    printf("%s\n", str_err.c_str());
    Shutdown();
    return TransportAdapter::FAIL;
  }
  ws_.binary(true);
  write_thread_->start(threads::ThreadOptions());
  controller_->ConnectDone(device_uid_, app_handle_);

  // Start async read
  ws_.async_read(
    buffer_,
    std::bind(
      &WebsocketClientConnection::OnRead,
      this,
      std::placeholders::_1,
      std::placeholders::_2));

  // Start IO Service thread. Allows for async reads without blocking.
  io_service_thread_ = std::thread([&](){ ioc_.run(); printf("io_service_thread_ END!!!\n");
   });

  // Start async write thread
  
  printf("End of websockets\n");

  return TransportAdapter::OK;
}

void WebsocketClientConnection::Recv(boost::system::error_code ec) {
  printf("Recv\n");
  if (shutdown_) {
    printf("shutdown_\n");
    return;
  }

  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    printf("%s\n", str_err.c_str());
    LOG4CXX_ERROR(logger_, str_err);
    //shutdown_ = true;
    //ioc_.stop();
    //thread_delegate_->SetShutdown();
    //controller_->deleteController(this);
    Shutdown();
    return;
  }
  printf("calling async read\n");

  ws_.async_read(
    buffer_,
    std::bind(
      &WebsocketClientConnection::OnRead,
      this,
      std::placeholders::_1,
      std::placeholders::_2));
}

void WebsocketClientConnection::OnRead(boost::system::error_code ec,
                            std::size_t bytes_transferred) {
  printf("OnRead\n");
  boost::ignore_unused(bytes_transferred);
  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    printf("%s\n", str_err.c_str());
    LOG4CXX_ERROR(logger_, str_err);
    Shutdown();
    controller_->ConnectionAborted(
        device_uid_, app_handle_, CommunicationError());
    
    printf("return error\n");
    return;
  }

  std::string data_str = boost::beast::buffers_to_string(buffer_.data());
  LOG4CXX_DEBUG(logger_, "Cloud Transport Received: " << data_str);
  printf("%s\n", data_str.c_str());

  ssize_t size = (ssize_t)buffer_.size();  
  const uint8_t* data = boost::asio::buffer_cast<const uint8_t*>(boost::beast::buffers_front(buffer_.data()));

  ::protocol_handler::RawMessagePtr frame(
    new protocol_handler::RawMessage(0, 0, data, size));

  controller_->DataReceiveDone(device_uid_, app_handle_, frame);

  buffer_.consume(buffer_.size());
  Recv(ec);
}

TransportAdapter::Error WebsocketClientConnection::SendData(
    ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);
  printf("Send DATA!!!\n");
  sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
  message_queue_.push(message);
  printf("Data pushed to queue!!!\n");
  return TransportAdapter::OK;
}

TransportAdapter::Error WebsocketClientConnection::Disconnect() {
  LOG4CXX_AUTO_TRACE(logger_);
  Shutdown();
  return TransportAdapter::OK;
}

void WebsocketClientConnection::Shutdown() {
  
  shutdown_ = true;

  if (thread_delegate_) {
    thread_delegate_->SetShutdown();
    write_thread_->join();
    printf("Joined Thread Delegate!!!\n");
    delete thread_delegate_;
  }
  if (buffer_.size()) {
    buffer_.consume(buffer_.size());
  }
  printf("End of shutdown!!!\n");
}

WebsocketClientConnection::LoopThreadDelegate::LoopThreadDelegate(
    MessageQueue<Message, AsyncQueue>* message_queue, WebsocketClientConnection* handler)
    : message_queue_(*message_queue), handler_(*handler), shutdown_(false) {}

void WebsocketClientConnection::LoopThreadDelegate::threadMain() {
  printf("Starting write thread\n");
  while (!message_queue_.IsShuttingDown() && !shutdown_) {
    DrainQueue();
    message_queue_.wait();
  }
  DrainQueue();
}

void WebsocketClientConnection::LoopThreadDelegate::exitThreadMain() {
  shutdown_ = true;
  if (!message_queue_.IsShuttingDown()) {
    message_queue_.Shutdown();
  }
}

void WebsocketClientConnection::LoopThreadDelegate::DrainQueue() {
  while (!message_queue_.empty()) {
    Message message_ptr;
    message_queue_.pop(message_ptr);
    if (!shutdown_) {
      printf("Calling Write!!!\n");
      boost::system::error_code ec;
      handler_.ws_.write(boost::asio::buffer(message_ptr->data(), message_ptr->data_size()));
      if (ec) {
        LOG4CXX_ERROR(logger_, "Error writing to websocket");
        handler_.Shutdown();
        handler_.controller_->DataSendFailed(handler_.device_uid_, handler_.app_handle_, message_ptr, DataSendError());
      }
    }
  }
}

void WebsocketClientConnection::LoopThreadDelegate::SetShutdown() {
  shutdown_ = true;
  if (!message_queue_.IsShuttingDown()) {
    message_queue_.Shutdown();
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager