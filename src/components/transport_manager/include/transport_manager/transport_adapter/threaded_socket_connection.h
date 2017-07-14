/*
 * \file threaded_socket_connection.h
 * \brief Header for classes responsible for communication over sockets.
 * Copyright (c) 2017, Ford Motor Company
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_THREADED_SOCKET_CONNECTION_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_THREADED_SOCKET_CONNECTION_H_

#include <poll.h>
#include <queue>

#include "transport_manager/transport_adapter/connection.h"
#include "protocol/common.h"
#include "utils/atomic_object.h"
#include "utils/threads/thread_delegate.h"
#include "utils/lock.h"

using ::transport_manager::transport_adapter::Connection;

class Thread;

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController;

/**
 * @brief Class responsible for communication over sockets.
 */
class ThreadedSocketConnection : public Connection {
 public:
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

  /**
   * @brief Start thread creation.
   *
   * @return Information about possible reason of thread creation error.
   */
  TransportAdapter::Error Start();

  /**
   * @brief Checks is queue with frames to send empty or not.
   *
   * @return Information about queue is empty or not.
   */
  bool IsFramesToSendQueueEmpty() const;

  /**
   * @brief Check if connection has been terminated.
   *
   * @return Information which states that connection was marked as terminated.
   */
  bool IsConnectionTerminated() const {
    return terminate_flag_;
  }

  /**
   * @brief Set variable that hold socket No.
   */
  void set_socket(int socket) {
    socket_ = socket;
  }

 protected:
  /**
   * @brief Constructor.
   *
   * @param device_uid Device unique identifier.
   * @param app_handle Handle of application.
   * @param controller Pointer to the device adapter controller.
   */
  ThreadedSocketConnection(const DeviceUID& device_uid,
                           const ApplicationHandle& app_handle,
                           TransportAdapterController* controller);

  /**
   * @brief Destructor.
   */
  virtual ~ThreadedSocketConnection();

  /**
   * @brief Check if we can start the connection attempt and establish
   *connection status.
   *
   * @param error contains information of any error that occurred during
   *connection attempt.
   *
   * @return result that states whether we successfully connected or not.
   */
  virtual bool Establish(ConnectError** error) = 0;

  /**
   * @brief Return pointer to the device adapter controller.
   */
  TransportAdapterController* controller() {
    return controller_;
  }

  /**
   * @brief Return device unique identifier.
   */
  DeviceUID device_handle() const {
    return device_uid_;
  }

  /**
   * @brief Return handle of application.
   */
  ApplicationHandle application_handle() const {
    return app_handle_;
  }

  /**
   * @brief Get variable that hold socket No.
   */
  int get_socket() const {
    return socket_;
  }

  /**
   * @brief Checks current socket value, sends shutdown and close commands for
   * this socket and clear current socket value.
   */
  void ShutdownAndCloseSocket();

  /**
   * @brief This method will ensure that thread has finished running and then it
   * will delete this thread.
   */
  void StopAndJoinThread();

 private:
  class SocketConnectionDelegate : public threads::ThreadDelegate {
   public:
    explicit SocketConnectionDelegate(ThreadedSocketConnection* connection);
    void threadMain() OVERRIDE;
    void exitThreadMain() OVERRIDE;

   private:
    ThreadedSocketConnection* connection_;
  };

  int read_fd_;
  int write_fd_;
  void threadMain();
  void Transmit();
  void Finalize();
  TransportAdapter::Error Notify() const;
  bool Receive();
  bool Send();
  void Abort();

  TransportAdapterController* controller_;
  /**
   * @brief Frames that must be sent to remote device.
   **/
  typedef std::queue<protocol_handler::RawMessagePtr> FrameQueue;
  FrameQueue frames_to_send_;
  mutable sync_primitives::Lock frames_to_send_mutex_;

  sync_primitives::atomic_int socket_;
  bool terminate_flag_;
  bool unexpected_disconnect_;
  const DeviceUID device_uid_;
  const ApplicationHandle app_handle_;
  threads::Thread* thread_;
};
}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_THREADED_SOCKET_CONNECTION_H_
