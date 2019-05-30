/*
 * Copyright (c) 2019, Ford Motor Company
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

#include "transport_manager/cloud/sample_websocket_server.h"

namespace {
// Report a failure
void Fail(char const* tag, boost::system::error_code ec) {
  std::cerr << tag << ": " << ec.message() << "\n";
}
}  // namespace

namespace sample {
namespace websocket {

WSSession::WSServer::WSServer(tcp::socket&& socket)
    : ws_(std::move(socket)), strand_(ws_.get_executor()) {}

void WSSession::WSServer::AddURLRoute(const std::string& target) {
  url_routes_.insert(ParseRouteFromTarget(target));
}

void WSSession::WSServer::Run() {
  req_ = {};
  http::async_read(
      ws_.next_layer(),
      buffer_,
      req_,
      std::bind(&WSServer::OnWebsocketHandshake, this, std::placeholders::_1));
}

void WSSession::WSServer::OnWebsocketHandshake(
    const boost::system::error_code& ec) {
  if (ec) {
    return Fail("ERROR_HTTP_REQUEST_READ", ec);
  }
  if (websocket::is_upgrade(req_)) {
    // Check target
    std::string route = ParseRouteFromTarget(req_.target().to_string());
    if (!CanHandleRoute(route)) {
      auto error = boost::system::errc::make_error_code(
          boost::system::errc::address_not_available);
      ws_.next_layer().close();
      return Fail("ERROR_INVALID_TARGET", error);
    }
    // Accept the websocket handshake
    ws_.async_accept(
        req_,
        boost::asio::bind_executor(
            strand_,
            std::bind(&WSServer::OnAccept, this, std::placeholders::_1)));
  }
}

void WSSession::WSServer::OnAccept(beast::error_code ec) {
  if (ec) {
    return Fail("ERROR_WEBSOCKET_HANDSHAKE", ec);
  }
}

bool WSSession::WSServer::CanHandleRoute(const std::string& route) {
  if (url_routes_.find(route) == url_routes_.end()) {
    return false;
  }
  return true;
}

std::string WSSession::WSServer::ParseRouteFromTarget(
    const std::string& target) {
  std::string route = target;
  // Remove fragment
  auto fragment_pos = route.find('#');
  if (fragment_pos != std::string::npos) {
    route = route.substr(0, fragment_pos);
  }
  // Remove query
  auto query_pos = route.find('?');
  if (query_pos != std::string::npos) {
    route = route.substr(0, query_pos);
  }

  return route;
}

WSSession::WSSession(const std::string& address, uint16_t port)
    : address_(address)
    , port_(port)
    , acceptor_(ioc_)
    , socket_(ioc_)
    , ws_(nullptr) {
  endpoint_ = {boost::asio::ip::make_address(address), port};
  boost::system::error_code error;

  // Open the acceptor
  acceptor_.open(endpoint_.protocol(), error);
  if (error) {
    Fail("ERROR_ACCEPTOR_OPEN", error);
    return;
  }

  // Allow address reuse
  acceptor_.set_option(boost::asio::socket_base::reuse_address(true), error);
  if (error) {
    Fail("ERROR_SET_OPTION", error);
    return;
  }

  // Bind to the server address
  acceptor_.bind(endpoint_, error);
  if (error) {
    Fail("ERROR_BIND", error);
    return;
  }

  // Start listening for connections
  acceptor_.listen(boost::asio::socket_base::max_listen_connections, error);
  if (error) {
    Fail("ERROR_LISTEN", error);
    return;
  }
}

void WSSession::Run() {
  if (acceptor_.is_open()) {
    acceptor_.async_accept(
        socket_, std::bind(&WSSession::on_accept, this, std::placeholders::_1));
    ioc_.run();
  }
}

void WSSession::Stop() {
  try {
    ioc_.stop();
    acceptor_.close();
  } catch (...) {
    std::cerr << "Failed to close connection" << std::endl;
  }
}

void WSSession::AddRoute(const std::string& route) {
  if (ws_ == nullptr) {
    buffered_routes_.push(route);
    return;
  }
  ws_->AddURLRoute(route);
}

void WSSession::on_accept(boost::system::error_code ec) {
  if (ec) {
    Fail("ERROR_ON_ACCEPT", ec);
    ioc_.stop();
    return;
  }

  // Make websocket object and start
  ws_ = std::make_shared<WSServer>(std::move(socket_));
  // Load routes stored in buffer
  while (!buffered_routes_.empty()) {
    ws_->AddURLRoute(buffered_routes_.front());
    buffered_routes_.pop();
  }
  ws_->Run();
}

WSSSession::WSSServer::WSSServer(tcp::socket&& socket, ssl::context& ctx)
    : wss_(std::move(socket), ctx) {}

void WSSSession::WSSServer::AddURLRoute(const std::string& target) {
  url_routes_.insert(ParseRouteFromTarget(target));
}
void WSSSession::WSSServer::Run() {
  // Perform the SSL handshake
  wss_.next_layer().async_handshake(
      ssl::stream_base::server,
      std::bind(&WSSServer::OnSSLHandshake, this, std::placeholders::_1));
}

void WSSSession::WSSServer::OnSSLHandshake(beast::error_code ec) {
  if (ec) {
    return Fail("ERROR_SSL_HANDSHAKE", ec);
  }

  req_ = {};
  http::async_read(
      wss_.next_layer(),
      buffer_,
      req_,
      std::bind(&WSSServer::OnWebsocketHandshake, this, std::placeholders::_1));
}

void WSSSession::WSSServer::OnWebsocketHandshake(
    const boost::system::error_code& ec) {
  if (ec) {
    return Fail("ERROR_HTTP_REQUEST_READ", ec);
  }
  if (websocket::is_upgrade(req_)) {
    // Check target
    std::string route = ParseRouteFromTarget(req_.target().to_string());
    if (!CanHandleRoute(route)) {
      auto error = boost::system::errc::make_error_code(
          boost::system::errc::address_not_available);
      wss_.next_layer().next_layer().close();
      return Fail("ERROR_INVALID_TARGET", error);
    }
    // Accept the websocket handshake
    wss_.async_accept(
        req_, std::bind(&WSSServer::OnAccept, this, std::placeholders::_1));
  }
}

void WSSSession::WSSServer::OnAccept(beast::error_code ec) {
  if (ec) {
    return Fail("ERROR_ON_ACCEPT", ec);
  }
}

bool WSSSession::WSSServer::CanHandleRoute(const std::string& route) {
  if (url_routes_.find(route) == url_routes_.end()) {
    return false;
  }
  return true;
}

std::string WSSSession::WSSServer::ParseRouteFromTarget(
    const std::string& target) {
  std::string route = target;
  // Remove fragment
  auto fragment_pos = route.find('#');
  if (fragment_pos != std::string::npos) {
    route = route.substr(0, fragment_pos);
  }
  // Remove query
  auto query_pos = route.find('?');
  if (query_pos != std::string::npos) {
    route = route.substr(0, query_pos);
  }

  return route;
}

WSSSession::WSSSession(const std::string& address,
                       uint16_t port,
                       const std::string& certificate,
                       const std::string& private_key)
    : acceptor_(ioc_)
    , socket_(ioc_)
    , ctx_(ssl::context::sslv23_server)
    , wss_(nullptr) {
  beast::error_code ec;
  endpoint_ = {boost::asio::ip::make_address(address), port};

  // Load the certificate
  ctx_.use_certificate(
      boost::asio::buffer(certificate.c_str(), certificate.size()),
      ssl::context::file_format::pem,
      ec);
  if (ec) {
    Fail("ERROR_USE_CERTIFICATE", ec);
    return;
  }

  // Load the private key
  ctx_.use_rsa_private_key(
      boost::asio::buffer(private_key.c_str(), private_key.size()),
      ssl::context::file_format::pem,
      ec);
  if (ec) {
    Fail("ERROR_USE_RSA_PRIVATE_KEY", ec);
    return;
  }

  // Open the acceptor
  acceptor_.open(endpoint_.protocol(), ec);
  if (ec) {
    Fail("EEROR_ACCEPTOR_OPEN", ec);
    return;
  }

  // Allow address reuse
  acceptor_.set_option(net::socket_base::reuse_address(true), ec);
  if (ec) {
    Fail("ERROR_SET_OPTION", ec);
    return;
  }

  // Bind to the server address
  acceptor_.bind(endpoint_, ec);
  if (ec) {
    Fail("ERROR_BIND", ec);
    return;
  }

  // Start listening for connections
  acceptor_.listen(net::socket_base::max_listen_connections, ec);
  if (ec) {
    Fail("ERROR_LISTEN", ec);
    return;
  }
}

// Start accepting incoming connections
void WSSSession::Run() {
  do_accept();
}

void WSSSession::Stop() {
  try {
    ioc_.stop();
    acceptor_.close();
  } catch (...) {
    std::cerr << "Failed to close connection" << std::endl;
  }
}

void WSSSession::AddRoute(const std::string& route) {
  if (wss_ == nullptr) {
    buffered_routes_.push(route);
    return;
  }
  wss_->AddURLRoute(route);
}

void WSSSession::do_accept() {
  if (acceptor_.is_open()) {
    acceptor_.async_accept(
        socket_,
        std::bind(&WSSSession::on_accept, this, std::placeholders::_1));
    ioc_.run();
  }
}

void WSSSession::on_accept(boost::system::error_code ec) {
  if (ec) {
    Fail("ERROR_ON_ACCEPT", ec);
    ioc_.stop();
    return;
  }
  // Create the session and run it
  wss_ = std::make_shared<WSSServer>(std::move(socket_), ctx_);
  // Load routes stored in buffer
  while (!buffered_routes_.empty()) {
    wss_->AddURLRoute(buffered_routes_.front());
    buffered_routes_.pop();
  }
  wss_->Run();
}

}  // namespace websocket
}  // namespace sample
