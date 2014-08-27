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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_AOA_PPS_DEVICE_SCANNER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_AOA_PPS_DEVICE_SCANNER_H_

#include <map>
#include <string>

#include "utils/lock.h"
#include "utils/threads/thread.h"
#include "utils/threads/pulse_thread_delegate.h"

#include "transport_manager/transport_adapter/device_scanner.h"
#include "transport_manager/aoa/aoa_dynamic_device.h"
#include "transport_manager/aoa/aoa_wrapper.h"

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController;

class PPSDeviceScanner : public DeviceScanner {
 public:
  explicit PPSDeviceScanner(TransportAdapterController* controller);
  ~PPSDeviceScanner();

 protected:
  virtual TransportAdapter::Error Init();
  virtual TransportAdapter::Error Scan();
  virtual void Terminate();
  virtual bool IsInitialised() const;

 private:
  typedef std::map<DeviceUID, AOADevicePtr> DeviceContainer;

  static const std::string kUSBStackPath;
  static const std::string kPpsPathRoot;
  static const std::string kPpsPathAll;
  static const std::string kPpsPathCtrl;
  bool initialised_;
  TransportAdapterController* controller_;
  DeviceContainer devices_;
  sync_primitives::Lock devices_lock_;
  int fd_;
  threads::Thread* thread_;

  bool OpenPps();
  void ClosePps();
  bool ArmEvent(struct sigevent* event);
  void Process(uint8_t* buf, size_t size);
  std::string ParsePps(char* ppsdata, const char** vals);
  void WritePpsData(const char* objname, const char** attrs);
  bool CheckData(const char** attrs);
  void FillUsbInfo(const std::string& object_name, const char** attrs,
                   AOAWrapper::AOAUsbInfo* info);
  void AddDevice(const AOAWrapper::AOAUsbInfo& aoa_usb_info);
  void NotifyDevicesUpdated();

  class PpsThreadDelegate : public threads::PulseThreadDelegate {
   public:
    explicit PpsThreadDelegate(PPSDeviceScanner* parent);

   protected:
    virtual bool ArmEvent(struct sigevent* event);
    virtual void OnPulse();

   private:
    PPSDeviceScanner* parent_;
  };
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_AOA_PPS_DEVICE_SCANNER_H_
