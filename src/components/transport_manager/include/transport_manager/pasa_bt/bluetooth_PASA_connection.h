/**
 * \file bluetooth_socket_connection.h
 * \brief BluetoothPASAConnection class header file.
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

// Todd: BT Support
//#include <bluetooth/bluetooth.h>
//#include <bluetooth/hci.h>
//#include <bluetooth/hci_lib.h>
//#include <bluetooth/sdp.h>
//#include <bluetooth/sdp_lib.h>
//#include <bluetooth/rfcomm.h>

#include <poll.h>

#include <queue>
#include <mqueue.h>

#include "utils/logger.h"
#include "transport_manager/transport_adapter/connection.h"

#define MAX_QUEUE_NAME_SIZE     24
#define MAX_QUEUE_MSG_SIZE 		4095
#define MSGQ_MAX_MESSAGES		128
#define MSGQ_MESSAGE_SIZE 		MAX_QUEUE_MSG_SIZE

#define APPLINK_PACKET_MINSIZE	8
#define MAX_SPP_PACKET_SIZE		2047

#define GET_DATASIZE(X) ((X[4]<<24) | (X[5]<<16) | (X[6]<<8) | X[7])

typedef struct _SPPframe {
	uint16_t length;
	uint8_t  data[MAX_SPP_PACKET_SIZE];
}SPPframe;


using ::transport_manager::transport_adapter::Connection;

namespace transport_manager {
namespace transport_adapter {

class TransportAdapterController;

/**
 * @brief Class responsible for communication over bluetooth sockets.
 */
class BluetoothPASAConnection : public Connection {
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
  TransportAdapter::Error SendData(RawMessageSptr message);

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

  int read_fd_;
  int write_fd_;
  void Thread();
  void Transmit();
  void Finalize();
  TransportAdapter::Error Notify() const;
  bool Receive();
  bool Send();
  void Abort();
  void printBufferInHex(uint8_t *buf, int len);
  mqd_t openMsgQ(const char *queue_name, bool sender, bool block);

  friend void* StartBluetoothPASAConnection(void*);

  TransportAdapterController* controller_;
  /**
   * @brief Frames that must be sent to remote device.
   **/
  typedef std::queue<RawMessageSptr> FrameQueue;
  FrameQueue frames_to_send_;
  mutable pthread_mutex_t frames_to_send_mutex_;

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

#endif /* BLUETOOTH_SOCKET_CONNECTION_H_ */
