/*
 * Copyright (c) 2018 Xevo Inc.
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
 * Neither the names of the copyright holders nor the names of its contributors
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_NETWORK_INTERFACE_LISTENER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_NETWORK_INTERFACE_LISTENER_H_

#include <map>
#include <string>
#include <vector>

#include <netinet/in.h>
#include "utils/macro.h"
#include "utils/threads/thread_delegate.h"

class Thread;
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
class NetworkInterfaceListener {
 public:
  /**
   * @brief Constructor
   *
   * @param tcp_client_listener  an instance of TcpClientListener which receives
   *                             status updates
   * @param designated_interface  if we want to listen only on a specific
   *                              network interface, specify its name
   */
  NetworkInterfaceListener(TcpClientListener* tcp_client_listener,
                           const std::string designated_interface = "");

  /**
   * @brief Destructor
   */
  virtual ~NetworkInterfaceListener();

  /**
   * @brief Initialize this listener
   */
  bool Init();

  /**
   * @brief Deinitialize this listener
   */
  void Deinit();

  /**
   * @brief Start this listener
   */
  bool Start();

  /**
   * @brief Stop this listener
   */
  bool Stop();

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
    explicit ListenerThreadDelegate(NetworkInterfaceListener* parent);
    virtual void threadMain();
    void exitThreadMain();

   private:
    NetworkInterfaceListener* parent_;
  };

  DISALLOW_COPY_AND_ASSIGN(NetworkInterfaceListener);
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_NETWORK_INTERFACE_LISTENER_H_
