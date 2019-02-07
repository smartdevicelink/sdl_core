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
#include "transport_manager/cloud/cloud_device.h"

#include "transport_manager/transport_adapter/transport_adapter_controller.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {
CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

WebsocketClientConnection::WebsocketClientConnection(
    const DeviceUID& device_uid,
    const ApplicationHandle& app_handle,
    TransportAdapterController* controller)
    : controller_(controller)
    , ctx_(ssl::context::sslv23_client)
    , resolver_(ioc_)
    , shutdown_(false)
    , thread_delegate_(new LoopThreadDelegate(&message_queue_, this))
    , write_thread_(threads::CreateThread("WS Async Send", thread_delegate_))
    , device_uid_(device_uid)
    , app_handle_(app_handle) {}

WebsocketClientConnection::~WebsocketClientConnection() {
  ioc_.stop();
  if (io_service_thread_.joinable()) {
    io_service_thread_.join();
  }
}

void WebsocketClientConnection::AddCertificateAuthority(
    const std::string cert, boost::system::error_code& ec) {
  ctx_.add_certificate_authority(boost::asio::buffer(cert.data(), cert.size()),
                                 ec);
  if (ec) {
    return;
  }

  wss_->next_layer().set_verify_mode(ssl::verify_peer);
}

TransportAdapter::Error WebsocketClientConnection::Start() {
  LOG4CXX_AUTO_TRACE(logger_);
  DeviceSptr device = controller_->FindDevice(device_uid_);
  CloudDevice* cloud_device = static_cast<CloudDevice*>(device.get());
  CloudWebsocketTransportAdapter* cloud_ta =
      static_cast<CloudWebsocketTransportAdapter*>(controller_);
  cloud_properties = cloud_ta->GetAppCloudTransportConfig(device_uid_);
  auto const host = cloud_device->GetHost();
  auto const port = cloud_device->GetPort();
  boost::system::error_code ec;

  LOG4CXX_DEBUG(logger_, "Cloud app endpoint: " << cloud_properties.endpoint);
  LOG4CXX_DEBUG(logger_,
                "Cloud app certificate: " << cloud_properties.certificate);
  LOG4CXX_DEBUG(
      logger_,
      "Cloud app authentication token: " << cloud_properties.auth_token);
  LOG4CXX_DEBUG(
      logger_,
      "Cloud app transport type: " << cloud_properties.cloud_transport_type);
  LOG4CXX_DEBUG(logger_,
                "Cloud app hybrid app preference: "
                    << cloud_properties.hybrid_app_preference);

  auto const results = resolver_.resolve(host, port, ec);
  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    LOG4CXX_ERROR(logger_, "Could not resolve host/port: " << str_err);
    Shutdown();
    return TransportAdapter::FAIL;
  }

  // Make Connection to host IP Address over TCP
  if (cloud_properties.cloud_transport_type == "WSS") {
    wss_ = std::make_shared<WSS>(ioc_, ctx_);
    boost::asio::connect(
        wss_->next_layer().next_layer(), results.begin(), results.end(), ec);
  } else {
    ws_ = std::make_shared<WS>(ioc_);
    boost::asio::connect(ws_->next_layer(), results.begin(), results.end(), ec);
  }
  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    LOG4CXX_ERROR(logger_,
                  "Could not connect to websocket: " << host << ":" << port);
    LOG4CXX_ERROR(logger_, str_err);
    Shutdown();
    return TransportAdapter::FAIL;
  }

  if (cloud_properties.cloud_transport_type == "WSS") {
    AddCertificateAuthority(cloud_properties.certificate, ec);

    if (ec) {
      std::string str_err = "ErrorMessage: " + ec.message();
      LOG4CXX_ERROR(logger_,
                    "Failed to add certificate authority: "
                        << cloud_properties.certificate);
      LOG4CXX_ERROR(logger_, str_err);
      Shutdown();
      return TransportAdapter::FAIL;
    }

    // Perform SSL Handshake
    wss_->next_layer().handshake(ssl::stream_base::client, ec);

    if (ec) {
      std::string str_err = "ErrorMessage: " + ec.message();
      LOG4CXX_ERROR(logger_,
                    "Could not complete SSL Handshake failed with host/port: "
                        << host << ":" << port);
      LOG4CXX_ERROR(logger_, str_err);
      Shutdown();
      return TransportAdapter::FAIL;
    }
  }

  // Perform websocket handshake
  if (cloud_properties.cloud_transport_type == "WSS") {
    wss_->handshake(host, "/", ec);
  } else {
    ws_->handshake(host, "/", ec);
  }
  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    LOG4CXX_ERROR(logger_,
                  "Could not complete handshake with host/port: " << host << ":"
                                                                  << port);
    LOG4CXX_ERROR(logger_, str_err);
    Shutdown();
    return TransportAdapter::FAIL;
  }

  // Set the binary message write option
  if (cloud_properties.cloud_transport_type == "WSS") {
    wss_->binary(true);
  } else {
    ws_->binary(true);
  }
  write_thread_->start(threads::ThreadOptions());
  controller_->ConnectDone(device_uid_, app_handle_);

  // Start async read
  if (cloud_properties.cloud_transport_type == "WSS") {
    wss_->async_read(buffer_,
                     std::bind(&WebsocketClientConnection::OnRead,
                               this,
                               std::placeholders::_1,
                               std::placeholders::_2));
  } else {
    ws_->async_read(buffer_,
                    std::bind(&WebsocketClientConnection::OnRead,
                              this,
                              std::placeholders::_1,
                              std::placeholders::_2));
  }
  // Start IO Service thread. Allows for async reads without blocking.
  io_service_thread_ = std::thread([&]() {
    ioc_.run();
    LOG4CXX_DEBUG(logger_, "Ending Boost IO Thread");
  });

  LOG4CXX_DEBUG(logger_,
                "Successfully started websocket connection @: " << host << ":"
                                                                << port);

  return TransportAdapter::OK;
}

