/*
 * \file fake_device_adapter.cpp
 * \brief Fake device adapter for test without real devices
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

#include "transport_manager/fake_device_adapter.h"

#include <algorithm>
#include <functional>

#include "transport_manager/common.h"

using ::transport_manager::ApplicationList;
using ::transport_manager::ApplicationHandle;
using ::transport_manager::ConnectError;
using ::transport_manager::DeviceHandle;
using ::transport_manager::SearchDeviceError;
using ::transport_manager::RawMessageSptr;
using ::transport_manager::device_adapter::Connection;
using ::transport_manager::device_adapter::Device;
using ::transport_manager::device_adapter::DeviceAdapter;
using ::transport_manager::device_adapter::DeviceAdapterController;
using ::transport_manager::device_adapter::DeviceScanner;
using ::transport_manager::device_adapter::DeviceSptr;
using ::transport_manager::device_adapter::DeviceVector;
using ::transport_manager::device_adapter::ServerConnectionFactory;

namespace test {
namespace components {
namespace transport_manager {

class FakeDevice : public Device {
 public:
  FakeDevice(std::string name) : Device(name, name) {}
  bool isSameAs(const Device* other_device) const {
    return other_device->name() == name();
  }
  ApplicationList getApplicationList() const {
    return ApplicationList();
  }
};

class FakeConnection : public Connection {
 public:
  FakeConnection(const DeviceHandle& device_handle,
                 const ApplicationHandle& app_handle,
                 FakeDeviceAdapter *adapter)
    : adapter_(adapter),
      device_handle_(device_handle),
      app_handle_(app_handle) {}
  DeviceAdapter::Error sendData(RawMessageSptr message) {
    return DeviceAdapter::OK;
  }
  DeviceAdapter::Error disconnect() {
    ::std::pair<DeviceHandle, ApplicationHandle> con = ::std::make_pair(device_handle_, app_handle_);
    if (::std::find(adapter_->connections_.begin(),
                    adapter_->connections_.end(),
                    con) != adapter_->connections_.end()) {
      adapter_->connections_.remove(con);
      adapter_->disconnectDone(device_handle_, app_handle_);
//      if (::std::find_if(adapter_->connections_.begin(),
//                         adapter_->connections_.end()))
      adapter_->disconnectDevice(device_handle_);
    } else {
      // TODO: add disconnect failed
    }
    return DeviceAdapter::OK;
  }
 private:
  FakeDeviceAdapter *adapter_;
  const DeviceHandle device_handle_;
  const ApplicationHandle app_handle_;
};

class FakeDeviceScanner : public DeviceScanner {
 public:
  FakeDeviceScanner(FakeDeviceAdapter *adapter) : adapter_(adapter) {}
 protected:
  DeviceAdapter::Error init() { return DeviceAdapter::OK; }
  DeviceAdapter::Error scan() {
    if (!adapter_->devices_.empty()) {
      adapter_->searchDeviceDone(adapter_->devices_);
    } else {
      adapter_->searchDeviceFailed(SearchDeviceError());
    }
    return DeviceAdapter::OK;
  }
  void terminate() {}
  bool isInitialised() const { return true; }
  ~FakeDeviceScanner() {}
 private:
  FakeDeviceAdapter *adapter_;
};

struct Search : public ::std::unary_function<DeviceSptr, bool> {
  Search(const DeviceSptr dev) { device_ = dev; }
  bool operator() (const DeviceSptr d) {
    return d->name() == device_->name();
  }
 private:
  DeviceSptr device_;
};

class FakeConnectionFactory : public ServerConnectionFactory {
 public:
  FakeConnectionFactory(FakeDeviceAdapter *adapter) : adapter_(adapter) {}
  DeviceAdapter::Error init() { return DeviceAdapter::OK; }
  DeviceAdapter::Error createConnection(const DeviceHandle& device_handle,
                                          const ApplicationHandle& app_handle) {

    if (adapter_->devices_.end() != ::std::find_if(adapter_->devices_.begin(),
                                                   adapter_->devices_.end(),
                                                   Search(new FakeDevice(device_handle)))) {
      adapter_->connectDone(device_handle, app_handle);
    } else {
      adapter_->connectFailed(device_handle, app_handle, ConnectError());
    }
    return DeviceAdapter::OK;
  }
  void terminate() {}
  bool isInitialised() const { return true; }
  ~FakeConnectionFactory() {}
 private:
  FakeDeviceAdapter *adapter_;
};

FakeDeviceAdapter::FakeDeviceAdapter()
    : DeviceAdapterImpl(new FakeDeviceScanner(this),
                        new FakeConnectionFactory(this), 0) {}

void FakeDeviceAdapter::addDevice(std::string name) {
  devices_.push_back(new FakeDevice(name));
}

void FakeDeviceAdapter::clearDevices() {
  devices_.clear();
}

void FakeDeviceAdapter::addConnection(const DeviceHandle& device_id,
                                      const ApplicationHandle& app_id) {
  DeviceAdapterImpl::connectionCreated(new FakeConnection(device_id, app_id,this),
                                       device_id, app_id);
  connections_.push_back(::std::make_pair(device_id, app_id));
}

void FakeDeviceAdapter::clearConnection() {
  connections_.clear();
}

}  // namespace transport_manager
}  // namespace components
}  // namespace test

