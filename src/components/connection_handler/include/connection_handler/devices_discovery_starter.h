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

#ifndef SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_DEVICES_DISCOVERY_STARTER_H_
#define SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_DEVICES_DISCOVERY_STARTER_H_

#include "connection_handler/device.h"

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink connection_handler namespace.
 */
namespace connection_handler {
/**
 * \class DevicesDiscoveryStarter
 * \brief Starter of devices discovering process class
 */
class DevicesDiscoveryStarter {
 public:
  /**
   * \brief Method which should start devices discoveryng
   */
  virtual void StartDevicesDiscovery()=0;

  /**
   * \brief Connects to all services of device
   * \param deviceHandle Handle of device to connect to
   */
  virtual void ConnectToDevice(
      connection_handler::DeviceHandle device_handle) = 0;

  virtual void StartTransportManager() = 0;

 protected:
  /**
   * \brief Destructor
   */
  virtual ~DevicesDiscoveryStarter() {
  }
};
}  // namespace connection_handler

#endif  // SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_DEVICES_DISCOVERY_STARTER_H_
