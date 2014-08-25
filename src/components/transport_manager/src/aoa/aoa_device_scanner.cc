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

#include "utils/logger.h"

#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

AOADeviceScanner::AOADeviceScanner(TransportAdapterController* controller)
    : initialised_(false),
      observer_(NULL),
      controller_(controller) {
}

TransportAdapter::Error AOADeviceScanner::Init() {
  // TODO(KKolodiy) need to read this parameters from somewhere
  const std::string kPathToConfig = "/fs/mp/etc/mm/aoa.conf";
  const AOAWrapper::AOAUsbInfo info;
  observer_ = new ScannerObserver(this);
  //initialised_ = AOAWrapper::Init(kPathToConfig, info, observer_);
  initialised_ = AOAWrapper::Init(observer_);
  return (initialised_) ? TransportAdapter::OK : TransportAdapter::FAIL;
}

TransportAdapter::Error AOADeviceScanner::Scan() {
  return TransportAdapter::NOT_SUPPORTED;
}

void AOADeviceScanner::Terminate() {
  AOAWrapper::Shutdown();
  delete observer_;
  observer_ = NULL;
}

bool AOADeviceScanner::IsInitialised() const {
  return initialised_;
}

void AOADeviceScanner::NotifyDevicesUpdated() {
  DeviceVector devices;
  devices_lock_.Acquire();
  for (DeviceContainer::const_iterator i = devices_.begin();
      i != devices_.end(); ++i) {
    AOADevicePtr aoa_device = i->second;
    DeviceSptr device = AOADevicePtr::static_pointer_cast<Device>(aoa_device);
    devices.push_back(device);
  }
  devices_lock_.Release();
  controller_->SearchDeviceDone(devices);
}

std::string AOADeviceScanner::GetName() {
  sync_primitives::AutoLock loker(devices_lock_);
  return "AOA device #" + devices_.size();
}

std::string AOADeviceScanner::GetUniqueId() {
  sync_primitives::AutoLock loker(devices_lock_);
  return "AOA#" + devices_.size();
}

void AOADeviceScanner::AddDevice(AOAWrapper::AOAHandle hdl) {
  const std::string name = GetName();
  const std::string unique_id = GetUniqueId();
  AOADevicePtr aoa_device(new AOADevice(hdl, name, unique_id));
  sync_primitives::AutoLock loker(devices_lock_);
  devices_.insert(std::make_pair(hdl, aoa_device));
}

AOADeviceScanner::ScannerObserver::ScannerObserver(AOADeviceScanner* parent)
    : parent_(parent) {
}

void AOADeviceScanner::ScannerObserver::OnConnectedDevice(
    AOAWrapper::AOAHandle hdl) {
  LOG4CXX_TRACE(logger_, "AOA: new device is connected");
  parent_->AddDevice(hdl);
  parent_->NotifyDevicesUpdated();
}

}  // namespace transport_adapter
}  // namespace transport_manager
