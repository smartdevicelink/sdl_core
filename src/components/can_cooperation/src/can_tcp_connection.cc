#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include "./can_tcp_connection.h"
#include "json/json.h"
#include "utils/logger.h"

namespace can_cooperation {

CREATE_LOGGERPTR_GLOBAL(logger_, "CANTCPConnection");

CANTCPConnection::CANTCPConnection()
: address_("127.0.0.1")
, port_(8090)
, socket_(-1)
, current_state_(NONE) {
	socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == socket_) {
		current_state_ = INVALID;
	}
  std::ifstream in("./plugins/can_config.json");
  if (in.is_open()) {
    Json::Reader reader;
    Json::Value value;
    if (reader.parse(in, value, false)) {
      address_ = value["address"].asString();
      port_ = value["port"].asUInt();
    }
  }
  LOG4CXX_INFO(logger_, "Connecting to "
        << address_ << " on port " << port_);
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
    std::string data;
    const int kSize = 500;
    int read_chars = 0;
    do {
      char buf[kSize];
      read_chars = read(socket_, buf, sizeof(buf));
      switch(read_chars) {
        case 0: // closed connection
          current_state_ = CLOSED;
          break;
        case -1:  // error while reading
          current_state_ = INVALID;
          break;
        default:
          data += buf;
          break;
      }
    } while (read_chars >= kSize && OPENED == current_state_);
    if (!data.empty()) {
      received_.push_back(data);
    }
  }
  return current_state_;
}

void CANTCPConnection::WriteData(const std::string& message) {
  if (INVALID != current_state_) {
    to_send_.push_back(message);
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
