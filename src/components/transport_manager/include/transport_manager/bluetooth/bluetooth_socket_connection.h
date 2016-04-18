/*
 * \file bluetooth_socket_connection.h
 * \brief BluetoothSocketConnection class header file.
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_SOCKET_CONNECTION_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_SOCKET_CONNECTION_H_

#include <queue>

#ifdef OS_WINDOWS
#include "transport_manager/transport_adapter/connection.h"
#else
#include "transport_manager/transport_adapter/threaded_socket_connection.h"
#endif
#include "protocol/common.h"
#include "utils/threads/thread_delegate.h"
#include "utils/lock.h"
#include "utils/socket.h"
#include "utils/atomic_object.h"
#include "utils/threads/thread_delegate.h"
#ifdef OS_WINDOWS
#include <ws2bth.h>
#include <BluetoothAPIs.h>
#endif

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController;

/**
 * @brief Class responsible for communication over bluetooth sockets.
 */
#ifdef OS_WINDOWS
class BluetoothSocketConnection : public Connection {
#else
class BluetoothSocketConnection : public ThreadedSocketConnection<utils::BluetoothSocketConnection> {
#endif
 public:
  /**
   * @brief Constructor.
   *
   * @param device_uid Device unique identifier.
   * @param app_handle Handle of device.
   * @param controller Pointer to the device adapter controller.
   */
  BluetoothSocketConnection(const DeviceUID& device_uid,
                            const ApplicationHandle& app_handle,
                            TransportAdapterController* controller);

#ifdef OS_WINDOWS
  TransportAdapter::Error Start();
#endif
  /**
   * @brief Destructor.
   */
  virtual ~BluetoothSocketConnection();

 protected:
  /**
   * @brief Establish connection.
   *
   * @param error Connection error.
   *
   * @return True - connection established.
   * false - connection not established.
   */
  virtual bool Establish(ConnectError** error);

#ifdef OS_WINDOWS
  /**
  * @brief Send data frame.
  *
  * @param message Smart pointer to the raw message.
  *
  * @return Error Information about possible reason of sending data failure.
  */
  TransportAdapter::Error SendData(
      ::protocol_handler::RawMessagePtr message) OVERRIDE;

  /**
  * @brief Disconnect the current connection.
  *
  * @return Error Information about possible reason of Disconnect failure.
  */
  virtual TransportAdapter::Error Disconnect();

  /**
  * @brief Return pointer to the device adapter controller.
  */
  TransportAdapterController* controller();

  /**
  * @brief Return device unique identifier.
  */
  DeviceUID device_handle() const;

  /**
  * @brief Return handle of application.
  */
  ApplicationHandle application_handle() const;

 private:
  class BthConnectionDelegate : public threads::ThreadDelegate {
   public:
    explicit BthConnectionDelegate(BluetoothSocketConnection* connection);
    void threadMain() OVERRIDE;
    void exitThreadMain() OVERRIDE;

   private:
    BluetoothSocketConnection* connection_;
  };

  void OnError(int error);
  void OnData(const uint8_t* const buffer, std::size_t buffer_size);
  void OnCanWrite();
  void OnClose();

  void threadMain();
  void Transmit();
  void Finalize();
  TransportAdapter::Error Notify();
  bool Receive();
  void Send();
  void Abort();

  bool Close();
  bool IsValid() const;
  bool Send(const char* const buffer,
            const std::size_t size,
            std::size_t& bytes_written);
  void OnRead();
  void OnWrite();

  TransportAdapterController* controller_;

  /**
  * @brief Frames that must be sent to remote device.
  **/
  typedef std::queue<protocol_handler::RawMessagePtr> FrameQueue;
  FrameQueue frames_to_send_;
  mutable sync_primitives::Lock frames_to_send_mutex_;

  sync_primitives::atomic_bool terminate_flag_;
  sync_primitives::atomic_bool unexpected_disconnect_;

  const DeviceUID device_uid_;
  const ApplicationHandle app_handle_;

  HANDLE notify_event_;
  SOCKET rfcomm_socket_;

  threads::Thread* thread_;
#endif
};

}  // namespace transport_adapter
}  // namespace transport_manager

#endif /* BLUETOOTH_SOCKET_CONNECTION_H_ */
