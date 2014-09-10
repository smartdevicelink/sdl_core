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

#include "transport_manager/aoa/aoa_device_scanner.h"

#include <sstream>

#include "utils/logger.h"

#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

const std::string AOADeviceScanner::kPathToConfig = "";  // default on QNX /etc/mm/aoa.conf

AOADeviceScanner::AOADeviceScanner(TransportAdapterController* controller)
    : initialised_(false),
      life_(NULL),
      controller_(controller) {
}

TransportAdapter::Error AOADeviceScanner::Init() {
  LOG4CXX_TRACE(logger_, "AOA: init device scanner");
  life_ = new LifeDevice(this);
  if (kPathToConfig.empty()) {
    initialised_ = AOAWrapper::Init(life_);
  } else {
    initialised_ = AOAWrapper::Init(kPathToConfig, life_);
  }
  return (initialised_) ? TransportAdapter::OK : TransportAdapter::FAIL;
}

TransportAdapter::Error AOADeviceScanner::Scan() {
  return TransportAdapter::NOT_SUPPORTED;
}

void AOADeviceScanner::Terminate() {
  AOAWrapper::Shutdown();
  delete life_;
  life_ = NULL;
}

bool AOADeviceScanner::IsInitialised() const {
  return initialised_;
}

void AOADeviceScanner::Notify() {
  LOG4CXX_TRACE(logger_, "AOA: notify about all connected devices");
  DeviceVector devices;
  for (DeviceContainer::const_iterator i = devices_.begin();
      i != devices_.end(); ++i) {
    AOADevicePtr aoa_device = i->second;
    DeviceSptr device = AOADevicePtr::static_pointer_cast<Device>(aoa_device);
    devices.push_back(device);
  }
  controller_->SearchDeviceDone(devices);
}

std::string AOADeviceScanner::GetName(const std::string& unique_id) {
  return "AOA device " + unique_id;
}

std::string AOADeviceScanner::GetUniqueId() {
  sync_primitives::AutoLock locker(devices_lock_);
  static int counter = 0;
  ++counter;
  std::ostringstream stream;
  stream << counter;
  return "#_" + stream.str() + "_aoa";
}

void AOADeviceScanner::AddDevice(AOAWrapper::AOAHandle hdl) {
  LOG4CXX_TRACE(logger_, "AOA: add new device " << hdl);
  const std::string unique_id = GetUniqueId();
  const std::string name = GetName(unique_id);
  AOADevicePtr aoa_device(new AOADevice(hdl, name, unique_id));
  sync_primitives::AutoLock locker(devices_lock_);
  devices_.insert(std::make_pair(hdl, aoa_device));
  Notify();
}

void AOADeviceScanner::RemoveDevice(AOAWrapper::AOAHandle hdl) {
  LOG4CXX_TRACE(logger_, "AOA: remove device " << hdl);
  sync_primitives::AutoLock locker(devices_lock_);
  devices_.erase(hdl);
  Notify();
}

void AOADeviceScanner::LoopDevice(AOAWrapper::AOAHandle hdl) {
  LOG4CXX_TRACE(logger_, "AOA: loop of life device " << hdl);
  sync_primitives::AutoLock locker(life_lock_);
  while (AOAWrapper::IsHandleValid(hdl)) {
    LOG4CXX_TRACE(logger_, "AOA: wait cond " << hdl);
    life_cond_.Wait(locker);
  }
}

void AOADeviceScanner::StopDevice(AOAWrapper::AOAHandle hdl) {
  LOG4CXX_TRACE(logger_, "AOA: stop device " << hdl);
  life_cond_.Broadcast();
}

AOADeviceScanner::LifeDevice::LifeDevice(AOADeviceScanner* parent)
    : parent_(parent) {
}

void AOADeviceScanner::LifeDevice::Loop(AOAWrapper::AOAHandle hdl) {
  parent_->AddDevice(hdl);
  parent_->LoopDevice(hdl);
  parent_->RemoveDevice(hdl);
}

void AOADeviceScanner::LifeDevice::Died(AOAWrapper::AOAHandle hdl) {
  parent_->StopDevice(hdl);
}

}  // namespace transport_adapter
}  // :namespace transport_manager
