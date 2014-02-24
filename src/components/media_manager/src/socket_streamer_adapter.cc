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
#include "media_manager/video/socket_video_streamer_adapter.h"

namespace media_manager {

namespace {
log4cxx::LoggerPtr logger =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("SocketStreamerAdapter"));
}

SocketStreamerAdapter::SocketStreamerAdapter()
  : socket_fd_(0),
    is_ready_(false),
    messages_(),
    thread_(NULL) {
}

SocketStreamerAdapter::~SocketStreamerAdapter() {
  thread_->stop();
  delete thread_;
  if (socket_fd_ != -1) {
    ::close(socket_fd_);
  }
}

void SocketStreamerAdapter::StartActivity(int32_t application_key) {
  LOG4CXX_INFO(logger, "SocketStreamerAdapter::start");

  if (application_key == current_application_) {
    LOG4CXX_INFO(logger, "Already running for app " << application_key);
    return;
  }

  is_ready_ = true;
  current_application_ = application_key;

  for (std::set<MediaListenerPtr>::iterator it = media_listeners_.begin();
       media_listeners_.end() != it;
       ++it) {
    (*it)->OnActivityStarted(application_key);
  }
}

void SocketStreamerAdapter::StopActivity(int32_t application_key) {
  LOG4CXX_INFO(logger, "SocketStreamerAdapter::stop");

  if (application_key != current_application_) {
    LOG4CXX_WARN(logger, "Streaming is not active for " << application_key);
    return;
  }

  is_ready_ = false;
  current_application_ = 0;

  for (std::set<MediaListenerPtr>::iterator it = media_listeners_.begin();
       media_listeners_.end() != it;
       ++it) {
    (*it)->OnActivityEnded(application_key);
  }
}

bool SocketStreamerAdapter::is_app_performing_activity(
  int32_t application_key) {
  return (application_key == current_application_);
}

void SocketStreamerAdapter::SendData(
  int32_t application_key,
  const protocol_handler::RawMessagePtr& message) {
  LOG4CXX_INFO(logger, "SocketStreamerAdapter::sendData");

  if(NULL == thread_) {
    LOG4CXX_INFO(logger, "Create and start sending thread");
    thread_ = new threads::Thread("PipeStreamerAdapter", new Streamer(this));
    thread_->startWithOptions(
        threads::ThreadOptions(threads::Thread::kMinStackSize));
  }

  if (application_key != current_application_) {
    LOG4CXX_WARN(logger, "Currently working with other app "
                 << current_application_);
    return;
  }

  if (is_ready_) {
    messages_.push(message);
  }
}

SocketStreamerAdapter::Streamer::Streamer(
  SocketStreamerAdapter* const server)
  : server_(server),
    new_socket_fd_(0),
    is_first_loop_(true),
    is_client_connected_(false),
    stop_flag_(false) {
}

SocketStreamerAdapter::Streamer::~Streamer() {
  stop();
}

void SocketStreamerAdapter::Streamer::threadMain() {
  LOG4CXX_INFO(logger, "Streamer::threadMain");

  start();

  while (!stop_flag_) {
    new_socket_fd_ = accept(server_->socket_fd_, NULL, NULL);

    if (0 > new_socket_fd_) {
      LOG4CXX_ERROR(logger, "Socket is closed");
      sleep(1);
      continue;
    }

    is_client_connected_ = true;
    while (is_client_connected_) {
      while (!server_->messages_.empty()) {
        protocol_handler::RawMessagePtr msg = server_->messages_.pop();
        if (!msg) {
          LOG4CXX_ERROR(logger, "Null pointer message");
          continue;
        }

        is_client_connected_ = send(msg);
        static int32_t messsages_for_session = 0;
        ++messsages_for_session;

        LOG4CXX_INFO(logger, "Handling map streaming message. This is "
                     << messsages_for_session << " the message for "
                     << server_->current_application_);
        std::set<MediaListenerPtr>::iterator it =
            server_->media_listeners_.begin();
        for (; server_->media_listeners_.end() != it; ++it) {
          (*it)->OnDataReceived(server_->current_application_,
                                messsages_for_session);
        }
      }
      server_->messages_.wait();
      is_client_connected_ = is_ready();
    }

    stop();
  }
}

