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

#include "transport_manager/transport_adapter/client_connection_listener.h"

#include "utils/threads/thread_delegate.h"
#include "utils/threads/thread.h"

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController;

/**
 * @brief Listener of device adapter that use TCP transport.
 */
class TcpClientListener : public ClientConnectionListener,
                          public threads::ThreadDelegate {
 public:
  /**
   * @breaf Constructor.
   *
   * @param controller Pointer to the device adapter controller.
   * @param port Port No.
   * @param enable_keepalive If true enables TCP keepalive on accepted
   *connections
   */
  TcpClientListener(TransportAdapterController* controller, uint16_t port,
                    bool enable_keepalive);

  /**
   * @brief Start TCP client listener thread.
   */
  void threadMain();

  bool exitThreadMain();
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
  virtual TransportAdapter::Error Init();

  /**
   * @brief Stop TCP client listener.
   */
  virtual void Terminate();

  /**
   * @brief Check initialization.
   *
   * @return True if initialized.
   * @return False if not initialized.
   */
  virtual bool IsInitialised() const;

  /**
   * @brief
   *
   * @return Error information about possible reason of failure.
   */
  virtual TransportAdapter::Error StartListening();

  /**
   * @brief Terminate TCP client listener thread.
   */
  virtual TransportAdapter::Error StopListening();
 private:
  const uint16_t port_;
  const bool enable_keepalive_;
  TransportAdapterController* controller_;
  // TODO(Eamakhov): change to threads::Thread usage
  threads::Thread* thread_;
  int socket_;
  bool thread_started_;
  bool thread_stop_requested_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif /* TCP_CLIENT_LISTENER_H_ */
