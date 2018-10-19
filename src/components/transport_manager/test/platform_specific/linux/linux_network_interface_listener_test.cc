#include <arpa/inet.h>
#include <net/if.h>
#include <time.h>

#include "gtest/gtest.h"
#include "platform_specific_network_interface_listener_impl.h"
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

  void Init(const std::string interface_name) {
    interface_listener_impl_ = new PlatformSpecificNetworkInterfaceListener(
        &mock_tcp_client_listener_, interface_name);
    // disable events from actual network interfaces
    interface_listener_impl_->SetTesting(true);
  }

  void Deinit() {
    delete interface_listener_impl_;
  }

  void SetDummyInterfaceTable(struct InterfaceEntry* entries) {
    InterfaceStatusTable dummy_table;

    while (entries->name != NULL) {
      InterfaceStatus status;
      if (entries->ipv4_address != NULL) {
        struct in_addr addr;
        ASSERT_EQ(1, inet_pton(AF_INET, entries->ipv4_address, &addr));
        status.SetIPv4Address(&addr);
      }
      if (entries->ipv6_address != NULL) {
        struct in6_addr addr6;
        ASSERT_EQ(1, inet_pton(AF_INET6, entries->ipv6_address, &addr6));
        status.SetIPv6Address(&addr6);
      }
      status.SetFlags(entries->flags);

      dummy_table.insert(std::make_pair(entries->name, status));
      entries++;
    }

    interface_listener_impl_->OverwriteStatusTable(dummy_table);
  }

  void SleepFor(long msec) const {
    if (msec > 0) {
      struct timespec ts = {0, msec * 1000 * 1000};
      nanosleep(&ts, NULL);
    }
  }

  PlatformSpecificNetworkInterfaceListener* interface_listener_impl_;
  MockTcpClientListener mock_tcp_client_listener_;
};

TEST_F(NetworkInterfaceListenerTest, Init) {
  Init("");

  EXPECT_TRUE(interface_listener_impl_->Init());
  EXPECT_TRUE(0 <= interface_listener_impl_->GetSocket());

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, Deinit) {
  Init("");
  EXPECT_TRUE(interface_listener_impl_->Init());

  interface_listener_impl_->Deinit();

  EXPECT_EQ(-1, interface_listener_impl_->GetSocket());

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, Start_success) {
  Init("");
  EXPECT_TRUE(interface_listener_impl_->Init());

  struct InterfaceEntry entries[] = {
      {"dummy_int0", "1.2.3.4", NULL, IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};
  SetDummyInterfaceTable(entries);

  // after stated, it is expected that the listener notifies current IP address
  // (if it's available)
  TestAsyncWaiter waiter;
  EXPECT_CALL(mock_tcp_client_listener_,
              OnIPAddressUpdated(entries[0].ipv4_address, ""))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));

  EXPECT_TRUE(interface_listener_impl_->Start());

  // the "isThreadRunning_" flag of the thread will be update slightly later
  SleepFor(kThreadStartWaitMsec);

  EXPECT_TRUE(interface_listener_impl_->GetThread()->is_running());

  EXPECT_TRUE(waiter.WaitFor(1, kStartNotificationTimeoutMsec));

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, Start_twice) {
  Init("");
  EXPECT_TRUE(interface_listener_impl_->Init());

  // ignore OnIPAddressUpdated call
  EXPECT_CALL(mock_tcp_client_listener_, OnIPAddressUpdated(_, _))
      .Times(AtLeast(0));

  EXPECT_TRUE(interface_listener_impl_->Start());
  SleepFor(kThreadStartWaitMsec);

  EXPECT_FALSE(interface_listener_impl_->Start());

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, Stop_success) {
  Init("");
  EXPECT_TRUE(interface_listener_impl_->Init());

  // ignore OnIPAddressUpdated call
  EXPECT_CALL(mock_tcp_client_listener_, OnIPAddressUpdated(_, _))
      .Times(AtLeast(0));

  EXPECT_TRUE(interface_listener_impl_->Start());
  SleepFor(kThreadStartWaitMsec);

  EXPECT_TRUE(interface_listener_impl_->Stop());
  SleepFor(kThreadStartWaitMsec);

  EXPECT_FALSE(interface_listener_impl_->GetThread()->is_running());

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, Stop_twice) {
  Init("");
  EXPECT_TRUE(interface_listener_impl_->Init());

  // ignore OnIPAddressUpdated call
  EXPECT_CALL(mock_tcp_client_listener_, OnIPAddressUpdated(_, _))
      .Times(AtLeast(0));

  EXPECT_TRUE(interface_listener_impl_->Start());
  SleepFor(kThreadStartWaitMsec);

  EXPECT_TRUE(interface_listener_impl_->Stop());

  EXPECT_FALSE(interface_listener_impl_->Stop());

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, Stop_without_Start) {
  Init("");
  EXPECT_TRUE(interface_listener_impl_->Init());

  EXPECT_FALSE(interface_listener_impl_->Stop());

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, DesignatedInterface_IPAddressChanged) {
  Init("dummy_int0");
  EXPECT_TRUE(interface_listener_impl_->Init());

  struct InterfaceEntry entries1[] = {
      {"dummy_int0", "1.2.3.4", NULL, IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};
  struct InterfaceEntry entries2[] = {
      {"dummy_int0", "5.6.7.8", NULL, IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};

  SetDummyInterfaceTable(entries1);

  EXPECT_CALL(mock_tcp_client_listener_,
              OnIPAddressUpdated(entries1[0].ipv4_address, "")).Times(1);

  // this test case doesn't call Start() - we only check the behavior of
  // NotifyIPAddresses()
  interface_listener_impl_->testCallNotifyIPAddresses();

  SetDummyInterfaceTable(entries2);

  EXPECT_CALL(mock_tcp_client_listener_,
              OnIPAddressUpdated(entries2[0].ipv4_address, "")).Times(1);

  interface_listener_impl_->testCallNotifyIPAddresses();

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, DesignatedInterface_IPAddressNotChanged) {
  Init("dummy_int0");
  EXPECT_TRUE(interface_listener_impl_->Init());

  struct InterfaceEntry entries1[] = {
      {"dummy_int0", "1.2.3.4", NULL, IFF_UP | IFF_RUNNING},
      {"dummy_int1", "10.10.10.12", NULL, IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};
  struct InterfaceEntry entries2[] = {
      {"dummy_int0", "1.2.3.4", NULL, IFF_UP | IFF_RUNNING},
      {"dummy_int1", "172.16.23.30", NULL, IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};

  SetDummyInterfaceTable(entries1);

  EXPECT_CALL(mock_tcp_client_listener_,
              OnIPAddressUpdated(entries1[0].ipv4_address, "")).Times(1);

  interface_listener_impl_->testCallNotifyIPAddresses();

  SetDummyInterfaceTable(entries2);

  // OnIPAddressUpdated() shouldn't be notified
  EXPECT_CALL(mock_tcp_client_listener_, OnIPAddressUpdated(_, _)).Times(0);

  interface_listener_impl_->testCallNotifyIPAddresses();

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, DesignatedInterface_GoesUnavailable) {
  Init("dummy_int0");
  EXPECT_TRUE(interface_listener_impl_->Init());

  struct InterfaceEntry entries1[] = {
      {"dummy_int0", "1.2.3.4", "fdc2:12af:327a::1", IFF_UP | IFF_RUNNING},
      {"dummy_int1", "10.10.10.12", NULL, IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};
  struct InterfaceEntry entries2[] = {
      {"dummy_int0", "1.2.3.4", "fdc2:12af:327a::1", IFF_UP},
      {"dummy_int1", "10.10.10.12", NULL, IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};

  SetDummyInterfaceTable(entries1);

  EXPECT_CALL(mock_tcp_client_listener_,
              OnIPAddressUpdated(entries1[0].ipv4_address,
                                 entries1[0].ipv6_address)).Times(1);

  interface_listener_impl_->testCallNotifyIPAddresses();

  SetDummyInterfaceTable(entries2);

  EXPECT_CALL(mock_tcp_client_listener_, OnIPAddressUpdated("", "")).Times(1);

  interface_listener_impl_->testCallNotifyIPAddresses();

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, DesignatedInterface_Removed) {
  Init("dummy_int0");
  EXPECT_TRUE(interface_listener_impl_->Init());

  struct InterfaceEntry entries1[] = {
      {"dummy_int0", "1.2.3.4", "fdc2:12af:327a::1", IFF_UP | IFF_RUNNING},
      {"dummy_int1", "10.10.10.12", NULL, IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};
  struct InterfaceEntry entries2[] = {
      {"dummy_int1", "10.10.10.12", NULL, IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};

  SetDummyInterfaceTable(entries1);

  EXPECT_CALL(mock_tcp_client_listener_,
              OnIPAddressUpdated(entries1[0].ipv4_address,
                                 entries1[0].ipv6_address)).Times(1);

  interface_listener_impl_->testCallNotifyIPAddresses();

  SetDummyInterfaceTable(entries2);

  EXPECT_CALL(mock_tcp_client_listener_, OnIPAddressUpdated("", "")).Times(1);

  interface_listener_impl_->testCallNotifyIPAddresses();

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, DesignatedInterface_Added) {
  Init("dummy_int0");
  EXPECT_TRUE(interface_listener_impl_->Init());

  struct InterfaceEntry entries1[] = {
      {"dummy_int1", "10.10.10.12", NULL, IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};
  struct InterfaceEntry entries2[] = {
      {"dummy_int1", "10.10.10.12", NULL, IFF_UP | IFF_RUNNING},
      {"dummy_int0", "1.2.3.4", NULL, IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};

  SetDummyInterfaceTable(entries1);

  interface_listener_impl_->testCallNotifyIPAddresses();

  SetDummyInterfaceTable(entries2);

  EXPECT_CALL(mock_tcp_client_listener_,
              OnIPAddressUpdated(entries2[1].ipv4_address, "")).Times(1);

  interface_listener_impl_->testCallNotifyIPAddresses();

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, AutoSelectInterface_SelectInterface) {
  // automatically select network interface
  Init("");
  EXPECT_TRUE(interface_listener_impl_->Init());

  struct InterfaceEntry entries[] = {
      {"dummy_int1", "10.10.10.12", NULL, IFF_UP | IFF_RUNNING},
      {"net_dummy2", "192.168.2.3", "fdc2:12af:327a::22", IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};

  SetDummyInterfaceTable(entries);

  std::string output_ipv4_address;
  std::string output_ipv6_address;
  EXPECT_CALL(mock_tcp_client_listener_, OnIPAddressUpdated(_, _))
      .WillOnce(DoAll(SaveArg<0>(&output_ipv4_address),
                      SaveArg<1>(&output_ipv6_address)));

  interface_listener_impl_->testCallNotifyIPAddresses();

  std::string selected_interface =
      interface_listener_impl_->GetSelectedInterfaceName();

  // the interface listener should pick one of the interfaces
  EXPECT_TRUE((selected_interface == entries[0].name &&
               output_ipv4_address == entries[0].ipv4_address &&
               output_ipv6_address == "") ||
              (selected_interface == entries[1].name &&
               output_ipv4_address == entries[1].ipv4_address &&
               output_ipv6_address == entries[1].ipv6_address));

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest,
       AutoSelectInterface_SkipUnavailableInterface) {
  Init("");
  EXPECT_TRUE(interface_listener_impl_->Init());

  struct InterfaceEntry entries[] = {
      {"dummy_int1", "10.10.10.12", NULL, IFF_UP},
      {"net_dummy2", "192.168.2.3", "fdc2:12af:327a::22", IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};

  SetDummyInterfaceTable(entries);

  // dummy_int1 should not be selected
  struct InterfaceEntry* expected = &entries[1];
  EXPECT_CALL(mock_tcp_client_listener_,
              OnIPAddressUpdated(expected->ipv4_address,
                                 expected->ipv6_address)).Times(1);

  interface_listener_impl_->testCallNotifyIPAddresses();

  EXPECT_EQ(expected->name,
            interface_listener_impl_->GetSelectedInterfaceName());

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, AutoSelectInterface_SkipEmptyInterface) {
  Init("");
  EXPECT_TRUE(interface_listener_impl_->Init());

  struct InterfaceEntry entries[] = {
      {"dummy_int1", "10.10.10.12", NULL, IFF_UP | IFF_RUNNING},
      {"net_dummy2", NULL, NULL, IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};

  SetDummyInterfaceTable(entries);

  // net_dummy2 should not be selected
  struct InterfaceEntry* expected = &entries[0];
  EXPECT_CALL(mock_tcp_client_listener_,
              OnIPAddressUpdated(expected->ipv4_address, "")).Times(1);

  interface_listener_impl_->testCallNotifyIPAddresses();

  EXPECT_EQ(expected->name,
            interface_listener_impl_->GetSelectedInterfaceName());

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest,
       AutoSelectInterface_SkipLoopbackInterface) {
  Init("");
  EXPECT_TRUE(interface_listener_impl_->Init());

  struct InterfaceEntry entries[] = {
      {"dummy_int1", "10.10.10.12", NULL, IFF_UP | IFF_RUNNING | IFF_LOOPBACK},
      {"net_dummy2", "192.168.2.3", "fdc2:12af:327a::22", IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};

  // dummy_int1 should not be selected
  struct InterfaceEntry* expected = &entries[1];
  EXPECT_CALL(mock_tcp_client_listener_,
              OnIPAddressUpdated(expected->ipv4_address,
                                 expected->ipv6_address)).Times(1);

  SetDummyInterfaceTable(entries);

  interface_listener_impl_->testCallNotifyIPAddresses();

  EXPECT_EQ(expected->name,
            interface_listener_impl_->GetSelectedInterfaceName());

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, AutoSelectInterface_DisableInterface) {
  Init("");
  EXPECT_TRUE(interface_listener_impl_->Init());

  struct InterfaceEntry entries[] = {
      {"net_dummy0", "192.168.2.3", "fdc2:12af:327a::22", IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};

  EXPECT_CALL(mock_tcp_client_listener_, OnIPAddressUpdated(_, _)).Times(1);
  SetDummyInterfaceTable(entries);

  interface_listener_impl_->testCallNotifyIPAddresses();

  // make the interface "not running"
  entries[0].flags &= ~IFF_RUNNING;
  SetDummyInterfaceTable(entries);

  EXPECT_CALL(mock_tcp_client_listener_, OnIPAddressUpdated("", "")).Times(1);

  interface_listener_impl_->testCallNotifyIPAddresses();

  EXPECT_EQ("", interface_listener_impl_->GetSelectedInterfaceName());

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, AutoSelectInterface_EnableInterface) {
  Init("");
  EXPECT_TRUE(interface_listener_impl_->Init());

  struct InterfaceEntry entries[] = {
      {"net_dummy0", "192.168.2.3", "fdc2:12af:327a::22", IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};

  EXPECT_CALL(mock_tcp_client_listener_, OnIPAddressUpdated(_, _)).Times(1);
  SetDummyInterfaceTable(entries);

  interface_listener_impl_->testCallNotifyIPAddresses();

  // make the interface "not running"
  entries[0].flags &= ~IFF_RUNNING;
  SetDummyInterfaceTable(entries);

  EXPECT_CALL(mock_tcp_client_listener_, OnIPAddressUpdated(_, _)).Times(1);

  interface_listener_impl_->testCallNotifyIPAddresses();

  // make it running again
  entries[0].flags |= IFF_RUNNING;
  SetDummyInterfaceTable(entries);

  EXPECT_CALL(mock_tcp_client_listener_,
              OnIPAddressUpdated(entries[0].ipv4_address,
                                 entries[0].ipv6_address)).Times(1);

  interface_listener_impl_->testCallNotifyIPAddresses();

  EXPECT_EQ(entries[0].name,
            interface_listener_impl_->GetSelectedInterfaceName());

  Deinit();
}

TEST_F(NetworkInterfaceListenerTest, AutoSelectInterface_SwitchInterface) {
  Init("");
  EXPECT_TRUE(interface_listener_impl_->Init());

  struct InterfaceEntry entries[] = {
      {"dummy_int1",
       "10.10.10.12",
       "fd53:ba79:241d:30c1::78",
       IFF_UP | IFF_RUNNING},
      {"net_dummy2", "192.168.2.3", "fdc2:12af:327a::22", IFF_UP | IFF_RUNNING},
      {NULL, NULL, NULL, 0}};

  EXPECT_CALL(mock_tcp_client_listener_, OnIPAddressUpdated(_, _)).Times(1);
  SetDummyInterfaceTable(entries);

  interface_listener_impl_->testCallNotifyIPAddresses();

  const std::string selected_interface =
      interface_listener_impl_->GetSelectedInterfaceName();
  struct InterfaceEntry* selected = &entries[0];
  while (selected->name != NULL) {
    if (selected->name == selected_interface) {
      break;
    }
    selected++;
  }
  ASSERT_TRUE(selected->name != NULL);

  // make the interface "not running"
  selected->flags &= ~IFF_RUNNING;
  SetDummyInterfaceTable(entries);

  struct InterfaceEntry* switched;
  if (selected == &entries[0]) {
    switched = &entries[1];
  } else {
    switched = &entries[0];
  }

  EXPECT_CALL(mock_tcp_client_listener_,
              OnIPAddressUpdated(switched->ipv4_address,
                                 switched->ipv6_address)).Times(1);

  interface_listener_impl_->testCallNotifyIPAddresses();

  EXPECT_EQ(switched->name,
            interface_listener_impl_->GetSelectedInterfaceName());

  Deinit();
}

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test
