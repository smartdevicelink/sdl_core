/**
 * \file tcp_client_listener.h
 * \brief TcpClientListener class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_TCP_CLIENT_LISTENER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TCP_TCP_CLIENT_LISTENER_H_

#include "transport_manager/device_adapter/client_connection_listener.h"

namespace transport_manager {
namespace device_adapter {

class DeviceAdapterController;

/**
 * @brief Listener of device adapter that use TCP transport.
 */
class TcpClientListener : public ClientConnectionListener {
 public:

  /**
   * @breaf Constructor.
   *
   * @param controller Pointer to the device adapter controller.
   * @param port Port No.
   */
  TcpClientListener(DeviceAdapterController* controller, const uint16_t port);

  /**
   * @brief Start TCP client listener thread.
   */
  void thread();
 protected:

  /**
   * @brief Destructor.
   */
  virtual ~TcpClientListener();

  /**
   * @brief Run TCP client listener.
   *
   * @return Error information about possible reason of starting TCP listener listener failure.
   */
  virtual DeviceAdapter::Error init();

  /**
   * @brief Stop TCP client listener.
   */
  virtual void terminate();

  /**
   * @brief Check initialization.
   *
   * @return True if initialized.
   * @return False if not initialized.
   */
  virtual bool isInitialised() const;

  /**
   * @brief
   *
   * @return Error information about possible reason of failure.
   */
  virtual DeviceAdapter::Error startListening();

  /**
   * @brief Terminate TCP client listener thread.
   */
  virtual DeviceAdapter::Error stopListening();
 private:
  const uint16_t port_;
  DeviceAdapterController* controller_;
  pthread_t thread_;
  int socket_;
  bool thread_started_;
  bool shutdown_requested_;
  bool thread_stop_requested_;
};

}  // namespace device_adapter
}  // namespace transport_manager

#endif /* TCP_CLIENT_LISTENER_H_ */
