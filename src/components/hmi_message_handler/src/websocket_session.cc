#include "hmi_message_handler/websocket_session.h"
#include "hmi_message_handler/mb_controller.h"
#include <unistd.h>
using namespace boost::beast::websocket;
namespace hmi_message_handler {

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
  thread_->start(threads::ThreadOptions());
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
  thread_->join();
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
    LOG4CXX_ERROR(ws_logger_, str_err);
    shutdown_ = true;
    thread_delegate_->SetShutdown();
    controller_->deleteController(this);
    return;
  }

  ws_.async_read(buffer_,
                 boost::asio::bind_executor(strand_,
                                            std::bind(&WebsocketSession::Read,
                                                      shared_from_this(),
                                                      std::placeholders::_1,
                                                      std::placeholders::_2)));
}

void WebsocketSession::Send(std::string& message, Json::Value& json_message) {
  if (shutdown_) {
    return;
  }
  std::shared_ptr<std::string> message_ptr =
      std::make_shared<std::string>(message);
  message_queue_.push(message_ptr);
}

void WebsocketSession::sendJsonMessage(Json::Value& message) {
  std::string str_msg = m_writer.write(message);
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
    LOG4CXX_ERROR(ws_logger_, str_err);
    shutdown_ = true;
    thread_delegate_->SetShutdown();
    controller_->deleteController(this);
    buffer_.consume(buffer_.size());
    return;
  }

  std::string data = boost::beast::buffers_to_string(buffer_.data());
  m_receivingBuffer += data;

  Json::Value root;
  if (!m_reader.parse(m_receivingBuffer, root)) {
    std::string str_err = "Invalid JSON Message: " + data;
    LOG4CXX_ERROR(ws_logger_, str_err);
    return;
  }

  std::string wmes = m_receiverWriter.write(root);
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
            LOG4CXX_ERROR(ws_logger_,
                          "Not possible to initialize mControllersIdStart!");
          }
        } else if ("MB.subscribeTo" == method ||
                   "MB.unregisterComponent" == method ||
                   "MB.unsubscribeFrom" == method) {
          // nothing to do for now
        } else {
          controller_->processResponse(method, message);
        }
      } else {
        LOG4CXX_ERROR(ws_logger_,
                      "Request with id: " + id + " has not been found!");
      }
    } else {
      std::string method = message["method"].asString();
      std::string component_name = GetComponentName(method);

      if (component_name == "MB") {
        controller_->processInternalRequest(message, this);
      } else {
        controller_->pushRequest(message, this);
        controller_->processRequest(message);
      }
    }
  } else {
    LOG4CXX_ERROR(ws_logger_, "Message contains wrong data!\n");
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
  while (!message_queue_.empty()) {
    Message message_ptr;
    message_queue_.pop(message_ptr);
    if (!shutdown_) {
      handler_.ws_.write(boost::asio::buffer(*message_ptr));
    };
  }
}

void WebsocketSession::LoopThreadDelegate::SetShutdown() {
  shutdown_ = true;
  if (!message_queue_.IsShuttingDown()) {
    message_queue_.Shutdown();
  }
}
}
