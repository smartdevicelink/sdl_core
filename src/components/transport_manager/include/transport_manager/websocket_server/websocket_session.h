/*
Copyright (c) 2020 Livio, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

* Neither the name of SmartDeviceLink Consortium, Inc. nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_SESSION_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_SESSION_H_

#include <boost/asio/bind_executor.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <functional>

#include "protocol/raw_message.h"
#include "transport_manager/transport_adapter/transport_adapter.h"
#include "utils/logger.h"

#ifdef ENABLE_SECURITY
#include <boost/beast/websocket/ssl.hpp>
#endif  // ENABLE_SECURITY

namespace transport_manager {
namespace transport_adapter {

using Message = ::protocol_handler::RawMessagePtr;
using DataReceiveCallback = std::function<void(Message)>;
using DataSendDoneCallback = DataReceiveCallback;
using DataSendFailedCallback = DataReceiveCallback;
using DataWriteCallback = DataReceiveCallback;
using OnIOErrorCallback = std::function<void()>;

using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>
namespace websocket =
    boost::beast::websocket;  // from <boost/beast/websocket.hpp>
#ifdef ENABLE_SECURITY
namespace ssl = boost::asio::ssl;  // from <boost/asio/ssl.hpp>
#endif                             // ENABLE_SECURITY

CREATE_LOGGERPTR_GLOBAL(ws_logger_, "WebSocketSession")

class TransportAdapterController;

template <typename ExecutorType = tcp::socket&>
class WebSocketSession
    : public std::enable_shared_from_this<WebSocketSession<ExecutorType> > {
 public:
  WebSocketSession(boost::asio::ip::tcp::socket socket,
                   DataReceiveCallback data_receive,
                   DataSendDoneCallback data_send_done,
                   DataSendFailedCallback data_send_failed,
                   OnIOErrorCallback on_error);

#ifdef ENABLE_SECURITY
  WebSocketSession(boost::asio::ip::tcp::socket socket,
                   ssl::context& ctx,
                   DataReceiveCallback data_receive,
                   DataSendDoneCallback data_send_done,
                   DataSendFailedCallback data_send_failed,
                   OnIOErrorCallback on_error);
#endif  // ENABLE_SECURITY

  virtual ~WebSocketSession();

  virtual void AsyncAccept();

  virtual void AsyncRead(boost::system::error_code ec);

  virtual void WriteDown(Message message);

  virtual void Read(boost::system::error_code ec,
                    std::size_t bytes_transferred);

  virtual bool Shutdown();

 protected:
  tcp::socket socket_;
  websocket::stream<ExecutorType> ws_;
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;
  boost::beast::flat_buffer buffer_;
  DataReceiveCallback data_receive_;
  DataSendDoneCallback data_send_done_;
  DataSendFailedCallback data_send_failed_;
  OnIOErrorCallback on_io_error_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_SESSION_H_
