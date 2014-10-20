/*
 * \file info.h
 * \brief Information classes header file.
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

#ifndef SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_INFO_H_
#define SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_INFO_H_

#include <string>
#include "transport_manager/common.h"

/**
 * @brief transport_manager name space
 */
namespace transport_manager {

/**
 * @brief Base information class.
 */
class Info {
 protected:
  /**
   * @brief Variable that hold name.
   */
  std::string name_;

 public:
  /**
   * @brief Constructor.
   */
  Info() {}

  /**
   * @brief Constructor
   *
   * @param name Info class object name.
   */
  explicit Info(std::string name)
    : name_(name) {
  }

  /**
   * @brief Return string with name.
   */
  std::string name() const {
    return name_;
  }

  /**
   * @brief Destructor.
   */
  virtual ~Info() {}
};

/**
 * @brief Hold information about device.
 */
class DeviceInfo : public Info {
 protected:
  /**
   * @brief Variable that hold MAC address of device.
   */
  std::string mac_address_;

  /**
   * @brief Variable that hold handle of device.
   */
  DeviceHandle device_handle_;

  /**
   * @brief The connection type used by device.
   */
  ConnectionType connection_type_;

 public:
  /**
   * @brief Constructor.
   *
   * @param device_handle Handle of device.
   * @param mac_address MAC address of device.
   * @param name Name of device.
   */
  DeviceInfo(DeviceHandle device_handle, std::string mac_address,
             std::string name, const ConnectionType& connection_type)
    : Info(name),
      mac_address_(mac_address),
      device_handle_(device_handle),
      connection_type_(connection_type) {
  }

  /**
   * @brief Return mac_address.
   */
  std::string mac_address() const {
    return mac_address_;
  }

  /**
   * @brief Return device_handle field.
   */
  DeviceHandle device_handle() const {
    return device_handle_;
  }

  /**
   * @brief Return connection_type_.
   * @return
   */
  ConnectionType connection_type() const {
    return connection_type_;
  }

  /**
   * @brief Overloaded operator "==".
   */
  friend bool operator ==(const DeviceInfo& first, const DeviceInfo& second);

  // Needed for std::set container
  bool operator <(const DeviceInfo& than) const {
    return device_handle_ < than.device_handle_;
  }
};

/**
 * @brief Assign fields of one DeviceInfo class to another.
 */
inline bool operator ==(const DeviceInfo& first, const DeviceInfo& second) {
  return first.name_ == second.name_
         && first.mac_address_ == second.mac_address_
         && first.device_handle_ == second.device_handle_;
}
}  // namespace transport_manager
#endif  // SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_INFO_H_
