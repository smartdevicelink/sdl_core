/*
 * Copyright (c) 2014, Ford Motor Company
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
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "utils/logger.h"
#include "media_manager/socket_streamer_adapter.h"

namespace media_manager {

CREATE_LOGGERPTR_GLOBAL(logger, "SocketStreamerAdapter")

SocketStreamerAdapter::SocketStreamerAdapter(
    const std::string& ip,
    const uint16_t port,
    const std::string& header)
  : StreamerAdapter(new SocketStreamer(this, ip, port, header)) {
}

SocketStreamerAdapter::~SocketStreamerAdapter() {
}

SocketStreamerAdapter::SocketStreamer::SocketStreamer(
    SocketStreamerAdapter* const adapter,
    const std::string& ip,
    const uint16_t port,
    const std::string& header)
  : Streamer(adapter),
    ip_(ip),
    port_(port),
    header_(header),
    socket_fd_(0),
    send_socket_fd_(0),
    is_first_frame_(true) {
}

SocketStreamerAdapter::SocketStreamer::~SocketStreamer() {
}

bool SocketStreamerAdapter::SocketStreamer::Connect() {
  LOG4CXX_AUTO_TRACE(logger);
  socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (0 >= socket_fd_) {
    LOG4CXX_ERROR(logger, "Unable to create socket");
    return false;
  }

  int32_t optval = 1;
  if (-1 == setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR,
                       &optval, sizeof optval)) {
    LOG4CXX_ERROR(logger, "Unable to set sockopt");
    return false;
  }

  struct sockaddr_in serv_addr_ = { 0 };
  serv_addr_.sin_addr.s_addr = inet_addr(ip_.c_str());
  serv_addr_.sin_family = AF_INET;
  serv_addr_.sin_port = htons(port_);
  if (-1 == bind(socket_fd_,
                 reinterpret_cast<struct sockaddr*>(&serv_addr_),
                 sizeof(serv_addr_))) {
    LOG4CXX_ERROR(logger, "Unable to bind");
    return false;
  }

  if (-1 == listen(socket_fd_, 5)) {
    LOG4CXX_ERROR(logger, "Unable to listen");
    return false;
  }

  send_socket_fd_ = accept(socket_fd_, NULL, NULL);
  if (0 >= send_socket_fd_) {
    LOG4CXX_ERROR(logger, "Unable to accept");
    return false;
  }

  is_first_frame_ = true;
  LOG4CXX_INFO(logger, "Client connected: " << send_socket_fd_);
  return true;
}

void SocketStreamerAdapter::SocketStreamer::Disconnect() {
  LOG4CXX_AUTO_TRACE(logger);
  if (0 < send_socket_fd_) {
    close(send_socket_fd_);
  }
  if (0 < socket_fd_) {
    close(socket_fd_);
  }
}

bool SocketStreamerAdapter::SocketStreamer::Send(
    protocol_handler::RawMessagePtr msg) {
  LOG4CXX_AUTO_TRACE(logger);
  ssize_t ret;
  if (is_first_frame_) {
    ret = send(send_socket_fd_, header_.c_str(),
               header_.size(), MSG_NOSIGNAL);
    if (static_cast<uint32_t>(ret) != header_.size()) {
      LOG4CXX_ERROR(logger, "Unable to send data to socket");
      return false;
    }
    is_first_frame_ = false;
  }

  ret = send(send_socket_fd_, msg->data(),
             msg->data_size(), MSG_NOSIGNAL);
  if (-1 == ret) {
    LOG4CXX_ERROR(logger, "Unable to send data to socket");
    return false;
  }

  if (static_cast<uint32_t>(ret) != msg->data_size()) {
    LOG4CXX_WARN(logger, "Couldn't send all the data to socket "
                 << send_socket_fd_);
  }

  LOG4CXX_INFO(logger, "Streamer::sent " << msg->data_size());
  return true;
}

}  // namespace media_manager
