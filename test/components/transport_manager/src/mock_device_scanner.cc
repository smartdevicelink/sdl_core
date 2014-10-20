/*
 * \file mock_device_scanner.cc
 * \brief 
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

#include "transport_manager/mock_device_scanner.h"
#include "transport_manager/mock_transport_adapter.h"
#include "transport_manager/mock_device.h"

using ::transport_manager::SearchDeviceError;

namespace test {
namespace components {
namespace transport_manager {

MockDeviceScanner::MockDeviceScanner(MockTransportAdapter *controller)
    : controller_(controller),
      is_initialized_(false),
      is_search_failed_(false) {
}

TransportAdapter::Error MockDeviceScanner::Init() {
  is_initialized_ = true;
  return TransportAdapter::OK;
}

TransportAdapter::Error MockDeviceScanner::Scan() {
  if (is_search_failed_) {
    controller_->SearchDeviceFailed(SearchDeviceError());
  } else {
    controller_->SearchDeviceDone(devices_);
  }
  return TransportAdapter::OK;
}

void MockDeviceScanner::Terminate() {
}

void MockDeviceScanner::reset() {
  is_search_failed_ = false;
  for (DeviceVector::iterator it = devices_.begin();
      it != devices_.end();
      ++it) {
    static_cast<MockDevice*>(it->get())->Stop();
  }
  devices_.clear();
}

bool MockDeviceScanner::IsInitialised() const {
  return is_initialized_;
}

void MockDeviceScanner::AddDevice(const std::string& name,
                                  const std::string& unique_id, bool start) {
  MockDevice* dev = new MockDevice(name, unique_id, controller_);
  dev->addApplication();
  if (start) {
    dev->Start();
  }
  devices_.push_back(dev);
}

void MockDeviceScanner::RemoveDevice(const std::string& name) {
  for (DeviceVector::iterator t = devices_.begin(); t != devices_.end(); ++t) {
    if ((*t)->name() == name) {
      devices_.erase(t);
      break;
    }
  }
}

}  // namespace transport_manager
}  // namespace components
}  // namespace test

// vim: set ts=2 sw=2 et:
