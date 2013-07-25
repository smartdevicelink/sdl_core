/*
 * \file MockDeviceAdapter.h
 * \brief MockDeviceAdapter
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

#ifndef MOCKDEVICEADAPTER_H_
#define MOCKDEVICEADAPTER_H_

#include <map>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <transport_manager/transport_manager.h>
#include <transport_manager/device_adapter_impl.h>
#include <transport_manager/transport_manager_impl.h>
#include <transport_manager/mock_device_scanner.h>
#include <transport_manager/mock_connection_factory.h>
#include <transport_manager/mock_device_scanner.h>

using namespace transport_manager;
using ::transport_manager::device_adapter::DeviceAdapterController;
using ::transport_manager::device_adapter::DeviceVector;

namespace test  {
namespace components  {
namespace transport_manager {

class MockDeviceAdapter : public ::transport_manager::device_adapter::DeviceAdapterImpl {
 public:
  MockDeviceAdapter()
   : DeviceAdapterImpl(new MockDeviceScanner(this), new MockConnectionFactory(this), nullptr) { }

  MockDeviceScanner *device_scanner() const {
    return static_cast<MockDeviceScanner*>(device_scanner_);
  }

 protected:

   virtual ::transport_manager::device_adapter::DeviceType getDeviceType() const;

   virtual bool isSearchDevicesSupported() const;

   virtual bool isServerOriginatedConnectSupported() const;

   virtual bool isClientOriginatedConnectSupported() const;
};

} // namespace transport_manager
} // namespace components
} // namespace test


#endif /* MOCKDEVICEADAPTER_H_ */
