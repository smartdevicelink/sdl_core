#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "transport_manager/tcp_adapter.h"
#include "transport_manager/device_adapter_listener.h"
#include "transport_manager/device_handle_generator_impl.h"
#include "transport_manager/device_adapter_listener_impl.h"

TEST(DeviceAdapterTest, Basic) {
  using transport_manager::device_adapter::TcpDeviceAdapter;
  using transport_manager::device_adapter::DeviceAdapter;

  DeviceAdapter* device_adapter =
      static_cast<DeviceAdapter*>(new TcpDeviceAdapter);

  EXPECT_EQ("sdl-tcp", device_adapter->getDeviceType());
  EXPECT_TRUE(device_adapter->isClientOriginatedConnectSupported());
  EXPECT_FALSE(device_adapter->isServerOriginatedConnectSupported());
  EXPECT_FALSE(device_adapter->isSearchDevicesSupported());
}

TEST(DeviceAdapterTest, NotInitialised) {
  using transport_manager::device_adapter::TcpDeviceAdapter;
  using transport_manager::device_adapter::DeviceAdapter;

  DeviceAdapter* device_adapter =
      static_cast<DeviceAdapter*>(new TcpDeviceAdapter);

  EXPECT_EQ(DeviceAdapter::NOT_SUPPORTED, device_adapter->searchDevices());
  EXPECT_EQ(DeviceAdapter::NOT_SUPPORTED, device_adapter->connect(1, 2, 3));
  EXPECT_EQ(DeviceAdapter::BAD_STATE, device_adapter->acceptConnect(1, 2, 3));
  EXPECT_EQ(DeviceAdapter::BAD_STATE, device_adapter->declineConnect(1, 2));
  EXPECT_EQ(DeviceAdapter::BAD_STATE, device_adapter->disconnect(1));
  EXPECT_EQ(DeviceAdapter::BAD_STATE, device_adapter->disconnectDevice(1));
}

class ClientTcpSocket {
 public:
  bool connect(uint16_t server_port) {
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0)
      return false;

    struct sockaddr_in addr;
    memset((char*) &addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK );
    addr.sin_port = htons(server_port);

    if (::connect(socket_, (struct sockaddr*) &addr, sizeof(addr)) < 0)
      return false;
    else
      return true;
  }

 private:
  uint16_t port_;
  int socket_;
};

TEST(TcpAdapterTest, TtTtT) {
  using transport_manager::device_adapter::TcpDeviceAdapter;
  using transport_manager::device_adapter::DeviceAdapter;
  using transport_manager::DeviceHandleGeneratorImpl;

  DeviceAdapter* device_adapter =
      static_cast<DeviceAdapter*>(new TcpDeviceAdapter);
  const DeviceAdapter::Error error = device_adapter->init(
      new DeviceHandleGeneratorImpl, 0);
  ASSERT_EQ(DeviceAdapter::OK, error);

  while (!device_adapter->isInitialised())
    sleep(0);

  ClientTcpSocket client;
  EXPECT_TRUE(client.connect(TcpDeviceAdapter::default_port));
  delete device_adapter;
}
