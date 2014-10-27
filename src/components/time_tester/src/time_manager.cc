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

#include "time_manager.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "transport_manager/transport_manager_default.h"
#include "config_profile/profile.h"
#include "utils/resource_usage.h"

namespace time_tester {

CREATE_LOGGERPTR_GLOBAL(logger_, "TimeManager")

TimeManager::TimeManager():
  socket_fd_(0),
  thread_(NULL),
  messages_(),
  streamer_(NULL),
  app_observer(this),
  tm_observer(this),
  ph_observer(this) {
    ip_ = profile::Profile::instance()->server_address();
    port_ = profile::Profile::instance()->time_testing_port();
}

TimeManager::~TimeManager() {
  LOG4CXX_INFO(logger_, "Destroing TimeManager");
  Stop();
}

void TimeManager::Init(protocol_handler::ProtocolHandlerImpl* ph) {
  DCHECK(ph);
  if (!thread_) {
    streamer_ = new Streamer(this);
    thread_ = threads::CreateThread("TimeManager", streamer_ );
    application_manager::ApplicationManagerImpl::instance()->SetTimeMetricObserver(&app_observer);
    transport_manager::TransportManagerDefault::instance()->SetTimeMetricObserver(&tm_observer);
    ph->SetTimeMetricObserver(&ph_observer);
    thread_->startWithOptions(threads::ThreadOptions());
    LOG4CXX_INFO(logger_, "Create and start sending thread");
    }
}

void TimeManager::Stop() {
  if (thread_) {
    thread_->stop();
    thread_ = NULL;
    if (socket_fd_ != -1) {
      ::close(socket_fd_);
    }
  }
  messages_.Reset();
  LOG4CXX_INFO(logger_, "TimeManager stopped");
}

void TimeManager::SendMetric(utils::SharedPtr<MetricWrapper> metric) {
  if ((NULL != streamer_ )&& streamer_->is_client_connected_) {
    messages_.push(metric);
  }
}

TimeManager::Streamer::Streamer(
  TimeManager* const server)
  : is_client_connected_(false),
    server_(server),
    new_socket_fd_(0),
    stop_flag_(false) {
}

TimeManager::Streamer::~Streamer() {
  Stop();
}

void TimeManager::Streamer::threadMain() {
  LOG4CXX_INFO(logger_, "Streamer::threadMain");

  Start();

  while (!stop_flag_) {
    new_socket_fd_ = accept(server_->socket_fd_, NULL, NULL);
    if (0 > new_socket_fd_) {
      LOG4CXX_ERROR(logger_, "Cant open socket . Socket is busy ");
      Stop();
      break;
    }

    is_client_connected_ = true;
    while (is_client_connected_) {
      while (!server_->messages_.empty()) {
        utils::SharedPtr<MetricWrapper> metric = server_->messages_.pop();
        is_client_connected_ = Send(metric->GetStyledString());
      }

      if (!IsReady()) {
        LOG4CXX_INFO(logger_, "Client disconnected.");
        Stop();
        break;
      }

      server_->messages_.wait();
    }
  }
}

bool TimeManager::Streamer::exitThreadMain() {
  LOG4CXX_INFO(logger_, "Streamer::exitThreadMain");
  stop_flag_ = true;
  Stop();
  server_->messages_.Shutdown();
  return false;
}

void TimeManager::Streamer::Start() {
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

  sockaddr_in serv_addr_ = { 0 };
  serv_addr_.sin_addr.s_addr = inet_addr(server_->ip_.c_str());
  serv_addr_.sin_family = AF_INET;
  serv_addr_.sin_port = htons(server_->port_);

  if (-1 == bind(server_->socket_fd_,
                 reinterpret_cast<struct sockaddr*>(&serv_addr_),
                 sizeof(serv_addr_))) {
    LOG4CXX_ERROR(logger_, "Unable to bind server "
                  << server_->ip_.c_str() << ':' << server_->port_);
    return;
  }
  if (-1 == listen(server_->socket_fd_, 1)) {
    LOG4CXX_ERROR(logger_, "Streamer listen error " << strerror(errno) );
    return;
  }
  LOG4CXX_INFO(logger_, "Streamer is listetning for connections");
}

void TimeManager::Streamer::Stop() {
  LOG4CXX_INFO(logger_, "SocketStreamerAdapter::Streamer::stop");
  if (!new_socket_fd_) {
    return;
  }

  if (-1 == shutdown(new_socket_fd_, SHUT_RDWR)) {
    LOG4CXX_ERROR(logger_, "Unable to shutdown socket");
    return;
  }

  if (-1 == close(new_socket_fd_)) {
    LOG4CXX_ERROR(logger_, "Unable to close socket");
    return;
  }

  new_socket_fd_ = -1;
  is_client_connected_ = false;
}

bool TimeManager::Streamer::IsReady() const {
  bool result = true;
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(new_socket_fd_, &fds);
  TimevalStruct tv;
  tv.tv_sec = 5;                       // set a 5 second timeout
  tv.tv_usec = 0;

  const int retval = select(new_socket_fd_ + 1, 0, &fds, 0, &tv);

  if (-1 == retval) {
    LOG4CXX_ERROR_EXT(logger_, "An error occurred");
    result = false;
  } else if (0 == retval) {
    LOG4CXX_ERROR_EXT(logger_, "The timeout expired");
    result = false;
  }

  return result;
}

bool TimeManager::Streamer::Send(const std::string& msg) {
  if (!IsReady()) {
    LOG4CXX_ERROR_EXT(logger_, " Socket is not ready");
    return false;
  }

  if (-1 == ::send(new_socket_fd_, msg.c_str(),
                   msg.size(), MSG_NOSIGNAL)) {
    LOG4CXX_ERROR_EXT(logger_, " Unable to send");
    return false;
  }
  return true;
}
}  // namespace time_tester
