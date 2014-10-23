#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "can_cooperation/../../src/can_tcp_connection.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

namespace can_module_testing {
using namespace can_cooperation;

class TCPServer {
public:
  TCPServer(const std::string& address, int port);
  ~TCPServer();
  static void* Start(void* self_object);
  void Stop();
  bool Send(const std::string& message);
  bool Receive(std::string* message);
private:
  std::string address_;
  int port_;
  int socket_;
  int client_socket_;
};

TCPServer::TCPServer(const std::string& address, int port)
: address_(address)
, port_(port)
, socket_(-1)
, client_socket_(-1) {
  socket_ = socket(AF_INET, SOCK_STREAM, 0);
}

TCPServer::~TCPServer() {
}

void* TCPServer::Start(void *self_object) {
  TCPServer* self = static_cast<TCPServer*>(self_object);
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(self->address_.c_str());
  server_addr.sin_port = htons(self->port_);

  if (-1 == bind(self->socket_, (struct sockaddr *)&server_addr,
         sizeof(server_addr))) {
    printf("Failed to bind\n" );
    return (void*)-1;
  }
  if (-1 == listen(self->socket_, 1)) {
    printf("failed to listen");
    return (void*)-1;
  }
  struct sockaddr_in client_addr;
  socklen_t client_addr_size = sizeof(client_addr);
  self->client_socket_ = accept(self->socket_, (struct sockaddr*) &client_addr,
    &client_addr_size);
  if (-1 == self->client_socket_) {
    printf("Failed to accept\n");
    close(self->socket_);
    return (void*) -1;
  }
  return NULL;
}

void TCPServer::Stop() {
  close(client_socket_);
  close(socket_);
}

bool TCPServer::Send(const std::string& message) {
  if (-1 == write(client_socket_, message.c_str(), message.size()))
    return false;
  return true;
}

bool TCPServer::Receive(std::string* message) {
  char buf[500];
  if (-1 == read(client_socket_, buf, sizeof(buf)))
    return false;
  *message = buf;
  return true;
}

TEST(can_module, create) {
  TCPServer server("127.0.0.1", 8090);
  pthread_t thread;
  int result = pthread_create(&thread, 0, &TCPServer::Start, &server);
  printf("Result of thread creation is %d\n", result);
  sleep(1);

  //  Test connection established
  CANTCPConnection connection;
  ASSERT_TRUE(OPENED == connection.OpenConnection());
  void * res;
  result = pthread_join(thread, &res);
  printf("Join result is %d\n", result);
  free(res);

  //  Test sending message from server to client
  Json::Value value;
  value["model"] = "Mustang";
  value["data"] = Json::Value(Json::ValueType::objectValue);
  value["data"]["version"] = 1.0;
  value["data"]["series"] = "A";
  Json::FastWriter writer;
  std::string msg = writer.write(value);
  printf("Message to be sent %s", msg.c_str());
  server.Send(writer.write(value));
  ASSERT_TRUE(OPENED == connection.GetData());
  Json::Value received_msg = connection.ReadData();
  ASSERT_TRUE(received_msg == value);

  //  Test sending message from client to server
  received_msg["data"]["additional"] = 987;
  msg = writer.write(received_msg);
  printf("Modified received message %s\n", msg.c_str());
  connection.WriteData(received_msg);
  ASSERT_TRUE(OPENED == connection.Flash());
  std::string server_msg;
  ASSERT_TRUE(server.Receive(&server_msg));
  printf("Server recieved: %s\n", server_msg.c_str());
  Json::Value server_msg_json;
  Json::Reader reader;
  reader.parse(server_msg, server_msg_json, false);
  ASSERT_TRUE(server_msg_json == received_msg);

  //  Test closing the connection
  ASSERT_TRUE(CLOSED == connection.CloseConnection());
  server.Stop();
}
}  //  namespace can_module_testing