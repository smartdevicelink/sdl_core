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

#include "hmi_message_handler/websocket_session.h"
#include <unistd.h>
#include "hmi_message_handler/mb_controller.h"
#include "utils/jsoncpp_reader_wrapper.h"

using namespace boost::beast::websocket;
namespace hmi_message_handler {

SDL_CREATE_LOG_VARIABLE("HMIMessageHandler")

WebsocketSession::WebsocketSession(boost::asio::ip::tcp::socket socket,
                                   CMessageBrokerController* controller)
    : ws_(std::move(socket))
    , strand_(ws_.get_executor())
    , controller_(controller)
    , stop(false)
    , m_receivingBuffer("")
    , mControllersIdStart(-1)
    , mControllersIdCurrent(0)
    , shutdown_(false)
    , thread_delegate_(new LoopThreadDelegate(&message_queue_, this))
    , thread_(threads::CreateThread("WS Async Send", thread_delegate_)) {
  m_writer["indentation"] = "";
  thread_->Start(threads::ThreadOptions());
}

WebsocketSession::~WebsocketSession() {}

void WebsocketSession::Accept() {
  ws_.async_accept(boost::asio::bind_executor(
      strand_,
      std::bind(
          &WebsocketSession::Recv, shared_from_this(), std::placeholders::_1)));
}

void WebsocketSession::Shutdown() {
  shutdown_ = true;
  thread_delegate_->SetShutdown();
  thread_->Stop(threads::Thread::kThreadSoftStop);
  delete thread_delegate_;
  threads::DeleteThread(thread_);
}

bool WebsocketSession::IsShuttingDown() {
  return shutdown_;
}

void WebsocketSession::Recv(boost::system::error_code ec) {
  if (shutdown_) {
    return;
  }

  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    SDL_LOG_ERROR(str_err);
    shutdown_ = true;
    thread_delegate_->SetShutdown();
    controller_->deleteController(*this);
    return;
  }

  ws_.async_read(buffer_,
                 boost::asio::bind_executor(strand_,
                                            std::bind(&WebsocketSession::Read,
                                                      shared_from_this(),
                                                      std::placeholders::_1,
                                                      std::placeholders::_2)));
}

void WebsocketSession::Send(const std::string& message,
                            Json::Value& json_message) {
  if (shutdown_) {
    return;
  }
  std::shared_ptr<std::string> message_ptr =
      std::make_shared<std::string>(message);
  message_queue_.push(message_ptr);
}

void WebsocketSession::sendJsonMessage(Json::Value& message) {
  const std::string str_msg = Json::writeString(m_writer, message) + '\n';
  sync_primitives::AutoLock auto_lock(queue_lock_);
  if (!isNotification(message) && !isResponse(message)) {
    mWaitResponseQueue.insert(std::map<std::string, std::string>::value_type(
        message["id"].asString(), message["method"].asString()));
  }

  Send(str_msg, message);
}

void WebsocketSession::Read(boost::system::error_code ec,
                            std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);
  if (ec) {
    std::string str_err = "ErrorMessage: " + ec.message();
    SDL_LOG_ERROR(str_err);
    shutdown_ = true;
    thread_delegate_->SetShutdown();
    controller_->deleteController(*this);
    buffer_.consume(buffer_.size());
    return;
  }

  std::string data = boost::beast::buffers_to_string(buffer_.data());
  m_receivingBuffer += data;

  utils::JsonReader reader;
  Json::Value root;

  if (!reader.parse(data, &root)) {
    SDL_LOG_ERROR("Invalid JSON Message.");
    return;
  }

  const std::string wmes = Json::writeString(m_receiver_writer, root);
  ssize_t beginpos = m_receivingBuffer.find(wmes);
  if (-1 != beginpos) {
    m_receivingBuffer.erase(0, beginpos + wmes.length());
  } else {
    m_receivingBuffer.clear();
  }
  onMessageReceived(root);

  buffer_.consume(buffer_.size());

  Recv(ec);
}

std::string WebsocketSession::GetComponentName(std::string& method) {
  std::string return_string = "";
  if (method != "") {
    int position = method.find(".");
    if (position != -1) {
      return_string = method.substr(0, position);
    }
  }
  return return_string;
}

