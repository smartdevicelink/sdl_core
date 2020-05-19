/*
 *
 * Copyright (c) 2018, Livio
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

#include "transport_manager/cloud/cloud_device.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {
CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

CloudDevice::CloudDevice(std::string& host,
                         std::string& port,
                         std::string& name)
    : Device(name, std::string(name))
    , endpoint_(CloudAppEndpoint{.host = host,
                                 .port = port,
                                 .path = "/",
                                 .query = "",
                                 .fragment = ""}) {}

CloudDevice::CloudDevice(CloudAppEndpoint& endpoint, std::string& name)
    : Device(name, std::string(name)), endpoint_(endpoint) {}

bool CloudDevice::IsSameAs(const Device* other) const {
  LOG4CXX_TRACE(logger_, "enter. device: " << other);

  const CloudDevice* other_cloud_device =
      dynamic_cast<const CloudDevice*>(other);

  if (!other_cloud_device) {
    return false;
  }

  if (GetHost() != other_cloud_device->GetHost()) {
    return false;
  }

  if (GetPort() != other_cloud_device->GetPort()) {
    return false;
  }

  if (GetTarget() != other_cloud_device->GetTarget()) {
    return false;
  }

  return true;
}

ApplicationList CloudDevice::GetApplicationList() const {
  return ApplicationList{0};
}

const std::string& CloudDevice::GetHost() const {
  return endpoint_.host;
}

const std::string& CloudDevice::GetPort() const {
  return endpoint_.port;
}

const std::string CloudDevice::GetTarget() const {
  return endpoint_.path + endpoint_.query + endpoint_.fragment;
}

}  // namespace transport_adapter
}  // namespace transport_manager
