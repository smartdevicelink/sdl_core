#include <arpa/inet.h>
#include <net/if.h>
#include <time.h>

#include "gtest/gtest.h"
#include "transport_manager/tcp/network_interface_listener_impl.h"
#include "transport_manager/tcp/mock_tcp_client_listener.h"
#include "utils/test_async_waiter.h"
#include "utils/threads/thread.h"

namespace test {
namespace components {
namespace transport_manager_test {

namespace {
const long kThreadStartWaitMsec = 10;
const uint32_t kStartNotificationTimeoutMsec = 500;
}

using ::testing::_;
using ::testing::AtLeast;
using ::testing::SaveArg;

class NetworkInterfaceListenerTest : public ::testing::Test {
 public:
  NetworkInterfaceListenerTest()
      : interface_listener_impl_(NULL)
      , mock_tcp_client_listener_(NULL, 0, false, "") {}

  virtual ~NetworkInterfaceListenerTest() {}

 protected:
  struct InterfaceEntry {
    const char* name;
    const char* ipv4_address;
    const char* ipv6_address;
    unsigned int flags;
  };

  void Deinit() {
    delete interface_listener_impl_;
  }

  void SleepFor(long msec) const {
    if (msec > 0) {
      struct timespec ts = {0, msec * 1000 * 1000};
      nanosleep(&ts, NULL);
    }
  }

  NetworkInterfaceListenerImpl* interface_listener_impl_;
  MockTcpClientListener mock_tcp_client_listener_;
};

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test
