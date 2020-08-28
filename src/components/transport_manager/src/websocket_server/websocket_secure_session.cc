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

#include "transport_manager/websocket_server/websocket_secure_session.h"
#include <unistd.h>
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {

using namespace boost::beast::websocket;

template <typename ExecutorType>
WebSocketSecureSession<ExecutorType>::WebSocketSecureSession(
    tcp::socket socket,
    ssl::context& ctx,
    DataReceiveCallback data_receive,
    DataSendDoneCallback data_send_done,
    DataSendFailedCallback data_send_failed,
    OnIOErrorCallback on_error)
    : WebSocketSession<ExecutorType>(std::move(socket),
                                     ctx,
                                     data_receive,
                                     data_send_done,
                                     data_send_failed,
                                     on_error) {}

template <typename ExecutorType>
void WebSocketSecureSession<ExecutorType>::AsyncAccept() {
  LOG4CXX_AUTO_TRACE(ws_logger_);
  // Perform the SSL handshake
  WebSocketSecureSession<ExecutorType>::ws_.next_layer().async_handshake(
      ssl::stream_base::server,
      boost::asio::bind_executor(
          WebSocketSecureSession<ExecutorType>::strand_,
          std::bind(&WebSocketSecureSession::AsyncHandshake,
                    this->shared_from_this(),
                    std::placeholders::_1)));
}

template <typename ExecutorType>
void WebSocketSecureSession<ExecutorType>::AsyncHandshake(
    boost::system::error_code ec) {
  LOG4CXX_AUTO_TRACE(ws_logger_);
  if (ec) {
    auto str_err = "ErrorMessage: " + ec.message();
    LOG4CXX_ERROR(ws_logger_, str_err);
    WebSocketSession<ExecutorType>::on_io_error_();
    return;
  }

  WebSocketSession<ExecutorType>::AsyncAccept();
}

template class WebSocketSecureSession<ssl::stream<tcp::socket&> >;

}  // namespace transport_adapter
}  // namespace transport_manager