bool SocketStreamerAdapter::Streamer::exitThreadMain() {
  LOG4CXX_INFO(logger, "Streamer::exitThreadMain");
  stop_flag_ = true;
  stop();
  server_->messages_.Shutdown();
  return false;
}

void SocketStreamerAdapter::Streamer::start() {
  server_->socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (0 >= server_->socket_fd_) {
    LOG4CXX_ERROR_EXT(logger, "Server open error");
    return;
  }

  int32_t optval = 1;
  if (-1 == setsockopt(server_->socket_fd_, SOL_SOCKET, SO_REUSEADDR,
                       &optval, sizeof optval)) {
    LOG4CXX_ERROR_EXT(logger, "Unable to set sockopt");
    return;
  }

  struct sockaddr_in serv_addr_;
  memset(&serv_addr_, 0, sizeof(serv_addr_));
  serv_addr_.sin_addr.s_addr = inet_addr(server_->ip_.c_str());
  serv_addr_.sin_family = AF_INET;
  serv_addr_.sin_port = htons(server_->port_);

  if (-1 == bind(server_->socket_fd_,
                 reinterpret_cast<struct sockaddr*>(&serv_addr_),
                 sizeof(serv_addr_))) {
    LOG4CXX_ERROR_EXT(logger, "Unable to bind");
    return;
  }

  LOG4CXX_INFO(logger, "SocketStreamerAdapter::listen for connections");
  if (-1 == listen(server_->socket_fd_, 5)) {
    LOG4CXX_ERROR_EXT(logger, "Unable to listen");
    return;
  }
}

void SocketStreamerAdapter::Streamer::stop() {
  is_client_connected_ = false;
  if (!new_socket_fd_) {
    return;
  }

  if (-1 == shutdown(new_socket_fd_, SHUT_RDWR)) {
    LOG4CXX_ERROR(logger, "Unable to shutdown socket");
    return;
  }

  if (-1 == ::close(new_socket_fd_)) {
    LOG4CXX_ERROR(logger, "Unable to close socket");
    return;
  }

  new_socket_fd_ = -1;
}

bool SocketStreamerAdapter::Streamer::is_ready() const {
  bool result = true;
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(new_socket_fd_, &fds);
  struct timeval tv;
  tv.tv_sec = 5;                       // set a 5 second timeout
  tv.tv_usec = 0;

  int32_t retval = 0;
  retval = select(new_socket_fd_ + 1, 0, &fds, 0, &tv);

  if (-1 == retval) {
    LOG4CXX_ERROR_EXT(logger, "An error occurred");
    result = false;
  } else if (0 == retval) {
    LOG4CXX_ERROR_EXT(logger, "The timeout expired");
    result = false;
  }

  return result;
}

bool SocketStreamerAdapter::Streamer::send(
  const protocol_handler::RawMessagePtr& msg) {
  if (!is_ready()) {
    LOG4CXX_ERROR_EXT(logger, " Socket is not ready");
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

    if (-1 == ::send(new_socket_fd_, hdr, strlen(hdr), MSG_NOSIGNAL)) {
      LOG4CXX_ERROR_EXT(logger, " Unable to send");
      return false;
    }
  }

  if (-1 == ::send(new_socket_fd_, (*msg).data(),
                   (*msg).data_size(), MSG_NOSIGNAL)) {
    LOG4CXX_ERROR_EXT(logger, " Unable to send");
    return false;
  }

  LOG4CXX_INFO(logger, "Streamer::sent " << (*msg).data_size());
  return true;
}

}  // namespace media_manager
