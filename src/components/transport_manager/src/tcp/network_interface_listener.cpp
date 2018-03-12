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

#include "transport_manager/tcp/network_interface_listener.h"

#include <arpa/inet.h>
#include <asm/types.h>
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#ifdef __linux__
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#endif  // __linux__

#include "transport_manager/tcp/tcp_client_listener.h"
#include "utils/logger.h"
#include "utils/threads/thread.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

static std::string GetInterfaceName(unsigned int if_index);
static bool SetNonblocking(int s);

bool InterfaceStatus::IsAvailable() const {
  // check if the interface is UP and RUNNING
  return ((flags_ & IFF_UP) > 0) && ((flags_ & IFF_RUNNING) > 0);
}

bool InterfaceStatus::IsLoopback() const {
  return flags_ & IFF_LOOPBACK;
}

bool InterfaceStatus::HasIPAddress() const {
  return has_ipv4_ || has_ipv6_;
}

std::string InterfaceStatus::GetIPv4Address() const {
  char buf[INET_ADDRSTRLEN] = "";
  if (has_ipv4_) {
    inet_ntop(AF_INET, &ipv4_address_, buf, sizeof(buf));
  }
  return std::string(buf);
}

std::string InterfaceStatus::GetIPv6Address() const {
  char buf[INET6_ADDRSTRLEN] = "";
  if (has_ipv6_) {
    inet_ntop(AF_INET6, &ipv6_address_, buf, sizeof(buf));
  }
  return std::string(buf);
}

void InterfaceStatus::SetIPv4Address(struct in_addr* addr) {
  if (addr == NULL) {
    has_ipv4_ = false;
  } else {
    ipv4_address_ = *addr;
    has_ipv4_ = true;
  }
}

void InterfaceStatus::SetIPv6Address(struct in6_addr* addr) {
  if (addr == NULL) {
    has_ipv6_ = false;
  } else {
    ipv6_address_ = *addr;
    has_ipv6_ = true;
  }
}

NetworkInterfaceListener::NetworkInterfaceListener(
    TcpClientListener* tcp_client_listener,
    const std::string designated_interface)
    : tcp_client_listener_(tcp_client_listener)
    , designated_interface_(designated_interface)
    , selected_interface_("")
    , notified_ipv4_addr_("")
    , notified_ipv6_addr_("")
    , socket_(-1) {
  pipe_fds_[0] = pipe_fds_[1] = -1;
  thread_ = threads::CreateThread("NetworkInterfaceListener",
                                  new ListenerThreadDelegate(this));
}

NetworkInterfaceListener::~NetworkInterfaceListener() {
  LOG4CXX_AUTO_TRACE(logger_);

  Stop();
  Deinit();

  delete thread_->delegate();
  threads::DeleteThread(thread_);
}

bool NetworkInterfaceListener::Init() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (socket_ >= 0) {
    LOG4CXX_WARN(logger_, "Network interface listener is already initialized");
    return false;
  }

  socket_ = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
  if (socket_ == -1) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to create netlink socket");
    return false;
  }

  if (!SetNonblocking(socket_)) {
    LOG4CXX_WARN(logger_, "Failed to configure netlink socket to non-blocking");
  }

  struct sockaddr_nl addr;
  memset(&addr, 0, sizeof(addr));
  addr.nl_family = AF_NETLINK;
  addr.nl_pad = 0;
  addr.nl_pid = 0;
  addr.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR;

  if (bind(socket_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to bind netlink socket");
    close(socket_);
    socket_ = -1;
    return false;
  }

  if (pipe(pipe_fds_) != 0) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to create internal pipe");
    close(socket_);
    socket_ = -1;
    return false;
  }

  if (!SetNonblocking(pipe_fds_[0])) {
    LOG4CXX_WARN(logger_, "Failed to configure pipe to non-blocking");
  }

  return true;
}

void NetworkInterfaceListener::Deinit() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (socket_ >= 0) {
    close(socket_);
    socket_ = -1;
  }
  if (pipe_fds_[1] >= 0) {
    close(pipe_fds_[1]);
    pipe_fds_[1] = -1;
  }
  if (pipe_fds_[0] >= 0) {
    close(pipe_fds_[0]);
    pipe_fds_[0] = -1;
  }
}

bool NetworkInterfaceListener::Start() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (socket_ < 0) {
    LOG4CXX_WARN(logger_, "Interface listener is not initialized");
    return false;
  }

  if (thread_->is_running()) {
    LOG4CXX_WARN(logger_, "Interface listener is already started");
    return false;
  }

  if (!thread_->start()) {
    LOG4CXX_ERROR(logger_, "Failed to start interface listener");
    return false;
  }

  LOG4CXX_INFO(logger_, "Network interface listener started");
  return true;
}

