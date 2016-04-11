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

#include "telemetry_monitor/telemetry_monitor.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "transport_manager/transport_manager_default.h"
#include "utils/resource_usage.h"
#include "telemetry_monitor/telemetry_observable.h"

namespace telemetry_monitor {

CREATE_LOGGERPTR_GLOBAL(logger_, "TelemetryMonitor")

TelemetryMonitor::TelemetryMonitor(const std::string& server_address,
                                   uint16_t port):
  server_address_(server_address),
  port_(port),
  thread_(NULL),
  streamer_(NULL),
  app_observer(this),
  tm_observer(this),
  ph_observer(this) {

}

void TelemetryMonitor::Start() {
  streamer_ = new Streamer(this);
  thread_ = threads::CreateThread("TelemetryMonitor", streamer_ );
}

void TelemetryMonitor::set_streamer(Streamer* streamer) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (thread_ && !thread_->is_running()) {
    thread_->set_delegate(streamer);
    if (streamer_) {
      delete streamer_;
    }
    streamer_ = streamer;
  } else {
    LOG4CXX_ERROR(logger_, "Unable to replace streamer if it is active");
  }
}

const std::string& TelemetryMonitor::ip() const  {
  return server_address_;
}

int16_t TelemetryMonitor::port() const {
  return port_;
}

TelemetryMonitor::~TelemetryMonitor() {
  Stop();
}

void TelemetryMonitor::Init(
    TelemetryObservable<protocol_handler::PHTelemetryObserver>*
        protocol_handler,
    TelemetryObservable<application_manager::AMTelemetryObserver>*
        app_manager,
    TelemetryObservable<transport_manager::TMTelemetryObserver>*
        transport_manager) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(streamer_);

  app_manager->SetTelemetryObserver(&app_observer);
  transport_manager->SetTelemetryObserver(&tm_observer);
  protocol_handler->SetTelemetryObserver(&ph_observer);

  DCHECK_OR_RETURN_VOID(thread_);
  thread_->start(threads::ThreadOptions());
}

void TelemetryMonitor::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (thread_) {
    thread_->stop();
    thread_->join();
    threads::DeleteThread(thread_);
  }
  thread_ = NULL;
}

void TelemetryMonitor::SendMetric(utils::SharedPtr<MetricWrapper> metric) {
  if ((NULL != streamer_ )&& streamer_->is_client_connected_) {
    streamer_->PushMessage(metric);
  }
}

Streamer::Streamer(TelemetryMonitor* const server)
  : is_client_connected_(false),
    kserver_(server),
    server_socket_fd_(0),
    client_socket_fd_(0),
    stop_flag_(false) {
}

Streamer::~Streamer() {
  Stop();
}

void Streamer::threadMain() {
  LOG4CXX_AUTO_TRACE(logger_);

  Start();
  while (!stop_flag_) {
    LOG4CXX_INFO(logger_, "Server socket is listening ");
    client_socket_fd_ = accept(server_socket_fd_, NULL, NULL);
    if (0 > client_socket_fd_) {
      LOG4CXX_ERROR(logger_, "Cant open socket . Socket is busy ");
      Stop();
      break;
    }
    LOG4CXX_INFO(logger_, "Client connected");

    is_client_connected_ = true;
    while (is_client_connected_) {
      while (!messages_.empty()) {
        utils::SharedPtr<MetricWrapper> metric;
        if (!messages_.pop(metric)) {
          continue;
        }
        is_client_connected_ = Send(metric->GetStyledString());
      }

      if (!IsReady()) {
        LOG4CXX_INFO(logger_, "Client disconnected.");
        break;
      }

      messages_.wait();
    }
  }
}

void Streamer::exitThreadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
  Stop();
  messages_.Shutdown();
}

void Streamer::Start() {
  LOG4CXX_AUTO_TRACE(logger_);
  server_socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (0 >= server_socket_fd_) {
    LOG4CXX_ERROR(logger_, "Server open error");
    return;
  } else {
    LOG4CXX_DEBUG(logger_, "Server socket : " << server_socket_fd_);
  }

  int32_t optval = 1;
  if (-1 == setsockopt(server_socket_fd_, SOL_SOCKET, SO_REUSEADDR,
                       &optval, sizeof optval)) {
    LOG4CXX_ERROR(logger_, "Unable to set sockopt");
    return;
  }

  sockaddr_in serv_addr_ = { 0 };
  serv_addr_.sin_addr.s_addr = inet_addr(kserver_->ip().c_str());
  serv_addr_.sin_family = AF_INET;
  serv_addr_.sin_port = htons(kserver_->port());

  if (-1 == bind(server_socket_fd_,
                 reinterpret_cast<struct sockaddr*>(&serv_addr_),
                 sizeof(serv_addr_))) {
    LOG4CXX_ERROR(logger_, "Unable to bind server "
                  << kserver_->ip().c_str() << ':' << kserver_->port());
    return;
  }
  if (-1 == listen(server_socket_fd_, 1)) {
    LOG4CXX_ERROR(logger_, "Streamer listen error " << strerror(errno) );
    return;
  }
}

void Streamer::ShutDownAndCloseSocket(int32_t socket_fd) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (0 < socket_fd){
    LOG4CXX_INFO(logger_, "Shutdown socket");
    if (-1 == ::shutdown(socket_fd, SHUT_RDWR)) {
      LOG4CXX_ERROR(logger_, "Unable to shutdown socket");
    }
    if (-1 == close(socket_fd)) {
      LOG4CXX_ERROR(logger_, "Unable to close socket");
    }
  } else {
    LOG4CXX_WARN(logger_, "Socket in not connected: " << socket_fd);
  }
}

void Streamer::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (stop_flag_) {
    LOG4CXX_WARN(logger_, "Already Stopped");
    return;
  }
  stop_flag_ = true;
  messages_.Reset();
  LOG4CXX_WARN(logger_, "Stop server_socket_fd_");
  ShutDownAndCloseSocket(server_socket_fd_);
  server_socket_fd_ = -1;

  LOG4CXX_WARN(logger_, "Stop client_socket_fd_");
  ShutDownAndCloseSocket(client_socket_fd_);
  client_socket_fd_ = -1;
  is_client_connected_ = false;
}

bool Streamer::IsReady() const {
  bool result = true;
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(client_socket_fd_, &fds);
  TimevalStruct tv = {0, 0};
  tv.tv_sec = 5;                       // set a 5 second timeout
  tv.tv_usec = 0;

  const int retval = select(client_socket_fd_ + 1, 0, &fds, 0, &tv);

  if (-1 == retval) {
    LOG4CXX_ERROR(logger_, "An error occurred");
    result = false;
  } else if (0 == retval) {
    LOG4CXX_ERROR(logger_, "The timeout expired");
    result = false;
  }

  return result;
}

bool Streamer::Send(const std::string& msg) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!IsReady()) {
    LOG4CXX_ERROR(logger_, " Socket is not ready");
    return false;
  }

  if (-1 == ::send(client_socket_fd_, msg.c_str(),
                   msg.size(), MSG_NOSIGNAL)) {
    LOG4CXX_ERROR(logger_, " Unable to send");
    return false;
  }
  return true;
}

void Streamer::PushMessage(utils::SharedPtr<MetricWrapper> metric) {
  messages_.push(metric);
}
}  // namespace telemetry_monitor
