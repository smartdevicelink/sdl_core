/*
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

#ifndef SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_DEVICE_H_
#define SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_DEVICE_H_

#include <string>
#include <vector>

#include "transport_manager/common.h"
#include "utils/shared_ptr.h"

namespace transport_manager {
namespace transport_adapter {

/**
 * @brief Internal class describing device.
 **/
class Device {
 public:
  /**
   * @brief Constructor.
   *
   * @param name User-friendly device name.
   * @param unique_device_id device unique identifier.
   **/
  Device(const std::string& name, const DeviceUID& unique_device_id)
    : name_(name),
      unique_device_id_(unique_device_id),
      keep_on_disconnect_(false) {}
  /**
   * @brief Destructor.
   **/
  virtual ~Device() {}

  /**
   * @brief Compare devices.
   *
   * This method checks whether two SDevice structures
   * refer to the same device.
   *
   * @param other_Ddvice Device to compare with.
   *
   * @return true if devices are equal, false otherwise.
   **/
  virtual bool IsSameAs(const Device* other_device) const = 0;

  virtual ApplicationList GetApplicationList() const = 0;

  virtual void Stop() { }

  inline const DeviceUID& unique_device_id() const {
    return unique_device_id_;
  }

  /**
   * @brief Return name of device.
   */
  inline const std::string& name() const {
    return name_;
  }

  /**
   * @brief Get @link keep_on_disconnect_ @endlink value
   */
  inline bool keep_on_disconnect() const {
    return keep_on_disconnect_;
  }

  /**
   * @brief Set @link keep_on_disconnect_ @endlink value
   * @param keep_on_disconnect new value
   */
  inline void set_keep_on_disconnect(bool keep_on_disconnect) {
    keep_on_disconnect_ = keep_on_disconnect;
  }

 private:
  /**
   * @brief Device user-friendly name.
   **/
  std::string name_;

  /**
   * @brief Unique device identifier across all devices.
   **/
  DeviceUID unique_device_id_;

  /**
   * @brief If true, device will remain in list even if all its connections finished.
   **/
  bool keep_on_disconnect_;
};
typedef utils::SharedPtr<Device> DeviceSptr;
typedef std::vector<DeviceSptr>  DeviceVector;
}  // namespace transport_adapter
}  // namespace transport_manager
#endif  // SRC_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_DEVICE_H_