bool NetworkInterfaceListener::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!thread_->is_running()) {
    LOG4CXX_DEBUG(logger_, "interface listener is not running");
    return false;
  }

  thread_->join();

  LOG4CXX_INFO(logger_, "Network interface listener stopped");
  return true;
}

void NetworkInterfaceListener::Loop() {
  LOG4CXX_AUTO_TRACE(logger_);

  InitializeStatus();
  NotifyIPAddresses();

  char buf[4096];
  fd_set rfds;

  while (1) {
    FD_ZERO(&rfds);
    FD_SET(socket_, &rfds);
    FD_SET(pipe_fds_[0], &rfds);
    int nfds = socket_ > pipe_fds_[0] ? socket_ : pipe_fds_[0];

    int ret = select(nfds + 1, &rfds, NULL, NULL, NULL);
    if (ret < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        LOG4CXX_WARN(logger_,
                     "select failed for netlink. Aborting interface listener.");
        break;
      }
    }

    if (FD_ISSET(pipe_fds_[0], &rfds)) {
      ret = read(pipe_fds_[0], buf, sizeof(buf));
      if (ret < 0) {
        if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK) {
          LOG4CXX_WARN(
              logger_,
              "Failed to read from pipe. Aborting interface listener.");
          break;
        }
      } else if (ret == 0) {
        LOG4CXX_WARN(logger_,
                     "Pipe disconnected. Aborting interface listener.");
        break;
      } else {
        LOG4CXX_DEBUG(logger_, "received terminating event through pipe");
        break;
      }
    }

    if (FD_ISSET(socket_, &rfds)) {
      ret = recv(socket_, buf, sizeof(buf), 0);
      if (ret < 0) {
        if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK) {
          LOG4CXX_WARN(logger_,
                       "Failed to read from netlink socket. Aborting interface "
                       "listener.");
          break;
        }
      } else if (ret == 0) {
        LOG4CXX_WARN(
            logger_,
            "Netlink socket disconnected. Aborting interface listener.");
        break;
      } else {
        struct nlmsghdr* header = reinterpret_cast<struct nlmsghdr*>(buf);
        int len = ret;

        for (; NLMSG_OK(header, len); header = NLMSG_NEXT(header, len)) {
          if (header->nlmsg_type == NLMSG_ERROR) {
            LOG4CXX_WARN(logger_, "received error event from netlink");
            break;
          }

          std::vector<EventParam> params;

          if (header->nlmsg_type == RTM_NEWLINK ||
              header->nlmsg_type == RTM_DELLINK) {
            struct ifinfomsg* ifinfo_msg =
                reinterpret_cast<struct ifinfomsg*>(NLMSG_DATA(header));
            EventParam param(ifinfo_msg->ifi_index, ifinfo_msg->ifi_flags);
            params.push_back(param);

          } else if (header->nlmsg_type == RTM_NEWADDR ||
                     header->nlmsg_type == RTM_DELADDR) {
            struct ifaddrmsg* ifaddr_msg =
                reinterpret_cast<struct ifaddrmsg*>(NLMSG_DATA(header));
            unsigned int size = IFA_PAYLOAD(header);
            params = ParseIFAddrMessage(ifaddr_msg, size);

          } else {
            continue;
          }

          UpdateStatus(header->nlmsg_type, params);
        }
      }

      NotifyIPAddresses();
    }
  }
}

bool NetworkInterfaceListener::StopLoop() {
  LOG4CXX_AUTO_TRACE(logger_);

  LOG4CXX_INFO(logger_, "Stopping network interface listener");

  if (pipe_fds_[1] < 0) {
    LOG4CXX_WARN(logger_, "StopLoop called in invalid state");
    return false;
  }

  char dummy[1] = {0};
  int ret = write(pipe_fds_[1], dummy, sizeof(dummy));
  if (ret <= 0) {
    LOG4CXX_WARN_WITH_ERRNO(
        logger_, "Failed to send stop message to interface listener");
    return false;
  }

  return true;
}

