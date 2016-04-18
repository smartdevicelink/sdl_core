/*
 * \file threaded_socket_connection.h
 * \brief Header for classes responsible for communication over sockets.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_THREADED_SOCKET_CONNECTION_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_THREADED_SOCKET_CONNECTION_H_

#include <queue>

#include "transport_manager/transport_adapter/connection.h"
#include "protocol/common.h"
#include "utils/threads/thread_delegate.h"
#include "utils/lock.h"
#include "utils/socket.h"
#include "utils/atomic_object.h"
#include "utils/logger.h"
#include "utils/threads/thread.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

using ::transport_manager::transport_adapter::Connection;

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_transport_, "TransportManager")

/**
 * @brief Class responsible for communication over sockets.
 */
template <typename SocketType>
class ThreadedSocketConnection : public Connection,
                                 public utils::TcpConnectionEventHandler {
 public:
  /**
 * @brief Send data frame.
 *
 * @param message Smart pointer to the raw message.
 *
 * @return Error Information about possible reason of sending data failure.
 */
  TransportAdapter::Error SendData(::protocol_handler::RawMessagePtr message) {
    LOGGER_AUTO_TRACE(logger_transport_);
    sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
    frames_to_send_.push(message);
    return Notify();
  }

  /**
 * @brief Disconnect the current connection.
 *
 * @return Error Information about possible reason of Disconnect failure.
 */
  TransportAdapter::Error Disconnect() {
    LOGGER_AUTO_TRACE(logger_transport_);
    terminate_flag_ = true;
    return Notify();
  }

  /**
 * @brief Start thread creation.
 *
 * @return Information about possible reason of thread creation error.
 */
  TransportAdapter::Error Start() {
    LOGGER_AUTO_TRACE(logger_transport_);
    if (!thread_->start()) {
      LOGGER_ERROR(logger_transport_, "thread creation failed");
      return TransportAdapter::FAIL;
    }
    LOGGER_INFO(logger_transport_, "thread created");
    return TransportAdapter::OK;
  }

  /**
 * @brief Set variable that hold socket No. Takes the ownership.
 */
  void SetSocket(SocketType& socket_connection) {
    socket_connection_ = socket_connection;
    socket_connection_.SetEventHandler(this);
  }

  // Implementation of the TcpConnectionEventHandler

  void OnError(int error) {
    LOGGER_ERROR(logger_transport_, "Connection error: " << error);
    Abort();
  }

  void OnData(const uint8_t* const buffer, std::size_t buffer_size) {
    protocol_handler::RawMessagePtr frame(
        new protocol_handler::RawMessage(0, 0, buffer, buffer_size));
    controller_->DataReceiveDone(device_handle(), application_handle(), frame);
  }

  void OnCanWrite() {
    LOGGER_DEBUG(logger_transport_, "OnCanWrite event. Trying to send data.");
    Send();
  }

  void OnClose() {
    LOGGER_DEBUG(logger_transport_, "Connection has been closed");
    Abort();
  }

  // End of implementation

 protected:
  /**
 * @brief Constructor.
 *
 * @param device_uid Device unique identifier.
 * @param app_handle Handle of application.
 * @param controller Pointer to the device adapter controller.
 */
  ThreadedSocketConnection(const DeviceUID& device_id,
                           const ApplicationHandle& app_handle,
                           TransportAdapterController* controller)
      : controller_(controller)
      , frames_to_send_()
      , frames_to_send_mutex_()
      , socket_connection_()
      , terminate_flag_(false)
      , unexpected_disconnect_(false)
      , device_uid_(device_id)
      , app_handle_(app_handle)
      , thread_(NULL) {
    const std::string thread_name = std::string("Socket ") + device_handle();
    thread_ = threads::CreateThread(thread_name.c_str(),
                                    new SocketConnectionDelegate(this));
  }

  /**
 * @brief Destructor.
 */
  virtual ~ThreadedSocketConnection() {
    LOGGER_AUTO_TRACE(logger_transport_);
    Disconnect();
    thread_->join();
    delete thread_->delegate();
    threads::DeleteThread(thread_);
  }

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

 private:
  class SocketConnectionDelegate : public threads::ThreadDelegate {
   public:
    explicit SocketConnectionDelegate(ThreadedSocketConnection* connection)
        : connection_(connection) {}
    void threadMain() {
      LOGGER_AUTO_TRACE(logger_transport_);
      DCHECK(connection_);
      connection_->threadMain();
    }
    void exitThreadMain() {
      LOGGER_AUTO_TRACE(logger_transport_);
    }

   private:
    ThreadedSocketConnection* connection_;
  };

  void threadMain() {
    LOGGER_AUTO_TRACE(logger_transport_);
    controller_->ConnectionCreated(this, device_uid_, app_handle_);
    ConnectError* connect_error = NULL;
    if (!Establish(&connect_error)) {
      LOGGER_ERROR(logger_transport_, "Connection Establish failed");
      delete connect_error;
    }
    LOGGER_DEBUG(logger_transport_, "Connection established");
    controller_->ConnectDone(device_handle(), application_handle());
    while (!terminate_flag_) {
      Transmit();
    }
    LOGGER_DEBUG(logger_transport_, "Connection is to finalize");
    Finalize();
    sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
    while (!frames_to_send_.empty()) {
      LOGGER_INFO(logger_transport_, "removing message");
      ::protocol_handler::RawMessagePtr message = frames_to_send_.front();
      frames_to_send_.pop();
      controller_->DataSendFailed(
          device_handle(), application_handle(), message, DataSendError());
    }
  }

  void Transmit() {
    LOGGER_AUTO_TRACE(logger_transport_);
    LOGGER_DEBUG(logger_transport_, "Waiting for connection events. " << this);
    socket_connection_.Wait();

    LOGGER_DEBUG(logger_transport_, "Waited for connection events: " << this);
  }
  void Finalize() {
    LOGGER_AUTO_TRACE(logger_transport_);
    if (unexpected_disconnect_) {
      LOGGER_DEBUG(logger_transport_, "unexpected_disconnect");
      controller_->ConnectionAborted(
          device_handle(), application_handle(), CommunicationError());
    } else {
      LOGGER_DEBUG(logger_transport_, "not unexpected_disconnect");
      controller_->ConnectionFinished(device_handle(), application_handle());
    }
    socket_connection_.Close();
  }
  TransportAdapter::Error Notify() {
    LOGGER_AUTO_TRACE(logger_transport_);
    return socket_connection_.Notify() ? TransportAdapter::OK
                                       : TransportAdapter::FAIL;
  }

  bool Receive();
  void Send() {
    LOGGER_AUTO_TRACE(logger_transport_);
    LOGGER_DEBUG(logger_transport_, "Trying to send data if available");
    FrameQueue frames_to_send;
    {
      sync_primitives::AutoLock auto_lock(frames_to_send_mutex_);
      std::swap(frames_to_send, frames_to_send_);
    }

    size_t offset = 0;
    while (!frames_to_send.empty()) {
      ::protocol_handler::RawMessagePtr frame = frames_to_send.front();
      std::size_t bytes_sent = 0u;
      const bool sent = socket_connection_.Send(
          frame->data() + offset, frame->data_size() - offset, bytes_sent);
      if (!sent) {
        LOGGER_ERROR(logger_transport_, "Send failed for connection " << this);
        frames_to_send.pop();
        offset = 0;
        controller_->DataSendFailed(
            device_handle(), application_handle(), frame, DataSendError());
        Abort();
        return;
      }
      if (bytes_sent >= 0) {
        offset += bytes_sent;
        if (offset == frame->data_size()) {
          frames_to_send.pop();
          offset = 0;
          controller_->DataSendDone(
              device_handle(), application_handle(), frame);
        }
      }
    }
  }
  void Abort() {
    LOGGER_AUTO_TRACE(logger_transport_);
    unexpected_disconnect_ = true;
    terminate_flag_ = true;
  }

  TransportAdapterController* controller_;
  /**
 * @brief Frames that must be sent to remote device.
 **/
  typedef std::queue<protocol_handler::RawMessagePtr> FrameQueue;
  FrameQueue frames_to_send_;
  mutable sync_primitives::Lock frames_to_send_mutex_;

  SocketType socket_connection_;

  sync_primitives::atomic_bool terminate_flag_;
  sync_primitives::atomic_bool unexpected_disconnect_;

  const DeviceUID device_uid_;
  const ApplicationHandle app_handle_;
  threads::Thread* thread_;
};
}  // namespace transport_adapter
}  // namespace transport_manager

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_TRANSPORT_ADAPTER_THREADED_SOCKET_CONNECTION_H_
