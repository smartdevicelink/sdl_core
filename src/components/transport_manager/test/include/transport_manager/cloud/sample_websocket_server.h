
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
void fail(beast::error_code ec, char const* what) {
  std::cerr << what << ": " << ec.message() << "\n";
}

// Accepts incoming connections and launches the WSServer
class WSSession : public std::enable_shared_from_this<WSSession> {
 private:
  // Echoes back all received WebSocket messages
  class WSServer : public std::enable_shared_from_this<WSServer> {
   public:
    // Take ownership of the socket
    explicit WSServer(tcp::socket&& socket)
        : ws_(std::move(socket)), strand_(ws_.get_executor()) {}

    // Start the asynchronous operation
    void run() {
      // Accept the websocket handshake
      ws_.async_accept(
          boost::asio::bind_executor(strand_,
                                     std::bind(&WSServer::on_accept,
                                               shared_from_this(),
                                               std::placeholders::_1)));
    }

    void on_accept(beast::error_code ec) {
      std::cout << "SERVER: Accepting connection" << std::endl;
      if (ec) {
        std::cout << "SERVER: Connection accept FAILED" << std::endl;
        return fail(ec, "accept");
      }
      // Read a message
      // do_read();
    }

    // void do_read() {
    //   // Read a message into our buffer
    //   ws_.async_read(
    //       buffer_,
    //       boost::asio::bind_executor(strand_,
    //                                  std::bind(&WSServer::on_read,
    //                                            shared_from_this(),
    //                                            std::placeholders::_1,
    //                                            std::placeholders::_2)));
    // }

    // void do_write() {
    //   ws_.text(ws_.got_text());
    //   ws_.async_write(buffer_.data(),
    //                   std::bind(&WSServer::on_write,
    //                             shared_from_this(),
    //                             std::placeholders::_1,
    //                             std::placeholders::_2));
    // }

    // void on_read(beast::error_code ec, std::size_t bytes_transferred) {
    //   std::cout << "SERVER: receiving data" << std::endl;

    //   boost::ignore_unused(bytes_transferred);

    //   // This indicates that the WSServer was closed
    //   if (ec == websocket::error::closed) {
    //     std::cout << "SERVER: Websocket closed" << std::endl;
    //     return;
    //   }

    //   if (ec) {
    //     std::cout << "SERVER: Read failed" << std::endl;
    //     fail(ec, "read");
    //   }

    //   // Echo the message
    //   do_write();
    // }

    // void on_write(beast::error_code ec, std::size_t bytes_transferred) {
    //   std::cout << "SERVER: Sending data" << std::endl;

    //   boost::ignore_unused(bytes_transferred);

    //   if (ec) {
    //     std::cout << "SERVER: Write failed" << std::endl;
    //     return fail(ec, "write");
    //   }

    //   // Clear the buffer
    //   buffer_.consume(buffer_.size());

    //   // Do another read
    //   do_read();
    // }

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

