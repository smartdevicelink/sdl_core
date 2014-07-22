/*
 * Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_PASA_BT_BLUETOOTH_PASA_CONNECTION_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_PASA_BT_BLUETOOTH_PASA_CONNECTION_H_

#include <poll.h>
#include <mqueue.h>

#include <queue>
#include <string>

#include "transport_manager/transport_adapter/connection.h"
#include "utils/logger.h"
#include "utils/lock.h"

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController;

/**
 * @brief Class responsible for communication over bluetooth sockets.
 */
class BluetoothPASAConnection
    : public ::transport_manager::transport_adapter::Connection {
 public:
  /**
   * @brief Constructor.
   *
   * @param device_uid Device unique identifier.
   * @param app_handle Handle of device.
   * @param controller Pointer to the device adapter controller.
   */
  BluetoothPASAConnection(const DeviceUID& device_uid,
                          const ApplicationHandle& app_handle,
                          TransportAdapterController* controller);
  /**
   * @brief Send data frame.
   *
   * @param Message Smart pointer to the raw message.
   *
   * @return Error Information about possible reason of sending data failure.
   */
  TransportAdapter::Error SendData(RawMessagePtr message);

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
   * @brief Destructor.
   */
  virtual ~BluetoothPASAConnection();

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

  /**
   * @brief Return pointer to the device adapter controller.
   */
  TransportAdapterController* controller() const;

  /**
   * @brief Return device unique identifier.
   */
  DeviceUID device_handle() const;

  /**
   * @brief Return handle of application.
   */
  ApplicationHandle application_handle() const;

 private:
  int read_fd_;
  int write_fd_;
  void Thread();
  void Transmit();
  void Finalize();
  TransportAdapter::Error Notify() const;
  bool Receive();
  bool Send();
  void Abort();

  friend void* StartBluetoothPASAConnection(void*);

  TransportAdapterController* controller_;
  /**
   * @brief Frames that must be sent to remote device.
   **/
  typedef std::queue<RawMessagePtr> FrameQueue;
  FrameQueue frames_to_send_;
  mutable sync_primitives::Lock frames_to_send_lock_;

  pthread_t thread_;

  bool terminate_flag_;
  bool unexpected_disconnect_;
  const DeviceUID device_uid_;
  const ApplicationHandle app_handle_;

  std::string sPPQ;
  int sppDeviceFd;
};
}  // namespace transport_adapter
}  // namespace transport_manager
#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_PASA_BT_BLUETOOTH_PASA_CONNECTION_H_
