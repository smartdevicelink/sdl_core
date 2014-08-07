/**
 * \file mb_tcpserver.cpp
 * \brief MessageBroker TCP server.
 * \author AKara
 */

#include <cstring>
#include <cerrno>
#include <iostream>
#include <algorithm>
#include <vector>
#include <assert.h>

#include "MBDebugHelper.h"

#include "mb_tcpserver.hpp"
#include "CMessageBroker.hpp"

namespace NsMessageBroker {

TcpServer::TcpServer(const std::string& address, uint16_t port, NsMessageBroker::CMessageBroker* pMessageBroker) :
  Server(address, port) {
  m_protocol = networking::TCP;
  mpMessageBroker = pMessageBroker;
}

TcpServer::~TcpServer() {
  if (m_sock != -1) {
    Close();
  }
}

ssize_t TcpServer::Send(int fd, const std::string& data) {
  DBG_MSG(("Send to %d: %s\n", fd, data.c_str()));
  std::string rep = data;
  if (isWebSocket(fd)) {
    unsigned char buf[10] = {'\0'};
    ssize_t headerlen = mWebSocketHandler.prepareWebSocketDataHeader(
                          (unsigned char*)buf, (unsigned long)rep.length());
    std::string header = std::string((char*)buf, headerlen);
    rep = header + rep;
  }
  int bytesToSend = rep.length();
  const char* ptrBuffer = rep.c_str();
  do {
    int retVal = send(fd, ptrBuffer, bytesToSend, 0);
    if (retVal == -1) {
      return -1;
    }
    bytesToSend -= retVal;
    ptrBuffer += retVal;
  } while (bytesToSend > 0);
  return rep.length();
}

bool TcpServer::Recv(int fd) {
  DBG_MSG(("TcpServer::Recv(int fd)\n"));
  ssize_t nb = -1;

  std::string* pReceivingBuffer = getBufferFor(fd);
  std::vector<char> buf;
  buf.reserve(RECV_BUFFER_LENGTH + pReceivingBuffer->size());
  DBG_MSG(("Left in  pReceivingBuffer: %d : %s\n",
           pReceivingBuffer->size(), pReceivingBuffer->c_str()));
  buf.assign(pReceivingBuffer->c_str(),
             pReceivingBuffer->c_str() + pReceivingBuffer->size());
  buf.resize(RECV_BUFFER_LENGTH + pReceivingBuffer->size());
  nb = recv(fd, &buf[pReceivingBuffer->size()], MAX_RECV_DATA, 0);
  DBG_MSG(("Recieved %d from %d\n", nb, fd));
  nb += pReceivingBuffer->size();
  DBG_MSG(("Recieved with buffer %d from %d\n", nb, fd));

  if (nb > 0) {
    unsigned int recieved_data = nb;
    if (isWebSocket(fd)) {
      const unsigned int data_length =
        mWebSocketHandler.parseWebSocketDataLength(&buf[0], recieved_data);

      DBG_MSG(("Received %d actual data length %d\n",
               recieved_data, data_length));

      if (data_length > recieved_data) {
        DBG_MSG_ERROR(("Received %d actual data length %d\n",
                       recieved_data, data_length));
        DBG_MSG_ERROR(("Incomplete message"));
        *pReceivingBuffer = std::string(&buf[0], nb);
        return false;
      }
      unsigned int b_size = static_cast<unsigned int>(nb);
      mWebSocketHandler.parseWebSocketData(&buf[0], b_size);
      nb = b_size;
    }

    *pReceivingBuffer = std::string(&buf[0], nb);
    DBG_MSG(("pReceivingBuffer before onMessageReceived:%d : %s",
             pReceivingBuffer->size(), pReceivingBuffer->c_str()));
    // we need to check websocket clients here
    if (!checkWebSocketHandShake(fd, pReceivingBuffer))
    { //JSON MESSAGE received. Send data in CMessageBroker.
      if (mpMessageBroker) {
        mpMessageBroker->onMessageReceived(fd, *pReceivingBuffer);
      } else {
        return false;
      }
    } else { // client is a websocket
      std::string handshakeResponse =
        "HTTP/1.1 101 Switching Protocols\r\nUpgrade: WebSocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ";
      ssize_t webSocketKeyPos = pReceivingBuffer->find("Sec-WebSocket-Key: ");
      if (-1 != webSocketKeyPos) {
        std::string wsKey = pReceivingBuffer->substr(webSocketKeyPos + 19, 24);
        mWebSocketHandler.handshake_0405(wsKey);
        handshakeResponse += wsKey;
        handshakeResponse += "\r\n\r\n";
        pReceivingBuffer->clear();
        std::list<int>::iterator acceptedClientIt = find(m_AcceptedClients.begin(), m_AcceptedClients.end(), fd);
        if (m_AcceptedClients.end() != acceptedClientIt) {
          m_AcceptedClients.erase(acceptedClientIt);
        }
        Send(fd, handshakeResponse);
        m_WebSocketClients.push_back(fd);
      }
    }

    return true;
  }
  else {
    m_purge.push_back(fd);
    return false;
  }
}

bool TcpServer::checkWebSocketHandShake(int fd, std::string* pReceivingBuffer) {
  bool result = false;
  std::list<int>::iterator acceptedClientIt = find(m_AcceptedClients.begin(), m_AcceptedClients.end(), fd);
  if (m_AcceptedClients.end() != acceptedClientIt) {
    ssize_t httpheader = pReceivingBuffer->find("GET / HTTP/1.1");
    if (-1 != httpheader) { // here is a header
      DBG_MSG(("HTTP header detected!\n"));
      result = true;
    } else { // not winsocket client
      m_AcceptedClients.erase(acceptedClientIt);
    }
  }
  DBG_MSG(("TcpServer::checkWebSocket(): %d!\n", result));
  return result;
}

bool TcpServer::isWebSocket(int fd) {
  bool result = false;
  std::list<int>::iterator wsClientIt = find(m_WebSocketClients.begin(), m_WebSocketClients.end(), fd);
  if (m_WebSocketClients.end() != wsClientIt) {
    result = true;
  }
  return result;
}

std::string* TcpServer::getBufferFor(int fd) {
  std::string* res = 0;
  std::map <int, std::string*>::iterator it;
  it = m_receivingBuffers.find(fd);
  if (it != m_receivingBuffers.end()) {
    res = (*it).second;
  } else { // create a new buffer...
    res = new std::string("");
    printf("getBufferFor method!\n");
    m_receivingBuffers.insert(std::map<int, std::string*>::value_type(fd, res));
  }

  return res;
}

void TcpServer::WaitMessage(uint32_t ms) {
  fd_set fdsr;
  struct timeval tv;
  int max_sock = m_sock;

  tv.tv_sec = ms / 1000;
  tv.tv_usec = (ms % 1000) / 1000;

  FD_ZERO(&fdsr);

#ifdef _WIN32
  /* on Windows, a socket is not an int but a SOCKET (unsigned int) */
  FD_SET((SOCKET)m_sock, &fdsr);
#else
  FD_SET(m_sock, &fdsr);
#endif

  for (std::map<int, std::string*>::iterator it = m_receivingBuffers.begin();
       it != m_receivingBuffers.end() ; it++) {
#ifdef _WIN32
    FD_SET((SOCKET)((*it).first), &fdsr);
#else
    FD_SET(((*it).first), &fdsr);
#endif

    if (((*it).first) > max_sock) {
      max_sock = ((*it).first);
    }
  }

  max_sock++;

  if (select(max_sock, &fdsr, NULL, NULL, ms ? &tv : NULL) > 0) {
    if (FD_ISSET(m_sock, &fdsr)) {
      Accept();
    }

    for (std::map<int, std::string*>::iterator it = m_receivingBuffers.begin();
         it != m_receivingBuffers.end() ; it++) {
      if (FD_ISSET(((*it).first), &fdsr)) {
        Recv((*it).first);
      }
    }

    /* remove disconnect socket descriptor */
    for (std::list<int>::iterator it = m_purge.begin();
         it != m_purge.end() ; it++) {
      std::map <int, std::string*>::iterator itr;
      itr = m_receivingBuffers.find((*it));
      if (itr != m_receivingBuffers.end())
      { // delete receiving buffer of disconnected client
        delete(*itr).second;
        m_receivingBuffers.erase(itr);
      }
    }

    /* purge disconnected list */
    m_purge.erase(m_purge.begin(), m_purge.end());
  }
  else {
    /* error */
  }
}

bool TcpServer::Listen() const {
  if (m_sock == -1) {
    return false;
  }

  if (listen(m_sock, 5) == -1) {
    return false;
  }

  return true;
}

bool TcpServer::Accept() {

  int client = -1;
  socklen_t addrlen = sizeof(struct sockaddr_storage);

  if (m_sock == -1) {
    return false;
  }

  client = accept(m_sock, 0, &addrlen);

  if (client == -1) {
    return false;
  }

  std::string* res = new std::string("");
  m_receivingBuffers.insert(std::map<int, std::string*>::value_type(client, res));
  m_AcceptedClients.push_back(client);
  return true;
}

void TcpServer::Close() {
  /* close all client sockets */
  for (std::map<int, std::string*>::iterator it = m_receivingBuffers.begin();
       it != m_receivingBuffers.end() ; it++) {
    ::close((*it).first);
    if ((*it).second) {
      delete(*it).second;
    }
  }
  m_receivingBuffers.clear();
  Server::Close();
  /* listen socket should be closed in Server destructor */
}

void* TcpServer::MethodForThread(void* arg) {
  arg = arg;
  while (1) {
    WaitMessage(1000);
  }
  return NULL;
}

} /* namespace NsMessageBroker */
