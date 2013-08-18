/**
 * \file bluetooth_device.h
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_DEVICE_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_DEVICE_H_

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>

#include <vector>
#include <map>

#include "transport_manager/common.h"
#include "transport_manager/device_adapter/device.h"

namespace transport_manager {
namespace device_adapter {

typedef std::vector<uint8_t> RfcommChannelVector;
typedef std::map<ApplicationHandle, uint8_t> RfcommChannels;
class BluetoothDevice : public Device {
 public:

  static std::string getUniqueDeviceId(const bdaddr_t& device_address);

  /**
   * @brief Constructor.
   *
   * @param address Bluetooth address.
   * @param name Human-readable device name.
   * @param rfcomm_channels List of RFCOMM channels where SmartDeviceLink service has been discovered.
   **/
  BluetoothDevice(const bdaddr_t& address, const char* name,
                  const RfcommChannelVector& rfcomm_channels);

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
  virtual bool isSameAs(const Device* other) const;

  bool getRfcommChannel(const ApplicationHandle app_handle,
                        uint8_t* channel_out);

  virtual ApplicationList getApplicationList() const;

  const bdaddr_t& address() const {
    return address_;
  }

 private:
  /**
   * @brief Device bluetooth address.
   **/
  bdaddr_t address_;

  /**
   * @brief List of RFCOMM channels where SmartDeviceLink service has been discovered.
   **/
  RfcommChannels rfcomm_channels_;

  ApplicationHandle next_application_handle_;
};

}  // namespace device_adapter
}  // namespace transport_manager

#endif // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_DEVICE_H_
