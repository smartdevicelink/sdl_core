#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_PLATFORM_SPECIFIC_LINUX_PLATFORM_SPECIFIC_NETWORK_INTERFACE_LISTENER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_PLATFORM_SPECIFIC_LINUX_PLATFORM_SPECIFIC_NETWORK_INTERFACE_LISTENER_H_

#include <map>
#include <string>
#include <vector>

#include <netinet/in.h>
#include "transport_manager/tcp/network_interface_listener.h"
#include "utils/macro.h"
#include "utils/threads/thread_delegate.h"

struct ifaddrmsg;

namespace transport_manager {
namespace transport_adapter {

class TcpClientListener;

/**
 * @brief Struct to keep network interface's status flags and IP addresses
 */
class InterfaceStatus {
 public:
  InterfaceStatus() : flags_(0), has_ipv4_(false), has_ipv6_(false) {}
  ~InterfaceStatus() {}

  bool IsAvailable() const;
  bool IsLoopback() const;
  // only for debugging output
  unsigned int GetFlags() const {
    return flags_;
  }

  bool HasIPAddress() const;
  std::string GetIPv4Address() const;
  std::string GetIPv6Address() const;

  void SetFlags(unsigned int flags) {
    flags_ = flags;
  }

  // specify NULL to remove existing address
  void SetIPv4Address(struct in_addr* addr);
  void SetIPv6Address(struct in6_addr* addr);

 private:
  unsigned int flags_;
  bool has_ipv4_;
  bool has_ipv6_;
  struct in_addr ipv4_address_;
  struct in6_addr ipv6_address_;
};

typedef std::map<std::string, InterfaceStatus> InterfaceStatusTable;

/**
 * @brief Listener to detect various events on network interfaces
 */
class PlatformSpecificNetworkInterfaceListener
    : public NetworkInterfaceListener {
 public:
  /**
   * @brief Constructor
   *
   * @param tcp_client_listener  an instance of TcpClientListener which receives
   *                             status updates
   * @param designated_interface  if we want to listen only on a specific
   *                              network interface, specify its name
   */
  PlatformSpecificNetworkInterfaceListener(
      TcpClientListener* tcp_client_listener,
      const std::string designated_interface = "");

  /**
   * @brief Destructor
   */
  virtual ~PlatformSpecificNetworkInterfaceListener();

  /**
   * @brief Initialize this listener
   */
  bool Init() OVERRIDE;

  /**
   * @brief Deinitialize this listener
   */
  void Deinit() OVERRIDE;

  /**
   * @brief Start this listener
   */
  bool Start() OVERRIDE;

  /**
   * @brief Stop this listener
   */
  bool Stop() OVERRIDE;

#ifdef BUILD_TESTS
  void SetTesting(bool enabled) {
    testing_ = enabled;
  }

  int GetSocket() const {
    return socket_;
  }

  threads::Thread* GetThread() const {
    return thread_;
  }

  void OverwriteStatusTable(const InterfaceStatusTable dummy_table) {
    status_table_ = dummy_table;
  }

  void testCallNotifyIPAddresses() {
    NotifyIPAddresses();
  }

  const std::string& GetSelectedInterfaceName() const {
    return selected_interface_;
  }
#endif  // BUILD_TESTS

 private:
  // Struct to hold an event on a network interface.
  // The event can be either an update on flags or an update on IP address.
  struct EventParam {
    unsigned int if_index;
    unsigned int flags;
    struct sockaddr_storage address;

    EventParam(int interface_index, unsigned int interface_flags = 0)
        : if_index(interface_index), flags(interface_flags) {}
  };

  // parent class which we will notify the events to
  TcpClientListener* tcp_client_listener_;
  // if configured, NetworkInterfaceListener will always look into the IP
  // addresses of this interface
  const std::string designated_interface_;

  // a map to store status of each interface
  InterfaceStatusTable status_table_;
  // this is the name of the interface we are currently focusing on
  std::string selected_interface_;
  // previous IP addresses that we have notified
  std::string notified_ipv4_addr_;
  std::string notified_ipv6_addr_;

  int socket_;
  int pipe_fds_[2];
  threads::Thread* thread_;

#ifdef BUILD_TESTS
  bool testing_;
#endif

  void Loop();
  bool StopLoop();

  // reset status_table_ by fetching current status of each interface
  bool InitializeStatus();
  // update status_table_ by applying the events
  bool UpdateStatus(uint16_t type, std::vector<EventParam>& params);
  // update notified_ipv4_addr_ and notified_ipv6_addr_ then notify the parent
  // class of the change if necessary
  void NotifyIPAddresses();
  // Select an appropriate network interface that we will get IP addresses. Also
  // update selected_interface_.
  const std::string SelectInterface();
  // convert ifaddrmsg to a list of EventParam structs
  std::vector<EventParam> ParseIFAddrMessage(struct ifaddrmsg* message,
                                             unsigned int size);
  // for debugging
  void DumpTable() const;

  class ListenerThreadDelegate : public threads::ThreadDelegate {
   public:
    explicit ListenerThreadDelegate(
        PlatformSpecificNetworkInterfaceListener* parent);
    virtual void threadMain();
    void exitThreadMain();

   private:
    PlatformSpecificNetworkInterfaceListener* parent_;
  };

  DISALLOW_COPY_AND_ASSIGN(PlatformSpecificNetworkInterfaceListener);
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_PLATFORM_SPECIFIC_LINUX_PLATFORM_SPECIFIC_NETWORK_INTERFACE_LISTENER_H_
