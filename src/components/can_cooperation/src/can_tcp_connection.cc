/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "can_cooperation/can_tcp_connection.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "json/json.h"
#include "utils/logger.h"
#include "utils/threads/thread.h"
#include "utils/conditional_variable.h"
#include "functional_module/settings.h"

namespace can_cooperation {

CREATE_LOGGERPTR_GLOBAL(logger_, "CANTCPConnection")

class TCPClientDelegate : public threads::ThreadDelegate {
 public:
  explicit TCPClientDelegate(CANTCPConnection* can_connection);
  ~TCPClientDelegate();
  void threadMain();
  void exitThreadMain();

 private:
  CANTCPConnection* can_connection_;
  volatile bool stop_flag_;
  mutable sync_primitives::Lock stop_flag_lock_;
  mutable sync_primitives::ConditionalVariable stop_flag_cond_;
};

CANTCPConnection::CANTCPConnection()
    : CANConnection()
    , address_("127.0.0.1")
    , port_(8092)
    , socket_(-1)
    , current_state_(NONE)
    , thread_(NULL) {
  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == socket_) {
    current_state_ = INVALID;
  }

  functional_modules::Settings settings;
  settings.ReadParameter("Remote Control", "address", &address_);
  settings.ReadParameter("Remote Control", "port", &port_);

  LOG4CXX_INFO(logger_, "Connecting to " << address_ << " on port " << port_);
  if (OpenConnection() == ConnectionState::OPENED) {
    thread_ =
        threads::CreateThread("CANClientListener", new TCPClientDelegate(this));
    const size_t kStackSize = 16384;
    thread_->start(threads::ThreadOptions(kStackSize));
  } else {
    LOG4CXX_ERROR(logger_, "Failed to connect to CAN");
  }
}

CANTCPConnection::~CANTCPConnection() {
  if (INVALID != current_state_ && CLOSED != current_state_) {
    CloseConnection();
  }
  if (thread_) {
    thread_->stop();
    DeleteThread(thread_);
  }
  current_state_ = NONE;
}

ConnectionState CANTCPConnection::SendMessage(const CANMessage& message) {
  if (INVALID != current_state_) {
    to_send_.push_back(message);
  }
  return Flash();
}

ConnectionState CANTCPConnection::ReadMessage(CANMessage* message) {
  if (OPENED == current_state_) {
    DCHECK(message);
    if (!message) {
      LOG4CXX_ERROR(logger_, "Nul-pointer provided");
      return current_state_;
    }
    std::string data;
    const int kSize = 500;
    int read_chars = 0;
    do {
      char buf[kSize] = {0};
      read_chars = read(socket_, buf, sizeof(buf));
      switch (read_chars) {
        case 0:  // closed connection
          current_state_ = CLOSED;
          break;
        case -1:  // error while reading
          current_state_ = INVALID;
          break;
        default:
          data.append(buf, read_chars);
          break;
      }
    } while (read_chars >= kSize && OPENED == current_state_);
    if (!data.empty()) {
      Json::Reader reader;
      Json::Value value;
      if (reader.parse(data, value, false)) {
        *message = value;
      } else {
        LOG4CXX_ERROR(logger_, "Failed to parse incoming message from CAN ");
      }
    }
  }
  return current_state_;
}

ConnectionState CANTCPConnection::OpenConnection() {
  if (INVALID != current_state_) {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address_.c_str());
    server_addr.sin_port = htons(port_);
    if (-1 == connect(socket_,
                      reinterpret_cast<sockaddr*>(&server_addr),
                      sizeof(server_addr))) {
      current_state_ = INVALID;
    } else {
      current_state_ = OPENED;
    }
  }
  return current_state_;
}

ConnectionState CANTCPConnection::CloseConnection() {
  if (-1 == close(socket_)) {
    current_state_ = INVALID;
  } else {
    current_state_ = CLOSED;
  }
  return current_state_;
}

ConnectionState CANTCPConnection::Flash() {
  if (OPENED == current_state_ && to_send_.size() > 0) {
    Json::FastWriter writer;
    std::string msg = writer.write(to_send_.front());
    to_send_.pop_front();
    if (-1 == write(socket_, msg.c_str(), msg.size())) {
      current_state_ = INVALID;
      CloseConnection();
    }
  }
  return current_state_;
}

//  -------------- ThreadDelegate --------------
TCPClientDelegate::TCPClientDelegate(CANTCPConnection* can_connection)
    : can_connection_(can_connection), stop_flag_(false) {
  DCHECK(can_connection);
}

TCPClientDelegate::~TCPClientDelegate() {
  can_connection_ = NULL;
  stop_flag_ = true;
}

void TCPClientDelegate::threadMain() {
  while (!stop_flag_) {
    Json::Value message_from_can;
    while (can_connection_->ReadMessage(&message_from_can) ==
           ConnectionState::OPENED) {
      can_connection_->observer_->OnCANMessageReceived(message_from_can);
    }
    if (can_connection_->current_state_ != ConnectionState::OPENED) {
      std::string info;
      switch (can_connection_->current_state_) {
        case ConnectionState::CLOSED:
          info = "Connection was closed by CAN-bus module.";
          break;
        case ConnectionState::INVALID:
          info = "Failed to perform IO operation with CAN-bus module.";
          break;
        case ConnectionState::NONE:
          info = "Connection with CAN-bus module is not established.";
          break;
        case ConnectionState::OPENED:
        default:
          break;
      }
      can_connection_->observer_->OnCANConnectionError(
          can_connection_->current_state_, info);
    }
  }
  stop_flag_cond_.NotifyOne();
}

void TCPClientDelegate::exitThreadMain() {
  if (stop_flag_)
    return;
  sync_primitives::AutoLock run_lock(stop_flag_lock_);
  stop_flag_ = true;
  sync_primitives::ConditionalVariable::WaitStatus wait_status =
      stop_flag_cond_.WaitFor(run_lock, 10000);
  if (sync_primitives::ConditionalVariable::kTimeout == wait_status) {
    threads::ThreadDelegate::exitThreadMain();
  }
}

}  //  namespace can_cooperation
