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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_SECURE_SESSION_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_SECURE_SESSION_H_

#include <memory>
#include "transport_manager/websocket_server/websocket_session.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(wss_logger_, "WebSocketSecureSession")

template <typename ExecutorType = ssl::stream<tcp::socket&> >
class WebSocketSecureSession : public WebSocketSession<ExecutorType> {
 public:
  WebSocketSecureSession(tcp::socket,
                         ssl::context& ctx,
                         DataReceiveCallback data_receive,
                         DataSendDoneCallback data_send_done,
                         DataSendFailedCallback data_send_failed,
                         OnIOErrorCallback on_error);

  void AsyncAccept() OVERRIDE;
  virtual void AsyncHandshake(boost::system::error_code ec);

  std::shared_ptr<WebSocketSecureSession<ExecutorType> > shared_from_this() {
    auto base_ptr =
        static_cast<WebSocketSession<ExecutorType>*>(this)->shared_from_this();
    return std::static_pointer_cast<WebSocketSecureSession<ExecutorType> >(
        base_ptr);
  }
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_SECURE_SESSION_H_
