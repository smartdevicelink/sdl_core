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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_DEVICE_SCANNER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_DEVICE_SCANNER_H_

#include <applink_types.h>

#include "transport_manager/transport_adapter/device_scanner.h"
#include "utils/conditional_variable.h"
#include "utils/lock.h"

#include "transport_manager/pasa_bt/bluetooth_PASA_device.h"

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController;

/**
 * @brief Scan for devices using bluetooth.
 */
class BluetoothPASADeviceScanner : public DeviceScanner {
 public:

  /**
   * @brief Constructor.
   * @param controller Transport adapter controller
   * @param auto_repeat_search true - autorepeated or continous device search, false - search on demand
   * @param repeat_search_pause_sec - pause between device searches, 0 means continous search
   */
  BluetoothPASADeviceScanner(TransportAdapterController* controller,
                         bool auto_repeat_search, int repeat_search_pause_sec);
  /**
   * @brief Main thread
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
  /**
   * @brief Waits for external scan request or time out for repeated search or terminate request
   */
  void TimedWaitForDeviceScanRequest();

  /**
   * @brief Recieve PASA framework mq messages and convert it to SDL BT messages
   */
  static void* handlePASAFrameworkIncomingMessages(void *data);

  /**
   * @brief Connect BT device
   * Called on PASA FW BT SPP Connect Message
   */
  void connectBTDevice(void *data);

  /**
   * @brief Disconnect BT device
   * Called on PASA FW BT Disconnect Message
   */
  void disconnectBTDevice(void *data);
  /**
   * @brief Disconnect SPP (close connection)
   * Called on PPASA FW BT SPP Disconnect Message
   */
  void disconnectBTDeviceSPP(void *data);

  /**
   * @brief Summarizes the total list of devices (paired and scanned) and notifies controller
   * Called on PASA FW BT SPP END Message
   */
  void UpdateTotalDeviceList();

  /**
   * @brief Summarizes the total list of applications and notifies controller
   * Called on PASA FW BT SPP END Message
   */
  void UpdateTotalApplicationList();

  TransportAdapterController* controller_;
  pthread_t bt_device_scanner_thread_;

  pthread_t bt_PASA_msg_thread_;
  bool thread_started_;
  bool shutdown_requested_;
  bool device_scan_requested_;
  mutable sync_primitives::Lock device_scan_requested_lock_;
  sync_primitives::ConditionalVariable device_scan_requested_cv_;

  DeviceVector found_devices_with_sdl_;
  mutable sync_primitives::Lock devices_lock_;

  const bool auto_repeat_search_;
  const int auto_repeat_pause_sec_;

  mqd_t mPASAFWSendHandle;
  mqd_t mq_ToSDL;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif /* BLUETOOTH_DEVICE_SCANNER_H_ */
