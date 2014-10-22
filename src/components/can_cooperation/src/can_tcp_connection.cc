#include "./can_tcp_connection.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

namespace can_cooperation {

CANTCPConnection::CANTCPConnection()
: address_("127.0.0.1")
, port_(8090)
, socket_(-1)
, current_state_(NONE) {
	socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == socket_) {
		current_state_ = INVALID;
	}
}

CANTCPConnection::~CANTCPConnection() {
  if (INVALID != current_state_ && CLOSED != current_state_) {
    CloseConnection();
  }
	current_state_ = NONE;
}

ConnectionState CANTCPConnection::OpenConnection() {
  if (INVALID != current_state_) {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address_.c_str());
    server_addr.sin_port = htons(port_);
    if (-1 == connect(socket_, (struct sockaddr *)&server_addr,
              sizeof(server_addr))) {
      current_state_ = INVALID;
    } else {
      current_state_ = OPENED;
    }
  }
  return current_state_;
}

ConnectionState CANTCPConnection::CloseConnection() {
  if (-1 == close(socket_))
    current_state_ = INVALID;
  else
    current_state_ = CLOSED;
  return current_state_;
}

ConnectionState CANTCPConnection::Flash() {
  if (OPENED == current_state_ && to_send_.size() > 0) {
    std::string msg = to_send_.front();
    to_send_.pop_front();
    if (-1 == write(socket_, msg.c_str(), msg.size())) {
      current_state_ = INVALID;
      CloseConnection();
    }
  }
  return current_state_;
}

ConnectionState CANTCPConnection::GetData() {
  if (OPENED == current_state_) {
    char buf[500];
    if (-1 == read(socket_, buf, sizeof(buf))) {
      current_state_ = INVALID;
      CloseConnection();
    } else {
      received_.push_back(buf);
    }
  }
  return current_state_;
}

void CANTCPConnection::WriteData(const Json::Value& message) {
  if (INVALID != current_state_) {
    Json::FastWriter writer;
    to_send_.push_back(writer.write(message));
  }
}

Json::Value CANTCPConnection::ReadData() {
  Json::Value json_value;
  if (OPENED == current_state_ && received_.size() > 0) {
    Json::Reader reader;
    if (!reader.parse(received_.front(), json_value, false)) {
      json_value = Json::Value();
    }
    received_.pop_front();
  }

  return json_value;
}

}  //  namespace can_cooperation