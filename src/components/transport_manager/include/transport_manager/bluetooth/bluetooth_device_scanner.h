/**
 * \file bluetooth_device_scanner.h
 * \brief BluetoothDeviceScanner class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_DEVICE_SCANNER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_DEVICE_SCANNER_H_

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

/**
 * @brief Scan for devices using bluetooth.
 */
class BluetoothDeviceScanner : public DeviceScanner {
 public:

  /**
   * @brief Constructor.
   */
  BluetoothDeviceScanner(DeviceAdapterController* controller);

  /**
   * @brief Destructor.
   */
  ~BluetoothDeviceScanner();

  /**
   * @brief Main thread initialization.
   */
  void thread();
 protected:

  /**
   * @brief Start device scanner.
   *
   * @return Error information about reason of initialization failure.
   */
  virtual DeviceAdapter::Error init();

  /**
   * @brief
   */
  virtual void terminate();

  /**
   * @brief
   *
   * @return Error information about reason of Scan failure.
   */
  virtual DeviceAdapter::Error Scan();

  /**
   * @brief Check device scanner for initialization.
   *
   * @return true - initialized.
   * false - not initialized.
   */
  virtual bool IsInitialised() const;
 private:

  typedef std::vector<uint8_t> RfcommChannelVector;

  bool WaitForDeviceScanRequest();
  RfcommChannelVector DiscoverSmartDeviceLinkRFCOMMChannels(
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
