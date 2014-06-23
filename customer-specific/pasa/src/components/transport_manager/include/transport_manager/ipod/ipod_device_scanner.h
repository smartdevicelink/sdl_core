/**
 * \file ipod_device_scanner.h
 * \brief ipodDeviceScanner class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_IPOD_IPOD_DEVICE_SCANNER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_IPOD_IPOD_DEVICE_SCANNER_H_

#include <mqueue.h>
#include <applink_types.h>

#include "transport_manager/transport_adapter/device_scanner.h"
#include "utils/conditional_variable.h"
#include "utils/lock.h"

#include "transport_manager/ipod/ipod_device.h"
#define MAX_QUEUE_NAME_SIZE     24
#define MAX_QUEUE_MSG_SIZE 		4095
#define MSGQ_MAX_MESSAGES		128
#define MSGQ_MESSAGE_SIZE 		MAX_QUEUE_MSG_SIZE

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController;

// Todd: Ipod support

/**
 * @brief Scan for devices using ipod.
 */
class IpodDeviceScanner : public DeviceScanner {
  public:

  /**
   * @brief Constructor.
   * @param controller Transport adapter controller
   * @param auto_repeat_search true - autorepeated or continous device search, false - search on demand
   * @param repeat_search_pause_sec - pause between device searches, 0 means continous search
   */
  IpodDeviceScanner(TransportAdapterController* controller,
                         bool auto_repeat_search, int repeat_search_pause_sec);

    /**
     * @brief Destructor.
     */
    ~IpodDeviceScanner();

  /**
   * @brief Main thread initialization.
   */
  void Thread();

  mqd_t openMsgQ(const char *queue_name, bool sender, bool block);

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

   static void* handlePASAFrameworkIncomingMessages(void *data);
   void connectIpodDevice(void *data);
   void disconnectIpodDevice(void *data);
   int sendMsg(mqd_t q_fd, uint8_t msgType, uint32_t length, const void *data);

  /**
   * @brief Summarizes the total list of devices (paired and scanned) and notifies controller
   */
  void UpdateTotalDeviceList();

  /**
   * @brief Performs search of SDL-enabled devices
   */
  void DoInquiry();

  TransportAdapterController* controller_;
  pthread_t thread_;

// Todd: Ipod support
  pthread_t mPASAFWMsgHandlerThread;
  bool thread_started_;
  bool shutdown_requested_;
  bool device_scan_requested_;
  bool ready_;
  sync_primitives::Lock device_scan_requested_lock_;
  sync_primitives::ConditionalVariable device_scan_requested_cv_;

  DeviceVector paired_devices_with_sdl_;
  DeviceVector found_devices_with_sdl_;

  /**
   * @brief UUID of SmartDeviceLink service.
   **/
//  uuid_t smart_device_link_service_uuid_;

  const bool auto_repeat_search_;
  const int auto_repeat_pause_sec_;

// Todd: Ipod Support
  IpodDevice* mIncomingDevice;
  mqd_t mPASAFWSendHandle;

};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif /* IPOD_DEVICE_SCANNER_H_ */
