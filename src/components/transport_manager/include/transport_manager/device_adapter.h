/**
 * \file device_adapter.h
 * \brief DeviceAdapter class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_DEVICE_ADAPTER_DEVICE_ADAPTER
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_DEVICE_ADAPTER_DEVICE_ADAPTER

#include <string>
#include <vector>
#include <list>
#include "utils/shared_ptr.h"
#include "transport_manager/common.h"
#include "transport_manager/error.h"

namespace transport_manager {

class DeviceHandleGenerator;

namespace device_adapter {

class Configuration;
class DeviceAdapterListener;
typedef std::string DeviceType;
typedef int ApplicationHandle;
typedef std::vector<ApplicationHandle> ApplicationList;
typedef std::vector<DeviceHandle> DeviceList;
typedef std::list<DeviceAdapterListener *> DeviceAdapterListenerList;

class DeviceAdapter {
 public:
  enum Error {
    OK,
    FAIL,
    NOT_SUPPORTED,
    ALREADY_EXIST,
    BAD_STATE,
    BAD_PARAM
  };

 public:
  virtual ~DeviceAdapter() { }

  virtual DeviceType getDeviceType() const = 0;

  /* TODO
   virtual Error LoadState(DeviceAdapterState* state) = 0;
   virtual void SaveState(DeviceAdapterState* state) = 0;
   */

  virtual Error init(DeviceHandleGenerator* handle_generator,
                     Configuration* configuration) = 0;

  virtual void addListener(DeviceAdapterListener *listener) = 0;
  virtual bool isSearchDevicesSupported() const = 0;
  virtual Error searchDevices() = 0;

  virtual bool isServerOriginatedConnectSupported() const = 0;
  virtual Error connect(const DeviceHandle device_handle,
                        const ApplicationHandle app_handle,
                        const int session_id) = 0;

  virtual bool isClientOriginatedConnectSupported() const = 0;

  virtual Error disconnect(const int session_id) = 0;
  virtual Error disconnectDevice(const DeviceHandle device_handle) = 0;

  virtual Error sendData(const int session_id, const RawMessageSptr data) = 0;

  virtual DeviceList getDeviceList() const = 0;
  virtual ApplicationList getApplicationList(
      const DeviceHandle device_handle) const = 0;
};

} // namespace device_adapter

} // namespace transport_manager

#endif // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_DEVICE_ADAPTER_DEVICE_ADAPTER
