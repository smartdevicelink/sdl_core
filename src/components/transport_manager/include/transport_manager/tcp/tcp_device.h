/**
 * \file tcp_device.h
 * \brief TcpDevice class header file.
 *
 * Copyright (c) 2013, Ford Motor Company
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
 * Neither the name of the Ford Motor Company nor the names of its contributors
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_TCP_DEVICE_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_TCP_DEVICE_H_

#include "transport_manager/transport_adapter/device.h"

#include <map>
#include <memory.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace transport_manager {
namespace transport_adapter {

/**
 * @brief Information about device that use TCP transport.
 */
class TcpDevice : public Device {
 public:
  /**
   * @brief Constructor.
   *
   * @param in_addr Address.
   * @param name Device Name.
   **/
  TcpDevice(const in_addr_t& in_addr, const std::string& name);

  virtual ~TcpDevice();

  /**
   * @brief Compare devices.
   *
   * @param other Device to compare with.
   *
   * @return True if devices are equal, false otherwise.
   **/
  virtual bool IsSameAs(const Device* other) const;

  /**
   * @brief Update list of applications available on device.
   *
   * @return Container with list of applications.
   */
  virtual ApplicationList GetApplicationList() const;

  /**
   * @brief Add an application to the container of applications.
   *
   * @param socket Value of socket.
   */
  ApplicationHandle AddIncomingApplication(int socket_fd);

  /**
   * @brief Add application that was discovered before.
   *
   * @param Port No.
   */
  ApplicationHandle AddDiscoveredApplication(int port);

  /**
   * @brief Remove application from the container of applications.
   *
   * @param app_handle Handle of application.
   */
  void RemoveApplication(const ApplicationHandle app_handle);

  /**
   * @brief Return application's socket value.
   *
   * @param app_handle Handle of application.
   *
   * @return Application's socket value.
   */
  int GetApplicationSocket(const ApplicationHandle app_handle) const;

  /**
   * @brief Return application's port No.
   *
   * @param app_handle Handle of application.
   *
   * @return Application's port No.
   */
  int GetApplicationPort(const ApplicationHandle app_handle) const;

  /**
   * @brief Return address.
   *
   * @return Address.
   */
  in_addr_t in_addr() const {
    return in_addr_;
  }

 private:
  struct Application {
    bool incoming;
    int socket;
    uint16_t port;
  };
  std::map<ApplicationHandle, Application> applications_;
  mutable pthread_mutex_t applications_mutex_;
  const in_addr_t in_addr_;
  const std::string name_;
  ApplicationHandle last_handle_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif /* TCP_DEVICE_H_ */
