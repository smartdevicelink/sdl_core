/*
 * \file mock_device_adapter_listener.h
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

#ifndef MOCK_DEVICE_ADAPTER_LISTENER_H_
#define MOCK_DEVICE_ADAPTER_LISTENER_H_

#include <gmock/gmock.h>

#include "transport_manager/common.h"
#include "transport_manager/device_adapter_listener.h"

using namespace transport_manager;
using transport_manager::device_adapter::DeviceAdapter;
using transport_manager::device_adapter::DeviceAdapterListener;

namespace test {
namespace components {
namespace transport_manager {

struct MockDeviceAdapterListener : public DeviceAdapterListener {
  MOCK_METHOD1(onSearchDeviceDone, void (const DeviceAdapter* device_adapter));
  MOCK_METHOD2(onSearchDeviceFailed, void (const DeviceAdapter* device_adapter,
          const SearchDeviceError& error));
  MOCK_METHOD3(onConnectDone, void (const DeviceAdapter* device_adapter,
          const DeviceHandle& device, const ApplicationHandle& application));
  MOCK_METHOD4(onConnectFailed, void (const DeviceAdapter* device_adapter,
          const DeviceHandle& device, const ApplicationHandle& application,
          const ConnectError& error));
  MOCK_METHOD3(onDisconnectDone, void (const DeviceAdapter* device_adapter,
          const DeviceHandle& device, const ApplicationHandle& application));
  MOCK_METHOD4(onDisconnectFailed, void (const DeviceAdapter* device_adapter,
          const DeviceHandle& device, const ApplicationHandle& application, const DisconnectError& error));
  MOCK_METHOD3(onDisconnectDeviceDone, void (const DeviceAdapter* device_adapter,
          const DeviceHandle& device, const ApplicationHandle& application));
  MOCK_METHOD4(onDisconnectDeviceFailed, void (const DeviceAdapter* device_adapter,
          const DeviceHandle& device, const ApplicationHandle& application, const DisconnectDeviceError& error));
  MOCK_METHOD4(onDataSendDone, void (const DeviceAdapter* device_adapter,
          const DeviceHandle& device, const ApplicationHandle& application,
          const RawMessageSptr data_container));
  MOCK_METHOD5(onDataSendFailed, void (const DeviceAdapter* device_adapter,
          const DeviceHandle& device, const ApplicationHandle& application,
          const RawMessageSptr data_container,
          const DataSendError& error));
  MOCK_METHOD4(onDataReceiveDone, void (const DeviceAdapter* device_adapter,
          const DeviceHandle& device, const ApplicationHandle& application,
          const RawMessageSptr data_container));
  MOCK_METHOD4(onDataReceiveFailed, void (const DeviceAdapter* device_adapter,
          const DeviceHandle& device, const ApplicationHandle& application,
          const DataReceiveError& error));
  MOCK_METHOD3(onCommunicationError, void (const DeviceAdapter* device_adapter,
          const DeviceHandle& device, const ApplicationHandle& application));
  ~MockDeviceAdapterListener() {}

};

}
}
}

#endif /* MOCK_DEVICE_ADAPTER_LISTENER_H_ */
