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

#ifndef WEBSOCKET_SESSION_H
#define WEBSOCKET_SESSION_H

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
#include <mutex>
#include <queue>
#include "json/json.h"
#include "utils/macro.h"
#include "utils/lock.h"
#include "utils/atomic_object.h"
#include "utils/threads/thread.h"
#include "utils/threads/message_loop_thread.h"
#include "utils/message_queue.h"

using namespace boost::beast::websocket;
using ::utils::MessageQueue;

#ifdef DEBUG_ON
/**
* \def DBG_MSG
* \brief Debug message output with file name and line number.
* \param x formatted debug message.
* \return printf construction.
*/
#define DBG_MSG(x)                      \
  printf("%s:%d ", __FILE__, __LINE__); \
  printf x
#else
#define DBG_MSG(x)
#endif

#define DBG_MSG_ERROR(x)                         \
  printf("ERROR!!! %s:%d ", __FILE__, __LINE__); \
  printf x

typedef std::queue<std::shared_ptr<std::string> > AsyncQueue;
typedef std::shared_ptr<std::string> Message;

namespace hmi_message_handler {

CREATE_LOGGERPTR_GLOBAL(ws_logger_, "HMIMessageHandler")

class CMessageBrokerController;

class WebsocketSession : public std::enable_shared_from_this<WebsocketSession> {
  boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;
  boost::beast::multi_buffer buffer_;
  boost::beast::multi_buffer send_buffer_;
  CMessageBrokerController* controller_;

 public:
  WebsocketSession(boost::asio::ip::tcp::socket socket,
                   CMessageBrokerController* controller);

  ~WebsocketSession();

  void Accept();

  void Shutdown();

  bool IsShuttingDown();

  void Recv(boost::system::error_code ec);

  void Send(std::string& message, Json::Value& json_message);

  void SendFromQueue();

  void sendJsonMessage(Json::Value& message);

  void OnWrite(boost::system::error_code ec,
               std::size_t bytes_transferred,
               std::shared_ptr<std::string> message);

  void Read(boost::system::error_code ec, std::size_t bytes_transferred);

  int getNextMessageId();

  void prepareMessage(Json::Value& root);

  void prepareErrorMessage(int errCode,
                           std::string errMessage,
                           Json::Value& error);

  std::string getDestinationComponentName(Json::Value& root);

  bool isNotification(Json::Value& root);

  bool isResponse(Json::Value& root);

  std::string findMethodById(std::string id);

  void registerController(int id = 0);

  void unregisterController();

  void subscribeTo(std::string property);

  void unsubscribeFrom(std::string property);

  bool checkMessage(Json::Value& root, Json::Value& error);

  std::string getControllersName();

  std::string GetComponentName(std::string& method);

 protected:
  sync_primitives::atomic_bool stop;

 private:
  void onMessageReceived(Json::Value message);

  std::map<std::string, std::string> mWaitResponseQueue;

  std::string m_receivingBuffer;

  int mControllersIdStart;

  int mControllersIdCurrent;

  Json::Reader m_reader;
  Json::FastWriter m_writer;
  Json::FastWriter m_receiverWriter;

  sync_primitives::Lock queue_lock_;
  sync_primitives::Lock message_queue_lock_;
  std::atomic_bool shutdown_;

  MessageQueue<Message, AsyncQueue> message_queue_;

  class LoopThreadDelegate : public threads::ThreadDelegate {
   public:
    LoopThreadDelegate(MessageQueue<Message, AsyncQueue>* message_queue,
                       WebsocketSession* handler);

    virtual void threadMain() OVERRIDE;
    virtual void exitThreadMain() OVERRIDE;

    void OnWrite();

    void SetShutdown();

   private:
    void DrainQueue();
    MessageQueue<Message, AsyncQueue>& message_queue_;
    WebsocketSession& handler_;
    sync_primitives::Lock queue_lock_;
    sync_primitives::ConditionalVariable queue_new_items_;
    std::atomic_bool write_pending_;
    std::atomic_bool shutdown_;

    sync_primitives::Lock write_lock_;
  };

  LoopThreadDelegate* thread_delegate_;
  threads::Thread* thread_;
};

}  // hmi_message_handler

#endif /* WEBSOCKET_SESSION_H */