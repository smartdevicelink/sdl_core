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
CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

WebsocketClientConnection::WebsocketClientConnection(
    const DeviceUID& device_uid,
    const ApplicationHandle& app_handle,
    TransportAdapterController* controller)
    : controller_(controller)
    , ctx_(ssl::context::sslv23_client)
    , resolver_(ioc_)
    , ws_{ioc_, ctx_}
    , shutdown_(false)
    , thread_delegate_(new LoopThreadDelegate(&message_queue_, this))
    , write_thread_(threads::CreateThread("WS Async Send", thread_delegate_))
    , device_uid_(device_uid)
    , app_handle_(app_handle) {

      LOG4CXX_DEBUG(logger_, "CLOUD_DEBUG_WEBSOCKETCLIENTCONNECTION");
      // LOG4CXX_DEBUG(logger_, controller);
      
      // CloudWebsocketTransportAdapter* cta = static_cast<CloudWebsocketTransportAdapter*>(controller);
      // LOG4CXX_DEBUG(logger_, cta);
      
      // CloudAppTransportConfig temp = cta->GetCloudTransportConfiguration();


      // LOG4CXX_DEBUG(logger_, "Printing CloudAppTransportConfig: " << temp.size());
      // for(auto it = temp.cbegin(); it != temp.cend(); it++){
      //   std::string txt = "DEVICE: " + it->first + "\n";
      //   txt += "ENDPOINT: " + it->second.endpoint + "\n";
      //   txt += "CERTIFICATE: " + it->second.certificate + "\n";
      //   txt += "AUTH_TOKEN: " + it->second.auth_token + "\n";
      //   LOG4CXX_DEBUG(logger_, txt); 

      // }      
    }



WebsocketClientConnection::~WebsocketClientConnection() {
  ioc_.stop();
  if (io_service_thread_.joinable()) {
    io_service_thread_.join();
  }
}

bool WebsocketClientConnection::AddCertificateAuthority(const std::string cert, boost::system::error_code& ec){
  if(cert == "not_specified"){
    ws_.next_layer().set_verify_mode(ssl::verify_none);
    return false;
  }

  ctx_.add_certificate_authority(boost::asio::buffer(cert.data(), cert.size()), ec);
  if(ec){
    return false;
  }

  ws_.next_layer().set_verify_mode(ssl::verify_peer);
  if(ec){
    return false;
  }

  return true;
}

