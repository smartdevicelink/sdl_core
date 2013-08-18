/*
 * \file info.h
 * \brief 
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_INFO_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_INFO_H_

#include <string>
#include "transport_manager/common.h"

namespace transport_manager {

class Info {
 protected:
  std::string name_;

 public:
  Info() {}
  explicit Info(std::string name)
      : name_(name) {
  }
  std::string name() const{
    return name_;
  }
  virtual ~Info() {}
};

class DeviceInfo : public Info {
 protected:
  std::string mac_address_;
  DeviceHandle device_handle_;

 public:
  DeviceInfo(DeviceHandle device_handle, std::string mac_address,
             std::string name)
      : Info(name),
        mac_address_(mac_address),
        device_handle_(device_handle) {}

  std::string mac_address() const {
    return mac_address_;
  }
  DeviceHandle device_handle() const {
    return device_handle_;
  }
  friend bool operator ==(const DeviceInfo &first, const DeviceInfo &second);
};

inline bool operator ==(const DeviceInfo &first, const DeviceInfo &second) {
  return first.name_ == second.name_
      && first.mac_address_ == second.mac_address_
      && first.device_handle_ == second.device_handle_;
}

class AdapterInfo : public Info {

};
}

#endif /* SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_INFO_H_ */
