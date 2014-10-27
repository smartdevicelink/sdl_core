#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <netinet/in.h>
#include <sys/types.h>
#include <ifaddrs.h>

#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/tcp/dnssd_service_browser.h"
#include "transport_manager/tcp/tcp_device.h"

namespace transport_manager {
namespace transport_adapter {

class MockTransportAdapterController : public TransportAdapterController {
 public:
  MOCK_METHOD1(AddDevice,
      DeviceSptr(DeviceSptr device));
  MOCK_METHOD1(SearchDeviceDone,
      void(const DeviceVector& devices));
  MOCK_METHOD1(SearchDeviceFailed,
      void(const SearchDeviceError& error));
  MOCK_CONST_METHOD1(FindDevice,
      DeviceSptr(const DeviceUID& device_handle));
  MOCK_METHOD3(ConnectionCreated,
      void(Connection* connection, const DeviceUID& device_handle, const ApplicationHandle& app_handle));
  MOCK_METHOD2(ConnectDone,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle));
  MOCK_METHOD3(ConnectFailed,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const ConnectError& error));
  MOCK_METHOD2(ConnectionFinished,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle));
  MOCK_METHOD3(ConnectionAborted,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const CommunicationError& error));
  MOCK_METHOD2(DisconnectDone,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle));
  MOCK_METHOD3(DataReceiveDone,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, RawMessagePtr message));
  MOCK_METHOD3(DataReceiveFailed,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const DataReceiveError& error));
  MOCK_METHOD3(DataSendDone,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, RawMessagePtr message));
  MOCK_METHOD4(DataSendFailed,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, RawMessagePtr message, const DataSendError& error));
};

in_addr_t GetIfaceAddress() {
  in_addr_t result = 0;
  ifaddrs* if_addrs = NULL;
  void * tmpAddrPtr = NULL;

  getifaddrs(&if_addrs);

  for (ifaddrs* ifa = if_addrs; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr->sa_family == AF_INET) {
      result = ((struct sockaddr_in *) ifa->ifa_addr)->sin_addr.s_addr;
      if (result != htonl(INADDR_LOOPBACK )) {
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

TEST(DnssdServiceBrowser, Basic) {
  MockTransportAdapterController controller;

  EXPECT_CALL(controller, SearchDeviceDone(HasService(4444)));

  DnssdServiceBrowser dnssd_service_browser(&controller);
  DeviceScanner& device_scanner = dnssd_service_browser;
  device_scanner.Init();
  while (!device_scanner.IsInitialised()) {
  }
  sleep(1);
  device_scanner.Scan();
}

}  // namespace
}  // namespace
