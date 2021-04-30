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

#include "hmi_message_handler/mb_controller.h"

using namespace boost::beast::websocket;
namespace hmi_message_handler {

SDL_CREATE_LOG_VARIABLE("HMIMessageHandler")

CMessageBrokerController::CMessageBrokerController(const std::string& address,
                                                   uint16_t port,
                                                   std::string name,
                                                   int num_threads)
    : address_(address)
    , acceptor_(ioc_)
    , socket_(ioc_)
    , mControllersIdCounter(1) {
  port_ = port;
  name_ = name;
  num_threads_ = num_threads;
  endpoint_ = {boost::asio::ip::make_address(address),
               static_cast<unsigned short>(port)};
  shutdown_ = false;
}

CMessageBrokerController::~CMessageBrokerController() {
  shutdown_.exchange(true);
  CloseConnection();
}

bool CMessageBrokerController::StartListener() {
  boost::system::error_code error;
  acceptor_.open(endpoint_.protocol(), error);
  if (error) {
    std::string str_err = "ErrorOpen: " + error.message();
    SDL_LOG_ERROR(str_err);
    return false;
  }

  acceptor_.set_option(boost::asio::socket_base::reuse_address(true), error);
  if (error) {
    std::string str_err = "ErrorSetOption: " + error.message();
    SDL_LOG_ERROR(str_err);
    return false;
  }
  acceptor_.bind(endpoint_, error);
  if (error) {
    std::string str_err = "ErrorBind: " + error.message();
    SDL_LOG_ERROR(str_err);
    return false;
  }
  acceptor_.listen(boost::asio::socket_base::max_listen_connections, error);
  if (error) {
    std::string str_err = "ErrorListen: " + error.message();
    SDL_LOG_ERROR(str_err);
    return false;
  }
  return true;
}

bool CMessageBrokerController::Run() {
  if (acceptor_.is_open() && !shutdown_) {
    acceptor_.async_accept(socket_,
                           std::bind(&CMessageBrokerController::StartSession,
                                     this,
                                     std::placeholders::_1));
    ioc_.run();
    return true;
  }
  return false;
}

void CMessageBrokerController::WaitForConnection() {
  if (acceptor_.is_open() && !shutdown_) {
    acceptor_.async_accept(socket_,
                           std::bind(&CMessageBrokerController::StartSession,
                                     this,
                                     std::placeholders::_1));
  }
}

void CMessageBrokerController::StartSession(boost::system::error_code ec) {
  if (ec) {
    SDL_LOG_ERROR("ErrorMessage: " << ec.message());
    CloseConnection();
    return;
  }
  if (shutdown_) {
    return;
  }
  std::shared_ptr<WebsocketSession> ws_ptr =
      std::make_shared<WebsocketSession>(std::move(socket_), this);
  ws_ptr->Accept();

  mConnectionListLock.Acquire();
  mConnectionList.push_back(ws_ptr);
  mConnectionListLock.Release();

  WaitForConnection();
}

bool CMessageBrokerController::isNotification(Json::Value& message) {
  bool ret = false;
  if (false == message.isMember("id")) {
    ret = true;
  }
  return ret;
}

void CMessageBrokerController::sendNotification(Json::Value& message) {
  std::string methodName = message["method"].asString();
  const auto result = getSubscribersFd(methodName);
  if (result.empty()) {
    SDL_LOG_ERROR("No subscribers for method: " << methodName);
  }

  for (const auto& ws : result) {
    ws->sendJsonMessage(message);
  }
}

bool CMessageBrokerController::isResponse(Json::Value& message) {
  bool ret = false;
  if ((true == message.isMember("result")) ||
      (true == message.isMember("error"))) {
    ret = true;
  }
  return ret;
}

void CMessageBrokerController::sendResponse(Json::Value& message) {
  std::weak_ptr<WebsocketSession> weak_ws;
  std::string id = message["id"].asString();

  {
    sync_primitives::AutoLock request_lock(mRequestListLock);
    const auto it = mRequestList.find(id);
    if (it != mRequestList.end()) {
      std::swap(weak_ws, it->second);
      mRequestList.erase(it);
    }
  }

  if (auto ws = weak_ws.lock()) {
    ws->sendJsonMessage(message);
  } else {
    SDL_LOG_ERROR("A request is not found for id: " << id);
  }
}

bool CMessageBrokerController::sendJsonMessage(Json::Value& message) {
  if (isNotification(message)) {
    sendNotification(message);
    return true;
  } else if (isResponse(message)) {
    sendResponse(message);
    return true;
  }

  // Send request
  std::shared_ptr<WebsocketSession> ws;
  std::string method = message["method"].asString();
  std::string component_name = GetComponentName(method);

  {
    sync_primitives::AutoLock lock(mControllersListLock);
    const auto it = mControllersList.find(component_name);
    if (it != mControllersList.end()) {
      ws = it->second.lock();
      if (!ws) {
        // Clear expired
        mControllersList.erase(it);
      }
    }
  }

  if (!ws) {
    SDL_LOG_ERROR(
        "A controller is not found for the method: " << component_name);
    return false;
  }

  ws->sendJsonMessage(message);
  return true;
}

void CMessageBrokerController::subscribeTo(std::string property) {}

void CMessageBrokerController::registerController(int id) {}

void CMessageBrokerController::unregisterController() {}

void* CMessageBrokerController::MethodForReceiverThread(void* arg) {
  return NULL;
}

bool CMessageBrokerController::Connect() {
  return true;
}

void CMessageBrokerController::exitReceivingThread() {
  shutdown_.exchange(true);

  mConnectionListLock.Acquire();
  std::vector<std::shared_ptr<hmi_message_handler::WebsocketSession> >::iterator
      it;
  for (it = mConnectionList.begin(); it != mConnectionList.end();) {
    (*it)->Shutdown();
    it = mConnectionList.erase(it);
  }
  mConnectionListLock.Release();
  CloseConnection();
}

std::string CMessageBrokerController::getMethodName(std::string& method) {
  std::string return_string = "";
  if (method != "") {
    int position = method.find(".");
    if (position != -1) {
      return_string = method.substr(position + 1);
    }
  }
  return return_string;
}

std::string CMessageBrokerController::GetComponentName(std::string& method) {
  std::string return_string = "";
  if (method != "") {
    int position = method.find(".");
    if (position != -1) {
      return_string = method.substr(0, position);
    }
  }
  return return_string;
}

bool CMessageBrokerController::addController(WebsocketSession& ws_session,
                                             const std::string& name) {
  sync_primitives::AutoLock lock(mControllersListLock);
  return mControllersList.emplace(name, ws_session.shared_from_this()).second;
}

void CMessageBrokerController::deleteController(WebsocketSession& ws_session) {
  removeSubscribersBySession(ws_session);
  sync_primitives::AutoLock lock(mControllersListLock);
  for (auto it = mControllersList.cbegin(); it != mControllersList.cend();) {
    const std::shared_ptr<WebsocketSession> ws = it->second.lock();
    if (!ws || ws.get() == &ws_session) {
      it = mControllersList.erase(it);
    } else {
      ++it;
    }
  }
}

void CMessageBrokerController::deleteController(const std::string& name) {
  std::weak_ptr<WebsocketSession> weak_ws;
  {
    sync_primitives::AutoLock lock(mControllersListLock);
    const auto it = mControllersList.find(name);
    if (it != mControllersList.end()) {
      std::swap(weak_ws, it->second);
      mControllersList.erase(it);
    }
  }

  if (auto ws = weak_ws.lock()) {
    removeSubscribersBySession(*ws);
  } else {
    SDL_LOG_ERROR("A controller is not found for the method: " << name);
  }
}

void CMessageBrokerController::removeSubscribersBySession(
    const WebsocketSession& ws) {
  sync_primitives::AutoLock lock(mSubscribersListLock);
  for (auto it_s = mSubscribersList.cbegin();
       it_s != mSubscribersList.cend();) {
    auto ws_session = it_s->second.lock();
    if (!ws_session || ws_session.get() == &ws) {
      it_s = mSubscribersList.erase(it_s);
    } else {
      ++it_s;
    }
  }
}

bool CMessageBrokerController::pushRequest(Json::Value& message,
                                           WebsocketSession& ws_session) {
  std::string id = message["id"].asString();
  sync_primitives::AutoLock lock(mRequestListLock);
  return mRequestList.emplace(std::move(id), ws_session.shared_from_this())
      .second;
}

bool CMessageBrokerController::addSubscriber(WebsocketSession& ws_session,
                                             const std::string& name) {
  sync_primitives::AutoLock lock(mSubscribersListLock);
  auto p = mSubscribersList.equal_range(name);
  for (auto it = p.first; it != p.second;) {
    const std::shared_ptr<WebsocketSession> ws = it->second.lock();
    if (!ws) {
      // Clear expired
      it = mSubscribersList.erase(it);
    } else if (ws.get() == &ws_session) {
      // Found an element: {name, ws_session}
      return false;
    } else {
      ++it;
    }
  }

  // Not found an element: {name, ws_session}
  mSubscribersList.emplace_hint(p.second, name, ws_session.shared_from_this());
  return true;
}

void CMessageBrokerController::deleteSubscriber(const WebsocketSession& ws,
                                                const std::string& name) {
  sync_primitives::AutoLock lock(mSubscribersListLock);
  auto p = mSubscribersList.equal_range(name);
  for (auto it = p.first; it != p.second;) {
    const std::shared_ptr<WebsocketSession> ws_session = it->second.lock();
    if (!ws_session || &ws == ws_session.get()) {
      it = mSubscribersList.erase(it);
    } else {
      ++it;
    }
  }
}

std::vector<std::shared_ptr<WebsocketSession> >
CMessageBrokerController::getSubscribersFd(const std::string& name) {
  std::vector<std::shared_ptr<WebsocketSession> > result;
  sync_primitives::AutoLock lock(mSubscribersListLock);
  auto p = mSubscribersList.equal_range(name);
  for (auto it = p.first; it != p.second;) {
    if (std::shared_ptr<WebsocketSession> ws = it->second.lock()) {
      result.push_back(std::move(ws));
      ++it;
    } else {
      // Clear expired
      it = mSubscribersList.erase(it);
    }
  }

  return result;
}

void CMessageBrokerController::processInternalRequest(
    const Json::Value& message, WebsocketSession& ws_session) {
  std::string method = message["method"].asString();
  std::string methodName = getMethodName(method);
  if (methodName == "registerComponent") {
    Json::Value params = message["params"];
    if (params.isMember("componentName") &&
        params["componentName"].isString()) {
      std::string controllerName = params["componentName"].asString();
      if (addController(ws_session, controllerName)) {
        Json::Value response;
        response["id"] = message["id"];
        response["jsonrpc"] = "2.0";
        response["result"] = getNextControllerId();
        ws_session.sendJsonMessage(response);
      } else {
        Json::Value error, err;
        error["id"] = message["id"];
        error["jsonrpc"] = "2.0";
        err["code"] = CONTROLLER_EXISTS;
        err["message"] = "Controller has been already registered.";
        error["error"] = err;
        ws_session.sendJsonMessage(error);
      }
    } else {
      Json::Value error, err;
      error["id"] = message["id"];
      error["jsonrpc"] = "2.0";
      err["code"] = INVALID_REQUEST;
      err["message"] = "Wrong method parameter.";
      error["error"] = err;
      ws_session.sendJsonMessage(error);
    }
  } else if (methodName == "subscribeTo") {
    Json::Value params = message["params"];
    if (params.isMember("propertyName") && params["propertyName"].isString()) {
      std::string propertyName = params["propertyName"].asString();
      if (addSubscriber(ws_session, propertyName)) {
        Json::Value response;
        response["id"] = message["id"];
        response["jsonrpc"] = "2.0";
        response["result"] = "OK";
        ws_session.sendJsonMessage(response);
      } else {
        Json::Value error, err;
        error["id"] = message["id"];
        error["jsonrpc"] = "2.0";
        err["code"] = CONTROLLER_EXISTS;
        err["message"] = "Subscribe has been already registered.";
        error["error"] = err;
        ws_session.sendJsonMessage(error);
      }
    } else {
      Json::Value error, err;
      error["id"] = message["id"];
      error["jsonrpc"] = "2.0";
      err["code"] = INVALID_REQUEST;
      err["message"] = "Wrong method parameter.";
      error["error"] = err;
      ws_session.sendJsonMessage(error);
    }

  } else if (methodName == "unregisterComponent") {
    Json::Value params = message["params"];
    if (params.isMember("componentName") &&
        params["componentName"].isString()) {
      std::string controllerName = params["componentName"].asString();
      deleteController(controllerName);
      Json::Value response;
      response["id"] = message["id"];
      response["jsonrpc"] = "2.0";
      response["result"] = "OK";
      ws_session.sendJsonMessage(response);
    } else {
      Json::Value error, err;
      error["id"] = message["id"];
      error["jsonrpc"] = "2.0";
      err["code"] = INVALID_REQUEST;
      err["message"] = "Wrong method parameter.";
      error["error"] = err;
      ws_session.sendJsonMessage(error);
    }
  } else if (methodName == "unsubscribeFrom") {
    Json::Value params = message["params"];
    if (params.isMember("propertyName") && params["propertyName"].isString()) {
      std::string propertyName = params["propertyName"].asString();
      deleteSubscriber(ws_session, propertyName);
      Json::Value response;
      response["id"] = message["id"];
      response["jsonrpc"] = "2.0";
      response["result"] = "OK";
      ws_session.sendJsonMessage(response);
    } else {
      Json::Value error, err;
      error["id"] = message["id"];
      error["jsonrpc"] = "2.0";
      err["code"] = INVALID_REQUEST;
      err["message"] = "Wrong method parameter.";
      error["error"] = err;
      ws_session.sendJsonMessage(error);
    }
  } else {
  }
}

int CMessageBrokerController::getNextControllerId() {
  return 1000 * mControllersIdCounter++;
}

void CMessageBrokerController::CloseConnection() {
  if (!ioc_.stopped()) {
    boost::system::error_code ec;

    acceptor_.cancel(ec);
    if (ec) {
      SDL_LOG_ERROR("Acceptor cancel error: " << ec.message());
    }

    acceptor_.close(ec);
    if (ec) {
      SDL_LOG_ERROR("Acceptor close error: " << ec.message());
    }

    socket_.close(ec);
    if (ec) {
      SDL_LOG_ERROR("Socket close error : " << ec.message());
    }

    ioc_.stop();
  }
}
}  // namespace hmi_message_handler
