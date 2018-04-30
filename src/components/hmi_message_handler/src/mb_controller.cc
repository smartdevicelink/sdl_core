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
  boost::system::error_code ec;
  socket_.close();
  acceptor_.close(ec);
  if (ec) {
    std::string str_err = "ErrorMessage Close: " + ec.message();
    LOG4CXX_ERROR(mb_logger_, str_err);
  }
  shutdown_ = true;
  ioc_.stop();
}

bool CMessageBrokerController::StartListener() {
  boost::system::error_code error;
  acceptor_.open(endpoint_.protocol(), error);
  if (error) {
    std::string str_err = "ErrorOpen: " + error.message();
    LOG4CXX_ERROR(mb_logger_, str_err);
    return false;
  }

  acceptor_.set_option(boost::asio::socket_base::reuse_address(true), error);
  if (error) {
    std::string str_err = "ErrorSetOption: " + error.message();
    LOG4CXX_ERROR(mb_logger_, str_err);
    return false;
  }
  acceptor_.bind(endpoint_, error);
  if (error) {
    std::string str_err = "ErrorBind: " + error.message();
    LOG4CXX_ERROR(mb_logger_, str_err);
    return false;
  }
  acceptor_.listen(boost::asio::socket_base::max_listen_connections, error);
  if (error) {
    std::string str_err = "ErrorListen: " + error.message();
    LOG4CXX_ERROR(mb_logger_, str_err);
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
    std::string str_err = "ErrorMessage: " + ec.message();
    LOG4CXX_ERROR(mb_logger_, str_err);
    ioc_.stop();
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
  std::vector<WebsocketSession*> result;
  int subscribersCount = getSubscribersFd(methodName, result);
  if (0 < subscribersCount) {
    std::vector<WebsocketSession*>::iterator it;
    for (it = result.begin(); it != result.end(); it++) {
      (*it)->sendJsonMessage(message);
    }
  } else {
    LOG4CXX_ERROR(mb_logger_, ("No subscribers for this property!\n"));
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
  WebsocketSession* ws;
  std::map<std::string, WebsocketSession*>::iterator it;
  sync_primitives::AutoLock request_lock(mRequestListLock);

  std::string id = message["id"].asString();
  it = mRequestList.find(id);
  if (it != mRequestList.end()) {
    ws = it->second;
    ws->sendJsonMessage(message);
    mRequestList.erase(it);
  }
}

void CMessageBrokerController::sendJsonMessage(Json::Value& message) {
  if (isNotification(message)) {
    sendNotification(message);
    return;
  } else if (isResponse(message)) {
    sendResponse(message);
    return;
  }

  // Send request
  WebsocketSession* ws;
  std::map<std::string, WebsocketSession*>::iterator it;
  std::string method = message["method"].asString();
  std::string component_name = GetComponentName(method);

  sync_primitives::AutoLock lock(mControllersListLock);
  it = mControllersList.find(component_name);
  if (it != mControllersList.end()) {
    ws = it->second;
    ws->sendJsonMessage(message);
  }
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
  shutdown_ = true;
  mConnectionListLock.Acquire();
  std::vector<std::shared_ptr<hmi_message_handler::WebsocketSession> >::iterator
      it;
  for (it = mConnectionList.begin(); it != mConnectionList.end();
       it = mConnectionList.erase(it)) {
    if (*it) {
      (*it)->Shutdown();
    }
  }
  mConnectionListLock.Release();

  boost::system::error_code ec;
  socket_.close();
  acceptor_.cancel(ec);
  if (ec) {
    std::string str_err = "ErrorMessage Cancel: " + ec.message();
    LOG4CXX_ERROR(mb_logger_, str_err);
  }
  acceptor_.close(ec);
  if (ec) {
    std::string str_err = "ErrorMessage Close: " + ec.message();
  }
  ioc_.stop();
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

bool CMessageBrokerController::addController(WebsocketSession* ws_session,
                                             std::string name) {
  bool result = false;
  std::map<std::string, WebsocketSession*>::iterator it;

  sync_primitives::AutoLock lock(mControllersListLock);
  it = mControllersList.find(name);
  if (it == mControllersList.end()) {
    mControllersList.insert(
        std::map<std::string, WebsocketSession*>::value_type(name, ws_session));
    result = true;
  } else {
    LOG4CXX_ERROR(mb_logger_, ("Controller already exists!\n"));
  }
  return result;
}

void CMessageBrokerController::deleteController(WebsocketSession* ws_session) {
  {
    sync_primitives::AutoLock lock(mControllersListLock);
    std::map<std::string, WebsocketSession*>::iterator it;
    for (it = mControllersList.begin(); it != mControllersList.end();) {
      if (it->second == ws_session) {
        mControllersList.erase(it++);
      } else {
        it++;
      }
    }
  }
  removeSubscribersBySession(ws_session);
}

void CMessageBrokerController::deleteController(std::string name) {
  std::map<std::string, WebsocketSession*>::iterator it;
  WebsocketSession* ws;
  {
    sync_primitives::AutoLock lock(mControllersListLock);
    it = mControllersList.find(name);
    if (it != mControllersList.end()) {
      ws = it->second;
      mControllersList.erase(it);
    } else {
      return;
    }
  }
  removeSubscribersBySession(ws);
}

void CMessageBrokerController::removeSubscribersBySession(
    const WebsocketSession* ws) {
  sync_primitives::AutoLock lock(mSubscribersListLock);
  std::multimap<std::string, WebsocketSession*>::iterator it_s =
      mSubscribersList.begin();
  for (; it_s != mSubscribersList.end();) {
    if (it_s->second == ws) {
      mSubscribersList.erase(it_s++);
    } else {
      ++it_s;
    }
  }
}

void CMessageBrokerController::pushRequest(Json::Value& message,
                                           WebsocketSession* ws_session) {
  sync_primitives::AutoLock lock(mRequestListLock);
  std::string id = message["id"].asString();
  mRequestList.insert(
      std::map<std::string, WebsocketSession*>::value_type(id, ws_session));
}

bool CMessageBrokerController::addSubscriber(WebsocketSession* ws_session,
                                             std::string name) {
  bool result = true;
  sync_primitives::AutoLock lock(mSubscribersListLock);
  std::pair<std::multimap<std::string, WebsocketSession*>::iterator,
            std::multimap<std::string, WebsocketSession*>::iterator> p =
      mSubscribersList.equal_range(name);
  if (p.first != p.second) {
    std::multimap<std::string, WebsocketSession*>::iterator itr;
    for (itr = p.first; itr != p.second; itr++) {
      if (ws_session == itr->second) {
        result = false;
        LOG4CXX_ERROR(mb_logger_, ("Subscriber already exists!\n"));
      }
    }
  }
  if (result) {
    mSubscribersList.insert(
        std::map<std::string, WebsocketSession*>::value_type(name, ws_session));
  }
  return result;
}

void CMessageBrokerController::deleteSubscriber(WebsocketSession* ws,
                                                std::string name) {
  sync_primitives::AutoLock lock(mSubscribersListLock);
  std::pair<std::multimap<std::string, WebsocketSession*>::iterator,
            std::multimap<std::string, WebsocketSession*>::iterator> p =
      mSubscribersList.equal_range(name);
  if (p.first != p.second) {
    std::multimap<std::string, WebsocketSession*>::iterator itr;
    for (itr = p.first; itr != p.second;) {
      if (ws == itr->second) {
        mSubscribersList.erase(itr++);
      } else {
        ++itr;
      }
    }
  }
}

int CMessageBrokerController::getSubscribersFd(
    std::string name, std::vector<WebsocketSession*>& result) {
  int res = 0;
  std::map<std::string, WebsocketSession*>::iterator it;

  sync_primitives::AutoLock lock(mSubscribersListLock);
  std::pair<std::multimap<std::string, WebsocketSession*>::iterator,
            std::multimap<std::string, WebsocketSession*>::iterator> p =
      mSubscribersList.equal_range(name);
  if (p.first != p.second) {
    std::multimap<std::string, WebsocketSession*>::iterator itr;
    for (itr = p.first; itr != p.second; itr++) {
      result.push_back(itr->second);
    }
  }

  res = result.size();
  return res;
}

void CMessageBrokerController::processInternalRequest(
    Json::Value& message, WebsocketSession* ws_session) {
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
        ws_session->sendJsonMessage(response);
      } else {
        Json::Value error, err;
        error["id"] = message["id"];
        error["jsonrpc"] = "2.0";
        err["code"] = CONTROLLER_EXISTS;
        err["message"] = "Controller has been already registered.";
        error["error"] = err;
        ws_session->sendJsonMessage(error);
      }
    } else {
      Json::Value error, err;
      error["id"] = message["id"];
      error["jsonrpc"] = "2.0";
      err["code"] = INVALID_REQUEST;
      err["message"] = "Wrong method parameter.";
      error["error"] = err;
      ws_session->sendJsonMessage(error);
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
        ws_session->sendJsonMessage(response);
      } else {
        Json::Value error, err;
        error["id"] = message["id"];
        error["jsonrpc"] = "2.0";
        err["code"] = CONTROLLER_EXISTS;
        err["message"] = "Subscribe has been already registered.";
        error["error"] = err;
        ws_session->sendJsonMessage(error);
      }
    } else {
      Json::Value error, err;
      error["id"] = message["id"];
      error["jsonrpc"] = "2.0";
      err["code"] = INVALID_REQUEST;
      err["message"] = "Wrong method parameter.";
      error["error"] = err;
      ws_session->sendJsonMessage(error);
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
      ws_session->sendJsonMessage(response);
    } else {
      Json::Value error, err;
      error["id"] = message["id"];
      error["jsonrpc"] = "2.0";
      err["code"] = INVALID_REQUEST;
      err["message"] = "Wrong method parameter.";
      error["error"] = err;
      ws_session->sendJsonMessage(error);
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
      ws_session->sendJsonMessage(response);
    } else {
      Json::Value error, err;
      error["id"] = message["id"];
      error["jsonrpc"] = "2.0";
      err["code"] = INVALID_REQUEST;
      err["message"] = "Wrong method parameter.";
      error["error"] = err;
      ws_session->sendJsonMessage(error);
    }
  } else {
  }
}

int CMessageBrokerController::getNextControllerId() {
  return 1000 * mControllersIdCounter++;
}
}
