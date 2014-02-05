/**
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

#ifndef SRC_COMPONENTS_RESUMPTION_INCLUDE_RESUMPTION_LAST_STATE_H_
#define SRC_COMPONENTS_RESUMPTION_INCLUDE_RESUMPTION_LAST_STATE_H_

#include <netinet/in.h>
#include <bluetooth/bluetooth.h>

#include <string>
#include <vector>

namespace resumption {

class LastState {
 public:
/**
 * @brief Type definition for application state on TCP device
 */
  typedef struct {
    uint16_t port;
  } TcpDeviceApplicationState;
/**
 * @brief Type definition for container of application states on TCP device
 */
  typedef std::vector<TcpDeviceApplicationState> TcpDeviceApplicationStateContainer;
/**
 * @brief Type definition for TCP device state
 */
  typedef struct {
    in_addr_t in_addr;
    std::string name;
    TcpDeviceApplicationStateContainer applications;
  } TcpDeviceState;
/**
 * @brief Type definition for container of TCP device states
 */
  typedef std::vector<TcpDeviceState> TcpDeviceStateContainer;
/**
 * @brief Type definition for application state on Bluetooth device
 */
  typedef struct {
    uint8_t rfcomm_channel;
  } BluetoothDeviceApplicationState;
/**
 * @brief Type definition for container of application states on Bluetooth device
 */
  typedef std::vector<BluetoothDeviceApplicationState> BluetoothDeviceApplicationStateContainer;
/**
 * @brief Type definition for Bluetooth device state
 */
  typedef struct {
    bdaddr_t address;
    std::string name;
    BluetoothDeviceApplicationStateContainer applications;
  } BluetoothDeviceState;
/**
 * @brief Type definition for container of Bluetooth device states
 */
  typedef std::vector<BluetoothDeviceState> BluetoothDeviceStateContainer;
/**
 * @brief Returns the singleton of class
 */
  static LastState* instance();
/**
 * @brief Getter for container of TCP device states
 *
 * @return Constant reference to container of TCP device states
 */
  const TcpDeviceStateContainer& tcp_device_states() const;
/**
 * @brief Setter for container of TCP device states
 */
  void StoreTcpDeviceStates(const TcpDeviceStateContainer& tcp_device_states);
/**
 * @brief Getter for container of Bluetooth device states
 *
 * @return Constant reference to container of Bluetooth device states
 */
  const BluetoothDeviceStateContainer& bluetooth_device_states() const;
/**
 * @brief Setter for container of Bluetooth device states
 */
  void StoreBluetoothDeviceStates(
    const BluetoothDeviceStateContainer& bluetooth_device_states);

 private:
  TcpDeviceStateContainer tcp_device_states_;
  BluetoothDeviceStateContainer bluetooth_device_states_;
};

}  // namespace resumption

#endif  // SRC_COMPONENTS_RESUMPTION_INCLUDE_RESUMPTION_LAST_STATE_H_
