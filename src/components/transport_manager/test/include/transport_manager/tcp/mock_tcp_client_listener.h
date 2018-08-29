#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_TCP_MOCK_TCP_CLIENT_LISTENER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_TCP_MOCK_TCP_CLIENT_LISTENER_H_

#include "gmock/gmock.h"
#include "transport_manager/tcp/tcp_client_listener.h"

namespace test {
namespace components {
namespace transport_manager_test {

using namespace ::transport_manager::transport_adapter;

class MockTcpClientListener : public TcpClientListener {
 public:
  MockTcpClientListener(TransportAdapterController* controller,
                        uint16_t port,
                        bool enable_keepalive,
                        const std::string designated_interface = "")
      : TcpClientListener(
            controller, port, enable_keepalive, designated_interface) {}
  MOCK_METHOD0(Init, TransportAdapter::Error());
  MOCK_METHOD0(Terminate, void());
  MOCK_CONST_METHOD0(IsInitialised, bool());
  MOCK_METHOD0(StartListening, TransportAdapter::Error());
  MOCK_METHOD0(StopListening, TransportAdapter::Error());
  MOCK_METHOD2(OnIPAddressUpdated,
               void(const std::string ipv4_addr, const std::string ipv6_addr));
};

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_TCP_MOCK_TCP_CLIENT_LISTENER_H_
