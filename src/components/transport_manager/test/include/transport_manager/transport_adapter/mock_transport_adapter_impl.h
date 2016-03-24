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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_MOCK_TRANSPORT_ADAPTER_IMPL_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_MOCK_TRANSPORT_ADAPTER_IMPL_H_

#include "gmock/gmock.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

using ::transport_manager::transport_adapter::TransportAdapterImpl;
using ::transport_manager::transport_adapter::DeviceScanner;
using ::transport_manager::transport_adapter::ServerConnectionFactory;
using ::transport_manager::transport_adapter::ClientConnectionListener;
using ::transport_manager::transport_adapter::DeviceType;
using ::transport_manager::transport_adapter::ConnectionSPtr;
using ::transport_manager::DeviceUID;
using ::transport_manager::ApplicationHandle;

namespace test {
namespace components {
namespace transport_manager_test {

class MockTransportAdapterImpl : public TransportAdapterImpl {
 public:
  MockTransportAdapterImpl(DeviceScanner* device_scanner,
                       ServerConnectionFactory* server_connection_factory,
                       ClientConnectionListener* client_connection_listener,
                       resumption::LastState& last_state)
      : TransportAdapterImpl(device_scanner, server_connection_factory,
                             client_connection_listener,last_state) {
  }

  ConnectionSPtr FindStatedConnection(const DeviceUID& device_handle,
                                        const ApplicationHandle& app_handle) {
    return this->FindEstablishedConnection(device_handle, app_handle);
  }
  virtual ~MockTransportAdapterImpl(){};

  virtual DeviceType GetDeviceType() const { return UNKNOWN; }

  MOCK_CONST_METHOD0(Store, void());
  MOCK_METHOD0(Restore, bool());
};

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_MOCK_TRANSPORT_ADAPTER_IMPL_H_
