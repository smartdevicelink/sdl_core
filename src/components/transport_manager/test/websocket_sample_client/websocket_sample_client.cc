/*
 * \file websocket_listener.h
 * \brief WebSocketListener class header file.
 *
 * Copyright (c) 2020
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

#include "transport_manager/websocket_server/websocket_sample_client.h"

namespace transport_manager {
namespace transport_adapter {

template <>
WSSampleClient<websocket::stream<ssl::stream<tcp::socket> > >::WSSampleClient(
    const std::string& host,
    const std::string& port,
    const SecurityParams& params)
    : resolver_(ioc_)
    , ctx_(ssl::context::sslv23_client)
    , ws_(nullptr)
    , io_pool_(1)
    , host_(host)
    , port_(port)
    , handshake_successful_(false) {
  ctx_.set_verify_mode(ssl::context::verify_peer);
  ctx_.load_verify_file(params.ca_cert_);
  ctx_.use_certificate_chain_file(params.client_cert_);
  ctx_.use_private_key_file(params.client_key_, boost::asio::ssl::context::pem);
  ws_.reset(new WSS(ioc_, ctx_));
}

template <>
WSSampleClient<WS>::WSSampleClient(const std::string& host,
                                   const std::string& port)
    : resolver_(ioc_)
    , ctx_(ssl::context::sslv23_client)
    , ws_(new WS(ioc_))
    , host_(host)
    , port_(port) {}

template <typename Stream>
bool WSSampleClient<Stream>::Run() {
  boost::system::error_code ec;
  ctx_.set_verify_mode(ssl::verify_none);

  auto results = resolver_.resolve(host_, port_, ec);
  if (ec) {
    std::cout << "ErrorMessage: " + ec.message() << std::endl;
    return false;
  }

  if (!Connect(results)) {
    return false;
  }

  if (!Handshake(host_, "/")) {
    return false;
  }

  ws_->async_read(buffer_,
                  std::bind(&WSSampleClient::OnRead,
                            this->shared_from_this(),
                            std::placeholders::_1,
                            std::placeholders::_2));
  boost::asio::post(io_pool_, [&]() { ioc_.run(); });
  return true;
}

template <typename Stream>
void WSSampleClient<Stream>::OnRead(beast::error_code ec,
                                    std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);
}

template <>
bool WSSampleClient<WS>::Connect(tcp::resolver::results_type& results) {
  boost::system::error_code ec;
  boost::asio::connect(ws_->next_layer(), results.begin(), results.end(), ec);
  if (ec) {
    return false;
  }
  return true;
}

template <>
bool WSSampleClient<WSS>::Connect(tcp::resolver::results_type& results) {
  boost::system::error_code ec;
  boost::asio::connect(ws_->lowest_layer(), results.begin(), results.end(), ec);
  if (ec) {
    return false;
  }
  return true;
}

template <>
bool WSSampleClient<WS>::Handshake(const std::string& host,
                                   const std::string& target) {
  boost::system::error_code ec;
  ws_->handshake(host, target, ec);
  if (ec) {
    return false;
  }
  return true;
}

template <>
void WSSampleClient<WS>::Stop() {
  ioc_.stop();
  ws_->lowest_layer().close();

  io_pool_.stop();
  io_pool_.join();
}

template <>
bool WSSampleClient<WSS>::Handshake(const std::string& host,
                                    const std::string& target) {
  boost::system::error_code ec;

  ws_->next_layer().handshake(ssl::stream_base::client, ec);
  if (ec) {
    std::cout << "ErrorMessage: " + ec.message() << std::endl;
    return false;
  }

  ws_->handshake(host, target, ec);
  if (ec) {
    return false;
  }

  handshake_successful_ = true;
  return true;
}

template <>
void WSSampleClient<WSS>::Stop() {
  ioc_.stop();
  ws_->next_layer().next_layer().shutdown(
      boost::asio::ip::tcp::socket::shutdown_both);
  ws_->lowest_layer().close();

  io_pool_.stop();
  io_pool_.join();
}

template <>
void WSSampleClient<WSS>::OnHandshakeTimeout() {
  if (!handshake_successful_) {
    Stop();
  }
}

template <>
bool WSSampleClient<WSS>::IsHandshakeSuccessful() const {
  return handshake_successful_;
}

template class WSSampleClient<WS>;
template class WSSampleClient<WSS>;
}  // namespace transport_adapter
}  // namespace transport_manager