bool NetworkInterfaceListener::InitializeStatus() {
  LOG4CXX_AUTO_TRACE(logger_);

  struct ifaddrs* if_list, *interface;
  if (getifaddrs(&if_list) != 0) {
    LOG4CXX_WARN(logger_,
                 "getifaddr failed, interface status won't be available until "
                 "a change occurs");
    return false;
  }

  // clear existing table
  status_table_.clear();

  for (interface = if_list; interface != NULL;
       interface = interface->ifa_next) {
    if (interface->ifa_name == NULL || interface->ifa_name[0] == '\0') {
      continue;
    }
    if (interface->ifa_addr == NULL) {
      continue;
    }

    std::string ifname(interface->ifa_name);
    InterfaceStatus& status = status_table_[ifname];

    switch (interface->ifa_addr->sa_family) {
      case AF_INET: {
        struct sockaddr_in* addr =
            reinterpret_cast<struct sockaddr_in*>(interface->ifa_addr);
        status.SetIPv4Address(&addr->sin_addr);
        break;
      }
      case AF_INET6: {
        struct sockaddr_in6* addr =
            reinterpret_cast<struct sockaddr_in6*>(interface->ifa_addr);
        status.SetIPv6Address(&addr->sin6_addr);
        break;
      }
      default:
        continue;
    }
    status.SetFlags(interface->ifa_flags);
  }

  freeifaddrs(if_list);

  LOG4CXX_DEBUG(logger_, "Successfully acquired network interface status");
  DumpTable();
  return true;
}

bool NetworkInterfaceListener::UpdateStatus(uint16_t type,
                                            std::vector<EventParam>& params) {
  LOG4CXX_AUTO_TRACE(logger_);

  for (std::vector<EventParam>::iterator it = params.begin();
       it != params.end();
       ++it) {
    std::string ifname = GetInterfaceName(it->if_index);
    if (ifname.empty()) {
      continue;
    }

    InterfaceStatus& status = status_table_[ifname];

    switch (type) {
      case RTM_NEWLINK: {
        LOG4CXX_DEBUG(logger_,
                      "netlink event: interface " << ifname
                                                  << " created or updated");
        status.SetFlags(it->flags);
        break;
      }
      case RTM_DELLINK:
        LOG4CXX_DEBUG(logger_,
                      "netlink event: interface " << ifname << " removed");
        status_table_.erase(ifname);
        break;
      case RTM_NEWADDR: {
        sockaddr* addr = reinterpret_cast<sockaddr*>(&it->address);
        if (addr->sa_family == AF_INET) {
          sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(addr);
          status.SetIPv4Address(&addr_in->sin_addr);
          LOG4CXX_DEBUG(logger_,
                        "netlink event: IPv4 address of interface "
                            << ifname << " updated to "
                            << status.GetIPv4Address());
        } else if (addr->sa_family == AF_INET6) {
          sockaddr_in6* addr_in6 = reinterpret_cast<sockaddr_in6*>(addr);
          status.SetIPv6Address(&addr_in6->sin6_addr);
          LOG4CXX_DEBUG(logger_,
                        "netlink event: IPv6 address of interface "
                            << ifname << " updated to "
                            << status.GetIPv6Address());
        }
        break;
      }
      case RTM_DELADDR: {
        sockaddr* addr = reinterpret_cast<sockaddr*>(&it->address);
        if (addr->sa_family == AF_INET) {
          LOG4CXX_DEBUG(logger_,
                        "netlink event: IPv4 address of interface "
                            << ifname << " removed");
          status.SetIPv4Address(NULL);
        } else if (addr->sa_family == AF_INET6) {
          LOG4CXX_DEBUG(logger_,
                        "netlink event: IPv6 address of interface "
                            << ifname << " removed");
          status.SetIPv6Address(NULL);
        }
        break;
      }
      default:
        LOG4CXX_WARN(logger_, "Unsupported netlink event (" << type << ")");
        break;
    }
  }
  return true;
}

void NetworkInterfaceListener::NotifyIPAddresses() {
  LOG4CXX_AUTO_TRACE(logger_);

  std::string ipv4_addr;
  std::string ipv6_addr;
  const std::string interface_name = SelectInterface();

  // note that if interface_name is empty (i.e. no interface is selected),
  // the IP addresses will be empty
  if (!interface_name.empty()) {
    InterfaceStatus& status = status_table_[interface_name];
    ipv4_addr = status.GetIPv4Address();
    ipv6_addr = status.GetIPv6Address();
  }

  if (notified_ipv4_addr_ != ipv4_addr || notified_ipv6_addr_ != ipv6_addr) {
    LOG4CXX_INFO(logger_,
                 "IP address updated: \"" << notified_ipv4_addr_ << "\" -> \""
                                          << ipv4_addr << "\", \""
                                          << notified_ipv6_addr_ << "\" -> \""
                                          << ipv6_addr << "\"");

    notified_ipv4_addr_ = ipv4_addr;
    notified_ipv6_addr_ = ipv6_addr;

    /* TODO: notify the change */
  }
}

