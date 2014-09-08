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

#include "transport_manager/transport_adapter/device_scanner.h"
#include "utils/conditional_variable.h"
#include "utils/lock.h"
#include "utils/threads/thread.h"

namespace transport_manager {
namespace transport_adapter {
class TransportAdapterController;

/**
 * @brief Scan for devices using bluetooth.
 */
class BluetoothDeviceScanner : public DeviceScanner {
 public:
  /**
   * @brief Constructor.
   * @param controller Transport adapter controller
   * @param auto_repeat_search true - autorepeated or continous device search, false - search on demand
   * @param repeat_search_pause_sec - pause between device searches, 0 means continous search
   */
  BluetoothDeviceScanner(TransportAdapterController* controller,
                         bool auto_repeat_search, int repeat_search_pause_sec);
  /**
   * @brief Destructor.
   */
  ~BluetoothDeviceScanner();
  /**
   * @brief Main thread initialization.
   */
  void Thread();
 protected:

  /**
   * @brief Start device scanner.
   *
   * @return Error information about reason of initialization failure.
   */
  virtual TransportAdapter::Error Init();

  /**
   * @brief Terminates device scanner thread
   */
  virtual void Terminate();

  /**
   * @brief Request device search
   *
   * @return Error information about reason of Scan failure.
   */
  virtual TransportAdapter::Error Scan();

  /**
   * @brief Check device scanner for initialization.
   *
   * @return true - initialized.
   * false - not initialized.
   */
  virtual bool IsInitialised() const;
 private:

  class BluetoothDeviceScannerDelegate: public threads::ThreadDelegate {
   public:
    explicit BluetoothDeviceScannerDelegate(BluetoothDeviceScanner* scanner);
    void threadMain() OVERRIDE;
   private:
    BluetoothDeviceScanner* scanner_;
  };

  typedef std::vector<uint8_t> RfcommChannelVector;

  /**
   * @brief Waits for external scan request or time out for repeated search or terminate request
   */
  void TimedWaitForDeviceScanRequest();

  /**
   * @brief Finds RFCOMM-channels of SDL enabled applications for set of devices
   * @param device_addresses Bluetooth addresses to search on
   * @return List of RFCOMM-channels lists
   */
  std::vector<RfcommChannelVector> DiscoverSmartDeviceLinkRFCOMMChannels(
    const std::vector<bdaddr_t>& device_addresses);

  /**
   * @brief Finds RFCOMM-channels of SDL enabled applications for given device
   * @param[out] discovered List of RFCOMM-channels to fill
   * @return true - if search was OK, false if it failed
   */
  bool DiscoverSmartDeviceLinkRFCOMMChannels(const bdaddr_t& device_address,
      RfcommChannelVector* discovered);

  /**
   * @brief Summarizes the total list of devices (paired and scanned) and notifies controller
   */
  void UpdateTotalDeviceList();

  /**
   * @brief Performs search of SDL-enabled devices
   */
  void DoInquiry();

  /**
   * @brief Checks if given devices have SDL service and creates appropriate BluetoothDevice objects
   * @param bd_address List of bluetooth addresses to check
   * @param device_handle HCI handle
   * @param[out] discovered_devices List of created BluetoothDevice objects to fill
   */
  void CheckSDLServiceOnDevices(const std::vector<bdaddr_t>& bd_address,
                                int device_handle,
                                DeviceVector* discovered_devices);

  TransportAdapterController* controller_;
  threads::Thread* thread_;
  bool shutdown_requested_;
  bool ready_;
  bool device_scan_requested_;
  sync_primitives::Lock device_scan_requested_lock_;
  sync_primitives::ConditionalVariable device_scan_requested_cv_;

  std::vector<bdaddr_t> paired_devices_;

  DeviceVector paired_devices_with_sdl_;
  DeviceVector found_devices_with_sdl_;

  /**
   * @brief UUID of SmartDeviceLink service.
   **/
  uuid_t smart_device_link_service_uuid_;

  const bool auto_repeat_search_;
  const int auto_repeat_pause_sec_;
};

}  // namespace transport_adapter
}  // namespace transport_manager
#endif /* BLUETOOTH_DEVICE_SCANNER_H_ */
