/*
Copyright (c) 2018 Livio, Inc.
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

#ifndef MB_CONTROLLER_H
#define MB_CONTROLLER_H

#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include "json/json.h"
#include "utils/macro.h"
#include "utils/lock.h"
#include "utils/atomic_object.h"
#include "websocket_session.h"

using namespace boost::beast::websocket;

namespace hmi_message_handler {

CREATE_LOGGERPTR_GLOBAL(mb_logger_, "HMIMessageHandler")

enum ErrorCode {
  CONTROLLER_EXISTS = -32000,
  SUBSCRIBTION_EXISTS = -32001,
  INVALID_REQUEST = -32600
};

class WebsocketSession;

class CMessageBrokerController
    : public std::enable_shared_from_this<CMessageBrokerController> {
 public:
  CMessageBrokerController(const std::string& address,
                           uint16_t port,
                           std::string name,
                           int num_ports);

  ~CMessageBrokerController();

  bool StartListener();

  bool Run();

  void WaitForConnection();

  void StartSession(boost::system::error_code ec);

  void OnAccept(
      boost::system::error_code ec,
      boost::asio::strand<boost::asio::io_context::executor_type>& strand,
      stream<boost::asio::ip::tcp::socket>& ws);

  void OnRead(boost::system::error_code ec, std::size_t bytes_transferred);

  bool isNotification(Json::Value& message);

  void sendNotification(Json::Value& message);

  bool isResponse(Json::Value& message);

  void sendResponse(Json::Value& message);

  void sendJsonMessage(Json::Value& message);

  void subscribeTo(std::string property);

  void registerController(int id = 0);

  void unregisterController();

  void* MethodForReceiverThread(void* arg);

  bool Connect();

  void exitReceivingThread();

  virtual void processResponse(std::string method, Json::Value& root) = 0;

  virtual void processRequest(Json::Value& root) = 0;

  virtual void processNotification(Json::Value& root) = 0;

  std::string getMethodName(std::string& method);

  std::string GetComponentName(std::string& method);

  void processInternalRequest(Json::Value& message,
                              WebsocketSession* ws_session);

  void pushRequest(Json::Value& message, WebsocketSession* ws_session);

  // Registry
  bool addController(WebsocketSession* ws_session, std::string name);

  void deleteController(WebsocketSession* ws_session);

  void deleteController(std::string name);

  void removeSubscribersBySession(const WebsocketSession* ws);

  bool addSubscriber(WebsocketSession* ws_session, std::string name);

  void deleteSubscriber(WebsocketSession* ws, std::string name);

  int getSubscribersFd(std::string name,
                       std::vector<WebsocketSession*>& result);

  int getNextControllerId();

 private:
  boost::asio::io_context ioc_;
  const std::string& address_;
  uint16_t port_;
  std::string name_;
  int num_threads_;
  std::vector<std::thread> thread_vector_;

  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::socket socket_;
  boost::beast::multi_buffer buffer_;
  boost::asio::ip::tcp::endpoint endpoint_;

  int mControllersIdCounter;

  // Registry
  std::vector<std::shared_ptr<hmi_message_handler::WebsocketSession> >
      mConnectionList;
  sync_primitives::Lock mConnectionListLock;

  std::map<std::string, WebsocketSession*> mControllersList;
  sync_primitives::Lock mControllersListLock;

  std::multimap<std::string, WebsocketSession*> mSubscribersList;
  sync_primitives::Lock mSubscribersListLock;

  std::map<std::string, WebsocketSession*> mRequestList;
  sync_primitives::Lock mRequestListLock;

  std::atomic_bool shutdown_;
};

}  // hmi_message_handler

#endif /* MB_CONTROLLER_H */