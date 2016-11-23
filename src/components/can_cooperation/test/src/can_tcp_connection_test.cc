/*
 * Copyright (c) 2015, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include "gtest/gtest.h"
#include "can_cooperation/can_tcp_connection.h"
#include "mock_can_connection.h"

namespace can_cooperation {

class TCPServer {
 public:
  TCPServer(const std::string& address, int port);
  ~TCPServer();
  void Start();
  void WaitConnect();
  void Stop();
  bool Send(const std::string& message);
  bool Receive(std::string* message);

 private:
  static void* Listen(void* self_object);
  std::string address_;
  int port_;
  int socket_;
  int client_socket_;
  pthread_t thread_;
  pthread_mutex_t mutex_;
};

TCPServer::TCPServer(const std::string& address, int port)
    : address_(address)
    , port_(port)
    , socket_(-1)
    , client_socket_(-1)
    , thread_(0)
    , mutex_(PTHREAD_MUTEX_INITIALIZER) {
  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(address_.c_str());
  server_addr.sin_port = htons(port_);

  if (-1 ==
      bind(socket_, (struct sockaddr*)&server_addr, sizeof(server_addr))) {
    printf("Failed to bind\n");
  }
}

TCPServer::~TCPServer() {
  close(socket_);
}

void* TCPServer::Listen(void* self_object) {
  TCPServer* self = static_cast<TCPServer*>(self_object);
  pthread_mutex_unlock(&self->mutex_);
  if (-1 == listen(self->socket_, 1)) {
    printf("failed to listen\n");
    return (void*)-1;
  }
  struct sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  self->client_socket_ =
      accept(self->socket_, (struct sockaddr*)&client_addr, &client_addr_size);
  if (-1 == self->client_socket_) {
    printf("Failed to accept\n");
    close(self->socket_);
    return (void*)-1;
  }
  return NULL;
}

void TCPServer::Start() {
  pthread_mutex_lock(&mutex_);
  int result = pthread_create(&thread_, 0, &TCPServer::Listen, this);
  if (result != 0) {
    printf("Result of thread creation is %d\n", result);
  }
  pthread_mutex_lock(&mutex_);
  pthread_mutex_unlock(&mutex_);
}

void TCPServer::WaitConnect() {
  int result = pthread_join(thread_, 0);
  if (result != 0) {
    printf("Join result is %d\n", result);
  }
}

void TCPServer::Stop() {
  close(client_socket_);
}

bool TCPServer::Send(const std::string& message) {
  if (-1 == write(client_socket_, message.c_str(), message.size())) {
    return false;
  }
  return true;
}

bool TCPServer::Receive(std::string* message) {
  char buf[500] = {0};
  if (-1 == read(client_socket_, buf, sizeof(buf))) {
    return false;
  }
  *message = buf;
  return true;
}

class TcpConnectionTest {
 public:
  TcpConnectionTest(CANTCPConnection& conn) : conn_(conn) {}
  ConnectionState state() const {
    return conn_.current_state_;
  }

 private:
  CANTCPConnection& conn_;
};

TEST(CanTcpConnectionTest, DISABLED_OpenClose) {
  TCPServer server("127.0.0.1", 8092);
  server.Start();
  sleep(4);
  CANTCPConnection conn;
  MockCANConnectionObserver mock;
  conn.set_observer(&mock);
  TcpConnectionTest test(conn);
  ASSERT_EQ(ConnectionState::OPENED, test.state());
  server.WaitConnect();

  Json::Value value;
  value["model"] = "Mustang";
  value["data"] = Json::Value(Json::ValueType::objectValue);
  value["data"]["version"] = 1.0;
  value["data"]["series"] = "A";
  value["data"]["additional"] = 987;

  conn.SendMessage(value);
  ASSERT_EQ(ConnectionState::OPENED, test.state());
  std::string server_msg;
  EXPECT_TRUE(server.Receive(&server_msg));
  Json::FastWriter writer;
  std::string result = writer.write(value);
  ASSERT_EQ(result, server_msg);

  value["model"] = "Mustang";
  value["data"] = Json::Value(Json::ValueType::objectValue);
  value["data"]["version"] = 2.0;
  value["data"]["series"] = "B";

  result = writer.write(value);
  server.Send(result);
  Json::Value received_msg;
  ASSERT_EQ(ConnectionState::OPENED, conn.ReadMessage(&received_msg));
  ASSERT_EQ(ConnectionState::OPENED, test.state());
  std::string client_result = writer.write(received_msg);
  ASSERT_EQ(result, client_result);

  server.Stop();
  sleep(2);
  EXPECT_EQ(ConnectionState::CLOSED, conn.SendMessage(received_msg));
}

}  // namespace can_cooperation
