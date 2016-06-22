/*
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

SDL_CREATE_LOGGER("TransportManager")

TcpDevice::TcpDevice(const utils::HostAddress& address, const std::string& name)
    : Device(name, name)
    , applications_mutex_()
    , address_(address)
    , last_handle_(0) {
  SDL_AUTO_TRACE();
  SDL_DEBUG("Created TCPDevice with name " << name);
}

bool TcpDevice::IsSameAs(const Device* other) const {
  SDL_AUTO_TRACE();
  SDL_DEBUG("Device: " << other);
  const TcpDevice* other_tcp_device = static_cast<const TcpDevice*>(other);

  if (other_tcp_device->address_ == address_) {
    SDL_TRACE(
        "exit with TRUE. Condition: other_tcp_device->address_ == address_");
    return true;
  } else {
    SDL_TRACE("exit with FALSE");
    return false;
  }
}

ApplicationList TcpDevice::GetApplicationList() const {
  SDL_AUTO_TRACE();
  ApplicationList app_list;
  app_list.reserve(applications_.size());
  sync_primitives::AutoLock locker(applications_mutex_);
  for (std::map<ApplicationHandle, Application>::const_iterator it =
           applications_.begin();
       it != applications_.end();
       ++it) {
    app_list.push_back(it->first);
  }
  return app_list;
}

ApplicationHandle TcpDevice::AddApplication(const uint16_t port,
                                            const bool is_incomming) {
  SDL_AUTO_TRACE();
  SDL_DEBUG("Port " << port);
  Application appplication(is_incomming, port);
  sync_primitives::AutoLock locker(applications_mutex_);
  const ApplicationHandle app_handle = ++last_handle_;
  applications_[app_handle] = appplication;
  SDL_DEBUG("App_handle " << app_handle);
  return app_handle;
}

void TcpDevice::RemoveApplication(const ApplicationHandle app_handle) {
  SDL_AUTO_TRACE();
  SDL_DEBUG("ApplicationHandle: " << app_handle);
  sync_primitives::AutoLock locker(applications_mutex_);
  applications_.erase(app_handle);
}

TcpDevice::~TcpDevice() {
  SDL_AUTO_TRACE();
}

int TcpDevice::GetApplicationPort(const ApplicationHandle app_handle) const {
  SDL_AUTO_TRACE();
  SDL_DEBUG("ApplicationHandle: " << app_handle);
  std::map<ApplicationHandle, Application>::const_iterator it =
      applications_.find(app_handle);
  if (applications_.end() == it) {
    SDL_WARN("Application was not found");
    return -1;
  }
  if (it->second.incoming) {
    SDL_DEBUG("Application is incoming");
    return -1;
  }
  SDL_DEBUG("port " << it->second.port);
  return it->second.port;
}

}  // namespace transport_adapter
}  // namespace transport_manager
