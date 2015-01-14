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

#ifndef SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_DEVICE_H_
#define SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_DEVICE_H_

#include <string>
#include <map>
#include <vector>

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink connection_handler namespace.
 */
namespace connection_handler {

/**
 * \brief Type for DeviceHandle
 */
typedef uint32_t DeviceHandle;
typedef std::vector<int32_t> AppList;

/**
 * \class Device
 * \brief Connection class
 */
class Device {
 public:
  /**
   * \brief Class constructor
   */
  Device(DeviceHandle device_handle, const std::string &user_friendly_name,
         const std::string &mac_address = "", const std::string& connection_type = "");

  /**
   * \brief Returns device handle
   * \return DeviceHandle
   */
  DeviceHandle device_handle() const;

  /**
   * \brief Returns user frendly device name
   * \return UserFriendlyName
   */
  std::string user_friendly_name() const;

  /**
      *\brief Also should be used for iOS USB connections
      *(better not know why this same method)
      */
  std::string mac_address() const;

  /**
   * @brief The type of connection used by device.
   * @return connection type (USB_serial, BTMAC, etc.)
   */
  std::string connection_type() const;

 private:
  /**
   * \brief Uniq device handle.
   */
  DeviceHandle device_handle_;

  /**
   * \brief User-friendly device name.
   */
  std::string user_friendly_name_;

  /**
   * \brief Mac address of device if available
   */
  std::string mac_address_;

  /**
   * \brief The type of connection used by device.
   */
  std::string connection_type_;
};

/**
 * \brief Type for Devices map
 */
typedef std::map<int32_t, Device> DeviceMap;
}  // namespace connection_handler
#endif  // SRC_COMPONENTS_CONNECTION_HANDLER_INCLUDE_CONNECTION_HANDLER_DEVICE_H_
