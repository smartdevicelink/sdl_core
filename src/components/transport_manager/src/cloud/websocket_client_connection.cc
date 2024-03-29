/*
 *
 * Copyright (c) 2018, Livio
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
#include "transport_manager/cloud/cloud_device.h"

#include "transport_manager/transport_adapter/transport_adapter_controller.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {
SDL_CREATE_LOG_VARIABLE("TransportManager")

WebsocketClientConnection::WebsocketClientConnection(
    const DeviceUID& device_uid,
    const ApplicationHandle& app_handle,
    TransportAdapterController* controller)
    : controller_(controller)
    , resolver_(ioc_)
    , ws_(std::make_shared<WS>(ioc_))
#ifdef ENABLE_SECURITY
    , ctx_(ssl::context::tlsv12_client)
    , wss_(std::make_shared<WSS>(ioc_, ctx_))
#endif  // ENABLE_SECURITY
    , shutdown_(false)
    , thread_delegate_(new LoopThreadDelegate(&message_queue_, this))
    , write_thread_(
          threads::CreateThread("Client WS Async Send", thread_delegate_))
    , device_uid_(device_uid)
    , app_handle_(app_handle)
    , io_pool_(1) {
#ifdef ENABLE_SECURITY
  wss_ciphers_ =
      "ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-"
      "CHACHA20-POLY1305:ECDHE-RSA-CHACHA20-POLY1305:ECDHE-ECDSA-AES128-GCM-"
      "SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-"
      "AES256-SHA384:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA256";
  SSL_CTX_set_cipher_list(ctx_.native_handle(), wss_ciphers_);
#endif
}

WebsocketClientConnection::~WebsocketClientConnection() {
  ioc_.stop();
  io_pool_.join();
}

#ifdef ENABLE_SECURITY
void WebsocketClientConnection::AddCertificateAuthority(
    const std::string cert, boost::system::error_code& ec) {
  ctx_.add_certificate_authority(boost::asio::buffer(cert.data(), cert.size()),
                                 ec);
  if (ec) {
    return;
  }

  wss_->next_layer().set_verify_mode(ssl::verify_peer);
}
#endif  // ENABLE_SECURITY

TransportAdapter::Error WebsocketClientConnection::Start() {
  SDL_LOG_AUTO_TRACE();
  DeviceSptr device = controller_->FindDevice(device_uid_);
  CloudDevice* cloud_device = static_cast<CloudDevice*>(device.get());
  CloudWebsocketTransportAdapter* cloud_ta =
      static_cast<CloudWebsocketTransportAdapter*>(controller_);
  cloud_properties = cloud_ta->GetAppCloudTransportConfig(device_uid_);
  auto const host = cloud_device->GetHost();
  auto const port = cloud_device->GetPort();
  boost::system::error_code ec;

  SDL_LOG_DEBUG("Cloud app endpoint: " << cloud_properties.endpoint);
  SDL_LOG_DEBUG("Cloud app certificate: " << cloud_properties.certificate);
  SDL_LOG_DEBUG(
      "Cloud app authentication token: " << cloud_properties.auth_token);
  SDL_LOG_DEBUG(
      "Cloud app transport type: " << cloud_properties.cloud_transport_type);
  SDL_LOG_DEBUG("Cloud app hybrid app preference: "
                << cloud_properties.hybrid_app_preference);

  auto const results = resolver_.resolve(host, port, ec);
  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    SDL_LOG_ERROR("Could not resolve host/port: " << str_err);
    return TransportAdapter::FAIL;
  }

  // Make Connection to host IP Address over TCP
  if (cloud_properties.cloud_transport_type == "WS") {
    boost::asio::connect(ws_->next_layer(), results.begin(), results.end(), ec);
  }
#ifdef ENABLE_SECURITY
  else if (cloud_properties.cloud_transport_type == "WSS") {
    boost::asio::connect(
        wss_->next_layer().next_layer(), results.begin(), results.end(), ec);
  }
#endif  // ENABLE_SECURITY

  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    SDL_LOG_ERROR("Could not connect to websocket: " << host << ":" << port);
    SDL_LOG_ERROR(str_err);
    return TransportAdapter::FAIL;
  }

#ifdef ENABLE_SECURITY
  if (cloud_properties.cloud_transport_type == "WSS") {
    AddCertificateAuthority(cloud_properties.certificate, ec);

    if (ec) {
      std::string str_err = "ErrorMessage: " + ec.message();
      SDL_LOG_ERROR("Failed to add certificate authority: "
                    << cloud_properties.certificate);
      SDL_LOG_ERROR(str_err);
      ResetWebsocketStream(cloud_properties.cloud_transport_type);
      return TransportAdapter::FAIL;
    }

    // Perform SSL Handshake
    wss_->next_layer().handshake(ssl::stream_base::client, ec);

    if (ec) {
      std::string str_err = "ErrorMessage: " + ec.message();
      SDL_LOG_ERROR("Could not complete SSL Handshake failed with host/port: "
                    << host << ":" << port);
      SDL_LOG_ERROR(str_err);
      ResetWebsocketStream(cloud_properties.cloud_transport_type);
      return TransportAdapter::FAIL;
    }
  }
#endif  // ENABLE_SECURITY

  // Perform websocket handshake
  if (cloud_properties.cloud_transport_type == "WS") {
    ws_->handshake(host, cloud_device->GetTarget(), ec);
  }
#ifdef ENABLE_SECURITY
  else if (cloud_properties.cloud_transport_type == "WSS") {
    wss_->handshake(host, cloud_device->GetTarget(), ec);
  }
#endif  // ENABLE_SECURITY
  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    SDL_LOG_ERROR("Could not complete handshake with host/port: " << host << ":"
                                                                  << port);
    SDL_LOG_ERROR(str_err);
    ResetWebsocketStream(cloud_properties.cloud_transport_type);
    return TransportAdapter::FAIL;
  }

  // Set the binary message write option
  if (cloud_properties.cloud_transport_type == "WS") {
    ws_->binary(true);
  }
#ifdef ENABLE_SECURITY
  else if (cloud_properties.cloud_transport_type == "WSS") {
    wss_->binary(true);
  }
#endif  // ENABLE_SECURITY
  write_thread_->Start(threads::ThreadOptions());
  controller_->ConnectDone(device_uid_, app_handle_);

  // Start async read
  if (cloud_properties.cloud_transport_type == "WS") {
    ws_->async_read(buffer_,
                    std::bind(&WebsocketClientConnection::OnRead,
                              this,
                              std::placeholders::_1,
                              std::placeholders::_2));
  }
#ifdef ENABLE_SECURITY
  else if (cloud_properties.cloud_transport_type == "WSS") {
    wss_->async_read(buffer_,
                     std::bind(&WebsocketClientConnection::OnRead,
                               this,
                               std::placeholders::_1,
                               std::placeholders::_2));
  }
#endif  // ENABLE_SECURITY

  boost::asio::post(io_pool_, [&]() { ioc_.run(); });

  SDL_LOG_DEBUG("Successfully started websocket connection @: " << host << ":"
                                                                << port);
  return TransportAdapter::OK;
}

void WebsocketClientConnection::Recv(boost::system::error_code ec) {
  if (shutdown_) {
    return;
  }

  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    SDL_LOG_ERROR(str_err);
    Shutdown();
    return;
  }
  if (cloud_properties.cloud_transport_type == "WS") {
    ws_->async_read(buffer_,
                    std::bind(&WebsocketClientConnection::OnRead,
                              this,
                              std::placeholders::_1,
                              std::placeholders::_2));
  }
#ifdef ENABLE_SECURITY
  else if (cloud_properties.cloud_transport_type == "WSS") {
    wss_->async_read(buffer_,
                     std::bind(&WebsocketClientConnection::OnRead,
                               this,
                               std::placeholders::_1,
                               std::placeholders::_2));
  }
#endif  // ENABLE_SECURITY
}

void WebsocketClientConnection::OnRead(boost::system::error_code ec,
                                       std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);
  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    SDL_LOG_ERROR(str_err);
    if (cloud_properties.cloud_transport_type == "WS") {
      boost::beast::get_lowest_layer(*ws_).close();
    }
#ifdef ENABLE_SECURITY
    else if (cloud_properties.cloud_transport_type == "WSS") {
      boost::beast::get_lowest_layer(*wss_).close();
    }
#endif  // ENABLE_SECURITY

    ioc_.stop();
    Shutdown();
    return;
  }

  ssize_t size = (ssize_t)buffer_.size();
  const uint8_t* data = boost::asio::buffer_cast<const uint8_t*>(
      boost::beast::buffers_front(buffer_.data()));

  ::protocol_handler::RawMessagePtr frame(
      new protocol_handler::RawMessage(0, 0, data, size, false));

  controller_->DataReceiveDone(device_uid_, app_handle_, frame);

  buffer_.consume(buffer_.size());
  Recv(ec);
}

TransportAdapter::Error WebsocketClientConnection::SendData(
    ::protocol_handler::RawMessagePtr message) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
  message_queue_.push(message);
  return TransportAdapter::OK;
}

TransportAdapter::Error WebsocketClientConnection::Disconnect() {
  SDL_LOG_AUTO_TRACE();
  Shutdown();
  return TransportAdapter::OK;
}

void WebsocketClientConnection::Shutdown() {
  SDL_LOG_AUTO_TRACE();
  shutdown_ = true;

  if (thread_delegate_) {
    thread_delegate_->SetShutdown();

    if (write_thread_->IsRunning()) {
      write_thread_->Stop(threads::Thread::kThreadSoftStop);
    }
    delete thread_delegate_;
    thread_delegate_ = NULL;
    threads::DeleteThread(write_thread_);
    write_thread_ = NULL;
  }
  if (buffer_.size()) {
    buffer_.consume(buffer_.size());
  }
  controller_->DisconnectDone(device_uid_, app_handle_);
}

void WebsocketClientConnection::ResetWebsocketStream(
    std::string cloud_transport_type) {
  if (cloud_transport_type == "WS") {
    ws_.reset();
    ws_ = std::make_shared<WS>(ioc_);
  }
#ifdef ENABLE_SECURITY
  else if (cloud_transport_type == "WSS") {
    ctx_ = ssl::context(ssl::context::tlsv12_client);
    SSL_CTX_set_cipher_list(ctx_.native_handle(), wss_ciphers_);
    wss_.reset();
    wss_ = std::make_shared<WSS>(ioc_, ctx_);
  }
#endif  // ENABLE_SECURITY
}

WebsocketClientConnection::LoopThreadDelegate::LoopThreadDelegate(
    MessageQueue<Message, AsyncQueue>* message_queue,
    WebsocketClientConnection* handler)
    : message_queue_(*message_queue), handler_(*handler), shutdown_(false) {}

void WebsocketClientConnection::LoopThreadDelegate::threadMain() {
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
    if (!message_ptr) {
      SDL_LOG_ERROR("Invalid message in message queue");
      continue;
    }
    if (!shutdown_) {
      boost::system::error_code ec;
      if (handler_.cloud_properties.cloud_transport_type == "WS") {
        handler_.ws_->write(
            boost::asio::buffer(message_ptr->data(), message_ptr->data_size()));
      }
#ifdef ENABLE_SECURITY
      else if (handler_.cloud_properties.cloud_transport_type == "WSS") {
        handler_.wss_->write(
            boost::asio::buffer(message_ptr->data(), message_ptr->data_size()));
      }
#endif  // ENABLE_SECURITY
      if (ec) {
        SDL_LOG_ERROR("Error writing to websocket");
        handler_.controller_->DataSendFailed(handler_.device_uid_,
                                             handler_.app_handle_,
                                             message_ptr,
                                             DataSendError());
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
