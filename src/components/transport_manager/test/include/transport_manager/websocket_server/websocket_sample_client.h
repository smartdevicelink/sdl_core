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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_SAMPLE_CLIENT_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_SAMPLE_CLIENT_

#include <boost/asio/bind_executor.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/beast/websocket/stream.hpp>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace transport_manager {
namespace transport_adapter {

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

using WS = websocket::stream<tcp::socket>;
using WSS = websocket::stream<ssl::stream<tcp::socket> >;

struct SecurityParams {
  std::string ca_cert_;
  std::string client_cert_;
  std::string client_key_;
};

template <typename Stream = WS>
class WSSampleClient
    : public std::enable_shared_from_this<WSSampleClient<Stream> > {
 public:
  WSSampleClient(const std::string& host, const std::string& port);
  WSSampleClient(const std::string& host,
                 const std::string& port,
                 const SecurityParams& params);
  ~WSSampleClient() {}

  /**
   * @brief Inside a Run(), functions are invoked from the boost (connection,
   * handshake, message) which are blocking calls
   * @return true if Run() did without errors
   **/
  bool Run();

  void OnRead(beast::error_code ec, std::size_t bytes_transferred);

  bool Connect(tcp::resolver::results_type& results);

  bool Handshake(const std::string& host, const std::string& target);

  void OnHandshakeTimeout();

  bool IsHandshakeSuccessful() const;

  void Stop();

 private:
  asio::io_context ioc_;
  tcp::resolver resolver_;
  ssl::context ctx_;
  std::unique_ptr<Stream> ws_;
  boost::asio::thread_pool io_pool_;
  beast::flat_buffer buffer_;
  std::string host_;
  std::string port_;
  std::atomic_bool handshake_successful_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_SAMPLE_CLIENT_
