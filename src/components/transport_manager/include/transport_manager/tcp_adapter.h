/**
 * \file tcp_adapter.h
 * \brief TcpDeviceAdapter class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_ADAPTER
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_ADAPTER

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "device_adapter_socket_communication.h"
#include "device_adapter_impl.h"

namespace transport_manager {

namespace device_adapter {

class TcpClientListener : public ClientConnectionListener {
 public:
  TcpClientListener(DeviceAdapterController* controller, const uint16_t port);
  void thread();
 protected:
  virtual ~TcpClientListener();
  virtual DeviceAdapter::Error init();
  virtual void terminate();
  virtual bool isInitialised() const;
 private:
  const uint16_t port_;
  DeviceAdapterController* controller_;
  pthread_t thread_;
  int socket_;
  bool thread_started_;
  bool shutdown_requested_;
  bool ready_;
};

class TcpDevice : public Device {
 public:
  /**
   * @brief Constructor.
   *
   * @param address Bluetooth address.
   * @param name Human-readable device name.
   * @param rfcomm_channels List of RFCOMM channels where SmartDeviceLink service has been discovered.
   **/
  TcpDevice(const in_addr& in_addr, const char* name);

  virtual ~TcpDevice();

  /**
   * @brief Compare devices.
   *
   * This method checks whether two SBluetoothDevice structures
   * refer to the same device.
   *
   * @param other Device to compare with.
   *
   * @return true if devices are equal, false otherwise.
   **/
  virtual bool isSameAs(const Device* other) const;

  virtual ApplicationList getApplicationList() const;

  ApplicationHandle addApplication(const int socket);
  void removeApplication(const ApplicationHandle app_handle);
  int getApplicationSocket(const ApplicationHandle app_handle) const;

 private:
  std::set<int> applications_;
  mutable pthread_mutex_t applications_mutex_;
  const in_addr in_addr_;
  const std::string name_;
};

class TcpSocketConnection : public ThreadedSocketConnection {
 public:
  TcpSocketConnection(const DeviceHandle& device_handle,
                      const ApplicationHandle& app_handle,
                      DeviceAdapterController* controller);
  virtual ~TcpSocketConnection();
 protected:
  virtual bool establish(ConnectError** error);
};

class TcpDeviceAdapter : public DeviceAdapterImpl {
 public:
  TcpDeviceAdapter();
  virtual ~TcpDeviceAdapter();
  static const uint16_t default_port = 1234;
 protected:
  virtual DeviceType getDeviceType() const;
};

}  // namespace device_adapter

}  // namespace transport_manager

#endif // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_ADAPTER
