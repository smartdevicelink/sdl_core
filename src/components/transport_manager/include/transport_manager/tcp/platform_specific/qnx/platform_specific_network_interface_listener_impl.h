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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_PLATFORM_SPECIFIC_QNX_PLATFORM_SPECIFIC_NETWORK_INTERFACE_LISTENER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_PLATFORM_SPECIFIC_QNX_PLATFORM_SPECIFIC_NETWORK_INTERFACE_LISTENER_H_

#include <map>
#include <string>
#include <vector>

#include <netinet/in.h>
#include "transport_manager/tcp/network_interface_listener.h"
#include "utils/macro.h"
#include "utils/threads/thread_delegate.h"

class Thread;
struct ifaddrmsg;

namespace transport_manager {
namespace transport_adapter {

class TcpClientListener;

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

 private:
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_PLATFORM_SPECIFIC_QNX_PLATFORM_SPECIFIC_NETWORK_INTERFACE_LISTENER_H_
