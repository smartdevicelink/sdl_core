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

#include "transport_manager/aoa/aoa_dynamic_device.h"

#include "utils/logger.h"
#include "transport_manager/aoa/aoa_wrapper.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

const std::string AOADynamicDevice::kPathToConfig = "";  // default on QNX /etc/mm/aoa.conf

AOADynamicDevice::AOADynamicDevice(const std::string& name,
                                   const DeviceUID& unique_id,
                                   const AOAWrapper::AOAUsbInfo& info,
                                   TransportAdapterController* controller)
    : AOADevice(name, unique_id),
      observer_(new ScannerObserver(this)),
      controller_(controller) {
  if (kPathToConfig.empty()) {
    AOAWrapper::Init(info, observer_);
  } else {
    AOAWrapper::Init(kPathToConfig, info, observer_);
  }
}

AOADynamicDevice::AOADynamicDevice(const std::string& name,
                                   const DeviceUID& unique_id,
                                   TransportAdapterController* controller)
    : AOADevice(name, unique_id),
      observer_(new ScannerObserver(this)),
      controller_(controller) {
  if (kPathToConfig.empty()) {
    AOAWrapper::Init(observer_);
  } else {
    AOAWrapper::Init(kPathToConfig, observer_);
  }
}

AOADynamicDevice::~AOADynamicDevice() {
  AOAWrapper::Shutdown();
  delete observer_;
  observer_ = NULL;
}

void AOADynamicDevice::SetHandle(AOAWrapper::AOAHandle hdl) {
  // Now only one device supported
  handle_ = hdl;
  DeviceVector devices;
  devices.push_back(DeviceSptr(this));
  controller_->SearchDeviceDone(devices);
}

AOADynamicDevice::ScannerObserver::ScannerObserver(AOADynamicDevice* parent)
    : parent_(parent) {
}

void AOADynamicDevice::ScannerObserver::OnConnectedDevice(
    AOAWrapper::AOAHandle hdl) {
  LOG4CXX_TRACE(logger_, "AOA: new device is connected");
  parent_->SetHandle(hdl);
}

}  // namespace transport_adapter
}  // namespace transport_manager
