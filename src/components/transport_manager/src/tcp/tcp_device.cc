/**
 *
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

#include "transport_manager/tcp/tcp_device.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

TcpDevice::TcpDevice(const in_addr_t& in_addr, const std::string& name)
  :
  Device(name, name),
    in_addr_(in_addr),
    last_handle_(0) {
  pthread_mutex_init(&applications_mutex_, 0);
}

bool TcpDevice::IsSameAs(const Device* other) const {
  LOG4CXX_TRACE(logger_, "enter. Device: " << other);
  const TcpDevice* other_tcp_device = static_cast<const TcpDevice*>(other);

  if (other_tcp_device->in_addr_ == in_addr_) {
    LOG4CXX_TRACE(logger_,
                  "exit with TRUE. Condition: other_tcp_device->in_addr_ == in_addr_");
    return true;
  } else {
    LOG4CXX_TRACE(logger_, "exit with FALSE");
    return false;
  }
}

ApplicationList TcpDevice::GetApplicationList() const {
  LOG4CXX_TRACE(logger_, "enter");
  pthread_mutex_lock(&applications_mutex_);
  ApplicationList app_list;
  for (std::map<ApplicationHandle, Application>::const_iterator it =
         applications_.begin(); it != applications_.end(); ++it) {
    app_list.push_back(it->first);
  }
  pthread_mutex_unlock(&applications_mutex_);
  LOG4CXX_TRACE(logger_, "exit with app_list. It's size = " << app_list.size());
  return app_list;
}

ApplicationHandle TcpDevice::AddIncomingApplication(int socket_fd) {
  LOG4CXX_TRACE(logger_, "enter. Socket_fd: " << socket_fd);
  Application app;
  app.incoming = true;
  app.socket = socket_fd;
  app.port = 0; // this line removes compiler warning
  pthread_mutex_lock(&applications_mutex_);
  const ApplicationHandle app_handle = ++last_handle_;
  applications_[app_handle] = app;
  pthread_mutex_unlock(&applications_mutex_);
  LOG4CXX_TRACE(logger_, "exit with app_handle " << app_handle);
  return app_handle;
}

ApplicationHandle TcpDevice::AddDiscoveredApplication(int port) {
  LOG4CXX_TRACE(logger_, "enter. port " << port);
  Application app;
  app.incoming = false;
  app.socket = 0;  // this line removes compiler warning
  app.port = port;
  pthread_mutex_lock(&applications_mutex_);
  const ApplicationHandle app_handle = ++last_handle_;
  applications_[app_handle] = app;
  pthread_mutex_unlock(&applications_mutex_);
  LOG4CXX_TRACE(logger_, "exit with app_handle " << app_handle);
  return app_handle;
}


void TcpDevice::RemoveApplication(const ApplicationHandle app_handle) {
  LOG4CXX_TRACE(logger_, "enter. ApplicationHandle: " << app_handle);
  pthread_mutex_lock(&applications_mutex_);
  applications_.erase(app_handle);
  pthread_mutex_unlock(&applications_mutex_);
  LOG4CXX_TRACE(logger_, "exit");
}

TcpDevice::~TcpDevice() {
  pthread_mutex_destroy(&applications_mutex_);
}

int TcpDevice::GetApplicationSocket(const ApplicationHandle app_handle) const {
  LOG4CXX_TRACE(logger_, "enter. ApplicationHandle: " << app_handle);
  std::map<ApplicationHandle, Application>::const_iterator it = applications_.find(
        app_handle);
  if (applications_.end() == it) {
    LOG4CXX_TRACE(logger_, "exit with -1. Condition: applications_.end() == it");
    return -1;
  }
  if (!it->second.incoming) {
    LOG4CXX_TRACE(logger_, "exit with -1. Condition: !it->second.incoming");
    return -1;
  }
  LOG4CXX_TRACE(logger_, "exit with socket " << it->second.socket);
  return it->second.socket;
}

int TcpDevice::GetApplicationPort(const ApplicationHandle app_handle) const {
  LOG4CXX_TRACE(logger_, "enter. ApplicationHandle: " << app_handle);
  std::map<ApplicationHandle, Application>::const_iterator it = applications_.find(
        app_handle);
  if (applications_.end() == it) {
    LOG4CXX_TRACE(logger_, "exit with -1. Condition: applications_.end() == it");
    return -1;
  }
  if (it->second.incoming) {
    LOG4CXX_TRACE(logger_, "exit with -1. Condition: it->second.incoming");
    return -1;
  }
  LOG4CXX_TRACE(logger_, "exit with port " << it->second.port);
  return it->second.port;
}


}  // namespace transport_adapter
}  // namespace transport_manager
