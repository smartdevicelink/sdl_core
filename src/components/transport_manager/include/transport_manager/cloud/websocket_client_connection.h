/*
 * \file websocket_client_connection.h
 * \brief WebsocketClientConnection class header file.
 *
 * Copyright (c) 2018, Livio
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_CLOUD_WEBSOCKET_CLIENT_CONNECTION_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_CLOUD_WEBSOCKET_CLIENT_CONNECTION_H_

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#ifdef ENABLE_SECURITY
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/websocket/ssl.hpp>
#endif  // ENABLE_SECURITY
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include "transport_manager/cloud/cloud_websocket_transport_adapter.h"
#include "transport_manager/transport_adapter/connection.h"
#include "utils/message_queue.h"
#include "utils/threads/message_loop_thread.h"
#include "utils/threads/thread.h"

namespace websocket =
    boost::beast::websocket;  // from <boost/beast/websocket.hpp>

using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>
typedef websocket::stream<tcp::socket> WS;

#ifdef ENABLE_SECURITY
namespace ssl = boost::asio::ssl;  // from <boost/asio/ssl.hpp>
typedef websocket::stream<ssl::stream<tcp::socket> > WSS;
#endif  // ENABLE_SECURITY

using ::utils::MessageQueue;

typedef std::queue<protocol_handler::RawMessagePtr> AsyncQueue;
typedef protocol_handler::RawMessagePtr Message;

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController;

/**
 * @brief Class responsible for communication over bluetooth sockets.
 */
class WebsocketClientConnection
    : public std::enable_shared_from_this<WebsocketClientConnection>,
      public Connection {
 public:
  /**
   * @brief Constructor.
   *
   * @param device_uid Device unique identifier.
   * @param app_handle Handle of device.
   * @param controller Pointer to the device adapter controller.
   */
  WebsocketClientConnection(const DeviceUID& device_uid,
                            const ApplicationHandle& app_handle,
                            TransportAdapterController* controller);

  /**
   * @brief Destructor.
   */
  virtual ~WebsocketClientConnection();

  /**
   * @brief Check if we can start the connection attempt and establish
   *connection status.
   *
   * @param error contains information of any error that occurred during
   *connection attempt.
   *
   * @return result that states whether we successfully connected or not.
   */
  TransportAdapter::Error Start();

  /**
   * @brief Send data frame.
   *
   * @param message Smart pointer to the raw message.
   *
   * @return Error Information about possible reason of sending data failure.
   */
  TransportAdapter::Error SendData(::protocol_handler::RawMessagePtr message);

  /**
   * @brief Disconnect the current connection.
   *
   * @return Error Information about possible reason of Disconnect failure.
   */
  TransportAdapter::Error Disconnect();

#ifdef ENABLE_SECURITY
  /**
   * @brief Attempt to add provided certificate to the ssl::context
   *
   * @param cert Certificate string from policy table
   */
  void AddCertificateAuthority(std::string cert, boost::system::error_code& ec);
#endif  // ENABLE_SECURITY

  void Shutdown();

  void Recv(boost::system::error_code ec);

  void OnRead(boost::system::error_code ec, std::size_t bytes_transferred);

 private:
  TransportAdapterController* controller_;
  boost::asio::io_context ioc_;
  tcp::resolver resolver_;
  boost::beast::flat_buffer buffer_;
  std::string host_;
  std::string text_;
  WS ws_;
#ifdef ENABLE_SECURITY
  ssl::context ctx_;
  WSS wss_;
#endif  // ENABLE_SECURITY

  std::atomic_bool shutdown_;

  CloudAppProperties cloud_properties;
  typedef std::queue<protocol_handler::RawMessagePtr> FrameQueue;
  FrameQueue frames_to_send_;
  mutable sync_primitives::Lock frames_to_send_mutex_;

  MessageQueue<Message, AsyncQueue> message_queue_;

  class LoopThreadDelegate : public threads::ThreadDelegate {
   public:
    LoopThreadDelegate(MessageQueue<Message, AsyncQueue>* message_queue,
                       WebsocketClientConnection* handler);

    virtual void threadMain() OVERRIDE;
    virtual void exitThreadMain() OVERRIDE;

    void OnWrite();

    void SetShutdown();

   private:
    void DrainQueue();
    MessageQueue<Message, AsyncQueue>& message_queue_;
    WebsocketClientConnection& handler_;
    sync_primitives::Lock queue_lock_;
    sync_primitives::ConditionalVariable queue_new_items_;
    std::atomic_bool write_pending_;
    std::atomic_bool shutdown_;

    sync_primitives::Lock write_lock_;
  };

  LoopThreadDelegate* thread_delegate_;
  threads::Thread* write_thread_;
  std::thread io_service_thread_;

  const DeviceUID device_uid_;
  const ApplicationHandle app_handle_;

  boost::asio::thread_pool io_pool_;
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_SOCKET_CONNECTION_H_
