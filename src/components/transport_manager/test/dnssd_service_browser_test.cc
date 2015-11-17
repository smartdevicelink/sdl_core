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

#include "gmock/gmock.h"

#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/tcp/dnssd_service_browser.h"

namespace transport_manager {
namespace transport_adapter {

class MockTransportAdapterController : public TransportAdapterController {
 public:
  MOCK_METHOD1(AddDevice, DeviceSptr(DeviceSptr device));
  MOCK_METHOD1(SearchDeviceDone, void(const DeviceVector& devices));
  MOCK_METHOD1(SearchDeviceFailed, void(const SearchDeviceError& error));
  MOCK_CONST_METHOD1(FindDevice, DeviceSptr(const DeviceUID& device_handle));
  MOCK_METHOD3(ConnectionCreated,
               void(ConnectionSPtr connection, const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle));
  MOCK_METHOD2(ConnectDone, void(const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle));
  MOCK_METHOD3(ConnectFailed, void(const DeviceUID& device_handle,
                                   const ApplicationHandle& app_handle,
                                   const ConnectError& error));
  MOCK_METHOD2(ConnectionFinished, void(const DeviceUID& device_handle,
                                        const ApplicationHandle& app_handle));
  MOCK_METHOD3(ConnectionAborted, void(const DeviceUID& device_handle,
                                       const ApplicationHandle& app_handle,
                                       const CommunicationError& error));
  MOCK_METHOD2(DisconnectDone, void(const DeviceUID& device_handle,
                                    const ApplicationHandle& app_handle));
  MOCK_METHOD3(DataReceiveDone,
               void(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const ::protocol_handler::RawMessagePtr message));
  MOCK_METHOD3(DataReceiveFailed, void(const DeviceUID& device_handle,
                                       const ApplicationHandle& app_handle,
                                       const DataReceiveError& error));
  MOCK_METHOD3(DataSendDone,
               void(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const ::protocol_handler::RawMessagePtr message));
  MOCK_METHOD4(DataSendFailed,
               void(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const ::protocol_handler::RawMessagePtr message,
                    const DataSendError& error));
  MOCK_METHOD0(FindNewApplicationsRequest, void());
  MOCK_METHOD0(AckDevices, void());
  MOCK_METHOD1(ApplicationListUpdated, void(const DeviceUID& device_handle));
  MOCK_METHOD2(DeviceDisconnected, void(const DeviceUID& device_handle,
                                        const DisconnectDeviceError& error));
};

TEST(DnssdServiceBrowser, DISABLED_Init) {
  // Arrange
  MockTransportAdapterController controller;
  DnssdServiceBrowser dnssd_service_browser(&controller);
  // Check values after creation. Nothing is initialized
  EXPECT_TRUE(NULL == dnssd_service_browser.avahi_service_browser());
  EXPECT_TRUE(NULL == dnssd_service_browser.avahi_threaded_poll());
  EXPECT_TRUE(NULL == dnssd_service_browser.avahi_client());
  // Act
  const TransportAdapter::Error error = dnssd_service_browser.Init();
  ASSERT_EQ(TransportAdapter::OK, error);

  while (!dnssd_service_browser.IsInitialised()) {
    sleep(0);
  }
  ASSERT_TRUE(dnssd_service_browser.IsInitialised());
  // Check values are initialized and threaded poll started
  EXPECT_FALSE(NULL == dnssd_service_browser.avahi_service_browser());
  EXPECT_FALSE(NULL == dnssd_service_browser.avahi_threaded_poll());
  EXPECT_FALSE(NULL == dnssd_service_browser.avahi_client());
}

TEST(DnssdServiceBrowser, DISABLED_IsInitialized_ExpectFalse) {
  // Arrange
  MockTransportAdapterController controller;
  DnssdServiceBrowser dnssd_service_browser(&controller);
  // Check
  EXPECT_FALSE(dnssd_service_browser.IsInitialised());
}

TEST(DnssdServiceBrowser, DISABLED_Terminate_ExpectTerminated) {
  // Arrange
  MockTransportAdapterController controller;
  DnssdServiceBrowser dnssd_service_browser(&controller);
  // Init service browser and client
  const TransportAdapter::Error error = dnssd_service_browser.Init();
  ASSERT_EQ(TransportAdapter::OK, error);

  while (!dnssd_service_browser.IsInitialised()) {
    sleep(0);
  }
  ASSERT_TRUE(dnssd_service_browser.IsInitialised());
  // Client & browser are initialized and successfully started
  EXPECT_FALSE(NULL == dnssd_service_browser.avahi_service_browser());
  EXPECT_FALSE(NULL == dnssd_service_browser.avahi_threaded_poll());
  EXPECT_FALSE(NULL == dnssd_service_browser.avahi_client());
  dnssd_service_browser.Terminate();
  // Checks everything successfully terminated
  EXPECT_TRUE(NULL == dnssd_service_browser.avahi_service_browser());
  EXPECT_TRUE(NULL == dnssd_service_browser.avahi_threaded_poll());
  EXPECT_TRUE(NULL == dnssd_service_browser.avahi_client());
}

TEST(DnssdServiceBrowser, DISABLED_Scan_ExpectNotSupported) {
  // Arrange
  MockTransportAdapterController controller;
  DnssdServiceBrowser dnssd_service_browser(&controller);
  // At this moment Scan() can only return NOT SUPPORTED value
  EXPECT_EQ(TransportAdapter::NOT_SUPPORTED, dnssd_service_browser.Scan());
}

}  // namespace transport_adapter
}  // namespace transport_manager
