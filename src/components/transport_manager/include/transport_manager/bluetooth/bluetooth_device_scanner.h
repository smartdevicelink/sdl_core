/**
 * \file bluetooth_device_scanner.h
 * \brief BluetoothAdapter class header file.
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

#ifndef BLUETOOTH_DEVICE_SCANNER_H_
#define BLUETOOTH_DEVICE_SCANNER_H_

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>

#include "transport_manager/device_adapter/device_scanner.h"

namespace transport_manager {
namespace device_adapter {

class DeviceAdapterController;

class BluetoothDeviceScanner : public DeviceScanner {
 public:
  BluetoothDeviceScanner(DeviceAdapterController* controller);
  ~BluetoothDeviceScanner();
  void thread();
 protected:
  virtual DeviceAdapter::Error init();
  virtual void terminate();
  virtual DeviceAdapter::Error scan();
  virtual bool isInitialised() const;
 private:

  typedef std::vector<uint8_t> RfcommChannelVector;

  bool waitForDeviceScanRequest();
  RfcommChannelVector discoverSmartDeviceLinkRfcommChannels(
      const bdaddr_t& device_address);
  SearchDeviceError* doInquiry(DeviceVector* discovered_devices);

  DeviceAdapterController* controller_;
  pthread_t thread_;
  bool thread_started_;
  bool shutdown_requested_;
  bool device_scan_requested_;
  bool ready_;
  pthread_mutex_t device_scan_requested_mutex_;
  pthread_cond_t device_scan_requested_cond_;

  /**
   * @brief UUID of SmartDeviceLink service.
   **/
  uuid_t smart_device_link_service_uuid_;
};

}  // namespace device_adapter
}  // namespace transport_manager

#endif /* BLUETOOTH_DEVICE_SCANNER_H_ */
