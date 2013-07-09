/**
 * \file device_adapter_listener.h
 * \brief interface for DeviceAdapterListener header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_LISTENER
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_LISTENER

#include "transport_manager/common.h"
#include "transport_manager/device_adapter.h"

namespace transport_manager {

class DeviceAdapterListener {
 public:
  DeviceAdapterListener *next;
  DeviceAdapterListener()
    : next(NULL) { }
  class DeviceAdapterEvent {
   public:
    bool operator ==(const DeviceAdapterEvent &other);
    DeviceAdapterEvent(int type, SessionID session_id,
                       DeviceAdapter *device_adapter, RawMessageSptr data,
                       DeviceAdapterError *error);
    ~DeviceAdapterEvent();
    void set_event_type(int type);
    void set_session_id(int id);
    void set_device_adapter(DeviceAdapter *device_adapter);
    void set_data(RawMessageSptr data);
    void set_error(DeviceAdapterError *error);

    int event_type(void) const;
    int session_id(void) const;
    DeviceAdapter *device_adapter(void) const;
    RawMessageSptr data(void) const;
    DeviceAdapterError *error(void) const;

   private:
    int event_type_;
    int session_id_;
    DeviceAdapter *device_adapter_;
    RawMessageSptr event_data_;
    DeviceAdapterError *event_error_;
  };

  virtual ~DeviceAdapterListener();

 protected:
  virtual void onSearchDeviceDone(const DeviceAdapter* device_adapter) = 0;
 public:
  void raiseSearchDeviceDone(const DeviceAdapter* device_adapter) {
    for (DeviceAdapterListener *l = this; l; l = l->next) {
      l->onSearchDeviceDone(device_adapter);
    }
  }

 protected:
  virtual void onSearchDeviceFailed(const DeviceAdapter* device_adapter,
                                    const SearchDeviceError& error) = 0;
 public:
  void raiseSearchDeviceFailed(const DeviceAdapter* device_adapter,
                               const SearchDeviceError& error) {
    for (DeviceAdapterListener *l = this; l; l = l->next) {
      l->onSearchDeviceFailed(device_adapter, error);
    }
  }

 protected:
  virtual void onConnectDone(const DeviceAdapter* device_adapter,
                             const transport_manager::SessionID session_id) = 0;
 public:
  void raiseConnectDone(const DeviceAdapter* device_adapter,
                        const transport_manager::SessionID session_id) {
    for (DeviceAdapterListener *l = this; l; l = l->next) {
      l->onConnectDone(device_adapter, session_id);
    }
  }

 protected:
  virtual void onConnectFailed(const DeviceAdapter* device_adapter,
                               const transport_manager::SessionID session_id,
                               const ConnectError& error) = 0;
 public:
  void raiseConnectFailed(const DeviceAdapter* device_adapter,
                          const transport_manager::SessionID session_id,
                          const ConnectError& error) {
    for (DeviceAdapterListener *l = this; l; l = l->next) {
      l->onConnectFailed(device_adapter, session_id, error);
    }
  }

 protected:
  virtual void onDisconnectDone(const DeviceAdapter* device_adapter,
                                const SessionID session_id) = 0;
 public:
  void raiseDisconnectDone(const DeviceAdapter* device_adapter,
                           const transport_manager::SessionID session_id) {
    for (DeviceAdapterListener *l = this; l; l = l->next) {
      l->onDisconnectDone(device_adapter, session_id);
    }
  }

 protected:
  virtual void onDisconnectFailed(const DeviceAdapter* device_adapter,
                                  const SessionID session_id,
                                  const DisconnectError& error) = 0;
 public:
  void raiseDisconnectFailed(const DeviceAdapter* device_adapter,
                             const transport_manager::SessionID session_id,
                             const DisconnectError& error) {
    for (DeviceAdapterListener *l = this; l; l = l->next) {
      l->onDisconnectFailed(device_adapter, session_id, error);
    }
  }

 protected:
  virtual void onDisconnectDeviceDone(const DeviceAdapter* device_adapter,
                                      const SessionID session_id) = 0;
 public:
  void raiseDisconnectDeviceDone(
      const DeviceAdapter* device_adapter,
      const transport_manager::SessionID session_id) {
    for (DeviceAdapterListener *l = this; l; l = l->next) {
      l->onDisconnectDeviceDone(device_adapter, session_id);
    }
  }

 protected:
  virtual void onDisconnectDeviceFailed(const DeviceAdapter* device_adapter,
                                        const SessionID session_id,
                                        const DisconnectDeviceError& error) = 0;
 public:
  void raiseDisconnectDeviceFailed(
      const DeviceAdapter* device_adapter,
      const transport_manager::SessionID session_id,
      const DisconnectDeviceError& error) {
    for (DeviceAdapterListener *l = this; l; l = l->next) {
      l->onDisconnectDeviceFailed(device_adapter, session_id, error);
    }
  }

 protected:
  virtual void onDataSendDone(const DeviceAdapter* device_adapter,
                              const SessionID session_id,
                              const RawMessageSptr data_container) = 0;
 public:
  void raiseDataSendDone(const DeviceAdapter* device_adapter,
                         const SessionID session_id,
                         const RawMessageSptr data_container) {
    for (DeviceAdapterListener *l = this; l; l = l->next) {
      l->onDataSendDone(device_adapter, session_id, data_container);
    }
  }

 protected:
  virtual void onDataSendFailed(const DeviceAdapter* device_adapter,
                                const SessionID session_id,
                                const RawMessageSptr data_container,
                                const DataSendError& error) = 0;
 public:
  void raiseDataSendFailed(const DeviceAdapter* device_adapter,
                           const SessionID session_id,
                           const RawMessageSptr data_container,
                           const DataSendError& error) {
    for (DeviceAdapterListener *l = this; l; l = l->next) {
     l->onDataSendFailed(device_adapter, session_id, data_container, error);
    }
  }

 protected:
  virtual void onDataReceiveDone(const DeviceAdapter* device_adapter,
                                 const SessionID session_id,
                                 const RawMessageSptr data_container) = 0;
 public:
  void raiseDataReceiveDone(const DeviceAdapter* device_adapter,
                            const SessionID session_id,
                            const RawMessageSptr data_container) {
    for (DeviceAdapterListener *l = this; l; l = l->next) {
      l->onDataReceiveDone(device_adapter, session_id, data_container);
    }
  }

 protected:
  virtual void onDataReceiveFailed(const DeviceAdapter* device_adapter,
                                   const SessionID session_id,
                                   const DataReceiveError& error) = 0;
public:
  void raiseDataReceiveFailed(const DeviceAdapter* device_adapter,
                              const SessionID session_id,
                              const DataReceiveError& error) {
    for (DeviceAdapterListener *l = this; l; l = l->next) {
      l->onDataReceiveFailed(device_adapter, session_id, error);
    }
  }

protected:
  virtual void onCommunicationError(const DeviceAdapter* device_adapter,
                                    const SessionID session_id) = 0;
public:
  void raiseCommunicationError(const DeviceAdapter* device_adapter,
                               const SessionID session_id) {
    for (DeviceAdapterListener *l = this; l; l = l->next) {
      l->onCommunicationError(device_adapter, session_id);
    }
  }
};
}  //namespace
#endif // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_LISTENER
