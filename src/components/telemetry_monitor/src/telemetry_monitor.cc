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

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "transport_manager/transport_manager_default.h"
#include "utils/resource_usage.h"

#include "telemetry_monitor/telemetry_observable.h"

namespace telemetry_monitor {

SDL_CREATE_LOG_VARIABLE("TelemetryMonitor")

TelemetryMonitor::TelemetryMonitor(const std::string& server_address,
                                   uint16_t port)
    : server_address_(server_address)
    , port_(port)
    , thread_(NULL)
    , app_observer(this)
    , tm_observer(this)
    , ph_observer(this) {}

void TelemetryMonitor::Start() {
  streamer_ = streamer_ ? streamer_ : std::make_shared<Streamer>(this);
  thread_ = threads::CreateThread("TelemetryMonitor", streamer_.get());
}

void TelemetryMonitor::set_streamer(std::shared_ptr<Streamer> streamer) {
  SDL_LOG_AUTO_TRACE();
  if (thread_ && !thread_->IsRunning()) {
    thread_->SetDelegate(streamer_.get());
    streamer_ = streamer;
  } else {
    SDL_LOG_ERROR("Unable to replace streamer if it is active");
  }
}

const std::string& TelemetryMonitor::ip() const {
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
    TelemetryObservable<application_manager::AMTelemetryObserver>* app_manager,
    TelemetryObservable<transport_manager::TMTelemetryObserver>*
        transport_manager) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(streamer_);

  app_manager->SetTelemetryObserver(&app_observer);
  transport_manager->SetTelemetryObserver(&tm_observer);
  protocol_handler->SetTelemetryObserver(&ph_observer);

  DCHECK_OR_RETURN_VOID(thread_);
  thread_->Start(threads::ThreadOptions());
}

void TelemetryMonitor::Stop() {
  SDL_LOG_AUTO_TRACE();
  if (thread_) {
    thread_->Stop(threads::Thread::kThreadSoftStop);
    if (thread_->GetDelegate()) {
      streamer_.reset();
    }
    threads::DeleteThread(thread_);
  }
  thread_ = NULL;
}

void TelemetryMonitor::SendMetric(std::shared_ptr<MetricWrapper> metric) {
  if (streamer_ && streamer_->is_client_connected_) {
    streamer_->PushMessage(metric);
  }
}

Streamer::Streamer(TelemetryMonitor* const server)
    : is_client_connected_(false)
    , kserver_(server)
    , server_socket_fd_(0)
    , client_socket_fd_(0)
    , stop_flag_(false) {}

Streamer::~Streamer() {
  Stop();
}

void Streamer::threadMain() {
  SDL_LOG_AUTO_TRACE();

  Start();
  while (!stop_flag_) {
    SDL_LOG_INFO("Server socket is listening ");
    client_socket_fd_ = accept(server_socket_fd_, NULL, NULL);
    if (0 > client_socket_fd_) {
      SDL_LOG_ERROR("Cant open socket . Socket is busy ");
      Stop();
      break;
    }
    SDL_LOG_INFO("Client connected");

    is_client_connected_ = true;
    while (is_client_connected_) {
      while (!messages_.empty()) {
        std::shared_ptr<MetricWrapper> metric;
        if (!messages_.pop(metric)) {
          continue;
        }
        is_client_connected_ = Send(metric->GetStyledString());
      }

      if (!IsReady()) {
        SDL_LOG_INFO("Client disconnected.");
        break;
      }

      messages_.wait();
    }
  }
}

void Streamer::exitThreadMain() {
  SDL_LOG_AUTO_TRACE();
  Stop();
  messages_.Shutdown();
  ThreadDelegate::exitThreadMain();
}

void Streamer::Start() {
  SDL_LOG_AUTO_TRACE();
  server_socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (0 >= server_socket_fd_) {
    SDL_LOG_ERROR("Server open error");
    return;
  } else {
    SDL_LOG_DEBUG("Server socket : " << server_socket_fd_);
  }

  int32_t optval = 1;
  if (-1 == setsockopt(server_socket_fd_,
                       SOL_SOCKET,
                       SO_REUSEADDR,
                       &optval,
                       sizeof optval)) {
    SDL_LOG_ERROR("Unable to set sockopt");
    return;
  }

  sockaddr_in serv_addr_ = {0};
  serv_addr_.sin_addr.s_addr = inet_addr(kserver_->ip().c_str());
  serv_addr_.sin_family = AF_INET;
  serv_addr_.sin_port = htons(kserver_->port());

  if (-1 == bind(server_socket_fd_,
                 reinterpret_cast<struct sockaddr*>(&serv_addr_),
                 sizeof(serv_addr_))) {
    SDL_LOG_ERROR("Unable to bind server " << kserver_->ip().c_str() << ':'
                                           << kserver_->port());
    return;
  }
  if (-1 == listen(server_socket_fd_, 1)) {
    SDL_LOG_ERROR("Streamer listen error " << strerror(errno));
    return;
  }
}

void Streamer::ShutDownAndCloseSocket(int32_t socket_fd) {
  SDL_LOG_AUTO_TRACE();
  if (0 < socket_fd) {
    SDL_LOG_INFO("Shutdown socket");
    if (-1 == ::shutdown(socket_fd, SHUT_RDWR)) {
      SDL_LOG_ERROR("Unable to shutdown socket");
    }
    if (-1 == close(socket_fd)) {
      SDL_LOG_ERROR("Unable to close socket");
    }
  } else {
    SDL_LOG_WARN("Socket in not connected: " << socket_fd);
  }
}

void Streamer::Stop() {
  SDL_LOG_AUTO_TRACE();
  if (stop_flag_) {
    SDL_LOG_WARN("Already Stopped");
    return;
  }
  stop_flag_ = true;
  messages_.Reset();
  SDL_LOG_WARN("Stop server_socket_fd_");
  ShutDownAndCloseSocket(server_socket_fd_);
  server_socket_fd_ = -1;

  SDL_LOG_WARN("Stop client_socket_fd_");
  ShutDownAndCloseSocket(client_socket_fd_);
  client_socket_fd_ = -1;
  is_client_connected_ = false;
}

bool Streamer::IsReady() const {
  bool result = true;
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(client_socket_fd_, &fds);
  // need to construct a timeval for the duration, can't use duration
  struct timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;

  const int retval = select(client_socket_fd_ + 1, 0, &fds, 0, &tv);

  if (-1 == retval) {
    SDL_LOG_ERROR("An error occurred");
    result = false;
  } else if (0 == retval) {
    SDL_LOG_ERROR("The timeout expired");
    result = false;
  }

  return result;
}

bool Streamer::Send(const std::string& msg) {
  SDL_LOG_AUTO_TRACE();
  if (!IsReady()) {
    SDL_LOG_ERROR(" Socket is not ready");
    return false;
  }

  if (-1 == ::send(client_socket_fd_, msg.c_str(), msg.size(), MSG_NOSIGNAL)) {
    SDL_LOG_ERROR(" Unable to send");
    return false;
  }
  return true;
}

void Streamer::PushMessage(std::shared_ptr<MetricWrapper> metric) {
  messages_.push(metric);
}
}  // namespace telemetry_monitor