TransportAdapter::Error WebsocketClientConnection::Start() {
  LOG4CXX_AUTO_TRACE(logger_);
  DeviceSptr device = controller_->FindDevice(device_uid_);
  CloudDevice* cloud_device = static_cast<CloudDevice*>(device.get());
  auto const host = cloud_device->GetHost();
  auto const port = cloud_device->GetPort();
  boost::system::error_code ec;
  auto const results = resolver_.resolve(host, port, ec);

  LOG4CXX_DEBUG(logger_, "CLOUD_CONN_START");  

  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    LOG4CXX_ERROR(logger_, "Could not resolve host/port: " << str_err);
    Shutdown();
    return TransportAdapter::FAIL;
  }

  LOG4CXX_DEBUG(logger_, "CLOUD_CONN_RESOLVE");    

  // Make Connection to host IP Address over TCP
  boost::asio::connect(ws_.next_layer().next_layer(), results.begin(), results.end(), ec);
  
  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    LOG4CXX_ERROR(logger_,
                  "Could not connect to websocket: " << host << ":" << port);
    LOG4CXX_ERROR(logger_, str_err);
    Shutdown();
    return TransportAdapter::FAIL;
  }

  LOG4CXX_DEBUG(logger_, "CLOUD_CONN_TCP");  
  
  std::string cert = "-----BEGIN CERTIFICATE-----\nMIIDBjCCAe6gAwIBAgIJAOsbkrgCZxgtMA0GCSqGSIb3DQEBCwUAMBgxFjAUBgNV\nBAMMDTE5Mi4xNjguMS4xMjgwHhcNMTgxMTI4MTgxNjQwWhcNMjExMTI3MTgxNjQw\nWjAYMRYwFAYDVQQDDA0xOTIuMTY4LjEuMTI4MIIBIjANBgkqhkiG9w0BAQEFAAOC\nAQ8AMIIBCgKCAQEAlVrHDi+XdO9fNMcknqs2Hn0AQjKQZAq0juy8r7gDqkzHw9zY\nFnfssIUzP7R6F1/80ulyjwUN6G+SI7phbivr2gmSdfkYiJVdwBKf611srIrNF/Eh\nllt/2sjwZNyTai4pzZv9/svix5nIVCHdKZD6wsxCFOdNhVJGBd9uQ4Pk1hQoW/jj\nsUF/NBUa49k31/IQiqQ6T1xQvSkEUYd1kstS7utO2V0Z9rHH4/+4HNyPMKipkCi2\n/7WuvQGDyHTnNUFmEANn4X06iQAVon9L8IVRcGwtgsWJ0fuVGK5POtU4m37Q35MW\n3RWF3OzyP/6PxRX5ljQFmkwGkqzHrNzOZN+zMQIDAQABo1MwUTAdBgNVHQ4EFgQU\nc1BN7ZNXq+OA5hT+vq1NOMoUrR8wHwYDVR0jBBgwFoAUc1BN7ZNXq+OA5hT+vq1N\nOMoUrR8wDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEANBnwEOwM\nRynBmaRBBVZBxcWsaWQ6h7XETLSU+Pqrv2cm+g8pS1pFwc69OS5ER730UcCADlLv\n7Lu61m9kOxHuDzeQR5ofkVbFx1swUqSZt7GZvopVtwFeto4zUfAyVfCS6n6MGSvN\nWlathEg3mlXlAjtX1qEg1JzJ5DjowiPpJ2FZMsakJrF3Ju0D0Dskg0wgqZLKcHYC\ndV7bm2SpC7kfqeuSXIC8hbfolJBaysFRjIXXWwDhtW2i4KRDq4b54EVON2af6RD7\nbsOiY4Qt4Aw5UOE+DN/dbkXXNHKDWN/VN1MI9wGJiriXf4BWSJWgrrsXHkpzqmMj\nrHR2ik90MT9pkQ==\n-----END CERTIFICATE-----";
  std::string fake_cert = "-----BEGIN CERTIFICATE-----\nMIIDBjCCAe6gAwIBAgIJAOTd19xp9ISxMA0GCSqGSIb3DQEBCwUAMBgxFjAUBgNV\nBAMMDTE5Mi4xNjguMS4xMjgwHhcNMTgxMTI4MTM1NjI0WhcNMjExMTI3MTM1NjI0\nWjAYMRYwFAYDVQQDDA0xOTIuMTY4LjEuMTI4MIIBIjANBgkqhkiG9w0BAQEFAAOC\nAQ8AMIIBCgKCAQEAlVrHDi+XdO9fNMcknqs2Hn0AQjKQZAq0juy8r7gDqkzHw9zY\nFnfssIUzP7R6F1/80ulyjwUN6G+SI7phbivr2gmSdfkYiJVdwBKf611srIrNF/Eh\nllt/2sjwZNyTai4pzZv9/svix5nIVCHdKZD6wsxCFOdNhVJGBd9uQ4Pk1hQoW/jj\nsUF/NBUa49k31/IQiqQ6T1xQvSkEUYd1kstS7utO2V0Z9rHH4/+4HNyPMKipkCi2\n/7WuvQGDyHTnNUFmEANn4X06iQAVon9L8IVRcGwtgsWJ0fuVGK5POtU4m37Q35MW\n3RWF3OzyP/6PxRX5ljQFmkwGkqzHrNzOZN+zMQIDAQABo1MwUTAdBgNVHQ4EFgQU\nc1BN7ZNXq+OA5hT+vq1NOMoUrR8wHwYDVR0jBBgwFoAUc1BN7ZNXq+OA5hT+vq1N\nOMoUrR8wDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAUQ8MwCmb\nDrwCNGaUdGNXoiAWrltPE2buzpq8OCVxXBSqSumAe1SkjyKROBbevM7/Cl4V3qgj\nEHjUXNrzFR9WTCOOjJ9W0wiO25FDBguU5VCkuhvfL6lCgVuJRRi9LtiY2hT5fPdM\nrMvS0/iUV3RXcKNyIGOXH1OcOim+PVph2wj0x/N1Nmhqkh+2LtFHhsE/Zgygndla\nsbyRTNQ3bnzGUwMl8jPa7KSeggtfmjIFc4VXEgrOzKHhJO2C96FmVMo97QqKQvq+\nWjPfzHnI3hhr1GPPMHFQQZG5j4a708Qo9fZcKJxUvm0O7kdFFOqcMyo086cu1YUP\nGy5s8MqShPUp1Q==\n-----END CERTIFICATE-----";
  std::string no_cert = "not_specified";
  std::string invalid_cert = "-----BEGIN CERTIFICATE-----\nWjAYMRYwFAYDVQQDDA0xOTIuMTY4LjEuMTI4MIIBIjANBgkqhkiG9w0BAQEFAAOC\nAQ8AMIIBCgKCAQEAlVrHDi+XdO9fNMcknqs2Hn0AQjKQZAq0juy8r7gDqkzHw9zY\nFnfssIUzP7R6F1/80ulyjwUN6G+SI7phbivr2gmSdfkYiJVdwBKf611srIrNF/Eh\nllt/2sjwZNyTai4pzZv9/svix5nIVCHdKZD6wsxCFOdNhVJGBd9uQ4Pk1hQoW/jj\nsUF/NBUa49k31/IQiqQ6T1xQvSkEUYd1kstS7utO2V0Z9rHH4/+4HNyPMKipkCi2\n/7WuvQGDyHTnNUFmEANn4X06iQAVon9L8IVRcGwtgsWJ0fuVGK5POtU4m37Q35MW\n3RWF3OzyP/6PxRX5ljQFmkwGkqzHrNzOZN+zMQIDAQABo1MwUTAdBgNVHQ4EFgQU\nc1BN7ZNXq+OA5hT+vq1NOMoUrR8wHwYDVR0jBBgwFoAUc1BN7ZNXq+OA5hT+vq1N\nOMoUrR8wDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAUQ8MwCmb\nDrwCNGaUdGNXoiAWrltPE2buzpq8OCVxXBSqSumAe1SkjyKROBbevM7/Cl4V3qgj\nEHjUXNrzFR9WTCOOjJ9W0wiO25FDBguU5VCkuhvfL6lCgVuJRRi9LtiY2hT5fPdM\nrMvS0/iUV3RXcKNyIGOXH1OcOim+PVph2wj0x/N1Nmhqkh+2LtFHhsE/Zgygndla\nsbyRTNQ3bnzGUwMl8jPa7KSeggtfmjIFc4VXEgrOzKHhJO2C96FmVMo97QqKQvq+\nWjPfzHnI3hhr1GPPMHFQQZG5j4a708Qo9fZcKJxUvm0O7kdFFOqcMyo086cu1YUP\nGy5s8MqShPUp1Q==\n-----END CERTIFICATE-----";
  std::string test_cert = "";

  bool isAdded = AddCertificateAuthority(cert, ec);

  if(isAdded){
    LOG4CXX_INFO(logger_, "Certificate Authority added successfully");
  }else{
    LOG4CXX_INFO(logger_, "Failed to add certificate authority");
  }

  if(ec){
    std::string str_err = "ErrorMessage: " + ec.message();
    LOG4CXX_ERROR(logger_,
                  "Failed to add certificate authority: " << invalid_cert);
    LOG4CXX_ERROR(logger_, str_err);
    Shutdown();
    return TransportAdapter::FAIL;
  }

  //Perform SSL Handshake
  ws_.next_layer().handshake(ssl::stream_base::client, ec);

  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    LOG4CXX_ERROR(logger_,
                  "Could not complete SSL Handshake failed with host/port: " << host << ":"
                                                                  << port);
    LOG4CXX_ERROR(logger_, str_err);
    Shutdown();
    return TransportAdapter::FAIL;
  }

  LOG4CXX_DEBUG(logger_, "CLOUD_CONN_SSL");  

  // Perform websocket handshake
  ws_.handshake(host, "/", ec);

  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    LOG4CXX_ERROR(logger_,
                  "Could not complete handshake with host/port: " << host << ":"
                                                                  << port);
    LOG4CXX_ERROR(logger_, str_err);
    Shutdown();
    return TransportAdapter::FAIL;
  }

  LOG4CXX_DEBUG(logger_, "CLOUD_CONN_WEBSOCKET");  
  
  // Set the binary message write option
  ws_.binary(true);
  write_thread_->start(threads::ThreadOptions());
  controller_->ConnectDone(device_uid_, app_handle_);

  // Start async read
  ws_.async_read(buffer_,
                 std::bind(&WebsocketClientConnection::OnRead,
                           this,
                           std::placeholders::_1,
                           std::placeholders::_2));

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

  ws_.async_read(buffer_,
                 std::bind(&WebsocketClientConnection::OnRead,
                           this,
                           std::placeholders::_1,
                           std::placeholders::_2));
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
      handler_.ws_.write(
          boost::asio::buffer(message_ptr->data(), message_ptr->data_size()));
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