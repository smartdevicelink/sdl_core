/**
 * \file device_adapter_event.h
 * \brief DeviceAdapterEvent class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_EVENT
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_EVENT

namespace transport_manager {

namespace device_adapter {
class DeviceAdapter;
}

using device_adapter::DeviceAdapter;

class DeviceAdapterEvent {
 public:
  bool operator ==(const DeviceAdapterEvent &other);
  DeviceAdapterEvent(int type,
                     DeviceAdapter *device_adapter,
                     const DeviceHandle &device_handle,
                     const ApplicationHandle& application_id,
                     RawMessageSptr data,
                     BaseError *error);
  ~DeviceAdapterEvent();
  void set_event_type(int type);
  void set_connection_uid(ConnectionUID id);
  void set_device_adapter(device_adapter::DeviceAdapter *device_adapter);
  void set_data(RawMessageSptr data);
  void set_error(BaseError *error);
  void set_device_handle(const DeviceHandle &device_handle);

  const DeviceHandle &device_handle() const;
  int event_type(void) const;
  ApplicationHandle application_id(void) const;
  device_adapter::DeviceAdapter *device_adapter(void) const;
  RawMessageSptr data(void) const;
  BaseError *event_error(void) const;

 private:
  DeviceHandle device_handle_;
  int event_type_;
  ApplicationHandle application_id_;
  device_adapter::DeviceAdapter *device_adapter_;
  RawMessageSptr event_data_;
  BaseError *event_error_;
};

}  // namespace

#endif // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_EVENT
