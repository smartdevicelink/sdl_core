/**
 * \file device_adapter_socket_communication.h
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_SOCKET_COMMUNICATION
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_SOCKET_COMMUNICATION

#include <poll.h>

#include <queue>

#include "utils/logger.h"
#include "transport_manager/device_adapter/connection.h"

using ::transport_manager::device_adapter::Connection;

namespace transport_manager {
namespace device_adapter {

class DeviceAdapterController;

class ThreadedSocketConnection : public Connection {
 public:
  DeviceAdapter::Error sendData(RawMessageSptr message);
  DeviceAdapter::Error disconnect();

  DeviceAdapter::Error start();

  void set_socket(int socket) {
    socket_ = socket;
  }
 protected:
  ThreadedSocketConnection(const DeviceUID& device_uid,
                           const ApplicationHandle& app_handle,
                           DeviceAdapterController* controller);
  virtual ~ThreadedSocketConnection();

  virtual bool establish(ConnectError** error) = 0;

  DeviceAdapterController* getController() {
    return controller_;
  }

  DeviceUID device_handle() const {
    return device_uid_;
  }

  ApplicationHandle application_handle() const {
    return app_handle_;
  }

 private:
  int read_fd_;
  int write_fd_;
  void thread();
  void transmit();
  void finalize();
  DeviceAdapter::Error notify() const;
  bool receive();
  bool send();
  void abort();

  friend void* startThreadedSocketConnection(void*);

  DeviceAdapterController* controller_;
  /**
   * @brief Frames that must be sent to remote device.
   **/
  typedef std::queue<RawMessageSptr> FrameQueue;
  FrameQueue frames_to_send_;
  mutable pthread_mutex_t frames_to_send_mutex_;

  pthread_t thread_;

  int socket_;
  bool terminate_flag_;
  bool unexpected_disconnect_;
  const DeviceUID device_uid_;
  const ApplicationHandle app_handle_;
};

extern log4cxx::LoggerPtr logger_;

}  // namespace device_adapter
}  // namespace transport_manager

#endif //SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_DEVICE_ADAPTER_SOCKET_COMMUNICATION
