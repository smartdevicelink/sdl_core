
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_CLOUD_SAMPLE_WEBSOCKET_SERVER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_CLOUD_SAMPLE_WEBSOCKET_SERVER_H_

#include <unistd.h>
#include <algorithm>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace beast = boost::beast;          // from <boost/beast.hpp>
namespace http = beast::http;            // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>
namespace net = boost::asio;             // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;        // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

//------------------------------------------------------------------------------

// Report a failure
void Fail(char const* tag, boost::system::error_code ec) {
  std::cerr << tag << ": " << ec.message() << "\n";
}

// Accepts incoming connections and launches the WSServer
class WSSession : public std::enable_shared_from_this<WSSession> {
 private:
  class WSServer : public std::enable_shared_from_this<WSServer> {
   public:
    explicit WSServer(tcp::socket&& socket)
        : ws_(std::move(socket)), strand_(ws_.get_executor()) {}

    void Run() {
      // Accept the websocket handshake
      ws_.async_accept(boost::asio::bind_executor(
          strand_,
          std::bind(
              &WSServer::OnAccept, shared_from_this(), std::placeholders::_1)));
    }

    void OnAccept(beast::error_code ec) {
      if (ec) {
        return Fail("ERROR_CONNECTION_ACCEPT", ec);
      }
    }

   private:
    websocket::stream<tcp::socket> ws_;
    beast::flat_buffer buffer_;
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
  };

 public:
  WSSession(const std::string& address, uint16_t port)
      : address_(address), port_(port), acceptor_(ioc_), socket_(ioc_) {
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

  void Run() {
    if (acceptor_.is_open()) {
      acceptor_.async_accept(
          socket_,
          std::bind(&WSSession::on_accept, this, std::placeholders::_1));
      ioc_.run();
    }
  }

  void Stop() {
    try {
      ioc_.stop();
      acceptor_.close();
    } catch (...) {
      std::cerr << "Failed to close connection" << std::endl;
    }
  }

 private:
  void on_accept(boost::system::error_code ec) {
    if (ec) {
      Fail("ERROR_ON_ACCEPT", ec);
      ioc_.stop();
      return;
    }

    // Make websocket object and start
    ws_ = std::make_shared<WSServer>(std::move(socket_));
    ws_->Run();
  }

  boost::asio::io_context ioc_;
  const std::string& address_;
  uint16_t port_;
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  beast::flat_buffer buffer_;
  boost::asio::ip::tcp::endpoint endpoint_;
  std::shared_ptr<WSServer> ws_;
};

// Accepts incoming connections and launches the sessions
class WSSSession : public std::enable_shared_from_this<WSSSession> {
 private:
  // Echoes back all received WebSocket messages
  class WSSServer : public std::enable_shared_from_this<WSSServer> {
   public:
    // Take ownership of the socket
    WSSServer(tcp::socket&& socket, ssl::context& ctx)
        : wss_(std::move(socket), ctx) {}

    // Start the asynchronous operation
    void Run() {
      // Perform the SSL handshake
      wss_.next_layer().async_handshake(ssl::stream_base::server,
                                        std::bind(&WSSServer::OnSSLHandshake,
                                                  shared_from_this(),
                                                  std::placeholders::_1));
    }

    void OnSSLHandshake(beast::error_code ec) {
      if (ec) {
        return Fail("ERROR_SSL_HANDSHAKE", ec);
      }

      // Accept the websocket handshake
      wss_.async_accept(std::bind(
          &WSSServer::OnAccept, shared_from_this(), std::placeholders::_1));
    }

    void OnAccept(beast::error_code ec) {
      if (ec) {
        return Fail("ERROR_ON_ACCEPT", ec);
      }
    }

   private:
    websocket::stream<ssl::stream<tcp::socket> > wss_;
    beast::flat_buffer buffer_;
  };

 public:
  WSSSession(const std::string& address,
             uint16_t port,
             const std::string& certificate,
             const std::string& private_key)
      : acceptor_(ioc_), socket_(ioc_), ctx_(ssl::context::sslv23_server) {
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
  void Run() {
    do_accept();
  }

  void Stop() {
    try {
      ioc_.stop();
      acceptor_.close();
    } catch (...) {
      std::cerr << "Failed to close connection" << std::endl;
    }
  }

 private:
  void do_accept() {
    if (acceptor_.is_open()) {
      acceptor_.async_accept(socket_,
                             std::bind(&WSSSession::on_accept,
                                       shared_from_this(),
                                       std::placeholders::_1));
      ioc_.run();
    }
  }

  void on_accept(boost::system::error_code ec) {
    if (ec) {
      Fail("ERROR_ON_ACCEPT", ec);
      ioc_.stop();
      return;
    }
    // Create the session and run it
    wss_ = std::make_shared<WSSServer>(std::move(socket_), ctx_);
    wss_->Run();
  }

 private:
  boost::asio::io_context ioc_;
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  ssl::context ctx_;
  tcp::endpoint endpoint_;
  std::shared_ptr<WSSServer> wss_;
};

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_CLOUD_SAMPLE_WEBSOCKET_SERVER_H_