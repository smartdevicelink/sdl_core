/**
 * Copyright (c) 2013, Ford Motor Company
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
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include "config_profile/profile.h"
#include "media_manager/socket_video_server.h"

namespace media_manager {

namespace video_stream_producer_consumer {

log4cxx::LoggerPtr SocketVideoServer::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("SocketVideoServer"));

SocketVideoServer::SocketVideoServer()
  : port_(profile::Profile::instance()->navi_server_port()),
    ip_(profile::Profile::instance()->server_address()),
    socket_(0),
    is_ready_(false),
    messages_(),
    delegate_(NULL),
    thread_(NULL) {
  delegate_ = new VideoStreamer(this);
  if (delegate_) {
    thread_ = new threads::Thread("SocketVideoServer", delegate_);
  }
}

SocketVideoServer::~SocketVideoServer() {
  delete thread_;
  thread_ = NULL;
  if (socket_ != -1) {
    ::close(socket_);
  }
}

bool SocketVideoServer::start() {
  LOG4CXX_INFO(logger_, "SocketVideoServer::start");

  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (0 >= socket_) {
    LOG4CXX_ERROR_EXT(logger_, "Server open error");
    return false;
  }

  int optval = 1;
  if (-1 == setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval)) {
    LOG4CXX_ERROR_EXT(logger_, "Unable to set sockopt");
    return false;
  }

  struct sockaddr_in serv_addr_;
  memset(&serv_addr_, 0, sizeof(serv_addr_));
  serv_addr_.sin_addr.s_addr = inet_addr(ip_.c_str());
  serv_addr_.sin_family = AF_INET;
  serv_addr_.sin_port = htons(port_);

  if (-1 == bind(socket_, reinterpret_cast<struct sockaddr*>(&serv_addr_), sizeof(serv_addr_))) {
    LOG4CXX_ERROR_EXT(logger_, "Unable to bind");
    return false;
  }

  LOG4CXX_INFO(logger_, "VideoServer::listen for connections");
  if (-1 == listen(socket_, 5)) {
    LOG4CXX_ERROR_EXT(logger_, "Unable to listen");
    return false;
  }

  is_ready_ = true;
  if (delegate_ && thread_) {
    thread_->startWithOptions(
      threads::ThreadOptions(threads::Thread::kMinStackSize));
  }

  return true;
}

bool SocketVideoServer::stop() {
  LOG4CXX_INFO(logger_, "VideoServer::stop");

  if (delegate_ && thread_) {
    thread_->stop();
  }

  if (socket_ != -1) {
    ::close(socket_);
  }

  return true;
}

void SocketVideoServer::sendMsg(const protocol_handler::RawMessagePtr& message) {
  LOG4CXX_INFO(logger_, "SocketVideoServer::sendData");

  if (is_ready_) {
    messages_.push(message);
  }
}

SocketVideoServer::VideoStreamer::VideoStreamer(SocketVideoServer* const server)
  : server_(server),
    socket_fd_(0),
    is_first_loop_(true),
    is_client_connected_(false),
    stop_flag_(false) {
}

SocketVideoServer::VideoStreamer::~VideoStreamer() {
  stop();
}

void SocketVideoServer::VideoStreamer::threadMain() {
  LOG4CXX_INFO(logger_, "VideoStreamer::threadMain");

  while (!stop_flag_) {

    socket_fd_ = accept(server_->socket_, NULL, NULL);

    if (0 > socket_fd_) {
      LOG4CXX_ERROR(logger_, "Socket is closed");
      sleep(1);
      continue;
    }

    is_client_connected_ = true;
    while (is_client_connected_) {
      server_->messages_.wait();
      protocol_handler::RawMessagePtr msg = server_->messages_.pop();
      if (!msg) {
        LOG4CXX_ERROR(logger_, "Null pointer message");
        continue;
      }

      is_client_connected_ = send(msg);
    }

    stop();
  }
}

bool SocketVideoServer::VideoStreamer::exitThreadMain() {
  LOG4CXX_INFO(logger_, "VideoStreamer::exitThreadMain");
  stop_flag_ = true;
  stop();
  return true;
}

void SocketVideoServer::VideoStreamer::stop() {
  LOG4CXX_INFO(logger_, "VideoStreamer::stop");

  is_client_connected_ = false;
  if (!socket_fd_) {
    return;
  }

  if (-1 == shutdown(socket_fd_, SHUT_RDWR)) {
    LOG4CXX_ERROR(logger_, "Unable to shutdown socket");
    return;
  }

  if (-1 == ::close(socket_fd_)) {
    LOG4CXX_ERROR(logger_, "Unable to close socket");
    return;
  }

  socket_fd_ = -1;
}

bool SocketVideoServer::VideoStreamer::is_ready() const {
  LOG4CXX_INFO(logger_, "VideoStreamer::is_ready");

  bool result = true;
  fd_set fds;
  FD_ZERO(&fds);                       ///< set fdset to the null set.
  FD_SET(socket_fd_, &fds);
  struct timeval tv;
  tv.tv_sec = 5;                       ///< set a 5 second timeout
  tv.tv_usec = 0;

  int retval = 0;
  retval = select(socket_fd_ + 1, 0, &fds, 0, &tv);

  if (-1 == retval) {
    LOG4CXX_ERROR_EXT(logger_, "An error occurred");
    result = false;
  } else if (0 == retval) {
    LOG4CXX_ERROR_EXT(logger_, "The timeout expired");
    result = false;
  }

  return result;
}

bool SocketVideoServer::VideoStreamer::send(
  const protocol_handler::RawMessagePtr& msg) {
  if (!is_ready()) {
    LOG4CXX_ERROR_EXT(logger_, " Socket is not ready");
    return false;
  }

  if (is_first_loop_) {
    is_first_loop_ = false;
    char hdr[] = {"HTTP/1.1 200 OK\r\n "
                  "Connection: Keep-Alive\r\n"
                  "Keep-Alive: timeout=15, max=300\r\n"
                  "Server: SDL\r\n"
                  "Content-Type: video/mp4\r\n\r\n"
                 };

    if (-1 == ::send(socket_fd_, hdr, strlen(hdr), MSG_NOSIGNAL)) {
      LOG4CXX_ERROR_EXT(logger_, " Unable to send");
      return false;
    }
  }

  if (-1 == ::send(socket_fd_, (*msg).data(), (*msg).data_size(), MSG_NOSIGNAL)) {
    LOG4CXX_ERROR_EXT(logger_, " Unable to send");
    return false;
  }

  LOG4CXX_INFO(logger_, "VideoStreamer::sent " << (*msg).data_size());
  return true;
}

}  // namespace video_stream_producer_consumer
}  // namespace media_manager