void WebsocketClientConnection::Recv(boost::system::error_code ec) {
  if (shutdown_) {
    return;
  }

  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    LOG4CXX_ERROR(logger_, str_err);
    Shutdown();
    return;
  }
  if (cloud_properties.cloud_transport_type == "WSS") {
    wss_->async_read(buffer_,
                     std::bind(&WebsocketClientConnection::OnRead,
                               this,
                               std::placeholders::_1,
                               std::placeholders::_2));
  } else {
    ws_->async_read(buffer_,
                    std::bind(&WebsocketClientConnection::OnRead,
                              this,
                              std::placeholders::_1,
                              std::placeholders::_2));
  }
}

void WebsocketClientConnection::OnRead(boost::system::error_code ec,
                                       std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);
  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    LOG4CXX_ERROR(logger_, str_err);
    Shutdown();
    controller_->ConnectionAborted(
        device_uid_, app_handle_, CommunicationError());
    return;
  }

  std::string data_str = boost::beast::buffers_to_string(buffer_.data());
  LOG4CXX_DEBUG(logger_, "Cloud Transport Received: " << data_str);

  ssize_t size = (ssize_t)buffer_.size();
  const uint8_t* data = boost::asio::buffer_cast<const uint8_t*>(
      boost::beast::buffers_front(buffer_.data()));

  ::protocol_handler::RawMessagePtr frame(
      new protocol_handler::RawMessage(0, 0, data, size));

  controller_->DataReceiveDone(device_uid_, app_handle_, frame);

  buffer_.consume(buffer_.size());
  Recv(ec);
}

TransportAdapter::Error WebsocketClientConnection::SendData(
    ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
  message_queue_.push(message);
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
    delete thread_delegate_;
  }
  if (buffer_.size()) {
    buffer_.consume(buffer_.size());
  }
  controller_->DisconnectDone(device_uid_, app_handle_);
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
    if (!shutdown_) {
      boost::system::error_code ec;
      if (handler_.cloud_properties.cloud_transport_type == "WSS") {
        handler_.wss_->write(
            boost::asio::buffer(message_ptr->data(), message_ptr->data_size()));
      } else {
        handler_.ws_->write(
            boost::asio::buffer(message_ptr->data(), message_ptr->data_size()));
      }
      if (ec) {
        LOG4CXX_ERROR(logger_, "Error writing to websocket");
        handler_.Shutdown();
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
