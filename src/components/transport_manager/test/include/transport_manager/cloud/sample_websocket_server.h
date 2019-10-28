
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
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <thread>

namespace sample {
namespace websocket {

namespace beast = boost::beast;          // from <boost/beast.hpp>
namespace http = beast::http;            // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>
namespace net = boost::asio;             // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;        // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

// Accepts incoming connections and launches the WSServer
class WSSession {
 private:
  class WSServer {
   public:
    explicit WSServer(tcp::socket&& socket);
    void AddURLRoute(const std::string& route);
    // Start the asynchronous operation
    void Run();

   private:
    void OnWebsocketHandshake(const boost::system::error_code& ec);
    void OnAccept(beast::error_code ec);
    // Check if route can be handled by the server
    bool CanHandleRoute(const std::string& route);
    std::string ParseRouteFromTarget(const std::string& target);

    websocket::stream<tcp::socket> ws_;
    beast::flat_buffer buffer_;
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    http::request<http::string_body> req_;
    std::set<std::string> url_routes_;
  };

 public:
  WSSession(const std::string& address, uint16_t port);
  // Start Accepting incoming connections
  void Run();
  void Stop();
  // Add route endpoint which can be handled the server
  void AddRoute(const std::string& route);

 private:
  void on_accept(boost::system::error_code ec);
  boost::asio::io_context ioc_;
  const std::string& address_;
  uint16_t port_;
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  beast::flat_buffer buffer_;
  boost::asio::ip::tcp::endpoint endpoint_;
  std::shared_ptr<WSServer> ws_;
  std::queue<std::string> buffered_routes_;
};

// Accepts incoming connections and launches the sessions
class WSSSession {
 private:
  class WSSServer {
   public:
    // Take ownership of the socket
    WSSServer(tcp::socket&& socket, ssl::context& ctx);
    void AddURLRoute(const std::string& route);
    // Start the asynchronous operation
    void Run();

   private:
    void OnSSLHandshake(beast::error_code ec);
    void OnWebsocketHandshake(const boost::system::error_code& ec);
    void OnAccept(beast::error_code ec);

    // Check if route can be handled by the server
    bool CanHandleRoute(const std::string& route);
    std::string ParseRouteFromTarget(const std::string& target);

    websocket::stream<ssl::stream<tcp::socket> > wss_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    std::set<std::string> url_routes_;
  };

 public:
  WSSSession(const std::string& address,
             uint16_t port,
             const std::string& certificate,
             const std::string& private_key);
  // Start accepting incoming connections
  void Run();
  void Stop();
  // Add route endpoint which can be handled the server
  void AddRoute(const std::string& route);

 private:
  void do_accept();
  void on_accept(boost::system::error_code ec);

 private:
  boost::asio::io_context ioc_;
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  ssl::context ctx_;
  tcp::endpoint endpoint_;
  std::shared_ptr<WSSServer> wss_;
  std::queue<std::string> buffered_routes_;
};

}  // namespace websocket
}  // namespace sample

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_CLOUD_SAMPLE_WEBSOCKET_SERVER_H_
