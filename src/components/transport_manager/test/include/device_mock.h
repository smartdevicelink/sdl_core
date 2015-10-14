/*
 * Copyright (c) 2015, Ford Motor Company
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_DEVICE_MOCK_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_DEVICE_MOCK_H_

#include "gmock/gmock.h"
#include "transport_manager/transport_adapter/device.h"
#include "transport_manager/common.h"
#include "transport_manager/tcp/tcp_device.h"

namespace test {
namespace components {
namespace transport_manager_test {

class DeviceMock : public ::transport_manager::transport_adapter::Device {
 public:
  DeviceMock(const std::string& name, const std::string& unique_device_id)
      : Device(name, unique_device_id) {}
  MOCK_CONST_METHOD1(IsSameAs, bool(const Device* other_device));
  MOCK_CONST_METHOD0(GetApplicationList, std::vector<int>());
  MOCK_METHOD0(Stop, void());
};

class TCPDeviceMock : public ::transport_manager::transport_adapter::TcpDevice {
 public:
  TCPDeviceMock(const uint32_t& in_addr_t, const std::string& name)
      : TcpDevice(in_addr_t, name) {}
  MOCK_CONST_METHOD1(IsSameAs, bool(const Device* other_device));
  MOCK_CONST_METHOD0(GetApplicationList, std::vector<int>());
  MOCK_METHOD0(Stop, void());
  MOCK_CONST_METHOD1(
      GetApplicationPort,
      int(const ::transport_manager::ApplicationHandle app_handle));
};

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_DEVICE_MOCK_H_