    acceptor_.open(endpoint_.protocol(), error);
    if (error) {
      std::cerr << "ErrorOpen: " << error.message() << std::endl;
      return;
    }
    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), error);
    if (error) {
      std::cerr << "ErrorSetOption: " << error.message() << std::endl;
      return;
    }
    acceptor_.bind(endpoint_, error);
    if (error) {
      std::cerr << "ErrorBind: " << error.message() << std::endl;
      return;
    }
    acceptor_.listen(boost::asio::socket_base::max_listen_connections, error);
    if (error) {
      std::cerr << "ErrorListen: " << error.message() << std::endl;
      return;
    }
  }

  void run() {
    if (acceptor_.is_open()) {
      acceptor_.async_accept(
          socket_,
          std::bind(&WSSession::StartSession, this, std::placeholders::_1));
      ioc_.run();
    }
  }

  void stop() {
    std::cout << "SERVER: Closing connection" << std::endl;

    try {
      ioc_.stop();
      acceptor_.close();
    } catch (...) {
      std::cout << "SERVER: Failed to close connection" << std::endl;
    }
  }

 private:
  void StartSession(boost::system::error_code ec) {
    if (ec) {
      std::cerr << "ErrorMessage: " << ec.message() << std::endl;
      ioc_.stop();
      return;
    }

    // Make websocket object and start
    ws_ = std::make_shared<WSServer>(std::move(socket_));
    ws_->run();
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
    void run() {
      // Perform the SSL handshake
      wss_.next_layer().async_handshake(ssl::stream_base::server,
                                        std::bind(&WSSServer::on_handshake,
                                                  shared_from_this(),
                                                  std::placeholders::_1));
    }

    void on_handshake(beast::error_code ec) {
      std::cout << "DEBUG: SSL Handshake" << std::endl;
      if (ec)
        return fail(ec, "handshake");

      // Accept the websocket handshake
      wss_.async_accept(std::bind(
          &WSSServer::on_accept, shared_from_this(), std::placeholders::_1));
    }

    void on_accept(beast::error_code ec) {
      std::cout << "DEBUG: Server accepted connection" << std::endl;
      if (ec)
        return fail(ec, "accept");

      // Read a message
      // do_read();
    }

    // void
    // do_read()
    // {
    //     // Read a message into our buffer
    //     wss_.async_read(
    //         buffer_,
    //         std::bind(
    //             &WSSServer::on_read,
    //             shared_from_this(),
    //             std::placeholders::_1,
    //             std::placeholders::_2));
    // }

    // void
    // on_read(
    //     beast::error_code ec,
    //     std::size_t bytes_transferred)
    // {
    //     boost::ignore_unused(bytes_transferred);

    //     // This indicates that the WSSServer was closed
    //     if(ec == websocket::error::closed)
    //         return;

    //     if(ec)
    //         fail(ec, "read");

    //     // Echo the message
    //     wss_.text(wss_.got_text());
    //     wss_.async_write(
    //         buffer_.data(),
    //         std::bind(
    //             &WSSServer::on_write,
    //             shared_from_this(),
    //             std::placeholders::_1,
    //             std::placeholders::_2));
    // }

    // void
    // on_write(
    //     beast::error_code ec,
    //     std::size_t bytes_transferred)
    // {
    //     boost::ignore_unused(bytes_transferred);

    //     if(ec)
    //         return fail(ec, "write");

    //     // Clear the buffer
    //     buffer_.consume(buffer_.size());

    //     // Do another read
    //     do_read();
    // }
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

    std::cout << "DEBUG: Adding certificate" << std::endl;
    // Load the certificate
    ctx_.use_certificate(
        boost::asio::buffer(certificate.c_str(), certificate.size()),
        ssl::context::file_format::pem,
        ec);
    if (ec) {
      fail(ec, "Load certficate");
      return;
    }

    std::cout << "DEBUG: Adding private key" << std::endl;
    // Load the private key
    ctx_.use_rsa_private_key(
        boost::asio::buffer(private_key.c_str(), private_key.size()),
        ssl::context::file_format::pem,
        ec);
    if (ec) {
      fail(ec, "Load private key");
      return;
    }

    // Open the acceptor
    acceptor_.open(endpoint_.protocol(), ec);
    if (ec) {
      fail(ec, "open");
      return;
    }

    // Allow address reuse
    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if (ec) {
      fail(ec, "set_option");
      return;
    }

    std::cout << "DEBUG: Binding to host:" << address << ", port:" << port
              << std::endl;
    // Bind to the server address
    acceptor_.bind(endpoint_, ec);
    if (ec) {
      fail(ec, "bind");
      return;
    }

    std::cout << "DEBUG: Start listening for connections" << std::endl;
    // Start listening for connections
    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
      fail(ec, "listen");
      return;
    }
  }

  // Start accepting incoming connections
  void run() {
    do_accept();
  }

  void stop() {
    std::cout << "SERVER: Closing connection" << std::endl;

    try {
      ioc_.stop();
      acceptor_.close();
    } catch (...) {
      std::cout << "SERVER: Failed to close connection" << std::endl;
    }
  }

 private:
  void do_accept() {
    if (acceptor_.is_open()) {
      std::cout << "DEBUG: Waiting to accept incoming connections" << std::endl;
      // The new connection gets its own strand
      acceptor_.async_accept(socket_,
                             std::bind(&WSSSession::on_accept,
                                       shared_from_this(),
                                       std::placeholders::_1));
      ioc_.run();
    }
  }

  void on_accept(boost::system::error_code ec) {
    std::cout << "DEBUG: Accepted incoming connection" << std::endl;
    if (ec) {
      fail(ec, "accept");
      ioc_.stop();
      return;
    } else {
      std::cout << "DEBUG: Creating secure websocket server" << std::endl;
      // Create the session and run it
      wss_ = std::make_shared<WSSServer>(std::move(socket_), ctx_);
      wss_->run();
    }
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