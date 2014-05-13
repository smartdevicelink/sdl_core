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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_IAP_DEVICE_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_IAP_DEVICE_H_

#include <map>
#include <ipod/ipod.h>

#include "utils/threads/thread.h"
#include "utils/threads/pulse_thread_delegate.h"
#include "utils/lock.h"

#include "transport_manager/mme/mme_device.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {

class IAPConnection;

class IAPDevice : public MmeDevice {
 public:
  IAPDevice(const std::string& mount_point,
            const std::string& name,
            const DeviceUID& unique_device_id,
            TransportAdapterController* controller);

  ~IAPDevice();

  virtual Protocol protocol() const {
    return IAP;
  }

 protected:
  virtual ApplicationList GetApplicationList() const;

 private:
  ipod_hdl_t* RegisterConnection(ApplicationHandle app_id, IAPConnection* connection);
  void UnregisterConnection(ApplicationHandle app_id);
  void OnSessionOpened(uint32_t protocol_id, int session_id);
  void OnSessionClosed(int session_id);
  void OnDataReady(int session_id);

  TransportAdapterController* controller_;
  ipod_hdl_t* ipod_hdl_;
  utils::SharedPtr<threads::Thread> receiver_thread_;
  ApplicationHandle last_app_id_;

  typedef std::map<uint32_t, ApplicationHandle> AppContainer;
  AppContainer apps_;
  mutable sync_primitives::Lock apps_lock_;

  typedef std::map<int, ApplicationHandle> AppTable;
  AppTable app_table_;
  sync_primitives::Lock app_table_lock_;

  typedef std::map<ApplicationHandle, IAPConnection*> ConnectionContainer;
  ConnectionContainer connections_;
  sync_primitives::Lock connections_lock_;

  class IAPEventThreadDelegate : public threads::PulseThreadDelegate {
   public:
    IAPEventThreadDelegate(ipod_hdl_t* ipod_hdl, IAPDevice* parent);

   protected:
    virtual bool ArmEvent(struct sigevent* event);
    virtual void OnPulse();

   private:
    static const size_t kEventsBufferSize = 32;
    static const int kProtocolNameSize = 256;

    void ParseEvents();
    void AcceptSession(uint32_t protocol_id);
    void AcceptSession(uint32_t protocol_id, const char* protocol_name);
    void CloseSession(uint32_t session_id);
    void DataReady(uint32_t session_id);
    void OpenSession(uint32_t protocol_id);
    void OpenSession(uint32_t protocol_id, const char* protocol_name);

    IAPDevice* parent_;
    ipod_hdl_t* ipod_hdl_;
    ipod_eaf_event_t events_[kEventsBufferSize];
  };

  friend class IAPConnection;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_IAP_DEVICE_H_
