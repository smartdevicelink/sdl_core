/*
 * \file bluetooth_device.h
 * \brief BluetoothDevice class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_DEVICE_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_DEVICE_H_

#ifdef OS_WINDOWS
#include "utils/winhdr.h"
#include <ws2bth.h>
#include <BluetoothAPIs.h>
#else
#include <bluetooth/bluetooth.h>
#endif
#include <vector>

#include "transport_manager/common.h"
#include "transport_manager/transport_adapter/device.h"

namespace transport_manager {
namespace transport_adapter {

/**
 * @brief Type definition for vector that holds uint8_t variables.
 */
typedef std::vector<uint8_t> RfcommChannelVector;

/**
 * @brief Information about device that use bluetooth transport.
 */
class BluetoothDevice : public Device {
 public:
#if defined(OS_WINDOWS)
#define BLUETOOTH_ADDR_INFO BLUETOOTH_DEVICE_INFO
#else
#define BLUETOOTH_ADDR_INFO bdaddr_t
#endif
  /**
   * @brief Return device unique identifier.
   *
   * @return string with device unique identifier.
   */
  static std::string GetUniqueDeviceId(
      const BLUETOOTH_ADDR_INFO& device_address);

  /**
   * @brief Constructor.
   *
   * @param address Bluetooth address.
   * @param name Human-readable device name.
   * @param rfcomm_channels List of RFCOMM channels where SmartDeviceLink
   *service
   * @param sock_addr_bth_server reference on SOCKADDR_BTH for connect to the
   *SmartDeviceLink service
   *has been discovered.
   **/
  BluetoothDevice(const BLUETOOTH_ADDR_INFO& device_address,
                  const char* device_name,
                  const RfcommChannelVector& rfcomm_channels
#if defined(OS_WINDOWS)
                  ,
                  const SOCKADDR_BTH& sock_addr_bth_server);
#else
                  );
#endif
  /**
   * @brief Compare devices.
   *
   * This method checks whether two SBluetoothDevice structures
   * refer to the same device.
   *
   * @param other Device to compare with.
   *
   * @return true if devices are equal, false otherwise.
   **/
  virtual bool IsSameAs(const Device* other) const;

  bool GetRfcommChannel(const ApplicationHandle app_handle,
                        uint8_t* channel_out);

  /**
   * @brief Update list of applications available on device.
   *
   * @return Container with list of applications.
   */
  virtual ApplicationList GetApplicationList() const;

  /**
   * @brief Return device bluetooth address.
   *
   * @return Device bluetooth address.
   */
  const BLUETOOTH_ADDR_INFO& address() const;

#if defined(OS_WINDOWS)
  SOCKADDR_BTH getSocketBthAddr();
#endif
 private:
  /**
   * @brief Device bluetooth address.
   **/
  BLUETOOTH_ADDR_INFO address_;

#if defined(OS_WINDOWS)
  /**
  * @brief windows struct for bluetooth connection
  **/
  SOCKADDR_BTH sock_addr_bth_server_;
#endif

  /* @brief List of RFCOMM channels where SmartDeviceLink service has been
  *discovered.
  **/
  RfcommChannelVector rfcomm_channels_;
};

inline const BLUETOOTH_ADDR_INFO& BluetoothDevice::address() const {
  return address_;
}

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_DEVICE_H_