void WebsocketSession::onMessageReceived(Json::Value message) {
  // Determine message type and process...
  Json::Value error;
  if (checkMessage(message, error)) {
    if (isNotification(message)) {
      controller_->processNotification(message);
    } else if (isResponse(message)) {
      std::string id = message["id"].asString();
      std::string method = findMethodById(id);
      if ("" != method) {
        if ("MB.registerComponent" == method) {
          if (message.isMember("result") && message["result"].isInt()) {
            mControllersIdStart = message["result"].asInt();
          } else {
            SDL_LOG_ERROR("Not possible to initialize mControllersIdStart!");
          }
        } else if ("MB.subscribeTo" == method ||
                   "MB.unregisterComponent" == method ||
                   "MB.unsubscribeFrom" == method) {
          // nothing to do for now
        } else {
          controller_->processResponse(method, message);
        }
      } else {
        SDL_LOG_ERROR("Request with id: " + id + " has not been found!");
      }
    } else {
      std::string method = message["method"].asString();
      std::string component_name = GetComponentName(method);

      if (component_name == "MB") {
        controller_->processInternalRequest(message, *this);
      } else {
        controller_->pushRequest(message, *this);
        controller_->processRequest(message);
      }
    }
  } else {
    SDL_LOG_ERROR("Message contains wrong data!\n");
    sendJsonMessage(error);
  }
}

bool WebsocketSession::isNotification(Json::Value& root) {
  bool ret = false;
  if (false == root.isMember("id")) {
    ret = true;
  }
  return ret;
}

bool WebsocketSession::isResponse(Json::Value& root) {
  bool ret = false;
  if ((true == root.isMember("result")) || (true == root.isMember("error"))) {
    ret = true;
  }
  return ret;
}

std::string WebsocketSession::findMethodById(std::string id) {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  std::string res = "";
  std::map<std::string, std::string>::iterator it;
  it = mWaitResponseQueue.find(id);
  if (it != mWaitResponseQueue.end()) {
    res = (*it).second;
    mWaitResponseQueue.erase(it);
  }
  return res;
}

bool WebsocketSession::checkMessage(Json::Value& root, Json::Value& error) {
  Json::Value err;
  /* check the JSON-RPC version => 2.0 */
  if (!root.isObject() || !root.isMember("jsonrpc") ||
      root["jsonrpc"] != "2.0") {
    error["id"] = Json::Value::null;
    error["jsonrpc"] = "2.0";
    err["code"] = hmi_message_handler::INVALID_REQUEST;
    err["message"] = "Invalid MessageBroker request.";
    error["error"] = err;
    return false;
  }

  if (root.isMember("id") && (root["id"].isArray() || root["id"].isObject())) {
    error["id"] = Json::Value::null;
    error["jsonrpc"] = "2.0";
    err["code"] = hmi_message_handler::INVALID_REQUEST;
    err["message"] = "Invalid MessageBroker request.";
    error["error"] = err;
    return false;
  }

  if (root.isMember("result") && root.isMember("error")) {
    /* message can't contain simultaneously result and error*/
    return false;
  }

  if (root.isMember("method")) {
    if (!root["method"].isString()) {
      error["id"] = Json::Value::null;
      error["jsonrpc"] = "2.0";
      err["code"] = hmi_message_handler::INVALID_REQUEST;
      err["message"] = "Invalid MessageBroker request.";
      error["error"] = err;
      return false;
    }
    /* Check the params is  an object*/
    if (root.isMember("params") && !root["params"].isObject()) {
      error["id"] = Json::Value::null;
      error["jsonrpc"] = "2.0";
      err["code"] = INVALID_REQUEST;
      err["message"] = "Invalid JSONRPC params.";
      error["error"] = err;
      return false;
    }
  } else if (!root.isMember("result") && !root.isMember("error")) {
    return false;
  }
  return true;
}

WebsocketSession::LoopThreadDelegate::LoopThreadDelegate(
    MessageQueue<Message, AsyncQueue>* message_queue, WebsocketSession* handler)
    : message_queue_(*message_queue), handler_(*handler), shutdown_(false) {}

void WebsocketSession::LoopThreadDelegate::threadMain() {
  while (!message_queue_.IsShuttingDown() && !shutdown_) {
    DrainQueue();
    message_queue_.wait();
  }
  DrainQueue();
}

void WebsocketSession::LoopThreadDelegate::exitThreadMain() {
  shutdown_ = true;
  if (!message_queue_.IsShuttingDown()) {
    message_queue_.Shutdown();
  }
}

void WebsocketSession::LoopThreadDelegate::DrainQueue() {
  Message message_ptr;
  while (!shutdown_ && message_queue_.pop(message_ptr)) {
    boost::system::error_code ec;
    handler_.ws_.write(boost::asio::buffer(*message_ptr), ec);
    if (ec) {
      SDL_LOG_ERROR("A system error has occurred: " << ec.message());
    }
  }
}

void WebsocketSession::LoopThreadDelegate::SetShutdown() {
  shutdown_ = true;
  if (!message_queue_.IsShuttingDown()) {
    message_queue_.Shutdown();
  }
}
}  // namespace hmi_message_handler
