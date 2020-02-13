/*
 * \file websocket_listener.h
 * \brief WebSocketListener class header file.
 *
 * Copyright (c) 2020
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_LISTENER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_LISTENER_H_

#include <boost/asio/thread_pool.hpp>
#include <memory>
#include <vector>
#include "transport_manager/transport_adapter/client_connection_listener.h"
#include "transport_manager/transport_manager_settings.h"
#include "transport_manager/websocket_server/websocket_connection.h"

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController;

/**
 * @brief Class responsible for communication over websockets.
 */
class WebSocketListener : public ClientConnectionListener {
 public:
  /**
   * @brief Constructor.
   * @param controller Pointer to the device adapter controller.
   * @param number of threads for listen incoming connections
   */
  WebSocketListener(TransportAdapterController* controller,
                    const TransportManagerSettings& settings,
                    const int num_threads = 1);

  /**
   * @brief Destructor.
   */
  ~WebSocketListener();

  TransportAdapter::Error Init() OVERRIDE;
  void Terminate() OVERRIDE;
  bool IsInitialised() const OVERRIDE {
    return true;
  }
  TransportAdapter::Error StartListening() OVERRIDE;
  TransportAdapter::Error StopListening() OVERRIDE {
    return TransportAdapter::OK;
  }
  TransportAdapter::Error SuspendListening() OVERRIDE {
    return TransportAdapter::OK;
  }
  TransportAdapter::Error ResumeListening() OVERRIDE {
    return TransportAdapter::OK;
  }

 protected:
#ifdef ENABLE_SECURITY
  TransportAdapter::Error AddCertificateAuthority();
#endif
  bool Run();
  bool WaitForConnection();
  void StartSession(boost::system::error_code ec);
  void Shutdown();

  template <typename Connection>
  void ProcessConnection(std::shared_ptr<Connection> connection,
                         const DeviceSptr,
                         const ApplicationHandle);

 private:
  TransportAdapterController* controller_;
  boost::asio::io_context ioc_;
#ifdef ENABLE_SECURITY
  ssl::context ctx_;
  bool start_secure_;
#endif  // ENABLE_SECURITY
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  std::unique_ptr<boost::asio::thread_pool> io_pool_;
  const int num_threads_;
  std::atomic_bool shutdown_;
  std::vector<std::shared_ptr<Connection> > connection_list_;
  sync_primitives::Lock connection_list_lock;
  const TransportManagerSettings& settings_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_WEBSOCKET_SERVER_WEBSOCKET_LISTENER_H_
