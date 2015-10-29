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

#include <gtest/gtest.h>
#include "encryption/hashing.h"
#include "connection_handler/device.h"

namespace test {
namespace components {
namespace connection_handle {

using namespace connection_handler;
TEST(ConnectionDevice, CompareDevicesWithDifferentMacAddresses) {
  DeviceHandle device_handle = 0;

  std::string connection_type = "BTMAC";
  std::string device_name = "test_name";
  std::string mac_address = "test_address";

  Device test_device(device_handle, device_name, mac_address, connection_type);

  EXPECT_EQ(device_handle, test_device.device_handle());
  EXPECT_EQ(device_name, test_device.user_friendly_name());
  EXPECT_NE(mac_address, test_device.mac_address());
  EXPECT_EQ(connection_type, test_device.connection_type());
  std::string hash_mac_address = test_device.mac_address();

  std::string test_next_mac_address = "test_address_";
  Device next_test_device(device_handle, device_name, test_next_mac_address, connection_type);
  EXPECT_NE(test_next_mac_address, next_test_device.mac_address());
  std::string hash_next_mac_address = next_test_device.mac_address();

  EXPECT_NE(hash_mac_address, hash_next_mac_address);
}

TEST(ConnectionDevice, MacAddressHash) {
  DeviceHandle device_handle = 0;
  std::string connection_type = "BTMAC";
  std::string device_name = "test_name";
  std::string mac_address = "test_address";

  Device test_device(device_handle, device_name, mac_address, connection_type);

  std::string hashed_mac_address = encryption::MakeHash(mac_address);
  EXPECT_EQ(hashed_mac_address, test_device.mac_address());
}

}  // namespace connection_handle
}  // namespace components
}  // namespace test

