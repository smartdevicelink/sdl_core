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
#include <errno.h>
#include "config_profile/profile.h"
#include "media_manager/video/socket_video_streamer_adapter.h"
#include "utils/logger.h"

namespace media_manager {

CREATE_LOGGERPTR_GLOBAL(logger, "SocketStreamerAdapter")

SocketStreamerAdapter::SocketStreamerAdapter()
  : socket_fd_(0),
    is_ready_(false),
    thread_(NULL),
    streamer_(NULL),
    messages_() {
}

SocketStreamerAdapter::~SocketStreamerAdapter() {
  thread_->stop();
  streamer_ = NULL;
}

void SocketStreamerAdapter::StartActivity(int32_t application_key) {
  LOG4CXX_TRACE(logger, "enter " << application_key);

  if (application_key == current_application_) {
    LOG4CXX_INFO(logger, "Already running for app " << application_key);
  } else {
    is_ready_ = true;
    current_application_ = application_key;

    messages_.Reset();

    for (std::set<MediaListenerPtr>::iterator it = media_listeners_.begin();
         media_listeners_.end() != it;
         ++it) {
      (*it)->OnActivityStarted(application_key);
    }
  }
  LOG4CXX_TRACE(logger, "exit");
}

void SocketStreamerAdapter::StopActivity(int32_t application_key) {
  LOG4CXX_TRACE(logger, "enter " << application_key);

  if (application_key != current_application_) {
    LOG4CXX_WARN(logger, "Streaming is not active for " << application_key);
  } else {
    is_ready_ = false;
    current_application_ = 0;

    if (streamer_) {
      streamer_->stop();
      messages_.Shutdown();
    }

    for (std::set<MediaListenerPtr>::iterator it = media_listeners_.begin();
         media_listeners_.end() != it;
         ++it) {
      (*it)->OnActivityEnded(application_key);
    }
  }
  LOG4CXX_TRACE(logger, "exit");
}

bool SocketStreamerAdapter::is_app_performing_activity(
  int32_t application_key) {
  return (application_key == current_application_);
}

void SocketStreamerAdapter::Init() {
  if (!thread_) {
    LOG4CXX_INFO(logger, "Create and start sending thread");
    streamer_ = new Streamer(this);
    thread_ = threads::CreateThread("SocketStreamer", streamer_);
    const size_t kStackSize = 16384;
    thread_->startWithOptions(threads::ThreadOptions(kStackSize));
  } else {
    LOG4CXX_WARN(logger, "thread is already exist");
  }
}

void SocketStreamerAdapter::SendData(
  int32_t application_key,
  const ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_INFO(logger, "SendData(application_key = " << application_key << ")");


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
  LOG4CXX_TRACE(logger,"enter " << this);
  sync_primitives::AutoLock auto_lock(thread_lock);
  start();

  while (!stop_flag_) {
    new_socket_fd_ = accept(server_->socket_fd_, NULL, NULL);
    LOG4CXX_INFO(logger, "Client connectd " << new_socket_fd_);
    if (0 > new_socket_fd_) {
      LOG4CXX_ERROR(logger, "Socket is closed " << strerror(errno));
      sleep(1);
      continue;
    }

    is_client_connected_ = true;
    is_first_loop_ = true;
    while (is_client_connected_) {
      while (!server_->messages_.empty()) {
        ::protocol_handler::RawMessagePtr msg = server_->messages_.pop();
        if (!msg) {
          LOG4CXX_ERROR(logger, "Null pointer message");
          continue;
        }

        is_client_connected_ = send(msg);
        static int32_t messages_for_session = 0;
        ++messages_for_session;

        LOG4CXX_INFO(logger, "Handling map streaming message. This is "
            << messages_for_session << " the message for "
            << server_->current_application_);
        std::set<MediaListenerPtr>::iterator it = server_->media_listeners_
            .begin();
        for (; server_->media_listeners_.end() != it; ++it) {
          (*it)->OnDataReceived(server_->current_application_,
                                messages_for_session);
        }
      }

      if (!is_ready()) {
        LOG4CXX_INFO(logger, "Client disconnected.");
        stop();
        break;
      }
      server_->messages_.wait();
    }
  }
  LOG4CXX_TRACE(logger,"exit " << this);
}

bool SocketStreamerAdapter::Streamer::exitThreadMain() {
  LOG4CXX_TRACE(logger,"enter " << this);
  stop_flag_ = true;
  stop();
  server_->messages_.Shutdown();
  //exith threadMainshould whait while threadMain will be finished
  if (server_->socket_fd_ != -1) {
    shutdown(server_->socket_fd_, SHUT_RDWR);
    close(server_->socket_fd_);
  }
  sync_primitives::AutoLock auto_lock(thread_lock);
  LOG4CXX_TRACE(logger,"exit " << this);
  return true;
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

  struct sockaddr_in serv_addr_ = { 0 };
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
  LOG4CXX_TRACE(logger,"enter " << this);
  if (0 == new_socket_fd_) {
    LOG4CXX_ERROR(logger, "Client Socket does not exits: ");
  } else if (-1 == shutdown(new_socket_fd_, SHUT_RDWR)) {
    LOG4CXX_ERROR(logger, "Unable to shutdown socket " << strerror(errno));
  } else if (-1 == ::close(new_socket_fd_)) {
    LOG4CXX_ERROR(logger, "Unable to close socket " << strerror(errno));
  }

  new_socket_fd_ = 0;
  is_client_connected_ = false;
  LOG4CXX_TRACE(logger,"exit" << this);
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
  const ::protocol_handler::RawMessagePtr msg) {
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
