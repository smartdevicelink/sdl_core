/**
 * \file mb_server.cpp
 * \brief MessageBroker server.
 * \author AKara
 */

#include "mb_server.hpp"

namespace NsMessageBroker {

Server::Server(const std::string& address, uint16_t port) {
  m_sock = -1;
  m_address = address;
  m_port = port;
}

Server::~Server() {
  if (m_sock != -1) {
    Close();
  }
}

int Server::GetSocket() const {
  return m_sock;
}

std::string Server::GetAddress() const {
  return m_address;
}

uint16_t Server::GetPort() const {
  return m_port;
}

bool Server::Bind() {
  m_sock = networking::bind(m_protocol, m_address, m_port, NULL, NULL);

  return (m_sock != -1) ? true : false;
}

void Server::Close() {
  ::close(m_sock);
  m_sock = -1;
}

} /* namespace NsMessageBroker */
