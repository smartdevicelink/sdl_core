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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_IAP2_DEVICE_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_IAP2_DEVICE_H_

#include <map>
#include <list>
#include <string>
#include <iap2/iap2.h>

#include "transport_manager/mme/mme_device.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "utils/shared_ptr.h"
#include "utils/threads/thread_delegate.h"
#include "utils/threads/thread.h"
#include "utils/lock.h"

namespace transport_manager {
namespace transport_adapter {

class IAP2Device : public MmeDevice {
 public:
  IAP2Device(const std::string& mount_point,
             const std::string& name,
             const DeviceUID& unique_device_id,
             TransportAdapterController* controller);

  ~IAP2Device();

  virtual bool Init();

  virtual Protocol protocol() const {
    return IAP2;
  }

 protected:
  virtual ApplicationList GetApplicationList() const;

 private:
  typedef std::map<std::string, int> ProtocolNamePool;
  typedef std::pair<std::string, iap2ea_hdl_t*> AppRecord;
  typedef std::map<ApplicationHandle, AppRecord> AppContainer;
  typedef std::map<std::string, utils::SharedPtr<threads::Thread> > ThreadContainer;

  bool RecordByAppId(ApplicationHandle app_id, AppRecord& record) const;

  void OnHubConnect(const std::string& protocol_name, iap2ea_hdl_t* handle);
  void OnConnect(const std::string& protocol_name, iap2ea_hdl_t* handle);
  void OnConnectFailed(const std::string& protocol_name);
  void OnDisconnect(ApplicationHandle app_id);

  bool ReturnToPool(const std::string& protocol_name);

  TransportAdapterController* controller_;
  int last_app_id_;

  AppContainer apps_;
  mutable sync_primitives::Lock apps_lock_;

  ProtocolNamePool free_protocol_name_pool_;
  ProtocolNamePool protocol_in_use_name_pool_;
  sync_primitives::Lock protocol_name_pool_lock_;

  ThreadContainer legacy_connection_threads_;
  ThreadContainer hub_connection_threads_;
  ThreadContainer pool_connection_threads_;
  sync_primitives::Lock pool_connection_threads_lock_;

  class IAP2HubConnectThreadDelegate : public threads::ThreadDelegate {
   public:
    IAP2HubConnectThreadDelegate(IAP2Device* parent, const std::string& protocol_name);
    void threadMain();
   private:
    static const int max_attempts = 3;  // TODO(nvaganov@luxoft.com) move to config

    IAP2Device* parent_;
    std::string protocol_name_;
  };

  class IAP2ConnectThreadDelegate : public threads::ThreadDelegate {
    public:
      IAP2ConnectThreadDelegate(IAP2Device* parent, const std::string& protocol_name);
      void threadMain();
    private:
      IAP2Device* parent_;
      std::string protocol_name_;
  };

  friend class IAP2Connection;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_IAP2_DEVICE_H_
