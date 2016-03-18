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

#ifndef TEST_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_MOCK_H_
#define TEST_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_MOCK_H_

#include <gmock/gmock.h>
#include <string>
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_adapter/transport_adapter_event.h"
#include "telemetry_monitor/telemetry_observable.h"

namespace test {
namespace components {
namespace transport_manager_test {

using ::transport_manager::DeviceHandle;
using ::transport_manager::ConnectionUID;
using ::transport_manager::transport_adapter::TransportAdapter;
using ::transport_manager::TransportAdapterEvent;
using ::transport_manager::TransportManagerListener;
/*
 * MOCK implementation of ::transport_manager::TransportManager interface
 */
class MockTransportManager
    : public ::transport_manager::TransportManager,
      public ::telemetry_monitor::TelemetryObservable<
          transport_manager::TMTelemetryObserver>{

 public:
  MOCK_METHOD0(Init, int());
  MOCK_METHOD0(Reinit, int());
  MOCK_METHOD0(SearchDevices, int());
  MOCK_METHOD1(ConnectDevice, int(const DeviceHandle));
  MOCK_METHOD1(DisconnectDevice, int(const DeviceHandle));
  MOCK_METHOD1(Disconnect, int(const ConnectionUID));
  MOCK_METHOD1(DisconnectForce, int(const ConnectionUID));
 MOCK_METHOD1(SendMessageToDevice,
              int(const ::protocol_handler::RawMessagePtr));
  MOCK_METHOD1(ReceiveEventFromDevice, int(const TransportAdapterEvent&));
  MOCK_METHOD1(AddTransportAdapter, int(TransportAdapter* adapter));
  MOCK_METHOD1(AddEventListener, int(TransportManagerListener* listener));
  MOCK_METHOD0(Stop, int());
  MOCK_METHOD1(RemoveDevice, int(const DeviceHandle));
  MOCK_CONST_METHOD1(Visibility, int(const bool&));
  MOCK_METHOD1(SetTelemetryObserver,
               void(transport_manager::TMTelemetryObserver* observer));

};

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test
#endif  // TEST_COMPONENTS_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_MANAGER_MOCK_H_
