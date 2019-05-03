
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
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
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

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_CLOUD_SAMPLE_WEBSOCKET_SERVER_H_