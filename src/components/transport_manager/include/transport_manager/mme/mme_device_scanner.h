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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_MME_DEVICE_SCANNER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_MME_DEVICE_SCANNER_H_

#include <map>

#include <mqueue.h>
#include <qdb/qdb.h>

#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "utils/lock.h"
#include "transport_manager/transport_adapter/device_scanner.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

#include "transport_manager/mme/mme_device.h"
#include "transport_manager/mme/pasa.h"

#define QNX_BARE_SYSTEM_WORKAROUND 0

namespace transport_manager {
namespace transport_adapter {

class MmeDeviceScanner : public DeviceScanner {
 public:
  MmeDeviceScanner(TransportAdapterController* controller);
  virtual ~MmeDeviceScanner();

  virtual TransportAdapter::Error Init();
  virtual TransportAdapter::Error Scan();
  virtual void Terminate();
  virtual bool IsInitialised() const;

private:
  typedef uint64_t qdb_int;
  typedef qdb_int msid_t;
  typedef std::vector<msid_t> MsidContainer;
  typedef std::map<msid_t, MmeDevicePtr> DeviceContainer;

  void OnDeviceArrived(const MmeDeviceInfo* mme_device_info);
  void OnDeviceLeft(const MmeDeviceInfo* mme_device_info);
  void NotifyDevicesUpdated();
  bool GetMmeList(MsidContainer& msids);
  bool GetMmeInfo(
    msid_t msid,
    std::string& mount_point,
    MmeDevice::Protocol& protocol,
    std::string& unique_device_id,
    std::string& vendor,
    std::string& product,
    bool& attached
  );

  static const char* event_mq_name;
  static const char* ack_mq_name;

  TransportAdapterController* controller_;
  bool initialised_;
  mqd_t event_mqd_;
  mqd_t ack_mqd_;
  qdb_hdl_t* qdb_hdl_;
  utils::SharedPtr<threads::Thread> notify_thread_;
  DeviceContainer devices_;
  sync_primitives::Lock devices_lock_;

  class NotifyThreadDelegate : public threads::ThreadDelegate {
   public:
    NotifyThreadDelegate(mqd_t event_mqd, mqd_t ack_mqd, MmeDeviceScanner* parent);
    virtual void threadMain();

   private:
    static const int kBufferSize = MAX_QUEUE_MSG_SIZE;
    static const int kAckBufferSize = MAX_QUEUE_MSG_SIZE;

    MmeDeviceScanner* parent_;
    mqd_t event_mqd_;
    mqd_t ack_mqd_;
    bool run_;
    char buffer_[kBufferSize];
    char ack_buffer_[kAckBufferSize];
  };
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_MME_DEVICE_SCANNER_H_
