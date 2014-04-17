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


#include "time_manager.h"
#include "transport_manager/transport_manager_default.h"
#include "config_profile/profile.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>


namespace time_tester {

#ifdef ENABLE_LOG
log4cxx::LoggerPtr TimeManager::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("TimeManager"));
#endif // ENABLE_LOG


TimeManager::TimeManager():socket_fd_(0),
  is_ready_(false),
  messages_(),
  thread_(NULL),
  streamer_(NULL)  {

}

TimeManager::~TimeManager() {
  LOG4CXX_INFO(logger_, "Destroint TimeManager");
  Stop();
}

void TimeManager::Init(protocol_handler::ProtocolHandlerImpl* ph) {
  if (!thread_) {
    application_manager::ApplicationManagerImpl::instance()->SetTimeMetricObserver(&app_observer);
    transport_manager::TransportManagerDefault::instance()->SetTimeMetricObserver(&tm_observer);;
    if (!thread_) {
      ip_ = profile::Profile::instance()->server_address();
      port_ = profile::Profile::instance()->time_testing_port();;
      streamer_ = new Streamer(this);
      thread_ = new threads::Thread("SocketAdapter", streamer_);
      const size_t kStackSize = 16384;
      thread_->startWithOptions(threads::ThreadOptions(kStackSize));
      LOG4CXX_INFO(logger_, "Create and start sending thread");
      is_ready_ = true;
    }
//    worker_ = new Worker(this);
//    thread_ = new threads::Thread("TimeManager", worker_);
//    thread_->startWithOptions(threads::ThreadOptions());
  }
}

void TimeManager::Stop() {
  thread_->stop();
  streamer_ = NULL;
  delete thread_;
  if (socket_fd_ != -1) {
    ::close(socket_fd_);
  }
}

void TimeManager::SendMetric(utils::SharedPtr<Metric> metric) {
  LOG4CXX_INFO(logger_, "PushMetric");
  messages_.push(metric);
}


TimeManager::Streamer::Streamer(
  TimeManager* const server)
  : server_(server),
    new_socket_fd_(0),
    is_first_loop_(true),
    is_client_connected_(false),
    stop_flag_(false) {
}

TimeManager::Streamer::~Streamer() {
  stop();
}

void TimeManager::Streamer::threadMain() {
  LOG4CXX_INFO(logger_, "Streamer::threadMain");

  start();

  while (!stop_flag_) {
    new_socket_fd_ = accept(server_->socket_fd_, NULL, NULL);
    if (0 > new_socket_fd_) {
      LOG4CXX_ERROR(logger_, "Socket is closed");
      sleep(1);
      continue;
    }

    is_client_connected_ = true;
    is_first_loop_ = true;
    while (is_client_connected_) {
      LOG4CXX_INFO(logger_, "Messages wait");
      while (!server_->messages_.empty()) {
        LOG4CXX_INFO(logger_, "Messages is not empty");
        utils::SharedPtr<Metric> metric = server_->messages_.pop();
        std::string msg = metric->GetStyledString();
        is_client_connected_ = send(msg);
        static int32_t messsages_for_session = 0;
        ++messsages_for_session;
      }

      if (!is_ready()) {
        LOG4CXX_INFO(logger_, "Client disconnected.");
        stop();
        break;
      }

      server_->messages_.wait();
    }
  }
}

bool TimeManager::Streamer::exitThreadMain() {
  LOG4CXX_INFO(logger_, "Streamer::exitThreadMain");
  stop_flag_ = true;
  stop();
  server_->messages_.Shutdown();
  return false;
}

void TimeManager::Streamer::start() {
  server_->socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (0 >= server_->socket_fd_) {
    LOG4CXX_ERROR_EXT(logger_, "Server open error");
    return;
  }

  int32_t optval = 1;
  if (-1 == setsockopt(server_->socket_fd_, SOL_SOCKET, SO_REUSEADDR,
                       &optval, sizeof optval)) {
    LOG4CXX_ERROR_EXT(logger_, "Unable to set sockopt");
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
    LOG4CXX_ERROR_EXT(logger_, "Unable to bind");
    return;
  }
  if (-1 == listen(server_->socket_fd_, 1)) {
    LOG4CXX_INFO(logger_, "Streamer listen error " << strerror(errno) );
    return;
  }
  LOG4CXX_INFO(logger_, "Streamer is listetning for connections");
}

void TimeManager::Streamer::stop() {
  LOG4CXX_INFO(logger_, "SocketStreamerAdapter::Streamer::stop");
  if (!new_socket_fd_) {
    return;
  }

  if (-1 == shutdown(new_socket_fd_, SHUT_RDWR)) {
    LOG4CXX_ERROR(logger_, "Unable to shutdown socket");
    return;
  }

  if (-1 == ::close(new_socket_fd_)) {
    LOG4CXX_ERROR(logger_, "Unable to close socket");
    return;
  }

  new_socket_fd_ = -1;
  is_client_connected_ = false;
}

bool TimeManager::Streamer::is_ready() const {
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
    LOG4CXX_ERROR_EXT(logger_, "An error occurred");
    result = false;
  } else if (0 == retval) {
    LOG4CXX_ERROR_EXT(logger_, "The timeout expired");
    result = false;
  }

  return result;
}

bool TimeManager::Streamer::send(
  const std::string& msg) {
  if (!is_ready()) {
    LOG4CXX_ERROR_EXT(logger_, " Socket is not ready");
    return false;
  }

  if (-1 == ::send(new_socket_fd_, msg.c_str(),
                   msg.size(), MSG_NOSIGNAL)) {
    LOG4CXX_ERROR_EXT(logger_, " Unable to send");
    return false;
  }

  LOG4CXX_INFO(logger_, "Streamer::sent " << msg.size());
  return true;
}

}
