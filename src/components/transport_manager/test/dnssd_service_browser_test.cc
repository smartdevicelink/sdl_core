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

#include <netinet/in.h>
#include <sys/types.h>
#include <ifaddrs.h>

#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/tcp/dnssd_service_browser.h"
#include "transport_manager/tcp/tcp_device.h"

namespace transport_manager {
namespace transport_adapter {

class MockTransportAdapterController: public TransportAdapterController {
public:
 MOCK_METHOD1(AddDevice,DeviceSptr(DeviceSptr device));
 MOCK_METHOD1(SearchDeviceDone, void(const DeviceVector& devices));
 MOCK_METHOD1(SearchDeviceFailed, void(const SearchDeviceError& error));
 MOCK_CONST_METHOD1(FindDevice, DeviceSptr(const DeviceUID& device_handle));
 MOCK_METHOD3(ConnectionCreated, void(ConnectionSPtr connection, const DeviceUID& device_handle, const ApplicationHandle& app_handle));
 MOCK_METHOD2(ConnectDone, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle));
 MOCK_METHOD3(ConnectFailed, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const ConnectError& error));
 MOCK_METHOD2(ConnectionFinished, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle));
 MOCK_METHOD3(ConnectionAborted,void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const CommunicationError& error));
 MOCK_METHOD2(DisconnectDone, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle));
 MOCK_METHOD3(DataReceiveDone, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const ::protocol_handler::RawMessagePtr message));
 MOCK_METHOD3(DataReceiveFailed, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const DataReceiveError& error));
 MOCK_METHOD3(DataSendDone, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const ::protocol_handler::RawMessagePtr message));
 MOCK_METHOD4(DataSendFailed, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const ::protocol_handler::RawMessagePtr message, const DataSendError& error));
 MOCK_METHOD0(FindNewApplicationsRequest, void());
 MOCK_METHOD1(ApplicationListUpdated, void(const DeviceUID& device_handle));
 MOCK_METHOD2(DeviceDisconnected, void (const DeviceUID& device_handle,const DisconnectDeviceError& error));
};

in_addr_t GetIfaceAddress() {
  in_addr_t result = 0;
  ifaddrs* if_addrs = NULL;
//  void * tmpAddrPtr = NULL;

  getifaddrs(&if_addrs);
  for (ifaddrs* ifa = if_addrs; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr->sa_family == AF_INET) {
      result = ((struct sockaddr_in *) ifa->ifa_addr)->sin_addr.s_addr;
      if (result != htonl(INADDR_LOOPBACK)) {
        break;
      }
    }
  }
  if (if_addrs)
    freeifaddrs(if_addrs);
  return result;
}
static in_addr_t iface_address = GetIfaceAddress();

MATCHER_P(HasService, service_port, ""){
for(DeviceVector::const_iterator it = arg.begin(); it != arg.end(); ++it) {
  TcpDevice* tcp_device = dynamic_cast<TcpDevice*>(it->get());
  if(tcp_device && tcp_device->in_addr() == iface_address) {
    ApplicationList app_list = tcp_device->GetApplicationList();
    for(ApplicationList::const_iterator it = app_list.begin(); it != app_list.end(); ++it) {
      if(tcp_device->GetApplicationPort(*it) == service_port) {
        return true;
      }
    }
  }
}
return false;
}

// TODO{ALeshin} APPLINK-11090 - Infinite loop
TEST(DnssdServiceBrowser, DISABLED_Basic) {
  MockTransportAdapterController controller;

  DnssdServiceBrowser dnssd_service_browser(&controller);
  DeviceScanner& device_scanner = dnssd_service_browser;

  const TransportAdapter::Error error = device_scanner.Init();
  ASSERT_EQ(TransportAdapter::OK, error);

  while (!device_scanner.IsInitialised()) {
    sleep(0);
  }
  ASSERT_TRUE(device_scanner.IsInitialised());

  EXPECT_EQ(TransportAdapter::NOT_SUPPORTED, device_scanner.Scan());  //method Scan now returns only NOT_SUPPORTED value

}

}  // namespace transport_adapter
}  // namespace transport_manager
