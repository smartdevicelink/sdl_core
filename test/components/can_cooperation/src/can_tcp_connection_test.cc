#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include "gtest/gtest.h"
#include "can_cooperation/can_tcp_connection.h"

using can_cooperation::CANTCPConnection;
using can_cooperation::ConnectionState;

namespace test {
namespace components {
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
};

TCPServer::TCPServer(const std::string& address, int port)
    : address_(address),
      port_(port),
      socket_(-1),
      client_socket_(-1),
      thread_(0) {
  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(address_.c_str());
  server_addr.sin_port = htons(port_);

  if (-1 == bind(socket_, (struct sockaddr *)&server_addr,
         sizeof(server_addr))) {
    printf("Failed to bind\n" );
  }
}

TCPServer::~TCPServer() {
  close(socket_);
}

void* TCPServer::Listen(void *self_object) {
  TCPServer* self = static_cast<TCPServer*>(self_object);
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

void TCPServer::Start() {
  int result = pthread_create(&thread_, 0, &TCPServer::Listen, this);
  printf("Result of thread creation is %d\n", result);
}

void TCPServer::WaitConnect() {
  int result = pthread_join(thread_, 0);
  printf("Join result is %d\n", result);
}

void TCPServer::Stop() {
  close(client_socket_);
}

bool TCPServer::Send(const std::string& message) {
  if (-1 == write(client_socket_, message.c_str(), message.size()))
    return false;
  return true;
}

bool TCPServer::Receive(std::string* message) {
  char buf[500] = {0};
  if (-1 == read(client_socket_, buf, sizeof(buf)))
    return false;
  *message = buf;
  return true;
}

TEST(CanTcpConnectionTest, OpenClose) {
  TCPServer server("127.0.0.1", 8090);
  server.Start();

  CANTCPConnection conn;
  EXPECT_EQ(ConnectionState::OPENED, conn.OpenConnection());
  server.WaitConnect();
  EXPECT_EQ(ConnectionState::CLOSED, conn.CloseConnection());

  server.Stop();
}

TEST(CanTcpConnectionTest, WriteData) {
  TCPServer server("127.0.0.1", 8090);
  server.Start();

  CANTCPConnection conn;
  EXPECT_EQ(ConnectionState::OPENED, conn.OpenConnection());
  server.WaitConnect();

  Json::Value value;
  value["model"] = "Mustang";
  value["data"] = Json::Value(Json::ValueType::objectValue);
  value["data"]["version"] = 1.0;
  value["data"]["series"] = "A";
  value["data"]["additional"] = 987;

  conn.WriteData(value.toStyledString());
  EXPECT_EQ(ConnectionState::OPENED, conn.Flash());
  std::string server_msg;
  EXPECT_TRUE(server.Receive(&server_msg));
  EXPECT_EQ(value.toStyledString(), server_msg);

  EXPECT_EQ(ConnectionState::CLOSED, conn.CloseConnection());

  server.Stop();
}

TEST(CanTcpConnectionTest, ReadData) {
  TCPServer server("127.0.0.1", 8090);
  server.Start();

  CANTCPConnection conn;
  EXPECT_EQ(ConnectionState::OPENED, conn.OpenConnection());
  server.WaitConnect();

  Json::Value value;
  value["model"] = "Mustang";
  value["data"] = Json::Value(Json::ValueType::objectValue);
  value["data"]["version"] = 1.0;
  value["data"]["series"] = "A";

  server.Send(value.toStyledString());
  EXPECT_EQ(ConnectionState::OPENED, conn.GetData());

  Json::Value received_msg = conn.ReadData();
  EXPECT_EQ(value.toStyledString(), received_msg.toStyledString());

  EXPECT_EQ(ConnectionState::CLOSED, conn.CloseConnection());

  server.Stop();
}

}  // namespace can_cooperation
}  // namespace components
}  // namespace test
