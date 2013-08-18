#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <netinet/in.h>
#include <sys/types.h>
#include <ifaddrs.h>

#include "transport_manager/device_adapter/device_adapter_controller.h"
#include "transport_manager/tcp/dnssd_service_browser.h"
#include "transport_manager/tcp/tcp_device.h"

namespace transport_manager {
namespace device_adapter {

class MockDeviceAdapterController : public DeviceAdapterController {
 public:
  MOCK_METHOD1(addDevice,
      DeviceSptr(DeviceSptr device));
  MOCK_METHOD1(searchDeviceDone,
      void(const DeviceVector& devices));
  MOCK_METHOD1(searchDeviceFailed,
      void(const SearchDeviceError& error));
  MOCK_CONST_METHOD1(findDevice,
      DeviceSptr(const DeviceUID& device_handle));
  MOCK_METHOD3(connectionCreated,
      void(ConnectionSptr connection, const DeviceUID& device_handle, const ApplicationHandle& app_handle));
  MOCK_METHOD2(connectDone,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle));
  MOCK_METHOD3(connectFailed,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const ConnectError& error));
  MOCK_METHOD2(connectionFinished,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle));
  MOCK_METHOD3(connectionAborted,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const CommunicationError& error));
  MOCK_METHOD2(disconnectDone,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle));
  MOCK_METHOD3(dataReceiveDone,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, RawMessageSptr message));
  MOCK_METHOD3(dataReceiveFailed,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const DataReceiveError& error));
  MOCK_METHOD3(dataSendDone,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, RawMessageSptr message));
  MOCK_METHOD4(dataSendFailed,
      void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, RawMessageSptr message, const DataSendError& error));
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
  if(tcp_device && tcp_device->getAddress() == iface_address) {
    ApplicationList app_list = tcp_device->getApplicationList();
    for(ApplicationList::const_iterator it = app_list.begin(); it != app_list.end(); ++it) {
      if(tcp_device->getApplicationPort(*it) == service_port) {
        return true;
      }
    }
  }
}
return false;
}

TEST(DnssdServiceBrowser, Basic) {
  MockDeviceAdapterController controller;

  EXPECT_CALL(controller, searchDeviceDone(HasService(4444)));

  DnssdServiceBrowser dnssd_service_browser(&controller);
  DeviceScanner& device_scanner = dnssd_service_browser;
  device_scanner.init();
  while (!device_scanner.isInitialised()) {
  }
  sleep(1);
  device_scanner.scan();
}

}  // namespace
}  // namespace