const std::string NetworkInterfaceListener::SelectInterface() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!designated_interface_.empty()) {
    return designated_interface_;
  }

  InterfaceStatusTable::iterator it;

  if (!selected_interface_.empty()) {
    // if current network interface is still available and has IP address, then
    // we use it
    it = status_table_.find(selected_interface_);
    if (it != status_table_.end()) {
      InterfaceStatus& status = it->second;
      if (status.IsAvailable() && status.HasIPAddress()) {
        return selected_interface_;
      }
    }
  }

  // pick a network interface that has IP address
  for (it = status_table_.begin(); it != status_table_.end(); ++it) {
    InterfaceStatus& status = it->second;
    // ignore loopback interfaces
    if (status.IsLoopback()) {
      continue;
    }
    // if the interface has to be UP and RUNNING, and must have an IP address
    if (!(status.IsAvailable() && status.HasIPAddress())) {
      continue;
    }

    selected_interface_ = it->first;
    LOG4CXX_DEBUG(logger_,
                  "selecting network interface: " << selected_interface_);
    return selected_interface_;
  }

  return std::string();
}

std::vector<NetworkInterfaceListener::EventParam>
NetworkInterfaceListener::ParseIFAddrMessage(struct ifaddrmsg* message,
                                             unsigned int size) {
  LOG4CXX_AUTO_TRACE(logger_);

  std::vector<EventParam> params;

  for (struct rtattr* attr = IFA_RTA(message); RTA_OK(attr, size);
       attr = RTA_NEXT(attr, size)) {
    if (!(attr->rta_type == IFA_LOCAL || attr->rta_type == IFA_ADDRESS)) {
      continue;
    }

    EventParam param(message->ifa_index);

    if (message->ifa_family == AF_INET) {
      // make sure the size of data is >= 4 bytes
      if (RTA_PAYLOAD(attr) < sizeof(struct in_addr)) {
        LOG4CXX_DEBUG(logger_,
                      "Invalid netlink event: insufficient IPv4 address data");
        continue;
      }
      struct in_addr* ipv4_addr =
          reinterpret_cast<struct in_addr*>(RTA_DATA(attr));

      struct sockaddr_in* sockaddr =
          reinterpret_cast<struct sockaddr_in*>(&param.address);
      sockaddr->sin_family = AF_INET;
      sockaddr->sin_addr = *ipv4_addr;

    } else if (message->ifa_family == AF_INET6) {
      // make sure the size of data is >= 16 bytes
      if (RTA_PAYLOAD(attr) < sizeof(struct in6_addr)) {
        LOG4CXX_DEBUG(logger_,
                      "Invalid netlink event: insufficient IPv6 address data");
        continue;
      }
      struct in6_addr* ipv6_addr =
          reinterpret_cast<struct in6_addr*>(RTA_DATA(attr));

      struct sockaddr_in6* sockaddr =
          reinterpret_cast<struct sockaddr_in6*>(&param.address);
      sockaddr->sin6_family = AF_INET6;
      sockaddr->sin6_addr = *ipv6_addr;

    } else {
      LOG4CXX_WARN(logger_,
                   "Unsupported family (" << message->ifa_family << ")");
      continue;
    }

    params.push_back(param);
  }

  return params;
}

void NetworkInterfaceListener::DumpTable() const {
  LOG4CXX_DEBUG(logger_,
                "Number of network interfaces: " << status_table_.size());

  for (auto it = status_table_.begin(); it != status_table_.end(); ++it) {
    const std::string ifname = it->first;
    const InterfaceStatus& status = it->second;

    LOG4CXX_DEBUG(
        logger_,
        "  " << ifname << " : flags=" << status.GetFlags()
             << " : available: " << (status.IsAvailable() ? "yes" : "no")
             << " IPv4: " << status.GetIPv4Address()
             << " IPv6: " << status.GetIPv6Address()
             << (status.IsLoopback() ? " (loopback)" : ""));
  }
}

NetworkInterfaceListener::ListenerThreadDelegate::ListenerThreadDelegate(
    NetworkInterfaceListener* parent)
    : parent_(parent) {}

void NetworkInterfaceListener::ListenerThreadDelegate::threadMain() {
  parent_->Loop();
}

void NetworkInterfaceListener::ListenerThreadDelegate::exitThreadMain() {
  parent_->StopLoop();
}

static std::string GetInterfaceName(unsigned int if_index) {
  char buf[IFNAMSIZ + 1] = "";
  if_indextoname(if_index, buf);
  return std::string(buf);
}

static bool SetNonblocking(int s) {
  int prev_flag = fcntl(s, F_GETFL, 0);
  if (prev_flag == -1) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to acquire socket flag");
    return false;
  }

  int ret = fcntl(s, F_SETFL, prev_flag | O_NONBLOCK);
  if (ret == -1) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_,
                             "Failed to configure socket to non-blocking");
    return false;
  }

  return true;
}

}  // namespace transport_adapter
}  // namespace transport_manager
